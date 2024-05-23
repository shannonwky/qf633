#pragma once
#include "Trade.h"
#include "Date.h"
#include "Market.h"
#include <cmath>
#include <vector>

class Swap : public Trade
{
public:
    Swap(Date start, Date end, double _notional, double _tradeRate, double _marketRate, double _frequency)
        : Trade("SwapTrade", start), startDate(start), maturityDate(end), notional(_notional),
          tradeRate(_tradeRate), marketRate(_marketRate), frequency(_frequency) {}

    double Payoff(double marketRate) const override;
    double getAnnuity(const RateCurve &curve) const;
    void Print() const override;

    // This is from template file
    //  Swap(Date start, Date end, double _notional, double _price): Trade("SwapTrade", start) {
    //  inline double Payoff(double marketPrice) const { return 0; };
    //  double getAnnuity() const; //implement this in a cpp file
private:
    Date startDate;
    Date maturityDate;
    double notional;
    double tradeRate;
    double marketRate;
    double frequency; // use 1 for annual, 2 for semi-annual, etc.
};

// //     implement this, using pv = annuity * (trade rate - market rate);
// //     approximately, annuity = sum of (notional * Discount factor at each period end);
// //     for a 2 year swap, with annual freq and notinal 100, annuity = 100 * Df(at year 1 end) + 100* Df(at year 2 end);
// //     Df = exp(-rT), r taken from curve;
