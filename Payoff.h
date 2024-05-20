#ifndef PAYOFF_H
#define PAYOFF_H
#include "Types.h"
#include "Pricer.h"
#include <cmath>

namespace PAYOFF
{
  double VanillaOption(OptionType optType, double strike, double S)
  {
    switch (optType)
      {
      case Call:
      return max(S - strike, 0.0);
      case Put:
      return max(strike - S, 0.0);
      case BinaryCall:
      return (S >= strike) ? 1.0 : 0.0;
      case BinaryPut:
      return (S <= strike) ? 1.0 : 0.0;
      default:
	throw "unsupported optionType";
      }
  }

  double CallSpread(double strike1, double strike2, double S)
  {
    if (S < strike1)
      return 0.0;
    else if (S > strike2)
      return 1.0;
    else
      return (S - strike1) / (strike2 - strike1);
  }
}
#endif




