#pragma once

#include <string>
#include "Controller.hpp"

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
    // std::vector<int> diffIndex(){
    //     ControllerValues conc = &Controller::GetControllerValues();
    //     std::vector<int> indexs;
    //     if(conc.toString().compare(toString()) != 0){
    //         std::vector<std::string> one = getWords(toString(), "!");
    //         std::vector<std::string> two = getWords(conc.toString(), "!");

    //         for(std::vector<std::string>::size_type i = 0; i != one.size(); i++) {
    //             if(stoi(one[i]) != stoi(two[i])){
    //                 auto it = find(two.begin(), two.end(), two[i]); 
    //                 int index = it - two.begin(); 
    //                 indexs.insert(index);
    //             }
    //         }
    //     }
    //     return indexs; 
    // }
    // std::vector<std::string> getWords(std::string s, std::string delim) {
    //     std::vector<std::string> res;
    //     std::string token = "";
    //     for (int i = 0; i < s.size(); i++) {
    //         bool flag = true;
    //         for (int j = 0; j < delim.size(); j++) {
    //             if (s[i + j] != delim[j]) flag = false;
    //         }
    //         if (flag) {
    //             if (token.size() > 0) {
    //                 res.push_back(token);
    //                 token = "";
    //                 i += delim.size() - 1;
    //             }
    //         } else {
    //             token += s[i];
    //         }
    //     }
    //     res.push_back(token);
    //     return res;
    // }
};

