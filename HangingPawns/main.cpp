// HangingPawns.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <unordered_map>
#include <HangingPawns.h>




int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cerr << "Usage: HangingPawns.exe C:/path/to/source/directory\n";
		return -1;
	}
	
	std::filesystem::path sourceDir{ argv[1] };
	HangingPawns hangingPawns;
	auto pawns = hangingPawns.Search(sourceDir);

	
	std::cout << std::left << std::setw(50) << "NAME";
	std::cout << std::right << std::setw(10) << "COUNT";
	std::cout << "\t\t" << "FILE(S)";
	std::cout << "\n";
	for (const auto& pi : pawns)
	{
		std::cout << std::left << std::setw(50)  << pi.name;
		std::cout << std::right << std::setw(10) << pi.includes.size();
		std::cout << "\t\t";
		if (pi.headerPath)
		{
			std::cout << pi.headerPath.value() << "\t";
		}

		if (pi.sourcePath)
		{
			std::cout << pi.sourcePath.value();
		}

		std::cout << "\n";
	}

	return 0;
}
