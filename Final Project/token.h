#include "att.h"

class ClToken
   {
public:
   ClToken();
   char *start() { if (tokenStart != NULL) return tokenStart; else return ""; }
   char *end() { if (tokenEnde != NULL) return tokenEnde; else return ""; }
   char *inhalt() { if (tokenInhalt != NULL) return tokenInhalt; else return ""; }
   int getToken(ifstream &datei);
   ClattToken att;
private:
   char *tokenStart;
   char *tokenEnde;
   char *tokenInhalt;
   } ;

enum zustand { istStartTag, istEndTag } ;
