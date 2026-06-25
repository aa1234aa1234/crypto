#include "CsvReader.h"
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

void CsvReader::ReadCsv(std::vector<Candle>& history, const char* filepath) {
	history.clear();
	std::ifstream file(filepath);

	if(!file.is_open()) { std::cerr << "couldnt open file " << filepath << std::endl; return; }
	std::string line;
	while(std::getline(file, line)) {
		std::stringstream ss(line);
		std::string word;
		std::vector<std::string> row;

		while(std::getline(ss, word, ',')) {
			row.push_back(word);
		}
		if(row[0][0] != '2') continue;

		history.push_back(Candle{row[0], std::stod(row[1]), std::stod(row[2]), std::stod(row[3]), std::stod(row[4]), std::stol(row[5])});
	}

	file.close();

}
