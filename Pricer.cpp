#include <cmath>
#include "Pricer.h"
#include "BlackModelPricer.h"


double Pricer::Price(const Market& mkt, Trade* trade) {
  double pv;
  if (trade->getType() == "TreeProduct") {
    TreeProduct* treePtr = dynamic_cast<TreeProduct*>(trade);
    if (treePtr) { //check if cast is sucessful
      pv = PriceTree(mkt, *treePtr);
    }
  }
  else{
    double price; //get from market data
    pv = trade->Payoff(price);
  }

  return pv;
}

//Black Model
//double BlackPricer::Price(const Market& mkt, Trade* trade, double S, double r, double vol, double strike, OptionType optType) {
//double BlackPricer::Price(const Market& mkt, const TreeProduct& trade, double S, double r, double vol, double strike, OptionType optType) {
double BlackPricer::Price(const Market& mkt, const TreeProduct& trade, double S, const RateCurve &curve, const VolCurve &volCurve, double strike, OptionType optType) {
  BlackModelPricer blackModelPricer;
  //double T = trade->GetExpiry() - mkt.asOf; // Time to maturity
  double T = trade.GetExpiry() - mkt.asOf;
  double days_left = T * 365.25;
  std::string tenor = std::to_string(T) + "Y";
  double rate = curve.getRate(tenor) * 0.01;
  double vol = volCurve.getVol(days_left) *0.01;
  return blackModelPricer.Price(S, rate, vol, T, strike, optType);
}


//Binomial Tree Model
std::tuple<double, double, double, double> BinomialTreePricer::PriceTree(const Market& mkt, const TreeProduct& trade, double stockPrice, const VolCurve &volCurve, const RateCurve &curve) {
//double BinomialTreePricer::PriceTree(const Market& mkt, const TreeProduct& trade, double stockPrice, const VolCurve &volCurve, const RateCurve &curve) {
  double T = trade.GetExpiry() - mkt.asOf;
  double dt = T / nTimeSteps;
  double days_left = T * 365.25;
  std::string tenor = std::to_string(T) + "Y";
  double rate = curve.getRate(tenor) * 0.01;
  double vol = volCurve.getVol(days_left) *0.01;

  
  ModelSetup(stockPrice, vol, rate, dt);
  
  // initialize
  for (int i = 0; i <= nTimeSteps; i++) {
    states[i] = trade.Payoff( GetSpot(nTimeSteps, i) );
  }    
  
  // price by backward induction
  for (int k = nTimeSteps-1; k >= 0; k--){
    for (int i = 0; i <= k; i++) {
    // calculate continuation value
      double df = exp(-rate *dt);	  
      double continuation = df * (states[i]*GetProbUp() + states[i+1]*GetProbDown());
      // calculate the option value at node(k, i)
      states[i] = trade.ValueAtNode( GetSpot(k, i), dt*k, continuation);
    }
  }

  double optionPrice = states[0];
  
  return std::make_tuple(optionPrice, days_left, rate, vol);

}

void CRRBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
  double b = std::exp((2*rate+sigma*sigma)*dt)+1;
  u = (b + std::sqrt(b*b - 4*std::exp(2*rate*dt))) / 2 / std::exp(rate*dt);
  p = (std::exp(rate*dt) -  1/u) / (u - 1/u);
  currentSpot = S0;
}

void JRRNBinomialTreePricer::ModelSetup(double S0, double sigma, double rate, double dt)
{
    u = std::exp( (rate - sigma*sigma/2) * dt + sigma * std::sqrt(dt) );
    d = std::exp( (rate - sigma*sigma/2) * dt - sigma * std::sqrt(dt) );
    p = (std::exp(rate*dt) -  d) / (u - d);
    currentSpot = S0;
}

