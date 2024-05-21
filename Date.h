#ifndef DATE_H
#define DATE_H

#include <iostream>

class Date
{
 public:
  int year;
  int month;
  int day;
  Date(int y, int m, int d) : year(y), month(m), day(d) {};
  Date(){};

  // friend double operator-(const Date& d1, const Date& d2);
  // friend std::ostream& operator<<(std::ostream& os, const Date& d);
  // friend std::istream& operator>>(std::istream& is, const Date& d);
  
  bool operator<(const Date& other) const;

  //double GetYearFraction() const;
  Date addYears(double years) const;
  
  private:
  int daysInMonth(int month, int year) const;
  bool isLeapYear(int year) const;


};

double operator-(const Date& d1, const Date& d2);
std::ostream& operator<<(std::ostream& os, const Date& date);
std::istream& operator>>(std::istream& is, Date& date);

#endif
