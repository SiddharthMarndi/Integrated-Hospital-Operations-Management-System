
#ifndef PATIENT_H
#define PATIENT_H

#include "person.h"
#include <string>
using namespace std;

class Patient : public Person {
private:
    string patientType;

public:
    Patient(int id, string name, int age, string contact, string type);

    void displayDetails() const override;

    string getPatientType() const {
        return patientType;
    }
};

#endif
