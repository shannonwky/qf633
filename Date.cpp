#include "Date.h"

std::ostream &operator<<(std::ostream &os, const Date &date)
{
    os << date.year << " " << date.month << " " << date.day;

    return os;
}

std::istream &operator>>(std::istream &is, Date &date)
{
    is >> date.year >> date.month >> date.day;

    return is;
}

double operator-(const Date &d1, const Date &d2)
{
    int yearDiff = d1.year - d2.year;
    int monthDiff = d1.month - d2.month;
    int dayDiff = d1.day - d2.day;

    return yearDiff + monthDiff / 12.0 + dayDiff / 365.0;
}