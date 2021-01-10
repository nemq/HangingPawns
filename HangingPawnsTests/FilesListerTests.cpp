#include "pch.h"
#include "FilesLister.h"
#include <fstream>

using namespace std;
namespace fs = std::filesystem;

class FilesListerFixture : public ::testing::Test
{
protected:
	void SetUp() override
	{
		initialPaths =
		{
			R"(C:\dir\header.h)",
			R"(C:\dir\Header.H)",
			R"(C:\dir\source.cpp)",
			R"(C:\dir\SoUrCe.CpP)",
			R"(C:\dir\hpp.hpp)",
			R"(C:\dir\HpP.HpP)",
			R"(C:\dir\csharp.cs)",
			R"(C:\dir\text.txt)",
			R"(C:\dir\noext)",
			R"(..)",
			R"(.)",
			R"(C:\dir\dots.in.name.h)",
			R"(C:\dir\space in name.h)"
		};

		lister = FilesLister{initialPaths};
	}

	void TearDown() override
	{
	}


	vector<fs::path> initialPaths;
	FilesLister lister;

};


TEST_F(FilesListerFixture, FilterByExt)
{
	string ext = ".cpp";
	auto filtered = lister.FilterByExt(ext).GetPaths();
	ASSERT_EQ(filtered.size(), 2);
	EXPECT_STRCASEEQ(filtered[0].string().c_str(), initialPaths[2].string().c_str());
	EXPECT_STRCASEEQ(filtered[1].string().c_str(), initialPaths[3].string().c_str());
}


TEST_F(FilesListerFixture, FilterByExtNoDot)
{
	string ext = "cpp";
	auto filtered = lister.FilterByExt(ext).GetPaths();
	ASSERT_EQ(filtered.size(), 2);
	EXPECT_STRCASEEQ(filtered[0].string().c_str(), initialPaths[2].string().c_str());
	EXPECT_STRCASEEQ(filtered[1].string().c_str(), initialPaths[3].string().c_str());
}

TEST_F(FilesListerFixture, FilterByExtEmptyExt)
{
	string ext;
	auto filtered = lister.FilterByExt(ext).GetPaths();
	EXPECT_TRUE(filtered.empty());
}

TEST_F(FilesListerFixture, FilterByExtCase)
{
	string ext = ".CpP";
	auto filtered = lister.FilterByExt(ext).GetPaths();
	ASSERT_EQ(filtered.size(), 2);
	EXPECT_STRCASEEQ(filtered[0].string().c_str(), initialPaths[2].string().c_str());
	EXPECT_STRCASEEQ(filtered[1].string().c_str(), initialPaths[3].string().c_str());
}


TEST_F(FilesListerFixture, FilterByExts)
{
	vector<string> exts = { ".CpP", "h", ".HPP" };
	auto filtered = lister.FilterByExts(exts).GetPaths();

	ASSERT_EQ(filtered.size(), 8);
	EXPECT_STRCASEEQ(filtered[0].string().c_str(), initialPaths[0].string().c_str());
	EXPECT_STRCASEEQ(filtered[3].string().c_str(), initialPaths[3].string().c_str());
	EXPECT_STRCASEEQ(filtered[5].string().c_str(), initialPaths[5].string().c_str());
	EXPECT_STRCASEEQ(filtered[7].string().c_str(), initialPaths[12].string().c_str());
}


TEST_F(FilesListerFixture, Remove)
{
	const auto& remainiginitialPaths = lister.Remove(initialPaths[4]).GetPaths();

	EXPECT_EQ(remainiginitialPaths.size(), initialPaths.size() - 1);
	EXPECT_TRUE(std::find(std::cbegin(remainiginitialPaths), std::cend(remainiginitialPaths), initialPaths[4]) == std::cend(remainiginitialPaths));
}

TEST_F(FilesListerFixture, RemoveAll)
{
	const auto& remainiginitialPaths = lister.RemoveAll().GetPaths();

	EXPECT_TRUE(remainiginitialPaths.empty());
}



