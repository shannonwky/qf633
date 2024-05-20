#ifndef PAYOFF_H
#define PAYOFF_H
#include "Types.h"
#include "Pricer.h"
#include <cmath>

namespace PAYOFF
{
  double VanillaOption(OptionType optType, double strike, double S);

  double CallSpread(double strike1, double strike2, double S);
}
#endif
