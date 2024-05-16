#pragma once
#include "Date.h"
#include <string>

class Trade
{
public:
    Trade(){};
    Trade(const std::string &tradeType, Date tradeDate) : tradeType(tradeType), tradeDate(tradeDate) {}
    inline std::string getType() const { return tradeType; };
    virtual double Payoff(double marketPrice) const = 0;
    // virtual double Payoff(double marketPrice) { return 0; };
    virtual ~Trade(){};

protected:
    std::string tradeType;
    Date tradeDate;
};