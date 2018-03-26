#include "process.h"

void process::block(int &a, int &b, int &c)
{
	int temp = 0;
	int n = b - a;

	for (int j = 0; j < n; ++j) {
		temp = container[j].c + container[j].q;

		if ( b< temp) {
			
		}
	}
}

process::process() {

}

process::~process() {

}

size_t process::size() const {

	return container.size();
}

bool process::loadFile(std::string filename) {

	std::fstream fileStream;
	fileStream.open(filename, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Error opening file" << std::endl;
		return false;
	}
	else {
		rpqsc temp;
		int n, lp;
		fileStream >> n >> lp;

		for (int i = 0; i < n; ++i) {
			temp.index = i;
			fileStream >> temp.r >> temp.p >> temp.q;
			container.push_back(temp);
		}
	}
	fileStream.close();
	return true;
}

int process::calculate() {

	container[0].s = container[0].r;
	container[0].c = container[0].s + container[0].p;

	for (unsigned int i = 1; i < container.size(); ++i) {
		container[i].s = std::max(container[i].r, container[i - 1].s + container[i - 1].p);
		container[i].c = container[i].s + container[i].p;
	}

	int tempCMax = 0;
	int cMaximum = 0;

	for (unsigned int i = 0; i < container.size(); ++i) {
		tempCMax = container[i].c + container[i].q;

		if (cMaximum < tempCMax) {
			cMaximum = tempCMax;
		}
	}

	return cMaximum;
}

int process::inOrder()
{
	return calculate();
}

int process::sortR()
{
	std::sort(container.begin(), container.end());

	return calculate();
}

int process::twoopt()
{
	int best = calculate();

	for (int i = 0; i < container.size() - 1; ++i) {
		for (int j = i + 1; j < container.size(); ++j) {
			std::swap(container[i], container[j]);
			int temp = calculate();
			if (temp >= best) {
				std::swap(container[i], container[j]);
			}
			else {
				best = temp;
				i = 0;
				j = 1;
			}
		}
	}

	return best;
}

int process::swoopt()
{
	sortR();
	return twoopt();
}

int process::schrage()
{
	for (int i = 0; i < container.size(); ++i) {

		N.push(container[i]);
	}
	container.erase(container.begin(), container.end());

	int t = 0;

	while (!(N.empty() && G.empty())) {

		while (!N.empty() && N.top().r <= t) {

			G.push(N.top());
			N.pop();
		}

		if (!G.empty()) {

			container.push_back(G.top());
			t += G.top().p;
			G.pop();
		}
		else {

			t = N.top().r;
		}
	}
	return calculate();
}

int process::schrage2()
{
	for (int i = 0; i < container.size(); ++i) {

		N.push(container[i]);
	}
	container.erase(container.begin(), container.end());

	int t = 0;
	rpqsc previous;

	while (!(N.empty() && G.empty())) {

		while (!N.empty() && N.top().r <= t) {

			G.push(N.top());

			if (N.top().q > previous.q) {

				previous.p = t - N.top().r;
				t = N.top().r;

				if (previous.p > 0) {

					container.pop_back();
					G.push(previous);
				}
			}

			N.pop();
		}

		if (!G.empty()) {

			container.push_back(G.top());
			t += G.top().p;
			previous = G.top();
			G.pop();
		}
		else {

			t = N.top().r;
		}

	}
	return calculate();
}

int process::carlier()
{
	std::vector <rpqsc> temp = container;

	U = schrage();

	if (U < UB) {

		UB = U;
	}
	else {

		container = temp;
	}

	//blok ab i c

	return calculate();
}

void process::showOrder() const
{
	for (int i = 0; i < container.size(); ++i) {
		std::cout << container[i].index << " ";
	}
	std::cout << std::endl;
}
