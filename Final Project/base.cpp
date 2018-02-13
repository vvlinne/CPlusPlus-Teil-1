#include "base.h"
#include <fstream>
#include <iostream>
#include <token.h>
#include <string.h>
#include <string>
#include <figure.h>
#include <vector>

using namespace std;

// Lädt Datei
ifstream Inputfile;

// Gibt Datei aus
ofstream Outputfile;

// Vektor enthält Figure-Objekte
vector<Figure> figureVector;

// Hauptfunktion
int main() {
    Base baseclass;
    baseclass.load();
}

// Menü-Funktionen, liest TXT und XML-Dateien ein
void Base::load() {
    cout << "Bitte absoluten Pfad zur XML-Datei eingeben." << endl;
    cout << "z.B. c:\\temp\\Daten.xml" << endl;
    cout << "Eingabe: ";
    cin >> this->userEingabe;
    this->parseXML();
    cout << "Verarbeitung der XML-Datei abgeschlossen." << endl;
    cout << "Bitte absoluten Pfad zur TXT-Datei eingeben." << endl;
    cout << "z.B. c:\\temp\\Daten.txt" << endl;
    cout << "Eingabe: ";
    cin >> this->userEingabe;
    this->parseTXT();
    cout << "Verarbeitung der TXT-Datei abgeschlossen." << endl;

    char eingabe;

    //
    int abbruch=0;
    for(;abbruch==0;){
        cout << " Waehlen Sie eine Option! " << endl << endl;
        cout << "     (K)onvertieren" << endl;
        cout << "     (S)uchen" << endl;
        cout << "     (E)xit" << endl << endl;

        cout << "Eingabe: ";
        cin >> eingabe;
          switch(toupper(eingabe)){ // Wandelt kleine in Großbuchstaben um

          // Konvertieren von XML- und TXT-Datei zu einer neuen XML-Datei
          case 'K': {
                cout << "Bitte absoluten Pfad zur erstellenden XML-Datei eingeben." << endl;
                cout << "z.B. c:\\temp\\neueXMLdatei.xml" << endl;
                cout << "Eingabe: ";
                this->convert();
                break;
          }
          // Suchoption nach Informationen in den Dateien
          case 'S': {
                this->search();
                break;
          }
          // Exit-Option
          case 'E': {
              abbruch=1;
              break;
            }
          }
      }
      cout << endl << "Exit!" << endl;
}

// Einlesen und Zwischenspeichern der angegebenen XML-Datei. Benutzt Token- und Attributklasse aus den Vorlesungfolien.
void Base::parseXML() {
    ClToken token;
    Inputfile.open(this->userEingabe);
    zaehler = 0;

    string name;
    string price;
    string size;
    string id;
    string origin;

    for (;;)
    {
        if (token.getToken(Inputfile)==0) break;
        for (int i=0;i<token.att.zahlAtt();i++) {
            if(strcmp(token.att.zeigeAttName(i), "id") == 0) {
                id = token.att.zeigeAttWert(i);
            }
        }

        if(strcmp(token.end(), "name") == 0) {
            name = token.inhalt();
        } else if(strcmp(token.end(), "origin") == 0) {
            origin = token.inhalt();
        } else if(strcmp(token.end(), "size") == 0) {
            size = token.inhalt();
        } else if(strcmp(token.end(), "price") == 0) {
            price = token.inhalt();
        }

        if(strcmp(token.end(), "figure") == 0) {
            Figure figureObject;
            figureObject.id = id;
            figureObject.name = name;
            figureObject.size = size;
            figureObject.price = price;
            figureObject.origin = origin;
            figureVector.push_back(figureObject);
        }
    }
    Inputfile.close();
}

// Einlesen und Zwischenspeichern der angegebenen TXT-Datei.
void Base::parseTXT() {
    char puffer[3000];

    enum zustandTyp {Start, inDaten};
    enum zustandTyp zustand;

    zustand = Start;
    zaehler = 0;

    string id;
    string storageUnit;
    string amount;

    Inputfile.open(this->userEingabe);
    for (zaehler=0;;)
        {
        Inputfile.get(zeichen);
        if (Inputfile.eof()){
            puffer[zaehler]='\0';
            amount = puffer;

            for (vector<Figure>::iterator it = figureVector.begin() ; it != figureVector.end(); it++) {
                string figureId = it->id;
               if(strcmp(figureId.c_str(), id.c_str()) == 0) {
                 it->additionalValues.push_back(storageUnit);
                 it->additionalValues.push_back(amount);
               }
            }
            break;
        }

        switch(zeichen)
           {
            case ' ':
               if (zaehler)
                  {
                   puffer[zaehler]='\0';
                   zaehler=0;

                   if(zustand == Start) {
                    id = "";
                    id = puffer;
                   } else {
                    storageUnit = puffer;
                   }
                  zustand = inDaten;
                }
               break;
            case '\n':
                if (zaehler)
                   {
                   puffer[zaehler]='\0';
                   amount = puffer;
                   zaehler=0;

                   for (vector<Figure>::iterator it = figureVector.begin() ; it != figureVector.end(); it++) {
                       string figureId = it->id;
                      if(strcmp(figureId.c_str(), id.c_str()) == 0) {
                        it->additionalValues.push_back(storageUnit);
                        it->additionalValues.push_back(amount);
                      }
                   }

                   zustand = Start;
                }
               break;
            default:
               puffer[zaehler]=zeichen;
               zaehler++;
               break;
               }
            }
    Inputfile.close();
}

// Konvertiert Daten aus TXT- und XML-Datei in eine neue XML-Datei.
void Base::convert() {
    cin >> userEingabe;
    Outputfile.open(userEingabe);
    Outputfile << "<listFigures>\n";
    for (vector<Figure>::iterator it = figureVector.begin() ; it != figureVector.end(); it++) {
        Outputfile << "<figure>\n<meta>\n<name id=\"" << it->id << "\">" << it->name << "</name>\n<origin>" << it->origin << "</origin>\n";
        Outputfile << "<size>" << it->size << "</size>\n<price>" << it->price << "</price>\n</meta>\n<stock>\n";

        for(int index = 0; index < it->additionalValues.size(); index++) {
            Outputfile << "<package place=\"" << it->additionalValues[index] << "\">" << it->additionalValues[index + 1] << "</package>\n";
            index++;
        }

        Outputfile << "</stock>\n</figure>\n";
    }
    Outputfile << "</listFigures>\n";

    cout << "Textdatei an der Stelle ( " << userEingabe << " ) erstellt." << endl << endl;
    Outputfile.close();
}

// Suchen nach Kategorie (Klasseneigenschaft) und Suchbegriff (Wert)
void Base::search() {
    char term[100];
    int gefundeneBegriffe = 0;
    cout << endl << "Nach welcher Kategorie soll gesucht werden?" << endl;
    cout << "Verfügbare Kategorien sind: id, name, size, price und origin." << endl;
    cout << "Eingabe: ";
    cin >> searchCategory;
    cout << "Welcher Suchbegriff soll enthalten sein?" << endl;
    cout << "Eingabe: ";
    cin.ignore();
    cin.getline(term, 100);
    cout << endl << "Kategorie (Klasseneigenschaft) " << searchCategory << " soll Begriff " << term << "enthalten." << endl << endl;

    for(int index=0;index<figureVector.size();index++){
        if(strcmp(figureVector[index].name.c_str(), term) == 0 && strcmp(searchCategory.c_str(), "name") == 0 ||
                strcmp(figureVector[index].origin.c_str(), term) == 0 && strcmp(searchCategory.c_str(), "origin") == 0 ||
                strcmp(figureVector[index].price.c_str(), term) == 0 && strcmp(searchCategory.c_str(), "price") == 0 ||
                strcmp(figureVector[index].size.c_str(), term) == 0 && strcmp(searchCategory.c_str(), "size") == 0 ||
                strcmp(figureVector[index].id.c_str(), term) == 0 && strcmp(searchCategory.c_str(), "id") == 0) {
                      gefundeneBegriffe++;
                      cout << "----------------- Result ------------" << endl;
                      cout << "id: " << figureVector[index].id << endl;
                      cout << "name: " << figureVector[index].name << endl;
                      cout << "origin: " << figureVector[index].origin << endl;
                      cout << "price: " << figureVector[index].price << endl;
                      cout << "size: " << figureVector[index].size << endl;
                      cout << endl << endl << endl;
        }
    }

    if(gefundeneBegriffe == 0) {
               cout << "Begriff " << term << " oder Klasseneigenschaft " << searchCategory << " nicht gefunden." << endl;
    }
}
