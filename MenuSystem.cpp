#include "MenuSystem.h"



void Menu::adminMenu() {
    while (true) {
        // Clears the console screen.
        system("cls");
        Login login;

        // Display the admin menu options.
        cout << "Admin Menu" << endl;
        cout << "Please choose an option:" << endl;
        cout << "1. Add user in customerLogin" << endl;
        cout << "2. Change customer password" << endl;
        cout << "3. Change admin password" << endl;
        cout << "4. Import files" << endl;
        cout << "5. Return to main menu" << endl;
        cout << "Enter your choice (1-5): ";

        // Get the user's input.
        string adminChoice;
        getline(cin, adminChoice);

        // Call the appropriate function based on the admin's choice.
        if (adminChoice == "1") {
            login.registrationHandle();
        }
        else if (adminChoice == "2") {
            login.changeCustomerPasswordHandler();
        }
        else if (adminChoice == "3") {
            login.changeAdminPasswordHandler();
        }
        else if (adminChoice == "4") {
            login.importFilesHandler();
        }
        else if (adminChoice == "5") {
            // Return to main menu
            displayMainMenu();
        }
        else {
            // Display an error message if the admin's input is invalid.
            cout << "Invalid choice. Please enter a number from 1 to 5." << endl;
            // Pause the program so the admin can see the error message.
            system("pause");
        }
    }
}










void Menu::displayLoginMenu() {
    Book userbook;
    // ISDEV is a macro that is defined to skip the login prompt if set to 1.
#define ISDEV 0


    // If ISDEV is not set to 1, the user is prompted to enter their login information.
    if (!ISDEV)
    {
        int loginChoice = 0;
        int userChoice;
        Login login;


        while (loginChoice < 2 || loginChoice > 3) {
            cout << "1. Register\n";
            cout << "2. Customer Login\n";
            cout << "3. Admin Login\n"; //Admin login not working yet. 
            cin >> loginChoice;

            switch (loginChoice)
            {
            case 1:
                login.registrationHandle();
                userChoice = 1;
                break;
            case 2:
                if (login.authUserHandler() == 1) {
                    std::cout << "Success!" << endl;

                    return;
                }

                userChoice = 2;
                break;
            case 3:
                if (login.adminLoginHandler() == 1) {
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
}

void Menu::displayMainMenu() {


// Creates an instance of a class named "Handler".
    /*Handler Handles;*/
    // Creates an instance of a class named "Shopping".
    Shopping shoppings;
    // Creates an instance of a class named "Book".
    Book bookss;

    // Creates an instance of a class named "Menu" with the title "What would you like to do: \n".
    Menu mainMenu("What would you like to do: \n");
    // Adds options to the menu with the name "Search for a book", "Add a book to the database", "Create a list", and "Exit".
    mainMenu.addOption("Search for a book", 1);
    mainMenu.addOption("Add a book to the shopping list", 2);
    mainMenu.addOption("View the shopping list", 3);
    mainMenu.addOption("Create a shopper account", 4);
    mainMenu.addOption("Exit", 5);




    // Prints a message indicating that the books are being read from the file.
    std::cout << "Reading books from file. Please wait... \n"
        << std::endl;
    bookss.readBooks();

    // An infinite loop that displays the main menu and allows the user to select an option.
    while (true)
    {
        // Clears the console screen.
        system("cls");

        // Displaysthe main menu.
        mainMenu.display();
        // Gets the user's choice from the main menu.
        int choice;
        mainMenu.getChoice(choice);

        // Switch statement to execute the appropriate action based on the user's choice.
        switch (choice)
        {
        case 1:
            // Calls the searchBookHandler function to search for a book.
            bookss.searchBookHandler();
            break;
        case 2:
            std::cout << "Loading..." << endl;

            // Calls the addBookHandler function to add a book to the database.
            shoppings.addToShoppingListHandler();
            break;
        case 3:
            // Calls the listHandler function to create a list of books.
            shoppings.displayShoppingList();
            break;
        case 4:
            shoppings.addShopperHandler();

        case 5:
            // Exits the program.
            exit(0);
            break;
        }
    }
}

