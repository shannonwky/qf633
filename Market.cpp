#include "Market.h"

using namespace std;

double RateCurve::convertToYears(const std::string &timeStr) const
{
  if (timeStr.find("M") != std::string::npos)
  {
    double months = std::stod(timeStr);
    return months / 12.0;
  }
  else if (timeStr.find("Y") != std::string::npos)
  {
    return std::stod(timeStr);
  }
  return 0.0;
}

void RateCurve::addRate(const std::string &tenorStr, double rate)
{
  double tenor = convertToYears(tenorStr);
  X.push_back(tenor);
  Y.push_back(rate);
}

double RateCurve::getRate(const std::string &tenorStr) const
{
  double tenor = convertToYears(tenorStr);

  tk::spline s(X, Y);
  double interpolatedY = s(tenor);
  // std::cout << "Interpolated value at x=" << tenor << " is " << interpolatedY << std::endl;

  return interpolatedY;
}

// Implement the display function outside the class declaration
void RateCurve::display() const
{
  std::cout << "Rate curve:" << std::endl;
  for (size_t i = 0; i < X.size(); i++)
  {
    std::cout << X[i] << ":" << Y[i] << std::endl;
  }
  std::cout << std::endl;
}

void VolCurve::addVol(Date tenor, double vol)
{
  tenors.push_back(tenor);
  vols.push_back(vol);
}

double VolCurve::getVol(Date tenor) const
{
  if (tenors.empty())
  {
    throw std::runtime_error("No Vols available");
  }

  // If the requested tenor is before the first tenor or after the last, return the boundary values
  if (tenor <= tenors.front())
  {
    return vols.front();
  }
  if (tenor >= tenors.back())
  {
    return vols.back();
  }

  // Find the two tenors surrounding the requested tenor
  for (size_t i = 1; i < tenors.size(); ++i)
  {
    if (tenor < tenors[i])
    {
      double t1 = tenors[i - 1].toDays();
      double t2 = tenors[i].toDays();
      double v1 = vols[i - 1];
      double v2 = vols[i];

      // Linear interpolation
      double interpolatedVol = v1 + (v2 - v1) * (tenor.toDays() - t1) / (t2 - t1);
      return interpolatedVol;
    }
  }

  // Default case, should not reach here
  return vols.back();
}

void VolCurve::display() const
{
  std::cout << "Vol curve:" << std::endl;
  for (size_t i = 0; i < tenors.size(); ++i)
  {
    std::cout << "Tenor: " << tenors[i].year << "-" << tenors[i].month << "-" << tenors[i].day << ", Volatility: " << vols[i] << "%" << std::endl;
  }
}

void Market::Print() const
{
  cout << "market asof: " << asOf << endl;

  for (auto curve : curves)
  {
    curve.second.display();
  }
  for (auto vol : vols)
  {
    vol.second.display();
  }
  for (const auto &pair : bondPrices)
  {
    std::cout << "Bond: " << pair.first << ", Price: " << pair.second << endl;
  }
  for (const auto &pair : stockPrices)
  {
    std::cout << "Stock: " << pair.first << ", Price: " << pair.second << endl;
  }
  /*
  add display for bond price and stock price

  */
}

void Market::addCurve(const std::string &curveName, const RateCurve &curve)
{
  curves.emplace(curveName, curve);
}

void Market::addVolCurve(const std::string &curveName, const VolCurve &curve)
{
  vols[curveName] = curve;
}

void Market::addBondPrice(const std::string &bondName, double price)
{
  bondPrices[bondName] = price;
}

void Market::addStockPrice(const std::string &stockName, double price)
{
  stockPrices[stockName] = price;
}

std::ostream &operator<<(std::ostream &os, const Market &mkt)
{
  os << mkt.asOf << std::endl;
  return os;
}

std::istream &operator>>(std::istream &is, Market &mkt)
{
  is >> mkt.asOf;
  return is;
}
