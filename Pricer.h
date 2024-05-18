#pragma _PRICER

#include <tuple>
#include <vector>
#include <cmath>

#include "EuropeanTrade.h"
#include "AmericanTrade.h"

std::tuple<double, double, double> crrCalib(double r, double vol, double t);
std::tuple<double, double, double> jrrnCalib(double r, double vol, double t);
std::tuple<double, double, double> jreqCalib(double r, double vol, double t);
std::tuple<double, double, double> tianCalib(double r, double vol, double t);

template <typename OptionType>
double binomialPricer(double S, double r, double vol, const OptionType &trade, int n, std::tuple<double, double, double> (*calib)(double, double, double))
{
    double t = trade.getExpiry() / n;
    std::tuple<double, double, double> calibResult = calib(r, vol, t);
    double u = std::get<0>(calibResult);
    double d = std::get<1>(calibResult);
    double p = std::get<2>(calibResult);

    // Set up the last time slice, there are n+1 nodes at the last time slice
    std::vector<double> vs(n + 1);
    for (int i = 0; i <= n; ++i)
    {
        vs[i] = trade.payoff(S * std::pow(u, n - i) * std::pow(d, i));
    }

    // Iterate backward
    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = 0; j <= i; ++j)
        {
            double nodeS = S * std::pow(u, i - j) * std::pow(d, j);
            double continuation = std::exp(-r * t) * (vs[j] * p + vs[j + 1] * (1 - p));
            vs[j] = trade.valueAtNode(t * i, nodeS, continuation);
        }
    }

    return vs[0];
}

// Cumulative normal distribution function
inline double cnorm(double x) { return (1.0 + erf(x / sqrt(2.0))) / 2.0; }

double blackPrice(double S, double r, double vol, double T, double strike, OptionType payoffType);