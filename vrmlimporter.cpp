#include "vrmlimporter.h"


Vrmlimporter::Vrmlimporter()
{
    scontent =""; //zawartosc pliku vrml
    shapescount = 0;
    
    //vector<vector< vector<float> > >shapeverts;
   // vector< vector< vector<int> > >shapeidxs;

}

int Vrmlimporter::importvrml(string name)
{
    string line;
    string shape, block, idxblock, dfblock;
    vector<string> rows;
    vector< vector<float> > floats2D; //wspolrzedne wiercholkow
    vector< vector<int> > coordidx; //indeksy wiercholkow
    vector< vector<float> > colors2D; //kolory
    vector< vector<int> > coloridx; //indeksy kolorow, dwuwymiary wektor, jesli byl by uzywany kolor na wiercholek
    long unsigned int scpos = 0;
    
    vector<float> floats1D;
    vector<int> ints1D;
    vector<float> defaultc;
    
    ifstream file( name );
    while (getline( file, line ))   
    {
        scontent += line + "\n"; //sklej wszystkie linie z powrotem
    }
    
    if (scontent == "") 
    {
        cout << "Brak pliku lub plik pusty" << endl;
        return 1; //jezeli nic nie wczytal nie ma sensu kontynuowac
    }
    
    string::iterator new_end = unique(scontent.begin(), scontent.end(), botharespaces); //usniecie podwojnych spacji
    scontent.erase(new_end, scontent.end());  
    
    while (scpos < scontent.size())
    {
        shape = extractblock ("Shape {", scontent, scpos);
        scpos = scpos + shape.size();
        
        idxblock = extractblock ("IndexedFaceSet {", shape, 0); //blok z indeksowanymi wspolrzednymi i kolorami
        if (idxblock != "NF") //NF gdy nie znaleziono danego bloku np Shape zwiera tylko IndexedLineSet
        {
            cout << "IFS znaleziony" << endl;
            //cout << idxblock << endl;
            
            //TODO znajdz coord DEF i zapis razem z Coordinate w tablicy/skrutkturze
            adddefblocks(shape);
            
            block = extractblock ("Coordinate {",  idxblock, 0); //coordIndex jest poziom wyżej niż point
            //cout << block << endl;
            if (block == "NF")
            {
                block = finddefblock(idxblock); //znajdz coord def wsrod wczesniej zdefiniowanych
            }
            cout <<"wczyctuje koordynaty" << block << endl;
            rows =  splitblock (block, "point [", "]", ","); 
            

            floats2D.clear();//w tym miejscu wektor musi byc pusty! 
            for(size_t i=0; i<rows.size(); ++i) //zrobic vector 2D
            {
                floats1D.clear();
                floats1D = parsefloats (rows[i]);
                floats2D.push_back(floats1D);
            }

            rows =  splitblock (idxblock, "coordIndex [", "]", "-1"); 
            coordidx.clear();//w tym miejscu wektor musi byc pusty! 
            cout << "ROWSSIZE " << rows.size() << endl;
            for(size_t i=0; i<rows.size(); ++i) //zrobic vector 2D
            {
                string str = rows[i];
                cout << str << "lp: " <<i << endl;
                str.erase( remove( str.begin(), str.end(), ','), str.end() ) ;
                ints1D.clear();
                ints1D = parseints (str);
                if (ints1D.size() > 0)
                    coordidx.push_back(ints1D);
            }
            block = extractblock ("Color {",  idxblock, 0); //kolory indeksowane
            if (block != "NF")
            {
                rows = splitblock (block, "color [", "]", ","); 
                colors2D.clear(); 
                for(size_t i=0; i<rows.size(); ++i) //zrobic vector 2D
                {
                    floats1D.clear();
                    floats1D = parsefloats (rows[i]);
                    colors2D.push_back(floats1D);
                }
                
                rows =  splitblock (idxblock, "colorIndex [", "]", ","); 
                coloridx.clear(); 
                for(size_t i=0; i<rows.size(); ++i) //zrobic vector 2D
                {
                    string str = rows[i];
                    ints1D.clear();
                    ints1D = parseints (str);
                    coloridx.push_back(ints1D);
                }
                shapecolors.push_back(colors2D);
                shapecoloridxs.push_back(coloridx);
            }
            
            block = extractblock ("Material {",  shape, 0); //material, kolor na bryle... czy kazda bryla ma diffuse color?
            if (block != "NF")
            {
                rows = splitblock (block, "diffuseColor ", "\n", "\n"); 
                diffusecolors.push_back(parsefloats (rows[0]));
                //cout << "punktkontrolny" << rows[0] << endl;

            }
            else //jesli nie ma diffusecolor to kolor domyslny
            {
                defaultc.clear();
                defaultc = {0.5, 0.5, 0.5};
                diffusecolors.push_back(defaultc);
            }
            
            if (shapecoloridxs.size() == 0)
            {
                //gdy nie ma niczego wstaw domyslny kolor
                colors2D.clear(); 
                floats1D.clear();
                floats1D = {1.0, 1.0, 1.0};
                colors2D.push_back(floats1D);
                ints1D.clear();
                ints1D = {0, 0, 0};
                coloridx.clear(); 
                coloridx.push_back(ints1D);
                shapecoloridxs.push_back(coloridx);
                shapecolors.push_back(colors2D);
                
            }
            
            shapeverts.push_back(floats2D);
            shapeidxs.push_back(coordidx);
            shapescount++;
        }
    
    }
    
    return 0;
}

string Vrmlimporter::extractblock (string keyword, string s, long unsigned int scpos) //wyciagniecie wszytkiego miedzy para {} 
{
    auto pos0 = s.find(keyword, scpos);
    string sprt = "";
    string subopenclose = ""; //fragment miedzy ostatnim otwiracym a zamykajcym
    unsigned long int openbr, closebr; 
    unsigned long int openpos, closepos;
    unsigned long int openpossub; //osobny licznik pozycji na podstring
    if (pos0!=string::npos)
    {
        openbr = 0;
        closebr = 0;
        openpos = s.find("{", pos0);
        closepos = openpos + 1;
        if (openpos!=string::npos) 
        {
            openbr++;
            while (openbr != closebr)
            {
                //sleep(2);
                cout << "op " << openbr << "cl " << closebr << endl;
                closepos = s.find("}", closepos + 1);
                if (closepos!=string::npos)
                {
                    closebr++;
                    subopenclose = s.substr(openpos, closepos-openpos + 1);
                    openpossub = subopenclose.find("{");
                    
                    while (openpossub!=string::npos)
                    {
                        openpossub = subopenclose.find("{", openpossub + 1);
                        openbr++;
                    }
                    openpos = openpos + subopenclose.size();
                    
                }

            }
        }
    }
    else
    {
            return "NF"; 
    }
    scpos = closepos;
    sprt = s.substr(pos0, scpos - pos0 + 1); //wycinek, 
    return sprt;

}

vector<string> Vrmlimporter::splitblock (string block, string startdel, string enddel, string del) 
{
/*dzieli str na mniejsze fragmeny np blabla [0 0 0,
 * 0 0 0] 
 * na 
 * 0 0 0 
 * 0 0 0
 */
    vector<string> vs;

    string delimiter = startdel; //np "["
    block.erase(0, block.find(delimiter)+delimiter.length());
    delimiter = enddel; //np "]"
    block.erase(block.find(delimiter),block.length());
    delimiter = del; // podzial linii np ","
    
    string::iterator new_end = unique(block.begin(), block.end(), botharespaces);
    block.erase(new_end, block.end());   
        
    size_t pos = 0;
    string token;
    while ((pos = block.find(delimiter)) != string::npos) {
        token = block.substr(0, pos);
        vs.push_back (token);
        block.erase(0, pos + delimiter.length());
    }
    if  ((block != " ")) vs.push_back (block); //czasami elemnt dzielacy jest na koncu i zostaje tylko po nim bezuzyteczna spacja
  
  return vs;

}

void Vrmlimporter::adddefblocks (string s)//dla wyciagniecia zdefiniowanych blokow w stylu coord DEF FaceC Coordinate { DEF
{
    string kw = "coord DEF";
    auto pos0 = s.find(kw, 0);
    vector <string> defb; //dwuelementowy wektor, pierwszy jako indentyfikato, drugi jako tresc
    while (pos0 != string::npos)
    { 
        pos0 = s.find(" ", pos0 + kw.length());

        auto pos1 = s.find(" ", pos0 + 1);
        string sd0 = s.substr(pos0 + 1, pos1 - pos0 - 1);


        kw = "point";
        auto pos2 = s.find (kw, pos1);
        kw = "]";
        auto pos3 = s.find (kw, pos2)+1;
        string sd1 = s.substr(pos2, pos3-pos2);
        
        defb.clear();
        defb.push_back(sd0);
        defb.push_back(sd1);
        defblocks.push_back(defb);
        
        cout << "koordynaty " << sd0 << endl;
        cout << sd1 << endl;
        kw = "coord DEF";
        pos0 = s.find(kw, pos3);
    }
}

string Vrmlimporter::finddefblock (string s)
{
    string kw = "coord USE";
    auto pos0 = s.find(kw, 0);
    if(pos0 != string::npos)
    {
        pos0 = s.find(" ", pos0 + kw.length());
        auto pos1 = s.find(" ", pos0 + 1);
        string sd0 = s.substr(pos0 + 1, pos1 - pos0 - 1);
        
        cout << "kw uzycia " << sd0 << endl;
        
        for (size_t i=0; i<defblocks.size(); ++i)
        {
            if (defblocks[i][0] == sd0)
            {
                //cout << defblocks[i][1] << endl;
                return defblocks[i][1];
            }

        }
    }
    return "NF";
}

bool botharespaces (char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); } //usuniecie wielu spacji

vector<float> Vrmlimporter::parsefloats (string block)
{

  vector<float> v;

  istringstream iss(block);

  copy(istream_iterator<float>(iss),
        istream_iterator<float>(),
        back_inserter(v));
  
  return v;

}

vector<int> Vrmlimporter::parseints (string block)
{

  vector<int> v;
  istringstream iss(block);

  copy(istream_iterator<int>(iss),
        istream_iterator<int>(),
        back_inserter(v));
  
  return v;

}

int Vrmlimporter::getshapescount()
{
    return shapescount;
}

vector< vector<float> > Vrmlimporter::getshapeverts(int id)
{
    return shapeverts[id];
}
vector< vector<int> > Vrmlimporter::getshapeidxs(int id)
{
    return shapeidxs[id];
}

vector< vector<float> > Vrmlimporter::getshapecolors(int id)
{

    return shapecolors[id];

}
vector< vector<int> > Vrmlimporter::getshapecoloridxs(int id)
{
    return shapecoloridxs[id];
}
vector<float> Vrmlimporter::getdiffusecolor(int id)
{
    return diffusecolors[id];    
}
Vrmlimporter::~Vrmlimporter()
{
    
}
