#pragma once
#include <iosfwd>
#include <vector>
#include <string>
#include <filesystem>

class IIncludesSearcher
{
public:
	struct Options
	{
		bool IgnoreBracketIncludes = false; //! i. e. #include <iostream>
		bool SimplifyRelativeQuoteIncludes = false; //! i.e. "#include a/b/c.h" -> "a.h" or #include "../x/y.h" -> "y.h"
		std::vector<std::string> IgnoredIncludes;
	};

	IIncludesSearcher() = default;
	IIncludesSearcher(const Options& opts) : options{ opts } {};
	virtual ~IIncludesSearcher() = default;

	virtual Options& Opts() { return options; }
	virtual std::vector<std::string> Search(std::istream& is) = 0;
	virtual std::vector<std::string> Search(const std::filesystem::path& path) = 0;
protected:
	Options options;

};


class IncludesSearcher : public IIncludesSearcher
{
public:

	IncludesSearcher() = default;
	IncludesSearcher(const IncludesSearcher&) = default;
	IncludesSearcher(IncludesSearcher&&) = default;
	IncludesSearcher(const Options& opts) : IIncludesSearcher{ opts } {};
	IncludesSearcher& operator=(const IncludesSearcher&) = default;
	IncludesSearcher& operator=(IncludesSearcher&&) = default;

	virtual ~IncludesSearcher() = default;


	virtual std::vector<std::string> Search(std::istream& is) override;
	virtual std::vector<std::string> Search(const std::filesystem::path& path) override;


protected:
	std::string SimplifyRelativeQuoteInclude(const std::string& include);

};

