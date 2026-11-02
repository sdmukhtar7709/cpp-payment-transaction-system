#include "TransactionManager.h"

#include <iomanip>
#include <iostream>
#include <limits>
#include <string>

namespace {
void printLine() {
    std::cout << "------------------------------------------------------------\n";
}

void printTransaction(const Transaction& transaction) {
    printLine();
    std::cout << "Transaction ID : " << transaction.id << '\n'
              << "Amount         : " << transaction.currency << ' '
              << std::fixed << std::setprecision(2) << transaction.amount << '\n'
              << "Description    : " << transaction.description << '\n'
              << "Customer Ref   : " << transaction.customerReference << '\n'
              << "Created At     : " << transaction.createdAt << '\n'
              << "Status         : " << transaction.statusToString() << '\n';
    printLine();
}

double readAmount() {
    double amount{};
    std::cout << "Enter amount: ";
    if (!(std::cin >> amount)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Please enter a valid numeric amount.");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return amount;
}

std::string readText(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

void showAll(const TransactionManager& manager) {
    const auto transactions = manager.getAll();
    if (transactions.empty()) {
        std::cout << "No transactions found.\n";
        return;
    }
    for (const auto& transaction : transactions) {
        printTransaction(transaction);
    }
}

void showSummary(const TransactionManager& manager) {
    const auto summary = manager.getSummary();
    printLine();
    std::cout << "                 TRANSACTION SUMMARY\n";
    printLine();
    std::cout << "Total transactions       : " << summary.total << '\n'
              << "Successful transactions : " << summary.successful << '\n'
              << "Failed transactions     : " << summary.failed << '\n'
              << "Refunded transactions   : " << summary.refunded << '\n'
              << "Successful amount       : INR " << std::fixed
              << std::setprecision(2) << summary.successfulAmount << '\n';
    printLine();
}

void showMenu() {
    std::cout << "\n========== PAYCORE PAYMENT MANAGEMENT ==========\n"
              << "1. Create payment\n"
              << "2. View all transactions\n"
              << "3. Search transaction by ID\n"
              << "4. View successful transactions\n"
              << "5. View failed transactions\n"
              << "6. Refund a transaction\n"
              << "7. View transaction summary\n"
              << "0. Exit\n"
              << "Choose an option: ";
}
}

int main() {
    try {
        TransactionManager manager("data/transactions.db");
        std::cout << "\nWelcome to PayCore - Payment Transaction Management System\n";

        bool running = true;
        while (running) {
            showMenu();
            int choice{};

            if (!(std::cin >> choice)) {
                if (std::cin.eof()) break;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid menu input. Enter a number from 0 to 7.\n";
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            try {
                switch (choice) {
                    case 1: {
                        const double amount = readAmount();
                        const std::string currency = readText("Currency (e.g. INR): ");
                        const std::string description = readText("Payment description: ");
                        const std::string customer = readText("Customer reference: ");

                        const Transaction transaction =
                            manager.createPayment(amount, currency, description, customer);
                        std::cout << "\nPayment processing completed.\n";
                        printTransaction(transaction);
                        if (transaction.status == TransactionStatus::Successful) {
                            std::cout << "Payment accepted by the simulated gateway.\n";
                        } else {
                            std::cout << "Payment declined by the simulated gateway.\n";
                        }
                        break;
                    }
                    case 2:
                        showAll(manager);
                        break;
                    case 3: {
                        const std::string id = readText("Enter transaction ID: ");
                        const Transaction* transaction = manager.findById(id);
                        if (transaction) printTransaction(*transaction);
                        else std::cout << "Transaction not found.\n";
                        break;
                    }
                    case 4:
                    case 5: {
                        const auto status = choice == 4
                            ? TransactionStatus::Successful
                            : TransactionStatus::Failed;
                        const auto filtered = manager.getByStatus(status);
                        if (filtered.empty()) {
                            std::cout << "No " << (choice == 4 ? "successful" : "failed")
                                      << " transactions found.\n";
                        }
                        for (const auto& transaction : filtered) printTransaction(transaction);
                        break;
                    }
                    case 6: {
                        const std::string id = readText("Enter transaction ID to refund: ");
                        if (manager.refundPayment(id)) {
                            std::cout << "Refund recorded successfully.\n";
                        } else {
                            std::cout << "Transaction ID not found.\n";
                        }
                        break;
                    }
                    case 7:
                        showSummary(manager);
                        break;
                    case 0:
                        running = false;
                        std::cout << "Thank you for using PayCore. Goodbye!\n";
                        break;
                    default:
                        std::cout << "Invalid choice. Select an option from 0 to 7.\n";
                }
            } catch (const std::exception& error) {
                std::cout << "Error: " << error.what() << '\n';
            }
        }
    } catch (const std::exception& error) {
        std::cerr << "Fatal application error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}
