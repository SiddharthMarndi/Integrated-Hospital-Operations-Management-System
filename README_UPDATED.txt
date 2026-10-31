HOSPITAL MANAGEMENT SYSTEM - UPDATED SOURCE

Changes in this version:
- Doctor Management includes manual department reassignment.
- Allowed departments are OPD, Emergency, ICU, and Surgical.
- Allowed specialization choices are Cardiology, Ob-Gyn, Pediatrics, General Medicine, and Orthopedics.
- Department reassignment saves to doctors.txt and preserves doctor specialization and availability.
- Doctor records with invalid department values are skipped on load; valid records are saved back in canonical department format.
- OPD doctor assignment uses a numbered department menu and enforces a maximum of 3 queued/active OPD patients per doctor.
- Includes 40 sample fictional doctor records in doctors.txt (10 per department).

Build in PowerShell from this folder:
g++ -std=c++17 -Wall -Wextra main.cpp person.cpp patient.cpp doctor.cpp opdqueue.cpp emergencyqueue.cpp bedmanager.cpp pharmacy.cpp staffmanager.cpp billing.cpp filemanager.cpp -o hospital.exe

Run:
.\hospital.exe

Keep the .txt data files in the same working directory as hospital.exe. Back them up before replacing data files.
