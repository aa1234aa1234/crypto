#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include "Candle.h"

class BackTester;
class IndicatorsEngine;

class Engine {
	std::vector<Candle> candles;
	BackTester* backTester;
	IndicatorsEngine* indicatorsEngine;
public:
	Engine();
	~Engine();

	void run();
	std::vector<Candle>& getCandles();

};

#endif
