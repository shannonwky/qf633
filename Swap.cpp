#include "Swap.h"
#include <cmath>

double Swap::Payoff(double marketRate) const { return getAnnuity() * (tradeRate - marketRate); };

double Swap::getAnnuity() const
{
    double time = maturityDate - marketDate;
    double discoutRate = getRateFromCurve(time);
    double df = getDiscountFactor(discoutRate, time);
    return (1 - df) * notional;
}

// simple assumption, it should get from task1
double Swap::getRateFromCurve(double time) const
{
    if (time <= 1.0)
    {
        return 0.03;
    }
    else
    {
        return 0.04;
    }
}

double Swap::getDiscountFactor(double rate, double time) const
{
    return std::exp(-rate * time);
}