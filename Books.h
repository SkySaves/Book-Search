#ifndef BooksHeader
#define BooksHeader

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
using namespace std;
/* ---------------- CLASSES ---------------- */

// This defines the class "Book" which has 6 data members : ISBN, title, author, publisher, year, and description.
// All of these variables are of type string except for year which is an integer.
class Book
{

public:
    string title;
    string author;
    string publisher;
    string ISBN;
    string description;
    int year;
};

/* ---------------- GLOBALS ---------------- */

// Empty vector of type Book named "books". This is where the output will be stored.
vector<Book> books; // Main CSV of books

// This is a global vector of Book objects which is used to store books that are added to the list.=
vector<Book> bookList; // Used by the make a list functionality

/* ---------------- METHODS ---------------- */

/**************************************************************************

readBooks - This function reads the books data from a csv file named books.csv and stores the information in the books vector.

This function uses an ifstream object named booksFile to open the file
named books.csv. The file is read line by line using the getline function
and each line is stored in the variable named line.
The function then uses the find function to locate the positions of the
commas in each line. These positions are stored in the variables i1, i2,
i3, i4 and i5.

A Book object named book is then created and the values of ISBN, title,
author, publisher, year and description are extracted from the line string
and stored in the book object. This process is done using stringstream and
the substr function.

Finally, the book object is added to the books vector. This process is
repeated until all the lines in the file have been read.
Once all the books have been added to the books vector, the booksFile
object is closed and the function returns.

The readBooks function assumes that the books.csv file exists and is
formatted correctly. If the file does not exist or if the format is
incorrect, the function will produce unexpected results.
*************************************************************************/
void readBooks()
{
    // Opens the "books.csv" file in input mode.
    ifstream booksFile("books.csv");
    // A string variable to store each line read from the file.
    string line;

    // A while loop that continues until the end of the file is reached.
    while (getline(booksFile, line))
    {
        // Variables that store the position of commas in the line.
        int i1 = line.find(",");
        int i2 = line.find(",", i1 + 1);
        int i3 = line.find(",", i2 + 1);
        int i4 = line.find(",", i3 + 1);
        int i5 = line.find(",", i4 + 1);

        // A Book object to store the information of the book.
        Book book;

        // A stringstream object to convert the year field from string to integer.
        stringstream ss;
        ss << line.substr(0, i1);
        ss >> book.ISBN;
        book.title = line.substr(i1 + 1, i2 - i1 - 1);
        book.author = line.substr(i2 + 1, i3 - i2 - 1);
        book.publisher = line.substr(i3 + 1, i4 - i3 - 1);
        ss.clear();
        ss << line.substr(i4 + 1, i5 - i4 - 1);
        ss >> book.year;
        book.description = line.substr(i5 + 1);
        // Adds the Book object to the books vector.
        books.push_back(book);
    }
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
            // Prints the ISBN, title, and author of each book on the current page.
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
        cout << "Page ["
            << page + 1
            << "] of ["
            << pages
            << "]"
            << endl;

        // Prompts the user to press enter to go to the next page or type "exit" to exit.
        cout << "Press enter to go to the next page" << endl;
        cout << "Type 'exit' to exit" << endl;

        // Gets the user's input.
        getline(cin, menuInput);

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




// searchBooks is a function that takes in a search query and a search choice, and returns a vector of books that match the query.
void searchBooks(string search, int searchChoice)
{
    // A vector to store the books that match the search query.
    vector<Book> foundBooks = vector<Book>();

    // Loops through all the books in the books vector.
    for (Book book : books)
    {
        // Uses the searchChoice to determine how to search for books.
        switch (searchChoice)
        {
        case 0:
            // Searches for books by title.
            if (book.title.find(search) != string::npos)
            {
                // Adds the book to the foundBooks vector if it matches the search query.
                foundBooks.push_back(book);
            }
            break;
        case 1:
            // Searches for books by author.
            if (book.author.find(search) != string::npos)
            {
                // Adds the book to the foundBooks vector if it matches the search query.
                foundBooks.push_back(book);
            }
            break;
        case 2:
            // Searches for books by ISBN.
            if (book.ISBN.find(search) != string::npos)
            {
                // Adds the book to the foundBooks vector if it matches the search query.
                foundBooks.push_back(book);
            }
            break;

        default:
            break;
        }
    }

    // Calls the displayAsPages function to display the found books one page at a time.
    displayAsPages(foundBooks);
}


// This function is used to add new books to the library by taking input from the user and writing the information to the "books.csv" file.
void addBook()
{
    // Declaring variables to store the ISBN, book title, book author, year of publication, and publisher.
    string ISBN;
    string bookTitle;
    string bookAuthor;
    string yearOfPublication;
    string publisher;
    // Creating an ofstream object to open the "books.csv" file in append mode.
    ofstream bookFile;
    bookFile.open("books.csv", ios::app);
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
    // Getting the book title, author, year of publication, and publisher from the user.
    cout << "Enter Book Title: ";
    cin >> bookTitle;
    cout << "Enter Book Author: ";
    // Clearing the input buffer to avoid any unwanted input.
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, bookAuthor);
    cout << "Enter Year of Publication: ";
    cin >> yearOfPublication;
    cout << "Enter Publisher: ";
    getline(cin, publisher);

    // Writing the information to the "books.csv" file, separating each field by a comma.
    bookFile << ISBN << "," << bookTitle << "," << bookAuthor << "," << yearOfPublication << "," << publisher << endl;
    // Closing the "books.csv" file.
    bookFile.close();
    // Printing a success message to let the user know that the book was added successfully.

    cout << "Book added successfully!" << endl;
    cout << "Re-building book list..." << endl;
    readBooks();
}




// addBook is a function that takes in the ISBN, title, author, year, and publisher of a book, and adds the book to the books.csv file.
void addBook(string ISBN, string title, string author, int year, string publisher)
{
    // Opens the books.csv file in append mode.
    ofstream bookFile;
    bookFile.open("books.csv", ios::app);

    // Writes the ISBN, title, author, year, and publisher of the book to the file.
    bookFile << ISBN << "," << title << "," << author << "," << year << "," << publisher << endl;

    // Closes the file.
    bookFile.close();

    // Confirms that the book was added successfully.
    cout << "Book added successfully!" << endl;

    // Re-reads the books from the books.csv file.
    cout << "Re-building book list..." << endl;
    readBooks();
}



void deleteBook()
{
    // Variables to store the book information
    string ISBN;
    string line;

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

    // Open the books.csv file for reading
    ifstream bookFile;
    bookFile.open("books.csv");

    // Open a temporary file for writing
    ofstream tempFile;
    tempFile.open("temp.csv");

    // Loop that reads the books.csv line by line
    while (getline(bookFile, line))
    {
        // Find the positions of the commas in the line
        int i1 = line.find(",");
        int i2 = line.find(",", i1 + 1);
        int i3 = line.find(",", i2 + 1);
        int i4 = line.find(",", i3 + 1);

        // Variables to store the book information
        Book book;
        book.ISBN = stoi(line.substr(0, i1));
        book.title = line.substr(i1 + 1, i2 - i1 - 1);
        book.author = line.substr(i2 + 1, i3 - i2 - 1);
        book.publisher = line.substr(i3 + 1, i4 - i3 - 1);
        stringstream ss(line.substr(i4 + 1));
        ss >> book.year;

        // If the ISBN of the book being read does not match the ISBN entered by the user, write the book information to the temp file
        if (book.ISBN != ISBN)
        {
            tempFile << book.ISBN << "," << book.title << "," << book.author << "," << book.publisher << "," << book.year << endl;
        }
    }

    // Close both the books.csv and temp files
    bookFile.close();
    tempFile.close();

    // Delete the books.csv file
    remove("books.csv");

    // Rename the temp file to books.csv
    rename("temp.csv", "books.csv");

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

// This function takes user input for the ISBN of the bookand returns the input ISBN.
string getISBN()
{
    string ISBN;
    cout << "Enter ISBN: ";
    cin >> ISBN;
    return ISBN;
}

// This function searches the books vector for the book with the given ISBN and returns the index of the book in the vector if found, and -1 if the book is not found.
int searchBookIndex(string ISBN)
{
    for (int i = 0; i < books.size(); i++)
    {
        if (books[i].ISBN == ISBN)
        {
            return i;
        }
    }
    return -1;
}


// addToList is a function that adds books to the bookList vector.
void addToList()
{
    // An infinite loop that prompts the user for the ISBN of a book and adds it to the bookList vector if it is found.
    while (true)
    {
        // Calls the getISBN function to get the ISBN of the book to be added to the list.
        string ISBN = getISBN();

        // Calls the searchBookIndex function to search for the book with the given ISBN.
        int index = searchBookIndex(ISBN);

        // If the book is found, adds it to the bookList vector.
        if (index != -1)
        {
            bookList.push_back(books[index]);
            cout << "Book added to list" << endl;
        }
        // If the book is not found, prompts the user to try again or exit.
        else
        {
            cout << "Book not found" << endl;
            cout << "Try again? (y/n): ";
            char choice;
            cin >> choice;
            if (choice == 'n')
            {
                // Exits the function if the user chooses not to try again.
                return;
            }
        }
    }
}


/*
Function: removeFromList()
Input: None
Output: None

The removeFromList function is used to remove a book from the bookList.
It first asks the user to input the ISBN of the book they want to remove.
It then searches the bookList for a book with the specified ISBN.
If it finds a book with the ISBN, it removes the book from the bookList and outputs "Book removed from list".
If it doesn't find a book with the ISBN, it outputs "Book not found in list" and gives the user the option to try again.
*/
void removeFromList()
{
    string ISBN = getISBN();
    int index = -1;
    // Search bookList for the specified ISBN
    for (int i = 0; i < bookList.size(); i++)
    {
        if (bookList[i].ISBN == ISBN)
        {
            index = i;
            break;
        }
    }
    // Remove book if it's found in the list
    if (index != -1)
    {
        bookList.erase(bookList.begin() + index);
        cout << "Book removed from list" << endl;
    }
    // If the book is not found in the list, prompt the user to try again
    else
    {
        cout << "Book not found in list" << endl;
        cout << "Try again? (y/n): ";
        char choice;
        cin >> choice;
        if (choice == 'y')
        {
            removeFromList();
        }
    }
}

/*
Function: displayList()
Input: None
Output: None

The displayList function is used to display all the books in the bookList.
It outputs the ISBN, title, and author of each book in the bookList,
and outputs the total number of books in the list.
*/
// displayList is a function that displays the books in the bookList vector.
void displayList()
{
    // Calls the displayAsPages function to display the books in the bookList vector.
    displayAsPages(bookList);
}

#endif