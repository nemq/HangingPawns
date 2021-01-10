#include "pch.h"
#include "HangingPawns.h"

using ::testing::NiceMock;

class FilesListerMock : public FilesLister
{
public:
	
	FilesListerMock()
	{
		ON_CALL(*this, GetPaths).WillByDefault([this]() { return this->FilesLister::GetPaths(); }); //! By default delegate to base class

		//! By default return reference to mock object, so not interesting call will not brake code.
		ON_CALL(*this, AddFiles).WillByDefault([this](std::filesystem::path directory) -> FilesLister& { return *this; });
		ON_CALL(*this, AddFilesRecursive).WillByDefault([this](std::filesystem::path directory) -> FilesLister& { return *this; });
		ON_CALL(*this, FilterByExt).WillByDefault([this](const std::string& ext) -> FilesLister& { return *this; });
		ON_CALL(*this, FilterByExts).WillByDefault([this](const std::vector<std::string>& exts) -> FilesLister& { return *this; });
		ON_CALL(*this, RemoveAll).WillByDefault([this]() -> FilesLister& { return *this; });
		ON_CALL(*this, Remove(::testing::An<const std::filesystem::path&>())).WillByDefault([this](const std::filesystem::path& p) -> FilesLister& { return *this; });
		ON_CALL(*this, Remove(::testing::An<const std::vector<std::filesystem::path>&>())).WillByDefault([this](const std::vector<std::filesystem::path>& paths) -> FilesLister& { return *this; });
	}

	FilesListerMock(const FilesListerMock&) = default;
	FilesListerMock(FilesListerMock&&) = default;

	FilesListerMock& operator=(const FilesListerMock&) = default;
	FilesListerMock& operator=(FilesListerMock&&) = default;

	virtual ~FilesListerMock() = default;

	MOCK_METHOD(const std::vector<std::filesystem::path>&, GetPaths, (), (const, override));
	MOCK_METHOD(FilesLister&, AddFiles, (std::filesystem::path directory), (override));
	MOCK_METHOD(FilesLister&, AddFilesRecursive, (std::filesystem::path directory), (override));
	MOCK_METHOD(FilesLister&, FilterByExt, (const std::string& ext), (override));
	MOCK_METHOD(FilesLister&, FilterByExts, (const std::vector<std::string>& exts), (override));
	MOCK_METHOD(FilesLister&, RemoveAll, (), (override));
	MOCK_METHOD(FilesLister&, Remove, (const std::filesystem::path& p), (override));
	MOCK_METHOD(FilesLister&, Remove, (const std::vector<std::filesystem::path>& paths), (override));

};

class IncludesSearcherMock : public IncludesSearcher
{
public:
	
	MOCK_METHOD(std::vector<std::string>, Search, (std::istream& is), (override));
	MOCK_METHOD(std::vector<std::string>, Search, (const std::filesystem::path& path), (override));
};



class HangingPawnsSearcherFixture : public ::testing::Test
{
	

protected:

	NiceMock<FilesListerMock>* CreateFilesListerMock()
	{
		auto* lister = new  NiceMock<FilesListerMock>{};
		EXPECT_CALL(*lister, GetPaths).WillOnce(::testing::ReturnRef(HEADERS)).WillOnce(::testing::ReturnRef(SOURCES));
		return lister;
	}

	NiceMock<IncludesSearcherMock>* CreateIncludesSearcherMock()
	{
		auto* searcher = new  NiceMock<IncludesSearcherMock>{};

		EXPECT_CALL(*searcher, Search(HEADERS[0])).WillOnce(::testing::Return(HEADERS_INCLUDES[0]));
		EXPECT_CALL(*searcher, Search(HEADERS[1])).WillOnce(::testing::Return(HEADERS_INCLUDES[1]));
		EXPECT_CALL(*searcher, Search(HEADERS[2])).WillOnce(::testing::Return(HEADERS_INCLUDES[2]));
		EXPECT_CALL(*searcher, Search(SOURCES[0])).WillOnce(::testing::Return(SOURCES_INCLUDES[0]));
		EXPECT_CALL(*searcher, Search(SOURCES[1])).WillOnce(::testing::Return(SOURCES_INCLUDES[1]));
		EXPECT_CALL(*searcher, Search(SOURCES[2])).WillOnce(::testing::Return(SOURCES_INCLUDES[2]));

		return searcher;
	}

	const static std::vector<std::filesystem::path> HEADERS;
	const static std::vector<std::vector<std::string>> HEADERS_INCLUDES;
	const static std::vector<std::filesystem::path> SOURCES;
	const static std::vector<std::vector<std::string>> SOURCES_INCLUDES;
};

const std::vector<std::filesystem::path> HangingPawnsSearcherFixture::HEADERS =
{
	R"(C:\a.h)",
	R"(C:\b.h)",
	R"(C:\c.h)"
};

const std::vector<std::filesystem::path> HangingPawnsSearcherFixture::SOURCES =
{
	R"(C:\a.cpp)",
	R"(C:\b.cpp)",
	R"(C:\c.cpp)"
};

const std::vector<std::vector<std::string>> HangingPawnsSearcherFixture::HEADERS_INCLUDES =
{
	{"iostrem", "string", "vector"},
	{"filesystem"},
	{"list", "map"}
};

const std::vector<std::vector<std::string>> HangingPawnsSearcherFixture::SOURCES_INCLUDES =
{
	{"a.h"},
	{"b.h", "a.h"},
	{"c.h", "a.h", "b.h"}
};



TEST_F(HangingPawnsSearcherFixture, FindsRightIncludes)
{
	auto* filesLister = CreateFilesListerMock();
	auto* includesSearcher = CreateIncludesSearcherMock();
	HangingPawns pawns{ filesLister, includesSearcher};

	auto result = pawns.Search(std::filesystem::path{ "" });

	ASSERT_EQ(3, result.size());
	const auto& cPawn = result[2];
	EXPECT_STREQ(cPawn.name.c_str(), "c");

	const auto& cIncludes = cPawn.includes;
	ASSERT_EQ(5, cIncludes.size());
	
	auto searchInclude = [&cIncludes](const std::string& include) -> bool
	{
		auto it = std::find(std::cbegin(cIncludes), std::cend(cIncludes), include);
		return it != std::cend(cIncludes);
	};

	ASSERT_TRUE(searchInclude("a.h"));
	ASSERT_TRUE(searchInclude("b.h"));
	ASSERT_TRUE(searchInclude("c.h"));
	ASSERT_TRUE(searchInclude("list"));
	ASSERT_TRUE(searchInclude("map"));
}

TEST_F(HangingPawnsSearcherFixture, MergeHeadersAndSources)
{
	auto* filesLister = CreateFilesListerMock();
	auto* includesSearcher = CreateIncludesSearcherMock();
	HangingPawns pawns{ filesLister, includesSearcher};

	auto result = pawns.Search(std::filesystem::path{ "" });

	ASSERT_EQ(3, result.size());
	for (size_t i = 0; i < result.size(); i++)
	{
		ASSERT_TRUE(result[i].headerPath.has_value());
		ASSERT_TRUE(result[i].sourcePath.has_value());
	}
}

TEST_F(HangingPawnsSearcherFixture, SortedAscByIncludesCount)
{
	auto* filesLister = CreateFilesListerMock();
	auto* includesSearcher = CreateIncludesSearcherMock();
	HangingPawns pawns{ filesLister, includesSearcher};

	auto result = pawns.Search(std::filesystem::path{ "" });

	size_t maxIncldues = 0;
	for (size_t i = 0; i < result.size(); i++)
	{
		ASSERT_GE(result[i].includes.size(), maxIncldues);
		maxIncldues = result[i].includes.size();
	}
}



