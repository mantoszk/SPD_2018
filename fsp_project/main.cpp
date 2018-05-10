#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "timer.h"

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

		/*
		for (auto &k : toRemember)
		{
			for (auto &j : k.p)
			{
				std::cout << j.index << " ";
			}
			std::cout << std::endl;
			break;
		}
		*/

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
			//std::cout << "Deleted: " << tempSorted[0].p[0].index << std::endl;

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

	/*
	for (auto &i : toRemember)
	{
		for (auto &j : i.p)
		{
			std::cout << j.index << " ";
		}
		break;
	}
	std::cout << std::endl;
	*/

	return calculate(toRemember);
}

void calcEverything(std::vector <std::string> &filenames, std::vector <unsigned int> &cMax, unsigned int sampleCount, std::vector <double> &times, std::vector <unsigned int> &Vtask, std::vector <unsigned int> &Vmachine)
{
	timer stopwatch;
	
	for (unsigned int i = 0; i < sampleCount; ++i)
	{
		std::cout<<"nr: "<<i<<"\n";
		std::vector <machine> temp;
		loadContainer("data/" + filenames[i] + ".txt", temp, Vtask, Vmachine);
		cMax.push_back(calculate(temp));
		stopwatch.start();
		cMax.push_back(NEH(temp));
		times.push_back(stopwatch.end());
		stopwatch.start();
		cMax.push_back(NEHPlus(temp));
		times.push_back(stopwatch.end());
	}
}

void printResultsToFile(std::vector <unsigned int> &cMax, std::vector <std::string> &filenames, std::vector <unsigned int> &Vtask, std::vector <unsigned int> &Vmachine, std::vector <double> &times)
{
	std::fstream streamWriteResults, streamWriteTimes, streamPlotMachcMax, streamPlotMachTime, streamPlotTaskcMax, streamPlotTaskTime;
	streamWriteResults.open("results.txt", std::ios::out);
	streamWriteTimes.open("times.txt", std::ios::out);
	streamPlotMachcMax.open("machcMax.txt", std::ios::out);
	streamPlotMachTime.open("machTime.txt", std::ios::out);
	streamPlotTaskcMax.open("taskcMax.txt", std::ios::out);
	streamPlotTaskTime.open("taskTime.txt", std::ios::out);
	//streamWritePRD.open("PRDs.txt", std::ios::out);
	std::vector <std::string> resultsS, timesS, PRDS, PMC1, PTC1, PMC, PMT, PTC, PTT, PMCP, PMTP, PTCP, PTTP;

	if (!streamWriteResults.is_open() || !streamWriteTimes.is_open() || !streamPlotMachcMax.is_open() || !streamPlotMachTime.is_open() || !streamPlotTaskcMax.is_open() || !streamPlotTaskTime.is_open() ) {

		std::cerr << "Error opening file" << std::endl;
		return;
	}
	else {

		resultsS.push_back("Nazwa instancji &");
		resultsS.push_back("Zadania &");
		resultsS.push_back("Maszyny &");
		resultsS.push_back("12345 &");
		resultsS.push_back("NEH &");
		resultsS.push_back("NEH Plus\\\\\n");

		timesS.push_back("Nazwa instancji &");
		timesS.push_back("Zadania &");
		timesS.push_back("Maszyny &");
		timesS.push_back("NEH &");
		timesS.push_back("NEH Plus\\\\\n");
		
		//plot
		PMC1.push_back("coordinates {\n");
		PTC1.push_back("coordinates {\n");
		
		PMC.push_back("coordinates {\n");
		PMT.push_back("coordinates {\n");
		PTC.push_back("coordinates {\n");
		PTT.push_back("coordinates {\n");
		//plus
		PMCP.push_back("coordinates {\n");
		PMTP.push_back("coordinates {\n");
		PTCP.push_back("coordinates {\n");
		PTTP.push_back("coordinates {\n");

		const int algorithmsCount = 3;
		const int algorithmsCount2 = algorithmsCount - 1;
		
		unsigned int tempMach=0, tempTask=0;

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
			
			switch (i%3)
			{
				case 0: //123
					if(tempMach<Vmachine[k-1])
					{
						PMC1.push_back("("+std::to_string(Vmachine[k-1])+","+std::to_string(cMax[i])+")\n");
					}
					if( Vmachine[k-1]==20 && tempTask<Vtask[k-1] )
					{
						PTC1.push_back("("+std::to_string(Vtask[k-1])+","+std::to_string(cMax[i])+")\n");
					}
					break;
					
				case 1: //NEH
					if(tempMach<Vmachine[k-1])
					{
						PMC.push_back("("+std::to_string(Vmachine[k-1])+","+std::to_string(cMax[i])+")\n");
					}
					if( Vmachine[k-1]==20 && tempTask<Vtask[k-1] )
					{
						PTC.push_back("("+std::to_string(Vtask[k-1])+","+std::to_string(cMax[i])+")\n");
					}
					break;
					
				case 2: //NEH Plus
					if(tempMach<Vmachine[k-1])
					{
						PMCP.push_back("("+std::to_string(Vmachine[k-1])+","+std::to_string(cMax[i])+")\n");
						tempMach=Vmachine[k-1];
					}
					if( Vmachine[k-1]==20 && tempTask<Vtask[k-1] )
					{
						PTCP.push_back("("+std::to_string(Vtask[k-1])+","+std::to_string(cMax[i])+")\n");
						tempTask=Vtask[k-1];
					}
					break;
					
				default:
					break;
			}
			
		}
		
		PMC1.push_back("};\n");
		PTC1.push_back("};\n");
		PMC.push_back("};\n");
		PTC.push_back("};\n");
		PMCP.push_back("};\n");
		PTCP.push_back("};\n");
		
		for (auto &i : PMC1)
		{
			streamPlotMachcMax << i;
		}
		for (auto &i : PMC)
		{
			streamPlotMachcMax << i;
		}
		for (auto &i : PMCP)
		{
			streamPlotMachcMax << i;
		}
		
		for (auto &i : PTC1)
		{
			streamPlotTaskcMax << i;
		}
		for (auto &i : PTC)
		{
			streamPlotTaskcMax << i;
		}
		for (auto &i : PTCP)
		{
			streamPlotTaskcMax << i;
		}
		
		
		for (auto &i : resultsS)
		{
			streamWriteResults << i;
		}
		
		tempMach=0, tempTask=0;

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
			
			switch (i%2)
			{					
				case 0: //NEH
					if(tempMach<Vmachine[k-1])
					{
						PMT.push_back("("+std::to_string(Vmachine[k-1])+","+std::to_string(times[i])+")\n");
					}
					if( Vmachine[k-1]==20 && tempTask<Vtask[k-1] )
					{
						PTT.push_back("("+std::to_string(Vtask[k-1])+","+std::to_string(times[i])+")\n");
					}
					break;
					
				case 1: //NEH Plus
					if(tempMach<Vmachine[k-1])
					{
						PMTP.push_back("("+std::to_string(Vmachine[k-1])+","+std::to_string(times[i])+")\n");
						tempMach=Vmachine[k-1];
					}
					if( Vmachine[k-1]==20 && tempTask<Vtask[k-1] )
					{
						PTTP.push_back("("+std::to_string(Vtask[k-1])+","+std::to_string(times[i])+")\n");
						tempTask=Vtask[k-1];
					}
					break;
					
				default:
					break;
			}
		}
		
		PMT.push_back("};\n");
		PTT.push_back("};\n");
		PMTP.push_back("}\n;");
		PTTP.push_back("}\n;");

		for (auto &i : PMT)
		{
			streamPlotMachTime << i;
		}
		for (auto &i : PMTP)
		{
			streamPlotMachTime << i;
		}
		
		for (auto &i : PTT)
		{
			streamPlotTaskTime << i;
		}
		for (auto &i : PTTP)
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
	std::vector <std::string> filenames;
	std::vector <unsigned int> cMax, sizes, Vtask, Vmachine;
	std::vector <double> times;

	loadFilenames("data/filenames.txt", filenames);
	calcEverything(filenames, cMax, 120, times, Vtask, Vmachine);

	printResultsToFile(cMax, filenames, Vtask, Vmachine, times);
	system("pause");
	return 0;
}
