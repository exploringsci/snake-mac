#include <iostream>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <curses.h>

// Linked list for snake
struct Node{
    int ypos;
    int xpos;
    Node* next;
};

Node* tail = new Node;
Node* face = new Node;


char gameArray[10][10];
int head[2];
int direction[2] = {0, 0};
int velocity; // blocks per second
int length = 1;
float delay;
int loopCount = 0;
int rottenAppleAge = -1;

// Update snake's direction based on key pressed. 
void updateDirection(){
    int dir = -1;
    refresh();
    dir = getch();
    std::cin.clear();

    if (dir == KEY_UP){
        if (direction[0] == 1){
            return;
        }
        direction[0] = -1;
        direction[1] = 0;
    }
    else if (dir == KEY_LEFT){
        if (direction[1] == 1){
            return;
        }
        direction[0] = 0;
        direction[1] = -1;
    }
    else if (dir == KEY_RIGHT){
        if (direction[1] == -1){
            return;
        }
        direction[0] = 0;
        direction[1] = 1;
    }
    else if (dir == KEY_DOWN){
        if (direction[0] == -1){
            return;
        }
        direction[0] = 1;
        direction[1] = 0;
    } 
}

// If player wins
void win(){
    std::cout << "win" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << ":)" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100000));
    exit(0);
}

// If player loses
void gameOver(){
    std::cout << "lost" << std::endl;
    //system("vlc sad_trombone.ogg > capture");
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl;
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << "\\" << std::endl; 
    std::cout << ":(" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100000));
    exit(0);
}

// Print game board
void printGame(){
    erase();
    refresh();
    std::cout << "**********************" << "\n\r";
    for (int i = 0; i < 10; i ++){
        std::cout << "|";
        for (int j = 0; j < 10; j ++){
            std::cout << gameArray[i][j] << " ";
        }
        std::cout << "|" << "\n\r";
    }
    std::cout << "**********************" << "\n\r" << std::endl;
    refresh();

}

// Update snake velocity based on number of game loops
void updateVelocity(){
    velocity = loopCount / 100 + 2;
    velocity = velocity < 1 ? 1 : velocity; // If velocity == 0, set to 1 (default). 
}

// Place good apple on board
void placeApple(){
    // find number of blank squares (possible positions for a new apple)
    int numBlank = 0;
    for (int i = 0; i < 10; i ++){
        for (int j = 0; j < 10; j ++){
            if (gameArray[i][j] != 'X'){
                numBlank ++;
            }
        }
    }

    // place the apple (indicated by a '+') randomly
    int randPosn = int(rand() % numBlank);
    int posnCounter = 0;
    for (int i = 0; i < 10; i ++){
        for (int j = 0; j < 10; j ++){
            if (gameArray[i][j] != 'X'){
                if (posnCounter == randPosn){
                    gameArray[i][j] = '+';
                }
                posnCounter ++;
            }
        }
    }
}

// Place bad apple on board
void placeBadApple(){
    // find number of blank squares (possible positions for a new apple)
    int numBlank = 0;
    for (int i = 0; i < 10; i ++){
        for (int j = 0; j < 10; j ++){
            if (gameArray[i][j] == ' '){
                numBlank ++;
            }
        }
    }

    // place the apple (indicated by a '+') randomly
    int randPosn = int(rand() % numBlank);
    int posnCounter = 0;
    for (int i = 0; i < 10; i ++){
        for (int j = 0; j < 10; j ++){
            if (gameArray[i][j] == ' '){
                if (posnCounter == randPosn){
                    gameArray[i][j] = '~';
                }
                posnCounter ++;
            }
        }
    }
    rottenAppleAge = 5;
}

// Remove bad apple if it was avoided for long enough. 
void removeBadApple(){
    for (int i = 0; i < 10; i ++){
        for (int j = 0; j < 10; j ++){
            if (gameArray[i][j] == '~'){
                gameArray[i][j] = ' ';
            }
        }
    }
}

// Set up board
void setup(void){
    //ncurses necessary for proper output
    endwin();
    refresh();
    clear();
    wresize(stdscr, 12, 12);

    // Board starts out blank
    for (int i = 0; i < 10; i ++){
        for (int j = 0; j < 10; j ++){
            gameArray[i][j] = ' ';
        }
    }

    // Set initial snake position and velocity, place first apple. 
    head[0] = 5;
    head[1] = 5;
    velocity = 1;
    tail->ypos = 5;
    tail->xpos = 5;
    face = tail;
    placeApple();

    //ncurses
    initscr();
    nodelay(stdscr, true);
    keypad(stdscr, TRUE);
}

// Game loop
void loop(void){
    loopCount ++;
    updateDirection();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / velocity)); // remove in arduino code. 
    updateDirection();

    if (rottenAppleAge < 0){ // if rotten apple was avoided for long enough
        removeBadApple();
    }

    //If snake hits itself, game over
    if(gameArray[head[0] + direction[0]][head[1] + direction[1]] == 'X'){
        gameOver();
    }

    //If snake hits wall, game over
    else if(head[0] + direction[0] < 0 || head[1] + direction[1] < 0 || head[0] + direction[0] > 9 || head[1] + direction[1] > 9){
        gameOver();
    }

    //If snake hits good apple
    else if(gameArray[head[0] + direction[0]][head[1] + direction[1]] == '+'){
        if (length > 20){ // Win case
            win();
        }

        
        rottenAppleAge --;
        length++;

        // Add length to snake
        gameArray[head[0] + direction[0]][head[1] + direction[1]] = 'X';
        head[0] += direction[0];
        head[1] += direction[1];
        Node* nextNode = new Node;
        face->next = nextNode;
        nextNode->ypos = head[0];
        nextNode->xpos = head[1];
        nextNode->next = nullptr;
        face = nextNode;

        //Place good apple and rotten apple if length is a multiple of 5. 
        placeApple();
        if (length % 5 == 0){
            placeBadApple();
        }
    }

    // If snake hits bad apple
    else if(gameArray[head[0] + direction[0]][head[1] + direction[1]] == '~' ){
        length-= 4; // length decreases by 4. 
        gameArray[head[0] + direction[0]][head[1] + direction[1]] = 'X';
        head[0] += direction[0];
        head[1] += direction[1];

        //Tail retracts by 4. (5 retractions total since snake moves forward by 1 position)
        Node* nextNode = new Node;
        face->next = nextNode;
        nextNode->ypos = head[0];
        nextNode->xpos = head[1];
        nextNode->next = nullptr;
        face = nextNode;
        gameArray[tail->ypos][tail->xpos] = ' ';
        tail = tail->next;
        gameArray[tail->ypos][tail->xpos] = ' ';
        tail = tail->next;
        gameArray[tail->ypos][tail->xpos] = ' ';
        tail = tail->next;
        gameArray[tail->ypos][tail->xpos] = ' ';
        tail = tail->next;
        gameArray[tail->ypos][tail->xpos] = ' ';
        tail = tail->next;
    }

    // If normal movement
    else{
        gameArray[head[0] + direction[0]][head[1] + direction[1]] = 'X';
        head[0] += direction[0];
        head[1] += direction[1];
        Node* nextNode = new Node;
        face->next = nextNode;
        nextNode->ypos = head[0];
        nextNode->xpos = head[1];
        nextNode->next = nullptr;
        face = nextNode;
        gameArray[tail->ypos][tail->xpos] = ' ';
        tail = tail->next;
    }
    printGame();
    updateVelocity();
    updateDirection();
}

int main(){
    setup();
    while(true){
        loop();
    }
    return 0;
}
