#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<ctime>
#include<iomanip>
#include<cstdlib>
using namespace std;

int keys[256]={0},transkeys[256]={0};
void getkey(string keyfname);
void decrypt();
void encrypt();
void generatekey();
void speed(int bytespeed);
void itoa(char str[],int num);

long long int length,prep=0;
clock_t start,finish;
string orifname="",outfname="",keyfname="",modes="",version="3.9.2";
ifstream infile;
ofstream oufile;
char mode='n';
int errorcode=0,chunk=50000;

int main(int argc, char *argv[]){
    cout<<"=====================================================\n";
    cout<<"Path: "<<argv[0]<<"\n";
    cout<<"Encryptfile "<<version<<" Copyright c 2019\n";
    for(int i=0;i<argc;i++){
        string argu=argv[i];
        argu=argu.replace(0,1,"");
        if(argu=="e" || argu=="E"){
                mode='e';
                if(i+1<argc){
                    orifname=argv[i+1];
                }else{
                    errorcode=1;
                    break;
                }
                i++;
        }else if(argu=="d" || argu=="D"){
                mode='d';
                if(i+1<argc){
                    orifname=argv[i+1];
                }else{
                    errorcode=1;
                    break;
                }
                i++;
        }else if(argu=="k" || argu=="K"){
                if(i+1<argc){
                    keyfname=argv[i+1];
                }else{
                    errorcode=1;
                    break;
                }
                i++;
        }else if(argu=="o" || argu=="O"){
                if(i+1<argc){
                    outfname=argv[i+1];
                }else{
                    errorcode=1;
                    break;
                }
                i++;
        }else if(argu=="c" || argu=="C"){
                if(i+1<argc){
                    chunk=atoi(argv[i+1]);
                }else{
                    errorcode=1;
                    break;
                }
                i++;
        }else if(argu=="g" || argu=="G"){
                if(i+1<argc){
                    keyfname=argv[i+1];
                    cout<<"Key file: "<<keyfname<<"\n";
                    cout<<right<<setw(15)<<"Process"<<"|"<<setw(20)<<"Size"<<"|"<<setw(10)<<"Status"<<"|"<<setw(8)<<"Speed"<<"\n";
                    cout<<"--------------------------------------------------------\n";
                    generatekey();
                }else{
                    errorcode=1;
                    break;
                }
                i++;
        }
    }
    if((mode=='e' || mode=='d') && !errorcode){
        cout<<"Input file: "<<orifname<<"\n";
        cout<<"Output file: "<<outfname<<"\n";
        cout<<"Key file: "<<keyfname<<"\n";
        cout<<"Chunk size: "<<chunk<<"\n";
        cout<<right<<setw(15)<<"Process"<<"|"<<setw(20)<<"Size"<<"|"<<setw(10)<<"Status"<<"|"<<setw(8)<<"Speed"<<"\n";
        cout<<"--------------------------------------------------------\n";
        getkey(keyfname);
        if(orifname!=""){
            infile.open(orifname.c_str(), ios::in|ios::binary);
            infile.seekg (0, ios::end);
            length = infile.tellg();    //get file size
            infile.seekg (0, ios::beg);
        }else{
            cout<<"Unknown input file path\n";
            errorcode=1;
        }
        if(outfname!=""){
            oufile.open(outfname.c_str(), ios::out|ios::binary);
        }else{
            cout<<"Unknown output file path, please check whether you had used commend /o\n";
            errorcode=1;
        }
        if(keyfname==""){
            cout<<"Unknown key file path, if you wanna create new key, use commend /g\n";
            errorcode=1;
        }
        if(!errorcode){
            if(mode=='e') encrypt();
            if(mode=='d') decrypt();
            infile.close();
            oufile.close();
        }
    }
    cout<<"=====================================================\n";
}

void getkey(string keyfname){
    modes="Reading keys";
    cout<<"\r"<<right<<setw(15)<<modes<<"|"<<setw(20)<<256<<"|"<<setw(10)<<"Reading"<<"|"<<setw(8)<<"-"<<std::flush;
    int index=0;
    string line,keycode,temp="";
    ifstream keyfile(keyfname.c_str(),ios::in);
    getline(keyfile,line);
    for(int i=0;i<line.length();i++){
        if(line[i]=='_'){
            int intemp=atoi(temp.c_str()),transtemp=index;
            if(transtemp>127) transtemp-=256;
            transkeys[intemp]=transtemp;
            if(intemp>127) intemp-=256;
            keys[index]=intemp;
            temp="";
            index++;
            continue;
        }
        temp=temp+line[i];
    }
    cout<<"\r"<<right<<setw(15)<<modes<<"|"<<setw(20)<<256<<"|"<<setw(10)<<"Done"<<"|"<<setw(8)<<"-"<<std::flush<<"\n";
}

void encrypt(){
    modes="Encrypting";
    long long int code,times=length/chunk;
    char ch[chunk],temp;
    start=clock();
    for(long long int  i=0;i<times;i++){
        infile.read(ch,chunk);  //read into buffer
        for(int j=0;j<chunk;j++){
            code=int(ch[j]);
            if(code<0) code+=256;
            ch[j]=keys[code];
        }
        oufile.write(ch,chunk); //write from buffer
        finish=clock();
        if((finish-start)/CLOCKS_PER_SEC>=1){   //show progress and speed per 1s
            long long int nowtellg=infile.tellg();
			speed(nowtellg-prep);
            prep=infile.tellg();
            start=clock();
        }
    }
    for(long long int  i=0;i<length-chunk*times;i++){ //process the last data
        infile.get(temp);
        code=int(temp);
        if(code<0) code+=256;
        oufile.put(keys[code]);
        finish=clock();
        if((finish-start)/CLOCKS_PER_SEC>=1){
            long long int nowtellg=infile.tellg();
			speed(nowtellg-prep);
            prep=infile.tellg();
            start=clock();
        }
    }
    cout<<"\r"<<right<<setw(15)<<modes<<"|"<<setw(20)<<length<<"|"<<setw(10)<<"Done"<<"|"<<setw(8)<<"-"<<std::flush<<"\n";
}

void decrypt(){
    modes="Decrypting";
    long long int code,times=length/chunk;
    char ch[chunk],temp;
    start=clock();
    for(long long int  i=0;i<times;i++){
        infile.read(ch,chunk);  //read into buffer
        for(int j=0;j<chunk;j++){
            code=int(ch[j]);
            if(code<0) code+=256;
            ch[j]=transkeys[code];
        }
        oufile.write(ch,chunk); //write from buffer
        finish=clock();
        if((finish-start)/CLOCKS_PER_SEC>=1){   //show progress and speed per 1s
            long long int nowtellg=infile.tellg();
			speed(nowtellg-prep);
            prep=infile.tellg();
            start=clock();
        }
    }
    for(long long int  i=0;i<length-chunk*times;i++){ //process the last data
        infile.get(temp);
        code=int(temp);
        if(code<0) code+=256;
        oufile.put(transkeys[code]);
        finish=clock();
        if((finish-start)/CLOCKS_PER_SEC>=1){
            long long int nowtellg=infile.tellg();
			speed(nowtellg-prep);
            prep=infile.tellg();
            start=clock();
        }
    }
    cout<<"\r"<<right<<setw(15)<<modes<<"|"<<setw(20)<<length<<"|"<<setw(10)<<"Done"<<"|"<<setw(8)<<"-"<<std::flush<<"\n";
}

void speed(int bytespeed){
    int ucount=0,progress;
    double cspeed=bytespeed;
    string unit[5]={"B/s","KB/s","MB/s","GB/s","TB/s"};
    while(cspeed>1023){
        cspeed=(double)cspeed/1024;
        ucount++;
    }
    progress=infile.tellg()*100/length;
    cout<<"\r"<<right<<setw(15)<<modes<<"|"<<setw(20)<<length<<"|"<<setw(9)<<progress<<"%|"<<setw(4)<<round(cspeed)<<setw(4)<<unit[ucount]<<std::flush;
}


void generatekey(){
    srand(time(NULL));
    int gkeys[256]={0};
    for(int i=0;i<256;i++){
        cout<<"\r"<<right<<setw(15)<<"Generating keys"<<"|"<<setw(20)<<256<<"|"<<setw(9)<<int((i)*100/(3*256))<<"%|"<<setw(8)<<"-"<<std::flush;
        gkeys[i]=i;
    }
    for(int i=0;i<256;i++){
        cout<<"\r"<<right<<setw(15)<<"Generating keys"<<"|"<<setw(20)<<256<<"|"<<setw(9)<<int((i+256)*100/(3*256))<<"%|"<<setw(8)<<"-"<<std::flush;
        swap(gkeys[rand()%256],gkeys[rand()%256]);
    }
    ofstream keyout(keyfname.c_str(),ios::out);
    for(int i=0;i<256;i++){
        cout<<"\r"<<right<<setw(15)<<"Generating keys"<<"|"<<setw(20)<<256<<"|"<<setw(9)<<int((i+512)*100/(3*256))<<"%|"<<setw(8)<<"-"<<std::flush;
        char str[10];
        itoa(str,gkeys[i]);
        keyout<<str<<"_";
    }
        cout<<"\r"<<right<<setw(15)<<"Generating keys"<<"|"<<setw(20)<<256<<"|"<<setw(10)<<"Done"<<"|"<<setw(8)<<"-"<<std::flush<<"\n";
    keyout.close();
}

void itoa(char str[],int num){
    int N=0,temp=num;
    while(temp>0){
        N++;
        temp/=10;
    }
    for(int i=N-1;num>0;i--){
        str[i]=char(num%10+'0');
        num/=10;
    }
    str[N]='\0';
}
