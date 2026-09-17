#pragma once
#include <string>

inline std::string csv_escape(std::string s) {
    bool needQuotes = false;
    for (char c : s) {
        if (c == ',' || c == '"' || c == '\n') {
            needQuotes = true;
            break;
        }
    }
    if (!needQuotes) {
        return s;
    }
    std::string result = "\"";
    for (char c : s) {
        if (c == '"') {
            result += "\"\"";
        } else {
            result += c;
        }
    }
    result += "\"";
    return result;
}