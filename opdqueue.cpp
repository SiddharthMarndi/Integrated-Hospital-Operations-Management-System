#include "opdqueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
OPDEntry::OPDEntry(const Patient& patient, int doctorId): patient(patient), doctorId(doctorId) {}
OPDQueue::OPDQueue(const string& filename): dataFile(filename) {
    ifstream file(dataFile); string line;
    while (getline(file,line)) {
        string id,name,age,contact,type,doctor;
        stringstream ss(line);
        if (!getline(ss,id,'|') || !getline(ss,name,'|') || !getline(ss,age,'|') || !getline(ss,contact,'|') || !getline(ss,type,'|') || !getline(ss,doctor)) continue;
        try { patients.emplace(Patient(stoi(id),name,stoi(age),contact,type),stoi(doctor)); } catch (...) { cerr << "Skipping invalid OPD queue entry.\n"; }
    }
}
void OPDQueue::save() const {
    ofstream file(dataFile,ios::trunc); if (!file) { cerr << "Unable to save OPD queue.\n"; return; }
    auto copy=patients;
    while(!copy.empty()) { const auto& e=copy.front(); file << e.patient.getId()<<'|'<<e.patient.getName()<<'|'<<e.patient.getAge()<<'|'<<e.patient.getContact()<<'|'<<e.patient.getPatientType()<<'|'<<e.doctorId<<'\n'; copy.pop(); }
}
void OPDQueue::addPatient(const Patient& patient,int doctorId) { patients.emplace(patient,doctorId); save(); cout<<"Patient added to OPD queue for Doctor ID: "<<doctorId<<'\n'; }
bool OPDQueue::serveNextPatient(StaffManager& sm) {
    if(currentPatient) { cout<<"A consultation is already in progress.\n"; return false; }
    if(patients.empty()) { cout<<"OPD queue is empty.\n"; return false; }
    auto& next=patients.front();
    if(!sm.updateDoctorAvailability(next.doctorId,false)) { cout<<"Assigned doctor not found.\n"; return false; }
    currentPatient=next;
    cout<<"\n========== CONSULTATION STARTED ==========\n"; currentPatient->patient.displayDetails(); cout<<"Assigned Doctor ID: "<<currentPatient->doctorId<<"\nDoctor status: Busy\n==========================================\n";
    return true;
}
bool OPDQueue::finishConsultation(StaffManager& sm) {
    if(!currentPatient) { cout<<"No active consultation.\n"; return false; }
    int did=currentPatient->doctorId;
    if(!sm.updateDoctorAvailability(did,true)) { cout<<"Could not update doctor availability.\n"; return false; }
    if (!patients.empty()) {
        patients.pop();
    }
    currentPatient.reset();
    save();
    cout<<"Consultation completed. Doctor ID "<<did<<" is now available.\n"; return true;
}
void OPDQueue::displayQueue() { if(patients.empty()){cout<<"OPD queue is empty.\n";return;} auto copy=patients; cout<<"\n--- OPD Queue ---\n"; while(!copy.empty()){copy.front().patient.displayDetails();cout<<"Assigned Doctor ID: "<<copy.front().doctorId<<"\n-----------------\n";copy.pop();} }
bool OPDQueue::isEmpty(){return patients.empty();}
int OPDQueue::getQueueSize(){return static_cast<int>(patients.size());}
bool OPDQueue::hasActiveConsultation() const{return currentPatient.has_value();}

int OPDQueue::getPatientCountForDoctor(int doctorId) const {
    int count = 0;
    auto copy = patients;
    while (!copy.empty()) {
        if (copy.front().doctorId == doctorId) ++count;
        copy.pop();
    }
    if (currentPatient && currentPatient->doctorId == doctorId) {
        // The active consultation is still present at the front of the queue,
        // so it is already counted above. Do not count it twice.
    }
    return count;
}
