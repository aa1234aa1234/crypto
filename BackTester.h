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
			if(!wallet) { std::cout << "out of funds stopping backtesting" << std::endl; return; }
			double sma20 = indicators::sma(prices,20);
			double sma50 = indicators::sma(prices,50);

			if(sma20 > sma50) { wallet -= prices[i]; shares += 1; }
			else if(shares && sma20 < sma50) { wallet += prices[i]; shares -= 1; }

			std::cout << "wallet: " << wallet << std::endl << "profit margin: " << current/(wallet-current)/100 << "%" << std::endl;
			std::cout << std::endl;
		}
	}
};

#endif
