#include <cstdlib>
#include <filesystem>
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

    fs::current_path(directory);

    fs::path exePath{argv[0]};
    fs::path currentDir = exePath.parent_path();

    FileStatusStore store{currentDir};

    auto it = fs::recursive_directory_iterator{directory};
    for (const fs::directory_entry &entry : it)
    {
        if (entry.is_symlink())
        {
            FileStatus status = store.GetFileStatus(entry);
            if (status == FileStatus::None)
            {

            }
        }
    }

    return 0;
}
