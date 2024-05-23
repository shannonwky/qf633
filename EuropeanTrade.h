#ifndef _EUROPEAN_TRADE
#define _EUROPEAN_TRADE

#include <cassert>
#include "TreeProduct.h"
#include "Payoff.h"
#include "Types.h"

class EuropeanOption : public TreeProduct
{
public:
  EuropeanOption(const Date &expiry, double strike, OptionType optType) : expiry(expiry), strike(strike), optType(optType) {}

  virtual double Payoff(double S) const override
  {
    return PAYOFF::VanillaOption(optType, strike, S);
  }

  virtual double ValueAtNode(double S, double t, double continuation) const override
  {
    return continuation;
  }

  virtual const Date &GetExpiry() const override { return expiry; }
  virtual double getStrike() const { return strike; }
  virtual OptionType getPayoffType() const { return optType; }
  void Print() const override;

private:
  OptionType optType;
  double strike;
  Date expiry;
};

class EuroCallSpread : public TreeProduct
{
public:
  EuroCallSpread(double k1, double k2, const Date &expiry) : strike1(k1), strike2(k2), expiry(expiry)
  {
    assert(k1 < k2);
  };
  virtual double Payoff(double S) const { return PAYOFF::CallSpread(strike1, strike2, S); };
  virtual const Date &GetExpiry() const { return expiry; };

private:
  double strike1;
  double strike2;
  Date expiry;
};

#endif