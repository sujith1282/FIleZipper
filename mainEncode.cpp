#include <iostream>
#include <string>
#include "mainHuffman.hpp"

using namespace std;


int main(){

    cout<<"Enter the text file name "<<endl;
    string inFileName;
    cin>>inFileName;
    string outFileName=inFileName;
    inFileName+=".txt";
    outFileName+=".huf";

    mainHuffman f(inFileName,outFileName);
    f.compress();
    cout<<"Compression Done Successfully";   
    

    return 0;
}