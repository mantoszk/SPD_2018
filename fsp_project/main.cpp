#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "timer.h"
#include <ctime>

//grupa nr 2
//[SPD]_pt13_gr2_T3

struct task
{
	unsigned int value, index;
};

struct machine
{
	std::vector <task> p;
	std::vector <unsigned int> s, c;
	unsigned int index;
};

unsigned int calculate(std::vector <machine> &container)
{
	const unsigned int machines = container.size(), tasks = container[0].p.size();

	for (auto &i : container)
	{
		i.c.clear();
		i.s.clear();
	}

	for (unsigned int i = 0; i < machines; ++i)
	{
		for (unsigned int j = 0; j < tasks; ++j)
		{
			container[i].s.push_back(0);
			container[i].c.push_back(0);
		}
	}

	unsigned int startTime = NULL, endTime = NULL;

	for (unsigned int i = 0; i < tasks; ++i)
	{
		if (i == 0)
		{
			startTime = 0;
		}
		else
		{
			startTime = container[0].c[i - 1];
		}
		endTime = startTime + container[0].p[i].value;

		container[0].s[i] = startTime;
		container[0].c[i] = endTime;
		//std::cout << "StartTime: " << container[0].s[i] << " EndTime: " << container[0].c[i] << std::endl;;

		for (unsigned int j = 1; j < machines; ++j)
		{

			if (i == 0)
			{
				startTime = endTime;
			}
			else
			{
				startTime = max(container[j].c[i - 1], container[j - 1].c[i]);
			}
			endTime = startTime + container[j].p[i].value;

			container[j].s[i] = startTime;
			container[j].c[i] = endTime;
			//std::cout << "StartTime: " << container[j].s[i] << " EndTime: " << container[j].c[i] << std::endl;
		}
	}
	return container[machines - 1].c[tasks - 1];
}

void loadContainer(const std::string filename, std::vector <machine> &container, std::vector <unsigned int> &Vtask, std::vector <unsigned int> &Vmachine)
{
	std::fstream streamRead;
	streamRead.open(filename, std::ios::in);

	if (!streamRead.is_open())
	{
		std::cerr << "Error opening " + filename + " to read" << std::endl;
		return;
	}
	else
	{
		int taskCount = 0, machineCount = 0;
		streamRead >> taskCount >> machineCount;
		Vtask.push_back(taskCount);
		Vmachine.push_back(machineCount);

		for (int i = 0; i < machineCount; ++i)
		{
			machine tempMachine;
			tempMachine.index = i + 1;
			container.push_back(tempMachine);
		}

		for (int i = 0; i < taskCount; ++i)
		{
			for (int j = 0; j < machineCount; ++j)
			{
				task buff;
				buff.index = i + 1;
				streamRead >> buff.value;
				container[j].p.push_back(buff);
			}
		}
		streamRead.close();
	}
}

void loadFilenames(const std::string filename, std::vector <std::string> &container) {

	std::fstream streamRead;
	streamRead.open(filename, std::ios::in);

	if (!streamRead.is_open()) {

		std::cerr << "Error opening file to read" << std::endl;
		return;
	}
	else {
		std::string buff;

		while (!streamRead.eof())
		{
			std::getline(streamRead, buff);
			container.push_back(buff);
		}
	}
}

void swap_tasks(std::vector <machine> &container, const int ind_swap, const int ind_with)
{
	for (auto &i : container)
	{
		std::swap(i.p[ind_swap], i.p[ind_with]);
	}
}

void insert_task(std::vector <machine> &container, const int from, const int to)
{
	for (auto &i : container)
	{
		task temp = i.p[from];
		i.p.erase(i.p.begin() + from);
		i.p.insert(i.p.begin() + to, temp);
	}
}

int SASwap(std::vector <machine> &container, const double temperaturr, const double coolingCoefficient, const double tresholdNearToZero)
{
	const int size = container[0].p.size();
	double temperature = temperaturr;

	int rand1 = 0, rand2 = 0;
	double probability = 0.0, randomZeroOne = 0.0;
	int prevCMax = calculate(container);

	while (true)
	{
		do
		{
			rand1 = rand() % size;
			rand2 = rand() % size;
			swap_tasks(container, rand1, rand2);
		} while (rand1 == rand2);

		int nowCMax = calculate(container);

		if (nowCMax > prevCMax)
		{
			probability = exp((nowCMax - prevCMax) / temperature);
			randomZeroOne = ((double)rand() / (double)RAND_MAX);

			if (randomZeroOne > probability)
			{
				swap_tasks(container, rand1, rand2);
			}
		}
		if (temperature > tresholdNearToZero)
		{
			temperature *= coolingCoefficient;
		}
		else
		{
			break;
		}

		prevCMax = nowCMax;
	}
	return calculate(container);
}

int SAInsert(std::vector <machine> &container, const double temperaturr, const double coolingCoefficient, const double tresholdNearToZero)
{
	const int size = container[0].p.size();
	double temperature = temperaturr;

	int rand1 = 0, rand2 = 0;
	double probability = 0.0, randomZeroOne = 0.0;
	int prevCMax = calculate(container);

	while (true)
	{
		do
		{
			rand1 = rand() % size;
			rand2 = rand() % size;
			insert_task(container, rand1, rand2);
		} while (rand1 == rand2);

		int nowCMax = calculate(container);

		if (nowCMax > prevCMax)
		{
			probability = exp((nowCMax - prevCMax) / temperature);
			randomZeroOne = ((double)rand() / (double)RAND_MAX);

			if (randomZeroOne > probability)
			{
				swap_tasks(container, rand1, rand2);
			}
		}
		if (temperature > tresholdNearToZero)
		{
			temperature *= coolingCoefficient;
		}
		else
		{
			break;
		}

		prevCMax = nowCMax;
	}
	return calculate(container);
}

int AverageSASwap(std::vector <machine> &container, const double temperaturr, const double coolingCoefficient, const double tresholdNearToZero, const int instancesCount)
{
	std::vector<std::vector <machine>> multiplication;
	int average = 0;

	for (size_t i = 0; i < instancesCount; ++i)
	{
		multiplication.push_back(container);
	}

	for (auto &i : multiplication)
	{
		average += SASwap(i, temperaturr, coolingCoefficient, tresholdNearToZero);
	}

	return average/instancesCount;
}

int AverageSAInsert(std::vector <machine> &container, const double temperaturr, const double coolingCoefficient, const double tresholdNearToZero, const int instancesCount)
{
	std::vector<std::vector <machine>> multiplication;
	int average = 0;

	for (size_t i = 0; i < instancesCount; ++i)
	{
		multiplication.push_back(container);
	}

	for (auto &i : multiplication)
	{
		average += SAInsert(i, temperaturr, coolingCoefficient, tresholdNearToZero);
	}

	return average / instancesCount;
}

int NEH(std::vector <machine> &container)
{
	std::vector <task> priority;

	for (int i = 0; i < container[0].p.size(); ++i)
	{
		task temp = { static_cast<unsigned int>(0), i };
		priority.push_back(temp);
	}

	for (int i = 0; i < container.size(); ++i)
	{
		for (int j = 0; j < container[0].p.size(); ++j)
		{
			priority[j].value += container[i].p[j].value;
		}
	}

	std::sort(priority.begin(), priority.end(), [](task a, task b) {return a.value > b.value; });

	std::vector <machine> sorted;

	for (int i = 0; i < container.size(); ++i)
	{
		machine temp;
		sorted.push_back(temp);
		for (int j = 0; j < container[0].p.size(); ++j)
		{
			task temp2 = container[i].p[priority[j].index];
			sorted[i].p.push_back(temp2);
		}
	}

	std::vector <std::vector <machine>> table;
	std::vector <machine> toRemember;

	for (int i = 0; i < sorted.size(); ++i)
	{
		machine temp;
		toRemember.push_back(temp);
		task temp2 = sorted[i].p[0];
		toRemember[i].p.push_back(temp2);
	}

	for (int i = 0; i < sorted.size(); ++i)
	{
		sorted[i].p.erase(sorted[i].p.begin());
	}

	int ind = NULL;
	while (!sorted[0].p.empty())
	{
		for (int i = 0; i < toRemember[0].p.size() + 1; ++i)
		{
			table.push_back(toRemember);
		}

		for (int i = 0; i < toRemember[0].p.size() + 1; ++i)
		{
			for (int j = 0; j < toRemember.size(); ++j)
			{
				table[i][j].p.insert(table[i][j].p.begin() + i, sorted[j].p[0]);
			}
		}

		for (int i = 0; i < sorted.size(); ++i)
		{
			sorted[i].p.erase(sorted[i].p.begin());
		}

		int value = INT_MAX;
		for (int i = 0; i < table.size(); ++i) {

			int temp = calculate(table[i]);
			if (temp < value) {
				value = temp;
				ind = i;
			}
		}

		toRemember = table[ind];
		table.clear();
	}
	return calculate(toRemember);
}

int NEHPlus(std::vector <machine> &container)
{
	std::vector <task> priority;

	for (int i = 0; i < container[0].p.size(); ++i)
	{
		task temp = { 0, i };
		priority.push_back(temp);
	}

	for (int i = 0; i < container.size(); ++i)
	{
		for (int j = 0; j < container[0].p.size(); ++j)
		{
			priority[j].value += container[i].p[j].value;
		}
	}

	std::sort(priority.begin(), priority.end(), [](task a, task b) {return a.value > b.value; });

	std::vector <machine> sorted;

	for (int i = 0; i < container.size(); ++i)
	{
		machine temp;
		sorted.push_back(temp);
		for (int j = 0; j < container[0].p.size(); ++j)
		{
			task temp2 = container[i].p[priority[j].index];
			sorted[i].p.push_back(temp2);
		}
	}

	std::vector <std::vector <machine>> table;
	std::vector <machine> toRemember;

	for (int i = 0; i < sorted.size(); ++i)
	{
		machine temp;
		toRemember.push_back(temp);
		task temp2 = sorted[i].p[0];
		toRemember[i].p.push_back(temp2);
	}

	for (int i = 0; i < sorted.size(); ++i)
	{
		sorted[i].p.erase(sorted[i].p.begin());
	}

	int ind = NULL;
	int zzz = 1;

	while (!sorted[0].p.empty())
	{
		for (int i = 0; i < toRemember[0].p.size() + 1; ++i)
		{
			table.push_back(toRemember);
		}

		for (int i = 0; i < toRemember[0].p.size() + 1; ++i)
		{
			for (int j = 0; j < toRemember.size(); ++j)
			{
				table[i][j].p.insert(table[i][j].p.begin() + i, sorted[j].p[0]);
			}
		}

		for (int i = 0; i < sorted.size(); ++i)
		{
			sorted[i].p.erase(sorted[i].p.begin());
		}

		//najmniejszy Cmax
		int value = INT_MAX;
		for (int i = 0; i < table.size(); ++i) {

			int temp = calculate(table[i]);
			if (temp < value) {
				value = temp;
				ind = i;
			}
		}

		toRemember = table[ind];
		table.clear();

		/////////////////////////////////////////////////////////////////////////////
		if (zzz % 2 == 0)
		{
			//ponowne wstawianie elementu bez ktorego CMax jest najmniejsze (4. opcja)

			for (int i = 0; i < toRemember[0].p.size(); ++i)
			{
				table.push_back(toRemember);
			}

			//przechowuje prawdziwe indeksy usuwanych elementow, aby mozna je bylo odzyskac z container
			std::vector <int> indexHunter;

			for (int i = 0; i < toRemember[0].p.size(); ++i)
			{
				//prawdziwe indeksy zaczynaja sie od 1, wiec trzeba je przesunac
				indexHunter.push_back(table[i][0].p[i].index - 1);

				//pomijamy usuwanie ostatnio dodanego elementu, tablica z tym elementem bedzie najwieksza wiec na pewno nie zostanie wybrana
				if (i != ind)
				{
					for (int j = 0; j < toRemember.size(); ++j)
					{
						table[i][j].p.erase(table[i][j].p.begin() + i);
					}
				}
			}

			//szukanie najmniejszego CMaxa
			value = INT_MAX;
			for (int i = 0; i < table.size(); ++i) {

				int temp = calculate(table[i]);
				if (temp < value) {
					value = temp;
					ind = i;
				}
			}

			toRemember = table[ind];
			table.clear();

			//wrzucenie elementu to vectora sorted, w celu ponownego dodania
			std::vector <machine> tempSorted;
			for (int i = 0; i < container.size(); ++i)
			{
				machine xd;
				xd.p.push_back(container[i].p[indexHunter[ind]]);
				tempSorted.push_back(xd);
			}

			for (int i = 0; i < sorted.size(); ++i)
			{
				for (int j = 0; j < sorted[0].p.size(); ++j)
				{
					tempSorted[i].p.push_back(sorted[i].p[j]);
				}
			}
			sorted = tempSorted;
		}
		++zzz;
	}

	return calculate(toRemember);
}

void calcEverything(std::vector <std::string> &filenames, std::vector <unsigned int> &cMax, unsigned int sampleCount, std::vector <double> &times, std::vector <unsigned int> &Vtask, std::vector <unsigned int> &Vmachine, const double temperaturr, const double coolingCoefficient, const double tresholdNearToZero, const int instancesCount)
{
	timer stopwatch;

	for (unsigned int i = 0; i < sampleCount; ++i)
	{
		std::cout << "nr: " << i << "\n";
		std::vector <machine> temp;
		loadContainer("data/" + filenames[i] + ".txt", temp, Vtask, Vmachine);
		//cMax.push_back(calculate(temp));
		stopwatch.start();
		cMax.push_back(NEH(temp));
		times.push_back(stopwatch.end());
		stopwatch.start();
		cMax.push_back(NEHPlus(temp));
		times.push_back(stopwatch.end());
		stopwatch.start();
		cMax.push_back(AverageSASwap(temp, temperaturr, coolingCoefficient, tresholdNearToZero, instancesCount));
		times.push_back(stopwatch.end());
		stopwatch.start();
		cMax.push_back(AverageSASwap(temp, temperaturr, coolingCoefficient*0.33, tresholdNearToZero, instancesCount));
		times.push_back(stopwatch.end());
		stopwatch.start();
		cMax.push_back(AverageSAInsert(temp, temperaturr, coolingCoefficient, tresholdNearToZero, instancesCount));
		times.push_back(stopwatch.end());
		stopwatch.start();
		cMax.push_back(AverageSAInsert(temp, temperaturr, coolingCoefficient*0.33, tresholdNearToZero, instancesCount));
		times.push_back(stopwatch.end());
	}
}

void printResultsToFile(std::vector <unsigned int> &cMax, std::vector <std::string> &filenames, std::vector <unsigned int> &Vtask, std::vector <unsigned int> &Vmachine, std::vector <double> &times)
{
	std::fstream streamWriteResults, streamWriteTimes, streamPlotMachcMax, streamPlotMachTime, streamPlotTaskcMax, streamPlotTaskTime;
	streamWriteResults.open("latex/results.txt", std::ios::out);
	streamWriteTimes.open("latex/times.txt", std::ios::out);
	streamPlotMachcMax.open("machcMax.txt", std::ios::out);
	streamPlotMachTime.open("machTime.txt", std::ios::out);
	streamPlotTaskcMax.open("taskcMax.txt", std::ios::out);
	streamPlotTaskTime.open("taskTime.txt", std::ios::out);
	//streamWritePRD.open("PRDs.txt", std::ios::out);
	std::vector <std::string> resultsS, timesS;
	std::vector <std::string> a, A, b, B, c, C, d, D, aa, AA, bb, BB, cc, CC, dd, DD;

	if (!streamWriteResults.is_open() || !streamWriteTimes.is_open() || !streamPlotMachcMax.is_open() || !streamPlotMachTime.is_open() || !streamPlotTaskcMax.is_open() || !streamPlotTaskTime.is_open()) {

		std::cerr << "Error opening file" << std::endl;
		return;
	}
	else {

		resultsS.push_back("Nazwa instancji &");
		resultsS.push_back("Zadania &");
		resultsS.push_back("Maszyny &");
		resultsS.push_back("NEH &");
		resultsS.push_back("NEH Plus &");
		resultsS.push_back("SA Swap &");
		resultsS.push_back("SA Swap 1/3 &");
		resultsS.push_back("SA Insert &");
		resultsS.push_back("SA Insert 1/3\\\\\n");

		timesS.push_back("Nazwa instancji &");
		timesS.push_back("Zadania &");
		timesS.push_back("Maszyny &");
		timesS.push_back("NEH &");
		timesS.push_back("NEH Plus &");
		timesS.push_back("SA Swap &");
		timesS.push_back("SA Swap 1/3 &");
		timesS.push_back("SA Insert &");
		timesS.push_back("SA Insert 1/3\\\\\n");

		//plot
		a.push_back("coordinates {\n");
		A.push_back("coordinates {\n");

		b.push_back("coordinates {\n");
		B.push_back("coordinates {\n");

		c.push_back("coordinates {\n");
		C.push_back("coordinates {\n");

		d.push_back("coordinates {\n");
		D.push_back("coordinates {\n");

		//plus
		aa.push_back("coordinates {\n");
		AA.push_back("coordinates {\n");

		bb.push_back("coordinates {\n");
		BB.push_back("coordinates {\n");

		cc.push_back("coordinates {\n");
		CC.push_back("coordinates {\n");

		dd.push_back("coordinates {\n");
		DD.push_back("coordinates {\n");

		const int algorithmsCount = 6;
		const int algorithmsCount2 = algorithmsCount;

		unsigned int tempMach = 0, tempTask = 0;

		for (int i = 0, k = 0; i < cMax.size(); ++i)
		{
			if (i % algorithmsCount == 0)
			{
				resultsS.push_back(filenames[k] + " &");
				resultsS.push_back(std::to_string(Vtask[k]) + " &");
				resultsS.push_back(std::to_string(Vmachine[k]) + " &");
				++k;
			}
			if ((i + 1) % algorithmsCount == 0)
			{
				resultsS.push_back(std::to_string(cMax[i]) + " \\\\\n");
			}
			else {
				resultsS.push_back(std::to_string(cMax[i]) + " &");
			}

			switch (i % algorithmsCount)
			{
			case 0: //NEH
				if (tempMach < Vmachine[k - 1])
				{
					a.push_back("(" + std::to_string(Vmachine[k - 1]) + "," + std::to_string(cMax[i]) + ")\n");
				}
				if (Vmachine[k - 1] == 20 && tempTask < Vtask[k - 1])
				{
					A.push_back("(" + std::to_string(Vtask[k - 1]) + "," + std::to_string(cMax[i]) + ")\n");
				}
				break;

			case 1: //NEH Plus
				if (tempMach < Vmachine[k - 1])
				{
					b.push_back("(" + std::to_string(Vmachine[k - 1]) + "," + std::to_string(cMax[i]) + ")\n");
					tempMach = Vmachine[k - 1];
				}
				if (Vmachine[k - 1] == 20 && tempTask < Vtask[k - 1])
				{
					B.push_back("(" + std::to_string(Vtask[k - 1]) + "," + std::to_string(cMax[i]) + ")\n");
					tempTask = Vtask[k - 1];
				}
				break;
			case 2: //SA Switch
				if (tempMach < Vmachine[k - 1])
				{
					c.push_back("(" + std::to_string(Vmachine[k - 1]) + "," + std::to_string(cMax[i]) + ")\n");
					tempMach = Vmachine[k - 1];
				}
				if (Vmachine[k - 1] == 20 && tempTask < Vtask[k - 1])
				{
					C.push_back("(" + std::to_string(Vtask[k - 1]) + "," + std::to_string(cMax[i]) + ")\n");
					tempTask = Vtask[k - 1];
				}
				break;
			case 3: //SA Insert
				if (tempMach < Vmachine[k - 1])
				{
					d.push_back("(" + std::to_string(Vmachine[k - 1]) + "," + std::to_string(cMax[i]) + ")\n");
					tempMach = Vmachine[k - 1];
				}
				if (Vmachine[k - 1] == 20 && tempTask < Vtask[k - 1])
				{
					D.push_back("(" + std::to_string(Vtask[k - 1]) + "," + std::to_string(cMax[i]) + ")\n");
					tempTask = Vtask[k - 1];
				}
				break;

			default:
				break;
			}

		}

		a.push_back("};\n");
		A.push_back("};\n");
		b.push_back("};\n");
		B.push_back("};\n");
		c.push_back("};\n");
		C.push_back("};\n");
		d.push_back("};\n");
		D.push_back("};\n");

		for (auto &i : a)
		{
			streamPlotMachcMax << i;
		}
		for (auto &i : A)
		{
			streamPlotTaskcMax << i;
		}
		for (auto &i : b)
		{
			streamPlotMachcMax << i;
		}
		for (auto &i : B)
		{
			streamPlotTaskcMax << i;
		}
		for (auto &i : c)
		{
			streamPlotMachcMax << i;
		}
		for (auto &i : C)
		{
			streamPlotTaskcMax << i;
		}
		for (auto &i : d)
		{
			streamPlotMachcMax << i;
		}
		for (auto &i : D)
		{
			streamPlotTaskcMax << i;
		}

		for (auto &i : resultsS)
		{
			streamWriteResults << i;
		}

		tempMach = 0, tempTask = 0;

		for (int i = 0, k = 0; i < times.size(); ++i)
		{
			if (i % algorithmsCount2 == 0)
			{
				timesS.push_back(filenames[k] + " &");
				timesS.push_back(std::to_string(Vtask[k]) + " &");
				timesS.push_back(std::to_string(Vmachine[k]) + " &");
				++k;
			}
			if ((i + 1) % algorithmsCount2 == 0) {

				timesS.push_back(std::to_string(times[i]) + " \\\\\n");
			}
			else {
				timesS.push_back(std::to_string(times[i]) + " &");
			}

			switch (i % algorithmsCount)
			{
			case 0: //NEH
				if (tempMach < Vmachine[k - 1])
				{
					aa.push_back("(" + std::to_string(Vmachine[k - 1]) + "," + std::to_string(times[i]) + ")\n");
				}
				if (Vmachine[k - 1] == 20 && tempTask < Vtask[k - 1])
				{
					AA.push_back("(" + std::to_string(Vtask[k - 1]) + "," + std::to_string(times[i]) + ")\n");
				}
				break;

			case 1: //NEH Plus
				if (tempMach < Vmachine[k - 1])
				{
					bb.push_back("(" + std::to_string(Vmachine[k - 1]) + "," + std::to_string(times[i]) + ")\n");
					tempMach = Vmachine[k - 1];
				}
				if (Vmachine[k - 1] == 20 && tempTask < Vtask[k - 1])
				{
					BB.push_back("(" + std::to_string(Vtask[k - 1]) + "," + std::to_string(times[i]) + ")\n");
					tempTask = Vtask[k - 1];
				}
				break;
			case 2: //NEH Plus
				if (tempMach < Vmachine[k - 1])
				{
					cc.push_back("(" + std::to_string(Vmachine[k - 1]) + "," + std::to_string(times[i]) + ")\n");
					tempMach = Vmachine[k - 1];
				}
				if (Vmachine[k - 1] == 20 && tempTask < Vtask[k - 1])
				{
					CC.push_back("(" + std::to_string(Vtask[k - 1]) + "," + std::to_string(times[i]) + ")\n");
					tempTask = Vtask[k - 1];
				}
				break;
			case 3: //NEH Plus
				if (tempMach < Vmachine[k - 1])
				{
					dd.push_back("(" + std::to_string(Vmachine[k - 1]) + "," + std::to_string(times[i]) + ")\n");
					tempMach = Vmachine[k - 1];
				}
				if (Vmachine[k - 1] == 20 && tempTask < Vtask[k - 1])
				{
					DD.push_back("(" + std::to_string(Vtask[k - 1]) + "," + std::to_string(times[i]) + ")\n");
					tempTask = Vtask[k - 1];
				}
				break;

			default:
				break;
			}
		}

		aa.push_back("};\n");
		AA.push_back("};\n");
		bb.push_back("};\n");
		BB.push_back("};\n");
		cc.push_back("};\n");
		CC.push_back("};\n");
		dd.push_back("};\n");
		DD.push_back("};\n");

		for (auto &i : aa)
		{
			streamPlotMachTime << i;
		}
		for (auto &i : AA)
		{
			streamPlotTaskTime << i;
		}
		for (auto &i : bb)
		{
			streamPlotMachTime << i;
		}
		for (auto &i : BB)
		{
			streamPlotTaskTime << i;
		}
		for (auto &i : cc)
		{
			streamPlotMachTime << i;
		}
		for (auto &i : CC)
		{
			streamPlotTaskTime << i;
		}
		for (auto &i : dd)
		{
			streamPlotMachTime << i;
		}
		for (auto &i : DD)
		{
			streamPlotTaskTime << i;
		}
		for (auto &i : timesS)
		{
			streamWriteTimes << i;
		}

	}
}

int main()
{
	srand((unsigned)time(NULL));
	
	std::vector <std::string> filenames;
	std::vector <unsigned int> cMax, sizes, Vtask, Vmachine;
	std::vector <double> times;
	
	const double temperature = 100.0;
	const double coolingCoefficient = 0.75;
	const double zero = 1.0E-10;
	const int repsAmount = 250;

	loadFilenames("data/filenames.txt", filenames);
	calcEverything(filenames, cMax, 120, times, Vtask, Vmachine, temperature, coolingCoefficient, zero,repsAmount);
	printResultsToFile(cMax, filenames, Vtask, Vmachine, times);

	/*
	std::vector <machine> container;
	loadContainer("data/ta001.txt", container, Vtask, Vmachine);

	while (1)
	{
		std::cout << AverageSASwap(container, temperature, coolingCoefficient, zero, repsAmount);
		std::cout << std::endl;
		std::cout << AverageSAInsert(container, temperature, coolingCoefficient, zero, repsAmount);
		std::cout << std::endl;
	}

	*/
	system("pause");
	return 0;
}