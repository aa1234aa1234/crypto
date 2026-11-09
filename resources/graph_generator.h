//
// Created by sw506 on 2026-11-09.
//

#ifndef CRYPTO_GRAPH_GENERATOR_H
#define CRYPTO_GRAPH_GENERATOR_H

class IndicatorsEngine;

class graph_generator {
    const char* base_script = "import pandas as pd\n"
                              "import matplotlib.pyplot as plt\n";
    const char* transaction_log = "for i in range(len(buylogx):\n"
                                  "\tplt.text(buylogx[i], buylogy[i], 'buy', fontsize=10, ha='center')\n"
                                  "for i in range(selllogx):\n"
                                  "\tplt.text(selllogx[i], selllogy[i], 'sell', fontsize=10, ha='center')\n";

    IndicatorsEngine* indicatorsengine;

public:
    graph_generator(IndicatorsEngine* indicatorsengine) : indicatorsengine(indicatorsengine) {};
    ~graph_generator() {};

    void generate_script();
};


#endif //CRYPTO_GRAPH_GENERATOR_H
