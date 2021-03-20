// harchive.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <memory>
#include <filesystem>
#include "FileFunctions.h"

struct xfle_struct {
	char	name[144];
	unsigned int size;
};

int main(int argc, char* argv[])
{
	if (argc == 1) {
		std::cout << "HArchive - a tool to extract Harvester archives by ermaccer (.DAT, XFLE)\n"
			<< "Usage: hscramble <file>\n";
		return 1;
	}

	std::ifstream pFile(argv[1], std::ifstream::binary);

	if (!pFile)
	{
		std::cout << "ERROR: Could not open: " << argv[1] << "!" << std::endl;
		return 1;
	}

	if (pFile)
	{
		xfle_struct first_file;
		pFile.read((char*)&first_file, sizeof(xfle_struct));

		if (!(first_file.name[0] == 'X' && first_file.name[1] == 'F'))
		{
			std::cout << "ERROR: " << argv[1] << " is not a valid Harvester archive!" << std::endl;
		}

		// process :
		for (int i = 0; i < strlen(first_file.name); i++)
		{
			if (first_file.name[i] == ':')
			{
				first_file.name[i] = '_';
				break;
			}
		}

		std::string first_path = first_file.name;
		if (checkSlash(first_path))
			std::filesystem::create_directories(splitString(first_path, false));

		std::ofstream output(first_file.name, std::ofstream::binary);
		std::cout << "Processing: " << first_file.name << std::endl;
		std::unique_ptr<char[]> firstData = std::make_unique<char[]>(first_file.size);
		pFile.read(firstData.get(), first_file.size);

		output.write(firstData.get(), first_file.size);

		// rest of files

		unsigned int fileSize = std::filesystem::file_size(argv[1]);
		while ((unsigned int)pFile.tellg() < fileSize)
		{
			xfle_struct file;
			pFile.read((char*)&file, sizeof(xfle_struct));
			// process :
			for (int i = 0; i < strlen(file.name); i++)
			{
				if (file.name[i] == ':')
				{
					file.name[i] = '_';
					break;
				}
			}


			std::string path = file.name;
			if (checkSlash(path))
				std::filesystem::create_directories(splitString(path, false));

			std::ofstream output(file.name, std::ofstream::binary);
			std::cout << "Processing: " << file.name << std::endl;
			std::unique_ptr<char[]> Data = std::make_unique<char[]>(file.size);
			pFile.read(Data.get(), file.size);

			output.write(Data.get(), file.size);

		}

	}
}

