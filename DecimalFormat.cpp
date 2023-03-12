#include <iostream>
using namespace std;

class DecimalFormat{                
    int d1,d2,d3,d4;
    string decimalString;
    public:
    DecimalFormat(int n1,int n2,int n3,int n4){ 
        d1=n1,d2=n2,d3=n3,d4=n4;
        int arr[]={d1,d2,d3,d4};
        decimalString=to_string(d1)+"."+to_string(d2)+"."+to_string(d3)+"."+to_string(d4);
    }

    void getDecimal(int *arr){      //call by reference to store the decimal values
        arr[0]=d1;
        arr[1]=d2;
        arr[2]=d3;
        arr[3]=d4;
    }
    string getDecimalString(){      //returns Decimal string
        return decimalString;
    }
    void displayDecimal(){          //displays decimal string
        cout<<decimalString;
    }
};