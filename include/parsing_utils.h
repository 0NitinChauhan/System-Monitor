#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>

class ParsingUtils {
   public:
    static std::unordered_map<std::string, long> ParseMultipleLinesGetKeys(std::unordered_map<std::string, long> &expectedKeysMap, std::string filePath);

    static std::unordered_map<std::string, int> ParseMultipleLinesGetKeys(std::unordered_map<std::string, int> &expectedKeysMap, std::string filePath);

    static std::string MatchKeyAndReturnSingleLine(std::string expectedKey, std::string filePath);

    static std::string ReturnFileStreamLineOutput(std::string filePath);

    static std::vector<std::string> split(std::string line, char delimiter);

   private:
    static std::vector<std::string> splitOnSpace(std::string line);
};

#endif