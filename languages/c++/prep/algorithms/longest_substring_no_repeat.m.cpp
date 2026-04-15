// longest substring without repeating characters
// "abcabcbb" -> 3


#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

struct LongSubstrings {
    std::vector<std::string> substrings {};
    int longest = 0;

    std::ostream& print(std::ostream& os) const {
        os << "sub-strings[" << std::endl;
        for (const std::string& s : substrings) {
            os << "    " << s << std::endl;
        }
        os << "longest length is " << longest << std::endl;
        os << ']' << std::endl;
        return os;
    }
};


LongSubstrings findLongestSubstrings(const std::string& data) {
    LongSubstrings ret;
    int start = 0;
    int end = 0;

    // start -> end (inclusive) defines the sub-string

    std::unordered_map<char, int> lastCharLocation{};

    // keep extending sub-string to see if there is a longer one
    while (end < data.size()) {
        char endChar = data[end];
        std::cout << "checking " << endChar << " at " << end << ", start is " << start << std::endl;
        
        // check if the new char is in seen before 
        auto found = lastCharLocation.find(endChar);
        if (found != lastCharLocation.end()) {
            std::cout << "  found " << endChar << " most recent at " << found->second << std::endl;
            if (found->second >= start) {
                // found the previous character in the substring
                start = found->second + 1;
                std::cout << "      move start to " << start << std::endl;
            }
        }

        // recording sub-string
        int subLength = end - start + 1;
        std::string substr = data.substr(start, subLength);
        ret.substrings.push_back(substr);
        std::cout << "     now sub string is " << substr << std::endl;
        if (subLength > ret.longest) {
            ret.longest = subLength;
        }
        
        // update last char location for endChar
        lastCharLocation[endChar] = end;
        ++end;
    }

    return ret;
}

void run(const std::string& data) {
    findLongestSubstrings(data).print(std::cout);
}

int main(int argc, char *argv[]) { 
    run("abcabcbb"); 
    return 0;
}
