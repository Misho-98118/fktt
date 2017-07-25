#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <ctype.h>
char menu();
void create_txt_file();
void print_txt_file();
void count();


void main()
{
int ch;
clrscr();
do
{
ch=menu();
switch(ch)
	{
	case '1': create_txt_file();   break;
	case '2': count(); 	       break;
	case '3': print_txt_file();     break;
	case '4': 		       break;
	default:  printf("\n\n Not correct choise! ");
	getch();
	}
}
while(ch!='4');
exit(0);
}


char menu()

{
clrscr();
char ch;
printf("\n1. Create new text File\n\n");
printf("2. Count\n\n");
printf("3. Print text file\n\n");
printf("4. Exit \n\n");
printf("Make your choise: ");
ch=getch();
return(ch);
}

void create_txt_file()

{
char filename[80]=" ",ch;

FILE *fp;

clrscr();
printf("Enter full path to file:// ");

gets(filename);
printf("Now type the entry of your text FILE. For end press CTRL+Z.\n\n");
printf(">> ");
if((fp=fopen(filename,"w+"))!=NULL)
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

void print_txt_file()

{

FILE *fp;
char filename[80]=" ",ch;

clrscr();

printf("Enter full path to file you want to print://");
gets(filename);
if((fp=fopen(filename,"r"))!=NULL)
	{
	printf("\nHere is the entry of \\ %s :\n\n",filename);
	while((ch=getc(fp))!=EOF)
		{
		putc(ch,stdout);
		}
	fclose(fp);
	printf("\n\nPress Any Key to Return to Main Menu");
	getch();
	}
else
{
gotoxy(1,7);
printf("\nCan't find File!\n");
printf("Press Any Key to Return to Main Menu\n");
getch();
}
}


void count()

{
FILE *fp;
char filename[50]=" ";
int words=0,sent=0;
char ch;

clrscr();
printf("Inpunt filename :\\ ");
gets(filename);
if((fp=fopen(filename,"r"))!=NULL)
{
while((ch=getc(fp)) != EOF)
	{
	if(isspace(ch))
		{
		words++;
		}
	if(ch=='.' || ch=='!' || ch=='?')
		{
		sent++;
		}
	}
fclose(fp);
}
else
	{
	printf("Can't Open %s",filename);
	getch();
	exit(1);
	}
words++;
printf("\n\n Tne number of words in %s is %d",filename,words);
printf("\n\n The number of sentenses in %s is %d",filename,sent);
getchar();

}
