// #pragma once
// #include "Trade.h"

// class Bond : public Trade
// {
// public:
//     Bond(Date _startDate, Date _maturityDate, double _notional, double _tradePrice)
//         : Trade("BondTrade", _startDate), notional(_notional), tradePrice(_tradePrice), startDate(_startDate), maturityDate(_maturityDate) {}

//     inline double Payoff(double marketPrice) const override { return marketPrice - tradePrice; };

// private:
//     double notional;
//     double tradePrice;

//     int couponFrequency;
//     Date startDate;
//     Date maturityDate;
// };

#pragma once
#include "Trade.h"

class Bond : public Trade
{
public:
    Bond(int id, Date start, Date end, double _notional, double _price)
        : Trade(id, "BondTrade", start), startDate(start), maturityDate(end), notional(_notional), tradePrice(_price) {}

 
    virtual const Date &GetStart() const override { return startDate; }
    virtual const Date &GetExpiry() const override { return maturityDate; }
    double Payoff(double marketPrice) const override;
    void Print() const override;

    // Date GetExpiry() const override {
    //     return maturityDate;
    // }

private:
    double notional;
    double tradePrice;
    Date startDate;
    Date maturityDate;
};
