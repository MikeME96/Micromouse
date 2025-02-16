#include "maze.h"
#include "string.h"
#include <stdbool.h>
#include "libpic30.h"

//turns the robot 180 degrees and goes one cell forward
extern volatile int t;

//int t = 100;
void maze_turn_180(){
/
    __delay_ms(t);
    turn_180();
    while(flag != 0);
    __delay_ms(t);
//    wait(lim);
//    flag = 0;
    go_one_cell();
   while(flag != 0);
    __delay_ms(t);

}

//turns the robot right 90 degrees and goes one cell forward

void maze_turn_right(){

    __delay_ms(t);
    turn_right();
    while(flag != 0);
    __delay_ms(t);
//    wait(lim);
//    flag = 0;
    go_one_cell();
   while(flag != 0);
    __delay_ms(t);

//    wait(lim);
//    flag = 0;
    
}

//turns the robot left 90 degrees and goes one cell forward

void maze_turn_left(){

    __delay_ms(t);
    turn_left();
    
    while(flag != 0);
    __delay_ms(t);
//    wait(lim);
//    flag = 0;
    go_one_cell();
    while(flag != 0);
    __delay_ms(t);

//    wait(lim);
//    flag = 0;
    
}

//robot goes one cell forward

void maze_forward(){
    go_one_cell();
   while(flag != 0);
    __delay_ms(t);
//    flag = 0;
}

int getRight(){
	return rightWall();
}

int getLeft(){
	return leftWall();
}

int getFront(){
	return frontWall();
}


/////////////////////////////////////////EXPERIMENTAL MAZE///////////////////////////////////////
//#define MAX 16  // Maximum size of the queue
//#define SIZE 4  // Define the size of the 2D array
//
//#define SIZE 4
//#define MAX 16  // Maximum size of the queue

 int array[4][4] = {
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255}
    };


// Function to initialize the queue
void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

// Check if the queue is empty
int isEmpty(Queue* q) {
    return q->front == -1;
}

// Check if the queue is full
int isFull(Queue* q) {
    return (q->rear + 1) % MAX == q->front;
}

// Enqueue function to add a coordinate (x, y)
void enqueue(Queue* q, int x, int y) {
    if (isFull(q)) {
        printf("Queue is full!\n");
    } else {
        if (q->front == -1) q->front = 0;  // First element
        q->rear = (q->rear + 1) % MAX;
        q->items[q->rear][0] = x;  // Store x coordinate
        q->items[q->rear][1] = y;  // Store y coordinate
        //printf("Inserted (%d, %d)\n", x, y);
    }
}

// Dequeue function to remove and return the front coordinates
int* dequeue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        return NULL;  // Return NULL if the queue is empty
    } else {
        int* item = q->items[q->front];  // Get the front item (coordinates)
        if (q->front == q->rear) {
            // Queue becomes empty
            q->front = -1;
            q->rear = -1;
        } else {
            // Move front to the next element
            q->front = (q->front + 1) % MAX;
        }
        return item;  // Return the dequeued item (coordinates)
    }
}
void processQueue(Queue* q, int array[SIZE][SIZE],int x_target,int y_target) {
    while (!isEmpty(q)) {
        // Dequeue the front element
        int* coords = dequeue(q);
        int index = 0;
        if (coords != NULL) {
                
                setAdjacentCellsToValuePlusOne(array, coords[index], coords[index+1],q);
               
        }
    }
}
void printArray(int array[4][4]) {
//    for (int i = 0; i < 4; i++) {
//        // Print the top border of the cells
//        printf("+-----+-----+-----+-----+\n");
//        
//        // Print the row with values
//        for (int j = 0; j < 4; j++) {
//            printf("| %3d ", array[i][j]);  // Format number with padding
//        }
//        printf("|\n");  // Close the row
//        
//    }
//    // Print the bottom border of the cells
//    printf("+-----+-----+-----+-----+\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%d ", DISTANCE(array[i][j]));
        }
        printf("\r\n");
    }
}

void setAdjacentCellsToValuePlusOne(int array[SIZE][SIZE], int x, int y, Queue* q) { // NEW SET ADAJACENT CELL
    // Get the value of the input cell
    int value = DISTANCE(array[x][y]);

    // Check and set the cell above (West)
    if (x - 1 >= 0 && DISTANCE(array[x - 1][y]) == 255) {  // Check upper boundary and if it's unprocessed
        if (!(WEST_Wall(array[x][y])) && !(EAST_Wall(array[x - 1][y]))) {  // Check for no walls between
            SET_DISTANCE(array[x - 1][y] ,value + 1);
            enqueue(q, x - 1, y);  // Add the modified cell to the queue
        }
    }

    // Check and set the cell below (East)
    if (x + 1 < SIZE && DISTANCE(array[x + 1][y]) == 255) {  // Check lower boundary and if it's unprocessed
        if (!(EAST_Wall(array[x][y])) && !(WEST_Wall(array[x + 1][y]))) {  // Check for no walls between
            SET_DISTANCE(array[x + 1][y], value + 1);
            enqueue(q, x + 1, y);  // Add the modified cell to the queue
        }
    }

    // Check and set the cell to the left (South)
    if (y - 1 >= 0 && DISTANCE(array[x][y - 1]) == 255) {  // Check left boundary and if it's unprocessed
        if (!(SOUTH_Wall(array[x][y])) && !(NORTH_Wall(array[x][y - 1]))) {  // Check for no walls between
            SET_DISTANCE(array[x][y - 1], value + 1);
            enqueue(q, x, y - 1);  // Add the modified cell to the queue
        }
    }

    // Check and set the cell to the right (North)
    if (y + 1 < SIZE && DISTANCE(array[x][y + 1]) == 255) {  // Check right boundary and if it's unprocessed
        if (!(NORTH_Wall(array[x][y])) && !(SOUTH_Wall(array[x][y + 1]))) {  // Check for no walls between
            SET_DISTANCE(array[x][y + 1], value + 1);
            enqueue(q, x, y + 1);  // Add the modified cell to the queue
        }
    }
}

void printQueue(Queue* q) {
    if (isEmpty(q)) {
        printf("\r Queue is empty!\n");
        return;
    }

    printf("\r Queue contents (from front to rear):\n");
    int i = q->front;
    while (i != q->rear) {
        printf("(%d, %d) ", q->items[i][0], q->items[i][1]);
        i = (i + 1) % MAX;
    }
    // Print the last item (rear)
    printf(" (%d, %d)\n", q->items[q->rear][0], q->items[q->rear][1]);
}


Direction update_direction(Direction current_direction, char action) {
    switch (action) {
        case 'F': // Forward
            return current_direction; 
        case 'L': // Turn left
            return (current_direction + 3) % 4; 
        case 'R': // Turn right
            return (current_direction + 1) % 4; 
        case 'B': // Turn 180 degrees
            return (current_direction + 2) % 4; 
        default:
            return current_direction; // Invalid action
    }
}

const char* direction_to_string(Direction dir) {
    switch (dir) {
        case NORTH: return "North";
        case EAST: return "East";
        case SOUTH: return "South";
        case WEST: return "West";
        default: return "Unknown";
    }
}
void setMazeBorders() {
    // Set west walls for the first row (x = 0)
    for (int y = 0; y < 4; y++) {
        SET_WEST_Wall(array[0][y]);  // Set west wall for each cell in the first row
    }
    
    // Set east walls for the last row (x = 3)
    for (int y = 0; y < 4; y++) {
        SET_EAST_Wall(array[3][y]);  // Set east wall for each cell in the last row
    }
    
    // Set south walls for the first column (y = 0)
    for (int x = 0; x < 4; x++) {
        SET_SOUTH_Wall(array[x][0]);  // Set south wall for each cell in the first column
    }
    
    // Set north walls for the last column (y = 3)
    for (int x = 0; x < 4; x++) {
        SET_NORTH_Wall(array[x][3]);  // Set north wall for each cell in the last column
    }
}

void update_position(int *x, int *y, Direction direction, int num_cells) {
    switch (direction) {
        case NORTH:
            *y += num_cells;  // Move up the grid (decrease Y)
            break;
        case EAST:
            *x += num_cells;  // Move right (increase X)
            break;
        case SOUTH:
            *y -= num_cells;  // Move down the grid (increase Y)
            break;
        case WEST:
            *x -= num_cells;  // Move left (decrease X)
            break;
    }
}

void printMazeWithBorders() {
    // Loop through each row of the maze
    for (int x = 0; x < 4; x++) {
        // Print the top borders of each cell in the current row
        for (int y = 0; y < 4; y++) {
            printf("+");  // Corner of the cell
            if (NORTH_Wall(node[x][y])) {
                printf("-----");  // Horizontal wall
            } else {
                printf("     ");  // No horizontal wall
            }
        }
        printf("+\n");  // End of row
        
        // Print the left and right borders (vertical walls) of each cell
        for (int y = 0; y < 4; y++) {
            if (WEST_Wall(node[x][y])) {
                printf("|");  // Vertical wall
            } else {
                printf(" ");  // No vertical wall
            }
            printf("     ");  // Empty space inside the cell
        }
        // Print the right-most border of the last cell in the row
        printf("|\n");
    }
    
    // Print the bottom border of the last row
    for (int y = 0; y < 4; y++) {
        printf("+-----");
    }
    printf("+\n");
}
void printMazeWallBits() {
    for (int x = 0; x < 4; x++) {
//       int y=0;
        for (int y = 0; y < 4; y++) {
            printf("\r Cell (%d, %d):\n", x, y);
            printf("\r  North Wall: %d\n", NORTH_Wall(array[x][y]) ? 1 : 0);
            printf("\r  East Wall:  %d\n", EAST_Wall(array[x][y]) ? 1 : 0);
            printf("\r  South Wall: %d\n", SOUTH_Wall(array[x][y]) ? 1 : 0);
            printf("\r  West Wall:  %d\n", WEST_Wall(array[x][y]) ? 1 : 0);
            printf("\r  Distance:   %d\n", DISTANCE(array[x][y]));
            
        }
    }
}

bool isValid(int x, int y) {
    return x >= 0 && x < SIZE && y >= 0 && y < SIZE;
}

// Function to construct the path
void constructPath(int array[SIZE][SIZE], int startX, int startY, Coordinate path[MAX], int* pathLength) {
    int x = startX;
    int y = startY;
    *pathLength = 0;  // Initialize path length

    // Continue until the value of the cell is 0 (target)
    while (DISTANCE(array[x][y]) != 0) {
        // Add the current cell to the path
        path[*pathLength].x = x;
        path[*pathLength].y = y;
        (*pathLength)++;

        int currentValue = DISTANCE(array[x][y]);
        int nextX = x, nextY = y;
        int highestValue = -1;

        // Check all valid adjacent cells and find the one with the highest value less than currentValue
        if (isValid(x - 1, y) && !(WEST_Wall(array[x][y])) && !(EAST_Wall(array[x - 1][y])) && DISTANCE(array[x - 1][y]) < currentValue && DISTANCE(array[x - 1][y]) > highestValue) {
            highestValue = DISTANCE(array[x - 1][y]);
            nextX = x - 1;
            nextY = y;
        }
        if (isValid(x + 1, y) && !(EAST_Wall(array[x][y])) && !(WEST_Wall(array[x + 1][y])) && DISTANCE(array[x + 1][y]) < currentValue && DISTANCE(array[x + 1][y]) > highestValue) {
            highestValue = DISTANCE(array[x + 1][y]);
            nextX = x + 1;
            nextY = y;
        }
        if (isValid(x, y - 1) && !(SOUTH_Wall(array[x][y])) && !(NORTH_Wall(array[x][y - 1])) && DISTANCE(array[x][y - 1]) < currentValue && DISTANCE(array[x][y - 1]) > highestValue) {
            highestValue = DISTANCE(array[x][y - 1]);
            nextX = x;
            nextY = y - 1;
        }
        if (isValid(x, y + 1) && !(NORTH_Wall(array[x][y])) && !(SOUTH_Wall(array[x][y + 1])) && DISTANCE(array[x][y + 1]) < currentValue && DISTANCE(array[x][y + 1]) > highestValue) {
            highestValue = DISTANCE(array[x][y + 1]);
            nextX = x;
            nextY = y + 1;
        }

        // Move to the cell with the next highest value
        x = nextX;
        y = nextY;
    }

    // Add the target cell (value = 0) to the path
    path[*pathLength].x = x;
    path[*pathLength].y = y;
    (*pathLength)++;
}

// Function to print the constructed path
void printPath(Coordinate path[MAX], int pathLength) {
    printf("Path from start to target:\n");
    for (int i = 0; i < pathLength; i++) {
        printf("\r(%d, %d)\n", path[i].x, path[i].y);
    }
}

void moveToNextCell(int currentX, int currentY, Direction* direction, int nextX, int nextY) {
    int dx = nextX - currentX;
    int dy = nextY - currentY;

    // Decide the movement based on the direction and next cell coordinates
    switch (*direction) {
        case NORTH:
            if (dx == -1 && dy == 0) {
                // Turn left and move forward
                maze_turn_left();
                printf("\r Move left 1 cell (west)\n");
                *direction = WEST;
            } else if (dx == 1 && dy == 0) {
                // Turn right and move forward
                maze_turn_right();
                
                printf("\r Move right 1 cell (east)\n");
                *direction = EAST;
            } else if (dx == 0 && dy == 1) {
                //move forward
                maze_forward();
                printf("\r Move forward 1 cell (north)\n");
                *direction = NORTH;
            } else if (dx == 0 && dy == -1) {
                // Turn around and move forward
                maze_turn_180();
                
                printf("\r Move backward 1 cell (south)\n");
                *direction = SOUTH;
            }
            break;

        case EAST:
            if (dx == -1 && dy == 0) {
                // Turn around and move forward
                maze_turn_180();
                printf("\r Move backward 1 cell (west)\n");
                *direction = WEST;

            } else if (dx == 1 && dy == 0) {
                //move forward
                maze_forward();
                printf("\r Move forward 1 cell (east)\n");
                *direction = EAST;
                
            } else if (dx == 0 && dy == 1) {
                // Turn left and move forward
                maze_turn_left();
                printf("\r Move left 1 cell (west)\n");
                *direction = NORTH;

            } else if (dx == 0 && dy == -1) {
                // Turn right and move forward
                maze_turn_right();
                
                printf("\r Move right 1 cell (east)\n");
                *direction = SOUTH;
                
            }
            break;

        case SOUTH:
            if (dx == -1 && dy == 0) {
                // Turn right and move forward
                maze_turn_right();
                
                printf("\r Move right 1 cell (east)\n");
                *direction = WEST;
            } else if (dx == 1 && dy == 0) {
                // Turn left and move forward
                maze_turn_left();
                printf("\r Move left 1 cell (west)\n");
                *direction = EAST;
            } else if (dx == 0 && dy == 1) {
                // Turn around and move forward
                maze_turn_180();
                printf("\r Move backward 1 cell (west)\n");
                *direction = NORTH;
            } else if (dx == 0 && dy == -1) {
                //move forward
                maze_forward();
                printf("\r Move forward 1 cell (east)\n");
                *direction = SOUTH;
            }
            break;

        case WEST:
            if (dx == -1 && dy == 0) {
                ///move forward
                maze_forward();
                printf("\r Move forward 1 cell (east)\n");
                *direction = WEST;
            } else if (dx == 1 && dy == 0) {
                // Turn around and move forward
                maze_turn_180();
                printf("\r Move backward 1 cell (west)\n");
                *direction = EAST;
            } else if (dx == 0 && dy == 1) {
                 // Turn right and move forward
                maze_turn_right();
                
                printf("\r Move right 1 cell (east)\n");
                *direction = NORTH;
            } else if (dx == 0 && dy == -1) {
                // Turn left and move forward
                maze_turn_left();
                printf("\r Move left 1 cell (west)\n");
                *direction = SOUTH;
            }
            break;
    }
}
void setWalls(int X, int Y, Direction dir, int right, int left, int front){
    // updetes the maze's walls based on current direction and sensor data regarding the existance of adjacent walls
	if(dir == NORTH){
        
		if(right){
			SET_EAST_Wall(array[X][Y]);
			if(X < 3)
				SET_WEST_Wall(array[X+1][Y]);
		}
		if(left){
			SET_WEST_Wall(array[X][Y]);
			if(X > 0)
				SET_EAST_Wall(array[X-1][Y]);
		}
		if(front){
			SET_NORTH_Wall(array[X][Y]);
			if(Y < 3)
				SET_SOUTH_Wall(array[X][Y+1]);
		}
	} else if(dir == EAST){
		if(right){
			SET_SOUTH_Wall(array[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(array[X][Y-1]);
		}
		if(left){
			SET_NORTH_Wall(array[X][Y]);
			if(Y < 3)
				SET_SOUTH_Wall(array[X][Y+1]);
		}
		if(front){
			SET_EAST_Wall(array[X][Y]);
			if(X < 3)
				SET_WEST_Wall(array[X+1][Y]);
		}
	} else if(dir == SOUTH){
		if(right){
			SET_WEST_Wall(array[X][Y]);
			if(X > 0)
				SET_EAST_Wall(array[X-1][Y]);
		}
		if(left){
			SET_EAST_Wall(array[X][Y]);
			if(X < 3)
				SET_WEST_Wall(array[X+1][Y]);
		}
		if(front){
			SET_SOUTH_Wall(array[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(array[X][Y-1]);
		}
	} else if(dir == WEST){
		if(right){
			SET_NORTH_Wall(array[X][Y]);
			if(Y < 3)
				SET_SOUTH_Wall(array[X][Y+1]);
		}
		if(left){
			SET_SOUTH_Wall(array[X][Y]);
			if(Y > 0)
				SET_NORTH_Wall(array[X][Y-1]);
		}
		if(front){
			SET_WEST_Wall(array[X][Y]);
			if(X > 0)
				SET_EAST_Wall(array[X-1][Y]);
		}
	}  
}

void initializeArray(int array[4][4]) {
    // maze initialization
    int defaultValues[4][4] = {
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255},
        {255, 255, 255, 255}
    };

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            SET_DISTANCE(array[i][j], defaultValues[i][j]);
        }
    }
}
void explore(int startX, int startY, Direction direction, int targetX, int targetY){
    initializeArray(array);
    setMazeBorders();
    setWalls(startX, startY, direction, getRight(), getLeft(), getFront());
    // Set target cell
    SET_DISTANCE(array[targetX][targetY], 0);
    Queue q;
    initQueue(&q);
    enqueue(&q, targetX, targetY);  // Start filling the maze values from the target
    processQueue(&q, array, targetX,targetY);
    printArray(array);
    
    // Calculate initial path
    int currentX = startX;
    int currentY = startY;
    Coordinate path[MAX];  // To store the path
    int pathLength;  // To store the length of the path
    constructPath(array, currentX, currentY, path, &pathLength);
    printPath(path, pathLength);
//    printMazeWallBits();
    while (currentX != targetX || currentY != targetY) {
//        for (int i = 1; i < pathLength; i++) {
//            printf("entered i= %d", i);
            int nextX = path[1].x;
            int nextY = path[1].y;
            printf("current x: %d, current y: %d, nextX: %d,nextY: %d\n", currentX, currentY, nextX, nextY);
            moveToNextCell(currentX, currentY, &direction, nextX, nextY);  // Take one step
            
             // Update maze after each move
            update_position(&currentX, &currentY, direction, 1);
            setWalls(currentX, currentY, direction, getRight(), getLeft(), getFront());
//            printMazeWallBits();
            initializeArray(array);  // Recalculate the flood fill
            SET_DISTANCE(array[targetX][targetY], 0);
            enqueue(&q, targetX, targetY);
            processQueue(&q, array, targetX, targetY);  // Recalculate maze values
            
            printf("\r\n");
            printArray(array);
    
            constructPath(array, currentX, currentY, path, &pathLength);  // Recalculate path
            printPath(path, pathLength);
//        }
    }
    printf("DONE! ");
    printMazeWallBits();
    
    
   
}

void DriveSP(int startX, int startY, Direction direction, int targetX, int targetY){
    
}
