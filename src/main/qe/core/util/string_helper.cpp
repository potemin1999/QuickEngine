#include "string_helper.h"

void chop(string &str) {
    string spaces(" \t\f\v\n\r");
    unsigned int found = str.find_last_not_of(spaces);
    if (found != string::npos) {
        str.erase(found + 1);
    } else {
        str.clear();
    }
}

void split(string &str, char d, string *tokens) {
    stringstream iss(str);
    string item;
    int i = 0;

    while (getline(iss, item, d)) {
        if (item != " " && !item.empty()) {
            chop(item);
            tokens[i] = item;
            i++;
        }
    }
}