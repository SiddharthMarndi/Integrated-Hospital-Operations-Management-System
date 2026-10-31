#include "pharmacy.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

Pharmacy::Pharmacy(const string& filename) : dataFile(filename) { load(dataFile); }

void Pharmacy::load(const string& filename) {
    inventory.clear();
    ifstream file(filename);
    string name; int quantity;
    while (file >> std::ws && getline(file, name, '\t') && file >> quantity) {
        if (!name.empty() && quantity >= 0) inventory[name] = quantity;
        file.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

void Pharmacy::save(const string& filename) const {
    ofstream file(filename, ios::trunc);
    if (!file) { cout << "Unable to save pharmacy inventory.\n"; return; }
    for (const auto& item : inventory) file << item.first << '\t' << item.second << '\n';
}

void Pharmacy::addMedicine(string name, int quantity) {
    if (quantity <= 0) {
        cout << "Quantity must be positive.\n";
        return;
    }

    inventory[name] += quantity;
    save(dataFile);

    cout << quantity << " units of " << name
         << " added to inventory.\n";
}

bool Pharmacy::dispenseMedicine(string name, int quantity) {
    if (quantity <= 0) {
        cout << "Quantity must be positive.\n";
        return false;
    }

    if (inventory.find(name) == inventory.end() ||
        inventory[name] < quantity) {
        cout << "Insufficient stock or medicine not found.\n";
        return false;
    }

    inventory[name] -= quantity;
    save(dataFile);

    cout << quantity << " units of " << name
         << " dispensed.\n";

    return true;
}

void Pharmacy::displayInventory() {
    cout << "\n--- Pharmacy Inventory ---\n";

    if (inventory.empty()) {
        cout << "Inventory is empty.\n";
        return;
    }

    for (const auto& item : inventory) {
        cout << "Medicine: " << item.first
             << " | Quantity: " << item.second << endl;
    }
}

int Pharmacy::getStock(string name) {
    if (inventory.find(name) == inventory.end()) {
        return 0;
    }

    return inventory[name];
}