#pragma once
#include <iostream>
#include <vector>
#include "SaferCIN.h"

struct Shopper {
    std::string name;
    std::string email;
    double totalAmountSpent;
};

// Create a vector of Shopper objects
std::vector<Shopper> shoppers;




// Function to add a new shopper to the vector
void addShopper(std::string name, std::string email, double totalAmountSpent) {
	Shopper shopper;
	shopper.name = name;
	shopper.email = email;
	shopper.totalAmountSpent = totalAmountSpent;
	shoppers.push_back(shopper);
}



void addShopperHandler() {
	std::string name;
	std::string email;
	double totalAmountSpent;

	std::cout << "Enter the shopper's name: ";
	std::cin >> name;
	std::cout << "Enter the shopper's email: ";
	std::cin >> email;
	std::cout << "Enter the shopper's total amount spent: ";
	std::cin >> totalAmountSpent;

	addShopper(name, email, totalAmountSpent);
}


