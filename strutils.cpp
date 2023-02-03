#include "strutils.h"

vector<string> split(string str, string delimiter) {
    vector<string> results;

    string item;
    int start = 0, end = -1;
    int strLength = str.length();
    while (start < strLength) {
        end = str.find(delimiter, start);

        if (end == string::npos) end = strLength;

        item = str.substr(start, end - start);
        results.push_back(item);

        start = end + 1;
    }

    return results;
}
