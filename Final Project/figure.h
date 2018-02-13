#include <string>
#include <iostream>

// Figur-Object mit Eigenschaften einer Figur
class Figure {
public:
    Figure();
    string id;
    string name;
    string size;
    string price;
    string origin;
    vector<string> additionalValues;
};

Figure::Figure() {
}
