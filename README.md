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

### Running Example Input Files

Several example input files are provided in the `example_inputs/` folder. To run any of them, copy it to `orders.csv` first:

```bash
cp example_inputs/orders_example1.csv orders.csv && ./flower_exchange
```

Available examples:

| File | Description |
|------|-------------|
| `orders_example1.csv` | Example 1 |
| `orders_example2.csv` | Example 2 |
| `orders_example3.csv` | Example 3 |
| `orders_example4.csv` | Example 4 |
| `orders_example5.csv` | Example 5 |
| `orders_example6.csv` | Example 6 |
| `orders_example7.csv` | Example 7 |

Replace `orders_example1.csv` with whichever file you want to test.

---
