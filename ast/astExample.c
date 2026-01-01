#include <ast/ast.h>
#include <ast/displayAst.h>


// ======================================= Example Usage =======================================

/**
 *	class A {
 *		A() {}
 *		String ok() { return "ok\n"; }
 *	}
 *	class Main {
 *		public static void main(String args[]) {
 *			System.out.print(new A().ok());
 *		}
 *	}
 */
File* example1()
{
	ClassDecl* A = create_class("A");

	Constructor* constr_A = create_constructor(
		MOD_PUBLIC,
		"A",
		NULL,
		0,
		create_block(NULL, 0)
	);

    class_add_constructor(A, constr_A);

	Statement **ok_body_stmts = malloc(sizeof(Statement*));
	ok_body_stmts[0] = create_return_stmt(create_literal_string("ok\n"));
	Statement* ok_body = create_block(ok_body_stmts, 1);
    Method* ok = create_method(0, create_class_type("String"), "ok", NULL, 0,
        ok_body
    );

	class_add_method(A, ok);

	ClassDecl* main_class = create_class("Main");
	
	// String args
	Parameter* args_param = create_parameter(create_class_type("String"), "args");
	Parameter** main_params = malloc(sizeof(Parameter*));
	main_params[0] = args_param;
	
	// new A().ok()
	Expression* new_a = create_new(
		create_class_type("A"),
		NULL,  // no constructor arguments
		0
	);
	
	Expression* ok_method = create_dot(new_a, "ok\n");
	
	Expression* ok_call = create_call(
		ok_method,
		NULL,  // no arguments to ok()
		0
	);
	
	Expression* system = create_variable("System");
	Expression* system_out = create_dot(system, "out");
	
	Expression* print_method = create_dot(system_out, "print");
	
	Expression** print_args = malloc(sizeof(Expression*));
	print_args[0] = ok_call;
	Expression* print_call = create_call(
		print_method,
		print_args,
		1
	);
	
	Statement* print_stmt = create_expr_stmt(print_call);
	
	Statement** main_body_stmts = malloc(sizeof(Statement*));
	main_body_stmts[0] = print_stmt;
	Statement* main_body = create_block(main_body_stmts, 1);
	
	Method* main_method = create_method(
		MOD_PUBLIC | MOD_STATIC,
		create_type(TYPE_VOID),
		"main",
		main_params,
		1,
		main_body
	);
	
	class_add_method(main_class, main_method);

	File* f = create_file();
	file_add_class(f, A);
	file_add_class(f, main_class);
	return f;
}

/**
 *	class Main {
 *		public static void main(String args[]) { 
 *			int x = 1;
 *			int y = x + x;
 *			if (y == 2) System.out.print("ok\n");
 *		}
 *	}
 */
File* example2()
{
	ClassDecl* Main = create_class("Main");

	Parameter* args_param = create_parameter(
		create_class_type("String"),
		"args"
	);

	Parameter** main_params = malloc(sizeof(Parameter*));
	main_params[0] = args_param;

	Statement** main_stmts = malloc(3 * sizeof(Statement*));
	main_stmts[0] = create_var_decl(
		create_type(TYPE_INT),
		"x",
		create_literal_int(1)
	);
	main_stmts[1] = create_var_decl(
		create_type(TYPE_INT),
		"y",
		create_binary_op(
			OP_ADD,
			create_variable("x"),
			create_variable("x")
		)
	);

	Expression* system = create_variable("System");
	Expression* system_out = create_dot(system, "out");
	
	Expression* print_method = create_dot(system_out, "print");
	
	Expression** print_args = malloc(sizeof(Expression*));
	print_args[0] = create_literal_string("ok\n");
	Expression* print_call = create_call(
		print_method,
		print_args,
		1
	);

	Statement* stmt = create_expr_stmt(print_call);
	main_stmts[2] = create_if_stmt(
		create_binary_op(OP_EQ, create_variable("y"), create_literal_int(2)),
		stmt,
		NULL
	);

	Statement* main_body = create_block(main_stmts, 3);
	Method* main_method = create_method(
		MOD_PUBLIC | MOD_STATIC,
		create_type(TYPE_VOID),
		"main",
		main_params,
		1,
		main_body
	);
	class_add_method(Main, main_method);

	File* f = create_file();
	file_add_class(f, Main);
	return f;
}


/*
* class A {}
*/
File *example3(){

    ClassDecl* classA = create_class("A");

    File* file = create_file();
    file_add_class(file, classA);

    return file;
}

/*
*   class A {
*       int a;
*       int b = 5;
*   }
*/
File *example4(){

    ClassDecl* classA = create_class("A");

    Field* field_a = create_field(0, create_type(TYPE_INT), "a", NULL);
    class_add_field(classA, field_a);

    Field* field_b = create_field(0, create_type(TYPE_INT), "b", create_literal_int(5));
    class_add_field(classA, field_b);

    File* file = create_file();
    file_add_class(file, classA);

    return file;
}

/*
*   class A {
*       int a;
*       int getA() {
*           return a;
*       }
*   }
*/
File *example5(){

    ClassDecl* classA = create_class("A");

    Field* field_a = create_field(0, create_type(TYPE_INT), "a", NULL);
    class_add_field(classA, field_a);

    Statement **statement_list = malloc(sizeof(Statement*));
    statement_list[0] = create_return_stmt(create_variable("a"));
    Statement *block_statement = create_block(statement_list, 1);
    Method* method_getA = create_method(0, create_type(TYPE_INT), "getA", NULL, 0, block_statement);
    class_add_method(classA, method_getA);

    File* file = create_file();
    file_add_class(file, classA);

    return file;
}

/*
*   class A {
*       private int a;
*       int b = 1;
*       A(int c) {
*           a = c;
*       }
*       public static void main(String args[]) {
*           System.out.println(b);
*       }
*   }
*/

File *example6(){

    ClassDecl* classA = create_class("A");

    Field* field_a = create_field(MOD_PRIVATE, create_type(TYPE_INT), "a", NULL);
    class_add_field(classA, field_a);

    Field* field_b = create_field(0, create_type(TYPE_INT), "b", create_literal_int(1));
    class_add_field(classA, field_b);

    Statement **statement_list = malloc(sizeof(Statement*));
    statement_list[0] = create_expr_stmt(
        create_binary_op(
            OP_ASSIGN,
            create_variable("a"),
            create_variable("c")
        )
    );
    Statement *block_statement = create_block(statement_list, 1);
    Parameter **param_list = malloc(sizeof(Parameter*));
    param_list[0] = create_parameter(create_type(TYPE_INT), "c");
    Constructor* constructor_A = create_constructor(0, "A", param_list, 1, block_statement);
    class_add_constructor(classA, constructor_A);

    Statement **main_statements = malloc(sizeof(Statement*));
    Expression* system = create_variable("System");
    Expression* system_out = create_dot(system, "out");
    Expression* println_method = create_dot(system_out, "println");
    Expression** println_args = malloc(sizeof(Expression*));
    println_args[0] = create_variable("b");
    Expression* println_call = create_call(
        println_method,
        println_args,
        1
    );
    main_statements[0] = create_expr_stmt(println_call);
    Statement *block_statement_meth = create_block(main_statements, 1);
    Method* main_method = create_method(
        MOD_PUBLIC | MOD_STATIC,
        create_type(TYPE_VOID),
        "main",
        NULL,
        0,
        block_statement_meth
    );
    class_add_method(classA, main_method);


    File* file = create_file();
    file_add_class(file, classA);

    return file;
}

File *example7(){
    File *file = create_file();
    file_add_class(file, create_class("A"));
    file_add_class(file, create_class("B"));

    return file;
}

AST example8(){
	Expression *expr1 = create_binary_op(OP_ADD, create_literal_int(5), create_literal_int(10));
	Expression *expr2 = create_binary_op(OP_MUL, create_literal_int(2), create_literal_int(3));
	Expression *expr3 = create_binary_op(OP_SUB, expr1, expr2);
	Expression *expr4 = create_binary_op(OP_ADD, expr3, create_literal_int(0));
	Expression *expr5 = create_binary_op(OP_ADD, expr4, create_literal_int(4));
	Expression *expr6 = create_binary_op(OP_DIV, expr5, create_literal_int(2));
	Expression *expr7 = create_binary_op(OP_MOD, create_literal_int(3), expr6);
	return (AST){ .type = AST_EXPRESSION, .data.expression = expr7 };
}

AST example9(){
	File *file = create_file();
	for (int i = 0; i < 20; i++){
		ClassDecl *cls = create_class("A");
		file_add_class(file, cls);
	}
	return (AST){ .type = AST_FILE, .data.file = file };
}

int main(void){
    File *ast = example6();
	mainloop((AST){ .type = AST_FILE, .data.file = ast });
    return 0;
}
