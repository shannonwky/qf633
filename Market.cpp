#include "Market.h"

using namespace std;

double RateCurve::convertToYears(const std::string& timeStr) const {
    if (timeStr.find("M") != std::string::npos) {
        double months = std::stod(timeStr);
        return months / 12.0;
    } else if (timeStr.find("Y") != std::string::npos) {
        return std::stod(timeStr);
    }
    return 0.0;
}

void RateCurve::addRate(const std::string& tenorStr, double rate) {
    double tenor = convertToYears(tenorStr);
    X.push_back(tenor);
    Y.push_back(rate);
}

double RateCurve::getRate(const std::string& tenorStr) const {
    double tenor = convertToYears(tenorStr);

    tk::spline s(X, Y);
    double interpolatedY = s(tenor);
    //std::cout << "Interpolated value at x=" << tenor << " is " << interpolatedY << std::endl;

    return interpolatedY;
}

// Implement the display function outside the class declaration
void RateCurve::display() const {
    std::cout << "Rate curve:" << std::endl;
    for (size_t i = 0; i < X.size(); i++) {
        std::cout << X[i] << ":" << Y[i] << std::endl;
    }
    std::cout << std::endl;
}


void VolCurve::display() const {

}

void Market::Print() const
{
  cout << "market asof: " << asOf << endl;
  
  for (auto curve: curves) {
    curve.second.display();
  }
  for (auto vol: vols) {
    vol.second.display();
  }
  /*
  add display for bond price and stock price
  
  */
}

void Market::addCurve(const std::string& curveName, const RateCurve& curve){
  curves.emplace(curveName, curve);
}

std::ostream& operator<<(std::ostream& os, const Market& mkt)
{
  os << mkt.asOf << std::endl;
  return os;
}

std::istream& operator>>(std::istream& is, Market& mkt)
{
  is >> mkt.asOf;
  return is;
}


