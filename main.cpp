#include <fstream>
#include <ctime>
#include <chrono>
#include <iostream>


#include "EuropeanTrade.h"
#include "AmericanTrade.h"
#include "Bond.h"
#include "Swap.h"
#include "FileUtils.h"


/*
for above, there is no need to put all header file here right? 
only need to put ones you need, e.g., market.h contains date.h and spline.h, 
so need to be incldued here again
*/

using namespace std;

int main()
{
  // task 1, create an market data object, and update the market data from from txt file
  // 1.1 date
  std::cout << "\nTask1:\n\n";
  std::time_t t = std::time(0);
  auto now_ = std::localtime(&t);
  Date valueDate;
  valueDate.year = now_->tm_year + 1900;
  valueDate.month = now_->tm_mon + 1;
  valueDate.day = now_->tm_mday;
  // cout << "Year Type:" << valueDate.year << endl;
  // cout << "Month Type:" << valueDate.month << endl;
  // cout << "Date Type:" << valueDate.day << endl;
  Market mkt = Market(valueDate);

  // 1.2 ratecurve
  // Read rates and interpolate
  RateCurve curve;
  vector<string> tenors;
  vector<double> rates;
  // Assuming your file is named "curve.txt"
  readRatesFromFile("curve.txt", tenors, rates, true);
  // Display the contents of tenors and rates
  for (size_t i = 0; i < tenors.size(); ++i)
  {
    curve.addRate(tenors[i], rates[i]);
  }
  mkt.addCurve("USD-SOFR", curve);
  // curve.display();
  // Get rate for a specific tenor (example)
  // string tenor_check = "3.3Y";
  // double obtained_rate = curve.getRate(tenor_check);
  // std::cout << "Interpolated value at x=" << tenor_check << " is " << obtained_rate << std::endl;

  // 1.3 volcurve
  VolCurve volCurve("USD-ATM");
  readVolCurveFromFile("vol.txt", volCurve);
  mkt.addVolCurve("USD-ATM", volCurve);
//   volCurve.display();
//   Date tenorToCheck(2025, 1, 1); // Example date
//   double tenor = tenorToCheck - mkt.asOf;
//   double volvis = volCurve.getVol(tenor);
//   std::cout << "Interpolated volatility for tenor " << tenorToCheck << " is " << volvis << "%" << std::endl;
  // mkt.addVolCurve("USD-ATM", volCurve1);

  // 1.4 bond
  std::unordered_map<std::string, double> bondPrices;
  readBondPricesFromFile("bondPrices.txt", bondPrices);
  for (const auto &pair : bondPrices)
  {
    mkt.addBondPrice(pair.first, pair.second);
  }

  // 1.5 stock
  std::unordered_map<std::string, double> stockPrices;
  readStockPricesFromFile("stockPrices.txt", stockPrices);
  for (const auto &pair : stockPrices)
  {
    mkt.addStockPrice(pair.first, pair.second);
  }

  mkt.Print();
  std::cout << "\nEnd of Task 1\n\n"
            << std::endl;

  // task 2, create a portfolio of bond, swap, european option, american option
  // for each time, at least should have long / short, different tenor or expiry, different underlying
  // totally no less than 16 trades
  std::cout << "Task 2:\n\n";
  vector<Trade *> myPortfolio;

  int tradeID = 1;

  // Bonds
  // Adding bonds
  Trade *bond1 = new Bond(tradeID++ ,Date(2024, 1, 1), Date(2034, 1, 1), 10000000, 103.5);
  Trade *bond2 = new Bond(tradeID++ ,Date(2024, 1, 1), Date(2029, 1, 1), 5000000, 99.0);
  Trade *bond3 = new Bond(tradeID++ ,Date(2024, 1, 1), Date(2030, 1, 1), 15000000, 101.0);
  Trade *bond4 = new Bond(tradeID++ ,Date(2024, 1, 1), Date(2027, 1, 1), 7500000, 98.0);
  myPortfolio.push_back(bond1);
  myPortfolio.push_back(bond2);
  myPortfolio.push_back(bond3);
  myPortfolio.push_back(bond4);

  // Swaps
  // Adding swaps
  Trade *swap1 = new Swap(tradeID++ ,Date(2023, 1, 1), Date(2025, 1, 1), 1000000, 0.05, 0.04, 1);
  Trade *swap2 = new Swap(tradeID++ ,Date(2023, 1, 1), Date(2026, 1, 1), 2000000, 0.04, 0.03, 2);
  Trade *swap3 = new Swap(tradeID++ ,Date(2023, 1, 1), Date(2024, 1, 1), 3000000, 0.06, 0.05, 1);
  Trade *swap4 = new Swap(tradeID++ ,Date(2023, 1, 1), Date(2027, 1, 1), 4000000, 0.03, 0.02, 2);
  myPortfolio.push_back(swap1);
  myPortfolio.push_back(swap2);
  myPortfolio.push_back(swap3);
  myPortfolio.push_back(swap4);

  // Adding European options
  Trade *europeanCall1 = new EuropeanOption(tradeID++ ,Date(2025, 5, 19), 105.0, Call);
  Trade *europeanPut1 = new EuropeanOption(tradeID++ ,Date(2026, 5, 19), 100.0, Put);
  Trade *europeanCall2 = new EuropeanOption(tradeID++ ,Date(2025, 11, 19), 110.0, BinaryCall);
  Trade *europeanPut2 = new EuropeanOption(tradeID++ ,Date(2026, 11, 19), 95.0, BinaryPut);
  myPortfolio.push_back(europeanCall1);
  myPortfolio.push_back(europeanPut1);
  myPortfolio.push_back(europeanCall2);
  myPortfolio.push_back(europeanPut2);

  // Adding American options
  Trade *americanCall1 = new AmericanOption(tradeID++ ,Date(2025, 5, 19), 110.0, Call);
  Trade *americanPut1 = new AmericanOption(tradeID++ ,Date(2026, 5, 19), 95.0, Put);
  Trade *americanCall2 = new AmericanOption(tradeID++ ,Date(2025, 11, 19), 115.0, BinaryCall);
  Trade *americanPut2 = new AmericanOption(tradeID++ ,Date(2026, 11, 19), 90.0, BinaryPut);
  myPortfolio.push_back(americanCall1);
  myPortfolio.push_back(americanPut1);
  myPortfolio.push_back(americanCall2);
  myPortfolio.push_back(americanPut2);

  // Printing the portfolio
  std::cout << "Portfolio created with " << myPortfolio.size() << " trades.\n";
  for (const auto &trade : myPortfolio)
  {
    trade->Print();
  }

std::cout << "\nEnd of Task 2\n\n";
/*
when you use new, should use delete.
*/

// task 3, create a pricer and price the portfolio, output the pricing result of each deal.

std::cout << "\nTask 3:\n\n";
// Calculate prices for each trade
double nTimeSteps = 10;
double stockPrice = 100;
double bond_market_price = 100;

std::cout << "Storing Pricing logs in pricing_log.txt\n\n";
// Open a log file to store pricing results
std::ofstream logFile("pricing_log.txt");

for (const auto &trade : myPortfolio) {

    double pv = 0.0;

    if (AmericanOption* americanOption = dynamic_cast<AmericanOption*>(trade)) {
        CRRBinomialTreePricer crrPricer(nTimeSteps);
        std::tuple<double, double, double, double> result = crrPricer.PriceTree(mkt, *americanOption, stockPrice, volCurve, curve);
        pv = std::get<0>(result);

        cout << "Trade ID: " << std::to_string(americanOption->GetTradeID())
            << "\n Trade type: American Option \n Start date: " << mkt.asOf
            << " Maturity date: " << americanOption->GetExpiry()
            << " Present Value : " << pv << std::endl;
    
        logFile << "Trade ID: " << std::to_string(americanOption->GetTradeID())
            << "\n Trade type: American Option \n Start date: " << mkt.asOf
            << " Maturity date: " << americanOption->GetExpiry()
            << " Present Value : " << pv << std::endl;
    }
    
    else if (EuropeanOption* europeanOption = dynamic_cast<EuropeanOption*>(trade)) {
        CRRBinomialTreePricer crrPricer(nTimeSteps);
        std::tuple<double, double, double, double> result = crrPricer.PriceTree(mkt, *europeanOption, stockPrice, volCurve, curve);
        pv = std::get<0>(result);

        cout << "Trade ID: " << std::to_string(europeanOption->GetTradeID())
        << "\n Trade type: European Option \n Start date: " << mkt.asOf
        << " Maturity date: " << europeanOption->GetExpiry()
        << " Present Value : " << pv << std::endl;
 
        logFile << "Trade ID: " << std::to_string(europeanOption->GetTradeID())
            << "\n Trade type: European Option \n Start date: " << mkt.asOf
            << " Maturity date: " << europeanOption->GetExpiry()
            << " Present Value : " << pv << std::endl;
    }

    else if (Swap* swap = dynamic_cast<Swap*>(trade)) {
        double annuity = swap->getAnnuity(curve);
        pv = swap->Payoff(annuity);

        cout << "Trade ID: " << swap->GetTradeID()
        << "\n Trade type: Swap \n Start date: " << swap->GetStart()
        << " Maturity date: " << swap->GetExpiry()
        << " Present Value : " << pv << std::endl;
        
        logFile << "Trade ID: " << swap->GetTradeID()
                << "\n Trade type: Swap \n Start date: " << swap->GetStart()
                << " Maturity date: " << swap->GetExpiry()
                << " Present Value : " << pv << std::endl;
    }

    else if (Bond* bond = dynamic_cast<Bond*>(trade)){
        pv = bond->Payoff(bond_market_price);

        cout << "Trade ID: " << bond->GetTradeID() 
        << "\n Trade type: Bond \n Start date: " << bond->GetStart()
        << " Maturity date: " << bond-> GetExpiry()
        << " Payoff : " << pv << std::endl;

        logFile << "Trade ID: " << bond->GetTradeID() 
        << "\n Trade type: Bond \n Start date: " << bond->GetStart()
        << " Maturity date: " << bond-> GetExpiry()
        << " Payoff : " << pv << std::endl;

    }

    else {
        cout << "Unknown trade type" << std::endl;
        logFile << "Unknown trade type" << std::endl;
    }

}

    // Close the log file
    std::cout << "\nAll pricing logs stored, closing log file\n";
    logFile.close();

    // Clean up
    for (auto trade : myPortfolio) {
        delete trade;
    }

    myPortfolio.clear();    
    std::cout << "\nEnd of Task 3\n\n";


// task 4, analyzing pricing result
//  a) compare CRR binomial tree result for an european option vs Black model
//  b) compare CRR binomial tree result for an american option vs european option

std::cout << "\nTask 4" << std::endl;

std::cout << "\nTask 4a: Compare CRR binomial tree result for an European option vs Black model" << ", nTimeSteps: " << nTimeSteps << std::endl;
vector<Trade *> myPortfolio3;

BlackPricer blackPricer;

// Adding European options
Trade *europeanCall31 = new EuropeanOption(0,Date(2025, 5, 19), 105.0, Call);
Trade *europeanPut31 = new EuropeanOption(0,Date(2026, 6, 20), 100.0, Put);
Trade *europeanCall41 = new EuropeanOption(0,Date(2027, 7, 21), 110.0, BinaryCall);
Trade *europeanPut41 = new EuropeanOption(0,Date(2028, 8, 22), 95.0, BinaryPut);
myPortfolio3.push_back(europeanCall31);
myPortfolio3.push_back(europeanPut31);
myPortfolio3.push_back(europeanCall41);
myPortfolio3.push_back(europeanPut41);


for (const auto &trade : myPortfolio3) {

    if (EuropeanOption* europeanOption = dynamic_cast<EuropeanOption*>(trade)) {
        OptionType optType = europeanOption->getPayoffType();
        double strike = europeanOption->getStrike();
        CRRBinomialTreePricer crrPricer1(nTimeSteps);
        std::tuple<double, double, double, double> result = crrPricer1.PriceTree(mkt, *europeanOption, stockPrice, volCurve, curve);
        double Blackprice = blackPricer.Price(mkt, *europeanOption, stockPrice, curve, volCurve, strike, optType);

        cout<< "Days to Expiry: " << std::get<1>(result);
        cout<< " | Vol: " << std::get<3>(result);
        cout<< " | Interest Rate: " << std::get<2>(result);
        cout<< " | Option Type: " << OptionTypeToString(optType);
        cout<< " | Strike Price: " << strike;
        cout<< " | Stock Price: " << stockPrice;
        cout<< " | European: " << std::get<0>(result);
        cout<< " | Black: " << Blackprice << endl;
    }
}

// Cleanup: Deleting the trades
for (const auto &trade : myPortfolio3) {
    delete trade;
}
myPortfolio3.clear(); 

std::cout << "\nTask 4b: Compare CRR binomial tree result for an European option vs American option" << ", nTimeSteps: " << nTimeSteps << std::endl;

vector<Trade *> myPortfolio2;
vector<double> europeanPrices;
vector<double> americanPrices;
vector<double> days_to_expire;
vector<double> strike_1;
vector<double> vol_1;
vector<double> interest_rate_1;
std::vector<std::string> optionTypeStrings1;

// Adding European options
Trade *europeanCall3 = new EuropeanOption(0,Date(2025, 5, 19), 105.0, Call);
Trade *europeanPut3 = new EuropeanOption(0,Date(2026, 6, 20), 100.0, Put);
Trade *europeanCall4 = new EuropeanOption(0,Date(2027, 7, 21), 110.0, BinaryCall);
Trade *europeanPut4 = new EuropeanOption(0,Date(2028, 8, 22), 95.0, BinaryPut);

myPortfolio2.push_back(europeanCall3);
myPortfolio2.push_back(europeanPut3);
myPortfolio2.push_back(europeanCall4);
myPortfolio2.push_back(europeanPut4);

// Adding American options
Trade *americanCall3 = new AmericanOption(0,Date(2025, 5, 19), 105.0, Call);
Trade *americanPut3 = new AmericanOption(0,Date(2026, 6, 20), 100.0, Put);
Trade *americanCall4 = new AmericanOption(0,Date(2027, 7, 21), 110.0, BinaryCall);
Trade *americanPut4 = new AmericanOption(0,Date(2028, 8, 22), 95.0, BinaryPut);
myPortfolio2.push_back(americanCall3);
myPortfolio2.push_back(americanPut3);
myPortfolio2.push_back(americanCall4);
myPortfolio2.push_back(americanPut4);


for (const auto &trade : myPortfolio2) {
  if (AmericanOption* americanOption = dynamic_cast<AmericanOption*>(trade)) {
      OptionType optType = americanOption->getPayoffType();
      CRRBinomialTreePricer crrPricer1(nTimeSteps);
      std::tuple<double, double, double, double> result = crrPricer1.PriceTree(mkt, *americanOption, stockPrice, volCurve, curve);
      americanPrices.push_back(std::get<0>(result)); // Store American option price
  }

  else if (EuropeanOption* europeanOption = dynamic_cast<EuropeanOption*>(trade)) {
      OptionType optType = europeanOption->getPayoffType();
      double strike = europeanOption->getStrike();
      CRRBinomialTreePricer crrPricer1(nTimeSteps);
      std::tuple<double, double, double, double> result = crrPricer1.PriceTree(mkt, *europeanOption, stockPrice, volCurve, curve);
      optionTypeStrings1.push_back(OptionTypeToString(optType)); // Store option Type
      europeanPrices.push_back(std::get<0>(result)); // Store European option price
      days_to_expire.push_back(std::get<1>(result)); // Store days left
      interest_rate_1.push_back(std::get<2>(result)); //Store interest rate
      vol_1.push_back(std::get<3>(result)); //Store vol
      strike_1.push_back(strike);
  }

}

// Print comparison
for (size_t i = 0; i < europeanPrices.size(); ++i) {
        cout<< "Days to Expiry: " << days_to_expire[i];
        cout<< " | Vol: " << vol_1[i];
        cout<< " | Interest Rate: " << interest_rate_1[i];
        cout<< " | Option Type: " << optionTypeStrings1[i];
        cout<< " | Strike Price: " << strike_1[i];
        cout<< " | Stock Price: " << stockPrice;
        cout<< " | European: " << europeanPrices[i];
        cout<< " | American: " << americanPrices[i] << endl;
}

// Cleanup: Deleting the trades
for (const auto &trade : myPortfolio2) {
    delete trade;
}
myPortfolio2.clear(); 


cout << "\nEnd of Task 4\n\n" << endl;


// final
cout << "\nProject build successfully!" << endl;
return 0;
}
