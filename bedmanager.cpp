#include "bedmanager.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
BedManager::BedManager(int totalBeds, const string& filename) : bedPatientIds(totalBeds, 0), dataFile(filename) { load(dataFile); }
void BedManager::load(const string& filename) {
    ifstream file(filename); if (!file) return;
    string line;
    while (getline(file,line)) {
        istringstream ss(line); int number=0, value=0;
        if (!(ss >> number >> value) || number<1 || number>static_cast<int>(bedPatientIds.size())) continue;
        // New format stores patient ID. Old format stored 0/1 occupancy.
        bedPatientIds[number-1] = (value == 1 ? 0 : value);
    }
}
void BedManager::save(const string& filename) const {
    ofstream file(filename,ios::trunc); if(!file){cerr<<"Unable to save bed status.\n";return;}
    for(size_t i=0;i<bedPatientIds.size();++i) file << i+1 << ' ' << bedPatientIds[i] << '\n';
}
int BedManager::allocateBed(int patientId) {
    if(patientId<=0){cout<<"Invalid patient ID.\n";return -1;}
    for(size_t i=0;i<bedPatientIds.size();++i) if(bedPatientIds[i]==0){bedPatientIds[i]=patientId;save(dataFile);cout<<"Bed "<<i+1<<" allocated to patient "<<patientId<<".\n";return static_cast<int>(i+1);}
    cout<<"No beds available.\n";return -1;
}
bool BedManager::releaseBed(int bedNumber) {
    if(bedNumber<1||bedNumber>static_cast<int>(bedPatientIds.size())){cout<<"Invalid bed number.\n";return false;}
    if(bedPatientIds[bedNumber-1]==0){cout<<"Bed is already available.\n";return false;}
    bedPatientIds[bedNumber-1]=0;save(dataFile);cout<<"Bed "<<bedNumber<<" released.\n";return true;
}
bool BedManager::releaseBedForPatient(int patientId){for(size_t i=0;i<bedPatientIds.size();++i)if(bedPatientIds[i]==patientId)return releaseBed(static_cast<int>(i+1));cout<<"Patient has no allocated bed.\n";return false;}
int BedManager::getPatientId(int bedNumber) const {if(bedNumber<1||bedNumber>static_cast<int>(bedPatientIds.size()))return 0;return bedPatientIds[bedNumber-1];}
void BedManager::displayBeds(){cout<<"\n\033[1;36m--- Bed Status ---\033[0m\n";for(size_t i=0;i<bedPatientIds.size();++i){cout<<"Bed "<<i+1<<": ";if(bedPatientIds[i])cout<<"\033[1;31mOccupied\033[0m (Patient ID "<<bedPatientIds[i]<<")\n";else cout<<"\033[1;32mAvailable\033[0m\n";}}
int BedManager::getAvailableBeds(){int n=0;for(int id:bedPatientIds)if(id==0)++n;return n;}
int BedManager::getTotalBeds() const{return static_cast<int>(bedPatientIds.size());}
