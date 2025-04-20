#pragma once

#include <sqlite3.h>

#include <filesystem>

struct FileStatusStore
{
    FileStatusStore(const std::filesystem::path &directory);
    ~FileStatusStore();

  private:
    std::string m_sqlFile;
    sqlite3 *m_sqlite;
};
