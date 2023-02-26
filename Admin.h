#pragma once
#include <iostream>
#include <string>
#include "sqlite3.h"
#include "SaferCIN.h"
#include <fstream>





using namespace std;




void addCustomerHandler() {
    // Declare variables to store the new customer's information.
    string username;
    string password;

    // Prompt the admin to enter the new customer's information and read the input safely.
    cout << "Enter the new customer's username: ";
    saferCin(username);
    cout << "Enter the new customer's password: ";
    saferCin(password);

    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        // If the database fails to open, print an error message and return.
        sqlite3_close(db);
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Prepare the INSERT statement.
    sqlite3_stmt* stmt;
    string sql = "INSERT INTO customerLogin (Username, Password) VALUES (?, ?)";
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        // If the statement fails to prepare, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Bind the parameters to the statement.
    rc = sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_TRANSIENT);
    rc = sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_TRANSIENT);
    if (rc != SQLITE_OK) {
        // If binding fails, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cerr << "Error binding parameters: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Execute the statement.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        // If execution fails, print an error message, finalize the statement, close the database, and return.
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        cerr << "Error executing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Print a message to indicate that the registration is complete.
    cout << "New customer added successfully!" << endl;
    return;
}



void changeCustomerPasswordHandler() {
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


// if there are multiple admin logins in the database, this will change the password for all of them
void changeAdminPasswordHandler() {
    // Declare variables to store the admin's information.
    string adminUsername;
    string oldPassword;
    string newPassword;

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




void importFilesHandler() {
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












void adminMenu() {
    while (true) {
        // Clears the console screen.
        system("cls");

        // Display the admin menu options.
        cout << "Admin Menu" << endl;
        cout << "Please choose an option:" << endl;
        cout << "1. Add user to customerLogin" << endl;
        cout << "2. Change customer password" << endl;
        cout << "3. Change admin password" << endl;
        cout << "4. Import files" << endl;
        cout << "5. Return to main menu" << endl;
        cout << "Enter your choice (1-5): ";

        // Get the user's input.
        string adminChoice;
        getline(cin, adminChoice);

        // Call the appropriate function based on the admin's choice.
        if (adminChoice == "1") {
            addCustomerHandler();
        }
        else if (adminChoice == "2") {
            changeCustomerPasswordHandler();
        }
        else if (adminChoice == "3") {
            changeAdminPasswordHandler();
        }
        else if (adminChoice == "4") {
            importFilesHandler();
        }
        else if (adminChoice == "5") {
            // Return to main menu
            displayMainMenu();
        }
        else {
            // Display an error message if the admin's input is invalid.
            cout << "Invalid choice. Please enter a number from 1 to 5." << endl;
            // Pause the program so the admin can see the error message.
            system("pause");
        }
    }
}



int adminLoginHandler()
{
    // Clear the console screen.
    system("cls");

    // Declare variables to store admin login information and a retry prompt.
    string adminUsername;
    string adminPassword;
    string adminTryAgain = "y";

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










