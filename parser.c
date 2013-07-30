/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 39 "parser.lemon"


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

static json_object *xx_ret_return_statement(xx_parser_token *D, json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return"));
	json_object_object_add(ret, "expr", expr);

	if (D) {
		json_object_object_add(ret, "domain", json_object_new_string(D->token));
	}

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


// 640 "parser.c"
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
#define YYNOCODE 113
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy63;
  int yy225;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 422
#define YYNRULE 195
#define YYERRORSYMBOL 63
#define YYERRSYMDT yy225
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
 /*     0 */   334,  293,   88,  234,  618,    1,    2,  421,    4,    5,
 /*    10 */     6,  213,  222,  228,  225,  372,   42,   44,   46,  290,
 /*    20 */    55,   59,   64,  373,   71,  232,  221,  134,  237,  183,
 /*    30 */   167,  175,  189,   97,  341,  204,  231,  234,  396,  212,
 /*    40 */   399,  400,  150,  355,  367,  213,  222,  228,  225,  258,
 /*    50 */    42,   44,   46,  422,   55,   59,   64,  127,   71,  140,
 /*    60 */   131,  136,  134,  152,  167,  175,  189,  384,   50,  204,
 /*    70 */   231,  234,  353,   17,  368,  385,  394,  367,   14,  213,
 /*    80 */   222,  228,  225,    8,   42,   44,   46,   77,   55,   59,
 /*    90 */    64,  237,   71,  248,  130,  131,  136,  371,  167,  175,
 /*   100 */   189,    9,  343,  204,  231,  234,   86,  148,  156,  321,
 /*   110 */   164,  165,  256,  213,  222,  228,  225,  390,   42,   44,
 /*   120 */    46,  114,   55,   59,   64,  391,   71,   79,  110,  112,
 /*   130 */   250,  252,  167,  175,  189,  269,  282,  204,  231,  234,
 /*   140 */   289,   11,  314,  298,  302,  229,  221,  213,  222,  228,
 /*   150 */   225,  134,   42,   44,   46,   20,   55,   59,   64,  315,
 /*   160 */    71,  314,  298,  302,  129,   18,  167,  175,  189,   86,
 /*   170 */   350,  204,  231,  234,  443,  397,  368,  398,  382,  367,
 /*   180 */   388,  213,  222,  228,  225,  133,   42,   44,   46,   22,
 /*   190 */    55,   59,   64,  441,   71,  179,  319,   19,   85,  344,
 /*   200 */   167,  175,  189,  216,  287,  204,  231,  234,  446,   21,
 /*   210 */   128,  319,   19,   43,  344,  213,  222,  228,  225,  233,
 /*   220 */    42,   44,   46,  180,   55,   59,   64,  105,   71,    3,
 /*   230 */     4,    5,    6,   45,  167,  175,  189,   86,  352,  204,
 /*   240 */   231,  234,  444,   81,   98,  319,   19,   51,  344,  213,
 /*   250 */   222,  228,  225,   52,   42,   44,   46,  237,   55,   59,
 /*   260 */    64,  329,   71,  314,  298,  302,  107,   56,  167,  175,
 /*   270 */   189,   86,   63,  204,  231,  234,  440,   65,  254,  319,
 /*   280 */    19,   76,  344,  213,  222,  228,  225,   78,   42,   44,
 /*   290 */    46,   75,   55,   59,   64,  445,   71,   99,  319,   19,
 /*   300 */   146,  344,  167,  175,  189,  216,  332,  204,  231,  234,
 /*   310 */   294,  298,  302,  320,  109,  335,  357,  213,  222,  228,
 /*   320 */   225,  230,   42,   44,   46,  237,   55,   59,   64,  354,
 /*   330 */    71,  273,  355,  367,  101,  103,  167,  175,  189,  216,
 /*   340 */    70,  204,  231,  234,  195,  370,   67,  376,  357,   88,
 /*   350 */    86,  213,  222,  228,  225,  227,   42,   44,   46,  100,
 /*   360 */    55,   59,   64,   12,   71,  402,   88,   79,  110,  112,
 /*   370 */   167,  175,  189,   66,   27,  204,  231,  234,  237,  211,
 /*   380 */    87,  198,  201,  226,  221,  213,  222,  228,  225,  172,
 /*   390 */    42,   44,   46,  268,   55,   59,   64,   97,   71,  242,
 /*   400 */    88,  216,  237,   82,  167,  175,  189,   83,  283,  204,
 /*   410 */   231,  234,  237,  102,  168,  174,  237,  224,  104,  213,
 /*   420 */   222,  228,  225,  271,   42,   44,   46,   84,   55,   59,
 /*   430 */    64,   97,   71,  239,   88,  223,  221,  276,  167,  175,
 /*   440 */   189,  216,   25,  204,  231,  234,  237,  214,  221,  370,
 /*   450 */   160,  376,  111,  213,  222,  228,  225,  215,   42,   44,
 /*   460 */    46,  145,   55,   59,   64,   97,   71,  272,   88,  320,
 /*   470 */   389,  335,  167,  175,  189,  172,  279,  204,  231,  234,
 /*   480 */   293,  157,  345,  237,  191,  194,   86,  213,  222,  228,
 /*   490 */   225,  188,   42,   44,   46,  210,   55,   59,   64,   97,
 /*   500 */    71,  171,   88,  182,   47,  113,  167,  175,  189,  330,
 /*   510 */    54,  204,  231,  234,  207,  184,  115,  205,  237,  293,
 /*   520 */   170,  213,  222,  228,  225,  383,   42,   44,   46,  106,
 /*   530 */    55,   59,   64,   97,   71,  117,  169,   20,  237,   60,
 /*   540 */   167,  175,  189,  119,  292,  204,  231,  234,  316,  237,
 /*   550 */   411,  237,  415,   74,  132,  213,  222,  228,  225,  262,
 /*   560 */    42,   44,   46,  378,   55,   59,   64,  586,   71,   73,
 /*   570 */   264,  379,  260,  377,  167,  175,  189,  587,   58,  204,
 /*   580 */   231,  234,   72,  166,  142,  336,  588,  149,  143,  213,
 /*   590 */   222,  228,  225,  151,   42,   44,   46,  153,   55,   59,
 /*   600 */    64,  154,   71,  155,  158,  159,  162,  161,  167,  175,
 /*   610 */   189,  163,  280,  204,  231,  234,  176,  177,  181,  196,
 /*   620 */   185,  187,  190,  213,  222,  228,  225,  192,   42,   44,
 /*   630 */    46,  193,   55,   59,   64,  197,   71,  199,  200,  203,
 /*   640 */   202,  206,  167,  175,  189,  208,  288,  204,  231,  234,
 /*   650 */   218,  220,  209,  217,  236,  219,  246,  213,  222,  228,
 /*   660 */   225,  247,   42,   44,   46,  245,   55,   59,   64,  274,
 /*   670 */    71,  267,  249,  285,  251,  253,  167,  175,  189,  275,
 /*   680 */   326,  204,  231,  234,  284,  297,  296,  299,  300,  301,
 /*   690 */   303,  213,  222,  228,  225,  304,   42,   44,   46,  307,
 /*   700 */    55,   59,   64,  308,   71,  483,  484,  305,  485,  486,
 /*   710 */   167,  175,  189,  487,  281,  204,  231,  234,  488,  317,
 /*   720 */   322,  318,  325,  321,  331,  213,  222,  228,  225,  337,
 /*   730 */    42,   44,   46,  340,   55,   59,   64,  323,   71,  336,
 /*   740 */   339,  338,  346,  345,  167,  175,  189,  347,  328,  204,
 /*   750 */   231,  234,  348,  349,  358,  359,  360,  361,  363,  213,
 /*   760 */   222,  228,  225,  365,   42,   44,   46,  366,   55,   59,
 /*   770 */    64,  364,   71,  375,  381,  374,  387,  393,  167,  175,
 /*   780 */   189,  403,  380,  204,  231,  234,  404,  409,  406,  410,
 /*   790 */   413,  416,  386,  213,  222,  228,  225,   89,   42,   44,
 /*   800 */    46,  392,   55,   59,   64,   41,   71,  417,  309,  419,
 /*   810 */   309,  270,  167,  175,  189,   62,  309,  204,  231,  235,
 /*   820 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   830 */    39,   40,  309,   41,   90,   91,  309,  309,   92,   93,
 /*   840 */    94,  309,   95,   53,  309,  309,  309,  235,   29,   30,
 /*   850 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   860 */   309,  309,  309,  309,   41,  309,  309,  240,  255,  259,
 /*   870 */   261,  257,  243,  309,  286,  309,  236,  309,  235,   29,
 /*   880 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   890 */    40,   41,  306,  309,  309,  265,  412,   16,  395,  401,
 /*   900 */   400,   57,  394,  367,  295,  235,   29,   30,   31,   32,
 /*   910 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  309,
 /*   920 */   310,  311,  312,  313,  309,  309,  309,  309,   49,  309,
 /*   930 */   309,  309,  235,   29,   30,   31,   32,   33,   34,   35,
 /*   940 */    36,   37,   38,   39,   40,   41,  309,  309,  309,  309,
 /*   950 */   405,   16,  395,  401,  400,  291,  394,  367,  309,  235,
 /*   960 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   970 */    39,   40,   41,  309,  418,   16,  395,  401,  400,  309,
 /*   980 */   394,  367,  278,  309,  309,  309,  235,   29,   30,   31,
 /*   990 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1000 */   309,  309,  309,  309,   13,   16,  395,  401,  400,   69,
 /*  1010 */   394,  367,  309,  235,   29,   30,   31,   32,   33,   34,
 /*  1020 */    35,   36,   37,   38,   39,   40,   41,  309,  309,   90,
 /*  1030 */    91,  309,  309,   92,   93,   94,  327,   95,  309,  309,
 /*  1040 */   235,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1050 */    38,   39,   40,   41,  414,  309,  309,  369,  382,  309,
 /*  1060 */   388,  362,  309,  333,  309,  309,  309,  235,   29,   30,
 /*  1070 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1080 */    41,  407,  309,  309,  369,  382,  309,  388,  362,  309,
 /*  1090 */   342,  309,  309,  309,  235,   29,   30,   31,   32,   33,
 /*  1100 */    34,   35,   36,   37,   38,   39,   40,   41,   15,  309,
 /*  1110 */   309,  369,  382,  309,  388,  362,  309,  351,  309,  309,
 /*  1120 */   309,  235,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1130 */    37,   38,   39,   40,   41,  309,  309,  309,  309,  309,
 /*  1140 */   118,  116,  141,  309,   26,   80,  309,   41,  235,   29,
 /*  1150 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1160 */    40,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1170 */    37,   38,   39,   40,  241,  125,  423,  238,  309,  266,
 /*  1180 */   309,  309,  120,  121,  306,  309,  122,  123,  124,  309,
 /*  1190 */   147,    7,  263,  309,   10,  309,  295,  309,  408,  234,
 /*  1200 */     7,  309,  309,   10,  137,  309,  309,  408,  234,  309,
 /*  1210 */   324,  309,  310,  311,  312,  313,   90,   91,  309,  309,
 /*  1220 */    92,   93,   94,  309,   95,  244,  306,  240,  255,  259,
 /*  1230 */   261,  257,  243,  309,  309,  126,  236,  309,  295,  309,
 /*  1240 */   309,  139,  138,  309,   48,   92,   93,   94,   89,   95,
 /*  1250 */   309,  309,   23,  309,  310,  311,  312,  313,  240,  255,
 /*  1260 */   259,  261,  257,  243,  173,  420,  309,  236,  369,  382,
 /*  1270 */   439,  388,  362,  369,  382,  277,  388,  362,  240,  255,
 /*  1280 */   259,  261,  257,  243,  306,   90,   91,  236,  309,   92,
 /*  1290 */    93,   94,  309,   95,  309,   61,  295,  309,  309,   24,
 /*  1300 */   309,   90,   91,  309,   89,   92,   93,   94,  309,   95,
 /*  1310 */   309,  309,  310,  311,  312,  313,   89,  309,   96,  240,
 /*  1320 */   255,  259,  261,  257,  243,  309,  309,  309,  236,   89,
 /*  1330 */   186,  309,  309,  309,  309,  309,   68,  309,  309,  309,
 /*  1340 */   135,   90,   91,  144,  309,   92,   93,   94,  309,   95,
 /*  1350 */   309,  309,  309,   90,   91,  309,  309,   92,   93,   94,
 /*  1360 */   309,   95,   89,  309,  309,  309,   90,   91,  309,  309,
 /*  1370 */    92,   93,   94,  309,   95,  178,  108,   90,   91,  309,
 /*  1380 */   309,   92,   93,   94,  137,   95,  309,  438,  309,  309,
 /*  1390 */   369,  382,  309,  388,  362,  309,  309,  309,  309,   90,
 /*  1400 */    91,  309,  309,   92,   93,   94,  309,   95,  309,  309,
 /*  1410 */   309,  309,   90,   91,  309,  309,   92,   93,   94,  309,
 /*  1420 */    95,  139,  138,  309,  309,   92,   93,   94,  442,   95,
 /*  1430 */   309,  356,   19,  309,  344,  362,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    20,    1,   76,   23,   64,   65,   66,   67,   68,   69,
 /*    10 */    70,   31,   32,   33,   34,   17,   36,   37,   38,   19,
 /*    20 */    40,   41,   42,   25,   44,  104,  105,   76,   76,  103,
 /*    30 */    50,   51,   52,  107,   20,   55,   56,   23,   72,   17,
 /*    40 */    74,   75,   25,   77,   78,   31,   32,   33,   34,   97,
 /*    50 */    36,   37,   38,    0,   40,   41,   42,  106,   44,  108,
 /*    60 */   109,  110,   76,   46,   50,   51,   52,   17,   20,   55,
 /*    70 */    56,   23,   73,   74,   75,   25,   77,   78,   20,   31,
 /*    80 */    32,   33,   34,   16,   36,   37,   38,   25,   40,   41,
 /*    90 */    42,   76,   44,   16,  108,  109,  110,   16,   50,   51,
 /*   100 */    52,   17,   20,   55,   56,   23,    1,   45,   46,   28,
 /*   110 */    48,   49,   97,   31,   32,   33,   34,   17,   36,   37,
 /*   120 */    38,    2,   40,   41,   42,   25,   44,    8,    9,   10,
 /*   130 */    53,   54,   50,   51,   52,   30,   20,   55,   56,   23,
 /*   140 */    79,   16,   81,   82,   83,  104,  105,   31,   32,   33,
 /*   150 */    34,   76,   36,   37,   38,   28,   40,   41,   42,   79,
 /*   160 */    44,   81,   82,   83,    1,   78,   50,   51,   52,    1,
 /*   170 */    20,   55,   56,   23,   20,   74,   75,   23,   24,   78,
 /*   180 */    26,   31,   32,   33,   34,  110,   36,   37,   38,   29,
 /*   190 */    40,   41,   42,   20,   44,   17,   23,   24,   30,   26,
 /*   200 */    50,   51,   52,    1,   20,   55,   56,   23,   20,   16,
 /*   210 */    47,   23,   24,   17,   26,   31,   32,   33,   34,   17,
 /*   220 */    36,   37,   38,   45,   40,   41,   42,   29,   44,   67,
 /*   230 */    68,   69,   70,   17,   50,   51,   52,    1,   20,   55,
 /*   240 */    56,   23,   20,   45,   46,   23,   24,   39,   26,   31,
 /*   250 */    32,   33,   34,   19,   36,   37,   38,   76,   40,   41,
 /*   260 */    42,   79,   44,   81,   82,   83,   30,   19,   50,   51,
 /*   270 */    52,    1,   20,   55,   56,   23,   20,   16,   97,   23,
 /*   280 */    24,   16,   26,   31,   32,   33,   34,  100,   36,   37,
 /*   290 */    38,   99,   40,   41,   42,   20,   44,   16,   23,   24,
 /*   300 */    30,   26,   50,   51,   52,    1,   20,   55,   56,   23,
 /*   310 */    81,   82,   83,   24,  100,   26,   27,   31,   32,   33,
 /*   320 */    34,   17,   36,   37,   38,   76,   40,   41,   42,   74,
 /*   330 */    44,    1,   77,   78,   53,   54,   50,   51,   52,    1,
 /*   340 */    20,   55,   56,   23,   16,   24,   97,   26,   27,   76,
 /*   350 */     1,   31,   32,   33,   34,   17,   36,   37,   38,   47,
 /*   360 */    40,   41,   42,   19,   44,   21,   76,    8,    9,   10,
 /*   370 */    50,   51,   52,   43,   20,   55,   56,   23,   76,   30,
 /*   380 */   107,   53,   54,  104,  105,   31,   32,   33,   34,   76,
 /*   390 */    36,   37,   38,  103,   40,   41,   42,  107,   44,   97,
 /*   400 */    76,    1,   76,   16,   50,   51,   52,   29,   20,   55,
 /*   410 */    56,   23,   76,   47,  101,  102,   76,   17,   47,   31,
 /*   420 */    32,   33,   34,   97,   36,   37,   38,  103,   40,   41,
 /*   430 */    42,  107,   44,   97,   76,  104,  105,   97,   50,   51,
 /*   440 */    52,    1,   20,   55,   56,   23,   76,  104,  105,   24,
 /*   450 */    16,   26,  100,   31,   32,   33,   34,   17,   36,   37,
 /*   460 */    38,  103,   40,   41,   42,  107,   44,   97,   76,   24,
 /*   470 */    16,   26,   50,   51,   52,   76,   20,   55,   56,   23,
 /*   480 */     1,   47,   28,   76,   45,   46,    1,   31,   32,   33,
 /*   490 */    34,   17,   36,   37,   38,  103,   40,   41,   42,  107,
 /*   500 */    44,  102,   76,   29,   97,  100,   50,   51,   52,   30,
 /*   510 */    20,   55,   56,   23,   13,   30,  100,   16,   76,    1,
 /*   520 */     1,   31,   32,   33,   34,   16,   36,   37,   38,  103,
 /*   530 */    40,   41,   42,  107,   44,  100,   17,   28,   76,   97,
 /*   540 */    50,   51,   52,  100,   20,   55,   56,   23,   30,   76,
 /*   550 */    19,   76,   21,    1,   60,   31,   32,   33,   34,   97,
 /*   560 */    36,   37,   38,   17,   40,   41,   42,   60,   44,   17,
 /*   570 */    97,   25,   97,   16,   50,   51,   52,   60,   20,   55,
 /*   580 */    56,   23,   98,   99,   16,   28,   60,   16,   29,   31,
 /*   590 */    32,   33,   34,  100,   36,   37,   38,   47,   40,   41,
 /*   600 */    42,   25,   44,  100,   25,  100,   25,   47,   50,   51,
 /*   610 */    52,  100,   20,   55,   56,   23,   76,   17,   16,   47,
 /*   620 */    17,   17,   16,   31,   32,   33,   34,   16,   36,   37,
 /*   630 */    38,   17,   40,   41,   42,   17,   44,   47,   17,   17,
 /*   640 */    47,   17,   50,   51,   52,   16,   20,   55,   56,   23,
 /*   650 */    16,   76,   29,  105,   11,   25,   16,   31,   32,   33,
 /*   660 */    34,   46,   36,   37,   38,  111,   40,   41,   42,   16,
 /*   670 */    44,   29,   47,   19,   47,   47,   50,   51,   52,   43,
 /*   680 */    20,   55,   56,   23,   39,   76,   25,   16,   25,   76,
 /*   690 */    16,   31,   32,   33,   34,   25,   36,   37,   38,   16,
 /*   700 */    40,   41,   42,    5,   44,   16,   16,   76,   16,   16,
 /*   710 */    50,   51,   52,   16,   20,   55,   56,   23,   16,   19,
 /*   720 */    16,   20,   19,   28,   19,   31,   32,   33,   34,   16,
 /*   730 */    36,   37,   38,   19,   40,   41,   42,   29,   44,   28,
 /*   740 */    30,   29,   16,   28,   50,   51,   52,   29,   20,   55,
 /*   750 */    56,   23,   30,   19,   16,   25,   76,   17,   16,   31,
 /*   760 */    32,   33,   34,   76,   36,   37,   38,   17,   40,   41,
 /*   770 */    42,   25,   44,   17,   17,   76,   17,   17,   50,   51,
 /*   780 */    52,   16,   76,   55,   56,   23,   19,   18,   20,   16,
 /*   790 */    20,   16,   76,   31,   32,   33,   34,   16,   36,   37,
 /*   800 */    38,   76,   40,   41,   42,   70,   44,   19,  112,   20,
 /*   810 */   112,   30,   50,   51,   52,   80,  112,   55,   56,   84,
 /*   820 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   830 */    95,   96,  112,   70,   53,   54,  112,  112,   57,   58,
 /*   840 */    59,  112,   61,   80,  112,  112,  112,   84,   85,   86,
 /*   850 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*   860 */   112,  112,  112,  112,   70,  112,  112,    2,    3,    4,
 /*   870 */     5,    6,    7,  112,   80,  112,   11,  112,   84,   85,
 /*   880 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*   890 */    96,   70,    4,  112,  112,   30,   71,   72,   73,   74,
 /*   900 */    75,   80,   77,   78,   16,   84,   85,   86,   87,   88,
 /*   910 */    89,   90,   91,   92,   93,   94,   95,   96,   70,   31,
 /*   920 */    32,   33,   34,   35,  112,  112,  112,  112,   80,  112,
 /*   930 */   112,  112,   84,   85,   86,   87,   88,   89,   90,   91,
 /*   940 */    92,   93,   94,   95,   96,   70,  112,  112,  112,  112,
 /*   950 */    71,   72,   73,   74,   75,   80,   77,   78,  112,   84,
 /*   960 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   970 */    95,   96,   70,  112,   71,   72,   73,   74,   75,  112,
 /*   980 */    77,   78,   80,  112,  112,  112,   84,   85,   86,   87,
 /*   990 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   70,
 /*  1000 */   112,  112,  112,  112,   71,   72,   73,   74,   75,   80,
 /*  1010 */    77,   78,  112,   84,   85,   86,   87,   88,   89,   90,
 /*  1020 */    91,   92,   93,   94,   95,   96,   70,  112,  112,   53,
 /*  1030 */    54,  112,  112,   57,   58,   59,   80,   61,  112,  112,
 /*  1040 */    84,   85,   86,   87,   88,   89,   90,   91,   92,   93,
 /*  1050 */    94,   95,   96,   70,   20,  112,  112,   23,   24,  112,
 /*  1060 */    26,   27,  112,   80,  112,  112,  112,   84,   85,   86,
 /*  1070 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*  1080 */    70,   20,  112,  112,   23,   24,  112,   26,   27,  112,
 /*  1090 */    80,  112,  112,  112,   84,   85,   86,   87,   88,   89,
 /*  1100 */    90,   91,   92,   93,   94,   95,   96,   70,   20,  112,
 /*  1110 */   112,   23,   24,  112,   26,   27,  112,   80,  112,  112,
 /*  1120 */   112,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*  1130 */    93,   94,   95,   96,   70,  112,  112,  112,  112,  112,
 /*  1140 */    11,   12,   13,  112,   80,   16,  112,   70,   84,   85,
 /*  1150 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*  1160 */    96,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*  1170 */    93,   94,   95,   96,   11,   46,    0,   14,  112,   16,
 /*  1180 */   112,  112,   53,   54,    4,  112,   57,   58,   59,  112,
 /*  1190 */    61,   15,   29,  112,   18,  112,   16,  112,   22,   23,
 /*  1200 */    15,  112,  112,   18,   16,  112,  112,   22,   23,  112,
 /*  1210 */    30,   31,   32,   33,   34,   35,   53,   54,  112,  112,
 /*  1220 */    57,   58,   59,  112,   61,   62,    4,    2,    3,    4,
 /*  1230 */     5,    6,    7,  112,  112,   47,   11,  112,   16,  112,
 /*  1240 */   112,   53,   54,  112,   19,   57,   58,   59,   16,   61,
 /*  1250 */   112,  112,   30,   31,   32,   33,   34,   35,    2,    3,
 /*  1260 */     4,    5,    6,    7,   16,   20,  112,   11,   23,   24,
 /*  1270 */    20,   26,   27,   23,   24,   19,   26,   27,    2,    3,
 /*  1280 */     4,    5,    6,    7,    4,   53,   54,   11,  112,   57,
 /*  1290 */    58,   59,  112,   61,  112,   19,   16,  112,  112,   19,
 /*  1300 */   112,   53,   54,  112,   16,   57,   58,   59,  112,   61,
 /*  1310 */   112,   31,   32,   33,   34,   35,   16,  112,   30,    2,
 /*  1320 */     3,    4,    5,    6,    7,  112,  112,  112,   11,   16,
 /*  1330 */    30,  112,  112,  112,  112,  112,   19,  112,  112,  112,
 /*  1340 */    16,   53,   54,   30,  112,   57,   58,   59,  112,   61,
 /*  1350 */   112,  112,  112,   53,   54,  112,  112,   57,   58,   59,
 /*  1360 */   112,   61,   16,  112,  112,  112,   53,   54,  112,  112,
 /*  1370 */    57,   58,   59,  112,   61,   16,   30,   53,   54,  112,
 /*  1380 */   112,   57,   58,   59,   16,   61,  112,   20,  112,  112,
 /*  1390 */    23,   24,  112,   26,   27,  112,  112,  112,  112,   53,
 /*  1400 */    54,  112,  112,   57,   58,   59,  112,   61,  112,  112,
 /*  1410 */   112,  112,   53,   54,  112,  112,   57,   58,   59,  112,
 /*  1420 */    61,   53,   54,  112,  112,   57,   58,   59,   20,   61,
 /*  1430 */   112,   23,   24,  112,   26,   27,
};
#define YY_SHIFT_USE_DFLT (-21)
static short yy_shift_ofst[] = {
 /*     0 */  1185,   53, 1176,  -21,  -21,  -21,  -21,   67,   84,  -21,
 /*    10 */   125,  344, 1088,   58,  -21,  -21, 1367,  173,  -21,  127,
 /*    20 */   193,  160, 1222, 1280,  422,  -21,  354,  -21,  -21,  -21,
 /*    30 */   -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,
 /*    40 */   -21,  -21,  196,  -21,  216,  -21, 1163, 1225,  388,   48,
 /*    50 */   208,  234,  116,  490,  -21,  248,  694,  558,  -21, 1163,
 /*    60 */  1276,  592,  252,  -21,  261,  330, 1163, 1317,  762,  320,
 /*    70 */   -21,  265,  552,  -21,  265,  -21,   62, 1129,  119, 1129,
 /*    80 */   198,  387,  378, 1288,  168,  -21, 1232,  -21,  -21,  -21,
 /*    90 */   -21,  -21,  -21,  -21,  -21,  -21,  -21,  -21,  281,  312,
 /*   100 */   -21,  366,  -21,  371,  -21, 1346,  236,  -21,  -21,  -21,
 /*   110 */  1129,  -21, 1129,  -21, 1129,  359, 1129,  -21, 1129,  -21,
 /*   120 */   -21,  -21,  -21,  -21,  -21, 1188,  -21,  163,  -21, 1368,
 /*   130 */   -21,  494, 1324,  -21,  -21,  -21,  -21,  507,  517,  526,
 /*   140 */   -21,  568,  559, 1313,  -21,  270,  -21,  -21,  571,   17,
 /*   150 */  1129,  119,  550,  576, 1129,  119,  434,  579, 1129,  119,
 /*   160 */   560,  581, 1129,  119,  -21,  -21,  -21, 1248,  519,  -21,
 /*   170 */  1248,  -21,  -21,  -21,  -21, 1359,  600,  -21,  178,  -21,
 /*   180 */   602,  474, 1300,  485,  603,  -21,  604,  -21,  -21,  606,
 /*   190 */   439,  611,  614,  -21,  328,  572,  618,  -21,  590,  621,
 /*   200 */   -21,  593,  622,  -21,  501,  624,  -21,  629,  623, 1232,
 /*   210 */   349,   22,  -21,  634,  440,  -21,  634,  -21,  630,  976,
 /*   220 */   -21,  -21,  634,  400,  -21,  634,  338,  -21,  634,  304,
 /*   230 */   -21,  634,  202,  -21,  -21,  -21, 1163,  -21, 1163,  -21,
 /*   240 */  1163, 1163,  643, 1163,  640,  -21,  615,   77,  625,  -21,
 /*   250 */   627,  -21,  628,  -21,  643, 1163,  643, 1163,  643, 1163,
 /*   260 */   643, 1163,  643, 1163,  865,  -21,  642,  781,  105,  -21,
 /*   270 */   -21,  643,  643,  653,  636, 1163, 1256,  762,  456,  -21,
 /*   280 */   -21,  -21,  -21,  645,  654,  626,  184,  -21,  -21,    0,
 /*   290 */   762,  524,  -21,  888,  -21,  661,  976,  -21,  671,  663,
 /*   300 */   976,  -21,  674,  670,  976,  -21,  683,  698,  689,  690,
 /*   310 */   692,  693,  697,  702,  -21,  518,  700,  701,  -21,  445,
 /*   320 */   695,  704,  708, 1180,  703,  660,  -21,  728,  -21,  479,
 /*   330 */   705,  286,  -21,  -20,  -21,  711,  713,  712,  710,  714,
 /*   340 */    14,  -21,   82,  -21,  715,  726,  718,  722,  734,  150,
 /*   350 */   -21,  218,  -21, 1408,  275,  -21,  289,  738,  730,  976,
 /*   360 */   740,  -21,  742,  746,  976,  750,  -21,  -21,  -21,  321,
 /*   370 */    81,   -2,  -21,  976,  756,  -21,  557,  546,  -21,  976,
 /*   380 */   757,  -21,  509,   50,  -21,  976,  759,  -21,  454,  100,
 /*   390 */   -21,  976,  760,  -21,  -21, 1250,  154,  188,  425,  222,
 /*   400 */   -21,  256,  765,  767, 1061,  768,  -21,  -21,  769,  773,
 /*   410 */   531, 1034,  770,  -21,  -21,  775,  788, 1245,  789,  -21,
 /*   420 */   -21,  -21,
};
#define YY_REDUCE_USE_DFLT (-80)
static short yy_reduce_ofst[] = {
 /*     0 */   -60,  -80,  162,  -80,  -80,  -80,  -80,  -80,  -80,  -80,
 /*    10 */   -80,  -80,  933,  -80,  -80,  -80,   -1,   87,  -80,  -80,
 /*    20 */   -80,  -80,   80,   61, 1064,  -80, 1077,  -80,  -80,  -80,
 /*    30 */   -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,
 /*    40 */   -80,  -80,  -80,  -80,  -80,  -80,  407,  -80,  848, 1077,
 /*    50 */   -80,  -80,  763, 1077,  -80,  -80,  821, 1077,  -80,  442,
 /*    60 */   -80,  735, 1077,  -80,  -80,  -80,  249,  -80,  929, 1077,
 /*    70 */   -80,  484,  -80,  -80,  192,  -80,  -80,  187,  -80,  214,
 /*    80 */   -80,  -80,  -80,  324,  -80,  -80,  273,  -80,  -80,  -80,
 /*    90 */   -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,
 /*   100 */   -80,  -80,  -80,  -80,  -80,  426,  -80,  -80,  -80,  -80,
 /*   110 */   352,  -80,  405,  -80,  416,  -80,  435,  -80,  443,  -80,
 /*   120 */   -80,  -80,  -80,  -80,  -80,  -49,  -80,  -80,  -80,  -14,
 /*   130 */   -80,  -80,   75,  -80,  -80,  -80,  -80,  -80,  -80,  -80,
 /*   140 */   -80,  -80,  -80,  358,  -80,  -80,  -80,  -80,  -80,  -80,
 /*   150 */   493,  -80,  -80,  -80,  503,  -80,  -80,  -80,  505,  -80,
 /*   160 */   -80,  -80,  511,  -80,  -80,  -80,  -80,  313,  -80,  -80,
 /*   170 */   399,  -80,  -80,  -80,  -80,  540,  -80,  -80,  -80,  -80,
 /*   180 */   -80,  -80,  -74,  -80,  -80,  -80,  -80,  -80,  -80,  -80,
 /*   190 */   -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,
 /*   200 */   -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  392,
 /*   210 */   -80,  -80,  -80,  343,  -80,  -80,  548,  -80,  -80,  575,
 /*   220 */   -80,  -80,  331,  -80,  -80,  279,  -80,  -80,   41,  -80,
 /*   230 */   -80,  -79,  -80,  -80,  -80,  -80,  370,  -80,  336,  -80,
 /*   240 */   326,  302,  -80,  181,  554,  -80,  -80,  -80,  -80,  -80,
 /*   250 */   -80,  -80,  -80,  -80,  -80,   15,  -80,  -48,  -80,  475,
 /*   260 */   -80,  462,  -80,  473,  -80,  -80,  -80,  290,  -80,  -80,
 /*   270 */   -80,  -80,  -80,  -80,  -80,  340,  -80,  902, 1077,  -80,
 /*   280 */   -80,  -80,  -80,  -80,  -80,  794, 1077,  -80,  -80,  -80,
 /*   290 */   875, 1077,  -80,  229,  -80,  -80,  609,  -80,  -80,  -80,
 /*   300 */   613,  -80,  -80,  -80,  631,  -80,  -80,  -80,  -80,  -80,
 /*   310 */   -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,  -80,
 /*   320 */   -80,  -80,  -80,  182,  -80,  956,  -80, 1077,  -80,  -80,
 /*   330 */   -80,  983,  -80, 1077,  -80,  -80,  -80,  -80,  -80,  -80,
 /*   340 */  1010,  -80, 1077,  -80,  -80,  -80,  -80,  -80,  -80, 1037,
 /*   350 */   -80, 1077,  -80,  255,   87,  -80,  -80,  -80,  -80,  680,
 /*   360 */   -80,  -80,  -80,  -80,  687,  -80,  -80,  -80,  -80,  -80,
 /*   370 */   -80,  -80,  -80,  699,  -80,  -80,  -80,  -80,  -80,  706,
 /*   380 */   -80,  -80,  -80,  -80,  -80,  716,  -80,  -80,  -80,  -80,
 /*   390 */   -80,  725,  -80,  -80,  -80,  -34,  101,   87,  -80,   87,
 /*   400 */   -80,   87,  -80,  -80,  879,  -80,  -80,  -80,  -80,  -80,
 /*   410 */   -80,  825,  -80,  -80,  -80,  -80,  -80,  903,  -80,  -80,
 /*   420 */   -80,  -80,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   617,  617,  617,  424,  426,  427,  428,  617,  617,  429,
 /*    10 */   617,  617,  617,  617,  430,  434,  617,  617,  459,  617,
 /*    20 */   617,  617,  617,  617,  617,  463,  617,  467,  489,  491,
 /*    30 */   492,  493,  494,  495,  496,  497,  498,  499,  500,  501,
 /*    40 */   502,  503,  617,  504,  617,  505,  617,  617,  617,  617,
 /*    50 */   508,  617,  617,  617,  509,  617,  617,  617,  513,  617,
 /*    60 */   617,  617,  617,  515,  617,  617,  617,  617,  617,  617,
 /*    70 */   516,  617,  617,  518,  617,  519,  617,  617,  521,  617,
 /*    80 */   563,  617,  553,  617,  617,  576,  617,  578,  580,  581,
 /*    90 */   592,  593,  594,  595,  596,  597,  577,  579,  617,  617,
 /*   100 */   554,  617,  555,  617,  556,  617,  617,  574,  575,  557,
 /*   110 */   617,  558,  617,  559,  617,  560,  617,  561,  617,  562,
 /*   120 */   564,  565,  566,  567,  568,  617,  569,  617,  570,  617,
 /*   130 */   582,  617,  617,  584,  589,  590,  585,  590,  593,  592,
 /*   140 */   583,  617,  571,  617,  572,  617,  573,  591,  617,  617,
 /*   150 */   617,  522,  617,  617,  617,  523,  617,  617,  617,  524,
 /*   160 */   617,  617,  617,  525,  526,  527,  520,  617,  617,  528,
 /*   170 */   617,  529,  531,  532,  530,  617,  617,  533,  617,  534,
 /*   180 */   617,  617,  617,  617,  617,  535,  617,  536,  537,  617,
 /*   190 */   617,  617,  617,  538,  617,  617,  617,  539,  617,  617,
 /*   200 */   540,  617,  617,  541,  617,  617,  542,  617,  617,  617,
 /*   210 */   617,  617,  543,  617,  617,  544,  617,  549,  551,  617,
 /*   220 */   552,  550,  617,  617,  545,  617,  617,  546,  617,  617,
 /*   230 */   547,  617,  617,  548,  616,  490,  617,  598,  617,  599,
 /*   240 */   617,  617,  600,  617,  617,  601,  617,  617,  617,  613,
 /*   250 */   617,  614,  617,  615,  604,  617,  605,  617,  606,  617,
 /*   260 */   607,  617,  608,  617,  617,  609,  610,  617,  617,  611,
 /*   270 */   612,  603,  602,  617,  617,  617,  617,  617,  617,  517,
 /*   280 */   514,  512,  510,  506,  617,  617,  617,  511,  507,  617,
 /*   290 */   617,  617,  468,  617,  475,  477,  617,  480,  617,  478,
 /*   300 */   617,  481,  617,  479,  617,  482,  617,  617,  617,  617,
 /*   310 */   617,  617,  617,  617,  476,  617,  617,  617,  464,  617,
 /*   320 */   617,  617,  617,  617,  617,  617,  465,  617,  469,  617,
 /*   330 */   617,  617,  466,  617,  470,  617,  617,  617,  617,  617,
 /*   340 */   617,  472,  617,  474,  617,  617,  617,  617,  617,  617,
 /*   350 */   471,  617,  473,  617,  617,  457,  617,  617,  617,  617,
 /*   360 */   617,  460,  617,  617,  617,  617,  461,  462,  447,  617,
 /*   370 */   617,  617,  449,  617,  617,  452,  617,  617,  454,  617,
 /*   380 */   617,  456,  617,  617,  450,  617,  617,  451,  617,  617,
 /*   390 */   453,  617,  617,  455,  458,  617,  617,  617,  617,  617,
 /*   400 */   448,  617,  617,  617,  617,  617,  431,  435,  617,  617,
 /*   410 */   617,  617,  617,  432,  436,  617,  617,  617,  617,  433,
 /*   420 */   437,  425,
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
  "ADD",           "SUB",           "CONCAT",        "TYPEOF",      
  "INSTANCEOF",    "NEW",           "NOT",           "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",
  "BRACKET_CLOSE",  "EXTENDS",       "ABSTRACT",      "COMMENT",     
  "PUBLIC",        "ASSIGN",        "PROTECTED",     "CONST",       
  "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",  "TYPE_INTEGER",
  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",  
  "BREAK",         "CONTINUE",      "IF",            "ELSE",        
  "LOOP",          "WHILE",         "FOR",           "IN",          
  "LET",           "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",
  "INCR",          "DECR",          "ECHO",          "RETURN",      
  "UNSET",         "INTEGER",       "STRING",        "THROW",       
  "TYPE_VAR",      "DOUBLE",        "NULL",          "FALSE",       
  "COLON",         "TRUE",          "ISSET",         "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_comment",    "xx_class_definition",
  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",
  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",
  "xx_statement",  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",
  "xx_echo_statement",  "xx_return_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_for_statement",  "xx_eval_expr",  "xx_let_assignments",  "xx_let_assignment",
  "xx_assign_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_call_parameters",
  "xx_declare_variable_list",  "xx_declare_variable",  "xx_array_list",  "xx_call_parameter",
  "xx_array_item",  "xx_array_key",  "xx_array_value",  "xx_isset_expr",
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
 /*  94 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  95 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
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
 /* 109 */ "xx_echo_expression ::= xx_literal_expr",
 /* 110 */ "xx_echo_expression ::= IDENTIFIER",
 /* 111 */ "xx_return_statement ::= RETURN xx_literal_expr DOTCOMMA",
 /* 112 */ "xx_return_statement ::= RETURN IDENTIFIER DOTCOMMA",
 /* 113 */ "xx_return_statement ::= RETURN IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 114 */ "xx_return_statement ::= RETURN IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /* 115 */ "xx_return_statement ::= RETURN IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 116 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 117 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 118 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 119 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 120 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 121 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 122 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 123 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 124 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 125 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 126 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 127 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 128 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 129 */ "xx_declare_variable ::= IDENTIFIER",
 /* 130 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 131 */ "xx_assign_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 132 */ "xx_assign_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 133 */ "xx_assign_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 134 */ "xx_assign_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 135 */ "xx_assign_expr ::= xx_assign_expr ADD xx_assign_expr",
 /* 136 */ "xx_assign_expr ::= xx_assign_expr SUB xx_assign_expr",
 /* 137 */ "xx_assign_expr ::= xx_assign_expr CONCAT xx_assign_expr",
 /* 138 */ "xx_assign_expr ::= xx_assign_expr EQUALS xx_assign_expr",
 /* 139 */ "xx_assign_expr ::= INSTANCEOF xx_assign_expr",
 /* 140 */ "xx_assign_expr ::= TYPEOF xx_assign_expr",
 /* 141 */ "xx_assign_expr ::= IDENTIFIER",
 /* 142 */ "xx_assign_expr ::= INTEGER",
 /* 143 */ "xx_assign_expr ::= STRING",
 /* 144 */ "xx_assign_expr ::= DOUBLE",
 /* 145 */ "xx_assign_expr ::= NULL",
 /* 146 */ "xx_assign_expr ::= FALSE",
 /* 147 */ "xx_assign_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 148 */ "xx_assign_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 149 */ "xx_assign_expr ::= NEW IDENTIFIER",
 /* 150 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 151 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 152 */ "xx_assign_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 153 */ "xx_assign_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 154 */ "xx_assign_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 155 */ "xx_assign_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 156 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 157 */ "xx_call_parameters ::= xx_call_parameter",
 /* 158 */ "xx_call_parameter ::= xx_literal_expr",
 /* 159 */ "xx_call_parameter ::= IDENTIFIER",
 /* 160 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 161 */ "xx_array_list ::= xx_array_item",
 /* 162 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 163 */ "xx_array_item ::= xx_array_value",
 /* 164 */ "xx_array_key ::= IDENTIFIER",
 /* 165 */ "xx_array_key ::= STRING",
 /* 166 */ "xx_array_key ::= INTEGER",
 /* 167 */ "xx_array_value ::= xx_literal_expr",
 /* 168 */ "xx_array_value ::= IDENTIFIER",
 /* 169 */ "xx_assign_expr ::= TRUE",
 /* 170 */ "xx_literal_expr ::= INTEGER",
 /* 171 */ "xx_literal_expr ::= STRING",
 /* 172 */ "xx_literal_expr ::= DOUBLE",
 /* 173 */ "xx_literal_expr ::= NULL",
 /* 174 */ "xx_literal_expr ::= FALSE",
 /* 175 */ "xx_literal_expr ::= TRUE",
 /* 176 */ "xx_eval_expr ::= xx_literal_expr",
 /* 177 */ "xx_eval_expr ::= NOT xx_eval_expr",
 /* 178 */ "xx_eval_expr ::= TYPEOF xx_eval_expr",
 /* 179 */ "xx_eval_expr ::= ISSET xx_isset_expr",
 /* 180 */ "xx_eval_expr ::= xx_eval_expr TYPEOF xx_eval_expr",
 /* 181 */ "xx_eval_expr ::= xx_eval_expr EQUALS xx_eval_expr",
 /* 182 */ "xx_eval_expr ::= xx_eval_expr NOTEQUALS xx_eval_expr",
 /* 183 */ "xx_eval_expr ::= xx_eval_expr IDENTICAL xx_eval_expr",
 /* 184 */ "xx_eval_expr ::= xx_eval_expr NOTIDENTICAL xx_eval_expr",
 /* 185 */ "xx_eval_expr ::= xx_eval_expr LESS xx_eval_expr",
 /* 186 */ "xx_eval_expr ::= xx_eval_expr GREATER xx_eval_expr",
 /* 187 */ "xx_eval_expr ::= PARENTHESES_OPEN xx_eval_expr PARENTHESES_CLOSE",
 /* 188 */ "xx_eval_expr ::= IDENTIFIER",
 /* 189 */ "xx_eval_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 190 */ "xx_eval_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 191 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 192 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 193 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 194 */ "xx_comment ::= COMMENT",
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
// 705 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1629 "parser.c"
      break;
    case 65:
// 718 "parser.lemon"
{ json_object_put((yypminor->yy63)); }
// 1634 "parser.c"
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
  { 98, 3 },
  { 98, 1 },
  { 99, 3 },
  { 99, 5 },
  { 99, 7 },
  { 99, 5 },
  { 99, 6 },
  { 99, 2 },
  { 99, 2 },
  { 88, 3 },
  { 101, 3 },
  { 101, 1 },
  { 102, 1 },
  { 102, 1 },
  { 89, 3 },
  { 89, 3 },
  { 89, 8 },
  { 89, 7 },
  { 89, 5 },
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
  { 104, 3 },
  { 104, 1 },
  { 105, 1 },
  { 105, 3 },
  { 100, 3 },
  { 100, 4 },
  { 100, 4 },
  { 100, 4 },
  { 100, 3 },
  { 100, 3 },
  { 100, 3 },
  { 100, 3 },
  { 100, 2 },
  { 100, 2 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 2 },
  { 100, 3 },
  { 100, 2 },
  { 100, 4 },
  { 100, 5 },
  { 100, 4 },
  { 100, 3 },
  { 100, 6 },
  { 100, 5 },
  { 103, 3 },
  { 103, 1 },
  { 107, 1 },
  { 107, 1 },
  { 106, 3 },
  { 106, 1 },
  { 108, 3 },
  { 108, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 110, 1 },
  { 110, 1 },
  { 100, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 76, 1 },
  { 97, 1 },
  { 97, 2 },
  { 97, 2 },
  { 97, 2 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 97, 3 },
  { 97, 1 },
  { 97, 4 },
  { 97, 3 },
  { 111, 4 },
  { 111, 4 },
  { 111, 4 },
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
// 714 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy63;
}
// 2046 "parser.c"
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
      case 158:
      case 167:
      case 176:
// 720 "parser.lemon"
{
	yygotominor.yy63 = yymsp[0].minor.yy63;
}
// 2072 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 724 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_list(yymsp[-1].minor.yy63, yymsp[0].minor.yy63);
}
// 2083 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 68:
      case 98:
      case 108:
      case 128:
      case 157:
      case 161:
// 728 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_list(NULL, yymsp[0].minor.yy63);
}
// 2100 "parser.c"
        break;
      case 7:
// 744 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2109 "parser.c"
        break;
      case 8:
// 748 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, 0, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2119 "parser.c"
        break;
      case 9:
// 752 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy63, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2130 "parser.c"
        break;
      case 10:
// 756 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, 1, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-5].minor);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2141 "parser.c"
        break;
      case 11:
// 760 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy63, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2153 "parser.c"
        break;
      case 12:
// 764 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2163 "parser.c"
        break;
      case 13:
// 768 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2174 "parser.c"
        break;
      case 14:
// 772 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2185 "parser.c"
        break;
      case 15:
// 776 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-6].minor);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2197 "parser.c"
        break;
      case 16:
// 780 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(yymsp[0].minor.yy63, NULL, NULL, status->scanner_state);
}
// 2204 "parser.c"
        break;
      case 17:
// 784 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy63, status->scanner_state);
}
// 2211 "parser.c"
        break;
      case 18:
// 788 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(NULL, yymsp[0].minor.yy63, NULL, status->scanner_state);
}
// 2218 "parser.c"
        break;
      case 19:
// 792 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(yymsp[-1].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
}
// 2225 "parser.c"
        break;
      case 20:
// 796 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(yymsp[-1].minor.yy63, NULL, yymsp[0].minor.yy63, status->scanner_state);
}
// 2232 "parser.c"
        break;
      case 21:
// 800 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(yymsp[0].minor.yy63, NULL, yymsp[-1].minor.yy63, status->scanner_state);
}
// 2239 "parser.c"
        break;
      case 22:
// 804 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(NULL, yymsp[0].minor.yy63, yymsp[-1].minor.yy63, status->scanner_state);
}
// 2246 "parser.c"
        break;
      case 23:
// 808 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(yymsp[-2].minor.yy63, yymsp[0].minor.yy63, yymsp[-1].minor.yy63, status->scanner_state);
}
// 2253 "parser.c"
        break;
      case 24:
// 812 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_definition(yymsp[-1].minor.yy63, yymsp[0].minor.yy63, yymsp[-2].minor.yy63, status->scanner_state);
}
// 2260 "parser.c"
        break;
      case 27:
// 824 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2269 "parser.c"
        break;
      case 28:
// 828 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2278 "parser.c"
        break;
      case 29:
// 832 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2288 "parser.c"
        break;
      case 30:
// 836 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2298 "parser.c"
        break;
      case 31:
// 840 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2307 "parser.c"
        break;
      case 32:
// 844 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2316 "parser.c"
        break;
      case 33:
// 848 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2326 "parser.c"
        break;
      case 34:
// 852 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2336 "parser.c"
        break;
      case 38:
// 868 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2346 "parser.c"
        break;
      case 39:
// 872 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2356 "parser.c"
        break;
      case 41:
// 880 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2369 "parser.c"
        break;
      case 42:
// 884 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy63, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2382 "parser.c"
        break;
      case 43:
// 888 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2395 "parser.c"
        break;
      case 44:
// 892 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy63, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2408 "parser.c"
        break;
      case 45:
// 896 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy63, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2421 "parser.c"
        break;
      case 46:
// 900 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy63, yymsp[-1].minor.yy63, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-8].minor);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2434 "parser.c"
        break;
      case 47:
// 904 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy63, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2447 "parser.c"
        break;
      case 48:
// 908 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy63, yymsp[-1].minor.yy63, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-8].minor);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2460 "parser.c"
        break;
      case 49:
// 912 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2473 "parser.c"
        break;
      case 50:
// 916 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2486 "parser.c"
        break;
      case 51:
// 920 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy63, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2499 "parser.c"
        break;
      case 52:
// 924 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy63, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2512 "parser.c"
        break;
      case 53:
      case 97:
      case 107:
      case 127:
      case 156:
      case 160:
// 928 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_list(yymsp[-2].minor.yy63, yymsp[0].minor.yy63);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2525 "parser.c"
        break;
      case 55:
// 936 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2532 "parser.c"
        break;
      case 56:
// 940 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_parameter(yymsp[-1].minor.yy63, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2539 "parser.c"
        break;
      case 57:
// 944 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_parameter(NULL, yymsp[-1].minor.yy63, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2546 "parser.c"
        break;
      case 58:
// 948 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2554 "parser.c"
        break;
      case 59:
// 952 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_parameter(yymsp[-3].minor.yy63, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2562 "parser.c"
        break;
      case 60:
// 956 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_parameter(NULL, yymsp[-3].minor.yy63, yymsp[-2].minor.yy0, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2570 "parser.c"
        break;
      case 61:
// 960 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2579 "parser.c"
        break;
      case 62:
// 964 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(31,&yymsp[0].minor);
}
// 2587 "parser.c"
        break;
      case 63:
// 968 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(32,&yymsp[0].minor);
}
// 2595 "parser.c"
        break;
      case 64:
// 972 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(33,&yymsp[0].minor);
}
// 2603 "parser.c"
        break;
      case 65:
// 976 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(34,&yymsp[0].minor);
}
// 2611 "parser.c"
        break;
      case 66:
// 980 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(35,&yymsp[0].minor);
}
// 2619 "parser.c"
        break;
      case 82:
// 1044 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2628 "parser.c"
        break;
      case 83:
// 1048 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2637 "parser.c"
        break;
      case 84:
// 1052 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_if_statement(yymsp[-2].minor.yy63, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2647 "parser.c"
        break;
      case 85:
// 1056 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_if_statement(yymsp[-5].minor.yy63, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2660 "parser.c"
        break;
      case 86:
// 1060 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_if_statement(yymsp[-3].minor.yy63, yymsp[-1].minor.yy63, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2670 "parser.c"
        break;
      case 87:
// 1064 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_if_statement(yymsp[-7].minor.yy63, yymsp[-5].minor.yy63, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(38,&yymsp[-8].minor);
  yy_destructor(19,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2683 "parser.c"
        break;
      case 88:
// 1068 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_if_statement(yymsp[-6].minor.yy63, yymsp[-4].minor.yy63, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-7].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2696 "parser.c"
        break;
      case 89:
// 1072 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_if_statement(yymsp[-6].minor.yy63, NULL, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(38,&yymsp[-7].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2709 "parser.c"
        break;
      case 90:
// 1076 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2719 "parser.c"
        break;
      case 91:
// 1080 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_loop_statement(yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2729 "parser.c"
        break;
      case 92:
// 1084 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_while_statement(yymsp[-2].minor.yy63, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2739 "parser.c"
        break;
      case 93:
// 1088 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_while_statement(yymsp[-3].minor.yy63, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2749 "parser.c"
        break;
      case 94:
// 1092 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_for_statement(yymsp[-3].minor.yy63, NULL, yymsp[-5].minor.yy0, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2760 "parser.c"
        break;
      case 95:
// 1096 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_for_statement(yymsp[-3].minor.yy63, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(42,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2772 "parser.c"
        break;
      case 96:
// 1100 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_let_statement(yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2781 "parser.c"
        break;
      case 99:
// 1113 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2789 "parser.c"
        break;
      case 100:
// 1118 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2798 "parser.c"
        break;
      case 101:
// 1123 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_let_assignment("object-property-append", yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2809 "parser.c"
        break;
      case 102:
// 1128 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2819 "parser.c"
        break;
      case 103:
// 1133 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2829 "parser.c"
        break;
      case 104:
// 1138 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[0].minor);
}
// 2837 "parser.c"
        break;
      case 105:
// 1143 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[0].minor);
}
// 2845 "parser.c"
        break;
      case 106:
// 1147 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_echo_statement(yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2854 "parser.c"
        break;
      case 109:
// 1159 "parser.lemon"
{
	yygotominor.yy63 = yymsp[0].minor.yy63;;
}
// 2861 "parser.c"
        break;
      case 110:
      case 141:
      case 159:
      case 164:
      case 168:
      case 188:
// 1163 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 2873 "parser.c"
        break;
      case 111:
// 1168 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_return_statement(NULL, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2882 "parser.c"
        break;
      case 112:
// 1173 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_return_statement(NULL, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2891 "parser.c"
        break;
      case 113:
// 1178 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_mcall(yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy63, status->scanner_state);
  yy_destructor(51,&yymsp[-7].minor);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2903 "parser.c"
        break;
      case 114:
// 1183 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[-6].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2915 "parser.c"
        break;
      case 115:
// 1188 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_return_statement(yymsp[-3].minor.yy0, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), status->scanner_state);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2925 "parser.c"
        break;
      case 116:
// 1193 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2939 "parser.c"
        break;
      case 117:
// 1202 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2954 "parser.c"
        break;
      case 118:
// 1211 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2969 "parser.c"
        break;
      case 119:
// 1220 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2984 "parser.c"
        break;
      case 120:
// 1229 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2993 "parser.c"
        break;
      case 121:
// 1234 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy63, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 3005 "parser.c"
        break;
      case 122:
// 1238 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 3014 "parser.c"
        break;
      case 123:
// 1242 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 3023 "parser.c"
        break;
      case 124:
// 1246 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 3032 "parser.c"
        break;
      case 125:
// 1250 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 3041 "parser.c"
        break;
      case 126:
// 1254 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 3050 "parser.c"
        break;
      case 129:
// 1266 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3057 "parser.c"
        break;
      case 130:
// 1270 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 3065 "parser.c"
        break;
      case 131:
// 1275 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3073 "parser.c"
        break;
      case 132:
      case 191:
// 1280 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3083 "parser.c"
        break;
      case 133:
      case 192:
// 1285 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3093 "parser.c"
        break;
      case 134:
      case 193:
// 1290 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3103 "parser.c"
        break;
      case 135:
// 1295 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("add", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3111 "parser.c"
        break;
      case 136:
// 1300 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("sub", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3119 "parser.c"
        break;
      case 137:
// 1305 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("concat", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3127 "parser.c"
        break;
      case 138:
      case 181:
// 1310 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("equals", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3136 "parser.c"
        break;
      case 139:
// 1315 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("instanceof", yymsp[0].minor.yy63, NULL, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3144 "parser.c"
        break;
      case 140:
      case 178:
// 1320 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("typeof", yymsp[0].minor.yy63, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3153 "parser.c"
        break;
      case 142:
      case 166:
      case 170:
// 1330 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3162 "parser.c"
        break;
      case 143:
      case 165:
      case 171:
// 1335 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3171 "parser.c"
        break;
      case 144:
      case 172:
// 1340 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3179 "parser.c"
        break;
      case 145:
      case 173:
// 1345 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[0].minor);
}
// 3188 "parser.c"
        break;
      case 146:
      case 174:
// 1350 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[0].minor);
}
// 3197 "parser.c"
        break;
      case 147:
// 1355 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3206 "parser.c"
        break;
      case 148:
// 1360 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("array", yymsp[-1].minor.yy63, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3215 "parser.c"
        break;
      case 149:
// 1365 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3223 "parser.c"
        break;
      case 150:
// 1370 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3233 "parser.c"
        break;
      case 151:
// 1375 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(13,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3243 "parser.c"
        break;
      case 152:
      case 189:
// 1380 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3253 "parser.c"
        break;
      case 153:
      case 190:
// 1385 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3263 "parser.c"
        break;
      case 154:
// 1390 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy63, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3273 "parser.c"
        break;
      case 155:
// 1395 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3283 "parser.c"
        break;
      case 162:
// 1423 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_array_item(yymsp[-2].minor.yy63, yymsp[0].minor.yy63, status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
}
// 3291 "parser.c"
        break;
      case 163:
// 1427 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_array_item(NULL, yymsp[0].minor.yy63, status->scanner_state);
}
// 3298 "parser.c"
        break;
      case 169:
      case 175:
// 1451 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[0].minor);
}
// 3307 "parser.c"
        break;
      case 177:
// 1483 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("not", yymsp[0].minor.yy63, NULL, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3315 "parser.c"
        break;
      case 179:
// 1491 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("isset", yymsp[0].minor.yy63, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
}
// 3323 "parser.c"
        break;
      case 180:
// 1495 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("instanceof", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3331 "parser.c"
        break;
      case 182:
// 1503 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("not-equals", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3339 "parser.c"
        break;
      case 183:
// 1507 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("identical", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3347 "parser.c"
        break;
      case 184:
// 1511 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("not-identical", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3355 "parser.c"
        break;
      case 185:
// 1515 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("less", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3363 "parser.c"
        break;
      case 186:
// 1519 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_expr("greater", yymsp[-2].minor.yy63, yymsp[0].minor.yy63, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3371 "parser.c"
        break;
      case 187:
// 1523 "parser.lemon"
{
	yygotominor.yy63 = yymsp[-1].minor.yy63;
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3380 "parser.c"
        break;
      case 194:
// 1555 "parser.lemon"
{
	yygotominor.yy63 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3387 "parser.c"
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
// 671 "parser.lemon"


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

// 3463 "parser.c"
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
