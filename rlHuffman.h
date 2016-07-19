map<string,pair<string,int> > huffCodeWord;
map<string,pair<string,int> >::iterator itHuffCodeWord;
map<string,pair<string,int> >huffDecoWord;
map<string,pair<string,int> >::iterator itHuffDecoWord;
int nBit=0;
//Alternative of huffman, used with codewords
//Special function using for RUNLENGTH + HUFFMAN
void printCodes2(struct MinHeapNode* root, string str)
{
    if (!root)
        return;
    if (root->chars[0]!= 'Û')//default delimiter 'Û'
    {
        nBit+=str.size()*(root->freq);
        huffCodeWord[root->chars].first = str;
    }
    printCodes2(root->left, str + "0");
    printCodes2(root->right, str + "1");
}
//Creates th Huffman tree for strings of codewords
void HuffmanAlgorithm2(string chars[], unsigned int freq[], short size)
{
    struct MinHeapNode *left, *right, *top;

    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

    for (short i = 0; i < size; ++i)
        minHeap.push(new MinHeapNode(chars[i], freq[i]));
    while (minHeap.size() != 1){
        left = minHeap.top();
        minHeap.pop();
        right = minHeap.top();
        minHeap.pop();
        top = new MinHeapNode("Û", left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    printCodes2(minHeap.top(), "");
}




/*
============================RUNLENGTH + HUFFMAN ENCODING=======================================
*/
void encodeRUN_HUFFMAN(){
	unsigned int numCodeWord=0;

        string codeWord="",pivote="-1",aux;
        char k;
        int cont=0,mayor=1;
        long nCodeWord=0;
        puts("Criando os codewords[repeticao_character + caracter]");
        for(int i=0 ; i < textoActual.size(); i++)
        {
            k = textoActual[i];

			if(pivote=="-1")
				pivote=k;
			if(k!=pivote[0] || k==-1)
			{
				aux=intToString(cont);
				codeWord+=aux+"|"+pivote+" ";
				nCodeWord++;
				if(huffCodeWord.count(aux+pivote)<=0){
					huffCodeWord[aux+pivote]=make_pair("",1);
					numCodeWord++;
				}
				else //obtain the frequencies of codewords
                    huffCodeWord[aux+pivote].second=huffCodeWord[aux+pivote].second+1;

				pivote=k;
				cont=1;
			}
			else
				cont++;
        }

        cout<<"Codewords totales: "<<numCodeWord<<endl;
        unsigned int frecuenciaCodeWord[3000];
		string codeWordHuffman[3000];
		int x=0;
       	for(itHuffCodeWord=huffCodeWord.begin(); itHuffCodeWord!=huffCodeWord.end(); ++itHuffCodeWord)
    	{
    		frecuenciaCodeWord[x]=itHuffCodeWord->second.second;
    		codeWordHuffman[x]=itHuffCodeWord->first;
    		x++;
		}
		string rleBin="";
		string encabezado="";
		nBit=0; // number of '0' bits added to create an initial codeword
		//GET HUFFMAN CODES FOR EACH codeword
		HuffmanAlgorithm2(codeWordHuffman,frecuenciaCodeWord,numCodeWord);
		cout<<"Codigos atribuidos a cada palavra de codigo de forma eficiente\n\n";

       	for(itHuffCodeWord=huffCodeWord.begin(); itHuffCodeWord!=huffCodeWord.end(); ++itHuffCodeWord)
  			encabezado+=itHuffCodeWord->first+"|"+intToString(itHuffCodeWord->second.second)+"|";

		 puts("representacao final dos codeword obtidos...\n");

		//Final representation of encoding file
		int asciiChar,contBits=0;
		string codeNum="",codeChar;

		nBit=SizeASCII-(nBit%SizeASCII);
		if(nBit==7)
			nBit=0;
		encabezado="C"+intToString(nBit)+" "+intToString(encabezado.size())+" "+encabezado;
        encabezados.push_back(encabezado);

		string textoAux="";
		x=0;
		int n=codeWord.length();

		for(int i=0;i<n;i++)
		{
			if(codeWord[i]=='|'&&codeWord[i+2]==' ')
			{
				codeChar=codeWord[i+1];
				rleBin+=huffCodeWord[codeNum+codeChar].first;
				if(i==n-3)
					for(int i=0;i<nBit;i++)
					{
						contBits++;
						rleBin+="0";
					}
				contBits+=(huffCodeWord[codeNum+codeChar].first).size();
				if(contBits>=SizeASCII)
				{
					int rep=contBits/SizeASCII;
					for(int l=0;l<rep;l++)
					{
						asciiChar = binToInt(rleBin.substr(x,SizeASCII));
						if(asciiChar < 32)
		       		    	asciiChar += 128;
                        textoAux += (char)asciiChar;
						x+=SizeASCII;
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
/* --------------------------------------RUNLENGHT + HUFFMAN DECODING-------------------------------------------*/
void decodeRlHuffman(string textRLHF)
{
    unsigned int frecuenciaCodeWord[3000];
	string codeWordHuffman[3000];
	string texto="$";
	texto += textRLHF;

        //Gets the value of n bits to be ignored and fixed minimum size format bits of each codeword
        int nBit,formato,tamEncabezado;
        int pos;
        string letras,n;
        if(texto[0]=='$'&&texto[1]=='C')
        {

            n=texto[2];
            nBit=atoi(n.c_str());
            pos=texto.find(" ",3);
            n=texto.substr(pos,texto.find(" ",pos+1)-pos);
            pos=texto.find(" ",pos+1);
            tamEncabezado=atoi(n.c_str());
        }
        //Obtains the codewords of each character
        letras=texto.substr(texto.find(" ",pos)+1,tamEncabezado);
        texto=texto.substr(texto.find(" ",pos)+tamEncabezado+1,texto.length()-(texto.find(" ",pos)+tamEncabezado)-2);

    	//Extract the codewords and it's frequency
        int x=0;
        pos=0;
        for(int i=0;i<letras.size();i++)
		{
			if(letras[i]=='|'&&letras[i+1]!='|')
			{
			    for(int j=i+1;j<letras.size();j++)
				{
					if(letras[j]=='|')
					{
						huffCodeWord[letras.substr(pos,i-pos)].second=atoi(letras.substr(i+1,j-i-1).c_str());
						frecuenciaCodeWord[x]=huffCodeWord[letras.substr(pos,i-pos)].second;
    					codeWordHuffman[x]=letras.substr(pos,i-pos);
						x++;
						//cout<<letras.substr(pos,i-pos)<<" -> "<<huffCodeWord[letras.substr(pos,i-pos)].second<<endl;
						pos=j+1;
						i=pos;
						break;
					}
				}
			}
		}
		cout<<"\nCodeWords obtenidos \n";
        //get the Huffman codes for each codeword
        HuffmanAlgorithm2(codeWordHuffman,frecuenciaCodeWord,x);

        for(itHuffCodeWord = huffCodeWord.begin() ; itHuffCodeWord != huffCodeWord.end(); itHuffCodeWord++)
		{
			int t=(itHuffCodeWord->first).size();
            huffDecoWord[itHuffCodeWord->second.first].first = (itHuffCodeWord->first).substr(t-1,1);
            huffDecoWord[itHuffCodeWord->second.first].second = atoi((itHuffCodeWord->first).substr(0,t-1).c_str());
//            cout<<huffDecoWord[itHuffCodeWord->second.first].second<<huffDecoWord[itHuffCodeWord->second.first].first<<" -> "<<itHuffCodeWord->second.first<<endl;
        }
		cout<<"\nCodigos Huffman obtidos\n";
        cout<<"\nDecodificando o texto...\n";

        //Converts text to binary using the codes for each codeword obtained in the header
        int codeNum,contBits=0,nChar,l;
        string bin,auxBin="",codeChar="";
        nChar=texto.length();
        pos=0;

        string temp = "";
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
				if(contBits>700||i==nChar-1)
					for(l=pos;l<bin.size();l++)
					{
						codeChar+=bin[l];
						if(huffDecoWord.find(codeChar) != huffDecoWord.end())
						{
							codeNum=huffDecoWord[codeChar].second;
							for(int k=0;k<codeNum;k++)
                                temp += huffDecoWord[codeChar].first.c_str()[0];

							pos=l+1;
							contBits-=codeChar.size();
							codeChar="";
						}
					}
				codeChar="";
            }
        }
        textoActual = temp;

}

