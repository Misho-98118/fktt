/***************************************************************************
*Tazi programa sysdava tekstov fail, podrejda dumite po broq na sydarjash- *
*tite se simvoli vav vsqka duma, posle razpechatva sydarjanieto na faila.  *
***************************************************************************/


#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define SIZE 50

void background();          	 /*Prototipi na izpolzvanite funkcii*/
void mainMenu();
void create_file();
void print_file();
void sort_file();




void main() 			  /* Nachalo na glavnata funkciq */

{
char choice;                      /* Promenliva sydyrjashta izbora na potrebitelq*/

background();	                  /* Izvikvane na funkciqta sazdavashta fona na
				     rabotniq ekran */

mainMenu();                       /* Funkciq oformqshta glavnoto menu*/
do                                /* Nacalo na cikal organizirasht potrebitelski
				    izbor */
{
background();
mainMenu();
gotoxy(43,14);
choice=getch();
switch(choice)
	{
	case '1': create_file() ; break;
	case '2': sort_file()   ; break;
	case '3': print_file()  ; break;
	case '4':               ; break;
	default :         gotoxy(27,17);
	textcolor(4);
	cprintf("*** E R R O R ! ***");
	getch();
	delline();
	insline();
	textcolor(15);
	}
}
while(choice!='4');                      /* Izhod ot cikala pri izbranq opciq */
exit(0);
}                                    	 /*Krai na glalavnata funkciq */


void background()                        /* Nachalo na funciqta oformqshta
					    background-a na menu-to */

{
clrscr();
textbackground(1);
textcolor(15);
cprintf(" TECHICAL UNUVERSITY - SOFIA                                               FKTT");
gotoxy(1,25);
cprintf(" Mihail Mihailov                                                       PA042410");
textbackground(0);
textcolor(15);
}
						/*Tazi funkciq oformq glavnoto menu*/
void mainMenu()

{
textbackground(0);
textcolor(2);
gotoxy(25,7);
cprintf("M E N U: ");
gotoxy(25,9);
cprintf("1. Create new file ");
gotoxy(25,10);
cprintf("2. Sort symbols by Length ");
gotoxy(25,11);
cprintf("3. Print the file ");
gotoxy(25,12);
cprintf("4. Exit ");
gotoxy(25,14);
cprintf("Make your choice [ ]");
textcolor(15);
}

void create_file()                    		/* Funkcia sasdavashta tekstov fail */

{
char name[80]="",ch;                           /* Deklaracii na promenlivi*/

FILE *fp;

background();
gotoxy(5,5);
printf("Enter full path to file:// ");          /* Podkanq potrebitelq da vavede pat i ime
						 na faila, koito shte bade sazdavan */
gets(name);
printf("Now type the entry of your text FILE. For end press CTRL+Z.\n\n");
printf(">> ");
if((fp=fopen(name,"w+"))!=NULL)
	{
	while((ch=getc(stdin)) != EOF)          /* Zapisva vseki vaveden simvol vav faila*/
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

void print_file()           			                      /* Funkciqta otpechatva tekstov fail na ekrana*/

{

FILE *fp;
char name[80]="",ch;

background();
gotoxy(1,5);
printf("Enter full path to file you want to print://");
gets(name);
if((fp=fopen(name,"r"))!=NULL)
	{
	printf("\nHere is the entry of %s :\n\n",name);
	while((ch=getc(fp))!=EOF)                             /* Proverka za krai na faila*/
		{
		putc(ch,stdout);			      /* Kopira ch v standartniq izhod*/
		}
	fclose(fp);
	printf("\nPress Any Key to Return to Main Menu");
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


void sort_file()                         /* Sortira dumite v TXT-file po broi na sadarjashtati se
					   simvoli */
{

struct list_item                         /* Definirane na structura */
{
int word_lenght;
char *word;
} list[SIZE],help;                       /* declarirane na masiv i promenliva ot tip list_item */

char ch,buffer[50],name[80]="",new_name[80]="";
int i,j,p;
FILE *fp;

for(i=0;i<SIZE;i++)                      /* Nulirane na vsichki elementi na masiva list ot tip list_item */
{
list[i].word=NULL;
list[i].word_lenght=NULL;
}

clrscr();
background();
gotoxy(1,5);
printf("Enter full path to file you want to sort://");
gets(name);
printf("It will be created new file that contained the entry of %s \n",name);
printf("Enter full path for the new file and filename://");
gets(new_name);
if(strlen(new_name)==0)
	strcpy(new_name,name);
if((fp=fopen(name,"r"))!=NULL)
{
printf("\nDone!\n");
p=0; i=0;
while((ch=getc(fp))!=EOF)
	{
	if((!(isspace(ch))) && (!(ispunct(ch))))
		{
		buffer[i]=ch;                               /* ako ne e prepinatelen znak ili space simvola
								se zapisva v buferna promenliva */
		i++;
		}
	else
							     /* slaga se krai na niza. Izvlich se negovata daljina.
								zadelq se dinamichno prostranstvo ravno na goleminata
							       niza. Ukazatel s nachalo na tova prostranstvo se zapis-
							       va v masiva list ot tip list_item, kakto i daljinata
							       na dumata   */
	{
	buffer[i]='\0';
	i=0;
	if((strlen(buffer)))
		{
		list[p].word=(char*)malloc(strlen(buffer)+1);
			if(!list[p].word)
			{
			printf("Allocation Failed");
			getch();
			exit(1);
			}
		list[p].word_lenght=(strlen(buffer));
		strcpy(list[p].word,buffer);
		p++;
		}
	}
}
	fclose(fp);
	}

else
printf("\nError oppening file!\n");

/**********************************************************************************************/
/* Nachalo na sortirovka po daljina na simvolite v dadena duma                                */
/**********************************************************************************************/

for(i=0;i<SIZE-1;i++)
	for(j=i+1;j<SIZE;j++)
	if(((list[i].word_lenght>0)&&(list[j].word_lenght>0))&&((list[i].word_lenght)>(list[j].word_lenght)))
		{
		help=list[i];
		list[i]=list[j];
		list[j]=help;
		}


if((fp=fopen(new_name,"w+"))!=NULL)   					/*  Otvarqne na nov file i zapisvane na sortiranite danni */
	{
	i=0;
	printf("\New File Created!\n");
	while(list[i].word_lenght>0)
		{
		fputs(list[i].word,fp);
		fputs("\n",fp);
		i++;
		}
	fclose(fp);
	}
else
printf("\Error creating New File!\n");
getch();
}




