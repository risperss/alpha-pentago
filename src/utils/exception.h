#pragma once

#include <stdexcept>
#include <iostream>

namespace pen {

    class Exception : public std::runtime_error {
    public:
        explicit Exception(const std::string& what) : std::runtime_error(what) {
            std::cout << "Exception: " << what << std::endl;
        }
    };
}
