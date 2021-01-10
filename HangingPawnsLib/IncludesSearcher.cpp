#include "pch.h"
#include "IncludesSearcher.h"

#include <fstream>
#include <sstream>
#include <regex>

const static std::regex QUOTE_INCLUDE_REGEX { R"(#include[\s]+\"([\w|\.|/]+)\")" };
const static std::regex BRACKET_INCLUDE_REGEX { R"(#include[\s]+<([\w|\.|/]+)>)" };



std::vector<std::string> IncludesSearcher::Search(const std::filesystem::path& path)
{
	std::ifstream ifs{ path };
	return Search(ifs);
}

std::vector<std::string> IncludesSearcher::Search(std::istream& is)
{
	std::vector<std::string> includes;
	
	auto isIgnored = [this](const std::string& i) {
		const auto& ignoredIncludes = options.IgnoredIncludes;
		auto it = std::find(std::cbegin(ignoredIncludes), std::cend(ignoredIncludes), i);
		return it != std::cend(ignoredIncludes);
	};

	std::string line;
	while (std::getline(is, line))
	{
		std::smatch m;
		if (std::regex_search(line, m, QUOTE_INCLUDE_REGEX))
		{
			auto include = m[1].str();
			if (!isIgnored(include))
			{
				if (options.SimplifyRelativeQuoteIncludes)
				{
					include = SimplifyRelativeQuoteInclude(include);
				}
				includes.push_back(include);
			}
			continue;
		}

		if (!options.IgnoreBracketIncludes)
		{
			if (std::regex_search(line, m, BRACKET_INCLUDE_REGEX))
			{
				auto include = m[1];
				if (!isIgnored(include))
				{
					includes.push_back(include);
				}
				continue;
			}
		}

	}

	return includes;
}

std::string IncludesSearcher::SimplifyRelativeQuoteInclude(const std::string& include)
{
	auto pos = include.find_last_of('/');
	if (pos != std::string::npos)
	{
		return include.substr(pos + 1);
	}
	else
	{
		return include;
	}
}
