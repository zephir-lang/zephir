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
#define YYNSTATE 406
#define YYNRULE 188
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
 /*     0 */   310,  255,  235,  253,  595,    1,    2,  405,    4,    5,
 /*    10 */     6,  232,  241,  247,  244,   88,  250,   42,   44,   46,
 /*    20 */   246,   55,   59,   64,  114,   71,   50,    7,   61,  253,
 /*    30 */    10,  198,  205,  208,  392,  253,  223,  232,  241,  247,
 /*    40 */   244,   11,  250,   42,   44,   46,   66,   55,   59,   64,
 /*    50 */   191,   71,  266,   90,   92,  253,  203,  198,  205,  208,
 /*    60 */   199,  204,  223,  232,  241,  247,  244,  177,  250,   42,
 /*    70 */    44,   46,  362,   55,   59,   64,   79,   71,  312,   78,
 /*    80 */   363,  253,  188,  198,  205,  208,  361,  177,  223,  232,
 /*    90 */   241,  247,  244,  214,  250,   42,   44,   46,  320,   55,
 /*   100 */    59,   64,  338,   71,  316,  339,  351,  253,   94,  198,
 /*   110 */   205,  208,   47,  263,  223,  232,  241,  247,  244,  177,
 /*   120 */   250,   42,   44,   46,  177,   55,   59,   64,  115,   71,
 /*   130 */   264,  217,  220,  253,  347,  198,  205,  208,   60,  263,
 /*   140 */   223,  232,  241,  247,  244,  131,  250,   42,   44,   46,
 /*   150 */   177,   55,   59,   64,   79,   71,  318,  182,  235,  253,
 /*   160 */   148,  198,  205,  208,  348,  177,  223,  232,  241,  247,
 /*   170 */   244,  177,  250,   42,   44,   46,  252,   55,   59,   64,
 /*   180 */   181,   71,   54,  133,  135,  253,  178,  198,  205,  208,
 /*   190 */   245,  240,  223,  232,  241,  247,  244,  177,  250,   42,
 /*   200 */    44,   46,  183,   55,   59,   64,   79,   71,   58,  194,
 /*   210 */   354,  253,  360,  198,  205,  208,  367,  177,  223,  232,
 /*   220 */   241,  247,  244,  355,  250,   42,   44,   46,   20,   55,
 /*   230 */    59,   64,  267,   71,  325,  305,  235,  253,  203,  198,
 /*   240 */   205,  208,   91,  202,  223,  232,  241,  247,  244,  177,
 /*   250 */   250,   42,   44,   46,  243,   55,   59,   64,  206,   71,
 /*   260 */   327,  251,  240,  253,  349,  198,  205,  208,  350,  177,
 /*   270 */   223,  232,  241,  247,  244,  118,  250,   42,   44,   46,
 /*   280 */   368,   55,   59,   64,  227,   71,   27,   20,  369,  253,
 /*   290 */    96,  198,  205,  208,  242,  240,  223,  232,  241,  247,
 /*   300 */   244,  177,  250,   42,   44,   46,  117,   55,   59,   64,
 /*   310 */   142,   71,  265,   48,  201,  253,  164,  198,  205,  208,
 /*   320 */    89,  177,  223,  232,  241,  247,  244,  373,  250,   42,
 /*   330 */    44,   46,  200,   55,   59,   64,  110,   71,  334,  329,
 /*   340 */    74,  253,   43,  198,  205,  208,   87,  177,  223,  232,
 /*   350 */   241,  247,  244,  163,  250,   42,   44,   46,   73,   55,
 /*   360 */    59,   64,  258,   71,   70,  210,  213,  253,  257,  198,
 /*   370 */   205,  208,  256,  177,  223,  232,  241,  247,  244,  118,
 /*   380 */   250,   42,   44,   46,  228,   55,   59,   64,   98,   71,
 /*   390 */   336,   72,  197,  253,  276,  198,  205,  208,  358,  177,
 /*   400 */   223,  232,  241,  247,  244,  118,  250,   42,   44,   46,
 /*   410 */   230,   55,   59,   64,  273,   71,  275,  233,  240,  253,
 /*   420 */   359,  198,  205,  208,   86,  235,  223,  232,  241,  247,
 /*   430 */   244,  237,  250,   42,   44,   46,  139,   55,   59,   64,
 /*   440 */    67,   71,   25,  249,  395,  253,  399,  198,  205,  208,
 /*   450 */   364,  177,  223,  232,  241,  247,  244,  276,  250,   42,
 /*   460 */    44,   46,  356,   55,   59,   64,   85,   71,   63,  226,
 /*   470 */   357,  253,  224,  198,  205,  208,  365,  235,  223,  232,
 /*   480 */   241,  247,  244,    8,  250,   42,   44,   46,  314,   55,
 /*   490 */    59,   64,   65,   71,  270,  234,  304,  253,  319,  198,
 /*   500 */   205,  208,  248,  240,  223,  232,  241,  247,  244,  118,
 /*   510 */   250,   42,   44,   46,  374,   55,   59,   64,  108,   71,
 /*   520 */   262,  120,  375,  253,  159,  198,  205,  208,  209,  177,
 /*   530 */   223,  232,  241,  247,  244,  146,  250,   42,   44,   46,
 /*   540 */   176,   55,   59,   64,   14,   71,  271,  371,   12,  253,
 /*   550 */   386,  198,  205,  208,  370,  119,  223,  232,  241,  247,
 /*   560 */   244,  276,  250,   42,   44,   46,  211,   55,   59,   64,
 /*   570 */   104,   71,  261,  158,  212,  253,    9,  198,  205,  208,
 /*   580 */    18,  177,  223,  232,  241,  247,  244,  377,  250,   42,
 /*   590 */    44,   46,  300,   55,   59,   64,  272,   71,  298,  281,
 /*   600 */   285,  253,  100,  198,  205,  208,  376,  231,  223,  232,
 /*   610 */   241,  247,  244,  177,  250,   42,   44,   46,  145,   55,
 /*   620 */    59,   64,  299,   71,  298,  281,  285,  215,  216,  198,
 /*   630 */   205,  208,  136,  102,  223,   84,  143,  147,  141,  171,
 /*   640 */    81,  144,  112,   41,  177,  396,   16,  379,  385,  384,
 /*   650 */   387,  378,  351,  311,  218,  109,   76,  254,   29,   30,
 /*   660 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   670 */   388,  137,  422,  155,   45,  353,  366,   41,  372,  346,
 /*   680 */   149,  150,  164,  151,  152,  153,  154,  260,  113,  130,
 /*   690 */   239,  254,   29,   30,   31,   32,   33,   34,   35,   36,
 /*   700 */    37,   38,   39,   40,  390,  219,   41,  238,  402,   16,
 /*   710 */   379,  385,  384,  221,  378,  351,   69,  160,  161,  166,
 /*   720 */   254,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*   730 */    38,   39,   40,   41,   80,   95,   99,  101,   97,   83,
 /*   740 */   103,  105,  107,  335,  103,  105,  107,  254,   29,   30,
 /*   750 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   760 */   277,  281,  285,   41,  111,  389,   16,  379,  385,  384,
 /*   770 */   393,  378,  351,   62,  394,  236,  222,  254,   29,   30,
 /*   780 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*   790 */   397,  132,   41,  106,   13,   16,  379,  385,  384,  302,
 /*   800 */   378,  351,   26,  305,  177,  301,  254,   29,   30,   31,
 /*   810 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*   820 */    80,   95,   99,  101,   97,   83,  103,  105,  107,   49,
 /*   830 */   306,  307,   22,  254,   29,   30,   31,   32,   33,   34,
 /*   840 */    35,   36,   37,   38,   39,   40,  309,   56,  173,   41,
 /*   850 */    79,  122,  123,  186,  124,  125,  127,  126,  473,  274,
 /*   860 */   172,  177,   77,  254,   29,   30,   31,   32,   33,   34,
 /*   870 */    35,   36,   37,   38,   39,   40,  225,  289,   41,    3,
 /*   880 */     4,    5,    6,  179,  187,  472,  195,  196,  317,  471,
 /*   890 */   180,  278,  254,   29,   30,   31,   32,   33,   34,   35,
 /*   900 */    36,   37,   38,   39,   40,   41,  292,  293,  294,  295,
 /*   910 */   296,  297,  337,   17,  352,   57,  378,  351,  315,  254,
 /*   920 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   930 */    39,   40,  470,   15,  469,   41,  353,  366,  580,  372,
 /*   940 */   346,  380,  468,  383,  384,  326,  339,  351,  579,  254,
 /*   950 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*   960 */    39,   40,  426,  291,   41,  340,   19,  428,  328,  346,
 /*   970 */   303,   19,  467,  328,   53,  400,  320,  321,  254,   29,
 /*   980 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*   990 */    40,   41,  423,  381,  352,  353,  366,  351,  372,  346,
 /*  1000 */   304,  269,  319,  341,  322,  254,   29,   30,   31,   32,
 /*  1010 */    33,   34,   35,   36,   37,   38,   39,   40,  184,  391,
 /*  1020 */   323,   41,  353,  366,  578,  372,  346,  185,  430,  324,
 /*  1030 */   167,  303,   19,  290,  328,   28,   29,   30,   31,   32,
 /*  1040 */    33,   34,   35,   36,   37,   38,   39,   40,   80,   95,
 /*  1050 */    99,  101,   97,   83,  103,  105,  107,   80,   95,   99,
 /*  1060 */   101,   97,   83,  103,  105,  107,  288,  592,  169,  168,
 /*  1070 */   287,  124,  125,  127,  126,  354,  259,  360,  341,   80,
 /*  1080 */    95,   99,  101,   97,   83,  103,  105,  107,  121,  286,
 /*  1090 */   284,  404,  407,   75,  353,  366,  207,  372,  346,   80,
 /*  1100 */    95,   99,  101,   97,   83,  103,  105,  107,  289,    7,
 /*  1110 */   329,  330,   10,  283,  164,  331,  392,  253,   68,  120,
 /*  1120 */   121,  313,  278,  298,  281,  285,  122,  123,  189,  124,
 /*  1130 */   125,  127,  126,  282,  128,  333,  308,  292,  293,  294,
 /*  1140 */   295,  296,  297,  289,   82,  165,  157,  138,  332,  170,
 /*  1150 */   161,  166,  280,  129,  401,  177,  279,  278,  122,  123,
 /*  1160 */    24,  124,  125,  127,  126,  289,   21,   52,  162,  192,
 /*  1170 */   406,  134,  292,  293,  294,  295,  296,  297,  121,  278,
 /*  1180 */   120,   93,  193,  122,  123,   51,  124,  125,  127,  126,
 /*  1190 */   342,  167,  140,   23,  292,  293,  294,  295,  296,  297,
 /*  1200 */   398,  343,  403,  353,  366,  268,  372,  346,  175,  427,
 /*  1210 */   121,  344,  382,  366,  129,  372,  122,  123,  345,  124,
 /*  1220 */   125,  127,  126,  156,  174,  305,  305,  305,  305,  169,
 /*  1230 */   168,  305,  124,  125,  127,  126,  424,  305,  305,  303,
 /*  1240 */    19,  305,  328,  120,  305,  305,  305,  305,  122,  123,
 /*  1250 */   305,  124,  125,  127,  126,  425,  120,  305,  303,   19,
 /*  1260 */   305,  328,  429,  305,   79,  303,   19,  190,  328,  305,
 /*  1270 */   305,  229,  305,  305,  305,  177,  305,  129,  305,  305,
 /*  1280 */   305,  305,  305,  305,  116,  305,  305,  305,  305,  305,
 /*  1290 */   129,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    22,    1,    1,   25,   65,   66,   67,   68,   69,   70,
 /*    10 */    71,   33,   34,   35,   36,   18,   38,   39,   40,   41,
 /*    20 */    19,   43,   44,   45,   18,   47,   22,   17,   21,   25,
 /*    30 */    20,   53,   54,   55,   24,   25,   58,   33,   34,   35,
 /*    40 */    36,   18,   38,   39,   40,   41,   46,   43,   44,   45,
 /*    50 */    18,   47,   22,   56,   57,   25,   99,   53,   54,   55,
 /*    60 */   103,  104,   58,   33,   34,   35,   36,  110,   38,   39,
 /*    70 */    40,   41,   19,   43,   44,   45,   99,   47,   22,  102,
 /*    80 */    27,   25,   50,   53,   54,   55,   18,  110,   58,   33,
 /*    90 */    34,   35,   36,   18,   38,   39,   40,   41,   30,   43,
 /*   100 */    44,   45,   75,   47,   22,   78,   79,   25,   99,   53,
 /*   110 */    54,   55,   98,   99,   58,   33,   34,   35,   36,  110,
 /*   120 */    38,   39,   40,   41,  110,   43,   44,   45,   31,   47,
 /*   130 */    22,   56,   57,   25,   18,   53,   54,   55,   98,   99,
 /*   140 */    58,   33,   34,   35,   36,   18,   38,   39,   40,   41,
 /*   150 */   110,   43,   44,   45,   99,   47,   22,  102,    1,   25,
 /*   160 */    99,   53,   54,   55,   27,  110,   58,   33,   34,   35,
 /*   170 */    36,  110,   38,   39,   40,   41,   19,   43,   44,   45,
 /*   180 */    27,   47,   22,   56,   57,   25,   99,   53,   54,   55,
 /*   190 */   106,  107,   58,   33,   34,   35,   36,  110,   38,   39,
 /*   200 */    40,   41,   49,   43,   44,   45,   99,   47,   22,  102,
 /*   210 */    26,   25,   28,   53,   54,   55,   18,  110,   58,   33,
 /*   220 */    34,   35,   36,   18,   38,   39,   40,   41,   30,   43,
 /*   230 */    44,   45,   42,   47,   22,   30,    1,   25,   99,   53,
 /*   240 */    54,   55,   50,  104,   58,   33,   34,   35,   36,  110,
 /*   250 */    38,   39,   40,   41,   19,   43,   44,   45,   99,   47,
 /*   260 */    22,  106,  107,   25,   77,   53,   54,   55,   19,  110,
 /*   270 */    58,   33,   34,   35,   36,    1,   38,   39,   40,   41,
 /*   280 */    19,   43,   44,   45,   18,   47,   22,   30,   27,   25,
 /*   290 */    99,   53,   54,   55,  106,  107,   58,   33,   34,   35,
 /*   300 */    36,  110,   38,   39,   40,   41,   32,   43,   44,   45,
 /*   310 */    99,   47,   22,   21,    1,   25,   77,   53,   54,   55,
 /*   320 */    50,  110,   58,   33,   34,   35,   36,   18,   38,   39,
 /*   330 */    40,   41,   19,   43,   44,   45,   99,   47,   22,   30,
 /*   340 */     1,   25,   19,   53,   54,   55,   49,  110,   58,   33,
 /*   350 */    34,   35,   36,  114,   38,   39,   40,   41,   19,   43,
 /*   360 */    44,   45,   99,   47,   22,   48,   49,   25,   46,   53,
 /*   370 */    54,   55,   18,  110,   58,   33,   34,   35,   36,    1,
 /*   380 */    38,   39,   40,   41,   31,   43,   44,   45,   99,   47,
 /*   390 */    22,  100,  101,   25,    1,   53,   54,   55,   77,  110,
 /*   400 */    58,   33,   34,   35,   36,    1,   38,   39,   40,   41,
 /*   410 */    32,   43,   44,   45,   21,   47,   22,  106,  107,   25,
 /*   420 */    19,   53,   54,   55,   18,    1,   58,   33,   34,   35,
 /*   430 */    36,   18,   38,   39,   40,   41,   32,   43,   44,   45,
 /*   440 */    99,   47,   22,   19,   21,   25,   23,   53,   54,   55,
 /*   450 */    77,  110,   58,   33,   34,   35,   36,    1,   38,   39,
 /*   460 */    40,   41,   19,   43,   44,   45,  108,   47,   22,   15,
 /*   470 */    27,   25,   18,   53,   54,   55,   19,    1,   58,   33,
 /*   480 */    34,   35,   36,   18,   38,   39,   40,   41,   32,   43,
 /*   490 */    44,   45,   18,   47,   22,   19,   26,   25,   28,   53,
 /*   500 */    54,   55,  106,  107,   58,   33,   34,   35,   36,    1,
 /*   510 */    38,   39,   40,   41,   19,   43,   44,   45,   99,   47,
 /*   520 */    22,   77,   27,   25,    1,   53,   54,   55,   18,  110,
 /*   530 */    58,   33,   34,   35,   36,  108,   38,   39,   40,   41,
 /*   540 */    32,   43,   44,   45,   22,   47,   22,   19,   21,   25,
 /*   550 */    23,   53,   54,   55,   77,  111,   58,   33,   34,   35,
 /*   560 */    36,    1,   38,   39,   40,   41,   18,   43,   44,   45,
 /*   570 */    99,   47,   22,   50,   19,   25,   19,   53,   54,   55,
 /*   580 */    79,  110,   58,   33,   34,   35,   36,   19,   38,   39,
 /*   590 */    40,   41,   32,   43,   44,   45,   80,   47,   82,   83,
 /*   600 */    84,   25,   99,   53,   54,   55,   77,   19,   58,   33,
 /*   610 */    34,   35,   36,  110,   38,   39,   40,   41,    1,   43,
 /*   620 */    44,   45,   80,   47,   82,   83,   84,   50,   19,   53,
 /*   630 */    54,   55,   50,   99,   58,   11,   12,   13,   14,   15,
 /*   640 */    16,   18,   18,   71,  110,   72,   73,   74,   75,   76,
 /*   650 */    18,   78,   79,   81,   50,   31,   18,   85,   86,   87,
 /*   660 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*   670 */    21,   31,   22,   49,   19,   25,   26,   71,   28,   29,
 /*   680 */    56,   57,   77,   59,   60,   61,   62,   81,   48,   49,
 /*   690 */    77,   85,   86,   87,   88,   89,   90,   91,   92,   93,
 /*   700 */    94,   95,   96,   97,   22,   19,   71,   27,   72,   73,
 /*   710 */    74,   75,   76,   50,   78,   79,   81,  112,  113,  114,
 /*   720 */    85,   86,   87,   88,   89,   90,   91,   92,   93,   94,
 /*   730 */    95,   96,   97,   71,    2,    3,    4,    5,    6,    7,
 /*   740 */     8,    9,   10,   81,    8,    9,   10,   85,   86,   87,
 /*   750 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*   760 */    82,   83,   84,   71,   32,   72,   73,   74,   75,   76,
 /*   770 */    20,   78,   79,   81,   18,  107,   19,   85,   86,   87,
 /*   780 */    88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
 /*   790 */    22,   50,   71,   99,   72,   73,   74,   75,   76,   22,
 /*   800 */    78,   79,   81,   30,  110,   21,   85,   86,   87,   88,
 /*   810 */    89,   90,   91,   92,   93,   94,   95,   96,   97,   71,
 /*   820 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   81,
 /*   830 */    18,   31,   31,   85,   86,   87,   88,   89,   90,   91,
 /*   840 */    92,   93,   94,   95,   96,   97,   21,   21,   31,   71,
 /*   850 */    99,   56,   57,  102,   59,   60,   61,   62,   18,   81,
 /*   860 */    18,  110,   27,   85,   86,   87,   88,   89,   90,   91,
 /*   870 */    92,   93,   94,   95,   96,   97,   19,    4,   71,   68,
 /*   880 */    69,   70,   71,   48,   49,   18,   51,   52,   81,   18,
 /*   890 */    18,   18,   85,   86,   87,   88,   89,   90,   91,   92,
 /*   900 */    93,   94,   95,   96,   97,   71,   33,   34,   35,   36,
 /*   910 */    37,   38,   74,   75,   76,   81,   78,   79,   21,   85,
 /*   920 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*   930 */    96,   97,   18,   22,   18,   71,   25,   26,   63,   28,
 /*   940 */    29,   73,   18,   75,   76,   81,   78,   79,   63,   85,
 /*   950 */    86,   87,   88,   89,   90,   91,   92,   93,   94,   95,
 /*   960 */    96,   97,   22,    5,   71,   25,   26,   22,   28,   29,
 /*   970 */    25,   26,   18,   28,   81,   18,   30,   18,   85,   86,
 /*   980 */    87,   88,   89,   90,   91,   92,   93,   94,   95,   96,
 /*   990 */    97,   71,   22,   75,   76,   25,   26,   79,   28,   29,
 /*  1000 */    26,   81,   28,   29,   31,   85,   86,   87,   88,   89,
 /*  1010 */    90,   91,   92,   93,   94,   95,   96,   97,   50,   22,
 /*  1020 */    32,   71,   25,   26,   63,   28,   29,   27,   22,   21,
 /*  1030 */    18,   25,   26,   18,   28,   85,   86,   87,   88,   89,
 /*  1040 */    90,   91,   92,   93,   94,   95,   96,   97,    2,    3,
 /*  1050 */     4,    5,    6,    7,    8,    9,   10,    2,    3,    4,
 /*  1060 */     5,    6,    7,    8,    9,   10,   77,   21,   56,   57,
 /*  1070 */    27,   59,   60,   61,   62,   26,   21,   28,   29,    2,
 /*  1080 */     3,    4,    5,    6,    7,    8,    9,   10,   18,   18,
 /*  1090 */    77,   22,    0,  101,   25,   26,   19,   28,   29,    2,
 /*  1100 */     3,    4,    5,    6,    7,    8,    9,   10,    4,   17,
 /*  1110 */    30,   18,   20,   27,   77,   31,   24,   25,   21,   77,
 /*  1120 */    18,   80,   18,   82,   83,   84,   56,   57,   27,   59,
 /*  1130 */    60,   61,   62,   18,   32,   21,   32,   33,   34,   35,
 /*  1140 */    36,   37,   38,    4,   99,   18,  109,  105,   32,  112,
 /*  1150 */   113,  114,   77,  111,   21,  110,   27,   18,   56,   57,
 /*  1160 */    21,   59,   60,   61,   62,    4,   18,   21,   63,   50,
 /*  1170 */     0,   50,   33,   34,   35,   36,   37,   38,   18,   18,
 /*  1180 */    77,   50,   27,   56,   57,   42,   59,   60,   61,   62,
 /*  1190 */    18,   18,   32,   32,   33,   34,   35,   36,   37,   38,
 /*  1200 */    22,   27,   22,   25,   26,   21,   28,   29,  105,   22,
 /*  1210 */    18,   77,   25,   26,  111,   28,   56,   57,   19,   59,
 /*  1220 */    60,   61,   62,   50,   32,  115,  115,  115,  115,   56,
 /*  1230 */    57,  115,   59,   60,   61,   62,   22,  115,  115,   25,
 /*  1240 */    26,  115,   28,   77,  115,  115,  115,  115,   56,   57,
 /*  1250 */   115,   59,   60,   61,   62,   22,   77,  115,   25,   26,
 /*  1260 */   115,   28,   22,  115,   99,   25,   26,  102,   28,  115,
 /*  1270 */   115,  105,  115,  115,  115,  110,  115,  111,  115,  115,
 /*  1280 */   115,  115,  115,  115,  105,  115,  115,  115,  115,  115,
 /*  1290 */   111,
};
#define YY_SHIFT_USE_DFLT (-23)
static short yy_shift_ofst[] = {
 /*     0 */    10, 1170, 1092,  -23,  -23,  -23,  -23,  465,  557,  -23,
 /*    10 */    23,  527,  911,  522,  -23,  -23,  650, 1233,  -23,  257,
 /*    20 */  1148,  801, 1161, 1139,  420,  -23,  264,  -23,  -23,  -23,
 /*    30 */   -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,
 /*    40 */   -23,  -23,  323,  -23,  655,  -23,  624,  292,   30,    4,
 /*    50 */  1143, 1146,  290,  160,  -23,  826,  108,  186,  -23,  624,
 /*    60 */     7,  498,  446,  -23,  474,    0,  624, 1097,  576,  342,
 /*    70 */   -23,  638,  339,  -23,  638,  -23,  835,  624,  -23,  818,
 /*    80 */   624,  624,  -23,  624,  406,  -23,  297,   -3,  270,  -23,
 /*    90 */   192,  -23, 1131,  -23,  736,  624,  736,  624,  736,  624,
 /*   100 */   736,  624,  736,  624,  -23,  624,  -23,  624,  -23,  624,
 /*   110 */   732,  -23,  640,    6,   97, 1102,  274,  -23, 1070,  -23,
 /*   120 */   -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,  -23,
 /*   130 */   127,  741,  -23, 1121,  -23,  582,  -23, 1160,  404,  -23,
 /*   140 */   -23,  624,  -23,  623,  617,  406,  -23,  624,  -23,  -23,
 /*   150 */   -23,  -23,  -23,  -23,  -23, 1173,  -23,  523,  -23, 1012,
 /*   160 */   -23, 1105, 1127,  -23,  -23,  -23,  -23,  961,  885,  875,
 /*   170 */   -23,  842,  817, 1192,  -23,  508,  -23,  -23,  736,  872,
 /*   180 */   153,  624,  -23,  968, 1000,  624,  -23,   32, 1101,  624,
 /*   190 */   -23, 1119, 1155,  624,  -23,  -23,  -23,  -23,  624,  313,
 /*   200 */   -23,  624,  -23,  818,  -23,  624, 1077,  -23,  510,  317,
 /*   210 */   548,  555,  -23,   75,  577,  609,  -23,  604,  686,  -23,
 /*   220 */   663,  757,  -23,  454,  857,  -23,  266,  353, 1070,  378,
 /*   230 */   588,  -23,  413,  476,  -23,  413,  -23,  680,  795,  -23,
 /*   240 */   -23,  413,  235,  -23,  413,    1,  -23,  413,  424,  -23,
 /*   250 */   413,  157,  -23,  -23,  -23,  354,  322,  624, 1055,  576,
 /*   260 */   550,  -23,  -23, 1046,  -23,  -23,  190, 1184,  524,  472,
 /*   270 */   -23,  -23,  393,  576,  394,  -23,  873,  -23, 1129,  795,
 /*   280 */   -23, 1115, 1086,  795,  -23, 1071, 1043,  795,  -23, 1015,
 /*   290 */   958,  954,  924,  916,  914,  871,  867,  840,  -23,  560,
 /*   300 */   784,  777,  -23,  470,  773,  812,  800, 1104,  825,  -22,
 /*   310 */   -23,   56,  -23,  456,  897,   82,  -23,  134,  -23,  946,
 /*   320 */   959,  973,  988, 1008,  212,  -23,  238,  -23, 1080, 1093,
 /*   330 */  1084, 1116, 1114,  316,  -23,  368,  -23,  940, 1240,  -23,
 /*   340 */   974, 1172, 1174,  795, 1199,  -23,  116,  137,  795,  249,
 /*   350 */   -23,  -23,  -23, 1049,  205,  443,  -23,  795,  401,  -23,
 /*   360 */    68,   53,  -23,  795,  457,  -23,  198,  261,  -23,  795,
 /*   370 */   528,  -23,  309,  495,  -23,  795,  568,  -23,  -23,  970,
 /*   380 */  1187, 1006,  184,  945,  -23, 1214,  632,  649,  997,  682,
 /*   390 */   -23,  -23,  750,  756,  423, 1178,  768,  -23,  -23,  957,
 /*   400 */  1133, 1069, 1180,  -23,  -23,  -23,
};
#define YY_REDUCE_USE_DFLT (-62)
static short yy_reduce_ofst[] = {
 /*     0 */   -61,  -62,  811,  -62,  -62,  -62,  -62,  -62,  -62,  -62,
 /*    10 */   -62,  -62,  722,  -62,  -62,  -62,  838,  501,  -62,  -62,
 /*    20 */   -62,  -62,  542,  516,  721,  -62,  950,  -62,  -62,  -62,
 /*    30 */   -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,
 /*    40 */   -62,  -62,  -62,  -62,  -62,  -62,   14,  -62,  748,  950,
 /*    50 */   -62,  -62,  893,  950,  -62,  -62,  834,  950,  -62,   40,
 /*    60 */   -62,  692,  950,  -62,  -62,  -62,  341,  -62,  635,  950,
 /*    70 */   -62,  291,  -62,  -62,  992,  -62,  -62,  -23,  -62,  -62,
 /*    80 */    87, 1045,  -62,    9,  358,  -62,  -62,  -62,  -62,  -62,
 /*    90 */   -62,  -62,  -62,  -62,  -62,  191,  -62,  289,  -62,  503,
 /*   100 */   -62,  534,  -62,  471,  -62,  694,  -62,  419,  -62,  237,
 /*   110 */   -62,  -62,  -62,  -62,  -62, 1179,  -62,  -62,  444,  -62,
 /*   120 */   -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,
 /*   130 */   -62,  -62,  -62,  -62,  -62,  -62,  -62, 1042,  -62,  -62,
 /*   140 */   -62,  211,  -62,  -62,  -62,  427,  -62,   61,  -62,  -62,
 /*   150 */   -62,  -62,  -62,  -62,  -62, 1037,  -62,  -62,  -62,  605,
 /*   160 */   -62,  -62,  239,  -62,  -62,  -62,  -62,  -62,  -62,  -62,
 /*   170 */   -62,  -62,  -62, 1103,  -62,  -62,  -62,  -62,  -62,  -62,
 /*   180 */   -62,   55,  -62,  -62,  -62,  751,  -62,  -62,  -62, 1165,
 /*   190 */   -62,  -62,  -62,  107,  -62,  -62,  -62,  -62,  -43,  -62,
 /*   200 */   -62,  139,  -62,  -62,  -62,  159,  -62,  -62,  -62,  -62,
 /*   210 */   -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,
 /*   220 */   -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62, 1166,  -62,
 /*   230 */   -62,  -62,  311,  -62,  -62,  668,  -62,  -62,  613,  -62,
 /*   240 */   -62,  188,  -62,  -62,   84,  -62,  -62,  396,  -62,  -62,
 /*   250 */   155,  -62,  -62,  -62,  -62,  -62,  -62,  263,  -62,  606,
 /*   260 */   950,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  920,  950,
 /*   270 */   -62,  -62,  -62,  778,  950,  -62,  678,  -62,  -62, 1075,
 /*   280 */   -62,  -62,  -62, 1013,  -62,  -62,  -62,  989,  -62,  -62,
 /*   290 */   -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,  -62,
 /*   300 */   -62,  -62,  -62,  -62,  -62,  -62,  -62, 1041,  -62,  572,
 /*   310 */   -62,  950,  -62,  -62,  -62,  807,  -62,  950,  -62,  -62,
 /*   320 */   -62,  -62,  -62,  -62,  864,  -62,  950,  -62,  -62,  -62,
 /*   330 */   -62,  -62,  -62,  662,  -62,  950,  -62,   27,  501,  -62,
 /*   340 */   -62,  -62,  -62, 1134,  -62,  -62,  -62,  -62,  187,  -62,
 /*   350 */   -62,  -62,  -62,  -62,  -62,  -62,  -62,  321,  -62,  -62,
 /*   360 */   -62,  -62,  -62,  373,  -62,  -62,  -62,  -62,  -62,  477,
 /*   370 */   -62,  -62,  -62,  -62,  -62,  529,  -62,  -62,  -62,  868,
 /*   380 */   918,  501,  -62,  501,  -62,  501,  -62,  -62,  693,  -62,
 /*   390 */   -62,  -62,  -62,  -62,  -62,  573,  -62,  -62,  -62,  -62,
 /*   400 */   -62,  636,  -62,  -62,  -62,  -62,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   594,  594,  594,  408,  410,  411,  412,  594,  594,  413,
 /*    10 */   594,  594,  594,  594,  414,  418,  594,  594,  443,  594,
 /*    20 */   594,  594,  594,  594,  594,  447,  594,  451,  474,  476,
 /*    30 */   477,  478,  479,  480,  481,  482,  483,  484,  485,  486,
 /*    40 */   487,  488,  594,  489,  594,  490,  594,  594,  594,  594,
 /*    50 */   493,  594,  594,  594,  494,  594,  594,  594,  498,  594,
 /*    60 */   594,  594,  594,  500,  594,  594,  594,  594,  594,  594,
 /*    70 */   501,  594,  594,  503,  594,  504,  594,  594,  506,  533,
 /*    80 */   594,  594,  534,  594,  594,  535,  594,  594,  594,  589,
 /*    90 */   594,  590,  594,  591,  537,  594,  538,  594,  539,  594,
 /*   100 */   540,  594,  541,  594,  547,  594,  548,  594,  549,  594,
 /*   110 */   594,  542,  553,  594,  543,  594,  594,  568,  594,  570,
 /*   120 */   572,  573,  583,  584,  585,  586,  587,  588,  569,  571,
 /*   130 */   594,  594,  544,  594,  545,  594,  546,  594,  594,  565,
 /*   140 */   566,  594,  550,  594,  594,  594,  551,  594,  552,  554,
 /*   150 */   555,  556,  557,  558,  559,  594,  560,  594,  561,  594,
 /*   160 */   574,  594,  594,  576,  581,  582,  577,  582,  584,  583,
 /*   170 */   575,  594,  562,  594,  563,  594,  564,  567,  536,  594,
 /*   180 */   594,  594,  507,  594,  594,  594,  508,  594,  594,  594,
 /*   190 */   509,  594,  594,  594,  510,  511,  512,  505,  594,  594,
 /*   200 */   513,  594,  514,  516,  515,  594,  594,  517,  594,  594,
 /*   210 */   594,  594,  518,  594,  594,  594,  519,  594,  594,  520,
 /*   220 */   594,  594,  521,  594,  594,  522,  594,  594,  594,  594,
 /*   230 */   594,  523,  594,  594,  524,  594,  529,  531,  594,  532,
 /*   240 */   530,  594,  594,  525,  594,  594,  526,  594,  594,  527,
 /*   250 */   594,  594,  528,  593,  475,  594,  594,  594,  594,  594,
 /*   260 */   594,  502,  499,  594,  497,  495,  491,  594,  594,  594,
 /*   270 */   496,  492,  594,  594,  594,  452,  594,  459,  461,  594,
 /*   280 */   464,  594,  462,  594,  465,  594,  463,  594,  466,  594,
 /*   290 */   594,  594,  594,  594,  594,  594,  594,  594,  460,  594,
 /*   300 */   594,  594,  448,  594,  594,  594,  594,  594,  594,  594,
 /*   310 */   449,  594,  453,  594,  594,  594,  450,  594,  454,  594,
 /*   320 */   594,  594,  594,  594,  594,  456,  594,  458,  594,  594,
 /*   330 */   594,  594,  594,  594,  455,  594,  457,  594,  594,  441,
 /*   340 */   594,  594,  594,  594,  594,  444,  594,  594,  594,  594,
 /*   350 */   445,  446,  431,  594,  594,  594,  433,  594,  594,  436,
 /*   360 */   594,  594,  438,  594,  594,  440,  594,  594,  434,  594,
 /*   370 */   594,  435,  594,  594,  437,  594,  594,  439,  442,  594,
 /*   380 */   594,  594,  594,  594,  432,  594,  594,  594,  594,  594,
 /*   390 */   415,  419,  594,  594,  594,  594,  594,  416,  420,  594,
 /*   400 */   594,  594,  594,  417,  421,  409,
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
 /* 166 */ "xx_call_parameter ::= xx_literal_expr",
 /* 167 */ "xx_call_parameter ::= IDENTIFIER",
 /* 168 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 169 */ "xx_array_list ::= xx_array_item",
 /* 170 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 171 */ "xx_array_item ::= xx_array_value",
 /* 172 */ "xx_array_key ::= IDENTIFIER",
 /* 173 */ "xx_array_key ::= STRING",
 /* 174 */ "xx_array_key ::= INTEGER",
 /* 175 */ "xx_array_value ::= xx_literal_expr",
 /* 176 */ "xx_array_value ::= IDENTIFIER",
 /* 177 */ "xx_literal_expr ::= INTEGER",
 /* 178 */ "xx_literal_expr ::= STRING",
 /* 179 */ "xx_literal_expr ::= DOUBLE",
 /* 180 */ "xx_literal_expr ::= NULL",
 /* 181 */ "xx_literal_expr ::= FALSE",
 /* 182 */ "xx_literal_expr ::= TRUE",
 /* 183 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 184 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 185 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 186 */ "xx_eval_expr ::= xx_common_expr",
 /* 187 */ "xx_comment ::= COMMENT",
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
// 1586 "parser.c"
      break;
    case 66:
// 715 "parser.lemon"
{ json_object_put((yypminor->yy55)); }
// 1591 "parser.c"
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
// 1996 "parser.c"
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
      case 175:
      case 186:
// 717 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;
}
// 2024 "parser.c"
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
// 2035 "parser.c"
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
      case 169:
// 725 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(NULL, yymsp[0].minor.yy55);
}
// 2052 "parser.c"
        break;
      case 7:
// 741 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(17,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2061 "parser.c"
        break;
      case 8:
// 745 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2071 "parser.c"
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
// 2082 "parser.c"
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
// 2093 "parser.c"
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
// 2105 "parser.c"
        break;
      case 12:
// 761 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2115 "parser.c"
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
// 2126 "parser.c"
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
// 2137 "parser.c"
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
// 2149 "parser.c"
        break;
      case 16:
// 777 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
}
// 2156 "parser.c"
        break;
      case 17:
// 781 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 2163 "parser.c"
        break;
      case 18:
// 785 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, NULL, status->scanner_state);
}
// 2170 "parser.c"
        break;
      case 19:
// 789 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
}
// 2177 "parser.c"
        break;
      case 20:
// 793 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 2184 "parser.c"
        break;
      case 21:
// 797 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[0].minor.yy55, NULL, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2191 "parser.c"
        break;
      case 22:
// 801 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(NULL, yymsp[0].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2198 "parser.c"
        break;
      case 23:
// 805 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
}
// 2205 "parser.c"
        break;
      case 24:
// 809 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_definition(yymsp[-1].minor.yy55, yymsp[0].minor.yy55, yymsp[-2].minor.yy55, status->scanner_state);
}
// 2212 "parser.c"
        break;
      case 27:
// 821 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2221 "parser.c"
        break;
      case 28:
// 825 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2230 "parser.c"
        break;
      case 29:
// 829 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2240 "parser.c"
        break;
      case 30:
// 833 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PUBLIC, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2250 "parser.c"
        break;
      case 31:
// 837 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2259 "parser.c"
        break;
      case 32:
// 841 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2268 "parser.c"
        break;
      case 33:
// 845 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2278 "parser.c"
        break;
      case 34:
// 849 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_property(XX_T_PROTECTED, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2288 "parser.c"
        break;
      case 38:
// 865 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2298 "parser.c"
        break;
      case 39:
// 869 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2308 "parser.c"
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
// 2321 "parser.c"
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
// 2334 "parser.c"
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
// 2347 "parser.c"
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
// 2360 "parser.c"
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
// 2373 "parser.c"
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
// 2386 "parser.c"
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
// 2399 "parser.c"
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
// 2412 "parser.c"
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
// 2425 "parser.c"
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
// 2438 "parser.c"
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
// 2451 "parser.c"
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
// 2464 "parser.c"
        break;
      case 53:
      case 98:
      case 108:
      case 123:
      case 164:
      case 168:
// 925 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_list(yymsp[-2].minor.yy55, yymsp[0].minor.yy55);
  yy_destructor(1,&yymsp[-1].minor);
}
// 2477 "parser.c"
        break;
      case 55:
// 933 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2484 "parser.c"
        break;
      case 56:
// 937 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-1].minor.yy55, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2491 "parser.c"
        break;
      case 57:
// 941 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-1].minor.yy55, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2498 "parser.c"
        break;
      case 58:
// 945 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2506 "parser.c"
        break;
      case 59:
// 949 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(yymsp[-3].minor.yy55, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2514 "parser.c"
        break;
      case 60:
// 953 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_parameter(NULL, yymsp[-3].minor.yy55, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2522 "parser.c"
        break;
      case 61:
// 957 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(4,&yymsp[-2].minor);
  yy_destructor(5,&yymsp[0].minor);
}
// 2531 "parser.c"
        break;
      case 62:
// 961 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(33,&yymsp[0].minor);
}
// 2539 "parser.c"
        break;
      case 63:
// 965 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(34,&yymsp[0].minor);
}
// 2547 "parser.c"
        break;
      case 64:
// 969 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(35,&yymsp[0].minor);
}
// 2555 "parser.c"
        break;
      case 65:
// 973 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(36,&yymsp[0].minor);
}
// 2563 "parser.c"
        break;
      case 66:
// 977 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(37,&yymsp[0].minor);
}
// 2571 "parser.c"
        break;
      case 67:
// 981 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(38,&yymsp[0].minor);
}
// 2579 "parser.c"
        break;
      case 83:
// 1045 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2588 "parser.c"
        break;
      case 84:
// 1049 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2597 "parser.c"
        break;
      case 85:
// 1053 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-2].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2607 "parser.c"
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
// 2620 "parser.c"
        break;
      case 87:
// 1061 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_if_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2630 "parser.c"
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
// 2643 "parser.c"
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
// 2656 "parser.c"
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
// 2669 "parser.c"
        break;
      case 91:
// 1077 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2679 "parser.c"
        break;
      case 92:
// 1081 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_loop_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2689 "parser.c"
        break;
      case 93:
// 1085 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_while_statement(yymsp[-2].minor.yy55, NULL, status->scanner_state);
  yy_destructor(44,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2699 "parser.c"
        break;
      case 94:
// 1089 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_while_statement(yymsp[-3].minor.yy55, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(21,&yymsp[-2].minor);
  yy_destructor(22,&yymsp[0].minor);
}
// 2709 "parser.c"
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
// 2720 "parser.c"
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
// 2732 "parser.c"
        break;
      case 97:
// 1101 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2741 "parser.c"
        break;
      case 100:
// 1114 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("variable", yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2749 "parser.c"
        break;
      case 101:
// 1119 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("object-property", yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2758 "parser.c"
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
// 2769 "parser.c"
        break;
      case 103:
// 1129 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("variable-append", yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2779 "parser.c"
        break;
      case 104:
// 1134 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("array-index", yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2789 "parser.c"
        break;
      case 105:
// 1139 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("incr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(51,&yymsp[0].minor);
}
// 2797 "parser.c"
        break;
      case 106:
// 1144 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_let_assignment("decr", yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[0].minor);
}
// 2805 "parser.c"
        break;
      case 107:
// 1148 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_echo_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2814 "parser.c"
        break;
      case 110:
// 1160 "parser.lemon"
{
	yygotominor.yy55 = yymsp[0].minor.yy55;;
}
// 2821 "parser.c"
        break;
      case 111:
// 1165 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_return_statement(yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2830 "parser.c"
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
// 2844 "parser.c"
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
// 2859 "parser.c"
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
// 2874 "parser.c"
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
// 2889 "parser.c"
        break;
      case 116:
// 1206 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2898 "parser.c"
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
// 2910 "parser.c"
        break;
      case 118:
// 1215 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2919 "parser.c"
        break;
      case 119:
// 1219 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2928 "parser.c"
        break;
      case 120:
// 1223 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2937 "parser.c"
        break;
      case 121:
// 1227 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2946 "parser.c"
        break;
      case 122:
// 1231 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(19,&yymsp[0].minor);
}
// 2955 "parser.c"
        break;
      case 125:
// 1243 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2962 "parser.c"
        break;
      case 126:
// 1247 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 2970 "parser.c"
        break;
      case 128:
// 1255 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 2978 "parser.c"
        break;
      case 129:
// 1259 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("isset", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 2986 "parser.c"
        break;
      case 130:
// 1263 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("equals", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(2,&yymsp[-1].minor);
}
// 2994 "parser.c"
        break;
      case 131:
// 1267 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not-equals", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3002 "parser.c"
        break;
      case 132:
// 1271 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("identical", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(3,&yymsp[-1].minor);
}
// 3010 "parser.c"
        break;
      case 133:
// 1275 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("not-identical", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3018 "parser.c"
        break;
      case 134:
// 1279 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("less", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(4,&yymsp[-1].minor);
}
// 3026 "parser.c"
        break;
      case 135:
// 1283 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("greater", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3034 "parser.c"
        break;
      case 136:
// 1287 "parser.lemon"
{
	yygotominor.yy55 = yymsp[-1].minor.yy55;
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3043 "parser.c"
        break;
      case 137:
// 1291 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-1].minor);
}
// 3051 "parser.c"
        break;
      case 138:
      case 183:
// 1295 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3061 "parser.c"
        break;
      case 139:
      case 184:
// 1300 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3071 "parser.c"
        break;
      case 140:
      case 185:
// 1305 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3081 "parser.c"
        break;
      case 141:
// 1310 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("add", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3089 "parser.c"
        break;
      case 142:
// 1315 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("sub", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3097 "parser.c"
        break;
      case 143:
// 1320 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("concat", yymsp[-2].minor.yy55, yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3105 "parser.c"
        break;
      case 144:
// 1325 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("instanceof", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3113 "parser.c"
        break;
      case 145:
// 1330 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy55, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-3].minor);
  yy_destructor(1,&yymsp[-1].minor);
}
// 3122 "parser.c"
        break;
      case 146:
// 1335 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("typeof", yymsp[0].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3130 "parser.c"
        break;
      case 147:
      case 167:
      case 172:
      case 176:
// 1340 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3140 "parser.c"
        break;
      case 148:
      case 174:
      case 177:
// 1345 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3149 "parser.c"
        break;
      case 149:
      case 173:
      case 178:
// 1350 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3158 "parser.c"
        break;
      case 150:
      case 179:
// 1355 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3166 "parser.c"
        break;
      case 151:
      case 180:
// 1360 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[0].minor);
}
// 3175 "parser.c"
        break;
      case 152:
      case 182:
// 1365 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[0].minor);
}
// 3184 "parser.c"
        break;
      case 153:
      case 181:
// 1370 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(62,&yymsp[0].minor);
}
// 3193 "parser.c"
        break;
      case 154:
// 1375 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3202 "parser.c"
        break;
      case 155:
// 1380 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_expr("array", yymsp[-1].minor.yy55, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3211 "parser.c"
        break;
      case 156:
// 1385 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3219 "parser.c"
        break;
      case 157:
// 1390 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3229 "parser.c"
        break;
      case 158:
// 1395 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(15,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3239 "parser.c"
        break;
      case 159:
// 1400 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3248 "parser.c"
        break;
      case 160:
// 1405 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3257 "parser.c"
        break;
      case 162:
// 1415 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy55, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3267 "parser.c"
        break;
      case 163:
// 1420 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3277 "parser.c"
        break;
      case 170:
// 1448 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_array_item(yymsp[-2].minor.yy55, yymsp[0].minor.yy55, status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3285 "parser.c"
        break;
      case 171:
// 1452 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_array_item(NULL, yymsp[0].minor.yy55, status->scanner_state);
}
// 3292 "parser.c"
        break;
      case 187:
// 1519 "parser.lemon"
{
	yygotominor.yy55 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
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
