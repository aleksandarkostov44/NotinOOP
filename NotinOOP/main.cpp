#include <iostream>
#include "Core/NotinOOP.h"

int main() {
    system("chcp 65001 > nul");

    NotinOOP* app = NotinOOP::getInstance();
    app->run();

    NotinOOP::releaseInstance();

    return 0;
}