#include "AmericanTrade.h"
#include <iostream>

// void AmericanOption::Print() const
// {
//     std::cout << "American Option: "
//               << "Type: " << (optType == Call ? "Call" : "Put")
//               << ", Strike: " << strike
//               << ", Expiry: " << expiry.year << "-" << expiry.month << "-" << expiry.day
//               << std::endl;
// }

void AmericanOption::Print() const
{
    std::string optionTypeName;
    switch (optType) {
        case Call:
            optionTypeName = "Call";
            break;
        case Put:
            optionTypeName = "Put";
            break;
        case BinaryCall:
            optionTypeName = "Binary Call";
            break;
        case BinaryPut:
            optionTypeName = "Binary Put";
            break;
        default:
            optionTypeName = "Unknown";
            break;
    }

    std::cout << "American Option: "
              << "Type: " << optionTypeName
              << ", Strike: " << strike
              << ", Expiry: " << expiry.year << "-" << expiry.month << "-" << expiry.day
              << std::endl;
}

