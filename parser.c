/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 11 "parser.lemon"


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

static json_object *xx_ret_let_statement(xx_parser_token *D, xx_parser_token *T, json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("let"));
	if (D) {
		json_object_object_add(ret, "domain", json_object_new_string(D->token));
	}
	json_object_object_add(ret, "variable", json_object_new_string(T->token));
	json_object_object_add(ret, "expr", expr);

	return ret;
}

static json_object *xx_ret_if_statement(json_object *expr, json_object *statements, json_object *else_statements)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("if"));
	json_object_object_add(ret, "expr", expr);

	return ret;
}

static json_object *xx_ret_echo_statement(json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("echo"));
	json_object_object_add(ret, "expr", expr);

	return ret;
}

static json_object *xx_ret_declare_statement(int type, json_object *variables)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("declare"));
	json_object_object_add(ret, "data-type", json_object_new_int(type));
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

static json_object *xx_ret_comment(xx_parser_token *T)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("comment"));
	json_object_object_add(ret, "value", json_object_new_string(T->token));

	return ret;
}


// 320 "parser.c"
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
#define YYNOCODE 68
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy95;
  int yy135;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 231
#define YYNRULE 107
#define YYERRORSYMBOL 37
#define YYERRSYMDT yy135
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
 /*     0 */    38,   77,   75,   88,   13,   16,  204,  210,  209,  168,
 /*    10 */   203,  176,  181,   67,   76,   82,   79,   33,   34,   39,
 /*    20 */   182,   56,   85,  152,  102,  163,   88,  100,  164,  176,
 /*    30 */    99,   89,   29,   30,   31,   32,   67,   76,   82,   79,
 /*    40 */     7,   34,   39,   10,   56,   85,  150,  217,   88,   88,
 /*    50 */   227,   16,  204,  210,  209,  184,  203,  176,  102,   67,
 /*    60 */    76,   82,   79,   33,   34,   39,  231,   56,   85,  135,
 /*    70 */   206,  177,   88,  160,  176,  139,   55,   89,   29,   30,
 /*    80 */    31,   32,   67,   76,   82,   79,  187,   34,   39,   70,
 /*    90 */    56,   85,   25,   78,  188,   88,  339,    1,    2,  230,
 /*   100 */     4,    5,    6,   92,  102,   67,   76,   82,   79,   33,
 /*   110 */    34,   39,   91,   56,   85,  137,   93,   36,   88,  151,
 /*   120 */   129,  125,  144,   89,   29,   30,   31,   32,   67,   76,
 /*   130 */    82,   79,  167,   34,   39,  180,   56,   85,  101,   80,
 /*   140 */    75,   88,  221,   16,  204,  210,  209,  130,  203,  176,
 /*   150 */   193,   67,   76,   82,   79,   33,   34,   39,  194,   56,
 /*   160 */    85,  161,   59,   70,   88,  142,   42,   81,   92,   89,
 /*   170 */    29,   30,   31,   32,   67,   76,   82,   79,   21,   34,
 /*   180 */    39,   95,   56,   85,  141,   83,   75,   88,   92,   61,
 /*   190 */    62,   63,   64,   65,   66,  226,  104,   67,   76,   82,
 /*   200 */    79,   35,   34,   39,   70,   56,   85,  159,   84,  105,
 /*   210 */    88,  115,   54,  118,  119,  120,  121,  122,   18,  199,
 /*   220 */    67,   76,   82,   79,   33,   34,   39,  200,   56,   85,
 /*   230 */   143,   44,   53,   88,  136,   86,   75,  189,   89,   29,
 /*   240 */    30,   31,   32,   67,   76,   82,   79,  190,   34,   39,
 /*   250 */    70,   56,   85,   27,   87,   11,   88,   91,   45,   46,
 /*   260 */    47,   48,   49,   50,  130,  186,   67,   76,   82,   79,
 /*   270 */    33,   34,   39,   96,   56,   85,   90,  145,   70,   88,
 /*   280 */    37,   12,   69,  211,   89,   29,   30,   31,   32,   67,
 /*   290 */    76,   82,   79,   41,   34,   39,   97,   56,   85,   68,
 /*   300 */    75,   88,   61,   62,   63,   64,   65,   66,   51,   94,
 /*   310 */   192,   67,   76,   82,   79,   14,   34,   39,  104,   56,
 /*   320 */    85,  108,   20,   61,   62,   63,   64,   65,   66,  104,
 /*   330 */   132,  106,   23,  115,   74,  118,  119,  120,  121,  122,
 /*   340 */    22,   52,  104,  133,  115,   24,  118,  119,  120,  121,
 /*   350 */   122,  162,   17,  177,   33,  203,  176,  115,  109,  118,
 /*   360 */   119,  120,  121,  122,   26,  220,  195,  224,   89,   29,
 /*   370 */    30,   31,   32,  214,   16,  204,  210,  209,   33,  203,
 /*   380 */   176,   15,  158,  196,  178,  191,  198,  197,  171,  179,
 /*   390 */   157,  185,   28,   29,   30,   31,   32,  232,  154,  205,
 /*   400 */     7,  208,  209,   10,  164,  176,  216,  217,   88,  178,
 /*   410 */   191,  110,  197,  171,  129,  247,  144,  166,  178,  191,
 /*   420 */   228,  197,  171,  179,  223,  185,  166,  178,  191,  326,
 /*   430 */   197,  171,   43,  251,  156,  112,  165,   19,  154,  153,
 /*   440 */   171,  201,  248,  202,  155,  178,  191,  325,  197,  171,
 /*   450 */    72,  229,  113,  114,  178,  191,  324,  197,  171,  146,
 /*   460 */   249,  149,  148,  128,   19,  147,  153,  253,  126,   40,
 /*   470 */   128,   19,  124,  153,  123,  107,  111,  116,    3,    4,
 /*   480 */     5,    6,   98,  145,  123,  107,  111,  250,  323,  212,
 /*   490 */   128,   19,  117,  153,  138,  213,  123,  107,  111,  254,
 /*   500 */   140,  322,  128,   19,  292,  153,  252,  215,   73,  207,
 /*   510 */   191,  255,  197,  293,  128,   19,  127,  153,  294,  103,
 /*   520 */   107,  111,  218,  320,  219,  295,  131,  222,    9,  321,
 /*   530 */   296,  174,  134,  175,   57,   58,    8,  297,  172,  225,
 /*   540 */    71,  170,  173,   20,  169,   60,  176,  183,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     8,   65,   66,   11,   45,   46,   47,   48,   49,   13,
 /*    10 */    51,   52,    5,   21,   22,   23,   24,   44,   26,   27,
 /*    20 */    13,   29,   30,    8,    1,   48,   11,   54,   51,   52,
 /*    30 */     7,   58,   59,   60,   61,   62,   21,   22,   23,   24,
 /*    40 */     3,   26,   27,    6,   29,   30,    8,   10,   11,   11,
 /*    50 */    45,   46,   47,   48,   49,    5,   51,   52,    1,   21,
 /*    60 */    22,   23,   24,   44,   26,   27,    0,   29,   30,    8,
 /*    70 */    48,   49,   11,   54,   52,   18,    5,   58,   59,   60,
 /*    80 */    61,   62,   21,   22,   23,   24,    5,   26,   27,    1,
 /*    90 */    29,   30,    8,    5,   13,   11,   38,   39,   40,   41,
 /*   100 */    42,   43,   44,   50,    1,   21,   22,   23,   24,   44,
 /*   110 */    26,   27,    2,   29,   30,    8,   63,    7,   11,   54,
 /*   120 */    12,   18,   14,   58,   59,   60,   61,   62,   21,   22,
 /*   130 */    23,   24,    4,   26,   27,    4,   29,   30,    8,   65,
 /*   140 */    66,   11,   45,   46,   47,   48,   49,   16,   51,   52,
 /*   150 */     5,   21,   22,   23,   24,   44,   26,   27,   13,   29,
 /*   160 */    30,    8,    4,    1,   11,   54,   64,    5,   50,   58,
 /*   170 */    59,   60,   61,   62,   21,   22,   23,   24,    4,   26,
 /*   180 */    27,   63,   29,   30,    8,   65,   66,   11,   50,   31,
 /*   190 */    32,   33,   34,   35,   36,    7,    4,   21,   22,   23,
 /*   200 */    24,   63,   26,   27,    1,   29,   30,    8,    5,   13,
 /*   210 */    11,   19,   64,   21,   22,   23,   24,   25,   52,    5,
 /*   220 */    21,   22,   23,   24,   44,   26,   27,   13,   29,   30,
 /*   230 */     8,    4,   13,   11,   54,   65,   66,   50,   58,   59,
 /*   240 */    60,   61,   62,   21,   22,   23,   24,    5,   26,   27,
 /*   250 */     1,   29,   30,    8,    5,    4,   11,    2,   31,   32,
 /*   260 */    33,   34,   35,   36,   16,    4,   21,   22,   23,   24,
 /*   270 */    44,   26,   27,   18,   29,   30,    8,   16,    1,   11,
 /*   280 */    54,    7,    5,    9,   58,   59,   60,   61,   62,   21,
 /*   290 */    22,   23,   24,   13,   26,   27,    4,   29,   30,   65,
 /*   300 */    66,   11,   31,   32,   33,   34,   35,   36,   28,   17,
 /*   310 */     4,   21,   22,   23,   24,    8,   26,   27,    4,   29,
 /*   320 */    30,    4,   16,   31,   32,   33,   34,   35,   36,    4,
 /*   330 */    17,   50,   18,   19,   50,   21,   22,   23,   24,   25,
 /*   340 */    17,    4,    4,   18,   19,    7,   21,   22,   23,   24,
 /*   350 */    25,   47,   48,   49,   44,   51,   52,   19,   13,   21,
 /*   360 */    22,   23,   24,   25,   54,    7,   50,    9,   58,   59,
 /*   370 */    60,   61,   62,   45,   46,   47,   48,   49,   44,   51,
 /*   380 */    52,    8,    7,    5,   11,   12,    4,   14,   15,   12,
 /*   390 */    18,   14,   58,   59,   60,   61,   62,    0,   16,   46,
 /*   400 */     3,   48,   49,    6,   51,   52,    8,   10,   11,   11,
 /*   410 */    12,   50,   14,   15,   12,    8,   14,   15,   11,   12,
 /*   420 */     8,   14,   15,   12,    8,   14,   15,   11,   12,    5,
 /*   430 */    14,   15,    5,    8,   17,    4,   11,   12,   16,   14,
 /*   440 */    15,   50,    8,    5,    4,   11,   12,    5,   14,   15,
 /*   450 */     4,    8,   13,   50,   11,   12,    5,   14,   15,    4,
 /*   460 */     8,    7,   18,   11,   12,   17,   14,    8,    7,    4,
 /*   470 */    11,   12,   53,   14,   55,   56,   57,    4,   41,   42,
 /*   480 */    43,   44,   53,   16,   55,   56,   57,    8,    5,    4,
 /*   490 */    11,   12,   20,   14,   53,    7,   55,   56,   57,    8,
 /*   500 */     7,    5,   11,   12,    4,   14,    8,    8,   13,   11,
 /*   510 */    12,    8,   14,    4,   11,   12,    8,   14,    4,   55,
 /*   520 */    56,   57,    6,    5,    4,    4,    4,    8,    5,    5,
 /*   530 */     4,   50,    7,    5,   50,    5,    4,    4,    4,    4,
 /*   540 */    66,    5,   13,   16,   50,    5,   67,   50,
};
#define YY_SHIFT_USE_DFLT (-9)
static short yy_shift_ofst[] = {
 /*     0 */    37,   66,  397,   -9,   -9,   -9,   -9,  532,  523,   -9,
 /*    10 */   251,  274,  373,  307,   -9,   -9,  407,  479,   -9,  527,
 /*    20 */   174,  323,  314,  338,   84,   -9,  245,   -9,   -9,   -9,
 /*    30 */    -9,   -9,   -9,   -9,  292,  110,  268,   -8,   -9,  465,
 /*    40 */   280,  227,  427,   -9,  518,  524,  496,  483,  451,  442,
 /*    50 */   424,  337,  219,  227,   71,   -9,  158,  530,   -9,  540,
 /*    60 */    -9,   -9,   -9,   -9,   -9,   -9,   -9,  446,  277,   -9,
 /*    70 */   446,   -9,  495,  271,   -9,   -9,  446,   88,   -9,  446,
 /*    80 */   162,   -9,  446,  203,   -9,  446,  249,   -9,   -9,   -9,
 /*    90 */    -9,  292,   -9,   -9,  292,  255,   -9,   -9,   23,  290,
 /*   100 */   130,   -9,  192,   -9,  196,  271,   -9,  317,  345,  271,
 /*   110 */    -9,  431,  439,  271,   -9,  473,  472,  500,  509,  514,
 /*   120 */   521,  526,  533,   -9,  103,  461,  508,   -9,  108,  248,
 /*   130 */   522,  313,  325,  525,   61,   -9,  107,   -9,   57,  493,
 /*   140 */   176,   -9,  222,   -9,  467,  455,  448,  444,  454,   38,
 /*   150 */    -9,   15,   -9,  422,  440,  417,  372,  375,  199,   -9,
 /*   160 */   153,   -9,  425,  491,   -9,  402,  128,   -4,  271,  536,
 /*   170 */    -9,  534,  529,  271,  528,   -9,   -9,   -9,  411,  131,
 /*   180 */     7,   -9,  271,   50,   -9,  261,   81,   -9,  271,  242,
 /*   190 */    -9,  306,  145,   -9,  271,  378,   -9,  382,  214,   -9,
 /*   200 */   271,  438,   -9,   -9,  434,  498,  503,  377,  459,   -9,
 /*   210 */   452,  485,  488,  398,  499,   -9,   -9,  516,  520,  358,
 /*   220 */   416,  519,   -9,   -9,  535,  188,  443,  412,   -9,   -9,
 /*   230 */    -9,
};
#define YY_REDUCE_USE_DFLT (-65)
static short yy_reduce_ofst[] = {
 /*     0 */    58,  -65,  437,  -65,  -65,  -65,  -65,  -65,  -65,  -65,
 /*    10 */   -65,  -65,  -41,  -65,  -65,  -65,  304,  166,  -65,  -65,
 /*    20 */   -65,  -65,  419,  429,  310,  -65,  334,  -65,  -65,  -65,
 /*    30 */   -65,  -65,  -65,  -65,  138,  -65,  226,  334,  -65,  -65,
 /*    40 */   -65,  102,  -65,  -65,  -65,  -65,  -65,  -65,  -65,  -65,
 /*    50 */   -65,  -65,  -65,  148,  -65,  -65,  484,  -65,  -65,  -65,
 /*    60 */   -65,  -65,  -65,  -65,  -65,  -65,  -65,  234,  -65,  -65,
 /*    70 */   474,  -65,  -65,  284,  -65,  -65,  -64,  -65,  -65,   74,
 /*    80 */   -65,  -65,  120,  -65,  -65,  170,  -65,  -65,  -65,  -65,
 /*    90 */   -65,   53,  -65,  -65,  118,  -65,  -65,  -65,  -65,  -27,
 /*   100 */   334,  -65,  464,  -65,  -65,  281,  -65,  -65,  -65,  361,
 /*   110 */   -65,  -65,  -65,  403,  -65,  -65,  -65,  -65,  -65,  -65,
 /*   120 */   -65,  -65,  -65,  -65,  -65,  -65,  -65,  -65,  -65,  -65,
 /*   130 */   -65,  -65,  441,  -65,  180,  -65,  334,  -65,  -65,  -65,
 /*   140 */   111,  -65,  334,  -65,  -65,  -65,  -65,  -65,  -65,   65,
 /*   150 */   -65,  334,  -65,  -65,  -65,  -65,  -65,  -65,   19,  -65,
 /*   160 */   334,  -65,  -23,  166,  -65,  -65,  -65,  -65,  494,  -65,
 /*   170 */   -65,  -65,  -65,  481,  -65,  -65,  -65,  -65,  -65,  -65,
 /*   180 */   -65,  -65,  497,  -65,  -65,  -65,  -65,  -65,  187,  -65,
 /*   190 */   -65,  -65,  -65,  -65,  316,  -65,  -65,  -65,  -65,  -65,
 /*   200 */   391,  -65,  -65,  -65,  353,   22,  166,  -65,  166,  -65,
 /*   210 */   166,  -65,  -65,  328,  -65,  -65,  -65,  -65,  -65,  -65,
 /*   220 */    97,  -65,  -65,  -65,  -65,  -65,    5,  -65,  -65,  -65,
 /*   230 */   -65,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   338,  338,  338,  233,  235,  236,  237,  338,  338,  238,
 /*    10 */   338,  338,  338,  338,  239,  243,  338,  338,  268,  338,
 /*    20 */   338,  338,  338,  338,  338,  272,  338,  276,  298,  300,
 /*    30 */   301,  302,  303,  304,  338,  338,  338,  338,  306,  338,
 /*    40 */   338,  338,  338,  307,  338,  338,  338,  338,  338,  338,
 /*    50 */   338,  338,  338,  338,  338,  308,  338,  338,  309,  338,
 /*    60 */   310,  327,  328,  329,  330,  331,  332,  338,  338,  311,
 /*    70 */   338,  316,  318,  338,  319,  317,  338,  338,  312,  338,
 /*    80 */   338,  313,  338,  338,  314,  338,  338,  315,  337,  299,
 /*    90 */   305,  338,  333,  334,  338,  338,  335,  336,  338,  338,
 /*   100 */   338,  277,  338,  284,  286,  338,  289,  338,  287,  338,
 /*   110 */   290,  338,  288,  338,  291,  338,  338,  338,  338,  338,
 /*   120 */   338,  338,  338,  285,  338,  338,  338,  273,  338,  338,
 /*   130 */   338,  338,  338,  338,  338,  274,  338,  278,  338,  338,
 /*   140 */   338,  275,  338,  279,  338,  338,  338,  338,  338,  338,
 /*   150 */   281,  338,  283,  338,  338,  338,  338,  338,  338,  280,
 /*   160 */   338,  282,  338,  338,  266,  338,  338,  338,  338,  338,
 /*   170 */   269,  338,  338,  338,  338,  270,  271,  256,  338,  338,
 /*   180 */   338,  258,  338,  338,  261,  338,  338,  263,  338,  338,
 /*   190 */   265,  338,  338,  259,  338,  338,  260,  338,  338,  262,
 /*   200 */   338,  338,  264,  267,  338,  338,  338,  338,  338,  257,
 /*   210 */   338,  338,  338,  338,  338,  240,  244,  338,  338,  338,
 /*   220 */   338,  338,  241,  245,  338,  338,  338,  338,  242,  246,
 /*   230 */   234,
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
  "$",             "COMMA",         "EQUALS",        "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",
  "BRACKET_CLOSE",  "EXTENDS",       "ABSTRACT",      "COMMENT",     
  "PUBLIC",        "ASSIGN",        "PROTECTED",     "CONST",       
  "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",  "LESS",        
  "GREATER",       "TYPE_INTEGER",  "TYPE_DOUBLE",   "TYPE_BOOL",   
  "TYPE_STRING",   "TYPE_ARRAY",    "IF",            "LET",         
  "ARROW",         "ECHO",          "TYPE_VAR",      "INTEGER",     
  "STRING",        "DOUBLE",        "NULL",          "FALSE",       
  "TRUE",          "error",         "program",       "xx_language", 
  "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",
  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_literal_expr",  "xx_class_const_definition",
  "xx_class_method_definition",  "xx_parameter_list",  "xx_statement_list",  "xx_parameter",
  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",
  "xx_if_statement",  "xx_echo_statement",  "xx_declare_statement",  "xx_eval_expr",
  "xx_expr",       "xx_declare_variable_list",  "xx_declare_variable",
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
 /*  71 */ "xx_statement ::= xx_echo_statement",
 /*  72 */ "xx_statement ::= xx_declare_statement",
 /*  73 */ "xx_statement ::= xx_comment",
 /*  74 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  75 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  76 */ "xx_let_statement ::= LET IDENTIFIER ASSIGN xx_expr DOTCOMMA",
 /*  77 */ "xx_let_statement ::= LET IDENTIFIER ARROW IDENTIFIER ASSIGN xx_expr DOTCOMMA",
 /*  78 */ "xx_echo_statement ::= ECHO xx_literal_expr DOTCOMMA",
 /*  79 */ "xx_echo_statement ::= ECHO IDENTIFIER DOTCOMMA",
 /*  80 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /*  81 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /*  82 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /*  83 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /*  84 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /*  85 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /*  86 */ "xx_declare_variable_list ::= xx_declare_variable",
 /*  87 */ "xx_declare_variable ::= IDENTIFIER",
 /*  88 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  89 */ "xx_expr ::= IDENTIFIER",
 /*  90 */ "xx_expr ::= INTEGER",
 /*  91 */ "xx_expr ::= STRING",
 /*  92 */ "xx_expr ::= DOUBLE",
 /*  93 */ "xx_expr ::= NULL",
 /*  94 */ "xx_expr ::= FALSE",
 /*  95 */ "xx_expr ::= TRUE",
 /*  96 */ "xx_literal_expr ::= INTEGER",
 /*  97 */ "xx_literal_expr ::= STRING",
 /*  98 */ "xx_literal_expr ::= DOUBLE",
 /*  99 */ "xx_literal_expr ::= NULL",
 /* 100 */ "xx_literal_expr ::= FALSE",
 /* 101 */ "xx_literal_expr ::= TRUE",
 /* 102 */ "xx_eval_expr ::= xx_literal_expr",
 /* 103 */ "xx_eval_expr ::= xx_eval_expr EQUALS xx_eval_expr",
 /* 104 */ "xx_eval_expr ::= PARENTHESES_OPEN xx_eval_expr PARENTHESES_CLOSE",
 /* 105 */ "xx_eval_expr ::= IDENTIFIER",
 /* 106 */ "xx_comment ::= COMMENT",
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
// 339 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 949 "parser.c"
      break;
    case 39:
// 352 "parser.lemon"
{ json_object_put((yypminor->yy95)); }
// 954 "parser.c"
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
  { 38, 1 },
  { 39, 1 },
  { 40, 2 },
  { 40, 1 },
  { 41, 1 },
  { 41, 1 },
  { 41, 1 },
  { 42, 3 },
  { 43, 5 },
  { 43, 7 },
  { 43, 6 },
  { 43, 8 },
  { 43, 4 },
  { 43, 6 },
  { 43, 5 },
  { 43, 7 },
  { 45, 1 },
  { 45, 1 },
  { 45, 1 },
  { 45, 2 },
  { 45, 2 },
  { 45, 2 },
  { 45, 2 },
  { 45, 3 },
  { 45, 3 },
  { 46, 2 },
  { 46, 1 },
  { 49, 4 },
  { 49, 3 },
  { 49, 5 },
  { 49, 6 },
  { 49, 3 },
  { 49, 4 },
  { 49, 5 },
  { 49, 6 },
  { 47, 2 },
  { 47, 1 },
  { 48, 2 },
  { 51, 6 },
  { 51, 5 },
  { 48, 1 },
  { 52, 7 },
  { 52, 8 },
  { 52, 8 },
  { 52, 9 },
  { 52, 8 },
  { 52, 9 },
  { 52, 9 },
  { 52, 10 },
  { 52, 7 },
  { 52, 8 },
  { 52, 8 },
  { 52, 9 },
  { 53, 3 },
  { 53, 1 },
  { 55, 1 },
  { 55, 2 },
  { 55, 2 },
  { 55, 3 },
  { 55, 4 },
  { 55, 4 },
  { 57, 3 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
  { 54, 2 },
  { 54, 1 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 58, 1 },
  { 60, 4 },
  { 60, 5 },
  { 59, 5 },
  { 59, 7 },
  { 61, 3 },
  { 61, 3 },
  { 62, 3 },
  { 62, 3 },
  { 62, 3 },
  { 62, 3 },
  { 62, 3 },
  { 65, 3 },
  { 65, 1 },
  { 66, 1 },
  { 66, 3 },
  { 64, 1 },
  { 64, 1 },
  { 64, 1 },
  { 64, 1 },
  { 64, 1 },
  { 64, 1 },
  { 64, 1 },
  { 50, 1 },
  { 50, 1 },
  { 50, 1 },
  { 50, 1 },
  { 50, 1 },
  { 50, 1 },
  { 63, 1 },
  { 63, 3 },
  { 63, 3 },
  { 63, 1 },
  { 44, 1 },
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
// 348 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy95;
}
// 1278 "parser.c"
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
      case 102:
// 354 "parser.lemon"
{
	yygotominor.yy95 = yymsp[0].minor.yy95;
}
// 1294 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 358 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(yymsp[-1].minor.yy95, yymsp[0].minor.yy95);
}
// 1305 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 68:
      case 86:
// 362 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(NULL, yymsp[0].minor.yy95);
}
// 1318 "parser.c"
        break;
      case 7:
// 378 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_namespace(yymsp[-1].minor.yy0);
  yy_destructor(3,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1327 "parser.c"
        break;
      case 8:
// 382 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, 0, NULL);
  yy_destructor(6,&yymsp[-4].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1337 "parser.c"
        break;
      case 9:
// 386 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy95, 0, yymsp[-3].minor.yy0);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(9,&yymsp[-4].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1348 "parser.c"
        break;
      case 10:
// 390 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, 1, NULL);
  yy_destructor(10,&yymsp[-5].minor);
  yy_destructor(6,&yymsp[-4].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1359 "parser.c"
        break;
      case 11:
// 394 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy95, 1, yymsp[-3].minor.yy0);
  yy_destructor(10,&yymsp[-7].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(9,&yymsp[-4].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1371 "parser.c"
        break;
      case 12:
// 398 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL);
  yy_destructor(6,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1381 "parser.c"
        break;
      case 13:
// 402 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0);
  yy_destructor(6,&yymsp[-5].minor);
  yy_destructor(9,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1392 "parser.c"
        break;
      case 14:
// 406 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL);
  yy_destructor(10,&yymsp[-4].minor);
  yy_destructor(6,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1403 "parser.c"
        break;
      case 15:
// 410 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0);
  yy_destructor(10,&yymsp[-6].minor);
  yy_destructor(6,&yymsp[-5].minor);
  yy_destructor(9,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1415 "parser.c"
        break;
      case 16:
// 414 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[0].minor.yy95, NULL, NULL);
}
// 1422 "parser.c"
        break;
      case 17:
// 418 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy95);
}
// 1429 "parser.c"
        break;
      case 18:
// 422 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, yymsp[0].minor.yy95, NULL);
}
// 1436 "parser.c"
        break;
      case 19:
// 426 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, yymsp[0].minor.yy95, NULL);
}
// 1443 "parser.c"
        break;
      case 20:
// 430 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, NULL, yymsp[0].minor.yy95);
}
// 1450 "parser.c"
        break;
      case 21:
// 434 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[0].minor.yy95, NULL, yymsp[-1].minor.yy95);
}
// 1457 "parser.c"
        break;
      case 22:
// 438 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(NULL, yymsp[0].minor.yy95, yymsp[-1].minor.yy95);
}
// 1464 "parser.c"
        break;
      case 23:
// 442 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-2].minor.yy95, yymsp[0].minor.yy95, yymsp[-1].minor.yy95);
}
// 1471 "parser.c"
        break;
      case 24:
// 446 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_definition(yymsp[-1].minor.yy95, yymsp[0].minor.yy95, yymsp[-2].minor.yy95);
}
// 1478 "parser.c"
        break;
      case 27:
// 458 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0);
  yy_destructor(12,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1487 "parser.c"
        break;
      case 28:
// 462 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL);
  yy_destructor(12,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1496 "parser.c"
        break;
      case 29:
// 466 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, NULL);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1506 "parser.c"
        break;
      case 30:
// 470 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, yymsp[-5].minor.yy0);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1516 "parser.c"
        break;
      case 31:
// 474 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL);
  yy_destructor(14,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1525 "parser.c"
        break;
      case 32:
// 478 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0);
  yy_destructor(14,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1534 "parser.c"
        break;
      case 33:
// 482 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, NULL);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1544 "parser.c"
        break;
      case 34:
// 486 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, yymsp[-5].minor.yy0);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1554 "parser.c"
        break;
      case 38:
// 502 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, yymsp[-5].minor.yy0);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1564 "parser.c"
        break;
      case 39:
// 506 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy95, NULL);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1574 "parser.c"
        break;
      case 41:
// 514 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL);
  yy_destructor(12,&yymsp[-6].minor);
  yy_destructor(16,&yymsp[-5].minor);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1587 "parser.c"
        break;
      case 42:
// 518 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, NULL, NULL);
  yy_destructor(12,&yymsp[-7].minor);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1600 "parser.c"
        break;
      case 43:
// 522 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0);
  yy_destructor(12,&yymsp[-6].minor);
  yy_destructor(16,&yymsp[-5].minor);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1613 "parser.c"
        break;
      case 44:
// 526 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy95, NULL, yymsp[-8].minor.yy0);
  yy_destructor(12,&yymsp[-7].minor);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1626 "parser.c"
        break;
      case 45:
// 530 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, NULL);
  yy_destructor(12,&yymsp[-7].minor);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1639 "parser.c"
        break;
      case 46:
// 534 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, NULL);
  yy_destructor(12,&yymsp[-8].minor);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(17,&yymsp[-5].minor);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1652 "parser.c"
        break;
      case 47:
// 538 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, yymsp[-8].minor.yy0);
  yy_destructor(12,&yymsp[-7].minor);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1665 "parser.c"
        break;
      case 48:
// 542 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy95, yymsp[-1].minor.yy95, yymsp[-9].minor.yy0);
  yy_destructor(12,&yymsp[-8].minor);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(17,&yymsp[-5].minor);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1678 "parser.c"
        break;
      case 49:
// 546 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL);
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(16,&yymsp[-5].minor);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1691 "parser.c"
        break;
      case 50:
// 550 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0);
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(16,&yymsp[-5].minor);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1704 "parser.c"
        break;
      case 51:
// 554 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, NULL);
  yy_destructor(14,&yymsp[-7].minor);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1717 "parser.c"
        break;
      case 52:
// 558 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy95, yymsp[-8].minor.yy0);
  yy_destructor(14,&yymsp[-7].minor);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1730 "parser.c"
        break;
      case 53:
      case 85:
// 562 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_list(yymsp[-2].minor.yy95, yymsp[0].minor.yy95);
  yy_destructor(1,&yymsp[-1].minor);
}
// 1739 "parser.c"
        break;
      case 55:
// 570 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL);
}
// 1746 "parser.c"
        break;
      case 56:
// 574 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(yymsp[-1].minor.yy95, NULL, yymsp[0].minor.yy0, NULL);
}
// 1753 "parser.c"
        break;
      case 57:
// 578 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, yymsp[-1].minor.yy95, yymsp[0].minor.yy0, NULL);
}
// 1760 "parser.c"
        break;
      case 58:
// 582 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy95);
  yy_destructor(13,&yymsp[-1].minor);
}
// 1768 "parser.c"
        break;
      case 59:
// 586 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(yymsp[-3].minor.yy95, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy95);
  yy_destructor(13,&yymsp[-1].minor);
}
// 1776 "parser.c"
        break;
      case 60:
// 590 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_parameter(NULL, yymsp[-3].minor.yy95, yymsp[-2].minor.yy0, yymsp[0].minor.yy95);
  yy_destructor(13,&yymsp[-1].minor);
}
// 1784 "parser.c"
        break;
      case 61:
// 594 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 1793 "parser.c"
        break;
      case 62:
// 598 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(21,&yymsp[0].minor);
}
// 1801 "parser.c"
        break;
      case 63:
// 602 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(22,&yymsp[0].minor);
}
// 1809 "parser.c"
        break;
      case 64:
// 606 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(23,&yymsp[0].minor);
}
// 1817 "parser.c"
        break;
      case 65:
// 610 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(24,&yymsp[0].minor);
}
// 1825 "parser.c"
        break;
      case 66:
// 614 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(25,&yymsp[0].minor);
}
// 1833 "parser.c"
        break;
      case 74:
// 646 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-2].minor.yy95, NULL, NULL);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(7,&yymsp[-1].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1843 "parser.c"
        break;
      case 75:
// 650 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_if_statement(yymsp[-3].minor.yy95, yymsp[-1].minor.yy95, NULL);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(8,&yymsp[0].minor);
}
// 1853 "parser.c"
        break;
      case 76:
// 654 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_statement(NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1863 "parser.c"
        break;
      case 77:
// 658 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_let_statement(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy95);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(13,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1874 "parser.c"
        break;
      case 78:
// 662 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_echo_statement(yymsp[-1].minor.yy95);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1883 "parser.c"
        break;
      case 79:
// 666 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_echo_statement(xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0));
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1892 "parser.c"
        break;
      case 80:
// 670 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy95);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1901 "parser.c"
        break;
      case 81:
// 674 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy95);
  yy_destructor(22,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1910 "parser.c"
        break;
      case 82:
// 678 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy95);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1919 "parser.c"
        break;
      case 83:
// 682 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy95);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1928 "parser.c"
        break;
      case 84:
// 686 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy95);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 1937 "parser.c"
        break;
      case 87:
// 698 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL);
}
// 1944 "parser.c"
        break;
      case 88:
// 702 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy95);
  yy_destructor(13,&yymsp[-1].minor);
}
// 1952 "parser.c"
        break;
      case 89:
      case 105:
// 706 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0);
}
// 1960 "parser.c"
        break;
      case 90:
      case 96:
// 710 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0);
}
// 1968 "parser.c"
        break;
      case 91:
      case 97:
// 714 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0);
}
// 1976 "parser.c"
        break;
      case 92:
      case 98:
// 718 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0);
}
// 1984 "parser.c"
        break;
      case 93:
      case 99:
// 722 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_NULL, NULL);
  yy_destructor(34,&yymsp[0].minor);
}
// 1993 "parser.c"
        break;
      case 94:
      case 100:
// 726 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_FALSE, NULL);
  yy_destructor(35,&yymsp[0].minor);
}
// 2002 "parser.c"
        break;
      case 95:
      case 101:
// 730 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_literal(XX_T_TRUE, NULL);
  yy_destructor(36,&yymsp[0].minor);
}
// 2011 "parser.c"
        break;
      case 103:
// 762 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_expr("equals", yymsp[-2].minor.yy95, yymsp[0].minor.yy95, NULL);
  yy_destructor(2,&yymsp[-1].minor);
}
// 2019 "parser.c"
        break;
      case 104:
// 766 "parser.lemon"
{
	yygotominor.yy95 = yymsp[-1].minor.yy95;
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2028 "parser.c"
        break;
      case 106:
// 774 "parser.lemon"
{
	yygotominor.yy95 = xx_ret_comment(yymsp[0].minor.yy0);
}
// 2035 "parser.c"
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
// 323 "parser.lemon"


	//fprintf(stderr, "error!\n");

	if (status->scanner_state->start_length) {
		fprintf(stderr, "Syntax error, %s", status->scanner_state->start);
	} else {
		fprintf(stderr, "EOF");
	}

	//status->syntax_error_len = 48 + Z_STRLEN_P(status->scanner_state->active_file);
	//status->syntax_error = emalloc(sizeof(char) * status->syntax_error_len);

	status->status = XX_PARSING_FAILED;

// 2093 "parser.c"
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
			case XX_T_IF:
				xx_(xx_parser, XX_IF, NULL, parser_status);
				break;
			/*case XX_T_ELSE:
				xx_(xx_parser, XX_ELSE, NULL, parser_status);
				break;*/
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
			case XX_T_LESS:
				xx_(xx_parser, XX_LESS, NULL, parser_status);
				break;
			case XX_T_GREATER:
				xx_(xx_parser, XX_GREATER, NULL, parser_status);
				break;
			/*case XX_T_COLON:
				xx_(xx_parser, XX_COLON, NULL, parser_status);
				break;*/

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

			/*case XX_T_SBRACKET_OPEN:
				xx_(xx_parser, XX_SBRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_SBRACKET_CLOSE:
				xx_(xx_parser, XX_SBRACKET_CLOSE, NULL, parser_status);
				break;*/

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

	program = malloc(sizeof(char) * 2048);

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
