#pragma once

#include <optional>
#include <string>
#include <vector>

#include "Core.h"
#include "OnDiskFileIndex.h"
#include "OnDiskIndex.h"
#include "Query.h"
#include "ResultWriter.h"
#include "Task.h"

class OnDiskIndex;

class OnDiskDataset {
    std::string name;
    fs::path db_base;
    std::optional<OnDiskFileIndex> files_index;
    std::vector<OnDiskIndex> indices;
    std::set<std::string> taints;

    bool is_taint_compatible(const OnDiskDataset &other) const {
        return taints == other.taints;
    }
    std::string get_file_name(FileId fid) const;
    QueryResult query_str(const QString &str) const;
    QueryResult internal_execute(const Query &query) const;
    const OnDiskIndex &get_index_with_type(IndexType index_type) const;
    void drop_file(const std::string &fname) const;
    QueryResult pick_common(int cutoff,
                            const std::vector<Query> &queries) const;

   public:
    explicit OnDiskDataset(const fs::path &db_base, const std::string &fname);
    const std::string &get_name() const;
    fs::path get_base() const;
    const std::string &get_files_fname() const {
        return files_index->get_files_fname();
    }
    void toggle_taint(const std::string &taint_name);
    bool has_all_taints(const std::set<std::string> &taints) const;
    void execute(const Query &query, ResultWriter *out) const;
    uint64_t get_file_count() const { return files_index->get_file_count(); }
    void for_each_filename(std::function<void(const std::string &)> cb) const {
        files_index->for_each_filename(cb);
    }
    static void merge(const fs::path &db_base, const std::string &outname,
                      const std::vector<const OnDiskDataset *> &datasets,
                      Task *task);
    void save();
    void drop();
    std::string get_id() const;
    const std::vector<OnDiskIndex> &get_indexes() const { return indices; }
    const std::set<std::string> &get_taints() const { return taints; }
    static std::vector<const OnDiskDataset *> get_compact_candidates(
        const std::vector<const OnDiskDataset *> &datasets);
    static std::vector<std::vector<const OnDiskDataset *>>
    get_taint_compatible_datasets(
        const std::vector<const OnDiskDataset *> &datasets);
};

std::vector<FileId> internal_pick_common(
    int cutoff, const std::vector<const std::vector<FileId> *> &sources);
