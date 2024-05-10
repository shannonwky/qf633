#ifndef MARKET_H
#define MARKET_H

#include "Date.h"

class Market{
public:
    Date asOf;
    Market(){}
    Market(const Date& now): asOf(now){}
    void Print() const;
};

std::ostream& operator<<(std::ostream& os, const Market& mkt);
std::istream& operator>>(std::istream& is, Market& mkt);

#endif