#include "parsing_utils.h"

std::unordered_map<std::string, int> ParsingUtils::ParseMultipleLinesGetKeys(std::unordered_map<std::string, int> &expectedKeysMap, std::string filePath) {
    std::string line;
    std::string key;
    int value;
    std::ifstream fileStream(filePath);
    while (std::getline(fileStream, line)) {
        std::stringstream lineStream(line);
        while (lineStream >> key >> value) {
            if (expectedKeysMap.count(key) > 0) {
                expectedKeysMap[key] = value;
                break;
            }
        }
    }
    return expectedKeysMap;
}

std::unordered_map<std::string, long> ParsingUtils::ParseMultipleLinesGetKeys(std::unordered_map<std::string, long> &expectedKeysMap, std::string filePath) {
    std::string line;
    std::string key;
    long value;
    std::ifstream fileStream(filePath);
    while (std::getline(fileStream, line)) {
        std::stringstream lineStream(line);
        while (lineStream >> key >> value) {
            if (expectedKeysMap.count(key) > 0) {
                expectedKeysMap[key] = value;
                break;
            }
        }
    }
    return expectedKeysMap;
}

std::string ParsingUtils::MatchKeyAndReturnSingleLine(std::string expectedKey, std::string filePath) {
    std::string line;
    std::string key;

    std::ifstream fileStream(filePath);
    while (std::getline(fileStream, line)) {
        std::stringstream lineStream(line);
        while (lineStream >> key) {
            if (key == expectedKey) {
                return line;
            }
        }
    }
    return line;
}

std::string ParsingUtils::ReturnFileStreamLineOutput(std::string filePath) {
    std::ifstream fileStream(filePath);

    std::string line;
    if (fileStream.is_open()) {
        std::getline(fileStream, line);
    }
    return line;
};

std::vector<std::string> ParsingUtils::split(std::string line, char delimiter) {
    if (delimiter == ' ') {
        return ParsingUtils::splitOnSpace(line);
    }
    std::stringstream lineStream(line);
    std::string token;
    std::vector<std::string> output{};

    while (std::getline(lineStream, token, delimiter)) {
        output.push_back(token);
    }
    return output;
}

std::vector<std::string> ParsingUtils::splitOnSpace(std::string line) {
    std::vector<std::string> output;
    std::istringstream lineStream(line);
    for (std::string str; lineStream >> str;) {
        output.push_back(str);
    }
    return output;
}
