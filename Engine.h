#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include "Candle.h"

class BackTester;
class IndicatorsEngine;
class StrategyEngine;
class CurlClient;
class graph_generator;

class Engine {
	std::vector<Candle> candles;
	BackTester* backTester;
	IndicatorsEngine* indicatorsEngine;
	StrategyEngine* strategyEngine;
	CurlClient* curlClient;
	graph_generator* graphGenerator;
public:
	Engine();
	~Engine();

	void run();
	void initialize();
	std::vector<Candle>& getCandles();

};

#endif
