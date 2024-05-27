#pragma once
#include<string>
#include "Date.h"

#include "Trade.h"
#include "Date.h"
#include "Market.h"
#include <cmath>
#include <vector>

using namespace std;

class Trade {
public:
    Trade(){};
    //int GetTradeID() const { return tradeID; } // NEW
    Trade(int _ID, const string& _type,Date _tradeDate): tradeID(_ID), tradeType(_type), tradeDate(_tradeDate) {};

    inline int GetTradeID() const { return tradeID; }
    inline string getType(){ return tradeType; };
    virtual double Payoff(double marketPrice) const = 0;
    virtual const Date& GetExpiry() const
    { 
    return expiryDate; 
    }
    virtual const Date& GetStart() const
    { 
    return tradeDate; 
    }

    // virtual Date GetRate(const RateCurve &curve) const
    // {
    // std::string tenor = std::to_string(expiryDate - tradeDate) + "Y";
    // //double rate = tenor;
    // //double rate = curve.getRate(tenor)*0.01;
    // //return rate;
    // return expiryDate, tradeDate;
    // }



    virtual void Print() const = 0;
    // virtual void Print() const {
    //     cout << "Trade Type: " << tradeType << endl;
    //     cout << "Trade Date: " << tradeDate << endl;
    //     cout << "Expiry Date: " << expiryDate << endl;
    // }


    //virtual Date GetExpiry() const = 0; //Added new
    virtual ~Trade(){};

// private:
//     int tradeID; //NEW

protected:
    int tradeID; //new
    string tradeType;
    Date tradeDate;
    Date expiryDate;
};