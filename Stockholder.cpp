// Aksel Torgerson
// atorgerson
#include "Stockholder.h"

using namespace std;

struct PortfolioValue {
    float totalValue;
    void operator() (pair<string, pair<Stock*, int>> portfolio) {
        // Increment total value by the value of the stock times the number of shares the Stockholder owns
        totalValue += portfolio.second.first->new_value * portfolio.second.second;
    }
};

Stockholder::Stockholder() {}

Stockholder::Stockholder(string _name) {
    name = _name;
    portfolio = map<string, pair<Stock*, int>>();
    cash = 100000;
}

Stockholder::~Stockholder() {}

bool Stockholder::Purchase(Stock* stock, int count) {
    float totalPrice = stock->new_value * count;
    // Stockholder is allowed to order
    if (totalPrice < cash) {
        // Check if the stock exists in the Stockholders portfolio
        if (portfolio.find(stock->short_name) != portfolio.end()) {
            // Increase the int field of the pair in the map
            portfolio.find(stock->short_name)->second.second += count;
        } 
        // If it was not, make a new map entry and init it with the count
        else {
            portfolio.insert({stock->short_name, pair<Stock*, int>(stock, count)});
        }
        // Remove the cash from the Stockholders balance
        cash = cash - totalPrice;
        return true;
    } 
    // Stockholder does not have enough funds
    else {
        return false;
    }
}

bool Stockholder::Sell(string short_name, int count) {
    auto stock = portfolio.find(short_name);
    // If the Stockholder owns sufficient shares of the stock they wish to sell
    if (stock->second.second >= count) {
        // Sell value is the count * the new_value of the stock they are selling
        float sellValue = stock->second.second * stock->second.first->new_value;
        // Add that to the Stockholders cash on hand
        cash += sellValue;
        // Remove count shares from the Stockholders portfolio
        stock->second.second -= count;
        return true;
    } 
    // If not, return false
    else {
        return false;
    }
}

float Stockholder::NetWorth() {
    PortfolioValue value = for_each(portfolio.begin(), portfolio.end(), PortfolioValue());
    return value.totalValue + cash;
}

void Stockholder::Print() {
    cout << name << " $" << NetWorth() << endl;
    for (auto it = portfolio.begin(); it != portfolio.end(); it++) {
        it->second.first->Print();
        cout << " " << it->second.second << endl;
    }
}