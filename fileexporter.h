#ifndef FILEEXPORTER_H
#define FILEEXPORTER_H

#include <iostream>
#include <vector>
#include <valarray>
#include <string>
#include <fstream>
#include <regex>

using namespace std;

class Fileexporter
{
private:
    string webgltemplate;
    vector <valarray <float> >allnormalslist;
    vector <valarray <float> >allvertslist;
    vector <valarray <float> >allcolorslist;
public:
    Fileexporter();
    void addverts(vector <valarray <float> >vertslist);
    void addnormals(vector <valarray <float> >normalslist);
    void addcolors(vector <valarray <float> >colorslist);
    void savewebgl(string name);
    ~Fileexporter();
};

#endif
