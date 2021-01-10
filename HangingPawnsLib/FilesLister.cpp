#include "pch.h"
#include "FilesLister.h"
#include <boost/algorithm/string.hpp>

namespace fs = std::filesystem;

FilesLister& FilesLister::AddFiles(std::filesystem::path directory)
{
	for (auto it = fs::directory_iterator{ directory }; it != fs::directory_iterator{}; it++)
	{
		paths.push_back(it->path());
	}

	return *this;
}

FilesLister& FilesLister::AddFilesRecursive(std::filesystem::path directory)
{
	for (auto it = fs::recursive_directory_iterator{ directory }; it != fs::recursive_directory_iterator{}; it++)
	{
		paths.push_back(it->path());
	}

	return *this;
}

FilesLister& FilesLister::FilterByExt(const std::string& ext)
{
	const std::string normExt = NormalizeExt(ext);
	for (auto it = std::begin(paths); it != std::end(paths);)
	{
		if (it->has_extension() && boost::iequals(it->extension().string(), normExt))
		{
			it++;
		}
		else
		{
			it = paths.erase(it);
		}
	}

	return *this;
}

FilesLister& FilesLister::FilterByExts(const std::vector<std::string>& exts)
{
	const auto normExts = NormalizeExts(exts);

	for (auto it = std::begin(paths); it != std::end(paths);)
	{
		if (!it->has_extension())
		{
			it = paths.erase(it);

		}
		else
		{
			auto pred = [it](const std::string& e) {return boost::iequals(it->extension().string(), e); };
			auto found = std::find_if(std::cbegin(normExts), std::cend(normExts), pred) != std::cend(normExts);
			if (!found)
			{
				it = paths.erase(it);
			}
			else
			{
				it++;
			}
		}
	}

	return *this;

}

FilesLister& FilesLister::RemoveAll()
{
	paths.clear();
	return *this;
}

FilesLister& FilesLister::Remove(const std::filesystem::path& p)
{
	auto it = std::find(std::begin(paths), std::end(paths), p);
	if (it != std::end(paths))
	{
		paths.erase(it);
	}

	return *this;
}

FilesLister& FilesLister::Remove(const std::vector<std::filesystem::path>& paths)
{
	for (const auto& p : paths)
	{
		Remove(p);
	}

	return *this;
}

std::string FilesLister::NormalizeExt(const std::string& ext)
{
	if (ext.empty() || ext.front() == '.')
	{
		return ext;
	}
	else
	{
		return "." + ext;
	}
}

std::vector<std::string> FilesLister::NormalizeExts(const std::vector<std::string>& exts)
{
	std::vector<std::string> normExts;
	for (const auto& e : exts)
	{
		normExts.push_back(NormalizeExt(e));
	}

	return normExts;
}

