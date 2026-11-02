#ifndef PAYCORE_STORAGE_H
#define PAYCORE_STORAGE_H

#include "Transaction.h"
#include <string>
#include <vector>

class Storage {
public:
    explicit Storage(std::string filePath);

    std::vector<Transaction> load() const;
    void save(const std::vector<Transaction>& transactions) const;
    const std::string& filePath() const;

private:
    std::string filePath_;
};

#endif
