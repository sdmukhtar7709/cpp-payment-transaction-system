# PayCore – Payment Transaction Management System (C++)

PayCore is a console-based C++  project that simulates a basic payment transaction workflow. It demonstrates C++ programming fundamentals, object-oriented design, STL containers, validation, exception handling, persistence, and automated tests.

> **Important:** This is a learning/demo application. It does not connect to banks, card networks, payment processors, or real customer accounts. The payment gateway outcome is simulated. Do not use it to process real payments or store real payment-card information.

## Features

- Create payment transactions with generated transaction IDs
- Simulate payment approval or decline
- Validate amount, currency, description, and customer reference
- Search for transactions by ID
- List all, successful, and failed transactions
- Record refunds for successful transactions
- Display a transaction summary
- Persist transaction records between application runs
- Automated tests for validation, persistence, lookup, and refund rules

## Technology

- C++17
- Standard Template Library (STL)
- Object-Oriented Programming
- File handling and persistence
- CMake
- CTest

## Project structure

```text
PayCore-Cpp-Payment-System/
├── CMakeLists.txt
├── README.md
├── .gitignore
├── include/
│   ├── Storage.h
│   ├── Transaction.h
│   └── TransactionManager.h
├── src/
│   ├── main.cpp
│   ├── Storage.cpp
│   ├── Transaction.cpp
│   └── TransactionManager.cpp
├── tests/
│   └── test_transactions.cpp
└── data/
    └── .gitkeep
```

## Requirements

- A C++17-compatible compiler (GCC 8+, Clang 7+, or a recent Visual Studio)
- CMake 3.16+

Check your tools:

```bash
g++ --version
cmake --version
```

## Build and run

### Linux / macOS / Windows with MinGW or a Unix-like terminal

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

Run the application:

```bash
./build/paycore_app
```

On Windows, the executable may be located at:

```powershell
.\build\Debug\paycore_app.exe
```

or:

```powershell
.\build\paycore_app.exe
```

### Run automated tests

```bash
ctest --test-dir build --output-on-failure
```

You can also run the test executable directly:

```bash
./build/paycore_tests
```

## Menu options

1. Create payment
2. View all transactions
3. Search transaction by ID
4. View successful transactions
5. View failed transactions
6. Refund a transaction
7. View transaction summary
0. Exit

## Data storage

The application creates `data/transactions.db` when the first transaction is saved. It stores transaction metadata so records remain available after restarting the application. The file is intended for local demonstration only.

## Payment simulation

The demo gateway randomly approves approximately 90% of payment attempts. A failed attempt is retained in the transaction history. This is only a simulation to demonstrate control flow and transaction states.

## Concepts demonstrated

| Concept | Where it is used |
|---|---|
| Classes and structures | `Transaction`, `Storage`, `TransactionManager` |
| Encapsulation | Transaction operations are managed by `TransactionManager` |
| STL vectors and algorithms | Transaction collection, searching, filtering |
| Input validation | Payment validation methods |
| Exception handling | Invalid input and storage errors |
| File handling | Loading and saving transaction records |
| Date/time | Transaction timestamps |
| Unit testing | `tests/test_transactions.cpp` |
| Separation of concerns | Header files and implementation files |

## Possible future improvements

- Replace the simulated gateway with a sandbox payment API
- Add a relational database such as PostgreSQL
- Add structured application logging
- Add role-based access control
- Add stronger transaction and audit history
- Add integration tests and CI workflow

## Author

Add your name, GitHub profile, and contact details here before publishing.
