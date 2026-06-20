#pragma once
#include <vector>
#include <cmath>

namespace indicators {
	inline double sma(const std::vector<double>& prices, int period) {
		if(prices.size() < period) return NAN;

		double sum = 0.0;
		for(int i = prices.size() - period; i<prices.size(); i++) sum += prices[i];

		return sum/period;
	}

	
}
