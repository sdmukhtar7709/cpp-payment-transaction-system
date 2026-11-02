#include "Storage.h"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <stdexcept>

Storage::Storage(std::string filePath) : filePath_(std::move(filePath)) {}

std::vector<Transaction> Storage::load() const {
    std::vector<Transaction> transactions;
    std::ifstream input(filePath_);

    // A missing file is normal on the first application run.
    if (!input.is_open()) {
        return transactions;
    }

    Transaction transaction;
    std::string status;

    while (input >> std::quoted(transaction.id)
                 >> transaction.amount
                 >> std::quoted(transaction.currency)
                 >> std::quoted(transaction.description)
                 >> std::quoted(transaction.customerReference)
                 >> std::quoted(transaction.createdAt)
                 >> std::quoted(status)) {
        transaction.status = Transaction::statusFromString(status);
        transactions.push_back(transaction);
    }

    if (!input.eof()) {
        throw std::runtime_error("The transaction data file contains invalid data.");
    }
    return transactions;
}

void Storage::save(const std::vector<Transaction>& transactions) const {
    const std::filesystem::path path(filePath_);
    if (path.has_parent_path()) {
        std::filesystem::create_directories(path.parent_path());
    }

    const std::string temporaryPath = filePath_ + ".tmp";
    {
        std::ofstream output(temporaryPath, std::ios::trunc);
        if (!output.is_open()) {
            throw std::runtime_error("Unable to open transaction data file for writing.");
        }

        output << std::setprecision(2) << std::fixed;
        for (const auto& transaction : transactions) {
            output << std::quoted(transaction.id) << ' '
                   << transaction.amount << ' '
                   << std::quoted(transaction.currency) << ' '
                   << std::quoted(transaction.description) << ' '
                   << std::quoted(transaction.customerReference) << ' '
                   << std::quoted(transaction.createdAt) << ' '
                   << std::quoted(transaction.statusToString()) << '\n';
        }

        if (!output.good()) {
            throw std::runtime_error("Failed while writing transaction data.");
        }
    }

    std::error_code error;
    std::filesystem::rename(temporaryPath, filePath_, error);
    if (error) {
        // Replacement fallback for platforms where rename cannot overwrite.
        std::filesystem::remove(filePath_, error);
        error.clear();
        std::filesystem::rename(temporaryPath, filePath_, error);
    }
    if (error) {
        std::filesystem::remove(temporaryPath);
        throw std::runtime_error("Unable to finalize transaction data file: " + error.message());
    }
}

const std::string& Storage::filePath() const {
    return filePath_;
}
