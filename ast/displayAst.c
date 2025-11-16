#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "ast.h"
#include <stdbool.h>

SDL_Window *win;
SDL_Renderer *ren;

#define WIDTH 700
#define HEIGHT 500
#define GAP_HOR 50
#define GAP_VER 100
#define MOVE_ARROW 25
#define PADDING 5

void destroyWindow(void){
    SDL_DestroyWindow(win);
}

void destroyRenderer(void){
    SDL_DestroyRenderer(ren);
}

void createWindow(char* title, int x, int y, int length, int height){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Error in init :%s", SDL_GetError());
        printf("SDL_Init Error: %s\n", SDL_GetError());
        exit(1);        
    }
    
    atexit(&SDL_Quit);

    win = SDL_CreateWindow(title, x, y, length, height, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(&destroyWindow);
        
}

void createRenderer(void){
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (ren == NULL) {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        exit(1);
    }
    atexit(&destroyRenderer);
}

TTF_Font *createFont(char *path, int size){
    if (TTF_Init() == -1) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        exit(1);
    }
    atexit(&TTF_Quit);

    TTF_Font *newFont = TTF_OpenFont(path, size);
    if (!newFont) {
        printf("Failed to load font! TTF_Error: %s\n", TTF_GetError());
        exit(1);
    }    
    return newFont;

}

void drawTexture(SDL_Texture *texture, float x, float y, float length, float height){
    SDL_Rect rect = {x,y,length,height};
    SDL_RenderCopy(ren,texture, NULL, &rect);
}

SDL_Texture *createText(char *text, SDL_Color col, TTF_Font *fontText){
    SDL_Surface* textSurface = TTF_RenderText_Solid(fontText, text, col);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(ren, textSurface);
    SDL_FreeSurface(textSurface);
    return textTexture;
}

void drawRectangle( int x, int y, int length, int height, SDL_Color color){
    SDL_Rect rect = {x, y, length, height};
    SDL_SetRenderDrawColor(ren, color.r,color.g, color.b,color.a);
    SDL_RenderDrawRect(ren, &rect);
}


int displayNode(char *text, int x, int y, SDL_Color color, TTF_Font *fontText, int xPere, int yPere, bool drawLine){
    SDL_Texture *textureText = createText(text, color, fontText);
    int l1, h1;
    SDL_QueryTexture(textureText, NULL, NULL, &l1, &h1);
    drawTexture(textureText,x-l1/2,y-h1/2,l1,h1);
    SDL_DestroyTexture(textureText);
    drawRectangle(x - PADDING - l1/2, y - PADDING - h1/2, l1 + 2*PADDING, h1 + 2*PADDING, (SDL_Color){0,0,0,255});
    if (drawLine) {
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_RenderDrawLine(ren, x, y - h1 / 2 - PADDING, xPere, yPere);
    }
    //Return the next y to draw the next line
    return y+h1/2+PADDING;
}

char *itoa(int i){
    static char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d", i);
    return buffer;
}

char *ctoce(char c){
    static char str[2];
    str[0] = c;
    str[1] = '\0';
    return str;
}

void displayAST(AST *ast, TTF_Font *font, double scale, int offsetX, int offsetY, int depth, int depthMax, int numColumn, int numColumnMax, int xPere, int yPere, bool drawLine){
    // yPere is useful to draw the lines between the nodes
    if (ast == NULL) {
        return;
    }
    int gap = 2 << (depthMax - depth);
    int x = xPere - GAP_HOR*gap*numColumnMax + 2*GAP_HOR*gap*numColumn + offsetX;
    int y = GAP_VER * depth + offsetY;
    SDL_Color color = {0, 0, 0, 255};
    switch (ast->type){
        case (NUMBER):
            displayNode(itoa(ast->data.number), x, y, color, font, xPere + offsetX, yPere, drawLine);
            break;
        case (BINARY_OP):{
            int newY = displayNode(ctoce(ast->data.binary.op), x, y, color, font, xPere + offsetX, yPere, drawLine);
            displayAST(ast->data.binary.left, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 1, x-offsetX, newY, true);
            displayAST(ast->data.binary.right, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 1, x-offsetX, newY, true);
            break;
        }
        case (UNARY_OP):{
            int newY = displayNode(ctoce(ast->data.unary.op), x, y, color, font, xPere + offsetX, yPere, drawLine);
            displayAST(ast->data.unary.operand, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 0, x-offsetX, newY, true);
            break;
        }
        case (VARIABLE):
            displayNode(ast->data.variable, x, y, color, font, xPere, yPere, drawLine);
            break;
        default:
            break;
    }
}

void mainloop(AST *ast){
    createWindow("AST", 50,50,WIDTH,HEIGHT);
    createRenderer();
    TTF_Font *font = createFont("ast/font.ttf", 16);
    SDL_Event event;
    bool running = true;
    int offsetX = 0;
    int offsetY = 50;
    double scale = 1;
    int astDepth = depth(ast);
    bool mouseMoving = false;
    int xMouse;
    int yMouse;
    while (running){
        while (SDL_PollEvent(&event)) {
            switch (event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouseMoving = true;
                        SDL_GetMouseState(&xMouse, &yMouse);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouseMoving = false;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (mouseMoving){
                        int newX;
                        int newY; 
                        SDL_GetMouseState(&newX, &newY);
                        offsetX += newX - xMouse;
                        offsetY += newY - yMouse;
                        xMouse = newX;
                        yMouse = newY;
                    }
                    break;
                case SDL_KEYDOWN:{
                    switch (event.key.keysym.sym){
                    case (SDLK_q):
                        running = false;
                        break;
                    case (SDLK_LEFT):
                        offsetX += MOVE_ARROW;
                        break;
                    case (SDLK_RIGHT):
                        offsetX -= MOVE_ARROW;
                        break;
                    case (SDLK_UP):
                        offsetY += MOVE_ARROW;
                        break;
                    case (SDLK_DOWN):
                        offsetY -= MOVE_ARROW;
                        break;
                    default:
                        break;    
                    }
                    break;
                }
                default:
                    break;
            }
        }
        SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
        SDL_RenderClear(ren);
        displayAST(ast, font, scale, offsetX, offsetY, 0, astDepth, 0, 0, WIDTH/2, -1, false);
        SDL_RenderPresent(ren);
    }
    exit(0);
}

// int main(void){
//     AST *op1 = create_binary_op_node('*', create_number_node(54),create_number_node(32));
//     // AST *op2 = create_binary_op_node('/', create_number_node(3), create_number_node(9));
//     AST *op2 = create_unary_op_node('-', create_number_node(63));
//     AST *op3 = create_binary_op_node('-', op1, op2);
//     // AST *op4 = create_binary_op_node('+',op3,create_number_node(12));
//     mainloop(op3);
//     return 0;
// }
