#include "Shopping.h"


void Shopping::addToShoppingList() {
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
    }
}



void Shopping::addShopper(std::string name, std::string email, double totalAmountSpent) {
    Shopping shopper;
    shopper.name = name;
    shopper.email = email;
    shopper.totalAmountSpent = totalAmountSpent;
    ShoppingBooks.push_back(shopper);
}


void Shopping::addShopperHandler() {

    double totalAmountSpent;

    std::cout << "Enter the shopper's name: ";
    std::cin >> name;
    std::cout << "Enter the shopper's email: ";
    std::cin >> email;
    std::cout << "Enter the shopper's total amount spent: ";
    std::cin >> totalAmountSpent;

    addShopper(name, email, totalAmountSpent);

    std::cout << std::endl << "Shopper added." << std::endl;

    // Loop until user chooses to return to the main menu
    while (true) {
        std::cout << "Press any key to return to menu...";
        std::cin.ignore();
        std::cin.get();

        // Display main menu
        Menu mainM2("Main Menu");
        mainM2.displayMainMenu();


    }
}


void Shopping::addBookToShoppingList(const Shopping& item) {
    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    //Check database for 

    // Insert the new book to the shopping list.
    const char* sql = "INSERT INTO shoppingList (ISBN, [Book-Title], [Book-Author], MSRP, Quantity) VALUES (?, ?, ?, ?, ?;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, item.ISBN.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, item.title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, item.author.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, item.MSRP);
    sqlite3_bind_int(stmt, 5, item.quantity);


    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error inserting book into shopping list: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << std::endl << "Book added to shopping list." << std::endl;
}


void Shopping::addToShoppingListHandler() {
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
    std::cout << "ISBN\tTitle\tAuthor\tPrice\tQuantity" << std::endl;
    for (const Book& book : books) {
        std::cout << book.ISBN << '\t' << book.title << '\t' << book.author << '\t'
            << book.MSRP << '\t' << book.quantity << std::endl;
    }


    // Prompt the user to select books and add them to the shopping list.
    std::string answer;
    std::vector<Shopping> selectedBooks;
    do {
        std::cout << "Enter the ISBN of the book you want to add to the shopping list: ";
        std::string ISBN;
        std::getline(std::cin, ISBN);

        // Search for the book with the specified ISBN.
        Book selectedBook;
        bool foundBook = false;
        for (const Book& book : books) {
            if (book.ISBN == ISBN) {
                selectedBook = book;
                foundBook = true;
                break;
            }
        }

        if (foundBook) {
            // Ask the user how many copies of the book they want to add to the shopping list.
            std::cout << "Enter the quantity of the book you want to add to the shopping list: ";
            int quantity;
            std::cin >> quantity;

            // Add the selected book to the shopping list.
            Shopping item;
            item.ISBN = selectedBook.ISBN;
            item.title = selectedBook.title;
            item.quantity = quantity;
            addBookToShoppingList(item);
        }
        else {
            std::cout << "Book not found." << std::endl;
        }    // Ask the user if they want to select another book.
        std::cout << "Do you want to select another book? (Y/N) ";
        std::getline(std::cin, answer);
    } while (answer == "Y" || answer == "y");
}





