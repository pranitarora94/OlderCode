/*
Project on Bus Booking
Developed by: Pranit Arora,Ritwik Shokeen,Puneet Rana
*/
#include <fstream.h>
#include <string.h>
#include <conio.h>
#include <ctype.h>
#include <stdio.h>
#include <iomanip.h>
const char TL=218,TR=191,BL=192,BR=217,HL=196,VL=179;
void DispCh(int C,int R,char Ch);
void DispCh(int C,int R,char S[]);
void Color(int TC,int TB);
void Recta(int C1,int R1,int C2,int R2);
int VMenu(int C,int R,char Men[][15],int Noo);
int HMenu(int C,int R,char Men[][15],int Noo);

struct PASSENGER
{
  char Name[20];
  char Address[20];
  char Mobile[20];
};

class BUS
{
  char Date[12],MTime[10],From[20],To[20];
  int RNo,Charge;
  PASSENGER P[5];
public:
  BUS();
  int RRno();
  char *RDate();
  char *RFrom();
  char *RTo();
  int FindSeat();
  void AddPass(int seat);
  void RemovePass(char Mob[20]);
  void DispBus();
  void Schedule();
};

void EditBus()
{
  fstream f;
  f.open("Bus.dat",ios::binary|ios::out|ios::in);
  BUS B;
  int found=0,rno;
  cout<<"Enter Bus Route No.: ";
  cin>>rno;
  while(f.read((char* )&B,sizeof(B)) && found==0)
  {
    if(B.RRno()==rno)
    {
      found++;
      B.DispBus();
      getch();
      clrscr();
      B.Schedule();
      int Pos=f.tellg()-sizeof(B);
      f.seekp(Pos);
      f.write((char*)&B,sizeof(B));
    }
  }
}

void ScheduleBus()                    //binary
{
  fstream f;
  f.open("Bus.dat",ios::binary|ios::app);
  BUS B;
  char Ch;
  do
  {
    B.Schedule();
    f.write((char*)&B,sizeof(B));
    cout<<endl<<"More bus(es) to schedule [Y/N]?";
    cin>>Ch;
  }while (Ch!='N'&& Ch!='n');
  f.close();
}

void DispBus()                        //binary
{
  fstream f;
  f.open("Bus.dat",ios::binary|ios::in);
  BUS B;
  while(f.read((char* )&B,sizeof(B)))
  {
    B.DispBus();        cout<<endl;
  }
  f.close();
}

void DispBusDate()                    //binary
{
  char Date[20];
  cout<<"\tEnter Date of travel: ";
  gets(Date);
  fstream f;
  f.open("Bus.dat",ios::binary|ios::in);
  BUS B;
  int found=0;
  while(f.read((char*)&B,sizeof(B)))
  {
    if(strcmpi(B.RDate(),Date)==0)
    {
      B.DispBus();
      found++;
    }
  }
  if(found==0)
    cout<<endl<<" \t \t No Bus Available"<<endl;
  f.close();
}

void DispBusFromTo()                  //binary
{
  char From[20],To[20];
  cout<<"City of Departure:";gets(From);
  cout<<"Destination      :";gets(To);
  fstream f;
  f.open("Bus.dat",ios::binary|ios::in);
  BUS B;
  int found=0;
  while(f.read((char*)&B,sizeof(B)))
  {
    if(strcmpi(B.RFrom(),From)==0 && strcmpi(B.RTo(),To)==0)
    {
      B.DispBus();
      found++;
    }
  }
  if(found==0)
    cout<<endl<<"Sorry! No Bus Available"<<endl;
  f.close();
}


void BookSeat()                       //binary
{
  fstream f;
  f.open("Bus.dat",ios::binary|ios::in|ios::out);
  char Dep[20],Dest[20],DateT[12];
  BUS B;
  int Sno;
  int Book=0;
  cout<<"City of Departure          :";gets(Dep);
  cout<<"Destination                :";gets(Dest);
  cout<<"Date of Travel [DD-MM-YYYY]:";gets(DateT);
  while (!Book && f.read((char*)&B,sizeof(B)))
  {
    if(strcmpi(B.RFrom(),Dep)==0
       && strcmpi(B.RTo(),Dest)==0
       && strcmpi(B.RDate(),DateT)==0)
    {
      Book++;
      B.DispBus();
      Sno=B.FindSeat();
      if (Sno!=-1)
      {
	Book++;
	cout<<">> Seat no:"<<Sno+1<<" is available..."<<endl;
	char Option;
	cout<<">> Do you want to book now [Y/N]?";cin>>Option;
	if (Option=='Y')
	{
	  Book++;
	  B.AddPass(Sno);
	  int Pos=f.tellg()-sizeof(B);
	  f.seekp(Pos);
	  f.write((char*)&B,sizeof(B));
	}
      }
    }
  }
  cout<<endl;
  if (Book==0)
    cout<<">> Sorry! No bus available"<<endl;
  else if(Book==1)
    cout<<">> Sorry! No seat available"<<endl;
  else if(Book==2)
    cout<<">> Thank you.Please visit again!"<<endl;
  else
    cout<<">> Thank you! Your booking has been confirmed."<<endl;
}

void CancelSeat()                     //binary
{
  fstream f;
  f.open("Bus.dat",ios::binary|ios::in|ios::out);
  BUS B;
  char Dep[20],Dest[20],DateT[12],Mob[20];
  cout<<"City of Departure:";gets(Dep);
  cout<<"Destination      :";gets(Dest);
  cout<<"Date of Travel   :";gets(DateT);

  while (f.read((char*)&B,sizeof(B)))
  {
    if(strcmpi(B.RFrom(),Dep)==0
       && strcmpi(B.RTo(),Dest)==0
       && strcmpi(B.RDate(),DateT)==0)
    {
      B.DispBus();
      cout<<"Passenger mobile : ";
      gets(Mob);
      B.RemovePass(Mob);
      int Pos=f.tellg()-sizeof(B);
      f.seekp(Pos);
      f.write((char*)&B,sizeof(B));
    }
  }
}

BUS::BUS()
{
  for(int i=0;i<5;i++)
  {
    P[i].Name[0]='\0';
    P[i].Address[0]='\0';
    P[i].Mobile[0]='\0';
    RNo=-1;
  }
}

int BUS::RRno()
{
  return RNo;
}
char* BUS::RDate()
{
  return Date;
}

char* BUS::RFrom()
{
  return From;
}

char* BUS::RTo()
{
  return To;
}

int BUS::FindSeat()
{
  int found=0,i=0;
  while(i<5 && found==0)
  {
    if(P[i].Name[0]=='\0')
      found++;
    i++;
  }
  if(found==0)
    return -1;
  else
    return (i-1);
}

void BUS::AddPass(int seat)
{
  cout<<">>Seat No        : "<<seat+1<<endl;
  cout<<"Passenger name   : ";gets(P[seat].Name);
  cout<<"Passenger address: ";gets(P[seat].Address);
  cout<<"Passenger mobile : ";gets(P[seat].Mobile);
}

void BUS::RemovePass(char Mob[20])
{
  int i=0,Canc=0;
  while(i<5)
  {
    if(strcmpi(P[i].Mobile,Mob)==0)
    {
      P[i].Name[0]='\0';
      P[i].Address[0]='\0';
      P[i].Mobile[0]='\0';
      Canc++;
      cout<<"\t Your Booking of Seat "<<i+1<<" has been cancelled !"<<endl;
    }
    i++;
  }
  if(Canc==0)
  {
     cout<<endl<<"\t\t\t\t Seat not found";
  }
}

void BUS::DispBus()
{
  cout<<setw(5)<<RNo<<setw(13)<<Date<<setw(7)<<"%"<<MTime<<"%"
      <<setw(15)<<From<<setw(15)<<To<<endl;
  cout<<"Passengers: ";
  for(int i=0;i<5;i++)
  {
    if(P[i].Mobile[0]!='\0')
      cout<<" ["<<i+1<<"] "<<P[i].Name<<"-"<<P[i].Mobile;
  }
  cout<<endl;
}

void BUS::Schedule()
{
  cout<<"Route no.     :";cin>>RNo;
  cout<<"MTime         :";gets(MTime);
  cout<<"Date          :";gets(Date);
  cout<<"Departure city:";gets(From);
  cout<<"Destination   :";gets(To);
  cout<<"Charge        :";cin>>Charge;
  DispBus();
}

void main()
{
  clrscr();
  Color(RED,BLACK);
  clrscr();
  //While specifying the options, keep each option's no. of char same
  char Menu[][15]={"Admin  ",
		   "User   ",
		   "Quit   "};
  char SMenu[][15]={"Schedule Bus  ",
		    "Edit Buses    ",
		    "Display Buses ",
		    "Buses by Date ",
		    "Buses From/To ",          //Here
		    "Quit          "};
  char UMenu[][15]={"Display Buses ",
		    "Buses by Date ",
		    "Buses From/To ",
		    "Book Seat     ",
		    "Cancel seat   ",
		    "Quit          "};
  int Quit=0;
  do
  {
    clrscr();
    //Display Main Menu1
    int N=HMenu(1,1,Menu,3),SN;
    switch (N)
    {
    case 0://Displaying Vertical Popup menu
	 SN=VMenu(1,2,SMenu,6);
	 switch(SN)
	 {
	   case 0://Call the Function for SubOption1
		  clrscr();
		  ScheduleBus();
		  break;
	   case 1:clrscr();
		  EditBus();
		  break;
	   case 2:clrscr();
		  DispBus();
		  break;
	   case 3:clrscr();
		  DispBusDate();
		  break;
	   case 4:clrscr();
		  DispBusFromTo();
		  break;
	 }
	 getch();
	 break;
    case 1:
	SN=VMenu(1,2,UMenu,6);
	switch(SN)
	{
	  case 0://Call the Function for SubOption1
		  clrscr();
		  DispBus();
		  break;
	  case 1: clrscr();
		  DispBusDate();
		  break;
	  case 2: clrscr();
		  DispBusFromTo();
		  break;
	  case 3: clrscr();
		  BookSeat();
		  break;
	  case 4: clrscr();
		  CancelSeat();
		  break;
	 }
	 getch();
	 break;
    case 2:DispCh(30,10,"Thank you");
	 Quit=1;
    }
    getch();
  }
  while (!Quit);

  getch();
}
void DispCh(int C,int R,char Ch)
{
  gotoxy(C,R);cprintf("%c",Ch);//C 1-80 R 1-25
}

void DispCh(int C,int R,char S[])
{
  gotoxy(C,R);cprintf("%s",S);
}

void Color(int TC,int TB)
{
  textcolor(TC);
  textbackground(TB);
}

void Recta(int C1,int R1,int C2,int R2)
{
  DispCh(C1,R1,TL);
  DispCh(C2,R1,TR);
  DispCh(C1,R2,BL);
  DispCh(C2,R2,BR);
  for (int i=C1+1;i<C2;i++)
  {
    DispCh(i,R1,HL);
    DispCh(i,R2,HL);
  }
  for (i=R1+1;i<R2;i++)
  {
    DispCh(C1,i,VL);
    DispCh(C2,i,VL);
  }
}

int VMenu(int C,int R,char Men[][15],int Noo)
{
  Color(RED,BLACK);
  Recta(C,R,C+strlen(Men[0])+1,R+Noo+1);//Drawing a rectangle enclosing options
  for (int i=0;i<Noo;i++)
    DispCh(C+1,R+i+1,Men[i]);           //Displaying All options

  int Exit=0,Sel=0;

  do
  {
    int Tsel=Sel;
    Color(BLACK,RED);          //Changing color for highlighting
    DispCh(C+1,R+Sel+1,Men[Sel]);//Redisplaying the selected option with changed color
    Color(RED,BLACK);          //Changing color to normal
    char Ch=getch();             //Accepting option from user
    switch(Ch)
    {
    case 71:Sel=0;break;        //Home -> Move to first option
    case 79:Sel=Noo-1;break;    //End  -> Move to Last option
    case 72:if (Sel==0)         //Up   -> Checking if already on top
	      Sel=Noo-1;        //        Move to the last option
	    else                //
	      Sel--;            //        Move one option up
	    break;
    case 80:if (Sel==Noo-1)     //Down -> Checking if already at bottom
	      Sel=0;            //        Move to the first option
	    else                //
	      Sel++;            //        Move one step down
	    break;
    case 27:Sel=-1;      //Escape-> Getting out of the Menu without valid selection
    case 13:Exit=1;      //Enter-> Getting out of the Menu with current selection
    }
    DispCh(C+1,R+Tsel+1,Men[Tsel]);//Redisplaying the non-highlighted option
  }
  while (!Exit);
  return Sel;             //Returning the selected value from menu
}

int HMenu(int C,int R,char Men[][15],int Noo)
{
  int Exit=0,Sel=0;
  Color(RED,BLACK);
  for (int i=0;i<Noo;i++)
    DispCh(C+10*i+1,R,Men[i]);
  do
  {
    int Tsel=Sel;
    Color(BLACK,RED);
    DispCh(C+10*Sel+1,R,Men[Sel]);
    Color(RED,BLACK);
    char Ch=getch();
    switch(Ch)
    {
    case 71:Sel=0;break;     //Home
    case 79:Sel=Noo-1;break; //End
    case 75:if (Sel==0)      //Left
	  Sel=Noo-1;
	else
	  Sel--;
	break;
    case 77:if (Sel==Noo-1)  //Right
	  Sel=0;
	else
	  Sel++;
	break;
    case 27:Sel=-1;    //Escape
    case 13:Exit=1;   //Enter
    }
    DispCh(C+10*Tsel+1,R,Men[Tsel]);
  }
  while (!Exit);
  return Sel;
}
