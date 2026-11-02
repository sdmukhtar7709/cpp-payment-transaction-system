#include "Transaction.h"
#include <stdexcept>

std::string Transaction::statusToString() const {
    switch (status) {
        case TransactionStatus::Pending: return "PENDING";
        case TransactionStatus::Successful: return "SUCCESSFUL";
        case TransactionStatus::Failed: return "FAILED";
        case TransactionStatus::Refunded: return "REFUNDED";
    }
    return "UNKNOWN";
}

TransactionStatus Transaction::statusFromString(const std::string& value) {
    if (value == "PENDING") return TransactionStatus::Pending;
    if (value == "SUCCESSFUL") return TransactionStatus::Successful;
    if (value == "FAILED") return TransactionStatus::Failed;
    if (value == "REFUNDED") return TransactionStatus::Refunded;
    throw std::invalid_argument("Unknown transaction status: " + value);
}
