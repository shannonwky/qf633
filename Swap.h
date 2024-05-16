#pragma once
#include "Trade.h"

class Swap : public Trade
{
public:
    Swap(Date _startDate, Date _maturityDate, Date _tradeDate, Date _marketDate, double _notional, double _tradeRate, double _frequency)
        : Trade("SwapTrade", _tradeDate), startDate(_startDate), maturityDate(_maturityDate), notional(_notional), tradeRate(_tradeRate), frequency(_frequency), marketDate(_marketDate) {}
    double Payoff(double marketRate) const override;
    double getAnnuity() const;
    double getRateFromCurve(double time) const;
    double getDiscountFactor(double rate, double time) const;

private:
    Date startDate;
    Date maturityDate;
    Date marketDate;
    double notional;
    double tradeRate;
    double frequency; // use 1 for annual, 2 for semi-annual etc
};