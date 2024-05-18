#pragma once
#include "Trade.h"


class Bond : public Trade
{
public:
    Bond(Date _startDate, Date _maturityDate, double _notional, double _tradePrice)
        : Trade("BondTrade", _startDate), notional(_notional), tradePrice(_tradePrice), startDate(_startDate), maturityDate(_maturityDate) {}

    inline double Payoff(double marketPrice) const override { return marketPrice - tradePrice; };


private:
    double notional;
    double tradePrice;

    int couponFrequency;
    Date startDate;
    Date maturityDate;
};

