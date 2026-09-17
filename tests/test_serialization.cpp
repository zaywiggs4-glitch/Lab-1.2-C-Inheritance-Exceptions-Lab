#include <catch2/catch_test_macros.hpp>
#include "student.hpp"
#include "instructor.hpp"
#include "csv_writer.hpp"

TEST_CASE("Student JSON serialization") {
    Student s(10, "Alice", "alice@uni.edu", 2027, {"CS101", "HIST110"});
    auto j = s.to_json();
    CHECK(j.at("role") == "Student");
    CHECK(j.at("name") == "Alice");
    CHECK(j.at("grad_year") == 2027);
}

TEST_CASE("Instructor YAML serialization") {
    Instructor i(11, "Bob", "bob@uni.edu", "C-210", {"CS101"});
    auto y = i.to_yaml();
    CHECK(y["role"].as<std::string>() == "Instructor");
    CHECK(y["office"].as<std::string>() == "C-210");
}

TEST_CASE("CSV escaping handles commas and quotes") {
    Student s(12, "Eve, \"The Great\"", "eve@uni.edu", 2028, {"CS,101", "AI\"Lab"});
    auto row = s.csv_row();
    REQUIRE(row.find("\"Eve, \"\"The Great\"\"\"") != std::string::npos);
}

TEST_CASE("Validation throws on bad email") {
    REQUIRE_THROWS_AS(Student(13, "Zed", "not-an-email", 2026), ValidationError);
}

TEST_CASE("Student round trip through JSON") {
    Student original(20, "Sam", "sam@uni.edu", 2029, {"CS101"});
    nlohmann::json j = original.to_json();
    Student rebuilt = Student::from_json(j);
    CHECK(rebuilt.id() == original.id());
    CHECK(rebuilt.name() == original.name());
    CHECK(rebuilt.email() == original.email());
}

TEST_CASE("Instructor from_json throws on missing field") {
    nlohmann::json j;
    j["id"] = 5;
    j["name"] = "Missing Office";
    j["email"] = "a@b.com";
    REQUIRE_THROWS_AS(Instructor::from_json(j), ValidationError);
}

TEST_CASE("Polymorphic CSV writer includes both types") {
    std::vector<Person*> people;
    people.push_back(new Student(1, "Ada", "ada@uni.edu", 2026, {"CS101"}));
    people.push_back(new Instructor(2, "Grace", "grace@uni.edu", "Room 314", {"CS101"}));

    std::string csv = writeAllPeopleCsv(people);
    REQUIRE(csv.find("Ada") != std::string::npos);
    REQUIRE(csv.find("Grace") != std::string::npos);
    REQUIRE(csv.find("Room 314") != std::string::npos);

    for (int i = 0; i < (int)people.size(); i++) {
        delete people[i];
    }
}