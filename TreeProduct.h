#ifndef _TREE_PRODUCT_H
#define _TREE_PRODUCT_H
#include "Date.h"
#include "Trade.h"
//#pragma once

//option type of trade, will be priced using tree model
class TreeProduct: public Trade
{
public:
    TreeProduct() : Trade()
    {
        tradeType = "TreeProduct";
    };
    virtual const Date& GetExpiry() const = 0;
    virtual double ValueAtNode(double S, double t, double continuation) const = 0;
};

#endif
