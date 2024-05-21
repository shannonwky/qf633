#include "Swap.h"

double Swap::Payoff(double annuity) const {
    double payoff = annuity * (tradeRate - marketRate);
    std::cout << "Payout: " << payoff << std::endl;

    return payoff;
}

double Swap::getAnnuity(const RateCurve& curve) const {
    double annuity = 0.0;
    double period = 1.0 / frequency; // period in years
    Date paymentDate = startDate;

    while (paymentDate < maturityDate) {
        paymentDate = paymentDate.addYears(period);
        std::string tenor = std::to_string((paymentDate - startDate)) + "Y";
        double interestRate = curve.getRate(tenor);
        double discountFactor = std::exp(-interestRate * (paymentDate - startDate));
        annuity += notional * discountFactor;
        
        // std::cout << "Evaluation Date: " << paymentDate
        //           << " Tenor: " << tenor
        //           << " Interest Rate: " << interestRate
        //           << " Discount Factor: " << discountFactor
        //           << " Annuity: " << annuity << std::endl;
    }

    return annuity;
}
