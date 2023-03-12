#include <iostream>
#include "BinaryFormat.cpp"
#include "DecimalFormat.cpp"

class IPv4Format:public BinaryFormat,public DecimalFormat{             //inheriting BinaryFormat and DecimalFormat classes
    bool checkIPValid(int n1,int n2,int n3,int n4){       //checks if ip range is out of bounds ,returns false if invalid
        if((n1<=255 && n1>=0)&&(n2<=255 && n2>=0)&&(n3<=255 && n3>=0)&&(n4<=255 && n4>=0))
            return true;
        return false;
    }

    public:
    IPv4Format(int n1,int n2,int n3,int n4):BinaryFormat(n1,n2,n3,n4),DecimalFormat(n1,n2,n3,n4){   //paramiterized constructor
        if(!checkIPValid(n1,n2,n3,n4)){
            cout<<"\nIP invalid!\nAborting program!";
            abort();
        }
    }

    void display(int num_networkbits){     //displaying IP in both formats
        cout<<"\nDotted Decimal Format - ";
        displayDecimal();
        cout<<"/"<<num_networkbits;
        cout<<"\nBinary Format - ";
        displayBinary();
        cout<<"/"<<num_networkbits;
        cout<<endl;
    }
};
