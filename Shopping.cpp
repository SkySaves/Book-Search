#include "Shopping.h"
#include <string>
#include <regex>
#include <iostream>

#define TODO_LATER std::cout << "NOT IMPLEMENTED" << std::endl; system("pause");

//void Shopping::addToShoppingList() {
//    // Open the database.
//    sqlite3* db;
//    int rc = sqlite3_open("bookstore.db", &db);
//    if (rc != SQLITE_OK) {
//        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
//        return;
//    }
//
//    // Execute a SELECT statement to retrieve the book data sorted by price.
//    std::vector<Book> books;
//    const char* sql = "SELECT ISBN, [Book-Title], [Book-Author], MSRP, Quantity FROM books ORDER BY MSRP";
//    sqlite3_stmt* stmt;
//    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
//    if (rc != SQLITE_OK) {
//        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
//        sqlite3_finalize(stmt);
//        sqlite3_close(db);
//        return;
//    }
//
//    // Iterate through the results and populate the books vector.
//    while (sqlite3_step(stmt) == SQLITE_ROW) {
//        Book book;
//        book.ISBN = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
//        book.title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
//        book.author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
//        book.MSRP = sqlite3_column_double(stmt, 3);
//        book.quantity = sqlite3_column_int(stmt, 4);
//        books.push_back(book);
//    }
//
//    // Finalize the statement and close the database.
//    sqlite3_finalize(stmt);
//    sqlite3_close(db);
//
//    // Display the books.
//    if (books.empty()) {
//        std::cout << "No books found." << std::endl;
//        return;
//    }
//    std::cout << "ISBN\tTitle\tAuthor\tPrice\tQuantity" << std::endl;
//    for (const Book& book : books) {
//        std::cout << book.ISBN << '\t' << book.title << '\t' << book.author << '\t'
//            << book.MSRP << '\t' << book.quantity << std::endl;
//    }
//
//    // Save the books to the shopping list.
//    std::cout << "Do you want to save the shopping list to the database? (Y/N) ";
//    std::string answer;
//    std::getline(std::cin, answer);
//    if (answer == "Y" || answer == "y") {
//        rc = sqlite3_open("bookstore.db", &db);
//        if (rc != SQLITE_OK) {
//            std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
//            return;
//        }
//
//        // Delete the existing shopping list.
//        sql = "DELETE FROM shoppingList";
//        rc = sqlite3_exec(db, sql, nullptr, nullptr, nullptr);
//        if (rc != SQLITE_OK) {
//            std::cerr << "Error deleting shopping list: " << sqlite3_errmsg(db) << std::endl;
//            sqlite3_close(db);
//            return;
//        }
//
//        // Insert the new shopping list.
//        for (const Book& book : books) {
//            Shopping book2;
//            book2.ISBN = book.ISBN;
//            book2.title = book.title;
//            book2.quantity = book.quantity;
//            addBookToShoppingList(book2, );
//        }
//
//        // Finalize the statement and close the database.
//        sqlite3_finalize(stmt);
//        sqlite3_close(db);
//        // Display a message indicating the shopping list was saved.
//        std::cout << std::endl << "Shopping list saved." << std::endl;
//    }
//
//}




void Shopping::addShopper(std::string name, std::string email, double totalAmountSpent) {
    // Check if the email already exists in the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string selectSql = "SELECT * FROM shoppers WHERE Email = ?";
    sqlite3_stmt* selectStmt;
    rc = sqlite3_prepare_v2(db, selectSql.c_str(), -1, &selectStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(selectStmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding text to SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(selectStmt);
        sqlite3_close(db);
        return;
    }

    if (sqlite3_step(selectStmt) == SQLITE_ROW) {
        std::cerr << "Error: Shopper with email " << email << " already exists in database." << std::endl;
        sqlite3_finalize(selectStmt);
        sqlite3_close(db);
        return;
    }

    // Insert the new shopper into the database.
    std::string insertSql = "INSERT INTO shoppers (Name, Email, TotalSpent) VALUES (?, ?, ?)";
    sqlite3_stmt* insertStmt;
    rc = sqlite3_prepare_v2(db, insertSql.c_str(), -1, &insertStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(selectStmt);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(insertStmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding text to INSERT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(selectStmt);
        sqlite3_finalize(insertStmt);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(insertStmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding text to INSERT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(selectStmt);
        sqlite3_finalize(insertStmt);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_double(insertStmt, 3, totalAmountSpent);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding double to INSERT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(selectStmt);
        sqlite3_finalize(insertStmt);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(insertStmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing INSERT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(selectStmt);
        sqlite3_finalize(insertStmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(insertStmt);
    sqlite3_finalize(selectStmt);
    sqlite3_close(db);

    std::cout << "Shopper added successfully." << std::endl;

}




void Shopping::addShopperHandler() {
    // Added integ checks for adding shoppers

    double totalAmountSpent;

    std::regex emailRegex("([a-zA-Z0-9_\\-\\.]+)@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.)|(([a-zA-Z0-9\\-]+\\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\\]?)");
    std::regex nameRegex("[a-zA-Z]+");
    std::regex totalAmountSpentRegex("[0-9]+");

    std::cout << "Enter the shopper's name: ";
    saferCin(name);
    while (!std::regex_search(name, nameRegex)) {
        std::cout << "Invalid name. Please try again\n";
        saferCin(name);
    }

    std::cout << "Enter the shopper's email: ";
    saferCin(email);
    while (!std::regex_search(email, emailRegex)) {
        std::cout << "Invalid Email. Please try again\n";
        saferCin(email);
    }

    std::cout << "Enter the shopper's total amount spent: ";
    saferCin(totalAmountSpent);
    while (!std::regex_search(std::to_string(totalAmountSpent), totalAmountSpentRegex)) {
        std::cout << "Invalid total amount spent. Please try again\n";
        saferCin(totalAmountSpent);
    }

    addShopper(name, email, totalAmountSpent);


    // Removed recusive call to main menu
    // Being handled by the original menu now
    std::cout << std::endl << "Shopper added." << std::endl;
    std::cout << "Press Enter to Continue" << std::endl;
    pressEnter();
}


void Shopping::addBookToShoppingList(const Shopping& book, int quantity) {
    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Insert the new book to the shopping list.
    const char* sql = "INSERT INTO shoppingList (ISBN, [Book-Title], [Book-Author], MSRP, Quantity) VALUES (?, ?, ?, ?, ?)";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, book.ISBN.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, book.title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, book.author.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, book.MSRP);
    sqlite3_bind_int(stmt, 5, quantity);

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

    int rc = sqlite3_open("bookstore.db", &db); // Open the database and store the handle in db.
    if (rc != SQLITE_OK) { // If an error occurred, print an error message and return.
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    // Execute a SELECT statement to retrieve the book data sorted by price.
    std::vector<Shopping> books; // Create a vector of Shopping objects to hold the retrieved books.
    const char* sql = "SELECT ISBN, [Book-Title], [Book-Author], MSRP, Quantity FROM books ORDER BY MSRP";
    // Define the SQL query to execute.
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr); // Prepare the query and store the statement in stmt.
    if (rc != SQLITE_OK) { // If an error occurred, print an error message, finalize the statement, and return.
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) { // Loop through each row of the result set.
        Shopping book; // Create a new Shopping object to hold the book data.

        // Retrieve the ISBN value and store it in book.
        if (const unsigned char* isbn = sqlite3_column_text(stmt, 0)) {
            book.ISBN = reinterpret_cast<const char*>(isbn);
        }
        else {
            std::cerr << "Error retrieving ISBN: " << sqlite3_errmsg(db) << std::endl;
            continue;
        }

        // Retrieve the title value and store it in book.
        if (const unsigned char* title = sqlite3_column_text(stmt, 1)) {
            book.title = reinterpret_cast<const char*>(title);
        }
        else {
            std::cerr << "Error retrieving title: " << sqlite3_errmsg(db) << std::endl;
            continue;
        }

        // Retrieve the author value and handle errors
        const unsigned char* authorText = sqlite3_column_text(stmt, 2);
        if (authorText != nullptr) {
            book.author = reinterpret_cast<const char*>(authorText);
        }
        else {
            //std::cerr << "Error retrieving author: " << sqlite3_errmsg(db) << std::endl;
            continue; // Skip this row and move on to the next one
        }

        // Retrieve the MSRP value and store it in book.
        book.MSRP = sqlite3_column_double(stmt, 3);

        // Retrieve the quantity value and store it in book.
        book.quantity = sqlite3_column_int(stmt, 4);

        books.push_back(book); // Add the book to the vector of retrieved books.
    }

    // Finalize the statement and close the database.
    if (stmt != nullptr) { // If the statement was prepared successfully, finalize it.
        rc = sqlite3_finalize(stmt); // Finalize the statement.
        if (rc != SQLITE_OK) { // If an error occurred, print an error message.
            std::cerr << "Error finalizing statement: " << sqlite3_errmsg(db) << std::endl;
        }
    }
    sqlite3_close(db); // Close the database.

    // Display the books.
    if (books.empty()) { // If no books were retrieved, print a message and return.
        std::cout << "No books found." << std::endl;
        return;
    }

    system("cls");
    // Prompt the user to select books and add them to the shopping list.
    std::string answer;
    do {
        std::cout << "Enter the ISBN of the book you want to add to the shopping list: ";
        std::string ISBN;
        saferCin(ISBN); // Get the ISBN value from the user.


        // Check if ISBN is not empty
        if (ISBN.empty()) { // If the user did not enter an ISBN, print an error message and continue the loop.
            std::cerr << "ISBN cannot be empty." << std::endl;
            continue;
        }


        // Search for the book with the specified ISBN.
        Shopping selectedBook;
        bool foundBook = false;
        for (const Shopping& book : books) {
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
            saferCin(quantity);

            // Add the selected book to the shopping list.
            addBookToShoppingList(selectedBook, quantity);
        }
        else {
            std::cout << "Book not found." << std::endl;
        }

        // Ask the user if they want to select another book.
        std::cout << "Do you want to select another book? (Y/N) ";
        saferCin(answer);
    } while (answer == "Y" || answer == "y");

}

void Shopping::clearShoppingList() {
    sqlite3* db;

    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string sql = "DELETE FROM shoppingList";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error deleting shopping list: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "Shopping list cleared." << std::endl;
    std::cout << "Press Enter to continue..." << std::endl;
    pressEnter();

}

void Shopping::removeItemFromShoppingList(string& isbn) {
    sqlite3* db;

    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    std::string sql = "DELETE FROM shoppingList WHERE ISBN=" + isbn;
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error deleting row: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    std::cout << "Item removed from shopping list.\n";
    std::cout << "Press enter to continue...";
    pressEnter();

}

void Shopping::displayShoppingList() {
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    const char* sql = "SELECT ISBN, [Book-Title], [Book-Author], MSRP, Quantity FROM shoppingList";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // Store Menu items for while loop -Zane
    std::vector<std::tuple<std::string, std::string, std::string, double, int>> books;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        std::string isbn = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        std::string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        double msrp = sqlite3_column_double(stmt, 3);
        int quantity = sqlite3_column_int(stmt, 4);
        books.push_back(std::make_tuple(isbn, title, author, msrp, quantity));
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Added Menu System and refined control flow - Zane

    Menu checkMenu{ "Shopping List Menu" };
    checkMenu.addOption("Add Book", 1);
    checkMenu.addOption("Remove Book", 2);
    checkMenu.addOption("Clear List", 3);
    checkMenu.addOption("Checkout", 4);
    checkMenu.addOption("Exit", 5);

    while (true) {
        std::string loc_isbn;
        std::string clearList_choice;
        int choice;

        std::cout << "ISBN\t\tTitle\t\t\tAuthor\t\t\tMSRP\tQuantity\n";
        for (int i = 0; i < books.size(); i++) {
            std::cout << std::get<0>(books[i]) << "\t"
                << std::get<1>(books[i]) << "\t\t"
                << std::get<2>(books[i]) << "\t\t"
                << std::get<3>(books[i]) << "\t"
                << std::get<4>(books[i]) << "\n";
        }

        checkMenu.display();
        checkMenu.getChoice(choice, "What to do with your list: ");

        switch (choice) {
        case 1:
            // I think this is just a long function,
            // The vector asignment and sql statements and integrety checking just takes a long time i think.
            // Any way all this to say I didnt see any obvious way to make it faster :( -Zane
            this->addToShoppingListHandler();
            break;

        case 2:
            std::cout << "Please enter 10-13 digit ISBN: ";
            saferCin(loc_isbn);
            this->removeItemFromShoppingList(loc_isbn);

            break;

        case 3:
            std::cout << "Are you sure you want to clear your list?" << std::endl;
            std::cout << "(Y/N): ";
            saferCin(clearList_choice);

            if (clearList_choice == "Y" || clearList_choice == "y") {
                // This has introduced a new bug where the list doesnt update on remove now
                // Something about passing the db reference and updating it all in one statement to fix it -Zane
                this->clearShoppingList();
            }

            break;

        case 4:
            this->checkout();
            break;

        case 5:
            std::cout << "Returning to main menu..." << std::endl;
            return;

        default:
            break;
        }
    }

}

void Shopping::checkout() {
    sqlite3* db;
    sqlite3_stmt* selectStmt = nullptr;
    std::ostringstream receipt;
    double totalMsrp = 0;

    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }
    // Prompt the user to enter their email.
    std::string email;
    std::cout << "Please enter your email: ";
    std::cin >> email;

    // Check if shopper exists.
    const char* selectShopperSql = "SELECT Name FROM shoppers WHERE Email = ?";
    sqlite3_stmt* selectShopperStmt;
    rc = sqlite3_prepare_v2(db, selectShopperSql, -1, &selectShopperStmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(selectShopperStmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding text to SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(selectShopperStmt);
        sqlite3_close(db);
        return;
    }

    // Get the shopper's name if it exists in the database.
    std::string shopperName;
    rc = sqlite3_step(selectShopperStmt);
    if (rc == SQLITE_ROW) {
        shopperName = reinterpret_cast<const char*>(sqlite3_column_text(selectShopperStmt, 0));
        receipt << "\n" << std::left << std::setw(40) << "Shopper Name" << std::right << std::setw(20) << shopperName << "\n";
    }
    else {
        // Shopper doesn't exist, prompt to create an account.
        char answer;
        std::cout << "Email not found in database. Would you like to create an account? (y/n): ";
        std::cin >> answer;
        if (answer == 'y') {
            addShopperHandler();
        }
    }
    sqlite3_finalize(selectShopperStmt);

    // Check what's in the shoppingList table.
    const char* selectSql = "SELECT ISBN, [Book-Title], [Book-Author], MSRP, Quantity FROM shoppingList";
    sqlite3_stmt* selectStmt4 = nullptr;
    rc = sqlite3_prepare_v2(db, selectSql, -1, &selectStmt4, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    receipt << std::left << std::setw(15) << "ISBN" << std::setw(30) << "Title" << std::setw(20) << "Author" << std::setw(10) << "MSRP" << std::setw(10) << "Quantity" << "\n";
    // Iterate over the shopping list and calculate total MSRP.
    while (sqlite3_step(selectStmt4) == SQLITE_ROW) {
        std::string isbn = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt4, 0));
        std::string title = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt4, 1));
        std::string author = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt4, 2));
        double msrp = sqlite3_column_double(selectStmt4, 3);
        int quantity = sqlite3_column_int(selectStmt4, 4);

        double bookMsrp = msrp * quantity;
        totalMsrp += bookMsrp;

        receipt << std::left << std::setw(15) << isbn << std::setw(30) << title << std::setw(20) << author << std::setw(10) << msrp << std::setw(10) << quantity << std::setw(10) << bookMsrp << "\n";
    }

    // Finalize the SELECT statement.
    sqlite3_finalize(selectStmt4);

    double tax = totalMsrp * 0.09;
    double totalAmount = totalMsrp + tax;

    // Check if shopper exists.
    const char* selectShopperSql2 = "SELECT Name FROM shoppers WHERE Email = ?";
    sqlite3_stmt* selectShopperStmt3;
    rc = sqlite3_prepare_v2(db, selectShopperSql2, -1, &selectShopperStmt3, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_bind_text(selectShopperStmt3, 1, email.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        std::cerr << "Error binding text to SELECT statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(selectShopperStmt3);
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(selectShopperStmt3);
    if (rc == SQLITE_ROW) {
        shopperName = reinterpret_cast<const char*>(sqlite3_column_text(selectShopperStmt3, 0));

        receipt << "\n" << std::left << std::setw(40) << "Shopper Name" << std::right << std::setw(20) << shopperName << "\n";
    }
    sqlite3_finalize(selectShopperStmt3);

    receipt << "\n" << std::left << std::setw(15) << "" << std::setw(30) << "" << std::setw(20) << "" << std::setw(10) << "" << std::setw(10) << "" << std::setw(10) << totalMsrp << "\n"
        << std::left << std::setw(15) << "" << std::setw(30) << "" << std::setw(20) << "" << std::setw(10) << "" << std::setw(10) << "" << std::setw(10) << tax << "\n"
        << std::left << std::setw(15) << "" << std::setw(30) << "" << std::setw(20) << "" << std::setw(10) << "" << std::setw(10) << "" << std::setw(10) << totalAmount << "\n";

    // Save the receipt to a file.
    std::ofstream outFile("receipt.txt");
    outFile << receipt.str();
    outFile.close();

    sqlite3_close(db);
    std::cout << "Checkout completed. Receipt saved to receipt.txt." << std::endl;
    //get input from user to continue
    std::cout << "Press any key to continue: ";
    std::cin.get();
    std::cin.ignore();

    // Clear the shopping list.
    // That should be it to add the clear functionality
    // - Zane
    //clearShoppingList();
}