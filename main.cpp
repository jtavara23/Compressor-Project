#include <bits/stdc++.h>
#include "bwt.h"
#include "runLength.h"
#include "huffman.h"
#include "rlHuffman.h"

string input;
string token;
vector<string>vector_tokens;//vector com todas as palavras na entrada

/*==============================================================*/
// validate input compression methods
bool validate_routines(string routine){
    istringstream separator(routine);
    getline(separator, token, '=');
    getline(separator, token, '=');
    if(token=="true"||token=="1")
       return true;
    return false;
}
//get size of BWT coding block
int getSizeBlock(string tam){
    istringstream separator(tam);
    getline(separator, token, '=');
    getline(separator, token, '=');
    return atoi(token.c_str());
}

/*=============================MAIN PROGRAM==============================*/
int main(int argc,  char** argv){
    string aux,bwt,huffman,runLength;
    int tam ;
    bool erro = false;
    for(int v = 1 ; v < argc; v++){
        tam = strlen(argv[v]);
        aux.assign(argv[v],tam);
        vector_tokens.push_back(aux);
    }
    if(vector_tokens[0] == "encode"){
        for(int i = 1; i < vector_tokens.size(); i++){
            if(vector_tokens[i] == "-i")
                arquivo_Entrada = vector_tokens[++i];
            else if(vector_tokens[i] == "-o")
                arquivo_Saida = vector_tokens[++i];
            else if(vector_tokens[i].substr(0,6)=="--bwt=")
                bwt = vector_tokens[i];
            else if(vector_tokens[i].substr(0,10)=="--huffman=")
                huffman = vector_tokens[i];
            else if(vector_tokens[i].substr(0,7)=="--runl=")
                runLength = vector_tokens[i];
            else if(vector_tokens[i].substr(0,10)=="--txtblck=")
                tamanhoBlocoTexto = getSizeBlock(vector_tokens[i]);
            else{
                cout<<"ERRO:"<<vector_tokens[i]<<endl;
                erro = true;break;
            }
        }
        if(erro)
        {
            printf("Erro, entrada esta incorreta!, tente de novo\n");
            return 0;
        }
         else{
            if((Input=fopen(arquivo_Entrada.c_str(),"rt")) ==NULL)
                puts("\n the file can't be open");
            else{
                Output=fopen(arquivo_Saida.c_str(),"wb");
                char k;
                while(!feof(Input))
                {
                    k = fgetc(Input);
                    textoActual += k;
                }
                fclose(Input);

                if(validate_routines(bwt)){
                    char aux=textoActual[textoActual.size()-1];
                    encodeBWT(textoActual.substr(0,textoActual.size()-1));
                    if(validate_routines(huffman) && validate_routines(runLength))
                        textoActual+=aux, encodeRUN_HUFFMAN();
                    else if(validate_routines(huffman))
                        encodeHUFFMAN();
                    else if(validate_routines(runLength))
                        textoActual+=aux, encodeRUNLENGTH();
                }
                else if(validate_routines(huffman) && validate_routines(runLength)){
                     encodeRUN_HUFFMAN();
                }
                else if(validate_routines(huffman))
                    textoActual.erase(textoActual.end()-1), encodeHUFFMAN();
                else if(validate_routines(runLength)){
                    encodeRUNLENGTH();
                }
                fprintf(Output,"%d",encabezados.size());
                for(int i=encabezados.size()-1 ; i >=0; i--)
                    fprintf(Output,"%d$",encabezados[i].size());
                for(int i=encabezados.size()-1 ; i >=0; i--)
                    fprintf(Output,"%s",encabezados[i].c_str());
                fprintf(Output,"%s",textoActual.c_str());
                fclose(Output);
                encabezados.clear();
                cout<<"Compressao terminada\n";
            }
        }
    }
    //---------------------DECODING-----------------------
    else if(vector_tokens[0] == "decode"){
            int i = 1;
        while(i<4){
            if(vector_tokens[i] == "-i")
                  arquivo_Entrada = vector_tokens[i+1];
            else if(vector_tokens[i] == "-i")
                arquivo_Entrada = vector_tokens[i+1];
            else if(vector_tokens[i] == "-o")
                arquivo_Saida = vector_tokens[i+1];
            else if(vector_tokens[i] == "-o")
                arquivo_Saida = vector_tokens[i+1];
            else
                {erro =true; break;}
            i+=2;
        }
        if(!erro)
        {
            vector<int>sizeOfEncabezados;
            if((Input=fopen(arquivo_Entrada.c_str(),"rt")) ==NULL)
                puts("\n the file can't be open");
            else{
                Output=fopen(arquivo_Saida.c_str(),"wb");
                char k;
                textoActual = "";
                while(!feof(Input))
                {
                    k = fgetc(Input);
                    textoActual += k;//read an storage the input file a string
                }
                fclose(Input);

                int j = 0,tam;
                int nroEncabezados = (int)textoActual[j++]-48;//number of headers

                for(int i = 0 ; i < nroEncabezados ; i++){
                    k = textoActual[j++];
                    string s;
                    while(k!='$'){
                        s += k;
                        k =textoActual[j++];
                    }
                    tam = atoi(s.c_str());
                    sizeOfEncabezados.push_back(tam);//storage the size of each header
                }

                string aux;
                for(int i = 0 ; i < nroEncabezados ; i++){
                    aux = "";
                    for(int z = 0;z <sizeOfEncabezados[i]; z++){
                        aux += textoActual[j++];
                    }
                    encabezados.push_back(aux);//storage the content of each header
                }
                aux = "";
                //getting the encoded text
                for(;j<textoActual.size(); j++)
                    aux += textoActual[j];
                textoActual = aux;

                //reading the headers
                for(int i = 0 ; i < nroEncabezados ; i++){
                    char definer = encabezados[i][0];

                    if(definer == 'B'){//it's a BWT header
                        if(i==0)
                            textoActual.erase(textoActual.end()-1);
                        decodeBWT(encabezados[i].substr(1)+textoActual);
                    }
                    else if(definer == 'H'){//it's a huffman header
                        if(i==0)
                            textoActual.erase(textoActual.end()-1);
                        decodeHuffman(encabezados[i].substr(1)+textoActual);
                    }
                    else if(definer == 'R')//it's a Runlength header
                        decodeRun(encabezados[i]+textoActual);
                    else if(definer == 'C')//it's a Runlength_huffman header
                        decodeRlHuffman(encabezados[i]+textoActual);
                }
                fprintf(Output,"%s",textoActual.c_str());
                cout<<"Decompressao terminada\n";
            }
        }

        else{
            printf("Erro, entrada esta incorreta!, tente de novo\n");
            return 0;
            }
    }
    else{
            printf("Erro, entrada esta incorreta!, tente de novo\n");
            return 0;
    }
    printf("\n-->Arquivo de Entrada %s\n",arquivo_Entrada.c_str());
    printf("-->Arquivo de Saida %s\n",arquivo_Saida.c_str());
    printf("-->tamanho bloco BWT %d\n",tamanhoBlocoTexto);
    vector_tokens.clear();

    return 0;
}
