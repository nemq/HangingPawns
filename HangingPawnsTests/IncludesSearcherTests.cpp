#include "pch.h"
#include "IncludesSearcher.h"
#include <sstream>
#include <filesystem>
#include <fstream>

using namespace std;


class IncludesSearcherFixture : public ::testing::Test
{
protected:

	static void SetUpTestCase()
	{
		auto tempDir = std::filesystem::temp_directory_path();
		SourcePath = tempDir / "IncludesSearcherFixture.txt";
		
		std::ofstream ofs{ SourcePath };
		ofs << R"(

#include "pch.h"
#include "stdafx.h"

#include "Quote.h"
#include "quote.h"
#include "quote.hpp"

#include "Quote/relative.h"
#include "./Quote2/Relative.h"
#include "../../Quote3/relative.hpp"

#include <bracket.h>
#include <bracket.hpp>

#include <bracket/relative>
#include <bracket2/relative.h>
#include <bracket_3/relative.hpp>

		  #include		"white/chars.h"		    

int main(int argc, char* argv[])
{
	std::cout << "Hello world!\n";
}

)";

	}

	static void TearDownTestCase()
	{
		std::filesystem::remove(SourcePath);
	}

	void SetUp() override
	{

		lister.Opts() = IncludesSearcher::Options{};
	}

	IncludesSearcher lister;
	static std::filesystem::path SourcePath;

};

std::filesystem::path  IncludesSearcherFixture::SourcePath;


TEST_F(IncludesSearcherFixture, SearchCount)
{
	auto includes = lister.Search(SourcePath);
	ASSERT_EQ(includes.size(), 14);
}

TEST_F(IncludesSearcherFixture, SearchIncludeFormat)
{
	auto includes = lister.Search(SourcePath);
	ASSERT_EQ(includes.size(), 14);
	EXPECT_STREQ(includes[0].c_str(), "pch.h");
	EXPECT_STREQ(includes[2].c_str(), "Quote.h");
	EXPECT_STREQ(includes[5].c_str(), "Quote/relative.h");
	EXPECT_STREQ(includes[7].c_str(), "../../Quote3/relative.hpp");
	EXPECT_STREQ(includes[8].c_str(), "bracket.h");
	EXPECT_STREQ(includes[12].c_str(), "bracket_3/relative.hpp");
}

TEST_F(IncludesSearcherFixture, SearchIgnoreBrackets)
{
	
	lister.Opts().IgnoreBracketIncludes = true;

	auto includes = lister.Search(SourcePath);
	ASSERT_EQ(includes.size(), 9);
	EXPECT_STREQ(includes[0].c_str(), "pch.h");
	EXPECT_STREQ(includes[2].c_str(), "Quote.h");
	EXPECT_STREQ(includes[5].c_str(), "Quote/relative.h");
	EXPECT_STREQ(includes[7].c_str(), "../../Quote3/relative.hpp");
	EXPECT_STREQ(includes[8].c_str(), "white/chars.h");
}


TEST_F(IncludesSearcherFixture, SearchIgnoreIgnoreIncludes)
{
	
	lister.Opts().IgnoredIncludes = { "pch.h", "stdafx.h" };

	auto includes = lister.Search(SourcePath);
	ASSERT_EQ(includes.size(), 12);
	EXPECT_STREQ(includes[0].c_str(), "Quote.h");
	EXPECT_STREQ(includes[3].c_str(), "Quote/relative.h");
	EXPECT_STREQ(includes[5].c_str(), "../../Quote3/relative.hpp");
	EXPECT_STREQ(includes[11].c_str(), "white/chars.h");
}


TEST_F(IncludesSearcherFixture, SearchSimplifyRelativeQuoteIncludes)
{
	
	lister.Opts().SimplifyRelativeQuoteIncludes = true;

	auto includes = lister.Search(SourcePath);
	ASSERT_EQ(includes.size(), 14);
	EXPECT_STREQ(includes[5].c_str(), "relative.h");
	EXPECT_STREQ(includes[7].c_str(), "relative.hpp");
	EXPECT_STREQ(includes[12].c_str(), "bracket_3/relative.hpp");
}
