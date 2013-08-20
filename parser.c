/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 44 "parser.lemon"


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
#define YYNSTATE 469
#define YYNRULE 230
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
 /*     0 */   311,  419,  411,  430,   25,  366,  371,  314,  371,  386,
 /*    10 */   196,  386,  177,  275,  299,  305,  302,  152,  308,   44,
 /*    20 */    46,   48,  355,   57,  469,  197,  272,   68,   72,   77,
 /*    30 */   273,  199,   84,  178,  244,  137,  138,  139,  140,  141,
 /*    40 */   142,  243,  197,  239,  248,  251,  107,    8,  266,  284,
 /*    50 */   287,  290,  293,  296,  170,  311,  189,  174,  185,   71,
 /*    60 */   393,  394,  314,  395,  431,  432,  433,  434,  275,  299,
 /*    70 */   305,  302,  278,  308,   44,   46,   48,   98,   57,  179,
 /*    80 */   180,   97,   68,   72,   77,  197,    9,   84,  181,  182,
 /*    90 */   184,  183,  109,  111,  490,  304,  523,  445,  239,  248,
 /*   100 */   251,  152,  399,  266,  284,  287,  290,  293,  296,  197,
 /*   110 */   311,  126,  128,  130,  341,  198,  458,  314,  462,   90,
 /*   120 */   303,  283,  177,  275,  299,  305,  302,  152,  308,   44,
 /*   130 */    46,   48,  400,   57,  401,  197,  211,   68,   72,   77,
 /*   140 */    73,  199,   84,  334,   91,   92,   93,   94,   95,    7,
 /*   150 */   219,  197,   10,  239,  248,  251,  455,  314,  266,  284,
 /*   160 */   287,  290,  293,  296,   11,  311,  173,  174,  185,  352,
 /*   170 */   306,  283,  314,   14,  387,  389,  392,  396,  275,  299,
 /*   180 */   305,  302,  152,  308,   44,   46,   48,  390,   57,   20,
 /*   190 */   197,  194,   68,   72,   77,   80,  199,   84,  421,  387,
 /*   200 */   389,  392,  396,  197,  488,  422,   62,  370,  239,  248,
 /*   210 */   251,  249,  399,  266,  284,  287,  290,  293,  296,  197,
 /*   220 */   311,  522,   60,  391,  320,   22,   64,  314,  336,  492,
 /*   230 */   309,  283,  370,  275,  299,  305,  302,  399,  308,   44,
 /*   240 */    46,   48,   64,   57,  336,  253,  256,   68,   72,   77,
 /*   250 */   179,  180,   84,  210,  387,  389,  392,  396,  133,  181,
 /*   260 */   182,  184,  183,  239,  248,  251,  197,   21,  266,  284,
 /*   270 */   287,  290,  293,  296,   58,  311,  278,  334,  227,  350,
 /*   280 */   149,  203,  314,  223,  487,  197,  197,  370,  275,  299,
 /*   290 */   305,  302,  399,  308,   44,   46,   48,   98,   57,  307,
 /*   300 */    24,  218,   68,   72,   77,  197,   50,   84,  700,    1,
 /*   310 */     2,  468,    4,    5,    6,  354,  358,  362,  239,  248,
 /*   320 */   251,   45,  172,  266,  284,  287,  290,  293,  296,  269,
 /*   330 */   311,   18,   19,  267,  346,  386,  522,  314,  391,  387,
 /*   340 */   389,  392,  396,  275,  299,  305,  302,  152,  308,   44,
 /*   350 */    46,   48,  397,   57,   47,  197,  200,   68,   72,   77,
 /*   360 */    49,  199,   84,  334,  387,  389,  392,  396,  115,  491,
 /*   370 */   278,  197,  370,  239,  248,  251,  197,  399,  266,  284,
 /*   380 */   287,  290,  293,  296,  171,  311,  524,  321,  398,   27,
 /*   390 */   145,   53,  314,  310,  493,   90,  196,  370,  275,  299,
 /*   400 */   305,  302,  399,  308,   44,   46,   48,   98,   57,   54,
 /*   410 */   143,  235,   68,   72,   77,  197,  195,   84,  197,  244,
 /*   420 */    91,   92,   93,   94,   95,  240,  245,  197,  239,  248,
 /*   430 */   251,   61,  339,  266,  284,  287,  290,  293,  296,   79,
 /*   440 */   311,  524,   63,  398,  345,   85,  238,  314,   59,  426,
 /*   450 */   427,  428,  429,  275,  299,  305,  302,  117,  308,   44,
 /*   460 */    46,   48,   98,   57,  242,  197,  231,   68,   72,   77,
 /*   470 */   197,  403,   84,   65,  329,  420,  404,  405,   19,  257,
 /*   480 */    87,  386,  197,  239,  248,  251,  160,  241,  266,  284,
 /*   490 */   287,  290,  293,  296,  197,  311,   12,   66,  449,  332,
 /*   500 */    69,  196,  314,   86,  387,  389,  392,  396,  275,  299,
 /*   510 */   305,  302,  227,  308,   44,   46,   48,  232,   57,  388,
 /*   520 */   197,  201,   68,   72,   77,  260,  263,   84,  177,  431,
 /*   530 */   432,  433,  434,  426,  427,  428,  429,   78,  239,  248,
 /*   540 */   251,  119,  399,  266,  284,  287,  290,  293,  296,  197,
 /*   550 */   311,   74,   98,  521,  379,  470,  226,  314,   89,  436,
 /*   560 */   197,  204,   88,  275,  299,  305,  302,  399,  308,   44,
 /*   570 */    46,   48,   96,   57,  176,  104,  437,   68,   72,   77,
 /*   580 */   108,    7,   84,  438,   10,  291,  283,  347,  455,  314,
 /*   590 */   369,  358,  362,  239,  248,  251,  147,  105,  266,  284,
 /*   600 */   287,  290,  293,  296,  197,  311,  278,  206,  208,  385,
 /*   610 */   106,  196,  314,  110,    3,    4,    5,    6,  275,  299,
 /*   620 */   305,  302,  317,  308,   44,   46,   48,  112,   57,  292,
 /*   630 */   197,  212,   68,   72,   77,  380,  121,   84,  369,  358,
 /*   640 */   362,  353,  278,   98,  197,  288,  283,  222,  239,  248,
 /*   650 */   251,  197,  353,  266,  284,  287,  290,  293,  296,  146,
 /*   660 */   311,  381,  135,  151,   76,  289,  150,  314,  156,  123,
 /*   670 */   197,  157,  348,  275,  299,  305,  302,  197,  308,   44,
 /*   680 */    46,   48,  125,   57,  278,  324,  127,   68,   72,   77,
 /*   690 */   197,  154,   84,  197,  197,  278,  175,  278,  129,  197,
 /*   700 */   294,  283,  683,  239,  248,  251,  197,  295,  266,  284,
 /*   710 */   287,  290,  293,  296,  158,  311,  214,  101,  286,  377,
 /*   720 */   298,  684,  314,  278,  197,  197,  685,  191,  275,  299,
 /*   730 */   305,  302,  192,  308,   44,   46,   48,  113,   57,  285,
 /*   740 */   283,  216,   68,   72,   77,  197,  277,   84,  297,  283,
 /*   750 */   278,  300,  283,  131,  276,  283,  205,  247,  239,  248,
 /*   760 */   251,  197,  207,  266,  284,  287,  290,  293,  296,  209,
 /*   770 */   311,  224,  217,  301,  335,  221,  233,  314,  225,  252,
 /*   780 */   230,  234,  254,  275,  299,  305,  302,  255,  308,   44,
 /*   790 */    46,   48,  258,   57,  259,  261,  262,   68,   72,   77,
 /*   800 */   264,  265,   84,  268,  270,  271,  274,  280,  279,  281,
 /*   810 */   282,  312,  313,  239,  248,  251,  322,  323,  266,  284,
 /*   820 */   287,  290,  293,  296,  337,  311,  343,  349,  342,  333,
 /*   830 */   356,  357,  314,  359,  360,  361,  363,  364,  275,  299,
 /*   840 */   305,  302,  365,  308,   44,   46,   48,  367,   57,  368,
 /*   850 */   543,  372,   68,   72,   77,  373,  374,   84,  520,  376,
 /*   860 */   382,  526,  528,  532,  525,  410,  408,  409,  239,  248,
 /*   870 */   251,  527,  529,  266,  284,  287,  290,  293,  296,  530,
 /*   880 */   311,  534,  531,  533,  340,  413,  414,  314,  415,  416,
 /*   890 */   424,  425,  423,  275,  299,  305,  302,  521,  308,   44,
 /*   900 */    46,   48,  523,   57,  439,  440,  450,   68,   72,   77,
 /*   910 */   451,  457,   84,  453,  456,  463,  379,  460,  466,  464,
 /*   920 */   379,  379,  379,  239,  248,  251,  379,  379,  266,  284,
 /*   930 */   287,  290,  293,  296,  379,  311,  379,  379,  379,   83,
 /*   940 */   379,  379,  314,  379,  379,  379,  379,  379,  275,  299,
 /*   950 */   305,  302,  379,  308,   44,   46,   48,  379,   57,  379,
 /*   960 */   379,  379,   68,   72,   77,  379,  379,   84,  379,  379,
 /*   970 */   379,  379,  379,  379,  379,  379,  379,  379,  239,  248,
 /*   980 */   251,  379,  379,  266,  284,  287,  290,  293,  296,  379,
 /*   990 */   311,  379,  379,  379,   56,  379,  379,  314,  379,  379,
 /*  1000 */   379,  379,  379,  275,  299,  305,  302,  379,  308,   44,
 /*  1010 */    46,   48,  379,   57,  379,  379,  379,   68,   72,   77,
 /*  1020 */   379,  379,   84,  379,  379,  379,  379,  379,  379,  379,
 /*  1030 */   379,  379,  379,  239,  248,  251,  379,  379,  266,  284,
 /*  1040 */   287,  290,  293,  296,  379,  311,  379,  379,  379,   52,
 /*  1050 */   379,  379,  314,  379,  379,  379,  379,  379,  275,  299,
 /*  1060 */   305,  302,  379,  308,   44,   46,   48,  379,   57,  379,
 /*  1070 */   379,  379,   68,   72,   77,  379,  379,   84,  379,  379,
 /*  1080 */   379,  379,  379,  379,  379,  379,  379,  379,  239,  248,
 /*  1090 */   251,  379,  379,  266,  284,  287,  290,  293,  296,  379,
 /*  1100 */   311,  379,  379,  379,  327,  379,  379,  314,  379,  379,
 /*  1110 */   379,  379,  379,  275,  299,  305,  302,  379,  308,   44,
 /*  1120 */    46,   48,  379,   57,  379,  379,  379,   68,   72,   77,
 /*  1130 */   379,  379,   84,  379,  379,  379,  379,  379,  379,  379,
 /*  1140 */   379,  379,  379,  239,  248,  251,  379,  379,  266,  284,
 /*  1150 */   287,  290,  293,  296,  379,  311,  379,  379,  379,  383,
 /*  1160 */   379,  379,  314,  379,  379,  379,  379,  379,  275,  299,
 /*  1170 */   305,  302,  379,  308,   44,   46,   48,  379,   57,  379,
 /*  1180 */   379,  186,   68,   72,   77,  379,  379,   84,  379,  379,
 /*  1190 */   379,  379,  379,  379,  379,  379,  379,  379,  239,  248,
 /*  1200 */   251,  379,  311,  266,  284,  287,  290,  293,  296,  314,
 /*  1210 */   379,  379,  379,  379,  379,  275,  299,  305,  302,  379,
 /*  1220 */   308,   44,   46,   48,  379,   57,  366,  188,  187,   68,
 /*  1230 */    72,   77,  379,  379,   84,  379,  181,  182,  184,  183,
 /*  1240 */   379,  375,  379,  355,  379,  239,  248,  251,  379,  379,
 /*  1250 */   266,  284,  287,  290,  293,  296,  137,  138,  139,  140,
 /*  1260 */   141,  142,  379,  379,  379,  103,  155,  159,  153,  190,
 /*  1270 */   100,  379,  379,  148,  379,  379,  379,   43,  379,  431,
 /*  1280 */   432,  433,  434,  379,  136,  379,  137,  138,  139,  140,
 /*  1290 */   141,  142,  379,  379,  379,   28,   29,   30,   31,   32,
 /*  1300 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1310 */   379,  379,  168,  379,  407,  379,  379,  399,  379,  161,
 /*  1320 */   162,  246,  379,   43,  379,  379,  379,  163,  164,  165,
 /*  1330 */   166,  167,  379,  379,  379,  379,   75,  379,  379,  379,
 /*  1340 */   379,  315,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1350 */    37,   38,   39,   40,   41,   42,  379,  379,  379,   43,
 /*  1360 */   379,  452,   16,  442,  448,  447,  435,  246,  430,  441,
 /*  1370 */   405,   19,  384,  379,  386,  379,  379,  315,   29,   30,
 /*  1380 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1390 */    41,   42,   13,   16,  442,  448,  447,  435,  379,  430,
 /*  1400 */   441,  405,   19,  246,  379,  386,   43,  379,   99,  114,
 /*  1410 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  378,
 /*  1420 */   379,  379,  379,  379,  315,   29,   30,   31,   32,   33,
 /*  1430 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1440 */   379,  379,  379,  379,  379,  379,  379,  379,   90,  379,
 /*  1450 */   246,  379,   82,  379,  379,  379,  379,  315,   29,   30,
 /*  1460 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1470 */    41,   42,   43,   91,   92,   93,   94,   95,  215,  228,
 /*  1480 */   379,  236,  237,  246,  379,   51,  379,  379,  379,  379,
 /*  1490 */   315,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1500 */    38,   39,   40,   41,   42,  379,  379,  379,  444,  417,
 /*  1510 */   435,  379,  430,   43,  405,   19,  246,  379,  386,  379,
 /*  1520 */   379,  379,  379,  379,  379,  379,  351,  379,  379,  379,
 /*  1530 */   379,  315,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1540 */    37,   38,   39,   40,   41,   42,   43,  402,   17,  417,
 /*  1550 */   435,  379,  430,  441,  405,   19,  379,  246,  386,  344,
 /*  1560 */   379,  379,  379,  379,  315,   29,   30,   31,   32,   33,
 /*  1570 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1580 */   387,  389,  392,  396,  379,  379,  379,  379,  379,  379,
 /*  1590 */   246,  379,  326,  379,  379,  379,  379,  315,   29,   30,
 /*  1600 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1610 */    41,   42,  379,  379,  379,  407,  379,  379,  399,  379,
 /*  1620 */    43,  379,  379,  246,  379,  431,  432,  433,  434,  379,
 /*  1630 */   379,  379,  379,   67,  379,  379,  379,  379,  315,   29,
 /*  1640 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1650 */    40,   41,   42,   43,  379,  467,  379,  379,  418,  379,
 /*  1660 */   412,  379,  379,  399,  246,  379,  331,  379,  379,  379,
 /*  1670 */   379,  315,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1680 */    37,   38,   39,   40,   41,   42,   43,  379,  379,  379,
 /*  1690 */   379,  379,  379,  379,  379,  379,  379,  246,  379,  319,
 /*  1700 */   379,  379,  379,  379,  315,   29,   30,   31,   32,   33,
 /*  1710 */    34,   35,   36,   37,   38,   39,   40,   41,   42,  379,
 /*  1720 */   379,  379,  379,  379,  379,  379,  379,   43,  379,  379,
 /*  1730 */   246,  379,  387,  389,  392,  396,  379,  379,  379,  379,
 /*  1740 */    70,  379,  379,  379,  379,  315,   29,   30,   31,   32,
 /*  1750 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1760 */    43,  379,  489,  379,  379,  406,  379,  412,  379,  379,
 /*  1770 */   399,  246,  379,   26,  379,  379,  379,  379,  315,   29,
 /*  1780 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1790 */    40,   41,   42,   43,  379,  379,  379,  379,  379,  379,
 /*  1800 */   379,  379,  379,  379,  246,  379,  338,  379,  379,  379,
 /*  1810 */   379,  315,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1820 */    37,   38,   39,   40,   41,   42,  379,  379,  379,  379,
 /*  1830 */   379,  379,  379,  379,   43,  379,  379,  246,  379,  379,
 /*  1840 */   379,  379,  379,  379,  379,  379,  379,   55,  379,  379,
 /*  1850 */   379,  379,  315,   29,   30,   31,   32,   33,   34,   35,
 /*  1860 */    36,   37,   38,   39,   40,   41,   42,  103,  155,  159,
 /*  1870 */   153,  190,  100,  213,  379,  148,  379,  379,  246,  379,
 /*  1880 */   103,  155,  159,  153,  190,  100,  136,  379,  148,  379,
 /*  1890 */   379,  379,  379,  379,  379,  379,  379,  379,  379,  136,
 /*  1900 */   379,  103,  155,  159,  153,  190,  100,  379,  379,  148,
 /*  1910 */   379,  379,  379,  379,  168,  379,  379,  379,  379,  328,
 /*  1920 */   136,  161,  162,  379,  379,  379,  379,  168,  379,  163,
 /*  1930 */   164,  165,  166,  167,  161,  162,  379,  379,  379,  379,
 /*  1940 */   316,  379,  163,  164,  165,  166,  167,  379,  168,  103,
 /*  1950 */   155,  159,  153,  190,  100,  161,  162,  148,  379,  379,
 /*  1960 */   379,  379,  186,  163,  164,  165,  166,  167,  136,  103,
 /*  1970 */   155,  159,  153,  190,  100,  193,  379,  148,  443,  379,
 /*  1980 */   446,  447,  435,  379,  430,  404,  405,   19,  136,  379,
 /*  1990 */   386,  379,  431,  432,  433,  434,  168,  220,  379,  379,
 /*  2000 */   379,  379,  169,  161,  162,  379,  379,  379,  188,  187,
 /*  2010 */   379,  163,  164,  165,  166,  167,  168,  181,  182,  184,
 /*  2020 */   183,  379,  461,  161,  162,  418,  379,  412,  379,  379,
 /*  2030 */   399,  163,  164,  165,  166,  167,  103,  155,  159,  153,
 /*  2040 */   190,  100,  202,  379,  148,  379,  379,  379,  379,  379,
 /*  2050 */   379,  379,  379,  379,  379,  136,  103,  155,  159,  153,
 /*  2060 */   190,  100,  379,  379,  148,  379,  379,  379,  379,  379,
 /*  2070 */   379,  379,  379,  379,  379,  136,  103,  155,  159,  153,
 /*  2080 */   190,  100,  379,  168,  148,  379,  379,  379,  379,  379,
 /*  2090 */   161,  162,  379,  379,  379,  136,  379,  379,  163,  164,
 /*  2100 */   165,  166,  167,  168,  229,  379,  379,  379,  379,  379,
 /*  2110 */   161,  162,  379,  379,  379,  379,  379,  379,  163,  164,
 /*  2120 */   165,  166,  167,  168,  379,  379,  379,  379,  379,  379,
 /*  2130 */   161,  162,  431,  432,  433,  434,  379,  379,  163,  164,
 /*  2140 */   165,  166,  167,  379,  379,  132,  134,   99,  114,  118,
 /*  2150 */   120,  122,  124,  116,  102,  126,  128,  130,  379,  379,
 /*  2160 */   379,  379,   15,  379,  379,  418,  379,  412,  379,  697,
 /*  2170 */   399,  379,  379,  379,  379,  379,  132,  134,   99,  114,
 /*  2180 */   118,  120,  122,  124,  116,  102,  126,  128,  130,  379,
 /*  2190 */   431,  432,  433,  434,  431,  432,  433,  434,  379,  379,
 /*  2200 */    81,  379,  379,  379,  379,  379,  379,  132,  134,   99,
 /*  2210 */   114,  118,  120,  122,  124,  116,  102,  126,  128,  130,
 /*  2220 */   485,  379,  379,  418,  486,  412,  379,  418,  399,  412,
 /*  2230 */   379,  325,  399,  379,  431,  432,  433,  434,  132,  134,
 /*  2240 */    99,  114,  118,  120,  122,  124,  116,  102,  126,  128,
 /*  2250 */   130,  132,  134,   99,  114,  118,  120,  122,  124,  116,
 /*  2260 */   102,  126,  128,  130,  454,  379,  379,  418,  379,  412,
 /*  2270 */   379,  379,  399,  379,  379,  318,  379,  379,  379,  379,
 /*  2280 */   379,  379,  132,  134,   99,  114,  118,  120,  122,  124,
 /*  2290 */   116,  102,  126,  128,  130,  379,  379,  379,  379,  606,
 /*  2300 */   379,  379,  379,  379,  379,  379,  330,  379,  379,  379,
 /*  2310 */   379,  379,  379,  132,  134,   99,  114,  118,  120,  122,
 /*  2320 */   124,  116,  102,  126,  128,  130,  379,  379,  379,  379,
 /*  2330 */   379,  379,  379,  379,  379,  250,  379,  379,  379,  379,
 /*  2340 */   379,  379,  132,  134,   99,  114,  118,  120,  122,  124,
 /*  2350 */   116,  102,  126,  128,  130,  379,  379,  379,  379,  379,
 /*  2360 */   379,  144,  379,  379,  379,  379,  379,  379,  132,  134,
 /*  2370 */    99,  114,  118,  120,  122,  124,  116,  102,  126,  128,
 /*  2380 */   130,  459,   16,  442,  448,  447,  435,  379,  430,  441,
 /*  2390 */   405,   19,  379,  379,  386,  379,  379,  379,  379,  379,
 /*  2400 */   379,  379,  379,  379,  379,  366,  379,  379,  465,   16,
 /*  2410 */   442,  448,  447,  435,  379,  430,  441,  405,   19,  379,
 /*  2420 */    23,  386,  355,  379,  379,  379,  379,  379,  379,  379,
 /*  2430 */   379,  379,  379,  379,  379,  137,  138,  139,  140,  141,
 /*  2440 */   142,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    27,   99,   28,  101,   31,   10,  104,   34,  104,  107,
 /*    10 */     5,  107,  100,   40,   41,   42,   43,  129,   45,   46,
 /*    20 */    47,   48,   27,   50,    0,  137,  138,   54,   55,   56,
 /*    30 */    25,  143,   59,   27,  129,   40,   41,   42,   43,   44,
 /*    40 */    45,  136,  137,   70,   71,   72,   27,   27,   75,   76,
 /*    50 */    77,   78,   79,   80,  142,   27,  144,  145,  146,   31,
 /*    60 */     1,    2,   34,    4,    1,    2,    3,    4,   40,   41,
 /*    70 */    42,   43,    5,   45,   46,   47,   48,  129,   50,   73,
 /*    80 */    74,  133,   54,   55,   56,  137,   28,   59,   82,   83,
 /*    90 */    84,   85,   73,   74,   31,   28,   37,   34,   70,   71,
 /*   100 */    72,  129,   39,   75,   76,   77,   78,   79,   80,  137,
 /*   110 */    27,   16,   17,   18,   31,  143,   30,   34,   32,   35,
 /*   120 */   139,  140,  100,   40,   41,   42,   43,  129,   45,   46,
 /*   130 */    47,   48,    2,   50,    4,  137,  138,   54,   55,   56,
 /*   140 */   126,  143,   59,  129,   60,   61,   62,   63,   64,   26,
 /*   150 */    66,  137,   29,   70,   71,   72,   33,   34,   75,   76,
 /*   160 */    77,   78,   79,   80,   27,   27,  144,  145,  146,   31,
 /*   170 */   139,  140,   34,   31,    1,    2,    3,    4,   40,   41,
 /*   180 */    42,   43,  129,   45,   46,   47,   48,    3,   50,   37,
 /*   190 */   137,  138,   54,   55,   56,  129,  143,   59,   28,    1,
 /*   200 */     2,    3,    4,  137,   31,   35,   31,   34,   70,   71,
 /*   210 */    72,  129,   39,   75,   76,   77,   78,   79,   80,  137,
 /*   220 */    27,   37,   31,   39,   31,   38,   51,   34,   53,   31,
 /*   230 */   139,  140,   34,   40,   41,   42,   43,   39,   45,   46,
 /*   240 */    47,   48,   51,   50,   53,   65,   66,   54,   55,   56,
 /*   250 */    73,   74,   59,   38,    1,    2,    3,    4,  129,   82,
 /*   260 */    83,   84,   85,   70,   71,   72,  137,   27,   75,   76,
 /*   270 */    77,   78,   79,   80,  126,   27,    5,  129,  129,   31,
 /*   280 */    65,   66,   34,  134,   31,  137,  137,   34,   40,   41,
 /*   290 */    42,   43,   39,   45,   46,   47,   48,  129,   50,   28,
 /*   300 */    30,  133,   54,   55,   56,  137,   30,   59,   87,   88,
 /*   310 */    89,   90,   91,   92,   93,  108,  109,  110,   70,   71,
 /*   320 */    72,   28,    5,   75,   76,   77,   78,   79,   80,   23,
 /*   330 */    27,  103,  104,   27,   31,  107,   37,   34,   39,    1,
 /*   340 */     2,    3,    4,   40,   41,   42,   43,  129,   45,   46,
 /*   350 */    47,   48,    3,   50,   28,  137,  138,   54,   55,   56,
 /*   360 */   126,  143,   59,  129,    1,    2,    3,    4,  129,   31,
 /*   370 */     5,  137,   34,   70,   71,   72,  137,   39,   75,   76,
 /*   380 */    77,   78,   79,   80,   67,   27,   37,    5,   39,   31,
 /*   390 */   109,   49,   34,   28,   31,   35,    5,   34,   40,   41,
 /*   400 */    42,   43,   39,   45,   46,   47,   48,  129,   50,   30,
 /*   410 */   129,  133,   54,   55,   56,  137,   25,   59,  137,  129,
 /*   420 */    60,   61,   62,   63,   64,  135,  136,  137,   70,   71,
 /*   430 */    72,  127,  128,   75,   76,   77,   78,   79,   80,   57,
 /*   440 */    27,   37,  128,   39,   31,  130,  131,   34,   30,    1,
 /*   450 */     2,    3,    4,   40,   41,   42,   43,  129,   45,   46,
 /*   460 */    47,   48,  129,   50,    5,  137,  133,   54,   55,   56,
 /*   470 */   137,   97,   59,  100,  129,   27,  102,  103,  104,   27,
 /*   480 */     5,  107,  137,   70,   71,   72,  129,   28,   75,   76,
 /*   490 */    77,   78,   79,   80,  137,   27,   30,   52,   32,   31,
 /*   500 */    30,    5,   34,   28,    1,    2,    3,    4,   40,   41,
 /*   510 */    42,   43,  129,   45,   46,   47,   48,  134,   50,    3,
 /*   520 */   137,   25,   54,   55,   56,   73,   74,   59,  100,    1,
 /*   530 */     2,    3,    4,    1,    2,    3,    4,   27,   70,   71,
 /*   540 */    72,  129,   39,   75,   76,   77,   78,   79,   80,  137,
 /*   550 */    27,   30,  129,   37,   31,    0,  133,   34,   27,   27,
 /*   560 */   137,   27,  131,   40,   41,   42,   43,   39,   45,   46,
 /*   570 */    47,   48,  132,   50,  146,  141,   28,   54,   55,   56,
 /*   580 */    67,   26,   59,   35,   29,  139,  140,  105,   33,   34,
 /*   590 */   108,  109,  110,   70,   71,   72,  129,   27,   75,   76,
 /*   600 */    77,   78,   79,   80,  137,   27,    5,   73,   74,   31,
 /*   610 */    66,    5,   34,   67,   90,   91,   92,   93,   40,   41,
 /*   620 */    42,   43,  129,   45,   46,   47,   48,   67,   50,   28,
 /*   630 */   137,   25,   54,   55,   56,  105,  129,   59,  108,  109,
 /*   640 */   110,    5,    5,  129,  137,  139,  140,  133,   70,   71,
 /*   650 */    72,  137,    5,   75,   76,   77,   78,   79,   80,   25,
 /*   660 */    27,   25,  129,   38,   31,   28,   27,   34,   27,  129,
 /*   670 */   137,    5,   25,   40,   41,   42,   43,  137,   45,   46,
 /*   680 */    47,   48,  129,   50,    5,  129,  129,   54,   55,   56,
 /*   690 */   137,  129,   59,  137,  137,    5,   52,    5,  129,  137,
 /*   700 */   139,  140,   52,   70,   71,   72,  137,   28,   75,   76,
 /*   710 */    77,   78,   79,   80,  141,   27,  129,  129,   28,   31,
 /*   720 */    28,   52,   34,    5,  137,  137,   52,   27,   40,   41,
 /*   730 */    42,   43,   38,   45,   46,   47,   48,  129,   50,  139,
 /*   740 */   140,   27,   54,   55,   56,  137,   28,   59,  139,  140,
 /*   750 */     5,  139,  140,  129,  139,  140,   67,   28,   70,   71,
 /*   760 */    72,  137,   67,   75,   76,   77,   78,   79,   80,   67,
 /*   770 */    27,   67,  132,   28,   31,  132,   67,   34,  132,   27,
 /*   780 */   132,  132,   27,   40,   41,   42,   43,   28,   45,   46,
 /*   790 */    47,   48,   67,   50,   28,   67,   28,   54,   55,   56,
 /*   800 */    67,   28,   59,   28,   27,   38,   28,   27,  140,   35,
 /*   810 */   100,   65,   27,   70,   71,   72,   27,   57,   75,   76,
 /*   820 */    77,   78,   79,   80,   52,   27,   30,   30,   49,   31,
 /*   830 */    35,  100,   34,   27,   35,  100,   27,   35,   40,   41,
 /*   840 */    42,   43,  100,   45,   46,   47,   48,   27,   50,   11,
 /*   850 */    27,   37,   54,   55,   56,   27,   38,   59,   37,   30,
 /*   860 */    30,   37,   37,   37,   37,  100,   27,   35,   70,   71,
 /*   870 */    72,   37,   37,   75,   76,   77,   78,   79,   80,   37,
 /*   880 */    27,   37,   37,   37,   31,   27,   35,   34,  100,   28,
 /*   890 */    28,  101,  100,   40,   41,   42,   43,   37,   45,   46,
 /*   900 */    47,   48,   37,   50,  100,   28,   27,   54,   55,   56,
 /*   910 */    30,   27,   59,   31,   29,   27,  147,   31,   31,   30,
 /*   920 */   147,  147,  147,   70,   71,   72,  147,  147,   75,   76,
 /*   930 */    77,   78,   79,   80,  147,   27,  147,  147,  147,   31,
 /*   940 */   147,  147,   34,  147,  147,  147,  147,  147,   40,   41,
 /*   950 */    42,   43,  147,   45,   46,   47,   48,  147,   50,  147,
 /*   960 */   147,  147,   54,   55,   56,  147,  147,   59,  147,  147,
 /*   970 */   147,  147,  147,  147,  147,  147,  147,  147,   70,   71,
 /*   980 */    72,  147,  147,   75,   76,   77,   78,   79,   80,  147,
 /*   990 */    27,  147,  147,  147,   31,  147,  147,   34,  147,  147,
 /*  1000 */   147,  147,  147,   40,   41,   42,   43,  147,   45,   46,
 /*  1010 */    47,   48,  147,   50,  147,  147,  147,   54,   55,   56,
 /*  1020 */   147,  147,   59,  147,  147,  147,  147,  147,  147,  147,
 /*  1030 */   147,  147,  147,   70,   71,   72,  147,  147,   75,   76,
 /*  1040 */    77,   78,   79,   80,  147,   27,  147,  147,  147,   31,
 /*  1050 */   147,  147,   34,  147,  147,  147,  147,  147,   40,   41,
 /*  1060 */    42,   43,  147,   45,   46,   47,   48,  147,   50,  147,
 /*  1070 */   147,  147,   54,   55,   56,  147,  147,   59,  147,  147,
 /*  1080 */   147,  147,  147,  147,  147,  147,  147,  147,   70,   71,
 /*  1090 */    72,  147,  147,   75,   76,   77,   78,   79,   80,  147,
 /*  1100 */    27,  147,  147,  147,   31,  147,  147,   34,  147,  147,
 /*  1110 */   147,  147,  147,   40,   41,   42,   43,  147,   45,   46,
 /*  1120 */    47,   48,  147,   50,  147,  147,  147,   54,   55,   56,
 /*  1130 */   147,  147,   59,  147,  147,  147,  147,  147,  147,  147,
 /*  1140 */   147,  147,  147,   70,   71,   72,  147,  147,   75,   76,
 /*  1150 */    77,   78,   79,   80,  147,   27,  147,  147,  147,   31,
 /*  1160 */   147,  147,   34,  147,  147,  147,  147,  147,   40,   41,
 /*  1170 */    42,   43,  147,   45,   46,   47,   48,  147,   50,  147,
 /*  1180 */   147,   27,   54,   55,   56,  147,  147,   59,  147,  147,
 /*  1190 */   147,  147,  147,  147,  147,  147,  147,  147,   70,   71,
 /*  1200 */    72,  147,   27,   75,   76,   77,   78,   79,   80,   34,
 /*  1210 */   147,  147,  147,  147,  147,   40,   41,   42,   43,  147,
 /*  1220 */    45,   46,   47,   48,  147,   50,   10,   73,   74,   54,
 /*  1230 */    55,   56,  147,  147,   59,  147,   82,   83,   84,   85,
 /*  1240 */   147,   25,  147,   27,  147,   70,   71,   72,  147,  147,
 /*  1250 */    75,   76,   77,   78,   79,   80,   40,   41,   42,   43,
 /*  1260 */    44,   45,  147,  147,  147,   19,   20,   21,   22,   23,
 /*  1270 */    24,  147,  147,   27,  147,  147,  147,   93,  147,    1,
 /*  1280 */     2,    3,    4,  147,   38,  147,   40,   41,   42,   43,
 /*  1290 */    44,   45,  147,  147,  147,  111,  112,  113,  114,  115,
 /*  1300 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1310 */   147,  147,   66,  147,   36,  147,  147,   39,  147,   73,
 /*  1320 */    74,  137,  147,   93,  147,  147,  147,   81,   82,   83,
 /*  1330 */    84,   85,  147,  147,  147,  147,  106,  147,  147,  147,
 /*  1340 */   147,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1350 */   120,  121,  122,  123,  124,  125,  147,  147,  147,   93,
 /*  1360 */   147,   94,   95,   96,   97,   98,   99,  137,  101,  102,
 /*  1370 */   103,  104,  106,  147,  107,  147,  147,  111,  112,  113,
 /*  1380 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1390 */   124,  125,   94,   95,   96,   97,   98,   99,  147,  101,
 /*  1400 */   102,  103,  104,  137,  147,  107,   93,  147,    8,    9,
 /*  1410 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  106,
 /*  1420 */   147,  147,  147,  147,  111,  112,  113,  114,  115,  116,
 /*  1430 */   117,  118,  119,  120,  121,  122,  123,  124,  125,   93,
 /*  1440 */   147,  147,  147,  147,  147,  147,  147,  147,   35,  147,
 /*  1450 */   137,  147,  106,  147,  147,  147,  147,  111,  112,  113,
 /*  1460 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1470 */   124,  125,   93,   60,   61,   62,   63,   64,   65,   66,
 /*  1480 */   147,   68,   69,  137,  147,  106,  147,  147,  147,  147,
 /*  1490 */   111,  112,  113,  114,  115,  116,  117,  118,  119,  120,
 /*  1500 */   121,  122,  123,  124,  125,  147,  147,  147,   97,   98,
 /*  1510 */    99,  147,  101,   93,  103,  104,  137,  147,  107,  147,
 /*  1520 */   147,  147,  147,  147,  147,  147,  106,  147,  147,  147,
 /*  1530 */   147,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1540 */   120,  121,  122,  123,  124,  125,   93,   96,   97,   98,
 /*  1550 */    99,  147,  101,  102,  103,  104,  147,  137,  107,  106,
 /*  1560 */   147,  147,  147,  147,  111,  112,  113,  114,  115,  116,
 /*  1570 */   117,  118,  119,  120,  121,  122,  123,  124,  125,   93,
 /*  1580 */     1,    2,    3,    4,  147,  147,  147,  147,  147,  147,
 /*  1590 */   137,  147,  106,  147,  147,  147,  147,  111,  112,  113,
 /*  1600 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1610 */   124,  125,  147,  147,  147,   36,  147,  147,   39,  147,
 /*  1620 */    93,  147,  147,  137,  147,    1,    2,    3,    4,  147,
 /*  1630 */   147,  147,  147,  106,  147,  147,  147,  147,  111,  112,
 /*  1640 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1650 */   123,  124,  125,   93,  147,   31,  147,  147,   34,  147,
 /*  1660 */    36,  147,  147,   39,  137,  147,  106,  147,  147,  147,
 /*  1670 */   147,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1680 */   120,  121,  122,  123,  124,  125,   93,  147,  147,  147,
 /*  1690 */   147,  147,  147,  147,  147,  147,  147,  137,  147,  106,
 /*  1700 */   147,  147,  147,  147,  111,  112,  113,  114,  115,  116,
 /*  1710 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  147,
 /*  1720 */   147,  147,  147,  147,  147,  147,  147,   93,  147,  147,
 /*  1730 */   137,  147,    1,    2,    3,    4,  147,  147,  147,  147,
 /*  1740 */   106,  147,  147,  147,  147,  111,  112,  113,  114,  115,
 /*  1750 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1760 */    93,  147,   31,  147,  147,   34,  147,   36,  147,  147,
 /*  1770 */    39,  137,  147,  106,  147,  147,  147,  147,  111,  112,
 /*  1780 */   113,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1790 */   123,  124,  125,   93,  147,  147,  147,  147,  147,  147,
 /*  1800 */   147,  147,  147,  147,  137,  147,  106,  147,  147,  147,
 /*  1810 */   147,  111,  112,  113,  114,  115,  116,  117,  118,  119,
 /*  1820 */   120,  121,  122,  123,  124,  125,  147,  147,  147,  147,
 /*  1830 */   147,  147,  147,  147,   93,  147,  147,  137,  147,  147,
 /*  1840 */   147,  147,  147,  147,  147,  147,  147,  106,  147,  147,
 /*  1850 */   147,  147,  111,  112,  113,  114,  115,  116,  117,  118,
 /*  1860 */   119,  120,  121,  122,  123,  124,  125,   19,   20,   21,
 /*  1870 */    22,   23,   24,   25,  147,   27,  147,  147,  137,  147,
 /*  1880 */    19,   20,   21,   22,   23,   24,   38,  147,   27,  147,
 /*  1890 */   147,  147,  147,  147,  147,  147,  147,  147,  147,   38,
 /*  1900 */   147,   19,   20,   21,   22,   23,   24,  147,  147,   27,
 /*  1910 */   147,  147,  147,  147,   66,  147,  147,  147,  147,   58,
 /*  1920 */    38,   73,   74,  147,  147,  147,  147,   66,  147,   81,
 /*  1930 */    82,   83,   84,   85,   73,   74,  147,  147,  147,  147,
 /*  1940 */    58,  147,   81,   82,   83,   84,   85,  147,   66,   19,
 /*  1950 */    20,   21,   22,   23,   24,   73,   74,   27,  147,  147,
 /*  1960 */   147,  147,   27,   81,   82,   83,   84,   85,   38,   19,
 /*  1970 */    20,   21,   22,   23,   24,   25,  147,   27,   95,  147,
 /*  1980 */    97,   98,   99,  147,  101,  102,  103,  104,   38,  147,
 /*  1990 */   107,  147,    1,    2,    3,    4,   66,   67,  147,  147,
 /*  2000 */   147,  147,   67,   73,   74,  147,  147,  147,   73,   74,
 /*  2010 */   147,   81,   82,   83,   84,   85,   66,   82,   83,   84,
 /*  2020 */    85,  147,   31,   73,   74,   34,  147,   36,  147,  147,
 /*  2030 */    39,   81,   82,   83,   84,   85,   19,   20,   21,   22,
 /*  2040 */    23,   24,   25,  147,   27,  147,  147,  147,  147,  147,
 /*  2050 */   147,  147,  147,  147,  147,   38,   19,   20,   21,   22,
 /*  2060 */    23,   24,  147,  147,   27,  147,  147,  147,  147,  147,
 /*  2070 */   147,  147,  147,  147,  147,   38,   19,   20,   21,   22,
 /*  2080 */    23,   24,  147,   66,   27,  147,  147,  147,  147,  147,
 /*  2090 */    73,   74,  147,  147,  147,   38,  147,  147,   81,   82,
 /*  2100 */    83,   84,   85,   66,   67,  147,  147,  147,  147,  147,
 /*  2110 */    73,   74,  147,  147,  147,  147,  147,  147,   81,   82,
 /*  2120 */    83,   84,   85,   66,  147,  147,  147,  147,  147,  147,
 /*  2130 */    73,   74,    1,    2,    3,    4,  147,  147,   81,   82,
 /*  2140 */    83,   84,   85,  147,  147,    6,    7,    8,    9,   10,
 /*  2150 */    11,   12,   13,   14,   15,   16,   17,   18,  147,  147,
 /*  2160 */   147,  147,   31,  147,  147,   34,  147,   36,  147,   30,
 /*  2170 */    39,  147,  147,  147,  147,  147,    6,    7,    8,    9,
 /*  2180 */    10,   11,   12,   13,   14,   15,   16,   17,   18,  147,
 /*  2190 */     1,    2,    3,    4,    1,    2,    3,    4,  147,  147,
 /*  2200 */    30,  147,  147,  147,  147,  147,  147,    6,    7,    8,
 /*  2210 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2220 */    31,  147,  147,   34,   31,   36,  147,   34,   39,   36,
 /*  2230 */   147,   30,   39,  147,    1,    2,    3,    4,    6,    7,
 /*  2240 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2250 */    18,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2260 */    15,   16,   17,   18,   31,  147,  147,   34,  147,   36,
 /*  2270 */   147,  147,   39,  147,  147,   30,  147,  147,  147,  147,
 /*  2280 */   147,  147,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2290 */    14,   15,   16,   17,   18,  147,  147,  147,  147,   67,
 /*  2300 */   147,  147,  147,  147,  147,  147,   30,  147,  147,  147,
 /*  2310 */   147,  147,  147,    6,    7,    8,    9,   10,   11,   12,
 /*  2320 */    13,   14,   15,   16,   17,   18,  147,  147,  147,  147,
 /*  2330 */   147,  147,  147,  147,  147,   28,  147,  147,  147,  147,
 /*  2340 */   147,  147,    6,    7,    8,    9,   10,   11,   12,   13,
 /*  2350 */    14,   15,   16,   17,   18,  147,  147,  147,  147,  147,
 /*  2360 */   147,   25,  147,  147,  147,  147,  147,  147,    6,    7,
 /*  2370 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2380 */    18,   94,   95,   96,   97,   98,   99,  147,  101,  102,
 /*  2390 */   103,  104,  147,  147,  107,  147,  147,  147,  147,  147,
 /*  2400 */   147,  147,  147,  147,  147,   10,  147,  147,   94,   95,
 /*  2410 */    96,   97,   98,   99,  147,  101,  102,  103,  104,  147,
 /*  2420 */    25,  107,   27,  147,  147,  147,  147,  147,  147,  147,
 /*  2430 */   147,  147,  147,  147,  147,   40,   41,   42,   43,   44,
 /*  2440 */    45,
};
#define YY_SHIFT_USE_DFLT (-28)
static short yy_shift_ofst[] = {
 /*     0 */   123,   24,  555,  -28,  -28,  -28,  -28,   20,   58,  -28,
 /*    10 */   137,  466, 2131,  142,  -28,  -28, 2189,  173,  -28,  152,
 /*    20 */   240,  187, 2395,  270,  -27,  -28,  358,  -28,  -28,  -28,
 /*    30 */   -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28,
 /*    40 */   -28,  -28,  -28,  -28,  293,  -28,  326,  -28, 2057,  276,
 /*    50 */    83, 1018,  342,  379,  853,  963,  -28, 2057,  418,  191,
 /*    60 */   -28,  175,  -28,  -28,  177,  445, 1175, 1175,  470,  743,
 /*    70 */    28,  -28, 2057,  521,  798,  633,  -28,  510,  382, 1882,
 /*    80 */  2170, 1175,  908,  -28,  531,  475,  -28,  531,  -28, 1413,
 /*    90 */   -28,  -28,  -28,  -28,  -28,  -28, 2057,  -28, 2362, 2057,
 /*   100 */  2057,  -28, 2057,  570,  -28,  544,   19,  513,  -28,  546,
 /*   110 */   -28,  560,  -28,   95, 2057,   95, 2057,   95, 2057,   95,
 /*   120 */  2057,   95, 2057,   95, 2057,   95, 2057,  -28, 2057,  -28,
 /*   130 */  2057,  -28, 2057, 1400, 2057, 1400, 1246,  -28,  -28,  -28,
 /*   140 */   -28,  -28,  -28, 2336,  -28,  634, 2057,  -28,  215,  639,
 /*   150 */   625, 2017, 2362, 2057,  -28,  641,  666,  570,  -28, 2057,
 /*   160 */   -28,  -28,  -28,  -28,  -28,  -28,  -28,  -28, 1935,  -28,
 /*   170 */   317,  -28, 1154,  -28,  644,    6,  -28,  -28,  -28,  -28,
 /*   180 */   -28,  -28,  -28,  -28,  -28,  -28,  650,  669,  674,  -28,
 /*   190 */   700,  694, 1950,  -28,  391,  -28, 2057,  -28,  -28,  -28,
 /*   200 */   496,  -28,  -28,  534,  689,  -28,  695,  -28,  702,  -28,
 /*   210 */  1848,  606,  -28,  -28,   95,  714,   84, 2057,  -28, 1930,
 /*   220 */   360, 2057,  -28,  704,  360, 2057,  -28, 2232, 2037,  360,
 /*   230 */  2057,  -28,  709,  360, 2057,  -28,  -28,  -28,  -28, 2057,
 /*   240 */   459,  -28, 2057,  -28, 2362,  -28,  729,  -28, 2057, 2307,
 /*   250 */   -28,  752,  180,  755,  759,  -28,  452,  725,  766,  -28,
 /*   260 */   728,  768,  -28,  733,  773,  -28,  306,  775,  -28,  777,
 /*   270 */   767, 2057,    5,  778,  -28,  780,  718,  -28,  780,  -28,
 /*   280 */   774,  177,  -28,  -28,  780,  690,  -28,  780,  637,  -28,
 /*   290 */   780,  601,  -28,  780,  679,  -28,  780,  692,  -28,  780,
 /*   300 */   745,  -28,  780,   67,  -28,  780,  271,  -28,  780,  365,
 /*   310 */   -28,  746,  785,  625,  -28,  -28, 2057, 2245, 1175,  193,
 /*   320 */   -28,  789,  760, 1861, 2201, 1175, 1073,  -28, 2057, 2276,
 /*   330 */  1175,  468,  -28,  -28, 2139,  -28,  772, 1175, 1175,  -28,
 /*   340 */   -28,  779,  796,  303,  413,  -28,  -28,  647,  797,  248,
 /*   350 */   -28,  138,  -28,   -5,  -28,  795,  177,  -28,  806,  799,
 /*   360 */   177,  -28,  809,  802,  177,  -28,  820,  838,  823,  -28,
 /*   370 */   503,  814,  828,  818, 1216,  829,  688,  -28,  523,  -28,
 /*   380 */   636,  830, 1128,  -28,  578,  -28,  821,  516,  824,  184,
 /*   390 */   825,  826,   59,  827,  834,  835,  349,  842,  844,  130,
 /*   400 */   845,  846, 1731,  198,  -28,  -28, 1579,  839,  832,  177,
 /*   410 */   -26,  -28,  858,  851,  177,  861,  -28,  -28, 1278,  448,
 /*   420 */   170,  -28,  177,  862,  -28,  -28,  -28,  -28,  -28,  -28,
 /*   430 */   -28,  860,  299,  865,  404,  532,  548,  -28,  177,  877,
 /*   440 */   -28,  -28, 2193,   63,  363,  528,  338,  -28,  253,  879,
 /*   450 */   880, 2233,  882,  -28,  -28,  885,  884,   86, 1991,  886,
 /*   460 */   -28,  -28,  888,  889, 1624,  887,  -28,  -28,  -28,
};
#define YY_REDUCE_USE_DFLT (-113)
static short yy_reduce_ofst[] = {
 /*     0 */   221, -113,  524, -113, -113, -113, -113, -113, -113, -113,
 /*    10 */  -113, -113, 1298, -113, -113, -113, 1451,  228, -113, -113,
 /*    20 */  -113, -113,  482, -113, 1667, -113, 1184, -113, -113, -113,
 /*    30 */  -113, -113, -113, -113, -113, -113, -113, -113, -113, -113,
 /*    40 */  -113, -113, -113, -113, -113, -113, -113, -113,  234, -113,
 /*    50 */  1379, 1184, -113, -113, 1741, 1184, -113,  148, -113,  304,
 /*    60 */  -113,  314, -113, -113,  373, -113, 1527, 1184, -113, 1634,
 /*    70 */  1184, -113,   14, -113, 1230, 1184, -113, -113, -113,   66,
 /*    80 */  -113, 1346, 1184, -113,  315, -113, -113,  431, -113,  440,
 /*    90 */  -113, -113, -113, -113, -113, -113,  -52, -113, -113,  587,
 /*   100 */   588, -113,  608,  434, -113, -113, -113, -113, -113, -113,
 /*   110 */  -113, -113, -113, -113,  239, -113,  328, -113,  412, -113,
 /*   120 */   507, -113,  540, -113,  553, -113,  557, -113,  569, -113,
 /*   130 */   624, -113,  129, -113,  533, -113,  281, -113, -113, -113,
 /*   140 */  -113, -113, -113, -113, -113, -113,  467, -113, -113, -113,
 /*   150 */  -113,  218, -113,  562, -113, -113, -113,  573, -113,  357,
 /*   160 */  -113, -113, -113, -113, -113, -113, -113, -113,  -88, -113,
 /*   170 */  -113, -113,   22, -113, -113,  428, -113, -113, -113, -113,
 /*   180 */  -113, -113, -113, -113, -113, -113, -113, -113, -113, -113,
 /*   190 */  -113, -113,   53, -113, -113, -113,  -28, -113, -113, -113,
 /*   200 */  -113, -113, -113, -113, -113, -113, -113, -113, -113, -113,
 /*   210 */    -2, -113, -113, -113, -113, -113,  640,  168, -113,  149,
 /*   220 */   643,  514, -113, -113,  646,  423, -113, -113,  383,  648,
 /*   230 */   333, -113, -113,  649,  278, -113, -113, -113, -113,  290,
 /*   240 */  -113, -113,  -95, -113, -113, -113, -113, -113,   82, -113,
 /*   250 */  -113, -113, -113, -113, -113, -113, -113, -113, -113, -113,
 /*   260 */  -113, -113, -113, -113, -113, -113, -113, -113, -113, -113,
 /*   270 */  -113, -112, -113, -113, -113,  615, -113, -113,  668, -113,
 /*   280 */  -113,  710, -113, -113,  600, -113, -113,  506, -113, -113,
 /*   290 */   446, -113, -113,  561, -113, -113,  609, -113, -113,  612,
 /*   300 */  -113, -113,  -19, -113, -113,   31, -113, -113,   91, -113,
 /*   310 */  -113, -113, -113, -113, -113, -113,  493, -113, 1593, 1184,
 /*   320 */  -113, -113, -113,  556, -113, 1486, 1184, -113,  345, -113,
 /*   330 */  1560, 1184, -113, -113, -113, -113, -113, 1700, 1184, -113,
 /*   340 */  -113, -113, -113, 1453, 1184, -113, -113, -113, -113, 1420,
 /*   350 */  -113, 1184, -113,  207, -113, -113,  731, -113, -113, -113,
 /*   360 */   735, -113, -113, -113,  742, -113, -113, -113, -113, -113,
 /*   370 */   -96, -113, -113, -113,  530, -113, 1313, -113, 1184, -113,
 /*   380 */  -113, -113, 1266, -113, 1184, -113, -113, -113, -113, -113,
 /*   390 */  -113, -113, -113, -113, -113, -113, -113, -113, -113, -113,
 /*   400 */  -113, -113,  374,  228, -113, -113,  -96, -113, -113,  765,
 /*   410 */  -113, -113, -113, -113,  788, -113, -113, -113,  -98,  790,
 /*   420 */  -113, -113,  792, -113, -113, -113, -113, -113, -113, -113,
 /*   430 */  -113, -113, -113, -113, -113,  790, -113, -113,  804, -113,
 /*   440 */  -113, -113, 1883, 1411,  228,  -98,  228, -113,  228, -113,
 /*   450 */  -113, 1267, -113, -113, -113, -113, -113, -113, 2287, -113,
 /*   460 */  -113, -113, -113, -113, 2314, -113, -113, -113, -113,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   699,  699,  699,  471,  473,  474,  475,  699,  699,  476,
 /*    10 */   699,  699,  699,  699,  477,  481,  699,  699,  508,  699,
 /*    20 */   699,  699,  699,  699,  699,  512,  699,  514,  550,  552,
 /*    30 */   553,  554,  555,  556,  557,  558,  559,  560,  561,  562,
 /*    40 */   563,  564,  565,  566,  699,  567,  699,  568,  699,  699,
 /*    50 */   699,  699,  571,  699,  699,  699,  572,  699,  699,  699,
 /*    60 */   575,  699,  576,  577,  699,  699,  699,  579,  699,  699,
 /*    70 */   699,  582,  699,  699,  699,  699,  584,  699,  699,  699,
 /*    80 */   699,  699,  699,  585,  699,  699,  589,  699,  590,  699,
 /*    90 */   592,  593,  594,  595,  596,  597,  699,  598,  633,  699,
 /*   100 */   699,  634,  699,  699,  635,  699,  699,  699,  694,  699,
 /*   110 */   695,  699,  696,  637,  699,  638,  699,  639,  699,  640,
 /*   120 */   699,  641,  699,  642,  699,  643,  699,  650,  699,  651,
 /*   130 */   699,  652,  699,  653,  699,  654,  699,  544,  545,  546,
 /*   140 */   547,  548,  549,  699,  644,  699,  699,  645,  658,  699,
 /*   150 */   646,  699,  678,  699,  655,  699,  699,  699,  656,  699,
 /*   160 */   657,  659,  660,  661,  662,  663,  664,  665,  699,  666,
 /*   170 */   699,  667,  699,  679,  699,  699,  681,  686,  687,  688,
 /*   180 */   689,  690,  691,  692,  693,  682,  687,  689,  688,  680,
 /*   190 */   699,  668,  699,  669,  699,  670,  699,  673,  676,  677,
 /*   200 */   699,  674,  675,  699,  699,  647,  699,  648,  699,  649,
 /*   210 */   699,  699,  671,  672,  636,  699,  699,  699,  599,  699,
 /*   220 */   699,  699,  600,  699,  699,  699,  601,  699,  699,  699,
 /*   230 */   699,  602,  699,  699,  699,  603,  604,  605,  591,  699,
 /*   240 */   699,  607,  699,  608,  610,  609,  699,  611,  699,  699,
 /*   250 */   612,  699,  699,  699,  699,  613,  699,  699,  699,  614,
 /*   260 */   699,  699,  615,  699,  699,  616,  699,  699,  617,  699,
 /*   270 */   699,  699,  699,  699,  618,  699,  699,  619,  699,  629,
 /*   280 */   631,  699,  632,  630,  699,  699,  620,  699,  699,  621,
 /*   290 */   699,  699,  622,  699,  699,  623,  699,  699,  624,  699,
 /*   300 */   699,  625,  699,  699,  626,  699,  699,  627,  699,  699,
 /*   310 */   628,  699,  699,  699,  698,  551,  699,  699,  699,  699,
 /*   320 */   586,  699,  699,  699,  699,  699,  699,  587,  699,  699,
 /*   330 */   699,  699,  588,  583,  699,  581,  699,  699,  580,  578,
 /*   340 */   573,  569,  699,  699,  699,  574,  570,  699,  699,  699,
 /*   350 */   513,  699,  515,  699,  535,  537,  699,  540,  699,  538,
 /*   360 */   699,  541,  699,  539,  699,  542,  699,  699,  699,  536,
 /*   370 */   699,  699,  699,  699,  699,  699,  699,  516,  699,  518,
 /*   380 */   699,  699,  699,  517,  699,  519,  699,  699,  699,  699,
 /*   390 */   699,  699,  699,  699,  699,  699,  699,  699,  699,  699,
 /*   400 */   699,  699,  699,  699,  506,  509,  699,  699,  699,  699,
 /*   410 */   699,  510,  699,  699,  699,  699,  511,  494,  699,  699,
 /*   420 */   699,  496,  699,  699,  498,  500,  502,  503,  504,  505,
 /*   430 */   501,  502,  503,  504,  505,  699,  699,  497,  699,  699,
 /*   440 */   499,  507,  699,  699,  699,  699,  699,  495,  699,  699,
 /*   450 */   699,  699,  699,  478,  482,  699,  699,  699,  699,  699,
 /*   460 */   479,  483,  699,  699,  699,  699,  480,  484,  472,
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
  "NOT",           "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "EXTENDS",       "ABSTRACT",      "COMMENT",       "ASSIGN",      
  "CONST",         "FUNCTION",      "PARENTHESES_OPEN",  "INLINE",      
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
 /* 176 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 177 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 178 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 179 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 180 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 181 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 182 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 183 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 184 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 185 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 186 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 187 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 188 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 189 */ "xx_common_expr ::= IDENTIFIER",
 /* 190 */ "xx_common_expr ::= INTEGER",
 /* 191 */ "xx_common_expr ::= STRING",
 /* 192 */ "xx_common_expr ::= SCHAR",
 /* 193 */ "xx_common_expr ::= DOUBLE",
 /* 194 */ "xx_common_expr ::= NULL",
 /* 195 */ "xx_common_expr ::= TRUE",
 /* 196 */ "xx_common_expr ::= FALSE",
 /* 197 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 198 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 199 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 200 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 201 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 202 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 203 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 204 */ "xx_common_expr ::= xx_mcall_expr",
 /* 205 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 206 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 207 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 208 */ "xx_call_parameters ::= xx_call_parameter",
 /* 209 */ "xx_call_parameter ::= xx_common_expr",
 /* 210 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 211 */ "xx_array_list ::= xx_array_item",
 /* 212 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 213 */ "xx_array_item ::= xx_array_value",
 /* 214 */ "xx_array_key ::= IDENTIFIER",
 /* 215 */ "xx_array_key ::= STRING",
 /* 216 */ "xx_array_key ::= INTEGER",
 /* 217 */ "xx_array_value ::= xx_literal_expr",
 /* 218 */ "xx_array_value ::= IDENTIFIER",
 /* 219 */ "xx_literal_expr ::= INTEGER",
 /* 220 */ "xx_literal_expr ::= STRING",
 /* 221 */ "xx_literal_expr ::= DOUBLE",
 /* 222 */ "xx_literal_expr ::= NULL",
 /* 223 */ "xx_literal_expr ::= FALSE",
 /* 224 */ "xx_literal_expr ::= TRUE",
 /* 225 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 226 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 227 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 228 */ "xx_eval_expr ::= xx_common_expr",
 /* 229 */ "xx_comment ::= COMMENT",
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
// 781 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 1981 "parser.c"
      break;
    case 88:
// 794 "parser.lemon"
{ json_object_put((yypminor->yy133)); }
// 1986 "parser.c"
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
  { 129, 4 },
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
// 790 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy133;
}
// 2433 "parser.c"
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
      case 204:
      case 209:
      case 217:
      case 228:
// 796 "parser.lemon"
{
	yygotominor.yy133 = yymsp[0].minor.yy133;
}
// 2464 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 37:
      case 39:
      case 81:
      case 108:
// 800 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(yymsp[-1].minor.yy133, yymsp[0].minor.yy133);
}
// 2477 "parser.c"
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
      case 208:
      case 211:
// 804 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, yymsp[0].minor.yy133);
}
// 2497 "parser.c"
        break;
      case 7:
// 820 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2506 "parser.c"
        break;
      case 8:
// 824 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2516 "parser.c"
        break;
      case 9:
// 828 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy133, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2527 "parser.c"
        break;
      case 10:
// 832 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, 1, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2538 "parser.c"
        break;
      case 11:
// 836 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy133, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-7].minor);
  yy_destructor(29,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2550 "parser.c"
        break;
      case 12:
// 840 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2560 "parser.c"
        break;
      case 13:
// 844 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2571 "parser.c"
        break;
      case 14:
// 848 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2582 "parser.c"
        break;
      case 15:
// 852 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(29,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2594 "parser.c"
        break;
      case 16:
// 856 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
}
// 2601 "parser.c"
        break;
      case 17:
// 860 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy133, status->scanner_state);
}
// 2608 "parser.c"
        break;
      case 18:
// 864 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(NULL, yymsp[0].minor.yy133, NULL, status->scanner_state);
}
// 2615 "parser.c"
        break;
      case 19:
// 868 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[-1].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
}
// 2622 "parser.c"
        break;
      case 20:
// 872 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[-1].minor.yy133, NULL, yymsp[0].minor.yy133, status->scanner_state);
}
// 2629 "parser.c"
        break;
      case 21:
// 876 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[0].minor.yy133, NULL, yymsp[-1].minor.yy133, status->scanner_state);
}
// 2636 "parser.c"
        break;
      case 22:
// 880 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(NULL, yymsp[0].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
}
// 2643 "parser.c"
        break;
      case 23:
// 884 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[-2].minor.yy133, yymsp[0].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
}
// 2650 "parser.c"
        break;
      case 24:
// 888 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_definition(yymsp[-1].minor.yy133, yymsp[0].minor.yy133, yymsp[-2].minor.yy133, status->scanner_state);
}
// 2657 "parser.c"
        break;
      case 27:
// 900 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_property(yymsp[-2].minor.yy133, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2665 "parser.c"
        break;
      case 28:
// 904 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_property(yymsp[-2].minor.yy133, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 2673 "parser.c"
        break;
      case 29:
// 908 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_property(yymsp[-4].minor.yy133, yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2682 "parser.c"
        break;
      case 30:
// 912 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_property(yymsp[-4].minor.yy133, yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2691 "parser.c"
        break;
      case 33:
// 924 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2699 "parser.c"
        break;
      case 34:
// 928 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2707 "parser.c"
        break;
      case 35:
// 932 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2715 "parser.c"
        break;
      case 36:
// 936 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2723 "parser.c"
        break;
      case 41:
// 956 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2733 "parser.c"
        break;
      case 42:
// 960 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 2743 "parser.c"
        break;
      case 43:
// 964 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-6].minor.yy133, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2755 "parser.c"
        break;
      case 44:
// 968 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-7].minor.yy133, yymsp[-5].minor.yy0, yymsp[-3].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2767 "parser.c"
        break;
      case 45:
// 972 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-7].minor.yy133, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2779 "parser.c"
        break;
      case 46:
// 976 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-8].minor.yy133, yymsp[-6].minor.yy0, yymsp[-4].minor.yy133, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2791 "parser.c"
        break;
      case 47:
// 980 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-6].minor.yy133, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2803 "parser.c"
        break;
      case 48:
// 984 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-7].minor.yy133, yymsp[-5].minor.yy0, yymsp[-3].minor.yy133, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2815 "parser.c"
        break;
      case 49:
// 988 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-7].minor.yy133, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy133, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2827 "parser.c"
        break;
      case 50:
// 992 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_class_method(yymsp[-8].minor.yy133, yymsp[-6].minor.yy0, yymsp[-4].minor.yy133, yymsp[-1].minor.yy133, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(38,&yymsp[-5].minor);
  yy_destructor(25,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2839 "parser.c"
        break;
      case 52:
// 1004 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, json_object_new_string("public"));
  yy_destructor(1,&yymsp[0].minor);
}
// 2847 "parser.c"
        break;
      case 53:
// 1008 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, json_object_new_string("protected"));
  yy_destructor(2,&yymsp[0].minor);
}
// 2855 "parser.c"
        break;
      case 54:
// 1012 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, json_object_new_string("static"));
  yy_destructor(3,&yymsp[0].minor);
}
// 2863 "parser.c"
        break;
      case 55:
// 1016 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(NULL, json_object_new_string("private"));
  yy_destructor(4,&yymsp[0].minor);
}
// 2871 "parser.c"
        break;
      case 56:
// 1020 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(1,&yymsp[0].minor);
}
// 2880 "parser.c"
        break;
      case 57:
// 1024 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("public"));
  yy_destructor(1,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2889 "parser.c"
        break;
      case 58:
// 1028 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2898 "parser.c"
        break;
      case 59:
// 1032 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2907 "parser.c"
        break;
      case 60:
// 1036 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(3,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2916 "parser.c"
        break;
      case 61:
// 1040 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("static"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(3,&yymsp[0].minor);
}
// 2925 "parser.c"
        break;
      case 62:
// 1044 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(2,&yymsp[0].minor);
}
// 2934 "parser.c"
        break;
      case 63:
// 1048 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("protected"));
  yy_destructor(2,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2943 "parser.c"
        break;
      case 64:
// 1052 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(39,&yymsp[-1].minor);
  yy_destructor(4,&yymsp[0].minor);
}
// 2952 "parser.c"
        break;
      case 65:
// 1056 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(json_object_new_string("inline"), json_object_new_string("private"));
  yy_destructor(4,&yymsp[-1].minor);
  yy_destructor(39,&yymsp[0].minor);
}
// 2961 "parser.c"
        break;
      case 66:
      case 121:
      case 139:
      case 160:
      case 207:
      case 210:
// 1060 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_list(yymsp[-2].minor.yy133, yymsp[0].minor.yy133);
  yy_destructor(5,&yymsp[-1].minor);
}
// 2974 "parser.c"
        break;
      case 68:
// 1068 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2981 "parser.c"
        break;
      case 69:
// 1072 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(yymsp[-1].minor.yy133, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2988 "parser.c"
        break;
      case 70:
// 1076 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(NULL, yymsp[-1].minor.yy133, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 2995 "parser.c"
        break;
      case 71:
// 1080 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3003 "parser.c"
        break;
      case 72:
// 1084 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(yymsp[-3].minor.yy133, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3011 "parser.c"
        break;
      case 73:
// 1088 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_parameter(NULL, yymsp[-3].minor.yy133, yymsp[-2].minor.yy0, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3019 "parser.c"
        break;
      case 74:
// 1092 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(10,&yymsp[-2].minor);
  yy_destructor(11,&yymsp[0].minor);
}
// 3028 "parser.c"
        break;
      case 75:
// 1096 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(40,&yymsp[0].minor);
}
// 3036 "parser.c"
        break;
      case 76:
// 1100 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(41,&yymsp[0].minor);
}
// 3044 "parser.c"
        break;
      case 77:
// 1104 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(42,&yymsp[0].minor);
}
// 3052 "parser.c"
        break;
      case 78:
// 1108 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(43,&yymsp[0].minor);
}
// 3060 "parser.c"
        break;
      case 79:
// 1112 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(44,&yymsp[0].minor);
}
// 3068 "parser.c"
        break;
      case 80:
// 1116 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(45,&yymsp[0].minor);
}
// 3076 "parser.c"
        break;
      case 98:
// 1188 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(46,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3085 "parser.c"
        break;
      case 99:
// 1192 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(47,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3094 "parser.c"
        break;
      case 100:
// 1196 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-2].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3104 "parser.c"
        break;
      case 101:
// 1200 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-5].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3117 "parser.c"
        break;
      case 102:
// 1204 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-3].minor.yy133, yymsp[-1].minor.yy133, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3127 "parser.c"
        break;
      case 103:
// 1208 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-7].minor.yy133, yymsp[-5].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(48,&yymsp[-8].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3140 "parser.c"
        break;
      case 104:
// 1212 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-6].minor.yy133, yymsp[-4].minor.yy133, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3153 "parser.c"
        break;
      case 105:
// 1216 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_if_statement(yymsp[-6].minor.yy133, NULL, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3166 "parser.c"
        break;
      case 106:
// 1220 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_switch_statement(yymsp[-2].minor.yy133, NULL, status->scanner_state);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3176 "parser.c"
        break;
      case 107:
// 1224 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_switch_statement(yymsp[-3].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3186 "parser.c"
        break;
      case 110:
// 1236 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_case_clause(yymsp[-2].minor.yy133, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(51,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3195 "parser.c"
        break;
      case 111:
// 1240 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_case_clause(NULL, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3204 "parser.c"
        break;
      case 112:
// 1244 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3214 "parser.c"
        break;
      case 113:
// 1248 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_loop_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(54,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3224 "parser.c"
        break;
      case 114:
// 1252 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_while_statement(yymsp[-2].minor.yy133, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-3].minor);
  yy_destructor(30,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3234 "parser.c"
        break;
      case 115:
// 1256 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_while_statement(yymsp[-3].minor.yy133, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3244 "parser.c"
        break;
      case 116:
// 1260 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_for_statement(yymsp[-3].minor.yy133, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3255 "parser.c"
        break;
      case 117:
// 1264 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_for_statement(yymsp[-3].minor.yy133, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3267 "parser.c"
        break;
      case 118:
// 1268 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_for_statement(yymsp[-3].minor.yy133, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(56,&yymsp[-8].minor);
  yy_destructor(5,&yymsp[-6].minor);
  yy_destructor(57,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3279 "parser.c"
        break;
      case 119:
// 1272 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_for_statement(yymsp[-3].minor.yy133, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(56,&yymsp[-9].minor);
  yy_destructor(5,&yymsp[-7].minor);
  yy_destructor(57,&yymsp[-5].minor);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3292 "parser.c"
        break;
      case 120:
// 1276 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3301 "parser.c"
        break;
      case 123:
// 1289 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("assign");
  yy_destructor(35,&yymsp[0].minor);
}
// 3309 "parser.c"
        break;
      case 124:
// 1294 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("add-assign");
  yy_destructor(60,&yymsp[0].minor);
}
// 3317 "parser.c"
        break;
      case 125:
// 1299 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("sub-assign");
  yy_destructor(61,&yymsp[0].minor);
}
// 3325 "parser.c"
        break;
      case 126:
// 1303 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("mult-assign");
  yy_destructor(62,&yymsp[0].minor);
}
// 3333 "parser.c"
        break;
      case 127:
// 1307 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("div-assign");
  yy_destructor(63,&yymsp[0].minor);
}
// 3341 "parser.c"
        break;
      case 128:
// 1311 "parser.lemon"
{
	yygotominor.yy133 = json_object_new_string("concat-assign");
  yy_destructor(64,&yymsp[0].minor);
}
// 3349 "parser.c"
        break;
      case 129:
// 1316 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy133, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy133, status->scanner_state);
}
// 3356 "parser.c"
        break;
      case 130:
// 1321 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy133, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
}
// 3364 "parser.c"
        break;
      case 131:
// 1326 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy133, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(65,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3374 "parser.c"
        break;
      case 132:
// 1331 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy133, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy133, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3384 "parser.c"
        break;
      case 133:
// 1336 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy133, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3393 "parser.c"
        break;
      case 134:
// 1341 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy133, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy133, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(67,&yymsp[-2].minor);
}
// 3402 "parser.c"
        break;
      case 135:
// 1346 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(68,&yymsp[0].minor);
}
// 3410 "parser.c"
        break;
      case 136:
// 1351 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(69,&yymsp[0].minor);
}
// 3418 "parser.c"
        break;
      case 138:
// 1359 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_echo_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3427 "parser.c"
        break;
      case 141:
// 1371 "parser.lemon"
{
	yygotominor.yy133 = yymsp[0].minor.yy133;;
}
// 3434 "parser.c"
        break;
      case 142:
// 1376 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_mcall_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(28,&yymsp[0].minor);
}
// 3442 "parser.c"
        break;
      case 143:
// 1381 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_return_statement(yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3451 "parser.c"
        break;
      case 144:
// 1386 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3465 "parser.c"
        break;
      case 145:
// 1395 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3480 "parser.c"
        break;
      case 146:
// 1404 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3495 "parser.c"
        break;
      case 147:
// 1413 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(67,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3510 "parser.c"
        break;
      case 148:
// 1422 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3519 "parser.c"
        break;
      case 149:
// 1427 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy133, status->scanner_state);
  yy_destructor(75,&yymsp[-6].minor);
  yy_destructor(23,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3531 "parser.c"
        break;
      case 150:
// 1431 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3540 "parser.c"
        break;
      case 151:
// 1435 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3549 "parser.c"
        break;
      case 152:
// 1439 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3558 "parser.c"
        break;
      case 153:
// 1443 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3567 "parser.c"
        break;
      case 154:
// 1447 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3576 "parser.c"
        break;
      case 155:
// 1451 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3585 "parser.c"
        break;
      case 156:
// 1455 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(41,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3594 "parser.c"
        break;
      case 157:
// 1459 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3603 "parser.c"
        break;
      case 158:
// 1463 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3612 "parser.c"
        break;
      case 159:
// 1467 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3621 "parser.c"
        break;
      case 162:
// 1479 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3628 "parser.c"
        break;
      case 163:
// 1483 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 3636 "parser.c"
        break;
      case 165:
// 1491 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("not", yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 3644 "parser.c"
        break;
      case 166:
// 1495 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("isset", yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3652 "parser.c"
        break;
      case 167:
// 1499 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("equals", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3660 "parser.c"
        break;
      case 168:
// 1503 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("not-equals", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3668 "parser.c"
        break;
      case 169:
// 1507 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("identical", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3676 "parser.c"
        break;
      case 170:
// 1511 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("not-identical", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3684 "parser.c"
        break;
      case 171:
// 1515 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("less", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3692 "parser.c"
        break;
      case 172:
// 1519 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("greater", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3700 "parser.c"
        break;
      case 173:
// 1523 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("less-equal", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3708 "parser.c"
        break;
      case 174:
// 1527 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3716 "parser.c"
        break;
      case 175:
// 1531 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("list", yymsp[-1].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3725 "parser.c"
        break;
      case 176:
// 1535 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("cast", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(25,&yymsp[-1].minor);
}
// 3734 "parser.c"
        break;
      case 177:
// 1539 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-1].minor);
}
// 3742 "parser.c"
        break;
      case 178:
      case 225:
// 1543 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3752 "parser.c"
        break;
      case 179:
      case 226:
// 1548 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3762 "parser.c"
        break;
      case 180:
      case 227:
// 1553 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3772 "parser.c"
        break;
      case 181:
// 1558 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("add", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3780 "parser.c"
        break;
      case 182:
// 1563 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("sub", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3788 "parser.c"
        break;
      case 183:
// 1568 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("concat", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3796 "parser.c"
        break;
      case 184:
// 1573 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("and", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3804 "parser.c"
        break;
      case 185:
// 1578 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("or", yymsp[-2].minor.yy133, yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3812 "parser.c"
        break;
      case 186:
// 1583 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("instanceof", yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3820 "parser.c"
        break;
      case 187:
// 1588 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy133, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(5,&yymsp[-1].minor);
}
// 3829 "parser.c"
        break;
      case 188:
// 1593 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("typeof", yymsp[0].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3837 "parser.c"
        break;
      case 189:
      case 214:
      case 218:
// 1598 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3846 "parser.c"
        break;
      case 190:
      case 216:
      case 219:
// 1603 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3855 "parser.c"
        break;
      case 191:
      case 215:
      case 220:
// 1608 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 3864 "parser.c"
        break;
      case 192:
// 1613 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 3871 "parser.c"
        break;
      case 193:
      case 221:
// 1618 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 3879 "parser.c"
        break;
      case 194:
      case 222:
// 1623 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(83,&yymsp[0].minor);
}
// 3888 "parser.c"
        break;
      case 195:
      case 224:
// 1628 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[0].minor);
}
// 3897 "parser.c"
        break;
      case 196:
      case 223:
// 1633 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[0].minor);
}
// 3906 "parser.c"
        break;
      case 197:
// 1638 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-1].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3915 "parser.c"
        break;
      case 198:
// 1643 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_expr("array", yymsp[-1].minor.yy133, NULL, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(67,&yymsp[0].minor);
}
// 3924 "parser.c"
        break;
      case 199:
// 1648 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3932 "parser.c"
        break;
      case 200:
// 1653 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3942 "parser.c"
        break;
      case 201:
// 1658 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(23,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3952 "parser.c"
        break;
      case 202:
// 1663 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3961 "parser.c"
        break;
      case 203:
// 1668 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3970 "parser.c"
        break;
      case 205:
// 1678 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy133, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3980 "parser.c"
        break;
      case 206:
// 1683 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(25,&yymsp[0].minor);
}
// 3990 "parser.c"
        break;
      case 212:
// 1707 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_array_item(yymsp[-2].minor.yy133, yymsp[0].minor.yy133, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 3998 "parser.c"
        break;
      case 213:
// 1711 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_array_item(NULL, yymsp[0].minor.yy133, status->scanner_state);
}
// 4005 "parser.c"
        break;
      case 229:
// 1778 "parser.lemon"
{
	yygotominor.yy133 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4012 "parser.c"
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
// 747 "parser.lemon"


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

// 4088 "parser.c"
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
