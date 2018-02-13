#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include "att.h"

int ClattToken::getAttList(
char                      *eingabe)
{
char puffer[100];
int zaehler;
enum zustand { zwischenTags, inNamen, erwarteAttributNamen, erwarteAttributWert,
               verarbeiteAttributWert} ;
enum zustand zustand;

for (zaehler=0,zustand=inNamen,anzahlAtt=0;*eingabe!='\0';
     eingabe = eingabe + 1)
    {
   switch(*eingabe)
      {
   case ' ':
      if (zustand == inNamen)
         {
         zustand = erwarteAttributNamen;
         *eingabe='\0';
         zaehler=0;
         }
      else if (zustand == verarbeiteAttributWert)
         {
         puffer[zaehler] = *eingabe;
         zaehler++;
         }
      break;

   case '=':
      if (zustand == erwarteAttributNamen)
         {
         zustand = erwarteAttributWert;
         puffer[zaehler] = '\0';
         attName[anzahlAtt] = new char[zaehler+1];
         strcpy(attName[anzahlAtt],puffer);
         zaehler=0;
         }
      else if (zustand == verarbeiteAttributWert)
         {
         puffer[zaehler] = *eingabe;
         zaehler++;
         }
      else cout << "Fehlerhaftes Zeichen! '='" << endl;
      break;

   case '"':
      if (zustand == erwarteAttributWert) 
         {
         zustand = verarbeiteAttributWert;
         zaehler = 0;
         }
      else if (zustand == verarbeiteAttributWert)
         {
         zustand = erwarteAttributNamen;
         puffer[zaehler] = '\0';
         attValue[anzahlAtt] = new char[zaehler+1];
         strcpy(attValue[anzahlAtt],puffer);
         zaehler=0;
         anzahlAtt++;
         }
      else cout << "Fehlerhaftes Zeichen! '\"'" << endl;
      break;

   default:
      if (zustand >= erwarteAttributNamen)
         {
         puffer[zaehler] = *eingabe;
         zaehler++;
         }
      break;
      }
   }

return 1;
}
