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
		for(int i = 100; i<prices.size() - 1; i++) {
			int current = wallet;
			std::cout << "wallet: " << wallet << std::endl;
			if(wallet <= 0) { std::cout << "out of funds stopping backtesting" << std::endl; return; }
			double sma20 = indicators::sma(prices,i,20);
			double sma50 = indicators::sma(prices,i,50);

			double prev20 = indicators::sma(prices, i-1, 20);
			double prev50 = indicators::sma(prices, i-1, 50);

			bool goldencross = prev20 <= prev50 && sma20 > sma50;
			bool deathcross = prev20 >= prev50 && sma20 < sma50;

			if (goldencross && shares == 0) {
        		shares = wallet / prices[i + 1];
        		wallet -= shares * prices[i + 1];
    		}

    		if (deathcross && shares > 0) {
        		wallet += shares * prices[i + 1];
        		shares = 0;
    		}

			double equity = wallet + shares * prices[i];
			double profit = ((equity - current) / current) * 100.0;
			
			std::cout << "equity: " << equity << 
		}
	}
};

#endif
