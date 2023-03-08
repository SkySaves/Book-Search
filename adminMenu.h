#pragma once
#include <string>
#include <fstream>
#include "md5.h"

//Still trying to attempt to add this admin menu to the beginning.


// This function handles adding a new user to the database.
void adminAddUser() {
    // Clear the console screen.
    system("cls");

    // Declare variables to store user input.
    std::string username;
    std::string password;
    std::string confirmPassword;

    // Prompt the user to enter a new username and read the input.
    std::cout << "Enter a new username: ";
    std::getline(std::cin, username);

    // Prompt the user to enter a new password and read the input without echoing.
    std::cout << "Enter a new password: ";
    password = readPassword();

    // Prompt the user to confirm the new password and read the input without echoing.
    std::cout << "Confirm the new password: ";
    confirmPassword = readPassword();

    // If the password and confirmation do not match, print an error message and return.
    if (password != confirmPassword) {
        std::cout << "Error: Passwords do not match." << std::endl;
        std::cout << "Press enter to continue..." << std::endl;
        std::cin.get();
        return;
    }

    // Hash the password using the MD5 algorithm.
    std::string hashedPassword = md5(password);

    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Prepare the INSERT statement.
    std::stringstream ss;
    ss << "INSERT INTO users (username, password) VALUES ('" << username << "', '" << hashedPassword << "')";
    std::string sql = ss.str();

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Execute the INSERT statement.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Print a message indicating that the user was added successfully.
    std::cout << "User added successfully." << std::endl;
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.get();
}

// This function handles changing a user's password in the database.
    // Prompt the user to enter the username of the user whose password they want
void adminChangePassword()
{
    // Clear the console screen.
    system("cls");
    // Declare variables to store the username of the user whose password will be changed,
    // the new password, and a confirmation of the new password.
    std::string username;
    std::string newPassword;
    std::string newPasswordConfirmation;

    // Prompt the user to enter the username of the user whose password they want to change.
    std::cout << "Enter the username of the user whose password you want to change: ";
    std::getline(std::cin, username);

    // Check if the entered username is valid.
    if (!userExists(username)) {
        std::cout << "User with username \"" << username << "\" does not exist." << std::endl;
        std::cout << "Press enter to continue..." << std::endl;
        std::cin.get();
        return;
    }

    // Prompt the user to enter the new password.
    std::cout << "Enter the new password: ";
    std::getline(std::cin, newPassword);

    // Prompt the user to confirm the new password.
    std::cout << "Confirm the new password: ";
    std::getline(std::cin, newPasswordConfirmation);

    // Check if the new password and its confirmation match.
    if (newPassword != newPasswordConfirmation) {
        std::cout << "The new password and its confirmation do not match." << std::endl;
        std::cout << "Press enter to continue..." << std::endl;
        std::cin.get();
        return;
    }

    // Open the database.
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Prepare the UPDATE statement.
    std::stringstream ss;
    ss << "UPDATE users SET Password = '" << newPassword << "' WHERE Username = '" << username << "'";
    std::string sql = ss.str();

    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Execute the UPDATE statement.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        std::cerr << "Error updating password: " << sqlite3_errmsg(db) << std::endl;
        return;
    }

    // Finalize the statement and close the database.
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    // Print a success message.
    std::cout << "Password changed successfully." << std::endl;
    std::cout << "Press enter to continue..." << std::endl;
    std::cin.get();

}



