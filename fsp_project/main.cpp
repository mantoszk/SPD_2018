#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "timer.h"

//grupa nr 2
//[SPD]_pt13_gr2_T3

struct vi
{
	unsigned int value, index;
};

struct fsp
{

	std::vector <vi> p;
	std::vector <unsigned int> s, c;
};

unsigned int calculate(std::vector <fsp> &container)
{
	const unsigned int machines = container.size(), tasks = container[0].p.size();

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

void loadContainer(const std::string filename, std::vector <fsp> &container)
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
		int machines, tasks;
		streamRead >> machines >> tasks;

		for (int i = 0, k = -1; i < machines * tasks; ++i)
		{
			vi temp;
			streamRead >> temp.index >> temp.value;

			if (i % tasks == 0)
			{
				fsp temp2;
				temp2.p.push_back(temp);
				container.push_back(temp2);
				++k;
			}
			else
			{
				container[k].p.push_back(temp);
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

void calcEverything(std::vector <std::string> &filenames, std::vector <unsigned int> &cMax)
{
	int k = 0;
	for (std::string i : filenames)
	{
		std::vector <fsp> temp;
		loadContainer("data/" + i + ".txt", temp);
		cMax.push_back(calculate(temp));
		++k;
	}
}

int main()
{
	std::vector <std::string> filenames;
	std::vector <unsigned int> cMax;

	loadFilenames("data/filenames.txt", filenames);
	calcEverything(filenames, cMax);

	for (auto &i : cMax)
	{
		std::cout << i << std::endl;
	}
	system("pause");
	return 0;
}