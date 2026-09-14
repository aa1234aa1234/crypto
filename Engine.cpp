#include "Engine.h"

#include "Application.h"
#include "BackTester.h"
#include "IndicatorsEngine.h"
#include "StrategyEngine.h"
#include "curl/CurlClient.h"
#include <regex>
#include <algorithm>


Engine::Engine() {
	indicatorsEngine = new IndicatorsEngine();
	backTester = new BackTester(indicatorsEngine);
	strategyEngine = new StrategyEngine(indicatorsEngine);
	curlClient = new CurlClient();
	curlClient->renew_access_code();

	indicatorsEngine->addIndicator(new indicators::SMA(200));
	indicatorsEngine->addIndicator(new indicators::SMA(50));
	indicatorsEngine->addIndicator(new indicators::SMA(21));
	indicatorsEngine->addIndicator(new indicators::SMA(9));
	indicatorsEngine->addIndicator(new indicators::EMA(200, indicatorsEngine->getIndicator<indicators::SMA>({200})));
	indicatorsEngine->addIndicator(new indicators::EMA(50, indicatorsEngine->getIndicator<indicators::SMA>({50})));
	indicatorsEngine->addIndicator(new indicators::EMA(21, indicatorsEngine->getIndicator<indicators::SMA>({21})));
	indicatorsEngine->addIndicator(new indicators::EMA(9, indicatorsEngine->getIndicator<indicators::SMA>({9})));
	indicatorsEngine->addIndicator(new indicators::RSI(14));
	//indicatorsEngine->addIndicator(new indicators::RC<LOW>(curlClient->getCurrentPrice("AAPL", "ND").low_price));
	//indicatorsEngine->addIndicator(new indicators::RC<HIGH>(curlClient->getCurrentPrice("AAPL", "ND").low_price));
	indicatorsEngine->addIndicator(new indicators::RC<LOW>(258000));
	indicatorsEngine->addIndicator(new indicators::RC<HIGH>(258000));
	indicatorsEngine->addIndicator(new indicators::ATR(14,indicatorsEngine->getIndicator<indicators::RC<LOW>>({})));
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
	static double prev_price = 0;
	static std::vector<Candle>::iterator it;
	static std::vector<Candle> prices;
	if (prices.size() == 0)
	{
		CsvReader reader;
		reader.ReadCsv(prices, "../aa.csv");
		it = prices.begin();
		//std::reverse(prices.begin(), prices.end());
	}
	//auto ca = curlClient->getCurrentPrice("005930");
	//Candle candle = curlClient->getCurrentPrice("AAPL", "ND").to_candle();
	Candle candle = *it; ++it;
	candle.normalize();
	if (candle.close == prev_price) { return; }
	indicatorsEngine->update(candle);
	strategyEngine->run(candle);
	prev_price = candle.close;
	std::cout << candle.close << std::endl;
	if (it == prices.end()-1)
		Application::isRunning = false;
	//backTester->run(prices, candles);
}
