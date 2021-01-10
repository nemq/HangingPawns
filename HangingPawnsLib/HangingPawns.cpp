#include "pch.h"
#include "HangingPawns.h"
#include <unordered_map>

using namespace std;

HangingPawns::HangingPawns() : lister{ new FilesLister{} }, searcher{ new IncludesSearcher{} }
{
	searcher->Opts().IgnoreBracketIncludes = true;
	searcher->Opts().SimplifyRelativeQuoteIncludes = true;
}

vector<HangingPawns::PawnInfo> HangingPawns::Search(filesystem::path& sourceDirectory)
{
	vector<HangingPawns::PawnInfo> pawns;
	unordered_map<string, size_t> nameToIdx;

	const auto&  headersPaths = lister->AddFilesRecursive(sourceDirectory).FilterByExts({ ".h", ".hpp" }).GetPaths();
	for (const auto& p : headersPaths)
	{
		PawnInfo info;
		info.name = p.filename().replace_extension().string();
		info.includes = searcher->Search(p);
		info.headerPath = p;
		pawns.push_back(info);
		nameToIdx[info.name] = pawns.size() - 1;
	}
	lister->RemoveAll();

	const auto& sourcePaths = lister->AddFilesRecursive(sourceDirectory).FilterByExts({ ".c", ".cpp", ".cxx" }).GetPaths();
	for (const auto& p : sourcePaths)
	{
		auto sourceName = p.filename().replace_extension().string();
		auto sourceIncludes = searcher->Search(p);
		auto it = nameToIdx.find(sourceName);
		if (it != nameToIdx.end())
		{
			auto& info = pawns[it->second];
			info.sourcePath = p;
			info.includes.insert(end(info.includes), cbegin(sourceIncludes), cend(sourceIncludes));
		}
		else
		{
			PawnInfo info;
			info.name = sourceName;
			info.includes = sourceIncludes;
			pawns.push_back(info);
		}
	}
	lister->RemoveAll();

	sort(pawns.begin(), pawns.end(), [](const PawnInfo& l, const PawnInfo& r) { return l.includes.size() < r.includes.size(); });

	return pawns;
}
