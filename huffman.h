
map<char,string>huffmanCoding;//table of character with its respective binary format
map<string,char>huffmanDecoding;//table binary format with its character associated to
map<char,string>::iterator itHuffman;
map<string,char>::iterator itDeHuffman;
char letters[256];//array of letters(alphabet)
unsigned int realFrecuency[256];//frequency of each character
unsigned int numLetters = 0;//size of alphabet
/*
============================HUFFMAN CODING=======================================
*/
// A Huffman tree
struct MinHeapNode
{
    char charc;               // One of the input characters
    string chars;             //used for combined methods (RUNLENGTH-HUFFMAN)
    unsigned int freq;        // Frequency of the character
    MinHeapNode *left, *right;// Left and right node

    MinHeapNode(char charc, unsigned int freq)
    {
        left = right = NULL;
        this->charc = charc;
        this->freq = freq;
    }
    MinHeapNode(string chars, unsigned int freq)
    {
        left = right = NULL;
        this->chars = chars;
        this->freq = freq;
    }
};


struct compare
{
    bool operator()(MinHeapNode* l, MinHeapNode* r)
    {
        return (l->freq > r->freq);//queue ordered from less to greater
    }
};


void printCodes(struct MinHeapNode* root, string str)
{
    if (!root)
        return;

    if (root->charc != 'Û')//default delimiter 'Û'
       {
           huffmanCoding[root->charc] = str;
       }

    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}
void HuffmanAlgorithm(char charc[], unsigned int freq[], short size)
{
    struct MinHeapNode *left, *right, *top;

    // Create a min heap & inserts all characters of charc[]
    priority_queue<MinHeapNode*, vector<MinHeapNode*>, compare> minHeap;

    for (short i = 0; i < size; ++i)
        minHeap.push(new MinHeapNode(charc[i], freq[i]));

    while (minHeap.size() != 1)
    {
        // Extract the two minimum freq items from min heap
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        // Create a new internal node with frequency equal to the
        // sum of the two nodes frequencies. Make the two extracted
        // node as left and right children of this new node.
        top = new MinHeapNode('Û', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
        //Add this node to the min heap
        // 'Û' is a special value for internal nodes, not used
    }
    //creates the huffman codes
    printCodes(minHeap.top(), "");
}
void HuffmanCoding(string message)//generates the Output file
{
    string encabezado;
    if(message == "encode"){
            // ENCODE a OUTPUT file
            puts("Criando o Huffman header ..");
            encabezado = "H";
            encabezado += intToString(numLetters) + "|";
            for(int i = 0 ; i<numLetters ; i++){
                encabezado += (letters[i] + intToString(realFrecuency[i]) + "|");
            }
            encabezados.push_back(encabezado);
            string text = "",textoAux;
            char k;
            //read the text file
            for(int i=0 ; i < textoActual.size(); i++){
                k = textoActual[i];
                text += huffmanCoding[k];
            }
            //the quantity of bits that left over after the block division
            int division =text.size()%SizeASCII;
            if( division !=0 ){
                division = (SizeASCII-division);
                textoAux += (char)division;
                while(division--)
                    text.insert(text.begin(),'0');
            }
            else
                textoAux += (char)division;
             puts("Codificando o arquivo de saida..");
            //divides in blocks of the SizeASCII (7bits)
            string block;
            for(int i = 0 ; i < text.size(); ){
                block = "";
                for(int j = 0 ; j < SizeASCII ; j++){
                    block += text[i++];
                }

                int num = binToInt(block);
                if(num < 32){//if it's a not printable character
                    num+=128;

                }
                textoAux += (char)num;
            }
            textoActual = textoAux;
        }
        else{
            //storage the decoded message
            textoActual = message;
        }
        huffmanCoding.clear();//clear the huffman Table
}
void encodeHUFFMAN(){

    //creates a frecuency table
    unsigned int frecuency[256] ={0};
    char k;

    for(int i = 0 ; i< textoActual.size() ; i++)
    {
        k = textoActual[i];
        frecuency[k]++;
    }
    //creates an array with the frequency of each character
    for(int i = 0 ; i<256 ; i++)
        if(frecuency[i]>0){
           realFrecuency[numLetters]=frecuency[i];
           letters[numLetters] = char(i);
           numLetters++;
        }
    /*for(int i = 0 ; i<c ; i++){
        printf("%c %d\n",letters[i],realFrecuency[i]);
    }*/
    //create an encode file (header+encoded_text)
    HuffmanAlgorithm(letters,realFrecuency,numLetters);
    //write an encode file
    HuffmanCoding("encode");

}
/* --------------------------------------HUFFMAN DECODING-------------------------------------------*/
void decodeHuffman(string decodeMessage){

        int i=0,aux;
        char k;
        string text="";
        string frec="";
        while(decodeMessage[i]!='|')//default delimiter '|' of the Huffman header
            frec += decodeMessage[i++];
        i++;
        numLetters = atoi(frec.c_str());
        char value;
        for(int counter = 0,c=0 ; counter < numLetters ; counter++, i++, c++){
            value = decodeMessage[i++];
            letters[c] = value;//storages each character of the text
            frec = "";
            while(decodeMessage[i]!='|')
                frec += decodeMessage[i++];//storages the frequency of each character
            realFrecuency[c] = atoi(frec.c_str());
        }
        //creates the huffman tree
        puts("Criando o arvore de Huffman..");
        HuffmanAlgorithm(letters,realFrecuency,numLetters);
        //
        for(itHuffman = huffmanCoding.begin() ; itHuffman != huffmanCoding.end(); itHuffman++){
               //printf("%c %s\n",itHuffman->first,itHuffman->second.c_str());
                huffmanDecoding[itHuffman->second] = itHuffman->first;
            }
        //the quantity of zeros added
        puts("Decodificando o arquivo..");
        int division = (int)decodeMessage[i++];
        bool first =true;
        for(int j = i ; j < decodeMessage.size();){
            aux = (int)decodeMessage[j++];
            if(aux ==-1) break;
            if(aux < 0){
                aux = aux +128;
            }
            if(first){
                text += getCodeWord(aux,division+1,'h');
                division = 1;
                first =false;
            }
            else {
                text += getCodeWord(aux,division,'h');
            }
        }
        string code = decodeMessage = "";
        puts("Criando o arquivo de saida decodificado..");
        for(int j = 0 ; j < text.size(); j++){
            code += text[j];
            if(huffmanDecoding.find(code) != huffmanDecoding.end()){
                decodeMessage+=huffmanDecoding[code];
                code = "";
            }
        }
        //decode message
        HuffmanCoding(decodeMessage);
}


