#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include <ast/ast.h>

SDL_Window *win;
SDL_Renderer *ren;

#define WIDTH 1400
#define HEIGHT 1000
#define GAP_HOR 50
#define GAP_VER 100
#define MOVE_ARROW 25
#define PADDING 5
#define ZOOM_IN_MAX 5
#define ZOOM_OUT_MAX 0.01
#define DEFAULT_FONT_SIZE 16
#define FONT_PATH "ast/font.ttf"
#define FONT_SIZE_MIN 8
#define DEFAULT_OFFSET_Y 50

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

void initTTF(void){
    if (TTF_Init() == -1) {
        printf("TTF could not initialize! TTF_Error: %s\n", TTF_GetError());
        exit(1);
    }
    atexit(&TTF_Quit);
}

TTF_Font *createFont(char *path, int size){
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

void drawRectangle(double x, double y, int length, int height, SDL_Color color){
    SDL_Rect rect = {x, y, length, height};
    SDL_SetRenderDrawColor(ren, color.r,color.g, color.b,color.a);
    SDL_RenderDrawRect(ren, &rect);
}


double displayNode(char *text, double x, double y, SDL_Color color, TTF_Font *fontText, double xPere, double yPere, bool drawLine){
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

char *stringBinaryOp(BinaryOp op){
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
        case OP_MOD: return "%";
        case OP_EQ: return "==";
        case OP_NEQ: return "!=";
        case OP_LT: return "<";
        case OP_LTE: return "<=";
        case OP_GT: return ">";
        case OP_GTE: return ">=";
        case OP_AND: return "&&";
        case OP_OR: return "||";
        case OP_ASSIGN: return "=";
        default: return "?";
    }
}

char *stringUnaryOp(UnaryOp op){
    switch (op) {
        case UNARY_NOT: return "!";
        case UNARY_NEG: return "-";
        default: return "?";
    }
}

char *stringType(Type *type){
    switch (type->kind) {
        case TYPE_INT: return "int";
        case TYPE_BOOLEAN: return "boolean";
        case TYPE_STRING: return "string";
        case TYPE_VOID: return "void";
        case TYPE_CLASS: return type->class_name;
        default: return "?";
    }
}

bool isPublic(int modifier){
    return (modifier & MOD_PUBLIC) != 0;
}

bool isPrivate(int modifier){
    return (modifier & MOD_PRIVATE) != 0;
}

bool isStatic(int modifier){
    return (modifier & MOD_STATIC) != 0;
}

void displayAST(AST ast, TTF_Font *font, double scale, int offsetX, int offsetY, int depth, int depthMax, int numColumn, int numColumnMax, double xPere, double yPere, bool drawLine){
    // yPere is useful to draw the lines between the nodes
    double gap = (2 << (depthMax - depth))*scale*scale;
    double x = xPere + (- GAP_HOR*gap*numColumnMax + 2*GAP_HOR*gap*numColumn) + offsetX;
    double y = scale/scale*(GAP_VER * depth) + offsetY;
    SDL_Color color = {0, 0, 0, 255};
    switch (ast.type){
        case (AST_TYPE):{
            displayNode(stringType(ast.data.type), x, y, color, font, xPere + offsetX, yPere, drawLine);
            break;
        }
        case (AST_NAME):{
            displayNode(ast.data.name, x, y, color, font, xPere + offsetX, yPere, drawLine);
            break;
        }
        case (AST_EXPRESSION):{
            switch (ast.data.expression->kind){
                case (EXPR_LITERAL):{
                    switch (ast.data.expression->literal.kind){
                        case (LIT_INT):
                            displayNode(itoa(ast.data.expression->literal.int_val), x, y, color, font, xPere + offsetX, yPere, drawLine);
                            break;
                        case (LIT_BOOLEAN):
                            if (ast.data.expression->literal.bool_val){
                                displayNode("TRUE", x, y, color, font, xPere + offsetX, yPere, drawLine);
                            }
                            else{
                                displayNode("FALSE", x, y, color, font, xPere + offsetX, yPere, drawLine);
                            }
                            break;
                        case (LIT_STRING):{
                            char *str = malloc(strlen(ast.data.expression->literal.string_val) + 3);
                            sprintf(str, "\"%s\"", ast.data.expression->literal.string_val);
                            displayNode(str, x, y, color, font, xPere + offsetX, yPere, drawLine);
                            free(str);
                            break;
                        }
                        case (LIT_NULL):
                            displayNode("NULL", x, y, color, font, xPere + offsetX, yPere, drawLine);
                            break;
                    }
                    break;
                }
                case (EXPR_VARIABLE):{
                    displayNode(ast.data.expression->var_name, x, y, color, font, xPere + offsetX, yPere, drawLine);
                    break;
                }
                case (EXPR_BINARY_OP):{
                    double newY = displayNode(stringBinaryOp(ast.data.expression->binary.op), x, y, color, font, xPere + offsetX, yPere, drawLine);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->binary.left}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 1, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->binary.right}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 1, x-offsetX, newY, true);
                    break;
                }
                case (EXPR_UNARY_OP):{
                    double newY = displayNode(stringUnaryOp(ast.data.expression->unary.op), x, y, color, font, xPere + offsetX, yPere, drawLine);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->unary.operand}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 0, x-offsetX, newY, true);
                    break;
                }
                case (EXPR_CALL):{
                    double newY = displayNode("CALL", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    int nbChildren = 1 + ast.data.expression->call.arg_count;
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->call.callee}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, nbChildren - 1, x-offsetX, newY, true);
                    for (int i = 0; i < ast.data.expression->call.arg_count; i++){
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->call.args[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, i+1, nbChildren - 1, x-offsetX, newY, true);
                    }
                    break;
                }
                case (EXPR_INSTOF):{
                    double newY = displayNode("INSTANCEOF", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->instof.val}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 1, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_TYPE, .data.type = ast.data.expression->instof.comp}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 1, x-offsetX, newY, true);
                    break;
                }
                case (EXPR_NEW):{
                    double newY = displayNode("NEW", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    int nbChildren = 1 + ast.data.expression->new_expr.arg_count;
                    displayAST((AST){.type = AST_TYPE, .data.type = ast.data.expression->new_expr.type}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, nbChildren - 1, x-offsetX, newY, true);
                    for (int i = 0; i < ast.data.expression->new_expr.arg_count; i++){
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->new_expr.args[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, i+1, nbChildren, x-offsetX, newY, true);
                    }
                    break;
                }
                case (EXPR_FIELD_ACCESS):{
                    double newY = displayNode(".", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->field_access.object}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 1, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_NAME, .data.name = ast.data.expression->field_access.field_name}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 1, x-offsetX, newY, true);
                    break;
                }
                case (EXPR_CAST):{
                    double newY = displayNode("CAST", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    displayAST((AST){.type = AST_TYPE, .data.type = ast.data.expression->cast.target_type}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 1, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->cast.expr}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 1, x-offsetX, newY, true);
                    break;
                }
            }
            break;
        }
        case (AST_STATEMENT):{
            switch (ast.data.statement->kind){
                case (STMT_EXPR):{
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->expr}, font, scale, offsetX, offsetY, depth, depthMax, 0, 0, x-offsetX, yPere, true);
                    break;
                }
                case (STMT_BLOCK):{
                    double newY = displayNode("BLOCK", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    int nbChildren = ast.data.statement->block.count;
                    for (int i = 0; i < ast.data.statement->block.count; i++){
                        displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->block.stmts[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, i, nbChildren - 1, x-offsetX, newY, true);
                    }
                    break;
                }
                case (STMT_IF):{
                    double newY = displayNode("IF", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->if_stmt.condition}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 2, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->if_stmt.then_branch}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 2, x-offsetX, newY, true);
                    if (ast.data.statement->if_stmt.else_branch != NULL){
                        displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->if_stmt.else_branch}, font, scale, offsetX, offsetY, depth+1, depthMax, 2, 2, x-offsetX, newY, true);
                    }
                    break;
                }
                case (STMT_FOR):{
                    double newY = displayNode("FOR", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->for_stmt.init}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 3, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->for_stmt.condition}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 3, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->for_stmt.increment}, font, scale, offsetX, offsetY, depth+1, depthMax, 2, 3, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->for_stmt.body}, font, scale, offsetX, offsetY, depth+1, depthMax, 3, 3, x-offsetX, newY, true);
                    break;
                }
                case (STMT_RETURN):{
                    double newY = displayNode("RETURN", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    if (ast.data.statement->return_val != NULL){
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->return_val}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 0, x-offsetX, newY, true);
                    }
                    break;
                }
                case (STMT_VAR_DECL):{
                    double newY = displayNode("VAR_DECL", x, y, color, font, xPere + offsetX, yPere, drawLine);
                    displayAST((AST){.type = AST_TYPE, .data.type = ast.data.statement->var_decl.type}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 2, x-offsetX, newY, true);
                    displayAST((AST){.type = AST_NAME, .data.name = ast.data.statement->var_decl.name}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 2, x-offsetX, newY, true);
                    if (ast.data.statement->var_decl.initializer != NULL){
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->var_decl.initializer}, font, scale, offsetX, offsetY, depth+1, depthMax, 2, 2, x-offsetX, newY, true);
                    }
                    break;
                }
                default:
                    break;
            }
            break;
        }
        case (AST_PARAMETER):{
            double newY = displayNode("PARAM", x, y, color, font, xPere + offsetX, yPere, drawLine);
            displayAST((AST){.type = AST_TYPE, .data.type = ast.data.parameter->type}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, 1, x-offsetX, newY, true);
            displayAST((AST){.type = AST_NAME, .data.name = ast.data.parameter->name}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, 1, x-offsetX, newY, true);
            break;
        }
        case (AST_CONSTRUCTOR):{
            double newY = displayNode("CONSTRUCTOR", x, y, color, font, xPere + offsetX, yPere, drawLine);
            int nbChildren = 1 + ast.data.constructor->param_count;
            for (int i = 0; i < ast.data.constructor->param_count; i++){
                displayAST((AST){.type = AST_PARAMETER, .data.parameter = ast.data.constructor->params[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, i, nbChildren - 1, x-offsetX, newY, true);
            }
            displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.constructor->body}, font, scale, offsetX, offsetY, depth+1, depthMax, nbChildren - 1, nbChildren - 1, x-offsetX, newY, true);
            break;
        }
        case (AST_METHOD):{
            double newY = displayNode("METHOD", x, y, color, font, xPere + offsetX, yPere, drawLine);
            int nbChildren = 3 + ast.data.method->param_count;
            int indexStart = 0;
            if (isPublic(ast.data.method->modifiers)){
                nbChildren ++;
            }
            if (isPrivate(ast.data.method->modifiers)){
                nbChildren ++;
            }
            if (isStatic(ast.data.method->modifiers)){
                nbChildren ++;
            }
            if (isPublic(ast.data.method->modifiers)){
                displayAST((AST){.type = AST_NAME, .data.name = "public"}, font, scale, offsetX, offsetY, depth+1, depthMax, indexStart, nbChildren - 1, x-offsetX, newY, true);
                indexStart ++;
            }
            else if (isPrivate(ast.data.method->modifiers)){
                displayAST((AST){.type = AST_NAME, .data.name = "private"}, font, scale, offsetX, offsetY, depth+1, depthMax, indexStart, nbChildren - 1, x-offsetX, newY, true);
                indexStart ++;
            }
            if (isStatic(ast.data.method->modifiers)){
                displayAST((AST){.type = AST_NAME, .data.name = "static"}, font, scale, offsetX, offsetY, depth+1, depthMax, indexStart, nbChildren - 1, x-offsetX, newY, true);
                indexStart ++;
            }
            displayAST((AST){.type = AST_TYPE, .data.type = ast.data.method->return_type}, font, scale, offsetX, offsetY, depth+1, depthMax, indexStart, nbChildren - 1, x-offsetX, newY, true);
            indexStart++;
            displayAST((AST){.type = AST_NAME, .data.name = ast.data.method->name}, font, scale, offsetX, offsetY, depth+1, depthMax, indexStart, nbChildren - 1, x-offsetX, newY, true);
            indexStart++;
            for (int i = 0; i < ast.data.method->param_count; i++){
                displayAST((AST){.type = AST_PARAMETER, .data.parameter = ast.data.method->params[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, i+indexStart, nbChildren - 1, x-offsetX, newY, true);
            }
            displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.method->body}, font, scale, offsetX, offsetY, depth+1, depthMax, nbChildren - 1, nbChildren - 1, x-offsetX, newY, true);
            break;
        }
        case (AST_FIELD):{
            double newY = displayNode("ATTRIBUTE", x, y, color, font, xPere + offsetX, yPere, drawLine);
            int nbChildren = 2 + (ast.data.field->initializer != NULL ? 1 : 0);
            displayAST((AST){.type = AST_TYPE, .data.type = ast.data.field->type}, font, scale, offsetX, offsetY, depth+1, depthMax, 0, nbChildren - 1, x-offsetX, newY, true);
            displayAST((AST){.type = AST_NAME, .data.name = ast.data.field->name}, font, scale, offsetX, offsetY, depth+1, depthMax, 1, nbChildren - 1, x-offsetX, newY, true);
            if (ast.data.field->initializer != NULL){
                displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.field->initializer}, font, scale, offsetX, offsetY, depth+1, depthMax, 2, nbChildren - 1, x-offsetX, newY, true);
            }
            break;
        }
        case (AST_CLASS_DECL):{
            double newY = displayNode(ast.data.class_decl->name, x, y, color, font, xPere + offsetX, yPere, drawLine);
            int nbChildren = ast.data.class_decl->field_count + ast.data.class_decl->constructor_count + ast.data.class_decl->method_count;
            int index = 0;
            for (int i = 0; i < ast.data.class_decl->field_count; i++){
                displayAST((AST){.type = AST_FIELD, .data.field = ast.data.class_decl->fields[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, index, nbChildren - 1, x-offsetX, newY, true);
                index++;
            }
            for (int i = 0; i < ast.data.class_decl->constructor_count; i++){
                displayAST((AST){.type = AST_CONSTRUCTOR, .data.constructor = ast.data.class_decl->constructors[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, index, nbChildren - 1, x-offsetX, newY, true);
                index++;
            }
            for (int i = 0; i < ast.data.class_decl->method_count; i++){
                displayAST((AST){.type = AST_METHOD, .data.method = ast.data.class_decl->methods[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, index, nbChildren - 1, x-offsetX, newY, true);
                index++;
            }
            break;
        }
        case (AST_FILE):{
            double newY = displayNode("FILE", x, y, color, font, xPere + offsetX, yPere, drawLine);
            int nbChildren = ast.data.file->class_count;
            for (int i = 0; i < ast.data.file->class_count; i++){
                displayAST((AST){.type = AST_CLASS_DECL, .data.class_decl = ast.data.file->classes[i]}, font, scale, offsetX, offsetY, depth+1, depthMax, i, nbChildren - 1, x-offsetX, newY, true);
            }
            break;
        }
        default:
            break;
    }
}

void mainloop(File *ast){
    createWindow("AST", 50,50,WIDTH,HEIGHT);
    createRenderer();
    initTTF();
    TTF_Font *font = createFont(FONT_PATH, DEFAULT_FONT_SIZE);
    SDL_Event event;
    bool running = true;
    int offsetX = 0;
    int offsetY = DEFAULT_OFFSET_Y;
    double scale = 1;
    int astDepth = depthFile(ast);
    bool mouseMoving = false;
    int xMouse;
    int yMouse;
    while (running){
        while (SDL_PollEvent(&event)) {
            switch (event.type){
                case SDL_QUIT:
                    running = false;
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0 && scale < ZOOM_IN_MAX) { // Scroll up
                        scale *= 1.1;
                    } else if (event.wheel.y < 0 && scale > ZOOM_OUT_MAX) { // Scroll down
                        scale /= 1.1;
                    }
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
                        offsetX += (newX - xMouse)/scale;
                        offsetY += (newY - yMouse)/scale;
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
                        case (SDLK_r): //Reset
                            offsetX = 0;
                            offsetY = DEFAULT_OFFSET_Y;
                            scale = 1;
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
        displayAST((AST){.type = AST_FILE, .data.file = ast} , font, scale, offsetX*scale, offsetY*scale, 0, astDepth, 0, 0, WIDTH/2, -1, false);
        SDL_RenderPresent(ren);
    }
    TTF_CloseFont(font);
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
