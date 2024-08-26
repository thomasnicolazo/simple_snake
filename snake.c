#include "snake.h"

// Window setup
#define WINDOW_X 200
#define WINDOW_Y 200
#define WINDOW_WIDTH 1424
#define WINDOW_HEIGHT 1200
// grid setup
#define GRID_SIZE 900
#define GRID_CELL 40

typedef struct {
    int x;
    int y;
} Apple;

typedef struct {
    int x;
    int y;
	int dir;
    void *node;
} SnakePart;

enum direction {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

int score = 0;
int snakeLen = 3;
bool initialisation = true;
SnakePart *head = NULL;
Apple *apple = NULL;

void snakeFree(void)
{
	SnakePart *tmp = head;
	if(tmp == NULL){
		return;
	}
	while(tmp !=NULL)
	{
		head = tmp->node;
		free(tmp);
		tmp = head;
	}
}

void increaseSnake(void)
{
    SnakePart *tmp = head;
 	if(tmp == NULL)
	{
		fprintf(stderr, "ERROR: head should not be null at increaseSnake");
	}
    while(tmp->node != NULL)
    {
        tmp = tmp->node;
    }
    SnakePart *newS = malloc(sizeof(SnakePart));
            newS->node = NULL;
            tmp->node = newS;
            newS->dir = tmp->dir;
            switch(tmp->dir)
	        {
		    case UP:
                newS->x = tmp->x;
                newS->y = tmp->y + (GRID_SIZE/ GRID_CELL);
                break;
		    case DOWN:
                newS->x = tmp->x;
                newS->y = tmp->y - (GRID_SIZE/ GRID_CELL);
			    break;
		    case LEFT:
                newS->x = tmp->x + (GRID_SIZE/ GRID_CELL);
                newS->y = tmp->y;
			    break;
		    case RIGHT:
                newS->x = tmp->x - (GRID_SIZE/ GRID_CELL);
                newS->y = tmp->y;
			    break;
	        }
}

void initSnake(int x, int y)
{
	SnakePart *newS = malloc(sizeof(SnakePart));
	newS->x = x + (rand() % (GRID_CELL - 6) + 3) * (GRID_SIZE/ GRID_CELL);
	newS->y = y + (rand() % (GRID_CELL - 6)+ 3) * (GRID_SIZE/ GRID_CELL);
	newS->node = NULL;
    newS->dir = rand() % 4;
	head = newS;
	for(int i=0; i < snakeLen;i++)
    {
        increaseSnake();
    }

}

void moveSnake(void) {
    SnakePart *tmp = head;
    int prevX = tmp->x;
    int prevY = tmp->y;
    int prevDir = tmp->dir;

    switch (tmp->dir) {
        case UP:
            tmp->y -= GRID_SIZE / GRID_CELL;
            break;
        case DOWN:
            tmp->y += GRID_SIZE / GRID_CELL;
            break;
        case LEFT:
            tmp->x -= GRID_SIZE / GRID_CELL;
            break;
        case RIGHT:
            tmp->x += GRID_SIZE / GRID_CELL;
            break;
    }

    while (tmp->node != NULL) {
        tmp = tmp->node;
        int tmpX = tmp->x;
        int tmpY = tmp->y;
        int tmpDir = tmp->dir;

        tmp->x = prevX;
        tmp->y = prevY;
        tmp->dir = prevDir;

        prevX = tmpX;
        prevY = tmpY;
        prevDir = tmpDir;
    }
}

void drawSnake(SDL_Renderer *renderer)
{
	SDL_Rect part;
	SDL_SetRenderDrawColor(renderer, 0,255, 0,255);
	part.h = part.w = GRID_SIZE/GRID_CELL;
	SnakePart *tmp = head;
	if(tmp == NULL)
	{
		return;	
	}
	while(tmp != NULL)
	{
		part.x = tmp->x;
		part.y = tmp->y;
		SDL_RenderFillRect(renderer,&part);
		tmp = tmp->node;
	}
}

void initApple(int x, int y)
{
	SnakePart *tmp = head;
    bool isOnSnake = false;
	if(tmp == NULL)
	{
		fprintf(stderr, "ERROR: head shouold not be null at initApple");
	}
    do
    {
        Apple *fApple = malloc(sizeof(Apple));	
	    fApple->x = x + (rand() % (GRID_CELL - 6) + 3) * (GRID_SIZE/ GRID_CELL);
	    fApple->y = y + (rand() % (GRID_CELL - 6) + 3) * (GRID_SIZE/ GRID_CELL);
	    apple = fApple;
        tmp = head;
        while (tmp->node != NULL)
        {
            if (apple->x == tmp->x && apple->y == tmp->y)
            {
                free(apple);
                isOnSnake = true;
                break;
            }
            tmp = tmp->node;
            isOnSnake = false;
        }

    } while (isOnSnake);
}

void drawApple(SDL_Renderer *renderer)
{	
    SDL_SetRenderDrawColor(renderer, 255,0,0, 255);
   	SDL_Rect cellApple;
    cellApple.w = cellApple.h = GRID_SIZE/GRID_CELL;
    cellApple.x = apple->x;
    cellApple.y = apple->y;
	SDL_RenderFillRect(renderer,&cellApple);
}

void checkCollisionApple()
{
    int gridPositionX = WINDOW_WIDTH/2 - GRID_SIZE/2;
    int gridPositionY = WINDOW_HEIGHT/2 - GRID_SIZE/2;
    SnakePart *tmp = head;
	if(tmp == NULL)
	{
		fprintf(stderr, "ERROR: head shouold not be null at checkCollisionApple");
	}
    while(tmp->node !=NULL)
    {
        if(tmp->x == apple->x && tmp->y == apple->y)
        {
            score++;
            free(apple);
            initApple(gridPositionX,gridPositionY);
            increaseSnake();
        }
        tmp = tmp->node;
    }  
}

void checkCollisionBorder()
{
    int gridPositionX = WINDOW_WIDTH/2 - GRID_SIZE/2;
    int gridPositionY = WINDOW_HEIGHT/2 - GRID_SIZE/2;
    SnakePart *tmp = head;
	if(tmp == NULL)
	{
		fprintf(stderr, "ERROR: head shouold not be null at checkCollisionBorder");
	}
    if (tmp->x < gridPositionX || tmp->x > (GRID_SIZE+gridPositionX-(GRID_SIZE/GRID_CELL)) || tmp->y < gridPositionY || tmp->y >=(gridPositionY + GRID_SIZE-(GRID_SIZE/GRID_CELL)))
    {
        snakeFree();
        initSnake(gridPositionX,gridPositionY);
    }
}

void checkCollisionSnake()
{
    SnakePart *tmp = head;
    int gridPositionX = WINDOW_WIDTH/2 - GRID_SIZE/2;
    int gridPositionY = WINDOW_HEIGHT/2 - GRID_SIZE/2;
    if(tmp == NULL && tmp->node == NULL)
	{
		fprintf(stderr, "ERROR: head shouold not be null at checkCollisionSnake");
	}
    tmp=tmp->node;
    while(tmp != NULL)
    {
        if (head->x == tmp->x && head->y == tmp->y)
        {
            snakeFree();
            initSnake(gridPositionX,gridPositionY);
            break;
        }
        tmp = tmp->node;
        
    }

}

void drawGrid(SDL_Renderer *renderer, int x ,int y)
{
    SDL_SetRenderDrawColor(renderer, 55,55,55, 240);
    SDL_Rect cell;
    cell.w = cell.h = GRID_SIZE/GRID_CELL;
    for(int i=0;i<GRID_CELL;i++)
    {
        for(int j =0; j< GRID_CELL;j++)
        {
            // X and Y represent the top left corner of the grid
            cell.x = x + (i * cell.w);
            cell.y = y + (j * cell.h);
            SDL_RenderDrawRect(renderer,&cell);
        }
    }
}



int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window;
    SDL_Renderer *renderer;
    int gridPositionX = WINDOW_WIDTH/2 - GRID_SIZE/2;
    int gridPositionY = WINDOW_HEIGHT/2 - GRID_SIZE/2;
	srand(time(0));

    if(SDL_INIT_VIDEO < 0)
    {
        fprintf(stderr,"ERROR: Failed SDL_Init");
    }
    window = SDL_CreateWindow("Snake", WINDOW_X, WINDOW_Y, WINDOW_WIDTH, WINDOW_HEIGHT,SDL_WINDOW_BORDERLESS);
    
    if(!window)
    {
        fprintf(stderr,"ERROR: Failed to Create window");
    }

    renderer = SDL_CreateRenderer(window,0,0);

    if(!renderer)
    {
        fprintf(stderr,"ERROR: Failed to Create renderer");
    }

    bool running = true;
    SDL_Event event;
	initSnake(gridPositionX,gridPositionY);
	initApple(gridPositionX,gridPositionY);
    while (running)
    {
        while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYUP:
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    running = false;
                    break;
                case SDLK_UP:
                    if(head->dir != DOWN) head->dir = UP;
                    break;
                case SDLK_DOWN:
                    if(head->dir != UP) head->dir = DOWN;
                    break;
                case SDLK_LEFT:
                    if(head->dir != RIGHT) head->dir = LEFT;
                    break;
                case SDLK_RIGHT:
                    if(head->dir != LEFT) head->dir = RIGHT;
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
        SDL_RenderClear(renderer);
        drawGrid(renderer, gridPositionX,gridPositionY);
        // Draw snake
        moveSnake();
        checkCollisionBorder();
        checkCollisionApple();
        checkCollisionSnake();
    	drawApple(renderer);
	    drawSnake(renderer);
		SDL_SetRenderDrawColor(renderer, 25,25,27, 100);
        SDL_Delay(30);
        SDL_RenderPresent(renderer);
    }

    
	//free snake
	snakeFree();
    // free apple
    free(apple);
    //SDL_Delay(4000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
