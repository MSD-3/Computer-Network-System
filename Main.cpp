//#include "BinaryFormat.cpp"
#include <iostream>
#include <cmath>
#include<fstream>
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
        binaryString.resize(binaryString.size()-1);
    }

    void reverse(string &str){
        int n= str.length();
        for(int i=0;i<n/2;i++){
            swap(str[i],str[n-i-1]);
        }
    }

    void fillbits(string &str){
        while (str.length()<8)
            str+='0';
        return;        
    }

    void getBinary(int *arr){
        arr[0]=b1;
        arr[1]=b2;
        arr[2]=b3;
        arr[3]=b4;
    }

    string getBinaryString(){
        return binaryString;
    }

    void displayBinary(){
        cout<<binaryString;
    }    
};

class DecimalFormat{
    int d1,d2,d3,d4;
    string decimalString;
    public:
    DecimalFormat(int n1,int n2,int n3,int n4){
        d1=n1,d2=n2,d3=n3,d4=n4;
        int arr[]={d1,d2,d3,d4};
        decimalString=to_string(d1)+"."+to_string(d2)+"."+to_string(d3)+"."+to_string(d4);
    }

    void getDecimal(int *arr){
        arr[0]=d1;
        arr[1]=d2;
        arr[2]=d3;
        arr[3]=d4;
    }
    string getDecimalString(){
        return decimalString;
    }
    void displayDecimal(){
        cout<<decimalString;
    }
};


class IPv4Format:public BinaryFormat,public DecimalFormat{
    bool checkIPValid(int n1,int n2,int n3,int n4){       //checks if ip range is out of bounds ,returns false if invalid
        if((n1<=255 && n1>=0)&&(n2<=255 && n2>=0)&&(n3<=255 && n3>=0)&&(n4<=255 && n4>=0))
            return true;
        return false;
    }

    public:
    IPv4Format(int n1,int n2,int n3,int n4):BinaryFormat(n1,n2,n3,n4),DecimalFormat(n1,n2,n3,n4){
        if(!checkIPValid(n1,n2,n3,n4)){
            cout<<"\nIP invalid!\nAborting program!";
            abort();
        }
    }

    void display(){
        cout<<"\nDotted Decimal Format - ";
        displayDecimal();
        cout<<"\nBinary Format - ";
        displayBinary();
        cout<<endl;
    }
};

class classlessIPv4{

    int num_hosts,num_hostbits,num_networkbits;
    IPv4Format *privateIP,*networkMask,*networkID,*networkDBA;
    int nm1,nm2,nm3,nm4=0;
    public:
    classlessIPv4(int hosts,int p1,int p2,int p3,int p4){
        num_hostbits=ceil(log2(hosts+2));
        num_networkbits=32-num_hostbits;
        privateIP=new IPv4Format(p1,p2,p3,p4); //initialising IPv4 to p1.p2.p3.p4
        createNetworkMask();
        networkMask=new IPv4Format(nm1,nm2,nm3,nm4);
        generateIPAddresses();
    }
    
    int maskcalc(int k){      //calculating the network mask
        int temp=0,i=8;
        int nclone=i-num_hostbits+8*k;
        while(nclone!=0){
            temp+=pow(2,--i);
            nclone--;
        }
        return temp;
    }

    void createNetworkMask(){
        if(num_hostbits<=8){
            nm1=nm2=nm3=255;
            nm4=maskcalc(0);
        }
        else if(num_hostbits<=16){
            nm1=nm2=255;
            nm3=maskcalc(1);
            nm4=0;
        }
        else if(num_hostbits<=24){
            nm1=255;
            nm2=maskcalc(2);
            nm4=nm3=0;
        }
        else if(num_hostbits<=32){
            nm1=maskcalc(3);
            nm4=nm3=nm2=0;
        }
        else
            cout<<"Number of hosts too large for a single network.";
    }

    void generateIPAddresses(){
        calculateNID();
        calculateDBA();
        int IP[4],NID[4],DBA[4],temp[4];
        networkID->getDecimal(NID);
        networkDBA->getDecimal(DBA);
        fstream fout;
        fout.open("GeneratedIP.txt",ios::out);
        for(int i=0;i<4;i++){
            temp[i]=NID[i];
        }
        fout<<"Dotted Decimal Format\t\t\tBinary Format\n";
        
        while(temp[0]<=DBA[0]){
            IP[0]=temp[0]++;
            while(temp[1]<=DBA[1]){
                IP[1]=temp[1]++;
                while(temp[2]<=DBA[2]){
                    IP[2]=temp[2]++;
                    while(temp[3]<=DBA[3]){
                        IP[3]=temp[3]++;
                        IPv4Format ipaddress(IP[0],IP[1],IP[2],IP[3]);
                        fout<<"\t"<<ipaddress.getDecimalString()<<"\t\t"<<ipaddress.getBinaryString()<<endl;
                    }
                    temp[3]=NID[3];
                }
                temp[2]=NID[2];
            }
            temp[1]=NID[1];
        }
    }
    
    void calculateNID(){
        int nid[4],PIPDecimal[4],MaskDecimal[4];
        privateIP->getDecimal(PIPDecimal);
        networkMask->getDecimal(MaskDecimal);
        for(int i=0;i<4;i++){
            nid[i]=PIPDecimal[i]&MaskDecimal[i];
        }
        networkID= new IPv4Format(nid[0],nid[1],nid[2],nid[3]);
    }

    void calculateDBA(){
        int PIPDecimal[4],DBA[4],calc=0;
        privateIP->getDecimal(PIPDecimal);
        if(num_hostbits<8){
            DBA[0]=PIPDecimal[0];
            DBA[1]=PIPDecimal[1];
            DBA[2]=PIPDecimal[2];
            for(int i=0;i<num_hostbits;i++){
                calc+=pow(2,i);
            }
            DBA[3]=calc|PIPDecimal[3];
        }
        networkDBA=new IPv4Format(DBA[0],DBA[1],DBA[2],DBA[3]);
    }

    void display(){
        cout<<"Number of host bits required : "<<num_hostbits;
        cout<<"\nPrivate IP address -";
        privateIP->display();
        cout<<"\nNetwork Mask - ";
        networkMask->display();
        cout<<"\nPrivate Netowrk ID -";
        networkID->display();
        cout<<"\nNetwork DBA - ";
        networkDBA->display();
    }

    
};

int main(){
    int hosts,n1,n2,n3,n4;
    cout<<"Enter number of hosts required : ";
    cin>>hosts;
    cout<<"Enter IP address of private network - ";
    scanf("%d.%d.%d.%d",&n1,&n2,&n3,&n4);
    classlessIPv4 obj(hosts,n1,n2,n3,n4);
    obj.display();
}