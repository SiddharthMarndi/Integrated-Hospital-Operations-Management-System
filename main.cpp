#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <limits>
#include <algorithm>

#include "patient.h"
#include "doctor.h"
#include "opdqueue.h"
#include "emergencyqueue.h"
#include "bedmanager.h"
#include "pharmacy.h"
#include "staffmanager.h"
#include "billing.h"
#include "filemanager.h"

using namespace std;

namespace Colors {
    const string CYAN = "\033[1;36m";
    const string GREEN = "\033[1;32m";
    const string YELLOW = "\033[1;33m";
    const string RED = "\033[1;31m";
    const string MAGENTA = "\033[1;35m";
    const string RESET = "\033[0m";
}

const vector<string> DEPARTMENTS = {"OPD", "Emergency", "ICU", "Surgical"};
const vector<string> SPECIALIZATIONS = {
    "Cardiology", "Ob-Gyn", "Pediatrics", "General Medicine", "Orthopedics"
};

void clearInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << Colors::RED << "Enter a valid whole number.\n" << Colors::RESET;
        cin.clear();
        clearInput();
    }
}

double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) return value;
        cout << Colors::RED << "Enter a valid number.\n" << Colors::RESET;
        cin.clear();
        clearInput();
    }
}

string readLine(const string& prompt) {
    cout << prompt;
    string value;
    getline(cin, value);
    return value;
}

void showBanner() {
    cout << '\n' << Colors::CYAN
         << "============================================\n"
         << "       HOSPITAL MANAGEMENT SYSTEM\n"
         << "============================================\n"
         << Colors::RESET;
}

void showSection(const string& title) {
    cout << '\n' << Colors::MAGENTA
         << "--------------------------------------------\n"
         << "  " << title << '\n'
         << "--------------------------------------------\n"
         << Colors::RESET;
}

Patient* findPatient(vector<unique_ptr<Patient>>& patients, int patientId) {
    for (auto& patient : patients) {
        if (patient->getId() == patientId) return patient.get();
    }
    return nullptr;
}

int chooseFromList(const string& prompt, const vector<string>& options) {
    cout << '\n';
    for (size_t i = 0; i < options.size(); ++i) {
        cout << "  " << (i + 1) << ". " << options[i] << '\n';
    }
    int choice = readInt(prompt);
    clearInput();
    if (choice < 1 || choice > static_cast<int>(options.size())) return -1;
    return choice - 1;
}

// ------------------------------------------------------------
// PATIENT RECORDS
// ------------------------------------------------------------
void patientMenu(vector<unique_ptr<Patient>>& patients, int& nextPatientId) {
    int choice;
    do {
        showSection("PATIENT RECORDS");
        cout << "1. Register patient\n"
             << "2. View all patients\n"
             << "3. Search patient by ID\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        clearInput();

        if (choice == 1) {
            string name = readLine("Full name: ");
            int age = readInt("Age: ");
            clearInput();
            string contact = readLine("Contact number: ");
            string type = readLine("Patient type (OPD/Emergency/Inpatient): ");

            if (name.empty() || age <= 0) {
                cout << Colors::RED << "Name and age must be valid.\n" << Colors::RESET;
                continue;
            }

            const int id = nextPatientId++;
            patients.push_back(make_unique<Patient>(id, name, age, contact, type));
            FileManager::savePatient(*patients.back(), "patients.txt");
            cout << Colors::GREEN << "Patient registered. ID: " << id
                 << Colors::RESET << '\n';
        } else if (choice == 2) {
            if (patients.empty()) cout << "No patient records found.\n";
            for (const auto& patient : patients) {
                patient->displayDetails();
                cout << "--------------------------------------------\n";
            }
        } else if (choice == 3) {
            int id = readInt("Patient ID: ");
            Patient* patient = findPatient(patients, id);
            if (patient) patient->displayDetails();
            else cout << Colors::RED << "Patient not found.\n" << Colors::RESET;
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ------------------------------------------------------------
// OPD SERVICES
// ------------------------------------------------------------
void opdMenu(vector<unique_ptr<Patient>>& patients, OPDQueue& queue, StaffManager& staff) {
    int choice;
    do {
        showSection("OPD SERVICES");
        cout << "1. Add patient to OPD queue\n"
             << "2. View OPD queue\n"
             << "3. Start next consultation\n"
             << "4. Finish active consultation\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        clearInput();

        if (choice == 1) {
            int patientId = readInt("Patient ID: ");
            Patient* patient = findPatient(patients, patientId);
            if (!patient) {
                cout << Colors::RED << "Patient not found.\n" << Colors::RESET;
                continue;
            }

            int deptIndex = chooseFromList("Select department: ", DEPARTMENTS);
            if (deptIndex < 0) {
                cout << "Invalid department selection.\n";
                continue;
            }

            const string department = DEPARTMENTS[static_cast<size_t>(deptIndex)];
            vector<Doctor> doctors = staff.getAvailableDoctorsByDepartment(department);
            if (doctors.empty()) {
                cout << "No available doctors in " << department << ".\n";
                continue;
            }

            cout << "\nAvailable doctors in " << department << ":\n";
            for (const Doctor& doctor : doctors) {
                const int assigned = queue.getPatientCountForDoctor(doctor.getId());
                cout << "ID " << doctor.getId()
                     << " | Dr. " << doctor.getName()
                     << " | " << doctor.getSpecialization()
                     << " | Assigned patients: " << assigned << "/3\n";
            }

            int doctorId = readInt("Assign doctor ID: ");
            auto selected = find_if(doctors.begin(), doctors.end(),
                [doctorId](const Doctor& doctor) { return doctor.getId() == doctorId; });

            if (selected == doctors.end()) {
                cout << Colors::RED << "Invalid doctor selection.\n" << Colors::RESET;
                continue;
            }

            if (queue.getPatientCountForDoctor(doctorId) >= 3) {
                cout << Colors::RED
                     << "This doctor already has the maximum of 3 assigned OPD patients.\n"
                     << Colors::RESET;
                continue;
            }

            queue.addPatient(*patient, doctorId);
        } else if (choice == 2) {
            queue.displayQueue();
        } else if (choice == 3) {
            queue.serveNextPatient(staff);
        } else if (choice == 4) {
            queue.finishConsultation(staff);
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ------------------------------------------------------------
// EMERGENCY SERVICES
// ------------------------------------------------------------
void emergencyMenu(vector<unique_ptr<Patient>>& patients, EmergencyQueue& queue) {
    int choice;
    do {
        showSection("EMERGENCY SERVICES");
        cout << "1. Add patient to emergency queue\n"
             << "2. View emergency queue\n"
             << "3. Serve next emergency\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        clearInput();

        if (choice == 1) {
            int patientId = readInt("Patient ID: ");
            Patient* patient = findPatient(patients, patientId);
            if (!patient) {
                cout << "Patient not found. Register the patient first.\n";
                continue;
            }
            int priority = readInt("Priority (1-5, 5 highest): ");
            if (priority < 1 || priority > 5) {
                cout << "Priority must be between 1 and 5.\n";
                continue;
            }
            if (queue.addEmergencyPatient(*patient, priority)) {
                cout << Colors::GREEN << "Added to emergency queue.\n" << Colors::RESET;
            }
        } else if (choice == 2) {
            queue.displayQueue();
        } else if (choice == 3) {
            queue.serveNextPatient();
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ------------------------------------------------------------
// DOCTOR MANAGEMENT
// ------------------------------------------------------------
void doctorMenu(StaffManager& staff) {
    int choice;
    do {
        showSection("DOCTOR MANAGEMENT");
        cout << "1. Add doctor\n"
             << "2. View all doctors\n"
             << "3. Search doctor by ID\n"
             << "4. Update doctor department\n"
             << "5. Update doctor availability\n"
             << "6. View doctors by department\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        clearInput();

        if (choice == 1) {
            int id = readInt("Doctor ID: ");
            int age = readInt("Age: ");
            clearInput();
            string name = readLine("Doctor name: ");
            string contact = readLine("Contact number: ");

            int specializationIndex = chooseFromList("Select specialization: ", SPECIALIZATIONS);
            if (specializationIndex < 0) {
                cout << "Invalid specialization.\n";
                continue;
            }
            int departmentIndex = chooseFromList("Select department: ", DEPARTMENTS);
            if (departmentIndex < 0) {
                cout << "Invalid department.\n";
                continue;
            }

            Doctor doctor(id, name, age, contact,
                          SPECIALIZATIONS[static_cast<size_t>(specializationIndex)],
                          DEPARTMENTS[static_cast<size_t>(departmentIndex)]);
            staff.addDoctor(doctor);
        } else if (choice == 2) {
            staff.displayDoctors();
        } else if (choice == 3) {
            staff.findDoctor(readInt("Doctor ID: "));
        } else if (choice == 4) {
            int doctorId = readInt("Doctor ID to transfer: ");
            if (!staff.hasDoctorId(doctorId)) {
                cout << Colors::RED << "Doctor not found.\n" << Colors::RESET;
                continue;
            }

            cout << "\nCurrent doctor record:\n";
            staff.findDoctor(doctorId);
            int departmentIndex = chooseFromList("Select new department: ", DEPARTMENTS);
            if (departmentIndex < 0) {
                cout << "Invalid department selection.\n";
                continue;
            }

            const string newDepartment = DEPARTMENTS[static_cast<size_t>(departmentIndex)];
            if (staff.updateDoctorDepartment(doctorId, newDepartment)) {
                cout << Colors::GREEN
                     << "Doctor transferred to " << newDepartment
                     << ". Specialization and availability were preserved.\n"
                     << Colors::RESET;
            } else {
                cout << Colors::RED << "Could not update doctor department.\n" << Colors::RESET;
            }
        } else if (choice == 5) {
            int doctorId = readInt("Doctor ID: ");
            int status = readInt("Available? (1 = Yes, 0 = No): ");
            if (status != 0 && status != 1) {
                cout << "Enter 1 or 0.\n";
                continue;
            }
            if (staff.updateDoctorAvailability(doctorId, status == 1)) {
                cout << Colors::GREEN << "Availability updated and saved.\n" << Colors::RESET;
            } else {
                cout << Colors::RED << "Doctor not found.\n" << Colors::RESET;
            }
        } else if (choice == 6) {
            int departmentIndex = chooseFromList("Select department: ", DEPARTMENTS);
            if (departmentIndex < 0) {
                cout << "Invalid department selection.\n";
                continue;
            }
            vector<Doctor> doctors = staff.getDoctorsByDepartment(
                DEPARTMENTS[static_cast<size_t>(departmentIndex)]);
            if (doctors.empty()) cout << "No doctors assigned to that department.\n";
            for (const Doctor& doctor : doctors) {
                doctor.displayDetails();
                cout << "----------------------------------------\n";
            }
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ------------------------------------------------------------
// BED MANAGEMENT
// ------------------------------------------------------------
void bedMenu(vector<unique_ptr<Patient>>& patients, BedManager& beds) {
    int choice;
    do {
        showSection("BED MANAGEMENT");
        cout << "1. View bed status\n"
             << "2. Allocate bed to patient\n"
             << "3. Discharge patient / release bed\n"
             << "4. View available bed count\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        clearInput();

        if (choice == 1) {
            beds.displayBeds();
        } else if (choice == 2) {
            int patientId = readInt("Patient ID: ");
            if (!findPatient(patients, patientId)) {
                cout << "Patient not found. Register the patient first.\n";
                continue;
            }
            bool alreadyAllocated = false;
            for (int bed = 1; bed <= beds.getTotalBeds(); ++bed) {
                if (beds.getPatientId(bed) == patientId) {
                    alreadyAllocated = true;
                    break;
                }
            }
            if (alreadyAllocated) cout << "Patient already has a bed.\n";
            else beds.allocateBed(patientId);
        } else if (choice == 3) {
            beds.releaseBedForPatient(readInt("Patient ID to discharge: "));
        } else if (choice == 4) {
            cout << "Available beds: " << beds.getAvailableBeds()
                 << " / " << beds.getTotalBeds() << '\n';
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ------------------------------------------------------------
// PHARMACY
// ------------------------------------------------------------
void pharmacyMenu(Pharmacy& pharmacy) {
    int choice;
    do {
        showSection("PHARMACY MANAGEMENT");
        cout << "1. View inventory\n"
             << "2. Add medicine / restock\n"
             << "3. Dispense medicine\n"
             << "4. Check stock\n"
             << "0. Back\n";
        choice = readInt("Choice: ");
        clearInput();

        if (choice == 1) {
            pharmacy.displayInventory();
        } else if (choice == 2) {
            string name = readLine("Medicine name: ");
            int quantity = readInt("Quantity to add: ");
            if (name.empty() || quantity <= 0) cout << "Enter a name and positive quantity.\n";
            else pharmacy.addMedicine(name, quantity);
        } else if (choice == 3) {
            string name = readLine("Medicine name: ");
            int quantity = readInt("Quantity to dispense: ");
            if (name.empty() || quantity <= 0) cout << "Enter a name and positive quantity.\n";
            else pharmacy.dispenseMedicine(name, quantity);
        } else if (choice == 4) {
            string name = readLine("Medicine name: ");
            cout << "Current stock: " << pharmacy.getStock(name) << '\n';
        } else if (choice != 0) {
            cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

// ------------------------------------------------------------
// BILLING
// ------------------------------------------------------------
void billingMenu(vector<unique_ptr<Patient>>& patients) {
    int patientId = readInt("Patient ID for billing: ");
    Patient* patient = findPatient(patients, patientId);
    if (!patient) {
        cout << Colors::RED << "Patient not found.\n" << Colors::RESET;
        return;
    }

    cout << "Billing patient: " << patient->getName()
         << " (ID: " << patient->getId() << ")\n";
    double consultation = readDouble("Consultation fee (Rs): ");
    double bed = readDouble("Bed charge (Rs): ");
    double medicine = readDouble("Medicine charge (Rs): ");

    if (consultation < 0 || bed < 0 || medicine < 0) {
        cout << Colors::RED << "Charges cannot be negative.\n" << Colors::RESET;
        return;
    }

    Billing bill(consultation, bed, medicine);
    bill.generateBill();
    bill.saveBill(patient->getId(), patient->getName());
}

int main() {
    vector<unique_ptr<Patient>> patients;
    int nextPatientId = 1001;

    OPDQueue opdQueue;
    EmergencyQueue emergencyQueue;
    BedManager beds(50);
    Pharmacy pharmacy;
    StaffManager staff;

    FileManager::loadPatients(patients, nextPatientId);
    staff.loadDoctors();

    int choice;
    do {
        showBanner();
        cout << Colors::GREEN
             << "  1. Patient Records\n"
             << "  2. OPD Services\n"
             << "  3. Emergency Services\n"
             << "  4. Doctor Management\n"
             << "  5. Bed Management\n"
             << "  6. Pharmacy\n"
             << "  7. Billing\n"
             << "  0. Exit\n"
             << Colors::RESET;
        cout << "Patients loaded: " << patients.size()
             << " | Beds available: " << beds.getAvailableBeds()
             << "/" << beds.getTotalBeds() << '\n';

        choice = readInt("Select module: ");
        clearInput();

        switch (choice) {
            case 1: patientMenu(patients, nextPatientId); break;
            case 2: opdMenu(patients, opdQueue, staff); break;
            case 3: emergencyMenu(patients, emergencyQueue); break;
            case 4: doctorMenu(staff); break;
            case 5: bedMenu(patients, beds); break;
            case 6: pharmacyMenu(pharmacy); break;
            case 7: billingMenu(patients); break;
            case 0:
                cout << Colors::YELLOW << "Exiting Hospital Management System.\n"
                     << Colors::RESET;
                break;
            default:
                cout << Colors::RED << "Invalid module.\n" << Colors::RESET;
        }
    } while (choice != 0);

    return 0;
}
