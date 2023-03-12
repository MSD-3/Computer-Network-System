#include <iostream>
using namespace std;

class BinaryFormat{     //class to convert IPv4 address to binary
    int b1,b2,b3,b4;
    string binaryString;
    public:
    BinaryFormat(int n1,int n2,int n3,int n4){      //parameterised constructor
        b1=b2=b3=b4=0;
        string temp;
        int *bintemp[]={&b1,&b2,&b3,&b4};
        int numtemp[]={n1,n2,n3,n4};
        for(int i=0,j=0;i<4;i++){       //conversion of decimal to binary
            temp="";
            while(numtemp[i]!=0){
                temp+=char(48+numtemp[i]%2);
                numtemp[i]/=2;
            }
            fillbits(temp);
            reverse(temp); 
            binaryString+=temp+'.';
            *bintemp[i]=stoi(temp);
        }
        binaryString.resize(binaryString.size()-1); //removing the extra . in the end
    }

    void reverse(string &str){      //reversing the string
        int n= str.length();
        for(int i=0;i<n/2;i++){
            swap(str[i],str[n-i-1]);
        }
    }

    void fillbits(string &str){     //filling the remaining bits with 0 to make it 8 bit long
        while (str.length()<8)
            str+='0';
        return;        
    }

    void getBinary(int *arr){       //call by reference to store the binary values 
        arr[0]=b1;
        arr[1]=b2;
        arr[2]=b3;
        arr[3]=b4;
    }

    string getBinaryString(){       //returns binary String 
        return binaryString;
    }

    void displayBinary(){           //displays binary string
        cout<<binaryString;
    }    
};
