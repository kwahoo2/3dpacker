#include "trianglecalculator.h"

struct Triangle
{
    int id; //identyfikat, przydatny przy tescie
    int idx0; //ideksy wierzcholkow
    int idx1;
    int idx2;
    valarray <float> v0; //pozycje wierzcholkow
    valarray <float> v1;
    valarray <float> v2;
    
    valarray <float> Nf; //normalna sciany
    valarray <float> Nv0; //normalna wiercholka
    valarray <float> Nv1;
    valarray <float> Nv2;
    
    valarray <float> fcolor; //kolor calego trojkata
    
};

Trianglecalculator::Trianglecalculator()
{

}

valarray <float> Trianglecalculator::normv3 (valarray <float> vec) //normalizacja wektora
{
    float v0 = vec[0];
    float v1 = vec[1];
    float v2 = vec[2];

    
    float length = sqrt(v0 * v0 + v1 * v1 + v2 * v2);
    vec[0] = v0 / length;
    vec[1] = v1 / length;
    vec[2] = v2 / length;

    return vec;
}

float Trianglecalculator::getangle (valarray <float> N0, valarray <float> N1) //wyliczenie kata miedczy wektorami
{
    float dotproduct = N0[0] * N1[0] + N0[1] * N1[1] + N0[2] * N1[2];
    float magnitude = sqrt(N0[0] * N0[0] + N0[1] * N0[1] + N0[2] * N0[2]) * sqrt(N1[0] * N1[0] + N1[1] * N1[1] + N1[2] * N1[2]);
    float angle = acos(dotproduct / magnitude);
    return angle;
}

vector <int> Trianglecalculator::findtriangles (int index, vector <Triangle> trnlist) // zwraca numery wszyskich trojkatow zawierajacych wiecholek o danym indeksie
{
    vector <int> tlist;
    for (size_t i=0; i<trnlist.size(); ++i)
    {
        if ((trnlist[i].idx0 == index) || (trnlist[i].idx1 == index) || (trnlist[i].idx2 == index))
        {
            tlist.push_back(i);
        }
    }
    /*cout << "wiercholek " << index <<" nalezy do "<< endl;
    copy(tlist.begin(), tlist.end(), ostream_iterator<int>(cout, ", ")); // tylko do wyswietlania
    cout << "\n";*/
    return tlist;
}


Triangle Trianglecalculator::calctriangle (vector< vector<float> > points, vector< vector<int> > indexes, vector< vector<float> > colors, vector< vector<int> > coloridxs, long unsigned int trn)
{
    Triangle triangle; // wykorzystanie struktury
    
    vector<int> idx = indexes[trn]; //indeksy wiercholkow  1 trojkata
    vector< valarray <float> > v(3); //wektor matem 3 elementowy, wiercholek  
    //vector< valarray <float> > c(4); //kolor RGBA
    
    cout << "Trojkat " << trn << endl;
    cout << "Points size " << points.size() << endl;
    cout << idx.size() << endl;

    
    for (size_t j=0; j<3; ++j)
    {
        float p = points[idx[j]][0];
        float r = points[idx[j]][1];
        float s = points[idx[j]][2];
       // cout << p << " " << r << " " << s << ", ";
        v[j] = {p, r, s};
        
    }
    
    valarray <float> U = v[1] - v[0];
    valarray <float> V = v[2] - v[0];
    valarray <float> N = {U[1] * V[2] - U[2] * V[1], U[2] * V[0] - U[0] * V[2], U[0] * V[1] - U[1] * V[0]}; //iloczyn wektorowy
    N = normv3(N);
    
    /*
    Nx = UyVz - UzVy
    Ny = UzVx - UxVz
    Nz = UxVy - UyVx
    */
    
    //cout << "cs "<< trn<< endl;
    
    /*cout <<"wektory"<< endl;
    for(int n: U) cout << n << ' ';
    cout << endl;
    for(int n: V) cout << n << ' ';
    cout << endl;
    for(int n: N) cout << n << ' ';
    cout << endl;*/
    


    float cR, cG, cB, cA;
    if (coloridxs.size() > 1)
    {
        vector<int> colidx = coloridxs[trn]; //indeksy kolorow  1 trojkata
        cR = colors[colidx[0]][0];
        cG = colors[colidx[0]][1];
        cB = colors[colidx[0]][2];
        cA = 1.0; //ustawienie dla kanalu alfa, 1 - nieprzezroczysty
    }
    else
    {
        //miejsce na wykorzystanie diffuse lub domyslnego koloru
        cR = diffusecolor[0];
        cG = diffusecolor[1];
        cB = diffusecolor[2];
        cA = 1.0; //ustawienie dla kanalu alfa, 1 - nieprzezroczysty
        
    }
    
    valarray <float> fcolor = {cR, cG, cB, cA};
  
    triangle.id = trn;
    triangle.idx0 = idx[0]; //napelnienie struktury danymi, w tym miejscu nie bedzie jeszcze normalnych wierzchoków
    triangle.idx1 = idx[1];
    triangle.idx2 = idx[2];
    triangle.v0 = v[0];
    triangle.v1 = v[1];
    triangle.v2 = v[2];
    triangle.Nf = N;
    triangle.fcolor = fcolor;
    
    return triangle;
    

    
}

vector <Triangle> Trianglecalculator::calcvnormals (vector <Triangle> trnlist) //ppolicz normalne dla wszystkich wierzcholkow danego zestawu trojkatow, 
{
    vector <Triangle> outtrnlist = trnlist;
    
    for (size_t i=0; i<trnlist.size(); ++i)
        {
            Triangle t = trnlist[i]; //ten zawsze ten sam do pornwania
            int idx0 = t.idx0;
            int idx1 = t.idx1;
            int idx2 = t.idx2;
            vector <int> t0list = findtriangles (idx0, trnlist); //wszystkie trojkaty wspoldzielace wierzcholek 0 
            vector <int> t1list = findtriangles (idx1, trnlist);
            vector <int> t2list = findtriangles (idx2, trnlist);
            
            t.Nv0 = t.Nf; //przypisz wstepnie wierzcholkom normalnie powierzchni
            t.Nv1 = t.Nf;
            t.Nv2 = t.Nf;
            
            for (size_t j=0; j<t0list.size(); ++j)
            {
                Triangle t0 = trnlist[t0list[j]];
                if (t.id != t0.id) // unikaj poczenia wektora drukrotnie
                {
                    float angle = getangle (t.Nf, t0.Nf);
                    //cout << "angle0 "<< angle<< endl;
                    if (angle < creaseangle)
                    {
                     t.Nv0 = t.Nv0 + t0.Nf; 
                    }
                }
            }
            
            t.Nv0 = normv3 (t.Nv0);
            
            for (size_t k=0; k<t1list.size(); ++k)
            {
                Triangle t1 = trnlist[t1list[k]];
                if (t.id != t1.id) // unikaj poczenia wektora drukrotnie
                {
                    float angle = getangle (t.Nf, t1.Nf);
                    //cout << "angle2 "<< angle<< endl;
                    if (angle < creaseangle)
                    {
                     t.Nv1 = t.Nv1 + t1.Nf; 
                    }
                }
            }
            t.Nv1 = normv3 (t.Nv1);
            
            for (size_t l=0; l<t2list.size(); ++l)
            {
                Triangle t2 = trnlist[t2list[l]];
                if (t.id != t2.id) // unikaj poczenia wektora drukrotnie
                {
                    float angle = getangle (t.Nf, t2.Nf);
                    //cout << "angle2 "<< angle<< endl;
                    if (angle < creaseangle)
                    {
                     t.Nv2 = t.Nv2 + t2.Nf; 
                    }
                }
            }
            t.Nv2 = normv3 (t.Nv2);
            
            outtrnlist[i] = t;
        }
    return outtrnlist;    
    
}

pair <vector <Triangle>, vector < valarray <float> > >Trianglecalculator::idexnonrep (vector <Triangle> triangles) //usniecie powtarzajacych sie wiecholkow
{
    vector <Triangle> outtrn;  
    vector < valarray <float> > usedv; //uzyte wierzcholki, pozycja bedzie indeksem
    
    for(size_t i=0; i<triangles.size(); ++i)
    {    
        outtrn.push_back(triangles[i]); //wstepnie kopijemy zawartosc, indeksy beda podmieone pozniej
            
       /* int idx0 = triangles[i].idx0;
        int idx1 = triangles[i].idx1;
        int idx2 = triangles[i].idx2;*/
        valarray <float> v0 = triangles[i].v0; //pozycje wierzcholkow
        valarray <float> v1 = triangles[i].v1;
        valarray <float> v2 = triangles[i].v2;
        
        int idxtmp = -1;
        for(size_t j=0; j<usedv.size(); ++j)
            {
                valarray <float> vtmp = usedv[j];
                if ( (vtmp[0] == v0[0]) && (vtmp[1] == v0[1]) && (vtmp[2] == v0[2]) )
                {
                    idxtmp = j;
                }
            }
        if (idxtmp == -1)
        {
            idxtmp = usedv.size(); //indeks bedzie teraz id ostatniego elementu
            usedv.push_back(v0); //jesli takiego nie bylo to dodajemy go do listy  

        }
        outtrn[i].idx0 = idxtmp;
        
        idxtmp = -1;
        for(size_t j=0; j<usedv.size(); ++j)
            {
                valarray <float> vtmp = usedv[j];
                if ( (vtmp[0] == v1[0]) && (vtmp[1] == v1[1]) && (vtmp[2] == v1[2]) )
                {
                    idxtmp = j;
                }
            }
        if (idxtmp == -1)
        {
            idxtmp = usedv.size();
            usedv.push_back(v1); 

        }
        outtrn[i].idx1 = idxtmp;
        
        idxtmp = -1;
        for(size_t j=0; j<usedv.size(); ++j)
            {
                valarray <float> vtmp = usedv[j];
                if ( (vtmp[0] == v2[0]) && (vtmp[1] == v2[1]) && (vtmp[2] == v2[2]) )
                {
                    idxtmp = j;
                }
            }
        if (idxtmp == -1)
        {
            idxtmp = usedv.size();
            usedv.push_back(v2); 
        }
        outtrn[i].idx2 = idxtmp;
    
    }
    
    return make_pair(outtrn, usedv);
}

void Trianglecalculator::calcshape(vector< vector<float> > verts, vector< vector<int> > idxs, vector< vector<float> > colors, vector< vector<int> > coloridxs)
{
        /*Strategia:
     * 1 policz normalne scian
     * 2 usun powtarzajace sie indeksy (refacttrian)
     * 3 policz normalne wierzcholkow
     */ 

    cout << "IDX SIZE " <<idxs.size() << endl;
    for(size_t i=0; i<idxs.size(); ++i) //wyliczanie normalnych scian, itd.
    {
        cout << idxs[i][0] << " " << idxs[i][1] <<" "<< idxs[i][2] << endl; 
        triangles.push_back(calctriangle (verts, idxs, colors, coloridxs, i));

    }
    
    tie(refacttrian, refactv) = idexnonrep (triangles); //usuniecie nadmiarowych indeksow
    
    /*cout << "Refactored " << endl;
    for(size_t i=0; i<refacttrian.size(); ++i)
    {
        int idstr = refacttrian[i].id;
        cout << idstr << "\n";
        cout << refacttrian[i].idx0 << ", "<< refacttrian[i].idx1 << ", "<< refacttrian[i].idx2 << endl;
        valarray <float> refv0 = refacttrian[i].v0;
        valarray <float> refv1 = refacttrian[i].v1;
        valarray <float> refv2 = refacttrian[i].v2;
        cout << refv0[0] <<", " << refv0[1] <<", "<< refv0[2] << "\n" << refv1[0] <<", " << refv1[1] <<", "<< refv1[2] << "\n" << refv2[0] <<", " << refv2[1] <<", "<< refv2[2] << "\n";
    }*/
    
    refacttrian = calcvnormals (refacttrian); //wyliczenie normalnych wierzcholkow
    
}

vector <valarray <float> >Trianglecalculator::getvertslist() //pozycje wszyskich wierzcholkow
{
    vector <valarray <float> >vertices;
    
    for(size_t i=0; i<refacttrian.size(); ++i)
    {
        vertices.push_back(refacttrian[i].v0);
        vertices.push_back(refacttrian[i].v1);
        vertices.push_back(refacttrian[i].v2);
    }
    return vertices;
    
}

vector <valarray <float> >Trianglecalculator::getnormalslist() //normalne dls wszystkich wiercholkow
{
    vector <valarray <float> >normals;
    
    for(size_t i=0; i<refacttrian.size(); ++i)
    {
        normals.push_back(refacttrian[i].Nv0);
        normals.push_back(refacttrian[i].Nv1);
        normals.push_back(refacttrian[i].Nv2);
    }
    return normals;
    
}

vector <valarray <float> >Trianglecalculator::getcolorslist() //kolory wszystkich wiercholkow/trojkatow
{
    vector <valarray <float> >colors;
    
    for(size_t i=0; i<refacttrian.size(); ++i)
    {
        colors.push_back(refacttrian[i].fcolor); //wszystkie 3 wierzcholki maja ten sam kolor
        colors.push_back(refacttrian[i].fcolor);
        colors.push_back(refacttrian[i].fcolor);
    }
    return colors;
    
}

void Trianglecalculator::setdiffusecolor(vector <float> dc)
{
    diffusecolor = dc;
}

void Trianglecalculator::clearshape()
{
    triangles.clear();
    refacttrian.clear();
    refactv.clear();
    diffusecolor.clear();
}


Trianglecalculator::~Trianglecalculator()
{
    
}
