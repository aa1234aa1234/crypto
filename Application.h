//
// Created by sw_303 on 2026-06-18.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include "Engine.h"
#include "CsvReader.h"
#include "curl/curl_http.h"

class Application {
	CsvReader csvReader;
	Engine* engine;
public:
    static bool isRunning;
    Application();
    ~Application() {
    	delete engine;
    	curl_http::clear_curl();
	};

    void run();
};



#endif //APPLICATION_H
