#include "patient.h"
#include <iostream>
using namespace std;

// Patient constructor
Patient::Patient(int id, string name, int age, string contact, string type)
    : Person(id, name, age, contact) {
    patientType = type;
}

// Display patient details
void Patient::displayDetails() const {
    Person::displayDetails();
    cout << "Patient Type: " << patientType << endl;
}