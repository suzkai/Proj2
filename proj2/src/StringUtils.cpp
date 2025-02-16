#include "StringUtils.h"

namespace StringUtils{

std::string Slice(const std::string &str, ssize_t start, ssize_t end) noexcept {
    ssize_t str_len = str.size();

    // python-style handling
    // takes negative values and converts to postiive
    if (start < 0) {
        start += str_len;
    }
    if (end <= 0) {
        end += str_len; 
    }

    // ensure everything's in bounds
    if (start < 0) {
        start = 0;
    }
    if (end > str_len) {
        end = str_len;
    }   
    if (start >= end) {
        return ""; 
    }

    return str.substr(start, end - start);
}

std::string Capitalize(const std::string &str) noexcept{
    if (str.empty()) { 
        return "";
    }

    std::string given = str; 
    given[0] = std::toupper(given[0]); // caps first letter

    for (size_t i = 1; i < given.size(); i++) {
        given[i] = std::tolower(given[i]); // lowercasing rest of the string
    }

    return given;
}

std::string Upper(const std::string &str) noexcept {
    std::string given = str; 

    for (char &ch : given) { // go thru each character in the string
        ch = std::toupper(ch); // make each character capitalized
    }

    return given; 
}

std::string Lower(const std::string &str) noexcept{
    std::string given = str; 

    for (char &ch : given) { // go thru each character in the string
        ch = std::tolower(ch); // make each character lwoercase
    }

    return given; 
}

std::string LStrip(const std::string &str) noexcept {
    size_t start = 0;

    // skip whitespace
    while (start < str.size() && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n' || str[start] == '\r' || str[start] == '\f' || str[start] == '\v')) {
        start++;
    }

    return str.substr(start);
}

std::string RStrip(const std::string &str) noexcept {
    ssize_t end = str.size() - 1;

    // skip whitespace
    while (end >= 0 && (str[end] == ' ' || str[end] == '\t' || str[end] == '\n' || str[end] == '\r' || str[end] == '\f' || str[end] == '\v')) {
        end--;
    }

    return str.substr(0, end + 1);
}

std::string Strip(const std::string &str) noexcept{
    return RStrip(LStrip(str)); // reuse code from LStrip and Rstrip
}

// double check

std::string Center(const std::string &str, int width, char fill) noexcept{
    if (static_cast<int>(str.size()) >= width) {
        return str; // if string is already as wide / wider, return as is
    }

    int pad = width - static_cast<int>(str.size());
    int left_pad = pad / 2; // left padding smaller when odd 
    int right_pad = pad - left_pad; // right padding gets the rest

    return std::string(left_pad, fill) + str + std::string(right_pad, fill);
}

std::string LJust(const std::string &str, int width, char fill) noexcept{
    if (static_cast<int>(str.size()) >= width) {
        return str; // if string is already as wide / wider, return as is
    }

    return str + std::string(width - str.size(), fill);
}

std::string RJust(const std::string &str, int width, char fill) noexcept{
    if (static_cast<int>(str.size()) >= width) {
        return str; // if string is already as wide / wider, return as is
    }

    return std::string(width - str.size(), fill) + str;
}

std::string Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept{
    if (old.empty()){
        return str; // avoid infinite loop, if old is empty just retunr
    }

    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(old, pos)) != std::string::npos) { // loop till old is not in result
        result.replace(pos, old.length(), rep);
        pos += rep.length();
    }

    return result;
}

std::vector<std::string> Split(const std::string &str, const std::string &splt) noexcept {
    std::vector<std::string> result;
    size_t start = 0, end;

    // if splt is empty then split on whitespace
    if (splt.empty()) {
        std::string word;
        for (char ch : str) {
            if (std::isspace(ch)) {
                if (!word.empty()) {
                    result.push_back(word);
                    word.clear();
                }
            } else {
                word += ch;
            }
        }
        if (!word.empty()) result.push_back(word);
        return result;
    }

    // split using given delimiter
    while ((end = str.find(splt, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + splt.size();
    }
    
    result.push_back(str.substr(start));
    return result;
}


std::string Join(const std::string &str, const std::vector< std::string > &vect) noexcept{
    if (vect.empty()) {
        return "";
    }

    // iterate thru vector then add str + next element
    std::string result = vect[0];
    for (size_t i = 1; i < vect.size(); ++i) {
        result += str + vect[i];
    }

    return result;
}

std::string ExpandTabs(const std::string &str, int tabsize) noexcept {
    if (tabsize < 0){
        return str; // prevent weirdness
    }
    std::string result;
    int column = 0;

    for (char ch : str) {
        if (ch == '\t') {
            int spaces = 0;
            if(tabsize > 0) { // if tabsize is 0 then there'd be a problem w/ mod zero
                spaces = tabsize - (column % tabsize); // calc tabstop
            }
            result.append(spaces, ' '); 
            column += spaces; 
        } else {
            result.push_back(ch);
            column++;
            if (ch == '\n') column = 0; // reset column after newline
        }
    }

    return result;
}

// calcs the Levenshtein distance (edit distance) b/w 2 strings
// also if ignorecase is true then comparison is case-insensitive
int EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept {
    size_t lsize = left.size();
    size_t rsize = right.size();
    std::vector<std::vector<int>> dp(lsize + 1, std::vector<int>(rsize + 1, 0)); // 2d dp table

    for (size_t i = 0; i <= lsize; i++) dp[i][0] = i; // column 
    for (size_t j = 0; j <= rsize; j++) dp[0][j] = j; // row

    for (size_t i = 1; i <= lsize; i++) { // edit dist for each pair of chars in left and right strings
        for (size_t j = 1; j <= rsize; j++) {
            char lch = left[i - 1];
            char rch = right[j - 1];

            if (ignorecase) { // if ignorecase is true then convert all to lowercase 
                if (lch >= 'A' && lch <= 'Z') lch += ('a' - 'A');
                if (rch >= 'A' && rch <= 'Z') rch += ('a' - 'A');
            }

            if (lch == rch) { // if chars match, no need to edit - take value from the diagonal (previous edit dist)
                dp[i][j] = dp[i - 1][j - 1];
            } else { // otherwise find minimum of: deleting char from left string, inserting char in left string, replacing a char
                dp[i][j] = 1 + std::min(dp[i - 1][j], std::min(dp[i][j - 1], dp[i - 1][j - 1]));
            }
        }
    }

    return dp[lsize][rsize];
}

};