// #ifndef _PRICER
// #define _PRICER

// #include <vector>
// #include <cmath>

// #include "Trade.h"
// #include "TreeProduct.h"
// #include "Market.h"
// #include "Date.h"

// //interface
// class Pricer {
// public:
//   virtual double Price(const Market& mkt, Trade* trade);

// private:
//   virtual double PriceTree(const Market& mkt, const TreeProduct& trade) { return 0; };
// };

// class BinomialTreePricer: public Pricer
// {
// public:
//   BinomialTreePricer(int N) {
//     nTimeSteps = N;
//     states.resize(N+1);
//   }
//   double PriceTree(const Market& mkt, const TreeProduct& trade) override;

// protected:
//   virtual void ModelSetup(double S0, double sigma, double rate, double dt) = 0; // pure virtual
//   virtual double GetSpot(int ti, int si) const = 0;
//   virtual double GetProbUp() const = 0;
//   virtual double GetProbDown() const = 0;

// private:
//   int nTimeSteps;
//   std::vector<double> states;
// };

// class CRRBinomialTreePricer : public BinomialTreePricer
// {
// public:
//   CRRBinomialTreePricer(int N) : BinomialTreePricer(N){}

// protected:
//   void ModelSetup(double S0, double sigma, double rate, double dt);
//   double GetSpot(int ti, int si) const {
//     return currentSpot * std::pow(u, ti-2*si);
//   }
//   double GetProbUp() const {return p;}
//   double GetProbDown() const {return 1-p;}

// private:
//   double u; // up multiplicative
//   double p; // probability for up state
//   double currentSpot; // current market spot price
// };

// class JRRNBinomialTreePricer : public BinomialTreePricer
// {
// public:
//   JRRNBinomialTreePricer(int N) : BinomialTreePricer(N){}

// protected:
//   void ModelSetup(double S0, double sigma, double rate, double dt);

//   double GetSpot(int ti, int si) const
//   {
//     return currentSpot * std::pow(u, ti-si) * std::pow(d, si);
//   }
//   double GetProbUp() const {return p;}
//   double GetProbDown() const {return 1-p;}

// private:
//   double u; // up multiplicative
//   double d; // down multiplicative
//   double p; // probability for up state
//   double currentSpot; // current market spot price
// };

// #endif

#ifndef _PRICER
#define _PRICER

#include <tuple>
#include <vector>
#include <cmath>

#include "EuropeanTrade.h"
#include "AmericanTrade.h"

std::tuple<double, double, double> crrCalib(double r, double vol, double t);
std::tuple<double, double, double> jrrnCalib(double r, double vol, double t);
std::tuple<double, double, double> jreqCalib(double r, double vol, double t);
std::tuple<double, double, double> tianCalib(double r, double vol, double t);

template <typename OptionType>
double binomialPricer(double S, double r, double vol, const OptionType &trade, int n, std::tuple<double, double, double> (*calib)(double, double, double))
{
    double t = trade.getExpiry() / n;
    std::tuple<double, double, double> calibResult = calib(r, vol, t);
    double u = std::get<0>(calibResult);
    double d = std::get<1>(calibResult);
    double p = std::get<2>(calibResult);

    // Set up the last time slice, there are n+1 nodes at the last time slice
    std::vector<double> vs(n + 1);
    for (int i = 0; i <= n; ++i)
    {
        vs[i] = trade.payoff(S * std::pow(u, n - i) * std::pow(d, i));
    }

    // Iterate backward
    for (int i = n - 1; i >= 0; --i)
    {
        for (int j = 0; j <= i; ++j)
        {
            double nodeS = S * std::pow(u, i - j) * std::pow(d, j);
            double continuation = std::exp(-r * t) * (vs[j] * p + vs[j + 1] * (1 - p));
            vs[j] = trade.valueAtNode(t * i, nodeS, continuation);
        }
    }

    return vs[0];
}

// Cumulative normal distribution function
inline double cnorm(double x)
{
    return (1.0 + erf(x / sqrt(2.0))) / 2.0;
}

// Black model pricer
inline double blackPrice(double S, double r, double vol, double T, double strike, OptionType payoffType)
{
    double F = S * exp(r * T); // Forward price
    double stdev = vol * sqrt(T);
    double d1 = (log(F / strike) + 0.5 * stdev * stdev) / stdev;
    double d2 = d1 - stdev;
    if (payoffType == OptionType::Call)
    {
        return exp(-r * T) * (F * cnorm(d1) - strike * cnorm(d2));
    }
    else if (payoffType == OptionType::Put)
    {
        return exp(-r * T) * (strike * cnorm(-d2) - F * cnorm(-d1));
    }
    else if (payoffType == OptionType::BinaryCall)
    {
        return exp(-r * T) * cnorm(d2);
    }
    else if (payoffType == OptionType::BinaryPut)
    {
        return exp(-r * T) * cnorm(-d2);
    }
    else
    {
        throw std::invalid_argument("Unsupported payoffType");
    }
}

#endif
