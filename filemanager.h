#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "patient.h"
#include <vector>
#include <memory>
#include <string>
using namespace std;

class FileManager {
public:
    static void savePatient(const Patient& patient,
                            const string& filename = "patients.txt");

    static void loadPatients(vector<unique_ptr<Patient>>& patients,
                             int& nextId,
                             const string& filename = "patients.txt");
};

#endif