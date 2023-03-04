//Transferred everything from the Handlers.h and Handlers.cpp files to Books.h and Books.cpp



//#include "Handlers.h"
//
//
//
//
//void Handler::searchBookHandler() {
//    // Clears the console screen.
//    system("cls");
//
//    // Creates an instance of a class named "Menu" with the title "How would you like to search: \n".
//    Menu searchMenu("How would you like to search: \n");
//
//
//    // Adds options to the menu with the name "Title", "Author", "ISBN", and "Back to main menu".
//    searchMenu.addOption("Title", 0);
//    searchMenu.addOption("Author", 1);
//    searchMenu.addOption("ISBN", 2);
//    searchMenu.addOption("Back to main menu", 3);
//
//
//
//    // An infinite loop that displays the menu and allows the user to select an option.
//    while (true)
//    {
//        // Clears the console screen.
//        system("cls");
//
//        // Displays the search menu.
//        searchMenu.display();
//
//        // Gets the user's choice from the search menu.
//        searchMenu.getChoice(searchChoice, "Enter your search query: ");
//
//        // Switch statement to execute the appropriate action based on the user's choice.
//        switch (searchChoice)
//        {
//        case 0:
//            // Prompts the user to enter the title of the book they want to search for.
//            std::cout << "Enter the title of the book: ";
//            saferCin(query);
//            break;
//        case 1:
//            // Prompts the user to enter the author of the book they want to search for.
//            std::cout << "Enter the author of the book: ";
//            saferCin(query);
//            break;
//        case 2:
//            // Prompts the user to enter the ISBN of the book they want to search for.
//            std::cout << "Enter the ISBN of the book: ";
//            saferCin(query);
//            break;
//
//        case 3:
//            // Returns to the previous menu.
//            return;
//
//        default:
//            break;
//        }
//
//        // Calls the searchBooks function with the user's search query and choice as arguments.
//        bookss.searchBooks(query, searchChoice);
//    }
//}
//
//
//
//void Handler::listHandler() {
//    // Clears the console screen.
//    system("cls");
//
//    // Creates an instance of a class named "Menu" with the title "What would you like to do: \n".
//    Menu listMenu("What would you like to do: \n");
//
//    // Adds options to the menu with the name "Add a book to the list", "Remove a book from the list", "Display the list", and "Back to main menu".
//    listMenu.addOption("Add a book to the list", 0);
//    listMenu.addOption("Remove a book from the list", 1);
//    listMenu.addOption("Display the list", 2);
//    listMenu.addOption("Back to main menu", 3);
//    // An infinite loop that displays the menu and allows the user to select an option.
//    while (true)
//    {
//        // Clears the console screen.
//        system("cls");
//
//        // Displays the list menu.
//        listMenu.display();
//
//        // Gets the user's choice from the list menu.
//        int listChoice;
//        listMenu.getChoice(listChoice);
//
//        // Switch statement to execute the appropriate action based on the user's choice.
//        switch (listChoice)
//        {
//        case 0:
//            // Calls the addToList function to add a book to the list.
//            bookss.addToList();
//            break;
//
//        case 1:
//            // Calls the removeFromList function to remove a book from the list.
//            bookss.removeFromList();
//            break;
//
//        case 2:
//            // Calls the displayList function to display the books in the list.
//            bookss.displayList();
//            break;
//
//        case 3:
//            // Returns to the previous menu.
//            return;
//
//        default:
//            break;
//        }
//    }
//}
//
//
//
//
//void Handler::addBookHandler() {
//    // Clears the console screen.
//    system("cls");
//
//    // Prompts the user to enter the title of the book.
//    std::cout << "Enter the title of the book: ";
//    saferCin(bookTitle);
//
//    // Prompts the user to enter the author of the book.
//    std::cout << "Enter the author of the book: ";
//    saferCin(bookAuthor);
//
//    // Prompts the user to enter the publisher of the book.
//    std::cout << "Enter the publisher of the book: ";
//    saferCin(bookPublisher);
//
//    // Prompts the user to enter the year the book was published.
//    std::cout << "Enter the year the book was published: ";
//    saferCin(bookYear);
//
//    std::cout << "Enter the MSRP of the book: ";
//    saferCin(bookMSRP);
//
//    std::cout << "Enter the quantity of the book: ";
//    saferCin(bookQuantity);
//
//    std::cout << "Enter the description of the book: ";
//    saferCin(bookDescription);
//
//    // Loop to ensure the ISBN entered is 13 characters long.
//    int loopAmount = 0;
//    while (bookISBN.length() != 13)
//    {
//        if (loopAmount > 0)
//        {
//            // Prompts the user to enter a valid ISBN if the previous ISBN was not 13 characters long.
//            std::cout << "ISBN was not !-13-! characters.\nPlease try again: ";
//        }
//        else
//        {
//            // Prompts the user to enter the ISBN of the book.
//            std::cout << "Enter the !-13-! char ISBN of the book: ";
//        }
//
//        // Reads in the ISBN from the user.
//        saferCin(bookISBN);
//    }
//
//    // Calls the addBook function with the book information as arguments.
//    bookss.addBook(bookISBN, bookTitle, bookAuthor, bookYear, bookPublisher, bookMSRP, bookQuantity, bookDescription);
//}
//
//
//
//
//
//
//void Handler::addShopperHandler() {
//
//    Shopping shoppingg;
//    std::cout << "Enter the shopper's name: ";
//    std::cin >> name;
//    std::cout << "Enter the shopper's email: ";
//    std::cin >> email;
//    std::cout << "Enter the shopper's total amount spent: ";
//    std::cin >> totalAmountSpent;
//
//    shoppingg.addShopper(name, email, totalAmountSpent);
//}
//
//
//
