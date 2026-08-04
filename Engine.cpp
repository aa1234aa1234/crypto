#include "Engine.h"
#include "BackTester.h"
#include "IndicatorsEngine.h"


Engine::Engine() {
	indicatorsEngine = new IndicatorsEngine();
	backTester = new BackTester(indicatorsEngine);

	indicatorsEngine->addIndicator(new indicators::SMA(200));
	indicatorsEngine->addIndicator(new indicators::SMA(50));
	indicatorsEngine->addIndicator(new indicators::EMA(50, indicatorsEngine->getIndicator<indicators::SMA>({50})));
	indicatorsEngine->addIndicator(new indicators::RSI(14));
}

Engine::~Engine() {
	if(backTester) delete backTester;
	if (indicatorsEngine) delete indicatorsEngine;
}

std::vector<Candle>& Engine::getCandles() { return candles; }

void Engine::run() {
	std::vector<double> prices;
	for(auto& p : candles) {
		indicatorsEngine->update(p);
		prices.push_back(p.close);
	}
	backTester->run(prices, candles);
}
