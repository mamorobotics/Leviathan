#pragma once

#include <string>

class ControllerValues
{
public:
	float ljoyx, ljoyy, rjoyx, rjoyy, ltrigger, rtrigger;
    int a, b, x, y, lbumper, rbumper, up, down, left, right;

    std::string toString()
    {
        return std::to_string(ljoyx) + "!" + std::to_string(ljoyy) + "!" + std::to_string(rjoyx) + "!" + std::to_string(rjoyy) + "!" + std::to_string(ltrigger) + "!" + std::to_string(rtrigger) + "!"
         + std::to_string(a) + "!" + std::to_string(b) + "!" + std::to_string(x) + "!" + std::to_string(y) + "!" + std::to_string(lbumper) + "!" + std::to_string(rbumper) + "!" + std::to_string(up) + "!" + std::to_string(down) + "!" + std::to_string(left) + "!" + std::to_string(right);
    }
};

