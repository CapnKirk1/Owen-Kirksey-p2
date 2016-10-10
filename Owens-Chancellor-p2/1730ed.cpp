#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#define ESC 27
#define BACKSPACE 8
#define ENTER 13

char* text  = NULL;
long mag;

void menu();
/*
  OpenFile reads a text file and returns an int
  to check for errors
*/
static int Read(char *filename)
{
  FILE *file = NULL;
  file = fopen(filename, "rb");
  if (file == NULL)
    {
      return 1;
    }
  //obtain file size:
  fseek(file, 0, SEEK_END);
  mag = ftell(file);
  fseek(file, 0, SEEK_SET);
  text =  (char*) malloc(mag + 1);
  mag = fread(text, 1, mag, file);  
  //int count = 0;
  
  //  for(int i = 0 ; i < mag; i++)
  //    {
  //      if(*(text + i) == '\n')
  //	{ 
  //	  *(text + count) = 'n';	  
  //	}
  //      count++;
  //    }  
  if (mag == 0)
    {
      return 2;
    }
  fclose(file);
  return 0;
}
int getpos(int line, int colu)
{
  int posinbytes = 0;
  int countlin = 0;
  int countcol = 0;
  int countbytes = 0;
  for(int i = 0 ; i < mag ; i++)
    {
      if(*(text + i) == '\n')// && (line > countlin || colu > countcol))
	{ 
	  countlin++;
	  countcol = 0;
	  countbytes++;
	}
      countcol++;
      countbytes++;
      if(line == countlin && colu == countcol)
	{
	  posinbytes = (countbytes-countlin + 1);
	}
    }
      return posinbytes;
}

void back(int pos)
{
  move(0,0);
  char* textCopy = (char*) malloc(mag + 60000);
  for(int i = 0; i < pos; i++)
    {
      *(textCopy + i) = *(text + i);
    }  
  for(int i = pos; i < mag; i++)
    {
      *(textCopy + i) = *(text + i + 1);
    }
  for(int i = pos; i < mag; i++)
    {
      *(text + i) = *(textCopy + i);
    }
  printw(text);
}
void insert(int pos,int gch)
{
  move(0,0);
  
  char* textCopy = (char*) malloc(mag + 60000);
  for(int i = 0; i < pos; i++)
    {
      *(textCopy + i) = *(text + i);
    }  
  for(int i = pos; i < mag; i++)
    {
      *(textCopy + i + 1) = *(text + i);
    }
  *(textCopy + pos) = gch;
  for(int i = pos; i < mag; i++)
    {
      *(text + i) = *(textCopy + i);
    }
  printw(text);

}


int save()
{
  FILE * pFile;
  char str[256] = "";
  Read(str);
  char buffer[60000];
  for (int i = 0; i < mag ;i++)
    {
      buffer[i] = *(text+i);
    }
  pFile = fopen ("newFile.txt", "wb");
  fwrite (buffer , sizeof(char), sizeof(buffer), pFile);
  fclose (pFile);
  return 0;
}

/*
  Display prints the text to a tui
*/

static void Display()
{
  printw(text);
}

int main(int argc, char* argv[1]) {
  
  char mesg[] = "Just a String";
  int rows,cols;
  initscr();                 // start curses mode
  keypad(stdscr,TRUE);
  //  scrollok(stdscr,TRUE);
  noecho();
  getmaxyx(stdscr, rows, cols);
  // mvprintw(rows/2, (cols-strlen(mesg))/2, "%s", mesg);
  WINDOW *mainWindow;
  mainWindow = newwin(rows,cols,0,0);
  box(mainWindow,0,0);
  

  start_color();
  init_pair(1,COLOR_WHITE,COLOR_BLUE);
  bkgd(COLOR_PAIR(1));
  wrefresh(mainWindow);                 // print it on to the real screen

  
  char str[256] = "";
  if(argc == 2)
    {
      box(mainWindow, 0, 0);
      strcpy(str, argv[1]);
    }      
  if (strcmp(str, "") == 0)
    {
      printw("Please enter the file you wish to open: ");
      echo();
      getstr(str);
      noecho();
    }
    
  int errorCheck = Read(str);
  if (errorCheck == 0)
    {
      Display();
    }
  
  else if (errorCheck == 1)
    {
      //errorMenu();
      strcpy(str, "");
    }
  else if (errorCheck == 2)
    {
      // errorMenu();
    }
  int gch;
  int posi;
  int lin = 0;
  int col = 0;
  move(lin,col);
  while((gch = getch()) != ESC)
    {
      if(gch == KEY_F(2) || gch == KEY_F(1) || gch == BACKSPACE || gch == ENTER || gch == KEY_UP || gch == KEY_DOWN || gch == KEY_LEFT || gch == KEY_RIGHT){
      if(gch == KEY_F(2))
	{
	  save();
	}
      if(gch == KEY_F(1))
	{
	  menu();
	  refresh();
	}
      if(gch == BACKSPACE)
	{
	  posi = getpos(lin,col);
	  back(posi);
	  //col--;
	  move(lin,col);
	}  

      if(gch == ENTER)
	{
	  lin++;
	  col = 0;
	  move(lin,col);
	}
      if(gch == KEY_UP)
	{
	  lin--;
	  move(lin,col);
	}  
      if(gch == KEY_DOWN)
	{
	  lin++;
	  move(lin,col);
	  refresh();
        }  
      if(gch == KEY_LEFT)
	{
	  col--;
	  move(lin,col);
	  refresh();
	}  
      if(gch == KEY_RIGHT)
	{
	  col++;
	  move(lin,col);
	  refresh();
	}
    }
      else
	{
	  posi = getpos(lin,col);
	  insert(posi,gch);
	  col++;
	  move(lin,col);	  
	}
      refresh();
    }
  endwin();
  return EXIT_SUCCESS;
} //main

/*
void errorMenu()
{

 WINDOW *w;
  w = newwin(10,15,10,10);
  char list[4][8] = { "Error"};
  char item[7];
  int ch,i = 0;
  box (w,0,0);
  wrefresh(w);
  
  i = 0;
  noecho(); // disable echoing of characters on the screen
  keypad( w, TRUE ); // enable keyboard input for the window.
  curs_set( 0 ); // hide the default screen cursor.
  delwin(w);
  endwin();
}
*/
void menu()
{
  WINDOW *w;
  w = newwin(10,15,1,1);
  char list[4][8] = { "Open", "Save", "Save As", "Exit"};
  char item[7];
  int ch,i = 0;
  box (w,0,0);
  
  for(i = 0; i < 4; i++) 
    {
      if( i == 0 )
	{ 
	  wattron( w, A_STANDOUT ); // highlights the first item.
	}
      else
	{
	  wattroff( w, A_STANDOUT );
	}
      sprintf(item, "%-7s",  list[i]);
      mvwprintw( w, i+1, 2, "%s", item );
    }
  wrefresh(w);
  
  i = 0;
  noecho(); // disable echoing of characters on the screen
  keypad( w, TRUE ); // enable keyboard input for the window.
  curs_set( 0 ); // hide the default screen cursor.
     
  // get the input
  while(( ch = wgetch(w)) != 27)
    {
      
      // right pad with spaces to make the items appear with even width.
      sprintf(item, "%-7s",  list[i]); 
      mvwprintw( w, i+1, 2, "%s", item ); 
      // use a variable to increment or decrement the value based on the input.
      switch( ch ) {
      case KEY_UP:
	i--;
	i = ( i<0 ) ? 3 : i;
	break;
      case KEY_DOWN:
	i++;
	i = ( i>3 ) ? 0 : i;
	break;
      case KEY_ENTER:
	if (i == 4)
	  {
	    endwin();
	  }
	break;
	
      }
      // now highlight the next item in the list.
      wattron( w, A_STANDOUT );
      
      sprintf(item, "%-7s",  list[i]);
      mvwprintw( w, i+1, 2, "%s", item);
      wattroff( w, A_STANDOUT );
    }
  delwin(w);
  endwin();
 
}






/*
  FILE *file = NULL;
  file = fopen(filename, "rb");
  if (file == NULL)
    {
      return 1;
    }
  //obtain file size:
  fseek(file, 0, SEEK_END);
  long mag  = ftell(file);
  fseek(file, 0, SEEK_SET);
  text = (char*) malloc(mag + 1);
  mag = fread(text, 1, mag, file);
      
  if (mag == 0)
    {
      return 2;
    }
  fclose(file);
  return 0;
 */
