#pragma once

#ifndef LOGINHEADER
#define LOGINHEADER


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "md5.h"
#include "sqlite3.h"
#include <conio.h>
#include "Books.h"
#include "MenuSystem.h"
#include "Shopping.h"
#include "Handlers.h"

using namespace std;

static sqlite3_stmt* stmt2;

class Login
{
private:

    std::string password;
    char c;
    string username;
    string tryAgain;

    // Declare variables to store admin login information and a retry prompt.
    string adminUsername;
    string adminPassword;
    string oldPassword;
    string newPassword;
    string adminTryAgain = "y";


public:
    bool userExists(std::string username);

    std::string readPassword();

    bool userLogin(string username, string password);

//This function takes in the username and password from the user and checks if the user exists in the database, then returns true or false.
    bool adminLogin(string adminUsername, string adminPassword);

    int authUserHandler();

    void registrationHandle();

    int adminLoginHandler();

    void changeCustomerPasswordHandler();

    void changeCustomerPassword();

    void changeAdminPasswordHandler();

    void importFilesHandler();
};


#endif