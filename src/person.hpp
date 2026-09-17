#pragma once
#include "serializable.hpp"
#include "exceptions.hpp"
#include <string>

class Person : public Serializable {
protected:
    int id_;
    std::string name_;
    std::string email_;

    void validateEmail(const std::string& e) {
        int atPos = -1;
        int dotPos = -1;
        for (int i = 0; i < (int)e.size(); i++) {
            if (e[i] == '@') atPos = i;
            if (e[i] == '.') dotPos = i;
        }
        if (atPos <= 0 || dotPos <= atPos + 1 || dotPos >= (int)e.size() - 1) {
            throw ValidationError("Invalid email: " + e);
        }
    }

public:
    Person(int id, std::string name, std::string email) {
        if (id <= 0) {
            throw ValidationError("id must be positive");
        }
        if (name.empty()) {
            throw ValidationError("name is required");
        }
        validateEmail(email);
        id_ = id;
        name_ = name;
        email_ = email;
    }

    virtual ~Person() {}
    virtual std::string role() const = 0;

    int id() const { return id_; }
    std::string name() const { return name_; }
    std::string email() const { return email_; }
};