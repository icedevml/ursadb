#pragma once

#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "Core.h"
#include "Database.h"
#include "Query.h"
#include "RawFile.h"
#include "Task.h"

class OnDiskFileIndex {
    uint64_t file_count;
    fs::path db_base;
    std::string files_fname;
    std::string cache_fname;
    RawFile files_file;
    std::optional<RawFile> cache_file;

    void generate_namecache_file();

   public:
    OnDiskFileIndex(const fs::path &db_base, const std::string &files_fname);
    OnDiskFileIndex(const OnDiskFileIndex &) = delete;
    OnDiskFileIndex(OnDiskFileIndex &&) = default;
    ~OnDiskFileIndex();

    std::string get_file_name(FileId fid) const;

    const uint64_t get_file_count() const { return file_count; }
    const std::string &get_files_fname() const { return files_fname; }

    void for_each_filename(std::function<void(const std::string &)> cb) const;
};
