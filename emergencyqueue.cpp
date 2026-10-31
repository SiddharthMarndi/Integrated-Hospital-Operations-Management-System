#include "emergencyqueue.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
EmergencyEntry::EmergencyEntry(const Patient& p,int priority):patient(p),priority(priority){}
bool EmergencyComparator::operator()(const EmergencyEntry& a,const EmergencyEntry& b) const { if(a.priority==b.priority)return a.patient.getId()>b.patient.getId(); return a.priority<b.priority; }
EmergencyQueue::EmergencyQueue(const string& filename):dataFile(filename){ ifstream file(dataFile); string line; while(getline(file,line)){string id,name,age,contact,type,pri;stringstream ss(line);if(!getline(ss,id,'|')||!getline(ss,name,'|')||!getline(ss,age,'|')||!getline(ss,contact,'|')||!getline(ss,type,'|')||!getline(ss,pri))continue;try{int p=stoi(pri);if(p>=1&&p<=5)patients.emplace(Patient(stoi(id),name,stoi(age),contact,type),p);}catch(...){cerr<<"Skipping invalid emergency entry.\n";}} }
void EmergencyQueue::save() const {ofstream file(dataFile,ios::trunc);if(!file){cerr<<"Unable to save emergency queue.\n";return;}auto copy=patients;while(!copy.empty()){const auto&e=copy.top();file<<e.patient.getId()<<'|'<<e.patient.getName()<<'|'<<e.patient.getAge()<<'|'<<e.patient.getContact()<<'|'<<e.patient.getPatientType()<<'|'<<e.priority<<'\n';copy.pop();}}
bool EmergencyQueue::addEmergencyPatient(const Patient& patient,int priority){if(priority<1||priority>5){cout<<"Priority must be between 1 and 5.\n";return false;}patients.emplace(patient,priority);save();cout<<"Emergency patient added to priority queue.\n";return true;}
bool EmergencyQueue::serveNextPatient(){if(patients.empty()){cout<<"No emergency patients waiting.\n";return false;}cout<<"\nServing emergency patient:\nPriority: "<<patients.top().priority<<'\n';patients.top().patient.displayDetails();patients.pop();save();return true;}
void EmergencyQueue::displayQueue(){if(patients.empty()){cout<<"Emergency queue is empty.\n";return;}auto copy=patients;cout<<"\n--- Emergency Queue ---\n";while(!copy.empty()){cout<<"Priority: "<<copy.top().priority<<'\n';copy.top().patient.displayDetails();cout<<"-----------------------\n";copy.pop();}}
bool EmergencyQueue::isEmpty(){return patients.empty();}
