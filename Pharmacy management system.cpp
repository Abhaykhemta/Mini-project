#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
using namespace std;

class Medicine {
public:
int id;
char name[50];
int quantity;
float price;
char expiry[15];

void input() {
cout << "Enter ID: ";
cin >> id;
cout << "Enter Name: ";
cin.ignore();
cin.getline(name, 50);
cout << "Enter Quantity: ";
cin >> quantity;
cout << "Enter Price: ";
cin >> price;
cout << "Enter Expiry Date (DD/MM/YYYY): ";
cin >> expiry;
}

void display() {
cout << left << setw(10) << id
<< setw(20) << name
<< setw(10) << quantity
<< setw(10) << price
<< setw(15) << expiry << endl;
}
};

// 🔐 LOGIN SYSTEM
bool login() {
string user, pass;
cout << "\n===== LOGIN =====\n";
cout << "Username: ";
cin >> user;
cout << "Password: ";
cin >> pass;

if (user == "admin" && pass == "1234") {
cout << "Login Successful!\n";
return true;
} else {
cout << "Invalid Credentials!\n";
return false;
}
}

// ➕ ADD MEDICINE
void addMedicine() {
Medicine m;
ofstream file("medicine.dat", ios::binary | ios::app);

m.input();
file.write((char*)&m, sizeof(m));

cout << "Medicine Added!\n";
file.close();
}

// 📋 VIEW MEDICINE
void viewMedicine() {
Medicine m;
ifstream file("medicine.dat", ios::binary);

cout << "\nID Name Qty Price Expiry\n";
cout << "--------------------------------------------\n";

while (file.read((char*)&m, sizeof(m))) {
m.display();
}
file.close();
}

// 🔍 SEARCH
void searchMedicine() {
int id;
Medicine m;
bool found = false;

cout << "Enter ID: ";
cin >> id;

ifstream file("medicine.dat", ios::binary);

while (file.read((char*)&m, sizeof(m))) {
if (m.id == id) {
cout << "\nFound:\n";
m.display();
found = true;
}
}

if (!found) cout << "Not Found!\n";

file.close();
}

// ⚠️ LOW STOCK ALERT
void lowStock() {
Medicine m;
ifstream file("medicine.dat", ios::binary);

cout << "\n⚠ LOW STOCK MEDICINES (Qty < 10)\n";

while (file.read((char*)&m, sizeof(m))) {
if (m.quantity < 10) {
m.display();
}
}

file.close();
}

// 🧾 BILLING WITH GST
void billing() {
int id, qty;
float total = 0;
char choice;
string customer;

cout << "\nEnter Customer Name: ";
cin.ignore();
getline(cin, customer);

do {
Medicine m;
bool found = false;

cout << "Enter Medicine ID: ";
cin >> id;
cout << "Quantity: ";
cin >> qty;

fstream file("medicine.dat", ios::binary | ios::in | ios::out);

while (file.read((char*)&m, sizeof(m))) {
if (m.id == id) {
if (m.quantity >= qty) {
float cost = qty * m.price;
total += cost;

m.quantity -= qty;
int pos = -1 * sizeof(m);
file.seekp(pos, ios::cur);
file.write((char*)&m, sizeof(m));

cout << m.name << " = " << cost << endl;
} else {
cout << "Insufficient stock!\n";
}
found = true;
break;
}
}

if (!found) cout << "Not found!\n";

file.close();

cout << "More items? (y/n): ";
cin >> choice;

} while (choice == 'y' || choice == 'Y');

float gst = total * 0.18;
float final = total + gst;

cout << "\n===== BILL =====\n";
cout << "Customer: " << customer << endl;
cout << "Subtotal: " << total << endl;
cout << "GST (18%): " << gst << endl;
cout << "Total: " << final << endl;

ofstream sales("sales.txt", ios::app);
sales << customer << " " << final << endl;
sales.close();
}

// 📊 SALES REPORT
void salesReport() {
ifstream file("sales.txt");
string name;
float amount;

cout << "\n===== SALES REPORT =====\n";

while (file >> name >> amount) {
cout << "Customer: " << name << " | Amount: " << amount << endl;
}

file.close();
}

// 🗑 DELETE
void deleteMedicine() {
int id;
Medicine m;

cout << "Enter ID to delete: ";
cin >> id;

ifstream file("medicine.dat", ios::binary);
ofstream temp("temp.dat", ios::binary);

while (file.read((char*)&m, sizeof(m))) {
if (m.id != id)
temp.write((char*)&m, sizeof(m));
}

file.close();
temp.close();

remove("medicine.dat");
rename("temp.dat", "medicine.dat");

cout << "Deleted!\n";
}

// MAIN
int main() {
if (!login()) return 0;

int choice;

do {
cout << "\n===== PHARMACY SYSTEM =====\n";
cout << "1. Add Medicine\n";
cout << "2. View Medicine\n";
cout << "3. Search\n";
cout << "4. Low Stock Alert\n";
cout << "5. Billing\n";
cout << "6. Sales Report\n";
cout << "7. Delete Medicine\n";
cout << "8. Exit\n";
cout << "Choice: ";
cin >> choice;

switch (choice) {
case 1: addMedicine(); break;
case 2: viewMedicine(); break;
case 3: searchMedicine(); break;
case 4: lowStock(); break;
case 5: billing(); break;
case 6: salesReport(); break;
case 7: deleteMedicine(); break;
}

} while (choice != 8);

return 0;
}
