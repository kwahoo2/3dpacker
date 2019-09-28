#ifndef VRMLIMPORTER_H
#define VRMLIMPORTER_H

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <sstream>

#include <unistd.h> //sleep


using namespace std;

bool botharespaces (char lhs, char rhs); //musi byc globalny lub static, bo string iterator tego wymaga

class Vrmlimporter
{
private:
    string scontent; //zawartosc pliku vrml
    long unsigned int shapescount;
    
    string extractblock (string keyword, string s, long unsigned int scpos);
    vector<string> splitblock (string block, string startdel, string enddel, string del);
    vector<float> parsefloats (string block);
    vector<int> parseints (string block);
    void adddefblocks (string s); 
    string finddefblock (string s);
        
    vector<vector< vector<float> > >shapeverts;
    vector< vector< vector<int> > >shapeidxs;
    vector<vector< vector<float> > >shapecolors;
    vector< vector< vector<int> > >shapecoloridxs;
    vector< vector<float> >diffusecolors;
    vector< vector<string>> defblocks;

    
    
public: //domyslnie elementy klasy sa prywatne
    Vrmlimporter();
    ~Vrmlimporter();
    int importvrml(string name);
    int getshapescount();
    vector< vector<float> >getshapeverts(int id);
    vector< vector<int> >getshapeidxs(int id);
    vector< vector<float> >getshapecolors(int id);
    vector< vector<int> >getshapecoloridxs(int id);
    vector<float> getdiffusecolor(int id);
};

#endif
