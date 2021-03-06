#include "bits/stdc++.h"

using namespace std;


int isPowerOfTwo(int x)
{
	 while (((x % 2) == 0) && x > 1)
	   x /= 2;
	return (x == 1);
}

int get_redbits(int n)
{
	int k=3;
	int redbits=3;
	while(k!=n)
	{
		if(float(k+1+redbits) == pow(2,redbits))
		{
			redbits=redbits+1;
		}
		k++;
	}
	return redbits;
}

int ret1pos(int checkpos,int pos)
{
	string binary = bitset<12>(pos).to_string();
	if(binary[binary.size()-checkpos-1] == '1')
		return 1;
	else
		return 0;
}


void modifymessage(string msg,int redbits)
{
	//Modify message to send
	cout<<"Initial message: "<<msg<<endl;
	string msgmod(msg.size()+redbits,'0');
	string msgred(redbits,'0');
	cout<<"Unmodifed message: "<<msgmod<<endl;
	int k=0;
	int i=1;
	while(i<=msg.size()+redbits)
	{
		if(isPowerOfTwo(i) == 1)
		{
			// cout<<i<<endl;
		}
		else{
			msgmod[i-1]=msg[k];
			k++;
		}
		i++;
	}
	int l=0;
	cout<<"Modifed after phase 1: "<<msgmod<<endl;
	for(int i=1;i<=msg.size()+redbits;i++)
	{
		if(isPowerOfTwo(i) == 1)
		{
			int count=0;
			for(int j=1;j<=msg.size()+redbits;j++)
				if(ret1pos(i-1,j) == 1 && msgmod[j-1]=='1')
					count=count+1;
			// cout<<"Count value for "<<i<<": "<<count;
			if(count%2==0)
			{
				msgmod[i-1]='0';
				msgred[l++]='0';

			}
			else
			{
				msgmod[i-1]='1';
				msgred[l++]='1';
			}
		}
	}
	cout<<"Modifed message after phase 2 : "<<msgmod<<endl;
	cout<<"Hamming code binary: "<<msgred<<endl;
	bitset<20> msgredbit(msgred);
	bitset<20> msgmodbit(msgmod); 
	stringstream res;
	res << hex << uppercase << msgredbit.to_ulong();
	cout<<res.str()<<endl;
	stringstream res2;
	res2 << hex << uppercase << msgmodbit.to_ulong();
	cout<<res2.str()<<endl;
}


int error_check(string msg)
{
	//Check error
}

int main(int argc,char **argv)
{

	string hex(argv[1]);
	unsigned int msgnumber;
	sscanf(hex.c_str(), "%x", &msgnumber); 
	string binary = bitset<64>(msgnumber).to_string();
	// cout<<binary<<endl;
	int l=0;
	for(int i=0;i<binary.size();i++)
		if(binary[i]=='0')
			l++;
		else
			break;
	binary.erase(0,l);
	cout<<binary;
	modifymessage(binary,get_redbits(binary.size()));
	return 0;
}