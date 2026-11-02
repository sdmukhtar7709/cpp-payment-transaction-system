#ifndef PAYCORE_TRANSACTION_H
#define PAYCORE_TRANSACTION_H

#include <string>

enum class TransactionStatus {
    Pending,
    Successful,
    Failed,
    Refunded
};

struct Transaction {
    std::string id;
    double amount{0.0};
    std::string currency{"INR"};
    std::string description;
    std::string customerReference;
    std::string createdAt;
    TransactionStatus status{TransactionStatus::Pending};

    std::string statusToString() const;
    static TransactionStatus statusFromString(const std::string& value);
};

#endif
