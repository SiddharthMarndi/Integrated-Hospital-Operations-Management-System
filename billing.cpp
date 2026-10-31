#include "billing.h"
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

Billing::Billing(double consultationFee,
                 double bedCharge,
                 double medicineCharge) {
    this->consultationFee = consultationFee;
    this->bedCharge = bedCharge;
    this->medicineCharge = medicineCharge;
}

void Billing::setCharges(double consultationFee,
                         double bedCharge,
                         double medicineCharge) {
    this->consultationFee = consultationFee;
    this->bedCharge = bedCharge;
    this->medicineCharge = medicineCharge;
}

double Billing::calculateTotal() {
    return consultationFee + bedCharge + medicineCharge;
}

void Billing::generateBill() {
    cout << "\n========== PATIENT BILL ==========\n";
    cout << "Consultation Fee: Rs. " << consultationFee << endl;
    cout << "Bed Charge:       Rs. " << bedCharge << endl;
    cout << "Medicine Charge:  Rs. " << medicineCharge << endl;
    cout << "---------------------------------\n";
    cout << "Total Amount:     Rs. " << calculateTotal() << endl;
    cout << "=================================\n";
}
void Billing::saveBill(int patientId, const std::string& patientName, const std::string& filename) {
    std::ofstream file(filename, std::ios::app);
    if (!file) { std::cout << "Unable to save bill history.\n"; return; }
    file << patientId << ',' << patientName << ',' << std::fixed << std::setprecision(2)
         << consultationFee << ',' << bedCharge << ',' << medicineCharge << ','
         << calculateTotal() << '\n';
    std::cout << "Bill saved to " << filename << ".\n";
}
