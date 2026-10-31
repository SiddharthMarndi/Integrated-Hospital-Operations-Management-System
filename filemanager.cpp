#include "filemanager.h"
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;

void FileManager::savePatient(const Patient& patient,
                              const string& filename) {
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cout << "Unable to open file for saving.\n";
        return;
    }

    file << patient.getId() << ","
         << patient.getName() << ","
         << patient.getAge() << ","
         << patient.getContact() << ","
         << patient.getPatientType() << endl;

    file.close();
}

void FileManager::loadPatients(vector<unique_ptr<Patient>>& patients,
                               int& nextId,
                               const string& filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        return;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string idStr, name, ageStr, contact, type;

        getline(ss, idStr, ',');
        getline(ss, name, ',');
        getline(ss, ageStr, ',');
        getline(ss, contact, ',');
        getline(ss, type, ',');

        if (idStr.empty() || name.empty() || ageStr.empty() ||
            contact.empty() || type.empty()) {
            continue;
        }

        try {
            int id = stoi(idStr);
            int age = stoi(ageStr);

            patients.push_back(
                make_unique<Patient>(id, name, age, contact, type)
            );

            if (id >= nextId) {
                nextId = id + 1;
            }
        }
        catch (...) {
            cout << "Skipping invalid patient record.\n";
        }
    }

    file.close();
}