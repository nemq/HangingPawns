#pragma once
#include "FilesLister.h"
#include "IncludesSearcher.h"
#include <optional>
#include <filesystem>

class HangingPawns
{
	struct PawnInfo
	{
		std::string name;
		std::optional<std::filesystem::path> headerPath;
		std::optional<std::filesystem::path> sourcePath;
		std::vector<std::string> includes;
	};

public:
	HangingPawns();
	HangingPawns(IFilesLister* fl, IIncludesSearcher* is) : lister{ fl }, searcher{ is } {}

	std::vector<PawnInfo> Search(std::filesystem::path& sourceDirectory);
	

private:
	std::unique_ptr<IFilesLister> lister = nullptr;
	std::unique_ptr<IIncludesSearcher> searcher = nullptr;
};

