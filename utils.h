#pragma once

#ifndef __strUtils
#define __strutils

#include <string>
#include <vector>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

vector<string> split(string str, string delimiter);

void boundAngles(vec3* angle);

#endif // !__strUtils