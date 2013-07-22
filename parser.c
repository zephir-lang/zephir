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

static json_object *xx_ret_literal(int type, xx_parser_token *T)
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

	return ret;
}

static json_object *xx_ret_expr(char *type, json_object *left, json_object *right, json_object *extra)
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

	return ret;
}

static json_object *xx_ret_array_item(json_object *key, json_object *value)
{
	json_object *ret = json_object_new_object();

	if (key) {
		json_object_object_add(ret, "key", key);
	}
	json_object_object_add(ret, "value", value);

	return ret;
}

static json_object *xx_ret_namespace(xx_parser_token *T)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("namespace"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	return ret;
}

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition, int is_abstract, xx_parser_token *E)
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

	return ret;
}

static json_object *xx_ret_class_definition(json_object *properties, json_object *methods, json_object *constants)
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

	return ret;
}

static json_object *xx_ret_class_property(int visibility, xx_parser_token *T, json_object *default_value, xx_parser_token *D)
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

	return ret;
}

static json_object *xx_ret_class_const(xx_parser_token *T, json_object *default_value, xx_parser_token *D)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("const"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));
	json_object_object_add(ret, "default", default_value);

	if (D) {
		json_object_object_add(ret, "docblock", json_object_new_string(D->token));
	}

	return ret;
}

static json_object *xx_ret_class_method(int visibility, xx_parser_token *T, json_object *parameters, json_object *statements, xx_parser_token *D)
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

	return ret;
}

static json_object *xx_ret_parameter(json_object *type, json_object *cast, xx_parser_token *N, json_object *default_value)
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

static json_object *xx_ret_let_statement(json_object *assignments)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("let"));
	json_object_object_add(ret, "assignments", assignments);

	return ret;
}

static json_object *xx_ret_let_assignment(char *type, xx_parser_token *V, xx_parser_token *P, json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "assign-type", json_object_new_string(type));
	json_object_object_add(ret, "variable", json_object_new_string(V->token));
	if (P) {
		json_object_object_add(ret, "property", json_object_new_string(P->token));
	}
	json_object_object_add(ret, "expr", expr);

	return ret;
}

static json_object *xx_ret_if_statement(json_object *expr, json_object *statements, json_object *else_statements)
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

	return ret;
}

static json_object *xx_ret_while_statement(json_object *expr, json_object *statements)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("while"));
	json_object_object_add(ret, "expr", expr);

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	return ret;
}

static json_object *xx_ret_loop_statement(json_object *statements)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("loop"));

	if (statements) {
		json_object_object_add(ret, "statements", statements);
	}

	return ret;
}

static json_object *xx_ret_break_statement()
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("break"));

	return ret;
}

static json_object *xx_ret_continue_statement()
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("continue"));

	return ret;
}

static json_object *xx_ret_echo_statement(json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("echo"));
	json_object_object_add(ret, "expr", expr);

	return ret;
}

static json_object *xx_ret_return_statement(json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return"));
	json_object_object_add(ret, "expr", expr);

	return ret;
}

static json_object *xx_ret_declare_statement(int type, json_object *variables)
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

	return ret;
}

static json_object *xx_ret_declare_variable(xx_parser_token *T, json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "variable", json_object_new_string(T->token));
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	return ret;
}

static json_object *xx_ret_new_instance(xx_parser_token *T, json_object *parameters)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("new"));
	json_object_object_add(ret, "class", json_object_new_string(T->token));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	return ret;
}

static json_object *xx_ret_comment(xx_parser_token *T)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("comment"));
	json_object_object_add(ret, "value", json_object_new_string(T->token));

	return ret;
}


// 446 "parser.c"
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
#define YYNSTATE 304
#define YYNRULE 147
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
 /*     0 */   174,   80,  128,  151,  175,   97,   96,   91,   92,   93,
 /*    10 */    83,   94,  135,  130,  139,  145,  142,  166,   39,   41,
 /*    20 */    43,  175,   52,   56,   61,  198,  154,  157,  120,  125,
 /*    30 */   148,  169,  279,  250,  151,   45,  249,   89,   90,   91,
 /*    40 */    92,   93,  212,   94,  130,  139,  145,  142,   82,   39,
 /*    50 */    41,   43,  227,   52,   56,   61,   69,   71,  113,  120,
 /*    60 */   125,  148,  225,  154,  157,  151,   13,   16,  277,  283,
 /*    70 */   282,  292,  276,  249,  105,  130,  139,  145,  142,  107,
 /*    80 */    39,   41,   43,  161,   52,   56,   61,  131,  138,   68,
 /*    90 */   120,  125,  148,   47,  108,  104,  151,  452,    1,    2,
 /*   100 */   303,    4,    5,    6,  103,  109,  130,  139,  145,  142,
 /*   110 */    66,   39,   41,   43,  203,   52,   56,   61,  155,  154,
 /*   120 */   157,  120,  125,  148,  234,  167,  133,  151,   58,   89,
 /*   130 */    90,   91,   92,   93,  132,   94,   44,  130,  139,  145,
 /*   140 */   142,  175,   39,   41,   43,  253,   52,   56,   61,  140,
 /*   150 */   138,  172,  120,  125,  148,  208,  133,  203,  151,  294,
 /*   160 */    16,  277,  283,  282,  141,  276,  249,  370,  130,  139,
 /*   170 */   145,  142,  183,   39,   41,   43,  190,   52,   56,   61,
 /*   180 */   143,  138,  256,  120,  125,  148,  232,  123,   18,  151,
 /*   190 */   287,   16,  277,  283,  282,  259,  276,  249,   88,  130,
 /*   200 */   139,  145,  142,  133,   39,   41,   43,  218,   52,   56,
 /*   210 */    61,  144,  107,  254,  120,  125,  148,  170,  146,  138,
 /*   220 */   151,  255,   89,   90,   91,   92,   93,   87,   94,  257,
 /*   230 */   130,  139,  145,  142,  133,   39,   41,   43,  106,   52,
 /*   240 */    56,   61,  147,  149,  138,  120,  125,  148,  210,   62,
 /*   250 */   119,  151,  300,   16,  277,  283,  282,  260,  276,  249,
 /*   260 */     8,  130,  139,  145,  142,  261,   39,   41,   43,  207,
 /*   270 */    52,   56,   61,   12,  117,  284,  120,  125,  148,  214,
 /*   280 */   295,   65,  151,   89,   90,   91,   92,   93,  252,   94,
 /*   290 */   258,  239,  130,  139,  145,  142,  133,   39,   41,   43,
 /*   300 */   265,   52,   56,   61,  150,   53,   40,  120,  125,  148,
 /*   310 */    60,  263,   20,  151,   97,   96,   91,   92,   93,  202,
 /*   320 */    94,  217,  239,  130,  139,  145,  142,  266,   39,   41,
 /*   330 */    43,  271,   52,   56,   61,  267,  262,   14,  120,  125,
 /*   340 */   148,   27,  320,  227,  151,  251,  264,  181,  270,  244,
 /*   350 */     3,    4,    5,    6,  130,  139,  145,  142,  272,   39,
 /*   360 */    41,   43,  182,   52,   56,   61,  273,   72,  122,  120,
 /*   370 */   125,  148,  153,  324,   20,  151,  238,   19,  322,  226,
 /*   380 */   244,  201,   19,  304,  226,  130,  139,  145,  142,  365,
 /*   390 */    39,   41,   43,   67,   52,   56,   61,  155,  366,  222,
 /*   400 */   120,  125,  148,  216,  268,  269,  151,  235,   17,  250,
 /*   410 */   298,  276,  249,  111,  115,  156,  130,  139,  145,  142,
 /*   420 */   199,   39,   41,   43,  299,   52,   56,   61,  155,  205,
 /*   430 */   114,  120,  125,  148,   55,  296,   64,  151,  251,  264,
 /*   440 */   121,  270,  244,   88,   63,   21,   57,  130,  139,  145,
 /*   450 */   142,  221,   39,   41,   43,  129,   52,   56,   61,  305,
 /*   460 */   220,  274,  120,  125,  148,   25,    7,   81,  151,   10,
 /*   470 */    98,   85,   95,  290,  151,  176,  180,  184,  130,  139,
 /*   480 */   145,  142,  275,   39,   41,   43,  204,   52,   56,   61,
 /*   490 */   252,  179,  258,  120,  125,  148,  164,  289,  435,  151,
 /*   500 */   251,  264,  327,  270,  244,  201,   19,  219,  226,  130,
 /*   510 */   139,  145,  142,  301,   39,   41,   43,  218,   52,   56,
 /*   520 */    61,  293,  127,  297,  120,  125,  148,  165,   15,  126,
 /*   530 */   151,  251,  264,  323,  270,  244,  201,   19,  368,  226,
 /*   540 */   130,  139,  145,  142,  118,   39,   41,   43,  367,   52,
 /*   550 */    56,   61,  202,    9,  217,  120,  125,  148,  163,  302,
 /*   560 */   213,  151,  251,  264,  328,  270,  244,  201,   19,   11,
 /*   570 */   226,  130,  139,  145,  142,  436,   39,   41,   43,  124,
 /*   580 */    52,   56,   61,  155,  285,  178,  120,  125,  148,  223,
 /*   590 */   286,  278,  151,  281,  282,  236,  237,  249,  237,  249,
 /*   600 */   369,  158,  130,  139,  145,  142,  116,   39,   41,   43,
 /*   610 */    42,   52,   56,   61,  288,   22,  186,  120,  125,  148,
 /*   620 */    51,  321,  136,  151,  251,  264,  325,  270,  244,  280,
 /*   630 */   264,   48,  270,  130,  139,  145,  142,  100,   39,   41,
 /*   640 */    43,  187,   52,   56,   61,  134,   88,  155,  120,  125,
 /*   650 */   148,  151,    7,  231,  171,   10,  196,  180,  184,  290,
 /*   660 */   151,  130,  139,  145,  142,  160,   39,   41,   43,   38,
 /*   670 */    52,   56,   61,   84,   85,   95,  120,  125,  148,  168,
 /*   680 */   112,  177,  137,  152,   29,   30,   31,   32,   33,   34,
 /*   690 */    35,   36,   37,   49,   38,  206,  188,  240,  191,  192,
 /*   700 */   193,  194,  195,  177,  173,  230,   24,  241,  152,   29,
 /*   710 */    30,   31,   32,   33,   34,   35,   36,   37,  188,   38,
 /*   720 */   191,  192,  193,  194,  195,   70,  229,  242,  243,   59,
 /*   730 */   177,  189,  245,  152,   29,   30,   31,   32,   33,   34,
 /*   740 */    35,   36,   37,   38,   23,  188,  101,  191,  192,  193,
 /*   750 */   194,  195,  246,   54,  177,  185,  228,  152,   29,   30,
 /*   760 */    31,   32,   33,   34,   35,   36,   37,   38,  248,  188,
 /*   770 */   247,  191,  192,  193,  194,  195,   86,   50,  200,  291,
 /*   780 */   242,  152,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   790 */    37,   38,  242,  242,  242,  242,  211,  242,  196,  180,
 /*   800 */   184,   26,  242,  242,  242,  152,   29,   30,   31,   32,
 /*   810 */    33,   34,   35,   36,   37,   38,  242,  242,  242,  242,
 /*   820 */   326,  242,  242,  201,   19,  233,  226,  242,  242,  152,
 /*   830 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   840 */   197,  242,  196,  180,  184,  242,  242,  242,  242,  224,
 /*   850 */   242,  242,  242,  152,   29,   30,   31,   32,   33,   34,
 /*   860 */    35,   36,   37,   38,  242,  242,  242,  242,  242,  242,
 /*   870 */   242,  242,  242,   46,  242,  242,  242,  152,   29,   30,
 /*   880 */    31,   32,   33,   34,   35,   36,   37,   38,  242,  242,
 /*   890 */   242,  242,  242,  242,  242,  242,  242,  215,  242,  242,
 /*   900 */   242,  152,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   910 */    37,   38,  242,  242,  242,  242,  242,  242,  242,  242,
 /*   920 */   242,  209,  242,  242,  242,  152,   29,   30,   31,   32,
 /*   930 */    33,   34,   35,   36,   37,   38,   99,  242,   73,  242,
 /*   940 */   242,  242,  242,  242,  242,  242,  242,  162,  242,   28,
 /*   950 */    29,   30,   31,   32,   33,   34,   35,   36,   37,  242,
 /*   960 */   159,  242,  242,  242,  108,  242,  242,  242,   79,  242,
 /*   970 */   242,  242,  242,   74,   75,   76,   77,   78,  102,  110,
 /*   980 */   242,  242,   89,   90,   91,   92,   93,  242,   94,  242,
 /*   990 */   242,  242,  242,  242,  242,  242,  242,  242,  242,   89,
 /*  1000 */    90,   91,   92,   93,  242,   94,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    12,   39,    8,   15,    1,   43,   44,   45,   46,   47,
 /*    10 */     1,   49,    8,   25,   26,   27,   28,   33,   30,   31,
 /*    20 */    32,    1,   34,   35,   36,   22,    2,    3,   40,   41,
 /*    30 */    42,   12,   61,   62,   15,   11,   65,   43,   44,   45,
 /*    40 */    46,   47,   22,   49,   25,   26,   27,   28,   39,   30,
 /*    50 */    31,   32,   20,   34,   35,   36,    4,    5,   17,   40,
 /*    60 */    41,   42,   12,    2,    3,   15,   58,   59,   60,   61,
 /*    70 */    62,    8,   64,   65,    1,   25,   26,   27,   28,   63,
 /*    80 */    30,   31,   32,   22,   34,   35,   36,   85,   86,   84,
 /*    90 */    40,   41,   42,   12,    8,   22,   15,   51,   52,   53,
 /*   100 */    54,   55,   56,   57,   88,   89,   25,   26,   27,   28,
 /*   110 */     8,   30,   31,   32,   20,   34,   35,   36,   63,    2,
 /*   120 */     3,   40,   41,   42,   12,   11,    1,   15,   11,   43,
 /*   130 */    44,   45,   46,   47,    9,   49,   81,   25,   26,   27,
 /*   140 */    28,    1,   30,   31,   32,    8,   34,   35,   36,   85,
 /*   150 */    86,   11,   40,   41,   42,   12,    1,   20,   15,   58,
 /*   160 */    59,   60,   61,   62,    9,   64,   65,    8,   25,   26,
 /*   170 */    27,   28,   63,   30,   31,   32,   24,   34,   35,   36,
 /*   180 */    85,   86,   63,   40,   41,   42,   12,    8,   65,   15,
 /*   190 */    58,   59,   60,   61,   62,    8,   64,   65,   63,   25,
 /*   200 */    26,   27,   28,    1,   30,   31,   32,   20,   34,   35,
 /*   210 */    36,    9,   63,    9,   40,   41,   42,   12,   85,   86,
 /*   220 */    15,   17,   43,   44,   45,   46,   47,   92,   49,    9,
 /*   230 */    25,   26,   27,   28,    1,   30,   31,   32,   89,   34,
 /*   240 */    35,   36,    9,   85,   86,   40,   41,   42,   12,   82,
 /*   250 */    83,   15,   58,   59,   60,   61,   62,    9,   64,   65,
 /*   260 */     8,   25,   26,   27,   28,   17,   30,   31,   32,   11,
 /*   270 */    34,   35,   36,   11,   17,   13,   40,   41,   42,   12,
 /*   280 */    12,   83,   15,   43,   44,   45,   46,   47,   16,   49,
 /*   290 */    18,   19,   25,   26,   27,   28,    1,   30,   31,   32,
 /*   300 */     8,   34,   35,   36,    9,   11,    9,   40,   41,   42,
 /*   310 */    12,    9,   20,   15,   43,   44,   45,   46,   47,   16,
 /*   320 */    49,   18,   19,   25,   26,   27,   28,    9,   30,   31,
 /*   330 */    32,    8,   34,   35,   36,   17,   63,   12,   40,   41,
 /*   340 */    42,   12,   12,   20,   15,   15,   16,    8,   18,   19,
 /*   350 */    54,   55,   56,   57,   25,   26,   27,   28,    9,   30,
 /*   360 */    31,   32,   17,   34,   35,   36,   17,   84,    9,   40,
 /*   370 */    41,   42,   12,   12,   20,   15,   15,   16,   12,   18,
 /*   380 */    19,   15,   16,    0,   18,   25,   26,   27,   28,    8,
 /*   390 */    30,   31,   32,   17,   34,   35,   36,   63,    8,   11,
 /*   400 */    40,   41,   42,   12,   63,    9,   15,   60,   61,   62,
 /*   410 */     8,   64,   65,   37,   38,   81,   25,   26,   27,   28,
 /*   420 */    11,   30,   31,   32,   11,   34,   35,   36,   63,   21,
 /*   430 */    84,   40,   41,   42,   12,   12,    1,   15,   15,   16,
 /*   440 */    63,   18,   19,   63,    9,    8,   81,   25,   26,   27,
 /*   450 */    28,   22,   30,   31,   32,    9,   34,   35,   36,    0,
 /*   460 */    21,   63,   40,   41,   42,   12,    7,   87,   15,   10,
 /*   470 */    90,   91,   92,   14,   15,   68,   69,   70,   25,   26,
 /*   480 */    27,   28,    9,   30,   31,   32,    8,   34,   35,   36,
 /*   490 */    16,   63,   18,   40,   41,   42,   12,   12,   48,   15,
 /*   500 */    15,   16,   12,   18,   19,   15,   16,    8,   18,   25,
 /*   510 */    26,   27,   28,   12,   30,   31,   32,   20,   34,   35,
 /*   520 */    36,   11,    9,   13,   40,   41,   42,   12,   12,   63,
 /*   530 */    15,   15,   16,   12,   18,   19,   15,   16,    8,   18,
 /*   540 */    25,   26,   27,   28,   84,   30,   31,   32,    8,   34,
 /*   550 */    35,   36,   16,    9,   18,   40,   41,   42,   12,   12,
 /*   560 */    11,   15,   15,   16,   12,   18,   19,   15,   16,    8,
 /*   570 */    18,   25,   26,   27,   28,   48,   30,   31,   32,    9,
 /*   580 */    34,   35,   36,   63,    8,   17,   40,   41,   42,   12,
 /*   590 */    11,   59,   15,   61,   62,   61,   64,   65,   64,   65,
 /*   600 */     8,   81,   25,   26,   27,   28,   39,   30,   31,   32,
 /*   610 */     9,   34,   35,   36,   12,   21,   17,   40,   41,   42,
 /*   620 */    12,   12,   17,   15,   15,   16,   12,   18,   19,   15,
 /*   630 */    16,   33,   18,   25,   26,   27,   28,    8,   30,   31,
 /*   640 */    32,   63,   34,   35,   36,   86,   63,   63,   40,   41,
 /*   650 */    42,   15,    7,   11,   66,   10,   68,   69,   70,   14,
 /*   660 */    15,   25,   26,   27,   28,   81,   30,   31,   32,   57,
 /*   670 */    34,   35,   36,   90,   91,   92,   40,   41,   42,   67,
 /*   680 */     8,    8,   63,   71,   72,   73,   74,   75,   76,   77,
 /*   690 */    78,   79,   80,   11,   57,   22,   23,    8,   25,   26,
 /*   700 */    27,   28,   29,    8,   67,   22,   11,   17,   71,   72,
 /*   710 */    73,   74,   75,   76,   77,   78,   79,   80,   23,   57,
 /*   720 */    25,   26,   27,   28,   29,   84,   21,   63,    9,   67,
 /*   730 */     8,    8,    8,   71,   72,   73,   74,   75,   76,   77,
 /*   740 */    78,   79,   80,   57,   22,   23,   21,   25,   26,   27,
 /*   750 */    28,   29,   17,   67,    8,    8,    8,   71,   72,   73,
 /*   760 */    74,   75,   76,   77,   78,   79,   80,   57,    9,   23,
 /*   770 */    63,   25,   26,   27,   28,   29,   48,   67,   12,   10,
 /*   780 */    93,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   790 */    80,   57,   93,   93,   93,   93,   66,   93,   68,   69,
 /*   800 */    70,   67,   93,   93,   93,   71,   72,   73,   74,   75,
 /*   810 */    76,   77,   78,   79,   80,   57,   93,   93,   93,   93,
 /*   820 */    12,   93,   93,   15,   16,   67,   18,   93,   93,   71,
 /*   830 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   57,
 /*   840 */    66,   93,   68,   69,   70,   93,   93,   93,   93,   67,
 /*   850 */    93,   93,   93,   71,   72,   73,   74,   75,   76,   77,
 /*   860 */    78,   79,   80,   57,   93,   93,   93,   93,   93,   93,
 /*   870 */    93,   93,   93,   67,   93,   93,   93,   71,   72,   73,
 /*   880 */    74,   75,   76,   77,   78,   79,   80,   57,   93,   93,
 /*   890 */    93,   93,   93,   93,   93,   93,   93,   67,   93,   93,
 /*   900 */    93,   71,   72,   73,   74,   75,   76,   77,   78,   79,
 /*   910 */    80,   57,   93,   93,   93,   93,   93,   93,   93,   93,
 /*   920 */    93,   67,   93,   93,   93,   71,   72,   73,   74,   75,
 /*   930 */    76,   77,   78,   79,   80,   57,    6,   93,    8,   93,
 /*   940 */    93,   93,   93,   93,   93,   93,   93,    8,   93,   71,
 /*   950 */    72,   73,   74,   75,   76,   77,   78,   79,   80,   93,
 /*   960 */    21,   93,   93,   93,    8,   93,   93,   93,   38,   93,
 /*   970 */    93,   93,   93,   43,   44,   45,   46,   47,   22,   49,
 /*   980 */    93,   93,   43,   44,   45,   46,   47,   93,   49,   93,
 /*   990 */    93,   93,   93,   93,   93,   93,   93,   93,   93,   43,
 /*  1000 */    44,   45,   46,   47,   93,   49,
};
#define YY_SHIFT_USE_DFLT (-39)
static short yy_shift_ofst[] = {
 /*     0 */   645,  383,  459,  -39,  -39,  -39,  -39,  252,  544,  -39,
 /*    10 */   561,  262,  516,  325,  -39,  -39,  330,  521,  -39,  354,
 /*    20 */   437,  594,  722,  695,  453,  -39,  329,  -39,  -39,  -39,
 /*    30 */   -39,  -39,  -39,  -39,  -39,  -39,  -39,  -39,  -39,  297,
 /*    40 */   -39,  601,  -39,  939,   24,  515,   81,  598,  682,  484,
 /*    50 */   608,  -39,  294,  546,  422,  -39,  939,  117,  360,  298,
 /*    60 */   -39,  102,  435,  -39,  102,  -39,  376,  930,   52,  930,
 /*    70 */   -39,  930,  -39,  -39,  -39,  -39,  -39,  -39,  -39,  -38,
 /*    80 */   -39,    9,  -39,  271,  -39,  728,  240,  -39,  -39,  -39,
 /*    90 */   -39,  -39,  -39,  -39,  -39,  -39,  450,  527,  -39,  629,
 /*   100 */   725,  956,  -39,   73,  -39,   86,  -39,  -39,  -39,  -39,
 /*   110 */   -39,  672,   41,  930,   52,  567,  257,  930,   52,  -39,
 /*   120 */   179,  359,  -39,  570,  -39,   -6,  513,  -39,  446,  -39,
 /*   130 */     4,  125,  -39,    4,  -39,  605,  240,  -39,  -39,    4,
 /*   140 */   155,  -39,    4,  202,  -39,    4,  233,  -39,    4,  295,
 /*   150 */   -39,  -39,  -39,  -39,  939,  -39,  -39,  939,  -39,  939,
 /*   160 */    61,  -39,  -39,  -39,  -39,  -16,  114,  205,   19,  -39,
 /*   170 */   -39,  140,  636,  -12,  -39,  746,  -39,  568,  240,  -39,
 /*   180 */   339,  345,  240,  -39,  747,  599,  240,  -39,  723,  152,
 /*   190 */   381,  390,  540,  530,  592,  159,  -39,    3,  409,  766,
 /*   200 */   -39,  536,   94,  478,  408,  673,  258,  143,  -39,  236,
 /*   210 */   -39,   20,  549,  267,  -39,  391,  -39,  497,  499,  439,
 /*   220 */   429,  388,  577,  -39,   50,  -39,   32,  748,  705,  683,
 /*   230 */   642,  174,  -39,  112,  -39,  361,  490,  -39,  303,  689,
 /*   240 */   690,  240,  719,  -39,  724,  735,  240,  759,  -39,  -39,
 /*   250 */   -39,  272,  137,  204,  -39,  240,  220,  -39,  187,  248,
 /*   260 */   -39,  240,  302,  -39,  292,  318,  -39,  240,  396,  -39,
 /*   270 */   323,  349,  -39,  240,  473,  -39,  -39,  609,  614,  552,
 /*   280 */   474,  808,  -39,  366,  576,  579,  485,  602,  -39,  -39,
 /*   290 */   769,   63,  510,  423,  268,  -39,  -39,  402,  413,  547,
 /*   300 */   501,  -39,  -39,  -39,
};
#define YY_REDUCE_USE_DFLT (-30)
static short yy_reduce_ofst[] = {
 /*     0 */    46,  -30,  296,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*    10 */   -30,  -30,    8,  -30,  -30,  -30,  347,  123,  -30,  -30,
 /*    20 */   -30,  -30,  774,  588,  734,  -30,  878,  -30,  -30,  -30,
 /*    30 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*    40 */   -30,  -30,  -30,   55,  -30,  806,  878,  -30,  -30,  710,
 /*    50 */   878,  -30,  -30,  686,  878,  -30,  365,  -30,  662,  878,
 /*    60 */   -30,  167,  -30,  -30,  198,  -30,  -30,    5,  -30,  641,
 /*    70 */   -30,  283,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  380,
 /*    80 */   -30,  -30,  -30,  583,  -30,  -30,  135,  -30,  -30,  -30,
 /*    90 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   100 */   -30,   16,  -30,  -30,  -30,  149,  -30,  -30,  -30,  -30,
 /*   110 */   -30,  -30,  -30,  346,  -30,  -30,  -30,  460,  -30,  -30,
 /*   120 */   377,  -30,  -30,  -30,  -30,  466,  -30,  -30,  -30,  -30,
 /*   130 */     2,  -30,  -30,  559,  -30,  -30,  619,  -30,  -30,   64,
 /*   140 */   -30,  -30,   95,  -30,  -30,  133,  -30,  -30,  158,  -30,
 /*   150 */   -30,  -30,  -30,  -30,  334,  -30,  -30,  520,  -30,  584,
 /*   160 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  612,  878,  -30,
 /*   170 */   -30,  -30,  637,  878,  -30,  407,  -30,  -30,  428,  -30,
 /*   180 */   -30,  -30,  109,  -30,  -30,  -30,  578,  -30,  -30,  -30,
 /*   190 */   -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,  -30,
 /*   200 */   -30,  -30,  -30,  -30,  -30,  730,  -30,  854,  -30,  878,
 /*   210 */   -30,  -30,  -30,  830,  -30,  878,  -30,  -30,  -30,  -30,
 /*   220 */   -30,  -30,  782,  -30,  878,  -30,  -30,  -30,  -30,  -30,
 /*   230 */   -30,  758,  -30,  878,  -30,  534,  123,  -30,  -30,  -30,
 /*   240 */   -30,  664,  -30,  -30,  -30,  -30,  707,  -30,  -30,  -30,
 /*   250 */   -30,  -30,  -30,  -30,  -30,  119,  -30,  -30,  -30,  -30,
 /*   260 */   -30,  273,  -30,  -30,  -30,  -30,  -30,  341,  -30,  -30,
 /*   270 */   -30,  -30,  -30,  398,  -30,  -30,  -30,  532,  -29,  123,
 /*   280 */   -30,  123,  -30,  123,  -30,  -30,  132,  -30,  -30,  -30,
 /*   290 */   -30,  -30,  -30,  101,  -30,  -30,  -30,  -30,  -30,  194,
 /*   300 */   -30,  -30,  -30,  -30,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   451,  451,  451,  306,  308,  309,  310,  451,  451,  311,
 /*    10 */   451,  451,  451,  451,  312,  316,  451,  451,  341,  451,
 /*    20 */   451,  451,  451,  451,  451,  345,  451,  349,  371,  373,
 /*    30 */   374,  375,  376,  377,  378,  379,  380,  381,  382,  451,
 /*    40 */   383,  451,  384,  451,  451,  451,  451,  387,  451,  451,
 /*    50 */   451,  388,  451,  451,  451,  392,  451,  451,  451,  451,
 /*    60 */   394,  451,  451,  395,  451,  396,  451,  451,  398,  451,
 /*    70 */   414,  451,  415,  416,  417,  418,  419,  420,  421,  451,
 /*    80 */   422,  451,  423,  451,  431,  451,  451,  433,  437,  439,
 /*    90 */   440,  441,  442,  443,  444,  434,  440,  439,  432,  451,
 /*   100 */   424,  451,  425,  451,  426,  451,  427,  429,  430,  428,
 /*   110 */   438,  451,  451,  451,  399,  451,  451,  451,  400,  397,
 /*   120 */   451,  451,  401,  451,  402,  451,  451,  403,  451,  404,
 /*   130 */   451,  451,  405,  451,  410,  412,  451,  413,  411,  451,
 /*   140 */   451,  406,  451,  451,  407,  451,  451,  408,  451,  451,
 /*   150 */   409,  450,  372,  393,  451,  445,  446,  451,  447,  451,
 /*   160 */   451,  448,  449,  391,  389,  385,  451,  451,  451,  390,
 /*   170 */   386,  451,  451,  451,  350,  451,  357,  359,  451,  362,
 /*   180 */   451,  360,  451,  363,  451,  361,  451,  364,  451,  451,
 /*   190 */   451,  451,  451,  451,  451,  451,  358,  451,  451,  451,
 /*   200 */   346,  451,  451,  451,  451,  451,  451,  451,  347,  451,
 /*   210 */   351,  451,  451,  451,  348,  451,  352,  451,  451,  451,
 /*   220 */   451,  451,  451,  354,  451,  356,  451,  451,  451,  451,
 /*   230 */   451,  451,  353,  451,  355,  451,  451,  339,  451,  451,
 /*   240 */   451,  451,  451,  342,  451,  451,  451,  451,  343,  344,
 /*   250 */   329,  451,  451,  451,  331,  451,  451,  334,  451,  451,
 /*   260 */   336,  451,  451,  338,  451,  451,  332,  451,  451,  333,
 /*   270 */   451,  451,  335,  451,  451,  337,  340,  451,  451,  451,
 /*   280 */   451,  451,  330,  451,  451,  451,  451,  451,  313,  317,
 /*   290 */   451,  451,  451,  451,  451,  314,  318,  451,  451,  451,
 /*   300 */   451,  315,  319,  307,
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
 /* 134 */ "xx_assign_expr ::= TRUE",
 /* 135 */ "xx_literal_expr ::= INTEGER",
 /* 136 */ "xx_literal_expr ::= STRING",
 /* 137 */ "xx_literal_expr ::= DOUBLE",
 /* 138 */ "xx_literal_expr ::= NULL",
 /* 139 */ "xx_literal_expr ::= FALSE",
 /* 140 */ "xx_literal_expr ::= TRUE",
 /* 141 */ "xx_eval_expr ::= xx_literal_expr",
 /* 142 */ "xx_eval_expr ::= xx_eval_expr EQUALS xx_eval_expr",
 /* 143 */ "xx_eval_expr ::= xx_eval_expr IDENTICAL xx_eval_expr",
 /* 144 */ "xx_eval_expr ::= PARENTHESES_OPEN xx_eval_expr PARENTHESES_CLOSE",
 /* 145 */ "xx_eval_expr ::= IDENTIFIER",
 /* 146 */ "xx_comment ::= COMMENT",
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
// 467 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1248 "parser.c"
      break;
    case 52:
// 480 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 1253 "parser.c"
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
// 476 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy175;
}
// 1617 "parser.c"
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
      case 141:
// 482 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 1640 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 486 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 1651 "parser.c"
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
// 490 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 1667 "parser.c"
        break;
      case 7:
// 506 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1676 "parser.c"
        break;
      case 8:
// 510 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 0, NULL);
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1686 "parser.c"
        break;
      case 9:
// 514 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy175, 0, yymsp[-3].minor.yy0);
  yy_destructor(10,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1697 "parser.c"
        break;
      case 10:
// 518 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 1, NULL);
  yy_destructor(14,&yymsp[-5].minor);
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1708 "parser.c"
        break;
      case 11:
// 522 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy175, 1, yymsp[-3].minor.yy0);
  yy_destructor(14,&yymsp[-7].minor);
  yy_destructor(10,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1720 "parser.c"
        break;
      case 12:
// 526 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1730 "parser.c"
        break;
      case 13:
// 530 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0);
  yy_destructor(10,&yymsp[-5].minor);
  yy_destructor(13,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1741 "parser.c"
        break;
      case 14:
// 534 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1752 "parser.c"
        break;
      case 15:
// 538 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0);
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(10,&yymsp[-5].minor);
  yy_destructor(13,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1764 "parser.c"
        break;
      case 16:
// 542 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL);
}
// 1771 "parser.c"
        break;
      case 17:
// 546 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175);
}
// 1778 "parser.c"
        break;
      case 18:
// 550 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL);
}
// 1785 "parser.c"
        break;
      case 19:
// 554 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL);
}
// 1792 "parser.c"
        break;
      case 20:
// 558 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175);
}
// 1799 "parser.c"
        break;
      case 21:
// 562 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175);
}
// 1806 "parser.c"
        break;
      case 22:
// 566 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175);
}
// 1813 "parser.c"
        break;
      case 23:
// 570 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175);
}
// 1820 "parser.c"
        break;
      case 24:
// 574 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175);
}
// 1827 "parser.c"
        break;
      case 27:
// 586 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1836 "parser.c"
        break;
      case 28:
// 590 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1845 "parser.c"
        break;
      case 29:
// 594 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL);
  yy_destructor(16,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1855 "parser.c"
        break;
      case 30:
// 598 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0);
  yy_destructor(16,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1865 "parser.c"
        break;
      case 31:
// 602 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1874 "parser.c"
        break;
      case 32:
// 606 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1883 "parser.c"
        break;
      case 33:
// 610 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1893 "parser.c"
        break;
      case 34:
// 614 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1903 "parser.c"
        break;
      case 38:
// 630 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1913 "parser.c"
        break;
      case 39:
// 634 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 1923 "parser.c"
        break;
      case 41:
// 642 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1936 "parser.c"
        break;
      case 42:
// 646 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1949 "parser.c"
        break;
      case 43:
// 650 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1962 "parser.c"
        break;
      case 44:
// 654 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1975 "parser.c"
        break;
      case 45:
// 658 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 1988 "parser.c"
        break;
      case 46:
// 662 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL);
  yy_destructor(16,&yymsp[-8].minor);
  yy_destructor(20,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2001 "parser.c"
        break;
      case 47:
// 666 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2014 "parser.c"
        break;
      case 48:
// 670 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0);
  yy_destructor(16,&yymsp[-8].minor);
  yy_destructor(20,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2027 "parser.c"
        break;
      case 49:
// 674 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2040 "parser.c"
        break;
      case 50:
// 678 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2053 "parser.c"
        break;
      case 51:
// 682 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL);
  yy_destructor(18,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2066 "parser.c"
        break;
      case 52:
// 686 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0);
  yy_destructor(18,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2079 "parser.c"
        break;
      case 53:
      case 92:
      case 106:
      case 123:
      case 127:
// 690 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2091 "parser.c"
        break;
      case 55:
// 698 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL);
}
// 2098 "parser.c"
        break;
      case 56:
// 702 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL);
}
// 2105 "parser.c"
        break;
      case 57:
// 706 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL);
}
// 2112 "parser.c"
        break;
      case 58:
// 710 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2120 "parser.c"
        break;
      case 59:
// 714 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2128 "parser.c"
        break;
      case 60:
// 718 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2136 "parser.c"
        break;
      case 61:
// 722 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[0].minor);
}
// 2145 "parser.c"
        break;
      case 62:
// 726 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(25,&yymsp[0].minor);
}
// 2153 "parser.c"
        break;
      case 63:
// 730 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(26,&yymsp[0].minor);
}
// 2161 "parser.c"
        break;
      case 64:
// 734 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(27,&yymsp[0].minor);
}
// 2169 "parser.c"
        break;
      case 65:
// 738 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(28,&yymsp[0].minor);
}
// 2177 "parser.c"
        break;
      case 66:
// 742 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(29,&yymsp[0].minor);
}
// 2185 "parser.c"
        break;
      case 79:
// 794 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement();
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2194 "parser.c"
        break;
      case 80:
// 798 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement();
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2203 "parser.c"
        break;
      case 81:
// 802 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2213 "parser.c"
        break;
      case 82:
// 806 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(11,&yymsp[-4].minor);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2226 "parser.c"
        break;
      case 83:
// 810 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2236 "parser.c"
        break;
      case 84:
// 814 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175);
  yy_destructor(32,&yymsp[-8].minor);
  yy_destructor(11,&yymsp[-6].minor);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2249 "parser.c"
        break;
      case 85:
// 818 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(11,&yymsp[-5].minor);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2262 "parser.c"
        break;
      case 86:
// 822 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(11,&yymsp[-5].minor);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2275 "parser.c"
        break;
      case 87:
// 826 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2285 "parser.c"
        break;
      case 88:
// 830 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2295 "parser.c"
        break;
      case 89:
// 834 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(11,&yymsp[-1].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2305 "parser.c"
        break;
      case 90:
// 838 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 2315 "parser.c"
        break;
      case 91:
// 842 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2324 "parser.c"
        break;
      case 94:
// 854 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2332 "parser.c"
        break;
      case 95:
// 858 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy175);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2341 "parser.c"
        break;
      case 96:
// 862 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2351 "parser.c"
        break;
      case 97:
// 866 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2360 "parser.c"
        break;
      case 98:
// 870 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0));
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2369 "parser.c"
        break;
      case 99:
// 874 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2378 "parser.c"
        break;
      case 100:
// 878 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0));
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2387 "parser.c"
        break;
      case 101:
// 882 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2396 "parser.c"
        break;
      case 102:
// 886 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2405 "parser.c"
        break;
      case 103:
// 890 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2414 "parser.c"
        break;
      case 104:
// 894 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2423 "parser.c"
        break;
      case 105:
// 898 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[0].minor);
}
// 2432 "parser.c"
        break;
      case 108:
// 910 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL);
}
// 2439 "parser.c"
        break;
      case 109:
// 914 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175);
  yy_destructor(17,&yymsp[-1].minor);
}
// 2447 "parser.c"
        break;
      case 110:
// 918 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL);
  yy_destructor(4,&yymsp[-1].minor);
}
// 2455 "parser.c"
        break;
      case 111:
// 922 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2463 "parser.c"
        break;
      case 112:
      case 126:
      case 145:
// 926 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0);
}
// 2472 "parser.c"
        break;
      case 113:
      case 132:
      case 135:
// 930 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0);
}
// 2481 "parser.c"
        break;
      case 114:
      case 131:
      case 136:
// 934 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0);
}
// 2490 "parser.c"
        break;
      case 115:
      case 137:
// 938 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0);
}
// 2498 "parser.c"
        break;
      case 116:
      case 138:
// 942 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL);
  yy_destructor(46,&yymsp[0].minor);
}
// 2507 "parser.c"
        break;
      case 117:
      case 139:
// 946 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL);
  yy_destructor(47,&yymsp[0].minor);
}
// 2516 "parser.c"
        break;
      case 118:
// 950 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2525 "parser.c"
        break;
      case 119:
// 954 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2534 "parser.c"
        break;
      case 120:
// 958 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL);
  yy_destructor(6,&yymsp[-1].minor);
}
// 2542 "parser.c"
        break;
      case 121:
// 962 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL);
  yy_destructor(6,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2552 "parser.c"
        break;
      case 122:
// 966 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175);
  yy_destructor(6,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2562 "parser.c"
        break;
      case 129:
// 994 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(48,&yymsp[-1].minor);
}
// 2570 "parser.c"
        break;
      case 130:
// 998 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175);
}
// 2577 "parser.c"
        break;
      case 134:
      case 140:
// 1014 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL);
  yy_destructor(49,&yymsp[0].minor);
}
// 2586 "parser.c"
        break;
      case 142:
// 1046 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL);
  yy_destructor(2,&yymsp[-1].minor);
}
// 2594 "parser.c"
        break;
      case 143:
// 1050 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL);
  yy_destructor(3,&yymsp[-1].minor);
}
// 2602 "parser.c"
        break;
      case 144:
// 1054 "parser.lemon"
{
	yygotominor.yy175 = yymsp[-1].minor.yy175;
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2611 "parser.c"
        break;
      case 146:
// 1062 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0);
}
// 2618 "parser.c"
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
// 451 "parser.lemon"


	//fprintf(stderr, "error!\n");

	if (status->scanner_state->start_length) {
		fprintf(stderr, "Syntax error, %s", status->scanner_state->start);
	} else {
		fprintf(stderr, "EOF");
	}

	//status->syntax_error_len = 48 + Z_STRLEN_P(status->scanner_state->active_file);
	//status->syntax_error = emalloc(sizeof(char) * status->syntax_error_len);

	status->status = XX_PARSING_FAILED;

// 2676 "parser.c"
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
 * Receives the comment tokenizes and parses it
 */
int xx_parse_annotations(){

	/*zval *error_msg = NULL;

	ZVAL_NULL(result);

	if (Z_TYPE_P(comment) != IS_STRING) {
		//phalcon_throw_exception_string(phalcon_annotations_exception_ce, SL("Comment must be a string") TSRMLS_CC);
		return FAILURE;
	}

	if(XX_internal_parse_annotations(&result, comment, file_path, line, &error_msg TSRMLS_CC) == FAILURE){
		//phalcon_throw_exception_string(phalcon_annotations_exception_ce, Z_STRVAL_P(error_msg), Z_STRLEN_P(error_msg) TSRMLS_CC);
		return FAILURE;
	}*/

	return SUCCESS;
}

/**
 * Parses a comment returning an intermediate array representation
 */
int xx_parse_program(char *program, unsigned int program_length) {

	char *error;
	xx_scanner_state *state;
	xx_scanner_token token;
	int scanner_status, status = SUCCESS, start_lines;
	xx_parser_status *parser_status = NULL;
	void* xx_parser;
	/*zval processed_comment;*/

	/**
	 * Check if the comment has content
	 */
	/*if (!Z_STRVAL_P(comment)) {
		ZVAL_BOOL(*result, 0);
		return FAILURE;
	}

	if (Z_STRLEN_P(comment) < 2) {
		ZVAL_BOOL(*result, 0);
		return SUCCESS;
	}*/

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
	//state->mode = XX_MODE_RAW;
	//state->active_file = file_path;

	/**
	 * Possible start line
	 */
	/*if (Z_TYPE_P(line) == IS_LONG) {
		state->active_line = Z_LVAL_P(line) - start_lines;
	} else {
		state->active_line = 1;
	}*/

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
				//if (!*error_msg) {
					//XX_scanner_error_msg(parser_status, error_msg TSRMLS_CC);
				//}
				fprintf(stderr, "scanner error\n");
				status = FAILURE;
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
		fprintf(stderr, "error!\n");
	}



	xx_Free(xx_parser, xx_wrapper_free);

	if (status != FAILURE) {
		if (parser_status->status == XX_PARSING_OK) {
			fprintf(stderr, "%s\n", json_object_to_json_string(parser_status->ret));
			/*if (parser_status->ret) {
				ZVAL_ZVAL(*result, parser_status->ret, 0, 0);
				ZVAL_NULL(parser_status->ret);
				zval_ptr_dtor(&parser_status->ret);
			} else {
				array_init(*result);
			}*/
		}
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
	int i;

	program = malloc(sizeof(char) * 10240);

	if (argc > 0) {

		fp = fopen(argv[1], "r");
		if (!fp) {
			fprintf(stderr, "Cant open file\n");
			exit(1);
		}

		i = 0;
		while (!feof(fp)) {
			ch = fgetc(fp);
			program[i++] = ch;
		}
		program[i - 1] = '\0';
		fclose(fp);

		//fprintf(stderr, "%s\n", program);

		xx_parse_program(program, i - 1);
	}
}
