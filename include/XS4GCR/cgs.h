#ifndef INCLUDE_XS4GCR_CGS_H
#define INCLUDE_XS4GCR_CGS_H

#define pow2(A) ((A) * (A))
#define pow3(A) ((A) * (A) * (A))
#define pow4(A) ((A) * (A) * (A) * (A))

// https://en.wikipedia.org/wiki/Gaussian_units
namespace XS4GCR {
namespace cgs {

// basic units
static const double centimeter = 1;   // LENGTH
static const double gram = 1;         // MASS
static const double second = 1;       // TIME
static const double kelvin = 1;       // TEMPERATURE
static const double steradian = 1;    // SOLID ANGLE
static const double erg = 1;          // ENERGY
static const double kayser = 1;       // WAVENUMBER
static const double hertz = 1;        // FREQUENCY
static const double statcoulomb = 1;  // ELECTRIC CHARGE
static const double gauss = 1;        // MAGNETIC FLUX DENSITY

// prefixes
static const double peta = 1e15;
static const double tera = 1e12;
static const double giga = 1e9;
static const double mega = 1e6;
static const double kilo = 1e3;
static const double milli = 1e-3;
static const double micro = 1e-6;
static const double nano = 1e-9;
static const double pico = 1e-12;

// derived units
static const double parsec = 3.0856775807e18 * centimeter;
static const double electronvolt = 1.60217657e-12 * erg;
static const double year = 3.15569e7 * second;
// static const double meter = 1e2 * centimeter;
// static const double kilometer = kilo * meter;
// static const double kiloyear = kilo * year;
// static const double kiloparsec = kilo * parsec;
// static const double megaparsec = mega * parsec;
// static const double microgauss = micro * gauss;
static const double megaelectronvolt = mega * electronvolt;
static const double gigaelectronvolt = giga * electronvolt;
static const double teraelectronvolt = tera * electronvolt;
static const double petaelectronvolt = peta * electronvolt;
static const double barn = 1e-24 * pow2(centimeter);
static const double mbarn = 1e-27 * pow2(centimeter);

// physical constants
static const double c = 2.99792458e10 * centimeter / second;
static const double electronMass = 9.10938370e-28 * gram;
static const double protonMass = 1.6726219e-24 * gram;
static const double kB = 1.380649e-16 * erg / kelvin;
static const double elementaryCharge = 4.80320427e-10 * statcoulomb;
static const double sigmaTh = 6.6524e-25 * pow2(centimeter);

// derived quantities
static const double volt = electronvolt / c;
static const double gigavolt = giga * volt;
static const double electronMassC2 = electronMass * pow2(c);
static const double protonMassC2 = protonMass * pow2(c);

// abbreviations
static const double cm = centimeter;
static const double cm2 = pow2(centimeter);
static const double cm3 = pow3(centimeter);
// static const double km = kilometer;
// static const double pc = parsec;
// static const double kpc = kiloparsec;
// static const double kyr = kiloyear;
// static const double muG = microgauss;
static const double eV = electronvolt;
static const double MeV = megaelectronvolt;
static const double GeV = gigaelectronvolt;
static const double TeV = teraelectronvolt;
static const double PeV = petaelectronvolt;
static const double GV = gigavolt;
static const double K = kelvin;

}  // namespace cgs
}  // namespace XS4GCR

#endif