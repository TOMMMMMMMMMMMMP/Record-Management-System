#include <iostream>
#include <limits>
#include "include/record_manager.h"

void showMenu() {
    std::cout << "\n===== Record Management System =====\n";
    std::cout << "1. Add record\n";
    std::cout << "2. Display all records\n";
    std::cout << "3. Search by ID\n";
    std::cout << "4. Delete record\n";
    std::cout << "5. Update record\n";
    std::cout << "6. Sort records\n";
    std::cout << "7. Export to CSV\n";
    std::cout << "0. Quit\n";
    std::cout << "====================================\n";
    std::cout << "Choice: ";
}

int main() {
    int choice = -1;

    do {
        showMenu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: addRecord(); break;
            case 2: displayAllRecords(); break;
            case 3: searchRecordByID(); break;
            case 4: deleteRecord(); break;
            case 5: updateRecord(); break;
            case 6: sortRecords(); break;
            case 7: exportToCSV(); break;
            case 0: std::cout << "Goodbye!\n"; break;
            default: std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}
