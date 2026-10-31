#include "person.h"
#include <iostream>
using namespace std;

// Constructor
Person::Person(int i, string n, int a, string c) {
    id = i;
    name = n;
    age = a;
    contact = c;
}

// Display person details
void Person::displayDetails() const {
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;
    cout << "Age: " << age << endl;
    cout << "Contact: " << contact << endl;
}

int Person::getId() const {
    return id;
}

string Person::getName() const {
    return name;
}

int Person::getAge() const {
    return age;
}

string Person::getContact() const {
    return contact;
}