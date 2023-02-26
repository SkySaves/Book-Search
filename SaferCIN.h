#pragma once
#include <iostream>
#include <limits>
#include <string>

// Helper function to safely get input from the user

void saferCin(int& input)
{
    // Prompts the user for input and stores it in the input int reference.
    std::cin >> input;
    // Ignores any excess characters in the input buffer after the desired input has been retrieved.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void saferCin(double& input)
{
	// Prompts the user for input and stores it in the input int reference.
	std::cin >> input;
	// Ignores any excess characters in the input buffer after the desired input has been retrieved.
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void saferCin(float& input)
{
	// Prompts the user for input and stores it in the input int reference.
	std::cin >> input;
	// Ignores any excess characters in the input buffer after the desired input has been retrieved.
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void saferCin(char& input)
{
	// Prompts the user for input and stores it in the input int reference.
	std::cin >> input;
	// Ignores any excess characters in the input buffer after the desired input has been retrieved.
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


void saferCin(string& input) {
	// Prompts the user for input and stores it in the input int reference.
	std::cin >> input;
	// Ignores any excess characters in the input buffer after the desired input has been retrieved.
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
