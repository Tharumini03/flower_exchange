# 🌸 Flower Exchange

A C++ simulation of a basic order-matching trading exchange, built as part of the **LSEG C++ Workshop Series** (University of Moratuwa).

The program reads buy/sell orders from `orders.csv`, processes them through a matching engine, and writes execution reports to `execution_rep.csv`.

---

## 📁 Project Structure

```
flower_exchange/
├── main.cpp          # Entry point — reads orders.csv, runs exchange, writes execution_rep.csv
├── Exchange.h        # Order, Report, OrderBook structs + Exchange class declaration
├── Exchange.cpp      # Matching engine — validates, sorts, and matches orders
├── Utils.h           # Utility function declarations
├── Utils.cpp         # CSV parsing, timestamp generation, order validation
├── orders.csv        # Sample input file
├── execution_rep.csv # Generated output file (created when you run the program)
└── .gitignore        # Ignores compiled binary and object files
```

---

## ⚙️ How to Compile

You need a C++ compiler that supports **C++11 or later** (e.g. `g++`).

```bash
g++ -std=c++17 main.cpp Exchange.cpp Utils.cpp -o flower_exchange
```

---

## ▶️ How to Run

Place your `orders.csv` file in the same folder as the compiled binary, then run:

```bash
./flower_exchange
```

The program will read `orders.csv` and produce `execution_rep.csv` in the same directory.

---

## 📥 Input Format — `orders.csv`

The file must have exactly **5 comma-separated columns** with a header row:

```
Cl.Or.ID,Instrument,Side,Quantity,Price
```

| Column     | Type   | Valid Values                                  |
|------------|--------|-----------------------------------------------|
| Cl.Or.ID   | String | Alphanumeric, max 7 characters                |
| Instrument | String | `Rose`, `Lavender`, `Lotus`, `Tulip`, `Orchid`|
| Side       | Int    | `1` = Buy, `2` = Sell                         |
| Quantity   | Int    | Multiple of 10, between 10 and 990            |
| Price      | Double | Greater than 0.0                              |

**Example `orders.csv`:**
```
Cl.Or.ID,Instrument,Side,Quantity,Price
aa13,Rose,2,100,55.00
aa14,Rose,2,100,45.00
aa15,Rose,1,100,35.00
```

---

## 📤 Output Format — `execution_rep.csv`

```
Order ID,Cl.Ord.ID,Instrument,Side,Exec Status,Quantity,Price,Reason,TransactionTime
```

| Column          | Description                                              |
|-----------------|----------------------------------------------------------|
| Order ID        | System-generated ID e.g. `ord1`, `ord2`                  |
| Cl.Ord.ID       | The trader's original client order ID                    |
| Instrument      | Flower type                                              |
| Side            | `1` = Buy, `2` = Sell                                    |
| Exec Status     | `New` / `Reject` / `Fill` / `Pfill`                      |
| Quantity        | Quantity in this execution                               |
| Price           | Execution price (always the passive order's price)       |
| Reason          | Rejection reason — empty if not rejected                 |
| TransactionTime | Timestamp in `YYYYMMDD-HHMMSS.sss` format                |

**Example `execution_rep.csv`:**
```
Order ID,Cl.Ord.ID,Instrument,Side,Exec Status,Quantity,Price,Reason,TransactionTime
ord1,aa13,Rose,2,New,100,55.00,,20260324-104512.123
ord2,aa14,Rose,2,New,100,45.00,,20260324-104512.123
ord3,aa15,Rose,1,New,100,35.00,,20260324-104512.124
```

---

## 🔄 Execution Status Explained

| Status   | Meaning                                                               |
|----------|-----------------------------------------------------------------------|
| `New`    | Order entered the book — no matching order found yet                  |
| `Reject` | Order failed validation and was not accepted                          |
| `Fill`   | Order was **fully** matched and executed                              |
| `Pfill`  | Order was **partially** matched — remaining quantity stays in the book|

---

## 📚 How the Matching Engine Works

The exchange maintains **one order book per instrument** (up to 5 total). Each book has a buy side and a sell side.

**Sorting rules:**
- Buy side — highest price first; ties broken by arrival order (FIFO)
- Sell side — lowest price first; ties broken by arrival order (FIFO)

**A trade executes when:**
- An incoming **buy** order price ≥ the best **sell** price in the book
- An incoming **sell** order price ≤ the best **buy** price in the book

> The execution price is always the **passive order's price** (the order already sitting in the book).

**What happens after matching:**
- If the incoming order is fully matched → `Fill` for both sides
- If the incoming order is partially matched → `Pfill` for the incoming, `Fill` or `Pfill` for the passive
- If no match is found → `New` report, order rests in the book

---

## ✅ Validation Rules

An order is **rejected** if any of the following apply:

- `Cl.Or.ID` is empty
- `Cl.Or.ID` is longer than 7 characters
- `Instrument` is empty or not one of the 5 valid flowers
- `Side` is not `1` or `2`
- `Quantity` is less than 10, 1000 or above, or not a multiple of 10
- `Price` is not greater than 0

---

## 🔧 Code Overview

### `Utils.cpp` / `Utils.h`
Contains three helper utilities used across the project:
- `trim()` — removes leading/trailing whitespace and `\r` from strings
- `splitCSV()` — splits a CSV line by comma into a vector of strings
- `getTimeStamp()` — returns the current local time as `YYYYMMDD-HHMMSS.sss`
- `validateOrder()` — checks all validation rules, returns `false` + reason if invalid

### `Exchange.h`
Defines three data structures:
- `Order` — holds all fields for one incoming order including a `seq` (sequence) counter for FIFO priority
- `Report` — holds all fields for one execution report written to the output CSV
- `OrderBook` — holds the buy and sell lists for one instrument

### `Exchange.cpp`
The core matching logic:
- `process()` — assigns an order ID and sequence number, validates, then calls `matchBuy()` or `matchSell()`
- `matchBuy()` — tries to match an incoming buy against the sorted sell book
- `matchSell()` — tries to match an incoming sell against the sorted buy book
- `sortBooks()` — re-sorts both sides of a book after any change
- `addToBook()` — adds an unmatched (or partially matched) order to the book

### `main.cpp`
- Opens `orders.csv` and reads it line by line
- Skips the header row and empty lines
- Parses each row into an `Order` struct and passes it to `Exchange::process()`
- Collects all returned `Report` objects and writes them to `execution_rep.csv`

---

## 🏫 Project Info

- **Workshop:** LSEG C++ Workshop Series for UoM
- **Language:** C++17
- **Completion Date:** 27 March 2026
- **Demo Date:** 8 April 2026 — LSEG Malabe
