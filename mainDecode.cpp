#include <iostream>
#include <string>
#include "mainhuffman.hpp"

using namespace std;


int main(){

    cout<<"Enter the text file name "<<endl;
    string inFileName;
    cin>>inFileName;
    string outFileName=inFileName;
    inFileName+=".huf";
    outFileName+="New.txt";

    mainHuffman f(inFileName,outFileName);
    f.decompress();
    cout<<"Decompressed Successfully";   
    

    return 0;
}