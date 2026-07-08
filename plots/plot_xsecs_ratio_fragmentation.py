from __future__ import print_function

import argparse
import csv
import os
import re

import matplotlib.pyplot as plt
import numpy as np


PLOT_DIR = os.path.dirname(os.path.abspath(__file__))
ROOT_DIR = os.path.dirname(PLOT_DIR)
DEFAULT_OUTPUT_DIR = os.path.join(PLOT_DIR, 'figs')
DEFAULT_TABLE_GLOB = os.path.join(ROOT_DIR, 'build', 'output', 'crams_fragmentation_*.csv')

plt.style.use(os.path.join(PLOT_DIR, 'xs4gcr.mplstyle'))

Z_OF = {
    'H': 1, 'He': 2, 'Li': 3, 'Be': 4, 'B': 5, 'C': 6, 'N': 7, 'O': 8,
    'F': 9, 'Ne': 10, 'Na': 11, 'Mg': 12, 'Al': 13, 'Si': 14, 'P': 15,
    'S': 16, 'Cl': 17, 'Ar': 18, 'K': 19, 'Ca': 20, 'Sc': 21, 'Ti': 22,
    'V': 23, 'Cr': 24, 'Mn': 25, 'Fe': 26, 'Co': 27, 'Ni': 28, 'Cu': 29,
    'Zn': 30,
}
SYMBOL_OF = dict((z, symbol) for symbol, z in Z_OF.items())

try:
    STRING_TYPES = (basestring,)
except NameError:
    STRING_TYPES = (str,)


class FragmentationCurve:
    def __init__(self, model, filename, energy, sigma):
        self.model = model
        self.filename = filename
        self.energy = energy
        self.sigma = sigma


class RatioCurve:
    def __init__(self, model, filename, energy, ratio):
        self.model = model
        self.filename = filename
        self.energy = energy
        self.ratio = ratio


def fullmatch(pattern, value):
    return re.match(pattern + r'\Z', value)


def savefig(fig, filename):
    directory = os.path.dirname(os.path.abspath(filename))
    if directory and not os.path.isdir(directory):
        os.makedirs(directory)
    print('- saving plot on ' + filename)
    fig.savefig(filename)


def parse_isotope(value):
    value = value.strip()

    numeric = fullmatch(r'(\d+)\s*[,/:]\s*(\d+)', value)
    if numeric:
        return int(numeric.group(1)), int(numeric.group(2))

    symbolic = fullmatch(r'([A-Za-z]{1,2})\s*[-_]?\s*(\d+)', value)
    if symbolic:
        symbol = symbolic.group(1).capitalize()
        if symbol not in Z_OF:
            raise ValueError('unknown element symbol: ' + symbolic.group(1))
        return Z_OF[symbol], int(symbolic.group(2))

    reversed_symbolic = fullmatch(r'(\d+)\s*[-_]?\s*([A-Za-z]{1,2})', value)
    if reversed_symbolic:
        symbol = reversed_symbolic.group(2).capitalize()
        if symbol not in Z_OF:
            raise ValueError('unknown element symbol: ' + reversed_symbolic.group(2))
        return Z_OF[symbol], int(reversed_symbolic.group(1))

    raise ValueError('cannot parse isotope: ' + value)


def parse_ratio_channel(value):
    """Parse a ratio spec like 'C12->Be10/Be9'.

    Returns (projectile, numerator_fragment, denominator_fragment).
    """
    parts = re.split(r'\s*(?:->|-->|to)\s*', value.strip(), maxsplit=1)
    if len(parts) != 2:
        raise ValueError('ratio must look like C12->Be10/Be9')
    projectile = parse_isotope(parts[0])

    fragments = re.split(r'\s*/\s*', parts[1].strip(), maxsplit=1)
    if len(fragments) != 2:
        raise ValueError('ratio must look like C12->Be10/Be9 (two fragments separated by /)')
    numerator = parse_isotope(fragments[0])
    denominator = parse_isotope(fragments[1])
    return projectile, numerator, denominator


def isotope_label(isotope):
    z, a = isotope
    return SYMBOL_OF.get(z, 'Z' + str(z)) + str(a)


def isotope_tex(isotope):
    z, a = isotope
    symbol = SYMBOL_OF.get(z, 'Z' + str(z))
    return r'{}^{%d}{\rm %s}' % (a, symbol)


def table_model_name(filename):
    with open(filename) as f:
        for line in f:
            if not line.startswith('#'):
                break
            if line.startswith('# model:'):
                return line.split(':', 1)[1].strip()

    stem = os.path.splitext(os.path.basename(filename))[0]
    return stem.replace('crams_fragmentation_', '').replace('_', ' ')


def csv_rows_without_comments(filename):
    with open(filename, 'r') as f:
        for line in f:
            if not line.startswith('#'):
                yield line


def load_channel(filename, projectile, fragment):
    model = table_model_name(filename)
    reader = csv.reader(csv_rows_without_comments(filename))

    try:
        header = next(reader)
    except StopIteration:
        raise ValueError('empty CSV table: ' + str(filename))

    if header[:4] != ['Z_proj', 'A_proj', 'Z_frag', 'A_frag']:
        raise ValueError('unexpected fragmentation CSV header in ' + str(filename))

    energy = np.array([float(value) for value in header[4:]])
    wanted = (projectile[0], projectile[1], fragment[0], fragment[1])

    for row in reader:
        if len(row) < 5:
            continue
        channel = tuple(int(value) for value in row[:4])
        if channel == wanted:
            sigma = np.array([float(value) for value in row[4:]])
            return FragmentationCurve(model, filename, energy, sigma)

    raise KeyError(model + ' has no ' + isotope_label(projectile) + ' -> ' + isotope_label(fragment) + ' channel')


def load_ratio(filename, projectile, numerator, denominator):
    num = load_channel(filename, projectile, numerator)
    den = load_channel(filename, projectile, denominator)

    if not np.array_equal(num.energy, den.energy):
        den_sigma = np.interp(num.energy, den.energy, den.sigma, left=np.nan, right=np.nan)
    else:
        den_sigma = den.sigma

    with np.errstate(divide='ignore', invalid='ignore'):
        ratio = np.where(den_sigma > 0.0, num.sigma / den_sigma, np.nan)

    return RatioCurve(num.model, filename, num.energy, ratio)


def default_model_files():
    names = [
        'crams_fragmentation_evoli2019.csv',
        'crams_fragmentation_evoli2026_w93.csv',
        'crams_fragmentation_evoli2026_st99.csv',
        'crams_fragmentation_usine_galprop17_opt12.csv',
        'crams_fragmentation_usine_galprop17_opt22.csv',
        'crams_fragmentation_usine_webber03+coste12.csv',
    ]
    output_dir = os.path.join(ROOT_DIR, 'build', 'output')
    return [os.path.join(output_dir, name) for name in names]


def output_filename(projectile, numerator, denominator, output_dir=DEFAULT_OUTPUT_DIR):
    name = 'xsecs_ratio_fragmentation_{}_{}_over_{}.pdf'.format(
        isotope_label(projectile), isotope_label(numerator), isotope_label(denominator))
    return os.path.join(output_dir, name)


def plot_ratio_fragmentation_channel(channel, model_files=None, output=None, xlim=None, ylim=None, no_tex=False):
    if isinstance(channel, STRING_TYPES):
        projectile, numerator, denominator = parse_ratio_channel(channel)
    else:
        projectile, numerator, denominator = channel

    if no_tex:
        plt.rcParams['text.usetex'] = False

    if model_files is None or len(model_files) == 0:
        model_files = default_model_files()

    curves = []
    for filename in model_files:
        try:
            curves.append(load_ratio(filename, projectile, numerator, denominator))
        except KeyError as error:
            print('- skipping ' + str(error.args[0]))

    if not curves:
        raise ValueError('no selected CSV table contains ' + isotope_label(projectile) + ' -> ' +
                         isotope_label(numerator) + ' and ' + isotope_label(denominator))

    reference_model = 'Evoli2026_W93'
    reference = next((curve for curve in curves if curve.model == reference_model), None)

    fig = plt.figure(figsize=(10.5, 10.5))
    fig.set_layout_engine('constrained', hspace=0.0)
    gs = fig.add_gridspec(2, 1, height_ratios=[3, 2], hspace=0.1)
    ax = fig.add_subplot(gs[0])
    ax_res = fig.add_subplot(gs[1], sharex=ax)

    dotted_models = {'Evoli2026_ST99', 'USINE_GALPROP17_OPT22'}
    color_models = {
        'Evoli2019': 'tab:gray',
        'Evoli2026_ST99': 'tab:red',
        'Evoli2026_W93': 'tab:green',
        'USINE_GALPROP17_OPT12': 'tab:blue',
        'USINE_GALPROP17_OPT22': 'tab:orange',
        'USINE_WEBBER03_COSTE12': 'tab:olive',
    }

    if xlim is not None:
        xmin, xmax = xlim
    else:
        xmin, xmax = curves[0].energy.min(), curves[0].energy.max()

    plotted = False
    for curve in curves:
        positive = np.isfinite(curve.ratio) & (curve.ratio > 0.0)
        if not np.any(positive):
            print('- skipping ' + curve.model + ': ratio is undefined')
            continue
        linestyle = ':' if curve.model in dotted_models else '-'
        color = color_models.get(curve.model)
        ax.plot(curve.energy[positive], curve.ratio[positive], label=curve.model,
                linestyle=linestyle, color=color)
        plotted = True

        if reference is not None:
            ref_ratio = np.interp(curve.energy, reference.energy, reference.ratio,
                                  left=np.nan, right=np.nan)
            valid = positive & np.isfinite(ref_ratio) & (ref_ratio > 0.0)
            if np.any(valid):
                relative = curve.ratio[valid] / ref_ratio[valid] - 1.0
                ax_res.plot(curve.energy[valid], relative, linestyle=linestyle, color=color)

    if not plotted:
        raise ValueError('selected CSV tables only contain undefined ratios for ' +
                         isotope_label(projectile) + ' -> ' + isotope_label(numerator) + '/' +
                         isotope_label(denominator))

    ax.set_xscale('log')
    ax.set_ylabel(r'$\sigma({}) / \sigma({})$'.format(isotope_tex(numerator), isotope_tex(denominator)))
    ax.set_title(r'${} \rightarrow {} / {}$ on H'.format(
        isotope_tex(projectile), isotope_tex(numerator), isotope_tex(denominator)))
    plt.setp(ax.get_xticklabels(), visible=False)

    ax_res.axhline(0.0, color='k', linestyle='-', linewidth=0.8)
    ax_res.set_xscale('log')
    ax_res.set_xlabel(r'$T_{\rm kin}/n$ [GeV/n]')
    ax_res.set_ylabel(r'ratio$ / $ratio$_{\rm W93} - 1$')

    ax_res.set_ylim(-0.25, 0.25)
    ax_res.fill_between([xmin, xmax], -0.1, 0.1, color='tab:cyan', alpha=0.15, zorder=0)

    ax.set_xlim([xmin, xmax])

    if ylim is not None:
        ax.set_ylim(ylim)

    ax.legend(fontsize=13)

    if output is None:
        output = output_filename(projectile, numerator, denominator)
    savefig(fig, output)
    return fig, (ax, ax_res)


def parse_args():
    parser = argparse.ArgumentParser(
        description='Plot ratios of CRAMS secondary production cross sections for one projectile.')
    parser.add_argument('channel', nargs='?', default='C12->Be10/Be9',
                        help='ratio channel, e.g. C12->Be10/Be9')
    parser.add_argument('model_files', nargs='*',
                        help='model CSV files; defaults to build/output/crams_fragmentation_*.csv')
    parser.add_argument('-o', '--output',
                        help='output PDF path; defaults to plots/figs/xsecs_ratio_fragmentation_<channel>.pdf')
    parser.add_argument('--xlim', nargs=2, type=float, metavar=('XMIN', 'XMAX'),
                        help='x-axis limits in GeV/n')
    parser.add_argument('--ylim', nargs=2, type=float, metavar=('YMIN', 'YMAX'),
                        help='y-axis limits for the ratio')
    parser.add_argument('--no-tex', action='store_true',
                        help='disable LaTeX text rendering for machines without a TeX install')
    return parser.parse_args()


if __name__ == '__main__':
    args = parse_args()
    files = args.model_files
    plot_ratio_fragmentation_channel(args.channel, files, args.output, args.xlim, args.ylim, args.no_tex)
