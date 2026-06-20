#include "Engine.h"
#include "BackTester.h"


Engine::Engine() {
	backTester = new BackTester();
}

Engine::~Engine() {
	if(backTester) delete backTester;
}

std::vector<Candle>& Engine::getCandles() { return candles; }

void Engine::run() {
	std::vector<double> prices;
	for(auto& p : candles) {
		prices.push_back(p.close);
	}
	backTester->run(prices, candles);
}
