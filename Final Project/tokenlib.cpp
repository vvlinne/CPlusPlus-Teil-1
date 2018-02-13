#include <iostream>
using namespace std;
#include <fstream>
#include <string.h>
#include "token.h"

ClToken::ClToken()
{
tokenStart=NULL;
tokenEnde=NULL;
tokenInhalt=NULL;
}

int  ClToken::getToken(
ifstream              &datei)
{
int zaehler;
enum zustand zustand;
char zeichen;
char puffer[100];

if (tokenStart!=NULL)
   {
   delete tokenStart;
   tokenStart=NULL;
   }
if (tokenEnde!=NULL)
   {
   delete tokenEnde;
   tokenEnde=NULL;
   }
if (tokenInhalt!=NULL)
   {
   delete tokenInhalt;
   tokenInhalt=NULL;
   }
for (zaehler=0;;)
    {
    datei.get(zeichen);
    if (datei.eof())
       {
       if (tokenStart == NULL && tokenEnde == NULL && tokenInhalt == NULL) return 0;
       return 1;
       }
    switch(zeichen)
       {
    case '<':
       datei.get(zeichen);
       if (zeichen=='/')
          {
          zustand = istEndTag;
          if (zaehler!=0)
             {
             puffer[zaehler]='\0';
             tokenInhalt = new char[zaehler+1];
             strcpy(tokenInhalt,puffer);
             }
          }
       else
          {
          zustand=istStartTag;
          datei.putback(zeichen);
          }
       zaehler=0;
       break;
    case '>':
       puffer[zaehler]='\0';
       if (zustand==istStartTag)
          {
          tokenStart = new char[zaehler+1];
          strcpy(tokenStart,puffer);
          att.getAttList(puffer);
          }
       else
          {
          tokenEnde = new char[zaehler+1];
          strcpy(tokenEnde,puffer);
          return 1;
          }
       zaehler=0;
       break;
    case '\n':
       break;
    default:
       puffer[zaehler]=zeichen;
       zaehler++;
       break;
       }
    }
}
