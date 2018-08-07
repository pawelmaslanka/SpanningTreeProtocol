#pragma once

// C Standard Library
#include <cstdint>

// C++ Standard Library
#include <stdexcept>
#include <string>

namespace SpanningTree {
    /**
     * @brief 
     * 
     */
class SmException : std::runtime_error {
  public:
    /**
     * @brief Construct a new SMException object
     * 
     * @param msg Message to put in exception
     * @param func Name of function that throws this exception
     * @param line Number of line where the exception has been thrown
     */
    SmException(std::string&& msg, std::string&& func, const int line);
};

inline SmException::SmException(std::string&& msg, std::string&& func, const int line)
    : std::runtime_error(func + "():" + std::to_string(line) + " >> " + msg) {
    // Nothing more to do
}

} // namespace SpanningTree
