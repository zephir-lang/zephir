/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 12 "parser.lemon"


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

static json_object *xx_ret_let_statement(char *type, xx_parser_token *V, xx_parser_token *P, json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("let"));
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


// 378 "parser.c"
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
#define YYNOCODE 79
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy103;
  int yy157;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 263
#define YYNRULE 125
#define YYERRORSYMBOL 42
#define YYERRSYMDT yy157
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
 /*     0 */   184,   51,   78,  118,   68,   67,   62,   63,   64,  211,
 /*    10 */    65,  217,  198,   97,  106,  112,  109,   33,   34,   39,
 /*    20 */    74,   80,  102,   92,  115,  193,  195,  183,  118,  196,
 /*    30 */   208,  119,   29,   30,   31,   32,  121,  124,   97,  106,
 /*    40 */   112,  109,   33,   34,   39,  135,  139,  143,   92,  115,
 /*    50 */    27,  134,   26,  118,  128,  357,  119,   29,   30,   31,
 /*    60 */    32,   41,   21,   97,  106,  112,  109,  134,   34,   39,
 /*    70 */   157,   54,  141,   92,  115,   38,   82,   87,  118,  389,
 /*    80 */     1,    2,  262,    4,    5,    6,  171,   59,   97,  106,
 /*    90 */   112,  109,  372,   34,   39,  161,  186,  176,   92,  115,
 /*   100 */   169,   95,   53,  118,  259,   16,  236,  242,  241,   58,
 /*   110 */   235,  208,  213,   97,  106,  112,  109,  104,   34,   39,
 /*   120 */   214,   98,  105,   92,  115,  133,   79,  373,  118,  122,
 /*   130 */    60,   61,   62,   63,   64,   12,   65,  243,   97,  106,
 /*   140 */   112,  109,   35,   34,   39,  100,  107,  105,   92,  115,
 /*   150 */   173,   99,  219,  118,  122,   60,   61,   62,   63,   64,
 /*   160 */   220,   65,  375,   97,  106,  112,  109,  123,   34,   39,
 /*   170 */   100,  110,  105,   92,  115,   25,  108,  103,  118,  246,
 /*   180 */    16,  236,  242,  241,  225,  235,  208,  263,   97,  106,
 /*   190 */   112,  109,  226,   34,   39,  100,  100,  212,   92,  115,
 /*   200 */   191,  111,  114,  118,   13,   16,  236,  242,  241,  162,
 /*   210 */   235,  208,  136,   97,  106,  112,  109,   20,   34,   39,
 /*   220 */   113,  105,  218,   92,  115,  120,  215,  147,  118,  150,
 /*   230 */   151,  152,  153,  154,  177,  121,  124,  216,   97,  106,
 /*   240 */   112,  109,   36,   34,   39,  116,  105,  224,   92,  115,
 /*   250 */   167,   76,   78,  118,  253,   16,  236,  242,  241,   20,
 /*   260 */   235,  208,   42,   97,  106,  112,  109,   71,   34,   39,
 /*   270 */    75,   77,  181,   92,  115,  182,  142,  325,  118,   60,
 /*   280 */    61,   62,   63,   64,  161,   65,  176,  198,   97,  106,
 /*   290 */   112,  109,   43,   34,   39,  211,  100,  217,   92,  115,
 /*   300 */   175,  134,  117,  118,   68,   67,   62,   63,   64,  131,
 /*   310 */    65,  257,  258,   97,  106,  112,  109,  118,   34,   39,
 /*   320 */    70,  122,   44,   92,  115,   33,  180,   97,  106,  112,
 /*   330 */   109,  129,   34,   39,  125,  174,  361,   92,  115,  119,
 /*   340 */    29,   30,   31,   32,  126,  101,  122,   50,   72,   59,
 /*   350 */   222,   45,   46,   47,   48,   49,  221,   81,  136,  127,
 /*   360 */    60,   61,   62,   63,   64,  136,   65,   79,   24,   55,
 /*   370 */    56,   66,   23,  147,  136,  150,  151,  152,  153,  154,
 /*   380 */   147,   73,  150,  151,  152,  153,  154,  178,  165,  147,
 /*   390 */    33,  150,  151,  152,  153,  154,   60,   61,   62,   63,
 /*   400 */    64,  231,   65,   33,   28,   29,   30,   31,   32,  232,
 /*   410 */   179,   86,   33,  168,  252,  324,  256,  119,   29,   30,
 /*   420 */    31,   32,   37,   33,  230,  149,  119,   29,   30,   31,
 /*   430 */    32,  362,   33,  132,    8,  177,  186,  119,   29,   30,
 /*   440 */    31,   32,  192,  227,  228,  260,  119,   29,   30,   31,
 /*   450 */    32,   15,  172,   83,  210,  223,  264,  229,  203,  279,
 /*   460 */   144,    7,  210,  223,   10,  229,  203,  248,  249,  118,
 /*   470 */   210,  223,  280,  229,  203,  210,  223,  283,  229,  203,
 /*   480 */   197,   19,   59,  185,  203,  237,   18,  240,  241,  261,
 /*   490 */   196,  208,  210,  223,  255,  229,  203,  210,  223,   52,
 /*   500 */   229,  203,   69,   56,   66,  358,  363,  194,   17,  209,
 /*   510 */   353,  235,  208,   57,    7,   96,  130,   10,  155,  139,
 /*   520 */   143,  249,  118,  282,  233,  234,  160,   19,  170,  185,
 /*   530 */   155,  139,  143,  287,  145,   84,  160,   19,  285,  185,
 /*   540 */   166,  160,   19,  281,  185,  164,  160,   19,  284,  185,
 /*   550 */   163,  239,  223,  286,  229,   94,  160,   19,  156,  185,
 /*   560 */   155,  139,  143,    3,    4,    5,    6,   93,  238,  209,
 /*   570 */   162,  146,  208,  360,   85,   11,  244,  159,   14,  354,
 /*   580 */    91,   88,  329,  158,  245,    9,   89,  247,  355,  328,
 /*   590 */    90,  138,  250,  251,  137,  356,  148,  327,  199,   22,
 /*   600 */   359,  200,  201,  202,  190,  204,  326,  254,  206,  205,
 /*   610 */   207,  187,   40,  140,  205,  205,  205,  205,  189,  205,
 /*   620 */   188,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    10,   32,   55,   13,   35,   36,   37,   38,   39,   14,
 /*    10 */    41,   16,   17,   23,   24,   25,   26,   49,   28,   29,
 /*    20 */    73,   74,    6,   33,   34,   10,   53,   59,   13,   56,
 /*    30 */    57,   63,   64,   65,   66,   67,    2,    3,   23,   24,
 /*    40 */    25,   26,   49,   28,   29,   60,   61,   62,   33,   34,
 /*    50 */    10,    1,   59,   13,   20,    7,   63,   64,   65,   66,
 /*    60 */    67,   15,    6,   23,   24,   25,   26,    1,   28,   29,
 /*    70 */    20,    1,   15,   33,   34,   10,   30,   31,   13,   43,
 /*    80 */    44,   45,   46,   47,   48,   49,   20,   55,   23,   24,
 /*    90 */    25,   26,   40,   28,   29,   14,   18,   16,   33,   34,
 /*   100 */    10,    6,   32,   13,   50,   51,   52,   53,   54,   77,
 /*   110 */    56,   57,    7,   23,   24,   25,   26,   55,   28,   29,
 /*   120 */    15,   70,   71,   33,   34,   10,    6,   40,   13,   55,
 /*   130 */    35,   36,   37,   38,   39,    9,   41,   11,   23,   24,
 /*   140 */    25,   26,   68,   28,   29,    1,   70,   71,   33,   34,
 /*   150 */    10,    7,    7,   13,   55,   35,   36,   37,   38,   39,
 /*   160 */    15,   41,    7,   23,   24,   25,   26,   68,   28,   29,
 /*   170 */     1,   70,   71,   33,   34,   10,    7,   15,   13,   50,
 /*   180 */    51,   52,   53,   54,    7,   56,   57,    0,   23,   24,
 /*   190 */    25,   26,   15,   28,   29,    1,    1,    6,   33,   34,
 /*   200 */    10,    7,    7,   13,   50,   51,   52,   53,   54,   18,
 /*   210 */    56,   57,    6,   23,   24,   25,   26,   18,   28,   29,
 /*   220 */    70,   71,    6,   33,   34,   10,   55,   21,   13,   23,
 /*   230 */    24,   25,   26,   27,   18,    2,    3,    7,   23,   24,
 /*   240 */    25,   26,    9,   28,   29,   70,   71,    6,   33,   34,
 /*   250 */    10,    1,   55,   13,   50,   51,   52,   53,   54,   18,
 /*   260 */    56,   57,   69,   23,   24,   25,   26,    6,   28,   29,
 /*   270 */    20,   74,    9,   33,   34,   10,   55,    6,   13,   35,
 /*   280 */    36,   37,   38,   39,   14,   41,   16,   17,   23,   24,
 /*   290 */    25,   26,    7,   28,   29,   14,    1,   16,   33,   34,
 /*   300 */    10,    1,    7,   13,   35,   36,   37,   38,   39,    9,
 /*   310 */    41,    6,    9,   23,   24,   25,   26,   13,   28,   29,
 /*   320 */     4,   55,    6,   33,   34,   49,   20,   23,   24,   25,
 /*   330 */    26,    6,   28,   29,   68,   59,    7,   33,   34,   63,
 /*   340 */    64,   65,   66,   67,   19,   71,   55,   31,   19,   55,
 /*   350 */     7,   35,   36,   37,   38,   39,   55,   41,    6,   68,
 /*   360 */    35,   36,   37,   38,   39,    6,   41,    6,    9,   75,
 /*   370 */    76,   77,   20,   21,    6,   23,   24,   25,   26,   27,
 /*   380 */    21,   20,   23,   24,   25,   26,   27,    6,   20,   21,
 /*   390 */    49,   23,   24,   25,   26,   27,   35,   36,   37,   38,
 /*   400 */    39,    7,   41,   49,   63,   64,   65,   66,   67,   15,
 /*   410 */    19,    7,   49,   59,    9,    6,   11,   63,   64,   65,
 /*   420 */    66,   67,   59,   49,    6,   22,   63,   64,   65,   66,
 /*   430 */    67,    7,   49,   59,    6,   18,   18,   63,   64,   65,
 /*   440 */    66,   67,   59,   55,    7,   10,   63,   64,   65,   66,
 /*   450 */    67,   10,    9,    6,   13,   14,    0,   16,   17,   10,
 /*   460 */     6,    5,   13,   14,    8,   16,   17,   10,   12,   13,
 /*   470 */    13,   14,   10,   16,   17,   13,   14,   10,   16,   17,
 /*   480 */    13,   14,   55,   16,   17,   51,   57,   53,   54,   10,
 /*   490 */    56,   57,   13,   14,   10,   16,   17,   13,   14,   72,
 /*   500 */    16,   17,   75,   76,   77,    7,    7,   52,   53,   54,
 /*   510 */     7,   56,   57,   40,    5,    7,   58,    8,   60,   61,
 /*   520 */    62,   12,   13,   10,   55,    7,   13,   14,   58,   16,
 /*   530 */    60,   61,   62,   10,   15,   15,   13,   14,   10,   16,
 /*   540 */     9,   13,   14,   10,   16,   19,   13,   14,   10,   16,
 /*   550 */     6,   13,   14,   10,   16,    7,   13,   14,   58,   16,
 /*   560 */    60,   61,   62,   46,   47,   48,   49,   55,   53,   54,
 /*   570 */    18,   55,   57,    7,   69,    6,    6,   10,   10,    7,
 /*   580 */     7,   32,    6,    9,    9,    7,   15,   10,    7,    6,
 /*   590 */    69,   55,    8,    6,   15,    7,    6,    6,    6,   19,
 /*   600 */     7,   15,   55,    7,    9,    6,    6,   10,   55,   15,
 /*   610 */     7,    6,    6,    6,   78,   78,   78,   78,   20,   78,
 /*   620 */    19,
};
#define YY_SHIFT_USE_DFLT (-32)
static short yy_shift_ofst[] = {
 /*     0 */   509,  187,  456,  -32,  -32,  -32,  -32,  428,  578,  -32,
 /*    10 */   569,  126,  441,  568,  -32,  -32,  449,  513,  -32,  199,
 /*    20 */    56,  580,  352,  359,  165,  -32,   40,  -32,  -32,  -32,
 /*    30 */   -32,  -32,  -32,  -32,  325,  233,  215,   65,  -32,  606,
 /*    40 */    46,  316,  285,  -32,  503,  572,  581,  588,   48,  498,
 /*    50 */   -31,  593,   70,  566,  269,  -32,  473,  244,  -32,  -32,
 /*    60 */   -32,  -32,  -32,  -32,  -32,  -32,  -32,   52,   87,  -32,
 /*    70 */   261,  329,  361,  424,  250,  499,  120,  -32,  -32,  -32,
 /*    80 */   -32,  155,  447,  520,  316,  404,  -32,  549,  571,  316,
 /*    90 */   573,  -32,   95,  548,  -32,  508,  -32,   16,  144,  -32,
 /*   100 */    16,  -32,  162,  244,  -32,  -32,   16,  169,  -32,   16,
 /*   110 */   194,  -32,   16,  195,  -32,   16,  295,  -32,  -32,  -32,
 /*   120 */   -32,  325,  -32,  -32,  325,  -32,  325,   34,  -32,  -32,
 /*   130 */   300,  304,  115,  -32,  206,  -32,  579,  244,  -32,  607,
 /*   140 */    57,  244,  -32,  454,  519,  244,  -32,  590,  403,  409,
 /*   150 */   271,  600,  591,  583,  576,  -32,   50,  574,  567,  -32,
 /*   160 */    81,  552,  544,  526,  368,  531,  240,  -32,   90,  -32,
 /*   170 */    66,  443,  140,  -32,  290,  -32,  417,  381,  391,  306,
 /*   180 */   263,  265,  -32,  -10,  -32,   78,  605,  601,  598,  595,
 /*   190 */   190,  -32,   15,  -32,  467,  543,  -32,  270,  592,  586,
 /*   200 */   244,  596,  -32,  599,  594,  244,  603,  -32,  -32,  -32,
 /*   210 */    -5,  191,  105,  -32,  244,  230,  -32,  216,  145,  -32,
 /*   220 */   244,  343,  -32,  241,  177,  -32,  244,  437,  -32,  418,
 /*   230 */   394,  -32,  244,  518,  -32,  -32,  462,  538,  523,  281,
 /*   240 */   528,  -32,  533,  570,  575,  457,  577,  -32,  -32,  584,
 /*   250 */   587,  405,  484,  597,  -32,  -32,  305,  303,  479,  435,
 /*   260 */   -32,  -32,  -32,
};
#define YY_REDUCE_USE_DFLT (-54)
static short yy_reduce_ofst[] = {
 /*     0 */    36,  -54,  517,  -54,  -54,  -54,  -54,  -54,  -54,  -54,
 /*    10 */   -54,  -54,  154,  -54,  -54,  -54,  455,  429,  -54,  -54,
 /*    20 */   -54,  -54,  500,  458,   -7,  -54,  341,  -54,  -54,  -54,
 /*    30 */   -54,  -54,  -54,  -54,   74,  -54,  363,  341,  -54,  -54,
 /*    40 */   -54,  193,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,
 /*    50 */   427,  -54,  -54,  -54,  294,  -54,  -54,   32,  -54,  -54,
 /*    60 */   -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,
 /*    70 */   -54,  -54,  -53,  -54,  -54,  -54,  197,  -54,  -54,  -54,
 /*    80 */   -54,  -54,  -54,  -54,  505,  -54,  -54,  -54,  -54,  521,
 /*    90 */   -54,  -54,  512,  -54,  -54,  -54,  -54,   51,  -54,  -54,
 /*   100 */   274,  -54,  -54,   62,  -54,  -54,   76,  -54,  -54,  101,
 /*   110 */   -54,  -54,  150,  -54,  -54,  175,  -54,  -54,  -54,  -54,
 /*   120 */   -54,   99,  -54,  -54,  266,  -54,  291,  -54,  -54,  -54,
 /*   130 */   -54,  374,  341,  -54,  -15,  -54,  -54,  536,  -54,  -54,
 /*   140 */   -54,  221,  -54,  -54,  -54,  516,  -54,  -54,  -54,  -54,
 /*   150 */   -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,  -54,
 /*   160 */   -54,  -54,  -54,  -54,  470,  -54,  354,  -54,  341,  -54,
 /*   170 */   -54,  -54,  276,  -54,  341,  -54,  -54,  -54,  -54,  -54,
 /*   180 */   -54,  -32,  -54,  341,  -54,  -54,  -54,  -54,  -54,  -54,
 /*   190 */   383,  -54,  341,  -54,  -27,  429,  -54,  -54,  -54,  -54,
 /*   200 */   547,  -54,  -54,  -54,  -54,  553,  -54,  -54,  -54,  -54,
 /*   210 */   -54,  -54,  -54,  -54,  171,  -54,  -54,  -54,  -54,  -54,
 /*   220 */   301,  -54,  -54,  -54,  -54,  -54,  388,  -54,  -54,  -54,
 /*   230 */   -54,  -54,  469,  -54,  -54,  -54,  434,  515,  429,  -54,
 /*   240 */   429,  -54,  429,  -54,  -54,  129,  -54,  -54,  -54,  -54,
 /*   250 */   -54,  -54,  204,  -54,  -54,  -54,  -54,  -54,   54,  -54,
 /*   260 */   -54,  -54,  -54,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   388,  388,  388,  265,  267,  268,  269,  388,  388,  270,
 /*    10 */   388,  388,  388,  388,  271,  275,  388,  388,  300,  388,
 /*    20 */   388,  388,  388,  388,  388,  304,  388,  308,  330,  332,
 /*    30 */   333,  334,  335,  336,  388,  388,  388,  388,  338,  388,
 /*    40 */   388,  388,  388,  339,  388,  388,  388,  388,  388,  388,
 /*    50 */   388,  388,  388,  388,  388,  368,  388,  388,  370,  374,
 /*    60 */   376,  377,  378,  379,  380,  381,  371,  377,  376,  369,
 /*    70 */   388,  388,  388,  388,  388,  388,  388,  364,  366,  367,
 /*    80 */   365,  388,  388,  388,  388,  388,  340,  388,  388,  388,
 /*    90 */   388,  341,  388,  388,  342,  388,  343,  388,  388,  344,
 /*   100 */   388,  349,  351,  388,  352,  350,  388,  388,  345,  388,
 /*   110 */   388,  346,  388,  388,  347,  388,  388,  348,  387,  331,
 /*   120 */   337,  388,  382,  383,  388,  384,  388,  388,  385,  386,
 /*   130 */   388,  388,  388,  309,  388,  316,  318,  388,  321,  388,
 /*   140 */   319,  388,  322,  388,  320,  388,  323,  388,  388,  388,
 /*   150 */   388,  388,  388,  388,  388,  317,  388,  388,  388,  305,
 /*   160 */   388,  388,  388,  388,  388,  388,  388,  306,  388,  310,
 /*   170 */   388,  388,  388,  307,  388,  311,  388,  388,  388,  388,
 /*   180 */   388,  388,  313,  388,  315,  388,  388,  388,  388,  388,
 /*   190 */   388,  312,  388,  314,  388,  388,  298,  388,  388,  388,
 /*   200 */   388,  388,  301,  388,  388,  388,  388,  302,  303,  288,
 /*   210 */   388,  388,  388,  290,  388,  388,  293,  388,  388,  295,
 /*   220 */   388,  388,  297,  388,  388,  291,  388,  388,  292,  388,
 /*   230 */   388,  294,  388,  388,  296,  299,  388,  388,  388,  388,
 /*   240 */   388,  289,  388,  388,  388,  388,  388,  272,  276,  388,
 /*   250 */   388,  388,  388,  388,  273,  277,  388,  388,  388,  388,
 /*   260 */   274,  278,  266,
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
  "NEW",           "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",  "EXTENDS",     
  "ABSTRACT",      "COMMENT",       "PUBLIC",        "ASSIGN",      
  "PROTECTED",     "CONST",         "FUNCTION",      "PARENTHESES_OPEN",
  "PARENTHESES_CLOSE",  "LESS",          "GREATER",       "TYPE_INTEGER",
  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",  
  "IF",            "LET",           "ARROW",         "SBRACKET_OPEN",
  "SBRACKET_CLOSE",  "ECHO",          "TYPE_VAR",      "INTEGER",     
  "STRING",        "DOUBLE",        "NULL",          "FALSE",       
  "COLON",         "TRUE",          "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_literal_expr",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",  "xx_statement_list",
  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",
  "xx_let_statement",  "xx_if_statement",  "xx_echo_statement",  "xx_declare_statement",
  "xx_eval_expr",  "xx_assign_expr",  "xx_declare_variable_list",  "xx_declare_variable",
  "xx_array_list",  "xx_call_parameters",  "xx_call_parameter",  "xx_array_item",
  "xx_array_key",  "xx_array_value",
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
 /*  76 */ "xx_let_statement ::= LET IDENTIFIER ASSIGN xx_assign_expr DOTCOMMA",
 /*  77 */ "xx_let_statement ::= LET IDENTIFIER ARROW IDENTIFIER ASSIGN xx_assign_expr DOTCOMMA",
 /*  78 */ "xx_let_statement ::= LET IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr DOTCOMMA",
 /*  79 */ "xx_echo_statement ::= ECHO xx_literal_expr DOTCOMMA",
 /*  80 */ "xx_echo_statement ::= ECHO IDENTIFIER DOTCOMMA",
 /*  81 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /*  82 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /*  83 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /*  84 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /*  85 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /*  86 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /*  87 */ "xx_declare_variable_list ::= xx_declare_variable",
 /*  88 */ "xx_declare_variable ::= IDENTIFIER",
 /*  89 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  90 */ "xx_assign_expr ::= IDENTIFIER",
 /*  91 */ "xx_assign_expr ::= INTEGER",
 /*  92 */ "xx_assign_expr ::= STRING",
 /*  93 */ "xx_assign_expr ::= DOUBLE",
 /*  94 */ "xx_assign_expr ::= NULL",
 /*  95 */ "xx_assign_expr ::= FALSE",
 /*  96 */ "xx_assign_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /*  97 */ "xx_assign_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /*  98 */ "xx_assign_expr ::= NEW IDENTIFIER",
 /*  99 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 100 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 101 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 102 */ "xx_call_parameters ::= xx_call_parameter",
 /* 103 */ "xx_call_parameter ::= xx_literal_expr",
 /* 104 */ "xx_call_parameter ::= IDENTIFIER",
 /* 105 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 106 */ "xx_array_list ::= xx_array_item",
 /* 107 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 108 */ "xx_array_item ::= xx_array_value",
 /* 109 */ "xx_array_key ::= STRING",
 /* 110 */ "xx_array_key ::= INTEGER",
 /* 111 */ "xx_array_value ::= xx_literal_expr",
 /* 112 */ "xx_assign_expr ::= TRUE",
 /* 113 */ "xx_literal_expr ::= INTEGER",
 /* 114 */ "xx_literal_expr ::= STRING",
 /* 115 */ "xx_literal_expr ::= DOUBLE",
 /* 116 */ "xx_literal_expr ::= NULL",
 /* 117 */ "xx_literal_expr ::= FALSE",
 /* 118 */ "xx_literal_expr ::= TRUE",
 /* 119 */ "xx_eval_expr ::= xx_literal_expr",
 /* 120 */ "xx_eval_expr ::= xx_eval_expr EQUALS xx_eval_expr",
 /* 121 */ "xx_eval_expr ::= xx_eval_expr IDENTICAL xx_eval_expr",
 /* 122 */ "xx_eval_expr ::= PARENTHESES_OPEN xx_eval_expr PARENTHESES_CLOSE",
 /* 123 */ "xx_eval_expr ::= IDENTIFIER",
 /* 124 */ "xx_comment ::= COMMENT",
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
// 398 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1058 "parser.c"
      break;
    case 44:
// 411 "parser.lemon"
{ json_object_put((yypminor->yy103)); }
// 1063 "parser.c"
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
  { 43, 1 },
  { 44, 1 },
  { 45, 2 },
  { 45, 1 },
  { 46, 1 },
  { 46, 1 },
  { 46, 1 },
  { 47, 3 },
  { 48, 5 },
  { 48, 7 },
  { 48, 6 },
  { 48, 8 },
  { 48, 4 },
  { 48, 6 },
  { 48, 5 },
  { 48, 7 },
  { 50, 1 },
  { 50, 1 },
  { 50, 1 },
  { 50, 2 },
  { 50, 2 },
  { 50, 2 },
  { 50, 2 },
  { 50, 3 },
  { 50, 3 },
  { 51, 2 },
  { 51, 1 },
  { 54, 4 },
  { 54, 3 },
  { 54, 5 },
  { 54, 6 },
  { 54, 3 },
  { 54, 4 },
  { 54, 5 },
  { 54, 6 },
  { 52, 2 },
  { 52, 1 },
  { 53, 2 },
  { 56, 6 },
  { 56, 5 },
  { 53, 1 },
  { 57, 7 },
  { 57, 8 },
  { 57, 8 },
  { 57, 9 },
  { 57, 8 },
  { 57, 9 },
  { 57, 9 },
  { 57, 10 },
  { 57, 7 },
  { 57, 8 },
  { 57, 8 },
  { 57, 9 },
  { 58, 3 },
  { 58, 1 },
  { 60, 1 },
  { 60, 2 },
  { 60, 2 },
  { 60, 3 },
  { 60, 4 },
  { 60, 4 },
  { 62, 3 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 61, 1 },
  { 59, 2 },
  { 59, 1 },
  { 63, 1 },
  { 63, 1 },
  { 63, 1 },
  { 63, 1 },
  { 63, 1 },
  { 65, 4 },
  { 65, 5 },
  { 64, 5 },
  { 64, 7 },
  { 64, 7 },
  { 66, 3 },
  { 66, 3 },
  { 67, 3 },
  { 67, 3 },
  { 67, 3 },
  { 67, 3 },
  { 67, 3 },
  { 70, 3 },
  { 70, 1 },
  { 71, 1 },
  { 71, 3 },
  { 69, 1 },
  { 69, 1 },
  { 69, 1 },
  { 69, 1 },
  { 69, 1 },
  { 69, 1 },
  { 69, 2 },
  { 69, 3 },
  { 69, 2 },
  { 69, 4 },
  { 69, 5 },
  { 73, 3 },
  { 73, 1 },
  { 74, 1 },
  { 74, 1 },
  { 72, 3 },
  { 72, 1 },
  { 75, 3 },
  { 75, 1 },
  { 76, 1 },
  { 76, 1 },
  { 77, 1 },
  { 69, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 55, 1 },
  { 68, 1 },
  { 68, 3 },
  { 68, 3 },
  { 68, 3 },
  { 68, 1 },
  { 49, 1 },
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
// 407 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy103;
}
// 1405 "parser.c"
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
      case 103:
      case 111:
      case 119:
// 413 "parser.lemon"
{
	yygotominor.yy103 = yymsp[0].minor.yy103;
}
// 1423 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 417 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_list(yymsp[-1].minor.yy103, yymsp[0].minor.yy103);
}
// 1434 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 68:
      case 87:
      case 102:
      case 106:
// 421 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_list(NULL, yymsp[0].minor.yy103);
}
// 1449 "parser.c"
        break;
      case 7:
// 437 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_namespace(yymsp[-1].minor.yy0);
  yy_destructor(5,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1458 "parser.c"
        break;
      case 8:
// 441 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, 0, NULL);
  yy_destructor(8,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1468 "parser.c"
        break;
      case 9:
// 445 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy103, 0, yymsp[-3].minor.yy0);
  yy_destructor(8,&yymsp[-6].minor);
  yy_destructor(11,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1479 "parser.c"
        break;
      case 10:
// 449 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, 1, NULL);
  yy_destructor(12,&yymsp[-5].minor);
  yy_destructor(8,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1490 "parser.c"
        break;
      case 11:
// 453 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy103, 1, yymsp[-3].minor.yy0);
  yy_destructor(12,&yymsp[-7].minor);
  yy_destructor(8,&yymsp[-6].minor);
  yy_destructor(11,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1502 "parser.c"
        break;
      case 12:
// 457 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1512 "parser.c"
        break;
      case 13:
// 461 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0);
  yy_destructor(8,&yymsp[-5].minor);
  yy_destructor(11,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1523 "parser.c"
        break;
      case 14:
// 465 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1534 "parser.c"
        break;
      case 15:
// 469 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0);
  yy_destructor(12,&yymsp[-6].minor);
  yy_destructor(8,&yymsp[-5].minor);
  yy_destructor(11,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1546 "parser.c"
        break;
      case 16:
// 473 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[0].minor.yy103, NULL, NULL);
}
// 1553 "parser.c"
        break;
      case 17:
// 477 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy103);
}
// 1560 "parser.c"
        break;
      case 18:
// 481 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, yymsp[0].minor.yy103, NULL);
}
// 1567 "parser.c"
        break;
      case 19:
// 485 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, yymsp[0].minor.yy103, NULL);
}
// 1574 "parser.c"
        break;
      case 20:
// 489 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, NULL, yymsp[0].minor.yy103);
}
// 1581 "parser.c"
        break;
      case 21:
// 493 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[0].minor.yy103, NULL, yymsp[-1].minor.yy103);
}
// 1588 "parser.c"
        break;
      case 22:
// 497 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, yymsp[0].minor.yy103, yymsp[-1].minor.yy103);
}
// 1595 "parser.c"
        break;
      case 23:
// 501 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, yymsp[-1].minor.yy103);
}
// 1602 "parser.c"
        break;
      case 24:
// 505 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, yymsp[0].minor.yy103, yymsp[-2].minor.yy103);
}
// 1609 "parser.c"
        break;
      case 27:
// 517 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0);
  yy_destructor(14,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1618 "parser.c"
        break;
      case 28:
// 521 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL);
  yy_destructor(14,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1627 "parser.c"
        break;
      case 29:
// 525 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1637 "parser.c"
        break;
      case 30:
// 529 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1647 "parser.c"
        break;
      case 31:
// 533 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1656 "parser.c"
        break;
      case 32:
// 537 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1665 "parser.c"
        break;
      case 33:
// 541 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL);
  yy_destructor(16,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1675 "parser.c"
        break;
      case 34:
// 545 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0);
  yy_destructor(16,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1685 "parser.c"
        break;
      case 38:
// 561 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1695 "parser.c"
        break;
      case 39:
// 565 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1705 "parser.c"
        break;
      case 41:
// 573 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL);
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1718 "parser.c"
        break;
      case 42:
// 577 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, NULL, NULL);
  yy_destructor(14,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1731 "parser.c"
        break;
      case 43:
// 581 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0);
  yy_destructor(14,&yymsp[-6].minor);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1744 "parser.c"
        break;
      case 44:
// 585 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, NULL, yymsp[-8].minor.yy0);
  yy_destructor(14,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1757 "parser.c"
        break;
      case 45:
// 589 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103, NULL);
  yy_destructor(14,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1770 "parser.c"
        break;
      case 46:
// 593 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, NULL);
  yy_destructor(14,&yymsp[-8].minor);
  yy_destructor(18,&yymsp[-7].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1783 "parser.c"
        break;
      case 47:
// 597 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103, yymsp[-8].minor.yy0);
  yy_destructor(14,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1796 "parser.c"
        break;
      case 48:
// 601 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy103, yymsp[-1].minor.yy103, yymsp[-9].minor.yy0);
  yy_destructor(14,&yymsp[-8].minor);
  yy_destructor(18,&yymsp[-7].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1809 "parser.c"
        break;
      case 49:
// 605 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1822 "parser.c"
        break;
      case 50:
// 609 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0);
  yy_destructor(16,&yymsp[-6].minor);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1835 "parser.c"
        break;
      case 51:
// 613 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103, NULL);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1848 "parser.c"
        break;
      case 52:
// 617 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103, yymsp[-8].minor.yy0);
  yy_destructor(16,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1861 "parser.c"
        break;
      case 53:
      case 86:
      case 101:
      case 105:
// 621 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_list(yymsp[-2].minor.yy103, yymsp[0].minor.yy103);
  yy_destructor(1,&yymsp[-1].minor);
}
// 1872 "parser.c"
        break;
      case 55:
// 629 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL);
}
// 1879 "parser.c"
        break;
      case 56:
// 633 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(yymsp[-1].minor.yy103, NULL, yymsp[0].minor.yy0, NULL);
}
// 1886 "parser.c"
        break;
      case 57:
// 637 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, yymsp[-1].minor.yy103, yymsp[0].minor.yy0, NULL);
}
// 1893 "parser.c"
        break;
      case 58:
// 641 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy103);
  yy_destructor(15,&yymsp[-1].minor);
}
// 1901 "parser.c"
        break;
      case 59:
// 645 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(yymsp[-3].minor.yy103, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy103);
  yy_destructor(15,&yymsp[-1].minor);
}
// 1909 "parser.c"
        break;
      case 60:
// 649 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, yymsp[-3].minor.yy103, yymsp[-2].minor.yy0, yymsp[0].minor.yy103);
  yy_destructor(15,&yymsp[-1].minor);
}
// 1917 "parser.c"
        break;
      case 61:
// 653 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 1926 "parser.c"
        break;
      case 62:
// 657 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(23,&yymsp[0].minor);
}
// 1934 "parser.c"
        break;
      case 63:
// 661 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(24,&yymsp[0].minor);
}
// 1942 "parser.c"
        break;
      case 64:
// 665 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(25,&yymsp[0].minor);
}
// 1950 "parser.c"
        break;
      case 65:
// 669 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(26,&yymsp[0].minor);
}
// 1958 "parser.c"
        break;
      case 66:
// 673 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(27,&yymsp[0].minor);
}
// 1966 "parser.c"
        break;
      case 74:
// 705 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-2].minor.yy103, NULL, NULL);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(9,&yymsp[-1].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1976 "parser.c"
        break;
      case 75:
// 709 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, NULL);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(10,&yymsp[0].minor);
}
// 1986 "parser.c"
        break;
      case 76:
// 713 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_statement("variable", yymsp[-3].minor.yy0, NULL, yymsp[-1].minor.yy103);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1996 "parser.c"
        break;
      case 77:
// 717 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_statement("object-property", yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2007 "parser.c"
        break;
      case 78:
// 721 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_statement("variable-append", yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2019 "parser.c"
        break;
      case 79:
// 725 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_echo_statement(yymsp[-1].minor.yy103);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2028 "parser.c"
        break;
      case 80:
// 729 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_echo_statement(xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0));
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2037 "parser.c"
        break;
      case 81:
// 733 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy103);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2046 "parser.c"
        break;
      case 82:
// 737 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy103);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2055 "parser.c"
        break;
      case 83:
// 741 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy103);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2064 "parser.c"
        break;
      case 84:
// 745 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy103);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2073 "parser.c"
        break;
      case 85:
// 749 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy103);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 2082 "parser.c"
        break;
      case 88:
// 761 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL);
}
// 2089 "parser.c"
        break;
      case 89:
// 765 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy103);
  yy_destructor(15,&yymsp[-1].minor);
}
// 2097 "parser.c"
        break;
      case 90:
      case 104:
      case 123:
// 769 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0);
}
// 2106 "parser.c"
        break;
      case 91:
      case 110:
      case 113:
// 773 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0);
}
// 2115 "parser.c"
        break;
      case 92:
      case 109:
      case 114:
// 777 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0);
}
// 2124 "parser.c"
        break;
      case 93:
      case 115:
// 781 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0);
}
// 2132 "parser.c"
        break;
      case 94:
      case 116:
// 785 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_NULL, NULL);
  yy_destructor(38,&yymsp[0].minor);
}
// 2141 "parser.c"
        break;
      case 95:
      case 117:
// 789 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_FALSE, NULL);
  yy_destructor(39,&yymsp[0].minor);
}
// 2150 "parser.c"
        break;
      case 96:
// 793 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("empty-array", NULL, NULL, NULL);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2159 "parser.c"
        break;
      case 97:
// 797 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array", yymsp[-1].minor.yy103, NULL, NULL);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2168 "parser.c"
        break;
      case 98:
// 801 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL);
  yy_destructor(4,&yymsp[-1].minor);
}
// 2176 "parser.c"
        break;
      case 99:
// 805 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL);
  yy_destructor(4,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2186 "parser.c"
        break;
      case 100:
// 809 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103);
  yy_destructor(4,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2196 "parser.c"
        break;
      case 107:
// 837 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_array_item(yymsp[-2].minor.yy103, yymsp[0].minor.yy103);
  yy_destructor(40,&yymsp[-1].minor);
}
// 2204 "parser.c"
        break;
      case 108:
// 841 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_array_item(NULL, yymsp[0].minor.yy103);
}
// 2211 "parser.c"
        break;
      case 112:
      case 118:
// 857 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_TRUE, NULL);
  yy_destructor(41,&yymsp[0].minor);
}
// 2220 "parser.c"
        break;
      case 120:
// 889 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("equals", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL);
  yy_destructor(2,&yymsp[-1].minor);
}
// 2228 "parser.c"
        break;
      case 121:
// 893 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("identical", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL);
  yy_destructor(3,&yymsp[-1].minor);
}
// 2236 "parser.c"
        break;
      case 122:
// 897 "parser.lemon"
{
	yygotominor.yy103 = yymsp[-1].minor.yy103;
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2245 "parser.c"
        break;
      case 124:
// 905 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_comment(yymsp[0].minor.yy0);
}
// 2252 "parser.c"
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
// 382 "parser.lemon"


	//fprintf(stderr, "error!\n");

	if (status->scanner_state->start_length) {
		fprintf(stderr, "Syntax error, %s", status->scanner_state->start);
	} else {
		fprintf(stderr, "EOF");
	}

	//status->syntax_error_len = 48 + Z_STRLEN_P(status->scanner_state->active_file);
	//status->syntax_error = emalloc(sizeof(char) * status->syntax_error_len);

	status->status = XX_PARSING_FAILED;

// 2310 "parser.c"
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
