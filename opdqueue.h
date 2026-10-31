#ifndef OPDQUEUE_H
#define OPDQUEUE_H
#include "patient.h"
#include "staffmanager.h"
#include <queue>
#include <string>
#include <optional>
using namespace std;
struct OPDEntry { Patient patient; int doctorId; OPDEntry(const Patient& patient, int doctorId); };
class OPDQueue {
private:
    queue<OPDEntry> patients;
    optional<OPDEntry> currentPatient;
    string dataFile;
    void save() const;
public:
    explicit OPDQueue(const string& filename = "opd_queue.txt");
    void addPatient(const Patient& patient, int doctorId);
    bool serveNextPatient(StaffManager& staffManager);
    bool finishConsultation(StaffManager& staffManager);
    void displayQueue();
    bool isEmpty();
    int getQueueSize();
    int getPatientCountForDoctor(int doctorId) const;
    bool hasActiveConsultation() const;
};
#endif
