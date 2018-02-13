#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Grundklasse der Anwendung
class Base
{
public:
    Base();
    int zaehler=0;
    char zeichen;
    char puffer[3000];
    string searchCategory;
    char userEingabe[128];

    void load();
    void convert();
    void parseXML();
    void parseTXT();
    void search();
};

Base::Base() {
}
