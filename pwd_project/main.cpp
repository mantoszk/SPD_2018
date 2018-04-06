#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

//grupa nr 2
//[SPD]_pt13_gr2_T3

struct pwd {

	int p = NULL, w = NULL, d = NULL;
	int index = NULL;
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
		T = std::max(C - i.d, 0);
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

		std::cerr << "Error opening file to read" << std::endl;
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

int main(){

	std::vector <pwd> container;

	loadContainer("witi/data10.txt", container);
	coutPWD(container);
	
	std::cout << calcWiti(container) << std::endl;
	coutOrder(container);

	sortLessD(container);
	std::cout << calcWiti(container) << std::endl;
	coutOrder(container);

	system("pause");
	return 0;
}