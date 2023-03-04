#pragma once

#include <iostream>
#include <vector>
#include <string>
#include "sqlite3.h"
#include "Books.h"



// addToList is a function that adds books to the shoppingList table in the bookstore.db database.
void addToList(string ISBN)
{
    // An infinite loop that prompts the user for the ISBN of a book and adds it to the shoppingList table if it is found.
    while (true)
    {
        // Calls the getISBN function to get the ISBN of the book to be added to the list.


        // Open the database
        sqlite3* db;
        int rc = sqlite3_open("bookstore.db", &db);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Prepare the SELECT statement

        string sql = "SELECT * FROM books WHERE ISBN = ?;";
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Bind the parameter to the statement
        rc = sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the statement and fetch results
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve the values from the current row
            string title(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            string author(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            int MSRP = sqlite3_column_int(stmt, 3);

            // Prepare the INSERT statement
            string sql2 = "INSERT INTO shoppingList (ISBN, 'Book-Title', 'Book-Author', MSRP) VALUES (?, ?, ?, ?);";
            sqlite3_stmt* stmt2;
            rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stmt2, nullptr);
            if (rc != SQLITE_OK) {
                sqlite3_finalize(stmt2);
                sqlite3_close(db);
                std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
                return;
            }

            // Bind the parameters to the statement
            rc = sqlite3_bind_text(stmt2, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_text(stmt2, 2, title.c_str(), -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_text(stmt2, 3, author.c_str(), -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_int(stmt2, 4, MSRP);

            // Execute the statement
            rc = sqlite3_step(stmt2);

            // Finalize the statements and close the database
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt2);
            sqlite3_close(db);

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




using namespace std;
// addToList is a function that adds books to the shoppingList table in the bookstore.db database.
void addToList()
{
    // An infinite loop that prompts the user for the ISBN of a book and adds it to the shoppingList table if it is found.
    while (true)
    {
        string ISBN;
        cout << "Enter ISBN: ";
        cin >> ISBN;

        // Open the database
        sqlite3* db;
        int rc = sqlite3_open("bookstore.db", &db);
        if (rc != SQLITE_OK) {
            sqlite3_close(db);
            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Prepare the SELECT statement

        string sql = "SELECT * FROM books WHERE ISBN = ?;";
        rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
            return;
        }

        // Bind the parameter to the statement
        rc = sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);

        // Execute the statement and fetch results
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            // Retrieve the values from the current row
            string title(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
            string author(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
            int MSRP = sqlite3_column_int(stmt, 3);

            // Prepare the INSERT statement
            string sql2 = "INSERT INTO shoppingList (ISBN, 'Book-Title', 'Book-Author', MSRP) VALUES (?, ?, ?, ?);";
            sqlite3_stmt* stmt2;
            rc = sqlite3_prepare_v2(db, sql2.c_str(), -1, &stmt2, nullptr);
            if (rc != SQLITE_OK) {
                sqlite3_finalize(stmt2);
                sqlite3_close(db);
                std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
                return;
            }

            // Bind the parameters to the statement
            rc = sqlite3_bind_text(stmt2, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_text(stmt2, 2, title.c_str(), -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_text(stmt2, 3, author.c_str(), -1, SQLITE_TRANSIENT);
            rc = sqlite3_bind_int(stmt2, 4, MSRP);

            // Execute the statement
            rc = sqlite3_step(stmt2);

            // Finalize the statements and close the database
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt2);
            sqlite3_close(db);

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





void removeFromList(string ISBN)
{

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Prepare the DELETE statement

    string sql = "DELETE FROM shoppingList WHERE ISBN = ?;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind the ISBN parameter to the statement
    rc = sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement and check for errors
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error deleting book from shopping list: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        cout << "Book removed from list" << endl;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}








void removeFromList()
{

    string ISBN;
    cout << "Enter ISBN: ";
    cin >> ISBN;

    // Open the database
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Prepare the DELETE statement

    string sql = "DELETE FROM shoppingList WHERE ISBN = ?;";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Bind the ISBN parameter to the statement
    rc = sqlite3_bind_text(stmt, 1, ISBN.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement and check for errors
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error deleting book from shopping list: " << sqlite3_errmsg(db) << std::endl;
    }
    else {
        cout << "Book removed from list" << endl;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}


