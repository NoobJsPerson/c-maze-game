#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
_Bool isWindows = 1;
#elif defined(__unix__)
#include <sys/ioctl.h>
_Bool isWindows = 0;
#endif
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
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
	if(isWindows)
	{
		CONSOLE_SCREEN_BUFFER_INFO csbi;
    		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    		columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    		rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
	} else {
		struct winsize size;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		columns = size.ws_col;
		rows = size.ws_row;
	}
	if(columns < 12 || rows < 4)
	{
		printf("Your terminal window is to small please enlarge it\n");
		return;
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
	if(coordinates[0] == 0 && coordinates[1] == 0 || coordinates[0] == 0 && coordinates[1] == 11 || coordinates[0] == 4 && coordinates[1] == 0 || coordinates[0] == 4 && coordinates[1] == 11 )
	{
		printf("\033[1;31mGame Over!\n\033[0mYour Score: %d",score);
		printf("\nRestart: r, Leave: any other character\n");
		char b;
		scanf("%s",&b);
		switch(b)
		{
			case 'r':
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
	scanf("%s",&action);
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
