// This code is an implementation of a Bookstore Application.
#include <vector>
#include <string>

#include "Login.h"
#include "Books.h"
#include "MenuSystem.h"

// ISDEV is a macro that is defined to skip the login prompt if set to 1.
#define ISDEV 1

// searchBookHandler is a function that provides the user with options to search for a book in the bookstore database.
void searchBookHandler()
{
    // Clears the console screen.
    system("cls");

    // Creates an instance of a class named "Menu" with the title "How would you like to search: \n".
    Menu searchMenu("How would you like to search: \n");

    // Adds options to the menu with the name "Title", "Author", "ISBN", and "Back to main menu".
    searchMenu.addOption("Title", 0);
    searchMenu.addOption("Author", 1);
    searchMenu.addOption("ISBN", 2);
    searchMenu.addOption("Back to main menu", 3);

    // Variables to store the search query and the user's choice.
    string query;
    int searchChoice;

    // An infinite loop that displays the menu and allows the user to select an option.
    while (true)
    {
        // Clears the console screen.
        system("cls");

        // Displays the search menu.
        searchMenu.display();

        // Gets the user's choice from the search menu.
        searchMenu.getChoice(searchChoice, "Enter your search query: ");

        // Switch statement to execute the appropriate action based on the user's choice.
        switch (searchChoice)
        {
        case 0:
            // Prompts the user to enter the title of the book they want to search for.
            std::cout << "Enter the title of the book: ";
            saferCin(query);
            break;
        case 1:
            // Prompts the user to enter the author of the book they want to search for.
            std::cout << "Enter the author of the book: ";
            saferCin(query);
            break;
        case 2:
            // Prompts the user to enter the ISBN of the book they want to search for.
            std::cout << "Enter the ISBN of the book: ";
            saferCin(query);
            break;

        case 3:
            // Returns to the previous menu.
            return;

        default:
            break;
        }

        // Calls the searchBooks function with the user's search query and choice as arguments.
        searchBooks(query, searchChoice);
    }
}

// listHandler is a function that provides the user with options to create and manage a list of books.
void listHandler()
{
    // Clears the console screen.
    system("cls");

    // Creates an instance of a class named "Menu" with the title "What would you like to do: \n".
    Menu listMenu("What would you like to do: \n");

    // Adds options to the menu with the name "Add a book to the list", "Remove a book from the list", "Display the list", and "Back to main menu".
    listMenu.addOption("Add a book to the list", 0);
    listMenu.addOption("Remove a book from the list", 1);
    listMenu.addOption("Display the list", 2);
    listMenu.addOption("Back to main menu", 3);
    // An infinite loop that displays the menu and allows the user to select an option.
    while (true)
    {
        // Clears the console screen.
        system("cls");

        // Displays the list menu.
        listMenu.display();

        // Gets the user's choice from the list menu.
        int listChoice;
        listMenu.getChoice(listChoice);

        // Switch statement to execute the appropriate action based on the user's choice.
        switch (listChoice)
        {
        case 0:
            // Calls the addToList function to add a book to the list.
            addToList();
            break;

        case 1:
            // Calls the removeFromList function to remove a book from the list.
            removeFromList();
            break;

        case 2:
            // Calls the displayList function to display the books in the list.
            displayList();
            break;

        case 3:
            // Returns to the previous menu.
            return;

        default:
            break;
        }
    }
}

// addBookHandler is a function that allows the user to add a book to the bookstore database.
void addBookHandler()
{
    // Clears the console screen.
    system("cls");
    // Variables to store the book information.
    std::string bookTitle, bookAuthor, bookISBN, bookPublisher, bookDescription;
    int bookYear;

    // Prompts the user to enter the title of the book.
    std::cout << "Enter the title of the book: ";
    saferCin(bookTitle);

    // Prompts the user to enter the author of the book.
    std::cout << "Enter the author of the book: ";
    saferCin(bookAuthor);

    // Prompts the user to enter the publisher of the book.
    std::cout << "Enter the publisher of the book: ";
    saferCin(bookPublisher);

    // Prompts the user to enter the year the book was published.
    std::cout << "Enter the year the book was published: ";
    saferCin(bookYear);

    // Loop to ensure the ISBN entered is 13 characters long.
    int loopAmount = 0;
    while (bookISBN.length() != 13)
    {
        if (loopAmount > 0)
        {
            // Prompts the user to enter a valid ISBN if the previous ISBN was not 13 characters long.
            std::cout << "ISBN was not !-13-! characters.\nPlease try again: ";
        }
        else
        {
            // Prompts the user to enter the ISBN of the book.
            std::cout << "Enter the !-13-! char ISBN of the book: ";
        }

        // Reads in the ISBN from the user.
        saferCin(bookISBN);
    }

    // Calls the addBook function with the book information as arguments.
    addBook(bookISBN, bookTitle, bookAuthor, bookYear, bookPublisher);
}

// authUserHandler is a function that prompts the user to enter their username and password and authenticates them.
int authUserHandler()
{
    // Clears the console screen.
    system("cls");
    // Variables to store the user's username and password.
    string username;
    string password;
    string tryAgain;

    // Prompts the user to enter their username.
    std::cout << "Enter your username: ";
    saferCin(username);

    // Prompts the user to enter their password.
    std::cout << "Enter your password: ";
    saferCin(password);

    // An infinite loop that continues until the user's login information is authenticated.
    while (true)
    {
        // Calls the userLogin function with the user's username and password as arguments.
        if (userLogin(username, password))
        {
            // Breaks out of the loop if the user's login information is authenticated.
            break;
        }
        else
        {
            // Prompts the user to try again or exit the program if their login information is not authenticated.
            std::cout << "Invalid login information. Try again? (y/n): " << std::endl;
            saferCin(tryAgain);

            if (tryAgain == "y")
            {
                // Prompts the user to enter their username again.
                std::cout << "Enter your username: " << std::endl;
                saferCin(username);

                // Prompts the user to enter their password again.
                std::cout << "Enter your password: " << std::endl;
                saferCin(password);
            }
            else
            {
                // Exits the program.
                exit(0);
                return 0;
            }
        }
    }

    // Returns 1 since the user's login information must be authenticated to reach this point.
    return 1;
}

// The main function is the entry point of the program.
int main()
{
    // Creates an instance of a class named "Menu" with the title "What would you like to do: \n".
    Menu mainMenu("What would you like to do: \n");
    // Adds options to the menu with the name "Search for a book", "Add a book to the database", "Create a list", and "Exit".
    mainMenu.addOption("Search for a book", 0);
    mainMenu.addOption("Add a book to the database", 1);
    mainMenu.addOption("Create a list", 2);
    mainMenu.addOption("Exit", 3);

    // Welcome message.
    std::cout << "Welcome to the CIS 227 Bookstore. Please enter your login information." << endl;

    // If ISDEV is not set to 1, the user is prompted to enter their login information.
    if (!ISDEV)
    {
        authUserHandler();
    }

    // Prints a message indicating that the books are being read from the file.
    std::cout << "Reading books from file. Please wait... \n"
        << std::endl;
    readBooks();

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
        case 0:
            // Calls the searchBookHandler function to search for a book.
            searchBookHandler();
            break;
        case 1:
            // Calls the addBookHandler function to add a book to the database.
            addBookHandler();
            break;
        case 2:
            // Calls the listHandler function to create a list of books.
            listHandler();
            break;
        case 3:
            // Exits the program.
            exit(0);
            break;
        }
    }
}
