#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include <ast/ast.h>

SDL_Window *win;
SDL_Renderer *ren;

#define WIDTH 1400
#define HEIGHT 1000
#define GAP_HOR 2
#define GAP_VER 100
#define MOVE_ARROW 25
#define PADDING 5
#define ZOOM_IN_MAX 50
#define ZOOM_OUT_MAX 0.00001
#define DEFAULT_FONT_SIZE 16
#define FONT_PATH "ast/font.ttf"
#define FONT_SIZE_MIN 8
#define DEFAULT_OFFSET_X WIDTH/2
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

int gapBetweenChildren(int xMin, int xMax, int nbChildren){
    return (xMax - xMin) / nbChildren;
}

int calculateXChildren(int xMin, int xMax, int nbChildren, int indexChild){
    return xMin + indexChild*gapBetweenChildren(xMin, xMax, nbChildren) + gapBetweenChildren(xMin, xMax, nbChildren)/2;
}

void displayAST(AST ast, TTF_Font *font, double scale, int offsetX, int offsetY, int xMin, int xMax, int xCurrent, int yCurrent, int xPere, int yPere, bool drawLine){
    SDL_Color color = {0, 0, 0, 255};
    switch (ast.type){
        case (AST_EXPRESSION):{
            switch (ast.data.expression->kind){
                case (EXPR_BINARY_OP):{
                    int newYPere = displayNode(stringBinaryOp(ast.data.expression->binary.op), xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 2;
                    int x0 = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int x1 = calculateXChildren(xMin, xMax, nbChildren, 1);
                    int newY = yCurrent + GAP_VER*scale;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->binary.left}, font, scale, offsetX, offsetY, x0 - gap/2, x0 + gap/2, x0, newY, xCurrent + offsetX, newYPere, true);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->binary.right}, font, scale, offsetX, offsetY, x1 - gap/2, x1 + gap/2, x1, newY, xCurrent + offsetX, newYPere, true);
                    break;
                }
                case (EXPR_UNARY_OP):{
                    int newYPere = displayNode(stringUnaryOp(ast.data.expression->unary.op), xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 1;
                    int x0 = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int newY = yCurrent + GAP_VER*scale;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->unary.operand}, font, scale, offsetX, offsetY, x0 - gap/2, x0 + gap/2, x0, newY, xCurrent + offsetX, newYPere, true);
                    break;
                }
                case (EXPR_VARIABLE):{
                    displayNode(ast.data.expression->var_name, xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    break;
                }
                case (EXPR_CALL):{
                    int newYPere = displayNode("CALL", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = ast.data.expression->call.arg_count + 1;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    int xCallee = calculateXChildren(xMin, xMax, nbChildren, 0);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->call.callee}, font, scale, offsetX, offsetY, xCallee - gap/2, xCallee + gap/2, xCallee, newY, xCurrent + offsetX, newYPere, true);
                    for (int i = 0; i < ast.data.expression->call.arg_count; i++){
                        int xArg = calculateXChildren(xMin, xMax, nbChildren, i+1);
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->call.args[i]}, font, scale, offsetX, offsetY, xArg - gap/2, xArg + gap/2, xArg, newY, xCurrent + offsetX, newYPere, true);
                    }
                    break;
                }
                case (EXPR_INSTOF):{
                    int newYPere = displayNode("INSTANCEOF", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 2;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    int xVal = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int xComp = calculateXChildren(xMin, xMax, nbChildren, 1);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->instof.val}, font, scale, offsetX, offsetY, xVal - gap/2, xVal + gap/2, xVal, newY, xCurrent + offsetX, newYPere, true);
                    displayAST((AST){.type = AST_TYPE, .data.type = ast.data.expression->instof.comp}, font, scale, offsetX, offsetY, xComp - gap/2, xComp + gap/2, xComp, newY, xCurrent + offsetX, newYPere, true);
                    break;
                }
                case (EXPR_NEW):{
                    int newYPere = displayNode("NEW", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = ast.data.expression->new_expr.arg_count + 1;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    int xType = calculateXChildren(xMin, xMax, nbChildren, 0);
                    displayAST((AST){.type = AST_TYPE, .data.type = ast.data.expression->new_expr.type}, font, scale, offsetX, offsetY, xType - gap/2, xType + gap/2, xType, newY, xCurrent + offsetX, newYPere, true);
                    for (int i = 0; i < ast.data.expression->new_expr.arg_count; i++){
                        int xArg = calculateXChildren(xMin, xMax, nbChildren, i+1);
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->new_expr.args[i]}, font, scale, offsetX, offsetY, xArg - gap/2, xArg + gap/2, xArg, newY, xCurrent + offsetX, newYPere, true);
                    }
                    break;
                }
                case (EXPR_FIELD_ACCESS):{
                    int newYPere = displayNode("DOT", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 2;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    int xObject = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int xFieldName = calculateXChildren(xMin, xMax, nbChildren, 1);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->field_access.object}, font, scale, offsetX, offsetY, xObject - gap/2, xObject + gap/2, xObject, newY, xCurrent + offsetX, newYPere, true);
                    displayAST((AST){.type = AST_NAME, .data.name = ast.data.expression->field_access.field_name}, font, scale, offsetX, offsetY, xFieldName - gap/2, xFieldName + gap/2, xFieldName, newY, xCurrent + offsetX, newYPere, true);
                    break;
                }
                case (EXPR_CAST):{
                    int newYPere = displayNode("CAST", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 2;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    int xTargetType = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int xExpr = calculateXChildren(xMin, xMax, nbChildren, 1);
                    displayAST((AST){.type = AST_TYPE, .data.type = ast.data.expression->cast.target_type}, font, scale, offsetX, offsetY, xTargetType - gap/2, xTargetType + gap/2, xTargetType, newY, xCurrent + offsetX, newYPere, true);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.expression->cast.expr}, font, scale, offsetX, offsetY, xExpr - gap/2, xExpr + gap/2, xExpr, newY, xCurrent + offsetX, newYPere, true);
                    break;
                }
                case (EXPR_LITERAL):{
                    switch (ast.data.expression->literal.kind){
                        case (LIT_INT):
                            displayNode(itoa(ast.data.expression->literal.int_val), xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                            break;
                        case (LIT_BOOLEAN):
                            if (ast.data.expression->literal.bool_val){
                                displayNode("TRUE", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                            }
                            else{
                                displayNode("FALSE", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                            }
                            break;
                        case (LIT_STRING):{
                            char *str = malloc(strlen(ast.data.expression->literal.string_val) + 3);
                            sprintf(str, "\"%s\"", ast.data.expression->literal.string_val);
                            displayNode(str, xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                            free(str);
                            break;
                        }
                        case (LIT_NULL):
                            displayNode("NULL", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                            break;
                    }
                }
            }
            break;
        }
        case (AST_STATEMENT):{
            switch (ast.data.statement->kind){
                case (STMT_EXPR):{
                    int newYPere = displayNode("EXPR", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 1;
                    int x0 = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int newY = yCurrent + GAP_VER*scale;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->expr}, font, scale, offsetX, offsetY, x0 - gap/2, x0 + gap/2, x0, newY, xCurrent + offsetX, newYPere, true);
                    break;
                }
                case (STMT_RETURN):{
                    int newYPere = displayNode("RETURN", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    if (ast.data.statement->return_val == NULL){
                        break;
                    }
                    int nbChildren = 1;
                    int x0 = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int newY = yCurrent + GAP_VER*scale;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->return_val}, font, scale, offsetX, offsetY, x0 - gap/2, x0 + gap/2, x0, newY, xCurrent + offsetX, newYPere, true);
                    break;
                }
                case (STMT_BLOCK):{
                    int newYPere = displayNode("BLOCK", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = ast.data.statement->block.count;
                    if (nbChildren == 0){
                        break;
                    }
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    for (int i = 0; i < ast.data.statement->block.count; i++){
                        int xStmt = calculateXChildren(xMin, xMax, nbChildren, i);
                        displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->block.stmts[i]}, font, scale, offsetX, offsetY, xStmt - gap/2, xStmt + gap/2, xStmt, newY, xCurrent + offsetX, newYPere, true);
                    }
                    break;
                }
                case (STMT_VAR_DECL):{
                    int newYPere = displayNode("VAR_DECL", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 2;
                    if (ast.data.statement->var_decl.initializer != NULL){
                        nbChildren = 3;
                    }
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    int xType = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int xName = calculateXChildren(xMin, xMax, nbChildren, 1);
                    displayAST((AST){.type = AST_TYPE, .data.type = ast.data.statement->var_decl.type}, font, scale, offsetX, offsetY, xType - gap/2, xType + gap/2, xType, newY, xCurrent + offsetX, newYPere, true);
                    displayAST((AST){.type = AST_NAME, .data.name = ast.data.statement->var_decl.name}, font, scale, offsetX, offsetY, xName - gap/2, xName + gap/2, xName, newY, xCurrent + offsetX, newYPere, true);
                    if (ast.data.statement->var_decl.initializer != NULL){
                        int xInit = calculateXChildren(xMin, xMax, nbChildren, 2);
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->var_decl.initializer}, font, scale, offsetX, offsetY, xInit - gap/2, xInit + gap/2, xInit, newY, xCurrent + offsetX, newYPere, true);
                    }
                    break;
                }
                case (STMT_FOR):{
                    int newYPere = displayNode("FOR_STMT", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 4;
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    int xInit = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int xCond = calculateXChildren(xMin, xMax, nbChildren, 1);
                    int xUpdate = calculateXChildren(xMin, xMax, nbChildren, 2);
                    if (ast.data.statement->for_stmt.init != NULL){
                        displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->for_stmt.init}, font, scale, offsetX, offsetY, xInit - gap/2, xInit + gap/2, xInit, newY, xCurrent + offsetX, newYPere, true);
                        
                    }
                    else{
                        displayNode("NIL", xInit + offsetX, newY + offsetY, color, font, xCurrent + offsetX, newYPere, true);
                    }
                    if (ast.data.statement->for_stmt.condition != NULL){
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->for_stmt.condition}, font, scale, offsetX, offsetY, xCond - gap/2, xCond + gap/2, xCond, newY, xCurrent + offsetX, newYPere, true);
                    }
                    else{
                        displayNode("NIL", xCond + offsetX, newY + offsetY, color, font, xCurrent + offsetX, newYPere, true);
                    }
                    if (ast.data.statement->for_stmt.increment != NULL){
                        displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->for_stmt.increment}, font, scale, offsetX, offsetY, xUpdate - gap/2, xUpdate + gap/2, xUpdate, newY, xCurrent + offsetX, newYPere, true);
                    }
                    else{
                        displayNode("NIL", xUpdate + offsetX, newY + offsetY, color, font, xCurrent + offsetX, newYPere, true);
                    }
                    int xBody = calculateXChildren(xMin, xMax, nbChildren, 3);
                    displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->for_stmt.body}, font, scale, offsetX, offsetY, xBody - gap/2, xBody + gap/2, xBody, newY, xCurrent + offsetX, newYPere, true);
                    break;
                }
                case (STMT_IF):{
                    int newYPere = displayNode("IF_STMT", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    int nbChildren = 2;
                    if (ast.data.statement->if_stmt.else_branch != NULL){
                        nbChildren = 3;
                    }
                    int gap = gapBetweenChildren(xMin, xMax, nbChildren);
                    int newY = yCurrent + GAP_VER*scale;
                    int xCond = calculateXChildren(xMin, xMax, nbChildren, 0);
                    int xThen = calculateXChildren(xMin, xMax, nbChildren, 1);
                    displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.statement->if_stmt.condition}, font, scale, offsetX, offsetY, xCond - gap/2, xCond + gap/2, xCond, newY, xCurrent + offsetX, newYPere, true);
                    displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->if_stmt.then_branch}, font, scale, offsetX, offsetY, xThen - gap/2, xThen + gap/2, xThen, newY, xCurrent + offsetX, newYPere, true);
                    if (ast.data.statement->if_stmt.else_branch != NULL){
                        int xElse = calculateXChildren(xMin, xMax, nbChildren, 2);
                        displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.statement->if_stmt.else_branch}, font, scale, offsetX, offsetY, xElse - gap/2, xElse + gap/2, xElse, newY, xCurrent + offsetX, newYPere, true);
                    }
                    break;
                }
                case (STMT_BREAK):{
                    displayNode("BREAK", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    break;
                }
                case (STMT_CONTINUE):{
                    displayNode("CONTINUE", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
                    break;
                }
            }
            break;
        }
        case (AST_NAME):{
            displayNode(ast.data.name, xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
            break;
        }
        case (AST_TYPE):{
            displayNode(stringType(ast.data.type), xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
            break;
        }
        case (AST_PARAMETER):{
            int newYPere = displayNode("PARAM", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
            int nbChildren = 2;
            int gap = gapBetweenChildren(xMin, xMax, nbChildren);
            int xType = calculateXChildren(xMin, xMax, nbChildren, 0);
            int xName = calculateXChildren(xMin, xMax, nbChildren, 1);
            displayAST((AST){.type = AST_TYPE, .data.type = ast.data.parameter->type}, font, scale, offsetX, offsetY, xType - gap/2, xType + gap/2, xType, yCurrent + GAP_VER*scale, xCurrent + offsetX, newYPere, true);
            displayAST((AST){.type = AST_NAME, .data.name = ast.data.parameter->name}, font, scale, offsetX, offsetY, xName - gap/2, xName + gap/2, xName, yCurrent + GAP_VER*scale, xCurrent + offsetX, newYPere, true);
            break;
        }
        case (AST_CONSTRUCTOR):{
            int newYPere = displayNode("CONSTRUCTOR", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
            int nbChildren = ast.data.constructor->param_count + 1;
            int gap = gapBetweenChildren(xMin, xMax, nbChildren);
            int newY = yCurrent + GAP_VER*scale;
            if (isPublic(ast.data.constructor->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, 1, 0);
                displayAST((AST){.type = AST_NAME, .data.name = "public"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, newY, xCurrent + offsetX, newYPere, true);
                newY += GAP_VER*scale;
                newYPere += GAP_VER*scale;
            }
            else if (isPrivate(ast.data.constructor->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, 0);
                displayAST((AST){.type = AST_NAME, .data.name = "private"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, newY, xCurrent + offsetX, newYPere, true);
                newY += GAP_VER*scale;
                newYPere += GAP_VER*scale;
            }
            for (int i = 0; i < ast.data.constructor->param_count; i++){
                int xParam = calculateXChildren(xMin, xMax, nbChildren, i+1);
                displayAST((AST){.type = AST_PARAMETER, .data.parameter = ast.data.constructor->params[i]}, font, scale, offsetX, offsetY, xParam - gap/2, xParam + gap/2, xParam, newY, xCurrent + offsetX, newYPere, true);
            }
            int xBlock = calculateXChildren(xMin, xMax, nbChildren, 0);
            displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.constructor->body}, font, scale, offsetX, offsetY, xBlock - gap/2, xBlock + gap/2, xBlock, newY, xCurrent + offsetX, newYPere, true);
            break;
        }
        case (AST_METHOD):{
            int newYPere = displayNode("METHOD", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
            int nbChildren = ast.data.method->param_count + 3;
            if (isPublic(ast.data.method->modifiers) || isPrivate(ast.data.method->modifiers)){
                nbChildren += 1;
            }
            if (isStatic(ast.data.method->modifiers)){
                nbChildren += 1;
            }
            int gap = gapBetweenChildren(xMin, xMax, nbChildren);
            int newY = yCurrent + GAP_VER*scale;
            int indexChild = 0;
            if (isPublic(ast.data.method->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_NAME, .data.name = "public"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, newY, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            if (isPrivate(ast.data.method->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_NAME, .data.name = "private"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, newY, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            if (isStatic(ast.data.method->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_NAME, .data.name = "static"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, newY, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            int xReturnType = calculateXChildren(xMin, xMax, nbChildren, indexChild);
            indexChild++;
            displayAST((AST){.type = AST_TYPE, .data.type = ast.data.method->return_type}, font, scale, offsetX, offsetY, xReturnType - gap/2, xReturnType + gap/2, xReturnType, newY, xCurrent + offsetX, newYPere, true);
            int xName = calculateXChildren(xMin, xMax, nbChildren, indexChild);
            indexChild++;
            displayAST((AST){.type = AST_NAME, .data.name = ast.data.method->name}, font, scale, offsetX, offsetY, xName - gap/2, xName + gap/2, xName, newY, xCurrent + offsetX, newYPere, true);
            for (int i = 0; i < ast.data.method->param_count; i++){
                int xParam = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                indexChild++;
                displayAST((AST){.type = AST_PARAMETER, .data.parameter = ast.data.method->params[i]}, font, scale, offsetX, offsetY, xParam - gap/2, xParam + gap/2, xParam, newY, xCurrent + offsetX, newYPere, true);
            }
            int xBlock = calculateXChildren(xMin, xMax, nbChildren, nbChildren - 1);
            displayAST((AST){.type = AST_STATEMENT, .data.statement = ast.data.method->body}, font, scale, offsetX, offsetY, xBlock - gap/2, xBlock + gap/2, xBlock, newY, xCurrent + offsetX, newYPere, true);
            break;
        }
        case (AST_FIELD):{
            int newYPere = displayNode("FIELD", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
            int nbChildren = 2;
            if (isPublic(ast.data.field->modifiers) || isPrivate(ast.data.field->modifiers)){
                nbChildren += 1;
            }
            if (isStatic(ast.data.field->modifiers)){
                nbChildren += 1;
            }
            if (ast.data.field->initializer != NULL){
                nbChildren += 1;
            }
            int gap = gapBetweenChildren(xMin, xMax, nbChildren);
            int indexChild = 0;
            if (isPublic(ast.data.field->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_NAME, .data.name = "public"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, yCurrent + GAP_VER*scale, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            if (isPrivate(ast.data.field->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_NAME, .data.name = "private"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, yCurrent + GAP_VER*scale, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            if (isStatic(ast.data.field->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_NAME, .data.name = "static"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, yCurrent + GAP_VER*scale, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            int xType = calculateXChildren(xMin, xMax, nbChildren, indexChild);
            indexChild++;
            displayAST((AST){.type = AST_TYPE, .data.type = ast.data.field->type}, font, scale, offsetX, offsetY, xType - gap/2, xType + gap/2, xType, yCurrent + GAP_VER*scale, xCurrent + offsetX, newYPere, true);
            int xName = calculateXChildren(xMin, xMax, nbChildren, indexChild);
            indexChild++;
            if (ast.data.field->initializer != NULL){
                int xInitializer = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_EXPRESSION, .data.expression = ast.data.field->initializer}, font, scale, offsetX, offsetY, xInitializer - gap/2, xInitializer + gap/2, xInitializer, yCurrent + GAP_VER*scale, xCurrent + offsetX, newYPere, true);
            }
            displayAST((AST){.type = AST_NAME, .data.name = ast.data.field->name}, font, scale, offsetX, offsetY, xName - gap/2, xName + gap/2, xName, yCurrent + GAP_VER*scale, xCurrent + offsetX, newYPere, true);
            break;
        }
        case (AST_CLASS_DECL):{
            int newYPere = displayNode("CLASS", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
            int nbChildren = 1 + ast.data.class_decl->field_count + ast.data.class_decl->method_count + ast.data.class_decl->constructor_count;
            if (isPublic(ast.data.class_decl->modifiers) || isPrivate(ast.data.class_decl->modifiers)){
                nbChildren += 1;
            }
            if (ast.data.class_decl->super_class != NULL){
                nbChildren += 1;
            }
            int gap = gapBetweenChildren(xMin, xMax, nbChildren);
            int newY = yCurrent + GAP_VER*scale;
            int indexChild = 0;
            if (isPublic(ast.data.class_decl->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_NAME, .data.name = "public"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, newY, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            if (isPrivate(ast.data.class_decl->modifiers)){
                int xModifier = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_NAME, .data.name = "private"}, font, scale, offsetX, offsetY, xModifier - gap/2, xModifier + gap/2, xModifier, newY, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            int xName = calculateXChildren(xMin, xMax, nbChildren, indexChild);
            displayAST((AST){.type = AST_NAME, .data.name = ast.data.class_decl->name}, font, scale, offsetX, offsetY, xName - gap/2, xName + gap/2, xName, newY, xCurrent + offsetX, newYPere, true);
            indexChild++;
            if (ast.data.class_decl->super_class != NULL){
                int xSuperClass = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                indexChild++;
                displayAST((AST){.type = AST_NAME, .data.name = ast.data.class_decl->super_class}, font, scale, offsetX, offsetY, xSuperClass - gap/2, xSuperClass + gap/2, xSuperClass, newY, xCurrent + offsetX, newYPere, true);
            }
            for (int i = 0; i < ast.data.class_decl->field_count; i++){
                int xChild = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_FIELD, .data.field = ast.data.class_decl->fields[i]}, font, scale, offsetX, offsetY, xChild - gap/2, xChild + gap/2, xChild, newY, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            for (int i = 0; i < ast.data.class_decl->constructor_count; i++){
                int xChild = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_CONSTRUCTOR, .data.constructor = ast.data.class_decl->constructors[i]}, font, scale, offsetX, offsetY, xChild - gap/2, xChild + gap/2, xChild, newY, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            for (int i = 0; i < ast.data.class_decl->method_count; i++){
                int xChild = calculateXChildren(xMin, xMax, nbChildren, indexChild);
                displayAST((AST){.type = AST_METHOD, .data.method = ast.data.class_decl->methods[i]}, font, scale, offsetX, offsetY, xChild - gap/2, xChild + gap/2, xChild, newY, xCurrent + offsetX, newYPere, true);
                indexChild++;
            }
            break;
        }
        case (AST_FILE):{
            int newYPere = displayNode("FILE", xCurrent + offsetX, yCurrent + offsetY, color, font, xPere, yPere, drawLine);
            int nbChildren = ast.data.file->class_count;
            int gap = gapBetweenChildren(xMin, xMax, nbChildren);
            int newY = yCurrent + GAP_VER*scale;
            for (int i = 0; i < ast.data.file->class_count; i++){
                int xChild = calculateXChildren(xMin, xMax, nbChildren, i);
                displayAST((AST){.type = AST_CLASS_DECL, .data.class_decl = ast.data.file->classes[i]}, font, scale, offsetX, offsetY, xChild - gap/2, xChild + gap/2, xChild, newY, xCurrent + offsetX, newYPere, true);
            }

            break;
        }
        default:{
            break;
        }
    }
}

void mainloop(AST ast){
    createWindow("AST", 50,50,WIDTH,HEIGHT);
    createRenderer();
    initTTF();
    TTF_Font *font = createFont(FONT_PATH, DEFAULT_FONT_SIZE);
    SDL_Event event;
    bool running = true;
    int offsetX = DEFAULT_OFFSET_X;
    int offsetY = DEFAULT_OFFSET_Y;
    double scale = 1;
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
                    SDL_GetMouseState(&xMouse, &yMouse);
                    if (event.wheel.y > 0 && scale < ZOOM_IN_MAX) { // Scroll up
                        scale *= 1.1;
                        offsetX += (offsetX - xMouse) * 0.1;
                        offsetY += (offsetY - yMouse) * 0.1;
                    } else if (event.wheel.y < 0 && scale > ZOOM_OUT_MAX) { // Scroll down
                        scale /= 1.1;
                        offsetX -= (offsetX - xMouse) * 0.1;
                        offsetY -= (offsetY - yMouse) * 0.1;
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
                        offsetX += (newX - xMouse);
                        offsetY += (newY - yMouse);
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
                            offsetX = DEFAULT_OFFSET_X;
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
        int xMin = -scale * WIDTH/2;
        int xMax = scale * WIDTH/2;
        displayAST(ast, font, scale, offsetX, offsetY, xMin, xMax, 0, 0, -1, -1, false);
        SDL_RenderPresent(ren);
    }
    TTF_CloseFont(font);
    exit(0);
}
