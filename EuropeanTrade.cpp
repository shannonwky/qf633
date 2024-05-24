#include "EuropeanTrade.h"
#include <iostream>

// void EuropeanOption::Print() const
// {
//     std::cout << "European Option: "
//               << "Type: " << (optType == Call ? "Call" : "Put")
//               << ", Strike: " << strike
//               << ", Expiry: " << expiry.year << "-" << expiry.month << "-" << expiry.day
//               << std::endl;
// }


void EuropeanOption::Print() const
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

    std::cout << "European Option: "
              << "Type: " << optionTypeName
              << ", Strike: " << strike
              << ", Expiry: " << expiry.year << "-" << expiry.month << "-" << expiry.day
              << std::endl;
}
