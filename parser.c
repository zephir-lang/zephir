/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 10 "parser.lemon"


#include "json/json.h"

#include "string.h"
#include "parser.h"
#include "scanner.h"
#include "xx.h"

static json_object *xx_ret_literal(int type, xx_parser_token *T)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_int(type));
	if (T) {
		json_object_object_add(ret, "value", json_object_new_string(T->token));
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

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("class"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

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

static json_object *xx_ret_let_statement(xx_parser_token *T, json_object *expr)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("let"));
	json_object_object_add(ret, "variable", json_object_new_string(T->token));
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


// 254 "parser.c"
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
#define YYNOCODE 61
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy55;
  int yy121;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 193
#define YYNRULE 93
#define YYERRORSYMBOL 32
#define YYERRSYMDT yy121
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
 /*     0 */   124,   77,   50,   51,   52,   53,   54,   55,   27,   77,
 /*    10 */    83,   56,   65,   71,   68,   80,   33,   45,   74,   56,
 /*    20 */    65,   71,   68,  150,   33,   45,   74,  122,   77,  204,
 /*    30 */   159,  172,  135,  178,  152,  118,   77,   61,   56,   65,
 /*    40 */    71,   68,   32,   33,   45,   74,   56,   65,   71,   68,
 /*    50 */     9,   33,   45,   74,  140,   77,   28,   29,   30,   31,
 /*    60 */   187,  158,  142,   77,  157,   56,   65,   71,   68,  153,
 /*    70 */    33,   45,   74,   56,   65,   71,   68,   83,   33,   45,
 /*    80 */    74,  133,   77,  143,   17,  158,  161,  184,  157,  116,
 /*    90 */    77,  120,   56,   65,   71,   68,  111,   33,   45,   74,
 /*   100 */    56,   65,   71,   68,  154,   33,   45,   74,   82,   77,
 /*   110 */   207,  146,   19,  167,  134,  152,  131,   77,   11,   56,
 /*   120 */    65,   71,   68,  126,   33,   45,   74,   56,   65,   71,
 /*   130 */    68,  278,   33,   45,   74,   25,   77,  287,    1,    2,
 /*   140 */   192,    4,    5,    6,   66,   64,   56,   65,   71,   68,
 /*   150 */    85,   33,   45,   74,  160,  144,  166,   85,  145,  157,
 /*   160 */    24,   59,  114,   96,   67,   99,  100,  101,  102,  103,
 /*   170 */    96,   77,   99,  100,  101,  102,  103,   85,  110,   14,
 /*   180 */   125,   56,   65,   71,   68,   85,   33,   45,   74,   23,
 /*   190 */    96,   38,   99,  100,  101,  102,  103,   48,   96,   86,
 /*   200 */    99,  100,  101,  102,  103,  162,   13,   16,  185,  191,
 /*   210 */   190,  163,  184,  157,   39,   40,   41,   42,   43,   44,
 /*   220 */    50,   51,   52,   53,   54,   55,   32,  203,  159,  172,
 /*   230 */    59,  178,  152,   70,   32,  173,  117,   69,   64,  130,
 /*   240 */    78,   29,   30,   31,  141,   20,   32,  168,   78,   29,
 /*   250 */    30,   31,    7,  169,   32,   10,  123,  155,   77,  156,
 /*   260 */    78,   29,   30,   31,   26,   32,   72,   64,   78,   29,
 /*   270 */    30,   31,  277,   59,   32,  132,   73,   75,   64,   78,
 /*   280 */    29,   30,   31,   59,   81,   87,   76,   89,   78,   29,
 /*   290 */    30,   31,   15,  159,  172,  129,  178,  152,  186,   59,
 /*   300 */   189,  190,   58,  145,  157,  194,  110,    7,  125,  147,
 /*   310 */    10,  174,  105,   77,  104,   88,   92,  175,  209,  109,
 /*   320 */    19,  179,  134,  206,  109,   19,   18,  134,  205,  109,
 /*   330 */    19,  135,  134,  208,  188,  172,  128,  178,  210,  109,
 /*   340 */    19,  119,  134,  104,   88,   92,  211,  109,   19,   79,
 /*   350 */   134,  104,   88,   92,    8,    3,    4,    5,    6,   84,
 /*   360 */    88,   92,  160,  180,  166,  147,   57,   64,   83,  181,
 /*   370 */   107,   90,   12,  127,   91,  164,  165,  193,   20,   93,
 /*   380 */   170,   62,  106,   34,   94,   21,   95,   22,  126,  276,
 /*   390 */   171,   35,  111,  121,   97,   98,  272,  176,  177,  248,
 /*   400 */   108,  249,   63,  275,  250,   36,  251,  182,  274,  183,
 /*   410 */    37,  252,  253,   60,  113,  112,  115,  273,   49,  155,
 /*   420 */    47,  139,  138,  151,  148,  137,  136,  149,   46,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,   26,   27,   28,   29,   30,   31,    7,    8,
 /*    10 */     1,   18,   19,   20,   21,    6,   23,   24,   25,   18,
 /*    20 */    19,   20,   21,   45,   23,   24,   25,    7,    8,    7,
 /*    30 */     8,    9,   13,   11,   12,    7,    8,    3,   18,   19,
 /*    40 */    20,   21,   39,   23,   24,   25,   18,   19,   20,   21,
 /*    50 */     4,   23,   24,   25,    7,    8,   53,   54,   55,   56,
 /*    60 */    43,   44,    7,    8,   47,   18,   19,   20,   21,    3,
 /*    70 */    23,   24,   25,   18,   19,   20,   21,    1,   23,   24,
 /*    80 */    25,    7,    8,   42,   43,   44,    3,   46,   47,    7,
 /*    90 */     8,   15,   18,   19,   20,   21,   13,   23,   24,   25,
 /*   100 */    18,   19,   20,   21,   10,   23,   24,   25,    7,    8,
 /*   110 */     7,    8,    9,    3,   11,   12,    7,    8,    3,   18,
 /*   120 */    19,   20,   21,   13,   23,   24,   25,   18,   19,   20,
 /*   130 */    21,    4,   23,   24,   25,    7,    8,   33,   34,   35,
 /*   140 */    36,   37,   38,   39,   58,   59,   18,   19,   20,   21,
 /*   150 */     3,   23,   24,   25,    9,   43,   11,    3,   46,   47,
 /*   160 */     6,    1,   15,   16,    4,   18,   19,   20,   21,   22,
 /*   170 */    16,    8,   18,   19,   20,   21,   22,    3,    9,    7,
 /*   180 */    11,   18,   19,   20,   21,    3,   23,   24,   25,   15,
 /*   190 */    16,    3,   18,   19,   20,   21,   22,    3,   16,   10,
 /*   200 */    18,   19,   20,   21,   22,    4,   40,   41,   42,   43,
 /*   210 */    44,   10,   46,   47,   26,   27,   28,   29,   30,   31,
 /*   220 */    26,   27,   28,   29,   30,   31,   39,    7,    8,    9,
 /*   230 */     1,   11,   12,    4,   39,    3,   49,   58,   59,    6,
 /*   240 */    53,   54,   55,   56,   49,   13,   39,    4,   53,   54,
 /*   250 */    55,   56,    2,   10,   39,    5,   49,   45,    8,    4,
 /*   260 */    53,   54,   55,   56,   49,   39,   58,   59,   53,   54,
 /*   270 */    55,   56,    4,    1,   39,   49,    4,   58,   59,   53,
 /*   280 */    54,   55,   56,    1,   49,   45,    4,    3,   53,   54,
 /*   290 */    55,   56,    7,    8,    9,   15,   11,   12,   41,    1,
 /*   300 */    43,   44,    4,   46,   47,    0,    9,    2,   11,   12,
 /*   310 */     5,    4,   48,    8,   50,   51,   52,   10,    7,    8,
 /*   320 */     9,    3,   11,    7,    8,    9,   47,   11,    7,    8,
 /*   330 */     9,   13,   11,    7,    8,    9,   14,   11,    7,    8,
 /*   340 */     9,   48,   11,   50,   51,   52,    7,    8,    9,   48,
 /*   350 */    11,   50,   51,   52,    3,   36,   37,   38,   39,   50,
 /*   360 */    51,   52,    9,    4,   11,   12,   58,   59,    1,   10,
 /*   370 */     6,   10,    6,    3,   45,   45,    4,    0,   13,    3,
 /*   380 */    45,   10,   15,    3,   10,    3,   45,   14,   13,    4,
 /*   390 */     4,   10,   13,    6,    3,   17,    4,   45,    4,    3,
 /*   400 */     7,    3,   45,    4,    3,   57,    3,   45,    4,    4,
 /*   410 */     4,    3,    3,   59,   14,    3,    6,    4,    4,   60,
 /*   420 */     4,    6,   15,    4,    3,   14,    3,   10,   45,
};
#define YY_SHIFT_USE_DFLT (-25)
static short yy_shift_ofst[] = {
 /*     0 */   250,  377,  305,  -25,  -25,  -25,  -25,  351,   46,  -25,
 /*    10 */   115,  366,  285,  172,  -25,  -25,  220,  316,  -25,  365,
 /*    20 */   382,  373,  174,  154,  128,  -25,    1,  -25,  -25,  -25,
 /*    30 */   -25,  -25,  -25,  380,  381,  188,  406,  -25,  392,  413,
 /*    40 */   404,  399,  385,  268,  127,  194,  416,  -25,  414,  -25,
 /*    50 */   -25,  -25,  -25,  -25,  -25,  -25,   34,  298,  -25,   34,
 /*    60 */   -25,  371,  -24,  -25,  -25,   34,  160,  -25,   34,  229,
 /*    70 */   -25,   34,  272,  -25,   34,  282,  -25,  -25,  -25,    9,
 /*    80 */   163,  101,  -25,  182,  -25,  189,  -24,  -25,  284,  361,
 /*    90 */   -24,  -25,  376,  374,  -24,  -25,  391,  378,  396,  398,
 /*   100 */   401,  403,  408,  409,  -25,  367,  364,  393,  -25,  169,
 /*   110 */   379,  412,  400,  147,  410,   82,  -25,   28,  -25,   76,
 /*   120 */   387,   20,  -25,   -7,  -25,  375,  370,  322,  280,  233,
 /*   130 */   109,  -25,   74,  -25,   19,  423,  411,  407,  415,   47,
 /*   140 */   -25,   55,  -25,  103,  331,  -25,  297,  421,  417,  -24,
 /*   150 */   419,  -25,   66,   94,  -24,  255,  -25,  -25,  -25,  353,
 /*   160 */    83,  201,  -25,  -24,  372,  -25,  110,  243,  -25,  -24,
 /*   170 */   386,  -25,  232,  307,  -25,  -24,  394,  -25,  318,  359,
 /*   180 */   -25,  -24,  405,  -25,  -25,   22,  326,  339,  145,  311,
 /*   190 */   -25,  321,  -25,
};
#define YY_REDUCE_USE_DFLT (-23)
static short yy_reduce_ofst[] = {
 /*     0 */   104,  -23,  319,  -23,  -23,  -23,  -23,  -23,  -23,  -23,
 /*    10 */   -23,  -23,  166,  -23,  -23,  -23,   41,  279,  -23,  -23,
 /*    20 */   -23,  -23,  264,  301,  215,  -23,    3,  -23,  -23,  -23,
 /*    30 */   -23,  -23,  -23,  -23,  -23,  348,  -23,  -23,  -23,  -23,
 /*    40 */   -23,  -23,  -23,  -23,  -23,  383,  -23,  -23,  -23,  -23,
 /*    50 */   -23,  -23,  -23,  -23,  -23,  -23,  308,  -23,  -23,  354,
 /*    60 */   -23,  -23,  357,  -23,  -23,   86,  -23,  -23,  179,  -23,
 /*    70 */   -23,  208,  -23,  -23,  219,  -23,  -23,  -23,  -23,  -23,
 /*    80 */   235,    3,  -23,  309,  -23,  -23,  240,  -23,  -23,  -23,
 /*    90 */   329,  -23,  -23,  -23,  341,  -23,  -23,  -23,  -23,  -23,
 /*   100 */   -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,
 /*   110 */   -23,  -23,  -23,  293,  -23,  187,  -23,    3,  -23,  -23,
 /*   120 */   -23,  207,  -23,    3,  -23,  -23,  -23,  -23,  -23,  -23,
 /*   130 */   226,  -23,    3,  -23,  -23,  -23,  -23,  -23,  -23,  195,
 /*   140 */   -23,    3,  -23,  112,  279,  -23,  -23,  -23,  -23,  -22,
 /*   150 */   -23,  -23,  -23,  -23,  212,  -23,  -23,  -23,  -23,  -23,
 /*   160 */   -23,  -23,  -23,  330,  -23,  -23,  -23,  -23,  -23,  335,
 /*   170 */   -23,  -23,  -23,  -23,  -23,  352,  -23,  -23,  -23,  -23,
 /*   180 */   -23,  362,  -23,  -23,  -23,  257,   17,  279,  -23,  279,
 /*   190 */   -23,  279,  -23,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   286,  286,  286,  195,  197,  198,  199,  286,  286,  200,
 /*    10 */   286,  286,  286,  286,  201,  202,  286,  286,  224,  286,
 /*    20 */   286,  286,  286,  286,  286,  228,  286,  232,  254,  256,
 /*    30 */   257,  258,  259,  286,  286,  286,  286,  260,  286,  286,
 /*    40 */   286,  286,  286,  286,  286,  286,  286,  261,  286,  262,
 /*    50 */   279,  280,  281,  282,  283,  284,  286,  286,  263,  286,
 /*    60 */   268,  270,  286,  271,  269,  286,  286,  264,  286,  286,
 /*    70 */   265,  286,  286,  266,  286,  286,  267,  285,  255,  286,
 /*    80 */   286,  286,  233,  286,  240,  242,  286,  245,  286,  243,
 /*    90 */   286,  246,  286,  244,  286,  247,  286,  286,  286,  286,
 /*   100 */   286,  286,  286,  286,  241,  286,  286,  286,  229,  286,
 /*   110 */   286,  286,  286,  286,  286,  286,  230,  286,  234,  286,
 /*   120 */   286,  286,  231,  286,  235,  286,  286,  286,  286,  286,
 /*   130 */   286,  237,  286,  239,  286,  286,  286,  286,  286,  286,
 /*   140 */   236,  286,  238,  286,  286,  222,  286,  286,  286,  286,
 /*   150 */   286,  225,  286,  286,  286,  286,  226,  227,  212,  286,
 /*   160 */   286,  286,  214,  286,  286,  217,  286,  286,  219,  286,
 /*   170 */   286,  221,  286,  286,  215,  286,  286,  216,  286,  286,
 /*   180 */   218,  286,  286,  220,  223,  286,  286,  286,  286,  286,
 /*   190 */   213,  286,  196,
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
  "$",             "COMMA",         "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "COMMENT",       "PUBLIC",        "ASSIGN",        "PROTECTED",   
  "CONST",         "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",
  "LESS",          "GREATER",       "TYPE_INTEGER",  "TYPE_DOUBLE", 
  "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",    "LET",         
  "ECHO",          "TYPE_VAR",      "INTEGER",       "STRING",      
  "DOUBLE",        "NULL",          "FALSE",         "TRUE",        
  "error",         "program",       "xx_language",   "xx_top_statement_list",
  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",  "xx_comment",  
  "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",
  "xx_class_property_definition",  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_parameter_list",  "xx_statement_list",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_echo_statement",
  "xx_declare_statement",  "xx_expr",       "xx_declare_variable_list",  "xx_declare_variable",
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
 /*   9 */ "xx_class_def ::= CLASS IDENTIFIER BRACKET_OPEN BRACKET_CLOSE",
 /*  10 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  11 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  12 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  13 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  14 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  15 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  16 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  17 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  18 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  19 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  20 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  21 */ "xx_class_property_definition ::= COMMENT PUBLIC IDENTIFIER DOTCOMMA",
 /*  22 */ "xx_class_property_definition ::= PUBLIC IDENTIFIER DOTCOMMA",
 /*  23 */ "xx_class_property_definition ::= PUBLIC IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  24 */ "xx_class_property_definition ::= COMMENT PUBLIC IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  25 */ "xx_class_property_definition ::= PROTECTED IDENTIFIER DOTCOMMA",
 /*  26 */ "xx_class_property_definition ::= COMMENT PROTECTED IDENTIFIER DOTCOMMA",
 /*  27 */ "xx_class_property_definition ::= PROTECTED IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  28 */ "xx_class_property_definition ::= COMMENT PROTECTED IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  29 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  30 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  31 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  32 */ "xx_class_const_definition ::= COMMENT CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  33 */ "xx_class_const_definition ::= CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  34 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  35 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  36 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  37 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  38 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  39 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  40 */ "xx_class_method_definition ::= PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE xx_parameter_list BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  41 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  42 */ "xx_class_method_definition ::= COMMENT PUBLIC FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  43 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= COMMENT PROTECTED FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  47 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  48 */ "xx_parameter_list ::= xx_parameter",
 /*  49 */ "xx_parameter ::= IDENTIFIER",
 /*  50 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  51 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  52 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  53 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  54 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  55 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  56 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  57 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  58 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  59 */ "xx_parameter_type ::= TYPE_STRING",
 /*  60 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  61 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  62 */ "xx_statement_list ::= xx_statement",
 /*  63 */ "xx_statement ::= xx_let_statement",
 /*  64 */ "xx_statement ::= xx_echo_statement",
 /*  65 */ "xx_statement ::= xx_declare_statement",
 /*  66 */ "xx_statement ::= xx_comment",
 /*  67 */ "xx_let_statement ::= LET IDENTIFIER ASSIGN xx_expr DOTCOMMA",
 /*  68 */ "xx_echo_statement ::= ECHO xx_literal_expr DOTCOMMA",
 /*  69 */ "xx_echo_statement ::= ECHO IDENTIFIER DOTCOMMA",
 /*  70 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /*  71 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /*  72 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /*  73 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /*  74 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /*  75 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /*  76 */ "xx_declare_variable_list ::= xx_declare_variable",
 /*  77 */ "xx_declare_variable ::= IDENTIFIER",
 /*  78 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  79 */ "xx_expr ::= IDENTIFIER",
 /*  80 */ "xx_expr ::= INTEGER",
 /*  81 */ "xx_expr ::= STRING",
 /*  82 */ "xx_expr ::= DOUBLE",
 /*  83 */ "xx_expr ::= NULL",
 /*  84 */ "xx_expr ::= FALSE",
 /*  85 */ "xx_expr ::= TRUE",
 /*  86 */ "xx_literal_expr ::= INTEGER",
 /*  87 */ "xx_literal_expr ::= STRING",
 /*  88 */ "xx_literal_expr ::= DOUBLE",
 /*  89 */ "xx_literal_expr ::= NULL",
 /*  90 */ "xx_literal_expr ::= FALSE",
 /*  91 */ "xx_literal_expr ::= TRUE",
 /*  92 */ "xx_comment ::= COMMENT",
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
// 272 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 826 "parser.c"
      break;
    case 34:
// 285 "parser.lemon"
{ json_object_put((yypminor->yy55)); }
// 831 "parser.c"
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
  { 33, 1 },
  { 34, 1 },
  { 35, 2 },
  { 35, 1 },
  { 36, 1 },
  { 36, 1 },
  { 36, 1 },
  { 37, 3 },
  { 38, 5 },
  { 38, 4 },
  { 40, 1 },
  { 40, 1 },
  { 40, 1 },
  { 40, 2 },
  { 40, 2 },
  { 40, 2 },
  { 40, 2 },
  { 40, 3 },
  { 40, 3 },
  { 41, 2 },
  { 41, 1 },
  { 44, 4 },
  { 44, 3 },
  { 44, 5 },
  { 44, 6 },
  { 44, 3 },
  { 44, 4 },
  { 44, 5 },
  { 44, 6 },
  { 42, 2 },
  { 42, 1 },
  { 43, 2 },
  { 46, 6 },
  { 46, 5 },
  { 43, 1 },
  { 47, 7 },
  { 47, 8 },
  { 47, 8 },
  { 47, 9 },
  { 47, 8 },
  { 47, 9 },
  { 47, 9 },
  { 47, 10 },
  { 47, 7 },
  { 47, 8 },
  { 47, 8 },
  { 47, 9 },
  { 48, 3 },
  { 48, 1 },
  { 50, 1 },
  { 50, 2 },
  { 50, 2 },
  { 50, 3 },
  { 50, 4 },
  { 50, 4 },
  { 52, 3 },
  { 51, 1 },
  { 51, 1 },
  { 51, 1 },
  { 51, 1 },
  { 51, 1 },
  { 49, 2 },
  { 49, 1 },
  { 53, 1 },
  { 53, 1 },
  { 53, 1 },
  { 53, 1 },
  { 54, 5 },
  { 55, 3 },
  { 55, 3 },
  { 56, 3 },
  { 56, 3 },
  { 56, 3 },
  { 56, 3 },
  { 56, 3 },
  { 58, 3 },
  { 58, 1 },
  { 59, 1 },
  { 59, 3 },
  { 57, 1 },
  { 57, 1 },
  { 57, 1 },
  { 57, 1 },
  { 57, 1 },
  { 57, 1 },
  { 57, 1 },
  { 45, 1 },
  { 45, 1 },
  { 45, 1 },
  { 45, 1 },
  { 45, 1 },
  { 45, 1 },
  { 39, 1 },
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
// 281 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy55;
}
// 1141 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 63:
      case 64:
      case 65:
      case 66:
// 287 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;
}
// 1155 "parser.c"
        break;
      case 2:
      case 19:
      case 29:
      case 31:
      case 61:
// 291 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-1].minor.yy55, yymsp[0].minor.yy55);
}
// 1166 "parser.c"
        break;
      case 3:
      case 20:
      case 30:
      case 34:
      case 48:
      case 62:
      case 76:
// 295 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(NULL, yymsp[0].minor.yy55);
}
// 1179 "parser.c"
        break;
      case 7:
// 311 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_namespace(yymsp[-1].minor.yy0);
  yy_destructor(2,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1188 "parser.c"
        break;
      case 8:
// 315 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55);
  yy_destructor(5,&yymsp[-4].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1198 "parser.c"
        break;
      case 9:
// 319 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-2].minor.yy0, NULL);
  yy_destructor(5,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1208 "parser.c"
        break;
      case 10:
// 323 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, NULL);
}
// 1215 "parser.c"
        break;
      case 11:
// 327 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy55);
}
// 1222 "parser.c"
        break;
      case 12:
// 331 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, NULL);
}
// 1229 "parser.c"
        break;
      case 13:
// 335 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, NULL);
}
// 1236 "parser.c"
        break;
      case 14:
// 339 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy55);
}
// 1243 "parser.c"
        break;
      case 15:
// 343 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, yymsp[-1].minor.yy55);
}
// 1250 "parser.c"
        break;
      case 16:
// 347 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, yymsp[-1].minor.yy55);
}
// 1257 "parser.c"
        break;
      case 17:
// 351 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, yymsp[-1].minor.yy55);
}
// 1264 "parser.c"
        break;
      case 18:
// 355 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, yymsp[-2].minor.yy55);
}
// 1271 "parser.c"
        break;
      case 21:
// 367 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1280 "parser.c"
        break;
      case 22:
// 371 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL);
  yy_destructor(9,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1289 "parser.c"
        break;
      case 23:
// 375 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL);
  yy_destructor(9,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1299 "parser.c"
        break;
      case 24:
// 379 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0);
  yy_destructor(9,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1309 "parser.c"
        break;
      case 25:
// 383 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1318 "parser.c"
        break;
      case 26:
// 387 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1327 "parser.c"
        break;
      case 27:
// 391 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL);
  yy_destructor(11,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1337 "parser.c"
        break;
      case 28:
// 395 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0);
  yy_destructor(11,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1347 "parser.c"
        break;
      case 32:
// 411 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1357 "parser.c"
        break;
      case 33:
// 415 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1367 "parser.c"
        break;
      case 35:
// 423 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL);
  yy_destructor(9,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-5].minor);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1380 "parser.c"
        break;
      case 36:
// 427 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, NULL, NULL);
  yy_destructor(9,&yymsp[-7].minor);
  yy_destructor(13,&yymsp[-6].minor);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1393 "parser.c"
        break;
      case 37:
// 431 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0);
  yy_destructor(9,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-5].minor);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1406 "parser.c"
        break;
      case 38:
// 435 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, NULL, yymsp[-8].minor.yy0);
  yy_destructor(9,&yymsp[-7].minor);
  yy_destructor(13,&yymsp[-6].minor);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1419 "parser.c"
        break;
      case 39:
// 439 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, NULL);
  yy_destructor(9,&yymsp[-7].minor);
  yy_destructor(13,&yymsp[-6].minor);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1432 "parser.c"
        break;
      case 40:
// 443 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, NULL);
  yy_destructor(9,&yymsp[-8].minor);
  yy_destructor(13,&yymsp[-7].minor);
  yy_destructor(14,&yymsp[-5].minor);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1445 "parser.c"
        break;
      case 41:
// 447 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, yymsp[-8].minor.yy0);
  yy_destructor(9,&yymsp[-7].minor);
  yy_destructor(13,&yymsp[-6].minor);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1458 "parser.c"
        break;
      case 42:
// 451 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy55, yymsp[-1].minor.yy55, yymsp[-9].minor.yy0);
  yy_destructor(9,&yymsp[-8].minor);
  yy_destructor(13,&yymsp[-7].minor);
  yy_destructor(14,&yymsp[-5].minor);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1471 "parser.c"
        break;
      case 43:
// 455 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL);
  yy_destructor(11,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-5].minor);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1484 "parser.c"
        break;
      case 44:
// 459 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0);
  yy_destructor(11,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-5].minor);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(6,&yymsp[-1].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1497 "parser.c"
        break;
      case 45:
// 463 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, NULL);
  yy_destructor(11,&yymsp[-7].minor);
  yy_destructor(13,&yymsp[-6].minor);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1510 "parser.c"
        break;
      case 46:
// 467 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, yymsp[-8].minor.yy0);
  yy_destructor(11,&yymsp[-7].minor);
  yy_destructor(13,&yymsp[-6].minor);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-2].minor);
  yy_destructor(7,&yymsp[0].minor);
}
// 1523 "parser.c"
        break;
      case 47:
      case 75:
// 471 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-2].minor.yy55, yymsp[0].minor.yy55);
  yy_destructor(1,&yymsp[-1].minor);
}
// 1532 "parser.c"
        break;
      case 49:
// 479 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL);
}
// 1539 "parser.c"
        break;
      case 50:
// 483 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy0, NULL);
}
// 1546 "parser.c"
        break;
      case 51:
// 487 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-1].minor.yy55, yymsp[0].minor.yy0, NULL);
}
// 1553 "parser.c"
        break;
      case 52:
// 491 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55);
  yy_destructor(10,&yymsp[-1].minor);
}
// 1561 "parser.c"
        break;
      case 53:
// 495 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-3].minor.yy55, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55);
  yy_destructor(10,&yymsp[-1].minor);
}
// 1569 "parser.c"
        break;
      case 54:
// 499 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-3].minor.yy55, yymsp[-2].minor.yy0, yymsp[0].minor.yy55);
  yy_destructor(10,&yymsp[-1].minor);
}
// 1577 "parser.c"
        break;
      case 55:
// 503 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 1586 "parser.c"
        break;
      case 56:
// 507 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(18,&yymsp[0].minor);
}
// 1594 "parser.c"
        break;
      case 57:
// 511 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(19,&yymsp[0].minor);
}
// 1602 "parser.c"
        break;
      case 58:
// 515 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(20,&yymsp[0].minor);
}
// 1610 "parser.c"
        break;
      case 59:
// 519 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(21,&yymsp[0].minor);
}
// 1618 "parser.c"
        break;
      case 60:
// 523 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(22,&yymsp[0].minor);
}
// 1626 "parser.c"
        break;
      case 67:
// 551 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_statement(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1636 "parser.c"
        break;
      case 68:
// 555 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_echo_statement(yymsp[-1].minor.yy55);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1645 "parser.c"
        break;
      case 69:
// 559 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_echo_statement(xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0));
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1654 "parser.c"
        break;
      case 70:
// 563 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy55);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1663 "parser.c"
        break;
      case 71:
// 567 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy55);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1672 "parser.c"
        break;
      case 72:
// 571 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy55);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1681 "parser.c"
        break;
      case 73:
// 575 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy55);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1690 "parser.c"
        break;
      case 74:
// 579 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy55);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 1699 "parser.c"
        break;
      case 77:
// 591 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL);
}
// 1706 "parser.c"
        break;
      case 78:
// 595 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy55);
  yy_destructor(10,&yymsp[-1].minor);
}
// 1714 "parser.c"
        break;
      case 79:
// 599 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0);
}
// 1721 "parser.c"
        break;
      case 80:
      case 86:
// 603 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0);
}
// 1729 "parser.c"
        break;
      case 81:
      case 87:
// 607 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0);
}
// 1737 "parser.c"
        break;
      case 82:
      case 88:
// 611 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0);
}
// 1745 "parser.c"
        break;
      case 83:
      case 89:
// 615 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_NULL, NULL);
  yy_destructor(29,&yymsp[0].minor);
}
// 1754 "parser.c"
        break;
      case 84:
      case 90:
// 619 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_FALSE, NULL);
  yy_destructor(30,&yymsp[0].minor);
}
// 1763 "parser.c"
        break;
      case 85:
      case 91:
// 623 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_TRUE, NULL);
  yy_destructor(31,&yymsp[0].minor);
}
// 1772 "parser.c"
        break;
      case 92:
// 651 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_comment(yymsp[0].minor.yy0);
}
// 1779 "parser.c"
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
// 256 "parser.lemon"


	//fprintf(stderr, "error!\n");

	if (status->scanner_state->start_length) {
		fprintf(stderr, "Syntax error, %s", status->scanner_state->start);
	} else {
		fprintf(stderr, "EOF");
	}

	//status->syntax_error_len = 48 + Z_STRLEN_P(status->scanner_state->active_file);
	//status->syntax_error = emalloc(sizeof(char) * status->syntax_error_len);

	status->status = XX_PARSING_FAILED;

// 1837 "parser.c"
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
