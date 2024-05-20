#include "BlackModelPricer.h"

double BlackModelPricer::cnorm(double x) const {
    return (1.0 + erf(x / sqrt(2.0))) / 2.0;
}

double BlackModelPricer::Price(double S, double r, double vol, double T, double strike, OptionType payoffType) const {
    double F = S * exp(r * T);  // Forward price
    double stdev = vol * sqrt(T);
    double d1 = (log(F / strike) + 0.5 * stdev * stdev) / stdev;
    double d2 = d1 - stdev;
    
    switch (payoffType) {
    case Call:
        return exp(-r * T) * (F * cnorm(d1) - strike * cnorm(d2));
    case Put:
        return exp(-r * T) * (strike * cnorm(-d2) - F * cnorm(-d1));
    case BinaryCall:
        return exp(-r * T) * cnorm(d2);
    case BinaryPut:
        return exp(-r * T) * cnorm(-d2);
    default:
        throw std::invalid_argument("Unsupported payoffType");
    }
}
