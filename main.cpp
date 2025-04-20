#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
	int result = std::system("git annex");
	cout << "result " << result << "\n";

	return 0;
}
