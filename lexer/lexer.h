typedef enum {
	TOKEN_NUMBER,

	TOKEN_BINOP_ADD,
	TOKEN_BINOP_SUB,
	TOKEN_BINOP_MULT,
	TOKEN_BINOP_DIV,
	
	TOKEN_LPAR,
	TOKEN_RPAR,
    TOKEN_CLASS,
    TOKEN_KEYWORD,
    TOKEN_ID,
	TOKEN_EOF,

    TOKEN_BOOLEAN,
    TOKEN_ELSE,
    TOKEN_EXTENDS,
    TOKEN_FALSE,
    TOKEN_FOR,
    TOKEN_IF,
    TOKEN_INSTANCEOF,
    TOKEN_INT,
    TOKEN_NEW,
    TOKEN_NULL,
    TOKEN_PUBLIC,
    TOKEN_RETURN,
    TOKEN_STATIC,
    TOKEN_THIS,
    TOKEN_TRUE,
    TOKEN_VOID,

} TokenType;


// TODO: what do we do with identifiers? (put them directely in the symbol table and pass them by index?)
/**
 * A token is a position of the start of the token and a beginning:
 * The content is recomputed on demand and casted to the right type
 * string for identifiers, integers for ints and string for strings
 * 
 * - position points to the begining of the identifier 
 * - the type is the type
 */
struct Token{
	TokenType type;			// Tagged union ;)
	int position;
} ;

typedef struct Token Token;

/** 
 * A Lexer is a position from which we have read all the preceding 
 * chars.
 */
typedef struct {
	char* source;
	int length;

	int pos;
	Token tok;
} Lexer;


const char *keywords[] = {"boolean", "else", "extends", "false", "for", "if",
"instanceof", "int", "new", "null", "public", "return", "static",
"this", "true", "void" };



Lexer* lexer_init(char* src, int len);
static char peek(Lexer* lex);
static char advance(Lexer* lex);
static void skip_whitespace(Lexer* lex);
void lexer_next(Lexer* lex);
void token_text(Lexer* lex, Token tok, char* buf, int bufsize);
int token_number(Lexer* lex, Token tok);
const char* token_type_name(TokenType type);
void token_pprint(Token tok, Lexer* lex);