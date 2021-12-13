#include <iostream>
#include "CsvReader.h"
#include <numeric>

int GetMaxId(const s_row& delt, const s_row& counts);
int GetMinValueCol(const matrix& inc, int col_id, const s_row& storage);
int GetMinValueRow(const matrix& inc, int row_id, const s_row& req);
int main() {
	setlocale(LC_ALL, "RUS");

	//Загрузка CSV файла, в котором хранятся значения
	auto cells = Load("./Input.csv");


	//Вывод начальной таблицы
	print(cells, "Начальная таблица");
	printSep();


	//Вывод значений стоимости через циклы
	matrix incosts;
	for (int i = 0; i < cells.size() - 1; i++)
	{
		incosts.push_back(s_row());
		for (int j = 0; j < cells[i].size() - 1; j++)
		{
			incosts.back().push_back(cells[i][j]);
		}
	}


	print(incosts, "Вывод значений стоимости");

	s_row storage;
	s_row req;


	//Вывод значений "Запасов" и "Необходимо" 
	for (int i = 0; i < cells.size() - 1; i++) {
		storage.push_back(cells[i].back());
	}

	for (int j = 0; j < cells.back().size() - 1; j++) {
		req.push_back(cells.back()[j]);
	}


	printSep();
	print(storage, "Все запасы");
	std::cout << std::accumulate(storage.begin(), storage.end(), 0) << std::endl;

	printSep();
	print(req, "Сколько необходимо");
	std::cout << std::accumulate(req.begin(), req.end(), 0) << std::endl;


	//Сравнение "Запасов" > "Необходимо"
	if (std::accumulate(storage.begin(), storage.end(), 0) < std::accumulate(req.begin(), req.end(), 0)) {
		std::cout << "Задача не подходит для данного метода, потому что значение \"необходимо\" больше значения \"запасов\"!" << std::endl;
		return 0;
	}

	//Расчитывание дельт
	s_row del_storage(storage.size(), 0);
	s_row del_req(req.size(), 0);

	printSep();
	matrix result(incosts.size(), s_row(incosts[0].size(), 0));
	


	while (std::accumulate(req.begin(), req.end(), 0))
	{
		/*std::cout << "Мы внутри" << std::endl;*/
		int min1 = 0, min2 = 0;

		for (int i = 0; i < incosts.size(); i++) {
			//std::cout << "Мы внутри V2.0" << std::endl;
			int counter = 0;
			for (int j = 0; j < incosts[i].size(); j++) {
				if (req[j] != 0) {
					if (counter++ == 0) {
						min1 = j;
					}
					else {
						min2 = j;
						break;
					}
				}
			}

			//std::cout << "s " << min1 << " " << min2 << std::endl;

			if (counter != 2) {
				del_storage[i] = 0;
				continue;
			}

			for (int j = min2 + 1; j < incosts[i].size(); j++) {
				if (req[j] != 0 && incosts[i][j] < incosts[i][min2]) {
					min2 = j;
				}
			}

			for (int j = min1 + 1; j < incosts[i].size(); j++) {
				if (req[j] != 0 && incosts[i][j] < incosts[i][min1] && j != min2) {
					min1 = j;
				}
			}
			//std::cout << "e " << min1 << " " << min2 << std::endl;
			del_storage[i] = abs(incosts[i][min2] - incosts[i][min1]);
		}

	
		for (int j = 0; j < incosts[0].size(); j++) {
			//std::cout << "Мы внутри V3.0" << std::endl;
			int counter = 0;
			for (int i = 0; i < incosts.size(); i++) {
				//std::cout << "Мы внутри V3.1" << std::endl;
				if (storage[i] != 0) {
					if (counter++ == 0) {
						min1 = i;
					}
					else {
						min2 = i;
						break;
					}
				}
			}

			if (counter != 2) {
				del_req[j] = 0;
				continue;
			}

			for (int i = min2 + 1; i < incosts.size(); i++) {
				//std::cout << "Мы внутри V3.2" << std::endl;
				if (storage[i] != 0 && incosts[i][j] < incosts[min2][j]) {
					min2 = i;
				}
			}

			for (int i = min1 + 1; i < incosts.size(); i++) {
				//std::cout << "Мы внутри V3.3" << std::endl;
				if (storage[i] != 0 && incosts[i][j] < incosts[min1][j] && i != min2) {
					min1 = i;
				}
			}
			del_req[j] = abs(incosts[min2][j] - incosts[min1][j]);
		}

		printSep();
		print(del_storage, "Дельта запасов");
		print(del_req, "Дельта необходимого");

		//Получение максимальных значений в дельтах
		int id_req = GetMaxId(del_req, req);
		int id_storage = GetMaxId(del_storage, storage);

		if (id_req > id_storage) 
		{
			id_storage = GetMinValueCol(incosts, id_req, storage);
		}
		else 
		{
			id_req = GetMinValueRow(incosts, id_storage, req);
		}

		if (storage[id_storage] > req[id_req])
		{
			storage[id_storage] -= req[id_req];
			result[id_storage][id_req] += req[id_req];
			req[id_req] = 0;
		}
		else 
		{
			req[id_req] -= storage[id_storage];
			result[id_storage][id_req] += storage[id_storage];
			storage[id_storage] = 0;
		}

		print(result, "Результирующая матрица");
	}

	int sum = 0;

	for (int i = 0; i < incosts.size(); i++)
	{
		for (int j = 0; j < incosts[0].size(); j++)
		{
			sum += incosts[i][j] * result[i][j];
		}
	}

	std::cout << "Итоговая сумма: " << sum << std::endl;

	std::ofstream fout("./Output.csv");

	for (int i = 0; i < result.size(); i++)
	{
		for (int j = 0; j < result[0].size(); j++)
		{
			fout << result[i][j] << ";";
		}
		fout << std::endl;
	}
	fout << "Итоговая сумма: " << ";" << sum;


	return 0;
}

int GetMaxId(const s_row& delt, const s_row& counts) 
{
	//находим первый элемент, который учитывается
	int max_id;
	for (int j = 0; j < counts.size(); j++)
	{
		if (counts[j] != 0)
		{
			max_id = j;
			break;
		}
	}

	//нахождение максимального элемента из тех, что учитывается
	for (int j = max_id + 1; j < counts.size(); j++)
	{
		if (delt[j] > delt[max_id] && counts[j] != 0)
		{
			max_id = j;
		}
	}
	return max_id;
}

//Нахождение минимального значения в столбце
int GetMinValueCol(const matrix& inc, int col_id, const s_row& storage)
{
	int min_id;
	for (int i = 0; i < storage.size(); i++)
	{
		if (storage[i] != 0)
		{
			min_id = i;
			break;
		}
	}

	//нахождение максимального элемента из тех, что учитывается
	for (int i = min_id + 1; i < storage.size(); i++)
	{
		if (inc[i][col_id] < inc[min_id][col_id] && storage[i] != 0)
		{
			min_id = i;
		}
	}
	return min_id;
}

int GetMinValueRow(const matrix& inc, int row_id, const s_row& req)
{
	int min_id;
	for (int j = 0; j < req.size(); j++)
	{
		if (req[j] != 0)
		{
			min_id = j;
			break;
		}
	}

	//нахождение максимального элемента из тех, что учитывается
	for (int j = min_id + 1; j < req.size(); j++)
	{
		if (inc[row_id][j] < inc[row_id][min_id] && req[j] != 0)
		{
			min_id = j;
		}
	}
	return min_id;
}
