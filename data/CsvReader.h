#ifndef CSVREADER_H
#define CSVREADER_H
#include "Candle.h"
#include <vector>

class CsvReader {
public:
	CsvReader() {}
	~CsvReader() {}

	void ReadCsv(std::vector<Candle>& history, const char* filepath);
};

#endif
