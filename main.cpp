#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>
#include <string>

#include "FileStatusStore.h"

namespace fs = std::filesystem;

static void Replace(std::string &s, const std::string &what, const std::string &with)
{
    size_t n = with.length();
    size_t pos = 0;
    while ((pos = s.find(what, pos)) != std::string::npos)
    {
        s.replace(pos, n, with);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage:\n\n" << argv[0] << " <dir> <remote>\n";
        std::cerr << "  where\n";
        std::cerr << "    <dir>    is the git annex directory you want to upload\n";
        std::cerr << "    <remote> is the git annex remote you want to upload to\n\n";
        return -1;
    }

    std::string directory{argv[1]};
    std::string remote{argv[2]};

    std::cout << argv[0] << " " << directory << " " << remote << "\n";

    fs::path current = fs::current_path();

    FileStatusStore store{current};

    fs::current_path(directory);

    auto it = fs::recursive_directory_iterator{directory};
    for (const fs::directory_entry &entry : it)
    {
        if (entry.is_symlink())
        {
            FileStatus status = store.GetFileStatus(entry);
            if (status == FileStatus::None)
            {
                std::string pathString = entry.path().string();
                Replace(pathString, "`", "\\`");
                std::string quotedFilename = std::string{"\""} + pathString + "\"";
                std::string cmdGet = std::format("git annex get {}", quotedFilename);
                std::string cmdDrop = std::format("git annex drop {}", quotedFilename);
                std::string cmdCopy =
                    std::format("git annex copy {} --to={}", quotedFilename, remote);

                int result = system(cmdGet.c_str());
                if (result != 0)
                {
                    std::string errorMsg = std::format("error getting {}", entry.path().string());
                    std::cerr << errorMsg << "\n";
                    continue;
                }

                result = system(cmdCopy.c_str());
                if (result != 0)
                {
                    std::string errorMsg = std::format("error copying {}", entry.path().string());
                    std::cerr << errorMsg << "\n";
                    continue;
                }

                store.UpdateFileStatus(entry, FileStatus::Uploaded);
                result = system(cmdDrop.c_str());
            }
        }
    }

    return 0;
}
