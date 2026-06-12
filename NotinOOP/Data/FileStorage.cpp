#include "FileStorage.h"
#include "../Models/Admin.h"
#include "../Models/Buyer.h"
#include "../Models/BonusDiscount.h"
#include "../Models/BrandDiscount.h"
#include <fstream>
#include <iostream>
#include <sstream>

void FileStorage::saveCatalog(const std::vector<Fragrance>& catalog, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Грешка при отваряне на файла за запис: " << filename << "\n";
        return;
    }

    for (const auto& f : catalog) {
        file << f.getFragranceId() << ","
            << f.getName() << ","
            << f.getBrand() << ","
            << f.getPrice() << ","
            << f.getFragranceFamily() << ","
            << f.getQuantity() << "\n";
    }
    file.close();
}

void FileStorage::loadCatalog(std::vector<Fragrance>& catalog, unsigned int& nextFragId, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    catalog.clear();
    std::string line;
    unsigned int maxId = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string idStr, name, brand, priceStr, family, qtyStr;

        std::getline(ss, idStr, ',');
        std::getline(ss, name, ',');
        std::getline(ss, brand, ',');
        std::getline(ss, priceStr, ',');
        std::getline(ss, family, ',');
        std::getline(ss, qtyStr, ',');

        unsigned int id = std::stoul(idStr);
        double price = std::stod(priceStr);
        unsigned int qty = std::stoul(qtyStr);

        catalog.push_back(Fragrance(id, name, brand, price, family, qty));

        if (id > maxId) maxId = id;
    }

    nextFragId = maxId + 1;
    file.close();
}

void FileStorage::saveUsers(const std::vector<User*>& users, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Грешка при отваряне на файла за запис: " << filename << "\n";
        return;
    }

    for (const User* u : users) {
        if (const Admin* admin = dynamic_cast<const Admin*>(u)) {
            file << "A," << admin->getUserId() << ","
                << admin->getUsername() << ","
                << admin->getPassword() << "\n";
        }
        else if (const Buyer* buyer = dynamic_cast<const Buyer*>(u)) {
            file << "B," << buyer->getUserId() << ","
                << buyer->getUsername() << ","
                << buyer->getPassword() << ","
                << buyer->getBalance() << "\n";
        }
    }
    file.close();
}

void FileStorage::loadUsers(std::vector<User*>& users, unsigned int& nextUserId, const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;

    for (User* u : users) delete u;
    users.clear();

    std::string line;
    unsigned int maxId = 0;

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string typeStr, idStr, username, password, balanceStr;

        std::getline(ss, typeStr, ',');
        std::getline(ss, idStr, ',');
        std::getline(ss, username, ',');
        std::getline(ss, password, ',');

        unsigned int id = std::stoul(idStr);

        if (typeStr == "A") {
            users.push_back(new Admin(id, username, password));
        }
        else if (typeStr == "B") {
            std::getline(ss, balanceStr, ',');
            double balance = std::stod(balanceStr);
            users.push_back(new Buyer(id, username, password, balance));
        }

        if (id > maxId) maxId = id;
    }

    nextUserId = maxId + 1;
    file.close();
}

void FileStorage::saveRelations(const std::vector<User*>& users, const std::string& cartsFile, const std::string& wishFile, const std::string& discFile, const std::string& purchFile) {
    std::ofstream cFile(cartsFile);
    std::ofstream wFile(wishFile);
    std::ofstream dFile(discFile);
    std::ofstream pFile(purchFile);

    for (const User* u : users) {
        if (const Buyer* b = dynamic_cast<const Buyer*>(u)) {
            unsigned int uId = b->getUserId();

            for (const auto& f : b->getCart()) {
                cFile << uId << "," << f.getFragranceId() << "\n";
            }

            for (const auto& f : b->getWishlist()) {
                wFile << uId << "," << f.getFragranceId() << "\n";
            }

            for (const Discount* d : b->getDiscounts()) {
                if (const BrandDiscount* brandD = dynamic_cast<const BrandDiscount*>(d)) {
                    dFile << uId << ",3," << brandD->getDiscountId() << "," << brandD->getDiscountPercent() << "," << brandD->getBrand() << "\n";
                }
                else if (const BonusDiscount* bonusD = dynamic_cast<const BonusDiscount*>(d)) {
                    dFile << uId << ",2," << bonusD->getDiscountId() << "," << bonusD->getDiscountPercent() << "," << bonusD->getAmount() << "\n";
                }
                else {
                    dFile << uId << ",1," << d->getDiscountId() << "," << d->getDiscountPercent() << ",\n";
                }
            }

            for (const Purchase& p : b->getPurchases()) {
                pFile << p.getPurchaseId() << "," << uId << "," << static_cast<int>(p.getState()) << ",";
                for (size_t i = 0; i < p.getFragrances().size(); ++i) {
                    pFile << p.getFragrances()[i].getFragranceId();
                    if (i < p.getFragrances().size() - 1) pFile << ";";
                }
                pFile << "\n";
            }
        }
    }
}

void FileStorage::loadRelations(std::vector<User*>& users, const std::vector<Fragrance>& catalog, unsigned int& nextPurchaseId, const std::string& cartsFile, const std::string& wishFile, const std::string& discFile, const std::string& purchFile) {
    auto findFrag = [&](unsigned int id) -> const Fragrance* {
        for (const auto& f : catalog) {
            if (f.getFragranceId() == id) return &f;
        }
        return nullptr;
        };

    auto findBuyer = [&](unsigned int id) -> Buyer* {
        for (User* u : users) {
            if (u->getUserId() == id) return dynamic_cast<Buyer*>(u);
        }
        return nullptr;
        };

    std::ifstream cFile(cartsFile);
    std::string line;
    while (std::getline(cFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string uIdStr, fIdStr;
        std::getline(ss, uIdStr, ',');
        std::getline(ss, fIdStr, ',');

        Buyer* b = findBuyer(std::stoul(uIdStr));
        const Fragrance* f = findFrag(std::stoul(fIdStr));
        if (b && f) b->addToCart(*f);
    }

    std::ifstream wFile(wishFile);
    while (std::getline(wFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string uIdStr, fIdStr;
        std::getline(ss, uIdStr, ',');
        std::getline(ss, fIdStr, ',');

        Buyer* b = findBuyer(std::stoul(uIdStr));
        const Fragrance* f = findFrag(std::stoul(fIdStr));
        if (b && f) b->addToWishlist(*f);
    }

    std::ifstream dFile(discFile);
    while (std::getline(dFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string uIdStr, typeStr, dIdStr, percentStr, paramStr;
        std::getline(ss, uIdStr, ',');
        std::getline(ss, typeStr, ',');
        std::getline(ss, dIdStr, ',');
        std::getline(ss, percentStr, ',');
        std::getline(ss, paramStr, ',');

        Buyer* b = findBuyer(std::stoul(uIdStr));
        if (!b) continue;

        unsigned int dId = std::stoul(dIdStr);
        double percent = std::stod(percentStr);
        int type = std::stoi(typeStr);

        if (type == 1) b->addDiscount(new Discount(dId, percent));
        else if (type == 2) b->addDiscount(new BonusDiscount(dId, percent, std::stod(paramStr)));
        else if (type == 3) b->addDiscount(new BrandDiscount(dId, percent, paramStr));
    }

    std::ifstream pFile(purchFile);
    unsigned int maxPurchId = 0;
    while (std::getline(pFile, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string pIdStr, uIdStr, stateStr, fragsListStr;
        std::getline(ss, pIdStr, ',');
        std::getline(ss, uIdStr, ',');
        std::getline(ss, stateStr, ',');
        std::getline(ss, fragsListStr, ',');

        Buyer* b = findBuyer(std::stoul(uIdStr));
        if (!b) continue;

        unsigned int pId = std::stoul(pIdStr);
        if (pId > maxPurchId) maxPurchId = pId;

        std::vector<Fragrance> pFrags;
        std::stringstream fragsSS(fragsListStr);
        std::string fIdStr;
        while (std::getline(fragsSS, fIdStr, ';')) {
            if (fIdStr.empty()) continue;
            const Fragrance* f = findFrag(std::stoul(fIdStr));
            if (f) pFrags.push_back(*f);
        }

        Purchase p(pId, b->getUserId(), pFrags);
        p.setState(static_cast<PurchaseState>(std::stoi(stateStr)));

        b->loadPurchase(p);
    }
    nextPurchaseId = maxPurchId + 1;
}