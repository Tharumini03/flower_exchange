#pragma once
#include <string>
#include <vector>

// Remove spaces from start and end
std::string trim(std::string s);

// Split one CSV line by comma
std::vector<std::string> splitCSV(std::string line);

// Get time like: YYYYMMDD-HHMMSS.sss
std::string getTimeStamp();

// Validate order fields, return false + reason if invalid
bool validateOrder(std::string clientId, std::string ins, int side, double price, int qty, std::string &reason);