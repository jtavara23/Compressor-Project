/* --------------------------------------RUNLENGHT ENCODING-------------------------------------------*/
void encodeRUNLENGTH(){
    //[Runlegnth_Codeword] made of : times_character + character;
	map<string,string> runCodeWord;//storages [binary representation, Runlength_codeword ] viceversa
	map<string,string>::iterator itRunCodeWord; // iterator of map(runCodeWord)
	unsigned int numCodeWord=0;//different number of codewords

    string codeWord="",aux;
    int cont=0,mayor=1;

    long nCodeWord=0;//numbers of codewords
    string pivote;// pivot to storage a different character
    pivote += textoActual[0];
    char k;
    for(int i=0 ; i < textoActual.size(); i++)
    {
        k = textoActual[i];
        if(k!=pivote[0] || k==-1){
            aux=intToString(cont);
            codeWord+=aux+"|"+pivote+" ";
            nCodeWord++;
            if(runCodeWord.count(aux+pivote)<=0){
                runCodeWord[aux+pivote]="";
                numCodeWord++;
            }
            pivote=k;
            cont=1;
        }
        else
            cont++;
    }

    string rleBin="";
    //Header indicating the method applied, start bit pivot, fixed minimum number bits to represent a number
    string encabezado="";
    cout<<"terminou encontrar os  "<<numCodeWord<<" codewords diferentes..\n";

    //Calculates a minimum fixed number of bits to represent a codeword
    int formato=0,n=numCodeWord;
    while(n!=0)
    {
        n=n/2;
        formato++;
    }
    cout<<"Tem "<<nCodeWord<< " codewords que forman o texto.."<<endl;
    cout<<"Alocando os codigos para cada codeword..\n\n";
    //Assigning codeword to each character
    int cod=0;
    for(itRunCodeWord=runCodeWord.begin();itRunCodeWord!=runCodeWord.end();itRunCodeWord++)
    {
        itRunCodeWord->second=intToBin(cod,formato);
        encabezado+=itRunCodeWord->first+"|";
        cod++;
    }

    cout<<"Codigos atribuidos a cada palavra de codigo de forma eficiente\n\n";
    puts("representacao final dos codeword obtidos...\n");

    //Representation of final encoding
    int nBit=0,asciiChar,contBits=0;
    string codeNum="",codeChar;

    if((nCodeWord*formato)%SizeASCII>0)
        nBit=SizeASCII-((nCodeWord*formato)%SizeASCII);

    encabezado="R"+intToString(nBit)+" "+intToString(formato)+" "+intToString(encabezado.size())+" "+encabezado;
    puts("Header runlength criado");

    encabezados.push_back(encabezado);
    n=codeWord.length();
    string textoAux = "";
    for(int i=0;i<n;i++)
    {
        if(codeWord[i]=='|'&&codeWord[i+2]==' ')
        {
            codeChar=codeWord[i+1];
            rleBin+=runCodeWord[codeNum+codeChar];
            if(i==n-3)
                for(int i=0;i<nBit;i++)
                {
                    contBits++;
                    rleBin+="0";
                }
            contBits+=formato;
            if(contBits>=SizeASCII)
            {
                int rep=contBits/SizeASCII;
                for(int l=0;l<rep;l++)
                {
                    asciiChar = binToInt(rleBin.substr(0,SizeASCII));
                    if(asciiChar < 32)
                        asciiChar += 128;
                    textoAux += (char)asciiChar;
                    rleBin=rleBin.substr(SizeASCII,rleBin.size()-SizeASCII+1);
                }
                contBits-=(SizeASCII*rep);
            }
            codeNum="";
            i=i+3;
        }
        codeNum+=codeWord[i];
    }
    textoActual = textoAux;

}
/* --------------------------------------RUNLENGHT DECODING-------------------------------------------*/
void decodeRun(string pass)
{
	map<string,string> runCodeWord;
	map<string,string>::iterator itRunCodeWord;
	string texto="$";
	texto += pass;

        cout<<"\nLendo Runlength header....\n";
        //Gets the value of nbits to be ignored and the size of the fixed minimum bit format of each codeword
        int nBit,formato,tamEncabezado;
        int pos;
        string letras,n;
        if(texto[0]=='$'&&texto[1]=='R')
        {
            n=texto[2];
            nBit=atoi(n.c_str());
            pos=texto.find(" ",4);
            n=texto.substr(4,pos-4);
            formato=atoi(n.c_str());
            n=texto.substr(pos,texto.find(" ",pos+1)-pos);
            pos=texto.find(" ",pos+1);
            tamEncabezado=atoi(n.c_str());
        }

        cout<<"\nObtem os codigos de cada caracter....\n";
        letras=texto.substr(texto.find(" ",pos)+1,tamEncabezado);
        texto=texto.substr(texto.find(" ",pos)+tamEncabezado+1,texto.length()-(texto.find(" ",pos)+tamEncabezado)-2);
        int cod=0;
        pos=0;

        for(int i=0;i<letras.size();i++)
		{
			if(letras[i]=='|'&&letras[i+1]!='|')
			{
				runCodeWord[intToBin(cod,formato)]=letras.substr(pos,i-pos);
				cod++;
				pos=i+1;
			}
		}
		cout<<"\nCodigos obtidos\n";

        cout<<"\nDecodificando o texto...\n";
        //Converts text to binary codes using each letter obtained in the header
        int codeNum,contBits=0,nChar;
        string bin,auxBin="",codeChar;
        nChar=texto.length();
        string temporal = "";
        for(int i=0;i<nChar;i++)
		{
            int numero = (int)(texto[i]);
            if(numero < 0&&numero!=-1)
                numero += 128;
            if(numero!=-1)
            {
            	auxBin=getCodeWord(numero,SizeASCII,'r');
            	contBits+=SizeASCII;
            	if(i==nChar-1)
            	{
              		auxBin=auxBin.substr(0,SizeASCII-nBit);
					contBits-=nBit;
				}
            	bin+=auxBin;
				if(contBits>=formato)
				{
					int rep=contBits/formato;
					for(int l=0;l<rep;l++){
						codeChar=runCodeWord[bin.substr(0,formato)];
						codeNum=atoi(codeChar.substr(0,codeChar.size()-1).c_str());
						codeChar=codeChar.substr(codeChar.size()-1,1);

		                for(int k=0;k<codeNum;k++)
                            temporal += codeChar.c_str()[0];
						bin=bin.substr(formato,bin.size()-formato +1);
					}
					contBits-=(formato*rep);
				}
            }
        }
        textoActual = temporal;
}

