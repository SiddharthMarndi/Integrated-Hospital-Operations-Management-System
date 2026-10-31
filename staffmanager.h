#ifndef STAFFMANAGER_H
#define STAFFMANAGER_H

#include "doctor.h"
#include <string>
#include <vector>

class StaffManager {
private:
    std::vector<Doctor> doctors;

public:
    void addDoctor(const Doctor& doctor);
    void loadDoctors(const std::string& filename = "doctors.txt");
    void saveDoctors(const std::string& filename = "doctors.txt") const;
    void displayDoctors() const;
    bool findDoctor(int doctorId) const;

    std::vector<Doctor> getAvailableDoctorsByDepartment(
        const std::string& department) const;

    std::vector<Doctor> getDoctorsByDepartment(
        const std::string& department) const;

    bool updateDoctorAvailability(int doctorId, bool status);
    bool updateDoctorDepartment(int doctorId, const std::string& department);
    bool hasDoctorId(int doctorId) const;
};

#endif
