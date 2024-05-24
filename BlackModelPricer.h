#ifndef BLACK_MODEL_PRICER_H
#define BLACK_MODEL_PRICER_H

#include "Types.h"
#include "TreeProduct.h"
#include <cmath>
#include <stdexcept>

class BlackModelPricer 
{
public:
    //BlackModelPricer(const Date &expiry, double strike, OptionType optType) : expiry(expiry), strike(strike), optType(optType) {}

    double Price(double S, double r, double vol, double T, double strike, OptionType payoffType) const;

    //virtual const Date &GetExpiry() const override { return expiry; }
    //virtual double getStrike() const { return strike; }
    //virtual OptionType getPayoffType() const { return optType; }

private:
    double cnorm(double x) const;
    OptionType optType;
    double strike;
    Date expiry;

};

#endif // BLACK_MODEL_PRICER_H
