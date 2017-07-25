/* Tazi programa sysdava tekstov fail, izvejda broq na dumite zapochvashti
s glavna bukva i zavyrshvashti na 'q', posle razpechatva sydarjanieto na
faila. */

#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define SIZE 50

void mainMenu();
void create_file();
void print_file();
void county();




void main() 			  /* Nachalo na glavnata funkciq */

{
char choice;                      /* Promenliva sydyrjashta izbora na potrebitelq*/

mainMenu();
do
{
mainMenu();

choice=getch();

switch(choice)
	{
	case '1': create_file() ; break;
	case '2': county()   ; break;
	case '3': print_file(); break;
	case '4':         ; break;
	default :
	getch();
	}
}
while(choice!='4');
exit(0);

}                                     /*Krai na glalavnata funkciq */



void mainMenu()

{
clrscr();
printf("M E N U: \n\n");
printf("1. Create new file \n\n");
printf("2. Nunber of the words  \n\n");
printf("3. Print the file \n\n");
printf("4. Exit \n\n");
printf("Make your choice==>");
}

void create_file()                    /* Funkcia sasdavashta tekstov fail */

{
char name[20], ch;
FILE *fp;

clrscr();
printf("Enter full path to file:// ");
gets(name);
if((fp=fopen(name,"w+"))!=NULL)
	{
	while((ch=getc(stdin)) != EOF)
		{
		putc(ch,fp);
		}
	fclose(fp);
	printf("\nFile Done! \n");
	printf("Press any key to Return to Main Menu!");
	getch();

	}
else
	{
	printf("\n Can't Create File!");
	}
}

void print_file()

{

FILE *fp;
clrscr();
char name[20], ch;

printf("Enter full path to file you want to print://");
gets(name);
if((fp=fopen(name,"r"))!=NULL)
	{
	printf("\nHere is the entry of %s :\n",name);
	while((ch=getc(fp))!=EOF)
		{
		putc(ch,stdout);
		}
	fclose(fp);
	printf("\nPress Any Key to Return to Main Menu");
	getch();
	}
else
{
printf("\nCan't find File!\n");
printf("Press Any Key to Return to Main Menu\n");
getch();
}
}


void county()
{
FILE *fp;
char name[20];

clrscr();
int br1=0, br2=0, i=0;
char buffer[50], ch;
printf("Enter filename ==>");
gets(name);
if((fp=fopen(name,"r"))!=NULL)
{

clrscr();

while((ch=getc(fp))!=EOF)
{
if(ch!=' ' && ch!='.' && ch!=',' && ch!='!' && ch!='?' && ch!='/r')
	{
	buffer[i]=ch;
	i++;
	}
else
{
buffer[i]='\0';
i=0;
if((buffer[i]>=65)&&(buffer[i]<=90))
br1++;
if((buffer[strlen(buffer)-1])=='q')
br2++;
}
}



fclose(fp);
printf("\nBroqt na dumite zapochvashti s glavna bukva =%d",br1);
printf("\n\nBroqt na dumite zavyrshvashti na q =%d",br2);
printf("\n\nPress Any Key to Return to Main Menu\n");
getch();
}
else
{
printf("\nCan't find File!\n");
printf("Press Any Key to Return to Main Menu\n");
getch();
}
}
