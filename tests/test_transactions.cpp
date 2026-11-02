#include "Transaction.h"
#include "TransactionManager.h"

#include <cassert>
#include <cstdio>
#include <iostream>
#include <stdexcept>

int main() {
    const std::string testFile = "test_transactions.db";
    std::remove(testFile.c_str());

    // Status conversion tests.
    Transaction transaction;
    transaction.status = TransactionStatus::Successful;
    assert(transaction.statusToString() == "SUCCESSFUL");
    assert(Transaction::statusFromString("FAILED") == TransactionStatus::Failed);

    // Manager starts with an empty test data file.
    TransactionManager manager(testFile);
    assert(manager.getAll().empty());

    // Invalid amounts must be rejected.
    bool rejected = false;
    try {
        manager.createPayment(-10.0, "INR", "Invalid test", "CUSTOMER-1");
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    assert(rejected);

    // A valid payment is created and assigned an ID.
    const Transaction created =
        manager.createPayment(250.50, "INR", "Test purchase", "CUSTOMER-1");
    assert(!created.id.empty());
    assert(created.amount == 250.50);
    assert(manager.findById(created.id) != nullptr);

    // Transaction is persisted and can be loaded by a new manager.
    TransactionManager reloaded(testFile);
    assert(reloaded.getAll().size() == 1);
    assert(reloaded.findById(created.id) != nullptr);

    // Refund only applies to successful payments.
    if (created.status == TransactionStatus::Successful) {
        assert(reloaded.refundPayment(created.id));
        assert(reloaded.findById(created.id)->status == TransactionStatus::Refunded);
    } else {
        bool refundRejected = false;
        try {
            reloaded.refundPayment(created.id);
        } catch (const std::logic_error&) {
            refundRejected = true;
        }
        assert(refundRejected);
    }

    std::remove(testFile.c_str());
    std::cout << "All PayCore tests passed.\n";
    return 0;
}
