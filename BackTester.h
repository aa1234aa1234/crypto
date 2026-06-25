#pragma once
#ifndef BACKTESTER_H
#define BACKTESTER_H
#include "Indicators.hpp"
#include <iostream>
#include "Candle.h"

class BackTester {
	double wallet = 100000;
	int shares = 0;
public:
	BackTester() {}
	~BackTester() {}

	void run(std::vector<double>& prices, std::vector<Candle>& candles) {
		int buys = 0, sells = 0;

		for(int i = 200; i<prices.size() - 1; i++) {
			std::cout << "date: " << candles[i].date << std::endl;
			std::cout << "wallet: " << wallet << std::endl;
			if(wallet <= 0) { std::cout << "out of funds stopping backtesting" << std::endl; return; }
			double ema50 = indicators::ema(prices, i, 50);
			double ema200 = indicators::ema(prices, i, 200);

			double sma20 = indicators::sma(prices,i,20);
			double sma50 = indicators::sma(prices,i,50);

			double prev20 = indicators::sma(prices, i-1, 20);
			double prev50 = indicators::sma(prices, i-1, 50);

			bool goldencross = prev20 <= prev50 && sma20 > sma50;
			bool deathcross = prev20 >= prev50 && sma20 < sma50;

			bool uptrend = prices[i] > ema50;

			if (goldencross && prices[i] > ema200 && shares == 0) {
        		shares = wallet / prices[i + 1];
        		wallet -= shares * prices[i + 1];
				buys += 1;
    		}

    		if ((deathcross) && shares > 0) {
        		wallet += shares * prices[i + 1];
        		shares = 0;
    			sells++;
    		}

			double equity = wallet + shares * prices[i];
			double profit = ((equity - 100000) / 100000) * 100.0;
			
			std::cout << "equity: " << equity << std::endl << "profit margin: " << profit << "%" << std::endl;
			std::cout << std::endl;
		}
		std::cout << "trades: " << buys + sells << std::endl << "buys: " << buys << std::endl << "sells: " << sells << std::endl;
		std::cout << "Final shares: " << shares << '\n';
		std::cout << "Last price: " << prices.back() << '\n';
		std::cout << "Share value: "
				  << shares * prices.back()
				  << '\n';
		double finalEquity = wallet + shares * prices.back();
		double finalReturn = (finalEquity - 100000.0) / 100000.0 * 100.0;
		std::cout << "Final Return: " << finalReturn << std::endl;
	}
};

#endif
