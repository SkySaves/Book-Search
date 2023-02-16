#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//This function reads the "login.csv" file and checks if the entered username and password match any of the usernames and passwords in the file.
//The file is read line by line and each line is split into the username and password using the "find" and "substr" string functions.
//If a match is found, the function returns true, otherwise it returns false.

bool userLogin(string username, string password) {

	// Opens the file `login.csv`
	ifstream loginFile("login.csv");
	string line;

	// Reads each line of the file
	while (getline(loginFile, line)) {

		// Finds the comma in the line
		int i = line.find(",");

		// Extracts the username and password from the line
		string user = line.substr(0, i);
		string pass = line.substr(i + 1);

		// If the extracted username and password match the provided `username` and `password`, the function returns `true`
		if (user == username && pass == password) {
			return true;
		}
	}

	// If the loop completes without finding a matching `username` and `password`, the function returns `false`
	return false;
}


