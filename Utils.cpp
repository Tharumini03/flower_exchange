#include "Utils.h"
#include <sstream>
#include <iomanip>
#include <chrono>
#include <ctime>

// Trim spaces
std::string trim(std::string s) {
    while (!s.empty() && (s[0] == ' ' || s[0] == '\t')) s.erase(0, 1);
    while (!s.empty() && (s.back() == ' ' || s.back() == '\t' || s.back() == '\r' || s.back() == '\n')) s.pop_back();
    return s;
}

// Split CSV line
std::vector<std::string> splitCSV(std::string line) {
    std::vector<std::string> parts;
    std::string item;
    std::stringstream ss(line);

    while (getline(ss, item, ',')) {
        parts.push_back(trim(item));
    }
    return parts;
}

// Timestamp function
std::string getTimeStamp() {
    using namespace std::chrono;

    auto now = system_clock::now();
    auto tt = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y%m%d-%H%M%S", &tm);

    std::stringstream out;
    out << buffer << "." << std::setw(3) << std::setfill('0') << ms.count();
    return out.str();
}

// Validate one order
bool validateOrder(std::string clientId, std::string ins, int side, double price, int qty, std::string &reason) {
    if (clientId.empty()) {
        reason = "Missing Client Order ID";
        return false;
    }

    if (clientId.size() > 7) {
        reason = "Client Order ID too long";
        return false;
    }

    if (ins.empty()) {
        reason = "Invalid instrument";
        return false;
    }

    if (!(side == 1 || side == 2)) {
        reason = "Invalid side";
        return false;
    }

    if (qty < 10 || qty > 1000 || qty % 10 != 0) {
        reason = "Invalid size";
        return false;
    }

    if (price <= 0) {
        reason = "Invalid price";
        return false;
    }

    reason = "";
    return true;
}
