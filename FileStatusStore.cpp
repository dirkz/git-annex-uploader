#include "FileStatusStore.h"

#include <string>

FileStatusStore::FileStatusStore(const std::filesystem::path &directory)
{
    std::string sqlFile = directory / "git_annex_uploader.sql";
    int result = sqlite3_open(sqlFile.c_str(), &m_sqlite);
};
