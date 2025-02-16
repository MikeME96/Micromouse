#ifndef Maze
#define Maze

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "timer.h"
#include "control.h"
#include "uart.h"
#include "sensor.h"
#include "gpio.h"

#define SIZE 4
#define MAX 16  // Maximum size of the queue

typedef struct {
    int items[MAX][2];  // Each item is a coordinate (x, y)
    int front;
    int rear;
} Queue;

typedef enum {
    NORTH = 0,
    EAST,
    SOUTH,
    WEST
} Direction;

typedef struct {
    int x;
    int y;
} Coordinate;

#define VISITED(node)	(node & 0b0000100000000000)
#define NORTH_Wall(node)(node & 0b0001000000000000)
#define EAST_Wall(node) (node & 0b0010000000000000)
#define SOUTH_Wall(node)(node & 0b0100000000000000)
#define WEST_Wall(node) (node & 0b1000000000000000)
#define DISTANCE(node)	(node & 0b0000011111111111)

#define SET_VISITED(node)			(node = node | 0b0000100000000000)
#define SET_NORTH_Wall(node)		(node = node | 0b0001000000000000)
#define SET_EAST_Wall(node)			(node = node | 0b0010000000000000)
#define SET_SOUTH_Wall(node)		(node = node | 0b0100000000000000)
#define SET_WEST_Wall(node)			(node = node | 0b1000000000000000)
#define SET_DISTANCE(node, value)	(node = (node & 0b1111100000000000) | value)

extern int last[4][4];
extern int array[4][4];
//void explore();

void setWalls(int X, int Y, Direction dir, int right, int left, int front);
int getRight();
int getLeft();
int getFront();
void setBorder();
int min(int a, int b);
void wait(int lim);
char driveToLast(int current_X, int current_Y, int new_X, int new_Y, char direction, int time);
void maze_forward();
void maze_turn_left();
void maze_turn_right();
void maze_turn_180();

/////////////////EXPERIMENTAL STUFF//////////////////
void initQueue(Queue* q);
int isEmpty(Queue* q);
int isFull(Queue* q);
void enqueue(Queue* q, int x, int y);
int* dequeue(Queue* q);
void processQueue(Queue* q, int array[SIZE][SIZE], int x_target, int y_target);
void printArray(int array[4][4]);
void setAdjacentCellsToValuePlusOne(int array[SIZE][SIZE], int x, int y,Queue* q);
void printQueue(Queue* q);
const char* direction_to_string(Direction dir);
Direction update_direction(Direction current_direction, char action);
void setMazeBorders();
void moveToNextCell(int currentX, int currentY, Direction* direction, int nextX, int nextY);
void update_position(int *x, int *y, Direction direction, int num_cells);
void explore(int startX, int startY, Direction direction, int targetX, int targetY);
#endif