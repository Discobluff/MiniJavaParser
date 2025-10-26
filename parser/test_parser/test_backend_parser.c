#include "../backend_parser.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

// Test counter
static int tests_passed = 0;
static int tests_failed = 0;

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

static jmp_buf test_jmp_buf;

// Mock AST node creator for testing
AST* create_test_ast_node(int type) {
    AST* node = malloc(sizeof(AST));
    node->type = type;
    return node;
}

// Mock token creator
Token create_test_token(TokenType type) {
    Token tok;
    tok.type = type;
	tok.position = 0;
	return tok;
}

// =============================================================================
// Stack (NUT) Tests
// =============================================================================

void test_stack_init() {
    TEST("stack_init - initialization") {
        Stack* stk = stack_init();
        
        ASSERT(stk != NULL, "Stack should be allocated");
        ASSERT(stk->top == 0, "Stack top should be 0");
        
        free(stk);
        TEST_END();
    }
}

void test_stack_push_single() {
    TEST("stack_push - push single element") {
        Stack* stk = stack_init();
        
        NUT nut;
        nut.type = RULE;
        nut.rule = 42;
        
        stack_push(stk, nut);
        
        ASSERT(stk->top == 1, "Stack top should be 1");
        ASSERT(stk->items[0].type == RULE, "Item type should be RULE");
        ASSERT(stk->items[0].rule == 42, "Rule value should be 42");
        
        free(stk);
        TEST_END();
    }
}

void test_stack_push_multiple() {
    TEST("stack_push - push multiple elements") {
        Stack* stk = stack_init();
        
        for (int i = 0; i < 10; i++) {
            NUT nut;
            nut.type = RULE;
            nut.rule = i;
            stack_push(stk, nut);
        }
        
        ASSERT(stk->top == 10, "Stack top should be 10");
        ASSERT(stk->items[0].rule == 0, "First item should be 0");
        ASSERT(stk->items[9].rule == 9, "Last item should be 9");
        
        free(stk);
        TEST_END();
    }
}

void test_stack_push_token() {
    TEST("stack_push - push token type") {
        Stack* stk = stack_init();
        
        NUT nut;
        nut.type = TOKEN;
        nut.token = create_test_token(TOKEN_BINOP_ADD);
        
        stack_push(stk, nut);
        
        ASSERT(stk->top == 1, "Stack top should be 1");
        ASSERT(stk->items[0].type == TOKEN, "Item type should be TOKEN");
        ASSERT(stk->items[0].token.type == TOKEN_BINOP_ADD, "Token type should be TOKEN_BINOP_ADD");
        
        free(stk);
        TEST_END();
    }
}

void test_stack_pop_single() {
    TEST("stack_pop - pop single element") {
        Stack* stk = stack_init();
        
        NUT nut;
        nut.type = RULE;
        nut.rule = 99;
        stack_push(stk, nut);
        
        NUT popped = stack_pop(stk);
        
        ASSERT(stk->top == 0, "Stack should be empty");
        ASSERT(popped.type == RULE, "Popped type should be RULE");
        ASSERT(popped.rule == 99, "Popped value should be 99");
        
        free(stk);
        TEST_END();
    }
}

void test_stack_pop_multiple() {
    TEST("stack_pop - pop multiple elements (LIFO order)") {
        Stack* stk = stack_init();
        
        for (size_t i = 0; i < 5; i++) {
            NUT nut;
            nut.type = RULE;
            nut.rule = i;
            stack_push(stk, nut);
        }
        
        for (size_t i = 5; i != 0; i--) {
            NUT popped = stack_pop(stk);
            ASSERT(popped.rule == i-1, "Items should be popped in LIFO order");
        }
        
        ASSERT(stk->top == 0, "Stack should be empty");
        
        free(stk);
        TEST_END();
    }
}

void test_stack_push_pop_mixed() {
    TEST("stack_push/pop - mixed operations") {
        Stack* stk = stack_init();
        
        NUT nut1;
        nut1.type = RULE;
        nut1.rule = 1;
        stack_push(stk, nut1);
        
        NUT nut2;
        nut2.type = RULE;
        nut2.rule = 2;
        stack_push(stk, nut2);
        
        NUT popped = stack_pop(stk);
        ASSERT(popped.rule == 2, "Should pop 2");
        ASSERT(stk->top == 1, "Stack top should be 1");
        
        NUT nut3;
        nut3.type = RULE;
        nut3.rule = 3;
        stack_push(stk, nut3);
        
        ASSERT(stk->top == 2, "Stack top should be 2");
        
        popped = stack_pop(stk);
        ASSERT(popped.rule == 3, "Should pop 3");
        
        popped = stack_pop(stk);
        ASSERT(popped.rule == 1, "Should pop 1");
        ASSERT(stk->top == 0, "Stack should be empty");
        
        free(stk);
        TEST_END();
    }
}

// Note: Overflow and underflow tests would cause exit(1), so they're documented
// but not automatically run. They could be tested with fork() or similar.
void test_stack_overflow_documentation() {
    printf("\n[INFO] stack_push overflow test:\n");
    printf("  Pushing more than PARSER_STACK_SIZE items will trigger:\n");
    printf("  - parser_error(\"PARSER INTERNAL STACK OVERFLOW!\")\n");
    printf("  - exit(1)\n");
    printf("  (Not tested automatically to avoid process termination)\n");
}

void test_stack_underflow_documentation() {
    printf("\n[INFO] stack_pop underflow test:\n");
    printf("  Popping from empty stack will trigger:\n");
    printf("  - parser_error(\"PARSER INTERNAL STACK UNDERFLOW!\")\n");
    printf("  - exit(1)\n");
    printf("  (Not tested automatically to avoid process termination)\n");
}

// =============================================================================
// ASTStack Tests
// =============================================================================

void test_ast_stack_init() {
    TEST("ast_stack_init - initialization") {
        ASTStack* stk = ast_stack_init();
        
        ASSERT(stk != NULL, "ASTStack should be allocated");
        ASSERT(stk->top == 0, "ASTStack top should be 0");
        
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_push_single() {
    TEST("ast_stack_push - push single node") {
        ASTStack* stk = ast_stack_init();
        AST* node = create_test_ast_node(1);
        
        ast_stack_push(stk, node);
        
        ASSERT(stk->top == 1, "Stack top should be 1");
        ASSERT(stk->items[0] == node, "Item should be the pushed node");
        ASSERT(stk->items[0]->type == 1, "Node type should be 1");
        
        free(node);
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_push_null() {
    TEST("ast_stack_push - push NULL (no-op)") {
        ASTStack* stk = ast_stack_init();
        
        ast_stack_push(stk, NULL);
        
        ASSERT(stk->top == 0, "Stack should remain empty when pushing NULL");
        
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_push_multiple() {
    TEST("ast_stack_push - push multiple nodes") {
        ASTStack* stk = ast_stack_init();
        AST* nodes[5];
        
        for (int i = 0; i < 5; i++) {
            nodes[i] = create_test_ast_node(i);
            ast_stack_push(stk, nodes[i]);
        }
        
        ASSERT(stk->top == 5, "Stack top should be 5");
        for (int i = 0; i < 5; i++) {
            ASSERT(stk->items[i] == nodes[i], "Items should be in order");
        }
        
        for (int i = 0; i < 5; i++) {
            free(nodes[i]);
        }
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_pop_single() {
    TEST("ast_stack_pop - pop single node") {
        ASTStack* stk = ast_stack_init();
        AST* node = create_test_ast_node(42);
        
        ast_stack_push(stk, node);
        AST* popped = ast_stack_pop(stk);
        
        ASSERT(stk->top == 0, "Stack should be empty");
        ASSERT(popped == node, "Popped node should match pushed node");
        ASSERT(popped->type == 42, "Node type should be 42");
        
        free(node);
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_pop_empty() {
    TEST("ast_stack_pop - pop from empty stack") {
        ASTStack* stk = ast_stack_init();
        
        AST* popped = ast_stack_pop(stk);
        
        ASSERT(popped == NULL, "Popping from empty stack should return NULL");
        ASSERT(stk->top == 0, "Stack top should remain 0");
        
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_pop_multiple() {
    TEST("ast_stack_pop - pop multiple nodes (LIFO)") {
        ASTStack* stk = ast_stack_init();
        AST* nodes[5];
        
        for (int i = 0; i < 5; i++) {
            nodes[i] = create_test_ast_node(i);
            ast_stack_push(stk, nodes[i]);
        }
        
        for (int i = 4; i >= 0; i--) {
            AST* popped = ast_stack_pop(stk);
            ASSERT(popped == nodes[i], "Nodes should be popped in LIFO order");
        }
        
        ASSERT(stk->top == 0, "Stack should be empty");
        
        for (int i = 0; i < 5; i++) {
            free(nodes[i]);
        }
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_peek() {
    TEST("ast_stack_peek - peek at top element") {
        ASTStack* stk = ast_stack_init();
        AST* node1 = create_test_ast_node(1);
        AST* node2 = create_test_ast_node(2);
        
        ast_stack_push(stk, node1);
        ast_stack_push(stk, node2);
        
        AST* peeked = ast_stack_peek(stk);
        
        ASSERT(peeked == node2, "Peek should return top element");
        ASSERT(stk->top == 2, "Peek should not modify stack");
        
        AST* popped = ast_stack_pop(stk);
        ASSERT(popped == peeked, "Peek should show same element as pop");
        
        free(node1);
        free(node2);
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_peek_empty() {
    TEST("ast_stack_peek - peek on empty stack") {
        ASTStack* stk = ast_stack_init();
        
        AST* peeked = ast_stack_peek(stk);
        
        ASSERT(peeked == NULL, "Peeking empty stack should return NULL");
        
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_mixed_operations() {
    TEST("ast_stack - mixed push/pop/peek operations") {
        ASTStack* stk = ast_stack_init();
        AST* node1 = create_test_ast_node(1);
        AST* node2 = create_test_ast_node(2);
        AST* node3 = create_test_ast_node(3);
        
        ast_stack_push(stk, node1);
        AST* peeked = ast_stack_peek(stk);
        ASSERT(peeked == node1, "Peek should show node1");
        
        ast_stack_push(stk, node2);
        peeked = ast_stack_peek(stk);
        ASSERT(peeked == node2, "Peek should show node2");
        
        AST* popped = ast_stack_pop(stk);
        ASSERT(popped == node2, "Should pop node2");
        
        ast_stack_push(stk, node3);
        peeked = ast_stack_peek(stk);
        ASSERT(peeked == node3, "Peek should show node3");
        
        ASSERT(stk->top == 2, "Stack should have 2 elements");
        
        free(node1);
        free(node2);
        free(node3);
        free(stk);
        TEST_END();
    }
}

void test_ast_stack_overflow_documentation() {
    printf("\n[INFO] ast_stack_push overflow test:\n");
    printf("  Pushing more than PARSER_STACK_SIZE items will trigger:\n");
    printf("  - parser_error(\"PARSER INTERNAL AST STACK OVERFLOW!\")\n");
    printf("  - exit(1)\n");
    printf("  (Not tested automatically to avoid process termination)\n");
}

// =============================================================================
// Main Test Runner
// =============================================================================

int main() {
    printf("=================================================\n");
    printf("Parser Stack API Test Suite\n");
    printf("=================================================\n");
    
    // Stack (NUT) tests
    printf("\n--- Stack (NUT) Tests ---\n");
    test_stack_init();
    test_stack_push_single();
    test_stack_push_multiple();
    test_stack_push_token();
    test_stack_pop_single();
    test_stack_pop_multiple();
    test_stack_push_pop_mixed();
    test_stack_overflow_documentation();
    test_stack_underflow_documentation();
    
    // ASTStack tests
    printf("\n--- ASTStack Tests ---\n");
    test_ast_stack_init();
    test_ast_stack_push_single();
    test_ast_stack_push_null();
    test_ast_stack_push_multiple();
    test_ast_stack_pop_single();
    test_ast_stack_pop_empty();
    test_ast_stack_pop_multiple();
    test_ast_stack_peek();
    test_ast_stack_peek_empty();
    test_ast_stack_mixed_operations();
    test_ast_stack_overflow_documentation();
    
    // Summary
    printf("\n=================================================\n");
    printf("Test Results:\n");
    printf("  ✅ Passed: %d\n", tests_passed);
    printf("  ❌ Failed: %d\n", tests_failed);
    printf("=================================================\n");
    
    return tests_failed == 0 ? 0 : 1;
}