#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

char randomChunk(){
        return rand() % 2 ? '#' : '-';                    }
void generateRandomMap(char map[5][13]){
        for(int i = 0; i < 4; i++){
                for(int j = 0; j < 11;j++){
                        map[i][j] = randomChunk();                        }
        }
}
void updateScene(int *coordinates, char map[5][13], char updateCode)
{
	struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        if(size.ws_col < 12 || size.ws_row < 4)
        {
                printf("Your terminal window is to sm
all please enlarge it\n");
                return;
        }
        static int score;
        static char direction;
        char action[1];
        char *a = action;
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
                printf("%s","\n");
                return;
        }
        printf("Score: ");
        printf("%d",score);
        printf("\n");
        for (int i = 0; i < 5; i++)
        {
                printf("%s", map[i]);
                printf("\n");
        }
        fgets(a,2,stdin);
        updateScene(coordinates, map, *action);
}
int main(int argc, char *argv[])
{
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
