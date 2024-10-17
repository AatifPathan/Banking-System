#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Customer {
public:
    string name;
    int id;

    Customer() : name(""), id(0) {}
    Customer(string n, int i) : name(n), id(i) {}
};

class Account {
private:
    int accountNumber;
    double balance;

public:
    Account() : accountNumber(0), balance(0.0) {}
    Account(int accNum, double bal) : accountNumber(accNum), balance(bal) {}

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposit successful. New balance: " << balance << endl;
    }

    bool withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient funds." << endl;
            return false;
        }
        balance -= amount;
        cout << "Withdrawal successful. New balance: " << balance << endl;
        return true;
    }

    void showBalance() {
        cout << "Account Number: " << accountNumber << ", Balance: " << balance << endl;
    }
};

class BankingSystem {
private:
    Customer customers[10];
    Account accounts[10];
    int customerCount;
    string dataFile;

public:
    BankingSystem(string filename) : customerCount(0), dataFile(filename) {
        loadFromFile();
    }

    ~BankingSystem() {
        saveToFile();
    }

    void createCustomer() {
        if (customerCount >= 10) {
            cout << "Cannot create more customers, limit reached!" << endl;
            return;
        }

        string name;
        int id, accNum;
        double initialDeposit;

        cin.ignore();
        cout << "Enter customer name: ";
        getline(cin, name);
        cout << "Enter customer ID: ";
        cin >> id;
        cout << "Enter account number: ";
        cin >> accNum;
        cout << "Enter initial deposit: ";
        cin >> initialDeposit;

        customers[customerCount] = Customer(name, id);
        accounts[customerCount] = Account(accNum, initialDeposit);
        customerCount++;

        cout << "Customer and account created successfully." << endl;
    }

    void deposit() {
        int accountNumber;
        double amount;

        cout << "Enter account number: ";
        cin >> accountNumber;
        cout << "Enter deposit amount: ";
        cin >> amount;

        for (int i = 0; i < customerCount; i++) {
            if (accounts[i].getAccountNumber() == accountNumber) {
                accounts[i].deposit(amount);
                return;
            }
        }

        cout << "Account not found." << endl;
    }

    void withdraw() {
        int accountNumber;
        double amount;

        cout << "Enter account number: ";
        cin >> accountNumber;
        cout << "Enter withdrawal amount: ";
        cin >> amount;

        for (int i = 0; i < customerCount; i++) {
            if (accounts[i].getAccountNumber() == accountNumber) {
                accounts[i].withdraw(amount);
                return;
            }
        }

        cout << "Account not found." << endl;
    }

    void transfer() {
        int fromAccount, toAccount;
        double amount;

        cout << "Enter source account number: ";
        cin >> fromAccount;
        cout << "Enter destination account number: ";
        cin >> toAccount;
        cout << "Enter transfer amount: ";
        cin >> amount;

        Account* from = nullptr;
        Account* to = nullptr;

        for (int i = 0; i < customerCount; i++) {
            if (accounts[i].getAccountNumber() == fromAccount) {
                from = &accounts[i];
            }
            if (accounts[i].getAccountNumber() == toAccount) {
                to = &accounts[i];
            }
        }

        if (from && to && from->withdraw(amount)) {
            to->deposit(amount);
            cout << "Transfer successful." << endl;
        } else {
            cout << "Transfer failed." << endl;
        }
    }

    void showBalance() {
        int accountNumber;
        cout << "Enter account number: ";
        cin >> accountNumber;

        for (int i = 0; i < customerCount; i++) {
            if (accounts[i].getAccountNumber() == accountNumber) {
                accounts[i].showBalance();
                return;
            }
        }

        cout << "Account not found." << endl;
    }

    void deleteCustomer() {
        int accountNumber;

        cout << "Enter account number to delete: ";
        cin >> accountNumber;

        for (int i = 0; i < customerCount; i++) {
            if (accounts[i].getAccountNumber() == accountNumber) {
                for (int j = i; j < customerCount - 1; j++) {
                    customers[j] = customers[j + 1];
                    accounts[j] = accounts[j + 1];
                }
                customerCount--;
                cout << "Customer and account deleted successfully." << endl;
                return;
            }
        }

        cout << "Account not found." << endl;
    }

    void saveToFile() {
        ofstream file(dataFile, ios::trunc);
        if (!file) {
            cout << "Error opening file for saving!" << endl;
            return;
        }

        file << customerCount << endl;
        for (int i = 0; i < customerCount; i++) {
            file << customers[i].name << " " << customers[i].id << " "
                 << accounts[i].getAccountNumber() << " " << accounts[i].getBalance() << endl;
        }

        file.close();
        cout << "Data saved to file." << endl;
    }

    void loadFromFile() {
        ifstream file(dataFile);
        if (!file) {
            cout << "No previous data found." << endl;
            return;
        }

        file >> customerCount;
        for (int i = 0; i < customerCount; i++) {
            string name;
            int id, accNum;
            double balance;

            file >> name >> id >> accNum >> balance;
            customers[i] = Customer(name, id);
            accounts[i] = Account(accNum, balance);
        }

        file.close();
        cout << "Data loaded from file." << endl;
    }
};

int main() {
    BankingSystem bank("bank_data.txt");

    int choice;
    while (true) {
        cout << "\n--- Banking System Menu ---\n";
        cout << "1. Create Customer and Account\n";
        cout << "2. Deposit\n";
        cout << "3. Withdraw\n";
        cout << "4. Transfer\n";
        cout << "5. Show Balance\n";
        cout << "6. Delete Customer\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                bank.createCustomer();
                break;
            case 2:
                bank.deposit();
                break;
            case 3:
                bank.withdraw();
                break;
            case 4:
                bank.transfer();
                break;
            case 5:
                bank.showBalance();
                break;
            case 6:
                bank.deleteCustomer();
                break;
            case 7:
                cout << "Exiting..." << endl;
                return 0;
            default:
                cout << "Invalid choice! Try again." << endl;
        }
    }

    return 0;
}
