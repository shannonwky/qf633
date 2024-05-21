#include "Date.h"

// Calculates the difference in years between two dates.
double operator-(const Date& d1, const Date& d2)
{
    int yearDiff = d1.year - d2.year;
    int monthDiff = d1.month - d2.month;
    int dayDiff = d1.day - d2.day;

    return yearDiff + monthDiff / 12.0 + dayDiff / 365.0;
}

// Outputs the date in "year month day" format.
std::ostream& operator<<(std::ostream& os, const Date& d)
{
    os << d.year << " " << d.month << " " << d.day << std::endl;
    return os;
}

// Reads the date in "year month day" format.
std::istream& operator>>(std::istream& is, Date& d)
{
    is >> d.year >> d.month >> d.day;
    return is;
}

// Compares if one date is less than another date.
bool Date::operator<(const Date& other) const {
    if (year != other.year) {
        return year < other.year;
    }
    if (month != other.month) {
        return month < other.month;
    }
    return day < other.day;
}

// Adds a specified number of years to the date.
Date Date::addYears(double years) const {
    int newYear = year + static_cast<int>(years);
    int newMonth = month;
    int newDay = day;

    double fractionalYear = years - static_cast<int>(years);
    if (fractionalYear > 0) {
        newMonth += static_cast<int>(fractionalYear * 12);
        if (newMonth > 12) {
            newYear++;
            newMonth -= 12;
        }
    }

    // Handle cases where the day might exceed the days in the new month.
    int daysInNewMonth = daysInMonth(newMonth, newYear);
    if (newDay > daysInNewMonth) {
        newDay = daysInNewMonth;
    }

    return Date(newYear, newMonth, newDay);
}

// Checks if a year is a leap year.
bool Date::isLeapYear(int year) const {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Returns the number of days in a given month and year.
int Date::daysInMonth(int month, int year) const {
    static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if (month == 2 && isLeapYear(year)) {
        return 29;
    }
    return daysInMonth[month - 1];
}



// double Date::GetYearFraction() const
// {
//     return year + month / 12.0 + day / 365.0;
// }