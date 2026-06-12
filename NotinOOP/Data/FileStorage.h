#pragma once
#include <vector>
#include <string>
#include "../Models/Fragrance.h"
#include "../Models/User.h"
#include "../Models/Purchase.h"

class FileStorage {
public:
    static void saveCatalog(const std::vector<Fragrance>& catalog, const std::string& filename);
    static void loadCatalog(std::vector<Fragrance>& catalog, unsigned int& nextFragId, const std::string& filename);

    static void saveUsers(const std::vector<User*>& users, const std::string& filename);
    static void loadUsers(std::vector<User*>& users, unsigned int& nextUserId, const std::string& filename);

    static void saveRelations(const std::vector<User*>& users,
        const std::string& cartsFile,
        const std::string& wishFile,
        const std::string& discFile,
        const std::string& purchFile);

    static void loadRelations(std::vector<User*>& users,
        const std::vector<Fragrance>& catalog,
        unsigned int& nextPurchaseId,
        const std::string& cartsFile,
        const std::string& wishFile,
        const std::string& discFile,
        const std::string& purchFile);
};