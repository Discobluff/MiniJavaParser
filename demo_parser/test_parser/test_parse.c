#include "../parse_refonte.h"
#include "../backend_parser.h"
#include "../../lexer/lexer.h"
#include "../../ast/ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

// Test counter
static int tests_passed = 0;
static int tests_failed = 0;
static jmp_buf test_jmp_buf;

#define TEST(name) \
    printf("\n[TEST] %s\n", name); \
    if (setjmp(test_jmp_buf) == 0)

#define ASSERT(condition, message) \
    do { \
        if (!(condition)) { \
            printf("  ❌ FAILED: %s\n", message); \
            tests_failed++; \
            longjmp(test_jmp_buf, 1); \
        } else { \
            printf("  ✓ %s\n", message); \
        } \
    } while(0)

#define TEST_END() \
    do { \
        tests_passed++; \
        printf("  ✅ Test passed\n"); \
    } while(0)

// Helper to verify AST node type
void verify_ast_type(AST* node, ASTtype expected_type, const char* description) {
    char msg[256];
    snprintf(msg, sizeof(msg), "%s - AST node exists", description);
    ASSERT(node != NULL, msg);
    
    snprintf(msg, sizeof(msg), "%s - AST type is correct", description);
    ASSERT(node->type == expected_type, msg);
}

// Helper to verify binary operation
void verify_binary_op(AST* node, char expected_op, const char* description) {
    verify_ast_type(node, BINARY_OP, description);
    
    char msg[256];
    snprintf(msg, sizeof(msg), "%s - operator is '%c'", description, expected_op);
    ASSERT(node->data.binary.op == expected_op, msg);
    
    snprintf(msg, sizeof(msg), "%s - has left child", description);
    ASSERT(node->data.binary.left != NULL, msg);
    
    snprintf(msg, sizeof(msg), "%s - has right child", description);
    ASSERT(node->data.binary.right != NULL, msg);
}

// Helper to verify unary operation
void verify_unary_op(AST* node, char expected_op, const char* description) {
    verify_ast_type(node, UNARY_OP, description);
    
    char msg[256];
    snprintf(msg, sizeof(msg), "%s - operator is '%c'", description, expected_op);
    ASSERT(node->data.unary.op == expected_op, msg);
    
    snprintf(msg, sizeof(msg), "%s - has operand", description);
    ASSERT(node->data.unary.operand != NULL, msg);
}

// Helper to verify number node
void verify_number(AST* node, size_t expected_value, const char* description) {
    verify_ast_type(node, NUMBER, description);
    
    char msg[256];
    snprintf(msg, sizeof(msg), "%s - value is %ld", description, expected_value);
    ASSERT(node->data.number == expected_value, msg);
}

// =============================================================================
// Primary Expression Tests
// =============================================================================

void test_parse_number() {
    TEST("parse_refonte_main - simple number") {
        Lexer* lex = lexer_init("42", 2);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_number(ast, 42, "Number 42");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_true() {
    TEST("parse_refonte_main - boolean true") {
        Lexer* lex = lexer_init("true", 4);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_number(ast, 1, "True (converted to 1)");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_false() {
    TEST("parse_refonte_main - boolean false") {
        Lexer* lex = lexer_init("false", 5);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_number(ast, 0, "False (converted to 0)");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

// =============================================================================
// Binary Operation Tests
// =============================================================================

void test_parse_addition() {
    TEST("parse_refonte_main - simple addition (2 + 3)") {
        Lexer* lex = lexer_init("2 + 3", 5);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '+', "Addition");
        verify_number(ast->data.binary.left, 2, "Left operand");
        verify_number(ast->data.binary.right, 3, "Right operand");

		free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_subtraction() {
    TEST("parse_refonte_main - simple subtraction (10 - 7)") {
        Lexer* lex = lexer_init("10 - 7", 6);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '-', "Subtraction");
        verify_number(ast->data.binary.left, 10, "Left operand");
        verify_number(ast->data.binary.right, 7, "Right operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_multiplication() {
    TEST("parse_refonte_main - simple multiplication (4 * 5)") {
        Lexer* lex = lexer_init("4 * 5", 5);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '*', "Multiplication");
        verify_number(ast->data.binary.left, 4, "Left operand");
        verify_number(ast->data.binary.right, 5, "Right operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_division() {
    TEST("parse_refonte_main - simple division (20 / 4)") {
        Lexer* lex = lexer_init("20 / 4", 6);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '/', "Division");
        verify_number(ast->data.binary.left, 20, "Left operand");
        verify_number(ast->data.binary.right, 4, "Right operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_modulo() {
    TEST("parse_refonte_main - simple modulo (17 % 5)") {
        Lexer* lex = lexer_init("17 % 5", 6);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '%', "Modulo");
        verify_number(ast->data.binary.left, 17, "Left operand");
        verify_number(ast->data.binary.right, 5, "Right operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

// =============================================================================
// Unary Operation Tests
// =============================================================================

void test_parse_unary_minus() {
    TEST("parse_refonte_main - unary minus (-5)") {
        Lexer* lex = lexer_init("-5", 2);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_unary_op(ast, '-', "Unary minus");
        verify_number(ast->data.unary.operand, 5, "Operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_unary_plus() {
    TEST("parse_refonte_main - unary plus (+8)") {
        Lexer* lex = lexer_init("+8", 2);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_unary_op(ast, '+', "Unary plus");
        verify_number(ast->data.unary.operand, 8, "Operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_negation() {
    TEST("parse_refonte_main - logical negation (!true)") {
        Lexer* lex = lexer_init("!true", 5);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_unary_op(ast, '!', "Logical negation");
        verify_number(ast->data.unary.operand, 1, "Operand (true)");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_double_negation() {
    TEST("parse_refonte_main - double negation (--5)") {
        Lexer* lex = lexer_init("--5", 3);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_unary_op(ast, '-', "First negation");
        verify_unary_op(ast->data.unary.operand, '-', "Second negation");
        verify_number(ast->data.unary.operand->data.unary.operand, 5, "Operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

// =============================================================================
// Operator Precedence Tests
// =============================================================================

void test_parse_precedence_mult_over_add() {
    TEST("parse_refonte_main - precedence: 2 + 3 * 4") {
        Lexer* lex = lexer_init("2 + 3 * 4", 9);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '+', "Top level addition");
        verify_number(ast->data.binary.left, 2, "Left is 2");
        verify_binary_op(ast->data.binary.right, '*', "Right is multiplication");
        verify_number(ast->data.binary.right->data.binary.left, 3, "Mult left is 3");
        verify_number(ast->data.binary.right->data.binary.right, 4, "Mult right is 4");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_precedence_mult_over_sub() {
    TEST("parse_refonte_main - precedence: 10 - 2 * 3") {
        Lexer* lex = lexer_init("10 - 2 * 3", 9);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '-', "Top level subtraction");
        verify_number(ast->data.binary.left, 10, "Left is 10");
        verify_binary_op(ast->data.binary.right, '*', "Right is multiplication");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_precedence_div_over_add() {
    TEST("parse_refonte_main - precedence: 5 + 20 / 4") {
        Lexer* lex = lexer_init("5 + 20 / 4", 10);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '+', "Top level addition");
        verify_number(ast->data.binary.left, 5, "Left is 5");
        verify_binary_op(ast->data.binary.right, '/', "Right is division");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

// =============================================================================
// Left Associativity Tests
// =============================================================================

void test_parse_left_assoc_addition() {
    TEST("parse_refonte_main - left associativity: 1 + 2 + 3") {
        Lexer* lex = lexer_init("1 + 2 + 3", 9);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '+', "Rightmost addition");
        verify_binary_op(ast->data.binary.left, '+', "Leftmost addition");
        verify_number(ast->data.binary.left->data.binary.left, 1, "First operand");
        verify_number(ast->data.binary.left->data.binary.right, 2, "Second operand");
        verify_number(ast->data.binary.right, 3, "Third operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_left_assoc_subtraction() {
    TEST("parse_refonte_main - left associativity: 10 - 3 - 2") {
        Lexer* lex = lexer_init("10 - 3 - 2", 10);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '-', "Rightmost subtraction");
        verify_binary_op(ast->data.binary.left, '-', "Leftmost subtraction");
        verify_number(ast->data.binary.left->data.binary.left, 10, "First operand");
        verify_number(ast->data.binary.left->data.binary.right, 3, "Second operand");
        verify_number(ast->data.binary.right, 2, "Third operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_left_assoc_multiplication() {
    TEST("parse_refonte_main - left associativity: 2 * 3 * 4") {
        Lexer* lex = lexer_init("2 * 3 * 4", 9);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '*', "Rightmost multiplication");
        verify_binary_op(ast->data.binary.left, '*', "Leftmost multiplication");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

// =============================================================================
// Complex Expression Tests
// =============================================================================

void test_parse_complex_expression_1() {
    TEST("parse_refonte_main - complex: 2 + 3 * 4 - 5") {
        Lexer* lex = lexer_init("2 + 3 * 4 - 5", 9);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '-', "Top level operation");
        ASSERT(ast->data.binary.left != NULL, "Has left subtree");
        ASSERT(ast->data.binary.right != NULL, "Has right subtree");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_complex_expression_2() {
    TEST("parse_refonte_main - complex: -2 * 3 + 4") {
        Lexer* lex = lexer_init("-2 * 3 + 4", 10);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_binary_op(ast, '+', "Top level addition");
        verify_binary_op(ast->data.binary.left, '*', "Multiplication");
        verify_unary_op(ast->data.binary.left->data.binary.left, '-', "Unary minus on 2");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_complex_expression_3() {
    TEST("parse_refonte_main - complex: 10 / 2 + 3 * 4 - 1") {
        Lexer* lex = lexer_init("10 / 2 + 3 * 4 - 1", 18);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        ASSERT(ast != NULL, "AST created");
        ASSERT(ast->type == BINARY_OP, "Root is binary operation");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

void test_parse_complex_unary_chain() {
    TEST("parse_refonte_main - complex: -+-5") {
        Lexer* lex = lexer_init("-+-5", 4);
		lexer_next(lex);
        AST* ast = parse_refonte_main(lex);
        
        verify_unary_op(ast, '-', "First unary minus");
        verify_unary_op(ast->data.unary.operand, '+', "Unary plus");
        verify_unary_op(ast->data.unary.operand->data.unary.operand, '-', "Second unary minus");
        verify_number(ast->data.unary.operand->data.unary.operand->data.unary.operand, 5, "Final operand");
        
        free_ast(ast);
        free(lex);
        TEST_END();
    }
}

// =============================================================================
// Main Test Runner
// =============================================================================

int main() {
    printf("=================================================\n");
    printf("Parse Refonte Test Suite\n");
    printf("=================================================\n");
    
    // // Primary expression tests
    // printf("\n--- Primary Expression Tests ---\n");
    // test_parse_number();
    // test_parse_true();
    // test_parse_false();
    
    // Binary operation tests
    printf("\n--- Binary Operation Tests ---\n");
    test_parse_addition();
    test_parse_subtraction();
    test_parse_multiplication();
    test_parse_division();
    test_parse_modulo();
    
    // Unary operation tests
    // printf("\n--- Unary Operation Tests ---\n");
    // test_parse_unary_minus();
    // test_parse_unary_plus();
    // test_parse_negation();
    // test_parse_double_negation();
    
    // // Operator precedence tests
    // printf("\n--- Operator Precedence Tests ---\n");
    // test_parse_precedence_mult_over_add();
    // test_parse_precedence_mult_over_sub();
    // test_parse_precedence_div_over_add();
    
    // // Left associativity tests
    // printf("\n--- Left Associativity Tests ---\n");
    // test_parse_left_assoc_addition();
    // test_parse_left_assoc_subtraction();
    // test_parse_left_assoc_multiplication();
    
    // // Complex expression tests
    // printf("\n--- Complex Expression Tests ---\n");
    // test_parse_complex_expression_1();
    // test_parse_complex_expression_2();
    // test_parse_complex_expression_3();
    // test_parse_complex_unary_chain();
    
    // Summary
    printf("\n=================================================\n");
    printf("Test Results:\n");
    printf("  ✅ Passed: %d\n", tests_passed);
    printf("  ❌ Failed: %d\n", tests_failed);
    printf("=================================================\n");
    
    return tests_failed == 0 ? 0 : 1;
}