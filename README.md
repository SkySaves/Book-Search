# Book-Search
Book search program for CIS 227

TODO:

1. After seasrch for a book, typing 'back' does not actually immediately bring you to the main menu. I found that pressing 'enter' multiple times brings you back to the main menu.

2. Correct SQL code for the ISBN search in the database when the user selects search book by ISBN. It currently prompts an error.

3. When the user selects "Display the book list" we get a null pointer error thrown. This needs to be corrected.

4. When the user selects the Author method of searching for a book, once the search completes it prompts to enter an ISBN to buy. The user has to press 'enter' twice to bring the next prompt up. This should be a single 'enter'. After it asks how many would you like to buy, it gives a SQL error.

5. The "Add a book to the shopping list" requires the user to enter far too much information about the book, it should only ask for an ISBN and quantity they would like to purchase, not only that, but it currently just doesn't work after you enter the information.

6. "View the shopping list" just brings up the regular list menu, which most options do not work. When You Enter '5' to Exit it prompts you with an error and says that it not a valid choice even though it is listed.

7. When you log into the admin functions, and chose to change a user's password, it does not take any input and prompts you that "" does not exists. Then you press enter and it closes.


There are probably more bugs but that is what I found for today 02/22/2023
