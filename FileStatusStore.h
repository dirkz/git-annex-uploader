#pragma once

#include <sqlite3.h>

#include <filesystem>

struct FileStatusStore
{
    FileStatusStore(const std::filesystem::path &directory);

  private:
    sqlite3 *m_sqlite;
};
