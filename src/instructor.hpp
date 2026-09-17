#pragma once
#include "person.hpp"
#include "csv.hpp"
#include <vector>

class Instructor : public Person {
    std::string office_;
    std::vector<std::string> teaches_;

public:
    Instructor(int id, std::string name, std::string email, std::string office, std::vector<std::string> teaches = {})
        : Person(id, name, email) {
        if (office.empty()) {
            throw ValidationError("office is required");
        }
        office_ = office;
        teaches_ = teaches;
    }

    std::string role() const override { return "Instructor"; }

    nlohmann::json to_json() const override {
        nlohmann::json j;
        j["role"] = role();
        j["id"] = id_;
        j["name"] = name_;
        j["email"] = email_;
        j["office"] = office_;
        j["teaches"] = teaches_;
        return j;
    }

    YAML::Node to_yaml() const override {
        YAML::Node n;
        n["role"] = role();
        n["id"] = id_;
        n["name"] = name_;
        n["email"] = email_;
        n["office"] = office_;
        for (int i = 0; i < (int)teaches_.size(); i++) {
            n["teaches"].push_back(teaches_[i]);
        }
        return n;
    }

    std::string csv_header() const override {
        return "role,id,name,email,office,teaches";
    }

    std::string csv_row() const override {
        std::string joined = "";
        for (int i = 0; i < (int)teaches_.size(); i++) {
            if (i > 0) joined += ";";
            joined += teaches_[i];
        }
        return csv_escape(role()) + "," + std::to_string(id_) + "," + csv_escape(name_) + "," + csv_escape(email_) + "," + csv_escape(office_) + "," + csv_escape(joined);
    }

    static Instructor from_json(const nlohmann::json& j) {
        if (!j.contains("id") || !j.contains("name") || !j.contains("email") || !j.contains("office")) {
            throw ValidationError("missing required field for Instructor");
        }
        int id = j.at("id").get<int>();
        std::string name = j.at("name").get<std::string>();
        std::string email = j.at("email").get<std::string>();
        std::string office = j.at("office").get<std::string>();
        std::vector<std::string> teaches = {};
        if (j.contains("teaches")) {
            teaches = j.at("teaches").get<std::vector<std::string>>();
        }
        return Instructor(id, name, email, office, teaches);
    }
};