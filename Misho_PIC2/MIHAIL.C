#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <process.h>
#include <alloc.h>
#include <dos.h>
#include <graphics.h>

#define DN_LEN      9
#define NAME_LEN   61
#define CITY_LEN   21
#define STREET_LEN 31
#define TEL_LEN    11
#define FAX_LEN    11
#define PNAME_LEN  21

#define MAX_FILE_NAME 81

#define FILE_TEMP  "TEMP0001.TMP"
#define FILE_FIRM  "FIRM.DAT"

#define K_ENTER   13
#define K_BS       8
#define K_DEL 83*256

#define MCOL     BLUE;
#define MBKCOL   CYAN;
#define DCOL     WHITE;
#define DBKCOL   RED;
#define ICOL     BROWN;
#define IBKCOL   GREEN;

#define CUR_HI  1
#define CUR_LO  2
#define CUR_OFF 3

#define SCREEN_BUFFER 4097

typedef struct
{
 char dn[DN_LEN];
 char fname[PNAME_LEN],sname[PNAME_LEN],tname[PNAME_LEN];
 float pech[10];
} Firm, *pFirm;

typedef struct
{
 char dn[DN_LEN],name[NAME_LEN],city[CITY_LEN],street[STREET_LEN],tel[TEL_LEN],fax[FAX_LEN];
 unsigned int r_day,r_mon,r_year;
} tFirm, *ptFirm;

typedef struct _Pid *pPid;
typedef struct _Pid
{
 pPid prev,next;
 char dn[DN_LEN];
 char name[NAME_LEN];
 char city[CITY_LEN];
 char street[STREET_LEN];
 unsigned int r_day,r_mon,r_year;
} Pid;

void AddFirm(int i);
void SearchFirm(void);
void RemFirm(void);
void EnterCurDate(void);
void Cvqt(void);

void checkarg(int argc,char *argv[]);
void SetCursor(unsigned char a);
void ErrorExit(char *s);
char *Enter(char *value,char *enter,int max);
void OpenFile(void);
void CloseFile(void);
int FindFirm(long *pos,char *dn);
void WriteFirm(pFirm pf);
void ReadFirm(pFirm pf);
void FRemFirm(long pos);
void GetFirm(pFirm pf,long *pos);
void ReadtFirm(ptFirm ptf,char *dn);
void WritetFirm(ptFirm ptf,char *dn);
void Spravka(void);

pPid createPid(char *dn,char *name,char *city,char *street,
	       unsigned int r_day,unsigned int r_mon,unsigned int r_year);
void destroyPid(pPid *pp);
pPid searchPid(pPid pp,char *dn);
void flip(pPid pp1,pPid pp2);
int sort(pPid *pa);

void win(int x, int y, int w, int h, int col, int bkcol);
void menu(int x, int y, int w, int h);
void dialog(int x, int y, int w, int h);
void info(int x, int y, int w, int h);

unsigned int toScreen(unsigned char c,unsigned char attr);
void clearscreen(void);
void GotoW(char x,char y);
void DrawTextW(char *s);

const char CH_0_9[]="0123456789";
const char CH_a_Z_0_9[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char CH_A_Z_0_9[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const char CH_a_Z[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char CH_a_Z_spc[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
const int MONTH[12]={31,28,31,30,31,30,31,31,30,31,30,31};

FILE *ff;
char ffname[MAX_FILE_NAME];
Firm firm;
struct date curdate;
int mcol = MCOL;
int mbkcol = MBKCOL;
int dcol = DCOL;
int dbkcol = DBKCOL;
int icol = ICOL;
int ibkcol = IBKCOL;
char BackGround[SCREEN_BUFFER];
unsigned int far *ScreenPointer=(unsigned int far *) 0xb8000000;
long CurPos=0;
int CurX,CurY,CurH,CurW;
unsigned char CurCol;

int main(int argc,char *argv[])
{
 char c=' ';
 _fmemcpy(BackGround,ScreenPointer,SCREEN_BUFFER);
 SetCursor(CUR_OFF);
 checkarg(argc,argv);
 strcpy(ffname,FILE_FIRM);
 OpenFile();
 memset(&firm,0,sizeof(Firm));
 EnterCurDate();
 while (c!='7')
 {
  do
  {
   menu(10,5,60,10);
   GotoW(5,1);DrawTextW("   [1] Dobavqne na firma");
   GotoW(5,2);DrawTextW("   [2] Aktualizaciq na firma");
   GotoW(5,3);DrawTextW("   [3] Iztrivane na firma ot kataloga");
   GotoW(5,4);DrawTextW("   [4] Informaciq za firmi ot ednakyv adres");
   GotoW(5,5);DrawTextW("   [5] Spravka za vsichki firmi");
   GotoW(5,6);DrawTextW("   [6] Nastroiki na cvqt");
   GotoW(5,7);DrawTextW("   [7] Krai");
   c=getch();
  } while (c<'1' || c>'7');
  switch (c)
  {
   case '1' : AddFirm(1);break;
   case '2' : AddFirm(0);break;
   case '3' : RemFirm();break;
   case '4' : SearchFirm();break;
   case '5' : Spravka();break;
   case '6' : Cvqt();break;
  }
 }
 CloseFile();
 SetCursor(CUR_LO);
 return 0;
}

void Cvqt(void)
{
 char s[100];
 char v[100];

 info(50,3,22,19);
 GotoW(3,1);DrawTextW("COLOR     0 do 15");
 GotoW(3,2);DrawTextW("BCKCOLOR  0 do  7");
 GotoW(3,4);DrawTextW("BLACK        ³  0");
 GotoW(3,5);DrawTextW("BLUE         ³  1");
 GotoW(3,6);DrawTextW("GREEN        ³  2");
 GotoW(3,7);DrawTextW("CYAN         ³  3");
 GotoW(3,8);DrawTextW("RED          ³  4");
 GotoW(3,9);DrawTextW("MAGENTA      ³  5");
 GotoW(3,10);DrawTextW("BROWN        ³  6");
 GotoW(3,11);DrawTextW("LIGHTGRAY    ³  7");
 GotoW(3,12);DrawTextW("DARKGRAY     ³  8");
 GotoW(3,13);DrawTextW("LIGHTBLUE    ³  9");
 GotoW(3,14);DrawTextW("LIGHTGREEN   ³ 10");
 GotoW(3,15);DrawTextW("LIGHTCYAN    ³ 11");
 GotoW(3,16);DrawTextW("LIGHTRED     ³ 12");
 GotoW(3,17);DrawTextW("LIGHTMAGENTA ³ 13");
 GotoW(3,18);DrawTextW("YELLOW       ³ 14");
 GotoW(3,19);DrawTextW("WHITE        ³ 15");
 dialog(5,8,40,10);
 memset(s,0,100);strcpy(s,CH_0_9);
 memset(v,0,100);sprintf(v,"%02d",mcol);
 GotoW(2,1);DrawTextW("Vavedete cvqt za tekst na menu:");
 do {
  sscanf(Enter(v,s,2),"%d",&mcol);
 } while (mcol>15);
 memset(v,0,100);sprintf(v,"%01d",mbkcol);
 GotoW(2,2);DrawTextW("Vavedete cvqt za fon na menu:");
 do {
  sscanf(Enter(v,s,1),"%d",&mbkcol);
 } while (mbkcol>7);
 memset(v,0,100);sprintf(v,"%02d",dcol);
 GotoW(2,3);DrawTextW("Vavedete cvqt za tekst na dialog:");
 do {
  sscanf(Enter(v,s,2),"%d",&dcol);
 } while (dcol>15);
 memset(v,0,100);sprintf(v,"%01d",dbkcol);
 GotoW(2,4);DrawTextW("Vavedete cvqt za fon na dialog:");
 do {
  sscanf(Enter(v,s,1),"%d",&dbkcol);
 } while (dbkcol>7);
 memset(v,0,100);sprintf(v,"%02d",icol);
 GotoW(2,5);DrawTextW("Vavedete cvqt za tekst na info:");
 do {
  sscanf(Enter(v,s,2),"%d",&icol);
 } while (icol>15);
 memset(v,0,100);sprintf(v,"%01d",ibkcol);
 GotoW(2,6);DrawTextW("Vavedete cvqt za fon na info:");
 do {
  sscanf(Enter(v,s,1),"%d",&ibkcol);
 } while (ibkcol>7);
}

void EnterCurDate(void)
{
 char s[100];
 char v[100];
 unsigned int c;

 memset(s,0,100);strcpy(s,CH_0_9);strcat(s,".");memset(v,0,100);
 dialog(15,15,50,3);
 GotoW(2,1);DrawTextW("Vavedete tekushta data:");
 while (1) {
  do {
   Enter(v,s,10);
    } while(v[4]!='.' || v[7]!='.'
	    || (strchr(CH_0_9,v[0])==NULL) || (strchr(CH_0_9,v[1])==NULL)
	    || (strchr(CH_0_9,v[2])==NULL) || (strchr(CH_0_9,v[3])==NULL)
	    || (strchr(CH_0_9,v[5])==NULL) || (strchr(CH_0_9,v[6])==NULL)
	    || (strchr(CH_0_9,v[8])==NULL) || (strchr(CH_0_9,v[9])==NULL));
  sscanf(v,"%u",&curdate.da_year);
  sscanf(v+5,"%u",&c);curdate.da_day=c;
  sscanf(v+8,"%u",&c);curdate.da_mon=c;
  if (curdate.da_year<1000 || curdate.da_year>3000) continue;
  if (curdate.da_mon<1 || curdate.da_mon>12) continue;
  if (curdate.da_day<1 || curdate.da_day>(MONTH[curdate.da_mon-1]+(curdate.da_mon==2?(((curdate.da_year%4)==0)-((curdate.da_year%100)==0)+((curdate.da_year%400)==0)):0))) continue;
  break;
 }
}

void AddFirm(int i)
{
 Firm f;
 tFirm tf;
 char s[100];
 char v[100];
 long pos;
 char c;
 int y;

 if (!i) {
  pos=0;
  GetFirm(&f,&pos);
  if (pos==EOF) {info(25,15,30,3);GotoW(2,1);DrawTextW("Nqma firmi!!!");getch();return;}
 }

 memset(&f,0,sizeof(Firm));
 memset(&tf,0,sizeof(tFirm));
 while (1) {
  dialog(3,1,74,22);
  y=1;
  GotoW(2,y++);DrawTextW("Vavedete danachen nomer na firmata:");
  memset(s,0,100);strcpy(s,CH_A_Z_0_9);memset(v,0,100);
  strcpy(f.dn,Enter(v,s,DN_LEN-1));
  if (strlen(v)<DN_LEN-1) continue;
  if ((strchr(CH_a_Z_spc,v[0])==NULL) || (strchr(CH_a_Z_spc,v[1])==NULL)
   || (strchr(CH_0_9,v[2])==NULL) || (strchr(CH_0_9,v[3])==NULL)
   || (strchr(CH_0_9,v[4])==NULL) || (strchr(CH_0_9,v[5])==NULL)
   || (strchr(CH_a_Z_spc,v[6])==NULL) || (strchr(CH_0_9,v[7])==NULL)) continue;
  pos=0;
  if (FindFirm(&pos,f.dn)) {
   if (i) {
    GotoW(2,y++);DrawTextW("Firmata syshtestvuva v bazata - Prezapis(Y/N)!");
    memset(s,0,100);strcpy(s,"YyNn");memset(v,0,100);
    Enter(v,s,1);if (v[0]=='N' || v[0]=='n') continue;
   }
   ReadtFirm(&tf,f.dn);
   strcpy(f.fname,firm.fname);
   strcpy(f.sname,firm.sname);
   strcpy(f.tname,firm.tname);
   for(pos=0;pos<10;pos++) f.pech[pos]=firm.pech[pos];
  }
  else
   if (!i) {GotoW(2,y++);DrawTextW("Tazi firma ne syshtestvuva v bazata danni!");getch();return;}
   else {memset(&firm,0,sizeof(Firm));strcpy(tf.dn,f.dn);}
  if (i) {
   GotoW(2,y++);DrawTextW("Vavedete imeto na firmata:");
   memset(s,0,100);strcpy(s,CH_a_Z_spc);
   Enter(tf.name,s,NAME_LEN-1);
   if (strlen(tf.name)>CurW-25) y++;
  }
  GotoW(2,y++);DrawTextW("Vavedete adres na registraciq(Grad,Ulica i nomer)");
  memset(s,0,100);strcpy(s,CH_a_Z_0_9);strcat(s," ,");
  while(1) {
   memset(v,0,100);
   GotoW(2,y);DrawTextW(":");if (strcmp(tf.city,"")!=0) sprintf(v,"%s,%s",tf.city,tf.street);
   Enter(v,s,CITY_LEN-1+STREET_LEN-1);
   memset(&tf.city,0,CITY_LEN);memset(&tf.street,0,STREET_LEN);
   pos=strlen(v)-strlen(strstr(v,","));if (pos>CITY_LEN-1) continue;
   strncpy(tf.city,v,pos);
   if (strlen(strstr(v,","))-1>STREET_LEN-1) continue;
   strncpy(tf.street,v+pos+1,strlen(strstr(v,",")));
   pos=0;
   if (strcmp(tf.city,"")==0) continue;
   do if (strchr(CH_a_Z_spc,tf.city[pos])==NULL) break;while (++pos<CITY_LEN);
   if (pos<CITY_LEN) continue;pos=0;
   if (strcmp(tf.street,"")==0) continue;
   do if (strchr(CH_a_Z_0_9,tf.street[pos])==NULL && tf.street[pos]!=' ') break;while (++pos<STREET_LEN);
   if (pos<STREET_LEN) continue;pos=0;
   break;
  }
  memset(s,0,100);strcpy(s,CH_0_9);strcat(s,".");
  memset(&v,0,100);
  if (i) {
   GotoW(2,++y);DrawTextW("Vavedete data na registraciq:");
   while (1) {
    do {
     Enter(v,s,10);
    } while(v[4]!='.' || v[7]!='.'
	    || (strchr(CH_0_9,v[0])==NULL) || (strchr(CH_0_9,v[1])==NULL)
	    || (strchr(CH_0_9,v[2])==NULL) || (strchr(CH_0_9,v[3])==NULL)
	    || (strchr(CH_0_9,v[5])==NULL) || (strchr(CH_0_9,v[6])==NULL)
	    || (strchr(CH_0_9,v[8])==NULL) || (strchr(CH_0_9,v[9])==NULL));
    sscanf(v,"%u",&tf.r_year);
    sscanf(v+5,"%u",&tf.r_day);
    sscanf(v+8,"%u",&tf.r_mon);
    if (tf.r_year<1000 || tf.r_year>3000) continue;
    if (tf.r_mon<1 || tf.r_mon>12) continue;
    if (tf.r_day<1 || tf.r_day>(MONTH[tf.r_mon-1]+(tf.r_mon==2?(((tf.r_year%4)==0)-((tf.r_year%100)==0)+((tf.r_year%400)==0)):0))) continue;
    if (tf.r_year>curdate.da_year) continue;
    if (tf.r_year==curdate.da_year && tf.r_mon>curdate.da_mon) continue;
    if (tf.r_year==curdate.da_year && tf.r_mon==curdate.da_mon && tf.r_day>curdate.da_day) continue;
    break;
   }
  }
  strcat(s,"-");
  if (i) for(pos=0;pos<10;pos++) f.pech[pos]=0;
  pos=curdate.da_year-tf.r_year;if (pos>10) pos=10;
  y++;
  while (pos) {
   GotoW(2,y++);memset(v,0,100);sprintf(v,"Vavedete pechalba predi %ld god.:",pos);
   DrawTextW(v);memset(v,0,100);
   if (!sscanf(Enter(v,s,8),"%f",&f.pech[--pos])) {pos++;y--;}
  }
  memset(s,0,100);strcpy(s,CH_a_Z);
  GotoW(2,y++);DrawTextW("Vavedete pyrvoto ime na prezidenta na firmata:");
  Enter(f.fname,s,PNAME_LEN-1);
  GotoW(2,y++);DrawTextW("Vavedete vtoroto ime na prezidenta na firmata:");
  Enter(f.sname,s,PNAME_LEN-1);
  GotoW(2,y++);DrawTextW("Vavedete tretoto ime na prezidenta na firmata:");
  Enter(f.tname,s,PNAME_LEN-1);
  GotoW(2,y++);DrawTextW("Vavedete (Telefon,Fax)");
  memset(s,0,100);strcpy(s,CH_0_9);strcat(s," ,");
  while(1) {
   memset(v,0,100);
   GotoW(2,y);DrawTextW(":");if (strcmp(tf.tel,"")!=0) sprintf(v,"%s,%s",tf.tel,tf.fax);
   Enter(v,s,TEL_LEN-1+FAX_LEN-1);
   memset(&tf.tel,0,TEL_LEN);memset(&tf.fax,0,FAX_LEN);
   pos=strlen(v)-strlen(strstr(v,","));if (pos>TEL_LEN-1) continue;
   strncpy(tf.tel,v,pos);
   if (strlen(strstr(v,","))-1>FAX_LEN-1) continue;
   strncpy(tf.fax,v+pos+1,strlen(strstr(v,",")));
   pos=0;
   if (strcmp(tf.tel,"")==0) continue;
   do if (strchr(CH_0_9,tf.tel[pos])==NULL) break;while (++pos<TEL_LEN);
   if (pos<TEL_LEN) continue;pos=0;
   if (strcmp(tf.fax,"")==0) continue;
   do if (strchr(CH_0_9,tf.fax[pos])==NULL) break;while (++pos<FAX_LEN);
   if (pos<FAX_LEN) continue;pos=0;
   break;
  }
  break;
 }
 WriteFirm(&f);
 WritetFirm(&tf,f.dn);
}

void RemFirm(void)
{
 Firm f;
 char s[100];
 char v[100];
 long pos;

 pos=0;
 GetFirm(&f,&pos);
 if (pos==EOF) {info(25,15,30,3);GotoW(2,1);DrawTextW("Nqma firmi!!!");getch();return;}

 memset(&f,0,sizeof(Firm));
 dialog(10,8,60,5);
 while (1) {
  GotoW(2,1);DrawTextW("Vavedete danachen nomer na firmata:");
  memset(s,0,100);strcpy(s,CH_A_Z_0_9);memset(v,0,100);
  strcpy(f.dn,Enter(v,s,DN_LEN-1));
  if (strlen(v)<DN_LEN-1) continue;
  if ((strchr(CH_a_Z_spc,v[0])==NULL) || (strchr(CH_a_Z_spc,v[1])==NULL)
   || (strchr(CH_0_9,v[2])==NULL) || (strchr(CH_0_9,v[3])==NULL)
   || (strchr(CH_0_9,v[4])==NULL) || (strchr(CH_0_9,v[5])==NULL)
   || (strchr(CH_a_Z_spc,v[6])==NULL) || (strchr(CH_0_9,v[7])==NULL)) continue;
  break;
 }
 pos=0;
 if (FindFirm(&pos,f.dn)) FRemFirm(pos);
 else {GotoW(2,2);DrawTextW("Tazi firma ne syshtestvuva v kataloga!!");getch();}
}


void SearchFirm(void)
{
 Firm f;
 tFirm tf,tf2;
 char v[100];
 long pos,pos2,ok=0;
 int y=1;

 pos=0;
 GetFirm(&f,&pos);
 if (pos==EOF) {info(25,15,30,3);GotoW(2,1);DrawTextW("Nqma firmi!!!");getch();return;}

 info(5,2,70,20);
 memset(&f,0,sizeof(Firm));
 memset(&tf,0,sizeof(tFirm));
 ok=pos=0;
 do {
  GetFirm(&f,&pos);
  if (pos==EOF) break;
  ReadtFirm(&tf,f.dn);
  pos2=0;
  do {
   GetFirm(&f,&pos2);
   if (pos2==EOF) break;
   ReadtFirm(&tf2,f.dn);
   if ((stricmp(tf.city,tf2.city)==0) && (strcmp(tf.dn,tf2.dn)!=0) &&
       ((stricmp(tf.street,tf2.street)==0) ||
	(stricmp(tf.tel,tf2.tel)==0) ||
	(stricmp(tf.fax,tf2.fax)==0))) continue;
   break;
  } while(pos2!=EOF);
  if (pos==pos2) {
   pos2=0;
   do {
    GetFirm(&f,&pos2);
    if (pos2==EOF) break;
    ReadtFirm(&tf2,f.dn);
    if ((stricmp(tf.city,tf2.city)==0) &&
	((stricmp(tf.street,tf2.street)==0) ||
	 (stricmp(tf.tel,tf2.tel)==0) ||
	 (stricmp(tf.fax,tf2.fax)==0))) {
     GotoW(2,y++);memset(v,0,100);sprintf(v,"Danachen nomer:%s, Grad:%s",tf2.dn,tf2.city);
     DrawTextW(v);
     GotoW(2,y++);memset(v,0,100);sprintf(v,"Ulica i nomer:%s, Telefon:%s, Fax:%s",tf2.street,tf2.tel,tf2.fax);
     DrawTextW(v);
     getch();
     ok=1;
     if (y>10) {clearscreen();y=1;}
    }
   } while (pos2!=EOF);
  }
 } while (pos!=EOF);
 if (!ok) {GotoW(2,1);DrawTextW("Nqma takiva firmi!");getch();}
}

void ErrorExit(char *s)
{
 info(5,15,70,3);
 GotoW(2,1);DrawTextW(s);
 exit(1);
}

char *Enter(char *value,char *enter,int max)
{
 unsigned int key;
 int done=0;
 long pos,i;

 for (i=0,pos=CurPos;i<max;i++) {
  if ((pos+i)%80 == CurX+CurW+1) pos+=80-CurW-1;
  ScreenPointer[pos+i]=toScreen(' ',CurCol);
 }
 if (strcmp(value,"")!=0)
 {
  DrawTextW(value);
  pos=strlen(value);
  if (pos>max) return NULL;
 } else pos=0;
 do
 {
  key=getch();
  if (key==0) key=getch()*256;
  switch (key)
  {
   case K_ENTER:if (strcmp(value,"")!=0) {done=1;CurPos-=pos;}break;
   case K_BS:if (pos>0) {
	      if (CurPos%80 == CurX) CurPos+=CurW+1-80;
	      ScreenPointer[--CurPos]=toScreen(' ',CurCol);
	      value[--pos]=0;
	     }break;
   case K_DEL:for(;pos>0;) {
	       if (CurPos%80 == CurX) CurPos+=CurW+1-80;
	       ScreenPointer[--CurPos]=toScreen(' ',CurCol);
	       value[--pos]=0;
	      }break;
   default:if ((*strchr(enter,key)!=0) && (pos<max))
	    {
	     if (CurPos%80 == CurX+CurW+1) CurPos+=80-CurW-1;
	     ScreenPointer[CurPos++]=toScreen(key,CurCol);
	     value[pos++]=key;
	    }
  }
 } while (!done);
 value[max]=0;
 return value;
}

void OpenFile(void)
{
 if ((ff=fopen(ffname,"rb+"))==NULL)
  if ((ff=fopen(ffname,"wb+"))==NULL) ErrorExit("Can't create file!");
}

void CloseFile(void)
{
 fclose(ff);
}

int FindFirm(long *pos,char *dn)
{
 int found=0;
 long p;
 fseek(ff,*pos,SEEK_SET);
 while ((!feof(ff))&&!found)
 {
  p=ftell(ff);
  if (fread(&firm,sizeof(Firm),1,ff)==0) break;
  if (strcmp(firm.dn,dn)==0) found=1;
 }
 if (feof(ff)) *pos=EOF;
 else *pos=ftell(ff);
//vrashtane na ukazatelq predi namerenia element
 if (found) fseek(ff,p,SEEK_SET);
 return found;
}

void WriteFirm(pFirm pf)
{
 fwrite(pf,sizeof(Firm),1,ff);
 CloseFile();
 OpenFile();
}

void ReadFirm(pFirm pf)
{
 fread(pf,sizeof(Firm),1,ff);
}

void FRemFirm(long pos)
{
 FILE *f;
 if ((f=fopen(FILE_TEMP,"wb+"))==NULL) ErrorExit("Can't create temp file!");
 rewind(ff);
//kopirane s preskachane na elementa koito ne ni triabva
 do
 {
  ReadFirm(&firm);
  if (pos==ftell(ff)) {if (remove(firm.dn)!=0) ErrorExit("Can't delete firm file!");ReadFirm(&firm);}
  if (feof(ff)) break;
  fwrite(&firm,sizeof(Firm),1,f);
 } while (!feof(ff));
 fclose(f);
 CloseFile();
 if (remove(ffname)!=0) ErrorExit("Can't delete temp file!");
 if (rename(FILE_TEMP,ffname)!=0) ErrorExit("Can't rename files!");
 OpenFile();
}

void GetFirm(pFirm pf,long *pos)
{
 fseek(ff,*pos,SEEK_SET);
 fread(pf,sizeof(Firm),1,ff);
 if (feof(ff)) *pos=EOF;
 else *pos=ftell(ff);
}

void ReadtFirm(ptFirm ptf,char *dn)
{
 FILE *f;
 char s[100];
 memset(ptf,0,sizeof(tFirm));
 if ((f=fopen(dn,"rt"))==NULL) ErrorExit("Can't open file!");
 fgets(ptf->dn,DN_LEN+1,f);ptf->dn[strlen(ptf->dn)-1]=0;
 fgets(ptf->name,NAME_LEN+1,f);ptf->name[strlen(ptf->name)-1]=0;
 fgets(ptf->city,CITY_LEN+1,f);ptf->city[strlen(ptf->city)-1]=0;
 fgets(ptf->street,STREET_LEN+1,f);ptf->street[strlen(ptf->street)-1]=0;
 fgets(ptf->tel,TEL_LEN+1,f);ptf->tel[strlen(ptf->tel)-1]=0;
 fgets(ptf->fax,FAX_LEN+1,f);ptf->fax[strlen(ptf->fax)-1]=0;
 memset(s,0,100);fgets(s,15,f);
 sscanf(s,"%u",&ptf->r_year);
 sscanf(s+5,"%u",&ptf->r_day);
 sscanf(s+8,"%u",&ptf->r_mon);
 fclose(f);
}

void WritetFirm(ptFirm ptf,char *dn)
{
 FILE *f;
 char s[100];
 if ((f=fopen(dn,"wt"))==NULL) ErrorExit("Can't create file!");
 fprintf(f,"%s\n",ptf->dn);
 fprintf(f,"%s\n",ptf->name);
 fprintf(f,"%s\n",ptf->city);
 fprintf(f,"%s\n",ptf->street);
 fprintf(f,"%s\n",ptf->tel);
 fprintf(f,"%s\n",ptf->fax);
 fprintf(f,"%04u.%02u.%02u\n",ptf->r_year,ptf->r_day,ptf->r_mon);
 fclose(f);
}


void Spravka(void)
{
 pPid pa=NULL;
 char s[100];
 int k,n=0,ok=0;
 char v[100];

 if (!sort(&pa)) return;
 do {
  info(1,1,80,25);
  if ((pa->next!=NULL) && (n==16 || ok==1)) pa=pa->next;
  ok=0;
  memset(v,0,100);strcpy(v,pa->city);
  GotoW(1,1);memset(s,0,100);sprintf(s,"Grad:%s",v);
  DrawTextW(s);
  GotoW(1,2);DrawTextW("DN       Ime na firmata");
  GotoW(1,3);DrawTextW("Adres                          Data na reg");
  n=1;
  do
  {
   GotoW(1,4+n);DrawTextW(pa->dn);
   GotoW(1+DN_LEN,4+n++);DrawTextW(pa->name);
   GotoW(1,4+n);DrawTextW(pa->street);
   GotoW(1+STREET_LEN,4+n++);
   memset(s,0,100);sprintf(s,"%04u.%02u.%02u",pa->r_year,pa->r_day,pa->r_mon);
   DrawTextW(s);
   n++;
   if ((pa->next!=NULL) && (stricmp(v,pa->next->city)!=0)) {ok=1;break;}
   if ((pa->next!=NULL) && (n!=16)) pa=pa->next;//sledvashtata .. ako ima takava
   else break;
  } while (n!=16);
  GotoW(2,22);DrawTextW("Natisnete klavish za da prodyljite...");getch();
 } while (pa->next!=NULL);
 destroyPid(&pa);
}

/*------------------------------------------------*/
pPid createPid(char *dn,char *name,char *city,char *street,
	       unsigned int r_day,unsigned int r_mon,unsigned int r_year)
{
 pPid tmp;
 if ((tmp=malloc(sizeof(Pid)))==NULL) ErrorExit("Not enough memory");
 tmp->next=NULL;
 tmp->prev=NULL;
 memset(tmp->dn,0,DN_LEN);
 if (dn!=NULL) strcpy(tmp->dn,dn);
 memset(tmp->name,0,NAME_LEN);
 if (name!=NULL) strcpy(tmp->name,name);
 memset(tmp->city,0,CITY_LEN);
 if (city!=NULL) strcpy(tmp->city,city);
 memset(tmp->street,0,STREET_LEN);
 if (street!=NULL) strcpy(tmp->street,street);
 tmp->r_day=r_day;
 tmp->r_mon=r_mon;
 tmp->r_year=r_year;
 return tmp;
}

void destroyPid(pPid *pp)
{
 pPid tmp=*pp;
 if (tmp==NULL) return;
 while (tmp->prev!=NULL) tmp=tmp->prev;
 while(tmp!=NULL)
 {
  *pp=tmp->next;
  free(tmp);
  tmp=*pp;
 }
}

pPid searchPid(pPid pp,char *dn)
{
 pPid tmp=pp;
 if (tmp==NULL) return NULL;
 while (tmp->prev!=NULL) tmp=tmp->prev;
 while (tmp!=NULL)
 {
  if (strcmp(tmp->dn,dn)==0) break;
  tmp=tmp->next;
 }
 return tmp;
}

void flip(pPid pp1,pPid pp2)
{
 Pid tmp;
 memset(&tmp,0,sizeof(Pid));
 strcpy(tmp.dn,pp1->dn);memset(pp1->dn,0,DN_LEN);strcpy(pp1->dn,pp2->dn);memset(pp2->dn,0,DN_LEN);strcpy(pp2->dn,tmp.dn);
 strcpy(tmp.name,pp1->name);memset(pp1->name,0,NAME_LEN);strcpy(pp1->name,pp2->name);memset(pp2->name,0,NAME_LEN);strcpy(pp2->name,tmp.name);
 strcpy(tmp.city,pp1->city);memset(pp1->city,0,CITY_LEN);strcpy(pp1->city,pp2->city);memset(pp2->city,0,CITY_LEN);strcpy(pp2->city,tmp.city);
 strcpy(tmp.street,pp1->street);memset(pp1->street,0,STREET_LEN);strcpy(pp1->street,pp2->street);memset(pp2->street,0,STREET_LEN);strcpy(pp2->street,tmp.street);
 tmp.r_day=pp1->r_day;pp1->r_day=pp2->r_day;pp2->r_day=tmp.r_day;
 tmp.r_mon=pp1->r_mon;pp1->r_mon=pp2->r_mon;pp2->r_mon=tmp.r_mon;
 tmp.r_year=pp1->r_year;pp1->r_year=pp2->r_year;pp2->r_year=tmp.r_year;
}

int sort(pPid *pa)
{
 Firm f;
 tFirm tf;
 pPid tmp;
 long i=0;

 do
 {
  memset(&f,0,sizeof(Firm));
  memset(&tf,0,sizeof(tFirm));
  GetFirm(&f,&i);
  if (i==EOF) break;
  ReadtFirm(&tf,f.dn);
  if (searchPid(*pa,f.dn)==NULL)
  {
   tmp=createPid(f.dn,tf.name,tf.city,tf.street,tf.r_day,tf.r_mon,tf.r_year);
   if (*pa!=NULL)
   {
    (*pa)->next=tmp;
    tmp->prev=*pa;
   }
   *pa=tmp;
  }
 } while (i!=EOF);
 if (*pa==NULL) {info(25,15,30,3);GotoW(2,1);DrawTextW("Nqma firmi za spravkata!!!");return 0;}
 while ((*pa)->prev!=NULL) *pa=(*pa)->prev;
 while (*pa!=NULL)
 {
  tmp=(*pa)->next;
  while (tmp!=NULL)
  {
   if (stricmp(tmp->city,(*pa)->city)<0) flip(tmp,*pa);
   else
    if (stricmp(tmp->city,(*pa)->city)==0)
     if ((tmp->r_year>(*pa)->r_year) || (tmp->r_year==(*pa)->r_year && tmp->r_mon>(*pa)->r_mon)
      || (tmp->r_year==(*pa)->r_year && tmp->r_mon==(*pa)->r_mon && tmp->r_day>(*pa)->r_day)) flip(tmp,*pa);
   tmp=tmp->next;
  }
  if ((*pa)->next!=NULL) *pa=(*pa)->next;
  else break;
 }
 while ((*pa)->prev!=NULL) *pa=(*pa)->prev;
 return 1;
}
/*------------------------------------------------*/

void win(int x, int y, int w, int h,int col, int bkcol)
{
 CurCol=col+16*bkcol;
 CurX=x-1;
 CurY=y-1;
 CurW=w;if (CurW>79) CurW=79;
 CurH=h;if (CurH>24) CurH=24;
 clearscreen();
}

void menu(int x, int y, int w, int h)
{
 _fmemcpy(ScreenPointer,BackGround,SCREEN_BUFFER);
 win(x,y,w,h,mcol,mbkcol);
}

void dialog(int x, int y, int w, int h)
{
 win(x,y,w,h,dcol,dbkcol);
}

void info(int x, int y, int w, int h)
{
 win(x,y,w,h,icol,ibkcol);
}

void checkarg(int argc,char *argv[])
{
 FILE *f;
 int i;
 if (argc>1 && strcmp(argv[1],"")!=0)//ima 1vi parametyr
  {
   if ((f=fopen(argv[1],"rt"))==NULL) ErrorExit("Can't open color data file!");
   fscanf(f,"%u",&i);mcol=i & 0x0f;//file-a s dannite za cvetovete e otvoren
   fscanf(f,"%u",&i);mbkcol=i & 0x07;
   fscanf(f,"%u",&i);dcol=i & 0x0f;
   fscanf(f,"%u",&i);dbkcol=i & 0x07;
   fscanf(f,"%u",&i);icol=i & 0x0f;
   fscanf(f,"%u",&i);ibkcol=i & 0x07;
   fclose(f);
  }
}

void SetCursor(unsigned char a)
{
 union REGS r;
 if (a==CUR_HI)
 {
  r.h.ch=0;
  r.h.cl=7;
 }
 else
  if (a==CUR_LO)
  {
   r.h.ch=6;
   r.h.cl=7;
  }
  else
   if (a==CUR_OFF)
   {
    r.h.ch=32;
    r.h.cl=0;
   }
 r.h.ah=1;
 int86(0x10,&r,&r);
}

unsigned int toScreen(unsigned char c,unsigned char attr)
{
 return (attr*256+c);
}

void clearscreen(void)
{
 unsigned int xx,yy;
 for(yy=CurY;yy<=CurY+CurH;yy++)
  for(xx=CurX;xx<=CurX+CurW;xx++)
   ScreenPointer[xx+yy*80]=toScreen(' ',CurCol);
}

void GotoW(char x,char y)
{
 if (x>CurW) x=CurW;
 if (y>CurH) y=CurH;
 x+=CurX;
 y+=CurY;
 CurPos=x+y*80;
}

void DrawTextW(char *s)
{
 char *t=s;
 char x,y,maxx,maxy;
 if (s==NULL) return;
 x=CurPos%80;maxx=CurX+CurW;
 y=CurPos/80;maxy=CurY+CurH-1;
 while(*t!=0)
 {
  ScreenPointer[CurPos++]=toScreen(*(t++),CurCol);
  if (++x>maxx)
  {
   x=CurX;
   if (++y>maxy) return;
   CurPos=x+y*80;
  }
 }
}
