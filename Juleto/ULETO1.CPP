#include <stdio.h>
#include <conio.h>
#include <string.h>
#define FSIZE 30
#define TSIZE 100



int CreateFile(void);
int ReadFile(void);
int Transfer(void);

int main(void)
{
int izbor,check;

 izbor=0;check=-1;
 do
  {
  clrscr();
 /*Sazdavane na menuto*/
 gotoxy(3,2);
 printf("Select an option: ");
 gotoxy(20,10);
 printf("1.Sazdaite tekstovi fail.");
 gotoxy(20,11);
 printf("2.Izvejdane na teksta.");
 gotoxy(20,12);
 printf("3.Razdelqne na dumi po-dalgi ot zadadena dyljina.");
 gotoxy(20,13);
 printf("4.Exit.");
 gotoxy(21,2);

 scanf("%d",&izbor);

/*Izbor na funkcia*/
switch (izbor)
 {
  case 1:
  check=CreateFile();
  break;
  case 2:
  check=ReadFile();
  break;
  case 3:
  check=Transfer();
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
 /*Vavejdane na teksta i imeto na tekstovia fail*/
 printf("Enter text: ");
 gets(text);
 printf("\nEnter file name: ");
 gets(filename);

 /*Zapisvane na tekstovia fail*/
 if ((fp=fopen(filename, "w")) == NULL)
  printf("Can't Create File %s",filename);
  getch();
  return 1;


 /*Zapis na teksta v tekstovia fail*/
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
 /*Izbor na faila*/
 printf("Enter file name: ");
 gets(filename);

 /*Zarejdane na faila*/
 if((fp=fopen(filename,"r"))==NULL);
 printf("Can't open file %s",filename);
 getch();
  return 1;

 printf("\n\n");

/*Izvejdane na sadarjanieto ot faila*/
  while((ch=fgetc(fp))!=EOF)
   putchar(ch);

fclose(fp);


do ch=getche(); while(ch!='\r');
return 1;
}



int Transfer(void)
{
 char file1[FSIZE], file2[FSIZE], word[TSIZE];
 FILE *from, *to;
 char ch;
 int count,i,c1;

  clrscr();


 /*Vavejdane na imenata na faila za 4etene i faila za zapis*/
 printf("Enter character number: ");
 scanf("%d",&count);
 ch=getchar();
 printf("Enter file name: ");
 gets(file1);
 printf("Enter name of the file where the words will be transfered: ");
 gets(file2);



/*Otvariane na dvata faila*/
 if((from=fopen(file1,"r")) == NULL)
 printf("Can't Find file %s",file1);
 getch();
 return 1;

 if ((to=fopen(file2, "w+")) == NULL)
 printf("Can't Create file %s",file2);
 getch();
 return 1;

 i=0;
/*Razdelia ne i prehvarliane na dumite s pove4e simvoli ot zadadenia v vtoria tekstovi fail*/

 do
 {
  ch=fgetc(from);
  word[i]=ch;
   i++;
   if((ch==' ')||(ch==EOF))
    {
     word[i]='\0';
     if(count<(i-1))
      for(c1=0;c1<strlen(word);c1++)
	 {
	  if(c1==(i-1)/2)
	   {
	     fputc(' ',to);
	     putchar(' ');
	   }
	  fputc(word[c1],to);
	  putchar(word[c1]);
	 }
     i=0;
    }
  }
  while (ch!=EOF);

/*Zatvariane na dvata faila*/
fclose(from);
fclose(to);


/*iz4akva da se atisne ENTER sled koeto se vra6ta v glavnata funkcia*/
do ch=getche(); while(ch!='\r');
return 1;
}