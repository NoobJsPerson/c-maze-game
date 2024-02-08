#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#ifdef __unix__
#include <termios.h>
#include <sys/ioctl.h>
static struct termios old, new;
struct winsize size;
char getch()
{
  char ch;
  tcgetattr(0, &old); /* grab old terminal i/o settings */
  new = old; /* make new settings same as old settings */
  new.c_lflag &= ~ICANON; /* disable buffered i/o */
  new.c_lflag &= ~ECHO; /* set echo mode */
  tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
  ch = getchar();
  tcsetattr(0, TCSANOW, &old); /* set terminal i/o settings back to old */
  return ch;
}
void get_size(int *columns,int *rows){
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	*columns = size.ws_col;
	*rows = size.ws_row;
}
#elif defined(_WIN32)
#include <conio.h>
#include <windows.h>
CONSOLE_SCREEN_BUFFER_INFO csbi;
void get_size(int *columns,int *rows){
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    *columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    *rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}
#endif
char randomChunk(){
	int r = rand();
	return r % 2 ? '#' : '-';
}
void generateRandomMap(char map[5][13]){
	for(int i = 0; i < 5; i++){
		for(int j = 0; j < 12;j++){
			map[i][j] = randomChunk();
		}
	}
}
void updateScene(int *coordinates, char map[5][13], char updateCode)
{
	int columns, rows;
	get_size(&columns, &rows);
	if(columns < 12 || rows < 4)
	{
		printf("Your terminal window is too small please enlarge it\n");
		while(1) {
			get_size(&columns, &rows);
			if(columns >= 12 && rows >= 4) return updateScene(coordinates, map, '0');
		}
	}
	static int score;
	static char direction;
	char action;
	printf("\033c");
	switch (updateCode)
	{
	case 'w':
		if (map[coordinates[0] - 1][coordinates[1]] != '#')
		{
			if(coordinates[0] == 0)
			{
				generateRandomMap(map);
				map[coordinates[0]][coordinates[1]] = randomChunk();
                                map[4][coordinates[1]] = '*';
				coordinates[0] = 4;
				if(direction != 'd') score++;
				direction = 'u';
			}
			else
			{
				map[coordinates[0]][coordinates[1]] = '-';
				map[coordinates[0] - 1][coordinates[1]] = '*';
				coordinates[0]--;
			}
		}
		break;
	case 's':
		if (map[coordinates[0] + 1][coordinates[1]] != '#')
		{
			if(coordinates[0] == 4)
			{
				generateRandomMap(map);
				map[coordinates[0]][coordinates[1]] = randomChunk();
                                map[0][coordinates[1]] = '*';
				coordinates[0] = 0;
				if(direction != 'u') score++;
				direction = 'd';
			}
			else
			{
				map[coordinates[0]][coordinates[1]] = '-';
				map[coordinates[0] + 1][coordinates[1]] = '*';
				coordinates[0]++;
			}
		}
		break;
	case 'a':
		if (map[coordinates[0]][coordinates[1] - 1] != '#')
		{
			if(coordinates[1] == 0)
			{
				generateRandomMap(map);
				map[coordinates[0]][coordinates[1]] = randomChunk();
                                map[coordinates[0]][11] = '*';
				coordinates[1] = 11;
				if(direction != 'r') score++;
				direction = 'l';
			}
			else
			{
				map[coordinates[0]][coordinates[1]] = '-';
				map[coordinates[0]][coordinates[1] - 1] = '*';
				coordinates[1]--;
			}
		}
		break;
	case 'd':
		if (map[coordinates[0]][coordinates[1] + 1] != '#')
		{
			if(coordinates[1] == 11)
			{
				generateRandomMap(map);
				map[coordinates[0]][coordinates[1]] = randomChunk();
				map[coordinates[0]][0] = '*';
				coordinates[1] = 0;
				if(direction != 'l') score++;
				direction = 'r';
			}
			else
			{
				map[coordinates[0]][coordinates[1]] = '-';
				map[coordinates[0]][coordinates[1] + 1] = '*';
				coordinates[1]++;
			}
		}
	}
	if(updateCode == 'q' || coordinates[0] == 0 && coordinates[1] == 0 || coordinates[0] == 0 && coordinates[1] == 11 || coordinates[0] == 4 && coordinates[1] == 0 || coordinates[0] == 4 && coordinates[1] == 11 )
	{
		printf("\033[1;31mGame Over!\n\033[0mYour Score: %d",score);
		printf("\nRestart: r, Leave: any other character\n");
		char b;
		b = getch();
		switch(b)
		{
			case 'r':
				printf("\033c");
				score =  0;
				action = '0';
				coordinates[0] = 2;
				coordinates[1] = 6;
				for(int i = 0; i < 12; i++)
				{
					map[0][i] = map[4][i] = '#';
					map[1][i] = map[3][i] = i == 0 || i == 11 ? '#' : '-';
					map[2][i] = i == 0 ? '#' : i == 6 ? '*' : '-';
				}
        			break;
			default:
				return;
		}
	}
	printf("Score: %d",score);
	printf("\n");
	for (int i = 0; i < 5; i++)
	{
		printf("%s", map[i]);
		printf("\n");
	}
	action = getch();
	updateScene(coordinates, map, action);
}
int main(int argc, char *argv[])
{
	srand(time(NULL));
	int coordinates[2] = {2, 6};
	char map[5][13] = {
		{"############"},
		{"#----------#"},
		{"#-----*-----"},
		{"#----------#"},
		{"############"}};
	updateScene(coordinates, map, '0');
	return 0;
}
