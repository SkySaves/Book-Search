#include "Login.h"



bool Login::userExists(std::string username) {
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

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt2, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt2);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Bind the username parameter to the SELECT statement.
    rc = sqlite3_bind_text(stmt2, 1, username.c_str(), -1, SQLITE_STATIC);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt2);
        sqlite3_close(db);
        std::cerr << "Error binding username parameter: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Execute the SELECT statement and get the result.
    bool exists = false;
    if (sqlite3_step(stmt2) == SQLITE_ROW) {
        int count = sqlite3_column_int(stmt2, 0);
        if (count > 0) {
            exists = true;
        }
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt2);
    sqlite3_close(db);

    return exists;
}



std::string Login::readPassword()
{


    while ((c = _getch()) != '\r') {
        if (c == '\b') {
            if (!password.empty()) {
                password.pop_back();
                std::cout << "\b \b";
            }
        }
        else {
            password.push_back(c);
            std::cout << "*";
        }
    }

    std::cout << std::endl;

    return password;
}

bool Login::userLogin(string username, string password) {

    sqlite3* db;
    sqlite3_stmt* stmt;

    // Opens the database
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    // Prepare the SELECT statement
    string sql = "SELECT UserID FROM customerLogin WHERE Username = ? AND Password = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);


    // Bind the parameters to the statement
    rc = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);


    // Execute the statement. This will verify if the user exist or not.
    rc = sqlite3_step(stmt);
    bool result = false;
    if (rc == SQLITE_ROW) {
        result = true;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}



bool Login::adminLogin(string adminUsername, string adminPassword) {
    sqlite3* db;
    sqlite3_stmt* stmt;

    // Opens the database
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        return false;
    }

    // Prepare the SELECT statement
    string sql = "SELECT Username, Password FROM adminLogin WHERE Username = ? AND Password = ?";

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);

    // Bind the parameters to the statement
    rc = sqlite3_bind_text(stmt, 1, adminUsername.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, adminPassword.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement. This will verify if the admin exists or not.
    rc = sqlite3_step(stmt);
    bool result = false;
    if (rc == SQLITE_ROW) {
        result = true;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return result;
}


int Login::authUserHandler() {
    // Clears the console screen.
    system("cls");
    // Welcome message.
    std::cout << "Welcome to the CIS 227 Bookstore. Please enter your login information." << endl;

    // Prompts the user to enter their username.
    std::cout << "Enter your username: ";
    std::cin >> (username);

    // Prompts the user to enter their password.
    std::cout << "Enter your password: ";
    std::cin >> (password);

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
            std::cin >> (tryAgain);

            if (tryAgain == "y")
            {
                // Prompts the user to enter their username again.
                std::cout << "Enter your username: " << std::endl;
                std::cin >> (username);

                // Prompts the user to enter their password again.
                std::cout << "Enter your password: " << std::endl;
                std::cin >> (password);
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


void Login::registrationHandle() {

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

int Login::adminLoginHandler()
{
    // Clear the console screen.
    system("cls");


    while (adminTryAgain == "y")
    {
        // Prompt the user to enter their admin username and password, and read the input safely.
        std::cout << "Enter admin username: ";
        saferCin(adminUsername);
        std::cout << "Enter admin password: ";
        saferCin(adminPassword);

        // If the entered admin login information is valid, exit the loop.
        if (adminLogin(adminUsername, adminPassword))
        {
            break;
        }
        // If the entered admin login information is invalid, prompt the user to retry or quit.
        else
        {
            std::cout << "Invalid login information. Try again? (y/n): " << std::endl;
            saferCin(adminTryAgain);
        }
    }

    // If the user chooses to quit, exit the program and return 0.
    if (adminTryAgain == "n")
    {
        return 0;
    }

    // If the admin login was successful, return 1 to indicate success.
    return 1;
}



void Login::changeCustomerPasswordHandler() {

    // Prompt the admin to enter the customer's information and read the input safely.
    cout << "Enter the customer's username: ";
    saferCin(username);
    cout << "Enter the new password: ";
    saferCin(newPassword);

    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        // If the database fails to open, print an error message and return.
        sqlite3_close(db);
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Prepare the UPDATE statement.
    sqlite3_stmt* stmt;
    string sql = "UPDATE customerLogin SET Password = ? WHERE Username = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // If the statement fails to prepare, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Bind the parameters to the statement.
    rc = sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement.
    rc = sqlite3_step(stmt);

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Print a message to indicate that the password has been changed.
    // Clear the console.
    system("cls");
    cout << "Password changed successfully for customer " << username << endl;
}



void Login::changeAdminPasswordHandler() {

    // Prompt the admin to enter their information and read the input safely.
    cout << "Enter your admin username: ";
    saferCin(adminUsername);
    cout << "Enter your old password: ";
    saferCin(oldPassword);
    cout << "Enter your new password: ";
    saferCin(newPassword);

    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        // If the database fails to open, print an error message and return.
        sqlite3_close(db);
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Check if the admin's old password is correct
    sqlite3_stmt* stmt;
    string sql = "SELECT AdminID FROM adminLogin WHERE Username = ? AND Password = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    rc = sqlite3_bind_text(stmt, 1, adminUsername.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, oldPassword.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_ROW) {
        // If the admin's old password is incorrect, print an error message and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cerr << "Error: Old password is incorrect." << endl;
        return;
    }
    sqlite3_finalize(stmt);

    // Prepare the UPDATE statement to change the admin's password.
    sql = "UPDATE adminLogin SET Password = ? WHERE Username = ? AND Password = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // If the statement fails to prepare, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Bind the parameters to the statement.
    rc = sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, adminUsername.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 3, oldPassword.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement.
    rc = sqlite3_step(stmt);

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Print a message to indicate that the password change is complete.
    cout << "Password change complete!" << endl;
}


void Login::importFilesHandler() {
    // Declare variables to store the file name and the SQL query.
    string fileName;
    string sqlQuery;

    // Prompt the admin to enter the file name and read the input safely.
    cout << "Enter the name of the file to import: ";
    saferCin(fileName);

    // Open the file.
    ifstream file(fileName.c_str());
    if (!file.is_open()) {
        // If the file fails to open, print an error message and return.
        cerr << "Error opening file: " << fileName << endl;
        return;
    }

    // Read each line from the file and execute it as an SQL statement.
    sqlite3* db;
    char* errMsg = NULL;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        // If the database fails to open, print an error message and return.
        sqlite3_close(db);
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    while (getline(file, sqlQuery)) {
        rc = sqlite3_exec(db, sqlQuery.c_str(), NULL, NULL, &errMsg);
        if (rc != SQLITE_OK) {
            // If the statement fails to execute, print an error message and continue with the next statement.
            cerr << "Error executing statement: " << errMsg << endl;
            sqlite3_free(errMsg);
        }
    }

    // Close the file and the database.
    file.close();
    sqlite3_close(db);

    // Print a message to indicate that the import is complete.
    cout << "Import complete!" << endl;
}



void Login::changeCustomerPassword() {
    // Declare variables to store the customer's information.
    string username;
    string newPassword;

    // Prompt the admin to enter the customer's information and read the input safely.
    cout << "Enter the customer's username: ";
    saferCin(username);
    cout << "Enter the new password: ";
    saferCin(newPassword);

    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        // If the database fails to open, print an error message and return.
        sqlite3_close(db);
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Prepare the UPDATE statement.
    sqlite3_stmt* stmt;
    string sql = "UPDATE customerLogin SET Password = ? WHERE Username = ?";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // If the statement fails to prepare, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Bind the parameters to the statement.
    rc = sqlite3_bind_text(stmt, 1, newPassword.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, username.c_str(), -1, SQLITE_TRANSIENT);

    // Execute the statement.
    rc = sqlite3_step(stmt);

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Print a message to indicate that the password has been changed.
    // Clear the console.
    system("cls");
    cout << "Password changed successfully for customer " << username << endl;

}

