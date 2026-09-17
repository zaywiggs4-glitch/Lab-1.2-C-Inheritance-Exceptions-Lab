#include "student.hpp"
#include "instructor.hpp"
#include <iostream>
#include <vector>

int main() {
    try {
        std::vector<Person*> people;
        people.push_back(new Student(1, "Ada", "ada@uni.edu", 2026, {"CS101", "MATH200"}));
        people.push_back(new Instructor(2, "Grace", "grace@uni.edu", "Room 314", {"CS101"}));

        nlohmann::json j = nlohmann::json::array();
        for (int i = 0; i < (int)people.size(); i++) {
            j.push_back(people[i]->to_json());
        }
        std::cout << "JSON:\n" << j.dump(2) << "\n\n";

        YAML::Node y;
        for (int i = 0; i < (int)people.size(); i++) {
            y.push_back(people[i]->to_yaml());
        }
        std::cout << "YAML:\n" << YAML::Dump(y) << "\n";

        std::cout << "\nCSV (Student):\n";
        Student tmpS(99, "Tmp", "tmp@uni.edu", 2030);
        std::cout << tmpS.csv_header() << "\n";
        std::cout << static_cast<Student*>(people[0])->csv_row() << "\n";

        std::cout << "\nCSV (Instructor):\n";
        Instructor tmpI(98, "TmpI", "tmpI@uni.edu", "Room 1");
        std::cout << tmpI.csv_header() << "\n";
        std::cout << static_cast<Instructor*>(people[1])->csv_row() << "\n";

        for (int i = 0; i < (int)people.size(); i++) {
            delete people[i];
        }
    } catch (const ValidationError& e) {
        std::cerr << "ValidationError: " << e.what() << "\n";
        return 2;
    } catch (const SerializationError& e) {
        std::cerr << "SerializationError: " << e.what() << "\n";
        return 3;
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << "\n";
        return 1;
    }
    return 0;
}