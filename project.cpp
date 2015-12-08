#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <stdio.h>
#include <time.h>
using namespace std;
struct all_count{
	map<string,int>subdata;
	int total;
};
struct broked{
	string word;
	string type;
};

class allow{
	public:
	//break a string with "/" 
	broked str;
   size_t pos;
	void broker(string allstr){
		pos = allstr.find("/");                                         //leaner in length of return mean in penn.txt ~0
		//cout<<pos<<endl;
		str.type = allstr.substr(++pos);                                //leaner in length of return mean in penn.txt ~0
		str.word = allstr.substr(0,--pos);                              //leaner in length of return mean in penn.txt ~0
	}
	broked get_new(){
		return str;
	}
};
//i'm makeing a class which insert element in map by campairing value not key 

class english{
	public:
	map< string,all_count> data;
	map <string,all_count>::iterator it,itlowp,itupp;
	multimap<int,string>freqsort;
	all_count temp;
	multimap<int,string>::iterator itlow,itup;
	string prefix;
	int total_freq;
	//i'm using multimap this map work only for question 2  (find +-10% frequency word)//
	void freq_insert(){
		for(map<string,all_count>::iterator itfreq = data.begin();itfreq != data.end();++itfreq){
		freqsort.insert(pair<int ,string>(itfreq->second.total,itfreq->first));    // element insertition logn
		}
	}
	void insertw(string key,string str2){      //max Complexity   4logn  
		 // cout<<key<<endl;
		  //cout<<str2<<endl;
		  map<string,int> val;
		  all_count fcount;
		  int subcount = 1;
		  val.insert(pair<string,int>(str2,subcount));
		  fcount.subdata =val;
		  fcount.total=1;
		  
          if(data.insert(pair<string,all_count>(key,fcount)).second ==false){//key element  exist  //Complexity logn
			  data.find(key)->second.total++;                                                      //Complexity logn
			 if(data.find(key)->second.subdata.insert(pair<string,int>(str2,subcount)).second ==false){//type also exist  //Complexity logn
				 data.find(key)->second.subdata.find(str2)->second ++;                            //Complexity logn
			 }
		 }
	}
	void search(string key){     // total Complexity logn
		temp = data.find(key)->second;   
		prefix =key;
		total_freq = temp.total;
	}
	void frequency(){  //Complexity o(1)
		cout<< "\033[3;36mFrequency of word \033[1;33m "<<prefix<<" =\033[31m"<<total_freq<<"\n";
	}
	void less_great_freq(){        //total Complexity 2logn + constant   // max 2logn+n
		 itlow = freqsort.lower_bound(0.9*total_freq);   //Complexity logn
		 itup = freqsort.upper_bound(1.1*total_freq);    //Complexity logn
		for (itlow; itlow!=itup; ++itlow)                //Complexity  constant
         std::cout << (*itlow).second << "\033[3;34m(" << (*itlow).first<< ")\033[0m ";
		printf("\n");
	}
	void categry(){     //max n  
		int catc;
		 for (std::map<string,int>::iterator its=temp.subdata.begin(); its!=temp.subdata.end(); ++its){  //Complexity  constant
			 catc = its->second;
		 std::cout << its->first << "\033[3;34m => \033[0m" << catc<< "\033[3;34m => \033[0m"<<catc*100.0/total_freq<<'\n';
		}
	}
	void prefixshare_non_case_sensitive(unsigned int n,string str,unsigned char s){
		unsigned int n1;
		string strpre;
    if(n >=65&& n <91 ){ n1 = n+32;
		printf("%s","\033[2;33m IF SEARCH NON CASE SENCITIVE (first letter)TNEN THESE + ALSO \033[0m\n");
		str.at(0)=(unsigned char)n1;
		strpre = str;
		str.at(str.size()-1) =s;
		itlowp = data.lower_bound(strpre);  //Complexity logn
		 itupp = data.upper_bound(str);    //Complexity logn
		 itupp--;
		for (it = itlowp; it!=itupp; ++it)   //loop 
         std::cout << it->first << "\033[3;34m(" << it->second.total<<")\033[0m";
         printf("\n");
	}
    else if(n >= 97 && n < 123){n1 = n-32;
		printf("%s","\033[2;33m IF SEARCH NON CASE SENCITIVE(first letter) TNEN THESE + ALSO \033[0m\n");
		str.at(0)=(unsigned char)n1;
		strpre = str;
		str.at(str.size()-1) =s;
		itlowp = data.lower_bound(strpre);   //Complexity logn
		 itupp = data.upper_bound(str);      //Complexity logn
		 itupp--;
		for (it = itlowp; it!=itupp; ++it)   //loop
         std::cout << it->first << "\033[3;34m(" << it->second.total<<")\033[0m";
         printf("\n");
		}
	}
	void prefixshare(int p){
		//cout<<prefix.substr(0,prefix.size()*p/100) <<" next "<<prefix.substr(0,1)<<endl;
	unsigned char ascii;
	string str,strp;
	str =prefix.substr(0,prefix.size()*p/100);
	strp =str;
	if(str.size() !=0){
	unsigned int n;
	ascii = str.at(str.size()-1);
	n =(unsigned int)ascii;
	unsigned int m= n+1;
	unsigned char s;
	s=(unsigned char)m;
	str.at(str.size()-1) = s;
	//cout <<str<<endl;
		 itlowp = data.lower_bound(strp); //Complexity logn
		 itupp = data.upper_bound(str);   //Complexity logn
		 itupp--;
		for (it = itlowp; it!=itupp; ++it) //loop
         std::cout << it->first << "\033[3;34m(" << it->second.total<<")\033[0m";
         printf("\n");
         prefixshare_non_case_sensitive((unsigned int)strp.at(0),strp,s);
	}
	else {printf("%s","\033[3;33mPrefix doesn't exists on this P% of word increase p \033[0m\n");
	}
}
};
int main(){
	//string(int) type output indicate that string is a word and (int) is frequency of this word 
	//this particular format used in showing +-10% frequency word and same P% prefix shared words
	/*string=>int=>float   this formate it used to show all category  here string shows category name ,
	int indicate the frequency of of word in string category and float shows the percantage of word in string category */
	//the function prefixshare_non_case_sensitive (....)  used to show word which share p% prefix of given word (non-case sensitive)
	// if prefix is Inc than  ^function show all word which have inc prefix
	cout<< "\033[1;31m if only case sensitive case considered than comment line 137 > prefixshare_non_case_sensitive((unsigned int)strp.at(0),strp,s) \n";
	clock_t start,end;
	start =clock();
	string get;
	int p;
	allow brocker;
	english dict;
	while(cin >>get&& get !="-1"){   //Complexity 4nlogn if n element inserted 
		allow brocker;
		brocker.broker(get);
		dict.insertw(brocker.get_new().word,brocker.get_new().type);
	}
	dict.freq_insert();   //Complexity nlogn
	end =clock();
	float tot = ((float)end-(float)start)/CLOCKS_PER_SEC;
	start =clock();
	system("clear");
	while(cin>>get){   //7logn + loop constant time (output cout loop)
		//allow brocker;
		//brocker.broker(get);
		dict.search(get);
		dict.frequency();
		printf("%s ","\033[3;36mAns2  Words whose frequencies are UPTO +-10% of word \033[0m\n");
		dict.less_great_freq();
		printf("%s ","\033[3;36mAns3,4,5 Names of syntactic categories:The number of times:percentage\033[0m\n");
		dict.categry();
		cin >>p;
		printf("%s ", "\033[3;36mAns6 List of all other words that share the same p% prefix as W \033[0m\n");
		dict.prefixshare(p);
	}
	end = clock();
	float totfind = ((float)end-(float)start)/CLOCKS_PER_SEC;
	printf("\033[3;38mInput file reading time %f ,\nSearch output++ operation time, %f \033[0m\n",tot,totfind);
	
}
//time (5n+7)logn + loop of constant time 
