#include <iostream>
#include <iomanip>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>


typedef std::vector<std::vector<int>> matrix;
typedef std::vector<int> s_row;


void printSep() {
	std::cout << "----------------------------------------------------" << std::endl;
}


void print(const s_row &row, std::string_view _label) {
	std::cout << _label;
	for (auto cell : row) {
		std::cout << std::setw(7) << cell;
	}
	std::cout << std::endl;
}

void print(const matrix &data, std::string_view _label) {
	std::cout << _label << std::endl;
	for (auto row : data)
	{
		for (auto cell : row)
		{
			std::cout << std::setw(7) << cell;
		}
		std::cout << std::endl;
	}
}

matrix Load(std::string_view _FileName)
{
	std::ifstream Fin(_FileName.data());

	if (!Fin.is_open()) {
		std::cout << "Can't open file" << std::endl;
		return matrix();
	}

	matrix cells;
	std::string Line;

	while (std::getline(Fin, Line))
	{
		cells.push_back(s_row());

		size_t Start = 0;
		size_t Pos = Line.find(';');
		while (Pos != std::string::npos)
		{
			//std::cout << Start << ";" << Pos - Start << " : " << Line.substr(Start, Pos - Start) << "    ";
			cells.back().push_back(std::stoi(Line.substr(Start, Pos - Start)));

			Start = Pos + 1;
			Pos = Line.find(';', Start);
		}
		//std::cout << std::endl;
		cells.back().push_back(std::stoi(Line.substr(Start)));
	}
	return cells;
}
