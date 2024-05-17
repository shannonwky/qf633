#ifndef MARKET_H
#define MARKET_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include "Date.h"
#include <string>
#include "spline.h" // Include the spline header if needed

using namespace std;


class RateCurve {
private:
    std::vector<double> X; // Tenor values
    std::vector<double> Y; // Rate values

    // Function to convert tenor strings to numerical values in years
    double convertToYears(const std::string& timeStr) const {
        if (timeStr.find("M") != std::string::npos) {
            // Extract the numeric part and convert it to years
            double months = std::stod(timeStr);
            return months / 12.0;
        } else if (timeStr.find("Y") != std::string::npos) {
            // Extract the numeric part
            return std::stod(timeStr);
        }
        // If the format is not recognized, return 0
        return 0.0;
    }

public:
    // Function to add a rate to the curve
    void addRate(const std::string& tenorStr, double rate) {
        double tenor = convertToYears(tenorStr);
        X.push_back(tenor);
        Y.push_back(rate);
    }

    // Function to get the rate using linear interpolation
    double getRate(const std::string& tenorStr) const {
        double tenor = convertToYears(tenorStr);

        tk::spline s(X, Y);
        double interpolatedY = s(tenor);
        // double derivative = s.deriv(1, x);

        // Output the interpolated value and its derivative (if necessary)
        std::cout << "Interpolated value at x=" << tenor << " is " << interpolatedY << std::endl;


        // // Find the two closest tenors
        // size_t i = 0;
        // while (i < X.size() && X[i] < tenor) {
        //     i++;
        // }

        // // Perform linear interpolation
        // if (i == 0) {
        //     return Y[0]; // Extrapolation: Return the first rate
        // } else if (i == X.size()) {
        //     return Y.back(); // Extrapolation: Return the last rate
        // } else {
        //     // Interpolation between X[i-1] and X[i]
        //     double x0 = X[i - 1];
        //     double x1 = X[i];
        //     double y0 = Y[i - 1];
        //     double y1 = Y[i];
        //     return y0 + (y1 - y0) * ((tenor - x0) / (x1 - x0));
        // }
    }

    // Function to display the rate curve
    void display() const;
};







class VolCurve { // atm vol curve without smile
public:
  VolCurve(){}
  VolCurve(const string& _name): name(_name) {} ; 
  void addVol(Date tenor, double rate); //implement this
  double getVol(Date tenor) const; //implement this function using linear interpolation
  void display() const; //implement this

private:
  string name;
  vector<Date> tenors;
  vector<double> vols;
};

class Market
{
public:
  Date asOf;
  Market(const Date& now): asOf(now) {}
  void Print() const;
  void addCurve(const std::string& curveName, const RateCurve& curve);//implement this
  void addVolCurve(const std::string& curveName, const VolCurve& curve);//implement this
  void addBondPrice(const std::string& bondName, double price);//implement this
  void addVolCurve(const std::string& stockName, double price);//implement this
  
  inline RateCurve getCurve(const string& name) { return curves[name]; };
  inline VolCurve getVolCurve(const string& name) { return vols[name]; };

private:
  
  unordered_map<string, VolCurve> vols;
  unordered_map<string, RateCurve> curves;
  unordered_map<string, double> bondPrices;
  unordered_map<string, double> stockPrices;
};

std::ostream& operator<<(std::ostream& os, const Market& obj);
std::istream& operator>>(std::istream& is, Market& obj);

#endif
