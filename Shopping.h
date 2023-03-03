#pragma once

#ifndef SHOPPING
#define SHOPPING

#include "Login.h"


class Shopping
{

	private:
		std::string title;
		std::string author;
		std::string publisher;
		std::string ISBN;
		std::string description;
		std::string bookTitle, bookAuthor, bookISBN, bookPublisher, bookDescription;
		int year;
		double MSRP;
		int bookYear, bookMSRP, bookQuantity;
		std::string name;
		std::string email;
		double totalAmountSpent;
		std::string isbn;
		int quantity;

		std::vector <Shopping> shoppingList;

		std::vector <Shopping> ShoppingBooks;


	public:

		void addToShoppingListHandler();

		//void addToShoppingList();

		void addShopper(std::string name, std::string email, double totalAmountSpent);

		void addShopperHandler();
		void addBookToShoppingList(const Shopping& book, int quantity);

};



#endif