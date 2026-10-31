#ifndef BEDMANAGER_H
#define BEDMANAGER_H
#include <string>
#include <vector>
using namespace std;
class BedManager {
private:
    vector<int> bedPatientIds; // 0 means available; otherwise patient ID
    string dataFile;
public:
    BedManager(int totalBeds, const string& filename = "beds.txt");
    void load(const string& filename);
    void save(const string& filename = "beds.txt") const;
    int allocateBed(int patientId);
    bool releaseBed(int bedNumber);
    bool releaseBedForPatient(int patientId);
    int getPatientId(int bedNumber) const;
    void displayBeds();
    int getAvailableBeds();
    int getTotalBeds() const;
};
#endif
