#include "bits/stdc++.h"

using namespace std;


void subtract(string a,string b,string &c){
	for(int i=0;i<b.size();i++)
	{
		if(a[i] == b[i])
			c[i]='0';
		else
			c[i]='1';
	}
}


void divide(string num,string denom)
{
	int i;
	string a=num.substr(0,denom.size());
	string b;
	b.assign(denom);
	string zero(denom.size(),'0');
	i=denom.size()-1;
	while(i!=num.size())
	{
		string c(denom.size(),'0');
		if(a[0] == '0')
			subtract(a,zero,c);	
		else
			subtract(a,b,c);
		c.append(1,num[++i]);
		c.erase(0,1);
		a.assign(c);
	}
	cout<<"CRC: "<<a<<endl;
}


int main(int argc,char **argv)
{
	string a(argv[1]);
	string b(argv[2]);
	cout<<"Message: "<<a<<endl;
	cout<<"Polynomial: "<<b<<endl;
	a.append(b.size()-1,'0');
	cout<<"Modified msg: "<<a<<endl;
	divide(a,b);
	return 0;
}