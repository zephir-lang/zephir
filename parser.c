/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 43 "parser.lemon"


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
		case XX_T_SCHAR:
			json_object_object_add(ret, "type", json_object_new_string("schar"));
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

static json_object *xx_ret_class_property(json_object *visibility, xx_parser_token *T, json_object *default_value, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "visibility", visibility);
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

static json_object *xx_ret_class_method(json_object *visibility, xx_parser_token *T, json_object *parameters, json_object *statements, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "visibility", visibility);
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
		case XX_TYPE_UINTEGER:
			return json_object_new_string("uint");
		case XX_TYPE_DOUBLE:
			return json_object_new_string("double");
		case XX_TYPE_BOOL:
			return json_object_new_string("bool");
		case XX_TYPE_STRING:
			return json_object_new_string("string");
		case XX_TYPE_CHAR:
			return json_object_new_string("char");
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

static json_object *xx_ret_let_assignment(char *type, json_object *operator, xx_parser_token *V, xx_parser_token *P, json_object *index_expr, json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "assign-type", json_object_new_string(type));
	if (operator) {
		json_object_object_add(ret, "operator", operator);
	}
	json_object_object_add(ret, "variable", json_object_new_string(V->token));
	if (P) {
		json_object_object_add(ret, "property", json_object_new_string(P->token));
	}
	if (index_expr) {
		json_object_object_add(ret, "index-expr", index_expr);
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

static json_object *xx_ret_switch_statement(json_object *expr, json_object *clauses, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("switch"));
	json_object_object_add(ret, "expr", expr);

	if (clauses) {
		json_object_object_add(ret, "clauses", clauses);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_case_clause(json_object *expr, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("case"));

	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

	if (statements) {
		json_object_object_add(ret, "statements", statements);
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

static json_object *xx_ret_for_statement(json_object *expr, xx_parser_token *K, xx_parser_token *V, int reverse, json_object *statements, xx_scanner_state *state)
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

	json_object_object_add(ret, "reverse", json_object_new_int(reverse));

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

static json_object *xx_ret_mcall_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
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
		case XX_T_TYPE_UINTEGER:
			json_object_object_add(ret, "data-type", json_object_new_string("uint"));
			break;
		case XX_T_TYPE_LONG:
			json_object_object_add(ret, "data-type", json_object_new_string("long"));
			break;
		case XX_T_TYPE_ULONG:
			json_object_object_add(ret, "data-type", json_object_new_string("ulong"));
			break;
		case XX_T_TYPE_CHAR:
			json_object_object_add(ret, "data-type", json_object_new_string("char"));
			break;
		case XX_T_TYPE_UCHAR:
			json_object_object_add(ret, "data-type", json_object_new_string("uchar"));
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


// 711 "parser.c"
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
#define YYNOCODE 148
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy133;
  int yy295;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 466
#define YYNRULE 229
#define YYERRORSYMBOL 86
#define YYERRSYMDT yy295
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
 /*     0 */   302,   12,   58,  446,  331,  325,  387,  305,   22,  428,
 /*    10 */   429,  430,  431,  188,  266,  290,  296,  293,  524,  299,
 /*    20 */    44,   46,   48,   98,   57,  170,  171,  222,   68,   72,
 /*    30 */    77,  188,  526,   84,  172,  173,  175,  174,  458,  519,
 /*    40 */    60,  415,  388,  409,  230,  239,  242,  396,  271,  257,
 /*    50 */   275,  278,  281,  284,  287,  302,  269,  390,  391,  318,
 /*    60 */   392,   64,  305,  327,  428,  429,  430,  431,  527,  266,
 /*    70 */   290,  296,  293,  143,  299,   44,   46,   48,  292,   57,
 /*    80 */   394,  188,  191,   68,   72,   77,   98,  190,   84,  143,
 /*    90 */   213,  168,  520,  482,  188,  187,  415,  188,  409,  230,
 /*   100 */   239,  242,  396,  189,  257,  275,  278,  281,  284,  287,
 /*   110 */   302,  163,   49,  521,   52,  325,  395,  305,  457,  428,
 /*   120 */   429,  430,  431,  188,  266,  290,  296,  293,  186,  299,
 /*   130 */    44,   46,   48,  161,   57,  180,  165,  176,   68,   72,
 /*   140 */    77,   18,   19,   84,  218,  383,  168,   73,   15,  223,
 /*   150 */   325,  415,  188,  409,  230,  239,  242,  396,  188,  257,
 /*   160 */   275,  278,  281,  284,  287,  302,  345,  349,  353,  382,
 /*   170 */    61,  330,  305,  162,  428,  429,  430,  431,  107,  266,
 /*   180 */   290,  296,  293,  261,  299,   44,   46,   48,  143,   57,
 /*   190 */   164,  165,  176,   68,   72,   77,  188,  263,   84,  416,
 /*   200 */   187,  427,  190,  483,  368,   80,  415,  383,  409,  230,
 /*   210 */   239,  242,  396,  188,  257,  275,  278,  281,  284,  287,
 /*   220 */   302,  126,  128,  130,   71,  109,  111,  305,  312,  428,
 /*   230 */   429,  430,  431,  192,  266,  290,  296,  293,  143,  299,
 /*   240 */    44,   46,   48,  205,   57,  187,  188,  202,   68,   72,
 /*   250 */    77,  188,  190,   84,    3,    4,    5,    6,  464,  177,
 /*   260 */   101,  415,  531,  409,  230,  239,  242,  396,  188,  257,
 /*   270 */   275,  278,  281,  284,  287,  302,   87,   98,  203,  336,
 /*   280 */    79,  217,  305,  377,   90,  188,  366,  349,  353,  266,
 /*   290 */   290,  296,  293,   88,  299,   44,   46,   48,   86,   57,
 /*   300 */   160,   85,  229,   68,   72,   77,  179,  178,   84,  168,
 /*   310 */    91,   92,   93,   94,   95,  172,  173,  175,  174,  230,
 /*   320 */   239,  242,  248,  262,  257,  275,  278,  281,  284,  287,
 /*   330 */   302,  233,  244,  247,  326,  260,  238,  305,  258,  384,
 /*   340 */   386,  389,  393,  195,  266,  290,  296,  293,  143,  299,
 /*   350 */    44,   46,   48,  232,   57,  167,  188,  185,   68,   72,
 /*   360 */    77,  338,  190,   84,  366,  349,  353,  528,  486,  251,
 /*   370 */   254,  403,  530,  409,  230,  239,  242,  396,   69,  257,
 /*   380 */   275,  278,  281,  284,  287,  302,  267,  274,  187,   25,
 /*   390 */   197,  199,  305,   54,  428,  429,  430,  431,   21,  266,
 /*   400 */   290,  296,  293,  235,  299,   44,   46,   48,  344,   57,
 /*   410 */   234,  188,   98,   68,   72,   77,   97,  201,   84,  218,
 /*   420 */   188,  264,  368,  451,  214,  383,  415,  188,  409,  230,
 /*   430 */   239,  242,  396,  340,  257,  275,  278,  281,  284,  287,
 /*   440 */   302,  339,   62,   20,  323,  140,  194,  305,  450,  384,
 /*   450 */   386,  389,  393,  207,  266,  290,  296,  293,  113,  299,
 /*   460 */    44,   46,   48,   64,   57,  327,  188,  265,   68,   72,
 /*   470 */    77,   98,   98,   84,  115,  226,  209,  405,  488,  188,
 /*   480 */   188,  367,  188,  221,  230,  239,  242,  396,   59,  257,
 /*   490 */   275,  278,  281,  284,  287,  302,  269,   66,  117,  337,
 /*   500 */   119,  455,  305,  459,   90,  121,  188,  406,  188,  266,
 /*   510 */   290,  296,  293,  188,  299,   44,   46,   48,  268,   57,
 /*   520 */   397,  123,  398,   68,   72,   77,  276,  274,   84,  188,
 /*   530 */    91,   92,   93,   94,   95,    7,  210,   65,   10,  230,
 /*   540 */   239,  242,  452,  305,  257,  275,  278,  281,  284,  287,
 /*   550 */   302,  269,  279,  274,   56,  269,  418,  305,  407,  384,
 /*   560 */   386,  389,  393,  419,  266,  290,  296,  293,  125,  299,
 /*   570 */    44,   46,   48,  277,   57,  408,  188,  280,   68,   72,
 /*   580 */    77,  127,  129,   84,  131,  133,  282,  274,  484,  188,
 /*   590 */   188,  367,  188,  188,  230,  239,  242,  396,  303,  257,
 /*   600 */   275,  278,  281,  284,  287,  302,  285,  274,  400,  332,
 /*   610 */   288,  274,  305,  401,  402,   19,   89,  269,  383,  266,
 /*   620 */   290,  296,  293,  135,  299,   44,   46,   48,  137,   57,
 /*   630 */   269,  188,  145,   68,   72,   77,  188,  304,   84,  289,
 /*   640 */   188,  269,  696,    1,    2,  465,    4,    5,    6,  230,
 /*   650 */   239,  242,  283,  410,  257,  275,  278,  281,  284,  287,
 /*   660 */   302,  291,  274,  286,  374,  151,  105,  305,  142,  384,
 /*   670 */   386,  389,  393,  188,  266,  290,  296,  293,  240,  299,
 /*   680 */    44,   46,   48,  411,   57,  347,  188,  412,   68,   72,
 /*   690 */    77,  294,  274,   84,  519,  269,  269,  388,  489,  297,
 /*   700 */   274,  367,  300,  274,  230,  239,  242,  396,  413,  257,
 /*   710 */   275,  278,  281,  284,  287,  302,  269,  295,  298,  311,
 /*   720 */   308,  344,  305,  224,  428,  429,  430,  431,  188,  266,
 /*   730 */   290,  296,  293,  315,  299,   44,   46,   48,  301,   57,
 /*   740 */   385,  188,  320,   68,   72,   77,  521,  348,   84,  395,
 /*   750 */   188,  243,  350,  487,  378,  270,  442,  352,  351,  230,
 /*   760 */   239,  242,  396,  422,  257,  275,  278,  281,  284,  287,
 /*   770 */   302,  208,  198,  518,   83,   50,  434,  305,  460,  384,
 /*   780 */   386,  389,  393,  435,  266,  290,  296,  293,  354,  299,
 /*   790 */    44,   46,   48,  420,   57,  421,  225,  272,   68,   72,
 /*   800 */    77,  355,  245,   84,  356,  358,  273,   47,  485,  182,
 /*   810 */   540,  367,  359,  246,  230,  239,  242,  396,  313,  257,
 /*   820 */   275,  278,  281,  284,  287,  302,  541,  149,  542,  343,
 /*   830 */   518,  314,  305,  543,  384,  386,  389,  393,   14,  266,
 /*   840 */   290,  296,  293,  183,  299,   44,   46,   48,  520,   57,
 /*   850 */   544,  545,  200,   68,   72,   77,  546,  147,   84,  436,
 /*   860 */    78,  212,  437,  490,  177,  249,  367,   45,  369,  230,
 /*   870 */   239,  242,  396,  370,  257,  275,  278,  281,  284,  287,
 /*   880 */   302,  112,  250,  235,  376,  371,  453,  305,  461,  231,
 /*   890 */   236,  188,  373,  110,  266,  290,  296,  293,  447,  299,
 /*   900 */    44,   46,   48,  679,   57,  454,  252,  253,   68,   72,
 /*   910 */    77,  179,  178,   84,  169,  423,  424,  425,  426,  166,
 /*   920 */   172,  173,  175,  174,  230,  239,  242,  108,  466,  257,
 /*   930 */   275,  278,  281,  284,  287,  302,   11,   63,  215,  380,
 /*   940 */   433,  106,  305,  255,  428,  429,  430,  431,  448,  266,
 /*   950 */   290,  296,  293,  379,  299,   44,   46,   48,  680,   57,
 /*   960 */   104,  170,  171,   68,   72,   77,  256,  328,   84,  216,
 /*   970 */   172,  173,  175,  174,  148,   74,  517,    9,  404,  230,
 /*   980 */   239,  242,  396,  681,  257,  275,  278,  281,  284,  287,
 /*   990 */   302,  463,  523,   96,   27,   24,    8,  305,  259,  384,
 /*  1000 */   386,  389,  393,  525,  266,  290,  296,  293,   53,  299,
 /*  1010 */    44,   46,   48,  529,   57,  141,  334,  333,   68,   72,
 /*  1020 */    77,  522,  196,   84,  378,  428,  429,  430,  431,  423,
 /*  1030 */   424,  425,  426,  404,  230,  239,  242,  396,  378,  257,
 /*  1040 */   275,  278,  281,  284,  287,  302,  378,  378,  378,   76,
 /*  1050 */   378,  378,  305,  378,  417,  378,  378,  378,  378,  266,
 /*  1060 */   290,  296,  293,  396,  299,   44,   46,   48,  378,   57,
 /*  1070 */   378,  378,  378,   68,   72,   77,  378,  378,   84,  378,
 /*  1080 */   384,  386,  389,  393,  378,  378,  378,  378,  378,  230,
 /*  1090 */   239,  242,  378,  378,  257,  275,  278,  281,  284,  287,
 /*  1100 */   302,  378,  378,  378,  341,  378,  378,  305,  378,  378,
 /*  1110 */   378,  378,  378,  378,  266,  290,  296,  293,  396,  299,
 /*  1120 */    44,   46,   48,  378,   57,  378,  378,  378,   68,   72,
 /*  1130 */    77,  378,  378,   84,  378,  378,  378,  378,  378,  378,
 /*  1140 */   378,  378,  378,  378,  230,  239,  242,  378,  378,  257,
 /*  1150 */   275,  278,  281,  284,  287,  302,  378,  378,  378,  324,
 /*  1160 */   378,  378,  305,  378,  378,  378,  378,  378,  378,  266,
 /*  1170 */   290,  296,  293,  378,  299,   44,   46,   48,  378,   57,
 /*  1180 */   378,  378,  378,   68,   72,   77,  378,  378,   84,  378,
 /*  1190 */   378,  378,  378,  378,  378,  378,  302,  378,  378,  230,
 /*  1200 */   239,  242,  467,  305,  257,  275,  278,  281,  284,  287,
 /*  1210 */   266,  290,  296,  293,  378,  299,   44,   46,   48,  378,
 /*  1220 */    57,  378,  378,  378,   68,   72,   77,    7,  378,   84,
 /*  1230 */    10,  378,  378,  378,  452,  305,  378,  378,   43,  378,
 /*  1240 */   230,  239,  242,  378,  378,  257,  275,  278,  281,  284,
 /*  1250 */   287,  317,  378,  378,  378,  378,  306,   29,   30,   31,
 /*  1260 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1270 */    42,   43,  399,   17,  414,  432,  378,  427,  438,  402,
 /*  1280 */    19,  378,  237,  383,  342,  378,  378,  378,  378,  306,
 /*  1290 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1300 */    39,   40,   41,   42,   43,  378,  441,  414,  432,  378,
 /*  1310 */   427,  378,  402,   19,  378,  237,  383,   82,  378,  378,
 /*  1320 */   378,  378,  306,   29,   30,   31,   32,   33,   34,   35,
 /*  1330 */    36,   37,   38,   39,   40,   41,   42,  378,  440,   43,
 /*  1340 */   443,  444,  432,  378,  427,  401,  402,   19,  237,  378,
 /*  1350 */   383,  378,   75,  378,  378,  378,  378,  306,   29,   30,
 /*  1360 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1370 */    41,   42,  378,  378,  378,  378,   43,  378,  378,  378,
 /*  1380 */   378,  378,  378,  237,  378,  378,  378,  378,  378,   70,
 /*  1390 */   378,  378,  378,  378,  306,   29,   30,   31,   32,   33,
 /*  1400 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1410 */   378,  378,  378,  378,  378,  378,  378,  378,  378,  378,
 /*  1420 */   237,  378,   67,  378,  378,  378,  378,  306,   29,   30,
 /*  1430 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1440 */    41,   42,   43,  378,  378,  378,  378,  378,  378,  378,
 /*  1450 */   378,  378,  378,  237,  378,   55,  378,  378,  378,  378,
 /*  1460 */   306,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1470 */    38,   39,   40,   41,   42,  378,  378,   43,  378,  378,
 /*  1480 */   378,  378,  378,  378,  378,  378,  237,  378,  378,  378,
 /*  1490 */    51,  378,  378,  378,  378,  306,   29,   30,   31,   32,
 /*  1500 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1510 */   378,  378,  378,  378,   43,  378,  378,  378,  378,  378,
 /*  1520 */   378,  237,  378,  378,  378,  378,  378,  310,  378,  378,
 /*  1530 */   378,  378,  306,   29,   30,   31,   32,   33,   34,   35,
 /*  1540 */    36,   37,   38,   39,   40,   41,   42,   43,  378,  378,
 /*  1550 */   378,  378,  378,  378,  378,  378,  378,  378,  237,  378,
 /*  1560 */   322,  378,  378,  378,  378,  306,   29,   30,   31,   32,
 /*  1570 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1580 */    43,  378,  378,  378,  378,  378,  378,  378,  378,  378,
 /*  1590 */   378,  237,  378,   26,  378,  378,  378,  378,  306,   29,
 /*  1600 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1610 */    40,   41,   42,  378,  378,   43,  378,  378,  378,  378,
 /*  1620 */   378,  378,  378,  378,  237,  378,  378,  378,  381,  378,
 /*  1630 */   378,  378,  378,  306,   29,   30,   31,   32,   33,   34,
 /*  1640 */    35,   36,   37,   38,   39,   40,   41,   42,  378,  378,
 /*  1650 */   378,  378,   43,  378,  378,  378,  378,  378,  378,  237,
 /*  1660 */   378,  378,  378,  378,  378,  375,  378,  378,  378,  378,
 /*  1670 */   306,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1680 */    38,   39,   40,   41,   42,   43,  378,  378,  378,  378,
 /*  1690 */   378,  378,  378,  378,  378,  378,  237,  378,  329,  378,
 /*  1700 */   378,  378,  378,  306,   29,   30,   31,   32,   33,   34,
 /*  1710 */    35,   36,   37,   38,   39,   40,   41,   42,   43,  378,
 /*  1720 */   378,  378,  378,  378,  378,  378,  378,  378,  378,  237,
 /*  1730 */   378,  335,  378,  378,  378,  378,  306,   29,   30,   31,
 /*  1740 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1750 */    42,  378,  378,   43,  378,  378,  378,  378,  378,  378,
 /*  1760 */   378,  378,  237,  378,  378,  378,  378,  378,  378,  378,
 /*  1770 */   378,   28,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1780 */    37,   38,   39,   40,   41,   42,  378,  378,  378,  357,
 /*  1790 */   378,  103,  146,  150,  144,  181,  100,  237,  139,  378,
 /*  1800 */   378,  378,  378,  378,  378,  346,  378,  378,  378,  136,
 /*  1810 */   378,  103,  146,  150,  144,  181,  100,  372,  139,  360,
 /*  1820 */   361,  362,  363,  364,  365,  378,  378,  378,  378,  136,
 /*  1830 */   184,  378,  378,  378,  378,  378,  378,  378,  159,  220,
 /*  1840 */   378,  378,  378,  378,  378,  152,  153,  378,  378,  378,
 /*  1850 */   378,  378,  378,  154,  155,  156,  157,  158,  159,  378,
 /*  1860 */   378,  378,  378,  378,  378,  152,  153,  378,  378,  378,
 /*  1870 */   378,  378,  378,  154,  155,  156,  157,  158,  103,  146,
 /*  1880 */   150,  144,  181,  100,  378,  139,  378,  378,  378,  378,
 /*  1890 */   378,  378,  378,  378,  378,  378,  136,  378,  103,  146,
 /*  1900 */   150,  144,  181,  100,  378,  139,  378,  378,  378,  378,
 /*  1910 */   456,   16,  439,  445,  444,  432,  136,  427,  438,  402,
 /*  1920 */    19,  378,  378,  383,  378,  159,  211,  378,  378,  378,
 /*  1930 */   378,  378,  152,  153,  378,  378,  378,  307,  378,  378,
 /*  1940 */   154,  155,  156,  157,  158,  159,  378,   90,  378,  378,
 /*  1950 */   378,  378,  152,  153,  378,  378,  378,  378,  378,  378,
 /*  1960 */   154,  155,  156,  157,  158,  103,  146,  150,  144,  181,
 /*  1970 */   100,  378,  139,   91,   92,   93,   94,   95,  206,  219,
 /*  1980 */   378,  227,  228,  136,  204,  103,  146,  150,  144,  181,
 /*  1990 */   100,  378,  139,  378,  378,  378,  378,  378,  378,  378,
 /*  2000 */   378,  378,  378,  136,  193,  378,  103,  146,  150,  144,
 /*  2010 */   181,  100,  159,  139,  378,  378,  378,  378,  378,  152,
 /*  2020 */   153,  378,  378,  378,  136,  378,  378,  154,  155,  156,
 /*  2030 */   157,  158,  159,  378,  378,  378,  378,  378,  378,  152,
 /*  2040 */   153,  378,  378,  378,  378,  319,  378,  154,  155,  156,
 /*  2050 */   157,  158,  378,  159,  378,  378,  378,  378,  378,  378,
 /*  2060 */   152,  153,  378,  378,  378,  378,  378,  378,  154,  155,
 /*  2070 */   156,  157,  158,  103,  146,  150,  144,  181,  100,  378,
 /*  2080 */   139,  357,  378,  378,  378,  378,  378,  378,  378,  378,
 /*  2090 */   378,  136,  378,  378,  378,  378,  378,  346,  132,  134,
 /*  2100 */    99,  114,  118,  120,  122,  124,  116,  102,  126,  128,
 /*  2110 */   130,  360,  361,  362,  363,  364,  365,  378,  378,  378,
 /*  2120 */   159,  378,  378,  378,  378,  378,  378,  152,  153,  378,
 /*  2130 */   138,  378,  378,  378,  378,  154,  155,  156,  157,  158,
 /*  2140 */   378,  378,  132,  134,   99,  114,  118,  120,  122,  124,
 /*  2150 */   116,  102,  126,  128,  130,  378,  378,  378,  378,  378,
 /*  2160 */   378,  378,  378,  378,  378,  321,  378,  378,  378,  378,
 /*  2170 */   378,  378,  132,  134,   99,  114,  118,  120,  122,  124,
 /*  2180 */   116,  102,  126,  128,  130,  132,  134,   99,  114,  118,
 /*  2190 */   120,  122,  124,  116,  102,  126,  128,  130,  378,  378,
 /*  2200 */   378,  378,  378,  378,  378,  378,  378,  378,  693,  378,
 /*  2210 */   378,  378,  378,  378,  378,  132,  134,   99,  114,  118,
 /*  2220 */   120,  122,  124,  116,  102,  126,  128,  130,  378,  378,
 /*  2230 */   378,  378,  378,  603,  378,  378,  241,  378,  378,  378,
 /*  2240 */   132,  134,   99,  114,  118,  120,  122,  124,  116,  102,
 /*  2250 */   126,  128,  130,  378,  378,  378,  378,  378,  378,  378,
 /*  2260 */   378,  378,  378,   81,  378,  378,  378,  378,  378,  378,
 /*  2270 */   132,  134,   99,  114,  118,  120,  122,  124,  116,  102,
 /*  2280 */   126,  128,  130,  378,  378,  378,  378,  378,  378,  378,
 /*  2290 */   378,  378,  378,  309,  378,  378,  378,  378,  378,  378,
 /*  2300 */   132,  134,   99,  114,  118,  120,  122,  124,  116,  102,
 /*  2310 */   126,  128,  130,  378,  378,  378,  378,  378,  378,  378,
 /*  2320 */   378,  378,  378,  316,  378,  378,  378,  378,  378,  378,
 /*  2330 */   132,  134,   99,  114,  118,  120,  122,  124,  116,  102,
 /*  2340 */   126,  128,  130,  449,   16,  439,  445,  444,  432,  378,
 /*  2350 */   427,  438,  402,   19,  378,  378,  383,  378,  378,  378,
 /*  2360 */   378,  378,  378,  378,  378,   13,   16,  439,  445,  444,
 /*  2370 */   432,  378,  427,  438,  402,   19,  378,  378,  383,   99,
 /*  2380 */   114,  118,  120,  122,  124,  116,  102,  126,  128,  130,
 /*  2390 */   357,  378,  378,  378,  378,  462,   16,  439,  445,  444,
 /*  2400 */   432,  378,  427,  438,  402,   19,  346,  378,  383,  378,
 /*  2410 */   378,  378,  378,  378,  378,  378,  378,  378,   23,  378,
 /*  2420 */   360,  361,  362,  363,  364,  365,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    26,   29,  126,   31,   30,  129,    3,   33,   37,    1,
 /*    10 */     2,    3,    4,  137,   40,   41,   42,   43,   36,   45,
 /*    20 */    46,   47,   48,  129,   50,   73,   74,  133,   54,   55,
 /*    30 */    56,  137,   36,   59,   82,   83,   84,   85,   30,   36,
 /*    40 */    30,   33,   39,   35,   70,   71,   72,   39,   26,   75,
 /*    50 */    76,   77,   78,   79,   80,   26,    5,    1,    2,   30,
 /*    60 */     4,   51,   33,   53,    1,    2,    3,    4,   36,   40,
 /*    70 */    41,   42,   43,  129,   45,   46,   47,   48,   27,   50,
 /*    80 */     3,  137,  138,   54,   55,   56,  129,  143,   59,  129,
 /*    90 */   133,  100,   36,   30,  137,    5,   33,  137,   35,   70,
 /*   100 */    71,   72,   39,  143,   75,   76,   77,   78,   79,   80,
 /*   110 */    26,    5,  126,   36,   30,  129,   39,   33,   30,    1,
 /*   120 */     2,    3,    4,  137,   40,   41,   42,   43,   38,   45,
 /*   130 */    46,   47,   48,  142,   50,  144,  145,  146,   54,   55,
 /*   140 */    56,  103,  104,   59,  129,  107,  100,  126,   30,  134,
 /*   150 */   129,   33,  137,   35,   70,   71,   72,   39,  137,   75,
 /*   160 */    76,   77,   78,   79,   80,   26,  108,  109,  110,   30,
 /*   170 */   127,  128,   33,   67,    1,    2,    3,    4,   26,   40,
 /*   180 */    41,   42,   43,   26,   45,   46,   47,   48,  129,   50,
 /*   190 */   144,  145,  146,   54,   55,   56,  137,  138,   59,   99,
 /*   200 */     5,  101,  143,   30,  104,  129,   33,  107,   35,   70,
 /*   210 */    71,   72,   39,  137,   75,   76,   77,   78,   79,   80,
 /*   220 */    26,   16,   17,   18,   30,   73,   74,   33,    5,    1,
 /*   230 */     2,    3,    4,   38,   40,   41,   42,   43,  129,   45,
 /*   240 */    46,   47,   48,  129,   50,    5,  137,  138,   54,   55,
 /*   250 */    56,  137,  143,   59,   90,   91,   92,   93,   30,   26,
 /*   260 */   129,   33,   36,   35,   70,   71,   72,   39,  137,   75,
 /*   270 */    76,   77,   78,   79,   80,   26,    5,  129,   38,   30,
 /*   280 */    57,  133,   33,  105,   34,  137,  108,  109,  110,   40,
 /*   290 */    41,   42,   43,  131,   45,   46,   47,   48,   27,   50,
 /*   300 */    67,  130,  131,   54,   55,   56,   73,   74,   59,  100,
 /*   310 */    60,   61,   62,   63,   64,   82,   83,   84,   85,   70,
 /*   320 */    71,   72,   26,   37,   75,   76,   77,   78,   79,   80,
 /*   330 */    26,    5,   65,   66,   30,   23,   27,   33,   26,    1,
 /*   340 */     2,    3,    4,   26,   40,   41,   42,   43,  129,   45,
 /*   350 */    46,   47,   48,   27,   50,  146,  137,  138,   54,   55,
 /*   360 */    56,  105,  143,   59,  108,  109,  110,   36,   30,   73,
 /*   370 */    74,   33,   36,   35,   70,   71,   72,   39,   29,   75,
 /*   380 */    76,   77,   78,   79,   80,   26,  139,  140,    5,   30,
 /*   390 */    73,   74,   33,   29,    1,    2,    3,    4,   26,   40,
 /*   400 */    41,   42,   43,  129,   45,   46,   47,   48,    5,   50,
 /*   410 */   136,  137,  129,   54,   55,   56,  133,   37,   59,  129,
 /*   420 */   137,   38,  104,   30,  134,  107,   33,  137,   35,   70,
 /*   430 */    71,   72,   39,   29,   75,   76,   77,   78,   79,   80,
 /*   440 */    26,   38,   30,   36,   30,   65,   66,   33,   30,    1,
 /*   450 */     2,    3,    4,   26,   40,   41,   42,   43,  129,   45,
 /*   460 */    46,   47,   48,   51,   50,   53,  137,   27,   54,   55,
 /*   470 */    56,  129,  129,   59,  129,  133,  133,   26,   30,  137,
 /*   480 */   137,   33,  137,  132,   70,   71,   72,   39,   29,   75,
 /*   490 */    76,   77,   78,   79,   80,   26,    5,   52,  129,   30,
 /*   500 */   129,   29,   33,   31,   34,  129,  137,   34,  137,   40,
 /*   510 */    41,   42,   43,  137,   45,   46,   47,   48,   27,   50,
 /*   520 */     2,  129,    4,   54,   55,   56,  139,  140,   59,  137,
 /*   530 */    60,   61,   62,   63,   64,   25,   66,  100,   28,   70,
 /*   540 */    71,   72,   32,   33,   75,   76,   77,   78,   79,   80,
 /*   550 */    26,    5,  139,  140,   30,    5,   27,   33,  100,    1,
 /*   560 */     2,    3,    4,   34,   40,   41,   42,   43,  129,   45,
 /*   570 */    46,   47,   48,   27,   50,   27,  137,   27,   54,   55,
 /*   580 */    56,  129,  129,   59,  129,  129,  139,  140,   30,  137,
 /*   590 */   137,   33,  137,  137,   70,   71,   72,   39,   65,   75,
 /*   600 */    76,   77,   78,   79,   80,   26,  139,  140,   97,   30,
 /*   610 */   139,  140,   33,  102,  103,  104,   26,    5,  107,   40,
 /*   620 */    41,   42,   43,  129,   45,   46,   47,   48,  129,   50,
 /*   630 */     5,  137,  129,   54,   55,   56,  137,   26,   59,   27,
 /*   640 */   137,    5,   87,   88,   89,   90,   91,   92,   93,   70,
 /*   650 */    71,   72,   27,   26,   75,   76,   77,   78,   79,   80,
 /*   660 */    26,  139,  140,   27,   30,  129,   26,   33,   37,    1,
 /*   670 */     2,    3,    4,  137,   40,   41,   42,   43,  129,   45,
 /*   680 */    46,   47,   48,   34,   50,   34,  137,  100,   54,   55,
 /*   690 */    56,  139,  140,   59,   36,    5,    5,   39,   30,  139,
 /*   700 */   140,   33,  139,  140,   70,   71,   72,   39,   27,   75,
 /*   710 */    76,   77,   78,   79,   80,   26,    5,   27,   27,   30,
 /*   720 */   129,    5,   33,   67,    1,    2,    3,    4,  137,   40,
 /*   730 */    41,   42,   43,  129,   45,   46,   47,   48,   27,   50,
 /*   740 */     3,  137,  129,   54,   55,   56,   36,  100,   59,   39,
 /*   750 */   137,   26,   26,   30,   38,  140,   33,  100,   34,   70,
 /*   760 */    71,   72,   39,  101,   75,   76,   77,   78,   79,   80,
 /*   770 */    26,  132,   67,   36,   30,   29,   27,   33,   26,    1,
 /*   780 */     2,    3,    4,   34,   40,   41,   42,   43,   26,   45,
 /*   790 */    46,   47,   48,  100,   50,   27,  132,   34,   54,   55,
 /*   800 */    56,   34,   26,   59,  100,   26,  100,   27,   30,   26,
 /*   810 */    26,   33,   11,   27,   70,   71,   72,   39,   26,   75,
 /*   820 */    76,   77,   78,   79,   80,   26,   26,  141,   26,   30,
 /*   830 */    36,   57,   33,   26,    1,    2,    3,    4,   30,   40,
 /*   840 */    41,   42,   43,   37,   45,   46,   47,   48,   36,   50,
 /*   850 */    26,   26,   67,   54,   55,   56,   26,   26,   59,  100,
 /*   860 */    26,  132,   27,   30,   26,   67,   33,   27,   36,   70,
 /*   870 */    71,   72,   39,   26,   75,   76,   77,   78,   79,   80,
 /*   880 */    26,   67,   27,  129,   30,   37,   28,   33,   29,  135,
 /*   890 */   136,  137,   29,   67,   40,   41,   42,   43,   26,   45,
 /*   900 */    46,   47,   48,   52,   50,   26,   67,   27,   54,   55,
 /*   910 */    56,   73,   74,   59,   26,    1,    2,    3,    4,   52,
 /*   920 */    82,   83,   84,   85,   70,   71,   72,   67,    0,   75,
 /*   930 */    76,   77,   78,   79,   80,   26,   26,  128,   67,   30,
 /*   940 */    26,   66,   33,   67,    1,    2,    3,    4,   29,   40,
 /*   950 */    41,   42,   43,   29,   45,   46,   47,   48,   52,   50,
 /*   960 */   141,   73,   74,   54,   55,   56,   27,   52,   59,  132,
 /*   970 */    82,   83,   84,   85,    5,   29,   36,   27,   35,   70,
 /*   980 */    71,   72,   39,   52,   75,   76,   77,   78,   79,   80,
 /*   990 */    26,   30,   36,  132,   30,   29,   26,   33,   27,    1,
 /*  1000 */     2,    3,    4,   36,   40,   41,   42,   43,   49,   45,
 /*  1010 */    46,   47,   48,   36,   50,   26,   29,   49,   54,   55,
 /*  1020 */    56,   36,   67,   59,  147,    1,    2,    3,    4,    1,
 /*  1030 */     2,    3,    4,   35,   70,   71,   72,   39,  147,   75,
 /*  1040 */    76,   77,   78,   79,   80,   26,  147,  147,  147,   30,
 /*  1050 */   147,  147,   33,  147,   26,  147,  147,  147,  147,   40,
 /*  1060 */    41,   42,   43,   39,   45,   46,   47,   48,  147,   50,
 /*  1070 */   147,  147,  147,   54,   55,   56,  147,  147,   59,  147,
 /*  1080 */     1,    2,    3,    4,  147,  147,  147,  147,  147,   70,
 /*  1090 */    71,   72,  147,  147,   75,   76,   77,   78,   79,   80,
 /*  1100 */    26,  147,  147,  147,   30,  147,  147,   33,  147,  147,
 /*  1110 */   147,  147,  147,  147,   40,   41,   42,   43,   39,   45,
 /*  1120 */    46,   47,   48,  147,   50,  147,  147,  147,   54,   55,
 /*  1130 */    56,  147,  147,   59,  147,  147,  147,  147,  147,  147,
 /*  1140 */   147,  147,  147,  147,   70,   71,   72,  147,  147,   75,
 /*  1150 */    76,   77,   78,   79,   80,   26,  147,  147,  147,   30,
 /*  1160 */   147,  147,   33,  147,  147,  147,  147,  147,  147,   40,
 /*  1170 */    41,   42,   43,  147,   45,   46,   47,   48,  147,   50,
 /*  1180 */   147,  147,  147,   54,   55,   56,  147,  147,   59,  147,
 /*  1190 */   147,  147,  147,  147,  147,  147,   26,  147,  147,   70,
 /*  1200 */    71,   72,    0,   33,   75,   76,   77,   78,   79,   80,
 /*  1210 */    40,   41,   42,   43,  147,   45,   46,   47,   48,  147,
 /*  1220 */    50,  147,  147,  147,   54,   55,   56,   25,  147,   59,
 /*  1230 */    28,  147,  147,  147,   32,   33,  147,  147,   93,  147,
 /*  1240 */    70,   71,   72,  147,  147,   75,   76,   77,   78,   79,
 /*  1250 */    80,  106,  147,  147,  147,  147,  111,  112,  113,  114,
 /*  1260 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1270 */   125,   93,   96,   97,   98,   99,  147,  101,  102,  103,
 /*  1280 */   104,  147,  137,  107,  106,  147,  147,  147,  147,  111,
 /*  1290 */   112,  113,  114,  115,  116,  117,  118,  119,  120,  121,
 /*  1300 */   122,  123,  124,  125,   93,  147,   97,   98,   99,  147,
 /*  1310 */   101,  147,  103,  104,  147,  137,  107,  106,  147,  147,
 /*  1320 */   147,  147,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1330 */   119,  120,  121,  122,  123,  124,  125,  147,   95,   93,
 /*  1340 */    97,   98,   99,  147,  101,  102,  103,  104,  137,  147,
 /*  1350 */   107,  147,  106,  147,  147,  147,  147,  111,  112,  113,
 /*  1360 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1370 */   124,  125,  147,  147,  147,  147,   93,  147,  147,  147,
 /*  1380 */   147,  147,  147,  137,  147,  147,  147,  147,  147,  106,
 /*  1390 */   147,  147,  147,  147,  111,  112,  113,  114,  115,  116,
 /*  1400 */   117,  118,  119,  120,  121,  122,  123,  124,  125,   93,
 /*  1410 */   147,  147,  147,  147,  147,  147,  147,  147,  147,  147,
 /*  1420 */   137,  147,  106,  147,  147,  147,  147,  111,  112,  113,
 /*  1430 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1440 */   124,  125,   93,  147,  147,  147,  147,  147,  147,  147,
 /*  1450 */   147,  147,  147,  137,  147,  106,  147,  147,  147,  147,
 /*  1460 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1470 */   121,  122,  123,  124,  125,  147,  147,   93,  147,  147,
 /*  1480 */   147,  147,  147,  147,  147,  147,  137,  147,  147,  147,
 /*  1490 */   106,  147,  147,  147,  147,  111,  112,  113,  114,  115,
 /*  1500 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1510 */   147,  147,  147,  147,   93,  147,  147,  147,  147,  147,
 /*  1520 */   147,  137,  147,  147,  147,  147,  147,  106,  147,  147,
 /*  1530 */   147,  147,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1540 */   119,  120,  121,  122,  123,  124,  125,   93,  147,  147,
 /*  1550 */   147,  147,  147,  147,  147,  147,  147,  147,  137,  147,
 /*  1560 */   106,  147,  147,  147,  147,  111,  112,  113,  114,  115,
 /*  1570 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1580 */    93,  147,  147,  147,  147,  147,  147,  147,  147,  147,
 /*  1590 */   147,  137,  147,  106,  147,  147,  147,  147,  111,  112,
 /*  1600 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1610 */   123,  124,  125,  147,  147,   93,  147,  147,  147,  147,
 /*  1620 */   147,  147,  147,  147,  137,  147,  147,  147,  106,  147,
 /*  1630 */   147,  147,  147,  111,  112,  113,  114,  115,  116,  117,
 /*  1640 */   118,  119,  120,  121,  122,  123,  124,  125,  147,  147,
 /*  1650 */   147,  147,   93,  147,  147,  147,  147,  147,  147,  137,
 /*  1660 */   147,  147,  147,  147,  147,  106,  147,  147,  147,  147,
 /*  1670 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1680 */   121,  122,  123,  124,  125,   93,  147,  147,  147,  147,
 /*  1690 */   147,  147,  147,  147,  147,  147,  137,  147,  106,  147,
 /*  1700 */   147,  147,  147,  111,  112,  113,  114,  115,  116,  117,
 /*  1710 */   118,  119,  120,  121,  122,  123,  124,  125,   93,  147,
 /*  1720 */   147,  147,  147,  147,  147,  147,  147,  147,  147,  137,
 /*  1730 */   147,  106,  147,  147,  147,  147,  111,  112,  113,  114,
 /*  1740 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1750 */   125,  147,  147,   93,  147,  147,  147,  147,  147,  147,
 /*  1760 */   147,  147,  137,  147,  147,  147,  147,  147,  147,  147,
 /*  1770 */   147,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1780 */   120,  121,  122,  123,  124,  125,  147,  147,  147,   10,
 /*  1790 */   147,   19,   20,   21,   22,   23,   24,  137,   26,  147,
 /*  1800 */   147,  147,  147,  147,  147,   26,  147,  147,  147,   37,
 /*  1810 */   147,   19,   20,   21,   22,   23,   24,   38,   26,   40,
 /*  1820 */    41,   42,   43,   44,   45,  147,  147,  147,  147,   37,
 /*  1830 */    38,  147,  147,  147,  147,  147,  147,  147,   66,   67,
 /*  1840 */   147,  147,  147,  147,  147,   73,   74,  147,  147,  147,
 /*  1850 */   147,  147,  147,   81,   82,   83,   84,   85,   66,  147,
 /*  1860 */   147,  147,  147,  147,  147,   73,   74,  147,  147,  147,
 /*  1870 */   147,  147,  147,   81,   82,   83,   84,   85,   19,   20,
 /*  1880 */    21,   22,   23,   24,  147,   26,  147,  147,  147,  147,
 /*  1890 */   147,  147,  147,  147,  147,  147,   37,  147,   19,   20,
 /*  1900 */    21,   22,   23,   24,  147,   26,  147,  147,  147,  147,
 /*  1910 */    94,   95,   96,   97,   98,   99,   37,  101,  102,  103,
 /*  1920 */   104,  147,  147,  107,  147,   66,   67,  147,  147,  147,
 /*  1930 */   147,  147,   73,   74,  147,  147,  147,   58,  147,  147,
 /*  1940 */    81,   82,   83,   84,   85,   66,  147,   34,  147,  147,
 /*  1950 */   147,  147,   73,   74,  147,  147,  147,  147,  147,  147,
 /*  1960 */    81,   82,   83,   84,   85,   19,   20,   21,   22,   23,
 /*  1970 */    24,  147,   26,   60,   61,   62,   63,   64,   65,   66,
 /*  1980 */   147,   68,   69,   37,   38,   19,   20,   21,   22,   23,
 /*  1990 */    24,  147,   26,  147,  147,  147,  147,  147,  147,  147,
 /*  2000 */   147,  147,  147,   37,   38,  147,   19,   20,   21,   22,
 /*  2010 */    23,   24,   66,   26,  147,  147,  147,  147,  147,   73,
 /*  2020 */    74,  147,  147,  147,   37,  147,  147,   81,   82,   83,
 /*  2030 */    84,   85,   66,  147,  147,  147,  147,  147,  147,   73,
 /*  2040 */    74,  147,  147,  147,  147,   58,  147,   81,   82,   83,
 /*  2050 */    84,   85,  147,   66,  147,  147,  147,  147,  147,  147,
 /*  2060 */    73,   74,  147,  147,  147,  147,  147,  147,   81,   82,
 /*  2070 */    83,   84,   85,   19,   20,   21,   22,   23,   24,  147,
 /*  2080 */    26,   10,  147,  147,  147,  147,  147,  147,  147,  147,
 /*  2090 */   147,   37,  147,  147,  147,  147,  147,   26,    6,    7,
 /*  2100 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2110 */    18,   40,   41,   42,   43,   44,   45,  147,  147,  147,
 /*  2120 */    66,  147,  147,  147,  147,  147,  147,   73,   74,  147,
 /*  2130 */    38,  147,  147,  147,  147,   81,   82,   83,   84,   85,
 /*  2140 */   147,  147,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2150 */    14,   15,   16,   17,   18,  147,  147,  147,  147,  147,
 /*  2160 */   147,  147,  147,  147,  147,   29,  147,  147,  147,  147,
 /*  2170 */   147,  147,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2180 */    14,   15,   16,   17,   18,    6,    7,    8,    9,   10,
 /*  2190 */    11,   12,   13,   14,   15,   16,   17,   18,  147,  147,
 /*  2200 */   147,  147,  147,  147,  147,  147,  147,  147,   29,  147,
 /*  2210 */   147,  147,  147,  147,  147,    6,    7,    8,    9,   10,
 /*  2220 */    11,   12,   13,   14,   15,   16,   17,   18,  147,  147,
 /*  2230 */   147,  147,  147,   67,  147,  147,   27,  147,  147,  147,
 /*  2240 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2250 */    16,   17,   18,  147,  147,  147,  147,  147,  147,  147,
 /*  2260 */   147,  147,  147,   29,  147,  147,  147,  147,  147,  147,
 /*  2270 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2280 */    16,   17,   18,  147,  147,  147,  147,  147,  147,  147,
 /*  2290 */   147,  147,  147,   29,  147,  147,  147,  147,  147,  147,
 /*  2300 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2310 */    16,   17,   18,  147,  147,  147,  147,  147,  147,  147,
 /*  2320 */   147,  147,  147,   29,  147,  147,  147,  147,  147,  147,
 /*  2330 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2340 */    16,   17,   18,   94,   95,   96,   97,   98,   99,  147,
 /*  2350 */   101,  102,  103,  104,  147,  147,  107,  147,  147,  147,
 /*  2360 */   147,  147,  147,  147,  147,   94,   95,   96,   97,   98,
 /*  2370 */    99,  147,  101,  102,  103,  104,  147,  147,  107,    8,
 /*  2380 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2390 */    10,  147,  147,  147,  147,   94,   95,   96,   97,   98,
 /*  2400 */    99,  147,  101,  102,  103,  104,   26,  147,  107,  147,
 /*  2410 */   147,  147,  147,  147,  147,  147,  147,  147,   38,  147,
 /*  2420 */    40,   41,   42,   43,   44,   45,
};
#define YY_SHIFT_USE_DFLT (-49)
static short yy_shift_ofst[] = {
 /*     0 */   510,  928, 1202,  -49,  -49,  -49,  -49,  970,  950,  -49,
 /*    10 */   910,  -28,  118,  808,  -49,  -49,   63,  778,  -49,  407,
 /*    20 */   372,  -29, 2380,  966,  359,  -49,  964,  -49,  -49,  -49,
 /*    30 */   -49,  -49,  -49,  -49,  -49,  -49,  -49,  -49,  -49,  -49,
 /*    40 */   -49,  -49,  -49,  -49,  840,  -49,  780,  -49, 2054,  746,
 /*    50 */   579,   84,  959,  364,  -26,  524,  -49, 2054,  459,   10,
 /*    60 */   -49,  412,  -49,  -49,  -48,  445, 1170, 1170,  349,  304,
 /*    70 */   194,  -49, 2054,  946, 1129, 1019,  -49,  834,  223, 1879,
 /*    80 */  2234, 1170,  744,  -49,  590,  271,  -49,  590,  -49, 1913,
 /*    90 */   -49,  -49,  -49,  -49,  -49,  -49, 2054,  -49, 2324, 2054,
 /*   100 */  2054,  -49, 2054,  640,  -49,  875,  152,  860,  -49,  826,
 /*   110 */   -49,  814,  -49,  205, 2054,  205, 2054,  205, 2054,  205,
 /*   120 */  2054,  205, 2054,  205, 2054,  205, 2054,  -49, 2054,  -49,
 /*   130 */  2054,  -49, 2054, 2371, 2054, 2371, 2054, 2092,  -49,  380,
 /*   140 */   989,  631, 1966, 2324, 2054,  -49,  831,  969,  640,  -49,
 /*   150 */  2054,  -49,  -49,  -49,  -49,  -49,  -49,  -49,  -49,  233,
 /*   160 */   -49,  106,  -49,  838,  -49,  867,  888,  -49,  -49,  -49,
 /*   170 */   -49,  -49,  -49,  -49,  -49,  -49,  -49,  851,  906,  931,
 /*   180 */   -49,  783,  806, 1792,  -49,   90,  -49, 2054,  -49,  -49,
 /*   190 */   -49,  195,  -49,  -49,  317,  955,  -49,  705,  -49,  785,
 /*   200 */   -49, 1946,  240,  -49,  -49,  205,  427,  470, 2054,  -49,
 /*   210 */  1859,  250, 2054,  -49,  871,  250, 2054,  -49, 2166, 1772,
 /*   220 */   250, 2054,  -49,  656,  250, 2054,  -49,  -49,  -49,  -49,
 /*   230 */  2054,  326,  -49, 2054,  -49, 2324,  -49,  309,  -49, 2054,
 /*   240 */  2209,  -49,  725,  267,  776,  786,  -49,  296,  798,  855,
 /*   250 */   -49,  839,  880,  -49,  876,  939,  -49,  312,  971,  -49,
 /*   260 */   157,  286, 2054,  383,  440,  -49,   22,  491,  -49,   22,
 /*   270 */   -49,  763,  -48,  -49,  -49,   22,  546,  -49,   22,  550,
 /*   280 */   -49,   22,  625,  -49,   22,  636,  -49,   22,  612,  -49,
 /*   290 */    22,   51,  -49,   22,  690,  -49,   22,  691,  -49,   22,
 /*   300 */   711,  -49,  533,  611,  631,  -49,  -49, 2054, 2264, 1170,
 /*   310 */   689,  -49,  792,  774, 1987, 2294, 1170,   29,  -49, 2054,
 /*   320 */  2136, 1170,  414,  -49,  -49, 2179,  -49,  915, 1170, 1170,
 /*   330 */   -49,  -49,  968,  987,  469,  249,  -49,  -49,  403,  404,
 /*   340 */  1074,  -49,  799,  -49, 2071,  -49,  651,  -48,  -49,  726,
 /*   350 */   724,  -48,  -49,  762,  767,  -48,  -49,  779,  801,  784,
 /*   360 */   800,  802,  807,  824,  825,  830,  -49, 1079,  832,  847,
 /*   370 */   848, 1779,  863,  634,  -49,  854,  -49,  716,  924,  909,
 /*   380 */   -49,  139,  -49,  940,  737,  956,    3,  967,  977,   56,
 /*   390 */   985,  -18,   -4,   77,   32,  226,  518,  331,  336,  338,
 /*   400 */   668,  -49,  -49,  998,  451,  473,  -48,  548,  -49,  627,
 /*   410 */   649,  -48,  681,  -49,  -49,  943, 1028,  529,  -49,  -48,
 /*   420 */   768,  -49,  -49,  -49,  -49,  -49,  -49,  -49,  794,  658,
 /*   430 */   812,  710,  914,  749,  -49,  -48,  835,  -49,  -49,  173,
 /*   440 */   723,  833, 1024,  448,  -49,  558,  872,  919,  393,  418,
 /*   450 */   -49,  -49,  858,  879,  472,    8,   88,  -49,  -49,  752,
 /*   460 */   859,  228,  961,  -49,  -49,  -49,
};
#define YY_REDUCE_USE_DFLT (-125)
static short yy_reduce_ofst[] = {
 /*     0 */   555, -125,  164, -125, -125, -125, -125, -125, -125, -125,
 /*    10 */  -125, -125, 2271, -125, -125, -125, 1176,   38, -125, -125,
 /*    20 */  -125, -125,  256, -125, 1487, -125, 1660, -125, -125, -125,
 /*    30 */  -125, -125, -125, -125, -125, -125, -125, -125, -125, -125,
 /*    40 */  -125, -125, -125, -125, -125, -125, -125, -125,  -14, -125,
 /*    50 */  1384, 1660, -125, -125, 1349, 1660, -125, -124, -125,   43,
 /*    60 */  -125,  809, -125, -125,  437, -125, 1316, 1660, -125, 1283,
 /*    70 */  1660, -125,   21, -125, 1246, 1660, -125, -125, -125,   76,
 /*    80 */  -125, 1211, 1660, -125,  171, -125, -125,  162, -125,  861,
 /*    90 */  -125, -125, -125, -125, -125, -125,  283, -125, -125,  114,
 /*   100 */   131, -125,  329,  819, -125, -125, -125, -125, -125, -125,
 /*   110 */  -125, -125, -125, -125,  345, -125,  369, -125,  371, -125,
 /*   120 */   376, -125,  392, -125,  439, -125,  452, -125,  453, -125,
 /*   130 */   455, -125,  456, -125,  494, -125,  499, -125, -125, -125,
 /*   140 */  -125, -125,  -56, -125,  503, -125, -125, -125,  686, -125,
 /*   150 */   536, -125, -125, -125, -125, -125, -125, -125, -125,   -9,
 /*   160 */  -125, -125, -125,   46, -125, -125,  209, -125, -125, -125,
 /*   170 */  -125, -125, -125, -125, -125, -125, -125, -125, -125, -125,
 /*   180 */  -125, -125, -125,  219, -125, -125, -125,  -40, -125, -125,
 /*   190 */  -125, -125, -125, -125, -125, -125, -125, -125, -125, -125,
 /*   200 */  -125,  109, -125, -125, -125, -125, -125,  639,  343, -125,
 /*   210 */   290,  729,  -43, -125, -125,  837,  148, -125, -125,   15,
 /*   220 */   351, -106, -125, -125,  664,  342, -125, -125, -125, -125,
 /*   230 */   754, -125, -125,  274, -125, -125, -125, -125, -125,  549,
 /*   240 */  -125, -125, -125, -125, -125, -125, -125, -125, -125, -125,
 /*   250 */  -125, -125, -125, -125, -125, -125, -125, -125, -125, -125,
 /*   260 */  -125, -125,   59, -125, -125, -125,  247, -125, -125,  615,
 /*   270 */  -125, -125,  706, -125, -125,  387, -125, -125,  413, -125,
 /*   280 */  -125,  447, -125, -125,  467, -125, -125,  471, -125, -125,
 /*   290 */   522, -125, -125,  552, -125, -125,  560, -125, -125,  563,
 /*   300 */  -125, -125, -125, -125, -125, -125, -125,  591, -125, 1421,
 /*   310 */  1660, -125, -125, -125,  604, -125, 1145, 1660, -125,  613,
 /*   320 */  -125, 1454, 1660, -125, -125, -125, -125, -125, 1592, 1660,
 /*   330 */  -125, -125, -125, -125, 1625, 1660, -125, -125, -125, -125,
 /*   340 */  1178, -125, 1660, -125,   58, -125, -125,  647, -125, -125,
 /*   350 */  -125,  657, -125, -125, -125,  704, -125, -125, -125, -125,
 /*   360 */  -125, -125, -125, -125, -125, -125, -125,  318, -125, -125,
 /*   370 */  -125,  178, -125, 1559, -125, 1660, -125, -125, -125, 1522,
 /*   380 */  -125, 1660, -125, -125, -125, -125, -125, -125, -125, -125,
 /*   390 */  -125, -125, -125, -125, -125, -125, -125, -125, -125,  511,
 /*   400 */    38, -125, -125,  318, -125, -125,  458, -125, -125, -125,
 /*   410 */  -125,  587, -125, -125, -125,  100,  662, -125, -125,  693,
 /*   420 */  -125, -125, -125, -125, -125, -125, -125, -125, -125, -125,
 /*   430 */  -125, -125,  662, -125, -125,  759, -125, -125, -125, 1243,
 /*   440 */  1209,   38,  100,   38, -125,   38, -125, -125, 2249, -125,
 /*   450 */  -125, -125, -125, -125, -125, 1816, -125, -125, -125, -125,
 /*   460 */  -125, 2301, -125, -125, -125, -125,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   695,  695,  695,  468,  470,  471,  472,  695,  695,  473,
 /*    10 */   695,  695,  695,  695,  474,  478,  695,  695,  505,  695,
 /*    20 */   695,  695,  695,  695,  695,  509,  695,  511,  547,  549,
 /*    30 */   550,  551,  552,  553,  554,  555,  556,  557,  558,  559,
 /*    40 */   560,  561,  562,  563,  695,  564,  695,  565,  695,  695,
 /*    50 */   695,  695,  568,  695,  695,  695,  569,  695,  695,  695,
 /*    60 */   572,  695,  573,  574,  695,  695,  695,  576,  695,  695,
 /*    70 */   695,  579,  695,  695,  695,  695,  581,  695,  695,  695,
 /*    80 */   695,  695,  695,  582,  695,  695,  586,  695,  587,  695,
 /*    90 */   589,  590,  591,  592,  593,  594,  695,  595,  630,  695,
 /*   100 */   695,  631,  695,  695,  632,  695,  695,  695,  690,  695,
 /*   110 */   691,  695,  692,  634,  695,  635,  695,  636,  695,  637,
 /*   120 */   695,  638,  695,  639,  695,  640,  695,  646,  695,  647,
 /*   130 */   695,  648,  695,  649,  695,  650,  695,  695,  641,  654,
 /*   140 */   695,  642,  695,  674,  695,  651,  695,  695,  695,  652,
 /*   150 */   695,  653,  655,  656,  657,  658,  659,  660,  661,  695,
 /*   160 */   662,  695,  663,  695,  675,  695,  695,  677,  682,  683,
 /*   170 */   684,  685,  686,  687,  688,  689,  678,  683,  685,  684,
 /*   180 */   676,  695,  664,  695,  665,  695,  666,  695,  669,  672,
 /*   190 */   673,  695,  670,  671,  695,  695,  643,  695,  644,  695,
 /*   200 */   645,  695,  695,  667,  668,  633,  695,  695,  695,  596,
 /*   210 */   695,  695,  695,  597,  695,  695,  695,  598,  695,  695,
 /*   220 */   695,  695,  599,  695,  695,  695,  600,  601,  602,  588,
 /*   230 */   695,  695,  604,  695,  605,  607,  606,  695,  608,  695,
 /*   240 */   695,  609,  695,  695,  695,  695,  610,  695,  695,  695,
 /*   250 */   611,  695,  695,  612,  695,  695,  613,  695,  695,  614,
 /*   260 */   695,  695,  695,  695,  695,  615,  695,  695,  616,  695,
 /*   270 */   626,  628,  695,  629,  627,  695,  695,  617,  695,  695,
 /*   280 */   618,  695,  695,  619,  695,  695,  620,  695,  695,  621,
 /*   290 */   695,  695,  622,  695,  695,  623,  695,  695,  624,  695,
 /*   300 */   695,  625,  695,  695,  695,  694,  548,  695,  695,  695,
 /*   310 */   695,  583,  695,  695,  695,  695,  695,  695,  584,  695,
 /*   320 */   695,  695,  695,  585,  580,  695,  578,  695,  695,  577,
 /*   330 */   575,  570,  566,  695,  695,  695,  571,  567,  695,  695,
 /*   340 */   695,  510,  695,  512,  695,  532,  534,  695,  537,  695,
 /*   350 */   535,  695,  538,  695,  536,  695,  539,  695,  695,  695,
 /*   360 */   695,  695,  695,  695,  695,  695,  533,  695,  695,  695,
 /*   370 */   695,  695,  695,  695,  513,  695,  515,  695,  695,  695,
 /*   380 */   514,  695,  516,  695,  695,  695,  695,  695,  695,  695,
 /*   390 */   695,  695,  695,  695,  695,  695,  695,  695,  695,  695,
 /*   400 */   695,  503,  506,  695,  695,  695,  695,  695,  507,  695,
 /*   410 */   695,  695,  695,  508,  491,  695,  695,  695,  493,  695,
 /*   420 */   695,  495,  497,  499,  500,  501,  502,  498,  499,  500,
 /*   430 */   501,  502,  695,  695,  494,  695,  695,  496,  504,  695,
 /*   440 */   695,  695,  695,  695,  492,  695,  695,  695,  695,  695,
 /*   450 */   475,  479,  695,  695,  695,  695,  695,  476,  480,  695,
 /*   460 */   695,  695,  695,  477,  481,  469,
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
  "$",             "PUBLIC",        "PROTECTED",     "STATIC",      
  "PRIVATE",       "COMMA",         "AND",           "OR",          
  "EQUALS",        "IDENTICAL",     "LESS",          "GREATER",     
  "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",  "NOTEQUALS",   
  "ADD",           "SUB",           "CONCAT",        "ISSET",       
  "FETCH",         "TYPEOF",        "INSTANCEOF",    "NEW",         
  "NOT",           "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",  "EXTENDS",     
  "ABSTRACT",      "COMMENT",       "ASSIGN",        "CONST",       
  "FUNCTION",      "PARENTHESES_OPEN",  "PARENTHESES_CLOSE",  "INLINE",      
  "TYPE_INTEGER",  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "SWITCH",        "CASE",        
  "COLON",         "DEFAULT",       "LOOP",          "WHILE",       
  "FOR",           "IN",            "REVERSE",       "LET",         
  "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",     "DIVASSIGN",   
  "CONCATASSIGN",  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",
  "INCR",          "DECR",          "ECHO",          "RETURN",      
  "UNSET",         "INTEGER",       "STRING",        "THROW",       
  "TYPE_UINTEGER",  "TYPE_CHAR",     "TYPE_UCHAR",    "TYPE_LONG",   
  "TYPE_ULONG",    "SCHAR",         "DOUBLE",        "NULL",        
  "TRUE",          "FALSE",         "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_property_visibility_list",
  "xx_literal_expr",  "xx_property_visibility",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_method_visibility_list",  "xx_parameter_list",  "xx_statement_list",  "xx_method_visibility",
  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",
  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",
  "xx_return_statement",  "xx_mcall_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_switch_statement",  "xx_for_statement",  "xx_eval_expr",  "xx_case_clauses",
  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",
  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",
  "xx_echo_expression",  "xx_mcall_expr",  "xx_call_parameters",  "xx_declare_variable_list",
  "xx_declare_variable",  "xx_isset_expr",  "xx_array_list",  "xx_call_parameter",
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
 /*  27 */ "xx_class_property_definition ::= COMMENT xx_property_visibility_list IDENTIFIER DOTCOMMA",
 /*  28 */ "xx_class_property_definition ::= xx_property_visibility_list IDENTIFIER DOTCOMMA",
 /*  29 */ "xx_class_property_definition ::= COMMENT xx_property_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  30 */ "xx_class_property_definition ::= xx_property_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  31 */ "xx_property_visibility_list ::= xx_property_visibility_list xx_property_visibility",
 /*  32 */ "xx_property_visibility_list ::= xx_property_visibility",
 /*  33 */ "xx_property_visibility ::= PUBLIC",
 /*  34 */ "xx_property_visibility ::= PROTECTED",
 /*  35 */ "xx_property_visibility ::= STATIC",
 /*  36 */ "xx_property_visibility ::= PRIVATE",
 /*  37 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  38 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  39 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  40 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  41 */ "xx_class_const_definition ::= COMMENT CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  42 */ "xx_class_const_definition ::= CONST IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  43 */ "xx_class_method_definition ::= xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  45 */ "xx_class_method_definition ::= xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  46 */ "xx_class_method_definition ::= xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  47 */ "xx_class_method_definition ::= COMMENT xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  48 */ "xx_class_method_definition ::= COMMENT xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  49 */ "xx_class_method_definition ::= COMMENT xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  50 */ "xx_class_method_definition ::= COMMENT xx_method_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  51 */ "xx_method_visibility_list ::= xx_method_visibility",
 /*  52 */ "xx_method_visibility ::= PUBLIC",
 /*  53 */ "xx_method_visibility ::= PROTECTED",
 /*  54 */ "xx_method_visibility ::= STATIC",
 /*  55 */ "xx_method_visibility ::= PRIVATE",
 /*  56 */ "xx_method_visibility ::= STATIC PUBLIC",
 /*  57 */ "xx_method_visibility ::= PUBLIC STATIC",
 /*  58 */ "xx_method_visibility ::= STATIC PROTECTED",
 /*  59 */ "xx_method_visibility ::= PROTECTED STATIC",
 /*  60 */ "xx_method_visibility ::= STATIC PRIVATE",
 /*  61 */ "xx_method_visibility ::= PRIVATE STATIC",
 /*  62 */ "xx_method_visibility ::= INLINE PROTECTED",
 /*  63 */ "xx_method_visibility ::= PROTECTED INLINE",
 /*  64 */ "xx_method_visibility ::= INLINE PRIVATE",
 /*  65 */ "xx_method_visibility ::= PRIVATE INLINE",
 /*  66 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  67 */ "xx_parameter_list ::= xx_parameter",
 /*  68 */ "xx_parameter ::= IDENTIFIER",
 /*  69 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  70 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  71 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  72 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  73 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  74 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  75 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  76 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  77 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  78 */ "xx_parameter_type ::= TYPE_STRING",
 /*  79 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  80 */ "xx_parameter_type ::= TYPE_VAR",
 /*  81 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  82 */ "xx_statement_list ::= xx_statement",
 /*  83 */ "xx_statement ::= xx_let_statement",
 /*  84 */ "xx_statement ::= xx_if_statement",
 /*  85 */ "xx_statement ::= xx_loop_statement",
 /*  86 */ "xx_statement ::= xx_echo_statement",
 /*  87 */ "xx_statement ::= xx_return_statement",
 /*  88 */ "xx_statement ::= xx_mcall_statement",
 /*  89 */ "xx_statement ::= xx_unset_statement",
 /*  90 */ "xx_statement ::= xx_throw_statement",
 /*  91 */ "xx_statement ::= xx_declare_statement",
 /*  92 */ "xx_statement ::= xx_break_statement",
 /*  93 */ "xx_statement ::= xx_continue_statement",
 /*  94 */ "xx_statement ::= xx_while_statement",
 /*  95 */ "xx_statement ::= xx_switch_statement",
 /*  96 */ "xx_statement ::= xx_for_statement",
 /*  97 */ "xx_statement ::= xx_comment",
 /*  98 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  99 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 100 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 101 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 102 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 103 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 104 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 105 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 106 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 107 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 108 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 109 */ "xx_case_clauses ::= xx_case_clause",
 /* 110 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 111 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 112 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 113 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 114 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 115 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 116 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 117 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 118 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 119 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 120 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 121 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 122 */ "xx_let_assignments ::= xx_let_assignment",
 /* 123 */ "xx_assignment_operator ::= ASSIGN",
 /* 124 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 125 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 126 */ "xx_assignment_operator ::= MULASSIGN",
 /* 127 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 128 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 129 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 130 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 131 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 132 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 133 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 134 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 135 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 136 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 137 */ "xx_index_expr ::= xx_common_expr",
 /* 138 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 139 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 140 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 141 */ "xx_echo_expression ::= xx_common_expr",
 /* 142 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 143 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 144 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 145 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 146 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 147 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 148 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 149 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 150 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 151 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 152 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 153 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 154 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 155 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 156 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 157 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 158 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 159 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 160 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 161 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 162 */ "xx_declare_variable ::= IDENTIFIER",
 /* 163 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 164 */ "xx_assign_expr ::= xx_common_expr",
 /* 165 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 166 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 167 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 168 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 169 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 170 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 171 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 172 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 173 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 174 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 175 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 176 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 177 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 178 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 179 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 180 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 181 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 182 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 183 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 184 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 185 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 186 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 187 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 188 */ "xx_common_expr ::= IDENTIFIER",
 /* 189 */ "xx_common_expr ::= INTEGER",
 /* 190 */ "xx_common_expr ::= STRING",
 /* 191 */ "xx_common_expr ::= SCHAR",
 /* 192 */ "xx_common_expr ::= DOUBLE",
 /* 193 */ "xx_common_expr ::= NULL",
 /* 194 */ "xx_common_expr ::= TRUE",
 /* 195 */ "xx_common_expr ::= FALSE",
 /* 196 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 197 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 198 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 199 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 200 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 201 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 202 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 203 */ "xx_common_expr ::= xx_mcall_expr",
 /* 204 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 205 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 206 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 207 */ "xx_call_parameters ::= xx_call_parameter",
 /* 208 */ "xx_call_parameter ::= xx_common_expr",
 /* 209 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 210 */ "xx_array_list ::= xx_array_item",
 /* 211 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 212 */ "xx_array_item ::= xx_array_value",
 /* 213 */ "xx_array_key ::= IDENTIFIER",
 /* 214 */ "xx_array_key ::= STRING",
 /* 215 */ "xx_array_key ::= INTEGER",
 /* 216 */ "xx_array_value ::= xx_literal_expr",
 /* 217 */ "xx_array_value ::= IDENTIFIER",
 /* 218 */ "xx_literal_expr ::= INTEGER",
 /* 219 */ "xx_literal_expr ::= STRING",
 /* 220 */ "xx_literal_expr ::= DOUBLE",
 /* 221 */ "xx_literal_expr ::= NULL",
 /* 222 */ "xx_literal_expr ::= FALSE",
 /* 223 */ "xx_literal_expr ::= TRUE",
 /* 224 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 225 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 226 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 227 */ "xx_eval_expr ::= xx_common_expr",
 /* 228 */ "xx_comment ::= COMMENT",
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
    case 64:
    case 65:
    case 66:
    case 67:
    case 68:
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
    case 82:
    case 83:
    case 84:
    case 85:
// 780 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1976 "parser.c"
      break;
    case 88:
// 793 "parser.lemon"
{ json_object_put((yypminor->yy133)); }
// 1981 "parser.c"
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
  { 87, 1 },
  { 88, 1 },
  { 89, 2 },
  { 89, 1 },
  { 90, 1 },
  { 90, 1 },
  { 90, 1 },
  { 91, 3 },
  { 92, 5 },
  { 92, 7 },
  { 92, 6 },
  { 92, 8 },
  { 92, 4 },
  { 92, 6 },
  { 92, 5 },
  { 92, 7 },
  { 94, 1 },
  { 94, 1 },
  { 94, 1 },
  { 94, 2 },
  { 94, 2 },
  { 94, 2 },
  { 94, 2 },
  { 94, 3 },
  { 94, 3 },
  { 95, 2 },
  { 95, 1 },
  { 98, 4 },
  { 98, 3 },
  { 98, 6 },
  { 98, 5 },
  { 99, 2 },
  { 99, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 96, 2 },
  { 96, 1 },
  { 97, 2 },
  { 97, 1 },
  { 102, 6 },
  { 102, 5 },
  { 103, 7 },
  { 103, 8 },
  { 103, 8 },
  { 103, 9 },
  { 103, 8 },
  { 103, 9 },
  { 103, 9 },
  { 103, 10 },
  { 104, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 2 },
  { 107, 2 },
  { 107, 2 },
  { 107, 2 },
  { 107, 2 },
  { 107, 2 },
  { 107, 2 },
  { 107, 2 },
  { 107, 2 },
  { 107, 2 },
  { 105, 3 },
  { 105, 1 },
  { 108, 1 },
  { 108, 2 },
  { 108, 2 },
  { 108, 3 },
  { 108, 4 },
  { 108, 4 },
  { 110, 3 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 106, 2 },
  { 106, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 121, 2 },
  { 122, 2 },
  { 113, 4 },
  { 113, 7 },
  { 113, 5 },
  { 113, 9 },
  { 113, 8 },
  { 113, 8 },
  { 124, 4 },
  { 124, 5 },
  { 127, 2 },
  { 127, 1 },
  { 128, 4 },
  { 128, 3 },
  { 114, 3 },
  { 114, 4 },
  { 123, 4 },
  { 123, 5 },
  { 125, 7 },
  { 125, 8 },
  { 125, 9 },
  { 125, 10 },
  { 112, 3 },
  { 130, 3 },
  { 130, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 132, 1 },
  { 131, 3 },
  { 131, 5 },
  { 131, 7 },
  { 131, 8 },
  { 131, 5 },
  { 131, 6 },
  { 131, 2 },
  { 131, 2 },
  { 134, 1 },
  { 115, 3 },
  { 135, 3 },
  { 135, 1 },
  { 136, 1 },
  { 117, 2 },
  { 116, 3 },
  { 118, 5 },
  { 118, 6 },
  { 118, 6 },
  { 118, 6 },
  { 119, 3 },
  { 119, 7 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 120, 3 },
  { 139, 3 },
  { 139, 1 },
  { 140, 1 },
  { 140, 3 },
  { 133, 1 },
  { 129, 2 },
  { 129, 2 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 4 },
  { 129, 4 },
  { 129, 4 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 3 },
  { 129, 2 },
  { 129, 4 },
  { 129, 2 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 2 },
  { 129, 3 },
  { 129, 2 },
  { 129, 4 },
  { 129, 5 },
  { 129, 4 },
  { 129, 3 },
  { 129, 1 },
  { 137, 6 },
  { 137, 5 },
  { 138, 3 },
  { 138, 1 },
  { 143, 1 },
  { 142, 3 },
  { 142, 1 },
  { 144, 3 },
  { 144, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 146, 1 },
  { 146, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 100, 1 },
  { 141, 4 },
  { 141, 4 },
  { 141, 4 },
  { 126, 1 },
  { 93, 1 },
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
// 789 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy133;
}
// 2427 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 83:
      case 84:
      case 85:
      case 86:
      case 87:
      case 88:
      case 89:
      case 90:
      case 91:
      case 92:
      case 93:
      case 94:
      case 95:
      case 96:
      case 97:
      case 137:
      case 164:
      case 203:
      case 208:
      case 216:
      case 227:
// 795 "parser.lemon"
{
	yygotominor.yy133 = yymsp[0].minor.yy133;
}
// 2458 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 37:
      case 39:
      case 81:
      case 108:
// 799 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(yymsp[-1].minor.yy133, yymsp[0].minor.yy133);
}
// 2471 "parser.c"
        break;
      case 3:
      case 26:
      case 32:
      case 38:
      case 40:
      case 51:
      case 67:
      case 82:
      case 109:
      case 122:
      case 140:
      case 161:
      case 207:
      case 210:
// 803 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, yymsp[0].minor.yy133);
}
// 2491 "parser.c"
        break;
      case 7:
// 819 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2500 "parser.c"
        break;
      case 8:
// 823 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, 0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2510 "parser.c"
        break;
      case 9:
// 827 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy133, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2521 "parser.c"
        break;
      case 10:
// 831 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, 1, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2532 "parser.c"
        break;
      case 11:
// 835 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy133, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-7].minor);
  yy_destructor(28,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2544 "parser.c"
        break;
      case 12:
// 839 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2554 "parser.c"
        break;
      case 13:
// 843 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2565 "parser.c"
        break;
      case 14:
// 847 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2576 "parser.c"
        break;
      case 15:
// 851 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(28,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2588 "parser.c"
        break;
      case 16:
// 855 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
}
// 2595 "parser.c"
        break;
      case 17:
// 859 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy133, status->scanner_state);
}
// 2602 "parser.c"
        break;
      case 18:
// 863 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(NULL, yymsp[0].minor.yy133, NULL, status->scanner_state);
}
// 2609 "parser.c"
        break;
      case 19:
// 867 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[-1].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
}
// 2616 "parser.c"
        break;
      case 20:
// 871 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[-1].minor.yy133, NULL, yymsp[0].minor.yy133, status->scanner_state);
}
// 2623 "parser.c"
        break;
      case 21:
// 875 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[0].minor.yy133, NULL, yymsp[-1].minor.yy133, status->scanner_state);
}
// 2630 "parser.c"
        break;
      case 22:
// 879 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(NULL, yymsp[0].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
}
// 2637 "parser.c"
        break;
      case 23:
// 883 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[-2].minor.yy133, yymsp[0].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
}
// 2644 "parser.c"
        break;
      case 24:
// 887 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[-1].minor.yy133, yymsp[0].minor.yy133, yymsp[-2].minor.yy133, status->scanner_state);
}
// 2651 "parser.c"
        break;
      case 27:
// 899 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_property(yymsp[-2].minor.yy133, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 2659 "parser.c"
        break;
      case 28:
// 903 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_property(yymsp[-2].minor.yy133, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 2667 "parser.c"
        break;
      case 29:
// 907 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_property(yymsp[-4].minor.yy133, yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2676 "parser.c"
        break;
      case 30:
// 911 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_property(yymsp[-4].minor.yy133, yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2685 "parser.c"
        break;
      case 33:
// 923 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2693 "parser.c"
        break;
      case 34:
// 927 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2701 "parser.c"
        break;
      case 35:
// 931 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2709 "parser.c"
        break;
      case 36:
// 935 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2717 "parser.c"
        break;
      case 41:
// 955 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2727 "parser.c"
        break;
      case 42:
// 959 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 2737 "parser.c"
        break;
      case 43:
// 963 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-6].minor.yy133, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2749 "parser.c"
        break;
      case 44:
// 967 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-7].minor.yy133, yymsp[-5].minor.yy0, yymsp[-3].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2761 "parser.c"
        break;
      case 45:
// 971 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-7].minor.yy133, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2773 "parser.c"
        break;
      case 46:
// 975 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-8].minor.yy133, yymsp[-6].minor.yy0, yymsp[-4].minor.yy133, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2785 "parser.c"
        break;
      case 47:
// 979 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-6].minor.yy133, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2797 "parser.c"
        break;
      case 48:
// 983 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-7].minor.yy133, yymsp[-5].minor.yy0, yymsp[-3].minor.yy133, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2809 "parser.c"
        break;
      case 49:
// 987 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-7].minor.yy133, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy133, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2821 "parser.c"
        break;
      case 50:
// 991 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-8].minor.yy133, yymsp[-6].minor.yy0, yymsp[-4].minor.yy133, yymsp[-1].minor.yy133, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 2833 "parser.c"
        break;
      case 52:
// 1003 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, json_object_new_string("public"));
  yy_destructor(1,&yymsp[0].minor);
}
// 2841 "parser.c"
        break;
      case 53:
// 1007 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, json_object_new_string("protected"));
  yy_destructor(2,&yymsp[0].minor);
}
// 2849 "parser.c"
        break;
      case 54:
// 1011 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, json_object_new_string("static"));
  yy_destructor(3,&yymsp[0].minor);
}
// 2857 "parser.c"
        break;
      case 55:
// 1015 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, json_object_new_string("private"));
  yy_destructor(4,&yymsp[0].minor);
}
// 2865 "parser.c"
        break;
      case 56:
// 1019 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(1,&yymsp[0].minor);
}
// 2874 "parser.c"
        break;
      case 57:
// 1023 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(1,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2883 "parser.c"
        break;
      case 58:
// 1027 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2892 "parser.c"
        break;
      case 59:
// 1031 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2901 "parser.c"
        break;
      case 60:
// 1035 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2910 "parser.c"
        break;
      case 61:
// 1039 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2919 "parser.c"
        break;
      case 62:
// 1043 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2928 "parser.c"
        break;
      case 63:
// 1047 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2937 "parser.c"
        break;
      case 64:
// 1051 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2946 "parser.c"
        break;
      case 65:
// 1055 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2955 "parser.c"
        break;
      case 66:
      case 121:
      case 139:
      case 160:
      case 206:
      case 209:
// 1059 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(yymsp[-2].minor.yy133, yymsp[0].minor.yy133);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2968 "parser.c"
        break;
      case 68:
// 1067 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2975 "parser.c"
        break;
      case 69:
// 1071 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(yymsp[-1].minor.yy133, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2982 "parser.c"
        break;
      case 70:
// 1075 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(NULL, yymsp[-1].minor.yy133, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2989 "parser.c"
        break;
      case 71:
// 1079 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 2997 "parser.c"
        break;
      case 72:
// 1083 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(yymsp[-3].minor.yy133, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 3005 "parser.c"
        break;
      case 73:
// 1087 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(NULL, yymsp[-3].minor.yy133, yymsp[-2].minor.yy0, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 3013 "parser.c"
        break;
      case 74:
// 1091 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 3022 "parser.c"
        break;
      case 75:
// 1095 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(40,&yymsp[0].minor);
}
// 3030 "parser.c"
        break;
      case 76:
// 1099 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(41,&yymsp[0].minor);
}
// 3038 "parser.c"
        break;
      case 77:
// 1103 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(42,&yymsp[0].minor);
}
// 3046 "parser.c"
        break;
      case 78:
// 1107 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(43,&yymsp[0].minor);
}
// 3054 "parser.c"
        break;
      case 79:
// 1111 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(44,&yymsp[0].minor);
}
// 3062 "parser.c"
        break;
      case 80:
// 1115 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(45,&yymsp[0].minor);
}
// 3070 "parser.c"
        break;
      case 98:
// 1187 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3079 "parser.c"
        break;
      case 99:
// 1191 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3088 "parser.c"
        break;
      case 100:
// 1195 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-2].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3098 "parser.c"
        break;
      case 101:
// 1199 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-5].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3111 "parser.c"
        break;
      case 102:
// 1203 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-3].minor.yy133, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3121 "parser.c"
        break;
      case 103:
// 1207 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-7].minor.yy133, yymsp[-5].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3134 "parser.c"
        break;
      case 104:
// 1211 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-6].minor.yy133, yymsp[-4].minor.yy133, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3147 "parser.c"
        break;
      case 105:
// 1215 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-6].minor.yy133, NULL, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3160 "parser.c"
        break;
      case 106:
// 1219 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_switch_statement(yymsp[-2].minor.yy133, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3170 "parser.c"
        break;
      case 107:
// 1223 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_switch_statement(yymsp[-3].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3180 "parser.c"
        break;
      case 110:
// 1235 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_case_clause(yymsp[-2].minor.yy133, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3189 "parser.c"
        break;
      case 111:
// 1239 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_case_clause(NULL, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3198 "parser.c"
        break;
      case 112:
// 1243 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3208 "parser.c"
        break;
      case 113:
// 1247 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_loop_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3218 "parser.c"
        break;
      case 114:
// 1251 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_while_statement(yymsp[-2].minor.yy133, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3228 "parser.c"
        break;
      case 115:
// 1255 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_while_statement(yymsp[-3].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3238 "parser.c"
        break;
      case 116:
// 1259 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_for_statement(yymsp[-3].minor.yy133, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3249 "parser.c"
        break;
      case 117:
// 1263 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_for_statement(yymsp[-3].minor.yy133, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3261 "parser.c"
        break;
      case 118:
// 1267 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_for_statement(yymsp[-3].minor.yy133, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(56,&yymsp[-8].minor);
  yy_destructor(5,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3273 "parser.c"
        break;
      case 119:
// 1271 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_for_statement(yymsp[-3].minor.yy133, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(56,&yymsp[-9].minor);
  yy_destructor(5,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[0].minor);
}
// 3286 "parser.c"
        break;
      case 120:
// 1275 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3295 "parser.c"
        break;
      case 123:
// 1288 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("assign");
  yy_destructor(34,&yymsp[0].minor);
}
// 3303 "parser.c"
        break;
      case 124:
// 1293 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("add-assign");
  yy_destructor(60,&yymsp[0].minor);
}
// 3311 "parser.c"
        break;
      case 125:
// 1298 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("sub-assign");
  yy_destructor(61,&yymsp[0].minor);
}
// 3319 "parser.c"
        break;
      case 126:
// 1302 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("mult-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3327 "parser.c"
        break;
      case 127:
// 1306 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("div-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3335 "parser.c"
        break;
      case 128:
// 1310 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("concat-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3343 "parser.c"
        break;
      case 129:
// 1315 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy133, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy133, status->scanner_state);
}
// 3350 "parser.c"
        break;
      case 130:
// 1320 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy133, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
}
// 3358 "parser.c"
        break;
      case 131:
// 1325 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy133, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(65,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3368 "parser.c"
        break;
      case 132:
// 1330 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy133, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy133, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3378 "parser.c"
        break;
      case 133:
// 1335 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy133, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3387 "parser.c"
        break;
      case 134:
// 1340 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy133, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy133, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3396 "parser.c"
        break;
      case 135:
// 1345 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[0].minor);
}
// 3404 "parser.c"
        break;
      case 136:
// 1350 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[0].minor);
}
// 3412 "parser.c"
        break;
      case 138:
// 1358 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_echo_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3421 "parser.c"
        break;
      case 141:
// 1370 "parser.lemon"
{
	yygotominor.yy133 = yymsp[0].minor.yy133;;
}
// 3428 "parser.c"
        break;
      case 142:
// 1375 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_mcall_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(27,&yymsp[0].minor);
}
// 3436 "parser.c"
        break;
      case 143:
// 1380 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_return_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3445 "parser.c"
        break;
      case 144:
// 1385 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3459 "parser.c"
        break;
      case 145:
// 1394 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3474 "parser.c"
        break;
      case 146:
// 1403 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3489 "parser.c"
        break;
      case 147:
// 1412 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3504 "parser.c"
        break;
      case 148:
// 1421 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3513 "parser.c"
        break;
      case 149:
// 1426 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy133, status->scanner_state);
  yy_destructor(75,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3525 "parser.c"
        break;
      case 150:
// 1430 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3534 "parser.c"
        break;
      case 151:
// 1434 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3543 "parser.c"
        break;
      case 152:
// 1438 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3552 "parser.c"
        break;
      case 153:
// 1442 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3561 "parser.c"
        break;
      case 154:
// 1446 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3570 "parser.c"
        break;
      case 155:
// 1450 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3579 "parser.c"
        break;
      case 156:
// 1454 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3588 "parser.c"
        break;
      case 157:
// 1458 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3597 "parser.c"
        break;
      case 158:
// 1462 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3606 "parser.c"
        break;
      case 159:
// 1466 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(27,&yymsp[0].minor);
}
// 3615 "parser.c"
        break;
      case 162:
// 1478 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3622 "parser.c"
        break;
      case 163:
// 1482 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 3630 "parser.c"
        break;
      case 165:
// 1490 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("not", yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3638 "parser.c"
        break;
      case 166:
// 1494 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("isset", yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3646 "parser.c"
        break;
      case 167:
// 1498 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("equals", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3654 "parser.c"
        break;
      case 168:
// 1502 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("not-equals", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3662 "parser.c"
        break;
      case 169:
// 1506 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("identical", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3670 "parser.c"
        break;
      case 170:
// 1510 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("not-identical", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3678 "parser.c"
        break;
      case 171:
// 1514 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("less", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3686 "parser.c"
        break;
      case 172:
// 1518 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("greater", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3694 "parser.c"
        break;
      case 173:
// 1522 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("less-equal", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3702 "parser.c"
        break;
      case 174:
// 1526 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3710 "parser.c"
        break;
      case 175:
// 1530 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("list", yymsp[-1].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3719 "parser.c"
        break;
      case 176:
// 1534 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-1].minor);
}
// 3727 "parser.c"
        break;
      case 177:
      case 224:
// 1538 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3737 "parser.c"
        break;
      case 178:
      case 225:
// 1543 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3747 "parser.c"
        break;
      case 179:
      case 226:
// 1548 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3757 "parser.c"
        break;
      case 180:
// 1553 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("add", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3765 "parser.c"
        break;
      case 181:
// 1558 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("sub", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3773 "parser.c"
        break;
      case 182:
// 1563 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("concat", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3781 "parser.c"
        break;
      case 183:
// 1568 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("and", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3789 "parser.c"
        break;
      case 184:
// 1573 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("or", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3797 "parser.c"
        break;
      case 185:
// 1578 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("instanceof", yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3805 "parser.c"
        break;
      case 186:
// 1583 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3814 "parser.c"
        break;
      case 187:
// 1588 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("typeof", yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3822 "parser.c"
        break;
      case 188:
      case 213:
      case 217:
// 1593 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3831 "parser.c"
        break;
      case 189:
      case 215:
      case 218:
// 1598 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3840 "parser.c"
        break;
      case 190:
      case 214:
      case 219:
// 1603 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3849 "parser.c"
        break;
      case 191:
// 1608 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 3856 "parser.c"
        break;
      case 192:
      case 220:
// 1613 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3864 "parser.c"
        break;
      case 193:
      case 221:
// 1618 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[0].minor);
}
// 3873 "parser.c"
        break;
      case 194:
      case 223:
// 1623 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3882 "parser.c"
        break;
      case 195:
      case 222:
// 1628 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3891 "parser.c"
        break;
      case 196:
// 1633 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3900 "parser.c"
        break;
      case 197:
// 1638 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("array", yymsp[-1].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3909 "parser.c"
        break;
      case 198:
// 1643 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3917 "parser.c"
        break;
      case 199:
// 1648 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3927 "parser.c"
        break;
      case 200:
// 1653 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3937 "parser.c"
        break;
      case 201:
// 1658 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3946 "parser.c"
        break;
      case 202:
// 1663 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3955 "parser.c"
        break;
      case 204:
// 1673 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3965 "parser.c"
        break;
      case 205:
// 1678 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(38,&yymsp[0].minor);
}
// 3975 "parser.c"
        break;
      case 211:
// 1702 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_array_item(yymsp[-2].minor.yy133, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3983 "parser.c"
        break;
      case 212:
// 1706 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_array_item(NULL, yymsp[0].minor.yy133, status->scanner_state);
}
// 3990 "parser.c"
        break;
      case 228:
// 1773 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 3997 "parser.c"
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
// 746 "parser.lemon"


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

// 4073 "parser.c"
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
			case XX_T_PRIVATE:
				xx_(xx_parser, XX_PRIVATE, NULL, parser_status);
				break;
			case XX_T_STATIC:
				xx_(xx_parser, XX_STATIC, NULL, parser_status);
				break;
			case XX_T_INLINE:
				xx_(xx_parser, XX_INLINE, NULL, parser_status);
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
			case XX_T_REVERSE:
				xx_(xx_parser, XX_REVERSE, NULL, parser_status);
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
			case XX_T_ADDASSIGN:
				xx_(xx_parser, XX_ADDASSIGN, NULL, parser_status);
				break;
			case XX_T_SUBASSIGN:
				xx_(xx_parser, XX_SUBASSIGN, NULL, parser_status);
				break;
			case XX_T_MULASSIGN:
				xx_(xx_parser, XX_MULASSIGN, NULL, parser_status);
				break;
			case XX_T_CONCATASSIGN:
				xx_(xx_parser, XX_CONCATASSIGN, NULL, parser_status);
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
			case XX_T_LESSEQUAL:
				xx_(xx_parser, XX_LESSEQUAL, NULL, parser_status);
				break;
			case XX_T_GREATEREQUAL:
				xx_(xx_parser, XX_GREATEREQUAL, NULL, parser_status);
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
			case XX_T_SWITCH:
				xx_(xx_parser, XX_SWITCH, NULL, parser_status);
				break;
			case XX_T_CASE:
				xx_(xx_parser, XX_CASE, NULL, parser_status);
				break;
			case XX_T_DEFAULT:
				xx_(xx_parser, XX_DEFAULT, NULL, parser_status);
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
			case XX_T_TYPE_UINTEGER:
				xx_(xx_parser, XX_TYPE_UINTEGER, NULL, parser_status);
				break;
			case XX_T_TYPE_CHAR:
				xx_(xx_parser, XX_TYPE_CHAR, NULL, parser_status);
				break;
			case XX_T_TYPE_UCHAR:
				xx_(xx_parser, XX_TYPE_UCHAR, NULL, parser_status);
				break;
			case XX_T_TYPE_LONG:
				xx_(xx_parser, XX_TYPE_LONG, NULL, parser_status);
				break;
			case XX_T_TYPE_ULONG:
				xx_(xx_parser, XX_TYPE_ULONG, NULL, parser_status);
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
			case XX_T_AND:
				xx_(xx_parser, XX_AND, NULL, parser_status);
				break;
			case XX_T_OR:
				xx_(xx_parser, XX_OR, NULL, parser_status);
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
			case XX_T_SCHAR:
				xx_parse_with_token(xx_parser, XX_T_SCHAR, XX_SCHAR, &token, parser_status);
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
