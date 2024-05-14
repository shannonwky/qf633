#pragma once
#include "Trade.h"

class Bond : public Trade {
public:
    Bond(Date start, Date end, double _notional, double _price)
        : Trade("BondTrade", start), startDate(start), maturityDate(end), notional(_notional), tradePrice(_price) {}

    double Payoff(double marketPrice) const override {
        // Assuming that the payoff is the difference between the trade price and the market price times the notional.
        return notional * (tradePrice - marketPrice);
    }

    // Date GetExpiry() const override {
    //     return maturityDate;
    // }

private:
    double notional;
    double tradePrice;
    Date startDate;
    Date maturityDate;
};
