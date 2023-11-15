#ifndef MAINHUFFMAN_HPP
#define MAINHUFFMAN_HPP

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

class Node{
    public:
        char data;
        string code;
        int freq;
        Node *left,*right;

        Node(){
            left=right=NULL;
        }
};


class mainHuffman{
    public:
        fstream inFile,outFile;
        string inFileName,outFileName;
        vector<Node*> arr;
        Node *root;
        int remove=0;

        class Compare{
        public:
            bool operator()(Node *l,Node *r){
                return l->freq > r->freq;
            }
        };

        priority_queue< Node* , vector<Node*> , Compare > minHeap;

        void createArr();

        void createMinHeap();

        void createTree();

        void traverse(Node*,string);

        void createCodes();

        int binToDec(string);

        void saveEncodedFile();

        string decToBin(int);

        void buildTree(char,string);

        void getTree();

        void saveDecodedFile();

        mainHuffman(string inFileName,string outFileName){
            this->inFileName=inFileName;
            this->outFileName=outFileName;
            createArr();
        }


        void compress();

        void decompress();


};




#endif