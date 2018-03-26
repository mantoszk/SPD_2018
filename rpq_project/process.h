#pragma once

#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <iostream>

struct rpqsc {

	int r = NULL, p = NULL, q = NULL, s = NULL, c = NULL, index = NULL;

	bool operator < (const rpqsc& str) const {

		return (r < str.r);
	}

};

struct compareR {

	bool operator ()(const rpqsc & raz, const rpqsc & dwa)
	{
		//q - malej¹co
		if (raz.r > dwa.r) return true;

		if (raz.r < dwa.r) return false;

		return false;
	}
};

struct compareQ {

	bool operator ()(const rpqsc & raz, const rpqsc & dwa)
	{
		//q - malej¹co
		if (raz.q < dwa.q) return true;

		if (raz.q > dwa.q) return false;

		return false;
	}
};

class process {

private:
	std::vector <rpqsc> container;
	int calculate();

	std::priority_queue <rpqsc, std::vector <rpqsc>, compareR> N;
	std::priority_queue <rpqsc, std::vector <rpqsc>, compareQ> G;

	int U = 0, UB = _MAX_INT_DIG, LB = 0;

	void block(int &a, int &b, int &c);

public:
	process();
	~process();
	size_t size() const;
	void showOrder() const;

	bool loadFile(std::string filename);
	
	int inOrder();
	int sortR();
	int twoopt();
	int swoopt();

	int schrage();
	int schrage2();
	int carlier();
};