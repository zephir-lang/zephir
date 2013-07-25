/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 13 "parser.lemon"


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
	json_object_object_add(ret, "expr", expr);

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

static json_object *xx_ret_echo_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("echo"));
	json_object_object_add(ret, "expr", expr);

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


// 538 "parser.c"
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
#define YYNOCODE 94
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy175;
  int yy187;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 305
#define YYNRULE 148
#define YYERRORSYMBOL 50
#define YYERRSYMDT yy187
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
 /*     0 */   154,   89,  371,  152,  454,    1,    2,  304,    4,    5,
 /*    10 */     6,   62,  120,  131,  140,  146,  143,   20,   39,   41,
 /*    20 */    43,   40,   52,   56,   61,  267,  155,  158,  121,  126,
 /*    30 */   149,  215,   80,  268,  152,   45,   98,   97,   92,   93,
 /*    40 */    94,  294,   95,  298,  131,  140,  146,  143,   64,   39,
 /*    50 */    41,   43,  305,   52,   56,   61,   63,  117,  106,  121,
 /*    60 */   126,  149,  233,  155,  158,  152,  301,   16,  278,  284,
 /*    70 */   283,  370,  277,  250,  176,  131,  140,  146,  143,  105,
 /*    80 */    39,   41,   43,  162,   52,   56,   61,  141,  139,  176,
 /*    90 */   121,  126,  149,  170,  108,  213,  152,  295,   16,  278,
 /*   100 */   284,  283,  266,  277,  250,  269,  131,  140,  146,  143,
 /*   110 */   199,   39,   41,   43,   20,   52,   56,   61,  156,  104,
 /*   120 */   110,  121,  126,  149,  171,  129,   21,  152,  288,   16,
 /*   130 */   278,  284,  283,  254,  277,  250,   57,  131,  140,  146,
 /*   140 */   143,  369,   39,   41,   43,  204,   52,   56,   61,  255,
 /*   150 */   155,  158,  121,  126,  149,  235,  108,  256,  152,   58,
 /*   160 */    90,   91,   92,   93,   94,   12,   95,  285,  131,  140,
 /*   170 */   146,  143,  270,   39,   41,   43,  260,   52,   56,   61,
 /*   180 */   147,  139,  107,  121,  126,  149,  211,   89,  219,  152,
 /*   190 */    13,   16,  278,  284,  283,  272,  277,  250,   72,  131,
 /*   200 */   140,  146,  143,  134,   39,   41,   43,  228,   52,   56,
 /*   210 */    61,  148,   83,  273,  121,  126,  149,  209,   89,  134,
 /*   220 */   152,  274,   98,   97,   92,   93,   94,  142,   95,  368,
 /*   230 */   131,  140,  146,  143,  156,   39,   41,   43,  367,   52,
 /*   240 */    56,   61,   69,   71,  115,  121,  126,  149,  165,  124,
 /*   250 */    82,  152,  157,   90,   91,   92,   93,   94,  253,   95,
 /*   260 */   259,  131,  140,  146,  143,  176,   39,   41,   43,  275,
 /*   270 */    52,   56,   61,  132,  139,  173,  121,  126,  149,   51,
 /*   280 */   109,  134,  152,  261,   90,   91,   92,   93,   94,  151,
 /*   290 */    95,  262,  131,  140,  146,  143,  156,   39,   41,   43,
 /*   300 */   276,   52,   56,   61,  150,  139,   88,  121,  126,  149,
 /*   310 */   164,  144,  139,  152,   44,   90,   91,   92,   93,   94,
 /*   320 */   203,   95,  218,  131,  140,  146,  143,  366,   39,   41,
 /*   330 */    43,  436,   52,   56,   61,   87,  191,  114,  121,  126,
 /*   340 */   149,   27,   65,  190,  152,   90,   91,   92,   93,   94,
 /*   350 */   253,   95,  259,  240,  131,  140,  146,  143,  134,   39,
 /*   360 */    41,   43,   18,   52,   56,   61,  133,  179,    9,  121,
 /*   370 */   126,  149,   55,   15,  437,  152,  252,  265,  286,  271,
 /*   380 */   245,    3,    4,    5,    6,  131,  140,  146,  143,  134,
 /*   390 */    39,   41,   43,   22,   52,   56,   61,  145,  287,  188,
 /*   400 */   121,  126,  149,   60,  303,  289,  152,  252,  265,  328,
 /*   410 */   271,  245,  202,   19,  101,  227,  131,  140,  146,  143,
 /*   420 */    66,   39,   41,   43,   67,   52,   56,   61,  156,  113,
 /*   430 */   102,  121,  126,  149,  166,  290,   14,  152,  252,  265,
 /*   440 */   292,  271,  245,   88,  112,  116,  159,  131,  140,  146,
 /*   450 */   143,  167,   39,   41,   43,  293,   52,   56,   61,  306,
 /*   460 */   187,  168,  121,  126,  149,   47,    7,   81,  152,   10,
 /*   470 */    99,   85,   96,  291,  152,  186,  296,  184,  131,  140,
 /*   480 */   146,  143,   70,   39,   41,   43,  183,   52,   56,   61,
 /*   490 */   299,  182,   68,  121,  126,  149,  175,  325,  300,  152,
 /*   500 */   239,   19,  324,  227,  245,  202,   19,  180,  227,  131,
 /*   510 */   140,  146,  143,  222,   39,   41,   43,  223,   52,   56,
 /*   520 */    61,  130,  221,  219,  121,  126,  149,  224,  321,  127,
 /*   530 */   152,  252,  265,  326,  271,  245,  281,  265,  128,  271,
 /*   540 */   131,  140,  146,  143,  220,   39,   41,   43,  136,   52,
 /*   550 */    56,   61,  156,  228,  231,  121,  126,  149,  226,  229,
 /*   560 */   230,  152,  236,   17,  251,  237,  277,  250,  238,  250,
 /*   570 */   161,  131,  140,  146,  143,   48,   39,   41,   43,  232,
 /*   580 */    52,   56,   61,  135,  214,   49,  121,  126,  149,  217,
 /*   590 */   297,  125,  152,  252,  265,   11,  271,  245,  137,  177,
 /*   600 */   181,  185,  131,  140,  146,  143,  208,   39,   41,   43,
 /*   610 */   123,   52,   56,   61,  302,  206,  138,  121,  126,  149,
 /*   620 */    25,  322,  242,  152,  252,  265,  329,  271,  245,  202,
 /*   630 */    19,  205,  227,  131,  140,  146,  143,  241,   39,   41,
 /*   640 */    43,  244,   52,   56,   61,   86,   88,  122,  121,  126,
 /*   650 */   149,  152,  279,  243,  282,  283,  246,  238,  250,  247,
 /*   660 */   204,  131,  140,  146,  143,   42,   39,   41,   43,   38,
 /*   670 */    52,   56,   61,   84,   85,   96,  121,  126,  149,   59,
 /*   680 */   248,  178,  201,  153,   29,   30,   31,   32,   33,   34,
 /*   690 */    35,   36,   37,  249,   38,  207,  189,    8,  192,  193,
 /*   700 */   194,  195,  196,  178,   50,  119,   24,  257,  153,   29,
 /*   710 */    30,   31,   32,   33,   34,   35,   36,   37,  189,   38,
 /*   720 */   192,  193,  194,  195,  196,  203,  258,  218,  240,  225,
 /*   730 */   178,   53,  200,  153,   29,   30,   31,   32,   33,   34,
 /*   740 */    35,   36,   37,   38,   23,  189,  118,  192,  193,  194,
 /*   750 */   195,  196,  264,  210,  178,  263,  243,  153,   29,   30,
 /*   760 */    31,   32,   33,   34,   35,   36,   37,   38,  243,  189,
 /*   770 */   243,  192,  193,  194,  195,  196,  243,  174,  243,  243,
 /*   780 */   243,  153,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   790 */    37,   38,    7,  243,  243,   10,  243,  243,  243,  291,
 /*   800 */   152,  169,  243,  243,  243,  153,   29,   30,   31,   32,
 /*   810 */    33,   34,   35,   36,   37,   38,  172,  243,  197,  181,
 /*   820 */   185,  243,  280,  251,  243,  234,  250,  243,  243,  153,
 /*   830 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   840 */   212,  243,  197,  181,  185,  243,  243,  243,  243,   54,
 /*   850 */   243,  243,  243,  153,   29,   30,   31,   32,   33,   34,
 /*   860 */    35,   36,   37,   38,  243,  243,  243,  243,  327,  243,
 /*   870 */   243,  202,   19,  216,  227,  243,  243,  153,   29,   30,
 /*   880 */    31,   32,   33,   34,   35,   36,   37,   38,  243,  243,
 /*   890 */   243,  243,  323,  243,  243,  202,   19,   46,  227,  243,
 /*   900 */   243,  153,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   910 */    37,   38,  243,  243,  243,  243,  198,  243,  197,  181,
 /*   920 */   185,   26,  243,  243,  243,  153,   29,   30,   31,   32,
 /*   930 */    33,   34,   35,   36,   37,   38,  100,  243,   73,  243,
 /*   940 */   243,  243,  243,  243,  243,  109,  243,  243,  243,   28,
 /*   950 */    29,   30,   31,   32,   33,   34,   35,   36,   37,  103,
 /*   960 */   243,  243,  243,  243,  243,  163,  243,  243,   79,  243,
 /*   970 */   243,  243,  243,   74,   75,   76,   77,   78,  160,  111,
 /*   980 */    90,   91,   92,   93,   94,  243,   95,  243,  243,  243,
 /*   990 */   243,  243,  243,  243,  243,  243,  243,  243,  243,  243,
 /*  1000 */    90,   91,   92,   93,   94,  243,   95,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    12,    8,    8,   15,   51,   52,   53,   54,   55,   56,
 /*    10 */    57,   82,   83,   25,   26,   27,   28,   20,   30,   31,
 /*    20 */    32,    9,   34,   35,   36,    9,    2,    3,   40,   41,
 /*    30 */    42,   12,   39,   17,   15,   11,   43,   44,   45,   46,
 /*    40 */    47,   11,   49,   13,   25,   26,   27,   28,    1,   30,
 /*    50 */    31,   32,    0,   34,   35,   36,    9,   39,    1,   40,
 /*    60 */    41,   42,   12,    2,    3,   15,   58,   59,   60,   61,
 /*    70 */    62,    8,   64,   65,    1,   25,   26,   27,   28,   22,
 /*    80 */    30,   31,   32,   22,   34,   35,   36,   85,   86,    1,
 /*    90 */    40,   41,   42,   12,   63,   22,   15,   58,   59,   60,
 /*   100 */    61,   62,    8,   64,   65,   63,   25,   26,   27,   28,
 /*   110 */    22,   30,   31,   32,   20,   34,   35,   36,   63,   88,
 /*   120 */    89,   40,   41,   42,   12,    8,    8,   15,   58,   59,
 /*   130 */    60,   61,   62,    8,   64,   65,   81,   25,   26,   27,
 /*   140 */    28,    8,   30,   31,   32,   20,   34,   35,   36,    9,
 /*   150 */     2,    3,   40,   41,   42,   12,   63,   17,   15,   11,
 /*   160 */    43,   44,   45,   46,   47,   11,   49,   13,   25,   26,
 /*   170 */    27,   28,    9,   30,   31,   32,    8,   34,   35,   36,
 /*   180 */    85,   86,   89,   40,   41,   42,   12,    8,   20,   15,
 /*   190 */    58,   59,   60,   61,   62,    8,   64,   65,   84,   25,
 /*   200 */    26,   27,   28,    1,   30,   31,   32,   20,   34,   35,
 /*   210 */    36,    9,    1,    9,   40,   41,   42,   12,    8,    1,
 /*   220 */    15,   17,   43,   44,   45,   46,   47,    9,   49,    8,
 /*   230 */    25,   26,   27,   28,   63,   30,   31,   32,    8,   34,
 /*   240 */    35,   36,    4,    5,   84,   40,   41,   42,   12,    8,
 /*   250 */    39,   15,   81,   43,   44,   45,   46,   47,   16,   49,
 /*   260 */    18,   25,   26,   27,   28,    1,   30,   31,   32,   63,
 /*   270 */    34,   35,   36,   85,   86,   11,   40,   41,   42,   12,
 /*   280 */     8,    1,   15,    9,   43,   44,   45,   46,   47,    9,
 /*   290 */    49,   17,   25,   26,   27,   28,   63,   30,   31,   32,
 /*   300 */     9,   34,   35,   36,   85,   86,   63,   40,   41,   42,
 /*   310 */    12,   85,   86,   15,   81,   43,   44,   45,   46,   47,
 /*   320 */    16,   49,   18,   25,   26,   27,   28,    8,   30,   31,
 /*   330 */    32,   48,   34,   35,   36,   92,   24,   17,   40,   41,
 /*   340 */    42,   12,   83,    8,   15,   43,   44,   45,   46,   47,
 /*   350 */    16,   49,   18,   19,   25,   26,   27,   28,    1,   30,
 /*   360 */    31,   32,   65,   34,   35,   36,    9,   17,    9,   40,
 /*   370 */    41,   42,   12,   12,   48,   15,   15,   16,    8,   18,
 /*   380 */    19,   54,   55,   56,   57,   25,   26,   27,   28,    1,
 /*   390 */    30,   31,   32,   21,   34,   35,   36,    9,   11,   63,
 /*   400 */    40,   41,   42,   12,   12,   12,   15,   15,   16,   12,
 /*   410 */    18,   19,   15,   16,    8,   18,   25,   26,   27,   28,
 /*   420 */     8,   30,   31,   32,   17,   34,   35,   36,   63,    8,
 /*   430 */    21,   40,   41,   42,   12,   12,   12,   15,   15,   16,
 /*   440 */    10,   18,   19,   63,   37,   38,   81,   25,   26,   27,
 /*   450 */    28,   33,   30,   31,   32,    8,   34,   35,   36,    0,
 /*   460 */    17,   11,   40,   41,   42,   12,    7,   87,   15,   10,
 /*   470 */    90,   91,   92,   14,   15,    8,   12,   63,   25,   26,
 /*   480 */    27,   28,   84,   30,   31,   32,   17,   34,   35,   36,
 /*   490 */     8,    8,   84,   40,   41,   42,   12,   12,   11,   15,
 /*   500 */    15,   16,   12,   18,   19,   15,   16,   63,   18,   25,
 /*   510 */    26,   27,   28,   22,   30,   31,   32,   11,   34,   35,
 /*   520 */    36,    9,   21,   20,   40,   41,   42,   12,   12,   63,
 /*   530 */    15,   15,   16,   12,   18,   19,   15,   16,    9,   18,
 /*   540 */    25,   26,   27,   28,    8,   30,   31,   32,    8,   34,
 /*   550 */    35,   36,   63,   20,   22,   40,   41,   42,   12,    8,
 /*   560 */    21,   15,   60,   61,   62,   61,   64,   65,   64,   65,
 /*   570 */    81,   25,   26,   27,   28,   33,   30,   31,   32,   11,
 /*   580 */    34,   35,   36,   86,   11,   11,   40,   41,   42,   12,
 /*   590 */    12,    9,   15,   15,   16,    8,   18,   19,   17,   68,
 /*   600 */    69,   70,   25,   26,   27,   28,   11,   30,   31,   32,
 /*   610 */     9,   34,   35,   36,   12,   21,   63,   40,   41,   42,
 /*   620 */    12,   12,   17,   15,   15,   16,   12,   18,   19,   15,
 /*   630 */    16,    8,   18,   25,   26,   27,   28,    8,   30,   31,
 /*   640 */    32,    9,   34,   35,   36,   48,   63,   63,   40,   41,
 /*   650 */    42,   15,   59,   63,   61,   62,    8,   64,   65,   17,
 /*   660 */    20,   25,   26,   27,   28,    9,   30,   31,   32,   57,
 /*   670 */    34,   35,   36,   90,   91,   92,   40,   41,   42,   67,
 /*   680 */    63,    8,   12,   71,   72,   73,   74,   75,   76,   77,
 /*   690 */    78,   79,   80,    9,   57,   22,   23,    8,   25,   26,
 /*   700 */    27,   28,   29,    8,   67,   84,   11,   63,   71,   72,
 /*   710 */    73,   74,   75,   76,   77,   78,   79,   80,   23,   57,
 /*   720 */    25,   26,   27,   28,   29,   16,    9,   18,   19,   67,
 /*   730 */     8,   11,   11,   71,   72,   73,   74,   75,   76,   77,
 /*   740 */    78,   79,   80,   57,   22,   23,   17,   25,   26,   27,
 /*   750 */    28,   29,    9,   67,    8,   63,   93,   71,   72,   73,
 /*   760 */    74,   75,   76,   77,   78,   79,   80,   57,   93,   23,
 /*   770 */    93,   25,   26,   27,   28,   29,   93,   67,   93,   93,
 /*   780 */    93,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   790 */    80,   57,    7,   93,   93,   10,   93,   93,   93,   14,
 /*   800 */    15,   67,   93,   93,   93,   71,   72,   73,   74,   75,
 /*   810 */    76,   77,   78,   79,   80,   57,   66,   93,   68,   69,
 /*   820 */    70,   93,   61,   62,   93,   67,   65,   93,   93,   71,
 /*   830 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   57,
 /*   840 */    66,   93,   68,   69,   70,   93,   93,   93,   93,   67,
 /*   850 */    93,   93,   93,   71,   72,   73,   74,   75,   76,   77,
 /*   860 */    78,   79,   80,   57,   93,   93,   93,   93,   12,   93,
 /*   870 */    93,   15,   16,   67,   18,   93,   93,   71,   72,   73,
 /*   880 */    74,   75,   76,   77,   78,   79,   80,   57,   93,   93,
 /*   890 */    93,   93,   12,   93,   93,   15,   16,   67,   18,   93,
 /*   900 */    93,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   910 */    80,   57,   93,   93,   93,   93,   66,   93,   68,   69,
 /*   920 */    70,   67,   93,   93,   93,   71,   72,   73,   74,   75,
 /*   930 */    76,   77,   78,   79,   80,   57,    6,   93,    8,   93,
 /*   940 */    93,   93,   93,   93,   93,    8,   93,   93,   93,   71,
 /*   950 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   22,
 /*   960 */    93,   93,   93,   93,   93,    8,   93,   93,   38,   93,
 /*   970 */    93,   93,   93,   43,   44,   45,   46,   47,   21,   49,
 /*   980 */    43,   44,   45,   46,   47,   93,   49,   93,   93,   93,
 /*   990 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   93,
 /*  1000 */    43,   44,   45,   46,   47,   93,   49,
};
#define YY_SHIFT_USE_DFLT (-13)
static short yy_shift_ofst[] = {
 /*     0 */   785,   52,  459,  -13,  -13,  -13,  -13,  689,  359,  -13,
 /*    10 */   587,  154,  361,  424,  -13,  -13,  516,  490,  -13,   -3,
 /*    20 */   118,  372,  722,  695,  608,  -13,  329,  -13,  -13,  -13,
 /*    30 */   -13,  -13,  -13,  -13,  -13,  -13,  -13,  -13,  -13,   12,
 /*    40 */   -13,  656,  -13,  957,   24,  422,  453,  542,  574,  236,
 /*    50 */   267,  -13,  720,  298,  360,  -13,  957,  148,  -12,  391,
 /*    60 */   -13,  412,   47,  -13,  412,  -13,  407,  930,  238,  930,
 /*    70 */   -13,  930,  -13,  -13,  -13,  -13,  -13,  -13,  -13,   -7,
 /*    80 */   -13,  211,  -13,  179,  -13,  597,  210,  -13,  -13,  -13,
 /*    90 */   -13,  -13,  -13,  -13,  -13,  -13,  -13,  283,  326,  -13,
 /*   100 */   406,  409,  937,  -13,   57,  -13,  272,  -13,  -13,  -13,
 /*   110 */   -13,  -13,  421,  320,  930,  238,   18,  729,  930,  238,
 /*   120 */   -13,  241,  601,  -13,  582,  -13,  117,  529,  -13,  512,
 /*   130 */   -13,  540,  357,  -13,  540,  -13,  581,  302,  -13,  -13,
 /*   140 */   540,  218,  -13,  540,  388,  -13,  540,  202,  -13,  540,
 /*   150 */   280,  -13,  -13,  -13,  -13,  957,  -13,  -13,  957,  -13,
 /*   160 */   957,   61,  -13,  -13,  -13,  -13,  418,  450,  112,   81,
 /*   170 */   -13,  -13,  264,  636,  484,  -13,  746,  -13,  350,  302,
 /*   180 */   -13,  483,  469,  302,  -13,  467,  443,  302,  -13,  335,
 /*   190 */   312,  319,  230,  221,  133,   63,   -6,  -13,   88,  721,
 /*   200 */   670,  -13,  304,  640,  623,  594,  673,  595,  205,  -13,
 /*   210 */   174,  -13,   73,  573,   19,  -13,  577,  -13,  503,  536,
 /*   220 */   501,  491,  506,  515,  -13,  546,  -13,  533,  551,  539,
 /*   230 */   532,  568,   50,  -13,  143,  -13,  485,  397,  -13,  709,
 /*   240 */   629,  605,  302,  632,  -13,  648,  642,  302,  684,  -13,
 /*   250 */   -13,  -13,  334,  125,  140,  -13,  302,  717,  -13,  168,
 /*   260 */   274,  -13,  302,  743,  -13,   94,   16,  -13,  302,  163,
 /*   270 */   -13,  187,  204,  -13,  302,  291,  -13,  -13,  609,  521,
 /*   280 */   614,  242,  856,  -13,  880,  370,  387,  423,  393,  -13,
 /*   290 */   -13,  430,  447,   30,  578,  464,  -13,  -13,  482,  487,
 /*   300 */   392,  602,  -13,  -13,  -13,
};
#define YY_REDUCE_USE_DFLT (-72)
static short yy_reduce_ofst[] = {
 /*     0 */   -47,  -72,  327,  -72,  -72,  -72,  -72,  -72,  -72,  -72,
 /*    10 */   -72,  -72,  132,  -72,  -72,  -72,  502,  297,  -72,  -72,
 /*    20 */   -72,  -72,  850,  750,  854,  -72,  878,  -72,  -72,  -72,
 /*    30 */   -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,
 /*    40 */   -72,  -72,  -72,  233,  -72,  830,  878,  -72,  -72,  637,
 /*    50 */   878,  -72,  -72,  782,  878,  -72,   55,  -72,  612,  878,
 /*    60 */   -72,  -71,  -72,  -72,  259,  -72,  -72,  408,  -72,  398,
 /*    70 */   -72,  114,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  380,
 /*    80 */   -72,  -72,  -72,  583,  -72,  -72,  243,  -72,  -72,  -72,
 /*    90 */   -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,
 /*   100 */   -72,  -72,   31,  -72,  -72,  -72,   93,  -72,  -72,  -72,
 /*   110 */   -72,  -72,  -72,  -72,  160,  -72,  -72,  -72,  621,  -72,
 /*   120 */   -72,  584,  -72,  -72,  -72,  -72,  466,  -72,  -72,  -72,
 /*   130 */   -72,  188,  -72,  -72,  497,  -72,  -72,  553,  -72,  -72,
 /*   140 */     2,  -72,  -72,  226,  -72,  -72,   95,  -72,  -72,  219,
 /*   150 */   -72,  -72,  -72,  -72,  -72,  171,  -72,  -72,  365,  -72,
 /*   160 */   489,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  734,  878,
 /*   170 */   -72,  -72,  -72,  710,  878,  -72,  531,  -72,  -72,  444,
 /*   180 */   -72,  -72,  -72,  414,  -72,  -72,  -72,  336,  -72,  -72,
 /*   190 */   -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,
 /*   200 */   -72,  -72,  -72,  -72,  -72,  -72,  774,  -72,  686,  -72,
 /*   210 */   878,  -72,  -72,  -72,  806,  -72,  878,  -72,  -72,  -72,
 /*   220 */   -72,  -72,  -72,  662,  -72,  878,  -72,  -72,  -72,  -72,
 /*   230 */   -72,  -72,  758,  -72,  878,  -72,  504,  297,  -72,  -72,
 /*   240 */   -72,  -72,  590,  -72,  -72,  -72,  -72,  617,  -72,  -72,
 /*   250 */   -72,  -72,  -72,  -72,  -72,  -72,  644,  -72,  -72,  -72,
 /*   260 */   -72,  -72,  692,  -72,  -72,  -72,  -72,  -72,   42,  -72,
 /*   270 */   -72,  -72,  -72,  -72,  206,  -72,  -72,  -72,  593,  761,
 /*   280 */   297,  -72,  297,  -72,  297,  -72,  -72,   70,  -72,  -72,
 /*   290 */   -72,  -72,  -72,  -72,   39,  -72,  -72,  -72,  -72,  -72,
 /*   300 */     8,  -72,  -72,  -72,  -72,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   453,  453,  453,  307,  309,  310,  311,  453,  453,  312,
 /*    10 */   453,  453,  453,  453,  313,  317,  453,  453,  342,  453,
 /*    20 */   453,  453,  453,  453,  453,  346,  453,  350,  372,  374,
 /*    30 */   375,  376,  377,  378,  379,  380,  381,  382,  383,  453,
 /*    40 */   384,  453,  385,  453,  453,  453,  453,  388,  453,  453,
 /*    50 */   453,  389,  453,  453,  453,  393,  453,  453,  453,  453,
 /*    60 */   395,  453,  453,  396,  453,  397,  453,  453,  399,  453,
 /*    70 */   415,  453,  416,  417,  418,  419,  420,  421,  422,  453,
 /*    80 */   423,  453,  424,  453,  432,  453,  453,  434,  438,  439,
 /*    90 */   441,  442,  443,  444,  445,  446,  435,  442,  441,  433,
 /*   100 */   453,  425,  453,  426,  453,  427,  453,  428,  430,  431,
 /*   110 */   429,  440,  453,  453,  453,  400,  453,  453,  453,  401,
 /*   120 */   398,  453,  453,  402,  453,  403,  453,  453,  404,  453,
 /*   130 */   405,  453,  453,  406,  453,  411,  413,  453,  414,  412,
 /*   140 */   453,  453,  407,  453,  453,  408,  453,  453,  409,  453,
 /*   150 */   453,  410,  452,  373,  394,  453,  447,  448,  453,  449,
 /*   160 */   453,  453,  450,  451,  392,  390,  386,  453,  453,  453,
 /*   170 */   391,  387,  453,  453,  453,  351,  453,  358,  360,  453,
 /*   180 */   363,  453,  361,  453,  364,  453,  362,  453,  365,  453,
 /*   190 */   453,  453,  453,  453,  453,  453,  453,  359,  453,  453,
 /*   200 */   453,  347,  453,  453,  453,  453,  453,  453,  453,  348,
 /*   210 */   453,  352,  453,  453,  453,  349,  453,  353,  453,  453,
 /*   220 */   453,  453,  453,  453,  355,  453,  357,  453,  453,  453,
 /*   230 */   453,  453,  453,  354,  453,  356,  453,  453,  340,  453,
 /*   240 */   453,  453,  453,  453,  343,  453,  453,  453,  453,  344,
 /*   250 */   345,  330,  453,  453,  453,  332,  453,  453,  335,  453,
 /*   260 */   453,  337,  453,  453,  339,  453,  453,  333,  453,  453,
 /*   270 */   334,  453,  453,  336,  453,  453,  338,  341,  453,  453,
 /*   280 */   453,  453,  453,  331,  453,  453,  453,  453,  453,  314,
 /*   290 */   318,  453,  453,  453,  453,  453,  315,  319,  453,  453,
 /*   300 */   453,  453,  316,  320,  308,
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
  "ADD",           "SUB",           "NEW",           "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",
  "BRACKET_CLOSE",  "EXTENDS",       "ABSTRACT",      "COMMENT",     
  "PUBLIC",        "ASSIGN",        "PROTECTED",     "CONST",       
  "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",  "LESS",        
  "GREATER",       "TYPE_INTEGER",  "TYPE_DOUBLE",   "TYPE_BOOL",   
  "TYPE_STRING",   "TYPE_ARRAY",    "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "LOOP",          "WHILE",       
  "LET",           "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",
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
  "xx_assign_expr",  "xx_declare_variable_list",  "xx_declare_variable",  "xx_array_list",
  "xx_call_parameters",  "xx_call_parameter",  "xx_array_item",  "xx_array_key",
  "xx_array_value",
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
 /*  97 */ "xx_echo_statement ::= ECHO xx_literal_expr DOTCOMMA",
 /*  98 */ "xx_echo_statement ::= ECHO IDENTIFIER DOTCOMMA",
 /*  99 */ "xx_return_statement ::= RETURN xx_literal_expr DOTCOMMA",
 /* 100 */ "xx_return_statement ::= RETURN IDENTIFIER DOTCOMMA",
 /* 101 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 102 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 103 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 104 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 105 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 106 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 107 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 108 */ "xx_declare_variable ::= IDENTIFIER",
 /* 109 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 110 */ "xx_assign_expr ::= xx_assign_expr ADD xx_assign_expr",
 /* 111 */ "xx_assign_expr ::= xx_assign_expr SUB xx_assign_expr",
 /* 112 */ "xx_assign_expr ::= IDENTIFIER",
 /* 113 */ "xx_assign_expr ::= INTEGER",
 /* 114 */ "xx_assign_expr ::= STRING",
 /* 115 */ "xx_assign_expr ::= DOUBLE",
 /* 116 */ "xx_assign_expr ::= NULL",
 /* 117 */ "xx_assign_expr ::= FALSE",
 /* 118 */ "xx_assign_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 119 */ "xx_assign_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 120 */ "xx_assign_expr ::= NEW IDENTIFIER",
 /* 121 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 122 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 123 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 124 */ "xx_call_parameters ::= xx_call_parameter",
 /* 125 */ "xx_call_parameter ::= xx_literal_expr",
 /* 126 */ "xx_call_parameter ::= IDENTIFIER",
 /* 127 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 128 */ "xx_array_list ::= xx_array_item",
 /* 129 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 130 */ "xx_array_item ::= xx_array_value",
 /* 131 */ "xx_array_key ::= STRING",
 /* 132 */ "xx_array_key ::= INTEGER",
 /* 133 */ "xx_array_value ::= xx_literal_expr",
 /* 134 */ "xx_array_value ::= IDENTIFIER",
 /* 135 */ "xx_assign_expr ::= TRUE",
 /* 136 */ "xx_literal_expr ::= INTEGER",
 /* 137 */ "xx_literal_expr ::= STRING",
 /* 138 */ "xx_literal_expr ::= DOUBLE",
 /* 139 */ "xx_literal_expr ::= NULL",
 /* 140 */ "xx_literal_expr ::= FALSE",
 /* 141 */ "xx_literal_expr ::= TRUE",
 /* 142 */ "xx_eval_expr ::= xx_literal_expr",
 /* 143 */ "xx_eval_expr ::= xx_eval_expr EQUALS xx_eval_expr",
 /* 144 */ "xx_eval_expr ::= xx_eval_expr IDENTICAL xx_eval_expr",
 /* 145 */ "xx_eval_expr ::= PARENTHESES_OPEN xx_eval_expr PARENTHESES_CLOSE",
 /* 146 */ "xx_eval_expr ::= IDENTIFIER",
 /* 147 */ "xx_comment ::= COMMENT",
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
// 577 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1341 "parser.c"
      break;
    case 52:
// 590 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 1346 "parser.c"
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
  { 51, 1 },
  { 52, 1 },
  { 53, 2 },
  { 53, 1 },
  { 54, 1 },
  { 54, 1 },
  { 54, 1 },
  { 55, 3 },
  { 56, 5 },
  { 56, 7 },
  { 56, 6 },
  { 56, 8 },
  { 56, 4 },
  { 56, 6 },
  { 56, 5 },
  { 56, 7 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 58, 2 },
  { 58, 2 },
  { 58, 2 },
  { 58, 2 },
  { 58, 3 },
  { 58, 3 },
  { 59, 2 },
  { 59, 1 },
  { 62, 4 },
  { 62, 3 },
  { 62, 5 },
  { 62, 6 },
  { 62, 3 },
  { 62, 4 },
  { 62, 5 },
  { 62, 6 },
  { 60, 2 },
  { 60, 1 },
  { 61, 2 },
  { 64, 6 },
  { 64, 5 },
  { 61, 1 },
  { 65, 7 },
  { 65, 8 },
  { 65, 8 },
  { 65, 9 },
  { 65, 8 },
  { 65, 9 },
  { 65, 9 },
  { 65, 10 },
  { 65, 7 },
  { 65, 8 },
  { 65, 8 },
  { 65, 9 },
  { 66, 3 },
  { 66, 1 },
  { 68, 1 },
  { 68, 2 },
  { 68, 2 },
  { 68, 3 },
  { 68, 4 },
  { 68, 4 },
  { 70, 3 },
  { 69, 1 },
  { 69, 1 },
  { 69, 1 },
  { 69, 1 },
  { 69, 1 },
  { 67, 2 },
  { 67, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 78, 2 },
  { 79, 2 },
  { 73, 4 },
  { 73, 7 },
  { 73, 5 },
  { 73, 9 },
  { 73, 8 },
  { 73, 8 },
  { 74, 3 },
  { 74, 4 },
  { 80, 4 },
  { 80, 5 },
  { 72, 3 },
  { 82, 3 },
  { 82, 1 },
  { 83, 3 },
  { 83, 5 },
  { 83, 5 },
  { 75, 3 },
  { 75, 3 },
  { 76, 3 },
  { 76, 3 },
  { 77, 3 },
  { 77, 3 },
  { 77, 3 },
  { 77, 3 },
  { 77, 3 },
  { 85, 3 },
  { 85, 1 },
  { 86, 1 },
  { 86, 3 },
  { 84, 3 },
  { 84, 3 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 2 },
  { 84, 3 },
  { 84, 2 },
  { 84, 4 },
  { 84, 5 },
  { 88, 3 },
  { 88, 1 },
  { 89, 1 },
  { 89, 1 },
  { 87, 3 },
  { 87, 1 },
  { 90, 3 },
  { 90, 1 },
  { 91, 1 },
  { 91, 1 },
  { 92, 1 },
  { 92, 1 },
  { 84, 1 },
  { 63, 1 },
  { 63, 1 },
  { 63, 1 },
  { 63, 1 },
  { 63, 1 },
  { 63, 1 },
  { 81, 1 },
  { 81, 3 },
  { 81, 3 },
  { 81, 3 },
  { 81, 1 },
  { 57, 1 },
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
// 586 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy175;
}
// 1711 "parser.c"
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
      case 125:
      case 133:
      case 142:
// 592 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 1734 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 596 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 1745 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 68:
      case 93:
      case 107:
      case 124:
      case 128:
// 600 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 1761 "parser.c"
        break;
      case 7:
// 616 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1770 "parser.c"
        break;
      case 8:
// 620 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 0, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1780 "parser.c"
        break;
      case 9:
// 624 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy175, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1791 "parser.c"
        break;
      case 10:
// 628 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 1, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-5].minor);
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1802 "parser.c"
        break;
      case 11:
// 632 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy175, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(14,&yymsp[-7].minor);
  yy_destructor(10,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1814 "parser.c"
        break;
      case 12:
// 636 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1824 "parser.c"
        break;
      case 13:
// 640 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-5].minor);
  yy_destructor(13,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1835 "parser.c"
        break;
      case 14:
// 644 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1846 "parser.c"
        break;
      case 15:
// 648 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(10,&yymsp[-5].minor);
  yy_destructor(13,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1858 "parser.c"
        break;
      case 16:
// 652 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 1865 "parser.c"
        break;
      case 17:
// 656 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 1872 "parser.c"
        break;
      case 18:
// 660 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 1879 "parser.c"
        break;
      case 19:
// 664 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 1886 "parser.c"
        break;
      case 20:
// 668 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 1893 "parser.c"
        break;
      case 21:
// 672 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 1900 "parser.c"
        break;
      case 22:
// 676 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 1907 "parser.c"
        break;
      case 23:
// 680 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 1914 "parser.c"
        break;
      case 24:
// 684 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 1921 "parser.c"
        break;
      case 27:
// 696 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1930 "parser.c"
        break;
      case 28:
// 700 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1939 "parser.c"
        break;
      case 29:
// 704 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1949 "parser.c"
        break;
      case 30:
// 708 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(16,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1959 "parser.c"
        break;
      case 31:
// 712 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1968 "parser.c"
        break;
      case 32:
// 716 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1977 "parser.c"
        break;
      case 33:
// 720 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1987 "parser.c"
        break;
      case 34:
// 724 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1997 "parser.c"
        break;
      case 38:
// 740 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2007 "parser.c"
        break;
      case 39:
// 744 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2017 "parser.c"
        break;
      case 41:
// 752 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2030 "parser.c"
        break;
      case 42:
// 756 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2043 "parser.c"
        break;
      case 43:
// 760 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2056 "parser.c"
        break;
      case 44:
// 764 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2069 "parser.c"
        break;
      case 45:
// 768 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2082 "parser.c"
        break;
      case 46:
// 772 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-8].minor);
  yy_destructor(20,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2095 "parser.c"
        break;
      case 47:
// 776 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2108 "parser.c"
        break;
      case 48:
// 780 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(16,&yymsp[-8].minor);
  yy_destructor(20,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2121 "parser.c"
        break;
      case 49:
// 784 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2134 "parser.c"
        break;
      case 50:
// 788 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2147 "parser.c"
        break;
      case 51:
// 792 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2160 "parser.c"
        break;
      case 52:
// 796 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2173 "parser.c"
        break;
      case 53:
      case 92:
      case 106:
      case 123:
      case 127:
// 800 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2185 "parser.c"
        break;
      case 55:
// 808 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2192 "parser.c"
        break;
      case 56:
// 812 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2199 "parser.c"
        break;
      case 57:
// 816 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2206 "parser.c"
        break;
      case 58:
// 820 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2214 "parser.c"
        break;
      case 59:
// 824 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2222 "parser.c"
        break;
      case 60:
// 828 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2230 "parser.c"
        break;
      case 61:
// 832 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2239 "parser.c"
        break;
      case 62:
// 836 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(25,&yymsp[0].minor);
}
// 2247 "parser.c"
        break;
      case 63:
// 840 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(26,&yymsp[0].minor);
}
// 2255 "parser.c"
        break;
      case 64:
// 844 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(27,&yymsp[0].minor);
}
// 2263 "parser.c"
        break;
      case 65:
// 848 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(28,&yymsp[0].minor);
}
// 2271 "parser.c"
        break;
      case 66:
// 852 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(29,&yymsp[0].minor);
}
// 2279 "parser.c"
        break;
      case 79:
// 904 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2288 "parser.c"
        break;
      case 80:
// 908 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2297 "parser.c"
        break;
      case 81:
// 912 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2307 "parser.c"
        break;
      case 82:
// 916 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(11,&yymsp[-4].minor);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2320 "parser.c"
        break;
      case 83:
// 920 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2330 "parser.c"
        break;
      case 84:
// 924 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(32,&yymsp[-8].minor);
  yy_destructor(11,&yymsp[-6].minor);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2343 "parser.c"
        break;
      case 85:
// 928 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(11,&yymsp[-5].minor);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2356 "parser.c"
        break;
      case 86:
// 932 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(11,&yymsp[-5].minor);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2369 "parser.c"
        break;
      case 87:
// 936 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2379 "parser.c"
        break;
      case 88:
// 940 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2389 "parser.c"
        break;
      case 89:
// 944 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2399 "parser.c"
        break;
      case 90:
// 948 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2409 "parser.c"
        break;
      case 91:
// 952 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2418 "parser.c"
        break;
      case 94:
// 964 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2426 "parser.c"
        break;
      case 95:
// 968 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2435 "parser.c"
        break;
      case 96:
// 972 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2445 "parser.c"
        break;
      case 97:
// 976 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2454 "parser.c"
        break;
      case 98:
// 980 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2463 "parser.c"
        break;
      case 99:
// 984 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2472 "parser.c"
        break;
      case 100:
// 988 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2481 "parser.c"
        break;
      case 101:
// 992 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2490 "parser.c"
        break;
      case 102:
// 996 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2499 "parser.c"
        break;
      case 103:
// 1000 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2508 "parser.c"
        break;
      case 104:
// 1004 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2517 "parser.c"
        break;
      case 105:
// 1008 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2526 "parser.c"
        break;
      case 108:
// 1020 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2533 "parser.c"
        break;
      case 109:
// 1024 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2541 "parser.c"
        break;
      case 110:
// 1028 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 2549 "parser.c"
        break;
      case 111:
// 1032 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2557 "parser.c"
        break;
      case 112:
      case 126:
      case 134:
      case 146:
// 1036 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 2567 "parser.c"
        break;
      case 113:
      case 132:
      case 136:
// 1040 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 2576 "parser.c"
        break;
      case 114:
      case 131:
      case 137:
// 1044 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 2585 "parser.c"
        break;
      case 115:
      case 138:
// 1048 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 2593 "parser.c"
        break;
      case 116:
      case 139:
// 1052 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[0].minor);
}
// 2602 "parser.c"
        break;
      case 117:
      case 140:
// 1056 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[0].minor);
}
// 2611 "parser.c"
        break;
      case 118:
// 1060 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2620 "parser.c"
        break;
      case 119:
// 1064 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2629 "parser.c"
        break;
      case 120:
// 1068 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 2637 "parser.c"
        break;
      case 121:
// 1072 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2647 "parser.c"
        break;
      case 122:
// 1076 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(6,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2657 "parser.c"
        break;
      case 129:
// 1104 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-1].minor);
}
// 2665 "parser.c"
        break;
      case 130:
// 1108 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2672 "parser.c"
        break;
      case 135:
      case 141:
// 1128 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[0].minor);
}
// 2681 "parser.c"
        break;
      case 143:
// 1160 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 2689 "parser.c"
        break;
      case 144:
// 1164 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 2697 "parser.c"
        break;
      case 145:
// 1168 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2706 "parser.c"
        break;
      case 147:
// 1176 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 2713 "parser.c"
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
// 543 "parser.lemon"


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

// 2789 "parser.c"
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
