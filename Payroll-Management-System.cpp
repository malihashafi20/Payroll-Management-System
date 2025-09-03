#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>
#include <iomanip>
#include <algorithm>
#include <ctime>

// Cross-platform console utilities
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

char getChar() {
    std::cout << "Press Enter to continue...";
    std::cin.get();
    return '\n';
}

// Utility functions
void printHeader(const std::string& title) {
    std::cout << std::string(80, '=') << std::endl;
    std::cout << std::setw(40 + title.length()/2) << title << std::endl;
    std::cout << std::string(80, '=') << std::endl;
}

void printSeparator() {
    std::cout << std::string(80, '-') << std::endl;
}

bool isValidDate(int d, int m, int y) {
    if (y < 1900 || y > 2100 || m < 1 || m > 12 || d < 1 || d > 31) {
        return false;
    }
    
    if (m == 2) {
        bool isLeap = (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0));
        if (d > (isLeap ? 29 : 28)) {
            return false;
        }
    } else if (m == 4 || m == 6 || m == 9 || m == 11) {
        if (d > 30) {
            return false;
        }
    }
    return true;
}

std::string getValidatedInput(const std::string& prompt, int maxLength) {
    std::string input;
    bool valid = false;
    
    do {
        std::cout << prompt << ": ";
        std::getline(std::cin, input);
        
        if (input == "0") return input; // Exit code
        
        if (input.length() > static_cast<size_t>(maxLength) || input.empty()) {
            std::cout << "Invalid input! Length should be between 1 and " << maxLength << " characters.\n";
            valid = false;
        } else {
            valid = true;
        }
    } while (!valid);
    
    std::transform(input.begin(), input.end(), input.begin(), ::toupper);
    return input;
}

class Employee {
public:
    int code{};
    std::string name;
    std::string address;
    std::string phone;
    int dd{}, mm{}, yy{};
    std::string designation;
    char grade{};
    char house_allowance{};
    char travel_allowance{};
    float loan{};
    float basic_salary{};

    void display() const;
    void displayForList() const;
};

// File operations
const std::string FILE_NAME = "EMPLOYEE.DAT";

std::vector<Employee> readAllRecords() {
    std::vector<Employee> records;
    std::ifstream file(FILE_NAME, std::ios::binary);
    if (file.is_open()) {
        Employee emp;
        while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
            records.push_back(emp);
        }
        file.close();
    }
    return records;
}

void writeAllRecords(const std::vector<Employee>& records) {
    std::ofstream file(FILE_NAME, std::ios::binary | std::ios::trunc);
    if (file.is_open()) {
        for (const auto& emp : records) {
            file.write(reinterpret_cast<const char*>(&emp), sizeof(Employee));
        }
        file.close();
    }
}

void Employee::display() const {
    printSeparator();
    std::cout << "Employee Code  : " << code << std::endl;
    std::cout << "Name           : " << name << std::endl;
    std::cout << "Address        : " << address << std::endl;
    std::cout << "Phone          : " << phone << std::endl;
    std::cout << "Joining Date   : " << dd << "/" << mm << "/" << yy << std::endl;
    std::cout << "Designation    : " << designation << std::endl;
    std::cout << "Grade          : " << grade << std::endl;
    
    if (grade != 'E') {
        std::cout << "House Allowance: " << house_allowance << std::endl;
        std::cout << "Travel Allow.  : " << travel_allowance << std::endl;
        std::cout << "Basic Salary   : $" << std::fixed << std::setprecision(2) << basic_salary << std::endl;
    }
    std::cout << "Loan Amount    : $" << std::fixed << std::setprecision(2) << loan << std::endl;
    printSeparator();
}

void Employee::displayForList() const {
    std::cout << std::left 
              << std::setw(6) << code
              << std::setw(20) << name.substr(0, 19)
              << std::setw(12) << phone.substr(0, 11)
              << std::setw(12) << (std::to_string(dd) + "/" + std::to_string(mm) + "/" + std::to_string(yy))
              << std::setw(15) << designation.substr(0, 14)
              << std::setw(6) << grade
              << std::setw(10);
    
    if (grade != 'E') {
        std::cout << "$" << std::fixed << std::setprecision(0) << basic_salary;
    } else {
        std::cout << "-";
    }
    std::cout << std::endl;
}

class PayrollSystem {
private:
    void editMenu();
public:
    void mainMenu();
    void newEmployee();
    void displayEmployee();
    void listEmployees();
    void salarySlip();
    void deleteEmployee();
    void modifyEmployee();
};

void PayrollSystem::mainMenu() {
    int choice;
    
    while (true) {
        clearScreen();
        printHeader("PAYROLL MANAGEMENT SYSTEM");
        
        std::cout << "\n\n";
        std::cout << "        1. NEW EMPLOYEE\n";
        std::cout << "        2. DISPLAY EMPLOYEE\n";
        std::cout << "        3. LIST OF EMPLOYEES\n";
        std::cout << "        4. SALARY SLIP\n";
        std::cout << "        5. EDIT MENU\n";
        std::cout << "        0. QUIT\n\n";
        std::cout << "Enter your choice (0-5): ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 0:
                std::cout << "\nThank you for using Payroll Management System!\n";
                return;
            case 1:
                newEmployee();
                break;
            case 2:
                displayEmployee();
                break;
            case 3:
                listEmployees();
                break;
            case 4:
                salarySlip();
                break;
            case 5:
                editMenu();
                break;
            default:
                std::cout << "\nInvalid choice! Please enter 0-5.\n";
                pauseScreen();
                break;
        }
    }
}

void PayrollSystem::editMenu() {
    int choice;
    
    while (true) {
        clearScreen();
        printHeader("EDIT MENU");
        
        std::cout << "\n\n";
        std::cout << "        1. DELETE RECORD\n";
        std::cout << "        2. MODIFY RECORD\n";
        std::cout << "        0. BACK TO MAIN MENU\n\n";
        std::cout << "Enter your choice (0-2): ";
        
        std::cin >> choice;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        switch (choice) {
            case 0:
                return;
            case 1:
                deleteEmployee();
                break;
            case 2:
                modifyEmployee();
                break;
            default:
                std::cout << "\nInvalid choice! Please enter 0-2.\n";
                pauseScreen();
                break;
        }
    }
}

void PayrollSystem::newEmployee() {
    clearScreen();
    printHeader("ADD NEW EMPLOYEE");
    
    Employee newEmp;
    std::vector<Employee> allEmployees = readAllRecords();

    // Auto-generate employee code
    int lastCode = 0;
    if (!allEmployees.empty()) {
        lastCode = allEmployees.back().code;
    }
    newEmp.code = lastCode + 1;

    std::cout << "\nEmployee Code: " << newEmp.code << " (auto-generated)\n";
    std::cout << "Enter '0' at any prompt to exit\n\n";

    // Get employee details
    newEmp.name = getValidatedInput("Employee Name", 25);
    if (newEmp.name == "0") return;

    newEmp.address = getValidatedInput("Address", 50);
    if (newEmp.address == "0") return;

    std::cout << "Phone Number (or press Enter for none): ";
    std::getline(std::cin, newEmp.phone);
    if (newEmp.phone == "0") return;
    if (newEmp.phone.empty()) newEmp.phone = "-";

    // Date input with validation
    bool validDate = false;
    do {
        std::cout << "\nJoining Date:\n";
        std::cout << "Day (1-31): ";
        std::cin >> newEmp.dd;
        std::cout << "Month (1-12): ";
        std::cin >> newEmp.mm;
        std::cout << "Year (1900-2100): ";
        std::cin >> newEmp.yy;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter numbers only.\n";
            validDate = false;
        } else {
            validDate = isValidDate(newEmp.dd, newEmp.mm, newEmp.yy);
            if (!validDate) {
                std::cout << "Invalid date! Please re-enter.\n";
            }
        }
    } while (!validDate);

    newEmp.designation = getValidatedInput("Designation", 20);
    if (newEmp.designation == "0") return;

    // Grade input
    do {
        std::cout << "Grade (A/B/C/D/E): ";
        std::cin >> newEmp.grade;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        newEmp.grade = toupper(newEmp.grade);
        
        if (newEmp.grade < 'A' || newEmp.grade > 'E') {
            std::cout << "Invalid grade! Please enter A, B, C, D, or E.\n";
        }
    } while (newEmp.grade < 'A' || newEmp.grade > 'E');

    // Grade-specific inputs
    if (newEmp.grade != 'E') {
        do {
            std::cout << "House Allowance (Y/N): ";
            std::cin >> newEmp.house_allowance;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            newEmp.house_allowance = toupper(newEmp.house_allowance);
        } while (newEmp.house_allowance != 'Y' && newEmp.house_allowance != 'N');

        do {
            std::cout << "Travel Allowance (Y/N): ";
            std::cin >> newEmp.travel_allowance;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            newEmp.travel_allowance = toupper(newEmp.travel_allowance);
        } while (newEmp.travel_allowance != 'Y' && newEmp.travel_allowance != 'N');

        do {
            std::cout << "Basic Salary (max 50000): $";
            std::cin >> newEmp.basic_salary;
            if (std::cin.fail() || newEmp.basic_salary < 0 || newEmp.basic_salary > 50000) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid salary! Please enter a value between 0 and 50000.\n";
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        } while (true);
    }

    do {
        std::cout << "Loan Amount (max 50000): $";
        std::cin >> newEmp.loan;
        if (std::cin.fail() || newEmp.loan < 0 || newEmp.loan > 50000) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid loan amount! Please enter a value between 0 and 50000.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    } while (true);

    // Confirm save
    char saveChoice;
    std::cout << "\nDo you want to save this record? (Y/N): ";
    std::cin >> saveChoice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (toupper(saveChoice) == 'Y') {
        allEmployees.push_back(newEmp);
        writeAllRecords(allEmployees);
        std::cout << "\nRecord added successfully!\n";
    } else {
        std::cout << "\nRecord not saved.\n";
    }
    
    pauseScreen();
}

void PayrollSystem::displayEmployee() {
    clearScreen();
    printHeader("DISPLAY EMPLOYEE");
    
    int searchCode;
    std::cout << "\nEnter Employee Code (0 to exit): ";
    std::cin >> searchCode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (searchCode == 0) return;

    std::vector<Employee> allEmployees = readAllRecords();
    bool found = false;
    
    for (const auto& emp : allEmployees) {
        if (emp.code == searchCode) {
            std::cout << "\n";
            emp.display();
            found = true;
            break;
        }
    }

    if (!found) {
        std::cout << "\nEmployee with code " << searchCode << " not found!\n";
    }

    pauseScreen();
}

void PayrollSystem::listEmployees() {
    clearScreen();
    printHeader("LIST OF EMPLOYEES");
    
    std::vector<Employee> allEmployees = readAllRecords();

    if (allEmployees.empty()) {
        std::cout << "\nNo employee records found!\n";
        pauseScreen();
        return;
    }

    std::cout << "\n";
    std::cout << std::left 
              << std::setw(6) << "CODE"
              << std::setw(20) << "NAME"
              << std::setw(12) << "PHONE"
              << std::setw(12) << "DOJ"
              << std::setw(15) << "DESIGNATION"
              << std::setw(6) << "GRADE"
              << std::setw(10) << "SALARY" << std::endl;
    
    printSeparator();
    
    int count = 0;
    for (const auto& emp : allEmployees) {
        emp.displayForList();
        count++;
        
        // Pagination for large lists
        if (count % 20 == 0) {
            std::cout << "\nPress Enter to continue or type 'q' and Enter to quit: ";
            std::string input;
            std::getline(std::cin, input);
            if (!input.empty() && tolower(input[0]) == 'q') break;
            std::cout << "\n";
        }
    }
    
    std::cout << "\nTotal employees: " << allEmployees.size() << std::endl;
    pauseScreen();
}

void PayrollSystem::deleteEmployee() {
    clearScreen();
    printHeader("DELETE EMPLOYEE");
    
    int searchCode;
    std::cout << "\nEnter Employee Code to delete (0 to exit): ";
    std::cin >> searchCode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (searchCode == 0) return;

    std::vector<Employee> allEmployees = readAllRecords();
    auto it = std::find_if(allEmployees.begin(), allEmployees.end(), 
                          [searchCode](const Employee& emp) {
                              return emp.code == searchCode;
                          });

    if (it == allEmployees.end()) {
        std::cout << "\nEmployee with code " << searchCode << " not found!\n";
        pauseScreen();
        return;
    }

    std::cout << "\nEmployee to be deleted:\n";
    it->display();
    
    char choice;
    std::cout << "\nAre you sure you want to delete this record? (Y/N): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (toupper(choice) == 'Y') {
        allEmployees.erase(it);
        writeAllRecords(allEmployees);
        std::cout << "\nRecord deleted successfully!\n";
    } else {
        std::cout << "\nDeletion cancelled.\n";
    }

    pauseScreen();
}

void PayrollSystem::modifyEmployee() {
    clearScreen();
    printHeader("MODIFY EMPLOYEE");
    
    int searchCode;
    std::cout << "\nEnter Employee Code to modify (0 to exit): ";
    std::cin >> searchCode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (searchCode == 0) return;

    std::vector<Employee> allEmployees = readAllRecords();
    auto it = std::find_if(allEmployees.begin(), allEmployees.end(), 
                          [searchCode](const Employee& emp) {
                              return emp.code == searchCode;
                          });

    if (it == allEmployees.end()) {
        std::cout << "\nEmployee with code " << searchCode << " not found!\n";
        pauseScreen();
        return;
    }

    Employee& empToModify = *it;
    std::cout << "\nCurrent employee details:\n";
    empToModify.display();

    char choice;
    std::cout << "\nDo you want to modify this record? (Y/N): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (toupper(choice) != 'Y') {
        return;
    }

    std::cout << "\nEnter new values (press Enter to keep current value):\n\n";

    // Modify fields
    std::string input;
    
    std::cout << "Name [" << empToModify.name << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        empToModify.name = input;
    }

    std::cout << "Address [" << empToModify.address << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        empToModify.address = input;
    }

    std::cout << "Phone [" << empToModify.phone << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) empToModify.phone = input;

    std::cout << "Designation [" << empToModify.designation << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        std::transform(input.begin(), input.end(), input.begin(), ::toupper);
        empToModify.designation = input;
    }

    std::cout << "Grade [" << empToModify.grade << "] (A/B/C/D/E): ";
    std::getline(std::cin, input);
    if (!input.empty() && input.length() == 1) {
        char newGrade = toupper(input[0]);
        if (newGrade >= 'A' && newGrade <= 'E') {
            empToModify.grade = newGrade;
        }
    }

    if (empToModify.grade != 'E') {
        std::cout << "House Allowance [" << empToModify.house_allowance << "] (Y/N): ";
        std::getline(std::cin, input);
        if (!input.empty() && input.length() == 1) {
            char newHouse = toupper(input[0]);
            if (newHouse == 'Y' || newHouse == 'N') {
                empToModify.house_allowance = newHouse;
            }
        }

        std::cout << "Travel Allowance [" << empToModify.travel_allowance << "] (Y/N): ";
        std::getline(std::cin, input);
        if (!input.empty() && input.length() == 1) {
            char newTravel = toupper(input[0]);
            if (newTravel == 'Y' || newTravel == 'N') {
                empToModify.travel_allowance = newTravel;
            }
        }

        std::cout << "Basic Salary [" << empToModify.basic_salary << "]: $";
        std::getline(std::cin, input);
        if (!input.empty()) {
            try {
                float newSalary = std::stof(input);
                if (newSalary >= 0 && newSalary <= 50000) {
                    empToModify.basic_salary = newSalary;
                }
            } catch (const std::exception&) {
                std::cout << "Invalid salary input - keeping current value.\n";
            }
        }
    }

    std::cout << "Loan Amount [" << empToModify.loan << "]: $";
    std::getline(std::cin, input);
    if (!input.empty()) {
        try {
            float newLoan = std::stof(input);
            if (newLoan >= 0 && newLoan <= 50000) {
                empToModify.loan = newLoan;
            }
        } catch (const std::exception&) {
            std::cout << "Invalid loan input - keeping current value.\n";
        }
    }

    // Confirm save
    std::cout << "\nDo you want to save changes? (Y/N): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (toupper(choice) == 'Y') {
        writeAllRecords(allEmployees);
        std::cout << "\nRecord modified successfully!\n";
    } else {
        std::cout << "\nChanges not saved.\n";
    }

    pauseScreen();
}

void PayrollSystem::salarySlip() {
    clearScreen();
    printHeader("SALARY SLIP");
    
    int searchCode;
    std::cout << "\nEnter Employee Code (0 to exit): ";
    std::cin >> searchCode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (searchCode == 0) return;

    std::vector<Employee> allEmployees = readAllRecords();
    auto it = std::find_if(allEmployees.begin(), allEmployees.end(), 
                          [searchCode](const Employee& emp) {
                              return emp.code == searchCode;
                          });

    if (it == allEmployees.end()) {
        std::cout << "\nEmployee with code " << searchCode << " not found!\n";
        pauseScreen();
        return;
    }

    const Employee& emp = *it;
    clearScreen();

    // Get current date
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    
    std::string months[] = {"January", "February", "March", "April", "May", "June",
                           "July", "August", "September", "October", "November", "December"};

    // Print salary slip
    std::cout << std::string(80, '=') << std::endl;
    std::cout << std::setw(45) << "SALARY SLIP" << std::endl;
    std::cout << std::setw(35) << months[timeinfo->tm_mon] << ", " << (timeinfo->tm_year + 1900) << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    std::cout << "\nEmployee Name: " << emp.name;
    std::cout << std::setw(40) << "Date: " << timeinfo->tm_mday << "/" 
              << (timeinfo->tm_mon + 1) << "/" << (timeinfo->tm_year + 1900) << std::endl;
    std::cout << "Designation  : " << emp.designation;
    std::cout << std::setw(40) << "Grade: " << emp.grade << std::endl;

    std::cout << std::string(80, '-') << std::endl;

    double hra = 0.0, ca = 0.0, da = 0.0, pf = 0.0, ld = 0.0, ot = 0.0;
    double basic = emp.basic_salary;

    if (emp.grade == 'E') {
        int days, hours;
        
        do {
            std::cout << "\nDays worked this month (0-31): ";
            std::cin >> days;
            if (std::cin.fail() || days < 0 || days > 31) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input! Please enter 0-31.\n";
            } else {
                break;
            }
        } while (true);

        do {
            std::cout << "Overtime hours (0 or more): ";
            std::cin >> hours;
            if (std::cin.fail() || hours < 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input! Please enter 0 or more.\n";
            } else {
                break;
            }
        } while (true);

        basic = days * 30.0;
        ot = hours * 10.0;
        ld = (15.0 * emp.loan) / 100.0;

    } else {
        if (emp.house_allowance == 'Y') hra = (5.0 * emp.basic_salary) / 100.0;
        if (emp.travel_allowance == 'Y') ca = (2.0 * emp.basic_salary) / 100.0;
        da = (5.0 * emp.basic_salary) / 100.0;
        pf = (2.0 * emp.basic_salary) / 100.0;
        ld = (15.0 * emp.loan) / 100.0;
        basic = emp.basic_salary;
    }

    double allowance = hra + ca + da + ot;
    double deduction = pf + ld;
    double netSalary = (basic + allowance) - deduction;

    // Display salary breakdown
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "\nSALARY BREAKDOWN:\n";
    std::cout << std::string(80, '-') << std::endl;
    
    std::cout << "Basic Salary                    : $" << std::setw(10) << basic << std::endl;
    
    std::cout << "\nALLOWANCES:\n";
    if (emp.grade != 'E') {
        std::cout << "  House Allowance (5%)          : $" << std::setw(10) << hra << std::endl;
        std::cout << "  Travel Allowance (2%)         : $" << std::setw(10) << ca << std::endl;
        std::cout << "  Dearness Allowance (5%)       : $" << std::setw(10) << da << std::endl;
    } else {
        std::cout << "  Overtime                      : $" << std::setw(10) << ot << std::endl;
    }
    std::cout << "  Total Allowances              : $" << std::setw(10) << allowance << std::endl;

    std::cout << "\nDEDUCTIONS:\n";
    if (emp.grade != 'E') {
        std::cout << "  Provident Fund (2%)           : $" << std::setw(10) << pf << std::endl;
    }
    std::cout << "  Loan Deduction (15%)          : $" << std::setw(10) << ld << std::endl;
    std::cout << "  Total Deductions              : $" << std::setw(10) << deduction << std::endl;

    std::cout << std::string(80, '=') << std::endl;
    std::cout << "NET SALARY                      : $" << std::setw(10) << netSalary << std::endl;
    std::cout << std::string(80, '=') << std::endl;

    std::cout << "\n\nCASHIER" << std::setw(65) << "EMPLOYEE" << std::endl;
    
    pauseScreen();
}

int main() {
    std::cout << "Welcome to Payroll Management System\n";
    std::cout << "====================================\n\n";
    
    PayrollSystem payroll;
    payroll.mainMenu();
    
    std::cout << "\nGoodbye!\n";
    return 0;
}