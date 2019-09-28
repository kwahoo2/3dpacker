#include "fileexporter.h"



Fileexporter::Fileexporter()
{
    ifstream temfile( "data-template.h" ); //wczytanie szablonu pliku webgl
    string line;
    while (getline(temfile, line))   
    {
        webgltemplate += line + "\n"; 
    }
}

void Fileexporter::addverts(vector <valarray <float> >vertslist)
{
    allvertslist.insert(end(allvertslist), begin(vertslist), end(vertslist));
}
void Fileexporter::addnormals(vector <valarray <float> >normalslist)
{
    allnormalslist.insert(end(allnormalslist), begin(normalslist), end(normalslist));
}
void Fileexporter::addcolors(vector <valarray <float> >colorslist)
{
    allcolorslist.insert(end(allcolorslist), begin(colorslist), end(colorslist));
}


void Fileexporter::savewebgl(string name)
{
        ofstream outfile(name);
        
        string s0, s1, s2, s3, s4; //kolejne fragmenty podmieniane w szablonie
        string webgldata = webgltemplate;
        
        for(size_t i=0; i<allvertslist.size(); ++i) //wspolrzedne wierzcholkow + normalne
        {
            valarray <float> v = allvertslist[i];
            s0 = s0 + to_string(v[0]) + "f, " + to_string(v[1]) + "f, "+ to_string(v[2]) + "f, ";
            valarray <float> Nv = allnormalslist[i];
            s0 = s0 + to_string(Nv[0]) + "f, " + to_string(Nv[1]) + "f, "+ to_string(Nv[2]) + "f, ";
            valarray <float> fc = allcolorslist[i];
            s0 = s0 + to_string(fc[0]) + "f, " + to_string(fc[1]) + "f, "+ to_string(fc[2]) + "f, "+ to_string(fc[3]) + "f,\n";
        }

        s1 = to_string(allvertslist.size());

        /*for(size_t i=0; i<allnormalslist.size(); ++i) //normalne wiercholkow
        {
            valarray <float> Nv = allnormalslist[i];
            s2 = s2 + to_string(Nv[0]) + ", " + to_string(Nv[1]) + ", "+ to_string(Nv[2]) + ", ";
        }
        
        for(size_t i=0; i<allcolorslist.size(); ++i) //normalne wiercholkow
        {
            valarray <float> fc = allcolorslist[i];
            s3 = s3 + to_string(fc[0]) + ", " + to_string(fc[1]) + ", "+ to_string(fc[2]) + ", "+ to_string(fc[3]) + ", ";
        }
        
        s4 = to_string(allcolorslist.size());*/
        
        webgldata = regex_replace(webgldata, regex("verticesData"), s0);
        webgldata = regex_replace(webgldata, regex("itemsData"), s1); //liczba elementow: wiercholkow/normalnych
        //webgldata = regex_replace(webgldata, regex("vertexnormalsData"), s2);
       // webgldata = regex_replace(webgldata, regex("colorsData"), s3);
        //webgldata = regex_replace(webgldata, regex("itemsColorData"), s4); //liczba elementow kolorow, 4xliczba wiercholkow
        //webgldata = regex_replace(webgldata, regex("bbMax"), "100"); //TODO
        
        outfile << webgldata << "\n";

        cout <<"Zapisano " << name << endl;
        outfile.close();
}

Fileexporter::~Fileexporter()
{
    
}
 
