
#ifndef PERSON_H
#define PERSON_H

#include <string>
using namespace std;

class Person {
protected:
    int id;
    string name;
    int age;
    string contact;

public:
    // Constructor
    Person(int i, string n, int a, string c);

    // Display person details
    virtual void displayDetails() const;

int getId() const;
string getName() const;
int getAge() const;
string getContact() const;

    // Virtual destructor
    virtual ~Person() = default;
};

#endif
