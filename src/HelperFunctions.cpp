#include "HelperFunctions.h"
std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

int countChar(std::string s,char c) {
  int count = 0;

  for (unsigned int i = 0; i < s.size(); i++)
    if (s[i] == c) count++;

  return count;
}
