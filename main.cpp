// GitAnnex.cpp : Defines the entry point for the application.
//

#include <iostream>
#include <cstdlib>

using namespace std;

int main()
{
	int result = std::system("git annex");
	cout << "result " << result << "\n";

	return 0;
}
