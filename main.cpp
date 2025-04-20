#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <dir>\n";
        cerr << "  where <dir> is the git annex directory you want to upload\n\n";
        return -1;
    }

    string directory{argv[1]};
    cout << "Processing " << directory << " ...\n";

    return 0;
}
