/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 40 "parser.lemon"


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

static json_object *xx_ret_for_statement(json_object *expr, xx_parser_token *K, xx_parser_token *V, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("for"));
	json_object_object_add(ret, "expr", expr);

	if (K) {
		json_object_object_add(ret, "key", json_object_new_string(K->token));
	}
	if (V) {
		json_object_object_add(ret, "value", json_object_new_string(V->token));
	}

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

static json_object *xx_ret_unset_statement(json_object *domain, json_object *index, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("unset"));

	json_object_object_add(ret, "domain", domain);
	json_object_object_add(ret, "index", index);

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

static json_object *xx_ret_throw_exception(xx_parser_token *T, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("throw"));
	json_object_object_add(ret, "domain", json_object_new_string(T->token));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_fcall(xx_parser_token *F, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fcall"));
	json_object_object_add(ret, "name", json_object_new_string(F->token));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_mcall(xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
	json_object_object_add(ret, "variable", json_object_new_string(O->token));
	json_object_object_add(ret, "name", json_object_new_string(M->token));

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


// 636 "parser.c"
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
#define YYNOCODE 115
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy7;
  int yy229;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 401
#define YYNRULE 186
#define YYERRORSYMBOL 63
#define YYERRSYMDT yy229
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
 /*     0 */    50,  342,  177,  249,  588,    1,    2,  400,    4,    5,
 /*    10 */     6,  228,  237,  243,  240,  115,   42,   44,   46,   77,
 /*    20 */    55,   59,   64,  179,   71,  160,    3,    4,    5,    6,
 /*    30 */   194,  201,  204,  199,  311,  219,  246,  249,  198,  175,
 /*    40 */   183,  357,  191,  192,  173,  228,  237,  243,  240,  358,
 /*    50 */    42,   44,   46,  118,   55,   59,   64,  153,   71,   61,
 /*    60 */   166,  157,  162,  343,  194,  201,  204,   51,  262,  219,
 /*    70 */   246,  249,  375,  344,  378,  379,  174,  334,  346,  228,
 /*    80 */   237,  243,  240,  226,   42,   44,   46,  173,   55,   59,
 /*    90 */    64,  294,   71,  293,  277,  281,  244,  236,  194,  201,
 /*   100 */   204,  142,  320,  219,  246,  249,  332,   17,  347,  350,
 /*   110 */   373,  346,  173,  228,  237,  243,  240,  120,   42,   44,
 /*   120 */    46,  300,   55,   59,   64,  419,   71,  160,  298,   19,
 /*   130 */    94,  323,  194,  201,  204,  118,   63,  219,  246,  249,
 /*   140 */   425,  173,  231,  298,   19,  138,  323,  228,  237,  243,
 /*   150 */   240,  129,   42,   44,   46,  251,   55,   59,   64,  242,
 /*   160 */    71,  120,  156,  157,  162,  117,  194,  201,  204,  100,
 /*   170 */   322,  219,  246,  249,  199,  273,  277,  281,  195,  200,
 /*   180 */   173,  228,  237,  243,  240,  173,   42,   44,   46,  171,
 /*   190 */    55,   59,   64,  423,   71,  129,  298,   19,   66,  323,
 /*   200 */   194,  201,  204,  345,   58,  219,  246,  249,  424,  120,
 /*   210 */   155,  298,   19,  120,  323,  228,  237,  243,  240,  264,
 /*   220 */    42,   44,   46,   91,   55,   59,   64,  308,   71,  293,
 /*   230 */   277,  281,   47,  259,  194,  201,  204,  116,  329,  219,
 /*   240 */   246,  249,  422,  129,  173,  377,  361,  119,  367,  228,
 /*   250 */   237,  243,  240,  120,   42,   44,   46,  154,   55,   59,
 /*   260 */    64,  420,   71,   88,  298,   19,  110,  323,  194,  201,
 /*   270 */   204,  263,  261,  219,  246,  249,  268,  173,  293,  277,
 /*   280 */   281,  225,  231,  228,  237,  243,  240,  129,   42,   44,
 /*   290 */    46,  224,   55,   59,   64,  299,   71,  314,  336,  239,
 /*   300 */    90,   92,  194,  201,  204,   96,  331,  219,  246,  249,
 /*   310 */    79,  333,  210,   78,  334,  346,  173,  228,  237,  243,
 /*   320 */   240,  173,   42,   44,   46,  118,   55,   59,   64,  137,
 /*   330 */    71,  103,  105,  107,  229,  236,  194,  201,  204,   75,
 /*   340 */    27,  219,  246,  249,   43,  113,  130,   60,  259,  213,
 /*   350 */   216,  228,  237,  243,  240,  139,   42,   44,   46,  173,
 /*   360 */    55,   59,   64,  349,   71,  355,  336,   12,   20,  381,
 /*   370 */   194,  201,  204,  272,  305,  219,  246,  249,   79,  376,
 /*   380 */   347,  178,  349,  346,  355,  228,  237,  243,  240,  173,
 /*   390 */    42,   44,   46,  363,   55,   59,   64,  131,   71,  241,
 /*   400 */   236,  364,  187,  309,  194,  201,  204,   98,  271,  219,
 /*   410 */   246,  249,   79,  160,  231,  182,  238,  236,  173,  228,
 /*   420 */   237,  243,  240,  173,   42,   44,   46,   89,   55,   59,
 /*   430 */    64,  248,   71,  184,  133,  135,  368,  362,  194,  201,
 /*   440 */   204,   87,  307,  219,  246,  249,  206,  209,  324,   20,
 /*   450 */   159,   72,  193,  228,  237,  243,  240,  351,   42,   44,
 /*   460 */    46,  354,   55,   59,   64,  352,   71,  247,  236,   74,
 /*   470 */    48,  254,  194,  201,  204,  353,   70,  219,  246,  249,
 /*   480 */    79,  222,  173,  186,  220,  231,   73,  228,  237,  243,
 /*   490 */   240,  173,   42,   44,   46,  369,   55,   59,   64,  390,
 /*   500 */    71,  394,  230,  370,  106,  108,  194,  201,  204,   67,
 /*   510 */   266,  219,  246,  249,   79,  173,  173,  190,  272,  197,
 /*   520 */   173,  228,  237,  243,  240,  173,   42,   44,   46,  118,
 /*   530 */    55,   59,   64,  299,   71,  314,  196,  269,   82,  102,
 /*   540 */   194,  201,  204,  104,  313,  219,  246,  249,  253,  173,
 /*   550 */   173,  252,  144,   85,  173,  228,  237,  243,  240,  172,
 /*   560 */    42,   44,   46,  173,   55,   59,   64,  272,   71,  231,
 /*   570 */   356,   86,  202,  359,  194,  201,  204,  360,  267,  219,
 /*   580 */   246,  249,  315,  173,    8,   18,  245,  205,   65,  228,
 /*   590 */   237,  243,  240,  207,   42,   44,   46,  295,   55,   59,
 /*   600 */    64,  365,   71,  366,   56,  233,   11,   14,  194,  201,
 /*   610 */   204,  211,   54,  219,  246,  249,  371,  372,  212,    9,
 /*   620 */   214,  215,   45,  228,  237,  243,  240,  382,   42,   44,
 /*   630 */    46,  383,   55,   59,   64,  385,   71,  235,  217,   76,
 /*   640 */   388,  218,  194,  201,  204,  389,   25,  219,  246,  249,
 /*   650 */   234,  232,  392,  134,  221,  136,  132,  228,  237,  243,
 /*   660 */   240,  301,   42,   44,   46,  302,   55,   59,   64,  300,
 /*   670 */    71,  304,  176,  297,  296,   22,  194,  201,  204,  208,
 /*   680 */   258,  219,  246,  249,  310,  395,  467,  169,  466,  465,
 /*   690 */   168,  228,  237,  243,  240,  180,   42,   44,   46,  464,
 /*   700 */    55,   59,   64,  315,   71,  181,  316,  463,  317,  462,
 /*   710 */   194,  201,  204,  223,  260,  219,  246,  249,  318,  319,
 /*   720 */   287,  286,  573,  185,  572,  228,  237,  243,  240,  284,
 /*   730 */    42,   44,   46,  283,   55,   59,   64,  324,   71,  325,
 /*   740 */   326,  396,  571,  327,  194,  201,  204,  282,  257,  219,
 /*   750 */   246,  249,  328,  280,  188,  279,  278,   21,   52,  228,
 /*   760 */   237,  243,  240,  189,   42,   44,   46,  276,   55,   59,
 /*   770 */    64,  275,   71,  401,  227,  337,   93,  338,  194,  201,
 /*   780 */   204,  158,  340,  219,  246,  249,  114,  339,  398,  302,
 /*   790 */   302,  302,  302,  228,  237,  243,  240,  302,   42,   44,
 /*   800 */    46,  302,   55,   59,   64,   41,   71,  302,  302,  302,
 /*   810 */   302,  302,  194,  201,  204,   49,  302,  219,  246,  250,
 /*   820 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   830 */    39,   40,   41,   80,   95,   99,  101,   97,   83,  103,
 /*   840 */   105,  107,  270,  302,  302,  302,  250,   29,   30,   31,
 /*   850 */    32,   33,   34,   35,   36,   37,   38,   39,   40,  302,
 /*   860 */   302,  302,  111,  302,   41,   80,   95,   99,  101,   97,
 /*   870 */    83,  103,  105,  107,   69,  302,  302,  302,  250,   29,
 /*   880 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   890 */    40,   41,  302,  397,   16,  374,  380,  379,  302,  373,
 /*   900 */   346,  256,  302,  302,  302,  250,   29,   30,   31,   32,
 /*   910 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  302,
 /*   920 */    13,   16,  374,  380,  379,  302,  373,  346,   26,  302,
 /*   930 */   302,  302,  250,   29,   30,   31,   32,   33,   34,   35,
 /*   940 */    36,   37,   38,   39,   40,   41,  302,  302,  302,  302,
 /*   950 */   391,   16,  374,  380,  379,  265,  373,  346,  302,  250,
 /*   960 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   970 */    39,   40,   41,  302,  302,  302,  302,  384,   16,  374,
 /*   980 */   380,  379,   62,  373,  346,  302,  250,   29,   30,   31,
 /*   990 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1000 */   122,  123,  302,  302,  124,  125,  127,  126,  302,   57,
 /*  1010 */   302,  302,  302,  250,   29,   30,   31,   32,   33,   34,
 /*  1020 */    35,   36,   37,   38,   39,   40,   41,  386,  302,  302,
 /*  1030 */   348,  361,  302,  367,  341,  302,   53,  302,  302,  302,
 /*  1040 */   250,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1050 */    38,   39,   40,   41,  302,  417,  302,    7,  348,  361,
 /*  1060 */    10,  367,  341,  306,  387,  249,  302,  250,   29,   30,
 /*  1070 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1080 */    41,  421,  302,  302,  335,   19,  302,  323,  341,  302,
 /*  1090 */   321,  302,  302,  302,  250,   29,   30,   31,   32,   33,
 /*  1100 */    34,   35,   36,   37,   38,   39,   40,   41,  302,  399,
 /*  1110 */   302,  302,  348,  361,  302,  367,  341,  312,  302,  302,
 /*  1120 */   302,  250,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1130 */    37,   38,   39,   40,   41,   15,  302,  302,  348,  361,
 /*  1140 */   302,  367,  341,  302,  330,  302,  302,   41,  250,   29,
 /*  1150 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1160 */    40,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1170 */    37,   38,   39,   40,   84,  143,  141,  167,   81,  302,
 /*  1180 */   112,   80,   95,   99,  101,   97,   83,  103,  105,  107,
 /*  1190 */   402,  418,  302,  109,  348,  361,  302,  367,  341,  255,
 /*  1200 */   302,  302,  302,  302,  302,  302,    7,  302,  302,   10,
 /*  1210 */   151,  302,  302,  387,  249,  302,  302,  145,  146,  302,
 /*  1220 */   302,  147,  148,  149,  150,   80,   95,   99,  101,   97,
 /*  1230 */    83,  103,  105,  107,  302,  163,  393,  302,  302,  348,
 /*  1240 */   361,  203,  367,  341,   80,   95,   99,  101,   97,   83,
 /*  1250 */   103,  105,  107,   80,   95,   99,  101,   97,   83,  103,
 /*  1260 */   105,  107,   68,  302,  302,  302,  152,  121,  302,  302,
 /*  1270 */   302,  585,  165,  164,  302,  121,  124,  125,  127,  126,
 /*  1280 */   302,  128,  302,  302,  302,  302,  302,  302,  302,  140,
 /*  1290 */   302,  302,  302,  302,  285,  121,  302,  302,  302,  302,
 /*  1300 */   302,  302,  302,  302,  122,  123,  302,  274,  124,  125,
 /*  1310 */   127,  126,  122,  123,  285,  302,  124,  125,  127,  126,
 /*  1320 */   302,  303,  288,  289,  290,  291,  292,  274,  285,  302,
 /*  1330 */    24,  302,  122,  123,  121,  302,  124,  125,  127,  126,
 /*  1340 */   302,  274,  288,  289,  290,  291,  292,  302,  170,  302,
 /*  1350 */   163,  285,  302,  302,  302,   23,  288,  289,  290,  291,
 /*  1360 */   292,  161,  302,  302,  274,  302,  302,  302,  302,  302,
 /*  1370 */   302,  122,  123,  302,  302,  124,  125,  127,  126,  288,
 /*  1380 */   289,  290,  291,  292,  302,  302,  302,  165,  164,  302,
 /*  1390 */   302,  124,  125,  127,  126,  302,  302,  302,  122,  123,
 /*  1400 */   302,  302,  124,  125,  127,  126,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    21,   17,   26,   24,   64,   65,   66,   67,   68,   69,
 /*    10 */    70,   32,   33,   34,   35,   30,   37,   38,   39,   26,
 /*    20 */    41,   42,   43,   47,   45,   76,   67,   68,   69,   70,
 /*    30 */    51,   52,   53,   98,   21,   56,   57,   24,  103,   46,
 /*    40 */    47,   18,   49,   50,  109,   32,   33,   34,   35,   26,
 /*    50 */    37,   38,   39,    1,   41,   42,   43,  108,   45,   20,
 /*    60 */   111,  112,  113,   26,   51,   52,   53,   40,   21,   56,
 /*    70 */    57,   24,   72,   76,   74,   75,   98,   77,   78,   32,
 /*    80 */    33,   34,   35,   31,   37,   38,   39,  109,   41,   42,
 /*    90 */    43,   79,   45,   81,   82,   83,  105,  106,   51,   52,
 /*   100 */    53,   98,   21,   56,   57,   24,   73,   74,   75,   17,
 /*   110 */    77,   78,  109,   32,   33,   34,   35,   76,   37,   38,
 /*   120 */    39,   29,   41,   42,   43,   21,   45,   76,   24,   25,
 /*   130 */    98,   27,   51,   52,   53,    1,   21,   56,   57,   24,
 /*   140 */    21,  109,    1,   24,   25,  104,   27,   32,   33,   34,
 /*   150 */    35,  110,   37,   38,   39,    1,   41,   42,   43,   18,
 /*   160 */    45,   76,  111,  112,  113,   31,   51,   52,   53,   98,
 /*   170 */    21,   56,   57,   24,   98,   81,   82,   83,  102,  103,
 /*   180 */   109,   32,   33,   34,   35,  109,   37,   38,   39,  104,
 /*   190 */    41,   42,   43,   21,   45,  110,   24,   25,   44,   27,
 /*   200 */    51,   52,   53,   18,   21,   56,   57,   24,   21,   76,
 /*   210 */     1,   24,   25,   76,   27,   32,   33,   34,   35,   20,
 /*   220 */    37,   38,   39,   48,   41,   42,   43,   79,   45,   81,
 /*   230 */    82,   83,   97,   98,   51,   52,   53,  104,   21,   56,
 /*   240 */    57,   24,   21,  110,  109,   24,   25,  110,   27,   32,
 /*   250 */    33,   34,   35,   76,   37,   38,   39,   48,   41,   42,
 /*   260 */    43,   21,   45,   17,   24,   25,   98,   27,   51,   52,
 /*   270 */    53,   40,   21,   56,   57,   24,   79,  109,   81,   82,
 /*   280 */    83,  104,    1,   32,   33,   34,   35,  110,   37,   38,
 /*   290 */    39,   30,   41,   42,   43,   25,   45,   27,   28,   18,
 /*   300 */    54,   55,   51,   52,   53,   98,   21,   56,   57,   24,
 /*   310 */    98,   74,   17,  101,   77,   78,  109,   32,   33,   34,
 /*   320 */    35,  109,   37,   38,   39,    1,   41,   42,   43,   30,
 /*   330 */    45,    8,    9,   10,  105,  106,   51,   52,   53,  100,
 /*   340 */    21,   56,   57,   24,   18,   46,   47,   97,   98,   54,
 /*   350 */    55,   32,   33,   34,   35,   31,   37,   38,   39,  109,
 /*   360 */    41,   42,   43,   25,   45,   27,   28,   20,   29,   22,
 /*   370 */    51,   52,   53,    1,   21,   56,   57,   24,   98,   74,
 /*   380 */    75,  101,   25,   78,   27,   32,   33,   34,   35,  109,
 /*   390 */    37,   38,   39,   18,   41,   42,   43,   17,   45,  105,
 /*   400 */   106,   26,   17,   31,   51,   52,   53,   98,   21,   56,
 /*   410 */    57,   24,   98,   76,    1,  101,  105,  106,  109,   32,
 /*   420 */    33,   34,   35,  109,   37,   38,   39,   48,   41,   42,
 /*   430 */    43,   18,   45,   48,   54,   55,   17,   17,   51,   52,
 /*   440 */    53,   47,   21,   56,   57,   24,   46,   47,   29,   29,
 /*   450 */   113,   99,  100,   32,   33,   34,   35,   18,   37,   38,
 /*   460 */    39,   18,   41,   42,   43,   26,   45,  105,  106,    1,
 /*   470 */    20,   98,   51,   52,   53,   76,   21,   56,   57,   24,
 /*   480 */    98,   14,  109,  101,   17,    1,   18,   32,   33,   34,
 /*   490 */    35,  109,   37,   38,   39,   18,   41,   42,   43,   20,
 /*   500 */    45,   22,   18,   26,   98,   98,   51,   52,   53,   98,
 /*   510 */    21,   56,   57,   24,   98,  109,  109,  101,    1,    1,
 /*   520 */   109,   32,   33,   34,   35,  109,   37,   38,   39,    1,
 /*   530 */    41,   42,   43,   25,   45,   27,   18,   20,   98,   98,
 /*   540 */    51,   52,   53,   98,   21,   56,   57,   24,   44,  109,
 /*   550 */   109,   17,   98,  107,  109,   32,   33,   34,   35,   31,
 /*   560 */    37,   38,   39,  109,   41,   42,   43,    1,   45,    1,
 /*   570 */    17,   17,   98,   76,   51,   52,   53,   18,   21,   56,
 /*   580 */    57,   24,   29,  109,   17,   78,   18,   17,   17,   32,
 /*   590 */    33,   34,   35,   17,   37,   38,   39,   31,   41,   42,
 /*   600 */    43,   76,   45,   18,   20,   17,   17,   21,   51,   52,
 /*   610 */    53,   48,   21,   56,   57,   24,   76,   18,   18,   18,
 /*   620 */    48,   18,   18,   32,   33,   34,   35,   17,   37,   38,
 /*   630 */    39,   20,   41,   42,   43,   21,   45,   76,   48,   17,
 /*   640 */    19,   18,   51,   52,   53,   17,   21,   56,   57,   24,
 /*   650 */    26,  106,   21,   48,   18,   48,   48,   32,   33,   34,
 /*   660 */    35,   17,   37,   38,   39,   30,   41,   42,   43,   29,
 /*   670 */    45,   20,   17,   21,   20,   30,   51,   52,   53,   18,
 /*   680 */    21,   56,   57,   24,   20,   17,   17,   30,   17,   17,
 /*   690 */    17,   32,   33,   34,   35,   48,   37,   38,   39,   17,
 /*   700 */    41,   42,   43,   29,   45,   26,   17,   17,   30,   17,
 /*   710 */    51,   52,   53,   17,   21,   56,   57,   24,   31,   20,
 /*   720 */     5,   17,   62,   26,   62,   32,   33,   34,   35,   76,
 /*   730 */    37,   38,   39,   26,   41,   42,   43,   29,   45,   17,
 /*   740 */    30,   20,   62,   31,   51,   52,   53,   17,   21,   56,
 /*   750 */    57,   24,   20,   76,   48,   26,   17,   17,   20,   32,
 /*   760 */    33,   34,   35,   26,   37,   38,   39,   76,   41,   42,
 /*   770 */    43,   26,   45,    0,   18,   17,   48,   26,   51,   52,
 /*   780 */    53,   62,   18,   56,   57,   24,   17,   76,   21,  114,
 /*   790 */   114,  114,  114,   32,   33,   34,   35,  114,   37,   38,
 /*   800 */    39,  114,   41,   42,   43,   70,   45,  114,  114,  114,
 /*   810 */   114,  114,   51,   52,   53,   80,  114,   56,   57,   84,
 /*   820 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   830 */    95,   96,   70,    2,    3,    4,    5,    6,    7,    8,
 /*   840 */     9,   10,   80,  114,  114,  114,   84,   85,   86,   87,
 /*   850 */    88,   89,   90,   91,   92,   93,   94,   95,   96,  114,
 /*   860 */   114,  114,   31,  114,   70,    2,    3,    4,    5,    6,
 /*   870 */     7,    8,    9,   10,   80,  114,  114,  114,   84,   85,
 /*   880 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*   890 */    96,   70,  114,   71,   72,   73,   74,   75,  114,   77,
 /*   900 */    78,   80,  114,  114,  114,   84,   85,   86,   87,   88,
 /*   910 */    89,   90,   91,   92,   93,   94,   95,   96,   70,  114,
 /*   920 */    71,   72,   73,   74,   75,  114,   77,   78,   80,  114,
 /*   930 */   114,  114,   84,   85,   86,   87,   88,   89,   90,   91,
 /*   940 */    92,   93,   94,   95,   96,   70,  114,  114,  114,  114,
 /*   950 */    71,   72,   73,   74,   75,   80,   77,   78,  114,   84,
 /*   960 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   970 */    95,   96,   70,  114,  114,  114,  114,   71,   72,   73,
 /*   980 */    74,   75,   80,   77,   78,  114,   84,   85,   86,   87,
 /*   990 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   70,
 /*  1000 */    54,   55,  114,  114,   58,   59,   60,   61,  114,   80,
 /*  1010 */   114,  114,  114,   84,   85,   86,   87,   88,   89,   90,
 /*  1020 */    91,   92,   93,   94,   95,   96,   70,   21,  114,  114,
 /*  1030 */    24,   25,  114,   27,   28,  114,   80,  114,  114,  114,
 /*  1040 */    84,   85,   86,   87,   88,   89,   90,   91,   92,   93,
 /*  1050 */    94,   95,   96,   70,  114,   21,  114,   16,   24,   25,
 /*  1060 */    19,   27,   28,   80,   23,   24,  114,   84,   85,   86,
 /*  1070 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*  1080 */    70,   21,  114,  114,   24,   25,  114,   27,   28,  114,
 /*  1090 */    80,  114,  114,  114,   84,   85,   86,   87,   88,   89,
 /*  1100 */    90,   91,   92,   93,   94,   95,   96,   70,  114,   21,
 /*  1110 */   114,  114,   24,   25,  114,   27,   28,   80,  114,  114,
 /*  1120 */   114,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*  1130 */    93,   94,   95,   96,   70,   21,  114,  114,   24,   25,
 /*  1140 */   114,   27,   28,  114,   80,  114,  114,   70,   84,   85,
 /*  1150 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*  1160 */    96,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*  1170 */    93,   94,   95,   96,   11,   12,   13,   14,   15,  114,
 /*  1180 */    17,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*  1190 */     0,   21,  114,   30,   24,   25,  114,   27,   28,   20,
 /*  1200 */   114,  114,  114,  114,  114,  114,   16,  114,  114,   19,
 /*  1210 */    47,  114,  114,   23,   24,  114,  114,   54,   55,  114,
 /*  1220 */   114,   58,   59,   60,   61,    2,    3,    4,    5,    6,
 /*  1230 */     7,    8,    9,   10,  114,   17,   21,  114,  114,   24,
 /*  1240 */    25,   18,   27,   28,    2,    3,    4,    5,    6,    7,
 /*  1250 */     8,    9,   10,    2,    3,    4,    5,    6,    7,    8,
 /*  1260 */     9,   10,   20,  114,  114,  114,   48,   17,  114,  114,
 /*  1270 */   114,   20,   54,   55,  114,   17,   58,   59,   60,   61,
 /*  1280 */   114,   31,  114,  114,  114,  114,  114,  114,  114,   31,
 /*  1290 */   114,  114,  114,  114,    4,   17,  114,  114,  114,  114,
 /*  1300 */   114,  114,  114,  114,   54,   55,  114,   17,   58,   59,
 /*  1310 */    60,   61,   54,   55,    4,  114,   58,   59,   60,   61,
 /*  1320 */   114,   31,   32,   33,   34,   35,   36,   17,    4,  114,
 /*  1330 */    20,  114,   54,   55,   17,  114,   58,   59,   60,   61,
 /*  1340 */   114,   17,   32,   33,   34,   35,   36,  114,   31,  114,
 /*  1350 */    17,    4,  114,  114,  114,   31,   32,   33,   34,   35,
 /*  1360 */    36,   17,  114,  114,   17,  114,  114,  114,  114,  114,
 /*  1370 */   114,   54,   55,  114,  114,   58,   59,   60,   61,   32,
 /*  1380 */    33,   34,   35,   36,  114,  114,  114,   54,   55,  114,
 /*  1390 */   114,   58,   59,   60,   61,  114,  114,  114,   54,   55,
 /*  1400 */   114,  114,   58,   59,   60,   61,
};
#define YY_SHIFT_USE_DFLT (-25)
static short yy_shift_ofst[] = {
 /*     0 */  1041,  773, 1190,  -25,  -25,  -25,  -25,  567,  601,  -25,
 /*    10 */   589,  347, 1114,  586,  -25,  -25, 1034,  240,  -25,  339,
 /*    20 */   740,  645, 1324, 1310,  625,  -25,  319,  -25,  -25,  -25,
 /*    30 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*    40 */   -25,  -25,  326,  -25,  604,  -25, 1163,  450,   47,  -21,
 /*    50 */    27,  738,  251,  591,  -25,  584,  693,  183,  -25, 1163,
 /*    60 */    39,  659,  115,  -25,  571,  154, 1163, 1242,  761,  455,
 /*    70 */   -25,  622,  468,  -25,  622,  -25,   -7, 1163,  -25,  863,
 /*    80 */  1163, 1163,  -25, 1163,  554,  -25,  394,  246,  379,  -25,
 /*    90 */   175,  -25,  728,  -25,  323, 1163,  323, 1163,  323, 1163,
 /*   100 */   323, 1163,  323, 1163,  -25, 1163,  -25, 1163,  -25, 1163,
 /*   110 */   831,  -25,  299,  769,  -15, 1250,  134,  -25, 1278,  -25,
 /*   120 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   130 */   380,  608,  -25,  605,  -25,  607,  -25, 1258,  324,  -25,
 /*   140 */   -25, 1163,  -25, 1163,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   150 */   -25, 1218,  -25,  209,  -25, 1333,  -25,  719, 1344,  -25,
 /*   160 */   -25,  -25,  -25,  680,  662,  660,  -25,  673,  657, 1317,
 /*   170 */   -25,  528,  -25,  -25,  323,  655,  -24, 1163,  -25,  647,
 /*   180 */   679, 1163,  -25,  385,  697, 1163,  -25,  706,  737, 1163,
 /*   190 */   -25,  -25,  -25,  -25, 1163,  518,  -25, 1163,  -25,  863,
 /*   200 */   -25, 1163, 1223,  -25,  570,  400,  576,  661,  -25,  295,
 /*   210 */   563,  600,  -25,  572,  603,  -25,  590,  623,  -25,  467,
 /*   220 */   636,  -25,  696,  261, 1278,   52,  756,  -25,  588,  484,
 /*   230 */   -25,  588,  -25,  624,  946,  -25,  -25,  588,  281,  -25,
 /*   240 */   588,  141,  -25,  588,  568,  -25,  588,  413,  -25,  -25,
 /*   250 */   -25,  534,  504, 1163, 1179,  761,  727,  -25,  -25, 1251,
 /*   260 */   -25,  -25,  231,  199,  557,  489,  -25,  -25,  517,  761,
 /*   270 */   387,  -25, 1347,  -25,  745,  946,  -25,  739,  729,  946,
 /*   280 */   -25,  730,  707,  946,  -25,  704,  715,  692,  690,  682,
 /*   290 */   672,  671,  669,  -25,  566,  654,  652,  -25,  508,  640,
 /*   300 */   644,  635, 1290,  651,  353,  -25,  421,  -25,  372,  664,
 /*   310 */    13,  -25,  523,  -25,  674,  689,  678,  687,  699,   81,
 /*   320 */   -25,  149,  -25,  708,  722,  710,  712,  732,  217,  -25,
 /*   330 */   285,  -25, 1060,  187,  -25,  270,  758,  751,  946,  764,
 /*   340 */   -25,  -16,   37,  946,  185,  -25,  -25,  -25,  338,   92,
 /*   350 */   439,  -25,  946,  443,  -25,  553,   23,  -25,  946,  559,
 /*   360 */   -25,  420,  375,  -25,  946,  585,  -25,  419,  477,  -25,
 /*   370 */   946,  599,  -25,  -25, 1170,  221,  119,  357,  172,  -25,
 /*   380 */   104,  610,  611, 1006,  614,  -25,  -25,  621,  628,  479,
 /*   390 */  1215,  631,  -25,  -25,  668,  721, 1088,  767,  -25,  -25,
 /*   400 */   -25,
};
#define YY_REDUCE_USE_DFLT (-66)
static short yy_reduce_ofst[] = {
 /*     0 */   -60,  -66,  -41,  -66,  -66,  -66,  -66,  -66,  -66,  -66,
 /*    10 */   -66,  -66,  849,  -66,  -66,  -66,   33,  507,  -66,  -66,
 /*    20 */   -66,  -66,   12,  197,  848,  -66, 1077,  -66,  -66,  -66,
 /*    30 */   -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,
 /*    40 */   -66,  -66,  -66,  -66,  -66,  -66,  135,  -66,  735, 1077,
 /*    50 */   -66,  -66,  956, 1077,  -66,  -66,  929, 1077,  -66,  250,
 /*    60 */   -66,  902, 1077,  -66,  -66,  -66,  411,  -66,  794, 1077,
 /*    70 */   -66,  352,  -66,  -66,  239,  -66,  -66,  212,  -66,  -66,
 /*    80 */   -22,  440,  -66,   32,  446,  -66,  -66,  -66,  -66,  -66,
 /*    90 */   -66,  -66,  -66,  -66,  -66,  207,  -66,  309,  -66,   71,
 /*   100 */   -66,  441,  -66,  445,  -66,  406,  -66,  407,  -66,  168,
 /*   110 */   -66,  -66,  -66,  -66,  -66,  133,  -66,  -66,  137,  -66,
 /*   120 */   -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,
 /*   130 */   -66,  -66,  -66,  -66,  -66,  -66,  -66,   41,  -66,  -66,
 /*   140 */   -66,    3,  -66,  454,  -66,  -66,  -66,  -66,  -66,  -66,
 /*   150 */   -66,  -51,  -66,  -66,  -66,   51,  -66,  -66,  337,  -66,
 /*   160 */   -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,   85,
 /*   170 */   -66,  -66,  -66,  -66,  -66,  -66,  -66,  280,  -66,  -66,
 /*   180 */   -66,  314,  -66,  -66,  -66,  382,  -66,  -66,  -66,  416,
 /*   190 */   -66,  -66,  -66,  -66,   76,  -66,  -66,  -65,  -66,  -66,
 /*   200 */   -66,  474,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,
 /*   210 */   -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,
 /*   220 */   -66,  -66,  -66,  -66,  177,  -66,  -66,  -66,  229,  -66,
 /*   230 */   -66,  545,  -66,  -66,  561,  -66,  -66,  311,  -66,  -66,
 /*   240 */   294,  -66,  -66,   -9,  -66,  -66,  362,  -66,  -66,  -66,
 /*   250 */   -66,  -66,  -66,  373,  -66,  821, 1077,  -66,  -66,  -66,
 /*   260 */   -66,  -66,  -66,  -66,  875, 1077,  -66,  -66,  -66,  762,
 /*   270 */  1077,  -66,   94,  -66,  -66,  691,  -66,  -66,  -66,  677,
 /*   280 */   -66,  -66,  -66,  653,  -66,  -66,  -66,  -66,  -66,  -66,
 /*   290 */   -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,  -66,
 /*   300 */   -66,  -66,  148,  -66,  983,  -66, 1077,  -66,  -66,  -66,
 /*   310 */  1037,  -66, 1077,  -66,  -66,  -66,  -66,  -66,  -66, 1010,
 /*   320 */   -66, 1077,  -66,  -66,  -66,  -66,  -66,  -66, 1064,  -66,
 /*   330 */  1077,  -66,  237,  507,  -66,  -66,  -66,  -66,  711,  -66,
 /*   340 */   -66,  -66,  -66,   -3,  -66,  -66,  -66,  -66,  -66,  -66,
 /*   350 */   -66,  -66,  399,  -66,  -66,  -66,  -66,  -66,  497,  -66,
 /*   360 */   -66,  -66,  -66,  -66,  525,  -66,  -66,  -66,  -66,  -66,
 /*   370 */   540,  -66,  -66,  -66,    0,  305,  507,  -66,  507,  -66,
 /*   380 */   507,  -66,  -66,  906,  -66,  -66,  -66,  -66,  -66,  -66,
 /*   390 */   879,  -66,  -66,  -66,  -66,  -66,  822,  -66,  -66,  -66,
 /*   400 */   -66,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   587,  587,  587,  403,  405,  406,  407,  587,  587,  408,
 /*    10 */   587,  587,  587,  587,  409,  413,  587,  587,  438,  587,
 /*    20 */   587,  587,  587,  587,  587,  442,  587,  446,  468,  470,
 /*    30 */   471,  472,  473,  474,  475,  476,  477,  478,  479,  480,
 /*    40 */   481,  482,  587,  483,  587,  484,  587,  587,  587,  587,
 /*    50 */   487,  587,  587,  587,  488,  587,  587,  587,  492,  587,
 /*    60 */   587,  587,  587,  494,  587,  587,  587,  587,  587,  587,
 /*    70 */   495,  587,  587,  497,  587,  498,  587,  587,  500,  527,
 /*    80 */   587,  587,  528,  587,  587,  529,  587,  587,  587,  582,
 /*    90 */   587,  583,  587,  584,  531,  587,  532,  587,  533,  587,
 /*   100 */   534,  587,  535,  587,  541,  587,  542,  587,  543,  587,
 /*   110 */   587,  536,  546,  587,  537,  587,  587,  561,  587,  563,
 /*   120 */   565,  566,  576,  577,  578,  579,  580,  581,  562,  564,
 /*   130 */   587,  587,  538,  587,  539,  587,  540,  587,  587,  558,
 /*   140 */   559,  587,  544,  587,  545,  547,  548,  549,  550,  551,
 /*   150 */   552,  587,  553,  587,  554,  587,  567,  587,  587,  569,
 /*   160 */   574,  575,  570,  575,  577,  576,  568,  587,  555,  587,
 /*   170 */   556,  587,  557,  560,  530,  587,  587,  587,  501,  587,
 /*   180 */   587,  587,  502,  587,  587,  587,  503,  587,  587,  587,
 /*   190 */   504,  505,  506,  499,  587,  587,  507,  587,  508,  510,
 /*   200 */   509,  587,  587,  511,  587,  587,  587,  587,  512,  587,
 /*   210 */   587,  587,  513,  587,  587,  514,  587,  587,  515,  587,
 /*   220 */   587,  516,  587,  587,  587,  587,  587,  517,  587,  587,
 /*   230 */   518,  587,  523,  525,  587,  526,  524,  587,  587,  519,
 /*   240 */   587,  587,  520,  587,  587,  521,  587,  587,  522,  586,
 /*   250 */   469,  587,  587,  587,  587,  587,  587,  496,  493,  587,
 /*   260 */   491,  489,  485,  587,  587,  587,  490,  486,  587,  587,
 /*   270 */   587,  447,  587,  454,  456,  587,  459,  587,  457,  587,
 /*   280 */   460,  587,  458,  587,  461,  587,  587,  587,  587,  587,
 /*   290 */   587,  587,  587,  455,  587,  587,  587,  443,  587,  587,
 /*   300 */   587,  587,  587,  587,  587,  444,  587,  448,  587,  587,
 /*   310 */   587,  445,  587,  449,  587,  587,  587,  587,  587,  587,
 /*   320 */   451,  587,  453,  587,  587,  587,  587,  587,  587,  450,
 /*   330 */   587,  452,  587,  587,  436,  587,  587,  587,  587,  587,
 /*   340 */   439,  587,  587,  587,  587,  440,  441,  426,  587,  587,
 /*   350 */   587,  428,  587,  587,  431,  587,  587,  433,  587,  587,
 /*   360 */   435,  587,  587,  429,  587,  587,  430,  587,  587,  432,
 /*   370 */   587,  587,  434,  437,  587,  587,  587,  587,  587,  427,
 /*   380 */   587,  587,  587,  587,  587,  410,  414,  587,  587,  587,
 /*   390 */   587,  587,  411,  415,  587,  587,  587,  587,  412,  416,
 /*   400 */   404,
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
  "LESS",          "GREATER",       "NOTIDENTICAL",  "NOTEQUALS",   
  "ADD",           "SUB",           "CONCAT",        "ISSET",       
  "TYPEOF",        "INSTANCEOF",    "NEW",           "NOT",         
  "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",      "CLASS",       
  "BRACKET_OPEN",  "BRACKET_CLOSE",  "EXTENDS",       "ABSTRACT",    
  "COMMENT",       "PUBLIC",        "ASSIGN",        "PROTECTED",   
  "CONST",         "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",
  "TYPE_INTEGER",  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "BREAK",         "CONTINUE",      "IF",          
  "ELSE",          "LOOP",          "WHILE",         "FOR",         
  "IN",            "LET",           "ARROW",         "SBRACKET_OPEN",
  "SBRACKET_CLOSE",  "INCR",          "DECR",          "ECHO",        
  "RETURN",        "UNSET",         "INTEGER",       "STRING",      
  "THROW",         "TYPE_VAR",      "DOUBLE",        "NULL",        
  "TRUE",          "FALSE",         "COLON",         "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_comment",    "xx_class_definition",
  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",
  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",
  "xx_statement",  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",
  "xx_echo_statement",  "xx_return_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_for_statement",  "xx_eval_expr",  "xx_common_expr",  "xx_let_assignments",
  "xx_let_assignment",  "xx_assign_expr",  "xx_echo_expressions",  "xx_echo_expression",
  "xx_call_parameters",  "xx_declare_variable_list",  "xx_declare_variable",  "xx_isset_expr",
  "xx_array_list",  "xx_mcall_expr",  "xx_call_parameter",  "xx_array_item",
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
 /*  71 */ "xx_statement ::= xx_loop_statement",
 /*  72 */ "xx_statement ::= xx_echo_statement",
 /*  73 */ "xx_statement ::= xx_return_statement",
 /*  74 */ "xx_statement ::= xx_unset_statement",
 /*  75 */ "xx_statement ::= xx_throw_statement",
 /*  76 */ "xx_statement ::= xx_declare_statement",
 /*  77 */ "xx_statement ::= xx_break_statement",
 /*  78 */ "xx_statement ::= xx_continue_statement",
 /*  79 */ "xx_statement ::= xx_while_statement",
 /*  80 */ "xx_statement ::= xx_for_statement",
 /*  81 */ "xx_statement ::= xx_comment",
 /*  82 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  83 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  84 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  85 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  86 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  87 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  88 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  89 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  90 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /*  91 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  92 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  93 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  94 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  95 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  96 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /*  97 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /*  98 */ "xx_let_assignments ::= xx_let_assignment",
 /*  99 */ "xx_let_assignment ::= IDENTIFIER ASSIGN xx_assign_expr",
 /* 100 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER ASSIGN xx_assign_expr",
 /* 101 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 102 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 103 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 104 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 105 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 106 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 107 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 108 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 109 */ "xx_echo_expression ::= xx_common_expr",
 /* 110 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 111 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 112 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 113 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 114 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 115 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 116 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 117 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 118 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 119 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 120 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 121 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 122 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 123 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 124 */ "xx_declare_variable ::= IDENTIFIER",
 /* 125 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 126 */ "xx_assign_expr ::= xx_common_expr",
 /* 127 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 128 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 129 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 130 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 131 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 132 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 133 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 134 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 135 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 136 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 137 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 138 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 139 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 140 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 141 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 142 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 143 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 144 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 145 */ "xx_common_expr ::= IDENTIFIER",
 /* 146 */ "xx_common_expr ::= INTEGER",
 /* 147 */ "xx_common_expr ::= STRING",
 /* 148 */ "xx_common_expr ::= DOUBLE",
 /* 149 */ "xx_common_expr ::= NULL",
 /* 150 */ "xx_common_expr ::= TRUE",
 /* 151 */ "xx_common_expr ::= FALSE",
 /* 152 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 153 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 154 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 155 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 156 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 157 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 158 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 159 */ "xx_common_expr ::= xx_mcall_expr",
 /* 160 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 161 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 162 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 163 */ "xx_call_parameters ::= xx_call_parameter",
 /* 164 */ "xx_call_parameter ::= xx_literal_expr",
 /* 165 */ "xx_call_parameter ::= IDENTIFIER",
 /* 166 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 167 */ "xx_array_list ::= xx_array_item",
 /* 168 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 169 */ "xx_array_item ::= xx_array_value",
 /* 170 */ "xx_array_key ::= IDENTIFIER",
 /* 171 */ "xx_array_key ::= STRING",
 /* 172 */ "xx_array_key ::= INTEGER",
 /* 173 */ "xx_array_value ::= xx_literal_expr",
 /* 174 */ "xx_array_value ::= IDENTIFIER",
 /* 175 */ "xx_literal_expr ::= INTEGER",
 /* 176 */ "xx_literal_expr ::= STRING",
 /* 177 */ "xx_literal_expr ::= DOUBLE",
 /* 178 */ "xx_literal_expr ::= NULL",
 /* 179 */ "xx_literal_expr ::= FALSE",
 /* 180 */ "xx_literal_expr ::= TRUE",
 /* 181 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 182 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 183 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 184 */ "xx_eval_expr ::= xx_common_expr",
 /* 185 */ "xx_comment ::= COMMENT",
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
    case 54:
    case 55:
    case 56:
    case 57:
    case 58:
    case 59:
    case 60:
    case 61:
    case 62:
// 702 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1605 "parser.c"
      break;
    case 65:
// 715 "parser.lemon"
{ json_object_put((yypminor->yy7)); }
// 1610 "parser.c"
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
  { 64, 1 },
  { 65, 1 },
  { 66, 2 },
  { 66, 1 },
  { 67, 1 },
  { 67, 1 },
  { 67, 1 },
  { 68, 3 },
  { 69, 5 },
  { 69, 7 },
  { 69, 6 },
  { 69, 8 },
  { 69, 4 },
  { 69, 6 },
  { 69, 5 },
  { 69, 7 },
  { 71, 1 },
  { 71, 1 },
  { 71, 1 },
  { 71, 2 },
  { 71, 2 },
  { 71, 2 },
  { 71, 2 },
  { 71, 3 },
  { 71, 3 },
  { 72, 2 },
  { 72, 1 },
  { 75, 4 },
  { 75, 3 },
  { 75, 5 },
  { 75, 6 },
  { 75, 3 },
  { 75, 4 },
  { 75, 5 },
  { 75, 6 },
  { 73, 2 },
  { 73, 1 },
  { 74, 2 },
  { 77, 6 },
  { 77, 5 },
  { 74, 1 },
  { 78, 7 },
  { 78, 8 },
  { 78, 8 },
  { 78, 9 },
  { 78, 8 },
  { 78, 9 },
  { 78, 9 },
  { 78, 10 },
  { 78, 7 },
  { 78, 8 },
  { 78, 8 },
  { 78, 9 },
  { 79, 3 },
  { 79, 1 },
  { 81, 1 },
  { 81, 2 },
  { 81, 2 },
  { 81, 3 },
  { 81, 4 },
  { 81, 4 },
  { 83, 3 },
  { 82, 1 },
  { 82, 1 },
  { 82, 1 },
  { 82, 1 },
  { 82, 1 },
  { 80, 2 },
  { 80, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 84, 1 },
  { 93, 2 },
  { 94, 2 },
  { 86, 4 },
  { 86, 7 },
  { 86, 5 },
  { 86, 9 },
  { 86, 8 },
  { 86, 8 },
  { 87, 3 },
  { 87, 4 },
  { 95, 4 },
  { 95, 5 },
  { 96, 7 },
  { 96, 9 },
  { 85, 3 },
  { 99, 3 },
  { 99, 1 },
  { 100, 3 },
  { 100, 5 },
  { 100, 7 },
  { 100, 5 },
  { 100, 6 },
  { 100, 2 },
  { 100, 2 },
  { 88, 3 },
  { 102, 3 },
  { 102, 1 },
  { 103, 1 },
  { 89, 3 },
  { 90, 5 },
  { 90, 6 },
  { 90, 6 },
  { 90, 6 },
  { 91, 3 },
  { 91, 7 },
  { 92, 3 },
  { 92, 3 },
  { 92, 3 },
  { 92, 3 },
  { 92, 3 },
  { 105, 3 },
  { 105, 1 },
  { 106, 1 },
  { 106, 3 },
  { 101, 1 },
  { 98, 2 },
  { 98, 2 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 4 },
  { 98, 4 },
  { 98, 4 },
  { 98, 3 },
  { 98, 3 },
  { 98, 3 },
  { 98, 2 },
  { 98, 2 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 2 },
  { 98, 3 },
  { 98, 2 },
  { 98, 4 },
  { 98, 5 },
  { 98, 4 },
  { 98, 3 },
  { 98, 1 },
  { 109, 6 },
  { 109, 5 },
  { 104, 3 },
  { 104, 1 },
  { 110, 1 },
  { 110, 1 },
  { 108, 3 },
  { 108, 1 },
  { 111, 3 },
  { 111, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 113, 1 },
  { 113, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 107, 4 },
  { 107, 4 },
  { 107, 4 },
  { 97, 1 },
  { 70, 1 },
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
// 711 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy7;
}
// 2013 "parser.c"
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
      case 79:
      case 80:
      case 81:
      case 126:
      case 159:
      case 164:
      case 173:
      case 184:
// 717 "parser.lemon"
{
	yygotominor.yy7 = yymsp[0].minor.yy7;
}
// 2041 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 721 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(yymsp[-1].minor.yy7, yymsp[0].minor.yy7);
}
// 2052 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 68:
      case 98:
      case 108:
      case 123:
      case 163:
      case 167:
// 725 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(NULL, yymsp[0].minor.yy7);
}
// 2069 "parser.c"
        break;
      case 7:
// 741 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(16,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2078 "parser.c"
        break;
      case 8:
// 745 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, 0, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2088 "parser.c"
        break;
      case 9:
// 749 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy7, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-6].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2099 "parser.c"
        break;
      case 10:
// 753 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, 1, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2110 "parser.c"
        break;
      case 11:
// 757 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy7, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-7].minor);
  yy_destructor(19,&yymsp[-6].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2122 "parser.c"
        break;
      case 12:
// 761 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2132 "parser.c"
        break;
      case 13:
// 765 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2143 "parser.c"
        break;
      case 14:
// 769 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2154 "parser.c"
        break;
      case 15:
// 773 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2166 "parser.c"
        break;
      case 16:
// 777 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
}
// 2173 "parser.c"
        break;
      case 17:
// 781 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 2180 "parser.c"
        break;
      case 18:
// 785 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, yymsp[0].minor.yy7, NULL, status->scanner_state);
}
// 2187 "parser.c"
        break;
      case 19:
// 789 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
}
// 2194 "parser.c"
        break;
      case 20:
// 793 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 2201 "parser.c"
        break;
      case 21:
// 797 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[0].minor.yy7, NULL, yymsp[-1].minor.yy7, status->scanner_state);
}
// 2208 "parser.c"
        break;
      case 22:
// 801 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, yymsp[0].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
}
// 2215 "parser.c"
        break;
      case 23:
// 805 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
}
// 2222 "parser.c"
        break;
      case 24:
// 809 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, yymsp[0].minor.yy7, yymsp[-2].minor.yy7, status->scanner_state);
}
// 2229 "parser.c"
        break;
      case 27:
// 821 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2238 "parser.c"
        break;
      case 28:
// 825 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2247 "parser.c"
        break;
      case 29:
// 829 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2257 "parser.c"
        break;
      case 30:
// 833 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2267 "parser.c"
        break;
      case 31:
// 837 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2276 "parser.c"
        break;
      case 32:
// 841 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2285 "parser.c"
        break;
      case 33:
// 845 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2295 "parser.c"
        break;
      case 34:
// 849 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2305 "parser.c"
        break;
      case 38:
// 865 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2315 "parser.c"
        break;
      case 39:
// 869 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2325 "parser.c"
        break;
      case 41:
// 877 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2338 "parser.c"
        break;
      case 42:
// 881 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2351 "parser.c"
        break;
      case 43:
// 885 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2364 "parser.c"
        break;
      case 44:
// 889 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2377 "parser.c"
        break;
      case 45:
// 893 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2390 "parser.c"
        break;
      case 46:
// 897 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-8].minor);
  yy_destructor(29,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2403 "parser.c"
        break;
      case 47:
// 901 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2416 "parser.c"
        break;
      case 48:
// 905 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy7, yymsp[-1].minor.yy7, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-8].minor);
  yy_destructor(29,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2429 "parser.c"
        break;
      case 49:
// 909 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2442 "parser.c"
        break;
      case 50:
// 913 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2455 "parser.c"
        break;
      case 51:
// 917 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2468 "parser.c"
        break;
      case 52:
// 921 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2481 "parser.c"
        break;
      case 53:
      case 97:
      case 107:
      case 122:
      case 162:
      case 166:
// 925 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2494 "parser.c"
        break;
      case 55:
// 933 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2501 "parser.c"
        break;
      case 56:
// 937 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(yymsp[-1].minor.yy7, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2508 "parser.c"
        break;
      case 57:
// 941 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, yymsp[-1].minor.yy7, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2515 "parser.c"
        break;
      case 58:
// 945 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2523 "parser.c"
        break;
      case 59:
// 949 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(yymsp[-3].minor.yy7, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2531 "parser.c"
        break;
      case 60:
// 953 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, yymsp[-3].minor.yy7, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2539 "parser.c"
        break;
      case 61:
// 957 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2548 "parser.c"
        break;
      case 62:
// 961 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(32,&yymsp[0].minor);
}
// 2556 "parser.c"
        break;
      case 63:
// 965 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(33,&yymsp[0].minor);
}
// 2564 "parser.c"
        break;
      case 64:
// 969 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(34,&yymsp[0].minor);
}
// 2572 "parser.c"
        break;
      case 65:
// 973 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(35,&yymsp[0].minor);
}
// 2580 "parser.c"
        break;
      case 66:
// 977 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(36,&yymsp[0].minor);
}
// 2588 "parser.c"
        break;
      case 82:
// 1041 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2597 "parser.c"
        break;
      case 83:
// 1045 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2606 "parser.c"
        break;
      case 84:
// 1049 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-2].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2616 "parser.c"
        break;
      case 85:
// 1053 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-5].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2629 "parser.c"
        break;
      case 86:
// 1057 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2639 "parser.c"
        break;
      case 87:
// 1061 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-7].minor.yy7, yymsp[-5].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(39,&yymsp[-8].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2652 "parser.c"
        break;
      case 88:
// 1065 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-6].minor.yy7, yymsp[-4].minor.yy7, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2665 "parser.c"
        break;
      case 89:
// 1069 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-6].minor.yy7, NULL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(39,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2678 "parser.c"
        break;
      case 90:
// 1073 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2688 "parser.c"
        break;
      case 91:
// 1077 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_loop_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2698 "parser.c"
        break;
      case 92:
// 1081 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_while_statement(yymsp[-2].minor.yy7, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(20,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2708 "parser.c"
        break;
      case 93:
// 1085 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_while_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2718 "parser.c"
        break;
      case 94:
// 1089 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, NULL, yymsp[-5].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(43,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2729 "parser.c"
        break;
      case 95:
// 1093 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(43,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 2741 "parser.c"
        break;
      case 96:
// 1097 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2750 "parser.c"
        break;
      case 99:
// 1110 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2758 "parser.c"
        break;
      case 100:
// 1115 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2767 "parser.c"
        break;
      case 101:
// 1120 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property-append", yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(46,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2778 "parser.c"
        break;
      case 102:
// 1125 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2788 "parser.c"
        break;
      case 103:
// 1130 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(47,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2798 "parser.c"
        break;
      case 104:
// 1135 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[0].minor);
}
// 2806 "parser.c"
        break;
      case 105:
// 1140 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[0].minor);
}
// 2814 "parser.c"
        break;
      case 106:
// 1144 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_echo_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2823 "parser.c"
        break;
      case 109:
// 1156 "parser.lemon"
{
	yygotominor.yy7 = yymsp[0].minor.yy7;;
}
// 2830 "parser.c"
        break;
      case 110:
// 1161 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_return_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2839 "parser.c"
        break;
      case 111:
// 1166 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2853 "parser.c"
        break;
      case 112:
// 1175 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2868 "parser.c"
        break;
      case 113:
// 1184 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2883 "parser.c"
        break;
      case 114:
// 1193 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(53,&yymsp[-5].minor);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(48,&yymsp[-1].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2898 "parser.c"
        break;
      case 115:
// 1202 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2907 "parser.c"
        break;
      case 116:
// 1207 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy7, status->scanner_state);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(14,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2919 "parser.c"
        break;
      case 117:
// 1211 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2928 "parser.c"
        break;
      case 118:
// 1215 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2937 "parser.c"
        break;
      case 119:
// 1219 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2946 "parser.c"
        break;
      case 120:
// 1223 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2955 "parser.c"
        break;
      case 121:
// 1227 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[0].minor);
}
// 2964 "parser.c"
        break;
      case 124:
// 1239 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2971 "parser.c"
        break;
      case 125:
// 1243 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 2979 "parser.c"
        break;
      case 127:
// 1251 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 2987 "parser.c"
        break;
      case 128:
// 1255 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("isset", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 2995 "parser.c"
        break;
      case 129:
// 1259 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("equals", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3003 "parser.c"
        break;
      case 130:
// 1263 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not-equals", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3011 "parser.c"
        break;
      case 131:
// 1267 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("identical", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3019 "parser.c"
        break;
      case 132:
// 1271 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not-identical", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3027 "parser.c"
        break;
      case 133:
// 1275 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("less", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3035 "parser.c"
        break;
      case 134:
// 1279 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("greater", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3043 "parser.c"
        break;
      case 135:
// 1283 "parser.lemon"
{
	yygotominor.yy7 = yymsp[-1].minor.yy7;
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3052 "parser.c"
        break;
      case 136:
// 1287 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
}
// 3060 "parser.c"
        break;
      case 137:
      case 181:
// 1291 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3070 "parser.c"
        break;
      case 138:
      case 182:
// 1296 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3080 "parser.c"
        break;
      case 139:
      case 183:
// 1301 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3090 "parser.c"
        break;
      case 140:
// 1306 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("add", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3098 "parser.c"
        break;
      case 141:
// 1311 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("sub", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3106 "parser.c"
        break;
      case 142:
// 1316 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("concat", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3114 "parser.c"
        break;
      case 143:
// 1321 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("instanceof", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3122 "parser.c"
        break;
      case 144:
// 1326 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("typeof", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3130 "parser.c"
        break;
      case 145:
      case 165:
      case 170:
      case 174:
// 1331 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3140 "parser.c"
        break;
      case 146:
      case 172:
      case 175:
// 1336 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3149 "parser.c"
        break;
      case 147:
      case 171:
      case 176:
// 1341 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3158 "parser.c"
        break;
      case 148:
      case 177:
// 1346 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3166 "parser.c"
        break;
      case 149:
      case 178:
// 1351 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[0].minor);
}
// 3175 "parser.c"
        break;
      case 150:
      case 180:
// 1356 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[0].minor);
}
// 3184 "parser.c"
        break;
      case 151:
      case 179:
// 1361 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[0].minor);
}
// 3193 "parser.c"
        break;
      case 152:
// 1366 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3202 "parser.c"
        break;
      case 153:
// 1371 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array", yymsp[-1].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(48,&yymsp[0].minor);
}
// 3211 "parser.c"
        break;
      case 154:
// 1376 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3219 "parser.c"
        break;
      case 155:
// 1381 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3229 "parser.c"
        break;
      case 156:
// 1386 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(14,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3239 "parser.c"
        break;
      case 157:
// 1391 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3248 "parser.c"
        break;
      case 158:
// 1396 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3257 "parser.c"
        break;
      case 160:
// 1406 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3267 "parser.c"
        break;
      case 161:
// 1411 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3277 "parser.c"
        break;
      case 168:
// 1439 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_array_item(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
}
// 3285 "parser.c"
        break;
      case 169:
// 1443 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_array_item(NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 3292 "parser.c"
        break;
      case 185:
// 1510 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3299 "parser.c"
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
// 668 "parser.lemon"


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

// 3375 "parser.c"
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

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team                                       |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the Zephir license,    |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the Zephir license and are unable   |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

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
			case XX_T_EXTENDS:
				xx_(xx_parser, XX_EXTENDS, NULL, parser_status);
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
			case XX_T_ISSET:
				xx_(xx_parser, XX_ISSET, NULL, parser_status);
				break;
			case XX_T_UNSET:
				xx_(xx_parser, XX_UNSET, NULL, parser_status);
				break;
			case XX_T_THROW:
				xx_(xx_parser, XX_THROW, NULL, parser_status);
				break;
			case XX_T_FOR:
				xx_(xx_parser, XX_FOR, NULL, parser_status);
				break;
			case XX_T_IN:
				xx_(xx_parser, XX_IN, NULL, parser_status);
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
			case XX_T_NOTEQUALS:
				xx_(xx_parser, XX_NOTEQUALS, NULL, parser_status);
				break;
			case XX_T_IDENTICAL:
				xx_(xx_parser, XX_IDENTICAL, NULL, parser_status);
				break;
			case XX_T_NOTIDENTICAL:
				xx_(xx_parser, XX_NOTIDENTICAL, NULL, parser_status);
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
			case XX_T_NOT:
				xx_(xx_parser, XX_NOT, NULL, parser_status);
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
			case XX_T_DOT:
				xx_(xx_parser, XX_CONCAT, NULL, parser_status);
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
					char *x = malloc(sizeof(char) * 10000);
					if (state->start) {
						sprintf(x, "Scanner error: %d %s", scanner_status, state->start);
					} else {
						sprintf(x, "Scanner error: %d", scanner_status);
					}
					json_object *syntax_error = json_object_new_object();
					json_object_object_add(syntax_error, "type", json_object_new_string("error"));
					json_object_object_add(syntax_error, "message", json_object_new_string(x));
					//free(x);
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
