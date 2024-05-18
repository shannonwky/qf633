#include <tuple>
#include <cmath>
#include "Pricer.h"

std::tuple<double, double, double> crrCalib(double r, double vol, double t) {
    double b = std::exp(vol * vol * t + r * t) + std::exp(-r * t);
    double u = (b + std::sqrt(b * b - 4)) / 2;
    double p = (std::exp(r * t) - (1 / u)) / (u - 1 / u);
    return std::make_tuple(u, 1/u, p);
}

std::tuple<double, double, double> jrrnCalib(double r, double vol, double t) {
    double u = std::exp((r - vol * vol / 2) * t + vol * std::sqrt(t));
    double d = std::exp((r - vol * vol / 2) * t - vol * std::sqrt(t));
    double p = (std::exp(r * t) - d) / (u - d);
    return std::make_tuple(u, d, p);
}

std::tuple<double, double, double> jreqCalib(double r, double vol, double t) {
    double u = std::exp((r - vol * vol / 2) * t + vol * std::sqrt(t));
    double d = std::exp((r - vol * vol / 2) * t - vol * std::sqrt(t));
    return std::make_tuple(u, d, 0.5);
}

std::tuple<double, double, double> tianCalib(double r, double vol, double t) {
    double v = std::exp(vol * vol * t);
    double u = 0.5 * std::exp(r * t) * v * (v + 1 + std::sqrt(v*v + 2*v - 3));
    double d = 0.5 * std::exp(r * t) * v * (v + 1 - std::sqrt(v*v + 2*v - 3));
    double p = (std::exp(r * t) - d) / (u - d);
    return std::make_tuple(u, d, p);
}

// Black model pricer
double blackPrice(double S, double r, double vol, double T, double strike, OptionType payoffType)
{
    double F = S * exp(r * T); // Forward price
    double stdev = vol * sqrt(T);
    double d1 = (log(F / strike) + 0.5 * stdev * stdev) / stdev;
    double d2 = d1 - stdev;
    if (payoffType == OptionType::Call)
    {
        return exp(-r * T) * (F * cnorm(d1) - strike * cnorm(d2));
    }
    else if (payoffType == OptionType::Put)
    {
        return exp(-r * T) * (strike * cnorm(-d2) - F * cnorm(-d1));
    }
    else if (payoffType == OptionType::BinaryCall)
    {
        return exp(-r * T) * cnorm(d2);
    }
    else if (payoffType == OptionType::BinaryPut)
    {
        return exp(-r * T) * cnorm(-d2);
    }
    else
    {
        throw std::invalid_argument("Unsupported payoffType");
    }
}

