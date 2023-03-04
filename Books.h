#pragma once

#ifndef BooksHeader
#define BooksHeader

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include "Login.h"

using namespace std;
/* ---------------- CLASSES ---------------- */

// This defines the class "Book" which has 6 data members : ISBN, title, author, publisher, year, and description.
// All of these variables are of type string except for year which is an integer.
class Book
{

    public:
        sqlite3_stmt* stmt;
        std::string title;
        std::string author;
        std::string publisher;
        std::string bookTitle;
        std::string bookAuthor;
        std::string ISBN;
        std::string description;
        int yearOfPublication;
        int year;
        double MSRP;
        int quantity;
        std::string bookISBN, bookPublisher, bookDescription;

        int bookYear, bookMSRP, bookQuantity;
        std::string name;
        std::string email;
        double totalAmountSpent;
        std::string query;
        int searchChoice;
        // Declare variables to store the customer's information.
        string username;
        string newPassword;



     

        Book(std::string bookTitle, std::string bookAuthor, std::string bookISBN, std::string bookPublisher, int bookYear, std::string bookDescription, int bookMSRP, int bookQuantity)
        {
			this->bookTitle = bookTitle;
			this->bookAuthor = bookAuthor;
			this->bookISBN = bookISBN;
			this->bookPublisher = bookPublisher;
			this->bookYear = bookYear;
			this->bookDescription = bookDescription;
			this->bookMSRP = bookMSRP;
			this->bookQuantity = bookQuantity;
		}

        Book(std::string bookTitle, std::string bookAuthor, std::string bookISBN, std::string bookPublisher, int bookYear, std::string bookDescription)
        {
			this->bookTitle = bookTitle;
			this->bookAuthor = bookAuthor;
			this->bookISBN = bookISBN;
			this->bookPublisher = bookPublisher;
			this->bookYear = bookYear;
			this->bookDescription = bookDescription;
		}

        Book()
        {
			title = "";
			author = "";
			ISBN = "";
			publisher = "";
			year = 0;
			description = "";
			MSRP = 0.0;
			quantity = 0;
		}

       

    /* ---------------- GLOBALS ---------------- */

    // Empty vector of type Book named "books". This is where the output will be stored.

    vector<Book> books;

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
    void readBooks();
 


    void addBook(string ISBN, string title, string author, int year, string publisher, int price, int quantity, string description);

    void addBook();

    void deleteBook();


    // Creates pages of books and displays them to the user
    // displayAsPages is a function that takes in a vector of Book objects and displays them one page at a time.
    void displayAsPages(vector<Book>& books);





    // searchBooks is a function that takes in a search query and a search choice, and returns a vector of books that match the query.
    int searchBooks(string search, int searchChoice);
    



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
    void addToList();

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
    void removeFromList();
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





    void searchBookHandler();


    // listHandler is a function that provides the user with options to create and manage a list of books.
    void listHandler();


    // addBookHandler is a function that allows the user to add a book to the bookstore database.
    void addBookHandler();



    void addShopperHandler();





};

#endif