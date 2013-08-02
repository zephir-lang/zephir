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
#define YYNSTATE 413
#define YYNRULE 192
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
 /*     0 */   332,  284,  228,  225,  606,    1,    2,  412,    4,    5,
 /*    10 */     6,  204,  213,  219,  216,  105,   42,   44,   46,  281,
 /*    20 */    55,   59,   64,  230,   71,  175,  170,  176,  174,  309,
 /*    30 */   169,  177,  180,  148,  283,  195,  222,  225,  344,   17,
 /*    40 */   359,  120,  385,  358,   80,  204,  213,  219,  216,  148,
 /*    50 */    42,   44,   46,   77,   55,   59,   64,  308,   71,  175,
 /*    60 */   135,  173,  174,  479,  169,  177,  180,  148,  271,  195,
 /*    70 */   222,  225,  228,  150,  158,  165,  166,  167,   80,  204,
 /*    80 */   213,  219,  216,  148,   42,   44,   46,  478,   55,   59,
 /*    90 */    64,  128,   71,  262,  141,  132,  137,   18,  169,  177,
 /*   100 */   180,  172,  270,  195,  222,  225,  387,  100,  390,  391,
 /*   110 */   194,  346,  358,  204,  213,  219,  216,  171,   42,   44,
 /*   120 */    46,  103,   55,   59,   64,  434,   71,  477,  389,  373,
 /*   130 */   228,  379,  169,  177,  180,  115,   58,  195,  222,  225,
 /*   140 */   349,   79,  111,  113,  102,  104,  476,  204,  213,  219,
 /*   150 */   216,  233,   42,   44,   46,  351,   55,   59,   64,  437,
 /*   160 */    71,  157,  310,   19,   80,  335,  169,  177,  180,  148,
 /*   170 */    70,  195,  222,  225,  435,  388,  359,  310,   19,  358,
 /*   180 */   335,  204,  213,  219,  216,  350,   42,   44,   46,  475,
 /*   190 */    55,   59,   64,  106,   71,  474,    3,    4,    5,    6,
 /*   200 */   169,  177,  180,   87,   25,  195,  222,  225,  432,   82,
 /*   210 */    99,  310,   19,  352,  335,  204,  213,  219,  216,  143,
 /*   220 */    42,   44,   46,  197,   55,   59,   64,  431,   71,  239,
 /*   230 */   310,   19,   86,  335,  169,  177,  180,  522,  317,  195,
 /*   240 */   222,  225,  280,  299,  305,  289,  293,  354,   89,  204,
 /*   250 */   213,  219,  216,  522,   42,   44,   46,  228,   55,   59,
 /*   260 */    64,  306,   71,  305,  289,  293,  241,  243,  169,  177,
 /*   270 */   180,   87,  341,  195,  222,  225,  436,  135,  245,  310,
 /*   280 */    19,   88,  335,  204,  213,  219,  216,  298,   42,   44,
 /*   290 */    46,  355,   55,   59,   64,  320,   71,  305,  289,  293,
 /*   300 */   147,   89,  169,  177,  180,   74,   27,  195,  222,  225,
 /*   310 */    87,  131,  132,  137,  182,  185,   89,  204,  213,  219,
 /*   320 */   216,   73,   42,   44,   46,  228,   55,   59,   64,  107,
 /*   330 */    71,  285,  289,  293,   98,   89,  169,  177,  180,  260,
 /*   340 */   343,  195,  222,  225,  146,  311,  247,  326,  348,   98,
 /*   350 */    89,  204,  213,  219,  216,  296,   42,   44,   46,  228,
 /*   360 */    55,   59,   64,   85,   71,  186,  101,   12,   98,  393,
 /*   370 */   169,  177,  180,  207,   54,  195,  222,  225,  201,  361,
 /*   380 */    47,  367,  348,   98,   89,  204,  213,  219,  216,  206,
 /*   390 */    42,   44,   46,  264,   55,   59,   64,  345,   71,  363,
 /*   400 */   346,  358,  189,  192,  169,  177,  180,  364,  319,  195,
 /*   410 */   222,  225,  259,   79,  111,  113,  152,   98,  356,  204,
 /*   420 */   213,  219,  216,  369,   42,   44,   46,  228,   55,   59,
 /*   430 */    64,  370,   71,   78,  162,   66,   80,  154,  169,  177,
 /*   440 */   180,  148,  273,  195,  222,  225,  228,  228,  249,  228,
 /*   450 */   311,  284,  326,  204,  213,  219,  216,  362,   42,   44,
 /*   460 */    46,  228,   55,   59,   64,  159,   71,  251,  253,  312,
 /*   470 */   255,  357,  169,  177,  180,  199,  278,  195,  222,  225,
 /*   480 */   307,  198,  267,  161,  196,   87,   80,  204,  213,  219,
 /*   490 */   216,  148,   42,   44,   46,  295,   55,   59,   64,  228,
 /*   500 */    71,  153,   72,  168,   80,  294,  169,  177,  180,  148,
 /*   510 */    50,  195,  222,  225,  108,  228,  205,  212,  214,  212,
 /*   520 */    60,  204,  213,  219,  216,  368,   42,   44,   46,  200,
 /*   530 */    55,   59,   64,  207,   71,  291,   67,  327,   87,  374,
 /*   540 */   169,  177,  180,  284,   63,  195,  222,  225,  292,  215,
 /*   550 */   361,   20,  367,  217,  212,  204,  213,  219,  216,  288,
 /*   560 */    42,   44,   46,  375,   55,   59,   64,  202,   71,  381,
 /*   570 */   130,  376,  321,  380,  169,  177,  180,  382,  325,  195,
 /*   580 */   222,  225,  220,  212,  402,  336,  406,  207,  290,  204,
 /*   590 */   213,  219,  216,  401,   42,   44,   46,  228,   55,   59,
 /*   600 */    64,  207,   71,  218,  223,  212,  207,  209,  169,  177,
 /*   610 */   180,  203,  274,  195,  222,  225,  129,  221,  263,  135,
 /*   620 */   164,  287,  224,  204,  213,  219,  216,    8,   42,   44,
 /*   630 */    46,   76,   55,   59,   64,  365,   71,  366,  576,  404,
 /*   640 */   242,   83,  169,  177,  180,  240,  279,  195,  222,  225,
 /*   650 */   163,   56,  160,  276,  372,  134,  371,  204,  213,  219,
 /*   660 */   216,  208,   42,   44,   46,   75,   55,   59,   64,  133,
 /*   670 */    71,  210,  575,   11,   22,  275,  169,  177,  180,  378,
 /*   680 */   272,  195,  222,  225,  377,   43,  211,  238,  156,  155,
 /*   690 */   407,  204,  213,  219,  216,  266,   42,   44,   46,   84,
 /*   700 */    55,   59,   64,  265,   71,  383,  384,  408,  227,   45,
 /*   710 */   169,  177,  180,  413,  334,  195,  222,  225,  151,   21,
 /*   720 */    52,   51,  258,  394,  574,  204,  213,  219,  216,  236,
 /*   730 */    42,   44,   46,   20,   55,   59,   64,  410,   71,  395,
 /*   740 */   237,  397,  244,  144,  169,  177,  180,  400,  323,  195,
 /*   750 */   222,  225,  330,  331,  329,  187,   14,  328,  327,  204,
 /*   760 */   213,  219,  216,  184,   42,   44,   46,  188,   55,   59,
 /*   770 */    64,  183,   71,  336,   65,  337,  338,    9,  169,  177,
 /*   780 */   180,  339,  191,  195,  222,  225,  190,  322,  340,  181,
 /*   790 */   316,  179,  313,  204,  213,  219,  216,   90,   42,   44,
 /*   800 */    46,  314,   55,   59,   64,   41,   71,  312,  193,  308,
 /*   810 */   308,   97,  169,  177,  180,   49,  308,  195,  222,  226,
 /*   820 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   830 */    39,   40,  308,   41,   91,   92,  308,  308,   93,   94,
 /*   840 */    95,  308,   96,  277,  308,  308,  308,  226,   29,   30,
 /*   850 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   860 */   308,  308,  308,  308,   41,  308,  308,  231,  246,  250,
 /*   870 */   252,  248,  234,  308,  269,  308,  227,  308,  226,   29,
 /*   880 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   890 */    40,   41,  297,  308,  308,  256,  403,   16,  386,  392,
 /*   900 */   391,  324,  385,  358,  286,  226,   29,   30,   31,   32,
 /*   910 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  300,
 /*   920 */   301,  302,  303,  304,  308,  308,  308,  308,   53,  308,
 /*   930 */   308,  308,  226,   29,   30,   31,   32,   33,   34,   35,
 /*   940 */    36,   37,   38,   39,   40,   41,  308,  308,  308,  308,
 /*   950 */    13,   16,  386,  392,  391,   26,  385,  358,  308,  226,
 /*   960 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   970 */    39,   40,   41,  308,  396,   16,  386,  392,  391,  308,
 /*   980 */   385,  358,   69,  308,  308,  308,  226,   29,   30,   31,
 /*   990 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1000 */   308,  308,  308,  308,  409,   16,  386,  392,  391,  333,
 /*  1010 */   385,  358,  308,  226,   29,   30,   31,   32,   33,   34,
 /*  1020 */    35,   36,   37,   38,   39,   40,   41,  308,  308,   91,
 /*  1030 */    92,  308,  308,   93,   94,   95,   62,   96,  308,  308,
 /*  1040 */   226,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1050 */    38,   39,   40,   41,  411,  308,  308,  360,  373,  308,
 /*  1060 */   379,  353,  308,  342,  308,  308,  308,  226,   29,   30,
 /*  1070 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1080 */    41,  398,  308,  308,  360,  373,  308,  379,  353,  308,
 /*  1090 */   318,  308,  308,  308,  226,   29,   30,   31,   32,   33,
 /*  1100 */    34,   35,   36,   37,   38,   39,   40,   41,  433,  308,
 /*  1110 */   308,  347,   19,  308,  335,  353,  308,   57,  308,  308,
 /*  1120 */   308,  226,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1130 */    37,   38,   39,   40,   41,  112,  308,  308,   80,  308,
 /*  1140 */   119,  117,  142,  148,  282,   81,  308,   41,  226,   29,
 /*  1150 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1160 */    40,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1170 */    37,   38,   39,   40,  232,  126,  308,  229,  308,  257,
 /*  1180 */   308,  308,  121,  122,  297,  308,  123,  124,  125,   90,
 /*  1190 */   149,   15,  254,  308,  360,  373,  286,  379,  353,    7,
 /*  1200 */   308,  114,   10,  145,   80,   90,  399,  225,  308,  148,
 /*  1210 */   315,  300,  301,  302,  303,  304,   91,   92,  308,  109,
 /*  1220 */    93,   94,   95,  308,   96,  235,   91,   92,  308,  308,
 /*  1230 */    93,   94,   95,  308,   96,  231,  246,  250,  252,  248,
 /*  1240 */   234,  297,   91,   92,  227,  308,   93,   94,   95,  308,
 /*  1250 */    96,   90,   61,  286,  110,  118,   24,   80,   80,  308,
 /*  1260 */   138,  308,  148,  148,  308,  261,  308,  308,  300,  301,
 /*  1270 */   302,  303,  304,  231,  246,  250,  252,  248,  234,  308,
 /*  1280 */   308,  308,  227,  308,  308,  308,  308,  308,   91,   92,
 /*  1290 */    68,  127,   93,   94,   95,  308,   96,  140,  139,  138,
 /*  1300 */   308,   93,   94,   95,  308,   96,  231,  246,  250,  252,
 /*  1310 */   248,  234,  308,  308,  175,  227,  136,  178,  308,  308,
 /*  1320 */   297,  308,  148,   48,  308,   90,  231,  246,  250,  252,
 /*  1330 */   248,  234,  286,  308,  308,  227,  140,  139,  308,  308,
 /*  1340 */    93,   94,   95,  268,   96,  308,   23,  300,  301,  302,
 /*  1350 */   303,  304,  308,   91,   92,  308,  308,   93,   94,   95,
 /*  1360 */   414,   96,   91,   92,  308,  308,   93,   94,   95,  116,
 /*  1370 */    96,  308,   80,  308,  308,    7,  308,  148,   10,  308,
 /*  1380 */   308,  430,  399,  225,  360,  373,  308,  379,  353,  405,
 /*  1390 */   308,  308,  360,  373,  308,  379,  353,  308,  308,  429,
 /*  1400 */   308,  308,  360,  373,  308,  379,  353,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    20,    1,   76,   23,   64,   65,   66,   67,   68,   69,
 /*    10 */    70,   31,   32,   33,   34,   47,   36,   37,   38,   19,
 /*    20 */    40,   41,   42,   97,   44,  100,  101,  102,  103,   20,
 /*    30 */    50,   51,   52,  108,   20,   55,   56,   23,   73,   74,
 /*    40 */    75,  100,   77,   78,  103,   31,   32,   33,   34,  108,
 /*    50 */    36,   37,   38,   25,   40,   41,   42,   19,   44,  100,
 /*    60 */    76,  102,  103,   16,   50,   51,   52,  108,   20,   55,
 /*    70 */    56,   23,   76,   45,   46,  100,   48,   49,  103,   31,
 /*    80 */    32,   33,   34,  108,   36,   37,   38,   16,   40,   41,
 /*    90 */    42,  107,   44,   97,  110,  111,  112,   78,   50,   51,
 /*   100 */    52,    1,   20,   55,   56,   23,   72,   16,   74,   75,
 /*   110 */    17,   77,   78,   31,   32,   33,   34,   17,   36,   37,
 /*   120 */    38,   47,   40,   41,   42,   20,   44,   16,   23,   24,
 /*   130 */    76,   26,   50,   51,   52,    2,   20,   55,   56,   23,
 /*   140 */    16,    8,    9,   10,   53,   54,   16,   31,   32,   33,
 /*   150 */    34,   97,   36,   37,   38,   76,   40,   41,   42,   20,
 /*   160 */    44,  100,   23,   24,  103,   26,   50,   51,   52,  108,
 /*   170 */    20,   55,   56,   23,   20,   74,   75,   23,   24,   78,
 /*   180 */    26,   31,   32,   33,   34,   25,   36,   37,   38,   16,
 /*   190 */    40,   41,   42,   29,   44,   16,   67,   68,   69,   70,
 /*   200 */    50,   51,   52,    1,   20,   55,   56,   23,   20,   45,
 /*   210 */    46,   23,   24,   17,   26,   31,   32,   33,   34,   16,
 /*   220 */    36,   37,   38,   17,   40,   41,   42,   20,   44,   16,
 /*   230 */    23,   24,   30,   26,   50,   51,   52,    1,   20,   55,
 /*   240 */    56,   23,   79,    5,   81,   82,   83,   16,   76,   31,
 /*   250 */    32,   33,   34,   17,   36,   37,   38,   76,   40,   41,
 /*   260 */    42,   79,   44,   81,   82,   83,   53,   54,   50,   51,
 /*   270 */    52,    1,   20,   55,   56,   23,   20,   76,   97,   23,
 /*   280 */    24,  109,   26,   31,   32,   33,   34,   16,   36,   37,
 /*   290 */    38,   25,   40,   41,   42,   79,   44,   81,   82,   83,
 /*   300 */    30,   76,   50,   51,   52,    1,   20,   55,   56,   23,
 /*   310 */     1,  110,  111,  112,   45,   46,   76,   31,   32,   33,
 /*   320 */    34,   17,   36,   37,   38,   76,   40,   41,   42,  104,
 /*   330 */    44,   81,   82,   83,  109,   76,   50,   51,   52,   30,
 /*   340 */    20,   55,   56,   23,  104,   24,   97,   26,   27,  109,
 /*   350 */    76,   31,   32,   33,   34,   76,   36,   37,   38,   76,
 /*   360 */    40,   41,   42,  104,   44,   16,   47,   19,  109,   21,
 /*   370 */    50,   51,   52,    1,   20,   55,   56,   23,  104,   24,
 /*   380 */    97,   26,   27,  109,   76,   31,   32,   33,   34,   17,
 /*   390 */    36,   37,   38,    1,   40,   41,   42,   74,   44,   17,
 /*   400 */    77,   78,   53,   54,   50,   51,   52,   25,   20,   55,
 /*   410 */    56,   23,  104,    8,    9,   10,   25,  109,   76,   31,
 /*   420 */    32,   33,   34,   17,   36,   37,   38,   76,   40,   41,
 /*   430 */    42,   25,   44,  100,   16,   43,  103,   46,   50,   51,
 /*   440 */    52,  108,   20,   55,   56,   23,   76,   76,   97,   76,
 /*   450 */    24,    1,   26,   31,   32,   33,   34,   16,   36,   37,
 /*   460 */    38,   76,   40,   41,   42,   47,   44,   97,   97,   28,
 /*   470 */    97,   17,   50,   51,   52,   16,   20,   55,   56,   23,
 /*   480 */    30,   13,   97,  100,   16,    1,  103,   31,   32,   33,
 /*   490 */    34,  108,   36,   37,   38,   25,   40,   41,   42,   76,
 /*   500 */    44,  100,   98,   99,  103,   16,   50,   51,   52,  108,
 /*   510 */    20,   55,   56,   23,   30,   76,  105,  106,  105,  106,
 /*   520 */    97,   31,   32,   33,   34,   16,   36,   37,   38,   29,
 /*   530 */    40,   41,   42,    1,   44,   25,   97,   28,    1,   16,
 /*   540 */    50,   51,   52,    1,   20,   55,   56,   23,   76,   17,
 /*   550 */    24,   28,   26,  105,  106,   31,   32,   33,   34,   76,
 /*   560 */    36,   37,   38,   17,   40,   41,   42,   30,   44,   17,
 /*   570 */     1,   25,   30,   16,   50,   51,   52,   25,   20,   55,
 /*   580 */    56,   23,  105,  106,   19,   28,   21,    1,   16,   31,
 /*   590 */    32,   33,   34,   16,   36,   37,   38,   76,   40,   41,
 /*   600 */    42,    1,   44,   17,  105,  106,    1,   16,   50,   51,
 /*   610 */    52,   17,   20,   55,   56,   23,   47,   17,   97,   76,
 /*   620 */    25,   25,   17,   31,   32,   33,   34,   16,   36,   37,
 /*   630 */    38,   16,   40,   41,   42,   76,   44,   17,   60,   20,
 /*   640 */    47,   16,   50,   51,   52,   47,   20,   55,   56,   23,
 /*   650 */    47,   19,   25,   19,   17,  112,   76,   31,   32,   33,
 /*   660 */    34,  106,   36,   37,   38,   99,   40,   41,   42,   60,
 /*   670 */    44,   25,   60,   16,   29,   39,   50,   51,   52,   17,
 /*   680 */    20,   55,   56,   23,   76,   17,   76,   46,   25,   47,
 /*   690 */    16,   31,   32,   33,   34,   43,   36,   37,   38,   29,
 /*   700 */    40,   41,   42,   16,   44,   76,   17,   19,   11,   17,
 /*   710 */    50,   51,   52,    0,   20,   55,   56,   23,   16,   16,
 /*   720 */    19,   39,   29,   16,   60,   31,   32,   33,   34,  113,
 /*   730 */    36,   37,   38,   28,   40,   41,   42,   20,   44,   19,
 /*   740 */    16,   20,   47,   29,   50,   51,   52,   18,   20,   55,
 /*   750 */    56,   23,   30,   19,   29,   47,   20,   16,   28,   31,
 /*   760 */    32,   33,   34,   17,   36,   37,   38,   17,   40,   41,
 /*   770 */    42,   16,   44,   28,   16,   16,   29,   17,   50,   51,
 /*   780 */    52,   30,   17,   55,   56,   23,   47,   19,   19,   16,
 /*   790 */    19,   17,   16,   31,   32,   33,   34,   16,   36,   37,
 /*   800 */    38,   29,   40,   41,   42,   70,   44,   28,   47,  114,
 /*   810 */   114,   30,   50,   51,   52,   80,  114,   55,   56,   84,
 /*   820 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   830 */    95,   96,  114,   70,   53,   54,  114,  114,   57,   58,
 /*   840 */    59,  114,   61,   80,  114,  114,  114,   84,   85,   86,
 /*   850 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*   860 */   114,  114,  114,  114,   70,  114,  114,    2,    3,    4,
 /*   870 */     5,    6,    7,  114,   80,  114,   11,  114,   84,   85,
 /*   880 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*   890 */    96,   70,    4,  114,  114,   30,   71,   72,   73,   74,
 /*   900 */    75,   80,   77,   78,   16,   84,   85,   86,   87,   88,
 /*   910 */    89,   90,   91,   92,   93,   94,   95,   96,   70,   31,
 /*   920 */    32,   33,   34,   35,  114,  114,  114,  114,   80,  114,
 /*   930 */   114,  114,   84,   85,   86,   87,   88,   89,   90,   91,
 /*   940 */    92,   93,   94,   95,   96,   70,  114,  114,  114,  114,
 /*   950 */    71,   72,   73,   74,   75,   80,   77,   78,  114,   84,
 /*   960 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   970 */    95,   96,   70,  114,   71,   72,   73,   74,   75,  114,
 /*   980 */    77,   78,   80,  114,  114,  114,   84,   85,   86,   87,
 /*   990 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   70,
 /*  1000 */   114,  114,  114,  114,   71,   72,   73,   74,   75,   80,
 /*  1010 */    77,   78,  114,   84,   85,   86,   87,   88,   89,   90,
 /*  1020 */    91,   92,   93,   94,   95,   96,   70,  114,  114,   53,
 /*  1030 */    54,  114,  114,   57,   58,   59,   80,   61,  114,  114,
 /*  1040 */    84,   85,   86,   87,   88,   89,   90,   91,   92,   93,
 /*  1050 */    94,   95,   96,   70,   20,  114,  114,   23,   24,  114,
 /*  1060 */    26,   27,  114,   80,  114,  114,  114,   84,   85,   86,
 /*  1070 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*  1080 */    70,   20,  114,  114,   23,   24,  114,   26,   27,  114,
 /*  1090 */    80,  114,  114,  114,   84,   85,   86,   87,   88,   89,
 /*  1100 */    90,   91,   92,   93,   94,   95,   96,   70,   20,  114,
 /*  1110 */   114,   23,   24,  114,   26,   27,  114,   80,  114,  114,
 /*  1120 */   114,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*  1130 */    93,   94,   95,   96,   70,  100,  114,  114,  103,  114,
 /*  1140 */    11,   12,   13,  108,   80,   16,  114,   70,   84,   85,
 /*  1150 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*  1160 */    96,   84,   85,   86,   87,   88,   89,   90,   91,   92,
 /*  1170 */    93,   94,   95,   96,   11,   46,  114,   14,  114,   16,
 /*  1180 */   114,  114,   53,   54,    4,  114,   57,   58,   59,   16,
 /*  1190 */    61,   20,   29,  114,   23,   24,   16,   26,   27,   15,
 /*  1200 */   114,  100,   18,   30,  103,   16,   22,   23,  114,  108,
 /*  1210 */    30,   31,   32,   33,   34,   35,   53,   54,  114,   30,
 /*  1220 */    57,   58,   59,  114,   61,   62,   53,   54,  114,  114,
 /*  1230 */    57,   58,   59,  114,   61,    2,    3,    4,    5,    6,
 /*  1240 */     7,    4,   53,   54,   11,  114,   57,   58,   59,  114,
 /*  1250 */    61,   16,   19,   16,  100,  100,   19,  103,  103,  114,
 /*  1260 */    16,  114,  108,  108,  114,   30,  114,  114,   31,   32,
 /*  1270 */    33,   34,   35,    2,    3,    4,    5,    6,    7,  114,
 /*  1280 */   114,  114,   11,  114,  114,  114,  114,  114,   53,   54,
 /*  1290 */    19,   47,   57,   58,   59,  114,   61,   53,   54,   16,
 /*  1300 */   114,   57,   58,   59,  114,   61,    2,    3,    4,    5,
 /*  1310 */     6,    7,  114,  114,  100,   11,   16,  103,  114,  114,
 /*  1320 */     4,  114,  108,   19,  114,   16,    2,    3,    4,    5,
 /*  1330 */     6,    7,   16,  114,  114,   11,   53,   54,  114,  114,
 /*  1340 */    57,   58,   59,   19,   61,  114,   30,   31,   32,   33,
 /*  1350 */    34,   35,  114,   53,   54,  114,  114,   57,   58,   59,
 /*  1360 */     0,   61,   53,   54,  114,  114,   57,   58,   59,  100,
 /*  1370 */    61,  114,  103,  114,  114,   15,  114,  108,   18,  114,
 /*  1380 */   114,   20,   22,   23,   23,   24,  114,   26,   27,   20,
 /*  1390 */   114,  114,   23,   24,  114,   26,   27,  114,  114,   20,
 /*  1400 */   114,  114,   23,   24,  114,   26,   27,
};
#define YY_SHIFT_USE_DFLT (-33)
static short yy_shift_ofst[] = {
 /*     0 */  1184,  713, 1360,  -33,  -33,  -33,  -33,  611,  760,  -33,
 /*    10 */   657,  348, 1171,  736,  -33,  -33, 1379,  188,  -33,  705,
 /*    20 */   703,  645, 1316, 1237,  184,  -33,  286,  -33,  -33,  -33,
 /*    30 */   -33,  -33,  -33,  -33,  -33,  -33,  -33,  -33,  -33,  -33,
 /*    40 */   -33,  -33,  668,  -33,  692,  -33, 1163, 1304,  592,  490,
 /*    50 */   682,  701,  422,  354,  -33,  632,  660,  116,  -33, 1163,
 /*    60 */  1233,   48,  524,  -33,  758,  392, 1163, 1271,  762,  150,
 /*    70 */   -33,  615,  304,  -33,  615,  -33,   28, 1129,  133, 1129,
 /*    80 */   -33,  164,  625,  670,  781,  202,  -33, 1309,  -33,  -33,
 /*    90 */   -33,  -33,  -33,  -33,  -33,  -33,  -33,  -33,  -33,   91,
 /*   100 */   319,  -33,   74,  -33,  -32,  -33, 1189,  484,  -33,  -33,
 /*   110 */   -33, 1129,  -33, 1129,  -33, 1129,  405, 1129,  -33, 1129,
 /*   120 */   -33,  -33,  -33,  -33,  -33,  -33, 1244,  -33,  569,  -33,
 /*   130 */  1283,  -33,  609, 1300,  -33,  -33,  -33,  -33,  664,  612,
 /*   140 */   578,  -33,  203,  714, 1173,  -33,  270,  -33,  -33,  -33,
 /*   150 */   702,  391, 1129,  133,  642,  663, 1129,  133,  418,  627,
 /*   160 */  1129,  133,  603,  595, 1129,  133,  -33,  -33,  -33, 1129,
 /*   170 */   100,  -33, 1129,  -33,  236,  133,  -33, 1129,  774,  -33,
 /*   180 */   773,  269,  755,  746,  -33,  349,  708,  750,  -33,  739,
 /*   190 */   765,  -33,  761,   93,  -33,  468,  206,  -33,  459,  500,
 /*   200 */  1309,  537,  594,  -33,  591,  372,  -33,  591,  -33,  646,
 /*   210 */   976,  -33,  -33,  591,  532,  -33,  591,  586,  -33,  591,
 /*   220 */   600,  -33,  591,  605,  -33,  -33,  -33, 1163,  -33, 1163,
 /*   230 */   -33, 1163, 1163,  697, 1163,  724,  -33,  641,  213,  598,
 /*   240 */   -33,  593,  -33,  695,  -33,  697, 1163,  697, 1163,  697,
 /*   250 */  1163,  697, 1163,  697, 1163,  865,  -33,  693, 1235,  309,
 /*   260 */   -33,  -33,  697,  697,  687,  652, 1163, 1324,  762,   82,
 /*   270 */   -33,  -33,  -33,  -33,  636,  634,  626,  456,  -33,  -33,
 /*   280 */     0,  762,   14,  -33,  888,  -33,  596,  976,  -33,  572,
 /*   290 */   510,  976,  -33,  489,  470,  976,  -33,  271,  238,  179,
 /*   300 */   173,  130,  111,   71,   47,  -33,  450,   38,    9,  -33,
 /*   310 */   426,  779,  776,  772, 1180,  771,  218,  -33,  388,  -33,
 /*   320 */   542,  768,  728,  -33,  558,  -33,  730,  741,  725,  722,
 /*   330 */   734,  -20,  -33,  694,  -33,  745,  759,  747,  751,  769,
 /*   340 */   252,  -33,  320,  -33, 1088,  256,  -33,  321,  124,  160,
 /*   350 */   976,  196,  -33,  231,  266,  976,  454,  -33,  -33,  -33,
 /*   360 */   355,  441,  382,  -33,  976,  620,  -33,  509,  406,  -33,
 /*   370 */   976,  637,  -33,  523,  546,  -33,  976,  662,  -33,  557,
 /*   380 */   552,  -33,  976,  689,  -33,  -33, 1361,  105,  139,  526,
 /*   390 */   154,  -33,  207,  707,  720, 1061,  721,  -33,  -33,  729,
 /*   400 */   577,  565, 1369,  619,  -33,  -33,  674,  688, 1034,  717,
 /*   410 */   -33,  -33,  -33,
};
#define YY_REDUCE_USE_DFLT (-76)
static short yy_reduce_ofst[] = {
 /*     0 */   -60,  -76,  129,  -76,  -76,  -76,  -76,  -76,  -76,  -76,
 /*    10 */   -76,  -76,  879,  -76,  -76,  -76,  -35,   19,  -76,  -76,
 /*    20 */   -76,  -76,  182,  163,  875,  -76, 1077,  -76,  -76,  -76,
 /*    30 */   -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,
 /*    40 */   -76,  -76,  -76,  -76,  -76,  -76,  283,  -76,  735, 1077,
 /*    50 */   -76,  -76,  848, 1077,  -76,  -76, 1037, 1077,  -76,  423,
 /*    60 */   -76,  956, 1077,  -76,  -76,  -76,  439,  -76,  902, 1077,
 /*    70 */   -76,  404,  -76,  -76,  566,  -76,  -76,  333,  -76, 1154,
 /*    80 */   -76,  -76,  -76,  -76,  259,  -76,  -76,  172,  -76,  -76,
 /*    90 */   -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,
 /*   100 */   -76,  -76,  -76,  -76,  -76,  -76,  225,  -76,  -76,  -76,
 /*   110 */   -76, 1035,  -76, 1101,  -76, 1269,  -76, 1155,  -76,  -59,
 /*   120 */   -76,  -76,  -76,  -76,  -76,  -76,  -16,  -76,  -76,  -76,
 /*   130 */   201,  -76,  -76,  543,  -76,  -76,  -76,  -76,  -76,  -76,
 /*   140 */   -76,  -76,  -76,  -76,  240,  -76,  -76,  -76,  -76,  -76,
 /*   150 */   -76,  -76,  401,  -76,  -76,  -76,   61,  -76,  -76,  -76,
 /*   160 */   383,  -76,  -76,  -76,  -25,  -76,  -76,  -76,  -76,  -75,
 /*   170 */   -76,  -76,  -41,  -76,  -76,  -76,  -76, 1214,  -76,  -76,
 /*   180 */   -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,
 /*   190 */   -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,
 /*   200 */   274,  -76,  -76,  -76,  411,  -76,  -76,  555,  -76,  -76,
 /*   210 */   610,  -76,  -76,  413,  -76,  -76,  448,  -76,  -76,  477,
 /*   220 */   -76,  -76,  499,  -76,  -76,  -76,  -76,  521,  -76,  -74,
 /*   230 */   -76,   -4,   54,  -76,  181,  616,  -76,  -76,  -76,  -76,
 /*   240 */   -76,  -76,  -76,  -76,  -76,  -76,  249,  -76,  351,  -76,
 /*   250 */   370,  -76,  371,  -76,  373,  -76,  -76,  -76,  308,  -76,
 /*   260 */   -76,  -76,  -76,  -76,  -76,  -76,  385,  -76,  794, 1077,
 /*   270 */   -76,  -76,  -76,  -76,  -76,  -76,  763, 1077,  -76,  -76,
 /*   280 */   -76, 1064, 1077,  -76,  250,  -76,  -76,  483,  -76,  -76,
 /*   290 */   -76,  472,  -76,  -76,  -76,  279,  -76,  -76,  -76,  -76,
 /*   300 */   -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,  -76,
 /*   310 */   -76,  -76,  -76,  -76,  216,  -76, 1010,  -76, 1077,  -76,
 /*   320 */   -76,  -76,  821,  -76, 1077,  -76,  -76,  -76,  -76,  -76,
 /*   330 */   -76,  929,  -76, 1077,  -76,  -76,  -76,  -76,  -76,  -76,
 /*   340 */   983,  -76, 1077,  -76,  323,   19,  -76,  -76,  -76,  -76,
 /*   350 */    79,  -76,  -76,  -76,  -76,  342,  -76,  -76,  -76,  -76,
 /*   360 */   -76,  -76,  -76,  -76,  559,  -76,  -76,  -76,  -76,  -76,
 /*   370 */   580,  -76,  -76,  -76,  -76,  -76,  608,  -76,  -76,  -76,
 /*   380 */   -76,  -76,  629,  -76,  -76,  -76,   34,  101,   19,  -76,
 /*   390 */    19,  -76,   19,  -76,  -76,  903,  -76,  -76,  -76,  -76,
 /*   400 */   -76,  -76,  825,  -76,  -76,  -76,  -76,  -76,  933,  -76,
 /*   410 */   -76,  -76,  -76,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   605,  605,  605,  415,  417,  418,  419,  605,  605,  420,
 /*    10 */   605,  605,  605,  605,  421,  425,  605,  605,  450,  605,
 /*    20 */   605,  605,  605,  605,  605,  454,  605,  458,  480,  482,
 /*    30 */   483,  484,  485,  486,  487,  488,  489,  490,  491,  492,
 /*    40 */   493,  494,  605,  495,  605,  496,  605,  605,  605,  605,
 /*    50 */   499,  605,  605,  605,  500,  605,  605,  605,  504,  605,
 /*    60 */   605,  605,  605,  506,  605,  605,  605,  605,  605,  605,
 /*    70 */   507,  605,  605,  509,  605,  510,  605,  605,  512,  605,
 /*    80 */   539,  550,  605,  540,  605,  605,  564,  605,  566,  568,
 /*    90 */   569,  580,  581,  582,  583,  584,  585,  565,  567,  605,
 /*   100 */   605,  541,  605,  542,  605,  543,  605,  605,  561,  562,
 /*   110 */   544,  605,  545,  605,  546,  605,  547,  605,  548,  605,
 /*   120 */   549,  551,  552,  553,  554,  555,  605,  556,  605,  557,
 /*   130 */   605,  570,  605,  605,  572,  577,  578,  573,  578,  581,
 /*   140 */   580,  571,  605,  558,  605,  559,  605,  560,  563,  579,
 /*   150 */   605,  605,  605,  513,  605,  605,  605,  514,  605,  605,
 /*   160 */   605,  515,  605,  605,  605,  516,  517,  518,  511,  605,
 /*   170 */   605,  519,  605,  520,  539,  605,  521,  605,  539,  523,
 /*   180 */   605,  605,  605,  605,  524,  605,  605,  605,  525,  605,
 /*   190 */   605,  526,  605,  605,  527,  605,  605,  528,  605,  605,
 /*   200 */   605,  605,  605,  529,  605,  605,  530,  605,  535,  537,
 /*   210 */   605,  538,  536,  605,  605,  531,  605,  605,  532,  605,
 /*   220 */   605,  533,  605,  605,  534,  604,  481,  605,  586,  605,
 /*   230 */   587,  605,  605,  588,  605,  605,  589,  605,  605,  605,
 /*   240 */   601,  605,  602,  605,  603,  592,  605,  593,  605,  594,
 /*   250 */   605,  595,  605,  596,  605,  605,  597,  598,  605,  605,
 /*   260 */   599,  600,  591,  590,  605,  605,  605,  605,  605,  605,
 /*   270 */   508,  505,  503,  501,  497,  605,  605,  605,  502,  498,
 /*   280 */   605,  605,  605,  459,  605,  466,  468,  605,  471,  605,
 /*   290 */   469,  605,  472,  605,  470,  605,  473,  605,  605,  605,
 /*   300 */   605,  605,  605,  605,  605,  467,  605,  605,  605,  455,
 /*   310 */   605,  605,  605,  605,  605,  605,  605,  456,  605,  460,
 /*   320 */   605,  605,  605,  457,  605,  461,  605,  605,  605,  605,
 /*   330 */   605,  605,  463,  605,  465,  605,  605,  605,  605,  605,
 /*   340 */   605,  462,  605,  464,  605,  605,  448,  605,  605,  605,
 /*   350 */   605,  605,  451,  605,  605,  605,  605,  452,  453,  438,
 /*   360 */   605,  605,  605,  440,  605,  605,  443,  605,  605,  445,
 /*   370 */   605,  605,  447,  605,  605,  441,  605,  605,  442,  605,
 /*   380 */   605,  444,  605,  605,  446,  449,  605,  605,  605,  605,
 /*   390 */   605,  439,  605,  605,  605,  605,  605,  422,  426,  605,
 /*   400 */   605,  605,  605,  605,  423,  427,  605,  605,  605,  605,
 /*   410 */   424,  428,  416,
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
  "xx_assign_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_common_expr",
  "xx_call_parameters",  "xx_declare_variable_list",  "xx_declare_variable",  "xx_array_list",
  "xx_mcall_expr",  "xx_call_parameter",  "xx_array_item",  "xx_array_key",
  "xx_array_value",  "xx_isset_expr",
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
 /* 127 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 128 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 129 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 130 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 131 */ "xx_common_expr ::= xx_assign_expr ADD xx_assign_expr",
 /* 132 */ "xx_common_expr ::= xx_assign_expr SUB xx_assign_expr",
 /* 133 */ "xx_common_expr ::= xx_assign_expr CONCAT xx_assign_expr",
 /* 134 */ "xx_common_expr ::= xx_assign_expr EQUALS xx_assign_expr",
 /* 135 */ "xx_common_expr ::= INSTANCEOF xx_assign_expr",
 /* 136 */ "xx_common_expr ::= TYPEOF xx_assign_expr",
 /* 137 */ "xx_common_expr ::= IDENTIFIER",
 /* 138 */ "xx_common_expr ::= INTEGER",
 /* 139 */ "xx_common_expr ::= STRING",
 /* 140 */ "xx_common_expr ::= DOUBLE",
 /* 141 */ "xx_common_expr ::= NULL",
 /* 142 */ "xx_common_expr ::= FALSE",
 /* 143 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 144 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 145 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 146 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 147 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 148 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 149 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 150 */ "xx_common_expr ::= xx_mcall_expr",
 /* 151 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 152 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 153 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 154 */ "xx_call_parameters ::= xx_call_parameter",
 /* 155 */ "xx_call_parameter ::= xx_literal_expr",
 /* 156 */ "xx_call_parameter ::= IDENTIFIER",
 /* 157 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 158 */ "xx_array_list ::= xx_array_item",
 /* 159 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 160 */ "xx_array_item ::= xx_array_value",
 /* 161 */ "xx_array_key ::= IDENTIFIER",
 /* 162 */ "xx_array_key ::= STRING",
 /* 163 */ "xx_array_key ::= INTEGER",
 /* 164 */ "xx_array_value ::= xx_literal_expr",
 /* 165 */ "xx_array_value ::= IDENTIFIER",
 /* 166 */ "xx_assign_expr ::= TRUE",
 /* 167 */ "xx_literal_expr ::= INTEGER",
 /* 168 */ "xx_literal_expr ::= STRING",
 /* 169 */ "xx_literal_expr ::= DOUBLE",
 /* 170 */ "xx_literal_expr ::= NULL",
 /* 171 */ "xx_literal_expr ::= FALSE",
 /* 172 */ "xx_literal_expr ::= TRUE",
 /* 173 */ "xx_eval_expr ::= xx_literal_expr",
 /* 174 */ "xx_eval_expr ::= NOT xx_eval_expr",
 /* 175 */ "xx_eval_expr ::= TYPEOF xx_eval_expr",
 /* 176 */ "xx_eval_expr ::= ISSET xx_isset_expr",
 /* 177 */ "xx_eval_expr ::= xx_eval_expr TYPEOF xx_eval_expr",
 /* 178 */ "xx_eval_expr ::= xx_eval_expr EQUALS xx_eval_expr",
 /* 179 */ "xx_eval_expr ::= xx_eval_expr NOTEQUALS xx_eval_expr",
 /* 180 */ "xx_eval_expr ::= xx_eval_expr IDENTICAL xx_eval_expr",
 /* 181 */ "xx_eval_expr ::= xx_eval_expr NOTIDENTICAL xx_eval_expr",
 /* 182 */ "xx_eval_expr ::= xx_eval_expr LESS xx_eval_expr",
 /* 183 */ "xx_eval_expr ::= xx_eval_expr GREATER xx_eval_expr",
 /* 184 */ "xx_eval_expr ::= PARENTHESES_OPEN xx_eval_expr PARENTHESES_CLOSE",
 /* 185 */ "xx_eval_expr ::= IDENTIFIER",
 /* 186 */ "xx_eval_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 187 */ "xx_eval_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 188 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 189 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 190 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 191 */ "xx_comment ::= COMMENT",
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
// 701 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1614 "parser.c"
      break;
    case 65:
// 714 "parser.lemon"
{ json_object_put((yypminor->yy7)); }
// 1619 "parser.c"
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
  { 100, 1 },
  { 103, 3 },
  { 103, 4 },
  { 103, 4 },
  { 103, 4 },
  { 103, 3 },
  { 103, 3 },
  { 103, 3 },
  { 103, 3 },
  { 103, 2 },
  { 103, 2 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 2 },
  { 103, 3 },
  { 103, 2 },
  { 103, 4 },
  { 103, 5 },
  { 103, 4 },
  { 103, 3 },
  { 103, 1 },
  { 108, 6 },
  { 108, 5 },
  { 104, 3 },
  { 104, 1 },
  { 109, 1 },
  { 109, 1 },
  { 107, 3 },
  { 107, 1 },
  { 110, 3 },
  { 110, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 112, 1 },
  { 112, 1 },
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
  { 113, 4 },
  { 113, 4 },
  { 113, 4 },
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
// 710 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy7;
}
// 2028 "parser.c"
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
      case 150:
      case 155:
      case 164:
      case 173:
// 716 "parser.lemon"
{
	yygotominor.yy7 = yymsp[0].minor.yy7;
}
// 2056 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 67:
// 720 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(yymsp[-1].minor.yy7, yymsp[0].minor.yy7);
}
// 2067 "parser.c"
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
      case 154:
      case 158:
// 724 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(NULL, yymsp[0].minor.yy7);
}
// 2084 "parser.c"
        break;
      case 7:
// 740 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2093 "parser.c"
        break;
      case 8:
// 744 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, 0, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2103 "parser.c"
        break;
      case 9:
// 748 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy7, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2114 "parser.c"
        break;
      case 10:
// 752 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, 1, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-5].minor);
  yy_destructor(18,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2125 "parser.c"
        break;
      case 11:
// 756 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy7, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-7].minor);
  yy_destructor(18,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2137 "parser.c"
        break;
      case 12:
// 760 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2147 "parser.c"
        break;
      case 13:
// 764 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2158 "parser.c"
        break;
      case 14:
// 768 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(18,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2169 "parser.c"
        break;
      case 15:
// 772 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(22,&yymsp[-6].minor);
  yy_destructor(18,&yymsp[-5].minor);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2181 "parser.c"
        break;
      case 16:
// 776 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
}
// 2188 "parser.c"
        break;
      case 17:
// 780 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 2195 "parser.c"
        break;
      case 18:
// 784 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, yymsp[0].minor.yy7, NULL, status->scanner_state);
}
// 2202 "parser.c"
        break;
      case 19:
// 788 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
}
// 2209 "parser.c"
        break;
      case 20:
// 792 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 2216 "parser.c"
        break;
      case 21:
// 796 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[0].minor.yy7, NULL, yymsp[-1].minor.yy7, status->scanner_state);
}
// 2223 "parser.c"
        break;
      case 22:
// 800 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, yymsp[0].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
}
// 2230 "parser.c"
        break;
      case 23:
// 804 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
}
// 2237 "parser.c"
        break;
      case 24:
// 808 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, yymsp[0].minor.yy7, yymsp[-2].minor.yy7, status->scanner_state);
}
// 2244 "parser.c"
        break;
      case 27:
// 820 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2253 "parser.c"
        break;
      case 28:
// 824 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2262 "parser.c"
        break;
      case 29:
// 828 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2272 "parser.c"
        break;
      case 30:
// 832 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2282 "parser.c"
        break;
      case 31:
// 836 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2291 "parser.c"
        break;
      case 32:
// 840 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2300 "parser.c"
        break;
      case 33:
// 844 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2310 "parser.c"
        break;
      case 34:
// 848 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2320 "parser.c"
        break;
      case 38:
// 864 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2330 "parser.c"
        break;
      case 39:
// 868 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2340 "parser.c"
        break;
      case 41:
// 876 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2353 "parser.c"
        break;
      case 42:
// 880 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2366 "parser.c"
        break;
      case 43:
// 884 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2379 "parser.c"
        break;
      case 44:
// 888 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2392 "parser.c"
        break;
      case 45:
// 892 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2405 "parser.c"
        break;
      case 46:
// 896 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-8].minor);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2418 "parser.c"
        break;
      case 47:
// 900 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2431 "parser.c"
        break;
      case 48:
// 904 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy7, yymsp[-1].minor.yy7, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-8].minor);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2444 "parser.c"
        break;
      case 49:
// 908 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2457 "parser.c"
        break;
      case 50:
// 912 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2470 "parser.c"
        break;
      case 51:
// 916 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2483 "parser.c"
        break;
      case 52:
// 920 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2496 "parser.c"
        break;
      case 53:
      case 97:
      case 107:
      case 122:
      case 153:
      case 157:
// 924 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2509 "parser.c"
        break;
      case 55:
// 932 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2516 "parser.c"
        break;
      case 56:
// 936 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(yymsp[-1].minor.yy7, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2523 "parser.c"
        break;
      case 57:
// 940 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, yymsp[-1].minor.yy7, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2530 "parser.c"
        break;
      case 58:
// 944 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2538 "parser.c"
        break;
      case 59:
// 948 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(yymsp[-3].minor.yy7, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2546 "parser.c"
        break;
      case 60:
// 952 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, yymsp[-3].minor.yy7, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2554 "parser.c"
        break;
      case 61:
// 956 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2563 "parser.c"
        break;
      case 62:
// 960 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(31,&yymsp[0].minor);
}
// 2571 "parser.c"
        break;
      case 63:
// 964 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(32,&yymsp[0].minor);
}
// 2579 "parser.c"
        break;
      case 64:
// 968 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(33,&yymsp[0].minor);
}
// 2587 "parser.c"
        break;
      case 65:
// 972 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(34,&yymsp[0].minor);
}
// 2595 "parser.c"
        break;
      case 66:
// 976 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(35,&yymsp[0].minor);
}
// 2603 "parser.c"
        break;
      case 82:
// 1040 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2612 "parser.c"
        break;
      case 83:
// 1044 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2621 "parser.c"
        break;
      case 84:
// 1048 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-2].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2631 "parser.c"
        break;
      case 85:
// 1052 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-5].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(19,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2644 "parser.c"
        break;
      case 86:
// 1056 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2654 "parser.c"
        break;
      case 87:
// 1060 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-7].minor.yy7, yymsp[-5].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(38,&yymsp[-8].minor);
  yy_destructor(19,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2667 "parser.c"
        break;
      case 88:
// 1064 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-6].minor.yy7, yymsp[-4].minor.yy7, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-7].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2680 "parser.c"
        break;
      case 89:
// 1068 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-6].minor.yy7, NULL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(38,&yymsp[-7].minor);
  yy_destructor(19,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2693 "parser.c"
        break;
      case 90:
// 1072 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2703 "parser.c"
        break;
      case 91:
// 1076 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_loop_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2713 "parser.c"
        break;
      case 92:
// 1080 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_while_statement(yymsp[-2].minor.yy7, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(19,&yymsp[-1].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2723 "parser.c"
        break;
      case 93:
// 1084 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_while_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2733 "parser.c"
        break;
      case 94:
// 1088 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, NULL, yymsp[-5].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2744 "parser.c"
        break;
      case 95:
// 1092 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(42,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(19,&yymsp[-2].minor);
  yy_destructor(20,&yymsp[0].minor);
}
// 2756 "parser.c"
        break;
      case 96:
// 1096 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2765 "parser.c"
        break;
      case 99:
// 1109 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2773 "parser.c"
        break;
      case 100:
// 1114 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2782 "parser.c"
        break;
      case 101:
// 1119 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property-append", yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2793 "parser.c"
        break;
      case 102:
// 1124 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2803 "parser.c"
        break;
      case 103:
// 1129 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2813 "parser.c"
        break;
      case 104:
// 1134 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[0].minor);
}
// 2821 "parser.c"
        break;
      case 105:
// 1139 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[0].minor);
}
// 2829 "parser.c"
        break;
      case 106:
// 1143 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_echo_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2838 "parser.c"
        break;
      case 109:
// 1155 "parser.lemon"
{
	yygotominor.yy7 = yymsp[0].minor.yy7;;
}
// 2845 "parser.c"
        break;
      case 110:
// 1160 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_return_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2854 "parser.c"
        break;
      case 111:
// 1165 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(52,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2868 "parser.c"
        break;
      case 112:
// 1174 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2883 "parser.c"
        break;
      case 113:
// 1183 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2898 "parser.c"
        break;
      case 114:
// 1192 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(52,&yymsp[-5].minor);
  yy_destructor(46,&yymsp[-3].minor);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2913 "parser.c"
        break;
      case 115:
// 1201 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2922 "parser.c"
        break;
      case 116:
// 1206 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy7, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(13,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2934 "parser.c"
        break;
      case 117:
// 1210 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2943 "parser.c"
        break;
      case 118:
// 1214 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2952 "parser.c"
        break;
      case 119:
// 1218 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2961 "parser.c"
        break;
      case 120:
// 1222 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2970 "parser.c"
        break;
      case 121:
// 1226 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(17,&yymsp[0].minor);
}
// 2979 "parser.c"
        break;
      case 124:
// 1238 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2986 "parser.c"
        break;
      case 125:
// 1242 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 2994 "parser.c"
        break;
      case 127:
// 1251 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3002 "parser.c"
        break;
      case 128:
      case 188:
// 1256 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3012 "parser.c"
        break;
      case 129:
      case 189:
// 1261 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3022 "parser.c"
        break;
      case 130:
      case 190:
// 1266 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3032 "parser.c"
        break;
      case 131:
// 1271 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("add", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3040 "parser.c"
        break;
      case 132:
// 1276 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("sub", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3048 "parser.c"
        break;
      case 133:
// 1281 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("concat", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3056 "parser.c"
        break;
      case 134:
      case 178:
// 1286 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("equals", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 3065 "parser.c"
        break;
      case 135:
// 1291 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("instanceof", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3073 "parser.c"
        break;
      case 136:
      case 175:
// 1296 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("typeof", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3082 "parser.c"
        break;
      case 137:
      case 156:
      case 161:
      case 165:
      case 185:
// 1301 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3093 "parser.c"
        break;
      case 138:
      case 163:
      case 167:
// 1306 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3102 "parser.c"
        break;
      case 139:
      case 162:
      case 168:
// 1311 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3111 "parser.c"
        break;
      case 140:
      case 169:
// 1316 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3119 "parser.c"
        break;
      case 141:
      case 170:
// 1321 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[0].minor);
}
// 3128 "parser.c"
        break;
      case 142:
      case 171:
// 1326 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[0].minor);
}
// 3137 "parser.c"
        break;
      case 143:
// 1331 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3146 "parser.c"
        break;
      case 144:
// 1336 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array", yymsp[-1].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(47,&yymsp[0].minor);
}
// 3155 "parser.c"
        break;
      case 145:
// 1341 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3163 "parser.c"
        break;
      case 146:
// 1346 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3173 "parser.c"
        break;
      case 147:
// 1351 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(13,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3183 "parser.c"
        break;
      case 148:
      case 186:
// 1356 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3193 "parser.c"
        break;
      case 149:
      case 187:
// 1361 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3203 "parser.c"
        break;
      case 151:
// 1371 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3213 "parser.c"
        break;
      case 152:
// 1376 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3223 "parser.c"
        break;
      case 159:
// 1404 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_array_item(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
}
// 3231 "parser.c"
        break;
      case 160:
// 1408 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_array_item(NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 3238 "parser.c"
        break;
      case 166:
      case 172:
// 1432 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[0].minor);
}
// 3247 "parser.c"
        break;
      case 174:
// 1464 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3255 "parser.c"
        break;
      case 176:
// 1472 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("isset", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
}
// 3263 "parser.c"
        break;
      case 177:
// 1476 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("instanceof", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3271 "parser.c"
        break;
      case 179:
// 1484 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not-equals", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3279 "parser.c"
        break;
      case 180:
// 1488 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("identical", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3287 "parser.c"
        break;
      case 181:
// 1492 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not-identical", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3295 "parser.c"
        break;
      case 182:
// 1496 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("less", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3303 "parser.c"
        break;
      case 183:
// 1500 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("greater", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3311 "parser.c"
        break;
      case 184:
// 1504 "parser.lemon"
{
	yygotominor.yy7 = yymsp[-1].minor.yy7;
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3320 "parser.c"
        break;
      case 191:
// 1536 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3327 "parser.c"
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
// 667 "parser.lemon"


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

// 3403 "parser.c"
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
