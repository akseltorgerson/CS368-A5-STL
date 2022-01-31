// Aksel Torgerson
// atorgerson
#include <fstream>
#include "Stockholder.h"

using namespace std;

// Menu enumerations
enum MENU_TYPE {MAIN = 0, USER = 1};
enum MAIN_MENU {LOGIN = 1, LIST_DIR = 2, ADVANCE_DAY = 3, EXIT = 4};
enum USER_MENU {VIEW_ALL = 1, VIEW_OWNED = 2, PURCHASE = 3, SELL = 4, LOGOUT = 5};

// Globals
int numStocks = 0;
Stockholder currUser;

// Containers
map<string, Stock> sc = map<string, Stock>();
map<string, Stockholder> shc = map<string, Stockholder>();

void MainMenu() {
    cout << "Welcome to the Stock Portfolio Manager!"       << endl
        << "[" << LOGIN << "] Log In"                       << endl
        << "[" << LIST_DIR  << "] List Directory"           << endl
        << "[" << ADVANCE_DAY  << "] Advance a Day"         << endl
        << "[" << EXIT  << "] Exit Program"                 << endl 
        << "Please enter a menu item: ";
}

void UserMenu() {
    cout << "What would you like to do?"                    << endl
        << "[" << VIEW_ALL    << "] List Available Stocks"  << endl
        << "[" << VIEW_OWNED << "] List My Stocks"          << endl
        << "[" << PURCHASE    << "] Purchase Stocks"        << endl
        << "[" << SELL    << "] Sell Stocks"                << endl
        << "[" << LOGOUT  << "] Log Out"                    << endl
        << "Please enter a menu item: ";
}

void LoadStockData() {
    ifstream input;
    input.open("stocks.txt");
    input >> numStocks;
    for (int i = 0; i < numStocks; i++) {
        Stock stock = Stock();
        input >> stock;
        sc.insert({stock.short_name, stock});
    }
    input.close();
}

int main() {
    // Load data from file
    LoadStockData();

    // Menu navigation variables
    int menu = MAIN;
    int mainSel;
    int userSel;
    int num;

    // Other variables
    string name;
    string stock;

    // Program loop
    while (1) {

        // If we are in the main menu
        if (menu == MAIN) {

            // Print menu
            MainMenu();

            // Get valid input
            cin >> mainSel;
            while (!cin || mainSel < 1 || mainSel > 4) {
                if (!cin) { cin.clear(); cin.ignore(100, '\n'); }
                cout << "Please enter a valid menu item: ";
                cin >> mainSel;
            }

            // Handle menu options here
            switch (mainSel) {

                case LOGIN: {
                    // Prompt for user's name, ensure valid input
                    cout << "Please enter your name: ";
                    cin >> name;
                    while (!cin) {
                        if (!cin) { cin.clear(); cin.ignore(100, '\n'); }
                        cout << "Please enter a valid name: ";
                        cin >> name;
                    }

                    // Check if that user exists and set the current user
                    auto user = shc.find(name);
                    if (user != shc.end()) {
                        currUser = user->second;
                    }

                    // If that user does not exist, create a new user
                    else {
                        Stockholder newUser = Stockholder(name);
                        shc.insert({name, newUser});
                        currUser = newUser;
                    }

                    // Switch menu to user menu
                    menu = USER;
                    break;
                }

                case LIST_DIR: {
                    // Interate through all the users, printing them as we go
                    for (auto it = shc.begin(); it != shc.end(); it++) {
                        cout << it->first << endl;
                    }
                    break;
                }

                case ADVANCE_DAY: {
                    // Interate through all the stocks, advancing the day as we go
                    for (auto it = sc.begin(); it != sc.end(); it++) {
                        it->second.AdvanceDay();
                    }
                    break;
                }

                case EXIT: {
                    // Just exit the program
                    return 0;
                    break;
                }
            }
        } 

        // If we are in the user menu
        else {

            // Print user menu
            UserMenu();

            // Get valid input
            cin >> userSel;
            while (!cin || userSel < 1 || userSel > 5) {
                if (!cin) { cin.clear(); cin.ignore(100, '\n'); }
                cout << "Please enter a valid menu item: ";
                cin >> userSel;
            }

            // Handle menu options here
            switch (userSel) {

                case VIEW_ALL: {
                    // Print all available stocks
                    cout << "Available stocks:" << endl;
                    for (auto it = sc.begin(); it != sc.end(); it++) {
                        it->second.Print();
                        cout << endl;
                    }

                    // Prompt for any number to return to the main menu
                    cout << "Please enter any number to return to the main menu: ";
                    cin >> num;
                    while (!cin) {
                        if (!cin) { cin.clear(); cin.ignore(100, '\n'); }
                        cout << "Please enter a valid number: ";
                        cin >> num;
                    }

                    // Change menu back to main
                    menu = MAIN;
                    break;
                }

                case VIEW_OWNED: {
                    // Print out current users stocks
                    cout << name << "'s" << " Stocks:" << endl;
                    shc.find(name)->second.Print();

                    // Prompt for any number to return to the main menu
                    cout << "Please enter any number to return to the main menu: ";
                    cin >> num;
                    while (!cin) {
                        if (!cin) { cin.clear(); cin.ignore(100, '\n'); }
                        cout << "Please enter a valid number: ";
                        cin >> num;
                    }

                    // Change menu back to main
                    menu = MAIN;
                    break;
                }

                case PURCHASE: {
                    cout << "Enter a ticker of a stock you would like to purchase: ";
                    cin >> stock;
                    auto it = sc.find(stock);
                    if (it != sc.end()) {
                        cout << "Enter the number of shares you would like to purchase: ";
                        cin >> num;
                        auto it2 = shc.find(name);
                        if (it2->second.Purchase(&(it->second), num)) {
                            cout << "Purchase successful!" << endl;
                        } else {
                            cout << "Insufficient balance" << endl;
                        }
                    } else {
                        cout << "This stock is not available!" << endl;
                    }
                    break;
                }

                case SELL: {
                    cout << "Enter a ticker of a stock you would like to sell: ";
                    cin >> stock;
                    auto it = shc.find(name);
                    cout << "Enter the number of shares you would like to sell: ";
                    cin >> num;
                    if (it->second.Sell(stock, num)) {
                        cout << "Sale successful!" << endl;
                    } else {
                        cout << "Insufficient number of shares!" << endl;
                    }
                    break;
                }

                case LOGOUT: {
                    menu = MAIN;
                    break;
                }
            }
        }
    }
}