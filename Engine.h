#ifndef ENGINE_H
#define ENGINE_H
#include <vector>
#include "Candle.h"

class BackTester;

class Engine {
	std::vector<Candle> candles;
	BackTester* backTester;
public:
	Engine();
	~Engine();

	void run();
	std::vector<Candle>& getCandles();

};

#endif
