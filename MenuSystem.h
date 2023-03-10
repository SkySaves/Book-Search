#pragma once

#ifndef MENU
#define MENU

#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <limits>
#include "Login.h"

/* ----------------- METHODS ----------------- */


// saferCin is a function that takes a string or int reference as an argument and prompts the user for input.
// It uses the standard cin input stream to retrieve the input, but with the addition of ignoring any excess characters in the input buffer after the desired input has been retrieved.

static void saferCin(std::string& input)
{
    // Prompts the user for input and stores it in the input string reference.
    std::cin >> input;
    // Ignores any excess characters in the input buffer after the desired input has been retrieved.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void saferCin(int& input)
{
    // Prompts the user for input and stores it in the input int reference.
    std::cin >> input;
    // Ignores any excess characters in the input buffer after the desired input has been retrieved.
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static void pressEnter() {
    // Probably un-nessicary but oh well
    std::cin.get();
}




/* ---------------- CLASSES ---------------- */


// Class definition for a menu object that can display options and receive input from the user.
class Menu
{
private:
    // A vector of tuples containing the name of the option and its corresponding return value.
    std::vector<std::tuple<std::string, int>> options;
    // The title of the menu to be displayed.
    std::string title;

public:

    Menu() {}

    // Constructor that takes in the title of the menu as a parameter.
    Menu(std::string title)
    {
        this->title = title;
    }
    // Adds an option to the menu with the given name and return value.
    void addOption(std::string name, int returnValue)
    {
        options.push_back(std::tuple<std::string, int>(name, returnValue));
    }

    // Displays the menu options to the console.
    void display()
    {
        std::cout << title << std::endl;
        for (int i = 0; i < options.size(); i++)
        {
            std::cout << i + 1
                << ": "
                << std::get<0>(options[i])
                << std::endl;
        }
    }

    // Prompts the user to enter their choice and sets the choice parameter to the corresponding return value of the selected option.
    void getChoice(int& choice)
    {
        std::cout << "Enter your choice: ";
        saferCin(choice);

        if (choice < 1 || choice > options.size())
        {
            std::cout << "Invalid choice. Try again.";
        }
        else
        {
            choice = std::get<1>(options[choice - 1]);
        }
    }

    // Prompts the user to enter their choice with the given prompt message and sets the choice parameter to the corresponding return value of the selected option.
    void getChoice(int& choice, std::string prompt)
    {
        std::cout << prompt;
        saferCin(choice);

        if (choice < 1 || choice > options.size())
        {
            std::cout << "Invalid choice. Try again.";
        }
        else
        {
            choice = std::get<1>(options[choice - 1]);
        }
    }


    void getChoice()
    {
		int choice;
		std::cout << "Enter your choice: ";
		saferCin(choice);

        if (choice < 1 || choice > options.size())
        {
			std::cout << "Invalid choice. Try again.";
		}
        else
        {
			choice = std::get<1>(options[choice - 1]);
		}
	}



    void displayMainMenu();
    void displayLoginMenu();
    void adminMenu();


};

#endif