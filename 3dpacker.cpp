#include "vrmlimporter.h"
#include "trianglecalculator.h"
#include "fileexporter.h"
#include <iostream>

using namespace std;

 
int main(int argc, char *argv[])
{
    if (argc < 3) 
    {
        cout << "Nie podano nazwy pliku wejsciowego i wyjsciowego" << endl;
        return 0;
    }
    cout <<"Rozpoczecie"<< endl;
    Vrmlimporter vimp;
    Trianglecalculator tcalc;
    Fileexporter fexp;
    
    string inputfile =  argv[1];
    string outputfile =  argv[2];
    vimp.importvrml(inputfile);
    
    int shapescount = vimp.getshapescount();
    for (int i = 0; i < shapescount; ++i)
    {
        vector < vector<float> > verts = vimp.getshapeverts(i);
        vector < vector<int> > coordidx = vimp.getshapeidxs(i);
        vector < vector<float> > colors = vimp.getshapecolors(i);
        vector < vector<int> > coloridxs = vimp.getshapecoloridxs(i);
        vector <float> dc = vimp.getdiffusecolor(i);
        tcalc.clearshape();
        tcalc.setdiffusecolor(dc);
        tcalc.calcshape(verts, coordidx, colors, coloridxs);
        fexp.addverts(tcalc.getvertslist()); //uzupenienie listy pozycji wiercholkow do zapisu
        fexp.addnormals(tcalc.getnormalslist());//uzupenienie listy normalnych wiercholkow do zapisu
        fexp.addcolors(tcalc.getcolorslist()); //uzupelnienie lsty kolorow
    }
    fexp.savewebgl(outputfile);
    
    return 0;
}
