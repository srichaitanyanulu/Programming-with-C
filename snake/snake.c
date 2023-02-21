// Snake Game in C
// For linux
// Tested on WSL2 (Ubuntu)
// Author: Sri Chaitanya Nulu

#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <termios.h>
#include <time.h>
 
#define BRED "\e[1;31m"
#define BGRN "\e[1;32m"
#define WHT "\e[0;37m"

struct Snake{
    int x;
    int y;
    
    struct Snake *prev;
    struct Snake *next;
};

struct Snake *snakeHead = NULL;
struct Snake *snakeTail = NULL;
unsigned int maxLength = 3, currLength = 0;
int foodX, foodY, score = 0;

//insert a part in front
void insertPart(int x, int y) {
   
   struct Snake *part = (struct Snake*) malloc(sizeof(struct Snake));
	
   part->x = x;
   part->y = y;
   part->prev = NULL;

   part->next = snakeHead;

   if(snakeHead != NULL){
        snakeHead->prev = part;
   }

   snakeHead = part;

   if(snakeTail==NULL){
        snakeTail = snakeHead;
   }

   currLength++;
}

//delete snake's tail
void deleteTail(){
    if (snakeTail != NULL)
	{
		struct Snake* temp = snakeTail;
		snakeTail = snakeTail->prev;
		if (snakeTail != NULL)
			snakeTail->next = NULL;
		
		free(temp);
	}
    currLength--;
}

//position of food
void randomizeFood(int grid_size){
   foodX = rand()%grid_size;
   foodY = rand()%grid_size;

}

//align text to center of the screen
void centerText(char* s, struct winsize w){
    for(int i = 1; i < (w.ws_col/2)-(strlen(s)/2); i++){
        printf(" ");
    }
    printf("%s", s);
}

//draw the array
void display(int grid_size, struct winsize w, char* grid[grid_size][grid_size]){

    system("clear");

    printf("\n");
    centerText("- SNAKE -", w);
    printf("\n\n");

    centerText("", w);
    printf("%d\n\n", score);

    

    for(int i = 0; i < grid_size; i++){

        //centering the grid
        for(int j = 0; j < (w.ws_col/2) - w.ws_row - 10; j++){
            printf("  ");
        }

        //placing tiles
        for(int j = 0; j < grid_size; j++){
            if(grid[i][j]=="O "){
                printf(BRED "%s", grid[i][j]);
            }else if(grid[i][j] == "# "){
                printf(BGRN "%s", grid[i][j]);
            }else{
                printf(WHT "%s", grid[i][j]);
            }
        }
        printf("\n");
    }
}

//update the 2D-array
void updateMap(struct winsize w, int grid_size, char* grid[grid_size][grid_size]){
    for(int i = 0; i < grid_size; i++){
        
        //setting up tiles and positions
        for(int j = 0; j < grid_size; j++){
            
            //tile
            grid[i][j] = "- ";

            struct Snake *temp = snakeHead;
            while(temp != NULL){
                if(i==temp->y && j==temp->x){
                    grid[i][j] = "O ";
                }
                temp = temp->next;
            }
            free(temp);
            if(i==foodX && j==foodY){
                grid[i][j] = "# ";
            }
        }
    }
    
    display(grid_size, w, grid);
}

//kbhit implementation for linux from https://gist.github.com/vsajip/1864660

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

// Driver Code
void main()
{  

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    //printf("Rows: %d Columns: %d", w.ws_row, w.ws_col);
    
    int grid_size = w.ws_row - 10;

    //grid
    char* grid[grid_size][grid_size];

    //snakeHead
    insertPart(grid_size/2, grid_size/2);

    time_t t;
   
   srand((unsigned) time(&t));
   
   randomizeFood(grid_size);

    updateMap(w, grid_size, grid);

    clock_t t1, t2;
    double time_taken;

    t1 = clock();

    int direction=3;


   while(1){

        t2 = clock() - t1;
        time_taken = ((double)t2)/CLOCKS_PER_SEC;

        //check if snake ate food
        if(snakeHead->x==foodY && snakeHead->y==foodX){
            maxLength += 1;
            score += 1;
            randomizeFood(grid_size);
        }

        //check snake collision
        if(currLength >= 4){
            struct Snake *temp = snakeHead;

            int i = 4;

            while(i){
                temp = temp->next;
                i--;
            }
            while(temp != NULL){
                if(snakeHead->y==temp->y && snakeHead->x==temp->x){
                    
                    printf("GAME OVER!!\n\n");
                    return;
                }
                temp = temp->next;
            }
            free(temp);   
        }

        //game control logic
        if(time_taken > 0.2){
                t1 = clock();

                int x = snakeHead->x;
                int y = snakeHead->y;


                if(direction == 1){

                    if(y==0) 
                        y = grid_size;

                    y = (y - 1)%grid_size;
                }
                else if(direction == 2){

                    y = (y + 1)%grid_size;
                }
                else if(direction == 3){
            
                    if(x==0) 
                        x = grid_size;

                    x = (x - 1)%grid_size;
                }
                else if(direction == 4){

                    x = (x + 1)%grid_size;
                }

                insertPart(x, y);
            

                if(currLength > maxLength){
                    deleteTail();
                }

                updateMap(w, grid_size, grid);
            }

            //wasd keys
            if (kbhit()) {
                switch (getchar()) {
                    case 'a':
                        if (direction!= 4) direction = 3;
                        break;
                    case 's':
                        if (direction!= 1) direction = 2;
                        break;
                    case 'd':
                        if (direction!= 3) direction = 4;
                        break;
                    case 'w':
                        if (direction!= 2) direction = 1;
                        break;
                }
            }

        }
}