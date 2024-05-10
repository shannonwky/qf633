#include "Market.h"

std::ostream& operator<<(std::ostream& os, const Market& mkt){
    os << mkt.asOf;
    return os;
}

std::istream& operator>>(std::istream& is, Market& mkt){
    is >> mkt.asOf;
    return is;
}

void Market::Print() const{
    std::cout << "maket asof: " << asOf << std::endl;
}