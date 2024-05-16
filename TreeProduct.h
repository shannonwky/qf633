#pragma once
#include "Date.h"
#include "Trade.h"

class TreeProduct : public Trade
{
public:
    TreeProduct() : Trade()
    {
        tradeType = "TreeProduct";
    };
    virtual const Date &GetExpiry() const = 0;
    virtual double valueAtNode(double stockPrice, double t, double continuationValue) const = 0;
};