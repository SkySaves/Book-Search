#include <vector>
#include <string>
#include <iostream>

#include "Books.h"
#include "Login.h" // Login.h is the header file for the login functions
#include "MenuSystem.h" // MenuSystem.h is the header file for the menu system
#include "Admin.h" // adminMenu.h is the header file for the admin menu
#include "Handlers.h"
#include "MainMenu.h"



#define ISDEV 0 // Skips login prompt if set to 1 -> 0 for production

int main()
{
    int userChoice;

    // Welcome message
    std::cout << "Welcome to the CIS 227 Bookstore. Please enter your login information." << endl;

    // ISDEV is defined at the top
    if (!ISDEV)
    {
        // Login Loop

        int loginChoice = 0;

        while (loginChoice < 2 || loginChoice > 3) {
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
                if (authUserHandler() == 1) {
                    std::cout << "Success!" << endl;
                    std::cout << "Reading books from file. Please wait... \n"
                        << std::endl;
                    readBooks();

                    displayMainMenu();
                }
               
                userChoice = 2;
                break;
            case 3:
                if (adminLoginHandler() == 1) {
					//display admin menu
                    adminMenu();
				}
                break;
            default:
                //clear cosole
                system("cls");
                std::cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
    }


    // Return statement at the end of the function
    return 0;
}
