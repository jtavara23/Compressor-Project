using namespace std;
FILE *Input, *Output;//input and output file
string arquivo_Entrada="";//name of input file
string arquivo_Saida="";//name of output file
int tamanhoBlocoTexto;//the size of block in BWT method
string textoActual;// the most important string , that is updated after a method is executed(works as input and output string)
vector<string>encabezados;//the headers of each method
#define SizeASCII 7

/*============================CONVERSION FUNCTIONS============================================*/
//converts a int number ,to a [string] of binary code
string getCodeWord(int number,int Size, char type){

    string output;
    for (int i = 0; i < CHAR_BIT; ++i)
        output += ((number >> i) & 1)+48;
    reverse(output.begin(),output.end());
    if(type == 'h')
        return output.substr(Size);
    if(type == 'r')
        return output.substr(CHAR_BIT - Size);

}

//converts a string binary code, to an integer number
int binToInt(string x){
    int idx = 0;int sum = 0; int factor;
    for (int i = x.size()-1; i>= 0; i--) {
        factor = 1 << idx;
        if (x[i] == '1') {
            sum += factor;
        }
        idx++;
    }

    return sum;
}

//Parse an integer to string
string intToString(int num){
    stringstream strstream;
    string cadNum;
    strstream << num;
    strstream >> cadNum;
	return cadNum;
}

//returns the binary representation of an integer
string intToBin(int n,int formato){
	char numBin[8];
	int residuo,k=formato-1;
	for(int i=0;i<formato;i++)
		numBin[i]='0';
	numBin[formato]='\0';
	while(n!=0)
	{
		residuo=n%2;
		n=n/2;
		numBin[k]=(char)(residuo+48);
		k--;
	}
	return (string)numBin;
}

/*==================INIT TABELA ASCII EXTENDS==========================*/
map<char,pair<string,int> >mapCaracter;
map<char,pair<string,int> >::iterator itMapCaracter;
map<int,pair<char,string> >mapAscii;
map<int,pair<char,string> >::iterator itMapAscii;
map<int,string>mapNumBin;
map<int,string>::iterator itMapNumBin;

void initMapsAscii()
{
	char c;
	for(int i=32;i<255;i++)
	{
		c=char(i);
		mapCaracter[c]=make_pair(getCodeWord(i,8,'r'),i);
		mapAscii[i]=make_pair(c,getCodeWord(i,8,'r'));
	}
	for(int i=1;i<32;i++)
	{
		mapNumBin[c]=getCodeWord(i,5,'r');

	}
}

