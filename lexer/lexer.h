typedef enum {
	TOKEN_NUMBER,

	TOKEN_BINOP_ADD,
	TOKEN_BINOP_SUB,
	TOKEN_BINOP_MULT,
	TOKEN_BINOP_DIV,
	TOKEN_BINOP_MOD,
	TOKEN_BINOP_OR,
	TOKEN_BINOP_AND,
	TOKEN_BINOP_EQ,
	TOKEN_BINOP_2EQUAL,
	TOKEN_BINOP_DIFF,

	TOKEN_BINOP_INF,
	TOKEN_BINOP_SUP,
	TOKEN_BINOP_INFEG,
	TOKEN_BINOP_SUPEG,
	
	TOKEN_LPAR,
	TOKEN_RPAR,

	TOKEN_LACC,
	TOKEN_RACC,

	TOKEN_PTSVG,
	TOKEN_VG,

	TOKEN_OPP,
	TOKEN_PTS,



	TOKEN_EOF,
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
typedef struct {
	TokenType type;			// Tagged union ;)
	int position;
} Token;


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