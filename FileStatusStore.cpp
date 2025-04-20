#include "FileStatusStore.h"

#include <iostream>
#include <stdexcept>
#include <string>

FileStatusStore::FileStatusStore(const std::filesystem::path &directory)
    : m_sqlFile{directory / "git_annex_uploader.sql"}
{
    std::cout << "SQL " << m_sqlFile << "\n";
    int result = sqlite3_open(m_sqlFile.c_str(), &m_sqlite);
    if (result != SQLITE_OK)
    {
        std::string errorMsg = "Could not open database: " + m_sqlFile;
        std::cerr << errorMsg << "\n";
        throw std::runtime_error{errorMsg};
    }
}

FileStatusStore::~FileStatusStore()
{
    if (m_sqlite)
    {
        int result = sqlite3_close(m_sqlite);
        if (result != SQLITE_OK)
        {
            std::string errorMsg = "Could not close database: " + m_sqlFile;
            std::cerr << errorMsg << "\n";
        }
    }
};
