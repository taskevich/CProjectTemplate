#include <iostream>

#include "Logger.h"

using std::cout;
using std::endl;

int main() {
    auto &logger = Logger::instance();

    logger.info("Hello World!");
    logger.info("Hello World!");

    return 0;
}
