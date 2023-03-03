#include "Login.h"



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


