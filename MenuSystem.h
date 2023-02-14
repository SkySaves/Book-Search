
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <limits>

/* ----------------- METHODS ----------------- */

// All but foolproof cin becuase I could not stop getting input errors
// -Zane
void saferCin(std::string &input)
{
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
void saferCin(int &input)
{
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

/* ---------------- CLASSES ---------------- */

// Small Menu object to handle menu creation and input
// I think it makes it eaiser to make menus with 🤷‍♂️
// Unfortunately, there is no support for user input and args.
// -Zane
class Menu
{
private:
    std::vector<std::tuple<std::string, int>> options;
    std::string title;

public:
    Menu(std::string title)
    {
        this->title = title;
    }

    // Used to add an option to the menu
    void addOption(std::string name, int returnValue)
    {
        options.push_back(std::tuple<std::string, int>(name, returnValue));
    }

    // Used to display said options
    void display()
    {
        std::cout << title << std::endl;
        for (int i = 0; i < options.size(); i++)
        {
            std::cout << i + 1
                      << ": "
                      << std::get<0>(options[i])
                      << std::endl;
        }
    }

    // Used to get the user's choice
    // Uses the vector size to determine if the choice is valid
    void getChoice(int &choice)
    {
        std::cout << "Enter your choice: ";
        saferCin(choice);

        if (choice < 1 || choice > options.size())
        {
            std::cout << "Invalid choice. Try again.";
        }
        else
        {
            choice = std::get<1>(options[choice - 1]);
        }
    }

    // Used to get the user's choice of prompt
    // Uses the vector size to determine if the choice is valid
    void getChoice(int &choice, std::string prompt)
    {
        std::cout << prompt;
        saferCin(choice);

        if (choice < 1 || choice > options.size())
        {
            std::cout << "Invalid choice. Try again.";
        }
        else
        {
            choice = std::get<1>(options[choice - 1]);
        }
    }
};