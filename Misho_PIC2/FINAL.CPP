#include<conio.h>
#include<stdio.h>
#include<IO.h>
#include<dos.h>
#include<iostream.h>
#include<iomanip.h>
#include<string.h>
#include<process.h>
#include<fstream.h>
#include<math.h>
#include<dir.h>
#define LEFT 331
#define RIGHT 333
#define UP 328
#define DOWN 336
#define ENTER 13
#define ESC 27
#define BSPC 8

#define CURSOR_HI  1
#define CURSOR_LO  2
#define CURSOR_OFF 3


char Date[11],DataBase_File[256];



char far *vRAM=(char far*)0xB8000000;
int getKey(void);
void set_Main_Menu_attr(char);
char get_Main_Menu_attr(void);
void set_Window_attr(char);
char get_Window_attr(void);
void set_BOX_attr(char);
char get_BOX_attr(void);
void set_BOX_select_attr(char);
void get_BOX_select_attr();
void color_menu();
void save_to_File();
char background_color_menu();
char letter_color_menu();
void refresh();
void SetCursor(unsigned char);
void Enter_Date();
int  convert(char *string, int s1, int s2);
float convert_f(char *string);
int check_Month();
int Open_DataBase();
int  Add_Firm();
int Edit_Firm();
char *get_DN();
char *get_Date();
char *get_FName();
char *get_SName();
char *get_Family();
int check_DN(char * DN);
int check_Date( char *d1, char *d2);
int Valid_DN(char * DN);
int Valid_Date(char *s1);
char *get_Dnumber();
char *get_Name();
char *get_City();
char *get_Street();
float *get_Money();
int years();
void Save_To_TXT();
void Save_To_BIN();
int Remove_BIN(char *);
void Remove_TXT(char *);
void SetFlag(int);
int GetFlag();
//***************************************************************************
class Color
{
char Main_Menu_attr;
char Window_attr;
char Button_attr,Button_select_attr;
char Pole_attr, Pole_select_attr;

public:
Color(ifstream &in);
~Color(){;}

void set_Main_Menu_attr(char attr )
	{ Main_Menu_attr=attr; }

char get_Main_Menu_attr(void)
	{ return Main_Menu_attr; }

void set_Window_attr(char attr)
		{ Window_attr=attr; }

char get_Window_attr(void)
	{ return Window_attr; }

void set_Pole_attr(char attr)
	{ Pole_attr=attr; }

void set_Pole_select_attr(char attr)
	{ Pole_select_attr=attr;}

char get_Pole_attr()
	{ return Pole_attr;}

char get_Pole_select_attr()
	{return Pole_select_attr;}

void set_Button_attr(char attr)
	{ Button_attr=attr; }

void set_Button_select_attr(char attr)
	{ Button_select_attr=attr;}

char get_Button_attr()
	{ return Button_attr;}

char get_Button_select_attr()
	{return Button_select_attr;}
};

Color *color_p;

//---------------------------------------------------------------------------

Color::Color(ifstream &in)
{
if(in){
in>>Main_Menu_attr;
in>>Window_attr;
in>>Button_attr;
in>>Button_select_attr;
in>>Pole_attr;
in>>Pole_select_attr;
in.close();

}
else{
Main_Menu_attr=0x30;
Window_attr=0x1C;
Button_attr=0x40;
Button_select_attr=0x70;
Pole_attr=0x60;
Pole_select_attr=0x70;
}
}

//***************************************************************************
class Window
{
char *backup;
char attr;
protected:
int _x1,_x2,_y1,_y2;
public:
Window(int x1, int y1, int x2, int y2);
~Window();
char background_color_menu();
char Letter_color_Menu();
void color_menu();
void refresh();
int Add_Firm();
int Edit_Firm();
void Remove_Menu();
int Spravka();
int Sort();
};
//---------------------------------------------------------------------------

Window::Window(int x1, int y1, int x2, int y2)
{
int r,c;

if(!(backup=new char[(x2-x1)*(y2-y1)*4]))
	cout<<"É∞•∏™† Ø∞® ß†§•´ø≠•≤Æ ≠† Ø†¨•≤!";    //Allocation Error;
_x1=x1; _x2=x2; _y1=y1; _y2=y2;
attr=color_p->get_Window_attr();

for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)
		{
		backup[r*160+c*2]=vRAM[r*160+c*2];
		backup[r*160+c*2+1]=vRAM[r*160+c*2+1];
		}

for(r=_y1;r<_y2;r++)
	{
	delay(10);
	for(c=_x1;c<_x2;c++)
		{
		vRAM[r*160+2*c]=' ';
		vRAM[r*160+2*c+1]=attr;
		}
	}

}
//---------------------------------------------------------------------------

Window::~Window()
{
register r,c;

for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)
		{
		vRAM[r*160+c*2]=backup[r*160+c*2];
		vRAM[r*160+c*2+1]=backup[r*160+c*2+1];
		}
delete [] backup;
}


//***************************************************************************

class Main_Menu : public Window
{
char attr;
public:
Main_Menu(int x1, int y1, int x2, int y2);
~Main_Menu()
{;}
void Menu_init(void);
void refresh();
};
Main_Menu *pointer=NULL;
//---------------------------------------------------------------------------
Main_Menu::Main_Menu(int x1, int y1, int x2, int y2 ) : Window(x1,y1,x2,y2)
{
register r,c;

attr=color_p->get_Main_Menu_attr();

for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)
		{
		vRAM[r*160+2*c]=' ';
		vRAM[r*160+2*c+1]=attr;
		}
}

//---------------------------------------------------------------------------

void Main_Menu::refresh()
{
register r,c;

attr=color_p->get_Main_Menu_attr();

for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)

		vRAM[r*160+2*c+1]=attr;



}

//***************************************************************************

class Button
{
char attr,select_attr;
protected:
int _x1,_x2,_y1,_y2;
public:
Button(int x1, int y1, int x2, int y2, char *string);
Button()
{ ; }
void refresh();
friend void change_attr(Button *object1, Button *object2=NULL);
};
//---------------------------------------------------------------------------

Button::Button(int x1, int y1, int x2, int y2, char *string)
{
register r,c;

_x1=x1; _y1=y1; _x2=x2; _y2=y2;
attr=color_p->get_Button_attr();
select_attr=color_p->get_Button_select_attr();

for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)
		vRAM[r*160+2*c+1]=attr;

gotoxy(_x1+(_x2-_x1)/2-strlen(string)/2,_y2);
cout<<string;

}

//---------------------------------------------------------------------------

void Button::refresh()
{
register r,c;

attr=color_p->get_Button_attr();

for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)
		vRAM[r*160+2*c+1]=attr;

}

//***************************************************************************

class Pole
{
int _x1,_x2,_y1,_y2;
char attr,select_attr;
public:
Pole(int x1, int y1, int x2, int y2);
~Pole()
{;}
void refresh();
void Control(char *, int );
};

//---------------------------------------------------------------------------

Pole::Pole(int x1, int y1, int x2, int y2)
{
register r,c;
_x1=x1; _y1=y1; _x2=x2; _y2=y2;
attr=color_p->get_Pole_attr();
select_attr=color_p->get_Pole_select_attr();
for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)
		{
		vRAM[r*160+2*c+1]=attr;
		vRAM[r*160+2*c]=' ';
		}
}

//***************************************************************************


class Error_BOX : public Window
{
public:
Error_BOX(int x1, int y1, int x2, int y2) : Window(x1,y1,x2,y2)
{
gotoxy(_x1+(x2-x1)/2-7,y1+2);
cout<<"*** É ê Ö ò ä Ä *** \a";             //Error
Button OK(_x1+(_x2-_x1)/2-6,_y2-2,_x1+(_x2-_x1)/2+6,_y2-1,"O.K");
}
~Error_BOX(){;}
friend void operator<<(Error_BOX &object, char *string);
void control(void);
};
//---------------------------------------------------------------------------

void Error_BOX::control(void)
{
while(getKey()!=ENTER);
}

//---------------------------------------------------------------------------
void operator<<(Error_BOX &object, char *string)
{
int i=object._x1+3, j=object._y1+4;
gotoxy(object._x1+3,object._y1+4);
while((*string!=NULL)&&(j<=object._y2-3))
	{
	if(i>=object._x2-3)
		{  j++;	gotoxy(object._x1+3,j);i=object._x1+3; }
	cout<<*string;
	string++;
	i++;
	}
}
//***************************************************************************

class Half_obj
{
char DNumber[9];
char City[20];
char Street[40];
char regDate[11];
char FName[61];
public:
Half_obj(){
	strset(DNumber,0);
	strset(Street,0);
	strset(FName,0);
	strset(City,0);
	strset(regDate,0);
	}
~Half_obj(){;};
char *getDN(){return DNumber;}
char *getcity() {return City; }
char *getDate() {return regDate; }
char *getFName() {return FName; }
char *getADDR() {return Street; }
int operator==(Half_obj &object);

friend void operator>>(ifstream &stream, Half_obj &object);

};

//--------------------------------------------------------------------------

int Half_obj::operator==(Half_obj &object)

{
if(strcmp(this->City,object.City)<0)
	return (0);
if(!strcmp(this->City,object.City))
	{
	if(check_Date(this->regDate,object.regDate))
		return (1);
	else
		return (0);
	}

if(strcmp(this->City,object.City)>0)
	return (1);

}

//***************************************************************************

class Firma_bin
{
char DNumber[9];
char FirstName[20];
char SurName[20];
char Family[20];
float money[10];
public:
Firma_bin();
~Firma_bin()
{;}
char *get_FName() { return FirstName; }
char *get_SName() { return SurName; }
char *get_Family() { return Family; }
char *get_DN() { return DNumber; }
float *get_Money() {return money; }
int Save_To_BIN();
friend ostream &operator<<(ostream &stream, Firma_bin &object);

int operator==(Half_obj &object)
{
if(!strcmp(this->get_DN(),object.getDN()))
	return 1;
else
	return 0;
}

};

//---------------------------------------------------------------------------

Firma_bin::Firma_bin()
{
register i;
strset(DNumber,0);
strset(FirstName,0);
strset(SurName,0);
strset(Family,0);
for(i=0;i<10;i++)
	money[i]=0.;
}
//***************************************************************************

class Firma_txt
{
char DNumber[9];
char FName[61];
char City[20];
char Street[40];
char regDate[11];
char Phone[12];
char Fax[12];
public:
Firma_txt()
{
strset(DNumber,0);
strset(FName,0);
strset(City,0);
strset(Street,0);
strset(regDate,0);
strset(Phone,0);
strset(Fax,0);
}
~Firma_txt()
{;}
char *get_DNumber() { return DNumber; }
char *get_Name() { return FName; }
char *get_Date() { return regDate; }
char *get_City() { return City; }
char *get_Street() {return Street; }
char *get_Phone() {return Phone; }
char *get_Fax() {return Fax; }
int years();
int Save_To_TXT();
friend void operator>>(ifstream &stream,Firma_txt &object);
friend ostream &operator<<(ostream &stream, Firma_txt &object);
int Firma_txt::operator==(Firma_txt &object);
};


//***************************************************************************

template <class data> class List
{
data DATA;
List *next;
public:
List(data);
~List(){};
void add(List *node){ node->next=this; next=NULL; }
List<data> sort();
List *get_next() { return next; }
data getData() { return DATA; }
void Destroy();
friend int operator<<(ostream &stream, List<data> &object);
};

//---------------------------------------------------------------------------

template <class data> List<data>::List(data param){
DATA=param;
next=NULL;
}

//---------------------------------------------------------------------------

template <class data> void List<data>::Destroy()
{
List *pointer=NULL,*temp=NULL;

for(pointer=this->next;pointer!=NULL;)
	{
	temp=pointer;
	pointer=pointer->next;
	delete temp;
	}
this->next=NULL;
}

//---------------------------------------------------------------------------

template <class data> List<data> List<data>::sort()
{

Half_obj temp;
List<Half_obj> Head_sort(temp);
List<Half_obj> *t=NULL,*u=NULL,*x=NULL,*b=NULL;

b=&Head_sort; b->next=NULL;
for(t=this->next;t->next!=NULL;t=u)
	{
	u=t->next;
	for(x=b;x->next!=NULL;x=x->next)
		if((x->next->DATA==t->DATA)==1)   break;
	t->next=x->next; x->next=t;

	}
return Head_sort;
}

//***************************************************************************

void Main_Menu::Menu_init()

{
int ch,i=0,flag=0;
Button *mas[8];

Button Menu_1(_x1+5,_y1+1,_x2-5,_y1+2,"ÑÆ°†¢ø≠• ≠† ¥®∞¨†"); //Add Firma
Button Menu_2(_x1+5,_y1+4,_x2-5,_y1+5,"ê•§†™≤®∞†≠• ≠† ®≠¥Æ∞¨†∂®ø≤†");   //redaktirane na informaciqta
Button Menu_3(_x1+5,_y1+7,_x2-5,_y1+8,"àß≤∞®¢†≠• Æ≤ °†ß†≤† §†≠≠®");     // iztrivane ot bazata danni
Button Menu_4(_x1+5,_y1+10,_x2-5,_y1+11,"ëØ∞†¢™† ß† ¥®∞¨®≤• ØÆ¨•π†¢†π® ±• ≠† •§≠Æ ¨ø±≤Æ");     //spravka za firmite na edno mqsto
Button Menu_5(_x1+5,_y1+13,_x2-5,_y1+14,"ëØ∞†¢™† ß† ¢±®∑™® ¥®∞¨® ØÆ †§∞•±≠† ∞•£®±≤∞†∂®ø ");   // spravka za all firmi po grada na registraciq
Button Menu_6(_x1+5,_y1+16,_x2-5,_y1+17,"àß°Æ∞ °†ß† §†≠≠® ");  // izbor baza danni
Button Menu_7(_x1+5,_y1+19,_x2-5,_y1+20,"ç†±≤∞Æ©™† ≠† ∂¢•≤Æ¢•≤•");            //nastroika na cvetovete
Button Menu_8(_x1+5,_y1+22,_x2-5,_y1+23,"àßµÆ§");  // izhod

mas[0]=&Menu_1; mas[1]=&Menu_2; mas[2]=&Menu_3; mas[3]=&Menu_4; mas[4]=&Menu_5; mas[5]=&Menu_6;
mas[6]=&Menu_7; mas[7]=&Menu_8;

change_attr(mas[0]);

do
	{
	ch=getKey();

	if(ch==UP)
	{
		 if(i==0)
			{
			i=7;
			change_attr(mas[7],mas[0]);
			}
		 else
			{
			i--;
			change_attr(mas[i],mas[i+1]);
			}
	}

	if(ch==DOWN)
	{
		if(i==7)
			{
			i=0;
			change_attr(mas[0],mas[7]);
			}
		else
			{
			i++;
			change_attr(mas[i],mas[i-1]);
	}               }
	if(ch==ENTER)
	switch(i)
	{
	case 0: {
		  if(!strlen(DataBase_File)){
		  Error_BOX Error(20,5,60,14);
		  Error<<"ç• • ØÆ±Æ∑•≠ ¥†©´ ± °†ß† §†≠≠®!";  //ne e posochen fail s baza danni
		  Error.control(); break;}
			  Window frame_1(3,0,75,25); frame_1.Add_Firm(); getch(); break;}

	case 1: { if(!strlen(DataBase_File)){
		  Error_BOX Error(20,5,60,14);
		  Error<<"ç• • ØÆ±Æ∑•≠ ¥†©´ ± °†ß† §†≠≠®!";
		  Error.control(); break;}
			Window frame_2(3,0,75,25); frame_2.Edit_Firm(); break;}

	case 2: {   if(!strlen(DataBase_File)){
		  Error_BOX Error(20,5,60,14);
		  Error<<"ç• • ØÆ±Æ∑•≠ ¥†©´ ± °†ß† §†≠≠®!";
		  Error.control(); break;}
			 Window frame_3(5,3,75,10); frame_3.Remove_Menu(); break;}

	case 3: {   if(!strlen(DataBase_File)){
		  Error_BOX Error(20,5,60,14);
		  Error<<"ç• • ØÆ±Æ∑•≠ ¥†©´ ± °†ß† §†≠≠®!";
		  Error.control(); break;}
			Window frame_4(0,0,80,25); frame_4.Sort(); break; }

	case 4: {   if(!strlen(DataBase_File)){
		  Error_BOX Error(20,5,60,14);
		  Error<<"ç• • ØÆ±Æ∑•≠ ¥†©´ ± °†ß† §†≠≠®!";
		  Error.control(); break;}
			Window frame_5(0,0,80,25); frame_5.Spravka();  break; }

	case 5: { Open_DataBase(); break;}

	case 6: { Window frame_6(5,1,70,24); frame_6.color_menu(); flag=1; }
		{ if(!flag) break; pointer->refresh();   register j;
		for(j=0;j<7;j++) mas[j]->refresh(); change_attr(mas[i]); flag=0;}
	}
}while(!(ch==ENTER&&i==7));

}

//***************************************************************************

void change_attr(Button *object1, Button *object2)
{

register r,c;

for(r=object1->_y1;r<object1->_y2;r++)
	for(c=object1->_x1;c<object1->_x2;c++)
	vRAM[160*r+2*c+1]=object1->select_attr;

if(object2!=NULL)
for(r=object2->_y1;r<object2->_y2;r++)
	for(c=object2->_x1;c<object2->_x2;c++)
	vRAM[160*r+2*c+1]=object2->attr;


}
//***************************************************************************

void Window::color_menu()
{
gotoxy((_x2-_x1)/2-7,_y1+1);
cout<<"åÖçû-çÄëíêéâäà çÄ ñÇÖíéÇÖíÖ";          // menu-nastroiki na cvetovete
Button Button1(_x1+3,_y1+1,_x1+30,_y1+2,"ñ¢ø≤ ≠† É´†¢≠Æ≤Æ å•≠æ");       // cvqt na glavnoto menu
Button Button2(_x1+3,_y1+3,_x1+30,_y1+4,"ñ¢ø≤ ≠† è∞ÆßÆ∞∂®≤•");  // cvqt na prozorcite
Button Button3(_x1+3,_y1+5,_x1+30,_y1+6,"í•™±≤ ¢ è∞ÆßÆ∞∂®≤•");  // tekst na proz
Button Button4(_x1+3,_y1+7,_x1+30,_y1+8,"å®£†≠• ≠† Ø∞ÆßÆ∞∂®≤• ");  // migane na proz
Button Button5(_x1+3,_y1+9,_x1+30,_y1+10,"ñ¢ø≤ ≠† °≥≤Æ≠®≤•");      // cvqt na butoni
Button Button6(_x1+3,_y1+11,_x1+30,_y1+12,"í•™±≤ ¢ °≥≤Æ≠®≤•");     // tekst v butoni
Button Button7(_x1+3,_y1+13,_x1+30,_y1+14,"å†∞™•∞ ¢ Å≥≤Æ≠®≤•");    // marker v butoni
Button Button8(_x1+3,_y1+15,_x1+30,_y1+16,"í•™±≤ ¢ å†∞™•∞");      // tekst v marker
Button Button9(_x1+3,_y1+17,_x1+30,_y1+18,"å®£†≠• ≠† °≥≤Æ≠");     //migane na buton
Button Button10(_x1+3,_y1+19,_x1+30,_y1+20,"å®£†≠• ¢ ¨†∞™•∞");    // migane w marker
Button Button11(_x1+33,_y1+1,_x1+60,_y1+2,"ñ¢ø≤ ≠† ØÆ´•≤†≤†");    // cwqt na poletata
Button Button12(_x1+33,_y1+3,_x1+60,_y1+4,"í•™±≤ ¢ ØÆ´•≤†≤†");     // tekst w poletata
Button Button13(_x1+33,_y1+5,_x1+60,_y1+6,"å†∞•™•∞ ¢ ØÆ´•≤†≤†");   // marker w poletata
Button Button14(_x1+33,_y1+7,_x1+60,_y1+8,"í•™±≤ ¢ ¨†∞™•∞");       //tekst w marker
Button Button15(_x1+33,_y1+9,_x1+60,_y1+10,"M®£†≠• ¢ ØÆ´•");      // migane w pole
Button Button16(_x1+33,_y1+11,_x1+60,_y1+12,"å®£†≠• ¢ ¨†∞™•∞ ≠† ØÆ´•");   // migane na marker w pole
Button Button17(_x1+33,_y1+13,_x1+60,_y1+14,"á†Ø†ß® ≠†±≤∞Æ©™®≤•");   // zapazi nastroikite



int ch,i=0;;
Button *mas[17];
mas[0]=&Button1; mas[1]=&Button2; mas[2]=&Button3; mas[3]=&Button4; mas[4]=&Button5;
mas[5]=&Button6; mas[6]=&Button7; mas[7]=&Button8; mas[8]=&Button9; mas[9]=&Button10;
mas[10]=&Button11; mas[11]=&Button12; mas[12]=&Button13; mas[13]=&Button14;
mas[14]=&Button15; mas[15]=&Button16; mas[16]=&Button17;

change_attr(mas[0]);

do
	{
	ch=getKey();

	if(ch==UP)
	{
		 if(i==0)
			{
			i=16;
			change_attr(mas[16],mas[0]);
			}
		 else
			{
			i--;
			change_attr(mas[i],mas[i+1]);
			}
	}

	if(ch==DOWN)
	{
		if(i==16)
			{
			i=0;
			change_attr(mas[0],mas[16]);
			}
		else
			{
			i++;
			change_attr(mas[i],mas[i-1]);
		       }
	}
if(ch==ENTER)
switch(i)
{
char temp;
	 case 0: { Window frame(40,3,60,21); temp=color_p->get_Main_Menu_attr();
		   temp&=0x8F; temp|=frame.background_color_menu(); color_p->set_Main_Menu_attr(temp); break; }

	 case 1: { Window frame(40,3,60,21); temp=color_p->get_Window_attr();

		   temp&=0x8F; temp|=frame.background_color_menu();  color_p->set_Window_attr(temp); break; }
	 case 2: { Window frame(40,1,60,21); temp=color_p->get_Window_attr();
		   temp&=0xF0; temp|=frame.Letter_color_Menu(); color_p->set_Window_attr(temp);  break; }


	 case 3: { temp=color_p->get_Window_attr();
		   temp^=0x8F; color_p->set_Window_attr(temp); break; }

	 case 4: { Window frame(25,3,45,20); temp=color_p->get_Button_attr();
		   temp&=0x8F; temp|=frame.background_color_menu(); color_p->set_Button_attr(temp); break; }

	 case 5: { Window frame(30,1,55,21); temp=color_p->get_Button_attr();
		   temp&=0xF0; temp|=frame.Letter_color_Menu(); color_p->set_Button_attr(temp);  break; }

	 case 6: { Window frame(25,3,45,20); temp=color_p->get_Button_select_attr();
		    temp&=0x8F; temp|=frame.background_color_menu(); color_p->set_Button_select_attr(temp); break; }

	 case 7:  { Window frame(30,1,55,21); temp=color_p->get_Button_select_attr();
		   temp&=0xF0; temp|=frame.Letter_color_Menu(); color_p->set_Button_select_attr(temp);  break; }

	 case 8: { temp=color_p->get_Button_attr();
		   temp^=0x8F; color_p->set_Button_attr(temp); break; }

	 case 9: { temp=color_p->get_Button_select_attr();
		   temp^=0x8F; color_p->set_Button_select_attr(temp); break; }

	 case 10: { Window frame(30,1,55,21); temp=color_p->get_Pole_attr(); temp&=0x8F;
		    temp|=frame.background_color_menu(); color_p->set_Pole_attr(temp); break;}

	 case 11: { Window frame(30,1,55,21); temp=color_p->get_Pole_select_attr();
		   temp&=0xF0; temp|=frame.Letter_color_Menu(); color_p->set_Pole_select_attr(temp);  break; }

	 case 12: { Window frame(30,1,55,21); temp=color_p->get_Pole_select_attr(); temp&=0x8F;
		    temp|=frame.background_color_menu(); color_p->set_Pole_select_attr(temp); break;}

	 case 13: { Window frame(30,1,55,21); temp=color_p->get_Pole_select_attr();
		   temp&=0xF0; temp|=frame.Letter_color_Menu(); color_p->set_Pole_select_attr(temp);  break; }

	 case 14: { temp=color_p->get_Pole_attr();
		   temp^=0x8F; color_p->set_Pole_attr(temp); break; }

	 case 15: { temp=color_p->get_Pole_select_attr();
		   temp^=0x8F; color_p->set_Pole_select_attr(temp); break; }

	 case 16: {save_to_File(); break;}

}

}while(ch!=ESC);

}
//***************************************************************************
char Window::background_color_menu()

{
Button Button1(_x1+3,_y1+2,_x2-3,_y1+3,"ó•∞≠Æ"); //cherno
Button Button2(_x1+3,_y1+4,_x2-3,_y1+5,"ë®≠ºÆ"); //sinio
Button Button3(_x1+3,_y1+6,_x2-3,_y1+7,"á•´•≠Æ"); //zeleno
Button Button4(_x1+3,_y1+8,_x2-3,_y1+9,"ñ®ø≠");  //ciqn
Button Button5(_x1+3,_y1+10,_x2-3,_y1+11,"ó•∞¢•≠Æ");  //red
Button Button6(_x1+3,_y1+12,_x2-3,_y1+13,"å†§¶•≠≤†"); //majenta
Button Button7(_x1+3,_y1+14,_x2-3,_y1+15,"ä†¥ø¢");    //brown
Button Button8(_x1+3,_y1+16,_x2-3,_y1+17,"ë¢•≤´Æ ë®¢Æ");  // swetlo sivo



int ch,i=0;
Button *mas[8];

mas[0]=&Button1; mas[1]=&Button2; mas[2]=&Button3; mas[3]=&Button4; mas[4]=&Button5;
mas[5]=&Button6; mas[6]=&Button7; mas[7]=&Button8;


change_attr(mas[0]);

do
	{
	ch=getKey();

	if(ch==UP)
	{
		 if(i==0)
			{
			i=7;
			change_attr(mas[7],mas[0]);
			}
		 else
			{
			i--;
			change_attr(mas[i],mas[i+1]);
			}
	}

	if(ch==DOWN)
	{
		if(i==7)
			{
			i=0;
			change_attr(mas[0],mas[7]);
			}
		else
			{
			i++;
			change_attr(mas[i],mas[i-1]);
	}               }
	}while(ch!=ENTER && ch!=ESC);


switch(i)
{
	 case 0: { return 0x00; }
	 case 1: { return 0x10; }
	 case 2: { return 0x20; }
	 case 3: { return 0x30; }
	 case 4: { return 0x40; }
	 case 5: { return 0x50; }
	 case 6: { return 0x60; }
	 case 7: { return 0x70; }
}
getch();
}

//***************************************************************************

char Window::Letter_color_Menu()

{
Button Button1(_x1+3,_y1+2,_x2-3,_y1+3,"ó•∞≠Æ");
Button Button2(_x1+3,_y1+3,_x2-3,_y1+4,"ë®≠ºÆ");
Button Button3(_x1+3,_y1+4,_x2-3,_y1+5,"á•´•≠Æ");
Button Button4(_x1+3,_y1+5,_x2-3,_y1+6,"ñ®ø≠");
Button Button5(_x1+3,_y1+6,_x2-3,_y1+7,"ó•∞¢•≠Æ");
Button Button6(_x1+3,_y1+7,_x2-3,_y1+8,"å†§¶•≠≤†");
Button Button7(_x1+3,_y1+8,_x2-3,_y1+9,"ä†¥ø¢");
Button Button8(_x1+3,_y1+9,_x2-3,_y1+10,"ë¢•≤´Æ ë®¢Æ");
Button Button9(_x1+3,_y1+10,_x2-3,_y1+11,"í∫¨≠Æ ë®¢Æ");  //tamno sivo
Button Button10(_x1+3,_y1+11,_x2-3,_y1+12,"ë¢•≤´Æ ë®≠ºÆ"); // svetlo sinio
Button Button11(_x1+3,_y1+12,_x2-3,_y1+13,"ë¢•≤´Æ á•´•≠Æ");  // svetlo zeleno
Button Button12(_x1+3,_y1+13,_x2-3,_y1+14,"ë¢•≤∫´ ñ®ø≠");  //svetal ciqn
Button Button13(_x1+3,_y1+14,_x2-3,_y1+15,"ë¢•≤´Æ ó•∞¢•≠Æ"); //light red
Button Button14(_x1+3,_y1+15,_x2-3,_y1+16,"ë¢•≤´† å†§¶•≠≤†"); //svetla majenta
Button Button15(_x1+3,_y1+16,_x2-3,_y1+17,"Ü∫´≤"); // yellow
Button Button16(_x1+3,_y1+17,_x2-3,_y1+18,"Åø´");  //bql

int ch,i=0;

Button *mas[16];

mas[0]=&Button1; mas[1]=&Button2; mas[2]=&Button3; mas[3]=&Button4; mas[4]=&Button5;
mas[5]=&Button6; mas[6]=&Button7; mas[7]=&Button8; mas[8]=&Button9; mas[9]=&Button10;
mas[10]=&Button11; mas[11]=&Button12; mas[12]=&Button13; mas[13]=&Button14;
mas[14]=&Button15; mas[15]=&Button16;



change_attr(mas[0]);

do
	{
	ch=getKey();

	if(ch==UP)
	{
		 if(i==0)
			{
			i=15;
			change_attr(mas[15],mas[0]);
			}
		 else
			{
			i--;
			change_attr(mas[i],mas[i+1]);
			}
	}

	if(ch==DOWN)
	{
		if(i==15)
			{
			i=0;
			change_attr(mas[0],mas[15]);
			}
		else
			{
			i++;
			change_attr(mas[i],mas[i-1]);
	}               }
	}while(ch!=ENTER && ch!=ESC);


switch(i)
{
	 case 0: { return 0x00; }
	 case 1: { return 0x01; }
	 case 2: { return 0x02; }
	 case 3: { return 0x03; }
	 case 4: { return 0x04; }
	 case 5: { return 0x05; }
	 case 6: { return 0x06; }
	 case 7: { return 0x07; }
	 case 8: { return 0x08; }
	 case 9: { return 0x09; }
	 case 10: { return 0x0A; }
	 case 11: { return 0x0B; }
	 case 12: { return 0x0C; }
	 case 13: { return 0x0D; }
	 case 14: { return 0x0E; }
	 case 15: { return 0x0F; }
}
getch();
}


//***************************************************************************

int Window::Add_Firm()

{

char temp[60];
int flag=0;
Firma_txt txt;
Firma_bin bin,tmp;
const char CH_a_Z_spc[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";

gotoxy(_x1+20,_y1+2);
cout<<"åÖçû - ÑÆ°†¢ø≠• ≠† ¥®∞¨†"; //menu-add firma
gotoxy(_x1+3,_y1+4);
cout<<"Ñ†≠†∑•≠ ≠Æ¨•∞ ≠† ¥®∞¨†≤†-¥Æ∞¨†≤ ëëÑÑÑÑëÑ(ë-£´†¢≠† ´†≤.°≥™¢.Ñ-∂®¥∞†):"; //danachen nomer format
Pole DNumber(_x1+2,_y1+4,_x1+10,_y1+5);
gotoxy(_x1+3,_y1+6);
cout<<"à¨• ≠† î®∞¨†≤†:"; //ima na firmata
Pole Firm_Name(_x1+2,_y1+6,_x1+62,_y1+7);
gotoxy(_x1+3,_y1+8);
cout<<"Ñ†≤† ≠† ê•£®±≤∞†∂®ø:";           // data na registraciq
Pole RDate(_x1+2,_y1+8,_x1+12,_y1+9);
gotoxy(_x1+3,_y1+10);
cout<<"à¨• ≠† Ø∞•ß®§•≠≤† ≠† ¥®∞¨†≤†:"; // ime na prezidanta na firmata
Pole First_Name(_x1+2,_y1+10,_x1+22,_y1+11);
gotoxy(_x1+3,_y1+12);
cout<<"è∞•ß®¨•:";  // prezime
Pole SurName(_x1+2,_y1+12,_x1+22,_y1+13);
gotoxy(_x1+3,_y1+14);
cout<<"î†¨®´®ø: ";           //familiq
Pole Family(_x1+2,_y1+14,_x1+22,_y1+15);
gotoxy(_x1+3,_y1+16);
cout<<"Ä§∞•± ß† ∞•£®±≤∞†∂®ø - <£∞†§>,<≥´®∂† ® ≠Æ¨•∞>";  //adressna registraciq
Pole Address(_x1+2,_y1+16,_x1+62,_y1+17);
gotoxy(_x1+3,_y1+18);
cout<<"<≤•´•¥Æ≠>,<¥†™±>";  //  phome,fax
Pole Phone(_x1+2,_y1+18,_x1+15,_y1+19);
do{

	DNumber.Control(bin.get_DN(),7);
	if(Valid_DN(bin.get_DN()))
		break;
	else
		{
		Error_BOX Error(15,5,50,14);
		Error<<"ç•¢†´®§•≠ ¥Æ∞¨†≤ ≠† Ñ†≠∫∑≠®ø ≠Æ¨•∞!"; //newalioden format DN
		Error.control();
		}
}while(1);

if(check_DN(bin.get_DN()))
	{
	Error_BOX Error(15,7,50,14);
	Error<<"Ç•∑• ±∫π•±≤¢≥¢† ¥®∞¨† ± ≤†™∫¢ §†≠∫∑•≠ ≠Æ¨•∞"; //ve4e sashtestvuva firma s takav DN
	Error.control();
	return 0;
	}

strcpy(txt.get_DNumber(),bin.get_DN());
Firm_Name.Control(txt.get_Name(),60);
do{
	RDate.Control(txt.get_Date(),10);
	if(Valid_Date(txt.get_Date())&&(check_Date(Date,txt.get_Date())))
		break;

	else{
	Error_BOX Error(15,5,45,12);
	Error<<" ç•¢†´®§≠† §†≤† ®´® ≠•¢†´®§•≠ ¥Æ∞¨†≤ ≠† §†≤†≤†"; // Invalid date or format
	Error.control();
	}
}while(1);

First_Name.Control(bin.get_FName(),20);
SurName.Control(bin.get_SName(),20);
Family.Control(bin.get_Family(),20);
do{
	strset(temp,0);
	Address.Control(temp,60);
	if(!strchr(temp,','))
		{
		Error_BOX Error(15,7,50,14);
		Error<<"ç•¢†´®§•≠ ¥Æ∞¨†≤ ≠† †§∞•±†"; //nevaliden format na adresa
		Error.control();
		}
	else{
	strcpy(txt.get_Street(),(strchr(temp,','))+1);
	strset(strchr(temp,','),0);
	strcpy(txt.get_City(),temp);
	break;
		}
}while(1);

do{
	strset(temp,0);
	Phone.Control(temp,21);
	if(!strchr(temp,',')||(strpbrk(temp,CH_a_Z_spc)))
		{
		Error_BOX Error(15,7,50,18);
		Error<<"ç•¢†´®§•≠ ¥Æ∞¨†≤ ≠† í•´•¥Æ≠ ® î†™±"; //nevaliden format na phone i fax
		Error.control();
		}
	else{
	strcpy(txt.get_Fax(),strchr(temp,',')+1);
	strset(strchr(temp,','),0);
	strcpy(txt.get_Phone(),temp);
	break;
	}
}while(1);

if(txt.years())
{
	int i;
	strset(temp,0);
	Pole Money(_x1+2,_y1+20,_x1+12,_y1+21);
	for(i=0;i<txt.years();i++)
		{
		gotoxy(_x1+3,_y1+20);
		cout<<"Ç∫¢•§•≤• Ø•∑†´°† ß† £Æ§®≠†"<<convert(Date,0,4)-i-1; //vsvedete pecalba za godina
		gotoxy(_x1+3,_y1+21);
		Money.Control(temp,10);
		bin.get_Money()[i]=convert_f(temp);
		}
}

if(!bin.Save_To_BIN())
	{
	Error_BOX Error(15,7,50,18);
	Error<<"ÑÆ°†¢ø≠•≤Æ ¢ °†ß†≤† §†≠≠® - çÖìëèÖòçé!"; // dobavqneto w bazata danni - neuspe6no
	Error.control();
	return 0;
	}

if(!txt.Save_To_TXT())
	{
	Error_BOX Error(15,7,50,18);
	Error<<"ë∫ß§†¢†≠•≤Æ ≠† ≤•™±≤Æ¢ ¥†©´ ± ®≠¥. ß† ¥®∞¨† - çÖìëèÖòçé!"; //sazdava na txt file s info za firma- neuspe6no
	Error.control();
	}
return 0;
}

//***************************************************************************

int Window::Edit_Firm()

{

char temp[60],TXT_FILENAME[13];
int flag=0;
Firma_txt txt;
Firma_bin bin,tmp;
const char CH_a_Z_spc[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ ";
int p=0,g=0;


gotoxy(_x1+20,_y1+2);
cout<<"åÖçû - ÑÆ°†¢ø≠• ≠† ¥®∞¨†";
gotoxy(_x1+3,_y1+4);
cout<<"Ç∫¢•§•≤• §†≠∫∑≠®ø≤ ≠Æ¨•∞ ≠† ¥®∞¨†≤†,™Æø≤Æ ®±™†≤• §† ∞•§†™≤®∞†≤•";
Pole DNumber(_x1+2,_y1+4,_x1+10,_y1+5);
gotoxy(_x1+3,_y1+6);
cout<<"à¨• ≠† î®∞¨†≤†:";
Pole Firm_Name(_x1+2,_y1+6,_x1+62,_y1+7);
gotoxy(_x1+3,_y1+8);
cout<<"Ñ†≤† ≠† ê•£®±≤∞†∂®ø:";
Pole RDate(_x1+2,_y1+8,_x1+12,_y1+9);
gotoxy(_x1+3,_y1+10);
cout<<"à¨• ≠† Ø∞•ß®§•≠≤† ≠† ¥®∞¨†≤†:";
Pole First_Name(_x1+2,_y1+10,_x1+22,_y1+11);
gotoxy(_x1+3,_y1+12);
cout<<"è∞•ß®¨•:";
Pole SurName(_x1+2,_y1+12,_x1+22,_y1+13);
gotoxy(_x1+3,_y1+14);
cout<<"î†¨®´®ø: ";
Pole Family(_x1+2,_y1+14,_x1+22,_y1+15);
gotoxy(_x1+3,_y1+16);
cout<<"Ä§∞•± ß† ∞•£®±≤∞†∂®ø - <£∞†§>,<≥´®∂† ® ≠Æ¨•∞>";
Pole Address(_x1+2,_y1+16,_x1+62,_y1+17);
gotoxy(_x1+3,_y1+18);
cout<<"<≤•´•¥Æ≠>,<¥†™±>";
Pole Phone(_x1+2,_y1+18,_x1+15,_y1+19);
do{
	DNumber.Control(bin.get_DN(),7);
	if(Valid_DN(bin.get_DN()))
		break;
	else
		{
		Error_BOX Error(15,5,50,14);
		Error<<"ç•¢†´®§•≠ ¥Æ∞¨†≤ ≠† Ñ†≠∫∑≠®ø ≠Æ¨•∞!";
		Error.control();
		}
}while(1);
fstream FILE(DataBase_File,ios::in|ios::out|ios::binary);
	if(!FILE)
	{
	Error_BOX Error(15,7,50,17);
	Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± °†ß†≤† §†≠≠®"; // reshka pri otvrqneto na file s BD
	Error.control();
	}
do{
FILE.read((char*)&tmp,sizeof(Firma_bin));
	if(!strcmp(tmp.get_DN(),bin.get_DN()))
		{
		flag=1;
		g=FILE.tellg();
		g-=sizeof(Firma_bin);
		break;
		}
}while(FILE.peek()!=EOF);
FILE.close();
if(!flag){
	Error_BOX Error(15,7,50,17);
	Error<<"çø¨† î®∞¨† ± ≤†™∫¢ §†≠†∑•≠ ≠Æ¨•∞!"; // nqma firma s takav DN
	Error.control();
	return 0;
	}

strset(TXT_FILENAME,0);
strcpy(TXT_FILENAME,bin.get_DN());
strcat(TXT_FILENAME,".txt");
ifstream TXT_FILE(TXT_FILENAME,ios::in);
if(!TXT_FILE){
	Error_BOX Error(15,7,50,17);
	Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± ≤•™±≤Æ¢†≤† ®≠¥Æ∞¨†∂®ø"; // gre6ka pri open na faila s txt info
	Error.control();
	return 0;
	}
TXT_FILE>>txt;
Remove_TXT(txt.get_DNumber());
gotoxy(_x1+3,_y1+7);
cout<<txt.get_Name();
gotoxy(_x1+3,_y1+9);
cout<<txt.get_Date();
gotoxy(_x1+3,_y1+11);
cout<<tmp.get_FName();
gotoxy(_x1+3,_y1+13);
cout<<tmp.get_SName();
gotoxy(_x1+3,_y1+15);
cout<<tmp.get_Family();
gotoxy(_x1+3,_y1+17);
cout<<txt.get_City()<<","<<txt.get_Street();
gotoxy(_x1+3,_y1+19);
cout<<txt.get_Phone()<<","<<txt.get_Fax();
gotoxy(_x1+3,_y1+21);
if(!txt.years())
	{
	cout<<"çø¨† ¢∫¢•§•≠® Ø•∑†´°®!"<<endl;  //nqma wawedeni pecalbi
	}
getch();

do{

	DNumber.Control(tmp.get_DN(),7);
	if(Valid_DN(tmp.get_DN()))
		break;
	else
		{
		Error_BOX Error(15,5,50,14);
		Error<<"ç•¢†´®§•≠ ¥Æ∞¨†≤ ≠† Ñ†≠∫∑≠®ø ≠Æ¨•∞!";  //nevaliden format na DN
		Error.control();
		}
}while(1);

if(check_DN(tmp.get_DN()))
	{
	Error_BOX Error(15,7,50,14);
	Error<<"Ç•∑• ±∫π•±≤¢≥¢† ¥®∞¨† ± ≤†™∫¢ §†≠∫∑•≠ ≠Æ¨•∞"; // ve4e sa6testv firma s takav DN
	Error.control();
	return 0;
	}

strcpy(txt.get_DNumber(),tmp.get_DN());
Firm_Name.Control(txt.get_Name(),60);

do{
	RDate.Control(txt.get_Date(),10);
	if(Valid_Date(txt.get_Date())&&(check_Date(Date,txt.get_Date())))
		break;

	else{
	Error_BOX Error(15,5,45,12);
	Error<<" ç•¢†´®§≠† §†≤† ®´® ≠•¢†´®§•≠ ¥Æ∞¨†≤ ≠† §†≤†≤†"; // Nevalidna data ili format
	Error.control();
	}
}while(1);
First_Name.Control(tmp.get_FName(),20);
SurName.Control(tmp.get_SName(),20);
Family.Control(tmp.get_Family(),20);
do{
	strset(temp,0);
	Address.Control(temp,60);
	if(!strchr(temp,','))
		{
		Error_BOX Error(15,7,50,14);
		Error<<"ç•¢†´®§•≠ ¥Æ∞¨†≤ ≠† †§∞•±†";  // nevaliden format na adresa
		Error.control();
		}
	else{
	strcpy(txt.get_Street(),(strchr(temp,','))+1);
	strset(strchr(temp,','),0);
	strcpy(txt.get_City(),temp);
		break;
		}
	}while(1);

do{
	strset(temp,0);
	Phone.Control(temp,21);
	if(!strchr(temp,',')||(strpbrk(temp,CH_a_Z_spc)))
		{
		Error_BOX Error(15,7,50,18);
		Error<<"ç•¢†´®§•≠ ¥Æ∞¨†≤ ≠† í•´•¥Æ≠ ® î†™±"; //nevaliden format na phone i fax
		Error.control();
		}
	else{
	strcpy(txt.get_Fax(),strchr(temp,',')+1);
	strset(strchr(temp,','),0);
	strcpy(txt.get_Phone(),temp);
	break;
		}
	}while(1);
if(txt.years())
	{
		int i;
		strset(temp,0);
		Pole Money(_x1+2,_y1+20,_x1+12,_y1+21);
		for(i=0;i<txt.years();i++)
			{
			gotoxy(_x1+3,_y1+20);
			cout<<"Ç∫¢•§•≤• Ø•∑†´°† ß† £Æ§®≠†"<<convert(Date,0,4)-i; // wuwedete pecalba za godina
			gotoxy(_x1+3,_y1+21);
			Money.Control(temp,10);
			bin.get_Money()[i]=convert_f(temp);

			}
	}
getch();

fstream out(DataBase_File,ios::out|ios::ate|ios::binary);
if(!out){
Error_BOX Error(15,7,50,17);
Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± °†ß†≤† §†≠≠®"; //greshka pri otwarqneto na file s BD
Error.control();
}
else{
out.seekp(g,ios::beg);
out.write((char *)&tmp,sizeof(Firma_bin));
out.close();
txt.Save_To_TXT();
}

return 0;
}

//***************************************************************************

void Window::Remove_Menu()
{

char DNum[9];
strset(DNum,0);
gotoxy(_x1+3,_y1+1);
cout<<"åÖçû - è∞•¨†µ¢†≠• ≠† ¥®∞¨† Æ≤ ™†≤†´Æ£† "; // menu- premahvane na firma ot katalga
gotoxy(_x1+3,_y1+3);
cout<<"Ç∫¢•§•≤• §†≠∫∑•≠ ≠Æ¨•∞ ≠† ¥®∞¨†≤†, ™Æø≤Æ ®±™†≤• §† ®ß≤∞®•≤•"; // vavedete DN na firmata , koqto iskate da
Pole Name(_x1+2,_y1+4,_x1+10,_y1+5);
do{

	Name.Control(DNum,7);
	if(Valid_DN(DNum))
		break;
	else
		{
		Error_BOX Error(15,5,50,14);
		Error<<"ç•¢†´®§•≠ ¥Æ∞¨†≤ ≠† Ñ†≠∫∑≠®ø ≠Æ¨•∞!"; //nevaliden format na DN
		Error.control();
		}
}while(1);

if(!check_DN(DNum)){
	Error_BOX Error(15,7,50,14);
	Error<<"çÖ ±∫π•±≤¢≥¢† ¥®∞¨† ± ≤†™∫¢ §†≠∫∑•≠ ≠Æ¨•∞"; // Ne sa6testvuva firma s takav DN
	Error.control();
	}
else{
	Remove_BIN(DNum);
	Remove_TXT(DNum);
	}
}

//***************************************************************************

int Remove_BIN(char *DN)
{
Firma_bin temp,end;
int p=0,handler=0,g=0;;

fstream FILE(DataBase_File,ios::in|ios::out|ios::binary);
if(!FILE){
Error_BOX Error(15,7,50,17);
Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± °†ß†≤† §†≠≠®! àß≤∞®¢†≠•≤Æ ≠•≥±Ø•∏Æ! "; //Greshka pri otwarqneto na BD! Iztrivaneto neuspeshno
Error.control();
return 0;
}
do{
FILE.read((char*)&temp,sizeof(Firma_bin));
if(!strcmp(temp.get_DN(),DN))
	{
	p=FILE.tellp();
	FILE.seekg(0,ios::end);
	g=FILE.tellp();
	FILE.seekg(g-sizeof(Firma_bin),ios::beg);
	g=FILE.tellg();
	FILE.read((char*)&end,sizeof(Firma_bin));
	g=FILE.tellg();
	FILE.seekp(p-sizeof(Firma_bin),ios::beg);
	p=FILE.tellp();
	FILE.write((char*)&end,sizeof(Firma_bin));
	g=FILE.tellg();
	handler=open(DataBase_File,ios::in|ios::out|ios::binary);
	chsize(handler,filelength(handler)-sizeof(Firma_bin));
	FILE.close();
	break;}
}while(FILE.peek()!=EOF);
FILE.close();

}

//***************************************************************************

void Remove_TXT(char *DN)

{
char FILE[13];
strset(FILE,0);
strcpy(FILE,DN);
strcat(FILE,".txt");
if(remove(FILE)){
	Error_BOX Error(15,7,50,14);
	Error<<"É∞•∏™† Ø∞® ®ß≤∞®¢†≠•≤Æ ≠† ≤•™±≤Æ¢®ø≤ ¥†©´ ± ®≠¥Æ∞¨†∂®ø ß† ¥®∞¨†≤†"; // greshka pri iztrivane na txt fail s inf za firma
	Error.control();
	}
}
//***************************************************************************

int check_Date( char *d1, char *d2)

{
if(convert(d1,0,4)>convert(d2,0,4))
	return 1;
       if(convert(d1,8,10)>convert(d2,8,10))
			return 1;
				if(convert(d1,5,7)>convert(d2,5,7))
					return 1;
						return 0;
}

//***************************************************************************
int Firma_txt::years()
{
if(convert(Date,0,4)-convert(this->regDate,0,4)>10)
	return 10;
else
	return (convert(Date,0,4)-convert(this->regDate,0,4));


}

//***************************************************************************

int Valid_Date(char *s1)
{
const char CH_0_9[]="0123456789";
const int MONTH[12]={31,28,31,30,31,30,31,31,30,31,30,31};

if(s1[4]=='.' && s1[7]=='.'&& (strchr(CH_0_9,s1[0])!=NULL)
	    && (strchr(CH_0_9,s1[1])!=NULL)
	    && (strchr(CH_0_9,s1[2])!=NULL) && (strchr(CH_0_9,s1[3])!=NULL)
	    && (strchr(CH_0_9,s1[5])!=NULL) && (strchr(CH_0_9,s1[6])!=NULL)
	    && (strchr(CH_0_9,s1[8])!=NULL) && (strchr(CH_0_9,s1[9])!=NULL))
	    {
	   if((convert(s1,0,4)>0)&&(convert(s1,5,7)
	<=((MONTH[convert(s1,8,10)-1]+check_Month()))&&(convert(s1,5,7)>=1)&&(convert(s1,8,10)<=12)
	   &&(convert(s1,8,10)>=1)))
	   return 1;
	   else
	    return 0;
	}
else
return 0;
}
//***************************************************************************

int Valid_DN(char *DN)

{

const char CH_0_9[]="0123456789";
const char CH_A_Z[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ";

if(	    (strlen(DN)==8) && (strchr(CH_A_Z,DN[0])!=NULL)
	    && (strchr(CH_A_Z,DN[1])!=NULL) && (strchr(CH_0_9,DN[2])!=NULL)
	    && (strchr(CH_0_9,DN[3])!=NULL) && (strchr(CH_0_9,DN[4])!=NULL)
	    && (strchr(CH_0_9,DN[5])!=NULL) && (strchr(CH_A_Z,DN[6])!=NULL)
	    && (strchr(CH_0_9,DN[7])!=NULL))

	    return 1;
else
	return 0;

}
//***************************************************************************

int Firma_bin::Save_To_BIN()
{
ofstream out(DataBase_File,ios::out|ios::ate|ios::binary);
if(!out){
	Error_BOX Error(15,7,50,17);
	Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± °†ß†≤† §†≠≠®!"; //Error open DB
	Error.control();
	return 0;
	}
else{
out.write((char *)&*this,sizeof(Firma_bin));
out.close();
}
return 1;
}

//***************************************************************************

int Firma_txt::Save_To_TXT()
{
char FNAME[13];
strset(FNAME,0);
strcpy(FNAME,(this->get_DNumber()));
strcat(FNAME,".txt");
ofstream out(FNAME,ios::out);

if(!out){
	Error_BOX Error(15,7,50,17);
	Error<<"É∞•∏™† Ø∞® ±∫ß§†¢†≠•≤Æ ≠† ≤•™±≤Æ¢ ¥†©´"; //greshka pri syzdavane na txt file
	Error.control();
	return 0;
	}
else{
	out<<this->get_DNumber()<<endl;
	out<<this->get_Name()<<endl;
	out<<this->get_City()<<","<<this->get_Street()<<endl;
	out<<this->get_Date()<<endl;
	out<<this->get_Phone()<<","<<this->get_Fax()<<endl;
	out.close();
	}
return 1;
}

//***************************************************************************
int check_DN(char * DN)
{

Firma_bin tmp;

fstream in(DataBase_File,ios::in|ios::out|ios::binary);
if(!in){
	Error_BOX Error(15,7,50,17);
	Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± °†ß†≤† §†≠≠® "; //error open DB
	Error.control();
	return 0;
	}
do
	{
	in.read((char*)&tmp,sizeof(Firma_bin));
	if(!strcmp(DN,tmp.get_DN()))
	      { in.close();  return 1; }
}while(in.peek()!=EOF);
in.close();
return 0;
}
//***************************************************************************

int Window::Sort()
{

Firma_bin bin_temp,bin_current;
Firma_txt txt_temp,txt_current;
int i=1,flag=0;
char TXT_FILENAME[13];

strset(TXT_FILENAME,0);
gotoxy(3,1);
cout<<"åÖçû - à≠¥Æ∞¨†∂®ø ß† ¥®∞¨® ®¨†π® •§®≠ ® ±∫π †§∞•± ®´® ≤•´•¥Æ≠ ®´® ¥†™±";
//menu - info za fiemi imashti edin i sa6ti adress
gotoxy(1,3);

fstream FILE(DataBase_File,ios::in|ios::out|ios::binary);
if(!FILE){
	Error_BOX Error(15,7,50,17);
	Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± °†ß†≤† §†≠≠® "; error open DB
	Error.control();
	return 0;
	}

do{
	FILE.read((char*)&bin_temp,sizeof(Firma_bin));
	strset(TXT_FILENAME,0);
	strcpy(TXT_FILENAME,bin_temp.get_DN());
	strcat(TXT_FILENAME,".txt");
	ifstream TXT(TXT_FILENAME,ios::in);
	if(!TXT){
		Error_BOX Error(15,7,50,17);
		Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± ≤•™±≤Æ¢†≤† ®≠¥Æ∞¨†∂®ø";
		//error open txt file s tekstova informaciq
		Error.control();
		return 0;
		}
	TXT>>txt_temp;
	TXT.close();
	flag=0;
	while(FILE.peek()!=EOF)
		{
		FILE.read((char*)&bin_current,sizeof(Firma_bin));
		strset(TXT_FILENAME,0);
		strcpy(TXT_FILENAME,bin_current.get_DN());
		strcat(TXT_FILENAME,".txt");
		ifstream TXT_FILE(TXT_FILENAME,ios::in);
		if(!TXT_FILE){
			Error_BOX Error(15,7,50,17);
			Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± ≤•™±≤Æ¢†≤† ®≠¥Æ∞¨†∂®ø";
					//error open txt file s tekstova informaciq
			Error.control();
			return 0;
			}
	TXT_FILE>>txt_current;
	TXT_FILE.close();
	if(txt_temp==txt_current)
		{
		cout<<txt_current;
		cout<<bin_current;
		flag=1;
		}
	}
	if(flag){
		cout<<txt_temp;
		cout<<bin_temp;
		}
	FILE.seekg(i*sizeof(Firma_bin),ios::beg);
	FILE.seekp(i*sizeof(Firma_bin),ios::beg);
	i++;
}while(FILE.peek()!=EOF);
FILE.close();
if(!flag)
	{cout<<endl<<"çø¨† ¥®∞¨® ∞•£®±≤∞®∞†≠® ≠† •§®≠ †§∞•±"; getch(); }
	//nqma firmi registrirani na edin adress

}

//***************************************************************************

ostream &operator<<(ostream &stream, Firma_bin &object)
{
int i,flag=0;
if(wherey()>20)
      {	cout<<"ç†≤®±≠•≤• ™´†¢®∏ ß† Ø∞Æ§∫´¶•≠®•...";  getch();} //press a key to continue
stream<<"àåÖ çÄ èêÖáàÑÖçíÄ: "; //ime na prezidenta
stream<<object.FirstName<<endl;
stream<<"èêÖáàåÖ: ";                       // prezime
stream<<object.SurName<<endl;
stream<<"îÄåàãàü: ";                 //familiq
stream<<object.Family<<endl;
for(i=0;i<10;i++){
	if(object.money[i]>0.) {
		stream<<"è•∑†´°† ß† £Æ§®≠† "<<convert(Date,0,4)-i<<": "; //pecalba za godina
		stream<<object.money[i]<<endl;
		flag=1;
		}
	 }
if(!flag)
	cout<<"Ç±• Æπ• ≠ø¨† Ø•∑†´°†!"<<endl<<endl;
	//wse o6te nqma pecalbi
}

//***************************************************************************

ostream &operator<<(ostream &stream, Firma_txt &object)

{
if(wherey()>17)
      {	cout<<"ç†≤®±≠•≤• ™´†¢®∏ ß† Ø∞Æ§∫´¶•≠®•...";  getch();} // press a key to continue
	stream<<"========================================================="<<endl;
	stream<<"ÄÑêÖë:  "; // adress
	stream<<object.City<<","<<object.Street<<"  ";
	stream<<"íÖãÖîéç: ";     // phone
	stream<<object.Phone<<" ";
	stream<<"îÄäë: ";            //fax
	stream<<object.Fax<<endl;
	stream<<"àåÖ çÄ îàêåÄíÄ: ";  // ime na firmata
	stream<<object.FName<<endl;
	stream<<"ÑÄçöóÖç çéåÖê: ";      // danachen nomer
	stream<<object.DNumber<<"  ";
	stream<<"ÑÄíÄ çÄ êÖÉàëíêÄñàü: ";     // data na registracvq
	stream<<object.regDate<<endl;

}

//***************************************************************************

int Firma_txt::operator==(Firma_txt &object)

{

if(!strcmp(this->City,object.City)||!strcmp(this->Street,object.Street)||!strcmp(this->Phone,object.Phone)
	||!strcmp(this->Fax,object.Fax))
	return 1;
else
	return 0;
}

//***************************************************************************

void operator>>(ifstream &stream, Firma_txt &object)

{
char string[60];
strset(string,0);

stream>>object.DNumber;
stream>>object.FName;
stream>>string;
strcpy(object.Street,(strchr(string,','))+1);
	strset(strchr(string,','),0);
	strcpy(object.City,string);
stream>>object.regDate;
strset(string,0);
stream>>string;
	strcpy(object.Fax,strchr(string,',')+1);
	strset(strchr(string,','),0);
	strcpy(object.Phone,string);
}
//***************************************************************************

void operator>>(ifstream &stream,Half_obj &object)

{

char string[60];
stream>>object.DNumber;
stream>>object.FName;
strset(string,0);
stream>>string;
strcpy(object.Street,(strchr(string,','))+1);
	strset(strchr(string,','),0);
	strcpy(object.City,string);
strset(string,0);
stream>>object.regDate;
}

//***************************************************************************
void save_to_File()

{
char FileName[256];

memset(FileName,0,256);
Window Save(3,7,75,14);
Pole Edit(5,9,70,10);
gotoxy(18,9);
cout<<"Ç∫¢•§•≤• ®¨• ≠† ¥†©´: ";  // vavedete ime na fail
Edit.Control(FileName,65);
ofstream out(FileName);
if(!out)
	{
	Error_BOX Error(25,5,65,15);
	Error<<"î†©´∫≤ ≠• ¨Æ¶• §† °†§• ±∫ß§†§•≠! ç†±≤∞Æ©™®≤• ≠• ¨Æ£†≤ §† ±• ß†Ø†ßø≤.";
	// failat ne mojhe da bade sazdaden nastroikite ne mogat da se zapametqt
	Error.control();
	}
else{
out<<color_p->get_Main_Menu_attr()<<endl;
out<<color_p->get_Window_attr()<<endl;
out<<color_p->get_Button_attr()<<endl;
out<<color_p->get_Button_select_attr()<<endl;
out<<color_p->get_Pole_attr()<<endl;
out<<color_p->get_Pole_select_attr()<<endl;
out.close();
}

}
//***************************************************************************

void Pole::Control(char *masiv, int number)
{
int i=0,ch;
register r,c;
for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)
		{
		vRAM[r*160+2*c+1]=select_attr;
		vRAM[r*160+2*c]=' ';
		}



strset(masiv,0);
do
{
ch=getKey();


if(i>=0&&i<=_x2&&i<=number&&ch!=BSPC&&ch!=ESC&&ch!=ENTER&&ch!=UP&&ch!=DOWN&&ch!=LEFT&&ch!=RIGHT)
	{ masiv[i]=ch; vRAM[160*_y1+2*(_x1+i)]=ch; i++; }


if(ch==BSPC&&i>0)
	{i--; ch=' ';  masiv[i]=ch; vRAM[160*_y1+2*(_x1+i)]=ch;}

}while(ch!=ENTER);
masiv[i]='\0';

for(r=_y1;r<_y2;r++)
	for(c=_x1;c<_x2;c++)
		{
		vRAM[r*160+2*c+1]=attr;
		}


}
//***************************************************************************

void SetCursor(unsigned char a)
{
 union REGS r;
 if (a==CURSOR_HI)
 {
  r.h.ch=0;
  r.h.cl=7;
 }
 else
  if (a==CURSOR_LO)
  {
   r.h.ch=6;
   r.h.cl=7;
  }
  else
   if (a==CURSOR_OFF)
   {
    r.h.ch=32;
    r.h.cl=0;
   }
 r.h.ah=1;
 int86(0x10,&r,&r);
}
//***************************************************************************

int Open_DataBase()
{

static char curr_dir[256];
static int flag=0;
char new_dir[61];

if(!flag)
	{
	strset(curr_dir,0);
	getcurdir(0,curr_dir);
	}
strset(new_dir,0);
strset(DataBase_File,0);
Window Open(3,7,75,14);
Pole Edit(5,9,70,10);
gotoxy(18,9);
cout<<"Ç∫¢•§•≤• ®¨•≤Æ ® Ø∫≤ø §Æ ¥†©´† ± °†ß†≤† §†≠≠®: "; //vavedete imeto i patq do fail s BD
Edit.Control(DataBase_File,65);
strcpy(new_dir,DataBase_File);
strset(strchr(new_dir,'.'),0);
if(flag)
	if(chdir(".."))
		{
		Error_BOX Error(25,5,65,15);
		Error<<"É∞•∏™† Ø∞® Ø∞Æ¨ø≠† ≠† §®∞•™≤Æ∞®ø."; // greshka pri promqna na dir
		Error.control();
		strset(DataBase_File,0);
		return 0;
		}

if(chdir(new_dir))
	if(mkdir(new_dir))
		{
		Error_BOX Error(25,5,65,15);
		Error<<"É∞•∏™† Ø∞® ±∫ß§†¢†≠•≤Æ ≠† §®∞•™≤Æ∞®ø."; //greshka pri sazdavaneto na dir
		Error.control();
		strset(DataBase_File,0);
		return 0;
		}
else{
	Error_BOX Error(25,5,65,15);
	Error<<"É∞•∏™† Ø∞® Ø∞Æ¨ø≠† ≠† §®∞•™≤Æ∞®ø."; // greshka pri promqna na dir
	Error.control();
	strset(DataBase_File,0);
	return 0;
	}

flag=1;
return 1;
}

//***************************************************************************

void Enter_Date()

{
const char CH_0_9[]="0123456789";
const int MONTH[12]={31,28,31,30,31,30,31,31,30,31,30,31};

Window Frame(10,17,70,22);
SetCursor(CURSOR_OFF);
memset(Date,0,11);
gotoxy(12,20);
cout<<"Ç∫¢•§•≤• ≤•™≥π† §†≤† ¢∫¢ ¥Æ∞¨†≤† ÉÉÉÉ.ÑÑ.åå"; // vavedete teku6ta data wyw formata
do{
Pole Current_Date(56,19,66,20);
Current_Date.Control(Date,9);
if(Valid_Date(Date))
	break;
else{
	Error_BOX Error(15,5,45,12);
	Error<<" ç•¢†´®§≠† §†≤† ®´® ≠•¢†´®§•≠ ¥Æ∞¨†≤ ≠† §†≤†≤†"; //nevalidna data ili nevaliden format na datata
	Error.control();
	}
}while(1);
}

//***************************************************************************

int convert(char *string, int s1=0, int s2=0)

{
int result=0;
int i,j;

for(i=s1,j=s2-s1-1;i<s2;i++,j--)
result+=(string[i]-48)*pow10(j);
return result;
}

//***************************************************************************

float convert_f(char *string)
{

float result=0.;
int i,j,size=0,pos=0;

for(i=strlen(string)-1,j=0;i>0;i--)
	{
	if(string[i]=='.')
		{
		pos=i;
		continue;
		}

	if(string[i]=='-')
		continue;

	result+=(string[i]-48)*pow10(j);
	j++;
	}
if(string[0]=='-')
	result*=-1;
if(pos)
	result=result/pow10(pos+1);
return result;
}

//***************************************************************************

int check_Month()

{
const int MONTH[12]={31,28,31,30,31,30,31,31,30,31,30,31};

if(convert(Date,5,7));
	if(convert(Date,0,4)%4==0&&convert(Date,0,4)%100!=0&&convert(Date,0,4)%400!=0)
	return 1;
else
return 0;

}

//***************************************************************************

int Window::Spravka()
{

Firma_bin bin_temp;
Half_obj temp;
List<Half_obj> first(temp),sort(temp),*p=NULL,*current=NULL;
char TXT_FILENAME[13];
p=&first;

gotoxy(3,1);
cout<<"åÖçû - à≠¥Æ∞¨†∂®ø ß† ¥®∞¨® ØÆ§∞•§•≠® ØÆ £∞†§† ≠† ∞•£®±≤∞†∂®ø"<<endl<<endl;
//menu- info za firma poderdeni po grada na registraciq
ifstream FILE(DataBase_File,ios::in|ios::binary);
if(!FILE){
	Error_BOX Error(15,7,50,17);
	Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± °†ß†≤† §†≠≠®. éØ•∞†∂®ø≤† Ø∞•™∞†≤•≠†!";
	//erroe open file  DB . operation canceled
	Error.control();
	return 0;
	}

while(FILE){
	FILE.read((char*)&bin_temp,sizeof(Firma_bin));
	strset(TXT_FILENAME,0);
	strcpy(TXT_FILENAME,bin_temp.get_DN());
	strcat(TXT_FILENAME,".txt");
	ifstream TXT(TXT_FILENAME,ios::in);
	if(!TXT){
		Error_BOX Error(15,7,50,17);
		Error<<"É∞•∏™† Ø∞® Æ≤¢†∞ø≠•≤Æ ≠† ¥†©´† ± ≤•™±≤Æ¢†≤† ®≠¥Æ∞¨†∂®ø. éØ•∞†∂®ø≤† Ø∞•™∞†≤•≠†!";
			//erroe open file txt with info . operation canceled
		Error.control();
		return 0;
		}
	TXT>>temp;
	TXT.close();
	current=new List<Half_obj>(temp);
		if(!current){Error_BOX Error(15,7,50,17);
		Error<<"É∞•∏™† Ø∞® ß†§•´ø≠•≤Æ ≠† Ø†¨•≤! è∞Æ∂•±† Ø∞•™∞†≤•≠ !";
		//error aloccation memmory proces canceled
		Error.control();
		return 0;
		}
	current->add(p);
	p=current;
}
sort=first.sort();
cout<<sort;
getch();
first.Destroy();
sort.Destroy();

}

//***************************************************************************
int operator<<(ostream &stream, List<data> &object)
{
Half_obj temp;
char city_temp[20];
strset(city_temp,0);
List<data> *t=NULL,*pointer=NULL;

pointer=&object;

strcpy(city_temp,pointer->next->getData().getcity());
cout<<pointer->next->getData().getcity()<<endl;
cout<<"==============================================================================="<<endl;

for(t=pointer->next;t!=NULL;t=t->next)
	{
	temp=t->getData();
	if(wherey()==21){
		cout<<"ç†≤®±≠•≤• ™´†¢®∏ ß† Ø∞Æ§∫´¶•≠®•!";   // press a key to continue
		getch();
		}
	if(strcmp(city_temp,t->getData().getcity())){
		cout<<endl<<t->getData().getcity()<<endl;
		cout<<"==============================================================================="<<endl;
		strcpy(city_temp,t->getData().getcity());
		}
	cout.width(10);
	cout.setf(ios::left);
	cout<<t->getData().getDN();
	cout.width(35);
	cout<<t->getData().getFName();
	cout.width(20);
	cout<<t->getData().getADDR();
	cout.width(11);
	cout<<t->getData().getDate()<<endl;
	}
}



//***************************************************************************
int getKey(void)
{
int ch;

ch=getch();
if(ch)
	return(ch);
ch=getch();
	return(256+ch);
}
//***************************************************************************

int main(int argc, char *argv[])
{

ifstream instream;

if(argc!=2)

	{
	cout<<"\n èÆ§†©≤• ¥†©´ ± ≠†±≤∞Æ©™®≤• ß† ∂¢•≤Æ¢•≤• Æ≤ ™Æ¨†≠§≠®ø ∞•§ \n";
	cout<<"ç†≤®±≠•≤• ™´†¢®∏ ß† §† Ø∞Æ§∫´¶®≤• ±∫± ±≤†≠§†∞≤≠® ≠†±≤∞Æ©™® ®´® ESC ß† ®ßµÆ§ \n";
	//podaite fail s nastroikite za cvetovete ot komandniq red \n
//        Natisnete klavish da prodaljite sas standartnite nastroiki ili esc za izhod
	if(getKey()==ESC)
		exit(0);
	instream.open(argv[1]);
	}
else
{
instream.open(argv[1]);
if(!instream)
	{
	cout<<"î†©´∫≤ ≠• • ≠†¨•∞•≠! \n";
	cout<<"ç†≤®±≠•≤• ™´†¢®∏ ß† §† Ø∞Æ§∫´¶®≤• ±∫± ±≤†≠§†∞≤≠® ≠†±≤∞Æ©™® ®´® ESC ß† ®ßµÆ§ \n";
	//file not found
	// press a key to continue with standart option
	if(getKey()==ESC)
		exit(0);
	}
}
Color ob(instream);
color_p=&ob;
Enter_Date();
Main_Menu Main_Frame(10,0,70,25);
pointer=&Main_Frame;
Main_Frame.Menu_init();

return 0;
}