#pragma once
#include <vector>
#include <cmath>

namespace indicators {
	inline double sma(const std::vector<double>& prices, int index, int period) {
		if(prices.size() < period) return NAN;

		double sum = 0.0;
		for(int i = (index > prices.size() ? prices.size() : index) - period + 1; i<=index; i++) sum += prices[i];

		return sum/period;
	}

	inline double ema(const std::vector<double>& prices, int index, int period)
	{
		double alpha = 2. / (period + 1.0);
		double ema = indicators::sma(prices, period - 1, period);

		for (int i = period; i<=index; i++)
		{
			ema = alpha * prices[i] + (1.0 - alpha) * ema;
		}

		return ema;
	}
}


//Trend:
//50 EMA > 200 EMA

//Momentum:
//RSI > 50

//Strength:
//ADX > 25

//Risk:
//ATR-based stop loss

//SMA crossover
// EMA crossover
// RSI mean reversion
// Bollinger Bands
// MACD
// breakout systems
// combinations of indicators
