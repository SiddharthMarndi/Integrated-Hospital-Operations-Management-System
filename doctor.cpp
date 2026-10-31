
#include "doctor.h"
#include <iostream>
using namespace std;

Doctor::Doctor(int id, string name, int age, string contact,
               string specialization, string department)
    : Person(id, name, age, contact),
      specialization(specialization),
      department(department),
      available(true) {
}

void Doctor::displayDetails() const {
    Person::displayDetails();

    cout << "Specialization: " << specialization << endl;
    cout << "Department: " << department << endl;

    cout << "Availability: "
         << (available ? "Available" : "Busy") << endl;
}

string Doctor::getSpecialization() const {
    return specialization;
}

string Doctor::getDepartment() const {
    return department;
}

bool Doctor::isAvailable() const {
    return available;
}

void Doctor::setAvailability(bool status) {
    available = status;
}
