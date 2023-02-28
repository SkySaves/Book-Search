#pragma once
#include "sqlite3.h"
#include <string>
#include <iostream>
#include "SaferCIN.h"
#include "Books.h"
#include "ListFunctions.h"
#include <vector>
#include "MainMenu.h"

using namespace std;



/* ---------------- Handlers ---------------- */


// A Menu option is a class that stores the name of a menu option and the function that should be called when the option is selected
void searchBookHandler()
{
    // Clear the console screen.
    system("cls");

    // Declare a menu object for selecting the search method.
    Menu searchMenu("How would you like to search: \n");
    searchMenu.addOption("Title", 0);
    searchMenu.addOption("Author", 1);
    searchMenu.addOption("ISBN", 2);
    searchMenu.addOption("Back to main menu", 3);

    // Declare variables to store the search query and search choice.
    string query;
    int searchChoice;

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Loop until the user chooses to return to the main menu.
    while (true)
    {
        // Clear the console screen.
        system("cls");

        // Display the search menu and prompt the user to enter a search query.
        searchMenu.display();
        searchMenu.getChoice(searchChoice, "Enter your search query: ");

        // Depending on the user's search choice, prompt them to enter a query for the corresponding field.
        switch (searchChoice)
        {
        case 0:
            std::cout << "Enter the title of the book: ";
            saferCin(query);
            break;
        case 1:
            std::cout << "Enter the author of the book: ";
            saferCin(query);
            break;
        case 2:
            std::cout << "Enter the ISBN of the book: ";
            saferCin(query);
            break;

            // If the user chooses to return to the main menu, return from the function.
        case 3:
            sqlite3_close(db);
            return;

        default:
            break;
        }

        // Call the searchBooks function with the user's query and search choice.
        searchBooks(query, searchChoice);

        // Pause the program and wait for the user to press a key to continue.
        cin.ignore();
        cin.get();
    }
}


void addBookHandler()
{
    // Clear the console screen.
    system("cls");

    // Declare variables to store book information.
    std::string bookTitle, bookAuthor, bookISBN, bookPublisher, bookDescription;
    int bookYear, bookMSRP, bookQuantity;

    // Prompt the user to enter book information and read the input safely.
    std::cout << "Enter the title of the book: ";
    saferCin(bookTitle);

    std::cout << "Enter the author of the book: ";
    saferCin(bookAuthor);

    std::cout << "Enter the publisher of the book: ";
    saferCin(bookPublisher);

    std::cout << "Enter the year the book was published: ";
    saferCin(bookYear);

    // Loop until the user enters an ISBN of the correct length.
    int loopAmount = 0;
    while (bookISBN.length() != 13)
    {
        // If the user has made an incorrect input previously, prompt them to try again.
        if (loopAmount > 0)
        {
            std::cout << "ISBN was not !-13-! characters.\nPlease try again: ";
        }
        // Otherwise, prompt them to enter the ISBN.
        else
        {
            std::cout << "Enter the !-13-! char ISBN of the book: ";
        }

        saferCin(bookISBN);
    }

    std::cout << "Enter the MSRP of the book: ";
    saferCin(bookMSRP);

    std::cout << "Enter the quantity of the book: ";
    saferCin(bookQuantity);

    std::cout << "Enter the description of the book: ";
    saferCin(bookDescription);

    // Add the book to the books table using the entered information.
    addBookToTable(bookISBN, bookTitle, bookAuthor, bookYear, bookPublisher, bookMSRP, bookQuantity, bookDescription);
}






// This function handles displaying the books in the database as pages.
void displayBooksHandler()
{
    // Clear the console screen.
    system("cls");


    // Display the books as pages.
    displayAsPages(books);

    // Wait for the user to press a key.
    system("pause");

}



void listHandler()
{
    // Clear the console screen.
    system("cls");

    // Declare a menu object for selecting the action to perform.
    Menu listMenu("What would you like to do: \n");
    listMenu.addOption("Add a book to the list", 0);
    listMenu.addOption("Remove a book from the list", 1);
    listMenu.addOption("Display the list", 2);
    listMenu.addOption("Back to main menu", 3);

    // Loop until the user chooses to return to the main menu.
    while (true)
    {
        // Clear the console screen.
        system("cls");

        // Display the list menu and prompt the user to enter a choice.
        listMenu.display();
        int listChoice;
        listMenu.getChoice(listChoice);

        // Depending on the user's choice, perform the corresponding action.
        switch (listChoice)
        {
        case 0:
            addToList();
            break;

        case 1:
            removeFromList();
            break;

        case 2:
            displayList();
            break;

            // If the user chooses to return to the main menu, exit the function.
        case 3:
            return;

        default:
            break;
        }
    }
}









int authUserHandler()
{
    // Clear the console screen.
    system("cls");

    // Declare variables to store the user's login information.
    std::string username;
    std::string password;
    char tryAgain = 'y';
    int retryCount = 0;
    const int maxRetryCount = 3;

    // Open the database connection.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        exit(1);
    }

    // Loop until the user enters valid login information or reaches the maximum number of retries.
    while (tryAgain == 'y' && retryCount < maxRetryCount)
    {
        // Prompt the user to enter their login information and read the input safely.
        std::cout << "Enter your username: ";
        saferCin(username);

        std::cout << "Enter your password: ";
        saferCin(password);



        // Prepare the SELECT statement
        std::string sql = "SELECT UserID FROM customerLogin WHERE Username = ? AND Password = ?";
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

        // Bind the parameters to the statement
        rc = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
        rc = sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the statement. This will verify if the user exists or not.
        bool result = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = true;
        }
        sqlite3_finalize(stmt);

        // If the user's login information is valid, exit the loop.
        if (result)
        {
            break;
        }
        else
        {
            // If the user's login information is not valid, prompt them to try again or exit.
            std::cout << "Invalid login information. Try again? (y/n): " << std::endl;
            std::cin >> tryAgain;
            std::cin.ignore();

            retryCount++;
        }
    }

    // Close the database connection.
    sqlite3_close(db);

    // If the user's login information is valid, return 1 to indicate success.
    // Note that this code assumes that the user's login information will always be valid if the while loop exits.
    return 1;
}




void registrationHandle() {
    // Declare variables to store the user's registration information.
    string username;
    string password;

    // Prompt the user to enter their registration information and read the input safely.
    std::cout << "Enter a new username: ";
    saferCin(username);
    std::cout << "Enter a new password: ";
    saferCin(password);

    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        // If the database fails to open, print an error message and return.
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Prepare the INSERT statement.
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO customerLogin (Username, Password, UserID) VALUES (?, ?, NULL)";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // If the statement fails to prepare, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind the parameters to the statement.
    rc = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        // If the first parameter fails to bind, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error binding parameter 1: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    rc = sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        // If the second parameter fails to bind, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error binding parameter 2: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Execute the statement.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        // If the statement fails to execute, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Print a message to indicate that the registration is complete.
    system("cls");
    std::cout << "Registration complete!\n";

    return;
}






