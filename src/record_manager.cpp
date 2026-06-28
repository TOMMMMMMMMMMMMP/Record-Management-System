#include "../include/record_manager.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <limits>
#include <iomanip>

// ── Helpers ──────────────────────────────────────────────────────────────────

std::vector<Book> loadAllBooks() {
    std::vector<Book> books;
    std::ifstream file(DATA_FILE);
    if (!file.is_open()) return books;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream ss(line);
        std::string token;
        Book b;

        std::getline(ss, token, '|'); b.id     = std::stoi(token);
        std::getline(ss, token, '|'); b.title  = token;
        std::getline(ss, token, '|'); b.author = token;
        std::getline(ss, token, '|'); b.year   = std::stoi(token);

        books.push_back(b);
    }
    file.close();
    return books;
}

void saveAllBooks(const std::vector<Book>& books) {
    std::ofstream file(DATA_FILE);
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

// ── Functions ─────────────────────────────────────────────────────────────────

void addRecord() {
    std::vector<Book> books = loadAllBooks();

    Book b;
    b.id = getNextID(books);

    std::cout << "\n--- Add Record ---\n";
    std::cout << "Auto ID: " << b.id << "\n";

    std::cout << "Title  : ";
    std::getline(std::cin, b.title);

    std::cout << "Author : ";
    std::getline(std::cin, b.author);

    std::cout << "Year   : ";
    std::cin >> b.year;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
    std::cout << "[searchRecordByID] Not implemented yet." << std::endl;
}

void deleteRecord() {
    std::cout << "[deleteRecord] Not implemented yet." << std::endl;
}

void updateRecord() {
    std::cout << "[updateRecord] Not implemented yet." << std::endl;
}

void sortRecords() {
    std::cout << "[sortRecords] Not implemented yet." << std::endl;
}

void exportToCSV() {
    std::cout << "[exportToCSV] Not implemented yet." << std::endl;
}
