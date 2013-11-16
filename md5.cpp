#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */
#include <string>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;

int md5(string md5,int n)
{
	
	int sum=0;int mul = 1;

	int a[128];int j=0;int i;
	for(i=0;i<32;i++)
	{
		if(md5[i]=='0')
		{
			a[j]=0;a[j+1]=0;a[j+2]=0;a[j+3]=0;j=j+4;
		}
		else if(md5[i]=='1')
		{
			a[j]=0;a[j+1]=0;a[j+2]=0;a[j+3]=1;j=j+4;
		}
		else if(md5[i]=='2')
		{
			a[j]=0;a[j+1]=0;a[j+2]=1;a[j+3]=0;j=j+4;
		}
		else if(md5[i]=='3')
		{
			a[j]=0;a[j+1]=0;a[j+2]=1;a[j+3]=1;j=j+4;
		}
		else if(md5[i]=='4')
		{
		a	[j]=0;a[j+1]=1;a[j+2]=0;a[j+3]=0;j=j+4;
		}
		else if(md5[i]=='5')
		{
			a[j]=0;a[j+1]=1;a[j+2]=0;a[j+3]=1;j=j+4;
		}
		else if(md5[i]=='6')
		{
			a[j]=0;a[j+1]=1;a[j+2]=1;a[j+3]=0;j=j+4;
		}
		else if(md5[i]=='7')
		{
			a[j]=0;a[j+1]=1;a[j+2]=1;a[j+3]=1;j=j+4;
		}
		else if(md5[i]=='8')
		{
			a[j]=1;a[j+1]=0;a[j+2]=0;a[j+3]=0;j=j+4;
		}
		else if(md5[i]=='9')
		{
			a[j]=1;a[j+1]=0;a[j+2]=0;a[j+3]=1;j=j+4;
		}
		else if(md5[i]=='a')
		{
			a[j]=1;a[j+1]=0;a[j+2]=1;a[j+3]=0;j=j+4;
		}
		else if(md5[i]=='b')
		{
			a[j]=1;a[j+1]=0;a[j+2]=1;a[j+3]=1;j=j+4;
		}
		else if(md5[i]=='c')
		{
			a[j]=1;a[j+1]=1;a[j+2]=0;a[j+3]=0;j=j+4;
		}
		else if(md5[i]=='d')
		{
			a[j]=1;a[j+1]=1;a[j+2]=0;a[j+3]=1;j=j+4;
		}
		else if(md5[i]=='e')
		{
			a[j]=1;a[j+1]=1;a[j+2]=1;a[j+3]=0;j=j+4;
		}
		else if(md5[i]=='f')
		{
			a[j]=1;a[j+1]=1;a[j+2]=1;a[j+3]=1;j=j+4;
		}
		
	}
	// for(i=0;i<128;i++)
	// {
	// 	cout<<a[i];
	// }
	// cout<<endl;
	for(i=127;i>=0;i--)
	{
		if(a[i]==1)
		{
			sum = sum + mul;mul=((mul*2)%n);
		}
	}
	return sum%n;
}
