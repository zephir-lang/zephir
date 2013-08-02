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
			for (i = 0; i < array_length; i++) {
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
#define YYNOCODE 116
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy55;
  int yy231;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 405
#define YYNRULE 187
#define YYERRORSYMBOL 64
#define YYERRSYMDT yy231
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
 /*     0 */   315,  305,   74,  252,  593,    1,    2,  404,    4,    5,
 /*    10 */     6,  231,  240,  246,  243,  159,  249,   42,   44,   46,
 /*    20 */    73,   55,   59,   64,  337,   71,   58,  338,  350,  252,
 /*    30 */   202,  197,  204,  207,  198,  203,  222,  231,  240,  246,
 /*    40 */   243,  160,  249,   42,   44,   46,  229,   55,   59,   64,
 /*    50 */   190,   71,  311,    7,  234,  252,   10,  197,  204,  207,
 /*    60 */   391,  252,  222,  231,  240,  246,  243,  167,  249,   42,
 /*    70 */    44,   46,  233,   55,   59,   64,   79,   71,  263,  189,
 /*    80 */   234,  252,  187,  197,  204,  207,   18,  160,  222,  231,
 /*    90 */   240,  246,  243,   88,  249,   42,   44,   46,  242,   55,
 /*   100 */    59,   64,  191,   71,  265,  169,  171,  252,  202,  197,
 /*   110 */   204,  207,  180,  201,  222,  231,  240,  246,  243,  160,
 /*   120 */   249,   42,   44,   46,  301,   55,   59,   64,  192,   71,
 /*   130 */   324,   90,   92,  252,  182,  197,  204,  207,   60,  262,
 /*   140 */   222,  231,  240,  246,  243,  213,  249,   42,   44,   46,
 /*   150 */   160,   55,   59,   64,  172,   71,  309,  300,   12,  252,
 /*   160 */   385,  197,  204,  207,   47,  262,  222,  231,  240,  246,
 /*   170 */   243,  275,  249,   42,   44,   46,  160,   55,   59,   64,
 /*   180 */   342,   71,  326,  216,  219,  252,  140,  197,  204,  207,
 /*   190 */   354,  272,  222,  231,  240,  246,  243,  254,  249,   42,
 /*   200 */    44,   46,  304,   55,   59,   64,   79,   71,   63,   78,
 /*   210 */   234,  252,  341,  197,  204,  207,  472,  160,  222,  231,
 /*   220 */   240,  246,  243,  139,  249,   42,   44,   46,  245,   55,
 /*   230 */    59,   64,   79,   71,  269,  181,  234,  252,  177,  197,
 /*   240 */   204,  207,   66,  160,  222,  231,  240,  246,  243,  160,
 /*   250 */   249,   42,   44,   46,  248,   55,   59,   64,  225,   71,
 /*   260 */   270,  223,  234,  252,   82,  197,  204,  207,  232,  239,
 /*   270 */   222,  231,  240,  246,  243,  160,  249,   42,   44,   46,
 /*   280 */   251,   55,   59,   64,   79,   71,   25,  193,  200,  252,
 /*   290 */   343,  197,  204,  207,  170,  160,  222,  231,  240,  246,
 /*   300 */   243,  159,  249,   42,   44,   46,  199,   55,   59,   64,
 /*   310 */   344,   71,  260,  241,  239,  252,  471,  197,  204,  207,
 /*   320 */   244,  239,  222,  231,  240,  246,  243,  159,  249,   42,
 /*   330 */    44,   46,  158,   55,   59,   64,  470,   71,   27,  247,
 /*   340 */   239,  252,  469,  197,  204,  207,  250,  239,  222,  231,
 /*   350 */   240,  246,  243,  275,  249,   42,   44,   46,  164,   55,
 /*   360 */    59,   64,   94,   71,   54,   72,  196,  252,  257,  197,
 /*   370 */   204,  207,  346,  160,  222,  231,  240,  246,  243,  160,
 /*   380 */   249,   42,   44,   46,  299,   55,   59,   64,  468,   71,
 /*   390 */   317,  355,  394,  252,  398,  197,  204,  207,  360,  356,
 /*   400 */   222,  231,  240,  246,  243,  159,  249,   42,   44,   46,
 /*   410 */   319,   55,   59,   64,   96,   71,  264,  209,  212,  252,
 /*   420 */    98,  197,  204,  207,  347,  160,  222,  231,  240,  246,
 /*   430 */   243,  160,  249,   42,   44,   46,  175,   55,   59,   64,
 /*   440 */   100,   71,  261,  135,  114,  252,  102,  197,  204,  207,
 /*   450 */   168,  160,  222,  231,  240,  246,  243,  160,  249,   42,
 /*   460 */    44,   46,  361,   55,   59,   64,  348,   71,  335,  349,
 /*   470 */   362,  252,  467,  197,  204,  207,  115,  104,  222,  231,
 /*   480 */   240,  246,  243,  275,  249,   42,   44,   46,  160,   55,
 /*   490 */    59,   64,  134,   71,   50,  367,   61,  252,  205,  197,
 /*   500 */   204,  207,  466,  368,  222,  231,  240,  246,  243,  160,
 /*   510 */   249,   42,   44,   46,  313,   55,   59,   64,  290,   71,
 /*   520 */   333,  289,  353,  252,  359,  197,  204,  207,  396,  106,
 /*   530 */   222,  231,  240,  246,  243,  366,  249,   42,   44,   46,
 /*   540 */   160,   55,   59,   64,  287,   71,  274,   20,  286,  252,
 /*   550 */   238,  197,  204,  207,  285,  108,  222,  231,  240,  246,
 /*   560 */   243,  372,  249,   42,   44,   46,  160,   55,   59,   64,
 /*   570 */   124,   71,   70,  328,  237,  252,   21,  197,  204,  207,
 /*   580 */    52,  160,  222,  231,  240,  246,  243,  283,  249,   42,
 /*   590 */    44,   46,  282,   55,   59,   64,  298,   71,  297,  280,
 /*   600 */   284,  252,  357,  197,  204,  207,  358,  110,  222,  231,
 /*   610 */   240,  246,  243,  405,  249,   42,   44,   46,  160,   55,
 /*   620 */    59,   64,  303,   71,  318,    3,    4,    5,    6,  197,
 /*   630 */   204,  207,   75,  281,  222,   84,  119,  123,  117,  153,
 /*   640 */    81,  227,  112,   80,   95,   99,  101,   97,   83,  103,
 /*   650 */   105,  107,  103,  105,  107,  109,  176,   93,  142,  143,
 /*   660 */   206,  144,  145,  147,  146,  118,  138,   84,  119,  123,
 /*   670 */   117,  153,   81,  131,  112,  312,  160,  297,  280,  284,
 /*   680 */   125,  126,   51,  127,  128,  129,  130,  109,  156,   80,
 /*   690 */    95,   99,  101,   97,   83,  103,  105,  107,  120,   84,
 /*   700 */   119,  123,  117,  153,   81,  131,  112,  271,   68,  297,
 /*   710 */   280,  284,  125,  126,  279,  127,  128,  129,  130,  109,
 /*   720 */   165,   80,   95,   99,  101,   97,   83,  103,  105,  107,
 /*   730 */    67,   84,  119,  123,  117,  153,   81,  131,  112,  278,
 /*   740 */   258,  160,   41,  363,  125,  126,  373,  127,  128,  129,
 /*   750 */   130,  109,  316,  364,  374,  399,  253,   29,   30,   31,
 /*   760 */    32,   33,   34,   35,   36,   37,   38,   39,   40,  131,
 /*   770 */   336,   17,  351,   41,  377,  350,  125,  126,   91,  127,
 /*   780 */   128,  129,  130,  334,   20,  224,  121,  253,   29,   30,
 /*   790 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   800 */   276,  280,  284,   41,  116,  395,   16,  378,  384,  383,
 /*   810 */   174,  377,  350,   57,  400,  160,  162,  253,   29,   30,
 /*   820 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   830 */    41,   80,   95,   99,  101,   97,   83,  103,  105,  107,
 /*   840 */   325,  267,  208,   86,  253,   29,   30,   31,   32,   33,
 /*   850 */    34,   35,   36,   37,   38,   39,   40,   89,  369,   41,
 /*   860 */   353,  111,  359,  340,  401,   16,  378,  384,  383,  259,
 /*   870 */   377,  350,  370,  253,   29,   30,   31,   32,   33,   34,
 /*   880 */    35,   36,   37,   38,   39,   40,  235,  210,  122,   41,
 /*   890 */   116,  388,   16,  378,  384,  383,  163,  377,  350,   26,
 /*   900 */   236,  160,  162,  253,   29,   30,   31,   32,   33,   34,
 /*   910 */    35,   36,   37,   38,   39,   40,   41,   80,   95,   99,
 /*   920 */   101,   97,   83,  103,  105,  107,  268,  266,   76,  155,
 /*   930 */   253,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*   940 */    38,   39,   40,  211,   48,   41,  116,   13,   16,  378,
 /*   950 */   384,  383,  157,  377,  350,   49,  154,  160,  162,  253,
 /*   960 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   970 */    39,   40,  375,  403,  288,   41,  352,  365,  376,  371,
 /*   980 */   345,  379,   87,  382,  383,  273,  338,  350,  277,  253,
 /*   990 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1000 */    39,   40,   41,  291,  292,  293,  294,  295,  296,  380,
 /*  1010 */   351,  402,   62,  350,   65,  214,  253,   29,   30,   31,
 /*  1020 */    32,   33,   34,   35,   36,   37,   38,   39,   40,  390,
 /*  1030 */   578,   41,  352,  365,  577,  371,  345,  303,  576,  318,
 /*  1040 */   340,   53,  255,  256,   85,  253,   29,   30,   31,   32,
 /*  1050 */    33,   34,   35,   36,   37,   38,   39,   40,  215,  421,
 /*  1060 */    14,   41,  352,  365,  428,  371,  345,  302,   19,    8,
 /*  1070 */   327,   69,  386,  387,   45,  253,   29,   30,   31,   32,
 /*  1080 */    33,   34,   35,   36,   37,   38,   39,   40,   41,  425,
 /*  1090 */   140,  217,  339,   19,  389,  327,  345,  226,  310,  218,
 /*  1100 */     9,  393,  253,   29,   30,   31,   32,   33,   34,   35,
 /*  1110 */    36,   37,   38,   39,   40,  422,  392,   41,  352,  365,
 /*  1120 */   220,  371,  345,  230,  140,  136,  137,  148,  184,  323,
 /*  1130 */   149,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1140 */    37,   38,   39,   40,   80,   95,   99,  101,   97,   83,
 /*  1150 */   103,  105,  107,  288,  149,   79,  133,  221,  185,  152,
 /*  1160 */   137,  148,  132,  590,  328,  116,  160,  277,  151,  150,
 /*  1170 */    24,  144,  145,  147,  146,  288,  160,  161,  322,  329,
 /*  1180 */   183,  330,  291,  292,  293,  294,  295,  296,  288,  277,
 /*  1190 */   141,  331,  151,  150,  173,  144,  145,  147,  146,   43,
 /*  1200 */   188,  321,  277,   23,  291,  292,  293,  294,  295,  296,
 /*  1210 */   332,  113,  166,  320,   77,  319,  307,  291,  292,  293,
 /*  1220 */   294,  295,  296,  406,   56,  314,  179,   22,  142,  143,
 /*  1230 */    11,  144,  145,  147,  146,  178,  186,  308,  194,  195,
 /*  1240 */     7,  306,  304,   10,  304,  304,   15,  391,  252,  352,
 /*  1250 */   365,  397,  371,  345,  352,  365,  423,  371,  345,  302,
 /*  1260 */    19,  116,  327,  304,  304,  304,  304,  228,  304,  304,
 /*  1270 */   304,  427,  160,  162,  302,   19,  429,  327,  304,  302,
 /*  1280 */    19,  426,  327,  304,  381,  365,  304,  371,  304,  304,
 /*  1290 */   424,  304,  304,  302,   19,  304,  327,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    22,   18,    1,   25,   65,   66,   67,   68,   69,   70,
 /*    10 */    71,   33,   34,   35,   36,    1,   38,   39,   40,   41,
 /*    20 */    19,   43,   44,   45,   75,   47,   22,   78,   79,   25,
 /*    30 */    99,   53,   54,   55,  103,  104,   58,   33,   34,   35,
 /*    40 */    36,  110,   38,   39,   40,   41,   32,   43,   44,   45,
 /*    50 */    18,   47,   22,   17,    1,   25,   20,   53,   54,   55,
 /*    60 */    24,   25,   58,   33,   34,   35,   36,   18,   38,   39,
 /*    70 */    40,   41,   19,   43,   44,   45,   99,   47,   22,  102,
 /*    80 */     1,   25,   50,   53,   54,   55,   79,  110,   58,   33,
 /*    90 */    34,   35,   36,   18,   38,   39,   40,   41,   19,   43,
 /*   100 */    44,   45,   50,   47,   22,   56,   57,   25,   99,   53,
 /*   110 */    54,   55,   27,  104,   58,   33,   34,   35,   36,  110,
 /*   120 */    38,   39,   40,   41,   22,   43,   44,   45,   27,   47,
 /*   130 */    22,   56,   57,   25,   49,   53,   54,   55,   98,   99,
 /*   140 */    58,   33,   34,   35,   36,   18,   38,   39,   40,   41,
 /*   150 */   110,   43,   44,   45,   50,   47,   22,   21,   21,   25,
 /*   160 */    23,   53,   54,   55,   98,   99,   58,   33,   34,   35,
 /*   170 */    36,    1,   38,   39,   40,   41,  110,   43,   44,   45,
 /*   180 */    27,   47,   22,   56,   57,   25,   77,   53,   54,   55,
 /*   190 */    18,   21,   58,   33,   34,   35,   36,    1,   38,   39,
 /*   200 */    40,   41,   30,   43,   44,   45,   99,   47,   22,  102,
 /*   210 */     1,   25,   18,   53,   54,   55,   18,  110,   58,   33,
 /*   220 */    34,   35,   36,  114,   38,   39,   40,   41,   19,   43,
 /*   230 */    44,   45,   99,   47,   22,  102,    1,   25,   99,   53,
 /*   240 */    54,   55,   46,  110,   58,   33,   34,   35,   36,  110,
 /*   250 */    38,   39,   40,   41,   19,   43,   44,   45,   15,   47,
 /*   260 */    22,   18,    1,   25,   99,   53,   54,   55,  106,  107,
 /*   270 */    58,   33,   34,   35,   36,  110,   38,   39,   40,   41,
 /*   280 */    19,   43,   44,   45,   99,   47,   22,  102,    1,   25,
 /*   290 */    77,   53,   54,   55,   50,  110,   58,   33,   34,   35,
 /*   300 */    36,    1,   38,   39,   40,   41,   19,   43,   44,   45,
 /*   310 */    19,   47,   22,  106,  107,   25,   18,   53,   54,   55,
 /*   320 */   106,  107,   58,   33,   34,   35,   36,    1,   38,   39,
 /*   330 */    40,   41,   32,   43,   44,   45,   18,   47,   22,  106,
 /*   340 */   107,   25,   18,   53,   54,   55,  106,  107,   58,   33,
 /*   350 */    34,   35,   36,    1,   38,   39,   40,   41,   32,   43,
 /*   360 */    44,   45,   99,   47,   22,  100,  101,   25,   99,   53,
 /*   370 */    54,   55,   18,  110,   58,   33,   34,   35,   36,  110,
 /*   380 */    38,   39,   40,   41,   32,   43,   44,   45,   18,   47,
 /*   390 */    22,   19,   21,   25,   23,   53,   54,   55,   18,   27,
 /*   400 */    58,   33,   34,   35,   36,    1,   38,   39,   40,   41,
 /*   410 */    30,   43,   44,   45,   99,   47,   22,   48,   49,   25,
 /*   420 */    99,   53,   54,   55,   27,  110,   58,   33,   34,   35,
 /*   430 */    36,  110,   38,   39,   40,   41,   32,   43,   44,   45,
 /*   440 */    99,   47,   22,    1,   18,   25,   99,   53,   54,   55,
 /*   450 */    50,  110,   58,   33,   34,   35,   36,  110,   38,   39,
 /*   460 */    40,   41,   19,   43,   44,   45,   77,   47,   22,   19,
 /*   470 */    27,   25,   18,   53,   54,   55,   31,   99,   58,   33,
 /*   480 */    34,   35,   36,    1,   38,   39,   40,   41,  110,   43,
 /*   490 */    44,   45,   50,   47,   22,   19,   21,   25,   99,   53,
 /*   500 */    54,   55,   18,   27,   58,   33,   34,   35,   36,  110,
 /*   510 */    38,   39,   40,   41,   32,   43,   44,   45,    5,   47,
 /*   520 */    22,   18,   26,   25,   28,   53,   54,   55,   22,   99,
 /*   530 */    58,   33,   34,   35,   36,   18,   38,   39,   40,   41,
 /*   540 */   110,   43,   44,   45,   77,   47,   22,   30,   27,   25,
 /*   550 */    77,   53,   54,   55,   18,   99,   58,   33,   34,   35,
 /*   560 */    36,   18,   38,   39,   40,   41,  110,   43,   44,   45,
 /*   570 */    99,   47,   22,   30,   27,   25,   18,   53,   54,   55,
 /*   580 */    21,  110,   58,   33,   34,   35,   36,   77,   38,   39,
 /*   590 */    40,   41,   27,   43,   44,   45,   80,   47,   82,   83,
 /*   600 */    84,   25,   77,   53,   54,   55,   19,   99,   58,   33,
 /*   610 */    34,   35,   36,    0,   38,   39,   40,   41,  110,   43,
 /*   620 */    44,   45,   26,   47,   28,   68,   69,   70,   71,   53,
 /*   630 */    54,   55,  101,   18,   58,   11,   12,   13,   14,   15,
 /*   640 */    16,   31,   18,    2,    3,    4,    5,    6,    7,    8,
 /*   650 */     9,   10,    8,    9,   10,   31,   32,   50,   56,   57,
 /*   660 */    19,   59,   60,   61,   62,   99,   63,   11,   12,   13,
 /*   670 */    14,   15,   16,   49,   18,   80,  110,   82,   83,   84,
 /*   680 */    56,   57,   42,   59,   60,   61,   62,   31,   32,    2,
 /*   690 */     3,    4,    5,    6,    7,    8,    9,   10,   18,   11,
 /*   700 */    12,   13,   14,   15,   16,   49,   18,   80,   21,   82,
 /*   710 */    83,   84,   56,   57,   77,   59,   60,   61,   62,   31,
 /*   720 */    32,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*   730 */    99,   11,   12,   13,   14,   15,   16,   49,   18,   27,
 /*   740 */    21,  110,   71,   77,   56,   57,   19,   59,   60,   61,
 /*   750 */    62,   31,   81,   19,   27,   18,   85,   86,   87,   88,
 /*   760 */    89,   90,   91,   92,   93,   94,   95,   96,   97,   49,
 /*   770 */    74,   75,   76,   71,   78,   79,   56,   57,   50,   59,
 /*   780 */    60,   61,   62,   81,   30,   19,    1,   85,   86,   87,
 /*   790 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*   800 */    82,   83,   84,   71,   99,   72,   73,   74,   75,   76,
 /*   810 */   105,   78,   79,   81,   21,  110,  111,   85,   86,   87,
 /*   820 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*   830 */    71,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*   840 */    81,   21,   18,   18,   85,   86,   87,   88,   89,   90,
 /*   850 */    91,   92,   93,   94,   95,   96,   97,   50,   77,   71,
 /*   860 */    26,   32,   28,   29,   72,   73,   74,   75,   76,   81,
 /*   870 */    78,   79,   19,   85,   86,   87,   88,   89,   90,   91,
 /*   880 */    92,   93,   94,   95,   96,   97,  107,   18,  108,   71,
 /*   890 */    99,   72,   73,   74,   75,   76,  105,   78,   79,   81,
 /*   900 */    18,  110,  111,   85,   86,   87,   88,   89,   90,   91,
 /*   910 */    92,   93,   94,   95,   96,   97,   71,    2,    3,    4,
 /*   920 */     5,    6,    7,    8,    9,   10,   81,   42,   18,   31,
 /*   930 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   940 */    95,   96,   97,   19,   21,   71,   99,   72,   73,   74,
 /*   950 */    75,   76,  105,   78,   79,   81,   18,  110,  111,   85,
 /*   960 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*   970 */    96,   97,   77,   22,    4,   71,   25,   26,   19,   28,
 /*   980 */    29,   73,   49,   75,   76,   81,   78,   79,   18,   85,
 /*   990 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*  1000 */    96,   97,   71,   33,   34,   35,   36,   37,   38,   75,
 /*  1010 */    76,   22,   81,   79,   18,   50,   85,   86,   87,   88,
 /*  1020 */    89,   90,   91,   92,   93,   94,   95,   96,   97,   22,
 /*  1030 */    63,   71,   25,   26,   63,   28,   29,   26,   63,   28,
 /*  1040 */    29,   81,   18,   46,  108,   85,   86,   87,   88,   89,
 /*  1050 */    90,   91,   92,   93,   94,   95,   96,   97,   19,   22,
 /*  1060 */    22,   71,   25,   26,   22,   28,   29,   25,   26,   18,
 /*  1070 */    28,   81,   18,   21,   19,   85,   86,   87,   88,   89,
 /*  1080 */    90,   91,   92,   93,   94,   95,   96,   97,   71,   22,
 /*  1090 */    77,   50,   25,   26,   22,   28,   29,   18,   81,   19,
 /*  1100 */    19,   18,   85,   86,   87,   88,   89,   90,   91,   92,
 /*  1110 */    93,   94,   95,   96,   97,   22,   20,   71,   25,   26,
 /*  1120 */    50,   28,   29,   19,   77,  112,  113,  114,   27,   21,
 /*  1130 */    18,   85,   86,   87,   88,   89,   90,   91,   92,   93,
 /*  1140 */    94,   95,   96,   97,    2,    3,    4,    5,    6,    7,
 /*  1150 */     8,    9,   10,    4,   18,   99,  109,   19,  102,  112,
 /*  1160 */   113,  114,   50,   21,   30,   99,  110,   18,   56,   57,
 /*  1170 */    21,   59,   60,   61,   62,    4,  110,  111,   32,   18,
 /*  1180 */    50,   31,   33,   34,   35,   36,   37,   38,    4,   18,
 /*  1190 */    18,   32,   56,   57,   31,   59,   60,   61,   62,   19,
 /*  1200 */    27,   31,   18,   32,   33,   34,   35,   36,   37,   38,
 /*  1210 */    21,   48,   49,   18,   27,   30,   32,   33,   34,   35,
 /*  1220 */    36,   37,   38,    0,   21,   21,   18,   31,   56,   57,
 /*  1230 */    18,   59,   60,   61,   62,   48,   49,   21,   51,   52,
 /*  1240 */    17,   31,  115,   20,   30,  115,   22,   24,   25,   25,
 /*  1250 */    26,   22,   28,   29,   25,   26,   22,   28,   29,   25,
 /*  1260 */    26,   99,   28,  115,  115,  115,  115,  105,  115,  115,
 /*  1270 */   115,   22,  110,  111,   25,   26,   22,   28,  115,   25,
 /*  1280 */    26,   22,   28,  115,   25,   26,  115,   28,  115,  115,
 /*  1290 */    22,  115,  115,   25,   26,  115,   28,
};
#define YY_SHIFT_USE_DFLT (-23)
static short yy_shift_ofst[] = {
 /*     0 */    36,  613, 1223,  -23,  -23,  -23,  -23, 1051, 1081,  -23,
 /*    10 */  1212,  137, 1224, 1038,  -23,  -23, 1037, 1268,  -23,  754,
 /*    20 */   558, 1196, 1171, 1149,  264,  -23,  316,  -23,  -23,  -23,
 /*    30 */   -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,
 /*    40 */   -23,  -23, 1180,  -23, 1055,  -23,  720,  923,   82,  472,
 /*    50 */   640,  559,  394,  342,  -23, 1203,   56,    4,  -23,  720,
 /*    60 */   475,  420,  186,  -23,  996,  196,  720,  687,  576,  550,
 /*    70 */   -23,  910,    1,  -23,  910,  -23, 1187,  720,  -23,  915,
 /*    80 */   720,  720,  -23,  720,  825,  -23,  933,   75,  807,  -23,
 /*    90 */   728,  -23,  607,  -23,  644,  720,  644,  720,  644,  720,
 /*   100 */   644,  720,  644,  720,  -23,  720,  -23,  720,  -23,  720,
 /*   110 */   829,  -23, 1163,  426,  445,  688,  915,  720,  -23,  680,
 /*   120 */   785,  825,  -23,  720,  -23,  -23,  -23,  -23,  -23,  -23,
 /*   130 */   -23, 1112,  -23,  442,  -23, 1136,  -23,  603, 1172,  -23,
 /*   140 */   -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  975,
 /*   150 */   971,  967,  -23,  938,  898,  656,  -23,  300,  -23,  720,
 /*   160 */   -23,  -23,  -23,  326,  -23,  -23,   49,  400,  -23,  244,
 /*   170 */   -23,  104,  -23,  624,  404,  -23,  -23,  644, 1208,   85,
 /*   180 */   720,  -23, 1130, 1101,  720,  -23,   32, 1173,  720,  -23,
 /*   190 */    52,  101,  720,  -23,  -23,  -23,  -23,  720,  287,  -23,
 /*   200 */   720,  -23,  915,  -23,  720,  641,  -23,  824,  369,  869,
 /*   210 */   924,  -23,  127,  965, 1039,  -23, 1041, 1080,  -23, 1070,
 /*   220 */  1138,  -23,  243,  766,  -23, 1079,  610,  720,   14, 1104,
 /*   230 */   -23,  882,   53,  -23,  882,  -23,  547,  602,  -23,  -23,
 /*   240 */   882,   79,  -23,  882,  209,  -23,  882,  235,  -23,  882,
 /*   250 */   261,  -23,  -23,  -23, 1024,  997,  720,  719,  576,  290,
 /*   260 */   -23,  -23, 1142,  -23,  -23,  885,  820,  238,  212,  -23,
 /*   270 */   -23,  170,  576,  524,  -23,  970,  -23,  712,  602,  -23,
 /*   280 */   615,  565,  602,  -23,  536,  521,  602,  -23,  503,  513,
 /*   290 */   484,  454,  370,  324,  318,  298,  198,  -23,  352,  136,
 /*   300 */   102,  -23,  596, 1214,  -17, 1210, 1184, 1216,  134,  -23,
 /*   310 */    30,  -23,  482, 1204,  -22,  -23,  368,  -23, 1185, 1195,
 /*   320 */  1170, 1146, 1108,  108,  -23,  160,  -23, 1134, 1161, 1150,
 /*   330 */  1159, 1189,  498,  -23,  446,  -23, 1067, 1042,  -23, 1011,
 /*   340 */   194,  153,  602,  291,  -23,  354,  397,  602,  450,  -23,
 /*   350 */   -23,  -23,  834,  172,  372,  -23,  602,  587,  -23,  380,
 /*   360 */   443,  -23,  602,  734,  -23,  517,  476,  -23,  602,  853,
 /*   370 */   -23,  543,  727,  -23,  602,  959,  -23,  -23, 1093, 1259,
 /*   380 */  1254,  496, 1249,  -23, 1234, 1054, 1052, 1007, 1072,  -23,
 /*   390 */   -23, 1096, 1083,  371, 1229,  506,  -23,  -23,  737,  793,
 /*   400 */   951,  989,  -23,  -23,  -23,
};
#define YY_REDUCE_USE_DFLT (-70)
static short yy_reduce_ofst[] = {
 /*     0 */   -61,  -70,  557,  -70,  -70,  -70,  -70,  -70,  -70,  -70,
 /*    10 */   -70,  -70,  875,  -70,  -70,  -70,  696,    7,  -70,  -70,
 /*    20 */   -70,  -70,  516,  627,  818,  -70, 1046,  -70,  -70,  -70,
 /*    30 */   -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,
 /*    40 */   -70,  -70,  -70,  -70,  -70,  -70,   66,  -70,  874, 1046,
 /*    50 */   -70,  -70,  960, 1046,  -70,  -70,  732, 1046,  -70,   40,
 /*    60 */   -70,  931, 1046,  -70,  -70,  -70,  631,  -70,  990, 1046,
 /*    70 */   -70,  265,  -70,  -70,  531,  -70,  -70,  107,  -70,  -70,
 /*    80 */   139,  165,  -70,  263,  936,  -70,  -70,  -70,  -70,  -70,
 /*    90 */   -70,  -70,  -70,  -70,  -70,  315,  -70,  321,  -70,  341,
 /*   100 */   -70,  347,  -70,  378,  -70,  430,  -70,  456,  -70,  508,
 /*   110 */   -70,  -70,  -70,  -70,  -70,  791,  -70,  566,  -70,  -70,
 /*   120 */   -70,  780,  -70,  471,  -70,  -70,  -70,  -70,  -70,  -70,
 /*   130 */   -70, 1047,  -70,  -70,  -70, 1013,  -70,  -70,  109,  -70,
 /*   140 */   -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,
 /*   150 */   -70,  -70,  -70,  -70,  -70,  847,  -70,  -70,  -70, 1066,
 /*   160 */   -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,
 /*   170 */   -70,  -70,  -70,  705,  -70,  -70,  -70,  -70,  -70,  -70,
 /*   180 */   133,  -70,  -70,  -70, 1056,  -70,  -70,  -70,  -23,  -70,
 /*   190 */   -70,  -70,  185,  -70,  -70,  -70,  -70,  -69,  -70,  -70,
 /*   200 */     9,  -70,  -70,  -70,  399,  -70,  -70,  -70,  -70,  -70,
 /*   210 */   -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,
 /*   220 */   -70,  -70,  -70,  -70,  -70,  -70,  -70, 1162,  -70,  -70,
 /*   230 */   -70,  162,  -70,  -70,  779,  -70,  -70,  473,  -70,  -70,
 /*   240 */   207,  -70,  -70,  214,  -70,  -70,  233,  -70,  -70,  240,
 /*   250 */   -70,  -70,  -70,  -70,  -70,  -70,  269,  -70,  788, 1046,
 /*   260 */   -70,  -70,  -70,  -70,  -70,  -70,  -70,  845, 1046,  -70,
 /*   270 */   -70,  -70,  904, 1046,  -70,  718,  -70,  -70,  637,  -70,
 /*   280 */   -70,  -70,  510,  -70,  -70,  -70,  467,  -70,  -70,  -70,
 /*   290 */   -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,  -70,
 /*   300 */   -70,  -70,  -70,  -70,  -70,  -70,  595,  -70, 1017,  -70,
 /*   310 */  1046,  -70,  -70,  -70,  671,  -70, 1046,  -70,  -70,  -70,
 /*   320 */   -70,  -70,  -70,  759,  -70, 1046,  -70,  -70,  -70,  -70,
 /*   330 */   -70,  -70,  702,  -70, 1046,  -70,  -51,    7,  -70,  -70,
 /*   340 */   -70,  -70,  213,  -70,  -70,  -70,  -70,  389,  -70,  -70,
 /*   350 */   -70,  -70,  -70,  -70,  -70,  -70,  525,  -70,  -70,  -70,
 /*   360 */   -70,  -70,  666,  -70,  -70,  -70,  -70,  -70,  781,  -70,
 /*   370 */   -70,  -70,  -70,  -70,  895,  -70,  -70,  -70,  908,  934,
 /*   380 */     7,  -70,    7,  -70,    7,  -70,  -70,  819,  -70,  -70,
 /*   390 */   -70,  -70,  -70,  -70,  733,  -70,  -70,  -70,  -70,  -70,
 /*   400 */   792,  -70,  -70,  -70,  -70,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   592,  592,  592,  407,  409,  410,  411,  592,  592,  412,
 /*    10 */   592,  592,  592,  592,  413,  417,  592,  592,  442,  592,
 /*    20 */   592,  592,  592,  592,  592,  446,  592,  450,  473,  475,
 /*    30 */   476,  477,  478,  479,  480,  481,  482,  483,  484,  485,
 /*    40 */   486,  487,  592,  488,  592,  489,  592,  592,  592,  592,
 /*    50 */   492,  592,  592,  592,  493,  592,  592,  592,  497,  592,
 /*    60 */   592,  592,  592,  499,  592,  592,  592,  592,  592,  592,
 /*    70 */   500,  592,  592,  502,  592,  503,  592,  592,  505,  532,
 /*    80 */   592,  592,  533,  592,  592,  534,  592,  592,  592,  587,
 /*    90 */   592,  588,  592,  589,  536,  592,  537,  592,  538,  592,
 /*   100 */   539,  592,  540,  592,  546,  592,  547,  592,  548,  592,
 /*   110 */   592,  541,  552,  592,  542,  592,  571,  592,  549,  592,
 /*   120 */   592,  592,  550,  592,  551,  553,  554,  555,  556,  557,
 /*   130 */   558,  592,  559,  592,  560,  592,  572,  592,  592,  574,
 /*   140 */   579,  580,  581,  582,  583,  584,  585,  586,  575,  580,
 /*   150 */   582,  581,  573,  592,  561,  592,  562,  592,  563,  592,
 /*   160 */   566,  569,  570,  592,  567,  568,  592,  592,  543,  592,
 /*   170 */   544,  592,  545,  592,  592,  564,  565,  535,  592,  592,
 /*   180 */   592,  506,  592,  592,  592,  507,  592,  592,  592,  508,
 /*   190 */   592,  592,  592,  509,  510,  511,  504,  592,  592,  512,
 /*   200 */   592,  513,  515,  514,  592,  592,  516,  592,  592,  592,
 /*   210 */   592,  517,  592,  592,  592,  518,  592,  592,  519,  592,
 /*   220 */   592,  520,  592,  592,  521,  592,  592,  592,  592,  592,
 /*   230 */   522,  592,  592,  523,  592,  528,  530,  592,  531,  529,
 /*   240 */   592,  592,  524,  592,  592,  525,  592,  592,  526,  592,
 /*   250 */   592,  527,  591,  474,  592,  592,  592,  592,  592,  592,
 /*   260 */   501,  498,  592,  496,  494,  490,  592,  592,  592,  495,
 /*   270 */   491,  592,  592,  592,  451,  592,  458,  460,  592,  463,
 /*   280 */   592,  461,  592,  464,  592,  462,  592,  465,  592,  592,
 /*   290 */   592,  592,  592,  592,  592,  592,  592,  459,  592,  592,
 /*   300 */   592,  447,  592,  592,  592,  592,  592,  592,  592,  448,
 /*   310 */   592,  452,  592,  592,  592,  449,  592,  453,  592,  592,
 /*   320 */   592,  592,  592,  592,  455,  592,  457,  592,  592,  592,
 /*   330 */   592,  592,  592,  454,  592,  456,  592,  592,  440,  592,
 /*   340 */   592,  592,  592,  592,  443,  592,  592,  592,  592,  444,
 /*   350 */   445,  430,  592,  592,  592,  432,  592,  592,  435,  592,
 /*   360 */   592,  437,  592,  592,  439,  592,  592,  433,  592,  592,
 /*   370 */   434,  592,  592,  436,  592,  592,  438,  441,  592,  592,
 /*   380 */   592,  592,  592,  431,  592,  592,  592,  592,  592,  414,
 /*   390 */   418,  592,  592,  592,  592,  592,  415,  419,  592,  592,
 /*   400 */   592,  592,  416,  420,  408,
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
  "FETCH",         "TYPEOF",        "INSTANCEOF",    "NEW",         
  "NOT",           "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",  "EXTENDS",     
  "ABSTRACT",      "COMMENT",       "PUBLIC",        "ASSIGN",      
  "PROTECTED",     "CONST",         "FUNCTION",      "PARENTHESES_OPEN",
  "PARENTHESES_CLOSE",  "TYPE_INTEGER",  "TYPE_DOUBLE",   "TYPE_BOOL",   
  "TYPE_STRING",   "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",       
  "CONTINUE",      "IF",            "ELSE",          "LOOP",        
  "WHILE",         "FOR",           "IN",            "LET",         
  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "INCR",        
  "DECR",          "ECHO",          "RETURN",        "UNSET",       
  "INTEGER",       "STRING",        "THROW",         "DOUBLE",      
  "NULL",          "TRUE",          "FALSE",         "COLON",       
  "error",         "program",       "xx_language",   "xx_top_statement_list",
  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",  "xx_comment",  
  "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",
  "xx_class_property_definition",  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_parameter_list",  "xx_statement_list",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_unset_statement",
  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",
  "xx_while_statement",  "xx_for_statement",  "xx_eval_expr",  "xx_common_expr",
  "xx_let_assignments",  "xx_let_assignment",  "xx_assign_expr",  "xx_echo_expressions",
  "xx_echo_expression",  "xx_call_parameters",  "xx_declare_variable_list",  "xx_declare_variable",
  "xx_isset_expr",  "xx_array_list",  "xx_mcall_expr",  "xx_call_parameter",
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
 /*  67 */ "xx_parameter_type ::= TYPE_VAR",
 /*  68 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  69 */ "xx_statement_list ::= xx_statement",
 /*  70 */ "xx_statement ::= xx_let_statement",
 /*  71 */ "xx_statement ::= xx_if_statement",
 /*  72 */ "xx_statement ::= xx_loop_statement",
 /*  73 */ "xx_statement ::= xx_echo_statement",
 /*  74 */ "xx_statement ::= xx_return_statement",
 /*  75 */ "xx_statement ::= xx_unset_statement",
 /*  76 */ "xx_statement ::= xx_throw_statement",
 /*  77 */ "xx_statement ::= xx_declare_statement",
 /*  78 */ "xx_statement ::= xx_break_statement",
 /*  79 */ "xx_statement ::= xx_continue_statement",
 /*  80 */ "xx_statement ::= xx_while_statement",
 /*  81 */ "xx_statement ::= xx_for_statement",
 /*  82 */ "xx_statement ::= xx_comment",
 /*  83 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  84 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  85 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  86 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  87 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  88 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  89 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  90 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  91 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /*  92 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  93 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  94 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  95 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  96 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  97 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /*  98 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /*  99 */ "xx_let_assignments ::= xx_let_assignment",
 /* 100 */ "xx_let_assignment ::= IDENTIFIER ASSIGN xx_assign_expr",
 /* 101 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER ASSIGN xx_assign_expr",
 /* 102 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 103 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 104 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE ASSIGN xx_assign_expr",
 /* 105 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 106 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 107 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 108 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 109 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 110 */ "xx_echo_expression ::= xx_common_expr",
 /* 111 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 112 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 113 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 114 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 115 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 116 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 117 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 118 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 119 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 120 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 121 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 122 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 123 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 124 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 125 */ "xx_declare_variable ::= IDENTIFIER",
 /* 126 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 127 */ "xx_assign_expr ::= xx_common_expr",
 /* 128 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 129 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 130 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 131 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 132 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 133 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 134 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 135 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 136 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 137 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 138 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 139 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 140 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 141 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 142 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 143 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 144 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 145 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 146 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 147 */ "xx_common_expr ::= IDENTIFIER",
 /* 148 */ "xx_common_expr ::= INTEGER",
 /* 149 */ "xx_common_expr ::= STRING",
 /* 150 */ "xx_common_expr ::= DOUBLE",
 /* 151 */ "xx_common_expr ::= NULL",
 /* 152 */ "xx_common_expr ::= TRUE",
 /* 153 */ "xx_common_expr ::= FALSE",
 /* 154 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 155 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 156 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 157 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 158 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 159 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 160 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 161 */ "xx_common_expr ::= xx_mcall_expr",
 /* 162 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 163 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 164 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 165 */ "xx_call_parameters ::= xx_call_parameter",
 /* 166 */ "xx_call_parameter ::= xx_common_expr",
 /* 167 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 168 */ "xx_array_list ::= xx_array_item",
 /* 169 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 170 */ "xx_array_item ::= xx_array_value",
 /* 171 */ "xx_array_key ::= IDENTIFIER",
 /* 172 */ "xx_array_key ::= STRING",
 /* 173 */ "xx_array_key ::= INTEGER",
 /* 174 */ "xx_array_value ::= xx_literal_expr",
 /* 175 */ "xx_array_value ::= IDENTIFIER",
 /* 176 */ "xx_literal_expr ::= INTEGER",
 /* 177 */ "xx_literal_expr ::= STRING",
 /* 178 */ "xx_literal_expr ::= DOUBLE",
 /* 179 */ "xx_literal_expr ::= NULL",
 /* 180 */ "xx_literal_expr ::= FALSE",
 /* 181 */ "xx_literal_expr ::= TRUE",
 /* 182 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 183 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 184 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 185 */ "xx_eval_expr ::= xx_common_expr",
 /* 186 */ "xx_comment ::= COMMENT",
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
    case 63:
// 702 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1585 "parser.c"
      break;
    case 66:
// 715 "parser.lemon"
{ json_object_put((yypminor->yy55)); }
// 1590 "parser.c"
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
  { 65, 1 },
  { 66, 1 },
  { 67, 2 },
  { 67, 1 },
  { 68, 1 },
  { 68, 1 },
  { 68, 1 },
  { 69, 3 },
  { 70, 5 },
  { 70, 7 },
  { 70, 6 },
  { 70, 8 },
  { 70, 4 },
  { 70, 6 },
  { 70, 5 },
  { 70, 7 },
  { 72, 1 },
  { 72, 1 },
  { 72, 1 },
  { 72, 2 },
  { 72, 2 },
  { 72, 2 },
  { 72, 2 },
  { 72, 3 },
  { 72, 3 },
  { 73, 2 },
  { 73, 1 },
  { 76, 4 },
  { 76, 3 },
  { 76, 5 },
  { 76, 6 },
  { 76, 3 },
  { 76, 4 },
  { 76, 5 },
  { 76, 6 },
  { 74, 2 },
  { 74, 1 },
  { 75, 2 },
  { 78, 6 },
  { 78, 5 },
  { 75, 1 },
  { 79, 7 },
  { 79, 8 },
  { 79, 8 },
  { 79, 9 },
  { 79, 8 },
  { 79, 9 },
  { 79, 9 },
  { 79, 10 },
  { 79, 7 },
  { 79, 8 },
  { 79, 8 },
  { 79, 9 },
  { 80, 3 },
  { 80, 1 },
  { 82, 1 },
  { 82, 2 },
  { 82, 2 },
  { 82, 3 },
  { 82, 4 },
  { 82, 4 },
  { 84, 3 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 83, 1 },
  { 81, 2 },
  { 81, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 85, 1 },
  { 94, 2 },
  { 95, 2 },
  { 87, 4 },
  { 87, 7 },
  { 87, 5 },
  { 87, 9 },
  { 87, 8 },
  { 87, 8 },
  { 88, 3 },
  { 88, 4 },
  { 96, 4 },
  { 96, 5 },
  { 97, 7 },
  { 97, 9 },
  { 86, 3 },
  { 100, 3 },
  { 100, 1 },
  { 101, 3 },
  { 101, 5 },
  { 101, 7 },
  { 101, 5 },
  { 101, 6 },
  { 101, 2 },
  { 101, 2 },
  { 89, 3 },
  { 103, 3 },
  { 103, 1 },
  { 104, 1 },
  { 90, 3 },
  { 91, 5 },
  { 91, 6 },
  { 91, 6 },
  { 91, 6 },
  { 92, 3 },
  { 92, 7 },
  { 93, 3 },
  { 93, 3 },
  { 93, 3 },
  { 93, 3 },
  { 93, 3 },
  { 106, 3 },
  { 106, 1 },
  { 107, 1 },
  { 107, 3 },
  { 102, 1 },
  { 99, 2 },
  { 99, 2 },
  { 99, 3 },
  { 99, 3 },
  { 99, 3 },
  { 99, 3 },
  { 99, 3 },
  { 99, 3 },
  { 99, 3 },
  { 99, 3 },
  { 99, 4 },
  { 99, 4 },
  { 99, 4 },
  { 99, 3 },
  { 99, 3 },
  { 99, 3 },
  { 99, 2 },
  { 99, 4 },
  { 99, 2 },
  { 99, 1 },
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
  { 99, 1 },
  { 110, 6 },
  { 110, 5 },
  { 105, 3 },
  { 105, 1 },
  { 111, 1 },
  { 109, 3 },
  { 109, 1 },
  { 112, 3 },
  { 112, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 114, 1 },
  { 114, 1 },
  { 77, 1 },
  { 77, 1 },
  { 77, 1 },
  { 77, 1 },
  { 77, 1 },
  { 77, 1 },
  { 108, 4 },
  { 108, 4 },
  { 108, 4 },
  { 98, 1 },
  { 71, 1 },
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
	status->ret = yymsp[0].minor.yy55;
}
// 1994 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
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
      case 82:
      case 127:
      case 161:
      case 166:
      case 174:
      case 185:
// 717 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;
}
// 2022 "parser.c"
        break;
      case 2:
      case 25:
      case 35:
      case 37:
      case 68:
// 721 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-1].minor.yy55, yymsp[0].minor.yy55);
}
// 2033 "parser.c"
        break;
      case 3:
      case 26:
      case 36:
      case 40:
      case 54:
      case 69:
      case 99:
      case 109:
      case 124:
      case 165:
      case 168:
// 725 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(NULL, yymsp[0].minor.yy55);
}
// 2050 "parser.c"
        break;
      case 7:
// 741 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2059 "parser.c"
        break;
      case 8:
// 745 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2069 "parser.c"
        break;
      case 9:
// 749 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2080 "parser.c"
        break;
      case 10:
// 753 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, 1, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2091 "parser.c"
        break;
      case 11:
// 757 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-7].minor);
  yy_destructor(20,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2103 "parser.c"
        break;
      case 12:
// 761 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2113 "parser.c"
        break;
      case 13:
// 765 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2124 "parser.c"
        break;
      case 14:
// 769 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2135 "parser.c"
        break;
      case 15:
// 773 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(24,&yymsp[-6].minor);
  yy_destructor(20,&yymsp[-5].minor);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2147 "parser.c"
        break;
      case 16:
// 777 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
}
// 2154 "parser.c"
        break;
      case 17:
// 781 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 2161 "parser.c"
        break;
      case 18:
// 785 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, NULL, status->scanner_state);
}
// 2168 "parser.c"
        break;
      case 19:
// 789 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
}
// 2175 "parser.c"
        break;
      case 20:
// 793 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 2182 "parser.c"
        break;
      case 21:
// 797 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2189 "parser.c"
        break;
      case 22:
// 801 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2196 "parser.c"
        break;
      case 23:
// 805 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2203 "parser.c"
        break;
      case 24:
// 809 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, yymsp[-2].minor.yy55, status->scanner_state);
}
// 2210 "parser.c"
        break;
      case 27:
// 821 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2219 "parser.c"
        break;
      case 28:
// 825 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2228 "parser.c"
        break;
      case 29:
// 829 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2238 "parser.c"
        break;
      case 30:
// 833 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2248 "parser.c"
        break;
      case 31:
// 837 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2257 "parser.c"
        break;
      case 32:
// 841 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2266 "parser.c"
        break;
      case 33:
// 845 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2276 "parser.c"
        break;
      case 34:
// 849 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2286 "parser.c"
        break;
      case 38:
// 865 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2296 "parser.c"
        break;
      case 39:
// 869 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2306 "parser.c"
        break;
      case 41:
// 877 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2319 "parser.c"
        break;
      case 42:
// 881 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2332 "parser.c"
        break;
      case 43:
// 885 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2345 "parser.c"
        break;
      case 44:
// 889 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, yymsp[-3].minor.yy55, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2358 "parser.c"
        break;
      case 45:
// 893 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2371 "parser.c"
        break;
      case 46:
// 897 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2384 "parser.c"
        break;
      case 47:
// 901 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2397 "parser.c"
        break;
      case 48:
// 905 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PUBLIC, yymsp[-6].minor.yy0, yymsp[-4].minor.yy55, yymsp[-1].minor.yy55, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2410 "parser.c"
        break;
      case 49:
// 909 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2423 "parser.c"
        break;
      case 50:
// 913 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2436 "parser.c"
        break;
      case 51:
// 917 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2449 "parser.c"
        break;
      case 52:
// 921 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_method(XX_T_PROTECTED, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy55, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2462 "parser.c"
        break;
      case 53:
      case 98:
      case 108:
      case 123:
      case 164:
      case 167:
// 925 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-2].minor.yy55, yymsp[0].minor.yy55);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2475 "parser.c"
        break;
      case 55:
// 933 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2482 "parser.c"
        break;
      case 56:
// 937 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2489 "parser.c"
        break;
      case 57:
// 941 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-1].minor.yy55, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2496 "parser.c"
        break;
      case 58:
// 945 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2504 "parser.c"
        break;
      case 59:
// 949 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-3].minor.yy55, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2512 "parser.c"
        break;
      case 60:
// 953 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-3].minor.yy55, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2520 "parser.c"
        break;
      case 61:
// 957 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2529 "parser.c"
        break;
      case 62:
// 961 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(33,&yymsp[0].minor);
}
// 2537 "parser.c"
        break;
      case 63:
// 965 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(34,&yymsp[0].minor);
}
// 2545 "parser.c"
        break;
      case 64:
// 969 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(35,&yymsp[0].minor);
}
// 2553 "parser.c"
        break;
      case 65:
// 973 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(36,&yymsp[0].minor);
}
// 2561 "parser.c"
        break;
      case 66:
// 977 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(37,&yymsp[0].minor);
}
// 2569 "parser.c"
        break;
      case 67:
// 981 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(38,&yymsp[0].minor);
}
// 2577 "parser.c"
        break;
      case 83:
// 1045 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2586 "parser.c"
        break;
      case 84:
// 1049 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2595 "parser.c"
        break;
      case 85:
// 1053 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-2].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2605 "parser.c"
        break;
      case 86:
// 1057 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-5].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(21,&yymsp[-4].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2618 "parser.c"
        break;
      case 87:
// 1061 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2628 "parser.c"
        break;
      case 88:
// 1065 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-7].minor.yy55, yymsp[-5].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(41,&yymsp[-8].minor);
  yy_destructor(21,&yymsp[-6].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2641 "parser.c"
        break;
      case 89:
// 1069 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-6].minor.yy55, yymsp[-4].minor.yy55, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2654 "parser.c"
        break;
      case 90:
// 1073 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-6].minor.yy55, NULL, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(21,&yymsp[-5].minor);
  yy_destructor(22,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2667 "parser.c"
        break;
      case 91:
// 1077 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2677 "parser.c"
        break;
      case 92:
// 1081 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_loop_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2687 "parser.c"
        break;
      case 93:
// 1085 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_while_statement(yymsp[-2].minor.yy55, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2697 "parser.c"
        break;
      case 94:
// 1089 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_while_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2707 "parser.c"
        break;
      case 95:
// 1093 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_for_statement(yymsp[-3].minor.yy55, NULL, yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(45,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2718 "parser.c"
        break;
      case 96:
// 1097 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_for_statement(yymsp[-3].minor.yy55, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(45,&yymsp[-8].minor);
  yy_destructor(1,&yymsp[-6].minor);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2730 "parser.c"
        break;
      case 97:
// 1101 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2739 "parser.c"
        break;
      case 100:
// 1114 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2747 "parser.c"
        break;
      case 101:
// 1119 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2756 "parser.c"
        break;
      case 102:
// 1124 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property-append", yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2767 "parser.c"
        break;
      case 103:
// 1129 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2777 "parser.c"
        break;
      case 104:
// 1134 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2787 "parser.c"
        break;
      case 105:
// 1139 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[0].minor);
}
// 2795 "parser.c"
        break;
      case 106:
// 1144 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[0].minor);
}
// 2803 "parser.c"
        break;
      case 107:
// 1148 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_echo_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2812 "parser.c"
        break;
      case 110:
// 1160 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;;
}
// 2819 "parser.c"
        break;
      case 111:
// 1165 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_return_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2828 "parser.c"
        break;
      case 112:
// 1170 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2842 "parser.c"
        break;
      case 113:
// 1179 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2857 "parser.c"
        break;
      case 114:
// 1188 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2872 "parser.c"
        break;
      case 115:
// 1197 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2887 "parser.c"
        break;
      case 116:
// 1206 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2896 "parser.c"
        break;
      case 117:
// 1211 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy55, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(15,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2908 "parser.c"
        break;
      case 118:
// 1215 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2917 "parser.c"
        break;
      case 119:
// 1219 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2926 "parser.c"
        break;
      case 120:
// 1223 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2935 "parser.c"
        break;
      case 121:
// 1227 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2944 "parser.c"
        break;
      case 122:
// 1231 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2953 "parser.c"
        break;
      case 125:
// 1243 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2960 "parser.c"
        break;
      case 126:
// 1247 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2968 "parser.c"
        break;
      case 128:
// 1255 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 2976 "parser.c"
        break;
      case 129:
// 1259 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("isset", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 2984 "parser.c"
        break;
      case 130:
// 1263 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("equals", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 2992 "parser.c"
        break;
      case 131:
// 1267 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not-equals", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3000 "parser.c"
        break;
      case 132:
// 1271 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("identical", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3008 "parser.c"
        break;
      case 133:
// 1275 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not-identical", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3016 "parser.c"
        break;
      case 134:
// 1279 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("less", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3024 "parser.c"
        break;
      case 135:
// 1283 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("greater", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3032 "parser.c"
        break;
      case 136:
// 1287 "parser.lemon"
{
	yygotominor.yy55 = yymsp[-1].minor.yy55;
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3041 "parser.c"
        break;
      case 137:
// 1291 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-1].minor);
}
// 3049 "parser.c"
        break;
      case 138:
      case 182:
// 1295 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3059 "parser.c"
        break;
      case 139:
      case 183:
// 1300 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3069 "parser.c"
        break;
      case 140:
      case 184:
// 1305 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3079 "parser.c"
        break;
      case 141:
// 1310 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("add", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3087 "parser.c"
        break;
      case 142:
// 1315 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("sub", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3095 "parser.c"
        break;
      case 143:
// 1320 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("concat", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3103 "parser.c"
        break;
      case 144:
// 1325 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("instanceof", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3111 "parser.c"
        break;
      case 145:
// 1330 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(1,&yymsp[-1].minor);
}
// 3120 "parser.c"
        break;
      case 146:
// 1335 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("typeof", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3128 "parser.c"
        break;
      case 147:
      case 171:
      case 175:
// 1340 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3137 "parser.c"
        break;
      case 148:
      case 173:
      case 176:
// 1345 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3146 "parser.c"
        break;
      case 149:
      case 172:
      case 177:
// 1350 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3155 "parser.c"
        break;
      case 150:
      case 178:
// 1355 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3163 "parser.c"
        break;
      case 151:
      case 179:
// 1360 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[0].minor);
}
// 3172 "parser.c"
        break;
      case 152:
      case 181:
// 1365 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[0].minor);
}
// 3181 "parser.c"
        break;
      case 153:
      case 180:
// 1370 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[0].minor);
}
// 3190 "parser.c"
        break;
      case 154:
// 1375 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3199 "parser.c"
        break;
      case 155:
// 1380 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array", yymsp[-1].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3208 "parser.c"
        break;
      case 156:
// 1385 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3216 "parser.c"
        break;
      case 157:
// 1390 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3226 "parser.c"
        break;
      case 158:
// 1395 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3236 "parser.c"
        break;
      case 159:
// 1400 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3245 "parser.c"
        break;
      case 160:
// 1405 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3254 "parser.c"
        break;
      case 162:
// 1415 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3264 "parser.c"
        break;
      case 163:
// 1420 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3274 "parser.c"
        break;
      case 169:
// 1444 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_array_item(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3282 "parser.c"
        break;
      case 170:
// 1448 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_array_item(NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 3289 "parser.c"
        break;
      case 186:
// 1515 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3296 "parser.c"
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

// 3372 "parser.c"
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
			case XX_T_FETCH:
				xx_(xx_parser, XX_FETCH, NULL, parser_status);
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
