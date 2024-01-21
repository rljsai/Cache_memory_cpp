#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define ll long long int 
#define b1 bitset<1>
   
   ll cache_size,block_size,mainmem_size;
   int ascvty;
   ll no_ofadd;
   ll no_ofsets;
   string blockoffset,set_no,tag;
   ll add_length;
   ll hit;
   ll no_ofoffsetbits;
   ll no_ofindexbits;
   ll no_oftagbits;
   ll set_num;
   ll N;
   ll A;
   ll readhits;
   ll writehits;
    int binaryToDecimal(int n)
{
    int num = n;
    int dec_value = 0;
 
    // Initializing base value to 1, i.e 2^0
    int base = 1;
 
    int temp = num;
    while (temp) {
        int last_digit = temp % 10;
        temp = temp / 10;
 
        dec_value += last_digit * base;
 
        base = base * 2;
    }
 
    return dec_value;
}

int main() {

  cin>>block_size>>ascvty>>cache_size;
  no_ofsets=cache_size/(block_size*ascvty);  
  add_length=24;
  no_ofoffsetbits=log2(block_size);
  no_ofindexbits=log2(no_ofsets);
  no_oftagbits=add_length-no_ofoffsetbits-no_ofindexbits;
  string offset;
  string index;
  string tag;
  bitset<1> writebit;

  string tagarray[no_ofsets][ascvty];
  string dataarray[no_ofsets][ascvty];
  vector<string> adds;
  N=0;
  A=0;
  readhits=0;
  writehits=0;
  ifstream myfile("input1.txt");
  string hex;
    while(getline(myfile,hex)){
         N++;
         adds.push_back(hex);
    }
  
  bitset<1> validbit[no_ofsets][ascvty];
    for(int i=0;i<no_ofsets;i++){
        for(int j=0;j<ascvty;j++){
             validbit[i][j]=0;
        }
   }
  
  bitset<1> dirtybit[no_ofsets][ascvty];
    for(int i=0;i<no_ofsets;i++){
        for(int j=0;j<ascvty;j++){
             dirtybit[i][j]=1;
        }
   }


    int counter_no[no_ofsets][ascvty];
    for(int i=0;i<no_ofsets;i++){
       for(int j=0;j<ascvty;j++)
        counter_no[i][j]=j;
    }

    hit=0;
    string address1;
    
    for(ll i=0;i<N;i++){
        
    address1=adds[i];
    writebit=(address1[1]=='1') ? 0:1;
    if(writebit==0){
        A++;
    }

    string address="";

    for(int j=2;j<8;j++){
        switch(address1[j]){
            case '0':
                address=address+"0000";
                break;
            case '1':
                address=address+"0001";
                break;
            case '2':
                address=address+"0010";
                break;
            case '3':
                address=address+"0011";
                break;
            case '4':
                address=address+"0100";
                break;
            case '5':
                address=address+"0101";
                break;
            case '6':
                address=address+"0110";
                break;
            case '7':
                address=address+"0111";
                break;
            case '8':
                address=address+"1000";
                break;
            case '9':
                address=address+"1001";
                break;
            case 'a':
                address=address+"1010";
                break;
            case 'b':
                address=address+"1011";
                break;
            case 'c':
                address=address+"1100";
                break;
            case 'd':
                address=address+"1101";
                break;
            case 'e':
                address=address+"1110";
                break;
            case 'f':
                address=address+"1111";
                break;
        
        }
    }
    
    tag =(address.substr(0,no_oftagbits));
    set_no =(address.substr(no_oftagbits,no_ofindexbits));
    offset =(address.substr(no_oftagbits+no_ofindexbits,no_ofoffsetbits));

    int set_num_binary=stoi(set_no);
   
    int set_num=binaryToDecimal(set_num_binary);


    bool is_hit=0;

    for(int j=0;j<ascvty;j++){
       if((tag==tagarray[set_num][j])&&(validbit[set_num][j]==1)){
          hit++;
          is_hit=1;
          if (writebit==0){
            readhits+=1;
          }
          else{
            writehits+=1;
          }

          if(writebit==0){                                //read hit
              //access data in dataarray[set_no.to_ulong()][j]
              //retreive the block offset data
              //return to the processor
           }

           if(writebit==1){                                   //write hit
              //access data in dataarray[set_no.to_ulong()][j]
              //modify the data with the incoming data
              //putting the new data back into the array
              dirtybit[set_num][j]=1;                      //setting dirty bit when data is modified
              
           }

           //updating counters
           int presentcounter_no=counter_no[set_num][j];
           for(int k=0;k<ascvty;k++){
              if(counter_no[set_num][k]>presentcounter_no){
                counter_no[set_num][k]--;
              }
           }
           counter_no[set_num][j]=ascvty-1;

       }
  }
 



    if(is_hit==0){

        for(int j=0;j<ascvty;j++){             //identifying block to be replaced
           if(counter_no[set_num][j]==0){
               if(dirtybit[set_num][j]==1){
                //data of that block is updated in the main memory
                //(implenting write back policy)
               } 
             dirtybit[set_num][j]==0;
   
             tagarray[set_num][j]=tag;
             validbit[set_num][j]=0;  //resetting validbit

            
            }
        }    
           
          if(writebit==0){                             //read miss
            //access main memory with the address
            //fetch dataarray and read required data
            //retrieve the blockoffset
            //send to the processor
            //acessing the dataarray and putting the updated data
            for(int j=0;j<ascvty;j++){
                 if(tag==tagarray[set_num][j]){   //checking tags
                       //updating dataarray[set_num][j]
                       validbit[set_num][j]=1;   //setting valid bit 

                       //updating counters
                           int presentcounter_no=counter_no[set_num][j];
                            for(int k=0;k<ascvty;k++){
                               if(counter_no[set_num][k]>presentcounter_no){
                                   counter_no[set_num][k]--;
                                 }
                            }
                            counter_no[set_num][j]=ascvty-1;

                    }     
            }
          }


          if(writebit==1){                          //write miss 
              //access main memory with the address
              //fetch dataarray and read required data
              //make changes in the data
            
              for(int j=0;j<ascvty;j++){
                 if(tag==tagarray[set_num][j]){           //checking tags
                    //modify dataarray[set_num][j] with the new data                      
                    validbit[set_num][j]=1;              //setting valid bit   
                    dirtybit[set_num][j]=1;              //setting dirty bit when data is modified 

                    //updating counters
                           int presentcounter_no=counter_no[set_num][j];

                            for(int k=0;k<ascvty;k++){
                               if(counter_no[set_num][k]>presentcounter_no){
                                   counter_no[set_num][k]--;
                                 }
                            }
                            counter_no[set_num][j]=ascvty-1;
                 }     
                }
          }

           
    }
    
}
cout<<"number of addresses : "<<N<<endl;
cout<<"number of reads : "<<A<<endl;
cout<<"number of hits : "<<hit<<endl;
cout<<"number of misses : "<<N-hit<<endl;
cout<<"raedhits : "<<readhits<<endl;
cout<<"writehits : "<<writehits<<endl;
float hitrate=(hit*100.0)/N;
 
  cout<<hitrate;
    return 0;
}