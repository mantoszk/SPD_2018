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
		int tasks, machines, buff, buff2;
		std::vector <int> values;

		read >> filename >> tasks >> machines;

		for (int i = 0; i < tasks * machines; ++i)
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
		else
		{
			filenames.push_back(filename);

			write << tasks << " " << machines << std::endl;

			for (int i = 0; i < machines*tasks * 2; ++i)
			{
				if (i % 2 == 1)
				{
					if (values[i] >= 0 && values[i] <= 9)
					{
						write << " ";
					}
					write << values[i] << " ";
				}

				if ((i + 1) % (machines *2) == 0)
				{
					write << std::endl;
				}
			}
			write << std::endl;
			
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