#pragma once
#include <stdexcept>

class ValidationError : public std::runtime_error {
public:
    ValidationError(const std::string& msg) : std::runtime_error(msg) {}
};

class SerializationError : public std::runtime_error {
public:
    SerializationError(const std::string& msg) : std::runtime_error(msg) {}
};