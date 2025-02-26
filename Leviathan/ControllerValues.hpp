#pragma once

#include <string>
#include <algorithm>
#include <cmath>
#include "Controller.hpp"

/**
 * @class ControllerValues
 * @brief Handles the representation of the values of a controller throughout the application.
 */
class ControllerValues
{
public:
	float ljoyx, ljoyy, rjoyx, rjoyy, ltrigger, rtrigger;
    int a, b, x, y, lbumper, rbumper, up, down, left, right;

    /**
     * @brief Transforms all values into a string seperated by "!". Does not modify values. 
     * @return Returns a string of values as stated above.
     */
    std::string toStringFull()
    {
        return std::to_string(ljoyx) + "!" + std::to_string(ljoyy) + "!" + std::to_string(rjoyx) + "!" + std::to_string(rjoyy) + "!" + std::to_string(ltrigger) + "!" + std::to_string(rtrigger) + "!"
         + std::to_string(a) + "!" + std::to_string(b) + "!" + std::to_string(x) + "!" + std::to_string(y) + "!" + std::to_string(lbumper) + "!" + std::to_string(rbumper) + "!" + std::to_string(up) + "!" + std::to_string(down) + "!" + std::to_string(left) + "!" + std::to_string(right);
    }
    /**
     * @brief Transforms all values into a string seperated by "!". It automatically tranasforms and clamps each value into a more 
     * "space effecient" storage medium with joy's mapped into non-float values with 4 sig-figs, and represents buttons only if pressed.
     * @return Returns a string of values as stated above.
     */
    std::string toStringPartial()
    {
        ljoyx = std::clamp((int)(ljoyx*1000), -999, 999);
        ljoyy = std::clamp((int)(ljoyy*1000), -999, 999);
        rjoyx = std::clamp((int)(rjoyx*1000), -999, 999);
        rjoyy = std::clamp((int)(rjoyy*1000), -999, 999);

        std::string str = std::to_string((int)ljoyx) + "!" + std::to_string((int)ljoyy) + "!" + std::to_string((int)rjoyx) + "!" + std::to_string((int)rjoyy) + "!" + std::to_string(std::clamp((int)std::round(ltrigger*100), 0, 99)) + "!" + std::to_string(std::clamp((int)std::round(rtrigger*100), 0, 99));
        
        if(a==1){
            str+="!a";
        }
        if(b==1){
            str+="!b";
        }
        if(x==1){
            str+="!x";
        }
        if(y==1){
            str+="!y";
        }
        if(lbumper==1){
            str+="!j";
        }
        if(rbumper==1){
            str+="!k";
        }
        if(up==1){
            str+="!u";
        }
        if(down==1){
            str+="!d";
        }
        if(left==1){
            str+="!l";
        }
        if(right==1){
            str+="!r";
        }
        
        return str;
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

