#include <cstdlib>
#include <iostream>

using namespace std;

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
    cout << "Processing " << directory << " ...\n";

    return 0;
}
