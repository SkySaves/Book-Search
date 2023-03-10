#include "Books.h"
#include "MenuSystem.h"
#include <regex>
#include <string>


void Book::readBooks() {
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


void Book::displayAsPages(vector<Book>& books) {
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
            // Prints the ISBN, title, author of each book on the current page.
            std::cout << "ISBN: " << books[i].ISBN << endl;
            std::cout << "Title: " << books[i].title << endl;
            std::cout << "Author: " << books[i].author << endl;
            std::cout << std::endl << std::endl;
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


int Book::searchBooks(string search, int searchChoice) {
    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }

    // Construct the SQL statement based on the user's search choice.
    std::stringstream ss;
    std::string column;
    switch (searchChoice) {
    case 0:
        column = "`Book-Title`";
        break;
    case 1:
        column = "`Book-Author`";
        break;
    case 2:
        column = "ISBN";
        break;
    default:
        column = "`Book-Title`";
        break;
    }
    ss << "SELECT * FROM books WHERE " << column << " LIKE ?";
    std::string sql = ss.str();

    // Prepare the SQL statement.
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return 0;
    }

    // Bind the search term to the SQL statement.
    std::string searchWildcard = "%" + search + "%";
    rc = sqlite3_bind_text(stmt, 1, searchWildcard.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error binding parameter: " << sqlite3_errmsg(db) << std::endl;
        return 0;
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

        // Filter the books based on the user's search choice.
        if (searchChoice == 0) { // Search by title.
            if (book.title.find(search) != string::npos) {
                books.push_back(book);
            }
        }
        else if (searchChoice == 1) { // Search by author.
            if (book.author.find(search) != string::npos) {
                books.push_back(book);
            }
        }
        else if (searchChoice == 2) { // Search by ISBN.
            if (book.ISBN.find(search) != string::npos) {
                books.push_back(book);
            }
        }
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    displayAsPages(books);
    return 1;
}






void Book::addBook(string ISBN, string title, string author, int year, string publisher, int price, int quantity, string description) {
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


void Book::addBook() {


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
}


void Book::deleteBook() {
    // Variables to store the book information

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


void Book::removeFromList() {
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

void Book::addToList() {
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







void Book::listHandler() {
    // Clears the console screen.
    system("cls");
    Book abook2;
    // Creates an instance of a class named "Menu" with the title "What would you like to do: \n".
    Menu listMenu("What would you like to do: \n");

    // Adds options to the menu with the name "Add a book to the list", "Remove a book from the list", "Display the list", and "Back to main menu".
    listMenu.addOption("Add a book to the list", 1);
    listMenu.addOption("Remove a book from the list", 2);
    listMenu.addOption("Display the list", 3);
    listMenu.addOption("Back to main menu", 4);
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
        case 1:
            // Calls the addToList function to add a book to the list.
            abook2.addBookHandler();
            break;

        case 2:
            // Calls the removeFromList function to remove a book from the list.
            abook2.removeFromList();
            break;

        case 3:
            // Calls the displayList function to display the books in the list.
            abook2.displayList();
            break;

        case 4:
            // Returns to the previous menu.
            return;

        default:
            break;
        }
    }
}




void Book::addBookHandler() {
    Book abook3;
    // Clears the console screen.
    system("cls");

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

    std::cout << "Enter the MSRP of the book: ";
    saferCin(bookMSRP);

    std::cout << "Enter the quantity of the book: ";
    saferCin(bookQuantity);

    std::cout << "Enter the description of the book: ";
    saferCin(bookDescription);

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
    abook3.addBook(bookISBN, bookTitle, bookAuthor, bookYear, bookPublisher, bookMSRP, bookQuantity, bookDescription);
}


void Book::addShopperHandler() {
    Shopping shopping;

    std::regex emailRegex("([a-zA-Z0-9_\\-\\.]+)@((\\[[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.)|(([a-zA-Z0-9\\-]+\\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\\]?)");
    std::regex nameRegex("[a-zA-Z]+");
    std::regex totalAmountSpentRegex("[0-9]+");

    std::cout << "Enter the shopper's name: ";
    saferCin(name);
    while (std::regex_search(name, nameRegex)) {
        std::cout << "Invalid name. Please try again: ";
        saferCin(name);
    }

    std::cout << "Enter the shopper's email: ";
    saferCin(email);
    while (std::regex_search(email, emailRegex)) {
        std::cout << "Invalid Email. Please try again";
        saferCin(email);
    }

    std::cout << "Enter the shopper's total amount spent: ";
    saferCin(totalAmountSpent);
    while (std::regex_search(std::to_string(totalAmountSpent), totalAmountSpentRegex)) {
        std::cout << "Invalid total amount spent. Please try again";
        saferCin(totalAmountSpent);
    }

    shopping.addShopper(name, email, totalAmountSpent);
}


void Book::searchBookHandler() {
    // Clears the console screen.
    system("cls");

    // Creates an instance of a class named "Menu" with the title "How would you like to search: \n".
    Menu searchMenu("How would you like to search: \n");


    // Adds options to the menu with the name "Title", "Author", "ISBN", and "Back to main menu".
    searchMenu.addOption("Title", 0);
    searchMenu.addOption("Author", 1);
    searchMenu.addOption("ISBN", 2);
    searchMenu.addOption("Back to main menu", 3);



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
        Book abook;
        // Calls the searchBooks function with the user's search query and choice as arguments.
        abook.searchBooks(query, searchChoice);
    }
}