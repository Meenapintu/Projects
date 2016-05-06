#include <conio.h>

int main()
{
	int n,sum=0;
	n=2000;
	int i=0;
	int array[n];
	for(;i<n;i++)
	{
		array[i]=i;
		sum=sum+i;
	}
	Print("Sum is : %d . \n",sum);
	return 0;
}

