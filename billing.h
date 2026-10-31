#ifndef BILLING_H
#define BILLING_H

#include <string>
class Billing {
private:
    double consultationFee;
    double bedCharge;
    double medicineCharge;

public:
    Billing(double consultationFee = 0,
            double bedCharge = 0,
            double medicineCharge = 0);

    void setCharges(double consultationFee,
                    double bedCharge,
                    double medicineCharge);

    double calculateTotal();
    void saveBill(int patientId, const std::string& patientName, const std::string& filename = "bills.txt");
    void generateBill();
};

#endif