#include "NotinOOP.h"
#include <iostream>
#include "../Data/FileStorage.h"

NotinOOP* NotinOOP::instance = nullptr;

NotinOOP* NotinOOP::getInstance() {
    if (instance == nullptr) {
        instance = new NotinOOP();
    }
    return instance;
}

void NotinOOP::releaseInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

NotinOOP::NotinOOP() {
    currentUser = nullptr;
    nextUserId = 1;
    nextFragranceId = 1;
    nextPurchaseId = 1;

    FileStorage::loadCatalog(catalog, nextFragranceId, "catalog.txt");
    FileStorage::loadUsers(users, nextUserId, "users.txt");

    if (users.empty()) {
        users.push_back(new Admin(nextUserId++, "admin", "admin123"));
    }

        FileStorage::loadRelations(users, catalog, nextPurchaseId, "carts.txt", "wishlists.txt", "discounts.txt", "purchases.txt");
}

NotinOOP::~NotinOOP() {
    for (User* u : users) {
        delete u;
    }
    users.clear();
}

void NotinOOP::addStrike(unsigned int uId) {
    bool found = false;
    for (auto& strike : strikeList) {
        if (strike.userId == uId) {
            strike.deletedReviewsCount++;
            found = true;

            if (strike.deletedReviewsCount >= 7) {
                std::string targetUsername = "";
                for (User* u : users) {
                    if (u->getUserId() == uId) {
                        targetUsername = u->getUsername();
                        break;
                    }
                }

                if (!targetUsername.empty()) {
                    for (auto it = users.begin(); it != users.end(); ++it) {
                        if ((*it)->getUserId() == uId) {
                            delete* it;
                            users.erase(it);
                            std::cout << "СИСТЕМНО СЪОБЩЕНИЕ: Потребител '" << targetUsername << "' беше автоматично блокиран заради 7 премахнати ревюта!\n";
                            break;
                        }
                    }
                }
            }
            break;
        }
    }

    if (!found) {
        strikeList.push_back({ uId, 1 });
    }
}

void NotinOOP::processGuestCommand(const std::string& command) {
    if (command == "register") {
        std::string username, password;
        std::cin >> username >> password;

        bool exists = false;
        for (User* u : users) {
            if (u->getUsername() == username) {
                exists = true;
                break;
            }
        }

        if (exists) {
            std::cout << "Грешка: Потребителското име вече е заето!\n";
        }
        else {
            users.push_back(new Buyer(nextUserId++, username, password));
            std::cout << "Успешна регистрация!\n";
        }
    }
    else if (command == "login") {
        std::string username, password;
        std::cin >> username >> password;

        for (User* u : users) {
            if (u->getUsername() == username && u->checkPassword(password)) {
                currentUser = u;
                std::cout << "Успешно влизане в системата като " << username << "!\n";
                return;
            }
        }
        std::cout << "Грешка: Грешно име или парола.\n";
    }
    else if (command == "help") {
        std::cout << "Налични команди за Гост:\n";
        std::cout << " 1. register <username> <password>\n";
        std::cout << " 2. login <username> <password>\n";
        std::cout << " 3. end\n";
    }
    else {
        std::cout << "Непозната команда или нямате права за нея. Напишете 'help' за списък.\n";
    }
}

void NotinOOP::processBuyerCommand(Buyer* buyer, const std::string& command) {
    if (command == "logout") {
        currentUser = nullptr;
        std::cout << "Успешно излязохте от профила си.\n";
    }
    else if (command == "help") {
        buyer->help();
    }
    else if (command == "add-to-balance") {
        double amount;
        std::cin >> amount;
        buyer->addToBalance(amount);
        std::cout << "Успешно добавени " << amount << " лв. Текущ баланс: " << buyer->getBalance() << " лв.\n";
    }
    else if (command == "view-cart") {
        buyer->viewCart();
    }
    else if (command == "checkout") {
        buyer->checkout(nextPurchaseId);
    }
    else if (command == "view-purchases") {
        buyer->viewPurchases();
    }
    else if (command == "view-bought") {
        buyer->viewBought();
    }
    else if (command == "view-wishlist") {
        buyer->viewWishlist();
    }
    else if (command == "add-to-wishlist") {
        std::string fName;
        std::cin >> fName;
        bool found = false;
        for (const auto& f : catalog) {
            if (f.getName() == fName) {
                buyer->addToWishlist(f);
                std::cout << fName << " беше добавен в любими!\n";
                found = true;
                break;
            }
        }
        if (!found) std::cout << "Парфюм с такова име не беше намерен в каталога.\n";
    }
    else if (command == "remove-from-wishlist") {
        std::string fName;
        std::cin >> fName;
        if (buyer->removeFromWishlist(fName)) {
            std::cout << fName << " беше премахнат от любими.\n";
        }
        else {
            std::cout << fName << " не е във вашия списък с любими.\n";
        }
    }
    else if (command == "add-to-cart") {
        std::string fName;
        std::cin >> fName;
        bool found = false;
        for (const auto& f : catalog) {
            if (f.getName() == fName) {
                buyer->addToCart(f);
                std::cout << fName << " беше добавен в количката!\n";
                found = true;
                break;
            }
        }
        if (!found) std::cout << "Парфюм с такова име не беше намерен в каталога.\n";
    }
    else if (command == "remove-from-cart") {
        std::string fName;
        std::cin >> fName;
        if (buyer->removeFromCart(fName)) {
            std::cout << fName << " беше премахнат от количката.\n";
        }
        else {
            std::cout << fName << " не е във вашата количка.\n";
        }
    }
    else if (command == "recommend") {
        buyer->recommend(catalog);
    }
    else if (command == "cancel") {
        unsigned int pId;
        std::cin >> pId;
        if (buyer->cancelPurchase(pId)) {
            std::cout << "Поръчка #" << pId << " беше успешно отказана.\n";
        }
        else {
            std::cout << "Грешка: Поръчката не съществува или вече е обработена.\n";
        }
    }
    else if (command == "make-review") {
        std::string fName;
        unsigned short rating;
        std::string comment;
        std::cin >> fName >> rating;

        std::getline(std::cin >> std::ws, comment);

        bool found = false;
        for (auto& f : catalog) {
            if (f.getName() == fName) {
                static unsigned int globalReviewId = 1;
                buyer->makeReview(f, globalReviewId++, rating, comment);
                found = true;
                break;
            }
        }
        if (!found) std::cout << "Парфюм с такова име не беше намерен.\n";
    }
    else {
        std::cout << "Невалидна команда за Купувач. Напишете 'help' за списък с командите.\n";
    }
}

void NotinOOP::processAdminCommand(Admin* admin, const std::string& command) {
    if (command == "logout") {
        currentUser = nullptr;
        std::cout << "Успешно излязохте от профила си.\n";
    }
    else if (command == "help") {
        admin->help();
    }
    else if (command == "create-fragrance") {
        std::string name, brand, family;
        double price;
        unsigned int qty;
        std::cin >> name >> brand >> price >> family >> qty;

        admin->createFragrance(catalog, nextFragranceId++, name, brand, price, family, qty);
    }
    else if (command == "remove-review") {
        unsigned int fId, rId;
        std::cin >> fId >> rId;

        unsigned int authorId = admin->removeReview(catalog, fId, rId);
        if (authorId != 0) {
            addStrike(authorId);
        }
        else {
            std::cout << "Грешка: Ревюто не беше намерено.\n";
        }
    }
    else if (command == "block-user") {
        std::string targetUsername;
        std::cin >> targetUsername;
        if (!admin->blockUser(users, targetUsername)) {
            std::cout << "Потребителят не беше намерен или не може да бъде блокиран.\n";
        }
    }
    else if (command == "add-quantity") {
        std::string fName;
        unsigned int qty;
        std::cin >> fName >> qty;
        if (!admin->addQuantity(catalog, fName, qty)) {
            std::cout << "Грешка: Парфюм с такова име не беше намерен.\n";
        }
    }
    else if (command == "deliver") {
        unsigned int pId;
        std::cin >> pId;
        bool delivered = false;

        for (User* u : users) {
            if (Buyer* b = dynamic_cast<Buyer*>(u)) {
                if (b->deliverPurchase(pId)) {
                    delivered = true;
                    break;
                }
            }
        }

        if (delivered) {
            std::cout << "Поръчка #" << pId << " е маркирана като ДОСТАВЕНА.\n";
        }
        else {
            std::cout << "Грешка: Поръчка #" << pId << " не беше намерена или вече е обработена.\n";
        }
    }
    else if (command == "grant-discount") {
        std::string targetUsername;
        int type;
        double percent;
        std::cin >> targetUsername >> type >> percent;

        Buyer* targetBuyer = nullptr;
        for (User* u : users) {
            if (u->getUsername() == targetUsername) {
                targetBuyer = dynamic_cast<Buyer*>(u);
                break;
            }
        }

        if (!targetBuyer) {
            std::cout << "Грешка: Не е намерен купувач с потребителско име '" << targetUsername << "'.\n";
        }
        else {
            static unsigned int nextDiscountId = 1;
            if (type == 1) {
                targetBuyer->addDiscount(new Discount(nextDiscountId++, percent));
                std::cout << "Успешно раздаден базов ваучер (" << percent << "%) на " << targetUsername << "\n";
            }
            else if (type == 2) {
                double amount;
                std::cin >> amount;
                targetBuyer->addDiscount(new BonusDiscount(nextDiscountId++, percent, amount));
                std::cout << "Успешно раздаден Бонус ваучер (" << percent << "% + " << amount << " лв.) на " << targetUsername << "\n";
            }
            else if (type == 3) {
                std::string brand;
                std::cin >> brand;
                targetBuyer->addDiscount(new BrandDiscount(nextDiscountId++, percent, brand));
                std::cout << "Успешно раздаден Бранд ваучер (" << percent << "% за марка " << brand << ") на " << targetUsername << "\n";
            }
            else {
                std::cout << "Грешка: Невалиден тип ваучер (използвайте 1, 2 или 3).\n";
            }
        }
    }
    else {
        std::cout << "Невалидна команда за Администратор. Напишете 'help' за списък с командите.\n";
    }
}

void NotinOOP::run() {
    std::cout << "Добре дошли в NotinOOP! Напишете 'help' за команди или 'end' за изход.\n";

    std::string command;
    while (true) {
        std::cout << "\n> ";
        std::cin >> command;

        if (command == "end") {
            std::cout << "Запазване на всички данни и релации...\n";
            FileStorage::saveCatalog(catalog, "catalog.txt");
            FileStorage::saveUsers(users, "users.txt");

            FileStorage::saveRelations(users, "carts.txt", "wishlists.txt", "discounts.txt", "purchases.txt");

            std::cout << "Системата се затвори успешно.\n";
            break;
        }

        if (currentUser == nullptr) {
            processGuestCommand(command);
        }
        else {
            if (Buyer* b = dynamic_cast<Buyer*>(currentUser)) {
                processBuyerCommand(b, command);
            }
            else if (Admin* a = dynamic_cast<Admin*>(currentUser)) {
                processAdminCommand(a, command);
            }
        }
    }
}