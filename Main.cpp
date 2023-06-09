//#include "BinaryFormat.cpp"
#include <iostream>
#include <cmath>
#include<fstream>
#include <list>
#include "IPv4Format.cpp"
using namespace std;

class classlessIPv4{
    /*
    num_hosts stores the number of hosts required - user input
    num_hostbits stores the calculated number of host bits required
    num_netowrkbits stores the calculated number of network bits required
    *privateIP  stores the Private IP inputed by the user
    *networkMAsk stores the network mask of the privateIP
    *networkID stores the netowrk ID of the network
    *networkDBA stores the netowrk Direct broadcast Address of the netowrk  
    */
    //int num_hosts,num_subnets,num_hostbits,num_networkbits,num_subnetbits;
    int num_hosts,num_hostbits,num_networkbits;
    IPv4Format *privateIP,*networkMask,*networkID,*networkDBA;
    int nm1,nm2,nm3,nm4=0;
    public:
    classlessIPv4(int hosts,int p1,int p2,int p3,int p4){
        num_hosts=hosts;
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

    void createNetworkMask(){       //creates network mask according to input
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

    void generateIPAddresses(){     //generates all possible IP addresses 
        calculateNID();
        calculateDBA();
        int IP[4],NID[4],DBA[4],temp[4];
        networkID->getDecimal(NID);
        networkDBA->getDecimal(DBA);
        fstream fout;
        fout.open("GeneratedIP.txt",ios::out| ios::app);      //stores output in GeneratedIP.txt
        fout<<"\nAll Generated IP addresses in network - \n";
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
        fout.close();
    }
    
    void calculateNID(){        //function to calculate NID
        int nid[4],PIPDecimal[4],MaskDecimal[4];
        privateIP->getDecimal(PIPDecimal);
        networkMask->getDecimal(MaskDecimal);
        for(int i=0;i<4;i++){
            nid[i]=PIPDecimal[i]&MaskDecimal[i];    //bitwise and the 4 parts of the IP with Network Mask
        }
        networkID= new IPv4Format(nid[0],nid[1],nid[2],nid[3]);
    }

    void calculateDBA(){        //calculates DBA address of the network,sets host bits to 1
        int PIPDecimal[4],DBA[4],calc=0;
        privateIP->getDecimal(PIPDecimal);
        if(num_hostbits<=8){    
            DBA[0]=PIPDecimal[0];
            DBA[1]=PIPDecimal[1];
            DBA[2]=PIPDecimal[2];
            for(int i=0;i<num_hostbits;i++){
                calc+=pow(2,i);
            }
            DBA[3]=calc|PIPDecimal[3];
        }

        else if(num_hostbits<=16){
            DBA[0]=PIPDecimal[0];
            DBA[1]=PIPDecimal[1];
            for(int i=0;i<(num_hostbits-8);i++){
                calc+=pow(2,i);
            }
            DBA[2]=calc|PIPDecimal[2];
            DBA[3]=255;
        }
        else if(num_hostbits<=24){
            DBA[0]=PIPDecimal[0];
            for(int i=0;i<(num_hostbits-16);i++){
                calc+=pow(2,i);
            }
            DBA[1]=calc|PIPDecimal[1];
            DBA[2]=DBA[3]=255;
        }
        else if(num_hostbits<=32){
            for(int i=0;i<(num_hostbits-24);i++){
                calc+=pow(2,i);
            }
            DBA[0]=calc|PIPDecimal[1];
            DBA[1]=DBA[2]=DBA[3]=255;
        }
        else{
            cout<<"\nError!";
            abort();
        }
        networkDBA=new IPv4Format(DBA[0],DBA[1],DBA[2],DBA[3]);
    }

    int getnum_hosts(){
        return num_hosts;
    }

    int getnum_hostbits(){
        return num_hostbits;
    }

    void getDecimalPrivateIP(int *arr){
        privateIP->getDecimal(arr);
    }

    IPv4Format *getDBA(){
        return networkDBA;
    }

    IPv4Format *getNID(){
        return networkID;
    }

    IPv4Format *getMask(){
        return networkMask;
    }

    void display(){     //displays information regarding the classlessIP
        cout<<"Number of host bits required : "<<num_hostbits;
        cout<<"\nPrivate IP address -";
        privateIP->display(num_networkbits);
        cout<<"\nNetwork Mask - ";
        networkMask->display(num_networkbits);
        cout<<"\nPrivate Netowrk ID -";
        networkID->display(num_networkbits);
        cout<<"\nNetwork DBA - ";
        networkDBA->display(num_networkbits);
    }

    
};

class Subnetting{
    /*
    num_subnets stores the number of subnets required -user input
    num_subnetbits stores number of subnet bits required 
    num_accomodablehosts stores the number of accomodable hosts after subnetting 
    subnetipnum stores the closest higher 2^n value to the number of hosts
    */
    int num_subnets,num_subnetbits,num_accomodablehosts;
    int subnetipnum[100];
    classlessIPv4 *obj;
    list <classlessIPv4> subnetlist;
    void checkHost_SubnetCountCompatibility(){      //checking if number of hosts required is accomodable after subnetting
        if (obj->getnum_hosts()>num_accomodablehosts){
            cout<<endl<<obj->getnum_hosts()<<" hosts cannot be accomodated in "<<num_subnets<<" subnetted network!\nTip:Reduce number of subnets";
            abort();
        }
        return;
    }

    void inputHostSubnet(){ //stores the number of hosts in each subnet
        int temp,sum_temp=0;
        for(int i=0;i<num_subnets;i++){
            cout<<"\nEnter number of hosts in subnet "<<i+1<<" : ";
            cin>>temp;
            subnetipnum[i]=pow(2,ceil(log2(temp)));
            sum_temp+=subnetipnum[i];
            if(sum_temp>num_accomodablehosts+2){    //checks if the total number of hosts is getting larger than accomodable number of hosts
                cout<<"\nHost division in subnet incompatible!! Try again!";
                inputHostSubnet();
            }
        }
    }

    void assignSubnetMask(){
        int hostbitsrequired,subnetbitsrequired,arr[4];
        fstream fout;
        fout.open("GeneratedIP.txt",ios::app);
        fout<<"\n\nSubnets \n\n";
        obj->getDecimalPrivateIP(arr);
        IPv4Format DBA_last(arr[0],arr[1],arr[2],arr[3]);       //declared to store the DBA of last assigned subnet
        classlessIPv4 *subnet;
        
        
        for(int i=0;i<num_subnets;i++){
            fout<<"\nSubnet "<<i+1<<endl;
            DBA_last.getDecimal(arr);
            //hostbitsrequired=log2(subnetipnum[i]);
            //subnetbitsrequired=obj->getnum_hostbits()-hostbitsrequired;
            subnet= new classlessIPv4(subnetipnum[i]-2,arr[0],arr[1],arr[2],arr[3]+1);
            subnetlist.push_back(*subnet);       //adds subnet to the list
            DBA_last = *subnet->getDBA();
            subnet->display();
        }
    }

    void displayNAT(){
        list<classlessIPv4>::iterator it;
        int i;
        cout<<"\n\n\nNetwork ID\t\tSubnet Mask\t\t\tInterface\n\n";
        for(it=subnetlist.begin(),i=0;it!=subnetlist.end();it++,i++){
            IPv4Format *tempNID,*tempMask;
            tempNID=it->getNID();
            tempMask=it->getMask();
            tempNID->displayDecimal();
            cout<<"\t\t";
            tempMask->displayDecimal();
            cout<<"\t\t\t"<<i<<endl;
        }
        cout<<"0.0.0.0\t\t\t0.0.0.0\t\t\t\tDefault\n";
    }
    public:
    Subnetting(classlessIPv4 *o, int subnet){       //Subnetting parameterised constructor
        obj=o;
        num_accomodablehosts=(pow(2,obj->getnum_hostbits())-pow(2,num_subnetbits+1));   //caculating number of accomodable hosts after reserving subnetid and dba for each subnet
        cout<<"\nNumber of accomodable hosts = "<<num_accomodablehosts;
        num_subnets=subnet;
        num_subnetbits=ceil(log(subnet));
        checkHost_SubnetCountCompatibility();
        inputHostSubnet();
        assignSubnetMask();
        displayNAT();
    }

};

int main(){
    int result = remove("GeneratedIP.txt");
    if(!result)
        cout<<"\nRemoved GeneratedIP.txt created due to previous execution of code.\n";
    int hosts,n1,n2,n3,n4,subnet;
    cout<<"Enter number of hosts required : ";
    cin>>hosts;
    cout<<"Enter IP address of private network - ";
    scanf("%d.%d.%d.%d",&n1,&n2,&n3,&n4);
    classlessIPv4 obj(hosts,n1,n2,n3,n4);
    obj.display();
    cout<<"Enter number of subnet needed - ";
    cin>>subnet;
    Subnetting sub(&obj,subnet);
}