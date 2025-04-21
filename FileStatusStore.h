#pragma once

#include <filesystem>

#include "sqlite3.h"

enum FileStatus
{
    None,
    Uploaded
};

struct FileStatusStore
{
    FileStatusStore(const std::filesystem::path &directory);
    ~FileStatusStore();

    FileStatus GetFileStatus(const std::filesystem::path &filename);
    void UpdateFileStatus(const std::filesystem::path &filename, FileStatus status);

  private:
    void CheckResult(int result);

    std::string m_sqlFile;
    sqlite3 *m_sqlite;
    sqlite3_stmt *m_stmtGetStatus;
    sqlite3_stmt *m_stmtInsertFile;
};
