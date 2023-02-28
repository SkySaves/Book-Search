#ifndef BooksHeader
#define BooksHeader

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <random>
#include <iomanip>
#include <conio.h>
#include "sqlite3.h"
#include <tuple>
#include "Login.h"
#include "Shoppers.h"
#include "SaferCIN.h"
#include "ListFunctions.h"
#include "Handlers.h"
#include "MainMenu.h"



using namespace std;



/* ---------------- CLASSES ---------------- */

// This defines the class "Book" which has 6 data members : ISBN, title, author, publisher, year, and description.
// All of these variables are of type string except for year which is an integer.
class Book {
public:
    string ISBN;
    string title;
    string author;
    int year;
    string publisher;
    int MSRP;
    int quantity;
    string description;
};


/* ---------------- GLOBALS ---------------- */

// Empty vector of type Book named "books". This is where the output will be stored.
vector<Book> books; // Main database of books

// This is a global vector of Book objects which is used to store books that are added to the list.
vector<Book> bookList; // Used by the "make a list" functionality

sqlite3_stmt* stmt;

/* ---------------- METHODS ---------------- */








void readBooks()
{
    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    //  This code prepares a SQL SELECT statement to retrieve all rows and columns from a "books" table in a SQLite database, and handles any errors that occur during the preparation process.
    string sql = "SELECT * FROM books";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Loop through the results of the SELECT statement and create Book objects. This code retrieves the value of the first column of the current row of a SQLite SELECT statement result set and assigns it to the ISBN field of a book object. If the column value is null, an empty string is assigned to the ISBN field.
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // A Book object to store the information of the book.
        Book book;

        // Get the columns of the current row of the SELECT statement and store them in the Book object.
        const char* isbnPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        if (isbnPtr != nullptr) {
            book.ISBN = isbnPtr;
        }
        else {
            book.ISBN = "";
        }

        const char* titlePtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        if (titlePtr != nullptr) {
            book.title = titlePtr;
        }
        else {
            book.title = "";
        }

        const char* authorPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        if (authorPtr != nullptr) {
            book.author = authorPtr;
        }
        else {
            book.author = "";
        }

        book.year = sqlite3_column_int(stmt, 3);

        const char* publisherPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        if (publisherPtr != nullptr) {
            book.publisher = publisherPtr;
        }
        else {
            book.publisher = "";
        }

        book.MSRP = sqlite3_column_int(stmt, 5);
        book.quantity = sqlite3_column_int(stmt, 6);

        const char* descriptionPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        if (descriptionPtr != nullptr) {
            book.description = descriptionPtr;
        }
        else {
            book.description = "";
        }

        // Adds the Book object to the books vector.
        books.push_back(book);
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}







// Creates pages of books and displays them to the user
// displayAsPages is a function that takes in a vector of Book objects and displays them one page at a time.
void displayAsPages(vector<Book>& books)
{



    // The number of books displayed on each page.
    int PAGESIZE = 1;
    // The current page number.
    int page = 0;

    // A string to store the user's input.
    string menuInput = "";

    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK)
    {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Execute a SELECT statement to retrieve the book data.

    books.clear();

    const char* sql = "SELECT ISBN, [Book-Title], [Book-Author], [Year-Of-Publisher], Publisher, MSRP, Quantity, Description FROM books";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Iterate through the results and populate the books vector.
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        Book book;
        book.ISBN = sqlite3_column_text(stmt, 0) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)) : "";
        book.title = sqlite3_column_text(stmt, 1) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) : "";
        book.author = sqlite3_column_text(stmt, 2) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) : "";
        book.year = sqlite3_column_int(stmt, 3);
        book.publisher = sqlite3_column_text(stmt, 4) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)) : "";
        book.MSRP = sqlite3_column_int(stmt, 5);
        book.quantity = sqlite3_column_int(stmt, 6);
        book.description = sqlite3_column_text(stmt, 7) ? reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7)) : "";
        books.push_back(book);
    }
    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // An infinite loop that continues until the user inputs "exit".
    while (true)
    {
        // Clears the console screen.
        system("cls");

        // Loops through the books on the current page.
        for (int i = page * PAGESIZE; i < (page + 1) * PAGESIZE; i++)
        {
            // Breaks out of the loop if the end of the books vector has been reached.
            if (i >= books.size())
            {
                break;
            }
            // Prints the ISBN, title, author of each book on the current page.
            std::cout << "ISBN: " << books[i].ISBN << endl;
            std::cout << "Title: " << books[i].title << endl;
            std::cout << "Author: " << books[i].author << endl;
            std::cout << std::endl
                << std::endl;
        }

        // Calculates the total number of pages based on the number of books and the PAGESIZE.
        double pages = books.size() / PAGESIZE;
        pages = pages == 0 ? 1 : pages;

        // Prints the current page number and the total number of pages.
        std::cout << "Page [" << page + 1 << "] of [" << pages << "]" << endl;

        // Prompts the user to press enter to go to the next page or type "exit"
        std::cout << "Press enter to go to the next page or type \"exit\" to exit: ";

        // Gets the user's input.
        std::getline(std::cin, menuInput);

        // Exits the loop if the user inputs "exit".
        if (menuInput == "exit")
        {
            // Clears the console screen.
            system("cls");
            break;
        }

        // Increments the page number.
        page++;
        // Resets the page number to 0 if the end of the books vector has been reached.
        if (page * PAGESIZE >= books.size())
        {
            page = 0;
        }
    }
    }




    void searchBooks(string search, int searchChoice)
    {
        // Open the database.
        sqlite3* db;
        int rc = sqlite3_open("bookstore.db", &db);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }
        // Construct the SQL statement based on the user's search choice.
        std::string columnName;
        switch (searchChoice) {
        case 0:
            columnName = "Book-Title";
            break;
        case 1:
            columnName = "Book-Author";
            break;
        case 2:
            columnName = "ISBN";
            break;
        default:
            return;
        }

        // Prepare the SELECT statement.
        std::stringstream ss;
        ss << "SELECT * FROM books WHERE `" << columnName << "` LIKE ?";
        std::string sql = ss.str();

        std::cout << "SQL statement: " << sql << std::endl; // add this line to print out the constructed SQL query

        // Prepare the SQL statement.
        sqlite3_stmt* stmt;
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Bind the search term to the SQL statement.
        rc = sqlite3_bind_text(stmt, 1, ("%" + search + "%").c_str(), -1, SQLITE_TRANSIENT);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            std::cerr << "Error binding parameter: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Loop through the results of the SELECT statement and create Book objects.
        vector<Book> books;
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Book book;

            // Handle null pointers using if statements.
            const char* isbnPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            if (isbnPtr != nullptr) {
                book.ISBN = isbnPtr;
            }
            else {
                book.ISBN = "";
            }

            const char* titlePtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            if (titlePtr != nullptr) {
                book.title = titlePtr;
            }
            else {
                book.title = "";
            }

            const char* authorPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            if (authorPtr != nullptr) {
                book.author = authorPtr;
            }
            else {
                book.author = "";
            }

            book.year = sqlite3_column_int(stmt, 3);

            const char* publisherPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
            if (publisherPtr != nullptr) {
                book.publisher = publisherPtr;
            }
            else {
                book.publisher = "";
            }

            book.MSRP = sqlite3_column_int(stmt, 5);
            book.quantity = sqlite3_column_int(stmt, 6);

            const char* descriptionPtr = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
            if (descriptionPtr != nullptr) {
                book.description = descriptionPtr;
            }
            else {
                book.description = "";
            }

            books.push_back(book);
        }
        // Finalize the statement and close the database.
        sqlite3_finalize(stmt);
        sqlite3_close(db);

        // If no books were found, print a message to the user.
        if (books.empty()) {
            char searchAgain;
            std::cout << "No books found matching the search criteria." << std::endl;
            std::cout << "Search again? y/n" << std::endl;
            saferCin(searchAgain);
            if (searchAgain == 'y') {
                searchBookHandler();
			}
            else {
				return;
			}
        }
        displayAsPages(books);
}
     



bool userExists(std::string username) {
    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Prepare the SELECT statement.

    std::string sql = "SELECT COUNT(*) FROM users WHERE username = ?";

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the username parameter to the SELECT statement.
    rc = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error binding username parameter: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Execute the SELECT statement and get the result.
    bool exists = false;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt, 0);
        if (count > 0) {
            exists = true;
        }
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return exists;
}




// Function to add a book to the "books" table in the "bookstore.db" database.
void addBookToTable(string ISBN, string title, string author, int year, string publisher, int price, int quantity, string description)
{
    // Open the "bookstore.db" database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);

    if (rc != SQLITE_OK) {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Create an SQL query to insert a new book into the "books" table.
    string sql = "INSERT INTO books (ISBN, Book_Title, Book_Author, Year_Of_Publisher, Publisher, MSRP, Quantity, Description) "
        "VALUES ('" + ISBN + "', '" + title + "', '" + author + "', '" + to_string(year) + "', '" + publisher + "', '" + to_string(price) + "', '" + to_string(quantity) + "', '" + description + "');";

    // Execute the SQL query.
    char* errMsg = NULL;
    rc = sqlite3_exec(db, sql.c_str(), NULL, NULL, &errMsg);

    if (rc != SQLITE_OK) {
        cerr << "Error executing SQL query: " << errMsg << endl;
        sqlite3_free(errMsg);
    }

    // Close the database connection.
    sqlite3_close(db);

    cout << "Book added to the table." << endl;
}





// Function to add a book to the "books" table in the "bookstore.db" database.
// does the same thing as the modified addBookToTable() function. It also prompts the user to enter the book's information, opens the database, prepares and executes an INSERT statement, and prints a success message. However, it uses a different method to insert the book's data into the database, using sqlite3_bind_text() and sqlite3_bind_int() to bind the parameters to the statement.
void addBook()
{
    // Declaring variables to store the ISBN, book title, book author, year of publication, publisher, MSRP, quantity, and description.
    string ISBN;
    string bookTitle;
    string bookAuthor;
    int yearOfPublication;
    string publisher;
    int MSRP;
    int quantity;
    string description;

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Getting the ISBN from the user
    // A do-while loop that continues until a valid ISBN is entered by the user.
    // The ISBN should be 13 digits long.
    do
    {
        cout << "Enter ISBN (13 digits): ";
        cin >> ISBN;
        // If the length of the entered ISBN is not 13, the user is prompted to enter a valid ISBN.
        if (ISBN.length() != 13)
        {
            cout << "Invalid ISBN. Try again." << endl;
        }
    } while (ISBN.length() != 13);

    // Getting the book title, author, year of publication, publisher, MSRP, quantity, and description from the user.
    cout << "Enter Book Title: ";
    cin >> bookTitle;
    cout << "Enter Book Author: ";
    // Clearing the input buffer to avoid any unwanted input.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, bookAuthor);
    cout << "Enter Year of Publication: ";
    cin >> yearOfPublication;
    cout << "Enter Publisher: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, publisher);
    cout << "Enter MSRP: ";
    cin >> MSRP;
    cout << "Enter Quantity: ";
    cin >> quantity;
    cout << "Enter Description: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, description);

    // Prepare the INSERT statement

    string sql = "INSERT INTO books(ISBN, Book_Title, Book_Author, Year_Of_Publisher, Publisher, MSRP, Quantity, Description) VALUES(?, ?, ?, ?, ?, ?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind the parameters to the statement
    rc = sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, bookTitle.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 3, bookAuthor.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_int(stmt, 4, yearOfPublication);
    rc = sqlite3_bind_text(stmt, 5, publisher.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement and check if it was successful
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error inserting row: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Printing a success message to let the user know that the book was added successfully.
    cout << "Book added successfully!" << endl;
    cout << "Re-building book list..." << endl;
    readBooks();

}





void deleteBook()
{
    // Variables to store the book information
    string ISBN;

    // Loop that continues until the user inputs a valid ISBN (13 digits)
    do
    {
        cout << "Enter ISBN of book to delete (13 digits): ";
        cin >> ISBN;

        // If the ISBN entered is not 13 digits, display an error message and ask the user to try again
        if (ISBN.length() != 13)
        {
            cout << "Invalid ISBN. Try again." << endl;
        }
    } while (ISBN.length() != 13);

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Prepare the DELETE statement

    string sql = "DELETE FROM books WHERE ISBN=?;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind the ISBN parameter to the statement
    rc = sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the DELETE statement
    rc = sqlite3_step(stmt);

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Display a success message to the user
    cout << "Book deleted successfully!" << endl;
}

/**
 * choices() function allows the user to make a choice of what they would like to do:
 * 1. Search for a book
 * 2. Add a book to the database
 * 3. Create a list
 * 4. Exit
 * It takes the user's choice as input and calls the relevant function for that choice
 * If the user input an invalid choice, the function will ask for a new choice again.
 */
void choices()
{
    system("CLS");
    cout << "What would you like to do?\n1. Search for a book\n2. Add a book to the database\n3. Create a list\n4. Exit\nEnter your choice: ";

    int choice;
    cin >> choice;

    // If the user chooses to search for a book
    if (choice == 1)
    {
        string search;
        cout << "Search: ";
        cin >> search;

        // Call the searchBooks function
        searchBooks(search, 0);
    }
    // If the user chooses to add a book to the database
    else if (choice == 2)
    {
        system("CLS");
        // Call the addBook function
        addBook();
    }
    // If the user chooses to create a list (currently not implemented)
    else if (choice == 3)
    {
        // code to create list
    }
    // If the user chooses to exit
    else if (choice == 4)
    {
        return;
    }
    // If the user input an invalid choice
    else
    {
        cout << "Invalid choice. Try again." << endl;
        cin.ignore();
        cin.get();
        // Call the choices function again to ask for a new choice
        choices();
    }

    cout << "Press enter to exit" << endl;
    return;
}





// This function searches the database for the book with the given ISBN and returns the row ID of the book if found, and -1 if the book is not found.
int searchBookIndex(string ISBN)
{
    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // Prepare the SELECT statement

    string sql = "SELECT rowid FROM books WHERE ISBN = ?;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return -1;
    }

    // Bind the ISBN parameter to the statement
    rc = sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement and fetch the row ID
    int rowID = -1;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        rowID = sqlite3_column_int(stmt, 0);
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return rowID;
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
};











void displayList()
{
    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Execute a SELECT statement to retrieve the book data sorted by price.
    std::vector<Book> books;
    const char* sql = "SELECT * FROM books ORDER BY MSRP";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Iterate through the results and populate the books vector.
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Book book;
        book.ISBN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        book.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        book.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        book.year = sqlite3_column_int(stmt, 3);
        book.publisher = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        book.MSRP = sqlite3_column_double(stmt, 5);
        book.quantity = sqlite3_column_int(stmt, 6);
        book.description = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 7));
        books.push_back(book);
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Display the books.
    if (books.empty()) {
        std::cout << "No books found." << std::endl;
        return;
    }
    std::cout << "ISBN\tTitle\tAuthor\tYear\tPublisher\tPrice\tQuantity\tDescription" << std::endl;
    for (const Book& book : books) {
        std::cout << book.ISBN << '\t' << book.title << '\t' << book.author << '\t'
            << book.year << '\t' << book.publisher << '\t' << book.MSRP << '\t'
            << book.quantity << '\t' << book.description << std::endl;
    }

    // Save the books to the shopping list.
    std::cout << "Do you want to save the shopping list to the database? (Y/N) ";
    std::string answer;
    std::getline(std::cin, answer);
    if (answer == "Y" || answer == "y") {
        rc = sqlite3_open("bookstore.db", &db);
        if (rc != SQLITE_OK) {
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Delete the existing shopping list.
        sql = "DELETE FROM shoppingList";
        rc = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
        if (rc != SQLITE_OK) {
            std::cerr << "Error deleting shopping list: " << sqlite3_errmsg(db) << std::endl;
            sqlite3_close(db);
            return;
        }

        // Insert the new shopping list.
        for (const Book& book : books) {
            char* errorMessage = nullptr;
            std::string sql = "INSERT INTO shoppingList (ISBN, [Book-Title], [Book-Author], [Year-Of-Publisher], Publisher, MSRP, Quantity, Description) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
            rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
            if (rc != SQLITE_OK) {
                std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return;
            }

            sqlite3_bind_text(stmt, 1, book.ISBN.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, book.title.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, book.author.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 4, book.year);
            sqlite3_bind_text(stmt, 5, book.publisher.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 6, book.MSRP);
            sqlite3_bind_int(stmt, 7, book.quantity);
            sqlite3_bind_text(stmt, 8, book.description.c_str(), -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                std::cerr << "Error inserting book into shopping list: " << sqlite3_errmsg(db) << std::endl;
                sqlite3_finalize(stmt);
                sqlite3_close(db);
                return;
            }

            sqlite3_finalize(stmt);
        }

        // Close the database.
        sqlite3_close(db);

        std::cout << std::endl << "Shopping list saved." << std::endl;

        // Display the main menu.
        displayMainMenu();
    }
}





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







//populates the MSRP.
/* int msrp() {
    // Open the books.csv file
    ifstream inFile("books.csv");
    if (!inFile) {
        cerr << "Unable to open file books.csv";
        exit(1);
    }

    // Open a new file to write the updated data
    ofstream outFile("books_updated.csv");
    if (!outFile) {
        cerr << "Unable to open file books_updated.csv";
        exit(1);
    }

    // Write the header row to the output file
    string header;
    getline(inFile, header);
    header += ",MSRP";
    outFile << header << endl;

    // Use a random number generator to generate the MSRP values
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(34.99, 103.97);

    // Loop through each line in the input file
    string line;
    while (getline(inFile, line)) {
        // Split the line into columns
        string isbn, title, author, year, publisher;
        istringstream ss(line);
        getline(ss, isbn, ',');
        getline(ss, title, ',');
        getline(ss, author, ',');
        getline(ss, year, ',');
        getline(ss, publisher, ',');

        // Generate a random MSRP value
        double msrp = dis(gen);

        // Write the updated line to the output file
        outFile << isbn << "," << title << "," << author << "," << year << "," << publisher << "," << fixed << setprecision(2) << msrp << endl;
    }

    // Close the files
    inFile.close();
    outFile.close();

    return 0;
} */










#endif