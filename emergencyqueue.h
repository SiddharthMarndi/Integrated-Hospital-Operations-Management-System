#ifndef EMERGENCYQUEUE_H
#define EMERGENCYQUEUE_H
#include "patient.h"
#include <queue>
#include <vector>
#include <string>
using namespace std;
struct EmergencyEntry { Patient patient; int priority; EmergencyEntry(const Patient& p,int priority); };
struct EmergencyComparator { bool operator()(const EmergencyEntry& a,const EmergencyEntry& b) const; };
class EmergencyQueue {
private:
    priority_queue<EmergencyEntry,vector<EmergencyEntry>,EmergencyComparator> patients;
    string dataFile;
    void save() const;
public:
    explicit EmergencyQueue(const string& filename="emergency_queue.txt");
    bool addEmergencyPatient(const Patient& patient,int priority);
    bool serveNextPatient();
    void displayQueue();
    bool isEmpty();
};
#endif
