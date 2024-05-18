// #ifndef _AMERICAN_TRADE
// #define _AMERICAN_TRADE

// #include <cassert>

// #include "TreeProduct.h"
// #include "Types.h"
// #include "Payoff.h"

// class AmericanOption : public TreeProduct
// {
// public:
//   AmericanOption(OptionType _optType, double _strike, const Date &_expiry) : optType(_optType), strike(_strike), expiryDate(_expiry) {}
//   virtual double Payoff(double S) const
//   {
//     return PAYOFF::VanillaOption(optType, strike, S);
//   }
//   virtual const Date &GetExpiry() const
//   {
//     return expiryDate;
//   }
//   virtual double ValueAtNode(double S, double t, double continuation) const
//   {
//     return std::max(Payoff(S), continuation);
//   }

// private:
//   OptionType optType;
//   double strike;
//   Date expiryDate;
// };

// class AmerCallSpread : public TreeProduct
// {
// public:
//   AmerCallSpread(double _k1, double _k2, const Date &_expiry)
//       : strike1(_k1), strike2(_k2), expiryDate(_expiry)
//   {
//     assert(_k1 < _k2);
//   };
//   virtual double Payoff(double S) const
//   {
//     return PAYOFF::CallSpread(strike1, strike2, S);
//   }
//   virtual const Date &GetExpiry() const
//   {
//     return expiryDate;
//   }

// private:
//   double strike1;
//   double strike2;
//   Date expiryDate;
// };

// #endif


#ifndef _AMERICAN_TRADE
#define _AMERICAN_TRADE

#include <algorithm>
#include <stdexcept>
#include "Types.h"

class AmericanOption {
public:
    AmericanOption(double expiry, double strike, OptionType payoffType)
        : expiry(expiry), strike(strike), payoffType(payoffType) {}

    double payoff(double S) const {
        if (payoffType == OptionType::Call) {
            return std::max(S - strike, 0.0);
        } else if (payoffType == OptionType::Put) {
            return std::max(strike - S, 0.0);
        } else if (payoffType == OptionType::BinaryCall) {
            return (S >= strike) ? 1.0 : 0.0;
        } else if (payoffType == OptionType::BinaryPut) {
            return (S <= strike) ? 1.0 : 0.0;
        } else {
            throw std::invalid_argument("Unsupported payoffType");
        }
    }

    double valueAtNode(double t, double S, double continuation) const {
        return std::max(payoff(S), continuation);
    }

    double getExpiry() const { return expiry; }
    double getStrike() const { return strike; }
    OptionType getPayoffType() const { return payoffType; }

private:
    double expiry;
    double strike;
    OptionType payoffType;
};

#endif

