#pragma once

#include <filesystem>

#include "sqlite3.h"

struct FileStatusStore
{
    FileStatusStore(const std::filesystem::path &directory);
    ~FileStatusStore();

  private:
    std::string m_sqlFile;
    sqlite3 *m_sqlite;
};
