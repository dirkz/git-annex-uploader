#include <cstdlib>
#include <filesystem>
#include <format>
#include <iostream>

#include "FileStatusStore.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage:\n\n" << argv[0] << " <dir> <remote>\n";
        cerr << "  where\n";
        cerr << "    <dir>    is the git annex directory you want to upload\n";
        cerr << "    <remote> is the git annex remote you want to upload to\n\n";
        return -1;
    }

    string directory{argv[1]};
    string remote{argv[2]};
    cout << "Remote " << remote << "\n";
    cout << "Processing " << directory << " ...\n";

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
                std::string quotedFilename = std::string{"\""} + entry.path().string() + "\"";
                std::string cmdGet = std::format("git annex get {}", quotedFilename);
                std::string cmdDrop = std::format("git annex drop {}", quotedFilename);
                std::string cmdCopy =
                    std::format("git annex copy {} --to={}", quotedFilename, remote);
                int resultWrong = system("git annex get blarg");
                int result = system(cmdGet.c_str());
                if (result != 0)
                {
                    result = system(cmdCopy.c_str());
                    if (result == 0)
                    {
                        store.UpdateFileStatus(entry, FileStatus::Uploaded);
                    }
                    result = system(cmdDrop.c_str());
                }
            }
        }
    }

    return 0;
}
