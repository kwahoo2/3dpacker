#ifndef TRIANGLECALCULATOR_H
#define TRIANGLECALCULATOR_H

#include <iostream>
#include <vector>
#include <valarray>
#include <cmath>
#include <tuple> // C++11, dla std::tie
#include <algorithm>
#include <iterator>



using namespace std;

static const float creaseangle = ( 30 * M_PI / 180.0 ); //kat od ktorego zalezy czy bedzie cieniowanie plaskie czy Gouraud

struct Triangle;

class Trianglecalculator
{
private:
    vector <Triangle> triangles; //dla pojedynczej bryly
    vector <Triangle> refacttrian; //trojkaty z uporzadkowanymi indeksami
    vector < valarray <float> > refactv;
    
    valarray <float> normv3 (valarray <float> vec);
    float getangle (valarray <float> N0, valarray <float> N1);
    vector <int> findtriangles (int index, vector <Triangle> trnlist);
    Triangle calctriangle (vector< vector<float> > points, vector< vector<int> > indexes, vector< vector<float> > colors, vector< vector<int> > coloridxs, long unsigned int trn);
    vector <Triangle> calcvnormals (vector <Triangle> trnlist);
    pair <vector <Triangle>, vector < valarray <float> > >idexnonrep (vector <Triangle> triangles);
    vector <float> diffusecolor; //diffuse dla wszystkich trojkatow jednego shape
public:
    Trianglecalculator();
    void calcshape(vector< vector<float> > verts, vector< vector<int> > idxs, vector< vector<float> > colors, vector< vector<int> > coloridxs);
    void clearshape();
    void setdiffusecolor(vector <float> dc);
    vector <valarray <float> >getvertslist();
    vector <valarray <float> >getnormalslist();
    vector <valarray <float> >getcolorslist();
    ~Trianglecalculator();
};

#endif
