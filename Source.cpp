#include <vector>
#include <string>
#include <iostream>

#include "Books.h"
#include "Login.h" // Login.h is the header file for the login functions
#include "MenuSystem.h" // MenuSystem.h is the header file for the menu system
#include "adminMenu.h" // adminMenu.h is the header file for the admin menu



#define ISDEV 0 // Skips login prompt if set to 1 -> 0 for production

int main()
{
    Menu mainMenu("What would you like to do: \n");
    mainMenu.addOption("Search for a book", 0);
    mainMenu.addOption("Add a book to the database", 1);
    mainMenu.addOption("Create a list", 2);
    mainMenu.addOption("Exit", 3);

    int userChoice = 0;

    // Welcome message
    std::cout << "Welcome to the CIS 227 Bookstore. Please enter your login information." << endl;

    // ISDEV is defined at the top
    if (!ISDEV)
    {
        int loginChoice;

        cout << "1. Register\n";
        cout << "2. Customer Login\n";
        cout << "3. Admin Login\n"; //Admin login not working yet. 
        cin >> loginChoice;

        switch (loginChoice)
        {
        case 1:
            registrationHandle();
            userChoice = 1;
            break;
        case 2:
            authUserHandler();
            userChoice = 2;
            break;
        case 3:
            adminLoginHandler();
            userChoice = 3;
            break;
        }

        // Can only get here if the user is authenticated
        // So everything after is auth safe
        std::cout << "Reading books from file. Please wait... \n"
            << std::endl;
        readBooks();

        // Main Loop, switch statement is switching on return value of the menu options
        if (userChoice == 1 || userChoice == 2) {
            while (true)
            {
                system("cls");

                displayMainMenu();

                userChoice = 0;

                switch (userChoice)
                {
                case 0:
                    searchBookHandler();
                    break;
                case 1:
                    addBookHandler();
                    break;
                case 2:
                    listHandler();
                    break;
                case 3:
                    std::cout << "Goodbye!" << endl;
                    return 0;
                default:
                    std::cout << "Invalid choice. Please try again." << endl;
                }

                std::cout << "Press enter to continue...";
                std::cin.ignore();
                std::cin.get();
            }
        }
        else if (userChoice == 3) {
            int adminChoice;
            cout << "Welcome to the Admin Menu!" << endl;
            cout << "Enter your choice" << endl;
            cout << "1. Add a new user." << endl;
            cout << "2. Change a user's password" << endl;
            cin >> adminChoice;

            switch (adminChoice)
            {
            case 1:
                adminAddUser();
                break;
            case 2:
                adminChangePassword();
                break;
            }

        }
    }

    return 0;
}
