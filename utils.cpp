#include "utils.h"

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

void boundAngles(vec3* angle)
{
    while (angle->x > 360) angle->x -= 360;
    while (angle->x < 0) angle->x += 360;

    while (angle->y > 360) angle->y -= 360;
    while (angle->y < 0) angle->y += 360;

    while (angle->z > 360) angle->z -= 360;
    while (angle->z < 0) angle->z += 360;
}
