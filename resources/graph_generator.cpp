//
// Created by sw506 on 2026-11-09.
//

#include "graph_generator.h"
#include "IndicatorsEngine.h"
#include <fstream>

void graph_generator::generate_script() {
    std::ofstream outfile("graph.py", std::ios::out);
    outfile << base_script;
    outfile.close();
}
