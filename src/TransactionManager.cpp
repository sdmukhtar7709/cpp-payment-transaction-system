#include "TransactionManager.h"

#include <algorithm>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <stdexcept>

TransactionManager::TransactionManager(const std::string& storagePath)
    : storage_(storagePath),
      transactions_(storage_.load()),
      randomEngine_(std::random_device{}()) {}

void TransactionManager::validatePayment(double amount,
                                         const std::string& currency,
                                         const std::string& description) {
    if (!std::isfinite(amount) || amount <= 0.0) {
        throw std::invalid_argument("Payment amount must be greater than zero.");
    }
    if (amount > 10000000.0) {
        throw std::invalid_argument("Payment amount exceeds the demo limit of 10,000,000.");
    }
    if (currency.size() != 3 ||
        !std::all_of(currency.begin(), currency.end(), [](unsigned char c) {
            return c >= 'A' && c <= 'Z';
        })) {
        throw std::invalid_argument("Currency must be a 3-letter uppercase code, e.g. INR.");
    }
    if (description.empty() || description.size() > 120) {
        throw std::invalid_argument("Description must contain 1 to 120 characters.");
    }
}

std::string TransactionManager::generateTransactionId() {
    const auto now = std::chrono::system_clock::now();
    const auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()).count();

    static unsigned long long sequence = 0;
    std::ostringstream id;
    id << "PAY-" << millis << '-' << ++sequence;
    return id.str();
}

std::string TransactionManager::currentTimestamp() {
    const auto now = std::chrono::system_clock::now();
    const std::time_t time = std::chrono::system_clock::to_time_t(now);
    std::tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &time);
#else
    localtime_r(&time, &localTime);
#endif

    std::ostringstream timestamp;
    timestamp << std::put_time(&localTime, "%Y-%m-%d %H:%M:%S");
    return timestamp.str();
}

bool TransactionManager::simulatePaymentGateway() {
    // Demo only: this does not connect to a bank, card network, or real gateway.
    std::uniform_int_distribution<int> outcome(1, 100);
    return outcome(randomEngine_) <= 90;
}

Transaction TransactionManager::createPayment(double amount,
                                                const std::string& currency,
                                                const std::string& description,
                                                const std::string& customerReference) {
    validatePayment(amount, currency, description);

    if (customerReference.empty() || customerReference.size() > 60) {
        throw std::invalid_argument("Customer reference must contain 1 to 60 characters.");
    }

    Transaction transaction;
    transaction.id = generateTransactionId();
    transaction.amount = std::round(amount * 100.0) / 100.0;
    transaction.currency = currency;
    transaction.description = description;
    transaction.customerReference = customerReference;
    transaction.createdAt = currentTimestamp();
    transaction.status = simulatePaymentGateway()
        ? TransactionStatus::Successful
        : TransactionStatus::Failed;

    transactions_.push_back(transaction);
    persist();
    return transaction;
}

bool TransactionManager::refundPayment(const std::string& transactionId) {
    auto iterator = std::find_if(transactions_.begin(), transactions_.end(),
        [&transactionId](const Transaction& transaction) {
            return transaction.id == transactionId;
        });

    if (iterator == transactions_.end()) {
        return false;
    }
    if (iterator->status != TransactionStatus::Successful) {
        throw std::logic_error("Only successful transactions can be refunded.");
    }

    iterator->status = TransactionStatus::Refunded;
    persist();
    return true;
}

const Transaction* TransactionManager::findById(const std::string& transactionId) const {
    const auto iterator = std::find_if(transactions_.begin(), transactions_.end(),
        [&transactionId](const Transaction& transaction) {
            return transaction.id == transactionId;
        });
    return iterator == transactions_.end() ? nullptr : &(*iterator);
}

std::vector<Transaction> TransactionManager::getAll() const {
    return transactions_;
}

std::vector<Transaction> TransactionManager::getByStatus(TransactionStatus status) const {
    std::vector<Transaction> result;
    std::copy_if(transactions_.begin(), transactions_.end(), std::back_inserter(result),
        [status](const Transaction& transaction) {
            return transaction.status == status;
        });
    return result;
}

TransactionSummary TransactionManager::getSummary() const {
    TransactionSummary summary;
    summary.total = transactions_.size();

    for (const auto& transaction : transactions_) {
        switch (transaction.status) {
            case TransactionStatus::Successful:
                ++summary.successful;
                summary.successfulAmount += transaction.amount;
                break;
            case TransactionStatus::Failed:
                ++summary.failed;
                break;
            case TransactionStatus::Refunded:
                ++summary.refunded;
                break;
            case TransactionStatus::Pending:
                break;
        }
    }
    return summary;
}

void TransactionManager::persist() const {
    storage_.save(transactions_);
}
