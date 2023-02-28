#pragma once
#include <iostream>
#include "Handlers.h"
#include "SaferCIN.h"


using namespace std;



void displayMainMenu() {
    std::string userChoice;


    while (true) {
        // Clears the console screen.
        system("cls");

        // Display the main menu options.
        std::cout << "Welcome to the Bookstore!" << std::endl;
        std::cout << "Please choose an option:" << std::endl;
        std::cout << "1. Search for a book" << std::endl;
        std::cout << "2. Add a book to the shopping list" << std::endl;
        std::cout << "3. View the shopping list" << std::endl;
        std::cout << "4. Create a Shopper account" << std::endl;
        std::cout << "5. Exit" << std::endl;
        std::cout << "Enter your choice (1-5): ";

        // Get the user's input.
        saferCin(userChoice);

        // Call the appropriate function based on the user's choice.
        if (userChoice == "1") {
            searchBookHandler();
            std::cout << "Book search complete." << std::endl;
        }
        else if (userChoice == "2") {
            addBookHandler();
            std::cout << "Book added to shopping list." << std::endl;
        }
        else if (userChoice == "3") {
            listHandler();
            std::cout << "Shopping list displayed." << std::endl;
        }
        else if (userChoice == "4") {
            addShopperHandler();
            std::cout << "Shopper account created." << std::endl;
        }
        else if (userChoice == "5") {
            break;
        }
        else {
            // Display an error message if the user's input is invalid.
            std::cout << "Invalid choice. Please enter a number from 1 to 5." << std::endl;
        }
        // Pause the program so the user can see the result message.
        std::cout << "Returning to the main menu. Press any key to continue...";
        std::cin.get();
    }
}



