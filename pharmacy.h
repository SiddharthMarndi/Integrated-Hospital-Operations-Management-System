#ifndef PHARMACY_H
#define PHARMACY_H

#include <string>
#include <map>
using namespace std;

class Pharmacy {
private:
    map<string, int> inventory;

public:
    Pharmacy(const string& filename = "pharmacy.txt");
    void load(const string& filename);
    void save(const string& filename = "pharmacy.txt") const;
    string dataFile;
    void addMedicine(string name, int quantity);
    bool dispenseMedicine(string name, int quantity);
    void displayInventory();
    int getStock(string name);
};

#endif