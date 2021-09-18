#ifndef INCLUDE_XS4GCR_CGS_H
#define INCLUDE_XS4GCR_CGS_H

#define pow2(A) ((A) * (A))
#define pow3(A) ((A) * (A) * (A))
#define pow4(A) ((A) * (A) * (A) * (A))

// https://en.wikipedia.org/wiki/Gaussian_units
namespace XS4GCR {
namespace cgs {

// basic units
static constexpr double centimeter = 1;   // LENGTH
static constexpr double gram = 1;         // MASS
static constexpr double second = 1;       // TIME
static constexpr double kelvin = 1;       // TEMPERATURE
static constexpr double steradian = 1;    // SOLID ANGLE
static constexpr double erg = 1;          // ENERGY
static constexpr double kayser = 1;       // WAVENUMBER
static constexpr double hertz = 1;        // FREQUENCY
static constexpr double statcoulomb = 1;  // ELECTRIC CHARGE
static constexpr double gauss = 1;        // MAGNETIC FLUX DENSITY

// prefixes
static constexpr double peta = 1e15;
static constexpr double tera = 1e12;
static constexpr double giga = 1e9;
static constexpr double mega = 1e6;
static constexpr double kilo = 1e3;
static constexpr double milli = 1e-3;
static constexpr double micro = 1e-6;
static constexpr double nano = 1e-9;
static constexpr double pico = 1e-12;
static constexpr double femto = 1e-15;

// derived units
static constexpr double parsec = 3.0856775807e18 * centimeter;
static constexpr double electronvolt = 1.60217657e-12 * erg;
static constexpr double year = 3.15569e7 * second;
static constexpr double meter = 1e2 * centimeter;
static constexpr double femtometre = femto * meter;
static constexpr double kilometer = kilo * meter;
static constexpr double kiloyear = kilo * year;
static constexpr double megayear = mega * year;
// static constexpr double kiloparsec = kilo * parsec;
// static constexpr double megaparsec = mega * parsec;
// static constexpr double microgauss = micro * gauss;
static constexpr double megaelectronvolt = mega * electronvolt;
static constexpr double gigaelectronvolt = giga * electronvolt;
static constexpr double teraelectronvolt = tera * electronvolt;
static constexpr double petaelectronvolt = peta * electronvolt;
static constexpr double barn = 1e-24 * pow2(centimeter);
static constexpr double mbarn = 1e-27 * pow2(centimeter);

// physical constexprants
static constexpr double c = 2.99792458e10 * centimeter / second;
static constexpr double electronMass = 9.10938370e-28 * gram;
static constexpr double protonMass = 1.6726219e-24 * gram;
static constexpr double kB = 1.380649e-16 * erg / kelvin;
static constexpr double elementaryCharge = 4.80320427e-10 * statcoulomb;
static constexpr double sigmaTh = 6.6524e-25 * pow2(centimeter);

// derived quantities
static constexpr double volt = electronvolt / c;
static constexpr double gigavolt = giga * volt;
static constexpr double electronMassC2 = electronMass * pow2(c);
static constexpr double protonMassC2 = protonMass * pow2(c);

// abbreviations
static constexpr double fm = femtometre;
static constexpr double cm = centimeter;
static constexpr double cm2 = pow2(centimeter);
static constexpr double cm3 = pow3(centimeter);
// static constexpr double km = kilometer;
// static constexpr double pc = parsec;
// static constexpr double kpc = kiloparsec;
static constexpr double kyr = kiloyear;
static constexpr double Myr = megayear;
// static constexpr double muG = microgauss;
static constexpr double eV = electronvolt;
static constexpr double MeV = megaelectronvolt;
static constexpr double GeV = gigaelectronvolt;
static constexpr double TeV = teraelectronvolt;
static constexpr double PeV = petaelectronvolt;
static constexpr double GV = gigavolt;
static constexpr double K = kelvin;

}  // namespace cgs
}  // namespace XS4GCR

#endif