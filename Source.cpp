#include <vector>
#include <string>

#include "Login.h"
#include "Books.h"
#include "MenuSystem.h"

#define ISDEV 1 // Skips login prompt if set to 1 -> 0 for production

// A Menu option is a class that stores the name of a menu option and the function that should be called when the option is selected
void searchBookHandler()
{
    system("cls");

    Menu searchMenu("How would you like to search: \n");
    searchMenu.addOption("Title", 0);
    searchMenu.addOption("Author", 1);
    searchMenu.addOption("ISBN", 2);
    searchMenu.addOption("Back to main menu", 3);

    string query;
    int searchChoice;

    while (true)
    {
        system("cls");

        searchMenu.display();
        searchMenu.getChoice(searchChoice, "Enter your search query: ");

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

        case 3:
            return;

        default:
            break;
        }

        searchBooks(query, searchChoice);
    }
}

void listHandler()
{
    system("cls");

    Menu listMenu("What would you like to do: \n");
    listMenu.addOption("Add a book to the list", 0);
    listMenu.addOption("Remove a book from the list", 1);
    listMenu.addOption("Display the list", 2);
    listMenu.addOption("Back to main menu", 3);

    while (true)
    {
        system("cls");

        listMenu.display();
        int listChoice;
        listMenu.getChoice(listChoice);

        if (listChoice == 0)
        {
            addToList();
        }
        else if (listChoice == 1)
        {
            removeFromList();
        }
        else if (listChoice == 2)
        {
            displayList();
        }
        else if (listChoice == 3)
        {
            break;
        }
        else
        {
            std::cout << "Invalid choice. Try again." << endl;
        }
    }
}

void addBookHandler()
{
    system("cls");

    std::string bookTitle, bookAuthor, bookISBN, bookPublisher, bookDescription;
    int bookYear;

    std::cout << "Enter the title of the book: ";
    saferCin(bookTitle);

    std::cout << "Enter the author of the book: ";
    saferCin(bookAuthor);

    std::cout << "Enter the publisher of the book: ";
    saferCin(bookPublisher);

    std::cout << "Enter the year the book was published: ";
    saferCin(bookYear);

    int loopAmount = 0;
    while (bookISBN.length() != 13)
    {
        if (loopAmount > 0)
        {
            std::cout << "ISBN was not !-13-! characters.\nPlease try again: ";
        }
        else
        {
            std::cout << "Enter the !-13-! char ISBN of the book: ";
        }

        saferCin(bookISBN);
    }

    addBook(bookISBN, bookTitle, bookAuthor, bookYear, bookPublisher);
}

int authUserHandler()
{
    system("cls");

    string username;
    string password;
    string tryAgain;

    std::cout << "Enter your username: ";
    saferCin(username);

    std::cout << "Enter your password: ";
    saferCin(password);

    while (true)
    {
        if (userLogin(username, password))
        {
            break;
        }
        else
        {
            std::cout << "Invalid login information. Try again? (y/n): " << std::endl;
            saferCin(tryAgain);

            if (tryAgain == "y")
            {
                std::cout << "Enter your username: " << std::endl;
                saferCin(username);

                std::cout << "Enter your password: " << std::endl;
                saferCin(password);
            }
            else
            {
                exit(0);
                return 0;
            }
        }
    }

    // Has to succeed, so we can always return 1
    return 1;
}

int main()
{
    Menu mainMenu("What would you like to do: \n");
    mainMenu.addOption("Search for a book", 0);
    mainMenu.addOption("Add a book to the database", 1);
    mainMenu.addOption("Create a list", 2);
    mainMenu.addOption("Exit", 3);

    // Welcome message
    std::cout << "Welcome to the CIS 227 Bookstore. Please enter your login information." << endl;

    // ISDEV is defined at the top
    if (!ISDEV)
    {
        authUserHandler();
    }

    // Can only get here if the user is authenticated
    // So everything after is auth safe
    std::cout << "Reading books from file. Please wait... \n"
              << std::endl;
    readBooks();

    // Main Loop, switch statement is switching on return value of the menu options
    while (true)
    {
        system("cls");

        int choice;
        mainMenu.display();
        mainMenu.getChoice(choice);

        switch (choice)
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
            exit(0);
            break;
        }
    }
}
