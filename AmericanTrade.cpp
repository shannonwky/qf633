#include "AmericanTrade.h"
#include <iostream>

void AmericanOption::Print() const
{
    std::cout << "American Option: "
              << "Type: " << (optType == Call ? "Call" : "Put")
              << ", Strike: " << strike
              << ", Expiry: " << expiry.year << "-" << expiry.month << "-" << expiry.day
              << std::endl;
}
