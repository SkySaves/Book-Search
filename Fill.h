#include "Login.h"
#include <algorithm>


//This function is used to update the quantity of books in the database.

static void updateBookQuantities(const std::string& databaseFilename) {
    sqlite3* db;
    int rc = sqlite3_open(databaseFilename.c_str(), &db);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Error opening database: " + std::string(sqlite3_errmsg(db)));
    }

    std::srand(std::time(nullptr)); // seed the random number generator

    const char* sql = "UPDATE books SET Quantity = ? WHERE rowid = ?";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_close(db);
        throw std::runtime_error("Error preparing statement: " + std::string(sqlite3_errmsg(db)));
    }

    const int min_qty = 2;
    const int max_qty = 56;

    char* errmsg;
    rc = sqlite3_exec(db, "BEGIN TRANSACTION", nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        throw std::runtime_error("Error beginning transaction: " + std::string(sqlite3_errmsg(db)));
    }

    const char* select_sql = "SELECT rowid FROM books";
    sqlite3_stmt* select_stmt;
    rc = sqlite3_prepare_v2(db, select_sql, -1, &select_stmt, nullptr);
    if (rc != SQLITE_OK) {
        sqlite3_exec(db, "ROLLBACK TRANSACTION", nullptr, nullptr, &errmsg);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        throw std::runtime_error("Error preparing statement: " + std::string(sqlite3_errmsg(db)));
    }

    while (sqlite3_step(select_stmt) == SQLITE_ROW) {
        int rowid = sqlite3_column_int(select_stmt, 0);
        int qty = std::rand() % (max_qty - min_qty + 1) + min_qty;
        sqlite3_bind_int(stmt, 1, qty);
        sqlite3_bind_int(stmt, 2, rowid);
        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            sqlite3_exec(db, "ROLLBACK TRANSACTION", nullptr, nullptr, &errmsg);
            sqlite3_reset(stmt);
            sqlite3_clear_bindings(stmt);
            sqlite3_finalize(stmt);
            sqlite3_finalize(select_stmt);
            sqlite3_close(db);
            throw std::runtime_error("Error updating row " + std::to_string(rowid) + ": " + std::string(sqlite3_errmsg(db)));
        }
        sqlite3_reset(stmt);
        sqlite3_clear_bindings(stmt);
    }

    rc = sqlite3_exec(db, "COMMIT TRANSACTION", nullptr, nullptr, &errmsg);
    if (rc != SQLITE_OK) {
        sqlite3_free(errmsg);
        sqlite3_finalize(stmt);
        sqlite3_finalize(select_stmt);
        sqlite3_close(db);
        throw std::runtime_error("Error committing transaction: " + std::string(sqlite3_errmsg(db)));
    }

    sqlite3_finalize(stmt);
    sqlite3_finalize(select_stmt);
    sqlite3_close(db);

    std::cout << "Book quantities updated." << std::endl;
    std::cout << "Press enter";
    std::cin.get();
}




static void removeSpecialChars() {
    sqlite3* db;
    int rc = sqlite3_open("bookstore.db", &db);
    if (rc != SQLITE_OK) {
        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    const char* sql = "UPDATE books SET [Book-Title] = REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE(REPLACE([Book-Title], '0', 'Not available'), '1', 'Not available'), '2', 'Not available'), '3', 'Not available'), '4', 'Not available'), '5', 'Not available'), '6', 'Not available'), '7', 'Not available'), '8', 'Not available'), '9', 'Not available') WHERE 1=1;";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        std::cerr << "Error executing statement: " << sqlite3_errmsg(db) << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}



//static void updateYearOfPublisher() {
//    // Open the database.
//    sqlite3* db;
//    int rc = sqlite3_open("bookstore.db", &db);
//    if (rc != SQLITE_OK) {
//        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
//        return;
//    }
//
//    // Execute a SELECT statement to retrieve the rows to update.
//    const char* selectSql = "SELECT ROWID, [Year-Of-Publisher] FROM books WHERE [Year-Of-Publisher] NOT LIKE '%[0-9]%'";
//    sqlite3_stmt* selectStmt;
//    rc = sqlite3_prepare_v2(db, selectSql, -1, &selectStmt, nullptr);
//    if (rc != SQLITE_OK) {
//        std::cerr << "Error preparing SELECT statement: " << sqlite3_errmsg(db) << std::endl;
//        sqlite3_close(db);
//        return;
//    }
//
//    // Execute an UPDATE statement to modify the Year-Of-Publisher field for the selected rows.
//    const char* updateSql = "UPDATE books SET [Year-Of-Publisher] = ? WHERE ROWID = ?";
//    sqlite3_stmt* updateStmt;
//    rc = sqlite3_prepare_v2(db, updateSql, -1, &updateStmt, nullptr);
//    if (rc != SQLITE_OK) {
//        std::cerr << "Error preparing UPDATE statement: " << sqlite3_errmsg(db) << std::endl;
//        sqlite3_finalize(selectStmt);
//        sqlite3_close(db);
//        return;
//    }
//
//    while (sqlite3_step(selectStmt) == SQLITE_ROW) {
//        int rowid = sqlite3_column_int(selectStmt, 0);
//        const char* year = reinterpret_cast<const char*>(sqlite3_column_text(selectStmt, 1));
//        std::string newYear;
//        if (std::all_of(year, year + strlen(year), ::isdigit)) {
//            // If the string contains only digits, use it as is.
//            newYear = year;
//        }
//        else {
//            // Otherwise, replace any non-digit characters with 0.
//            std::transform(year, year + strlen(year), std::back_inserter(newYear), [](char c) { return std::isdigit(c) ? c : '0'; });
//        }
//
//        rc = sqlite3_bind_text(updateStmt, 1, newYear.c_str(), -1, SQLITE_TRANSIENT);
//        if (rc != SQLITE_OK) {
//            std::cerr << "Error binding text to UPDATE statement: " << sqlite3_errmsg(db) << std::endl;
//            sqlite3_finalize(selectStmt);
//            sqlite3_finalize(updateStmt);
//            sqlite3_close(db);
//            return;
//        }
//
//        rc = sqlite3_bind_int(updateStmt, 2, rowid);
//        if (rc != SQLITE_OK) {
//            std::cerr << "Error binding integer to UPDATE statement: " << sqlite3_errmsg(db) << std::endl;
//            sqlite3_finalize(selectStmt);
//            sqlite3_finalize(updateStmt);
//            sqlite3_close(db);
//            return;
//        }
//
//        rc = sqlite3_step(updateStmt);
//        if (rc != SQLITE_DONE) {
//            std::cerr << "Error executing UPDATE statement: " << sqlite3_errmsg(db) << std::endl;
//            sqlite3_finalize(selectStmt);
//            sqlite3_finalize(updateStmt);
//            sqlite3_close(db);
//            return;
//        }
//    }
//}







//void removeNonAlphabeticAuthors() {
//    // Open the database.
//    sqlite3* db;
//    int rc = sqlite3_open("bookstore.db", &db);
//    if (rc != SQLITE_OK) {
//        std::cerr << "Error opening database: " << sqlite3_errmsg(db) << std::endl;
//        return;
//    }
//
//    // Execute an UPDATE statement to replace any non-alphabetic characters in the Book-Author column with an empty string.
//    const char* sql = "UPDATE books SET [Book-Author] = REPLACE([Book-Author], SUBSTR([Book-Author], "\
//        "INSTR([Book-Author], ' ') + 1), '') WHERE [Book-Author] LIKE '% %';";
//    sqlite3_stmt* updateStmt;
//    rc = sqlite3_prepare_v2(db, sql, -1, &updateStmt, nullptr);
//    if (rc != SQLITE_OK) {
//        std::cerr << "Error preparing statement: " << sqlite3_errmsg(db) << std::endl;
//        sqlite3_close(db);
//        return;
//    }
//
//    rc = sqlite3_step(updateStmt);
//    if (rc != SQLITE_DONE) {
//        std::cerr << "Error executing UPDATE statement: " << sqlite3_errmsg(sqlite3_db_handle(updateStmt)) << std::endl;
//    }
//
//    // Finalize the statement and close the database.
//    sqlite3_finalize(updateStmt);
//    sqlite3_close(db);
//}
