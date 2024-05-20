#ifndef BLACK_MODEL_PRICER_H
#define BLACK_MODEL_PRICER_H

#include "Types.h"
#include <cmath>
#include <stdexcept>

class BlackModelPricer {
public:
    double Price(double S, double r, double vol, double T, double strike, OptionType payoffType) const;

private:
    double cnorm(double x) const;
};

#endif // BLACK_MODEL_PRICER_H
