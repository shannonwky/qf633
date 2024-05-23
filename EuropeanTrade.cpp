#include "EuropeanTrade.h"
#include <iostream>

void EuropeanOption::Print() const
{
    std::cout << "European Option: "
              << "Type: " << (optType == Call ? "Call" : "Put")
              << ", Strike: " << strike
              << ", Expiry: " << expiry.year << "-" << expiry.month << "-" << expiry.day
              << std::endl;
}
