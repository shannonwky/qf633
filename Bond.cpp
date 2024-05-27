#include "Bond.h"
#include <iostream>


double Bond::Payoff(double marketPrice) const
{
    return notional * (tradePrice - marketPrice);
}

void Bond::Print() const
{
    std::cout << "Bond: "
              << "Start Date: " << startDate.year << "-" << startDate.month << "-" << startDate.day
              << ", Maturity Date: " << maturityDate.year << "-" << maturityDate.month << "-" << maturityDate.day
              << ", Notional: " << notional
              << ", Trade Price: " << tradePrice
              << ", Position: " << (notional > 0 ? "Long" : "Short")
              << std::endl;
}
