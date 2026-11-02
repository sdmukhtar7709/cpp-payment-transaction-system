#ifndef PAYCORE_TRANSACTION_MANAGER_H
#define PAYCORE_TRANSACTION_MANAGER_H

#include "Storage.h"
#include "Transaction.h"
#include <random>
#include <string>
#include <vector>

struct TransactionSummary {
    std::size_t total{0};
    std::size_t successful{0};
    std::size_t failed{0};
    std::size_t refunded{0};
    double successfulAmount{0.0};
};

class TransactionManager {
public:
    explicit TransactionManager(const std::string& storagePath);

    Transaction createPayment(double amount,
                              const std::string& currency,
                              const std::string& description,
                              const std::string& customerReference);

    bool refundPayment(const std::string& transactionId);
    const Transaction* findById(const std::string& transactionId) const;
    std::vector<Transaction> getAll() const;
    std::vector<Transaction> getByStatus(TransactionStatus status) const;
    TransactionSummary getSummary() const;

private:
    Storage storage_;
    std::vector<Transaction> transactions_;
    std::mt19937 randomEngine_;

    static void validatePayment(double amount,
                                const std::string& currency,
                                const std::string& description);
    static std::string generateTransactionId();
    static std::string currentTimestamp();
    bool simulatePaymentGateway();
    void persist() const;
};

#endif
