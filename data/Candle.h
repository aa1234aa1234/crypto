#ifndef CANDLE_H
#define CANDLE_H
#include <string>

struct Candle {
	std::string date;
	double close;
	double high;
	double low;
	double open;
	long volume;
};

#endif
