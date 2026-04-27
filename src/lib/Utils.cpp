#include "Utils.h"

namespace audit{
std:: string trim(const std::string& str){
    std::string result;

    for (char c : str){
        if (c==' ' || c=='\t' || c=='\n' || c=='\r'){
            continue;
        } else 
        {
            result += c;
        }
        }
    return result;
    }

std::vector<std::string> split(const std::string& str, char delimiter){
    std::string middle_result;
    std::vector<std::string> str_result;

    for (char c: str){
        if (c != delimiter){
            middle_result += c;
        } else {
            str_result.push_back(middle_result);
            middle_result.clear();

        }
    }
    return str_result;
}

bool startsWith(const std::string& str, const std::string& prefix){
    bool flag = true;
    for (int i; i<prefix.size();i ++){
        if (flag and str[i]==prefix[i]){
            flag=true;
        }

    }
    return flag;
}

}

