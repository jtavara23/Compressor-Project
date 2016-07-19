#include "definiciones.h"
#include <fstream>
vector<string> str_Actual;//storages derivations of a block
vector<string> rotations;//storages rotations of a block
vector<string>indexS;
vector<int>indexI;//


void bwt(string input,string &totalBwt) {

    int primaryidx;
    string bwt_str;
    rotations.clear();

//  1 Step: rotations
    for(int n=0;n<input.size();n++) {
        string s;
        s += input[n];
        for(int i=n+1;i!=n;i++) {
          if(i==input.size())
            i=0;
          s += input[i];
          if(i==input.size()-1)
            i=-1;
        }
        rotations.push_back(s);
    }

  //2 Step: sorting
  sort(rotations.begin(),rotations.end());
  /*
  for(size_t n=0;n<rotations.size();n++) {
    cout << rotations[n] << endl;
  }
    */
  for(size_t n=0;n<rotations.size();n++) {
    bwt_str += rotations[n].substr(rotations[n].size()-1,1);
    if(rotations[n] == input)
        primaryidx = n;
  }
//cout << "bwt string: " << bwt_str << endl;
//cout << "primaryidx: " << primaryidx << endl;

    indexS.push_back(intToString(primaryidx));
    totalBwt += bwt_str;//storages
}

string rbwt(string bwt_str,int primaryidx) {
    str_Actual.clear();
    for(size_t n=0;n<bwt_str.size();n++) {
        string s;
        s = bwt_str.substr(n,1);
        str_Actual.push_back(s);
    }
    for(;str_Actual[0].size() < bwt_str.size();) {
        vector<string> new_str = str_Actual;
        sort(new_str.begin(),new_str.end());
        for(size_t n=0;n<str_Actual.size();n++) {
          str_Actual[n] = str_Actual[n] + new_str[n].substr(new_str[n].size()-1,1);
        }
    }
    /*
      cout << "reversed transform:" << endl;
      for(size_t n=0;n<str_Actual.size();n++) {
        cout << str_Actual[n] << endl;
      }
    */
    sort(str_Actual.begin(),str_Actual.end());
    return str_Actual[primaryidx];
}


void encodeBWT(string text){

    textoActual = text;
    string s;
    char k;
    int cont = 1 ;
    string totalBwt;//text in bwt format
    puts("Trabalhando nos blocos..");
    for(int in=0 ; in < textoActual.size(); in++) {
        k = textoActual[in];
        s += k;
        if(cont == tamanhoBlocoTexto){
            cont=0;
            bwt(s,totalBwt);//calls the bwt method for a specific block
            s="";
        }
        cont++;
    }

    if(cont!=1)
        bwt(s,totalBwt);
    string encabezado = "B";//to recognize that the header is of BWT method
    //default delimiter ®
    encabezado += intToString(tamanhoBlocoTexto)+'®';
    for(int i = 0 ; i < indexS.size() ; i++)
        encabezado += indexS[i]+'®';
    encabezado += '®';
    //end of header
    puts("Header BWT criado..");
    encabezados.push_back(encabezado);
    textoActual = totalBwt;
}

void decodeBWT(string texto){

    string s;
    char c;
    int primaryidx;
    int tamanhoBlocoTexto;
    int index = 0;
    c = texto[index++];
    while(c!='®'){
        s += c;
        c = texto[index++];
    }
    tamanhoBlocoTexto = atoi(s.c_str());//get the size of blocks
    s = "";
    bool End = false;
    string temporal = "";
    puts("Decoding os blocos do BWT..");
    while( index <texto.size()){
        c = texto[index++];
        if(c == '®'){//default delimiter ®
            int cnt=0;
            while(index <texto.size()){
                for(int i = 0 ; i<tamanhoBlocoTexto ; i++){
                    if(index == texto.size())
                        {End =true ;break;}
                    c = texto[index++];
                    s += c;
                }
                if(!End || !s.empty()){
                    temporal += rbwt(s,indexI[cnt++]);
                }
                s = "";
            }
        }
        else{
            while(c!='®'){
                s += c;
                c = texto[index++];
            }
            primaryidx = atoi(s.c_str());
            indexI.push_back(primaryidx);
            s = "";
        }
    }
    textoActual = temporal;
}
