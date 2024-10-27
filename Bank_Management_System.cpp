#include <iostream>
#include <fstream>
#include <ctype.h>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <string.h>
#include <limits>

using namespace std;

class Date {
public:
    int day;
    int month;
    int year;

    Date operator+(int m) {
        Date res;
        int x = m / 12 + year;
        int y = m % 12 + month;
        if (y > 12) {
            y = y - 12;
            x++;
        }
        if ((y == 4 || y == 6 || y == 9 || y == 11) && day == 31) {
            day = 1;
            y++;
        }
        else if (y == 2) {
            if (x % 4 == 0 && day > 29) {
                day = day - 29;
                y++;
            }
            else if (day > 28) {
                day = day - 28;
                y++;
            }
        }
        res.day = day;
        res.month = y;
        res.year = x;

        return res;
    }
};

class Account {
    int AccNo;
    char Name[50];
    int dep;
    char accType;

public:
    void createAcc();
    void displayAcc();
    void update();
    void credit(int);
    void debit(int);
    void query();
    int getAccNo();
    int getCredit();
    char getType();
};

class recurrDep : public Account {
    Date d;
    int per;

public:
    void getrecurr() {
        Account::createAcc();
        cout << "Enter date of FD in dd mm yyyy format : ";
        cin >> d.day >> d.month >> d.year;
        cout << "Enter period of maturity in months : ";
        cin >> per;
    }

    void showDet() {
        Account::displayAcc();
        cout << "\nDate of FD : " << d.day << " : " << d.month << " : " << d.year << endl;
        cout << "Period of maturity : " << per << endl;
        maturity();
    }

    void maturity() {
        Date res = d + per;
        cout << "Date of maturity : " << res.day << " : " << res.month << " : " << res.year << endl;
    }
};

void Account::createAcc() {
    cout << endl << "Enter your Account Number : ";
    cin >> AccNo;

    cout << endl << "Enter the Account holder's name : ";
    cin.ignore(); // Using cin.ignore() to handle input buffer
    cin.getline(Name, 50);

    cout << endl << "Enter your Account accType, C for Current Account/ S for Savings Account/R for Fixed Deposit : ";
    cin >> accType;

    cout << endl << "Enter initial amount (for Saving and Fixed Deposit >=500Rs. and for Current >=1000 ) : ";
    cin >> dep;

    // Validation for initial amount
    if ((accType == 'S' || accType == 'R') && dep < 500) {
        cout << "Initial amount must be at least 500 for Savings and Fixed Deposit accounts." << endl;
        dep = 0; // Reset amount to 0
    }
    else if (accType == 'C' && dep < 1000) {
        cout << "Initial amount must be at least 1000 for Current accounts." << endl;
        dep = 0; // Reset amount to 0
    }

    if (dep >= 500 || (accType == 'C' && dep >= 1000)) {
        cout << endl << endl << "Account Created Successfully!\n";
    }
    else {
        cout << "Account creation failed due to invalid initial amount.\n";
    }
}

void Account::displayAcc() {
    cout << endl << "Account No. is" << "\t: " << AccNo;
    cout << endl << "Account Holder's name is" << ": " << Name;
    cout << endl << "\nType of Account" << "\t: " << accType << endl;
    cout << "Balance amount is" << "\t: " << dep;
}

void Account::update() {
    cout << endl << "The Account No. is" << "\t: " << AccNo;
    cout << endl << endl << "Enter account holder's name" << "\t: ";
    cin.ignore();
    cin.getline(Name, 50);
    cout << endl << "Enter your Account accType, C for Current Account/ S for Savings Account :"
         << "\t: ";
    cin >> accType;
    accType = toupper(accType);
    cout << endl << "Enter the amount to be credited" << "\t: ";
    cin >> dep;
}

void Account::credit(int x) {
    dep += x;
}

void Account::debit(int x) {
    if (dep >= x)
        dep -= x;
    else {
        cout << "Not enough balance!!" << endl;
        return;
    }
}

void Account::query() {
    cout << AccNo << "\t\t" << Name;
    int length = strlen(Name);
    for (int count = 0; count < 32 - length; count++)
        cout << ' ';

    cout << accType;

    for (int count = 0; count < 20; count++)
        cout << ' ';

    cout << dep << endl;
}

int Account::getAccNo() {
    return AccNo;
}

int Account::getCredit() {
    return dep;
}

char Account::getType() {
    return accType;
}

void writeAcc() {
    Account Acc;
    ofstream outFile;
    outFile.open("Account.dat", ios::binary | ios::app);
    Acc.createAcc();
    if (Acc.getCredit() > 0) { // Only write if account creation was successful
        outFile.write((char*)&Acc, sizeof(Account));
    }
    outFile.close();
}

void writeRecurringDeposit(recurrDep& rd) {
    ofstream outFile("RecurringDeposits.dat", ios::binary | ios::app);
    if (!outFile) {
        cout << "Unable to open the file to write..." << endl;
        return;
    }
    outFile.write((char*)&rd, sizeof(recurrDep));
    outFile.close();
}

void displayAccDetails(int n) {
    Account Acc;
    int key = 0;
    ifstream inFile;
    inFile.open("Account.dat", ios::binary);
    if (!inFile) {
        cout << "Unable to open the file..." << endl;
        return;
    }
    cout << endl << "Details of balance are : " << endl;
    while (inFile.read((char*)&Acc, sizeof(Account))) {
        if (Acc.getAccNo() == n) {
            Acc.displayAcc();
            key = 1;
        }
    }
    inFile.close();
    if (key == 0)
        cout << endl << "Invalid Account Number!";
}

void updateAcc(int n) {
    int key = 0;
    Account Acc;
    fstream File;
    File.open("Account.dat", ios::binary | ios::in | ios::out);
    if (!File) {
        cout << "Unable to open the file..." << endl;
        return;
    }
    while (File.read((char*)&Acc, sizeof(Account)) && key == 0) {
        if (Acc.getAccNo() == n) {
            Acc.displayAcc();
            cout << endl << "Enter new Account Details" << endl;
            Acc.update();
            long long location = (-1) * sizeof(Account);
            File.seekp(location, ios::cur);
            File.write((char*)&Acc, sizeof(Account));
            system("clear"); // Use "cls" for Windows
            cout << "\nRecord Updated Successfully\n";
            key = 1;
        }
    }
    File.close();
    if (key == 0)
        cout << endl << "Record was not found!";
}

void deleteAcc(int n) {
    Account Acc;
    ifstream inFile;
    ofstream outFile;
    inFile.open("Account.dat", ios::binary);
    if (!inFile) {
        cout << "Unable to open the file..." << endl;
        return;
    }
    outFile.open("Temp.dat", ios::binary);
    inFile.seekg(0, ios::beg);
    while (inFile.read((char*)&Acc, sizeof(Account))) {
        if (Acc.getAccNo() != n)
            outFile.write((char*)&Acc, sizeof(Account));
    }
    inFile.close();
    outFile.close();
    remove("Account.dat");
    rename("Temp.dat", "Account.dat");
    cout << "\nAccount Deleted Successfully\n";
}

void showAll() {
    Account Acc;
    ifstream inFile;
    inFile.open("Account.dat", ios::binary);
    if (!inFile) {
        cout << "Unable to open the file..." << endl;
        return;
    }
    cout << "\n\n\t\tList of Active Account Holders : " << endl
         << endl;

    cout << "Acc No.\t\tNAME\t\t\t\tType\t\t     Balance" << endl
         << endl;

    while (inFile.read((char*)&Acc, sizeof(Account))) {
        if (Acc.getCredit() > 0) // Only show accounts with a positive balance
            Acc.query();
    }

    inFile.close();
}

void debt(int n) {
    int amount;
    Account Acc;
    ifstream inFile;
    inFile.open("Account.dat", ios::binary);
    if (!inFile) {
        cout << "Unable to open the file..." << endl;
        return;
    }
    while (inFile.read((char*)&Acc, sizeof(Account))) {
        if (Acc.getAccNo() == n) {
            Acc.displayAcc();
            cout << "\nEnter amount to be debited : ";
            cin >> amount;
            Acc.debit(amount);
            cout << "\n\n\tAmount Debited Successfully!" << endl;
            cout << "\nUpdated Account Details:\n";
            Acc.displayAcc();
        }
    }
    inFile.close();
}

void creditAmount(int n) {
    int amount;
    Account Acc;
    ifstream inFile;
    inFile.open("Account.dat", ios::binary);
    if (!inFile) {
        cout << "Unable to open the file..." << endl;
        return;
    }
    while (inFile.read((char*)&Acc, sizeof(Account))) {
        if (Acc.getAccNo() == n) {
            Acc.displayAcc();
            cout << "\nEnter amount to be credited : ";
            cin >> amount;
            Acc.credit(amount);
            cout << "\n\n\tAmount Credited Successfully!" << endl;
            cout << "\nUpdated Account Details:\n";
            Acc.displayAcc();
        }
    }
    inFile.close();
}

void createRecurringDeposit() {
    recurrDep rd;
    rd.getrecurr(); // Collects details for the recurring deposit
    writeRecurringDeposit(rd); // Ensure this function is called to write data
}

void showRecurringDeposits() {
    ifstream inFile("RecurringDeposits.dat", ios::binary);
    recurrDep rd;

    if (!inFile) {
        cout << "Unable to open the file 'RecurringDeposits.dat'..." << endl;
        return; // Exit the function if the file cannot be opened
    }

    cout << "\nDetails of Recurring Deposits:\n";
    while (inFile.read((char*)&rd, sizeof(recurrDep))) {
        rd.showDet(); // Show the details of the recurring deposit
    }
    inFile.close();
}


int main() {
    int choice;

    do {
        cout << "\nMenu:\n";
        cout << "1. Create Account\n";
        cout << "2. Show All Accounts\n";
        cout << "3. Update Account\n";
        cout << "4. Delete Account\n";
        cout << "5. Credit Amount\n";
        cout << "6. Debit Amount\n";
        cout << "7. Query Account\n";
        cout << "8. Create Recurring Deposit\n"; 
        cout << "9. Show Recurring Deposits\n"; 
        cout << "10. Exit\n";
        cout << "Enter your choice: ";

        // Input validation for choice
        while (!(cin >> choice)) {
            cout << "Invalid input! Please enter a number (1-10): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        switch (choice) {
            case 1:
                writeAcc();
                break;
            case 2:
                showAll();
                break;
            case 3: {
                int accNo;
                cout << "Enter Account Number to update: ";
                cin >> accNo;
                updateAcc(accNo);
                break;
            }
            case 4: {
                int accNo;
                cout << "Enter Account Number to delete: ";
                cin >> accNo;
                deleteAcc(accNo);
                break;
            }
            case 5: {
                int accNo;
                cout << "Enter Account Number to credit amount: ";
                cin >> accNo;
                creditAmount(accNo);
                break;
            }
            case 6: {
                int accNo;
                cout << "Enter Account Number to debit amount: ";
                cin >> accNo;
                debt(accNo);
                break;
            }
            case 7: {
                int accNo;
                cout << "Enter Account Number to query: ";
                cin >> accNo;
                displayAccDetails(accNo);
                break;
            }
            case 8:
                createRecurringDeposit();
                break;
            case 9:
                showRecurringDeposits();
                break;
            case 10:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice! Please try again." << endl;
                break;
        }
    } while (choice != 10);

    return 0;
}

