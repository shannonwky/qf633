#include "Bond.h"
#include <iostream>

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
