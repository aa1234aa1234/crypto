#include "Engine.h"
#include "BackTester.h"
#include "IndicatorsEngine.h"


Engine::Engine() {
	indicatorsEngine = new IndicatorsEngine();
	backTester = new BackTester(indicatorsEngine);

	indicatorsEngine->addIndicator(new indicators::SMA(200));
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
