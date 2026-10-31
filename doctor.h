
#ifndef DOCTOR_H
#define DOCTOR_H

#include "person.h"
#include <string>
using namespace std;

class Doctor : public Person {
private:
    string specialization;
    string department;
    bool available;

public:
    Doctor(int id, string name, int age, string contact,
           string specialization, string department);

    void displayDetails() const override;

    string getSpecialization() const;
    string getDepartment() const;

    bool isAvailable() const;
    void setAvailability(bool status);
};

#endif
