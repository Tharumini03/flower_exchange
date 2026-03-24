#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

#include "Utils.h"
#include "Exchange.h"

using namespace std;

// Convert status number to text
string statusToText(int status) {
    if (status == 0) return "New";
    if (status == 1) return "Reject";
    if (status == 2) return "Fill";
    if (status == 3) return "Pfill";
    return "Unknown";
}

// Write all execution reports to csv
void writeReports(string filename, vector<Report> reports) {
    ofstream out(filename);

    out << "Order ID,Cl.Ord.ID,Instrument,Side,Exec Status,Quantity,Price,Reason,TransactionTime\n";
    out << fixed << setprecision(2);

    for (Report r : reports) {
        out << r.orderId << ","
            << r.clientId << ","
            << r.instrument << ","
            << r.side << ","
            << statusToText(r.status) << ","
            << r.qty << ","
            << r.price << ","
            << r.reason << ","
            << r.time << "\n";
    }
}

int main() {
    ifstream file("orders.csv");
    if (!file) {
        cout << "orders.csv not found!\n";
        return 0;
    }

    Exchange ex;
    vector<Report> allReports;

    string line;
    bool firstLine = true;

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // Skip header
        if (firstLine) {
            firstLine = false;
            if (line.find("Cl.Or.ID") != string::npos) continue;
        }

        vector<string> parts = splitCSV(line);

        Order o;
        o.clientId = "";
        o.instrument = "";
        o.side = 0;
        o.qty = 0;
        o.price = 0.0;
        o.seq = 0;
        o.orderId = "";

        // Wrong number of columns
        if (parts.size() != 5) {
            if (parts.size() > 0) o.clientId = parts[0];
            if (parts.size() > 1) o.instrument = parts[1];

            vector<Report> reps = ex.process(o);
            allReports.insert(allReports.end(), reps.begin(), reps.end());
            continue;
        }

        o.clientId = parts[0];
        o.instrument = parts[1];

        try {
            o.side = stoi(parts[2]);
            o.qty = stoi(parts[3]);
            o.price = stod(parts[4]);
            
        } catch (...) {
            // leave defaults if parsing fails
        }

        vector<Report> reps = ex.process(o);
        allReports.insert(allReports.end(), reps.begin(), reps.end());
    }

    writeReports("execution_rep.csv", allReports);

    cout << "Done! Created execution_rep.csv\n";
    return 0;
}