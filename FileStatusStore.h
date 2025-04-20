#pragma once

#include <filesystem>

#include "sqlite3.h"

enum FileStatus
{
    None,
    CouldNotGet,
    Uploaded
};

struct FileStatusStore
{
    FileStatusStore(const std::filesystem::path &directory);
    ~FileStatusStore();

    FileStatus GetFileStatus(std::filesystem::path filename);

  private:
    void CheckResult(int result);

    std::string m_sqlFile;
    sqlite3 *m_sqlite = nullptr;
    sqlite3_stmt *m_stmtGetStatus = nullptr;
};
