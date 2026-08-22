#ifndef CANDLE_H
#define CANDLE_H
#include <string>
#include <cmath>

struct Candle {
	std::string date;
	double close;
	double high;
	double low;
	double open;
	long volume;

	void normalize()
	{
		close = close < 0 ? close * -1 : close;
	}
};

#endif
