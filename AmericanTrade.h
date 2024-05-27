#ifndef _AMERICAN_TRADE
#define _AMERICAN_TRADE

#include <cassert>
#include <cmath>

#include "TreeProduct.h"
#include "Payoff.h"
#include "Types.h"

class AmericanOption : public TreeProduct
{
public:
  AmericanOption(int id, const Date &expiry, double strike, OptionType optType) : id(id), expiry(expiry), strike(strike), optType(optType) {}

  virtual double Payoff(double S) const override
  {
    return PAYOFF::VanillaOption(optType, strike, S);
  }

  virtual double ValueAtNode(double S, double t, double continuation) const override
  {
    return max(Payoff(S), continuation);
  }

  virtual double getRate(const RateCurve &curve, const Market& mkt) const {
      double days_left = (expiry - mkt.asOf) * 365.25;
      std::string tenor = std::to_string((expiry - mkt.asOf)) + "Y";
      double rate =  curve.getRate(tenor) * 0.01;
      return rate;
  }

  inline int GetTradeID() const { return id; }
  virtual const Date &GetExpiry() const override { return expiry; }
  virtual double getStrike() const { return strike; }
  virtual OptionType getPayoffType() const { return optType; }
  void Print() const override;

private:
  OptionType optType;
  double strike;
  Date expiry;
  int id;
};

class AmerCallSpread : public TreeProduct
{
public:
  AmerCallSpread(double k1, double k2, const Date &expiry)
      : strike1(k1), strike2(k2), expiry(expiry)
  {
    assert(k1 < k2);
  };
  virtual double Payoff(double S) const
  {
    return PAYOFF::CallSpread(strike1, strike2, S);
  }
  virtual const Date &GetExpiry() const
  {
    return expiry;
  }

private:
  double strike1;
  double strike2;
  Date expiry;
};

#endif
