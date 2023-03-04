#include <iostream>
#include "md5.h"

using std::cout; using std::endl; using std::string; using std::cin;

int main(int argc, char *argv[])
{
	string password;
	cout << "enter password :";
	cin >> password;

	auto n_hashed = [&](){

		if (password == md5(password))
				{
					 return true;
				}
	};
	auto hashed = n_hashed;
	printf((char*)hashed);




    return 0;
}



