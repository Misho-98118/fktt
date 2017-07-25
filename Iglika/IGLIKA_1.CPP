#include<stdio.h>
#include<conio.h>

int  granica(float *, char *);
void input_data(float *, int *, char *);   /*Definicii na funkciite */
void print_data(float *, int *, char *);
void sr_ar(float *, int *, char *);

main()                                    /*Nacalo na glavnata funkciq*/

{

float x[50], y[40];


clrscr();

int gran;


gran=granica(x,"x");
input_data(x,&gran,"x");
print_data(x,&gran,"x");
sr_ar(x,&gran,"x");

return 0;
}         /*Krai na glavnata funkciq*/


int granica(float *name, char *string)

{
int gra,num;
num=sizeof(name)/sizeof(float);
do
	{
	printf("Vavedete broi elementi na redicata %s \n",string);
	scanf("%d",&gra);
	}
while((gra<0)||(gra>num));
return(gra);

}

void input_data(float *name, int*lim, char *string)

{
int i;
printf("Vavejdane na elementite na masiva %s \n",string);
for(i=0;i<*lim;i++)
	{
	printf("Vavedete [%d]-element",i);
	scanf("%f",(name+i));
	}
printf("\n Masiva %s e vaveden!\n",string);
}

void print_data(float *name, int*lim, char *string)

{
int i;
printf("Elementite na masiva %s sa: \n",string);
for(i=0;i<*lim;i++)
	printf("%f  \n",*(name+i));
}

void sr_ar(float *name, int *lim, char *string)

{
float sum;
int k,i;

sum=0.;
k=0;

for(i=0;i<*lim;i++)
	if(*(name+i)>0)
		{
		sum+=*(name+i);
		k++;
		}

printf("Srednata aritmetichna stoinost na masiva %s e %f",string,sum/k);
}