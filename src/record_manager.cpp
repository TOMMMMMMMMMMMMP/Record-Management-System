#include "../include/record_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <iomanip>
#include <algorithm>

// ── Helpers ──────────────────────────────────────────────────────────────────

std::vector<Book> loadAllBooks() {
    std::vector<Book> books;
    std::ifstream file(DATA_FILE);
    if (!file.is_open()) return books;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        try {
            std::istringstream ss(line);
            std::string token;
            Book b;
            std::getline(ss, token, '|'); b.id     = std::stoi(token);
            std::getline(ss, token, '|'); b.title  = token;
            std::getline(ss, token, '|'); b.author = token;
            std::getline(ss, token, '|'); b.year   = std::stoi(token);
            books.push_back(b);
        } catch (...) {
            std::cout << "Warning: skipping corrupted line in data file.\n";
        }
    }
    file.close();
    return books;
}

void saveAllBooks(const std::vector<Book>& books) {
    std::ofstream file(DATA_FILE);
    if (!file.is_open()) {
        std::cout << "Error: could not open data file for writing.\n";
        return;
    }
    for (const auto& b : books) {
        file << b.id << "|" << b.title << "|" << b.author << "|" << b.year << "\n";
    }
    file.close();
}

int getNextID(const std::vector<Book>& books) {
    if (books.empty()) return 1;
    int maxID = 0;
    for (const auto& b : books) {
        if (b.id > maxID) maxID = b.id;
    }
    return maxID + 1;
}

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter a number.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
    }
}

int readYear(const std::string& prompt) {
    while (true) {
        int year = readInt(prompt);
        if (year >= 1000 && year <= 2100) return year;
        std::cout << "Invalid year. Please enter a year between 1000 and 2100.\n";
    }
}

std::string readNonEmpty(const std::string& prompt) {
    std::string value;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, value);
        if (!value.empty()) return value;
        std::cout << "This field cannot be empty. Please try again.\n";
    }
}

// ── Functions ─────────────────────────────────────────────────────────────────

void addRecord() {
    std::vector<Book> books = loadAllBooks();
    Book b;
    b.id = getNextID(books);

    std::cout << "\n--- Add Record ---\n";
    std::cout << "Auto ID: " << b.id << "\n";

    b.title  = readNonEmpty("Title  : ");
    b.author = readNonEmpty("Author : ");
    b.year   = readYear("Year   : ");

    books.push_back(b);
    saveAllBooks(books);
    std::cout << "Record added successfully (ID=" << b.id << ").\n";
}

void displayAllRecords() {
    std::vector<Book> books = loadAllBooks();

    if (books.empty()) {
        std::cout << "\nNo records found.\n";
        return;
    }

    std::cout << "\n";
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(30) << "Title"
              << std::setw(25) << "Author"
              << std::setw(6)  << "Year"
              << "\n";
    std::cout << std::string(67, '-') << "\n";

    for (const auto& b : books) {
        std::cout << std::left
                  << std::setw(6)  << b.id
                  << std::setw(30) << b.title
                  << std::setw(25) << b.author
                  << std::setw(6)  << b.year
                  << "\n";
    }
    std::cout << std::string(67, '-') << "\n";
    std::cout << books.size() << " record(s) found.\n";
}

void searchRecordByID() {
    std::vector<Book> books = loadAllBooks();

    if (books.empty()) {
        std::cout << "\nNo records found.\n";
        return;
    }

    int id = readInt("\n--- Search by ID ---\nEnter ID: ");

    for (const auto& b : books) {
        if (b.id == id) {
            std::cout << "\nRecord found:\n";
            std::cout << std::string(40, '-') << "\n";
            std::cout << "ID     : " << b.id     << "\n";
            std::cout << "Title  : " << b.title  << "\n";
            std::cout << "Author : " << b.author << "\n";
            std::cout << "Year   : " << b.year   << "\n";
            std::cout << std::string(40, '-') << "\n";
            return;
        }
    }
    std::cout << "No record found with ID " << id << ".\n";
}

void deleteRecord() {
    std::vector<Book> books = loadAllBooks();

    if (books.empty()) {
        std::cout << "\nNo records found.\n";
        return;
    }

    int id = readInt("\n--- Delete Record ---\nEnter ID to delete: ");

    bool found = false;
    std::vector<Book> updated;

    for (const auto& b : books) {
        if (b.id == id) {
            found = true;
            std::cout << "Deleting: [" << b.id << "] " << b.title << " by " << b.author << "\n";
        } else {
            updated.push_back(b);
        }
    }

    if (!found) {
        std::cout << "No record found with ID " << id << ".\n";
        return;
    }

    char confirm;
    std::cout << "Are you sure? (y/n): ";
    std::cin >> confirm;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (confirm == 'y' || confirm == 'Y') {
        saveAllBooks(updated);
        std::cout << "Record deleted successfully.\n";
    } else {
        std::cout << "Deletion cancelled.\n";
    }
}

void updateRecord() {
    std::vector<Book> books = loadAllBooks();

    if (books.empty()) {
        std::cout << "\nNo records found.\n";
        return;
    }

    int id = readInt("\n--- Update Record ---\nEnter ID to update: ");

    bool found = false;
    for (auto& b : books) {
        if (b.id == id) {
            found = true;

            std::cout << "\nCurrent record:\n";
            std::cout << std::string(40, '-') << "\n";
            std::cout << "ID     : " << b.id     << "\n";
            std::cout << "Title  : " << b.title  << "\n";
            std::cout << "Author : " << b.author << "\n";
            std::cout << "Year   : " << b.year   << "\n";
            std::cout << std::string(40, '-') << "\n";
            std::cout << "(Leave blank to keep current value)\n\n";

            std::string input;

            std::cout << "New title [" << b.title << "]: ";
            std::getline(std::cin, input);
            if (!input.empty()) b.title = input;

            std::cout << "New author [" << b.author << "]: ";
            std::getline(std::cin, input);
            if (!input.empty()) b.author = input;

            std::cout << "New year [" << b.year << "]: ";
            std::getline(std::cin, input);
            if (!input.empty()) {
                try {
                    int newYear = std::stoi(input);
                    if (newYear >= 1000 && newYear <= 2100) {
                        b.year = newYear;
                    } else {
                        std::cout << "Invalid year, keeping current value.\n";
                    }
                } catch (...) {
                    std::cout << "Invalid input, keeping current year.\n";
                }
            }

            std::cout << "\nRecord updated successfully.\n";
            break;
        }
    }

    if (!found) {
        std::cout << "No record found with ID " << id << ".\n";
        return;
    }

    saveAllBooks(books);
}

void sortRecords() {
    std::vector<Book> books = loadAllBooks();

    if (books.empty()) {
        std::cout << "\nNo records found.\n";
        return;
    }

    int choice = readInt("\n--- Sort Records ---\n1. Sort by title\n2. Sort by year\nChoice: ");

    if (choice == 1) {
        std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
            return a.title < b.title;
        });
        std::cout << "\nSorted by title:\n";
    } else if (choice == 2) {
        std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
            return a.year < b.year;
        });
        std::cout << "\nSorted by year:\n";
    } else {
        std::cout << "Invalid choice.\n";
        return;
    }

    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(30) << "Title"
              << std::setw(25) << "Author"
              << std::setw(6)  << "Year"
              << "\n";
    std::cout << std::string(67, '-') << "\n";

    for (const auto& b : books) {
        std::cout << std::left
                  << std::setw(6)  << b.id
                  << std::setw(30) << b.title
                  << std::setw(25) << b.author
                  << std::setw(6)  << b.year
                  << "\n";
    }
    std::cout << std::string(67, '-') << "\n";
}

void exportToCSV() {
    std::vector<Book> books = loadAllBooks();

    if (books.empty()) {
        std::cout << "\nNo records to export.\n";
        return;
    }

    const std::string CSV_FILE = "data/export.csv";
    std::ofstream file(CSV_FILE);

    if (!file.is_open()) {
        std::cout << "Error: could not create export file.\n";
        return;
    }

    file << "ID,Title,Author,Year\n";
    for (const auto& b : books) {
        file << b.id << ","
             << "\"" << b.title  << "\","
             << "\"" << b.author << "\","
             << b.year << "\n";
    }

    file.close();
    std::cout << "\nExported " << books.size() << " record(s) to " << CSV_FILE << "\n";
}
