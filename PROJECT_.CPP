#include<iostream.h>
#include<iomanip.h>
#include<time.h>
#include<conio.h>
#include<dos.h>
#include<fstream.h>
#include<process.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
#include<graphics.h>

/*
Global Variables:
Error messages, source text, Middle x and y coordinates,
Changed texts, length of source text
*/

char Error_msg[2][60]={
		   "\t Invalid command." ,
		   "\t Invalid Number." ,
		      };

char source_text[200],path[80],changed_text[200];
int length,midx=0,midy=0,Topy=0,Atx=0,Aty=0;
char currentTime[26];
time_t rawTime;

/*   **** GRAPHICS RELATED FUNCTIONS *****         */

// Text management related functions

void nextLine()
{
  gotoxy(Atx,Aty);
  Aty++;
}

void clearArea()
{
  int i;
  Aty=Topy;

  for(i=0;i<20;i++)
   {
     gotoxy(Atx,Aty+i);
     cout<<"\t\t\t\t\t\t\t\t        ";
   }

}

// Introduction graphics

void DrawIntro()
{
  int i,k=0;
  char str[8][50]= { "WELCOME TO PROJECT ALPHA....",
		      "SETTING UP A SECURE CONNECTION...",
		      "SETUP COMPLETE.",
		      "STARTING INTERRFACE.....",
		      "ENGINES READY.",
		      "CIPHERS LOADED.",
		      "ENTERING STARTUP PHASE",
		      "...."
		    };

  char str2[4]= {"/|\-"};

  while(k!=8)
  {
     cout<<"\n\t";
     for(i=0;str[k][i] != '\0';i++)
       {
	cout<<str[k][i];
	delay(70);
       }
    if(k==0)
      {
	cout<<"\n\tPress Enter to continue..";
	getch();
      }
    if(k==1 || k==3)
      {
	for(i=0;i<50;i++)
	 { cout<<str2[i%4]<<"\b";
	   delay(100);
	 }
	cout<<"\b";
      }
    else
      delay(200);

    k++;
    if(k<3)
      cout<<"\n\n\t";
  }

}


void DrawInterface()
{
  int i,x=2*midx-200;
  for(i=0;i<200;i++)
   {
    setfillstyle(1,WHITE);
    bar3d(0,0,x+i,midy/6,2,0);
    setfillstyle(1,LIGHTGRAY);
    bar(0,2*(midy)-40-2*i,100,2*midy);
    delay(10);
   }
}

//
void drawCipherTitle(char txt[],int n)
{
  int i,h,k;
  k=2*midy-438;        // 438 is the end pt of left bar

  if(n==0)
    setfillstyle(1,LIGHTBLUE);
  else if(n==1)
    setfillstyle(1,CYAN);
  else if(n==2)
    setfillstyle(1,LIGHTRED);
  else if(n==3)
    setfillstyle(1,LIGHTGRAY);
  else if(n==4)
    setfillstyle(1,WHITE);
  else
    setfillstyle(1,YELLOW);

  setcolor(BLACK);
  bar3d(100,k,2*midx,k+20,5,1);


  settextstyle(0,0,2);
  outtextxy(midx/2 + 100,k+2,txt);
}

void DrawSideBar(int x)
{
  int i;
  for(i=0;i<100;i++)
   {
     setfillstyle(9,RED);
     bar3d(0,midy/6 + 10,2+i/2,2*midy-50,1,0);
     delay(10);
   }

  setcolor(WHITE);
  settextstyle(3,1,4);
  if(x==1)
    outtextxy(0,midy/2,"ENCRYPTION");
  else if(x==0)
    outtextxy(0,midy/2,"DECRYPTION");

}

void RemoveSideBar()
{
  setfillstyle(1,LIGHTGRAY);
  for(int i=0;i<25;i++)
   bar(0,midy/6+4,4*i,2*midy);
}

/*
**** STRUCTURE AND QUEUE ****
Used for keeping history details.
*/

struct entry
{
  char time[27],type[11],job[11];

  void display()
  {
    cout<<setw(10)<<job<<"\t"<<setw(7)<<type;
    cout<<"\t"<<time;
  }
};

// Circular Queue housing the history details

class CircularQueue
{
  int front,rear,n;
  entry a[4];

  public:
  CircularQueue()
  {
    front=rear=-1;
    n=4;
  }

void Push(char Time[],char Type[],char Job[])
  {
     if( (front==0 && rear==n-1) ||  front==rear+1)
	{
	  if(front == n-1)
	    front=0;
	  else
	    front++; 			
	}
     if(rear==-1)
       {
	 front=0; rear=0;
       }
     else if(rear==n-1)
	 rear=0;
     else
	 rear++;

    strncpy(a[rear].time,Time,26);
    strcpy(a[rear].type,Type);
    strcpy(a[rear].job,Job);
  }

  void ShowQueue();
}history;

void CircularQueue::ShowQueue()
{
   clearArea();
   int k=1;
   drawCipherTitle("  History",4);

   nextLine();
   cout<<"\t    PROCESS   \t   TYPE  \t  DATE AND TIME";
   nextLine();

  if(front!=-1)
  {
    for(int i=front;i<=rear;k++,i++)
    {
     nextLine(); Aty++;
     cout<<"\t "<<k;
     a[i].display();

     if(i==n-1 && rear != n-1)
     i=-1;
    }
  }
  else
  {
     nextLine();
     cout<<"\t No recent commands given.";
  }

  nextLine();
  cout<<"Press Enter to continue...";
  getch();
}

/*   ****  ERROR HANDLER  **** */

int int_handler(int low,int up)
{
  int in;
  cin>>in;

  if(in>=low && in<=up)
   return(in);

  else
  {
    nextLine(); Aty++;
    cout<<Error_msg[1];
    nextLine();
    cout<<"\t You are showing suspicious behaviour.";
    nextLine();
    cout<<"\t Terminating session for security reasons.";
    nextLine();
    cout<<"  Press any key to exit...";
    getch();
    exit(0);
  }
}
/* **** FILE OUTPUT FUNCTION **** */

void getOutput_file(char* str)
{
  ofstream f;

  f.open("output.txt",ios::out);
  f.write("\n\n  Changed text : \n\n",20);
  f.write(" state ",7);
  f.write(str,length);

  f.close();
}

/*    **** MAIN ENCRYPTION/DECRYPTION RELATED FUNCTIONS **** */

// XOR CIPHER

void XorCipher(int control)
{
  char ch,key;
  int i;
  drawCipherTitle("Xor Cipher",0);
  clearArea();
  nextLine();
  cout<<"\t #Xor# Engine Initiated";
  nextLine();

  nextLine();
  cout<<"\t Enter Text (200 characters max): ";
  nextLine();
  cout<<"\t ";
  gets(source_text);
  length = strlen(source_text);
  nextLine();  Aty++;
  cout<<"\t Enter key : ";
  cin>>key;

  for(i=0;source_text[i] != '\0';i++)
    changed_text[i] = source_text[i]^key;

  nextLine();
  cout<<"\t Do you want to output the changed text to a file (Y/N) ? : ";
  cin>>ch;

  time(&rawTime);
  strncpy(currentTime,ctime(&rawTime),26);
  nextLine();

  if(ch=='Y' || ch =='y')
    getOutput_file(changed_text);

  nextLine();
   if(control==1)
    {
      cout<<"\t Successfully Encrypted at time ";
       for(i=11;i<19;i++)
	 cout<<currentTime[i];
      nextLine();
      cout<<"\t Encrypted Text : ";
      history.Push(currentTime,"Encryption","XOR");
    }
   else
    {
      cout<<"\t Successfully Decrypted at time";
       for(i=11;i<19;i++)
	 cout<<currentTime[i];
     nextLine();
     cout<<"\t Decrypted Text : ";
     history.Push(currentTime,"Decryption","XOR");
    }
   nextLine();
   cout<<"\t ";
   cout.write(changed_text,length);
   cout<<endl;
   nextLine();
}

// CAESAR CIPHER

void CaesarCipher(int control)
{
  char ch;
  int i,a=0,key,temp;

  drawCipherTitle("Caesar Cipher",1);
  clearArea();
  nextLine();
  cout<<"\t #Caesar# Engine Initiated";
  nextLine();
  nextLine();
  cout<<"\t Enter Text (200 characters max): ";
  nextLine();
  cout<<"\t ";
  gets(source_text);
  length = strlen(source_text);
  nextLine();
  cout<<"\t Enter key (Shift Distance 0 to 26) : ";
  key = int_handler(0,26);              // condition for 0

  if(control == 0)
    key = 26 - key;

  for(i=0;i<length;i++)
  {
     if(source_text[i] != ' ')
     {
      if(isupper(source_text[i]))
	a=90;
      else
	a=122;

      temp = source_text[i]+key;

      if(temp>a)
	temp = temp % a + a-26 ;
      changed_text[i]=temp;
     }
  }

  nextLine();
  cout<<"\t Do you want to output the changed text to a file (Y/N) ? : ";
  cin>>ch;

  time(&rawTime);
  strncpy(currentTime,ctime(&rawTime),26);
  nextLine();

  if(ch=='Y' || ch =='y')
    getOutput_file(changed_text);

  nextLine();
   if(control==1)
    {
      cout<<"\t Successfully Encrypted at :";
       for(i=11;i<19;i++)
	 cout<<currentTime[i];
      nextLine();
      cout<<"\t Encrypted Text : ";
      history.Push(currentTime,"Encryption","Caesar");
    }
   else
    {
      cout<<"\t Successfully Decrypted at :";
       for(i=11;i<19;i++)
	 cout<<currentTime[i];
     nextLine();
     cout<<"\t Decrypted Text : ";
     history.Push(currentTime,"Decryption","Caesar");
    }
   nextLine();
   cout<<"\t ";
   cout.write(changed_text,length);
   cout<<endl;
   nextLine();
}

// VIGNERE CIPHER

void VignereCipher(int control)
{
  char ch,key[200];
  int i,a=0,b=0,k=0,keyf=0;
  unsigned int temp=0;
  drawCipherTitle("Vignere Cipher",2);
  clearArea();
  nextLine();
  cout<<"\t\t #Vignere# Engine Initiated";
  nextLine();
  nextLine();
  cout<<"\t Enter Text (200 characters max): ";
  nextLine();
  cout<<"\t ";
  gets(source_text);
  length = strlen(source_text);

  nextLine();
  cout<<"\t Enter Password : ";
  gets(key);

    for(i=0;i<length;i++)
     {
      if(isalpha(source_text[i]))
       {
	 if(isupper(source_text[i]))
	   a = 90;
	 else
	   a = 122;
	 if(isupper(key[k]))
	   b = 64;
	 else
	   b = 96;

	 if(control==0)
	   keyf = 2*b + 28 - key[k];
	 else
	   keyf=key[k];

	 temp = source_text[i] + keyf - b - 1;
	 if(temp>a)
	   temp = temp % a + a - 26;

	 changed_text[i]=temp;

	 if(key[k+1] == '\0')
	   k=-1;
	 k++;
       }
     }

  nextLine();
  cout<<"\t Do you want to output the changed text to a file (Y/N) ? : ";
  cin>>ch;

  time(&rawTime);
  strncpy(currentTime,ctime(&rawTime),26);
  nextLine();

  if(ch=='Y' || ch =='y')
    getOutput_file(changed_text);

  nextLine();
  if(control==1)
    {
      cout<<"\t Successfully Encrypted at time ";
       for(i=11;i<19;i++)
	 cout<<currentTime[i];
      nextLine();
      cout<<"\t Encrypted Text : ";
      history.Push(currentTime,"Encryption","Vignere");
    }
   else
    {
      cout<<"\t Successfully Decrypted at time ";
       for(i=11;i<19;i++)
	 cout<<currentTime[i];
     nextLine();
     cout<<"\t Decrypted Text : ";
     history.Push(currentTime,"Decryption","Vignere");
    }
   nextLine();
   cout<<"\t ";
   cout.write(changed_text,length);
   cout<<endl;

   nextLine();
}

/* **** HELP FUNCTION **** */

void Help()
{

   drawCipherTitle("   Help",5);
   int *ptr = new int;

   do
   {
    clearArea();
    nextLine(); Aty++;
    cout<<"\t\t\t LIST OF COMMANDS ";
    nextLine(); Aty++;
    cout<<"\t\t\t  1.   Encryption";
    nextLine(); Aty++;
    cout<<"\t\t\t  2.   Decryption";
    nextLine(); Aty++;
    cout<<"\t\t\t  3.   About";
    nextLine(); Aty++;
    cout<<"\t\t\t  4.   History";
    nextLine(); Aty++;
    cout<<"\t\t\t  5.   Exit";
    nextLine();

    cout<<"\t Enter choice or enter 0 to exit : ";
    *ptr=int_handler(0,5);

    clearArea();
    nextLine();

    if(*ptr==1)
       {
	cout<<"\t Encryption.";
	nextLine();
	cout<<"\t\t Allows you to encrypt text according to the";
	nextLine();
	cout<<"\t\t ciphers available. User can also output the";
	nextLine();
	cout<<"\t\t result to a file";
	nextLine(); nextLine();
	cout<<"\t\t\t ***";
       }
    if(*ptr==2)
       {
	cout<<"\t Decryption.";
	nextLine();
	cout<<"\t\t Allows you to decrypt text according to the";
	nextLine();
	cout<<"\t\t ciphers available. User can also output the";
	nextLine();
	cout<<"\t\t result to a file";
	nextLine(); nextLine();
	cout<<"\t\t\t ***";
       }
    if(*ptr==3)
       {
	cout<<"\t About.";
	nextLine();
	cout<<"\t\t Opens a Menu which briefly explains the ";
	nextLine();
	cout<<"\t\t functionality of the ciphers available.";
	nextLine(); nextLine();
	cout<<"\t\t\t ***";
       }
     if(*ptr==4)
       {
	cout<<"\t History.";
	nextLine();
	cout<<"\t\t Shows the details of upto the last 4 decryptions or";
	nextLine();
	cout<<"\t\t encryptions which took place in this session.";
	nextLine(); nextLine();
	cout<<"\t\t\t ***";
       }
      if(*ptr==5)
       {
	cout<<"\t Exit.";
	nextLine();
	cout<<"\t\t Exits the program.";
	nextLine();
	nextLine();
	cout<<"\t\t\t ***";
       }

     if(*ptr!=0)
     {
      Aty+=3;
      nextLine();
      cout<<" Press Enter to go back... ";
      getch();
     }

    }while(*ptr != 0);

    delete ptr;
}

/* **** ABOUT FUNCTION **** */

void About()
{
   drawCipherTitle("   About",5);
   int *ptr = new int;

   do
   {
     clearArea();
     nextLine(); Aty++;
     cout<<"\t\t\t LIST OF CIPHERS ";
     nextLine(); Aty++;
     cout<<"\t\t\t   1. XOR CIPHER";
     nextLine(); Aty++;
     cout<<"\t\t\t   2. CAESAR CIPHER";
     nextLine(); Aty++;
     cout<<"\t\t\t   3. VIGNERE CIPHER";
     nextLine();

     cout<<"\t Enter choice or enter 0 to exit : ";
     *ptr=int_handler(0,3);

     clearArea();
     nextLine(); Aty++;

     if(*ptr==1)
      {
       cout<<"\t\t\t XOR Cipher.";
       nextLine(); Aty++;
       cout<<"\t It is a type of simple additive cipher, which uses ";
       nextLine(); Aty++;
       cout<<"\t exclusive disjunction (XOR) operation. With this ";
       nextLine(); Aty++;
       cout<<"\t logic, text can be encrypted by applying the";
       nextLine(); Aty++;
       cout<<"\t bitwise XOR operation to every using a key. To decrypt,";
       nextLine(); Aty++;
       cout<<"\t we merely apply the function again with the same key";
       nextLine(); Aty++;
       cout<<"\t since the function is reversible this way. ";
       nextLine(); Aty++;
       cout<<"\t Example.  Plaintext : L  Binary: 01001100";
       nextLine(); Aty++;
       cout<<"\t 	   Key       : 1  Binary: 00000001";
       nextLine(); Aty++;
       cout<<"\t       Xor Ciphertext: }  Binary: 01111101";
       nextLine(); Aty++;
      }

   else if(*ptr==2)
      {
       cout<<"\t\t\t Caesar Cipher.";
       nextLine(); Aty++;
       cout<<"\t Also known as Shift Cipher, it is one of the simplest and most ";
       nextLine(); Aty++;
       cout<<"\t widely known technique. It is a substitution cipher, in which ";
       nextLine(); Aty++;
       cout<<"\t each letter in the plaintext is replaced by a letter some fixed";
       nextLine(); Aty++;
       cout<<"\t number of positions (called shift key) down the alphabet.";
       nextLine(); Aty++;
       cout<<"\t For example : Plaintext - HELLO  and shift key = 2 ";
       nextLine(); Aty++;
       cout<<"\t then.        Ciphertext - JGNNQ";
       nextLine();
       cout<<"\t\t\t ***";
      }
   if(*ptr==3)
      {
       cout<<"\t\t\t Vignere Cipher.";
       nextLine(); Aty++;
       cout<<"\t It is a method of encrypting text by using a series of ";
       nextLine(); Aty++;
       cout<<"\t different Caesar ciphers base on the letters of a given ";
       nextLine(); Aty++;
       cout<<"\t keyword. It is a simple form of Polyalphabetic substitution";
       nextLine(); Aty++;
       cout<<"\t Example : Plaintext - ATTACK AT DAWN, and KEYWORD - LEMON";
       nextLine(); Aty++;
       cout<<"\t then   Produced key - LEMONL EM ONLE ";
       nextLine();  Aty++;
       cout<<"\t Hence,   Ciphertext - LXFOPV EF RNHR	";
       nextLine();
       cout<<"\t\t\t ***";
      }

      if(*ptr!=0)
      {
       Aty+=3;
       nextLine();
       cout<<" Press Enter to go back... ";
       getch();
      }

   }while(*ptr!=0);

   delete ptr;
}

/*	
**** COMMAND HANDLER ****
Handles various commands of the main menu.
*/

int Handle_Command(char command[20])
{
  int control=-1,c1=0;

    if(strnicmp(command,"encryption",10)==0)
      control=1;
    else if(strnicmp(command,"decryption",10)==0)
      control=0;
    else if(strnicmp(command,".help",5)==0)
	Help();
    else if(strnicmp(command,"about",5)==0)
      About();
    else if(strnicmp(command,"history",7)==0)
      history.ShowQueue();
    else if(strnicmp(command,"exit",4)==0)
      return(0);
    else
     {
       nextLine();
       cout<<Error_msg[0];
       return(1);
     }

  nextLine();
  nextLine();

  if(control!=-1)
  {
    DrawSideBar(control);
    cout<<"\t List of Algorithms :";
    nextLine();
    cout<<"\t\t  1. Xor Encryption";
    nextLine();
    cout<<"\t\t  2. Caesar Cipher";
    nextLine();
    cout<<"\t\t  3. Vignere Cipher";
    nextLine();
    cout<<"\t Enter choice : ";
    c1=int_handler(1,3);
    nextLine();
   }
     switch(c1)
     {
      case 1:
	XorCipher(control);
	break;
      case 2:
	CaesarCipher(control);
	break;
      case 3:
	VignereCipher(control);
	break;

      default:
	return(2);
     }

  return(1);
}

/*
**** MAIN FUNCTION ****
Controls the calling of command handler and introduction
*/

void main()
{
  clrscr();
  int ex;
  char ch,command[10];

  int gdriver = DETECT, gmode, errorcode;
  int i;


  initgraph(&gdriver, &gmode, "");


  errorcode = graphresult();
  if (errorcode != grOk)
  {
     printf("Graphics error: %s\n", grapherrormsg(errorcode));
     printf("Press any key to halt:");
     getch();
     exit(1);
  }


  midx=getmaxx()/2;
  midy=getmaxy()/2;

//  Introduction

  cout<<"\n\t Secure Boot?(Y/N) ";
  cin>>ch;
  if(ch=='y' || ch=='Y')
    DrawIntro();

// Interface

  clrscr();
  cleardevice();
  setbkcolor(DARKGRAY);
  DrawInterface();

  setcolor(BLACK);
  settextstyle(0,0,3);
  outtextxy(midx/2,3,"PROJECT ALPHA");
  settextstyle(0,0,0);

  Atx=getx()+15;
  Topy=gety()+6;
  Aty=Topy;

  gotoxy(Atx,Aty);
  cout<<"Hello, Welcome to Alpha";
  Aty++;

// Main menu

  do
  {
    drawCipherTitle("Main Menu",3);
    gotoxy(Atx,Aty); Aty++;
    RemoveSideBar();
    cout<<"  Enter command (Type .help for list of commands)";
    gotoxy(Atx+2,Aty);

    gets(command);
    nextLine();

    ex = Handle_Command(command);

// If invalid input is provided

    if(ex==1)
    {
      nextLine();
      cout<<"  Press Enter to continue...";
      getch();
    }


    clearArea();

  }while(ex!=0);

  closegraph();
}
/*	***** END OF PROGRAM ***** 	*/
