#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include "Candle.h"

class BackTester;
class IndicatorsEngine;
class StrategyEngine;

class Engine {
	std::vector<Candle> candles;
	BackTester* backTester;
	IndicatorsEngine* indicatorsEngine;
	StrategyEngine* strategyEngine;
public:
	Engine();
	~Engine();

	void run();
	std::vector<Candle>& getCandles();

};

#endif
