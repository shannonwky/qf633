#ifndef _PRICER
#define _PRICER

#include <vector>
#include <cmath>

#include "Trade.h"
#include "TreeProduct.h"
#include "Market.h"
#include "Date.h"
#include "Payoff.h"
#include "BlackModelPricer.h"


//interface
class Pricer {
public:
  virtual double Price(const Market& mkt, Trade* trade);

private:
  virtual double PriceTree(const Market& mkt, const TreeProduct& trade) { return 0; };
};

//Black Model
class BlackPricer : public Pricer {
public:
   //double Price(const Market& mkt, Trade* trade) override;
    //double Price(const Market& mkt, Trade* trade, double S, double r, double vol, double strike, OptionType optType);
    double Price(const Market& mkt, const TreeProduct& trade, double S, const RateCurve &curve, const VolCurve &volCurve, double strike, OptionType optType);
    //double Price(const Market& mkt, const TreeProduct& trade, double S, double r, double vol, double strike, OptionType optType);
};


//Binomial Tree 
class BinomialTreePricer: public Pricer
{
public:
  BinomialTreePricer(int N) {
    nTimeSteps = N;
    states.resize(N+1);
  }
  //double PriceTree(const Market& mkt, const TreeProduct& trade, double stockPrice, double vol, double rate);
  std::tuple<double, double, double, double> PriceTree(const Market& mkt, const TreeProduct& trade, double stockPrice, const VolCurve &volCurve, const RateCurve &curve);
  //double PriceTree(const Market& mkt, const TreeProduct& trade, double stockPrice, const VolCurve &volCurve, const RateCurve &curve);
  
protected:
  virtual void ModelSetup(double S0, double sigma, double rate, double dt) = 0; // pure virtual
  virtual double GetSpot(int ti, int si) const = 0;
  virtual double GetProbUp() const = 0;
  virtual double GetProbDown() const = 0;
  
private:
  int nTimeSteps;
  std::vector<double> states;
};

class CRRBinomialTreePricer : public BinomialTreePricer
{
public:
  CRRBinomialTreePricer(int N) : BinomialTreePricer(N){}

protected:
  void ModelSetup(double S0, double sigma, double rate, double dt); 
  double GetSpot(int ti, int si) const 
  {
    return currentSpot * std::pow(u, ti-2*si); 
  }
  double GetProbUp() const {return p;}
  double GetProbDown() const {return 1-p;}
  
private:
  double u; // up multiplicative
  double p; // probability for up state
  double currentSpot; // current market spot price
};

class JRRNBinomialTreePricer : public BinomialTreePricer
{
public:
  JRRNBinomialTreePricer(int N) : BinomialTreePricer(N){}
 
protected:
  void ModelSetup(double S0, double sigma, double rate, double dt);
  
  double GetSpot(int ti, int si) const
  {
    return currentSpot * std::pow(u, ti-si) * std::pow(d, si);
  }
  double GetProbUp() const {return p;}
  double GetProbDown() const {return 1-p;}

private:
  double u; // up multiplicative
  double d; // down multiplicative
  double p; // probability for up state
  double currentSpot; // current market spot price
}; 

#endif

