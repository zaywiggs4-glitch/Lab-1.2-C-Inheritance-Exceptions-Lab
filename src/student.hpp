#pragma once
#include "person.hpp"
#include "csv.hpp"
#include <vector>

class Student : public Person {
    int gradYear_;
    std::vector<std::string> courses_;

public:
    Student(int id, std::string name, std::string email, int gradYear, std::vector<std::string> courses = {})
        : Person(id, name, email) {
        if (gradYear < 2000) {
            throw ValidationError("grad_year too small");
        }
        gradYear_ = gradYear;
        courses_ = courses;
    }

    std::string role() const override { return "Student"; }

    nlohmann::json to_json() const override {
        nlohmann::json j;
        j["role"] = role();
        j["id"] = id_;
        j["name"] = name_;
        j["email"] = email_;
        j["grad_year"] = gradYear_;
        j["courses"] = courses_;
        return j;
    }

    YAML::Node to_yaml() const override {
        YAML::Node n;
        n["role"] = role();
        n["id"] = id_;
        n["name"] = name_;
        n["email"] = email_;
        n["grad_year"] = gradYear_;
        for (int i = 0; i < (int)courses_.size(); i++) {
            n["courses"].push_back(courses_[i]);
        }
        return n;
    }

    std::string csv_header() const override {
        return "role,id,name,email,grad_year,courses";
    }

    std::string csv_row() const override {
        std::string joined = "";
        for (int i = 0; i < (int)courses_.size(); i++) {
            if (i > 0) joined += ";";
            joined += courses_[i];
        }
        return csv_escape(role()) + "," + std::to_string(id_) + "," + csv_escape(name_) + "," + csv_escape(email_) + "," + std::to_string(gradYear_) + "," + csv_escape(joined);
    }

    static Student from_json(const nlohmann::json& j) {
        if (!j.contains("id") || !j.contains("name") || !j.contains("email") || !j.contains("grad_year")) {
            throw ValidationError("missing required field for Student");
        }
        int id = j.at("id").get<int>();
        std::string name = j.at("name").get<std::string>();
        std::string email = j.at("email").get<std::string>();
        int gradYear = j.at("grad_year").get<int>();
        std::vector<std::string> courses = {};
        if (j.contains("courses")) {
            courses = j.at("courses").get<std::vector<std::string>>();
        }
        return Student(id, name, email, gradYear, courses);
    }
};