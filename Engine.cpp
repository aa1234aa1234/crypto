#include "Engine.h"

#include "Application.h"
#include "BackTester.h"
#include "IndicatorsEngine.h"
#include "StrategyEngine.h"
#include "curl/CurlClient.h"

Engine::Engine() {
	indicatorsEngine = new IndicatorsEngine();
	backTester = new BackTester(indicatorsEngine);
	strategyEngine = new StrategyEngine(indicatorsEngine);
	curlClient = new CurlClient();
	curlClient->renew_access_code();

	indicatorsEngine->addIndicator(new indicators::SMA(200));
	indicatorsEngine->addIndicator(new indicators::SMA(50));
	indicatorsEngine->addIndicator(new indicators::EMA(200, indicatorsEngine->getIndicator<indicators::SMA>({200})));
	indicatorsEngine->addIndicator(new indicators::EMA(50, indicatorsEngine->getIndicator<indicators::SMA>({50})));
	indicatorsEngine->addIndicator(new indicators::RSI(14));
	initialize();
}

Engine::~Engine() {
	if(backTester) delete backTester;
	if (indicatorsEngine) delete indicatorsEngine;
	if (strategyEngine) delete strategyEngine;
}

std::vector<Candle>& Engine::getCandles() { return candles; }

void Engine::initialize()
{
	std::vector<double> prices;
	for(auto& p : candles) {
		indicatorsEngine->update(p);
		prices.push_back(p.close);
	}
}


void Engine::run() {
	Candle candle = curlClient->getCurrentPrice("005930").to_candle();
	indicatorsEngine->update(candle);
	strategyEngine->run(candle);
	std::cout << candle.close << std::endl;
	//backTester->run(prices, candles);
}
