#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#define FSIZE 30
#define TSIZE 100



int CreateFile(void);
int ReadFile(void);
int Change(void);

int main(void)
{
int izbor,check;
 
 izbor=0;check=-1;
 do
  {
  clrscr();
 //Sazdavane na menuto
 gotoxy(3,2);
 printf("Select an option: ");
 gotoxy(20,10);
 printf("1.Sazdaite tekstovi fail.");
 gotoxy(20,11);
 printf("2.Izvejdane na teksta.");
 gotoxy(20,12);
 printf("3.Prebroiavane na intervalite i glasnite bukvi.");
 gotoxy(20,13);
 printf("4.Exit.");
 gotoxy(21,2);
    scanf("%d",&izbor);
 
//Izbor na funkcia
switch (izbor)
 {
  case 1:
  check=CreateFile();
  break;
  case 2:
  check=ReadFile();  
  break;
  case 3:
  check=Change();
  break;
  case 4:check=0; break;
  default:break;
 }


  } while(check!=0);
  return 0;
}


int CreateFile(void)
{
 FILE *fp;
 char filename[FSIZE];
 char text[TSIZE];
 char *p,ch;

 clrscr();
 ch=getchar();
  //Vavejdane na teksta i imeto na tekstovia fail
 printf("Enter text: ");
 gets(text);
 printf("\nEnter file name: ");
 gets(filename);

 //Zapisvane na tekstovia fail
 if ((fp=fopen(filename, "w")) == NULL)
  return 1;
   
  
 //Zapis na teksta v tekstovia fail
 p=text;
 while(*p)
  {
   if(fputc(*p,fp)==EOF)
    return 1;
  p++;
 }
fclose(fp);

return 1;
}
 

int ReadFile(void)
{
 char filename[FSIZE];
 FILE *fp;
 char ch;
 
 clrscr();
 ch=getchar();
 //Izbor na faila
 printf("Enter file name: ");
 gets(filename);

 //Zarejdane na faila
 if((fp=fopen(filename,"r"))==NULL)
  return 1;

 printf("\n\n");

//Izvejdane na sadarjanieto ot faila
  while((ch=fgetc(fp))!=EOF)
   putchar(ch);
 
fclose(fp);


do ch=getche(); while(ch!='\r');
return 1;
}



int Change(void)
{
 char filename[FSIZE],text[TSIZE];
 FILE *fp;
 int a,t,i;
 char ch,*p;

 clrscr();
 a=0;t=0;i=0;
 ch=getchar();

 //Izbor na fail
 printf("Enter file name: ");
 gets(filename);

 //Otvariane na tekstovia fail
 if((fp=fopen(filename,"r"))==NULL)
  return 1;
 
 //Prebroiava i bukvite a i t, sled koeto gi zamestva saotvetno s '.' i '!'
 while((ch=fgetc(fp))!=EOF)
 {
  
  if ((toupper(ch))=='A')  { a++; ch='.'; }
  if ((toupper(ch))=='T')  { t++; ch='!'; }
  text[i]=ch; i++;
 }
fclose(fp);
text[i]='\0';

//Zapisvane na tekstovia fail
 if ((fp=fopen(filename, "w+")) == NULL)
  return 1;
   
  
 //Zapis na teksta v tekstovia fail
 p=text;
 while(*p)
  {
   if(fputc(*p,fp)==EOF)
    return 1;
  p++;
 }
fclose(fp);

//Izvejda rezultata ot prebroiavaneto
 printf("\n Broiat na bukvite 'a' v teksta e: %d ",a);
 printf("\n Broiat na bukvite 't' v teksta e: %d",t);

do ch=getche(); while(ch!='\r');
return 1;
}