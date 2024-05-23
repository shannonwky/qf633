#ifndef MARKET_H
#define MARKET_H

#include <vector>
#include <unordered_map>
#include "Date.h"
#include "spline.h"

using namespace std;

class RateCurve
{
private:
  std::vector<double> X; // Tenor values
  std::vector<double> Y; // Rate values

  double convertToYears(const std::string &timeStr) const;

public:
  void addRate(const std::string &tenorStr, double rate);
  double getRate(const std::string &tenorStr) const;
  void display() const;
};

class VolCurve
{ // atm vol curve without smile
public:
  VolCurve() {}
  VolCurve(const string &_name) : name(_name){};
  void addVol(Date tenor, double rate); // implement this
  double getVol(Date tenor) const;      // implement this function using linear interpolation
  void display() const;                 // implement this

private:
  string name;
  vector<Date> tenors;
  vector<double> vols;
};

class Market
{
public:
  Date asOf;
  Market(const Date &now) : asOf(now) {}
  void Print() const;
  void addCurve(const std::string &curveName, const RateCurve &curve);   // implement this
  void addVolCurve(const std::string &curveName, const VolCurve &curve); // implement this
  void addBondPrice(const std::string &bondName, double price);          // implement this
  void addStockPrice(const std::string &stockName, double price);        // implement this

  inline RateCurve getCurve(const string &name) { return curves[name]; };
  inline VolCurve getVolCurve(const string &name) { return vols[name]; };

private:
  unordered_map<string, VolCurve> vols;
  unordered_map<string, RateCurve> curves;
  unordered_map<string, double> bondPrices;
  unordered_map<string, double> stockPrices;
};

std::ostream &operator<<(std::ostream &os, const Market &obj);
std::istream &operator>>(std::istream &is, Market &obj);

#endif
