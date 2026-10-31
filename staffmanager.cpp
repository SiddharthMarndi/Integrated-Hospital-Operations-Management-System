#include "staffmanager.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

using namespace std;

namespace {
string trim(const string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

string normalizeDepartment(const string& department) {
    string result = trim(department);
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char ch) { return static_cast<char>(tolower(ch)); });

    if (result == "opd") return "OPD";
    if (result == "emergency") return "Emergency";
    if (result == "icu") return "ICU";
    if (result == "surgical" || result == "surgery") return "Surgical";
    return "";
}
}

void StaffManager::addDoctor(const Doctor& doctor) {
    if (hasDoctorId(doctor.getId())) {
        cout << "Doctor ID already exists.\n";
        return;
    }

    const string department = normalizeDepartment(doctor.getDepartment());
    if (department.empty()) {
        cout << "Invalid department. Use OPD, Emergency, ICU, or Surgical.\n";
        return;
    }

    doctors.emplace_back(doctor.getId(), doctor.getName(), doctor.getAge(),
                         doctor.getContact(), doctor.getSpecialization(), department);
    doctors.back().setAvailability(doctor.isAvailable());
    saveDoctors();
    cout << "Doctor record added and saved.\n";
}

void StaffManager::displayDoctors() const {
    if (doctors.empty()) {
        cout << "No doctors registered.\n";
        return;
    }

    cout << "\n--- Doctor Records ---\n";
    for (const Doctor& doctor : doctors) {
        doctor.displayDetails();
        cout << "----------------------------------------\n";
    }
}

bool StaffManager::findDoctor(int doctorId) const {
    for (const Doctor& doctor : doctors) {
        if (doctor.getId() == doctorId) {
            doctor.displayDetails();
            return true;
        }
    }
    cout << "Doctor not found.\n";
    return false;
}

vector<Doctor> StaffManager::getAvailableDoctorsByDepartment(
    const string& department) const {
    vector<Doctor> result;
    const string requested = normalizeDepartment(department);
    if (requested.empty()) return result;

    for (const Doctor& doctor : doctors) {
        if (normalizeDepartment(doctor.getDepartment()) == requested &&
            doctor.isAvailable()) {
            result.push_back(doctor);
        }
    }
    return result;
}

vector<Doctor> StaffManager::getDoctorsByDepartment(
    const string& department) const {
    vector<Doctor> result;
    const string requested = normalizeDepartment(department);
    if (requested.empty()) return result;

    for (const Doctor& doctor : doctors) {
        if (normalizeDepartment(doctor.getDepartment()) == requested) {
            result.push_back(doctor);
        }
    }
    return result;
}

bool StaffManager::updateDoctorAvailability(int doctorId, bool status) {
    for (Doctor& doctor : doctors) {
        if (doctor.getId() == doctorId) {
            doctor.setAvailability(status);
            saveDoctors();
            return true;
        }
    }
    return false;
}

bool StaffManager::updateDoctorDepartment(
    int doctorId, const string& department) {
    const string normalized = normalizeDepartment(department);
    if (normalized.empty()) return false;

    for (Doctor& doctor : doctors) {
        if (doctor.getId() == doctorId) {
            Doctor updated(doctor.getId(), doctor.getName(), doctor.getAge(),
                           doctor.getContact(), doctor.getSpecialization(), normalized);
            updated.setAvailability(doctor.isAvailable());
            doctor = updated;
            saveDoctors();
            return true;
        }
    }
    return false;
}

bool StaffManager::hasDoctorId(int doctorId) const {
    for (const Doctor& doctor : doctors) {
        if (doctor.getId() == doctorId) return true;
    }
    return false;
}

void StaffManager::loadDoctors(const string& filename) {
    doctors.clear();
    ifstream file(filename);
    if (!file) return;

    string line;
    while (getline(file, line)) {
        stringstream stream(line);
        string id, name, age, contact, specialization, department, available;

        if (!getline(stream, id, ',') || !getline(stream, name, ',') ||
            !getline(stream, age, ',') || !getline(stream, contact, ',') ||
            !getline(stream, specialization, ',') ||
            !getline(stream, department, ',') || !getline(stream, available)) {
            continue;
        }

        const string normalized = normalizeDepartment(department);
        if (normalized.empty()) {
            cerr << "Skipping doctor with invalid department: " << name << '\n';
            continue;
        }

        try {
            Doctor doctor(stoi(id), name, stoi(age), contact,
                          specialization, normalized);
            doctor.setAvailability(trim(available) == "1");

            if (!hasDoctorId(doctor.getId())) doctors.push_back(doctor);
        } catch (...) {
            cerr << "Skipping invalid doctor record.\n";
        }
    }

    // Rewrite accepted records in the canonical four-department format.
    saveDoctors(filename);
}

void StaffManager::saveDoctors(const string& filename) const {
    ofstream file(filename, ios::trunc);
    if (!file) {
        cerr << "Unable to save doctor records to " << filename << ".\n";
        return;
    }

    for (const Doctor& doctor : doctors) {
        file << doctor.getId() << ','
             << doctor.getName() << ','
             << doctor.getAge() << ','
             << doctor.getContact() << ','
             << doctor.getSpecialization() << ','
             << doctor.getDepartment() << ','
             << (doctor.isAvailable() ? 1 : 0) << '\n';
    }
}
