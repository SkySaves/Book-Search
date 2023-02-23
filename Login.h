#ifndef LoginHeader
#define LoginHeader



#include <iostream>
#include <fstream>
#include <string>
#include "sqlite3.h"
using namespace std;




bool userLogin(string username, string password) {
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

bool adminLogin(string adminUsername, string adminPassword) {
	sqlite3* db;
	sqlite3_stmt* stmt;

	// Opens the database
	int rc = sqlite3_open("bookstore.db", &db);
	if (rc != SQLITE_OK) {
		sqlite3_close(db);
		return false;
	}

	// Prepare the SELECT statement
	string sql = "SELECT AdminID FROM adminLogin WHERE Username = ? AND Password = ?";
	rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);


	// Bind the parameters to the statement
	rc = sqlite3_bind_text(stmt, 1, adminUsername.c_str(), -1, SQLITE_TRANSIENT);
	rc = sqlite3_bind_text(stmt, 2, adminPassword.c_str(), -1, SQLITE_TRANSIENT);


	// Execute the statement. This will verify if the admin exist or not.
	rc = sqlite3_step(stmt);
	bool result = false;
	if (rc == SQLITE_ROW) {
		result = true;
	}
	sqlite3_finalize(stmt);
	sqlite3_close(db);

	return result;
}




#endif