#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "timer.h"

//grupa nr 2
//[SPD]_pt13_gr2_T3

struct pwd {

	int p = NULL, w = NULL, d = NULL;
	int index = NULL;

	bool operator == (const pwd &val) const {

		if (p == val.p && w == val.w && d == val.d && index == val.index) {

			return true;
		}

		return false;
	}
};

struct sortD {

	bool operator () (const pwd &a, const pwd &b) const {
		return a.d < b.d;
	}
};

struct sortW {

	bool operator () (const pwd &a, const pwd &b) const {
		return a.w < b.w;
	}
};

const int calcWiti(const std::vector <pwd> &container) {

	int T = 0, C = 0, witiSum = 0;
	std::vector <int> witiElement;

	for (auto &i : container)
	{
		C += i.p;
		T = max(C - i.d, 0);
		witiElement.push_back(i.w*T);
	}

	for (auto &i : witiElement) {

		witiSum += i;
	}

	return witiSum;
}

void coutOrder(const std::vector <pwd> &container) {

	for (auto &i : container) {

		std::cout << i.index + 1 << " ";
	}
	std::cout << std::endl;
}

void coutPWD(const std::vector <pwd> &container) {

	std::cout << container.size() << std::endl;

	for (auto &i : container) {

		std::cout << i.p << " " << i.w << " " << i.d << std::endl;
	}
}

bool loadContainer(const std::string filename, std::vector <pwd> &container) {

	std::fstream streamRead;
	streamRead.open(filename, std::ios::in);

	if (!streamRead.is_open()) {

		std::cerr << "Error opening " + filename + " to read" << std::endl;
		return false;
	}
	else {
		int size = NULL;
		pwd buff;

		streamRead >> size;

		for (int i = 0; i < size; ++i) {
			buff.index = i;
			streamRead >> buff.p >> buff.w >> buff.d;
			container.push_back(buff);
		}
	}

	return true;
}

void sortLessD(std::vector <pwd> &container) {

	std::sort(container.begin(), container.end(), sortD());
}

void sortLessW(std::vector <pwd> &container) {

	std::sort(container.begin(), container.end(), sortW());
}

bool loadFilenames(const std::string filename, std::vector <std::string> &container) {

	std::fstream streamRead;
	streamRead.open(filename, std::ios::in);

	if (!streamRead.is_open()) {

		std::cerr << "Error opening file to read" << std::endl;
		return false;
	}
	else {
		std::string buff;

		while (!streamRead.eof())
		{
			std::getline(streamRead, buff);
			container.push_back(buff);
		}
	}

	return true;
}

void insertSort(std::vector <pwd> &container) {

	std::vector<std::vector <pwd>> table;
	std::vector<pwd> toRememeber;

	toRememeber.push_back(container[0]);
	container.erase(container.begin());
	table.clear();

	int ind = NULL;
	while (!container.empty()) {

		for (int i = 0; i < toRememeber.size() + 1; ++i) {

			table.push_back(toRememeber);
		}

		for (int i = 0; i < toRememeber.size() + 1; ++i) {

			table[i].insert(table[i].begin() + i, container[0]);

		}
		container.erase(container.begin());

		int value = INT_MAX;
		for (int i = 0; i < table.size(); ++i) {

			int temp = calcWiti(table[i]);

			if (temp <= value) {
				value = temp;
				ind = i;
			}
		}

		toRememeber = table[ind];
		table.clear();
	}
	container = toRememeber;
}

struct dynamicHelper
{
	int WiTi = NULL;
	std::vector <int> order;
};

void sortDynamic(std::vector <pwd> &container)
{
	unsigned long size = pow(2, container.size());
	std::vector <dynamicHelper> solution; //rozwiazania witi
	dynamicHelper zero;
	zero.WiTi = 0; zero.order.push_back(-1);
	solution.push_back(zero);

	//i to element wektora rozwiazan 010=2, 101=5
	for (unsigned long i = 1; i < size; ++i)
	{
		dynamicHelper buffer;
		unsigned long temp = i;
		int position = 0;

		//sprawdzenie poprzednich wartosci
		while (temp != 1)
		{
			temp >>= 1;
			++position;
		}
		temp <<= position;
		--temp;
		temp &= i; //wszstkie poprzednie witi

		//obliczenia, jezeli tylko jeden wyraz 0001 lub 0100
		if (temp == 0)
		{
			buffer.WiTi = max(container[position].p - container[position].d, 0)*container[position].w;
			buffer.order.push_back(position);
		}
		else
		{
			//suma p1+p2+p5 itp.
			int sum = 0;
			unsigned long pointer = 1;

			//przesuwa sie po kontenerze
			for (int j = 0; j <= position; ++j)
			{
				if ((pointer&i) == pointer)
				{
					sum += container[j].p;
				}
				pointer <<= 1;
			}
			//szukaj minimum z maximow - do edycji
			int minimum;
			pointer = 1;
			for (int j = 0; j <= position; ++j) //przesuwa sie po kontenerze
			{
				if ((pointer&i) == pointer)
				{
					minimum = max(sum - container[j].d, 0)*container[j].w + solution[(i^pointer)].WiTi;

					if (buffer.WiTi == NULL)
					{
						buffer.WiTi = minimum;
						buffer.order = solution[(i^pointer)].order;
						buffer.order.push_back(j);
					}
					//zeby nie porownywal z nulem w poprzednim ifie
					else if (minimum <= buffer.WiTi) 
					{
						buffer.WiTi = minimum;
						buffer.order = solution[(i^pointer)].order;
						buffer.order.push_back(j);
					}
				}
				pointer <<= 1;
			}
		}
		solution.push_back(buffer);
	}

	std::vector <pwd> temp;

	for (int i = 0; i < container.size(); ++i)
	{
		temp.push_back(container[solution[size - 1].order[i]]);
	}

	container = temp;
}

void calcEverything(std::vector <int> &WiTiTi, std::vector <std::string> &filenames, std::vector <int> &sizes, std::vector <double> &times)
{
	std::vector <pwd> container;
	timer stopwatch;
	const int algorithmCount = 4;
	for (std::string i : filenames)
	{
		loadContainer("witi/" + i, container);
		sizes.push_back(container.size());
		std::vector <std::vector <pwd>> vectorOfContainers;
		for (int i = 0; i < algorithmCount; ++i) {
			vectorOfContainers.push_back(container);
		}

		stopwatch.start();
		sortLessD(vectorOfContainers[1]);
		times.push_back(stopwatch.end());
		stopwatch.start();
		insertSort(vectorOfContainers[2]);
		times.push_back(stopwatch.end());
		stopwatch.start();
		sortDynamic(vectorOfContainers[3]);
		times.push_back(stopwatch.end());

		for (int i = 0; i < algorithmCount; ++i) {
			WiTiTi.push_back(calcWiti(vectorOfContainers[i]));
		}
		container.clear();
	}
}

void printResultsToFile(std::vector <int> &WiTiTi, std::vector <std::string> &filenames, std::vector <int> &sizes, std::vector <double> &times, std::vector <double> &PRD)
{
	std::fstream streamWriteResults, streamWriteTimes, streamWritePRD;
	streamWriteResults.open("results.txt", std::ios::out);
	streamWriteTimes.open("times.txt", std::ios::out);
	streamWritePRD.open("PRDs.txt", std::ios::out);
	std::vector <std::string> resultsS, timesS, PRDS;

	if (!streamWriteResults.is_open() || !streamWriteTimes.is_open() || !streamWritePRD.is_open()) {

		std::cerr << "Error opening file" << std::endl;
		return;
	}
	else {

		resultsS.push_back("Nazwa instancji &");
		resultsS.push_back("Rozmiar &");
		resultsS.push_back("12345 &");
		resultsS.push_back("sortD &");
		resultsS.push_back("insert &");
		resultsS.push_back("alg prog dyn\\\\\n");

		timesS.push_back("Nazwa instancji &");
		timesS.push_back("Rozmiar &");
		timesS.push_back("sortD &");
		timesS.push_back("insert &");
		timesS.push_back("alg prog dyn\\\\\n");

		timesS.push_back("Nazwa instancji &");
		timesS.push_back("Rozmiar &");
		timesS.push_back("sortD &");
		timesS.push_back("insert &");
		timesS.push_back("alg prog dyn\\\\\n");

		const int algorithmsCount = 4;
		const int algorithmsCount2 = algorithmsCount - 1;

		for (int i = 0, k = 0; i < WiTiTi.size(); ++i)
		{
			if (i % algorithmsCount == 0)
			{
				resultsS.push_back(filenames[k] + " &");
				resultsS.push_back(std::to_string(sizes[k]) + " &");
				++k;
			}
			if ((i + 1) % algorithmsCount == 0) {

				resultsS.push_back(std::to_string(WiTiTi[i]) + " \\\\\n");
			}
			else {
				resultsS.push_back(std::to_string(WiTiTi[i]) + " &");
			}
		}

		for (auto &i : resultsS)
		{
			streamWriteResults << i;
		}

		for (int i = 0, k = 0; i < times.size(); ++i)
		{
			if (i % algorithmsCount2 == 0)
			{
				timesS.push_back(filenames[k] + " &");
				timesS.push_back(std::to_string(sizes[k]) + " &");
				PRDS.push_back(filenames[k] + " &");
				PRDS.push_back(std::to_string(sizes[k]) + " &");
				++k;
			}
			if ((i + 1) % algorithmsCount2 == 0) {

				timesS.push_back(std::to_string(times[i]) + " \\\\\n");
				PRDS.push_back(std::to_string(PRD[i]) + " \\\\\n");
			}
			else {
				timesS.push_back(std::to_string(times[i]) + " &");
				PRDS.push_back(std::to_string(PRD[i]) + " &");
			}
		}

		for (auto &i : timesS)
		{
			streamWriteTimes << i;
		}

		for (auto &i : PRDS)
		{
			streamWritePRD << i;
		}
	}
}
void calcPRD(std::vector <int> &WiTiTi, std::vector <double> &PRD)
{
	int WiTiTiRef = NULL;
	for (int i = 0; i < WiTiTi.size(); ++i)
	{
		if (i % 3 == 0) {
			WiTiTiRef = WiTiTi[i + 2];
		}

		PRD.push_back(100 * (WiTiTi[i] - WiTiTiRef) / WiTiTiRef);
	}
}

int main() {

	std::vector <pwd> container;
	std::vector <int> WiTiTi, sizes;
	std::vector <std::string> filenames;
	std::vector <double> times;
	std::vector <double> PRD;


	loadFilenames("witi/filenames.txt", filenames);
	calcEverything(WiTiTi, filenames, sizes, times);
	calcPRD(WiTiTi, PRD);
	printResultsToFile(WiTiTi, filenames, sizes, times, PRD);

	system("pause");
	return 0;
}