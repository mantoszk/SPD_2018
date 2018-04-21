#include <fstream>
#include <string>
#include <vector>

void main()
{
	std::fstream read;
	read.open("data.txt", std::ios::in);
	if (!read.is_open())
	{
		return;
	}

	std::vector <std::string> filenames;

	while (!read.eof())
	{
		std::string filename;
		int size, machines, buff, buff2;
		std::vector <int> values;

		read >> filename >> size >> machines;

		for (int i = 0; i < size * machines; ++i)
		{
			read >> buff >> buff2;
			values.push_back(buff);
			values.push_back(buff2);
		}

		std::fstream write;
		write.open("data/" + filename + ".txt", std::ios::out);
		if (!write.is_open())
		{
			return;
		}
		else {
			filenames.push_back(filename);

			write << size << " " << machines << std::endl;
			for (int i = 0; i < size*machines * 2; ++i)
			{
				if (values[i] >= 0 && values[i] <= 9)
				{
					write << " " << values[i] << " ";
				}
				else
				{
					write << values[i] << " ";
				}

				if ((i + 1) % (2 * machines) == 0)
				{
					write << std::endl;
				}
				else if (i % 2 == 1)
				{
					write << " ";
				}
			}
			write.close();
		}
	}
	read.close();

	std::fstream write;
	write.open("data/filenames.txt", std::ios::out);
	if (!write.is_open())
	{
		return;
	}

	for (auto &i : filenames)
	{
		write << i << std::endl;
	}

	write.close();
}