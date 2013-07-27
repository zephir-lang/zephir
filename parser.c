/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 21 "parser.lemon"


#include "json/json.h"

#include "string.h"
#include "parser.h"
#include "scanner.h"
#include "xx.h"

static json_object *xx_ret_literal(int type, xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	switch (type) {
		case XX_T_IDENTIFIER:
			json_object_object_add(ret, "type", json_object_new_string("variable"));
			break;
		case XX_T_INTEGER:
			json_object_object_add(ret, "type", json_object_new_string("int"));
			break;
		case XX_T_DOUBLE:
			json_object_object_add(ret, "type", json_object_new_string("double"));
			break;
		case XX_T_NULL:
			json_object_object_add(ret, "type", json_object_new_string("null"));
			break;
		case XX_T_STRING:
			json_object_object_add(ret, "type", json_object_new_string("string"));
			break;
		default:
			if (type == XX_T_TRUE) {
				json_object_object_add(ret, "type", json_object_new_string("bool"));
				json_object_object_add(ret, "value", json_object_new_string("true"));
			} else {
				if (type == XX_T_FALSE) {
					json_object_object_add(ret, "type", json_object_new_string("bool"));
					json_object_object_add(ret, "value", json_object_new_string("false"));
				} else {
					fprintf(stderr, "literal??\n");
				}
			}
	}

	if (T) {
		json_object_object_add(ret, "value", json_object_new_string(T->token));
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_expr(char *type, json_object *left, json_object *right, json_object *extra, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string(type));

	if (left) {
		json_object_object_add(ret, "left", left);
	}
	if (right) {
		json_object_object_add(ret, "right", right);
	}
	if (extra) {
		json_object_object_add(ret, "extra", extra);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_array_item(json_object *key, json_object *value, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (key) {
		json_object_object_add(ret, "key", key);
	}
	json_object_object_add(ret, "value", value);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_namespace(xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("namespace"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition, int is_abstract, xx_parser_token *E, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("class"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));
	json_object_object_add(ret, "abtract", json_object_new_int(is_abstract));

	if (E) {
		json_object_object_add(ret, "extends", json_object_new_string(E->token));
	}

	if (class_definition) {
		json_object_object_add(ret, "definition", class_definition);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class_definition(json_object *properties, json_object *methods, json_object *constants, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (properties) {
		json_object_object_add(ret, "properties", properties);
	}
	if (methods) {
		json_object_object_add(ret, "methods", methods);
	}
	if (constants) {
		json_object_object_add(ret, "constants", constants);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class_property(int visibility, xx_parser_token *T, json_object *default_value, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (visibility == XX_T_PUBLIC) {
		json_object_object_add(ret, "visibility", json_object_new_string("public"));
	} else {
		json_object_object_add(ret, "visibility", json_object_new_string("protected"));
	}
	json_object_object_add(ret, "type", json_object_new_string("property"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (default_value) {
		json_object_object_add(ret, "default", default_value);
	}

	if (D) {
		json_object_object_add(ret, "docblock", json_object_new_string(D->token));
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class_const(xx_parser_token *T, json_object *default_value, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("const"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));
	json_object_object_add(ret, "default", default_value);

	if (D) {
		json_object_object_add(ret, "docblock", json_object_new_string(D->token));
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_class_method(int visibility, xx_parser_token *T, json_object *parameters, json_object *statements, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (visibility == XX_T_PUBLIC) {
		json_object_object_add(ret, "visibility", json_object_new_string("public"));
	} else {
		json_object_object_add(ret, "visibility", json_object_new_string("protected"));
	}
	json_object_object_add(ret, "type", json_object_new_string("method"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	if (D) {
		json_object_object_add(ret, "docblock", json_object_new_string(D->token));
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_parameter(json_object *type, json_object *cast, xx_parser_token *N, json_object *default_value, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("parameter"));
	json_object_object_add(ret, "name", json_object_new_string(N->token));

	if (type) {
		json_object_object_add(ret, "data-type", type);
	}
	if (cast) {
		json_object_object_add(ret, "cast", cast);
	}
	if (default_value) {
		json_object_object_add(ret, "default", default_value);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_type(int type)
{
	switch (type) {
		case XX_TYPE_INTEGER:
			return json_object_new_string("int");
		case XX_TYPE_DOUBLE:
			return json_object_new_string("double");
		case XX_TYPE_BOOL:
			return json_object_new_string("bool");
		case XX_TYPE_STRING:
			return json_object_new_string("string");
		case XX_TYPE_ARRAY:
			return json_object_new_string("array");
		case XX_TYPE_VAR:
			return json_object_new_string("variable");
		default:
			fprintf(stderr, "err 1?\n");
	}

}

static json_object *xx_ret_list(json_object *list_left, json_object *right_list)
{
	json_object *ret;
	int i, array_length;

	ret = json_object_new_array();

	if (list_left) {

		if (json_object_get_type(list_left) == json_type_array) {
			array_length = json_object_array_length(list_left);
			for (i = 0; i < array_length; i++){
				json_object_array_add(ret, json_object_array_get_idx(list_left, i));
			}
			//json_object_put(list_left);
		} else {
			json_object_array_add(ret, list_left);
		}

	}

	json_object_array_add(ret, right_list);

	return ret;
}

static json_object *xx_ret_let_statement(json_object *assignments, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("let"));
	json_object_object_add(ret, "assignments", assignments);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_let_assignment(char *type, xx_parser_token *V, xx_parser_token *P, json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "assign-type", json_object_new_string(type));
	json_object_object_add(ret, "variable", json_object_new_string(V->token));
	if (P) {
		json_object_object_add(ret, "property", json_object_new_string(P->token));
	}
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_if_statement(json_object *expr, json_object *statements, json_object *else_statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("if"));
	json_object_object_add(ret, "expr", expr);

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	if (else_statements) {
		json_object_object_add(ret, "else_statements", else_statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_while_statement(json_object *expr, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("while"));
	json_object_object_add(ret, "expr", expr);

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_loop_statement(json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("loop"));

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_break_statement(xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("break"));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_continue_statement(xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("continue"));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_echo_statement(json_object *expressions, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("echo"));
	json_object_object_add(ret, "expressions", expressions);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_return_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return"));
	json_object_object_add(ret, "expr", expr);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_declare_statement(int type, json_object *variables, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("declare"));

	switch (type) {
		case XX_T_TYPE_INTEGER:
			json_object_object_add(ret, "data-type", json_object_new_string("int"));
			break;
		case XX_T_TYPE_DOUBLE:
			json_object_object_add(ret, "data-type", json_object_new_string("double"));
			break;
		case XX_T_TYPE_BOOL:
			json_object_object_add(ret, "data-type", json_object_new_string("bool"));
			break;
		case XX_T_TYPE_STRING:
			json_object_object_add(ret, "data-type", json_object_new_string("string"));
			break;
		/*case XX_T_TYPE_ARRAY:
			json_object_object_add(ret, "data-type", json_object_new_string("array"));
			break;*/
		case XX_T_TYPE_VAR:
			json_object_object_add(ret, "data-type", json_object_new_string("variable"));
			break;
		default:
			fprintf(stderr, "err 2?\n");
	}

	json_object_object_add(ret, "variables", variables);

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_declare_variable(xx_parser_token *T, json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "variable", json_object_new_string(T->token));
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_new_instance(xx_parser_token *T, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("new"));
	json_object_object_add(ret, "class", json_object_new_string(T->token));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_comment(xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("comment"));
	json_object_object_add(ret, "value", json_object_new_string(T->token));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}


// 540 "parser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    xx_TOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is xx_TOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    xx_ARG_SDECL     A static variable declaration for the %extra_argument
**    xx_ARG_PDECL     A parameter declaration for the %extra_argument
**    xx_ARG_STORE     Code to store %extra_argument into yypParser
**    xx_ARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 100
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy115;
  int yy199;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 325
#define YYNRULE 159
#define YYERRORSYMBOL 54
#define YYERRSYMDT yy199
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* Next are that tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
static YYACTIONTYPE yy_action[] = {
 /*     0 */    25,  172,  325,  168,  485,    1,    2,  324,    4,    5,
 /*    10 */     6,  147,  156,  162,  159,  172,   39,   41,   43,   44,
 /*    20 */    52,   56,   61,    3,    4,    5,    6,   95,  134,  142,
 /*    30 */   165,   27,   90,   57,  168,   97,   98,   99,  100,  101,
 /*    40 */     8,  102,  147,  156,  162,  159,   95,   39,   41,   43,
 /*    50 */     9,   52,   56,   61,   62,  133,   91,   92,  103,  134,
 /*    60 */   142,  165,  186,  139,   15,  168,   11,  272,  285,   64,
 /*    70 */   291,  265,   89,  147,  156,  162,  159,   94,   39,   41,
 /*    80 */    43,   63,   52,   56,   61,  135,  141,   69,   75,  115,
 /*    90 */   134,  142,  165,   47,  113,  341,  168,  115,  272,  285,
 /*   100 */   137,  291,  265,  127,  147,  156,  162,  159,   14,   39,
 /*   110 */    41,   43,  136,   52,   56,   61,  111,  117,   12,  112,
 /*   120 */   305,  134,  142,  165,  185,  114,   18,  168,  139,  256,
 /*   130 */    17,  271,  124,  297,  270,  147,  156,  162,  159,  172,
 /*   140 */    39,   41,   43,   20,   52,   56,   61,  171,  174,  176,
 /*   150 */   178,  138,  134,  142,  165,   51,  196,  173,  168,   21,
 /*   160 */    45,  171,  174,  176,  178,  196,  147,  156,  162,  159,
 /*   170 */   193,   39,   41,   43,   58,   52,   56,   61,  197,  201,
 /*   180 */   205,  148,  155,  134,  142,  165,  184,   22,  345,  168,
 /*   190 */   219,  259,   19,   40,  247,  265,   42,  147,  156,  162,
 /*   200 */   159,  150,   39,   41,   43,   67,   52,   56,   61,  223,
 /*   210 */    48,  238,  260,  149,  134,  142,  165,   55,  257,   49,
 /*   220 */   168,  258,  270,  119,  123,   53,  131,  132,  147,  156,
 /*   230 */   162,  159,  172,   39,   41,   43,   66,   52,   56,   61,
 /*   240 */   172,  273,  196,  279,  260,  134,  142,  165,  170,    7,
 /*   250 */   175,  168,   10,  223,   95,  238,  311,  168,  177,  147,
 /*   260 */   156,  162,  159,   65,   39,   41,   43,  233,   52,   56,
 /*   270 */    61,  300,  271,  157,  155,  270,  134,  142,  165,   60,
 /*   280 */    88,  275,  168,  106,   92,  103,  150,  160,  155,  276,
 /*   290 */   147,  156,  162,  159,  150,   39,   41,   43,  158,   52,
 /*   300 */    56,   61,  150,  163,  155,  150,  161,  134,  142,  165,
 /*   310 */   191,  172,  342,  168,  164,  272,  285,  167,  291,  265,
 /*   320 */    72,  147,  156,  162,  159,  274,   39,   41,   43,  179,
 /*   330 */    52,   56,   61,  166,  155,  280,   68,  224,  134,  142,
 /*   340 */   165,  190,  172,  299,  168,  302,  303,  239,  258,  270,
 /*   350 */    74,   71,  147,  156,  162,  159,  281,   39,   41,   43,
 /*   360 */   181,   52,   56,   61,  282,   73,  273,  286,  279,  134,
 /*   370 */   142,  165,  195,   93,  310,  168,   76,  272,  285,   20,
 /*   380 */   291,  265,  465,  147,  156,  162,  159,   78,   39,   41,
 /*   390 */    43,  287,   52,   56,   61,  314,  466,  318,  292,  288,
 /*   400 */   134,  142,  165,  229,   80,  317,  168,  108,  272,  285,
 /*   410 */   248,  291,  265,  109,  147,  156,  162,  159,  120,   39,
 /*   420 */    41,   43,  293,   52,   56,   61,  121,  122,  125,  126,
 /*   430 */   294,  134,  142,  165,  231,  128,  323,  168,  129,  272,
 /*   440 */   285,  130,  291,  265,  143,  147,  156,  162,  159,  144,
 /*   450 */    39,   41,   43,  146,   52,   56,   61,  152,  151,  153,
 /*   460 */   154,  187,  134,  142,  165,  235,  200,  344,  168,  188,
 /*   470 */   222,   19,  202,  247,  199,  204,  147,  156,  162,  159,
 /*   480 */   203,   39,   41,   43,  206,   52,   56,   61,  207,  208,
 /*   490 */   210,  211,  220,  134,  142,  165,  237,  386,  221,  168,
 /*   500 */   387,  388,  218,  389,  217,  201,  205,  147,  156,  162,
 /*   510 */   159,  224,   39,   41,   43,  390,   52,   56,   61,  391,
 /*   520 */   225,  226,  239,  242,  134,  142,  165,  244,  228,  234,
 /*   530 */   168,  240,  243,  192,  241,  217,  201,  205,  147,  156,
 /*   540 */   162,  159,  248,   39,   41,   43,  249,   52,   56,   61,
 /*   550 */   250,  252,  251,  261,  262,  134,  142,  165,  246,  263,
 /*   560 */   264,  168,  266,  306,  232,  267,  217,  201,  205,  147,
 /*   570 */   156,  162,  159,  268,   39,   41,   43,  277,   52,   56,
 /*   580 */    61,  269,  278,  283,  284,  290,  134,  142,  165,  253,
 /*   590 */   289,  348,  168,  296,  222,   19,  295,  247,  307,  312,
 /*   600 */   147,  156,  162,  159,  309,   39,   41,   43,  313,   52,
 /*   610 */    56,   61,  319,  316,  320,  260,  260,  134,  142,  165,
 /*   620 */   255,  322,  346,  168,  260,  301,  285,  260,  291,  260,
 /*   630 */   260,  147,  156,  162,  159,  260,   39,   41,   43,  260,
 /*   640 */    52,   56,   61,  168,  260,  260,  260,  260,  134,  142,
 /*   650 */   165,  147,  156,  162,  159,  260,   39,   41,   43,  260,
 /*   660 */    52,   56,   61,   38,  171,  174,  176,  178,  134,  142,
 /*   670 */   165,  260,  260,   26,  260,  260,  260,  169,   29,   30,
 /*   680 */    31,   32,   33,   34,   35,   36,   37,  260,  182,  260,
 /*   690 */   260,  260,  260,   38,  260,  260,  349,  260,  209,  222,
 /*   700 */    19,  260,  247,   46,  260,  260,  198,  169,   29,   30,
 /*   710 */    31,   32,   33,   34,   35,   36,   37,  260,   38,  260,
 /*   720 */    23,  212,  213,  214,  215,  216,  260,  260,   50,  260,
 /*   730 */   260,  260,  169,   29,   30,   31,   32,   33,   34,   35,
 /*   740 */    36,   37,   38,  260,   13,   16,  298,  304,  303,  260,
 /*   750 */   297,  270,   54,  260,  260,  260,  169,   29,   30,   31,
 /*   760 */    32,   33,   34,   35,   36,   37,  260,  347,  260,   38,
 /*   770 */   222,   19,  260,  247,  209,  260,  260,  260,  260,   59,
 /*   780 */   260,  260,  198,  169,   29,   30,   31,   32,   33,   34,
 /*   790 */    35,   36,   37,  260,   38,  260,  227,  212,  213,  214,
 /*   800 */   215,  216,  260,  260,  189,  260,  260,  260,  169,   29,
 /*   810 */    30,   31,   32,   33,   34,   35,   36,   37,   38,  260,
 /*   820 */   260,  343,  260,  209,  222,   19,  260,  247,  194,  260,
 /*   830 */   260,  198,  169,   29,   30,   31,   32,   33,   34,   35,
 /*   840 */    36,   37,  260,  260,  260,   38,  212,  213,  214,  215,
 /*   850 */   216,  260,  260,  260,  260,  230,  260,  260,  260,  169,
 /*   860 */    29,   30,   31,   32,   33,   34,   35,   36,   37,  260,
 /*   870 */    38,  260,  308,   16,  298,  304,  303,  260,  297,  270,
 /*   880 */   236,  260,  260,  260,  169,   29,   30,   31,   32,   33,
 /*   890 */    34,   35,   36,   37,   38,  260,  315,   16,  298,  304,
 /*   900 */   303,  260,  297,  270,  245,  260,  260,  260,  169,   29,
 /*   910 */    30,   31,   32,   33,   34,   35,   36,   37,  260,  260,
 /*   920 */   260,   38,  260,  260,   79,   77,  107,  260,   70,  260,
 /*   930 */   260,  254,   38,  260,  260,  169,   29,   30,   31,   32,
 /*   940 */    33,   34,   35,   36,   37,  260,   28,   29,   30,   31,
 /*   950 */    32,   33,   34,   35,   36,   37,   86,  183,  260,  260,
 /*   960 */   209,  260,  260,   81,   82,   83,   84,   85,  198,  118,
 /*   970 */   180,   24,  260,   96,  260,  321,   16,  298,  304,  303,
 /*   980 */    96,  297,  270,  212,  213,  214,  215,  216,  260,  260,
 /*   990 */   260,  260,   97,   98,   99,  100,  101,  260,  102,  116,
 /*  1000 */   260,  260,   87,  260,  260,  260,   96,  260,  105,  104,
 /*  1010 */    99,  100,  101,  110,  102,  105,  104,   99,  100,  101,
 /*  1020 */   260,  102,  116,  260,  260,  260,  260,  260,  260,  140,
 /*  1030 */   260,  260,  260,  260,   97,   98,   99,  100,  101,  260,
 /*  1040 */   102,   97,   98,   99,  100,  101,  260,  102,  145,  260,
 /*  1050 */   260,  260,  260,  260,  260,  260,  260,   97,   98,   99,
 /*  1060 */   100,  101,  326,  102,   97,   98,   99,  100,  101,  260,
 /*  1070 */   102,  260,  260,    7,  260,  260,   10,  260,  260,  260,
 /*  1080 */   311,  168,  260,   97,   98,   99,  100,  101,  260,  102,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    16,   67,    0,   19,   55,   56,   57,   58,   59,   60,
 /*    10 */    61,   27,   28,   29,   30,   67,   32,   33,   34,   85,
 /*    20 */    36,   37,   38,   58,   59,   60,   61,   67,   44,   45,
 /*    30 */    46,   16,    1,   85,   19,   47,   48,   49,   50,   51,
 /*    40 */    12,   53,   27,   28,   29,   30,   67,   32,   33,   34,
 /*    50 */    13,   36,   37,   38,   86,   87,   96,   97,   98,   44,
 /*    60 */    45,   46,   16,   67,   16,   19,   12,   19,   20,    1,
 /*    70 */    22,   23,   41,   27,   28,   29,   30,   98,   32,   33,
 /*    80 */    34,   13,   36,   37,   38,   89,   90,    6,    7,   67,
 /*    90 */    44,   45,   46,   16,    1,   16,   19,   67,   19,   20,
 /*   100 */     1,   22,   23,   12,   27,   28,   29,   30,   16,   32,
 /*   110 */    33,   34,   13,   36,   37,   38,   94,   95,   15,   26,
 /*   120 */    17,   44,   45,   46,   16,   95,   69,   19,   67,   64,
 /*   130 */    65,   66,   41,   68,   69,   27,   28,   29,   30,   67,
 /*   140 */    32,   33,   34,   24,   36,   37,   38,    2,    3,    4,
 /*   150 */     5,   90,   44,   45,   46,   16,    1,   85,   19,   12,
 /*   160 */    15,    2,    3,    4,    5,    1,   27,   28,   29,   30,
 /*   170 */    15,   32,   33,   34,   15,   36,   37,   38,   72,   73,
 /*   180 */    74,   91,   92,   44,   45,   46,   16,   25,   16,   19,
 /*   190 */    26,   19,   20,   13,   22,   23,   13,   27,   28,   29,
 /*   200 */    30,    1,   32,   33,   34,   21,   36,   37,   38,   20,
 /*   210 */    35,   22,   23,   13,   44,   45,   46,   16,   65,   15,
 /*   220 */    19,   68,   69,   39,   40,   15,   42,   43,   27,   28,
 /*   230 */    29,   30,   67,   32,   33,   34,   12,   36,   37,   38,
 /*   240 */    67,   20,    1,   22,   23,   44,   45,   46,   16,   11,
 /*   250 */    85,   19,   14,   20,   67,   22,   18,   19,   85,   27,
 /*   260 */    28,   29,   30,   87,   32,   33,   34,   26,   36,   37,
 /*   270 */    38,   65,   66,   91,   92,   69,   44,   45,   46,   16,
 /*   280 */    93,   13,   19,   96,   97,   98,    1,   91,   92,   21,
 /*   290 */    27,   28,   29,   30,    1,   32,   33,   34,   13,   36,
 /*   300 */    37,   38,    1,   91,   92,    1,   13,   44,   45,   46,
 /*   310 */    16,   67,   16,   19,   13,   19,   20,   13,   22,   23,
 /*   320 */    12,   27,   28,   29,   30,   12,   32,   33,   34,   85,
 /*   330 */    36,   37,   38,   91,   92,   12,   88,   24,   44,   45,
 /*   340 */    46,   16,   67,   63,   19,   65,   66,   24,   68,   69,
 /*   350 */    88,   40,   27,   28,   29,   30,   13,   32,   33,   34,
 /*   360 */    85,   36,   37,   38,   21,   41,   20,   12,   22,   44,
 /*   370 */    45,   46,   16,   52,   16,   19,   88,   19,   20,   24,
 /*   380 */    22,   23,   52,   27,   28,   29,   30,   88,   32,   33,
 /*   390 */    34,   13,   36,   37,   38,   15,   52,   17,   12,   21,
 /*   400 */    44,   45,   46,   16,   88,   16,   19,   12,   19,   20,
 /*   410 */    24,   22,   23,   25,   27,   28,   29,   30,   12,   32,
 /*   420 */    33,   34,   13,   36,   37,   38,   21,   88,   21,   88,
 /*   430 */    21,   44,   45,   46,   16,   41,   16,   19,   21,   19,
 /*   440 */    20,   88,   22,   23,   67,   27,   28,   29,   30,   13,
 /*   450 */    32,   33,   34,   13,   36,   37,   38,   12,   92,   21,
 /*   460 */    67,   35,   44,   45,   46,   16,   67,   16,   19,   15,
 /*   470 */    19,   20,   12,   22,   21,   67,   27,   28,   29,   30,
 /*   480 */    21,   32,   33,   34,   12,   36,   37,   38,   21,   67,
 /*   490 */    12,    5,   15,   44,   45,   46,   16,   12,   16,   19,
 /*   500 */    12,   12,   70,   12,   72,   73,   74,   27,   28,   29,
 /*   510 */    30,   24,   32,   33,   34,   12,   36,   37,   38,   12,
 /*   520 */    12,   25,   24,   26,   44,   45,   46,   16,   15,   15,
 /*   530 */    19,   12,   15,   70,   25,   72,   73,   74,   27,   28,
 /*   540 */    29,   30,   24,   32,   33,   34,   12,   36,   37,   38,
 /*   550 */    25,   15,   26,   12,   21,   44,   45,   46,   16,   67,
 /*   560 */    13,   19,   12,   12,   70,   21,   72,   73,   74,   27,
 /*   570 */    28,   29,   30,   67,   32,   33,   34,   67,   36,   37,
 /*   580 */    38,   13,   13,   67,   13,   13,   44,   45,   46,   16,
 /*   590 */    67,   16,   19,   13,   19,   20,   67,   22,   15,   14,
 /*   600 */    27,   28,   29,   30,   16,   32,   33,   34,   12,   36,
 /*   610 */    37,   38,   12,   16,   15,   99,   99,   44,   45,   46,
 /*   620 */    16,   16,   16,   19,   99,   19,   20,   99,   22,   99,
 /*   630 */    99,   27,   28,   29,   30,   99,   32,   33,   34,   99,
 /*   640 */    36,   37,   38,   19,   99,   99,   99,   99,   44,   45,
 /*   650 */    46,   27,   28,   29,   30,   99,   32,   33,   34,   99,
 /*   660 */    36,   37,   38,   61,    2,    3,    4,    5,   44,   45,
 /*   670 */    46,   99,   99,   71,   99,   99,   99,   75,   76,   77,
 /*   680 */    78,   79,   80,   81,   82,   83,   84,   99,   26,   99,
 /*   690 */    99,   99,   99,   61,   99,   99,   16,   99,    4,   19,
 /*   700 */    20,   99,   22,   71,   99,   99,   12,   75,   76,   77,
 /*   710 */    78,   79,   80,   81,   82,   83,   84,   99,   61,   99,
 /*   720 */    26,   27,   28,   29,   30,   31,   99,   99,   71,   99,
 /*   730 */    99,   99,   75,   76,   77,   78,   79,   80,   81,   82,
 /*   740 */    83,   84,   61,   99,   62,   63,   64,   65,   66,   99,
 /*   750 */    68,   69,   71,   99,   99,   99,   75,   76,   77,   78,
 /*   760 */    79,   80,   81,   82,   83,   84,   99,   16,   99,   61,
 /*   770 */    19,   20,   99,   22,    4,   99,   99,   99,   99,   71,
 /*   780 */    99,   99,   12,   75,   76,   77,   78,   79,   80,   81,
 /*   790 */    82,   83,   84,   99,   61,   99,   26,   27,   28,   29,
 /*   800 */    30,   31,   99,   99,   71,   99,   99,   99,   75,   76,
 /*   810 */    77,   78,   79,   80,   81,   82,   83,   84,   61,   99,
 /*   820 */    99,   16,   99,    4,   19,   20,   99,   22,   71,   99,
 /*   830 */    99,   12,   75,   76,   77,   78,   79,   80,   81,   82,
 /*   840 */    83,   84,   99,   99,   99,   61,   27,   28,   29,   30,
 /*   850 */    31,   99,   99,   99,   99,   71,   99,   99,   99,   75,
 /*   860 */    76,   77,   78,   79,   80,   81,   82,   83,   84,   99,
 /*   870 */    61,   99,   62,   63,   64,   65,   66,   99,   68,   69,
 /*   880 */    71,   99,   99,   99,   75,   76,   77,   78,   79,   80,
 /*   890 */    81,   82,   83,   84,   61,   99,   62,   63,   64,   65,
 /*   900 */    66,   99,   68,   69,   71,   99,   99,   99,   75,   76,
 /*   910 */    77,   78,   79,   80,   81,   82,   83,   84,   99,   99,
 /*   920 */    99,   61,   99,   99,    8,    9,   10,   99,   12,   99,
 /*   930 */    99,   71,   61,   99,   99,   75,   76,   77,   78,   79,
 /*   940 */    80,   81,   82,   83,   84,   99,   75,   76,   77,   78,
 /*   950 */    79,   80,   81,   82,   83,   84,   40,   12,   99,   99,
 /*   960 */     4,   99,   99,   47,   48,   49,   50,   51,   12,   53,
 /*   970 */    25,   15,   99,   12,   99,   62,   63,   64,   65,   66,
 /*   980 */    12,   68,   69,   27,   28,   29,   30,   31,   99,   99,
 /*   990 */    99,   99,   47,   48,   49,   50,   51,   99,   53,   12,
 /*  1000 */    99,   99,   41,   99,   99,   99,   12,   99,   47,   48,
 /*  1010 */    49,   50,   51,   26,   53,   47,   48,   49,   50,   51,
 /*  1020 */    99,   53,   12,   99,   99,   99,   99,   99,   99,   12,
 /*  1030 */    99,   99,   99,   99,   47,   48,   49,   50,   51,   99,
 /*  1040 */    53,   47,   48,   49,   50,   51,   99,   53,   12,   99,
 /*  1050 */    99,   99,   99,   99,   99,   99,   99,   47,   48,   49,
 /*  1060 */    50,   51,    0,   53,   47,   48,   49,   50,   51,   99,
 /*  1070 */    53,   99,   99,   11,   99,   99,   14,   99,   99,   99,
 /*  1080 */    18,   19,   99,   47,   48,   49,   50,   51,   99,   53,
};
#define YY_SHIFT_USE_DFLT (-17)
static short yy_shift_ofst[] = {
 /*     0 */   238,    2, 1062,  -17,  -17,  -17,  -17,   28,   37,  -17,
 /*    10 */    54,  103,   48,   92,  -17,  -17,   79,  451,  -17,  119,
 /*    20 */   147,  162,  694,  956,  -16,  -17,   15,  -17,  -17,  -17,
 /*    30 */   -17,  -17,  -17,  -17,  -17,  -17,  -17,  -17,  -17,  180,
 /*    40 */   -17,  183,  -17,  945,  145,   46,   77,  175,  204,  108,
 /*    50 */   139,  -17,  210,  170,  201,  -17,  945,  159,  232,  263,
 /*    60 */   -17,  224,   68,  -17,  224,  -17,  184,  916,   81,  916,
 /*    70 */   311,  308,  324,  -17,  -17,  916,  -17,  916,  -17,  916,
 /*    80 */   -17,  -17,  -17,  -17,  -17,  -17,  961,  -17,   31,  -17,
 /*    90 */   968,  -17,  321,  994,  -17,  -17,  -17,  -17,  -17,  -17,
 /*   100 */   -17,  -17,  -17,  -17,  330,  344,  -17,  395,  388,  987,
 /*   110 */   -17,   93,  -17, 1010,  -17,  -17,  -17,  -17,  -17,  406,
 /*   120 */   405,  916,   81,   91,  407,  916,   81,  394,  417,  916,
 /*   130 */    81,  -17,  -17,  -17, 1017,   99,  -17, 1017,  -17,  -17,
 /*   140 */   -17,  -17, 1036,  436,  -17,  440,  -17,  445,  200,  -17,
 /*   150 */   445,  -17,  438,  -12,  -17,  -17,  445,  285,  -17,  445,
 /*   160 */   293,  -17,  445,  301,  -17,  445,  304,  -17,  -17,  -17,
 /*   170 */   -17,  945,  -17,  -17,  945,  -17,  945,  -17,  945,  -17,
 /*   180 */   945,  662,  -17,  -17,  -17,  -17,  426,  454,  294,  325,
 /*   190 */   -17,  -17,  155,  624,  356,  -17,  819,  -17,  453,  -12,
 /*   200 */   -17,  460,  459,  -12,  -17,  472,  467,  -12,  -17,  478,
 /*   210 */   486,  485,  488,  489,  491,  503,  507,  -17,  164,  477,
 /*   220 */   482,  -17,  233,  487,  508,  496,  770,  513,  387,  -17,
 /*   230 */   418,  -17,  241,  514,  449,  -17,  480,  -17,  498,  519,
 /*   240 */   509,  497,  517,  511,  -17,  542,  -17,  518,  534,  525,
 /*   250 */   526,  536,  573,  -17,  604,  -17,  172,  575,  -17,  189,
 /*   260 */   541,  533,  -12,  547,  -17,  550,  544,  -12,  568,  -17,
 /*   270 */   -17,  -17,  221,  313,  268,  -17,  -12,  569,  -17,  323,
 /*   280 */   343,  -17,  -12,  571,  -17,  355,  378,  -17,  -12,  572,
 /*   290 */   -17,  386,  409,  -17,  -12,  580,  -17,  -17,  296,  606,
 /*   300 */   680,  346,  751,  -17,  805,  551,  583,  358,  588,  -17,
 /*   310 */   -17,  585,  596,  380,  389,  597,  -17,  -17,  600,  599,
 /*   320 */   420,  605,  -17,  -17,  -17,
};
#define YY_REDUCE_USE_DFLT (-67)
static short yy_reduce_ofst[] = {
 /*     0 */   -51,  -67,  -35,  -67,  -67,  -67,  -67,  -67,  -67,  -67,
 /*    10 */   -67,  -67,  682,  -67,  -67,  -67,   65,   57,  -67,  -67,
 /*    20 */   -67,  -67,  432,  463,  602,  -67,  871,  -67,  -67,  -67,
 /*    30 */   -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,
 /*    40 */   -67,  -67,  -67,  -66,  -67,  632,  871,  -67,  -67,  657,
 /*    50 */   871,  -67,  -67,  681,  871,  -67,  -52,  -67,  708,  871,
 /*    60 */   -67,  -32,  -67,  -67,  176,  -67,  -67,  248,  -67,  262,
 /*    70 */   -67,  -67,  -67,  -67,  -67,  288,  -67,  299,  -67,  316,
 /*    80 */   -67,  -67,  -67,  -67,  -67,  -67,  187,  -67,  -67,  -67,
 /*    90 */   -40,  -67,  -67,  -21,  -67,  -67,  -67,  -67,  -67,  -67,
 /*   100 */   -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,   22,
 /*   110 */   -67,  -67,  -67,   30,  -67,  -67,  -67,  -67,  -67,  -67,
 /*   120 */   -67,  339,  -67,  -67,  -67,  341,  -67,  -67,  -67,  353,
 /*   130 */   -67,  -67,  -67,  -67,   -4,  -67,  -67,   61,  -67,  -67,
 /*   140 */   -67,  -67,  377,  -67,  -67,  -67,  -67,   90,  -67,  -67,
 /*   150 */   366,  -67,  -67,  393,  -67,  -67,  182,  -67,  -67,  196,
 /*   160 */   -67,  -67,  212,  -67,  -67,  242,  -67,  -67,  -67,  -67,
 /*   170 */   -67,   72,  -67,  -67,  165,  -67,  173,  -67,  244,  -67,
 /*   180 */   275,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  733,  871,
 /*   190 */   -67,  -67,  -67,  757,  871,  -67,  106,  -67,  -67,  399,
 /*   200 */   -67,  -67,  -67,  408,  -67,  -67,  -67,  422,  -67,  -67,
 /*   210 */   -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,  -67,
 /*   220 */   -67,  -67,  -67,  -67,  -67,  -67,  494,  -67,  784,  -67,
 /*   230 */   871,  -67,  -67,  -67,  809,  -67,  871,  -67,  -67,  -67,
 /*   240 */   -67,  -67,  -67,  833,  -67,  871,  -67,  -67,  -67,  -67,
 /*   250 */   -67,  -67,  860,  -67,  871,  -67,  153,   57,  -67,  -67,
 /*   260 */   -67,  -67,  492,  -67,  -67,  -67,  -67,  506,  -67,  -67,
 /*   270 */   -67,  -67,  -67,  -67,  -67,  -67,  510,  -67,  -67,  -67,
 /*   280 */   -67,  -67,  516,  -67,  -67,  -67,  -67,  -67,  523,  -67,
 /*   290 */   -67,  -67,  -67,  -67,  529,  -67,  -67,  -67,  280,  206,
 /*   300 */    57,  -67,   57,  -67,   57,  -67,  -67,  810,  -67,  -67,
 /*   310 */   -67,  -67,  -67,  -67,  834,  -67,  -67,  -67,  -67,  -67,
 /*   320 */   913,  -67,  -67,  -67,  -67,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   484,  484,  484,  327,  329,  330,  331,  484,  484,  332,
 /*    10 */   484,  484,  484,  484,  333,  337,  484,  484,  362,  484,
 /*    20 */   484,  484,  484,  484,  484,  366,  484,  370,  392,  394,
 /*    30 */   395,  396,  397,  398,  399,  400,  401,  402,  403,  484,
 /*    40 */   404,  484,  405,  484,  484,  484,  484,  408,  484,  484,
 /*    50 */   484,  409,  484,  484,  484,  413,  484,  484,  484,  484,
 /*    60 */   415,  484,  484,  416,  484,  417,  484,  484,  419,  484,
 /*    70 */   446,  484,  484,  441,  442,  484,  443,  484,  444,  484,
 /*    80 */   445,  447,  448,  449,  450,  451,  484,  452,  484,  453,
 /*    90 */   484,  461,  484,  484,  463,  467,  468,  470,  471,  472,
 /*   100 */   473,  474,  475,  464,  471,  470,  462,  484,  454,  484,
 /*   110 */   455,  484,  456,  484,  457,  459,  460,  458,  469,  484,
 /*   120 */   484,  484,  420,  484,  484,  484,  421,  484,  484,  484,
 /*   130 */   422,  423,  424,  418,  484,  484,  425,  484,  426,  428,
 /*   140 */   429,  427,  484,  484,  430,  484,  431,  484,  484,  432,
 /*   150 */   484,  437,  439,  484,  440,  438,  484,  484,  433,  484,
 /*   160 */   484,  434,  484,  484,  435,  484,  484,  436,  483,  393,
 /*   170 */   414,  484,  476,  477,  484,  478,  484,  479,  484,  480,
 /*   180 */   484,  484,  481,  482,  412,  410,  406,  484,  484,  484,
 /*   190 */   411,  407,  484,  484,  484,  371,  484,  378,  380,  484,
 /*   200 */   383,  484,  381,  484,  384,  484,  382,  484,  385,  484,
 /*   210 */   484,  484,  484,  484,  484,  484,  484,  379,  484,  484,
 /*   220 */   484,  367,  484,  484,  484,  484,  484,  484,  484,  368,
 /*   230 */   484,  372,  484,  484,  484,  369,  484,  373,  484,  484,
 /*   240 */   484,  484,  484,  484,  375,  484,  377,  484,  484,  484,
 /*   250 */   484,  484,  484,  374,  484,  376,  484,  484,  360,  484,
 /*   260 */   484,  484,  484,  484,  363,  484,  484,  484,  484,  364,
 /*   270 */   365,  350,  484,  484,  484,  352,  484,  484,  355,  484,
 /*   280 */   484,  357,  484,  484,  359,  484,  484,  353,  484,  484,
 /*   290 */   354,  484,  484,  356,  484,  484,  358,  361,  484,  484,
 /*   300 */   484,  484,  484,  351,  484,  484,  484,  484,  484,  334,
 /*   310 */   338,  484,  484,  484,  484,  484,  335,  339,  484,  484,
 /*   320 */   484,  484,  336,  340,  328,
};
#define YY_SZ_ACTTAB (sizeof(yy_action)/sizeof(yy_action[0]))

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammer, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  int stateno;       /* The state-number */
  int major;         /* The major token value.  This is the code
                     ** number for the token at this stack level */
  YYMINORTYPE minor; /* The user-supplied minor token value.  This
                     ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
  int yyerrcnt;                 /* Shifts left before out of the error */
  xx_ARG_SDECL                /* A place to hold %extra_argument */
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void xx_Trace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *yyTokenName[] = { 
  "$",             "COMMA",         "EQUALS",        "IDENTICAL",   
  "LESS",          "GREATER",       "ADD",           "SUB",         
  "TYPEOF",        "INSTANCEOF",    "NEW",           "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",
  "BRACKET_CLOSE",  "EXTENDS",       "ABSTRACT",      "COMMENT",     
  "PUBLIC",        "ASSIGN",        "PROTECTED",     "CONST",       
  "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",  "TYPE_INTEGER",
  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",  
  "BREAK",         "CONTINUE",      "IF",            "ELSE",        
  "LOOP",          "WHILE",         "LET",           "ARROW",       
  "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "INCR",          "DECR",        
  "ECHO",          "RETURN",        "TYPE_VAR",      "INTEGER",     
  "STRING",        "DOUBLE",        "NULL",          "FALSE",       
  "COLON",         "TRUE",          "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_literal_expr",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",  "xx_statement_list",
  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",
  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",
  "xx_return_statement",  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",
  "xx_while_statement",  "xx_eval_expr",  "xx_let_assignments",  "xx_let_assignment",
  "xx_assign_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_declare_variable_list",
  "xx_declare_variable",  "xx_array_list",  "xx_call_parameters",  "xx_call_parameter",
  "xx_array_item",  "xx_array_key",  "xx_array_value",
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *yyRuleName[] = {
 /*   0 */ "program ::= xx_language",
 /*   1 */ "xx_language ::= xx_top_statement_list",
 /*   2 */ "xx_top_statement_list ::= xx_top_statement_list xx_top_statement",
 /*   3 */ "xx_top_statement_list ::= xx_top_statement",
 /*   4 */ "xx_top_statement ::= xx_namespace_def",
 /*   5 */ "xx_top_statement ::= xx_class_def",
 /*   6 */ "xx_top_statement ::= xx_comment",
 /*   7 */ "xx_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*   8 */ "xx_class_def ::= CLASS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*   9 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  10 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  11 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  12 */ "xx_class_def ::= CLASS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  13 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  14 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  15 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  16 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  17 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  18 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  19 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  20 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  21 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  22 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  23 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  24 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  25 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  26 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  27 */ "xx_class_property_definition ::= COMMENT PUBLIC IDENTIFIER DOTCOMMA",
 /*  28 */ "xx_class_property_definition ::= PUBLIC IDENTIFIER DOTCOMMA",
 /*  29 */ "xx_class_property_definition ::= PUBLIC IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  30 */ "xx_class_property_definition ::= COMMENT PUBLIC IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  31 */ "xx_class_property_definition ::= PROTECTED IDENTIFIER DOTCOMMA",
 /*  32 */ "xx_class_property_definition ::= COMMENT PROTECTED IDENTIFIER DOTCOMMA",
 /*  33 */ "xx_class_property_definition ::= PROTECTED IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  34 */ "xx_class_property_definition ::= COMMENT PROTECTED IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  35 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  36 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  37 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  38 */ "xx_class_const_definition ::= COMMENT CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  39 */ "xx_class_const_definition ::= CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  40 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  41 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  42 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  43 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE xx_parameter_list BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  47 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  48 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  49 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  50 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  51 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  52 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  53 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  54 */ "xx_parameter_list ::= xx_parameter",
 /*  55 */ "xx_parameter ::= IDENTIFIER",
 /*  56 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  57 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  58 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  59 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  60 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  61 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  62 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  63 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  64 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  65 */ "xx_parameter_type ::= TYPE_STRING",
 /*  66 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  67 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  68 */ "xx_statement_list ::= xx_statement",
 /*  69 */ "xx_statement ::= xx_let_statement",
 /*  70 */ "xx_statement ::= xx_if_statement",
 /*  71 */ "xx_statement ::= xx_loop_statement",
 /*  72 */ "xx_statement ::= xx_echo_statement",
 /*  73 */ "xx_statement ::= xx_return_statement",
 /*  74 */ "xx_statement ::= xx_declare_statement",
 /*  75 */ "xx_statement ::= xx_break_statement",
 /*  76 */ "xx_statement ::= xx_continue_statement",
 /*  77 */ "xx_statement ::= xx_while_statement",
 /*  78 */ "xx_statement ::= xx_comment",
 /*  79 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  80 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  81 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  82 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  83 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  84 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  85 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  86 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  87 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /*  88 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  89 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  90 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  91 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /*  92 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /*  93 */ "xx_let_assignments ::= xx_let_assignment",
 /*  94 */ "xx_let_assignment ::= IDENTIFIER ASSIGN xx_assign_expr",
 /*  95 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER ASSIGN xx_assign_expr",
 /*  96 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /*  97 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /*  98 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /*  99 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 100 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 101 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 102 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 103 */ "xx_echo_expression ::= xx_literal_expr",
 /* 104 */ "xx_echo_expression ::= IDENTIFIER",
 /* 105 */ "xx_return_statement ::= RETURN xx_literal_expr DOTCOMMA",
 /* 106 */ "xx_return_statement ::= RETURN IDENTIFIER DOTCOMMA",
 /* 107 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 108 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 109 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 110 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 111 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 112 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 113 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 114 */ "xx_declare_variable ::= IDENTIFIER",
 /* 115 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 116 */ "xx_assign_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 117 */ "xx_assign_expr ::= xx_assign_expr ADD xx_assign_expr",
 /* 118 */ "xx_assign_expr ::= xx_assign_expr SUB xx_assign_expr",
 /* 119 */ "xx_assign_expr ::= INSTANCEOF xx_assign_expr",
 /* 120 */ "xx_assign_expr ::= TYPEOF xx_assign_expr",
 /* 121 */ "xx_assign_expr ::= IDENTIFIER",
 /* 122 */ "xx_assign_expr ::= INTEGER",
 /* 123 */ "xx_assign_expr ::= STRING",
 /* 124 */ "xx_assign_expr ::= DOUBLE",
 /* 125 */ "xx_assign_expr ::= NULL",
 /* 126 */ "xx_assign_expr ::= FALSE",
 /* 127 */ "xx_assign_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 128 */ "xx_assign_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 129 */ "xx_assign_expr ::= NEW IDENTIFIER",
 /* 130 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 131 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 132 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 133 */ "xx_call_parameters ::= xx_call_parameter",
 /* 134 */ "xx_call_parameter ::= xx_literal_expr",
 /* 135 */ "xx_call_parameter ::= IDENTIFIER",
 /* 136 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 137 */ "xx_array_list ::= xx_array_item",
 /* 138 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 139 */ "xx_array_item ::= xx_array_value",
 /* 140 */ "xx_array_key ::= STRING",
 /* 141 */ "xx_array_key ::= INTEGER",
 /* 142 */ "xx_array_value ::= xx_literal_expr",
 /* 143 */ "xx_array_value ::= IDENTIFIER",
 /* 144 */ "xx_assign_expr ::= TRUE",
 /* 145 */ "xx_literal_expr ::= INTEGER",
 /* 146 */ "xx_literal_expr ::= STRING",
 /* 147 */ "xx_literal_expr ::= DOUBLE",
 /* 148 */ "xx_literal_expr ::= NULL",
 /* 149 */ "xx_literal_expr ::= FALSE",
 /* 150 */ "xx_literal_expr ::= TRUE",
 /* 151 */ "xx_eval_expr ::= xx_literal_expr",
 /* 152 */ "xx_eval_expr ::= xx_eval_expr EQUALS xx_eval_expr",
 /* 153 */ "xx_eval_expr ::= xx_eval_expr IDENTICAL xx_eval_expr",
 /* 154 */ "xx_eval_expr ::= xx_eval_expr LESS xx_eval_expr",
 /* 155 */ "xx_eval_expr ::= xx_eval_expr GREATER xx_eval_expr",
 /* 156 */ "xx_eval_expr ::= PARENTHESES_OPEN xx_eval_expr PARENTHESES_CLOSE",
 /* 157 */ "xx_eval_expr ::= IDENTIFIER",
 /* 158 */ "xx_comment ::= COMMENT",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *xx_TokenName(int tokenType){
#ifndef NDEBUG
  if( tokenType>0 && tokenType<(sizeof(yyTokenName)/sizeof(yyTokenName[0])) ){
    return yyTokenName[tokenType];
  }else{
    return "Unknown";
  }
#else
  return "";
#endif
}

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to xx_ and xx_Free.
*/
void *xx_Alloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(YYCODETYPE yymajor, YYMINORTYPE *yypminor){
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 44:
    case 45:
    case 46:
    case 47:
    case 48:
    case 49:
    case 50:
    case 51:
    case 52:
    case 53:
// 587 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1381 "parser.c"
      break;
    case 56:
// 600 "parser.lemon"
{ json_object_put((yypminor->yy115)); }
// 1386 "parser.c"
      break;
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor( yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from xx_Alloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void xx_Free(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
  (*freeProc)((void*)pParser);
}

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  /* if( pParser->yyidx<0 ) return YY_NO_ACTION;  */
  i = yy_shift_ofst[stateno];
  if( i==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
    int iFallback;            /* Fallback token */
    if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
           && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
           yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
      }
#endif
      return yy_find_shift_action(pParser, iFallback);
    }
#endif
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  yyParser *pParser,        /* The parser */
  int iLookAhead            /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  i = yy_reduce_ofst[stateno];
  if( i==YY_REDUCE_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer ot the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
  if( yypParser->yyidx>=YYSTACKDEPTH ){
     xx_ARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     xx_ARG_STORE; /* Suppress warning about unused %extra_argument var */
     return;
  }
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 55, 1 },
  { 56, 1 },
  { 57, 2 },
  { 57, 1 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 59, 3 },
  { 60, 5 },
  { 60, 7 },
  { 60, 6 },
  { 60, 8 },
  { 60, 4 },
  { 60, 6 },
  { 60, 5 },
  { 60, 7 },
  { 62, 1 },
  { 62, 1 },
  { 62, 1 },
  { 62, 2 },
  { 62, 2 },
  { 62, 2 },
  { 62, 2 },
  { 62, 3 },
  { 62, 3 },
  { 63, 2 },
  { 63, 1 },
  { 66, 4 },
  { 66, 3 },
  { 66, 5 },
  { 66, 6 },
  { 66, 3 },
  { 66, 4 },
  { 66, 5 },
  { 66, 6 },
  { 64, 2 },
  { 64, 1 },
  { 65, 2 },
  { 68, 6 },
  { 68, 5 },
  { 65, 1 },
  { 69, 7 },
  { 69, 8 },
  { 69, 8 },
  { 69, 9 },
  { 69, 8 },
  { 69, 9 },
  { 69, 9 },
  { 69, 10 },
  { 69, 7 },
  { 69, 8 },
  { 69, 8 },
  { 69, 9 },
  { 70, 3 },
  { 70, 1 },
  { 72, 1 },
  { 72, 2 },
  { 72, 2 },
  { 72, 3 },
  { 72, 4 },
  { 72, 4 },
  { 74, 3 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 73, 1 },
  { 71, 2 },
  { 71, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 82, 2 },
  { 83, 2 },
  { 77, 4 },
  { 77, 7 },
  { 77, 5 },
  { 77, 9 },
  { 77, 8 },
  { 77, 8 },
  { 78, 3 },
  { 78, 4 },
  { 84, 4 },
  { 84, 5 },
  { 76, 3 },
  { 86, 3 },
  { 86, 1 },
  { 87, 3 },
  { 87, 5 },
  { 87, 5 },
  { 87, 6 },
  { 87, 2 },
  { 87, 2 },
  { 79, 3 },
  { 89, 3 },
  { 89, 1 },
  { 90, 1 },
  { 90, 1 },
  { 80, 3 },
  { 80, 3 },
  { 81, 3 },
  { 81, 3 },
  { 81, 3 },
  { 81, 3 },
  { 81, 3 },
  { 91, 3 },
  { 91, 1 },
  { 92, 1 },
  { 92, 3 },
  { 88, 4 },
  { 88, 3 },
  { 88, 3 },
  { 88, 2 },
  { 88, 2 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 1 },
  { 88, 2 },
  { 88, 3 },
  { 88, 2 },
  { 88, 4 },
  { 88, 5 },
  { 94, 3 },
  { 94, 1 },
  { 95, 1 },
  { 95, 1 },
  { 93, 3 },
  { 93, 1 },
  { 96, 3 },
  { 96, 1 },
  { 97, 1 },
  { 97, 1 },
  { 98, 1 },
  { 98, 1 },
  { 88, 1 },
  { 67, 1 },
  { 67, 1 },
  { 67, 1 },
  { 67, 1 },
  { 67, 1 },
  { 67, 1 },
  { 85, 1 },
  { 85, 3 },
  { 85, 3 },
  { 85, 3 },
  { 85, 3 },
  { 85, 3 },
  { 85, 1 },
  { 61, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  xx_ARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<sizeof(yyRuleName)/sizeof(yyRuleName[0]) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  // <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  // <lineno> <thisfile>
  **     break;
  */
      case 0:
// 596 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy115;
}
// 1762 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 69:
      case 70:
      case 71:
      case 72:
      case 73:
      case 74:
      case 75:
      case 76:
      case 77:
      case 78:
      case 134:
      case 142:
      case 151:
// 602 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;
}
// 1785 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 606 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-1].minor.yy115, yymsp[0].minor.yy115);
}
// 1796 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 68:
      case 93:
      case 102:
      case 113:
      case 133:
      case 137:
// 610 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(NULL, yymsp[0].minor.yy115);
}
// 1813 "parser.c"
        break;
      case 7:
// 626 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 1822 "parser.c"
        break;
      case 8:
// 630 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, 0, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 1832 "parser.c"
        break;
      case 9:
// 634 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy115, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 1843 "parser.c"
        break;
      case 10:
// 638 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, 1, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 1854 "parser.c"
        break;
      case 11:
// 642 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy115, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-7].minor);
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 1866 "parser.c"
        break;
      case 12:
// 646 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 1876 "parser.c"
        break;
      case 13:
// 650 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(14,&yymsp[-5].minor);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 1887 "parser.c"
        break;
      case 14:
// 654 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 1898 "parser.c"
        break;
      case 15:
// 658 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(14,&yymsp[-5].minor);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 1910 "parser.c"
        break;
      case 16:
// 662 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
}
// 1917 "parser.c"
        break;
      case 17:
// 666 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 1924 "parser.c"
        break;
      case 18:
// 670 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 1931 "parser.c"
        break;
      case 19:
// 674 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 1938 "parser.c"
        break;
      case 20:
// 678 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 1945 "parser.c"
        break;
      case 21:
// 682 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
}
// 1952 "parser.c"
        break;
      case 22:
// 686 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 1959 "parser.c"
        break;
      case 23:
// 690 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 1966 "parser.c"
        break;
      case 24:
// 694 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, yymsp[-2].minor.yy115, status->scanner_state);
}
// 1973 "parser.c"
        break;
      case 27:
// 706 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 1982 "parser.c"
        break;
      case 28:
// 710 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 1991 "parser.c"
        break;
      case 29:
// 714 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2001 "parser.c"
        break;
      case 30:
// 718 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2011 "parser.c"
        break;
      case 31:
// 722 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2020 "parser.c"
        break;
      case 32:
// 726 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2029 "parser.c"
        break;
      case 33:
// 730 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2039 "parser.c"
        break;
      case 34:
// 734 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2049 "parser.c"
        break;
      case 38:
// 750 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2059 "parser.c"
        break;
      case 39:
// 754 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2069 "parser.c"
        break;
      case 41:
// 762 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2082 "parser.c"
        break;
      case 42:
// 766 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-7].minor);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2095 "parser.c"
        break;
      case 43:
// 770 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2108 "parser.c"
        break;
      case 44:
// 774 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-7].minor);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2121 "parser.c"
        break;
      case 45:
// 778 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-7].minor);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2134 "parser.c"
        break;
      case 46:
// 782 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-8].minor);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(25,&yymsp[-5].minor);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2147 "parser.c"
        break;
      case 47:
// 786 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-7].minor);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2160 "parser.c"
        break;
      case 48:
// 790 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-8].minor);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(25,&yymsp[-5].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2173 "parser.c"
        break;
      case 49:
// 794 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-6].minor);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2186 "parser.c"
        break;
      case 50:
// 798 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-6].minor);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2199 "parser.c"
        break;
      case 51:
// 802 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-7].minor);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2212 "parser.c"
        break;
      case 52:
// 806 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-7].minor);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2225 "parser.c"
        break;
      case 53:
      case 92:
      case 101:
      case 112:
      case 132:
      case 136:
// 810 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2238 "parser.c"
        break;
      case 55:
// 818 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2245 "parser.c"
        break;
      case 56:
// 822 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2252 "parser.c"
        break;
      case 57:
// 826 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-1].minor.yy115, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2259 "parser.c"
        break;
      case 58:
// 830 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 2267 "parser.c"
        break;
      case 59:
// 834 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-3].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 2275 "parser.c"
        break;
      case 60:
// 838 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 2283 "parser.c"
        break;
      case 61:
// 842 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2292 "parser.c"
        break;
      case 62:
// 846 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(27,&yymsp[0].minor);
}
// 2300 "parser.c"
        break;
      case 63:
// 850 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(28,&yymsp[0].minor);
}
// 2308 "parser.c"
        break;
      case 64:
// 854 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(29,&yymsp[0].minor);
}
// 2316 "parser.c"
        break;
      case 65:
// 858 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(30,&yymsp[0].minor);
}
// 2324 "parser.c"
        break;
      case 66:
// 862 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(31,&yymsp[0].minor);
}
// 2332 "parser.c"
        break;
      case 79:
// 914 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2341 "parser.c"
        break;
      case 80:
// 918 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2350 "parser.c"
        break;
      case 81:
// 922 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-2].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2360 "parser.c"
        break;
      case 82:
// 926 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-5].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(16,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2373 "parser.c"
        break;
      case 83:
// 930 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2383 "parser.c"
        break;
      case 84:
// 934 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-7].minor.yy115, yymsp[-5].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(34,&yymsp[-8].minor);
  yy_destructor(15,&yymsp[-6].minor);
  yy_destructor(16,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2396 "parser.c"
        break;
      case 85:
// 938 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, yymsp[-4].minor.yy115, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-7].minor);
  yy_destructor(15,&yymsp[-5].minor);
  yy_destructor(16,&yymsp[-3].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2409 "parser.c"
        break;
      case 86:
// 942 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(34,&yymsp[-7].minor);
  yy_destructor(15,&yymsp[-5].minor);
  yy_destructor(16,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2422 "parser.c"
        break;
      case 87:
// 946 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2432 "parser.c"
        break;
      case 88:
// 950 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2442 "parser.c"
        break;
      case 89:
// 954 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2452 "parser.c"
        break;
      case 90:
// 958 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2462 "parser.c"
        break;
      case 91:
// 962 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2471 "parser.c"
        break;
      case 94:
// 974 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 2479 "parser.c"
        break;
      case 95:
// 978 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
}
// 2488 "parser.c"
        break;
      case 96:
// 982 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
}
// 2498 "parser.c"
        break;
      case 97:
// 986 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
}
// 2508 "parser.c"
        break;
      case 98:
// 990 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 2516 "parser.c"
        break;
      case 99:
// 994 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[0].minor);
}
// 2524 "parser.c"
        break;
      case 100:
// 998 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_echo_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2533 "parser.c"
        break;
      case 103:
// 1010 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;;
}
// 2540 "parser.c"
        break;
      case 104:
      case 121:
      case 135:
      case 143:
      case 157:
// 1014 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 2551 "parser.c"
        break;
      case 105:
// 1018 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2560 "parser.c"
        break;
      case 106:
// 1022 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2569 "parser.c"
        break;
      case 107:
// 1026 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2578 "parser.c"
        break;
      case 108:
// 1030 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2587 "parser.c"
        break;
      case 109:
// 1034 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2596 "parser.c"
        break;
      case 110:
// 1038 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2605 "parser.c"
        break;
      case 111:
// 1042 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(13,&yymsp[0].minor);
}
// 2614 "parser.c"
        break;
      case 114:
// 1054 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2621 "parser.c"
        break;
      case 115:
// 1058 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 2629 "parser.c"
        break;
      case 116:
// 1063 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2638 "parser.c"
        break;
      case 117:
// 1068 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("add", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 2646 "parser.c"
        break;
      case 118:
// 1073 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("sub", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 2654 "parser.c"
        break;
      case 119:
// 1078 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("instanceof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 2662 "parser.c"
        break;
      case 120:
// 1083 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("typeof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 2670 "parser.c"
        break;
      case 122:
      case 141:
      case 145:
// 1093 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 2679 "parser.c"
        break;
      case 123:
      case 140:
      case 146:
// 1098 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 2688 "parser.c"
        break;
      case 124:
      case 147:
// 1103 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 2696 "parser.c"
        break;
      case 125:
      case 148:
// 1108 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[0].minor);
}
// 2705 "parser.c"
        break;
      case 126:
      case 149:
// 1113 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[0].minor);
}
// 2714 "parser.c"
        break;
      case 127:
// 1118 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2723 "parser.c"
        break;
      case 128:
// 1123 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(41,&yymsp[0].minor);
}
// 2732 "parser.c"
        break;
      case 129:
// 1128 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 2740 "parser.c"
        break;
      case 130:
// 1133 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2750 "parser.c"
        break;
      case 131:
// 1138 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2760 "parser.c"
        break;
      case 138:
// 1166 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 2768 "parser.c"
        break;
      case 139:
// 1170 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 2775 "parser.c"
        break;
      case 144:
      case 150:
// 1190 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[0].minor);
}
// 2784 "parser.c"
        break;
      case 152:
// 1222 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 2792 "parser.c"
        break;
      case 153:
// 1226 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 2800 "parser.c"
        break;
      case 154:
// 1230 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 2808 "parser.c"
        break;
      case 155:
// 1234 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2816 "parser.c"
        break;
      case 156:
// 1238 "parser.lemon"
{
	yygotominor.yy115 = yymsp[-1].minor.yy115;
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 2825 "parser.c"
        break;
      case 158:
// 1246 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 2832 "parser.c"
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yypParser,yygoto);
  if( yyact < YYNSTATE ){
    yy_shift(yypParser,yyact,yygoto,&yygotominor);
  }else if( yyact == YYNSTATE + YYNRULE + 1 ){
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  xx_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  xx_ARG_FETCH;
#define TOKEN (yyminor.yy0)
// 553 "parser.lemon"


	//fprintf(stderr, "error!\n");

	json_object *syntax_error = json_object_new_object();

	json_object_object_add(syntax_error, "type", json_object_new_string("error"));

	/*if (status->scanner_state->start_length) {
		fprintf(stderr, "Syntax error, %s", status->scanner_state->start);
	} else {
		fprintf(stderr, "EOF");
	}*/

	//status->syntax_error_len = 48 + Z_STRLEN_P(status->scanner_state->active_file);
	//status->syntax_error = emalloc(sizeof(char) * status->syntax_error_len);

	if (status->scanner_state->start_length) {
		json_object_object_add(syntax_error, "message", json_object_new_string("Syntax error"));
	} else {
		json_object_object_add(syntax_error, "message", json_object_new_string("Unexpected EOF"));
	}

	json_object_object_add(syntax_error, "file", json_object_new_string(status->scanner_state->active_file));
	json_object_object_add(syntax_error, "line", json_object_new_int(status->scanner_state->active_line));
	json_object_object_add(syntax_error, "char", json_object_new_int(status->scanner_state->active_char));

	status->status = XX_PARSING_FAILED;

	status->ret = syntax_error;

	//status->scanner_state->active_file

// 2908 "parser.c"
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  xx_ARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  xx_ARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "xx_Alloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void xx_(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  xx_TOKENTYPE yyminor       /* The value for the token */
  xx_ARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
    if( yymajor==0 ) return;
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  xx_ARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,yymajor);
    if( yyact<YYNSTATE ){
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      if( yyendofinput && yypParser->yyidx>=0 ){
        yymajor = 0;
      }else{
        yymajor = YYNOCODE;
      }
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else if( yyact == YY_ERROR_ACTION ){
      int yymx;
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_shift_action(yypParser,YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }else{
      yy_accept(yypParser);
      yymajor = YYNOCODE;
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}

#define SUCCESS 1
#define FAILURE 0

const xx_token_names xx_tokens[] =
{
	{ XX_T_INTEGER,       		"INTEGER" },
	{ XX_T_DOUBLE,        		"DOUBLE" },
	{ XX_T_STRING,        		"STRING" },
	{ XX_T_IDENTIFIER,    		"IDENTIFIER" },
	{ XX_T_AT,	   	    		"@" },
	{ XX_T_COMMA,	   	    	"," },
	{ XX_T_ASSIGN,       		"=" },
	{ XX_T_COLON,            	":" },
	{ XX_T_PARENTHESES_OPEN,  	"(" },
	{ XX_T_PARENTHESES_CLOSE, 	")" },
	{ XX_T_BRACKET_OPEN,     	"{" },
	{ XX_T_BRACKET_CLOSE,    	"}" },
 	{ XX_T_SBRACKET_OPEN,    	"[" },
	{ XX_T_SBRACKET_CLOSE,   	"]" },
	{  0, NULL }
};

/**
 * Wrapper to alloc memory within the parser
 */
static void *xx_wrapper_alloc(size_t bytes){
	return malloc(bytes);
}

/**
 * Wrapper to free memory within the parser
 */
static void xx_wrapper_free(void *pointer){
	//free(pointer);
}

/**
 * Creates a parser_token to be passed to the parser
 */
static void xx_parse_with_token(void* xx_parser, int opcode, int parsercode, xx_scanner_token *token, xx_parser_status *parser_status){

	xx_parser_token *pToken;

	pToken = malloc(sizeof(xx_parser_token));
	pToken->opcode = opcode;
	pToken->token = token->value;
	pToken->token_len = token->len;
	pToken->free_flag = 1;

	xx_(xx_parser, parsercode, pToken, parser_status);

	token->value = NULL;
	token->len = 0;
}

/**
 * Creates an error message when it's triggered by the scanner
 */
static void xx_scanner_error_msg(xx_parser_status *parser_status){

	/*char *error, *error_part;
	XX_scanner_state *state = parser_status->scanner_state;

	//PHALCON_INIT_VAR(*error_msg);
	ALLOC_INIT_ZVAL(*error_msg);
	if (state->start) {
		error = emalloc(sizeof(char) * (128 + state->start_length +  Z_STRLEN_P(state->active_file)));
		if (state->start_length > 16) {
			error_part = estrndup(state->start, 16);
			sprintf(error, "Parsing error before '%s...' in %s on line %d", error_part, Z_STRVAL_P(state->active_file), state->active_line);
			efree(error_part);
		} else {
			sprintf(error, "Parsing error before '%s' in %s on line %d", state->start, Z_STRVAL_P(state->active_file), state->active_line);
		}
		ZVAL_STRING(*error_msg, error, 1);
	} else {
		error = emalloc(sizeof(char) * (64 + Z_STRLEN_P(state->active_file)));
		sprintf(error, "Parsing error near to EOF in %s", Z_STRVAL_P(state->active_file));
		ZVAL_STRING(*error_msg, error, 1);
	}
	efree(error);*/
}

/**
 * Parses a comment returning an intermediate array representation
 */
int xx_parse_program(char *program, unsigned int program_length, char *file_path) {

	char *error;
	xx_scanner_state *state;
	xx_scanner_token token;
	int scanner_status, status = SUCCESS, start_lines;
	xx_parser_status *parser_status = NULL;
	void* xx_parser;

	/**
	 * Check if the program has any length
	 */
	if (!program_length) {
		return FAILURE;
	}

	if (program_length < 2) {
		return SUCCESS;
	}

	/**
	 * Start the reentrant parser
	 */
	xx_parser = xx_Alloc(xx_wrapper_alloc);

	parser_status = malloc(sizeof(xx_parser_status));
	state = malloc(sizeof(xx_scanner_state));

	parser_status->status = XX_PARSING_OK;
	parser_status->scanner_state = state;
	parser_status->ret = NULL;
	parser_status->token = &token;
	parser_status->syntax_error = NULL;

	/**
	 * Initialize the scanner state
	 */
	state->active_token = 0;
	state->start = program;
	state->start_length = 0;
	state->active_file = file_path;
	state->active_line = 1;
	state->active_char = 1;

	state->end = state->start;

	while (0 <= (scanner_status = xx_get_token(state, &token))) {

		state->active_token = token.opcode;

		state->start_length = (program + program_length - state->start);

		switch (token.opcode) {

			case XX_T_IGNORE:
				break;

			case XX_T_NAMESPACE:
				xx_(xx_parser, XX_NAMESPACE, NULL, parser_status);
				break;
			case XX_T_ABSTRACT:
				xx_(xx_parser, XX_ABSTRACT, NULL, parser_status);
				break;
			case XX_T_CLASS:
				xx_(xx_parser, XX_CLASS, NULL, parser_status);
				break;
			case XX_T_PUBLIC:
				xx_(xx_parser, XX_PUBLIC, NULL, parser_status);
				break;
			case XX_T_PROTECTED:
				xx_(xx_parser, XX_PROTECTED, NULL, parser_status);
				break;
			case XX_T_FUNCTION:
				xx_(xx_parser, XX_FUNCTION, NULL, parser_status);
				break;
			case XX_T_LET:
				xx_(xx_parser, XX_LET, NULL, parser_status);
				break;
			case XX_T_ECHO:
				xx_(xx_parser, XX_ECHO, NULL, parser_status);
				break;
			case XX_T_RETURN:
				xx_(xx_parser, XX_RETURN, NULL, parser_status);
				break;
			case XX_T_IF:
				xx_(xx_parser, XX_IF, NULL, parser_status);
				break;
			case XX_T_ELSE:
				xx_(xx_parser, XX_ELSE, NULL, parser_status);
				break;
			case XX_T_LOOP:
				xx_(xx_parser, XX_LOOP, NULL, parser_status);
				break;
			case XX_T_CONTINUE:
				xx_(xx_parser, XX_CONTINUE, NULL, parser_status);
				break;
			case XX_T_BREAK:
				xx_(xx_parser, XX_BREAK, NULL, parser_status);
				break;
			case XX_T_WHILE:
				xx_(xx_parser, XX_WHILE, NULL, parser_status);
				break;
			case XX_T_NEW:
				xx_(xx_parser, XX_NEW, NULL, parser_status);
				break;
			case XX_T_CONST:
				xx_(xx_parser, XX_CONST, NULL, parser_status);
				break;
			case XX_T_TYPEOF:
				xx_(xx_parser, XX_TYPEOF, NULL, parser_status);
				break;
			case XX_T_INSTANCEOF:
				xx_(xx_parser, XX_INSTANCEOF, NULL, parser_status);
				break;

			case XX_T_DOTCOMMA:
				xx_(xx_parser, XX_DOTCOMMA, NULL, parser_status);
				break;
			case XX_T_COMMA:
				xx_(xx_parser, XX_COMMA, NULL, parser_status);
				break;
			case XX_T_ASSIGN:
				xx_(xx_parser, XX_ASSIGN, NULL, parser_status);
				break;
			case XX_T_EQUALS:
				xx_(xx_parser, XX_EQUALS, NULL, parser_status);
				break;
			case XX_T_IDENTICAL:
				xx_(xx_parser, XX_IDENTICAL, NULL, parser_status);
				break;
			case XX_T_LESS:
				xx_(xx_parser, XX_LESS, NULL, parser_status);
				break;
			case XX_T_GREATER:
				xx_(xx_parser, XX_GREATER, NULL, parser_status);
				break;
			case XX_T_COLON:
				xx_(xx_parser, XX_COLON, NULL, parser_status);
				break;
			case XX_T_ARROW:
				xx_(xx_parser, XX_ARROW, NULL, parser_status);
				break;

			case XX_T_PARENTHESES_OPEN:
				xx_(xx_parser, XX_PARENTHESES_OPEN, NULL, parser_status);
				break;
			case XX_T_PARENTHESES_CLOSE:
				xx_(xx_parser, XX_PARENTHESES_CLOSE, NULL, parser_status);
				break;

			case XX_T_BRACKET_OPEN:
				xx_(xx_parser, XX_BRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_BRACKET_CLOSE:
				xx_(xx_parser, XX_BRACKET_CLOSE, NULL, parser_status);
				break;

			case XX_T_SBRACKET_OPEN:
				xx_(xx_parser, XX_SBRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_SBRACKET_CLOSE:
				xx_(xx_parser, XX_SBRACKET_CLOSE, NULL, parser_status);
				break;

			case XX_T_NULL:
				xx_(xx_parser, XX_NULL, NULL, parser_status);
				break;
			case XX_T_TRUE:
				xx_(xx_parser, XX_TRUE, NULL, parser_status);
				break;
			case XX_T_FALSE:
				xx_(xx_parser, XX_FALSE, NULL, parser_status);
				break;
			case XX_T_COMMENT:
				xx_parse_with_token(xx_parser, XX_T_COMMENT, XX_COMMENT, &token, parser_status);
				break;

			case XX_T_TYPE_INTEGER:
				xx_(xx_parser, XX_TYPE_INTEGER, NULL, parser_status);
				break;
			case XX_T_TYPE_DOUBLE:
				xx_(xx_parser, XX_TYPE_DOUBLE, NULL, parser_status);
				break;
			case XX_T_TYPE_STRING:
				xx_(xx_parser, XX_TYPE_STRING, NULL, parser_status);
				break;
			case XX_T_TYPE_BOOL:
				xx_(xx_parser, XX_TYPE_BOOL, NULL, parser_status);
				break;
			case XX_T_TYPE_VAR:
				xx_(xx_parser, XX_TYPE_VAR, NULL, parser_status);
				break;

			case XX_T_ADD:
				xx_(xx_parser, XX_ADD, NULL, parser_status);
				break;
			case XX_T_SUB:
				xx_(xx_parser, XX_SUB, NULL, parser_status);
				break;
			case XX_T_INCR:
				xx_(xx_parser, XX_INCR, NULL, parser_status);
				break;
			case XX_T_DECR:
				xx_(xx_parser, XX_DECR, NULL, parser_status);
				break;

			case XX_T_INTEGER:
				xx_parse_with_token(xx_parser, XX_T_INTEGER, XX_INTEGER, &token, parser_status);
				break;
			case XX_T_DOUBLE:
				xx_parse_with_token(xx_parser, XX_T_DOUBLE, XX_DOUBLE, &token, parser_status);
				break;
			case XX_T_STRING:
				xx_parse_with_token(xx_parser, XX_T_STRING, XX_STRING, &token, parser_status);
				break;
			case XX_T_IDENTIFIER:
				xx_parse_with_token(xx_parser, XX_T_IDENTIFIER, XX_IDENTIFIER, &token, parser_status);
				break;

			default:
				parser_status->status = XX_PARSING_FAILED;
				fprintf(stderr, "Scanner: unknown opcode %d\n", token.opcode);
				/*if (!*error_msg) {
					error = emalloc(sizeof(char) * (48 + Z_STRLEN_P(state->active_file)));
					sprintf(error, "Scanner: unknown opcode %d on in %s line %d", token.opcode, Z_STRVAL_P(state->active_file), state->active_line);
					//PHALCON_INIT_VAR(*error_msg);
					ALLOC_INIT_ZVAL(*error_msg);
					ZVAL_STRING(*error_msg, error, 1);
					efree(error);
				}*/
				break;
		}

		if (parser_status->status != XX_PARSING_OK) {
			status = FAILURE;
			break;
		}

		state->end = state->start;
	}

	if (status != FAILURE) {
		switch (scanner_status) {
			case XX_SCANNER_RETCODE_ERR:
			case XX_SCANNER_RETCODE_IMPOSSIBLE:
				{
					json_object *syntax_error = json_object_new_object();
					json_object_object_add(syntax_error, "type", json_object_new_string("error"));
					json_object_object_add(syntax_error, "message", json_object_new_string("Scanner error"));
					parser_status->ret = syntax_error;
					status = FAILURE;
				}
				break;
			default:
				xx_(xx_parser, 0, NULL, parser_status);
		}
	}

	state->active_token = 0;
	state->start = NULL;

	if (parser_status->status != XX_PARSING_OK) {
		status = FAILURE;
		/*if (parser_status->syntax_error) {
			if (!*error_msg) {
				//PHALCON_INIT_VAR(*error_msg);
				ALLOC_INIT_ZVAL(*error_msg);
				ZVAL_STRING(*error_msg, parser_status->syntax_error, 1);
			}
			efree(parser_status->syntax_error);
		}*/
		//fprintf(stderr, "error!\n");
	}

	xx_Free(xx_parser, xx_wrapper_free);

	if (status != FAILURE) {
		if (parser_status->status == XX_PARSING_OK) {
			//printf("%s\n", json_object_to_json_string(parser_status->ret));
			/*if (parser_status->ret) {
				ZVAL_ZVAL(*result, parser_status->ret, 0, 0);
				ZVAL_NULL(parser_status->ret);
				zval_ptr_dtor(&parser_status->ret);
			} else {
				array_init(*result);
			}*/
		}
	}

	if (parser_status->ret) {
		printf("%s\n", json_object_to_json_string(parser_status->ret));
	}

	//efree(Z_STRVAL(processed_comment));*/

	free(parser_status);
	free(state);

	return status;
}

int main(int argc, char **argv) {

	FILE *fp;
	char ch;
	char *program;
	int i, length;

	if (argc > 0) {

		fp = fopen(argv[1], "r");
		if (!fp) {
			fprintf(stderr, "Cant open file\n");
			exit(1);
		}

		length = 1024;
		program = malloc(sizeof(char) * length);

		i = 0;
		while (!feof(fp)) {
			ch = fgetc(fp);
			if (i == length) {
				length += 1024;
				program = realloc(program, sizeof(char) * length);
			}
			program[i++] = ch;
		}
		program[i - 1] = '\0';
		fclose(fp);

		xx_parse_program(program, i - 1, argv[1]);

		free(program);
	}

}
