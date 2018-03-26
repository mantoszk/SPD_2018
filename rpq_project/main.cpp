#include <iostream>
#include "process.h"
#include "timer.h"
#include <vector>

//grupa nr 2
//[SPD]_pt13_gr2_T1

int main() {

	/*
	std::vector <std::string> filenames;
	std::vector <int> values;
	std::vector <double> times;
	std::vector <int> sizes;

	const std::string FileWithFileNames = "filenames.txt";
	const std::string ResultFileName = "results.txt";
	const std::string TimesFileName = "times.txt";

	{
		std::fstream streamIn;
		streamIn.open(FileWithFileNames, std::ios::in);

		if (!streamIn.is_open()) {

			std::cerr << "Error opening file" << std::endl;
			return -1;
		}
		else {
			while (!streamIn.eof()) {
				std::string temp;

				std::getline(streamIn, temp);
				filenames.push_back(temp);
			}
		}
		streamIn.close();
	}

	timer stopwatch;
	for (int i = 0; i < filenames.size(); ++i) {

		for (int j = 0; j < 4; ++j) {
			process tempProcess;
			if (!tempProcess.loadFile("pliki/"+filenames[i])) {

				std::cout << "Invalid filename" << std::endl;
				return -1;
			}
			else {
				if (j == 0) {

					sizes.push_back(tempProcess.size());
					stopwatch.start();
					values.push_back(tempProcess.inOrder());
					times.push_back(stopwatch.end());
				}
				else if (j == 1) {

					stopwatch.start();
					values.push_back(tempProcess.twoopt());
					times.push_back(stopwatch.end());
				}
				else if (j == 2) {

					stopwatch.start();
					values.push_back(tempProcess.sortR());
					times.push_back(stopwatch.end());
				}
				else if (j == 3) {

					stopwatch.start();
					values.push_back(tempProcess.swoopt());
					times.push_back(stopwatch.end());
				}
			}
		}
	}

	{
		std::fstream streamOutResults, streamOutTimes;
		streamOutResults.open(ResultFileName, std::ios::out);
		streamOutTimes.open(TimesFileName, std::ios::out);

		if (!streamOutResults.is_open() || !streamOutTimes.is_open()) {

			std::cerr << "Error opening file" << std::endl;
			return -1;
		}
		else {

			streamOutResults << "Nazwa instancji &" << "Rozmiar &" << "12345 &" << "2-opt &" << "sortR &" << "sortR+2-opt \\\\" << std::endl;
			streamOutTimes << "Nazwa instancji &" << "Rozmiar &" << "12345 &" << "2-opt &" << "sortR &" << "sortR+2-opt \\\\" << std::endl;

			for (int i = 0, k = 0; i < values.size(); ++i) {
				if (i % 4 == 0) {

					streamOutResults << filenames[k] << " &" << sizes[k] << " &";
					streamOutTimes << filenames[k] << " &" << sizes[k] << " &";
					++k;
				}
				streamOutResults << values[i];
				streamOutTimes << std::round(times[i]*1000000.0)/1000000.0*1000.0;

				if ((i + 1) % 4 == 0) {

					streamOutResults << " \\\\" << std::endl;
					streamOutTimes << " \\\\" << std::endl;
				}
				else {

					streamOutResults << " &";
					streamOutTimes << " &";
				}
			}
		}

		streamOutResults.close();
	}

	double timeSum = 0;
	for (int i = 0; i < times.size(); ++i) {
		timeSum += times[i];
	}
	std::cout << "Total time: " << timeSum << std::endl;
	*/

	std::vector <std::string> filenames;
	std::vector <int> values;
	std::vector <double> times;
	std::vector <int> sizes;

	const std::string FileWithFileNames = "filenames.txt";
	const std::string ResultFileName = "results.txt";
	const std::string TimesFileName = "times.txt";

	{
		std::fstream streamIn;
		streamIn.open(FileWithFileNames, std::ios::in);

		if (!streamIn.is_open()) {

			std::cerr << "Error opening file" << std::endl;
			return -1;
		}
		else {
			while (!streamIn.eof()) {
				std::string temp;

				std::getline(streamIn, temp);
				filenames.push_back(temp);
			}
		}
		streamIn.close();
	}

	timer stopwatch;
	for (int i = 0; i < filenames.size(); ++i) {

		for (int j = 0; j < 3; ++j) {
			process tempProcess;
			if (!tempProcess.loadFile("pliki/" + filenames[i])) {

				std::cout << "Invalid filename" << std::endl;
				return -1;
			}
			else {
				if (j == 0) {

					sizes.push_back(tempProcess.size());
					stopwatch.start();
					values.push_back(tempProcess.schrage());
					times.push_back(stopwatch.end());
				}
				else if (j == 1) {

					stopwatch.start();
					values.push_back(tempProcess.schrage2());
					times.push_back(stopwatch.end());
				}
				else if (j == 2) {

					stopwatch.start();
					values.push_back(tempProcess.carlier());
					times.push_back(stopwatch.end());
				}
			}
		}
	}
	{
		std::fstream streamOutResults, streamOutTimes;
		streamOutResults.open(ResultFileName, std::ios::out);
		streamOutTimes.open(TimesFileName, std::ios::out);

		if (!streamOutResults.is_open() || !streamOutTimes.is_open()) {

			std::cerr << "Error opening file" << std::endl;
			return -1;
		}
		else {

			streamOutResults << "Nazwa instancji &" << "Rozmiar &" << "Schrage &" << "Schrage 2 &" << "Carlier \\\\" << std::endl;
			streamOutTimes << "Nazwa instancji &" << "Rozmiar &" << "Schrage &" << "Schrage 2 &" << "Carlier \\\\" << std::endl;

			for (int i = 0, k = 0; i < values.size(); ++i) {
				if (i % 3 == 0) {

					streamOutResults << filenames[k] << " &" << sizes[k] << " &";
					streamOutTimes << filenames[k] << " &" << sizes[k] << " &";
					++k;
				}
				streamOutResults << values[i];
				streamOutTimes << std::round(times[i] * 1000000.0) / 1000000.0*1000.0;

				if ((i + 1) % 3 == 0) {

					streamOutResults << " \\\\" << std::endl;
					streamOutTimes << " \\\\" << std::endl;
				}
				else {

					streamOutResults << " &";
					streamOutTimes << " &";
				}
			}
		}

		streamOutResults.close();
	}

	double timeSum = 0;
	for (int i = 0; i < times.size(); ++i) {
		timeSum += times[i];
	}
	std::cout << "Total time: " << timeSum << std::endl;

	system("pause");
	return 0;
}
	