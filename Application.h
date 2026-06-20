//
// Created by sw_303 on 2026-06-18.
//

#ifndef APPLICATION_H
#define APPLICATION_H
#include "Engine.h"
#include "CsvReader.h"

class Application {
	CsvReader csvReader;
	Engine engine;
public:
    static bool isRunning;
    Application();
    ~Application() {
	};

    void run();
};



#endif //APPLICATION_H
