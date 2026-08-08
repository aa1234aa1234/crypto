#include "Engine.h"
#include "BackTester.h"
#include "IndicatorsEngine.h"
#include "StrategyEngine.h"


Engine::Engine() {
	indicatorsEngine = new IndicatorsEngine();
	backTester = new BackTester(indicatorsEngine);
	strategyEngine = new StrategyEngine(indicatorsEngine);

	indicatorsEngine->addIndicator(new indicators::SMA(200));
	indicatorsEngine->addIndicator(new indicators::SMA(50));
	indicatorsEngine->addIndicator(new indicators::EMA(200, indicatorsEngine->getIndicator<indicators::SMA>({200})));
	indicatorsEngine->addIndicator(new indicators::EMA(50, indicatorsEngine->getIndicator<indicators::SMA>({50})));
	indicatorsEngine->addIndicator(new indicators::RSI(14));
}

Engine::~Engine() {
	if(backTester) delete backTester;
	if (indicatorsEngine) delete indicatorsEngine;
	if (strategyEngine) delete strategyEngine;
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
