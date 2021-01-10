#pragma once
#include <filesystem>
#include <vector>

class IFilesLister
{
public:
	virtual ~IFilesLister() = default;
	virtual const std::vector<std::filesystem::path>& GetPaths() const = 0;
	virtual IFilesLister& AddFiles(std::filesystem::path directory) = 0;
	virtual IFilesLister& AddFilesRecursive(std::filesystem::path directory) = 0;
	virtual IFilesLister& FilterByExt(const std::string& ext) = 0;
	virtual IFilesLister& FilterByExts(const std::vector<std::string>& exts) = 0;
	virtual IFilesLister& RemoveAll() = 0;
	virtual IFilesLister& Remove(const std::filesystem::path& p) = 0;
	virtual IFilesLister& Remove(const std::vector<std::filesystem::path>& paths) = 0;
};


class FilesLister : public IFilesLister
{
public:
	FilesLister() = default;
	FilesLister(const std::vector<std::filesystem::path>& initialPaths) : paths{ initialPaths } {}
	virtual ~FilesLister() = default;
	virtual const std::vector<std::filesystem::path>& GetPaths() const override { return paths; }

	virtual FilesLister& AddFiles(std::filesystem::path directory) override;
	virtual FilesLister& AddFilesRecursive(std::filesystem::path directory) override;
	virtual FilesLister& FilterByExt(const std::string& ext) override;
	virtual FilesLister& FilterByExts(const std::vector<std::string>& exts) override;
	virtual FilesLister& RemoveAll() override;
	virtual FilesLister& Remove(const std::filesystem::path& p) override;
	virtual FilesLister& Remove(const std::vector<std::filesystem::path>& paths) override;

protected: 

	std::vector<std::filesystem::path> paths;
	virtual std::string NormalizeExt(const std::string& ext);
	virtual std::vector<std::string> NormalizeExts(const std::vector<std::string>& exts);
};

