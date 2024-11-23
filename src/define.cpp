//
// Created by LONG on 11/7/2024.
//
#include "define.h"
#include "stdexcept"


string ServiceToString(const Service& service) {
    switch (service) {
        case CatToc: return "Cat toc";
        case NhuomToc: return "Nhuom toc";
        case TayToc: return "Tay toc";
        case UonToc: return "Uon toc";
        case PhucHoiToc: return "Phuc hoi toc";
        case TaoKieu: return "Tao kieu";
        case GoiDau: return "Goi dau";
        default: return "Unknown";
    }
}

Service StringToService(const string& str) {
    if (str == "CatToc") return CatToc;
    if (str == "NhuomToc") return NhuomToc;
    if (str == "TayToc") return TayToc;
    if (str == "UonToc") return UonToc;
    if (str == "PhucHoiToc") return PhucHoiToc;
    if (str == "TaoKieu") return TaoKieu;
    if (str == "GoiDau") return GoiDau;
    throw std::invalid_argument("Unknown service: " + str);
}

vector<string> Split(const string& s, const char& delim) {
    vector<string> tokens;
    string token;
    for (const char& x : s) {
        if (x == delim) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        else token += x;
    }
    if (!token.empty()) tokens.push_back(token);
    token.clear();
    return tokens;
}

string Replace(const string& s, const char& oldChar, const char& newChar) {
    string result = s;
    for (char& x : result) {
        if (x == oldChar) {
            x = newChar;
        }
    }
    return result;
}

int ToNum(const string& s) {
    if (!IsNumber(s)) return -1;
    int ans=0;
    for (const char& x : s) {
        ans = ans * 10 + (x - '0');
    }
    return ans;
}


bool IsBetween(const int& v, const int& L, const int& R) {
    return (v >= L && v <= R);
}

bool IsBetween(const char& v, const char& L, const char& R) {
    return (v >= L && v <= R);
}

bool IsDigit(const char &x) {
    return IsBetween(x,'0','9');
}

bool IsNumber(const string& s) {
    for (const char& x : s) {
        if (!IsBetween(x, '0', '9'))    return false;
    }
    return true;
}

