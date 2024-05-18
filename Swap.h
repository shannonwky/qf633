#pragma once
#include "Trade.h"
#include "Date.h"
#include <cmath>
#include <vector>

class Swap : public Trade {
public:
    Swap(Date start, Date end, double _notional, double _price, double _tradeRate, double _frequency)
        : Trade("SwapTrade", start), startDate(start), maturityDate(end), notional(_notional),
          tradeRate(_tradeRate), frequency(_frequency) {}

    double Payoff(double marketRate) const override;
    double getAnnuity(double marketRate) const;

    // double getRateFromCurve(double time) const;
    // double getDiscountFactor(double rate, double time) const;


//     implement this, using pv = annuity * (trade rate - market rate);
//     approximately, annuity = sum of (notional * Discount factor at each period end);
//     for a 2 year swap, with annual freq and notinal 100, annuity = 100 * Df(at year 1 end) + 100* Df(at year 2 end);
//     Df = exp(-rT), r taken from curve;


private:
    Date startDate;
    Date maturityDate;
    double notional;
    double tradeRate;
    double frequency; // use 1 for annual, 2 for semi-annual, etc.
};