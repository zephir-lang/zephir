/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 22 "parser.lemon"


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
#define YYNOCODE 112
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy13;
  int yy223;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 408
#define YYNRULE 190
#define YYERRORSYMBOL 62
#define YYERRSYMDT yy223
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
 /*     0 */   312,   52,   88,  220,  599,    1,    2,  407,    4,    5,
 /*    10 */     6,  199,  208,  214,  211,  358,   42,   44,   46,   77,
 /*    20 */    55,   59,   64,  359,   71,   99,  357,  130,  223,  196,
 /*    30 */   159,  167,  175,   97,  314,  190,  217,  220,  307,  144,
 /*    40 */   148,  363,  156,  157,  408,  199,  208,  214,  211,  244,
 /*    50 */    42,   44,   46,  322,   55,   59,   64,  123,   71,  136,
 /*    60 */   127,  132,  101,  103,  159,  167,  175,    8,  318,  190,
 /*    70 */   217,  220,  382,   86,  385,  386,   14,  341,  353,  199,
 /*    80 */   208,  214,  211,    9,   42,   44,   46,   22,   55,   59,
 /*    90 */    64,  315,   71,  300,  284,  288,  218,  207,  159,  167,
 /*   100 */   175,   85,  267,  190,  217,  220,  162,  339,   17,  354,
 /*   110 */   125,  380,  353,  199,  208,  214,  211,  364,   42,   44,
 /*   120 */    46,  161,   55,   59,   64,  365,   71,    3,    4,    5,
 /*   130 */     6,  130,  159,  167,  175,  130,  320,  190,  217,  220,
 /*   140 */   427,  223,  181,  305,   19,   11,  330,  199,  208,  214,
 /*   150 */   211,   18,   42,   44,   46,  124,   55,   59,   64,  397,
 /*   160 */    71,  401,  242,  126,  127,  132,  159,  167,  175,  129,
 /*   170 */   327,  190,  217,  220,  301,   86,  300,  284,  288,  184,
 /*   180 */   187,  199,  208,  214,  211,   86,   42,   44,   46,   20,
 /*   190 */    55,   59,   64,  275,   71,  300,  284,  288,  369,   51,
 /*   200 */   159,  167,  175,  142,  274,  190,  217,  220,  431,   21,
 /*   210 */    20,  305,   19,  197,  330,  199,  208,  214,  211,  171,
 /*   220 */    42,   44,   46,  223,   55,   59,   64,  426,   71,  370,
 /*   230 */   305,   19,   88,  330,  159,  167,  175,  371,  329,  190,
 /*   240 */   217,  220,  430,   43,  240,  305,   19,  172,  330,  199,
 /*   250 */   208,  214,  211,   86,   42,   44,   46,   45,   55,   59,
 /*   260 */    64,  432,   71,   87,  305,   19,  223,  330,  159,  167,
 /*   270 */   175,   65,  336,  190,  217,  220,  429,   86,  234,  384,
 /*   280 */   368,  107,  374,  199,  208,  214,  211,  228,   42,   44,
 /*   290 */    46,   56,   55,   59,   64,  340,   71,  105,  341,  353,
 /*   300 */   375,  279,  159,  167,  175,  255,   58,  190,  217,  220,
 /*   310 */   202,  223,  331,   81,   98,  236,  238,  199,  208,  214,
 /*   320 */   211,  164,   42,   44,   46,  219,   55,   59,   64,  302,
 /*   330 */    71,  306,  257,  321,  343,  202,  159,  167,  175,   76,
 /*   340 */   278,  190,  217,  220,  383,  354,  160,  166,  353,   75,
 /*   350 */   216,  199,  208,  214,  211,  376,   42,   44,   46,   78,
 /*   360 */    55,   59,   64,  377,   71,  356,   88,  362,  343,  109,
 /*   370 */   159,  167,  175,  279,   25,  190,  217,  220,  280,  284,
 /*   380 */   288,   82,  223,  215,  207,  199,  208,  214,  211,   83,
 /*   390 */    42,   44,   46,  106,   55,   59,   64,   97,   71,  202,
 /*   400 */    88,  316,  223,  225,  159,  167,  175,  128,  338,  190,
 /*   410 */   217,  220,   79,  110,  213,  212,  207,  100,  202,  199,
 /*   420 */   208,  214,  211,  258,   42,   44,   46,   84,   55,   59,
 /*   430 */    64,   97,   71,  210,   88,  209,  207,  202,  159,  167,
 /*   440 */   175,  102,  266,  190,  217,  220,  200,  207,   12,  152,
 /*   450 */   388,  111,  201,  199,  208,  214,  211,  104,   42,   44,
 /*   460 */    46,  254,   55,   59,   64,   97,   71,  193,   88,  279,
 /*   470 */   191,  567,  159,  167,  175,  164,   27,  190,  217,  220,
 /*   480 */   149,  177,  180,  356,  259,  362,  276,  199,  208,  214,
 /*   490 */   211,  113,   42,   44,   46,  141,   55,   59,   64,   97,
 /*   500 */    71,  163,   72,  158,   74,  115,  159,  167,  175,  568,
 /*   510 */   273,  190,  217,  220,  223,  223,  223,  569,  223,   73,
 /*   520 */   138,  199,  208,  214,  211,   66,   42,   44,   46,  223,
 /*   530 */    55,   59,   64,  145,   71,  262,   67,   60,  223,   47,
 /*   540 */   159,  167,  175,  139,   54,  190,  217,  220,  223,  306,
 /*   550 */   250,  321,  146,  150,  153,  199,  208,  214,  211,  248,
 /*   560 */    42,   44,   46,  147,   55,   59,   64,  154,   71,  246,
 /*   570 */   151,  155,  168,  169,  159,  167,  175,  173,   63,  190,
 /*   580 */   217,  220,  174,  176,  178,  182,  179,  183,  185,  199,
 /*   590 */   208,  214,  211,  186,   42,   44,   46,  188,   55,   59,
 /*   600 */    64,  189,   71,  192,  194,  204,  195,  198,  159,  167,
 /*   610 */   175,  203,  268,  190,  217,  220,  205,  222,  232,  206,
 /*   620 */   231,  233,  261,  199,  208,  214,  211,  235,   42,   44,
 /*   630 */    46,  260,   55,   59,   64,  270,   71,  253,  271,  282,
 /*   640 */   237,  239,  159,  167,  175,  285,   50,  190,  217,  220,
 /*   650 */   283,  289,  286,  290,  287,  293,  294,  199,  208,  214,
 /*   660 */   211,  291,   42,   44,   46,  469,   55,   59,   64,  470,
 /*   670 */    71,  471,  472,  473,  474,  307,  159,  167,  175,  304,
 /*   680 */   265,  190,  217,  220,  303,  308,  311,  317,  323,  326,
 /*   690 */   309,  199,  208,  214,  211,  324,   42,   44,   46,  322,
 /*   700 */    55,   59,   64,  325,   71,  331,  332,  333,  335,  334,
 /*   710 */   159,  167,  175,  344,  269,  190,  217,  220,  345,  346,
 /*   720 */   347,  349,  351,  350,  352,  199,  208,  214,  211,  361,
 /*   730 */    42,   44,   46,  367,   55,   59,   64,  373,   71,  360,
 /*   740 */   366,  379,  389,  392,  159,  167,  175,  395,   70,  190,
 /*   750 */   217,  220,  372,  390,  396,  399,  378,  402,  403,  199,
 /*   760 */   208,  214,  211,  405,   42,   44,   46,  303,   55,   59,
 /*   770 */    64,  303,   71,  303,  303,  303,  303,  303,  159,  167,
 /*   780 */   175,  303,  220,  190,  217,  303,  303,  303,  303,  303,
 /*   790 */   199,  208,  214,  211,   89,   42,   44,   46,  303,   55,
 /*   800 */    59,   64,   41,   71,  303,  303,  303,  303,   96,  159,
 /*   810 */   167,  175,   49,  303,  190,  217,  221,   29,   30,   31,
 /*   820 */    32,   33,   34,   35,   36,   37,   38,   39,   40,  303,
 /*   830 */    41,   90,   91,  303,  303,   92,   93,   94,  303,   95,
 /*   840 */    62,  303,  303,  303,  221,   29,   30,   31,   32,   33,
 /*   850 */    34,   35,   36,   37,   38,   39,   40,  303,  303,  303,
 /*   860 */    41,  303,  303,  226,  241,  245,  247,  243,  229,  303,
 /*   870 */   264,  222,  303,  303,  221,   29,   30,   31,   32,   33,
 /*   880 */    34,   35,   36,   37,   38,   39,   40,   41,  303,  303,
 /*   890 */   251,  303,  398,   16,  381,  387,  386,   57,  380,  353,
 /*   900 */   303,  221,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   910 */    37,   38,   39,   40,   41,  303,   13,   16,  381,  387,
 /*   920 */   386,  303,  380,  353,  272,  303,  303,  303,  221,   29,
 /*   930 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   940 */    40,  303,  303,  303,   41,  303,  391,   16,  381,  387,
 /*   950 */   386,  303,  380,  353,   53,  303,  303,  303,  221,   29,
 /*   960 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   970 */    40,   41,  303,  292,  303,  303,  404,   16,  381,  387,
 /*   980 */   386,   69,  380,  353,  281,  221,   29,   30,   31,   32,
 /*   990 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  295,
 /*  1000 */   296,  297,  298,  299,  303,  303,  303,  303,   26,  303,
 /*  1010 */   303,  303,  221,   29,   30,   31,   32,   33,   34,   35,
 /*  1020 */    36,   37,   38,   39,   40,  303,   90,   91,   41,  303,
 /*  1030 */    92,   93,   94,   15,   95,  303,  355,  368,  277,  374,
 /*  1040 */   348,  303,  221,   29,   30,   31,   32,   33,   34,   35,
 /*  1050 */    36,   37,   38,   39,   40,   41,  425,  303,  303,  355,
 /*  1060 */   368,  303,  374,  348,  303,  313,  303,  303,  303,  221,
 /*  1070 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1080 */    39,   40,   41,  400,  303,  303,  355,  368,  303,  374,
 /*  1090 */   348,  303,  319,  303,  303,  303,  221,   29,   30,   31,
 /*  1100 */    32,   33,   34,   35,   36,   37,   38,   39,   40,  303,
 /*  1110 */   393,  303,   41,  355,  368,  303,  374,  348,  303,  303,
 /*  1120 */   303,  303,  328,  303,  303,  303,  221,   29,   30,   31,
 /*  1130 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1140 */   303,  303,  303,  303,  303,  114,  112,  137,  303,  337,
 /*  1150 */    80,  303,   41,  221,   29,   30,   31,   32,   33,   34,
 /*  1160 */    35,   36,   37,   38,   39,   40,   28,   29,   30,   31,
 /*  1170 */    32,   33,   34,   35,   36,   37,   38,   39,   40,  227,
 /*  1180 */   121,  303,  224,  292,  252,  303,  303,  116,  117,  303,
 /*  1190 */   303,  118,  119,  120,  281,  143,  303,  249,  303,  406,
 /*  1200 */   133,  303,  355,  368,  303,  374,  348,  303,  310,  295,
 /*  1210 */   296,  297,  298,  299,  303,  303,  303,  303,  303,  303,
 /*  1220 */   303,   90,   91,  303,   89,   92,   93,   94,  303,   95,
 /*  1230 */   230,  122,  303,  303,  303,  303,  303,  135,  134,  303,
 /*  1240 */   303,   92,   93,   94,  303,   95,   89,  292,  226,  241,
 /*  1250 */   245,  247,  243,  229,  303,  303,  222,  303,  281,  303,
 /*  1260 */   256,   90,   91,  303,   61,   92,   93,   94,  303,   95,
 /*  1270 */   303,  303,   23,  295,  296,  297,  298,  299,  303,  303,
 /*  1280 */   303,  303,  303,   90,   91,  409,  303,   92,   93,   94,
 /*  1290 */   303,   95,  226,  241,  245,  247,  243,  229,  303,    7,
 /*  1300 */   222,  303,   10,   89,  303,  303,  394,  220,  263,  303,
 /*  1310 */   303,  226,  241,  245,  247,  243,  229,  140,  303,  222,
 /*  1320 */   226,  241,  245,  247,  243,  229,  303,   48,  222,   89,
 /*  1330 */   303,  424,  303,  303,  355,  368,   68,  374,  348,  303,
 /*  1340 */    90,   91,  303,  108,   92,   93,   94,  428,   95,  292,
 /*  1350 */   342,   19,  165,  330,  348,  303,  303,  303,  303,  303,
 /*  1360 */   281,  131,  303,   24,  303,  303,   90,   91,  303,  303,
 /*  1370 */    92,   93,   94,  303,   95,  295,  296,  297,  298,  299,
 /*  1380 */   303,  303,  133,  303,  303,  303,  303,  303,  303,   90,
 /*  1390 */    91,  170,  303,   92,   93,   94,  303,   95,   90,   91,
 /*  1400 */   303,  303,   92,   93,   94,    7,   95,  303,   10,  303,
 /*  1410 */   303,  303,  394,  220,  303,  303,  303,  303,  303,  135,
 /*  1420 */   134,  303,  303,   92,   93,   94,  303,   95,   90,   91,
 /*  1430 */   303,  303,   92,   93,   94,  303,   95,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    19,   18,   75,   22,   63,   64,   65,   66,   67,   68,
 /*    10 */    69,   30,   31,   32,   33,   16,   35,   36,   37,   24,
 /*    20 */    39,   40,   41,   24,   43,   15,   15,   75,   75,  102,
 /*    30 */    49,   50,   51,  106,   19,   54,   55,   22,   27,   44,
 /*    40 */    45,   15,   47,   48,    0,   30,   31,   32,   33,   96,
 /*    50 */    35,   36,   37,   27,   39,   40,   41,  105,   43,  107,
 /*    60 */   108,  109,   52,   53,   49,   50,   51,   15,   19,   54,
 /*    70 */    55,   22,   71,    1,   73,   74,   19,   76,   77,   30,
 /*    80 */    31,   32,   33,   16,   35,   36,   37,   28,   39,   40,
 /*    90 */    41,   78,   43,   80,   81,   82,  103,  104,   49,   50,
 /*   100 */    51,   29,   19,   54,   55,   22,    1,   72,   73,   74,
 /*   110 */     1,   76,   77,   30,   31,   32,   33,   16,   35,   36,
 /*   120 */    37,   16,   39,   40,   41,   24,   43,   66,   67,   68,
 /*   130 */    69,   75,   49,   50,   51,   75,   19,   54,   55,   22,
 /*   140 */    19,   75,   15,   22,   23,   15,   25,   30,   31,   32,
 /*   150 */    33,   77,   35,   36,   37,   46,   39,   40,   41,   18,
 /*   160 */    43,   20,   96,  107,  108,  109,   49,   50,   51,  109,
 /*   170 */    19,   54,   55,   22,   78,    1,   80,   81,   82,   52,
 /*   180 */    53,   30,   31,   32,   33,    1,   35,   36,   37,   27,
 /*   190 */    39,   40,   41,   78,   43,   80,   81,   82,   15,   38,
 /*   200 */    49,   50,   51,   29,   19,   54,   55,   22,   19,   15,
 /*   210 */    27,   22,   23,   29,   25,   30,   31,   32,   33,   16,
 /*   220 */    35,   36,   37,   75,   39,   40,   41,   19,   43,   16,
 /*   230 */    22,   23,   75,   25,   49,   50,   51,   24,   19,   54,
 /*   240 */    55,   22,   19,   16,   96,   22,   23,   44,   25,   30,
 /*   250 */    31,   32,   33,    1,   35,   36,   37,   16,   39,   40,
 /*   260 */    41,   19,   43,  106,   22,   23,   75,   25,   49,   50,
 /*   270 */    51,   15,   19,   54,   55,   22,   19,    1,   15,   22,
 /*   280 */    23,   29,   25,   30,   31,   32,   33,   96,   35,   36,
 /*   290 */    37,   18,   39,   40,   41,   73,   43,   28,   76,   77,
 /*   300 */    15,    1,   49,   50,   51,   29,   19,   54,   55,   22,
 /*   310 */     1,   75,   27,   44,   45,   52,   53,   30,   31,   32,
 /*   320 */    33,   75,   35,   36,   37,   16,   39,   40,   41,   29,
 /*   330 */    43,   23,   96,   25,   26,    1,   49,   50,   51,   15,
 /*   340 */    19,   54,   55,   22,   73,   74,  100,  101,   77,   98,
 /*   350 */    16,   30,   31,   32,   33,   16,   35,   36,   37,   99,
 /*   360 */    39,   40,   41,   24,   43,   23,   75,   25,   26,   99,
 /*   370 */    49,   50,   51,    1,   19,   54,   55,   22,   80,   81,
 /*   380 */    82,   15,   75,  103,  104,   30,   31,   32,   33,   28,
 /*   390 */    35,   36,   37,  102,   39,   40,   41,  106,   43,    1,
 /*   400 */    75,   29,   75,   96,   49,   50,   51,   59,   19,   54,
 /*   410 */    55,   22,    8,    9,   16,  103,  104,   46,    1,   30,
 /*   420 */    31,   32,   33,   96,   35,   36,   37,  102,   39,   40,
 /*   430 */    41,  106,   43,   16,   75,  103,  104,    1,   49,   50,
 /*   440 */    51,   46,   19,   54,   55,   22,  103,  104,   18,   15,
 /*   450 */    20,   99,   16,   30,   31,   32,   33,   46,   35,   36,
 /*   460 */    37,  102,   39,   40,   41,  106,   43,   12,   75,    1,
 /*   470 */    15,   59,   49,   50,   51,   75,   19,   54,   55,   22,
 /*   480 */    46,   44,   45,   23,    1,   25,   18,   30,   31,   32,
 /*   490 */    33,   99,   35,   36,   37,  102,   39,   40,   41,  106,
 /*   500 */    43,  101,   97,   98,    1,   99,   49,   50,   51,   59,
 /*   510 */    19,   54,   55,   22,   75,   75,   75,   59,   75,   16,
 /*   520 */    15,   30,   31,   32,   33,   42,   35,   36,   37,   75,
 /*   530 */    39,   40,   41,   15,   43,   96,   96,   96,   75,   96,
 /*   540 */    49,   50,   51,   28,   19,   54,   55,   22,   75,   23,
 /*   550 */    96,   25,   24,   24,   46,   30,   31,   32,   33,   96,
 /*   560 */    35,   36,   37,   99,   39,   40,   41,   24,   43,   96,
 /*   570 */    99,   99,   75,   16,   49,   50,   51,   15,   19,   54,
 /*   580 */    55,   22,   16,   15,   15,   46,   16,   16,   46,   30,
 /*   590 */    31,   32,   33,   16,   35,   36,   37,   46,   39,   40,
 /*   600 */    41,   16,   43,   16,   15,   15,   28,   16,   49,   50,
 /*   610 */    51,  104,   19,   54,   55,   22,   24,   10,   15,   75,
 /*   620 */   110,   45,   42,   30,   31,   32,   33,   46,   35,   36,
 /*   630 */    37,   15,   39,   40,   41,   38,   43,   28,   18,   24,
 /*   640 */    46,   46,   49,   50,   51,   15,   19,   54,   55,   22,
 /*   650 */    75,   15,   24,   24,   75,   15,    5,   30,   31,   32,
 /*   660 */    33,   75,   35,   36,   37,   15,   39,   40,   41,   15,
 /*   670 */    43,   15,   15,   15,   15,   27,   49,   50,   51,   19,
 /*   680 */    19,   54,   55,   22,   18,   15,   18,   18,   15,   18,
 /*   690 */    28,   30,   31,   32,   33,   28,   35,   36,   37,   27,
 /*   700 */    39,   40,   41,   29,   43,   27,   15,   28,   18,   29,
 /*   710 */    49,   50,   51,   15,   19,   54,   55,   22,   24,   75,
 /*   720 */    16,   15,   75,   24,   16,   30,   31,   32,   33,   16,
 /*   730 */    35,   36,   37,   16,   39,   40,   41,   16,   43,   75,
 /*   740 */    75,   16,   15,   19,   49,   50,   51,   17,   19,   54,
 /*   750 */    55,   22,   75,   18,   15,   19,   75,   15,   18,   30,
 /*   760 */    31,   32,   33,   19,   35,   36,   37,  111,   39,   40,
 /*   770 */    41,  111,   43,  111,  111,  111,  111,  111,   49,   50,
 /*   780 */    51,  111,   22,   54,   55,  111,  111,  111,  111,  111,
 /*   790 */    30,   31,   32,   33,   15,   35,   36,   37,  111,   39,
 /*   800 */    40,   41,   69,   43,  111,  111,  111,  111,   29,   49,
 /*   810 */    50,   51,   79,  111,   54,   55,   83,   84,   85,   86,
 /*   820 */    87,   88,   89,   90,   91,   92,   93,   94,   95,  111,
 /*   830 */    69,   52,   53,  111,  111,   56,   57,   58,  111,   60,
 /*   840 */    79,  111,  111,  111,   83,   84,   85,   86,   87,   88,
 /*   850 */    89,   90,   91,   92,   93,   94,   95,  111,  111,  111,
 /*   860 */    69,  111,  111,    2,    3,    4,    5,    6,    7,  111,
 /*   870 */    79,   10,  111,  111,   83,   84,   85,   86,   87,   88,
 /*   880 */    89,   90,   91,   92,   93,   94,   95,   69,  111,  111,
 /*   890 */    29,  111,   70,   71,   72,   73,   74,   79,   76,   77,
 /*   900 */   111,   83,   84,   85,   86,   87,   88,   89,   90,   91,
 /*   910 */    92,   93,   94,   95,   69,  111,   70,   71,   72,   73,
 /*   920 */    74,  111,   76,   77,   79,  111,  111,  111,   83,   84,
 /*   930 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   940 */    95,  111,  111,  111,   69,  111,   70,   71,   72,   73,
 /*   950 */    74,  111,   76,   77,   79,  111,  111,  111,   83,   84,
 /*   960 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   970 */    95,   69,  111,    4,  111,  111,   70,   71,   72,   73,
 /*   980 */    74,   79,   76,   77,   15,   83,   84,   85,   86,   87,
 /*   990 */    88,   89,   90,   91,   92,   93,   94,   95,   69,   30,
 /*  1000 */    31,   32,   33,   34,  111,  111,  111,  111,   79,  111,
 /*  1010 */   111,  111,   83,   84,   85,   86,   87,   88,   89,   90,
 /*  1020 */    91,   92,   93,   94,   95,  111,   52,   53,   69,  111,
 /*  1030 */    56,   57,   58,   19,   60,  111,   22,   23,   79,   25,
 /*  1040 */    26,  111,   83,   84,   85,   86,   87,   88,   89,   90,
 /*  1050 */    91,   92,   93,   94,   95,   69,   19,  111,  111,   22,
 /*  1060 */    23,  111,   25,   26,  111,   79,  111,  111,  111,   83,
 /*  1070 */    84,   85,   86,   87,   88,   89,   90,   91,   92,   93,
 /*  1080 */    94,   95,   69,   19,  111,  111,   22,   23,  111,   25,
 /*  1090 */    26,  111,   79,  111,  111,  111,   83,   84,   85,   86,
 /*  1100 */    87,   88,   89,   90,   91,   92,   93,   94,   95,  111,
 /*  1110 */    19,  111,   69,   22,   23,  111,   25,   26,  111,  111,
 /*  1120 */   111,  111,   79,  111,  111,  111,   83,   84,   85,   86,
 /*  1130 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   69,
 /*  1140 */   111,  111,  111,  111,  111,   10,   11,   12,  111,   79,
 /*  1150 */    15,  111,   69,   83,   84,   85,   86,   87,   88,   89,
 /*  1160 */    90,   91,   92,   93,   94,   95,   83,   84,   85,   86,
 /*  1170 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   10,
 /*  1180 */    45,  111,   13,    4,   15,  111,  111,   52,   53,  111,
 /*  1190 */   111,   56,   57,   58,   15,   60,  111,   28,  111,   19,
 /*  1200 */    15,  111,   22,   23,  111,   25,   26,  111,   29,   30,
 /*  1210 */    31,   32,   33,   34,  111,  111,  111,  111,  111,  111,
 /*  1220 */   111,   52,   53,  111,   15,   56,   57,   58,  111,   60,
 /*  1230 */    61,   46,  111,  111,  111,  111,  111,   52,   53,  111,
 /*  1240 */   111,   56,   57,   58,  111,   60,   15,    4,    2,    3,
 /*  1250 */     4,    5,    6,    7,  111,  111,   10,  111,   15,  111,
 /*  1260 */    29,   52,   53,  111,   18,   56,   57,   58,  111,   60,
 /*  1270 */   111,  111,   29,   30,   31,   32,   33,   34,  111,  111,
 /*  1280 */   111,  111,  111,   52,   53,    0,  111,   56,   57,   58,
 /*  1290 */   111,   60,    2,    3,    4,    5,    6,    7,  111,   14,
 /*  1300 */    10,  111,   17,   15,  111,  111,   21,   22,   18,  111,
 /*  1310 */   111,    2,    3,    4,    5,    6,    7,   29,  111,   10,
 /*  1320 */     2,    3,    4,    5,    6,    7,  111,   18,   10,   15,
 /*  1330 */   111,   19,  111,  111,   22,   23,   18,   25,   26,  111,
 /*  1340 */    52,   53,  111,   29,   56,   57,   58,   19,   60,    4,
 /*  1350 */    22,   23,   15,   25,   26,  111,  111,  111,  111,  111,
 /*  1360 */    15,   15,  111,   18,  111,  111,   52,   53,  111,  111,
 /*  1370 */    56,   57,   58,  111,   60,   30,   31,   32,   33,   34,
 /*  1380 */   111,  111,   15,  111,  111,  111,  111,  111,  111,   52,
 /*  1390 */    53,   15,  111,   56,   57,   58,  111,   60,   52,   53,
 /*  1400 */   111,  111,   56,   57,   58,   14,   60,  111,   17,  111,
 /*  1410 */   111,  111,   21,   22,  111,  111,  111,  111,  111,   52,
 /*  1420 */    53,  111,  111,   56,   57,   58,  111,   60,   52,   53,
 /*  1430 */   111,  111,   56,   57,   58,  111,   60,
};
#define YY_SHIFT_USE_DFLT (-20)
static short yy_shift_ofst[] = {
 /*     0 */  1391,   44, 1285,  -20,  -20,  -20,  -20,   52,   67,  -20,
 /*    10 */   130,  430, 1014,   57,  -20,  -20, 1312,  121,  -20,  162,
 /*    20 */   194,   59, 1243, 1345,  355,  -20,  457,  -20,  -20,  -20,
 /*    30 */   -20,  -20,  -20,  -20,  -20,  -20,  -20,  -20,  -20,  -20,
 /*    40 */   -20,  -20,  227,  -20,  241,  -20, 1169, 1309,  695,  627,
 /*    50 */   161,  -17,  593,  525,  -20,  273,   83,  287,  -20, 1169,
 /*    60 */  1246,  423,  559,  -20,  256,  483, 1169, 1318,  760,  729,
 /*    70 */   -20,  324,  503,  -20,  324,  -20,   -5, 1135,  404, 1135,
 /*    80 */   269,  366,  361,  779,   72,  -20, 1209,  -20,  -20,  -20,
 /*    90 */   -20,  -20,  -20,  -20,  -20,  -20,  -20,  -20,   10,  371,
 /*   100 */   -20,  395,  -20,  411,  -20, 1314,  252,  -20,  -20,  -20,
 /*   110 */  1135,  -20, 1135,  -20, 1135,  -20,  -20,  -20,  -20,  -20,
 /*   120 */   -20, 1185,  -20,  109,  -20, 1367,  -20,  348, 1346,  -20,
 /*   130 */   -20,  -20,  -20,  412,  450,  458,  -20,  505,  515, 1288,
 /*   140 */   -20,  174,  -20,  -20,  518,  528, 1135,  404,  434,  529,
 /*   150 */  1135,  404,  508,  543, 1135,  404,  -20,  -20,  -20, 1337,
 /*   160 */   105,  -20, 1337,  -20,  -20,  -20,  -20, 1376,  557,  -20,
 /*   170 */   203,  -20,  562,  566,  -20,  568,  437,  569,  570,  -20,
 /*   180 */   127,  539,  571,  -20,  542,  577,  -20,  551,  585,  -20,
 /*   190 */   455,  587,  -20,  589,  578, 1209,  184,  591,  -20,  590,
 /*   200 */   436,  -20,  590,  -20,  592,  974,  -20,  -20,  590,  417,
 /*   210 */   -20,  590,  398,  -20,  590,  334,  -20,  590,  309,  -20,
 /*   220 */   -20,  -20, 1169,  -20, 1169,  -20, 1169, 1169,  607, 1169,
 /*   230 */   603,  -20,  576,  263,  581,  -20,  594,  -20,  595,  -20,
 /*   240 */   607, 1169,  607, 1169,  607, 1169,  607, 1169,  607, 1169,
 /*   250 */   861,  -20,  609, 1231,  276,  -20,  -20,  607,  607,  616,
 /*   260 */   580, 1169, 1290,  760,  661,  -20,  -20,  -20,  -20,  597,
 /*   270 */   620,  185,  491,  -20,  -20,  468,  760,  321,  -20,  969,
 /*   280 */   -20,  615,  974,  -20,  630,  628,  974,  -20,  636,  629,
 /*   290 */   974,  -20,  640,  651,  650,  654,  656,  657,  658,  659,
 /*   300 */   -20,  300,  666,  660,  -20,  526,  648,  670,  662, 1179,
 /*   310 */   668,  -19,  -20,   15,  -20,  372,  669,   49,  -20,  117,
 /*   320 */   -20,  672,  673,  667,  674,  671,  151,  -20,  219,  -20,
 /*   330 */   678,  691,  679,  680,  690,  253,  -20,  389,  -20, 1328,
 /*   340 */   189,  -20,  308,  698,  694,  974,  704,  -20,  706,  699,
 /*   350 */   974,  708,  -20,  -20,  -20,  342,   11,   -1,  -20,  974,
 /*   360 */   713,  -20,   26,  101,  -20,  974,  717,  -20,  183,  213,
 /*   370 */   -20,  974,  721,  -20,  285,  339,  -20,  974,  725,  -20,
 /*   380 */   -20, 1037,  257,  242,  460,  223,  -20,  208,  727,  735,
 /*   390 */  1091,  724,  -20,  -20,  730,  739,  141, 1064,  736,  -20,
 /*   400 */   -20,  742,  740, 1180,  744,  -20,  -20,  -20,
};
#define YY_REDUCE_USE_DFLT (-74)
static short yy_reduce_ofst[] = {
 /*     0 */   -59,  -74,   61,  -74,  -74,  -74,  -74,  -74,  -74,  -74,
 /*    10 */   -74,  -74,  846,  -74,  -74,  -74,   35,   74,  -74,  -74,
 /*    20 */   -74,  -74,   96,  115,  929,  -74, 1083,  -74,  -74,  -74,
 /*    30 */   -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,
 /*    40 */   -74,  -74,  -74,  -74,  -74,  -74,  443,  -74,  733, 1083,
 /*    50 */   -74,  -74,  875, 1083,  -74,  -74,  818, 1083,  -74,  441,
 /*    60 */   -74,  761, 1083,  -74,  -74,  -74,  440,  -74,  902, 1083,
 /*    70 */   -74,  405,  -74,  -74,  251,  -74,  -74,  260,  -74,  270,
 /*    80 */   -74,  -74,  -74,  325,  -74,  -74,  157,  -74,  -74,  -74,
 /*    90 */   -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,
 /*   100 */   -74,  -74,  -74,  -74,  -74,  291,  -74,  -74,  -74,  -74,
 /*   110 */   352,  -74,  392,  -74,  406,  -74,  -74,  -74,  -74,  -74,
 /*   120 */   -74,  -48,  -74,  -74,  -74,   56,  -74,  -74,   60,  -74,
 /*   130 */   -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  393,
 /*   140 */   -74,  -74,  -74,  -74,  -74,  -74,  464,  -74,  -74,  -74,
 /*   150 */   471,  -74,  -74,  -74,  472,  -74,  -74,  -74,  -74,  246,
 /*   160 */   -74,  -74,  400,  -74,  -74,  -74,  -74,  497,  -74,  -74,
 /*   170 */   -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,
 /*   180 */   -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,
 /*   190 */   -74,  -74,  -74,  -74,  -74,  -73,  -74,  -74,  -74,  343,
 /*   200 */   -74,  -74,  507,  -74,  -74,  544,  -74,  -74,  332,  -74,
 /*   210 */   -74,  312,  -74,  -74,  280,  -74,  -74,   -7,  -74,  -74,
 /*   220 */   -74,  -74,  327,  -74,  307,  -74,  236,  191,  -74,  148,
 /*   230 */   510,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,
 /*   240 */   -74,   66,  -74,  -47,  -74,  473,  -74,  463,  -74,  454,
 /*   250 */   -74,  -74,  -74,  359,  -74,  -74,  -74,  -74,  -74,  -74,
 /*   260 */   -74,  439,  -74,  791, 1083,  -74,  -74,  -74,  -74,  -74,
 /*   270 */   -74,  845, 1083,  -74,  -74,  -74,  959, 1083,  -74,  298,
 /*   280 */   -74,  -74,  575,  -74,  -74,  -74,  579,  -74,  -74,  -74,
 /*   290 */   586,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,
 /*   300 */   -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,   13,
 /*   310 */   -74,  986,  -74, 1083,  -74,  -74,  -74, 1013,  -74, 1083,
 /*   320 */   -74,  -74,  -74,  -74,  -74,  -74, 1043,  -74, 1083,  -74,
 /*   330 */   -74,  -74,  -74,  -74,  -74, 1070,  -74, 1083,  -74,  222,
 /*   340 */    74,  -74,  -74,  -74,  -74,  644,  -74,  -74,  -74,  -74,
 /*   350 */   647,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  -74,  664,
 /*   360 */   -74,  -74,  -74,  -74,  -74,  665,  -74,  -74,  -74,  -74,
 /*   370 */   -74,  677,  -74,  -74,  -74,  -74,  -74,  681,  -74,  -74,
 /*   380 */   -74,    1,  271,   74,  -74,   74,  -74,   74,  -74,  -74,
 /*   390 */   876,  -74,  -74,  -74,  -74,  -74,  -74,  822,  -74,  -74,
 /*   400 */   -74,  -74,  -74,  906,  -74,  -74,  -74,  -74,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   598,  598,  598,  410,  412,  413,  414,  598,  598,  415,
 /*    10 */   598,  598,  598,  598,  416,  420,  598,  598,  445,  598,
 /*    20 */   598,  598,  598,  598,  598,  449,  598,  453,  475,  477,
 /*    30 */   478,  479,  480,  481,  482,  483,  484,  485,  486,  487,
 /*    40 */   488,  489,  598,  490,  598,  491,  598,  598,  598,  598,
 /*    50 */   494,  598,  598,  598,  495,  598,  598,  598,  499,  598,
 /*    60 */   598,  598,  598,  501,  598,  598,  598,  598,  598,  598,
 /*    70 */   502,  598,  598,  504,  598,  505,  598,  598,  507,  598,
 /*    80 */   544,  598,  536,  598,  598,  557,  598,  559,  561,  562,
 /*    90 */   573,  574,  575,  576,  577,  578,  558,  560,  598,  598,
 /*   100 */   537,  598,  538,  598,  539,  598,  598,  555,  556,  540,
 /*   110 */   598,  541,  598,  542,  598,  543,  545,  546,  547,  548,
 /*   120 */   549,  598,  550,  598,  551,  598,  563,  598,  598,  565,
 /*   130 */   570,  571,  566,  571,  574,  573,  564,  598,  552,  598,
 /*   140 */   553,  598,  554,  572,  598,  598,  598,  508,  598,  598,
 /*   150 */   598,  509,  598,  598,  598,  510,  511,  512,  506,  598,
 /*   160 */   598,  513,  598,  514,  516,  517,  515,  598,  598,  518,
 /*   170 */   598,  519,  598,  598,  520,  598,  598,  598,  598,  521,
 /*   180 */   598,  598,  598,  522,  598,  598,  523,  598,  598,  524,
 /*   190 */   598,  598,  525,  598,  598,  598,  598,  598,  526,  598,
 /*   200 */   598,  527,  598,  532,  534,  598,  535,  533,  598,  598,
 /*   210 */   528,  598,  598,  529,  598,  598,  530,  598,  598,  531,
 /*   220 */   597,  476,  598,  579,  598,  580,  598,  598,  581,  598,
 /*   230 */   598,  582,  598,  598,  598,  594,  598,  595,  598,  596,
 /*   240 */   585,  598,  586,  598,  587,  598,  588,  598,  589,  598,
 /*   250 */   598,  590,  591,  598,  598,  592,  593,  584,  583,  598,
 /*   260 */   598,  598,  598,  598,  598,  503,  500,  498,  496,  492,
 /*   270 */   598,  598,  598,  497,  493,  598,  598,  598,  454,  598,
 /*   280 */   461,  463,  598,  466,  598,  464,  598,  467,  598,  465,
 /*   290 */   598,  468,  598,  598,  598,  598,  598,  598,  598,  598,
 /*   300 */   462,  598,  598,  598,  450,  598,  598,  598,  598,  598,
 /*   310 */   598,  598,  451,  598,  455,  598,  598,  598,  452,  598,
 /*   320 */   456,  598,  598,  598,  598,  598,  598,  458,  598,  460,
 /*   330 */   598,  598,  598,  598,  598,  598,  457,  598,  459,  598,
 /*   340 */   598,  443,  598,  598,  598,  598,  598,  446,  598,  598,
 /*   350 */   598,  598,  447,  448,  433,  598,  598,  598,  435,  598,
 /*   360 */   598,  438,  598,  598,  440,  598,  598,  442,  598,  598,
 /*   370 */   436,  598,  598,  437,  598,  598,  439,  598,  598,  441,
 /*   380 */   444,  598,  598,  598,  598,  598,  434,  598,  598,  598,
 /*   390 */   598,  598,  417,  421,  598,  598,  598,  598,  598,  418,
 /*   400 */   422,  598,  598,  598,  598,  419,  423,  411,
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
  "ADD",           "SUB",           "TYPEOF",        "INSTANCEOF",  
  "NEW",           "NOT",           "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "EXTENDS",       "ABSTRACT",      "COMMENT",       "PUBLIC",      
  "ASSIGN",        "PROTECTED",     "CONST",         "FUNCTION",    
  "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",  "TYPE_INTEGER",  "TYPE_DOUBLE", 
  "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",    "BREAK",       
  "CONTINUE",      "IF",            "ELSE",          "LOOP",        
  "WHILE",         "FOR",           "IN",            "LET",         
  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "INCR",        
  "DECR",          "ECHO",          "RETURN",        "UNSET",       
  "INTEGER",       "STRING",        "THROW",         "TYPE_VAR",    
  "DOUBLE",        "NULL",          "FALSE",         "COLON",       
  "TRUE",          "ISSET",         "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_literal_expr",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",  "xx_statement_list",
  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",
  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",
  "xx_return_statement",  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",
  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",  "xx_for_statement",
  "xx_eval_expr",  "xx_let_assignments",  "xx_let_assignment",  "xx_assign_expr",
  "xx_echo_expressions",  "xx_echo_expression",  "xx_call_parameters",  "xx_declare_variable_list",
  "xx_declare_variable",  "xx_array_list",  "xx_call_parameter",  "xx_array_item",
  "xx_array_key",  "xx_array_value",  "xx_isset_expr",
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
 /* 101 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 102 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 103 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 104 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 105 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 106 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 107 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 108 */ "xx_echo_expression ::= xx_literal_expr",
 /* 109 */ "xx_echo_expression ::= IDENTIFIER",
 /* 110 */ "xx_return_statement ::= RETURN xx_literal_expr DOTCOMMA",
 /* 111 */ "xx_return_statement ::= RETURN IDENTIFIER DOTCOMMA",
 /* 112 */ "xx_return_statement ::= RETURN IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 113 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 114 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 115 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 116 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 117 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 118 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 119 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 120 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 121 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 122 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 123 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 124 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 125 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 126 */ "xx_declare_variable ::= IDENTIFIER",
 /* 127 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 128 */ "xx_assign_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 129 */ "xx_assign_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 130 */ "xx_assign_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 131 */ "xx_assign_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 132 */ "xx_assign_expr ::= xx_assign_expr ADD xx_assign_expr",
 /* 133 */ "xx_assign_expr ::= xx_assign_expr SUB xx_assign_expr",
 /* 134 */ "xx_assign_expr ::= INSTANCEOF xx_assign_expr",
 /* 135 */ "xx_assign_expr ::= TYPEOF xx_assign_expr",
 /* 136 */ "xx_assign_expr ::= IDENTIFIER",
 /* 137 */ "xx_assign_expr ::= INTEGER",
 /* 138 */ "xx_assign_expr ::= STRING",
 /* 139 */ "xx_assign_expr ::= DOUBLE",
 /* 140 */ "xx_assign_expr ::= NULL",
 /* 141 */ "xx_assign_expr ::= FALSE",
 /* 142 */ "xx_assign_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 143 */ "xx_assign_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 144 */ "xx_assign_expr ::= NEW IDENTIFIER",
 /* 145 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 146 */ "xx_assign_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 147 */ "xx_assign_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 148 */ "xx_assign_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 149 */ "xx_assign_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 150 */ "xx_assign_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 151 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 152 */ "xx_call_parameters ::= xx_call_parameter",
 /* 153 */ "xx_call_parameter ::= xx_literal_expr",
 /* 154 */ "xx_call_parameter ::= IDENTIFIER",
 /* 155 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 156 */ "xx_array_list ::= xx_array_item",
 /* 157 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 158 */ "xx_array_item ::= xx_array_value",
 /* 159 */ "xx_array_key ::= IDENTIFIER",
 /* 160 */ "xx_array_key ::= STRING",
 /* 161 */ "xx_array_key ::= INTEGER",
 /* 162 */ "xx_array_value ::= xx_literal_expr",
 /* 163 */ "xx_array_value ::= IDENTIFIER",
 /* 164 */ "xx_assign_expr ::= TRUE",
 /* 165 */ "xx_literal_expr ::= INTEGER",
 /* 166 */ "xx_literal_expr ::= STRING",
 /* 167 */ "xx_literal_expr ::= DOUBLE",
 /* 168 */ "xx_literal_expr ::= NULL",
 /* 169 */ "xx_literal_expr ::= FALSE",
 /* 170 */ "xx_literal_expr ::= TRUE",
 /* 171 */ "xx_eval_expr ::= xx_literal_expr",
 /* 172 */ "xx_eval_expr ::= NOT xx_eval_expr",
 /* 173 */ "xx_eval_expr ::= TYPEOF xx_eval_expr",
 /* 174 */ "xx_eval_expr ::= ISSET xx_isset_expr",
 /* 175 */ "xx_eval_expr ::= xx_eval_expr TYPEOF xx_eval_expr",
 /* 176 */ "xx_eval_expr ::= xx_eval_expr EQUALS xx_eval_expr",
 /* 177 */ "xx_eval_expr ::= xx_eval_expr NOTEQUALS xx_eval_expr",
 /* 178 */ "xx_eval_expr ::= xx_eval_expr IDENTICAL xx_eval_expr",
 /* 179 */ "xx_eval_expr ::= xx_eval_expr NOTIDENTICAL xx_eval_expr",
 /* 180 */ "xx_eval_expr ::= xx_eval_expr LESS xx_eval_expr",
 /* 181 */ "xx_eval_expr ::= xx_eval_expr GREATER xx_eval_expr",
 /* 182 */ "xx_eval_expr ::= PARENTHESES_OPEN xx_eval_expr PARENTHESES_CLOSE",
 /* 183 */ "xx_eval_expr ::= IDENTIFIER",
 /* 184 */ "xx_eval_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 185 */ "xx_eval_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 186 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 187 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 188 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 189 */ "xx_comment ::= COMMENT",
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
// 688 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1617 "parser.c"
      break;
    case 64:
// 701 "parser.lemon"
{ json_object_put((yypminor->yy13)); }
// 1622 "parser.c"
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
  { 63, 1 },
  { 64, 1 },
  { 65, 2 },
  { 65, 1 },
  { 66, 1 },
  { 66, 1 },
  { 66, 1 },
  { 67, 3 },
  { 68, 5 },
  { 68, 7 },
  { 68, 6 },
  { 68, 8 },
  { 68, 4 },
  { 68, 6 },
  { 68, 5 },
  { 68, 7 },
  { 70, 1 },
  { 70, 1 },
  { 70, 1 },
  { 70, 2 },
  { 70, 2 },
  { 70, 2 },
  { 70, 2 },
  { 70, 3 },
  { 70, 3 },
  { 71, 2 },
  { 71, 1 },
  { 74, 4 },
  { 74, 3 },
  { 74, 5 },
  { 74, 6 },
  { 74, 3 },
  { 74, 4 },
  { 74, 5 },
  { 74, 6 },
  { 72, 2 },
  { 72, 1 },
  { 73, 2 },
  { 76, 6 },
  { 76, 5 },
  { 73, 1 },
  { 77, 7 },
  { 77, 8 },
  { 77, 8 },
  { 77, 9 },
  { 77, 8 },
  { 77, 9 },
  { 77, 9 },
  { 77, 10 },
  { 77, 7 },
  { 77, 8 },
  { 77, 8 },
  { 77, 9 },
  { 78, 3 },
  { 78, 1 },
  { 80, 1 },
  { 80, 2 },
  { 80, 2 },
  { 80, 3 },
  { 80, 4 },
  { 80, 4 },
  { 82, 3 },
  { 81, 1 },
  { 81, 1 },
  { 81, 1 },
  { 81, 1 },
  { 81, 1 },
  { 79, 2 },
  { 79, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 92, 2 },
  { 93, 2 },
  { 85, 4 },
  { 85, 7 },
  { 85, 5 },
  { 85, 9 },
  { 85, 8 },
  { 85, 8 },
  { 86, 3 },
  { 86, 4 },
  { 94, 4 },
  { 94, 5 },
  { 95, 7 },
  { 95, 9 },
  { 84, 3 },
  { 97, 3 },
  { 97, 1 },
  { 98, 3 },
  { 98, 5 },
  { 98, 5 },
  { 98, 6 },
  { 98, 2 },
  { 98, 2 },
  { 87, 3 },
  { 100, 3 },
  { 100, 1 },
  { 101, 1 },
  { 101, 1 },
  { 88, 3 },
  { 88, 3 },
  { 88, 5 },
  { 89, 5 },
  { 89, 6 },
  { 89, 6 },
  { 89, 6 },
  { 90, 3 },
  { 90, 7 },
  { 91, 3 },
  { 91, 3 },
  { 91, 3 },
  { 91, 3 },
  { 91, 3 },
  { 103, 3 },
  { 103, 1 },
  { 104, 1 },
  { 104, 3 },
  { 99, 3 },
  { 99, 4 },
  { 99, 4 },
  { 99, 4 },
  { 99, 3 },
  { 99, 3 },
  { 99, 2 },
  { 99, 2 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 2 },
  { 99, 3 },
  { 99, 2 },
  { 99, 4 },
  { 99, 5 },
  { 99, 4 },
  { 99, 3 },
  { 99, 6 },
  { 99, 5 },
  { 102, 3 },
  { 102, 1 },
  { 106, 1 },
  { 106, 1 },
  { 105, 3 },
  { 105, 1 },
  { 107, 3 },
  { 107, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 109, 1 },
  { 109, 1 },
  { 99, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 75, 1 },
  { 96, 1 },
  { 96, 2 },
  { 96, 2 },
  { 96, 2 },
  { 96, 3 },
  { 96, 3 },
  { 96, 3 },
  { 96, 3 },
  { 96, 3 },
  { 96, 3 },
  { 96, 3 },
  { 96, 3 },
  { 96, 1 },
  { 96, 4 },
  { 96, 3 },
  { 110, 4 },
  { 110, 4 },
  { 110, 4 },
  { 69, 1 },
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
// 697 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy13;
}
// 2029 "parser.c"
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
      case 153:
      case 162:
      case 171:
// 703 "parser.lemon"
{
	yygotominor.yy13 = yymsp[0].minor.yy13;
}
// 2055 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 707 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_list(yymsp[-1].minor.yy13, yymsp[0].minor.yy13);
}
// 2066 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 68:
      case 98:
      case 107:
      case 125:
      case 152:
      case 156:
// 711 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_list(NULL, yymsp[0].minor.yy13);
}
// 2083 "parser.c"
        break;
      case 7:
// 727 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(14,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2092 "parser.c"
        break;
      case 8:
// 731 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, 0, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2102 "parser.c"
        break;
      case 9:
// 735 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy13, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(17,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2113 "parser.c"
        break;
      case 10:
// 739 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, 1, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(17,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2124 "parser.c"
        break;
      case 11:
// 743 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy13, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-7].minor);
  yy_destructor(17,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2136 "parser.c"
        break;
      case 12:
// 747 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2146 "parser.c"
        break;
      case 13:
// 751 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(17,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2157 "parser.c"
        break;
      case 14:
// 755 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(17,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2168 "parser.c"
        break;
      case 15:
// 759 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(21,&yymsp[-6].minor);
  yy_destructor(17,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2180 "parser.c"
        break;
      case 16:
// 763 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(yymsp[0].minor.yy13, NULL, NULL, status->scanner_state);
}
// 2187 "parser.c"
        break;
      case 17:
// 767 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy13, status->scanner_state);
}
// 2194 "parser.c"
        break;
      case 18:
// 771 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(NULL, yymsp[0].minor.yy13, NULL, status->scanner_state);
}
// 2201 "parser.c"
        break;
      case 19:
// 775 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(yymsp[-1].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
}
// 2208 "parser.c"
        break;
      case 20:
// 779 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(yymsp[-1].minor.yy13, NULL, yymsp[0].minor.yy13, status->scanner_state);
}
// 2215 "parser.c"
        break;
      case 21:
// 783 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(yymsp[0].minor.yy13, NULL, yymsp[-1].minor.yy13, status->scanner_state);
}
// 2222 "parser.c"
        break;
      case 22:
// 787 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(NULL, yymsp[0].minor.yy13, yymsp[-1].minor.yy13, status->scanner_state);
}
// 2229 "parser.c"
        break;
      case 23:
// 791 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(yymsp[-2].minor.yy13, yymsp[0].minor.yy13, yymsp[-1].minor.yy13, status->scanner_state);
}
// 2236 "parser.c"
        break;
      case 24:
// 795 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_definition(yymsp[-1].minor.yy13, yymsp[0].minor.yy13, yymsp[-2].minor.yy13, status->scanner_state);
}
// 2243 "parser.c"
        break;
      case 27:
// 807 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2252 "parser.c"
        break;
      case 28:
// 811 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2261 "parser.c"
        break;
      case 29:
// 815 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2271 "parser.c"
        break;
      case 30:
// 819 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2281 "parser.c"
        break;
      case 31:
// 823 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2290 "parser.c"
        break;
      case 32:
// 827 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2299 "parser.c"
        break;
      case 33:
// 831 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2309 "parser.c"
        break;
      case 34:
// 835 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2319 "parser.c"
        break;
      case 38:
// 851 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2329 "parser.c"
        break;
      case 39:
// 855 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2339 "parser.c"
        break;
      case 41:
// 863 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2352 "parser.c"
        break;
      case 42:
// 867 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy13, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-7].minor);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2365 "parser.c"
        break;
      case 43:
// 871 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2378 "parser.c"
        break;
      case 44:
// 875 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy13, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-7].minor);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2391 "parser.c"
        break;
      case 45:
// 879 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy13, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-7].minor);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2404 "parser.c"
        break;
      case 46:
// 883 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy13, yymsp[-1].minor.yy13, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-8].minor);
  yy_destructor(27,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2417 "parser.c"
        break;
      case 47:
// 887 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy13, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-7].minor);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2430 "parser.c"
        break;
      case 48:
// 891 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy13, yymsp[-1].minor.yy13, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(23,&yymsp[-8].minor);
  yy_destructor(27,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2443 "parser.c"
        break;
      case 49:
// 895 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2456 "parser.c"
        break;
      case 50:
// 899 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2469 "parser.c"
        break;
      case 51:
// 903 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy13, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-7].minor);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2482 "parser.c"
        break;
      case 52:
// 907 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy13, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-7].minor);
  yy_destructor(27,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2495 "parser.c"
        break;
      case 53:
      case 97:
      case 106:
      case 124:
      case 151:
      case 155:
// 911 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_list(yymsp[-2].minor.yy13, yymsp[0].minor.yy13);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2508 "parser.c"
        break;
      case 55:
// 919 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2515 "parser.c"
        break;
      case 56:
// 923 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_parameter(yymsp[-1].minor.yy13, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2522 "parser.c"
        break;
      case 57:
// 927 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_parameter(NULL, yymsp[-1].minor.yy13, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2529 "parser.c"
        break;
      case 58:
// 931 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 2537 "parser.c"
        break;
      case 59:
// 935 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_parameter(yymsp[-3].minor.yy13, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 2545 "parser.c"
        break;
      case 60:
// 939 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_parameter(NULL, yymsp[-3].minor.yy13, yymsp[-2].minor.yy0, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 2553 "parser.c"
        break;
      case 61:
// 943 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2562 "parser.c"
        break;
      case 62:
// 947 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(30,&yymsp[0].minor);
}
// 2570 "parser.c"
        break;
      case 63:
// 951 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(31,&yymsp[0].minor);
}
// 2578 "parser.c"
        break;
      case 64:
// 955 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(32,&yymsp[0].minor);
}
// 2586 "parser.c"
        break;
      case 65:
// 959 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(33,&yymsp[0].minor);
}
// 2594 "parser.c"
        break;
      case 66:
// 963 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(34,&yymsp[0].minor);
}
// 2602 "parser.c"
        break;
      case 82:
// 1027 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2611 "parser.c"
        break;
      case 83:
// 1031 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2620 "parser.c"
        break;
      case 84:
// 1035 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_if_statement(yymsp[-2].minor.yy13, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2630 "parser.c"
        break;
      case 85:
// 1039 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_if_statement(yymsp[-5].minor.yy13, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2643 "parser.c"
        break;
      case 86:
// 1043 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_if_statement(yymsp[-3].minor.yy13, yymsp[-1].minor.yy13, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2653 "parser.c"
        break;
      case 87:
// 1047 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_if_statement(yymsp[-7].minor.yy13, yymsp[-5].minor.yy13, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(37,&yymsp[-8].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2666 "parser.c"
        break;
      case 88:
// 1051 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_if_statement(yymsp[-6].minor.yy13, yymsp[-4].minor.yy13, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(19,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2679 "parser.c"
        break;
      case 89:
// 1055 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_if_statement(yymsp[-6].minor.yy13, NULL, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2692 "parser.c"
        break;
      case 90:
// 1059 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2702 "parser.c"
        break;
      case 91:
// 1063 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_loop_statement(yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2712 "parser.c"
        break;
      case 92:
// 1067 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_while_statement(yymsp[-2].minor.yy13, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(18,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2722 "parser.c"
        break;
      case 93:
// 1071 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_while_statement(yymsp[-3].minor.yy13, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2732 "parser.c"
        break;
      case 94:
// 1075 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_for_statement(yymsp[-3].minor.yy13, NULL, yymsp[-5].minor.yy0, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2743 "parser.c"
        break;
      case 95:
// 1079 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_for_statement(yymsp[-3].minor.yy13, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(41,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2755 "parser.c"
        break;
      case 96:
// 1083 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_let_statement(yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2764 "parser.c"
        break;
      case 99:
// 1095 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 2772 "parser.c"
        break;
      case 100:
// 1099 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(24,&yymsp[-1].minor);
}
// 2781 "parser.c"
        break;
      case 101:
// 1103 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[-1].minor);
}
// 2791 "parser.c"
        break;
      case 102:
// 1107 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(24,&yymsp[-1].minor);
}
// 2801 "parser.c"
        break;
      case 103:
// 1111 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[0].minor);
}
// 2809 "parser.c"
        break;
      case 104:
// 1115 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[0].minor);
}
// 2817 "parser.c"
        break;
      case 105:
// 1119 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_echo_statement(yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2826 "parser.c"
        break;
      case 108:
// 1131 "parser.lemon"
{
	yygotominor.yy13 = yymsp[0].minor.yy13;;
}
// 2833 "parser.c"
        break;
      case 109:
      case 136:
      case 154:
      case 159:
      case 163:
      case 183:
// 1135 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 2845 "parser.c"
        break;
      case 110:
// 1140 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_return_statement(NULL, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2854 "parser.c"
        break;
      case 111:
// 1145 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_return_statement(NULL, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2863 "parser.c"
        break;
      case 112:
// 1150 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_return_statement(yymsp[-3].minor.yy0, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2873 "parser.c"
        break;
      case 113:
// 1155 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(51,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2887 "parser.c"
        break;
      case 114:
// 1164 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2902 "parser.c"
        break;
      case 115:
// 1173 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2917 "parser.c"
        break;
      case 116:
// 1182 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(51,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2932 "parser.c"
        break;
      case 117:
// 1191 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2941 "parser.c"
        break;
      case 118:
// 1196 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy13, status->scanner_state);
  yy_destructor(54,&yymsp[-6].minor);
  yy_destructor(12,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2953 "parser.c"
        break;
      case 119:
// 1200 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2962 "parser.c"
        break;
      case 120:
// 1204 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2971 "parser.c"
        break;
      case 121:
// 1208 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2980 "parser.c"
        break;
      case 122:
// 1212 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2989 "parser.c"
        break;
      case 123:
// 1216 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 2998 "parser.c"
        break;
      case 126:
// 1228 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3005 "parser.c"
        break;
      case 127:
// 1232 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3013 "parser.c"
        break;
      case 128:
// 1236 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-1].minor);
}
// 3021 "parser.c"
        break;
      case 129:
      case 186:
// 1241 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[0].minor);
}
// 3031 "parser.c"
        break;
      case 130:
      case 187:
// 1246 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[0].minor);
}
// 3041 "parser.c"
        break;
      case 131:
      case 188:
// 1251 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[0].minor);
}
// 3051 "parser.c"
        break;
      case 132:
// 1256 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("add", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3059 "parser.c"
        break;
      case 133:
// 1261 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("sub", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3067 "parser.c"
        break;
      case 134:
// 1266 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("instanceof", yymsp[0].minor.yy13, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3075 "parser.c"
        break;
      case 135:
      case 173:
// 1271 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("typeof", yymsp[0].minor.yy13, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3084 "parser.c"
        break;
      case 137:
      case 161:
      case 165:
// 1281 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3093 "parser.c"
        break;
      case 138:
      case 160:
      case 166:
// 1286 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3102 "parser.c"
        break;
      case 139:
      case 167:
// 1291 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3110 "parser.c"
        break;
      case 140:
      case 168:
// 1296 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(57,&yymsp[0].minor);
}
// 3119 "parser.c"
        break;
      case 141:
      case 169:
// 1301 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[0].minor);
}
// 3128 "parser.c"
        break;
      case 142:
// 1306 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
  yy_destructor(46,&yymsp[0].minor);
}
// 3137 "parser.c"
        break;
      case 143:
// 1311 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("array", yymsp[-1].minor.yy13, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(46,&yymsp[0].minor);
}
// 3146 "parser.c"
        break;
      case 144:
// 1316 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3154 "parser.c"
        break;
      case 145:
// 1321 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(28,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3164 "parser.c"
        break;
      case 146:
// 1326 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(12,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3174 "parser.c"
        break;
      case 147:
      case 184:
// 1331 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3184 "parser.c"
        break;
      case 148:
      case 185:
// 1336 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3194 "parser.c"
        break;
      case 149:
// 1341 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy13, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3204 "parser.c"
        break;
      case 150:
// 1346 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(28,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3214 "parser.c"
        break;
      case 157:
// 1374 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_array_item(yymsp[-2].minor.yy13, yymsp[0].minor.yy13, status->scanner_state);
  yy_destructor(59,&yymsp[-1].minor);
}
// 3222 "parser.c"
        break;
      case 158:
// 1378 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_array_item(NULL, yymsp[0].minor.yy13, status->scanner_state);
}
// 3229 "parser.c"
        break;
      case 164:
      case 170:
// 1402 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[0].minor);
}
// 3238 "parser.c"
        break;
      case 172:
// 1434 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("not", yymsp[0].minor.yy13, NULL, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3246 "parser.c"
        break;
      case 174:
// 1442 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("isset", yymsp[0].minor.yy13, NULL, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-1].minor);
}
// 3254 "parser.c"
        break;
      case 175:
// 1446 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("instanceof", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3262 "parser.c"
        break;
      case 176:
// 1450 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("equals", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3270 "parser.c"
        break;
      case 177:
// 1454 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("not-equals", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3278 "parser.c"
        break;
      case 178:
// 1458 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("identical", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3286 "parser.c"
        break;
      case 179:
// 1462 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("not-identical", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3294 "parser.c"
        break;
      case 180:
// 1466 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("less", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3302 "parser.c"
        break;
      case 181:
// 1470 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_expr("greater", yymsp[-2].minor.yy13, yymsp[0].minor.yy13, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3310 "parser.c"
        break;
      case 182:
// 1474 "parser.lemon"
{
	yygotominor.yy13 = yymsp[-1].minor.yy13;
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3319 "parser.c"
        break;
      case 189:
// 1506 "parser.lemon"
{
	yygotominor.yy13 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3326 "parser.c"
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
// 654 "parser.lemon"


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

// 3402 "parser.c"
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
