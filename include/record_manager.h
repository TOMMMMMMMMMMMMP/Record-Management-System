#ifndef RECORD_MANAGER_H
#define RECORD_MANAGER_H

#include "Book.h"
#include <vector>
#include <string>

const std::string DATA_FILE = "data/books.txt";

void addRecord();
void displayAllRecords();
void searchRecordByID();
void deleteRecord();
void updateRecord();
void sortRecords();
void exportToCSV();

#endif
