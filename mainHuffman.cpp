#include "mainHuffman.hpp"

using namespace std;

void mainHuffman::createArr(){
    for(int i=0;i<128;i++){
        arr.push_back(new Node());
        arr[i]->data=(char)i;
        arr[i]->freq=0;
    }
}

void mainHuffman::createMinHeap(){

    cout<<"creating minHeap"<<endl;

    inFile.open(inFileName,ios::in);
    char id;
    inFile.get(id);
    while(!inFile.eof()){
        arr[id]->freq++;
        inFile.get(id);
    }
    inFile.close();
    for(int i=0;i<128;i++){
        if(arr[i]->freq>0){
            minHeap.push(arr[i]);
        }
    }

    cout<<"minHeap Created"<<endl;
}

void mainHuffman::createTree(){

    cout<<"creating Tree"<<endl;

    priority_queue<Node* ,vector<Node*> ,Compare> tempPQ(minHeap);
    Node *left,*right;

    while(tempPQ.size()>1){
        left=tempPQ.top();
        tempPQ.pop();

        right=tempPQ.top();
        tempPQ.pop();

        root=new Node();
        root->freq = left->freq + right->freq;
        root->left=left;
        root->right=right;

        tempPQ.push(root);

    }

    cout<<"Tree Created"<<endl;
}

void mainHuffman::traverse(Node *r,string str){
    if(r->left==NULL and r->right==NULL){
        r->code=str;
        return;
    }
    traverse(r->left,str+'0');
    traverse(r->right,str+'1');
}

void mainHuffman::createCodes(){
    cout<<"creating codes"<<endl;
    traverse(root,"");
    cout<<"codes created"<<endl;
}


int mainHuffman::binToDec(string str){
    int res=0;
    for(auto i:str){
        res=res*2 + i-'0';
    }
    return res;
}

void mainHuffman::saveEncodedFile(){
    cout<<"Saving Encoded File"<<endl;
    inFile.open(inFileName,ios::in);
    outFile.open(outFileName,ios::out | ios::binary);
    string in="";
    string s="";    
    char id;

    cout<<"minHeap started"<<endl;

    in+=(char)minHeap.size();

    priority_queue<Node* ,vector<Node*>,Compare> tempPQ(minHeap);

    while(!tempPQ.empty()){
        Node *curr=tempPQ.top();
        in+=curr->data;
        int len=curr->code.size();
        // cout<<len<<endl;
        in+=(char)len;
        int size=len%8 ? len/8+1 : len/8;
        s.assign(size*8-len,'0');
        s+=curr->code;

        while(s.size()>0){
            in+=(char)binToDec(s.substr(0,8));
            s=s.substr(8);
        }
        tempPQ.pop();

    }
    
    s.clear();

    cout<<"minHeap ended"<<endl;

    cout<<"File started"<<endl;

    inFile.get(id);
    while(!inFile.eof()){
        s+=arr[id]->code;
        while(s.size()>8){
            in+=(char)binToDec(s.substr(0,8));
            s=s.substr(8);
        }
        inFile.get(id);        
    }

    
    int count=8-s.size();
    s.append(count,'0');

    cout<<"File ended"<<endl;
    
    in+=(char)binToDec(s.substr(0,8));
    in+=(char)count;

    outFile.write(in.c_str(),in.size());
    inFile.close();
    outFile.close();
    
}


string mainHuffman::decToBin(int inNum){
    string temp = "", res = "";
    while (inNum > 0) {
        temp += (inNum % 2 + '0');
        inNum /= 2;
    }
    res.append(8 - temp.length(), '0');
    for (int i = temp.length() - 1; i >= 0; i--) {
        res += temp[i];
    }
    return res;
}

void mainHuffman::buildTree(char data,string code){

    Node *temp=root;
    for(int i=0;i<code.size();i++){
        if(code[i]=='0'){
            if(temp->left==NULL){
                temp->left=new Node();
            }
            temp=temp->left;
        }
        else if(code[i]=='1'){
            if(temp->right==NULL){
                temp->right=new Node();
            }
            temp=temp->right;
        }
    }
    temp->data=data;
    temp->code=code;
}

void mainHuffman::getTree(){

    cout<<"creating tree"<<endl;

    inFile.open(inFileName,ios::in | ios::binary);

    unsigned char pq_size;
    inFile.read(reinterpret_cast<char*>(&pq_size),1);
    remove++;

    root=new Node();

    for(int i=0;i<pq_size;i++){
        char aCode,aLen;
        
        inFile.read(&aCode,1);
        remove++;
        inFile.read(&aLen,1);
        remove++;
        int len=(int)aLen;
        
        int size=len%8 ? len/8+1 : len/8;

        unsigned char hCodeC[size];
        inFile.read(reinterpret_cast<char*>(hCodeC),size);
        remove+=size;

        string codeString="";
        for(int i=0;i<size;i++){
            codeString+=decToBin(hCodeC[i]);
        }
        codeString=codeString.substr(size*8-len);
        buildTree(aCode,codeString);    

    }
    inFile.close();

    cout<<"tree created"<<endl;
}

void mainHuffman::saveDecodedFile(){
    cout<<"saving decoded file"<<endl;

    inFile.open(inFileName,ios::in | ios::binary);
    outFile.open(outFileName,ios::out);

    inFile.seekg(-1,ios::end);
    char endCnt0;

    inFile.read(&endCnt0,1);

    inFile.seekg(remove,ios::beg);

    vector<unsigned char> text;
    unsigned char textseg;
    inFile.read(reinterpret_cast<char*>(&textseg),1);
    while(!inFile.eof()){
        text.push_back(textseg);
        inFile.read(reinterpret_cast<char*>(&textseg),1);
    }

    Node *curr=root;
    string path;
    for(int i=0;i<text.size()-1;i++){
        
        path=decToBin(text[i]);

        if(i==text.size()-2){
            path=path.substr(0,8-endCnt0);
        }

        for(int j=0;j<8;j++){
            if(path[j]=='0'){
                curr=curr->left;
            }
            else{
                curr=curr->right;
            }
            if (curr->left == NULL && curr->right == NULL) {
                outFile.put(curr->data);
                curr = root;
            }
        }
    }
    inFile.close();
    outFile.close();

    cout<<"decoded file saved"<<endl;
}



void mainHuffman::compress(){
    createMinHeap();
    createTree();
    createCodes();
    saveEncodedFile();
}

void mainHuffman::decompress(){
    getTree();
    saveDecodedFile();
}