#include "Engine.h"
#include "BackTester.h"
#include "IndicatorsEngine.h"


Engine::Engine() {
	indicatorsEngine = new IndicatorsEngine();
	backTester = new BackTester(indicatorsEngine);

	indicatorsEngine->addIndicator(new indicators::SMA(200));
	indicatorsEngine->addIndicator(new indicators::EMA(200, indicatorsEngine->getIndicator<indicators::SMA>({200})));
	indicatorsEngine->addIndicator(new indicators::SMA(50));
	indicatorsEngine->addIndicator(new indicators::SMA(10));
	indicatorsEngine->addIndicator(new indicators::SMA(5));
	indicatorsEngine->addIndicator(new indicators::SMA(2));

	indicatorsEngine->update(Candle{"ewa", 100,100,100,100,100});
}

Engine::~Engine() {
	if(backTester) delete backTester;
	if (indicatorsEngine) delete indicatorsEngine;
}

std::vector<Candle>& Engine::getCandles() { return candles; }

void Engine::run() {
	std::vector<double> prices;
	for(auto& p : candles) {
		prices.push_back(p.close);
	}
	backTester->run(prices, candles);
}
