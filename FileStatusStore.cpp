#include "FileStatusStore.h"

#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

constexpr const char *CreateStatement = R"(
    CREATE TABLE IF NOT EXISTS files (
        name TEXT NOT NULL,
        status INT NOT NULL
    );
    CREATE INDEX IF NOT EXISTS filename_index ON files(name);
)";

constexpr const char *GetStatusQuery = R"(
    SELECT status FROM files WHERE name = ?;
)";

constexpr const char *InsertFile = R"(
    INSERT INTO files VALUES(?, ?);
)";

FileStatusStore::FileStatusStore(const std::filesystem::path &directory)
    : m_sqlFile{directory / "git_annex_uploader.sqlite"}
{
    std::cout << "SQL " << m_sqlFile << "\n";
    int result = sqlite3_open(m_sqlFile.c_str(), &m_sqlite);
    if (result != SQLITE_OK)
    {
        std::string errorMsg = "Could not open database: " + m_sqlFile;
        std::cerr << errorMsg << "\n";
        throw std::runtime_error{errorMsg};
    }

    char *errorMsg = nullptr;
    result = sqlite3_exec(m_sqlite, CreateStatement, nullptr, nullptr, &errorMsg);
    if (errorMsg)
    {
        std::cerr << "SQL error: " << errorMsg << "\n";
        sqlite3_free(errorMsg);
    }
    CheckResult(result);

    result = sqlite3_prepare_v2(m_sqlite, GetStatusQuery, -1, &m_stmtGetStatus, nullptr);
    CheckResult(result);

    result = sqlite3_prepare_v2(m_sqlite, InsertFile, -1, &m_stmtInsertFile, nullptr);
    CheckResult(result);
}

FileStatusStore::~FileStatusStore()
{
    if (m_stmtGetStatus)
    {
        sqlite3_finalize(m_stmtGetStatus);
    }

    if (m_stmtInsertFile)
    {
        sqlite3_finalize(m_stmtInsertFile);
    }

    if (m_sqlite)
    {
        int result = sqlite3_close(m_sqlite);
        if (result != SQLITE_OK)
        {
            std::string errorMsg = "Could not close database: " + m_sqlFile;
            std::cerr << errorMsg << "\n";
        }
    }
}

FileStatus FileStatusStore::GetFileStatus(const std::filesystem::path &filename)
{
    std::string filenameString = filename.string();

    int result = sqlite3_bind_text(m_stmtGetStatus, 1, filenameString.c_str(), -1, SQLITE_STATIC);
    CheckResult(result);

    FileStatus status = FileStatus::None;

    result = sqlite3_step(m_stmtGetStatus);

    switch (result)
    {
    case SQLITE_DONE:
        break;
    case SQLITE_ROW: {
        int s = sqlite3_column_int(m_stmtGetStatus, 0);
        status = static_cast<FileStatus>(s);
        break;
    }
    default:
        CheckResult(result);
    }

    result = sqlite3_reset(m_stmtGetStatus);
    CheckResult(result);

    return status;
}

void FileStatusStore::UpdateFileStatus(const std::filesystem::path &filename, FileStatus status)
{
    std::string filenameString = filename.string();

    int result = sqlite3_bind_text(m_stmtInsertFile, 1, filenameString.c_str(), -1, SQLITE_STATIC);
    CheckResult(result);

    result = sqlite3_bind_int(m_stmtInsertFile, 2, static_cast<int>(status));
    CheckResult(result);

    int resultStep = sqlite3_step(m_stmtInsertFile);

    result = sqlite3_reset(m_stmtInsertFile);
    CheckResult(result);

    if (resultStep != SQLITE_DONE)
    {
        std::string errorMsg = std::format("could not update {}", filename.string());
        throw std::runtime_error{errorMsg.c_str()};
    }
}

void FileStatusStore::CheckResult(int result)
{
    if (result != SQLITE_OK && result != SQLITE_DONE && result != SQLITE_ROW)
    {
        const char *errorMessage = sqlite3_errmsg(m_sqlite);
        const char *pError = "unknown error";
        if (errorMessage)
        {
            pError = errorMessage;
        }
        std::string errorString = std::string{"sqlite3 error: "} + pError;
        throw std::runtime_error{errorString};
    }
};
