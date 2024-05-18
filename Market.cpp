#include "Market.h"

using namespace std;


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


