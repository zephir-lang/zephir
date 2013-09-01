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
		case XX_T_CONSTANT:
			json_object_object_add(ret, "type", json_object_new_string("constant"));
			break;
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

static json_object *xx_ret_do_while_statement(json_object *expr, json_object *statements, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("do-while"));
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

static json_object *xx_ret_scall_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("scall"));
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

static json_object *xx_ret_scall(xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("scall"));
	json_object_object_add(ret, "class", json_object_new_string(O->token));
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


// 765 "parser.c"
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
#define YYNOCODE 154
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy115;
  int yy307;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 496
#define YYNRULE 238
#define YYERRORSYMBOL 91
#define YYERRSYMDT yy307
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
 /*     0 */    89,  148,  152,  146,  184,   86,  291,  294,  141,  718,
 /*    10 */   439,   17,  454,  462,    7,  468,  442,   10,  154,  438,
 /*    20 */   124,  482,  354,  125,  126,  127,  128,  129,  130,  131,
 /*    30 */   132,  133,  134,  135,  149,  120,  122,   88,  102,  106,
 /*    40 */   108,  110,  112,  104,  100,  114,  116,  118,  153,  414,
 /*    50 */   415,  417,  416,  418,  456,  162,  191,  192,  349,  221,
 /*    60 */   438,  171,   54,  155,  156,  354,  157,  158,  159,  160,
 /*    70 */   161,  361,   47,  313,  322,  331,  334,  325,  328,  337,
 /*    80 */   343,  340,  420,  346,   46,   48,   50,   20,   59,  419,
 /*    90 */   421,  719,   70,   74,   79,  217,  190,  470,  224,  473,
 /*   100 */   474,  462,  349,  441,  442,  357,   27,  438,  170,  354,
 /*   110 */   275,  286,  289,  191,  192,  304,  311,  313,  322,  331,
 /*   120 */   334,  325,  328,  337,  343,  340,  145,  346,   46,   48,
 /*   130 */    50,  219,   59,  458,  191,  192,   70,   74,   79,  217,
 /*   140 */   459,  193,  224,  471,  454,  462,  349,  413,  442,  111,
 /*   150 */   373,  438,  720,  354,  275,  286,  289,  191,  192,  304,
 /*   160 */    91,  313,  322,  331,  334,  325,  328,  337,  343,  340,
 /*   170 */   251,  346,   46,   48,   50,  247,   59,  206,  191,  192,
 /*   180 */    70,   74,   79,  217,   93,  352,  224,  440,  350,   19,
 /*   190 */   349,  441,  442,   11,  384,  438,  460,  354,  275,  286,
 /*   200 */   289,  497,   12,  304,  476,  313,  322,  331,  334,  325,
 /*   210 */   328,  337,  343,  340,  251,  346,   46,   48,   50,  260,
 /*   220 */    59,  461,  191,  192,   70,   74,   79,  217,    7,  283,
 /*   230 */   224,   10,  208,  210,  349,  482,  354,  123,   25,   95,
 /*   240 */    97,  354,  275,  286,  289,  191,  192,  304,  207,  313,
 /*   250 */   322,  331,  334,  325,  328,  337,  343,  340,  251,  346,
 /*   260 */    46,   48,   50,  268,   59,  285,  191,  192,   70,   74,
 /*   270 */    79,  217,  390,  199,  224,  412,  401,  405,  349,  362,
 /*   280 */   138,  140,  395,  204,  150,  354,  275,  286,  289,  191,
 /*   290 */   192,  304,   64,  313,  322,  331,  334,  325,  328,  337,
 /*   300 */   343,  340,  136,  346,   46,   48,   50,   62,   59,   94,
 /*   310 */   191,  192,   70,   74,   79,  217,  238,  423,  224,   66,
 /*   320 */   237,  379,  349,  438,  191,  192,  393,  466,  316,  354,
 /*   330 */   275,  286,  289,  467,   66,  304,  379,  313,  322,  331,
 /*   340 */   334,  325,  328,  337,  343,  340,  280,  346,   46,   48,
 /*   350 */    50,  315,   59,  279,  191,  192,   70,   74,   79,  217,
 /*   360 */    51,  307,  224,  216,  432,  305,  349,  412,  401,  405,
 /*   370 */   388,  191,  192,  354,  275,  286,  289,  332,  321,  304,
 /*   380 */   240,  313,  322,  331,  334,  325,  328,  337,  343,  340,
 /*   390 */   363,  346,   46,   48,   50,   65,   59,  114,  116,  118,
 /*   400 */    70,   74,   79,  217,   60,   19,  224,  216,   18,  185,
 /*   410 */   349,  438,    9,  117,  389,  191,  192,  354,  275,  286,
 /*   420 */   289,  191,  192,  304,  241,  313,  322,  331,  334,  325,
 /*   430 */   328,  337,  343,  340,  109,  346,   46,   48,   50,  485,
 /*   440 */    59,  489,  191,  192,   70,   74,   79,  217,  238,    8,
 /*   450 */   224,  238,  242,  490,  349,  246,  191,  192,   78,  191,
 /*   460 */   192,  354,  275,  286,  289,  323,  321,  304,  290,  313,
 /*   470 */   322,  331,  334,  325,  328,  337,  343,  340,  115,  346,
 /*   480 */    46,   48,   50,  209,   59,  295,  191,  192,   70,   74,
 /*   490 */    79,  217,   75,  477,  224,  216,  329,  321,  349,  347,
 /*   500 */   321,  220,  377,  191,  192,  354,  275,  286,  289,  191,
 /*   510 */   192,  304,  478,  313,  322,  331,  334,  325,  328,  337,
 /*   520 */   343,  340,  364,  346,   46,   48,   50,  292,   59,  186,
 /*   530 */   191,  192,   70,   74,   79,  217,   84,  293,  224,  216,
 /*   540 */   298,  301,  349,  335,  321,  287,  372,  191,  192,  354,
 /*   550 */   275,  286,  289,  191,  192,  304,  480,  313,  322,  331,
 /*   560 */   334,  325,  328,  337,  343,  340,  113,  346,   46,   48,
 /*   570 */    50,  151,   59,  211,  191,  192,   70,   74,   79,  217,
 /*   580 */   238,  245,  224,  238,  250,  483,  349,  255,  191,  192,
 /*   590 */    73,  191,  192,  354,  275,  286,  289,  190,  374,  304,
 /*   600 */   484,  313,  322,  331,  334,  325,  328,  337,  343,  340,
 /*   610 */    87,  346,   46,   48,   50,  296,   59,  189,  191,  192,
 /*   620 */    70,   74,   79,  217,  238,  491,  224,  238,  259,  297,
 /*   630 */   349,  263,  191,  192,  378,  191,  192,  354,  275,  286,
 /*   640 */   289,  190,  376,  304,  487,  313,  322,  331,  334,  325,
 /*   650 */   328,  337,  343,  340,  369,  346,   46,   48,   50,   98,
 /*   660 */    59,  214,  191,  192,   70,   74,   79,  217,  238,  248,
 /*   670 */   224,  238,  267,   80,  349,  271,  191,  192,  367,  191,
 /*   680 */   192,  354,  275,  286,  289,  338,  321,  304,  300,  313,
 /*   690 */   322,  331,  334,  325,  328,  337,  343,  340,  119,  346,
 /*   700 */    46,   48,   50,  166,   59,  299,  191,  192,   70,   74,
 /*   710 */    79,  217,  375,  464,  224,  216,  326,  321,  349,  316,
 /*   720 */   465,  107,  223,  191,  192,  354,  275,  286,  289,  191,
 /*   730 */   192,  304,   92,  313,  322,  331,  334,  325,  328,  337,
 /*   740 */   343,  340,  336,  346,   46,   48,   50,  380,   59,  397,
 /*   750 */   401,  405,   70,   74,   79,  217,  147,  249,  224,  121,
 /*   760 */   341,  321,  349,   76,  191,  192,  360,  191,  192,  354,
 /*   770 */   275,  286,  289,  165,  302,  304,   96,  313,  322,  331,
 /*   780 */   334,  325,  328,  337,  343,  340,   99,  346,   46,   48,
 /*   790 */    50,  139,   59,  303,  191,  192,   70,   74,   79,  217,
 /*   800 */   101,   61,  224,  103,   63,  382,  349,  385,  191,  192,
 /*   810 */   437,  191,  192,  354,  275,  286,  289,  225,  274,  304,
 /*   820 */   169,  313,  322,  331,  334,  325,  328,  337,  343,  340,
 /*   830 */   105,  346,   46,   48,   50,  396,   59,  386,  191,  192,
 /*   840 */    70,   74,   79,  217,  314,  321,  224,  344,  321,  306,
 /*   850 */   349,  253,   83,  308,  435,  433,  254,  354,  275,  286,
 /*   860 */   289,  396,  392,  304,  309,  313,  322,  331,  334,  325,
 /*   870 */   328,  337,  343,  340,  316,  346,   46,   48,   50,  190,
 /*   880 */    59,  391,  316,  227,   70,   74,   79,  217,  190,  218,
 /*   890 */   224,  312,  496,  318,  349,   56,  258,  324,   82,  196,
 /*   900 */   317,  354,  275,  286,  289,  333,  226,  304,  202,  313,
 /*   910 */   322,  331,  334,  325,  328,  337,  343,  340,  316,  346,
 /*   920 */    46,   48,   50,  493,   59,  399,  316,  316,   70,   74,
 /*   930 */    79,  217,  400,  402,  224,  319,  403,  404,  349,  261,
 /*   940 */   320,  339,  431,   55,  406,  354,  275,  286,  289,  348,
 /*   950 */   330,  304,  262,  313,  322,  331,  334,  325,  328,  337,
 /*   960 */   343,  340,  278,  346,   46,   48,   50,  407,   59,   71,
 /*   970 */   316,  316,   70,   74,   79,  217,  408,  410,  224,  411,
 /*   980 */   559,   52,  349,   68,   67,  277,  429,  143,  266,  354,
 /*   990 */   275,  286,  289,  327,  342,  304,   90,  313,  322,  331,
 /*  1000 */   334,  325,  328,  337,  343,  340,  316,  346,   46,   48,
 /*  1010 */    50,  229,   59,  269,  425,   24,   70,   74,   79,  217,
 /*  1020 */   426,  144,  224,   49,  270,   22,  349,  428,  434,  345,
 /*  1030 */    58,   21,  228,  354,  275,  286,  289,  445,  236,  304,
 /*  1040 */   446,  313,  322,  331,  334,  325,  328,  337,  343,  340,
 /*  1050 */   200,  346,   46,   48,   50,  351,   59,   14,  447,  448,
 /*  1060 */    70,   74,   79,  217,  450,  353,  224,  451,  452,  453,
 /*  1070 */   349,  393,   85,  393,  383,  393,  393,  354,  275,  286,
 /*  1080 */   289,  393,  230,  304,  393,  313,  322,  331,  334,  325,
 /*  1090 */   328,  337,  343,  340,  393,  346,   46,   48,   50,  393,
 /*  1100 */    59,  393,  393,  393,   70,   74,   79,  217,  393,  393,
 /*  1110 */   224,  393,  393,  393,  349,  231,  232,  233,  234,  235,
 /*  1120 */   393,  354,  275,  286,  289,  393,  393,  304,  393,  313,
 /*  1130 */   322,  331,  334,  325,  328,  337,  343,  340,  393,  346,
 /*  1140 */    46,   48,   50,  393,   59,  393,  393,  145,   70,   74,
 /*  1150 */    79,  217,  171,  393,  224,  191,  192,  213,  393,   45,
 /*  1160 */   393,  393,  194,  393,  393,  393,  275,  286,  289,  393,
 /*  1170 */   430,  304,  393,  393,  393,  355,   29,   30,   31,   32,
 /*  1180 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1190 */    43,   44,  393,  393,   45,  164,  171,  183,  168,  179,
 /*  1200 */   393,  393,  393,  282,  284,   77,    3,    4,    5,    6,
 /*  1210 */   355,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1220 */    38,   39,   40,   41,   42,   43,   44,  393,  393,   45,
 /*  1230 */   735,    1,    2,  495,    4,    5,    6,  393,  282,  284,
 /*  1240 */   381,  167,  168,  179,  393,  355,   29,   30,   31,   32,
 /*  1250 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1260 */    43,   44,  393,  393,  393,   45,  393,  414,  415,  417,
 /*  1270 */   416,  418,  393,  282,  284,  393,  436,  212,  393,  393,
 /*  1280 */   180,  355,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1290 */    37,   38,   39,   40,   41,   42,   43,   44,  488,  393,
 /*  1300 */   420,  455,   45,  449,  393,  393,  393,  419,  421,  282,
 /*  1310 */   284,  142,  205,  359,  198,  172,  393,  393,  355,   29,
 /*  1320 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1330 */    40,   41,   42,   43,   44,  182,  181,   45,  393,  175,
 /*  1340 */   176,  178,  177,  393,  393,  393,  282,  284,   81,  393,
 /*  1350 */   393,  393,  393,  355,   29,   30,   31,   32,   33,   34,
 /*  1360 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1370 */   173,  174,   45,  393,  175,  176,  178,  177,  393,  393,
 /*  1380 */   393,  282,  284,   72,  393,  393,  393,  393,  355,   29,
 /*  1390 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1400 */    40,   41,   42,   43,   44,  393,  393,  393,   45,  393,
 /*  1410 */   414,  415,  417,  416,  418,  393,  282,  284,  393,  366,
 /*  1420 */   393,  393,  393,  393,  355,   29,   30,   31,   32,   33,
 /*  1430 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1440 */    44,  512,  393,  420,  455,   45,  449,  393,  230,  393,
 /*  1450 */   419,  421,  282,  284,  393,  393,   53,  393,  393,  393,
 /*  1460 */   393,  355,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1470 */    37,   38,   39,   40,   41,   42,   43,   44,  393,  393,
 /*  1480 */    45,  231,  232,  233,  234,  235,  393,  256,  393,  282,
 /*  1490 */   284,   69,  393,  393,  393,  393,  355,   29,   30,   31,
 /*  1500 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1510 */    42,   43,   44,  173,  174,   45,  393,  175,  176,  178,
 /*  1520 */   177,  393,  393,  393,  282,  284,  222,  393,  393,  393,
 /*  1530 */   393,  355,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1540 */    37,   38,   39,   40,   41,   42,   43,   44,  393,  393,
 /*  1550 */   393,   45,  393,  414,  415,  417,  416,  418,  393,  282,
 /*  1560 */   284,  393,  394,  393,  393,  393,  393,  355,   29,   30,
 /*  1570 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1580 */    41,   42,   43,   44,  516,  393,  420,  443,   45,  449,
 /*  1590 */   393,  230,  393,  419,  421,  282,  284,  393,  393,   57,
 /*  1600 */   393,  393,  393,  393,  355,   29,   30,   31,   32,   33,
 /*  1610 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1620 */    44,  393,  393,   45,  231,  232,  233,  234,  235,  393,
 /*  1630 */   243,  393,  282,  284,  371,  393,  393,  393,  393,  355,
 /*  1640 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1650 */    39,   40,   41,   42,   43,   44,  393,  393,   45,  393,
 /*  1660 */   393,  393,  393,  393,  393,  393,  393,  282,  284,   26,
 /*  1670 */   393,  393,  393,  393,  355,   29,   30,   31,   32,   33,
 /*  1680 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1690 */    44,  393,  393,  393,   45,  393,  414,  415,  417,  416,
 /*  1700 */   418,  393,  282,  284,  393,  387,  393,  393,  393,  393,
 /*  1710 */   355,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1720 */    38,   39,   40,   41,   42,   43,   44,   15,  393,  420,
 /*  1730 */   455,   45,  449,  393,  280,  393,  419,  421,  282,  284,
 /*  1740 */   276,  281,  191,  192,  393,  393,  393,   28,   29,   30,
 /*  1750 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1760 */    41,   42,   43,   44,   88,  102,  106,  108,  110,  112,
 /*  1770 */   104,  100,  114,  116,  118,  282,  284,   89,  148,  152,
 /*  1780 */   146,  184,   86,  197,  393,  141,  393,  393,  393,  393,
 /*  1790 */   393,  393,  393,  393,  393,  154,  393,  124,   89,  148,
 /*  1800 */   152,  146,  184,   86,  393,  393,  141,  393,  393,  393,
 /*  1810 */   393,  393,  393,  393,  393,  393,  154,  393,  124,   89,
 /*  1820 */   148,  152,  146,  184,   86,  393,  393,  141,  393,  393,
 /*  1830 */   393,  393,  162,  393,  393,  393,  393,  154,  393,  124,
 /*  1840 */   155,  156,  393,  157,  158,  159,  160,  161,  145,  393,
 /*  1850 */   393,  393,  393,  162,  257,  393,  191,  192,  201,  393,
 /*  1860 */   393,  155,  156,  194,  157,  158,  159,  160,  161,  393,
 /*  1870 */   393,  393,  393,  393,  162,  244,  393,  414,  415,  417,
 /*  1880 */   416,  418,  155,  156,  393,  157,  158,  159,  160,  161,
 /*  1890 */   393,  393,  393,  393,  393,   89,  148,  152,  146,  184,
 /*  1900 */    86,  393,  393,  141,  393,  393,  393,  393,  517,  393,
 /*  1910 */   420,  472,  393,  154,  393,  124,  393,  419,  421,   89,
 /*  1920 */   148,  152,  146,  184,   86,  393,  393,  141,  393,  393,
 /*  1930 */   393,  393,  393,  393,  230,  393,  393,  154,  393,  124,
 /*  1940 */   393,  393,  356,  393,   13,   16,  469,  475,  474,  462,
 /*  1950 */   162,  468,  442,  393,  393,  438,  393,  393,  155,  156,
 /*  1960 */   393,  157,  158,  159,  160,  161,  368,  231,  232,  233,
 /*  1970 */   234,  235,  239,  264,  162,  252,  272,  273,  393,  393,
 /*  1980 */   393,  393,  155,  156,  393,  157,  158,  159,  160,  161,
 /*  1990 */   414,  415,  417,  416,  418,   89,  148,  152,  146,  184,
 /*  2000 */    86,  215,  393,  141,  393,  393,  393,  393,  393,  393,
 /*  2010 */   393,  393,  393,  154,  393,  124,   89,  148,  152,  146,
 /*  2020 */   184,   86,  203,  420,  141,  393,  444,  393,  393,  393,
 /*  2030 */   419,  421,  393,  393,  154,  393,  124,   89,  148,  152,
 /*  2040 */   146,  184,   86,  187,  393,  141,  393,  393,  393,  393,
 /*  2050 */   162,  393,  393,  393,  393,  154,  393,  124,  155,  156,
 /*  2060 */   393,  157,  158,  159,  160,  161,  393,  393,  393,  393,
 /*  2070 */   393,  162,  393,  393,  393,  393,  393,  393,  393,  155,
 /*  2080 */   156,  145,  157,  158,  159,  160,  161,  393,  393,  191,
 /*  2090 */   192,  310,  162,  393,  393,  393,  194,  393,  393,  393,
 /*  2100 */   155,  156,  393,  157,  158,  159,  160,  161,  414,  415,
 /*  2110 */   417,  416,  418,   89,  148,  152,  146,  184,   86,  393,
 /*  2120 */   393,  141,  393,  414,  415,  417,  416,  418,  393,  393,
 /*  2130 */   393,  154,  393,  124,   89,  148,  152,  146,  184,   86,
 /*  2140 */   393,  420,  141,  393,  393,  393,  424,  393,  419,  421,
 /*  2150 */   457,  393,  154,  393,  124,  393,  420,  145,  393,  393,
 /*  2160 */   393,  424,  393,  419,  421,  191,  192,  195,  162,  265,
 /*  2170 */   393,  393,  194,  393,  393,  393,  155,  156,  145,  157,
 /*  2180 */   158,  159,  160,  161,  393,  393,  191,  192,  188,  162,
 /*  2190 */   393,  393,  393,  194,  393,  393,  393,  155,  156,  393,
 /*  2200 */   157,  158,  159,  160,  161,  120,  122,   88,  102,  106,
 /*  2210 */   108,  110,  112,  104,  100,  114,  116,  118,  409,  120,
 /*  2220 */   122,   88,  102,  106,  108,  110,  112,  104,  100,  114,
 /*  2230 */   116,  118,  393,   23,  393,  398,  393,  393,  393,  180,
 /*  2240 */   393,  393,  393,  365,  393,  414,  415,  417,  416,  418,
 /*  2250 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  2260 */   135,  120,  122,   88,  102,  106,  108,  110,  112,  104,
 /*  2270 */   100,  114,  116,  118,  634,  409,  513,  393,  420,  455,
 /*  2280 */   137,  449,  393,  393,  393,  419,  421,  163,  393,  393,
 /*  2290 */   427,  393,  398,  393,  182,  181,  393,  393,  175,  176,
 /*  2300 */   178,  177,  393,  393,  393,  393,  393,  125,  126,  127,
 /*  2310 */   128,  129,  130,  131,  132,  133,  134,  135,  120,  122,
 /*  2320 */    88,  102,  106,  108,  110,  112,  104,  100,  114,  116,
 /*  2330 */   118,  486,   16,  469,  475,  474,  462,  393,  468,  442,
 /*  2340 */   288,  393,  438,  393,  393,  393,  393,  393,  120,  122,
 /*  2350 */    88,  102,  106,  108,  110,  112,  104,  100,  114,  116,
 /*  2360 */   118,  409,  479,   16,  469,  475,  474,  462,  393,  468,
 /*  2370 */   442,  393,  358,  438,  393,  393,  393,  393,  398,  393,
 /*  2380 */   120,  122,   88,  102,  106,  108,  110,  112,  104,  100,
 /*  2390 */   114,  116,  118,  125,  126,  127,  128,  129,  130,  131,
 /*  2400 */   132,  133,  134,  135,  370,  414,  415,  417,  416,  418,
 /*  2410 */   393,  393,  120,  122,   88,  102,  106,  108,  110,  112,
 /*  2420 */   104,  100,  114,  116,  118,  414,  415,  417,  416,  418,
 /*  2430 */   393,  393,  393,  393,  393,  393,  481,  393,  420,  455,
 /*  2440 */   393,  449,  393,  393,  393,  419,  421,  414,  415,  417,
 /*  2450 */   416,  418,  393,  393,  393,  393,  494,  393,  420,  455,
 /*  2460 */   393,  449,  393,  393,  393,  419,  421,  414,  415,  417,
 /*  2470 */   416,  418,  414,  415,  417,  416,  418,  393,  519,  393,
 /*  2480 */   420,  422,  414,  415,  417,  416,  418,  419,  421,  393,
 /*  2490 */   393,  393,  393,  393,  393,  393,  393,  393,  514,  393,
 /*  2500 */   420,  422,  393,  520,  393,  420,  422,  419,  421,  463,
 /*  2510 */   393,  393,  419,  421,  393,  420,  393,  393,  393,  393,
 /*  2520 */    20,  393,  419,  421,  414,  415,  417,  416,  418,  414,
 /*  2530 */   415,  417,  416,  418,  393,  393,  492,   16,  469,  475,
 /*  2540 */   474,  462,  393,  468,  442,  393,  393,  438,  393,  414,
 /*  2550 */   415,  417,  416,  418,  393,  515,  393,  420,  422,  393,
 /*  2560 */   518,  393,  420,  422,  419,  421,  393,  393,  393,  419,
 /*  2570 */   421,  393,  393,  393,  393,  393,  393,  393,  393,  393,
 /*  2580 */   393,  393,  420,  393,  393,  393,  393,  393,  393,  419,
 /*  2590 */   421,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    20,   21,   22,   23,   24,   25,   74,   75,   28,   60,
 /*    10 */   101,  102,  103,  104,   27,  106,  107,   30,   38,  110,
 /*    20 */    40,   34,   35,   43,   44,   45,   46,   47,   48,   49,
 /*    30 */    50,   51,   52,   53,   28,    7,    8,    9,   10,   11,
 /*    40 */    12,   13,   14,   15,   16,   17,   18,   19,  134,    1,
 /*    50 */     2,    3,    4,    5,  104,   75,  142,  143,   28,   31,
 /*    60 */   110,  105,   32,   83,   84,   35,   86,   87,   88,   89,
 /*    70 */    90,    6,   29,   43,   44,   45,   46,   47,   48,   49,
 /*    80 */    50,   51,   34,   53,   54,   55,   56,   39,   58,   41,
 /*    90 */    42,   60,   62,   63,   64,   65,    6,  100,   68,  102,
 /*   100 */   103,  104,   28,  106,  107,  134,   32,  110,  152,   35,
 /*   110 */    80,   81,   82,  142,  143,   85,   26,   43,   44,   45,
 /*   120 */    46,   47,   48,   49,   50,   51,  134,   53,   54,   55,
 /*   130 */    56,   66,   58,   29,  142,  143,   62,   63,   64,   65,
 /*   140 */    36,  149,   68,  102,  103,  104,   28,  110,  107,  134,
 /*   150 */    32,  110,   60,   35,   80,   81,   82,  142,  143,   85,
 /*   160 */    28,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*   170 */   134,   53,   54,   55,   56,  139,   58,   28,  142,  143,
 /*   180 */    62,   63,   64,   65,   28,   74,   68,  102,   77,  104,
 /*   190 */    28,  106,  107,   28,   32,  110,  105,   35,   80,   81,
 /*   200 */    82,    0,   31,   85,   33,   43,   44,   45,   46,   47,
 /*   210 */    48,   49,   50,   51,  134,   53,   54,   55,   56,  139,
 /*   220 */    58,   29,  142,  143,   62,   63,   64,   65,   27,   29,
 /*   230 */    68,   30,   83,   84,   28,   34,   35,  134,   32,   83,
 /*   240 */    84,   35,   80,   81,   82,  142,  143,   85,   76,   43,
 /*   250 */    44,   45,   46,   47,   48,   49,   50,   51,  134,   53,
 /*   260 */    54,   55,   56,  139,   58,   29,  142,  143,   62,   63,
 /*   270 */    64,   65,  108,   28,   68,  111,  112,  113,   28,   28,
 /*   280 */   112,  134,   32,   38,    6,   35,   80,   81,   82,  142,
 /*   290 */   143,   85,   32,   43,   44,   45,   46,   47,   48,   49,
 /*   300 */    50,   51,  134,   53,   54,   55,   56,   32,   58,   76,
 /*   310 */   142,  143,   62,   63,   64,   65,  134,  104,   68,   59,
 /*   320 */   138,   61,   28,  110,  142,  143,   32,  105,    6,   35,
 /*   330 */    80,   81,   82,   29,   59,   85,   61,   43,   44,   45,
 /*   340 */    46,   47,   48,   49,   50,   51,  134,   53,   54,   55,
 /*   350 */    56,   29,   58,  141,  142,  143,   62,   63,   64,   65,
 /*   360 */   131,   24,   68,  134,  108,   28,   28,  111,  112,  113,
 /*   370 */    32,  142,  143,   35,   80,   81,   82,  145,  146,   85,
 /*   380 */    28,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*   390 */    66,   53,   54,   55,   56,  133,   58,   17,   18,   19,
 /*   400 */    62,   63,   64,   65,  131,  104,   68,  134,  107,   28,
 /*   410 */    28,  110,   29,  134,   32,  142,  143,   35,   80,   81,
 /*   420 */    82,  142,  143,   85,  137,   43,   44,   45,   46,   47,
 /*   430 */    48,   49,   50,   51,  134,   53,   54,   55,   56,   31,
 /*   440 */    58,   33,  142,  143,   62,   63,   64,   65,  134,   28,
 /*   450 */    68,  134,  138,   28,   28,  138,  142,  143,   32,  142,
 /*   460 */   143,   35,   80,   81,   82,  145,  146,   85,   28,   43,
 /*   470 */    44,   45,   46,   47,   48,   49,   50,   51,  134,   53,
 /*   480 */    54,   55,   56,   76,   58,   28,  142,  143,   62,   63,
 /*   490 */    64,   65,  131,   28,   68,  134,  145,  146,   28,  145,
 /*   500 */   146,  134,   32,  142,  143,   35,   80,   81,   82,  142,
 /*   510 */   143,   85,   31,   43,   44,   45,   46,   47,   48,   49,
 /*   520 */    50,   51,  134,   53,   54,   55,   56,   28,   58,   40,
 /*   530 */   142,  143,   62,   63,   64,   65,  131,   29,   68,  134,
 /*   540 */    83,   84,   28,  145,  146,  134,   32,  142,  143,   35,
 /*   550 */    80,   81,   82,  142,  143,   85,   32,   43,   44,   45,
 /*   560 */    46,   47,   48,   49,   50,   51,  134,   53,   54,   55,
 /*   570 */    56,  147,   58,   76,  142,  143,   62,   63,   64,   65,
 /*   580 */   134,  137,   68,  134,  138,   30,   28,  138,  142,  143,
 /*   590 */    32,  142,  143,   35,   80,   81,   82,    6,   63,   85,
 /*   600 */    28,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*   610 */   134,   53,   54,   55,   56,   76,   58,   26,  142,  143,
 /*   620 */    62,   63,   64,   65,  134,   31,   68,  134,  138,   29,
 /*   630 */    28,  138,  142,  143,   32,  142,  143,   35,   80,   81,
 /*   640 */    82,    6,   29,   85,   32,   43,   44,   45,   46,   47,
 /*   650 */    48,   49,   50,   51,  134,   53,   54,   55,   56,   76,
 /*   660 */    58,   26,  142,  143,   62,   63,   64,   65,  134,   76,
 /*   670 */    68,  134,  138,   31,   28,  138,  142,  143,   32,  142,
 /*   680 */   143,   35,   80,   81,   82,  145,  146,   85,   29,   43,
 /*   690 */    44,   45,   46,   47,   48,   49,   50,   51,  134,   53,
 /*   700 */    54,   55,   56,    6,   58,   76,  142,  143,   62,   63,
 /*   710 */    64,   65,  131,   29,   68,  134,  145,  146,   28,    6,
 /*   720 */    36,  134,   32,  142,  143,   35,   80,   81,   82,  142,
 /*   730 */   143,   85,   75,   43,   44,   45,   46,   47,   48,   49,
 /*   740 */    50,   51,   29,   53,   54,   55,   56,   60,   58,  111,
 /*   750 */   112,  113,   62,   63,   64,   65,  134,  137,   68,  134,
 /*   760 */   145,  146,   28,   31,  142,  143,   32,  142,  143,   35,
 /*   770 */    80,   81,   82,   76,   76,   85,   76,   43,   44,   45,
 /*   780 */    46,   47,   48,   49,   50,   51,  134,   53,   54,   55,
 /*   790 */    56,   26,   58,   29,  142,  143,   62,   63,   64,   65,
 /*   800 */   134,   31,   68,  134,  132,  133,   28,   57,  142,  143,
 /*   810 */    32,  142,  143,   35,   80,   81,   82,  135,  136,   85,
 /*   820 */    60,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*   830 */   134,   53,   54,   55,   56,    6,   58,   31,  142,  143,
 /*   840 */    62,   63,   64,   65,  145,  146,   68,  145,  146,   29,
 /*   850 */    28,   28,   63,   28,   32,   26,  137,   35,   80,   81,
 /*   860 */    82,    6,   31,   85,   40,   43,   44,   45,   46,   47,
 /*   870 */    48,   49,   50,   51,    6,   53,   54,   55,   56,    6,
 /*   880 */    58,   26,    6,    6,   62,   63,   64,   65,    6,   28,
 /*   890 */    68,   29,    0,   28,   28,   31,  137,   29,   32,   26,
 /*   900 */   146,   35,   80,   81,   82,   29,   29,   85,   26,   43,
 /*   910 */    44,   45,   46,   47,   48,   49,   50,   51,    6,   53,
 /*   920 */    54,   55,   56,   32,   58,   36,    6,    6,   62,   63,
 /*   930 */    64,   65,  105,   28,   68,   36,   36,  105,   28,   76,
 /*   940 */   105,   29,   32,   57,   28,   35,   80,   81,   82,   29,
 /*   950 */    29,   85,  137,   43,   44,   45,   46,   47,   48,   49,
 /*   960 */    50,   51,    6,   53,   54,   55,   56,   36,   58,   31,
 /*   970 */     6,    6,   62,   63,   64,   65,  105,   28,   68,   12,
 /*   980 */    28,   31,   28,   60,  105,   29,   32,   28,  137,   35,
 /*   990 */    80,   81,   82,   29,   29,   85,  147,   43,   44,   45,
 /*  1000 */    46,   47,   48,   49,   50,   51,    6,   53,   54,   55,
 /*  1010 */    56,   28,   58,   76,   28,   31,   62,   63,   64,   65,
 /*  1020 */    40,   40,   68,   29,  137,   40,   28,   31,   31,   29,
 /*  1030 */    32,   28,  136,   35,   80,   81,   82,   38,  137,   85,
 /*  1040 */    36,   43,   44,   45,   46,   47,   48,   49,   50,   51,
 /*  1050 */    40,   53,   54,   55,   56,   28,   58,   32,  105,   29,
 /*  1060 */    62,   63,   64,   65,   38,   28,   68,   36,  105,   29,
 /*  1070 */    28,  153,   29,  153,   32,  153,  153,   35,   80,   81,
 /*  1080 */    82,  153,   36,   85,  153,   43,   44,   45,   46,   47,
 /*  1090 */    48,   49,   50,   51,  153,   53,   54,   55,   56,  153,
 /*  1100 */    58,  153,  153,  153,   62,   63,   64,   65,  153,  153,
 /*  1110 */    68,  153,  153,  153,   28,   69,   70,   71,   72,   73,
 /*  1120 */   153,   35,   80,   81,   82,  153,  153,   85,  153,   43,
 /*  1130 */    44,   45,   46,   47,   48,   49,   50,   51,  153,   53,
 /*  1140 */    54,   55,   56,  153,   58,  153,  153,  134,   62,   63,
 /*  1150 */    64,   65,  105,  153,   68,  142,  143,  144,  153,   98,
 /*  1160 */   153,  153,  149,  153,  153,  153,   80,   81,   82,  153,
 /*  1170 */   109,   85,  153,  153,  153,  114,  115,  116,  117,  118,
 /*  1180 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1190 */   129,  130,  153,  153,   98,  148,  105,  150,  151,  152,
 /*  1200 */   153,  153,  153,  142,  143,  109,   95,   96,   97,   98,
 /*  1210 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1220 */   124,  125,  126,  127,  128,  129,  130,  153,  153,   98,
 /*  1230 */    92,   93,   94,   95,   96,   97,   98,  153,  142,  143,
 /*  1240 */   109,  150,  151,  152,  153,  114,  115,  116,  117,  118,
 /*  1250 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1260 */   129,  130,  153,  153,  153,   98,  153,    1,    2,    3,
 /*  1270 */     4,    5,  153,  142,  143,  153,  109,   40,  153,  153,
 /*  1280 */    28,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1290 */   123,  124,  125,  126,  127,  128,  129,  130,   32,  153,
 /*  1300 */    34,   35,   98,   37,  153,  153,  153,   41,   42,  142,
 /*  1310 */   143,   74,   75,  109,   77,   28,  153,  153,  114,  115,
 /*  1320 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1330 */   126,  127,  128,  129,  130,   83,   84,   98,  153,   87,
 /*  1340 */    88,   89,   90,  153,  153,  153,  142,  143,  109,  153,
 /*  1350 */   153,  153,  153,  114,  115,  116,  117,  118,  119,  120,
 /*  1360 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1370 */    83,   84,   98,  153,   87,   88,   89,   90,  153,  153,
 /*  1380 */   153,  142,  143,  109,  153,  153,  153,  153,  114,  115,
 /*  1390 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1400 */   126,  127,  128,  129,  130,  153,  153,  153,   98,  153,
 /*  1410 */     1,    2,    3,    4,    5,  153,  142,  143,  153,  109,
 /*  1420 */   153,  153,  153,  153,  114,  115,  116,  117,  118,  119,
 /*  1430 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1440 */   130,   32,  153,   34,   35,   98,   37,  153,   36,  153,
 /*  1450 */    41,   42,  142,  143,  153,  153,  109,  153,  153,  153,
 /*  1460 */   153,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1470 */   123,  124,  125,  126,  127,  128,  129,  130,  153,  153,
 /*  1480 */    98,   69,   70,   71,   72,   73,  153,   75,  153,  142,
 /*  1490 */   143,  109,  153,  153,  153,  153,  114,  115,  116,  117,
 /*  1500 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*  1510 */   128,  129,  130,   83,   84,   98,  153,   87,   88,   89,
 /*  1520 */    90,  153,  153,  153,  142,  143,  109,  153,  153,  153,
 /*  1530 */   153,  114,  115,  116,  117,  118,  119,  120,  121,  122,
 /*  1540 */   123,  124,  125,  126,  127,  128,  129,  130,  153,  153,
 /*  1550 */   153,   98,  153,    1,    2,    3,    4,    5,  153,  142,
 /*  1560 */   143,  153,  109,  153,  153,  153,  153,  114,  115,  116,
 /*  1570 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1580 */   127,  128,  129,  130,   32,  153,   34,   35,   98,   37,
 /*  1590 */   153,   36,  153,   41,   42,  142,  143,  153,  153,  109,
 /*  1600 */   153,  153,  153,  153,  114,  115,  116,  117,  118,  119,
 /*  1610 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1620 */   130,  153,  153,   98,   69,   70,   71,   72,   73,  153,
 /*  1630 */    75,  153,  142,  143,  109,  153,  153,  153,  153,  114,
 /*  1640 */   115,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1650 */   125,  126,  127,  128,  129,  130,  153,  153,   98,  153,
 /*  1660 */   153,  153,  153,  153,  153,  153,  153,  142,  143,  109,
 /*  1670 */   153,  153,  153,  153,  114,  115,  116,  117,  118,  119,
 /*  1680 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1690 */   130,  153,  153,  153,   98,  153,    1,    2,    3,    4,
 /*  1700 */     5,  153,  142,  143,  153,  109,  153,  153,  153,  153,
 /*  1710 */   114,  115,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1720 */   124,  125,  126,  127,  128,  129,  130,   32,  153,   34,
 /*  1730 */    35,   98,   37,  153,  134,  153,   41,   42,  142,  143,
 /*  1740 */   140,  141,  142,  143,  153,  153,  153,  114,  115,  116,
 /*  1750 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1760 */   127,  128,  129,  130,    9,   10,   11,   12,   13,   14,
 /*  1770 */    15,   16,   17,   18,   19,  142,  143,   20,   21,   22,
 /*  1780 */    23,   24,   25,   26,  153,   28,  153,  153,  153,  153,
 /*  1790 */   153,  153,  153,  153,  153,   38,  153,   40,   20,   21,
 /*  1800 */    22,   23,   24,   25,  153,  153,   28,  153,  153,  153,
 /*  1810 */   153,  153,  153,  153,  153,  153,   38,  153,   40,   20,
 /*  1820 */    21,   22,   23,   24,   25,  153,  153,   28,  153,  153,
 /*  1830 */   153,  153,   75,  153,  153,  153,  153,   38,  153,   40,
 /*  1840 */    83,   84,  153,   86,   87,   88,   89,   90,  134,  153,
 /*  1850 */   153,  153,  153,   75,   76,  153,  142,  143,  144,  153,
 /*  1860 */   153,   83,   84,  149,   86,   87,   88,   89,   90,  153,
 /*  1870 */   153,  153,  153,  153,   75,   76,  153,    1,    2,    3,
 /*  1880 */     4,    5,   83,   84,  153,   86,   87,   88,   89,   90,
 /*  1890 */   153,  153,  153,  153,  153,   20,   21,   22,   23,   24,
 /*  1900 */    25,  153,  153,   28,  153,  153,  153,  153,   32,  153,
 /*  1910 */    34,   35,  153,   38,  153,   40,  153,   41,   42,   20,
 /*  1920 */    21,   22,   23,   24,   25,  153,  153,   28,  153,  153,
 /*  1930 */   153,  153,  153,  153,   36,  153,  153,   38,  153,   40,
 /*  1940 */   153,  153,   67,  153,   99,  100,  101,  102,  103,  104,
 /*  1950 */    75,  106,  107,  153,  153,  110,  153,  153,   83,   84,
 /*  1960 */   153,   86,   87,   88,   89,   90,   67,   69,   70,   71,
 /*  1970 */    72,   73,   74,   75,   75,   77,   78,   79,  153,  153,
 /*  1980 */   153,  153,   83,   84,  153,   86,   87,   88,   89,   90,
 /*  1990 */     1,    2,    3,    4,    5,   20,   21,   22,   23,   24,
 /*  2000 */    25,   26,  153,   28,  153,  153,  153,  153,  153,  153,
 /*  2010 */   153,  153,  153,   38,  153,   40,   20,   21,   22,   23,
 /*  2020 */    24,   25,   26,   34,   28,  153,   37,  153,  153,  153,
 /*  2030 */    41,   42,  153,  153,   38,  153,   40,   20,   21,   22,
 /*  2040 */    23,   24,   25,   26,  153,   28,  153,  153,  153,  153,
 /*  2050 */    75,  153,  153,  153,  153,   38,  153,   40,   83,   84,
 /*  2060 */   153,   86,   87,   88,   89,   90,  153,  153,  153,  153,
 /*  2070 */   153,   75,  153,  153,  153,  153,  153,  153,  153,   83,
 /*  2080 */    84,  134,   86,   87,   88,   89,   90,  153,  153,  142,
 /*  2090 */   143,  144,   75,  153,  153,  153,  149,  153,  153,  153,
 /*  2100 */    83,   84,  153,   86,   87,   88,   89,   90,    1,    2,
 /*  2110 */     3,    4,    5,   20,   21,   22,   23,   24,   25,  153,
 /*  2120 */   153,   28,  153,    1,    2,    3,    4,    5,  153,  153,
 /*  2130 */   153,   38,  153,   40,   20,   21,   22,   23,   24,   25,
 /*  2140 */   153,   34,   28,  153,  153,  153,   39,  153,   41,   42,
 /*  2150 */    28,  153,   38,  153,   40,  153,   34,  134,  153,  153,
 /*  2160 */   153,   39,  153,   41,   42,  142,  143,  144,   75,   76,
 /*  2170 */   153,  153,  149,  153,  153,  153,   83,   84,  134,   86,
 /*  2180 */    87,   88,   89,   90,  153,  153,  142,  143,  144,   75,
 /*  2190 */   153,  153,  153,  149,  153,  153,  153,   83,   84,  153,
 /*  2200 */    86,   87,   88,   89,   90,    7,    8,    9,   10,   11,
 /*  2210 */    12,   13,   14,   15,   16,   17,   18,   19,   11,    7,
 /*  2220 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2230 */    18,   19,  153,   26,  153,   28,  153,  153,  153,   28,
 /*  2240 */   153,  153,  153,   31,  153,    1,    2,    3,    4,    5,
 /*  2250 */    43,   44,   45,   46,   47,   48,   49,   50,   51,   52,
 /*  2260 */    53,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2270 */    16,   17,   18,   19,   76,   11,   32,  153,   34,   35,
 /*  2280 */    26,   37,  153,  153,  153,   41,   42,   76,  153,  153,
 /*  2290 */    26,  153,   28,  153,   83,   84,  153,  153,   87,   88,
 /*  2300 */    89,   90,  153,  153,  153,  153,  153,   43,   44,   45,
 /*  2310 */    46,   47,   48,   49,   50,   51,   52,   53,    7,    8,
 /*  2320 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2330 */    19,   99,  100,  101,  102,  103,  104,  153,  106,  107,
 /*  2340 */    29,  153,  110,  153,  153,  153,  153,  153,    7,    8,
 /*  2350 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2360 */    19,   11,   99,  100,  101,  102,  103,  104,  153,  106,
 /*  2370 */   107,  153,   31,  110,  153,  153,  153,  153,   28,  153,
 /*  2380 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2390 */    17,   18,   19,   43,   44,   45,   46,   47,   48,   49,
 /*  2400 */    50,   51,   52,   53,   31,    1,    2,    3,    4,    5,
 /*  2410 */   153,  153,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2420 */    15,   16,   17,   18,   19,    1,    2,    3,    4,    5,
 /*  2430 */   153,  153,  153,  153,  153,  153,   32,  153,   34,   35,
 /*  2440 */   153,   37,  153,  153,  153,   41,   42,    1,    2,    3,
 /*  2450 */     4,    5,  153,  153,  153,  153,   32,  153,   34,   35,
 /*  2460 */   153,   37,  153,  153,  153,   41,   42,    1,    2,    3,
 /*  2470 */     4,    5,    1,    2,    3,    4,    5,  153,   32,  153,
 /*  2480 */    34,   35,    1,    2,    3,    4,    5,   41,   42,  153,
 /*  2490 */   153,  153,  153,  153,  153,  153,  153,  153,   32,  153,
 /*  2500 */    34,   35,  153,   32,  153,   34,   35,   41,   42,   28,
 /*  2510 */   153,  153,   41,   42,  153,   34,  153,  153,  153,  153,
 /*  2520 */    39,  153,   41,   42,    1,    2,    3,    4,    5,    1,
 /*  2530 */     2,    3,    4,    5,  153,  153,   99,  100,  101,  102,
 /*  2540 */   103,  104,  153,  106,  107,  153,  153,  110,  153,    1,
 /*  2550 */     2,    3,    4,    5,  153,   32,  153,   34,   35,  153,
 /*  2560 */    32,  153,   34,   35,   41,   42,  153,  153,  153,   41,
 /*  2570 */    42,  153,  153,  153,  153,  153,  153,  153,  153,  153,
 /*  2580 */   153,  153,   34,  153,  153,  153,  153,  153,  153,   41,
 /*  2590 */    42,
};
#define YY_SHIFT_USE_DFLT (-69)
static short yy_shift_ofst[] = {
 /*     0 */   -13,  892,  201,  -69,  -69,  -69,  -69,  421,  383,  -69,
 /*    10 */   165,  171, 1695, 1025,  -69,  -69, 1409, 2523,  -69,   48,
 /*    20 */  1003,  985, 2207,  984,  206,  -69,   74,  -69,  -69,  -69,
 /*    30 */   -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,
 /*    40 */   -69,  -69,  -69,  -69,  -69,  -69,   43,  -69,  994,  -69,
 /*    50 */  2114,  950,  162,   30,  886,  864, 1042,  998,  -69, 2114,
 /*    60 */   770,  275,  -69,  260,  -69,  -69, 1430,  923, 1086, 1086,
 /*    70 */   938,  602,  558,  -69, 2114,  732,  470,  426,  -69,  642,
 /*    80 */   118,  866,  789, 2114, 1043,  -69, 2114,  -69, 2114,  132,
 /*    90 */   -69,  657,  156,  233,  -69,  700,  -69,  583,  -69,  380,
 /*   100 */  2114,  380, 2114,  380, 2114,  380, 2114,  380, 2114,  380,
 /*   110 */  2114,  380, 2114,  380, 2114,  -69, 2114,  -69, 2114,  -69,
 /*   120 */  2114, 1755, 2114, 1755,  -20,  -69,  -69,  -69,  -69,  -69,
 /*   130 */   -69,  -69,  -69,  -69,  -69,  -69, 2254,  -69,  765, 2114,
 /*   140 */   -69, 1237,  959,  981, 1757, 2405, 2114,  -69,    6,  278,
 /*   150 */   132,  -69, 2114,  -69,  -69,  -69,  -69,  -69,  -69,  -69,
 /*   160 */   -69,  -69, 2211,  -69,  697,  -69, 1252,  -69,  760, 1287,
 /*   170 */   -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,
 /*   180 */   -51,   31,   92,  -69,  381,  489, 2017,  -69,  591,  -69,
 /*   190 */  2114,  -69,  -69,  -69,  -69,  873,  -69,  -69,  245, 1010,
 /*   200 */  1996,  882,  -69,  -69,  -69,  149,  172,  -69,  407,  -69,
 /*   210 */   497,  -69, 1975,  635,  -69,  -69, 2405,  861,   65, 1875,
 /*   220 */    28, 1086,  690,  -69,  983,  877,  -69,  983,  -69, 1898,
 /*   230 */   -69,  -69,  -69,  -69,  -69,  -69, 2114,  -69, 2405,  352,
 /*   240 */  1555, 2114,  -69, 1799, 1046, 2114,  -69,  593, 1046, 2114,
 /*   250 */   -69, 2198,  823, 1412, 2114,  -69, 1778, 1046, 2114,  -69,
 /*   260 */   863, 1046, 2114,  -69, 2093, 1046, 2114,  -69,  937, 1046,
 /*   270 */  2114,  -69,  -69,  -69,  -69, 2114,  956,  -69, 2114,  -69,
 /*   280 */  2405,  -69,  200,  -69,  236,  -69, 2114, 2311,  -69,  440,
 /*   290 */   -68,  499,  508,  -69,  457,  539,  600,  -69,  629,  659,
 /*   300 */   -69,  698,  764,  -69,  337,  820,  -69,  825,  824, 2114,
 /*   310 */    90,  862,  -69,  865,  322,  -69,  865,  -69,  899, 1430,
 /*   320 */   -69,  -69,  865,  868,  -69,  865,  964,  -69,  865,  921,
 /*   330 */   -69,  865,  876,  -69,  865,  713,  -69,  865,  912,  -69,
 /*   340 */   865,  965,  -69,  865, 1000,  -69,  865,  920,  -69,  111,
 /*   350 */  1027, 1010, 1037,  981,  -69,  -69, 2114, 2341, 1086,  734,
 /*   360 */   -69,  251,  324, 1899, 2212, 1086,  646,  -69, 2114, 2373,
 /*   370 */  1086,  514,  -69,  535, 2114,  613,  -69,  -69,  -69,  687,
 /*   380 */  1086, 1086,  -69,  -69,  750,  806,  382,  338,  -69,  -69,
 /*   390 */   855,  831,  294,  -69,  250,  -69, 2350,  -69,  889, 1430,
 /*   400 */   -69,  905,  900, 1430,  -69,  916,  931, 1430,  -69,  949,
 /*   410 */   967,  952,  -69,  -69,  -69,  -69,  -69,  -69,  -69,  -69,
 /*   420 */   -69,  -69, 2548, 2107,  986,  980, 2264,  996,  954,  -69,
 /*   430 */   910,  -69,  829,  997,  822,  -69,  778,  -69,  -69, 1552,
 /*   440 */  2446,  -69,  -69, 1989,  999, 1004, 1430, 1030,  -69, 1026,
 /*   450 */  1031, 1430, 1040,  -69,  -69, 1989, 2122,  104,  -69, 1430,
 /*   460 */   192,  -69, 2481,  684,  -69, 1430,  304,  -69,  -69, 2244,
 /*   470 */  1876, 2471, 2548, 2528,  -69, 2466,  465,  481, 2404,  524,
 /*   480 */   -69,  -69,  555,  572,  408, 1266,  612,  -69,  -69,  425,
 /*   490 */   594, 2424,  891,  -69,  -69,  -69,
};
#define YY_REDUCE_USE_DFLT (-92)
static short yy_reduce_ofst[] = {
 /*     0 */  1138,  -92, 1111,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*    10 */   -92,  -92, 1845,  -92,  -92,  -92,  -91,  301,  -92,   37,
 /*    20 */   -92,  -92,  164,  -92, 1560,  -92, 1633,  -92,  -92,  -92,
 /*    30 */   -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*    40 */   -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*    50 */   229,  -92, 1347, 1633,  -92,  -92, 1490, 1633,  -92,  273,
 /*    60 */   -92,  672,  -92,  262,  -92,  -92,  879,  -92, 1382, 1633,
 /*    70 */   -92, 1274, 1633,  -92,  361,  -92, 1096, 1633,  -92,  -92,
 /*    80 */  1239, 1633,  -92,  405,  -92,  -92,  476,  -92,  652,  849,
 /*    90 */   -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*   100 */   666,  -92,  669,  -92,  696,  -92,  587,  -92,  300,  -92,
 /*   110 */    15,  -92,  432,  -92,  344,  -92,  279,  -92,  564,  -92,
 /*   120 */   625,  -92,  103,  -92,  168,  -92,  -92,  -92,  -92,  -92,
 /*   130 */   -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  147,
 /*   140 */   -92,  -92,  -92,  -92, 2023,  -92,  622,  -92,  -92,  -92,
 /*   150 */   424,  -92,  -86,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*   160 */   -92,  -92, 1047,  -92,  -92,  -92, 1091,  -92,  -92,  -44,
 /*   170 */   -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*   180 */   -92,  -92,  -92,  -92,  -92,  -92, 2044,  -92,  -92,  -92,
 /*   190 */    -8,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*   200 */  1714,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*   210 */   -92,  -92, 1013,  -92,  -92,  -92,  -92,  -92,  -92,  367,
 /*   220 */   -92, 1417, 1633,  -92,  682,  -92,  -92,  896,  -92,  901,
 /*   230 */   -92,  -92,  -92,  -92,  -92,  -92,  182,  -92,  -92,  -92,
 /*   240 */   287,  314,  -92,   36,  444,  317,  -92,  -92,  620,  446,
 /*   250 */   -92,  -92,  -92,  719,  449,  -92,   80,  759,  490,  -92,
 /*   260 */   -92,  815,  493,  -92,  124,  851,  534,  -92,  -92,  887,
 /*   270 */   537,  -92,  -92,  -92,  -92, 1600,  -92,  -92,  212,  -92,
 /*   280 */   -92,  -92,  -92,  -92,  -92,  -92,  411,  -92,  -92,  -92,
 /*   290 */   -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*   300 */   -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92, 1947,
 /*   310 */   -92,  -92,  -92,  699,  -92,  -92,  754,  -92,  -92,  835,
 /*   320 */   -92,  -92,  320,  -92,  -92,  571,  -92,  -92,  351,  -92,
 /*   330 */   -92,  232,  -92,  -92,  398,  -92,  -92,  540,  -92,  -92,
 /*   340 */   615,  -92,  -92,  702,  -92,  -92,  354,  -92,  -92,  -92,
 /*   350 */   -92,  -92,  -92,  -92,  -92,  -92,  -29,  -92, 1204, 1633,
 /*   360 */   -92,  -92,  -92,  388,  -92, 1310, 1633,  -92,  520,  -92,
 /*   370 */  1525, 1633,  -92,  -92,  581,  -92,  -92,  -92,  -92,  -92,
 /*   380 */  1131, 1633,  -92,  -92,  -92,  -92, 1596, 1633,  -92,  -92,
 /*   390 */   -92,  -92, 1453,  -92, 1633,  -92,  638,  -92,  -92,  827,
 /*   400 */   -92,  -92,  -92,  832,  -92,  -92,  -92,  871,  -92,  -92,
 /*   410 */   -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,  -92,
 /*   420 */   -92,  -92,  213,   37,  -92,  -92,  256,  -92, 1061,  -92,
 /*   430 */  1633,  -92,  -92,  -92, 1167,  -92, 1633,  -92,  -92,   85,
 /*   440 */   301,  -92,  -92,  213,  -92,  -92,  953,  -92,  -92,  -92,
 /*   450 */   -92,  963,  -92,  -92,  -92,  -50,   37,  -92,  -92,   91,
 /*   460 */   -92,  -92,   37,  -92,  -92,  222,  -92,  -92,  -92,   -3,
 /*   470 */    41,  301,  -50,  301,  -92,  301,  -92,  -92, 2263,  -92,
 /*   480 */   -92,  -92,  -92,  -92,  -92, 2232,  -92,  -92,  -92,  -92,
 /*   490 */   -92, 2437,  -92,  -92,  -92,  -92,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   734,  734,  734,  498,  500,  501,  502,  734,  734,  503,
 /*    10 */   734,  734,  734,  734,  504,  508,  734,  734,  529,  734,
 /*    20 */   734,  734,  734,  734,  734,  533,  734,  535,  571,  573,
 /*    30 */   574,  575,  576,  577,  578,  579,  580,  581,  582,  583,
 /*    40 */   584,  585,  586,  587,  588,  589,  734,  590,  734,  591,
 /*    50 */   734,  734,  734,  734,  594,  734,  734,  734,  595,  734,
 /*    60 */   734,  734,  598,  734,  599,  600,  734,  734,  734,  602,
 /*    70 */   734,  734,  734,  605,  734,  734,  734,  734,  607,  734,
 /*    80 */   734,  734,  734,  734,  734,  609,  734,  663,  734,  734,
 /*    90 */   664,  734,  734,  734,  729,  734,  730,  734,  731,  665,
 /*   100 */   734,  666,  734,  667,  734,  668,  734,  669,  734,  670,
 /*   110 */   734,  671,  734,  672,  734,  681,  734,  682,  734,  683,
 /*   120 */   734,  684,  734,  685,  734,  560,  561,  562,  563,  564,
 /*   130 */   565,  566,  567,  568,  569,  570,  734,  673,  734,  734,
 /*   140 */   674,  690,  734,  675,  734,  713,  734,  686,  734,  734,
 /*   150 */   734,  687,  734,  688,  689,  691,  692,  693,  694,  695,
 /*   160 */   696,  697,  734,  698,  734,  699,  734,  714,  734,  734,
 /*   170 */   716,  721,  722,  723,  724,  725,  726,  727,  728,  717,
 /*   180 */   722,  724,  723,  715,  734,  700,  734,  701,  734,  702,
 /*   190 */   734,  705,  706,  711,  712,  734,  709,  710,  734,  676,
 /*   200 */   734,  734,  707,  708,  677,  734,  734,  678,  734,  679,
 /*   210 */   734,  680,  734,  734,  703,  704,  732,  734,  734,  734,
 /*   220 */   734,  734,  734,  610,  734,  734,  614,  734,  615,  734,
 /*   230 */   617,  618,  619,  620,  621,  622,  734,  623,  662,  734,
 /*   240 */   734,  734,  624,  734,  734,  734,  625,  734,  734,  734,
 /*   250 */   626,  734,  734,  734,  734,  627,  734,  734,  734,  628,
 /*   260 */   734,  734,  734,  629,  734,  734,  734,  630,  734,  734,
 /*   270 */   734,  631,  632,  633,  616,  734,  734,  635,  734,  636,
 /*   280 */   638,  637,  734,  639,  734,  640,  734,  734,  641,  734,
 /*   290 */   734,  734,  734,  642,  734,  734,  734,  643,  734,  734,
 /*   300 */   644,  734,  734,  645,  734,  734,  646,  734,  734,  734,
 /*   310 */   734,  734,  647,  734,  734,  648,  734,  658,  660,  734,
 /*   320 */   661,  659,  734,  734,  649,  734,  734,  650,  734,  734,
 /*   330 */   651,  734,  734,  652,  734,  734,  653,  734,  734,  654,
 /*   340 */   734,  734,  655,  734,  734,  656,  734,  734,  657,  734,
 /*   350 */   734,  734,  734,  734,  733,  572,  734,  734,  734,  734,
 /*   360 */   611,  734,  734,  734,  734,  734,  734,  612,  734,  734,
 /*   370 */   734,  734,  613,  734,  734,  734,  608,  606,  604,  734,
 /*   380 */   734,  603,  601,  596,  592,  734,  734,  734,  597,  593,
 /*   390 */   734,  734,  734,  534,  734,  536,  734,  551,  553,  734,
 /*   400 */   556,  734,  554,  734,  557,  734,  555,  734,  558,  734,
 /*   410 */   734,  734,  552,  541,  543,  544,  545,  546,  547,  548,
 /*   420 */   549,  550,  734,  734,  734,  734,  734,  734,  734,  537,
 /*   430 */   734,  539,  734,  734,  734,  538,  734,  540,  542,  734,
 /*   440 */   734,  527,  530,  734,  734,  734,  734,  734,  531,  734,
 /*   450 */   734,  734,  734,  532,  521,  734,  734,  734,  523,  734,
 /*   460 */   734,  525,  734,  734,  524,  734,  734,  526,  528,  734,
 /*   470 */   734,  734,  734,  734,  522,  734,  734,  734,  734,  734,
 /*   480 */   505,  509,  734,  734,  734,  734,  734,  506,  510,  734,
 /*   490 */   734,  734,  734,  507,  511,  499,
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
  "PRIVATE",       "SCOPED",        "COMMA",         "AND",         
  "OR",            "EQUALS",        "IDENTICAL",     "LESS",        
  "GREATER",       "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",
  "NOTEQUALS",     "ADD",           "SUB",           "CONCAT",      
  "ISSET",         "FETCH",         "TYPEOF",        "INSTANCEOF",  
  "NEW",           "NOT",           "PARENTHESES_CLOSE",  "NAMESPACE",   
  "IDENTIFIER",    "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",
  "BRACKET_CLOSE",  "EXTENDS",       "ABSTRACT",      "COMMENT",     
  "ASSIGN",        "CONST",         "CONSTANT",      "FUNCTION",    
  "PARENTHESES_OPEN",  "INLINE",        "FINAL",         "TYPE_INTEGER",
  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",   
  "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "SWITCH",        "CASE",        
  "COLON",         "DEFAULT",       "LOOP",          "WHILE",       
  "DO",            "FOR",           "IN",            "REVERSE",     
  "LET",           "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",   
  "DIVASSIGN",     "CONCATASSIGN",  "ARROW",         "SBRACKET_OPEN",
  "SBRACKET_CLOSE",  "DOUBLECOLON",   "INCR",          "DECR",        
  "ECHO",          "RETURN",        "UNSET",         "INTEGER",     
  "STRING",        "THROW",         "SCHAR",         "DOUBLE",      
  "NULL",          "TRUE",          "FALSE",         "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_comment",    "xx_class_definition",
  "xx_class_properties_definition",  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",
  "xx_visibility_list",  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",
  "xx_parameter_list",  "xx_statement_list",  "xx_visibility",  "xx_parameter",
  "xx_parameter_type",  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",
  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",
  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",  "xx_eval_expr",
  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",
  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",
  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",  "xx_scall_expr",
  "xx_call_parameters",  "xx_declare_variable_list",  "xx_declare_variable",  "xx_isset_expr",
  "xx_array_list",  "xx_call_parameter",  "xx_array_item",  "xx_array_key",
  "xx_array_value",
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
 /*  27 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  28 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  29 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  30 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  31 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  32 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  33 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  34 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  35 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  36 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  37 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  38 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  39 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  40 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  41 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  42 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  43 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  44 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  45 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  46 */ "xx_visibility_list ::= xx_visibility",
 /*  47 */ "xx_visibility ::= PUBLIC",
 /*  48 */ "xx_visibility ::= PROTECTED",
 /*  49 */ "xx_visibility ::= PRIVATE",
 /*  50 */ "xx_visibility ::= STATIC",
 /*  51 */ "xx_visibility ::= SCOPED",
 /*  52 */ "xx_visibility ::= INLINE",
 /*  53 */ "xx_visibility ::= ABSTRACT",
 /*  54 */ "xx_visibility ::= FINAL",
 /*  55 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  56 */ "xx_parameter_list ::= xx_parameter",
 /*  57 */ "xx_parameter ::= IDENTIFIER",
 /*  58 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  59 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  60 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  61 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  62 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  63 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  64 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  65 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /*  66 */ "xx_parameter_type ::= TYPE_LONG",
 /*  67 */ "xx_parameter_type ::= TYPE_ULONG",
 /*  68 */ "xx_parameter_type ::= TYPE_CHAR",
 /*  69 */ "xx_parameter_type ::= TYPE_UCHAR",
 /*  70 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  71 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  72 */ "xx_parameter_type ::= TYPE_STRING",
 /*  73 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  74 */ "xx_parameter_type ::= TYPE_VAR",
 /*  75 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  76 */ "xx_statement_list ::= xx_statement",
 /*  77 */ "xx_statement ::= xx_let_statement",
 /*  78 */ "xx_statement ::= xx_if_statement",
 /*  79 */ "xx_statement ::= xx_loop_statement",
 /*  80 */ "xx_statement ::= xx_echo_statement",
 /*  81 */ "xx_statement ::= xx_return_statement",
 /*  82 */ "xx_statement ::= xx_mcall_statement",
 /*  83 */ "xx_statement ::= xx_scall_statement",
 /*  84 */ "xx_statement ::= xx_unset_statement",
 /*  85 */ "xx_statement ::= xx_throw_statement",
 /*  86 */ "xx_statement ::= xx_declare_statement",
 /*  87 */ "xx_statement ::= xx_break_statement",
 /*  88 */ "xx_statement ::= xx_continue_statement",
 /*  89 */ "xx_statement ::= xx_while_statement",
 /*  90 */ "xx_statement ::= xx_do_while_statement",
 /*  91 */ "xx_statement ::= xx_switch_statement",
 /*  92 */ "xx_statement ::= xx_for_statement",
 /*  93 */ "xx_statement ::= xx_comment",
 /*  94 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  95 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  96 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  97 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  98 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  99 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 100 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 101 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 102 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 103 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 104 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 105 */ "xx_case_clauses ::= xx_case_clause",
 /* 106 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 107 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 108 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 109 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 110 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 111 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 112 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 113 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 114 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 115 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 116 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 117 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 118 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 119 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 120 */ "xx_let_assignments ::= xx_let_assignment",
 /* 121 */ "xx_assignment_operator ::= ASSIGN",
 /* 122 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 123 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 124 */ "xx_assignment_operator ::= MULASSIGN",
 /* 125 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 126 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 127 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 128 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 129 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 130 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 131 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 132 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 133 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 134 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 135 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 136 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 137 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 138 */ "xx_index_expr ::= xx_common_expr",
 /* 139 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 140 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 141 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 142 */ "xx_echo_expression ::= xx_common_expr",
 /* 143 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 144 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 145 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 146 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 147 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 148 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 149 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 150 */ "xx_throw_statement ::= THROW IDENTIFIER DOTCOMMA",
 /* 151 */ "xx_throw_statement ::= THROW NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE DOTCOMMA",
 /* 152 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 153 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 154 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 155 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 156 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 157 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 158 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 159 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 160 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 161 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 162 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 163 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 164 */ "xx_declare_variable ::= IDENTIFIER",
 /* 165 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 166 */ "xx_assign_expr ::= xx_common_expr",
 /* 167 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 168 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 169 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 170 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 171 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 172 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 173 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 174 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 175 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 176 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 177 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 178 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 179 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 180 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 181 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 182 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 183 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 184 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 185 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 186 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 187 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 188 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 189 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 190 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 191 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 192 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 193 */ "xx_common_expr ::= CONSTANT",
 /* 194 */ "xx_common_expr ::= IDENTIFIER",
 /* 195 */ "xx_common_expr ::= INTEGER",
 /* 196 */ "xx_common_expr ::= STRING",
 /* 197 */ "xx_common_expr ::= SCHAR",
 /* 198 */ "xx_common_expr ::= DOUBLE",
 /* 199 */ "xx_common_expr ::= NULL",
 /* 200 */ "xx_common_expr ::= TRUE",
 /* 201 */ "xx_common_expr ::= FALSE",
 /* 202 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 203 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 204 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 205 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 206 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 207 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 208 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 209 */ "xx_common_expr ::= xx_mcall_expr",
 /* 210 */ "xx_common_expr ::= xx_scall_expr",
 /* 211 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 212 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 213 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 214 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 215 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 216 */ "xx_call_parameters ::= xx_call_parameter",
 /* 217 */ "xx_call_parameter ::= xx_common_expr",
 /* 218 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 219 */ "xx_array_list ::= xx_array_item",
 /* 220 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 221 */ "xx_array_item ::= xx_array_value",
 /* 222 */ "xx_array_key ::= IDENTIFIER",
 /* 223 */ "xx_array_key ::= STRING",
 /* 224 */ "xx_array_key ::= INTEGER",
 /* 225 */ "xx_array_value ::= xx_literal_expr",
 /* 226 */ "xx_array_value ::= IDENTIFIER",
 /* 227 */ "xx_literal_expr ::= INTEGER",
 /* 228 */ "xx_literal_expr ::= STRING",
 /* 229 */ "xx_literal_expr ::= DOUBLE",
 /* 230 */ "xx_literal_expr ::= NULL",
 /* 231 */ "xx_literal_expr ::= FALSE",
 /* 232 */ "xx_literal_expr ::= TRUE",
 /* 233 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 234 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 235 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 236 */ "xx_eval_expr ::= xx_common_expr",
 /* 237 */ "xx_comment ::= COMMENT",
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
    case 86:
    case 87:
    case 88:
    case 89:
    case 90:
// 835 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2089 "parser.c"
      break;
    case 93:
// 848 "parser.lemon"
{ json_object_put((yypminor->yy115)); }
// 2094 "parser.c"
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
  { 92, 1 },
  { 93, 1 },
  { 94, 2 },
  { 94, 1 },
  { 95, 1 },
  { 95, 1 },
  { 95, 1 },
  { 96, 3 },
  { 97, 5 },
  { 97, 7 },
  { 97, 6 },
  { 97, 8 },
  { 97, 4 },
  { 97, 6 },
  { 97, 5 },
  { 97, 7 },
  { 99, 1 },
  { 99, 1 },
  { 99, 1 },
  { 99, 2 },
  { 99, 2 },
  { 99, 2 },
  { 99, 2 },
  { 99, 3 },
  { 99, 3 },
  { 100, 2 },
  { 100, 1 },
  { 103, 4 },
  { 103, 3 },
  { 103, 6 },
  { 103, 5 },
  { 101, 2 },
  { 101, 1 },
  { 102, 2 },
  { 102, 1 },
  { 106, 6 },
  { 106, 5 },
  { 107, 7 },
  { 107, 8 },
  { 107, 8 },
  { 107, 9 },
  { 107, 8 },
  { 107, 9 },
  { 107, 9 },
  { 107, 10 },
  { 104, 2 },
  { 104, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 108, 3 },
  { 108, 1 },
  { 111, 1 },
  { 111, 2 },
  { 111, 2 },
  { 111, 3 },
  { 111, 4 },
  { 111, 4 },
  { 113, 3 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 109, 2 },
  { 109, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 125, 2 },
  { 126, 2 },
  { 116, 4 },
  { 116, 7 },
  { 116, 5 },
  { 116, 9 },
  { 116, 8 },
  { 116, 8 },
  { 129, 4 },
  { 129, 5 },
  { 132, 2 },
  { 132, 1 },
  { 133, 4 },
  { 133, 3 },
  { 117, 3 },
  { 117, 4 },
  { 127, 4 },
  { 127, 5 },
  { 128, 6 },
  { 128, 7 },
  { 130, 7 },
  { 130, 8 },
  { 130, 9 },
  { 130, 10 },
  { 115, 3 },
  { 135, 3 },
  { 135, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 136, 3 },
  { 136, 5 },
  { 136, 7 },
  { 136, 8 },
  { 136, 5 },
  { 136, 7 },
  { 136, 8 },
  { 136, 5 },
  { 136, 6 },
  { 136, 2 },
  { 136, 2 },
  { 139, 1 },
  { 118, 3 },
  { 140, 3 },
  { 140, 1 },
  { 141, 1 },
  { 120, 2 },
  { 121, 2 },
  { 119, 3 },
  { 122, 5 },
  { 122, 6 },
  { 122, 6 },
  { 122, 6 },
  { 123, 3 },
  { 123, 7 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 124, 3 },
  { 145, 3 },
  { 145, 1 },
  { 146, 1 },
  { 146, 3 },
  { 138, 1 },
  { 134, 2 },
  { 134, 2 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 4 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 4 },
  { 134, 4 },
  { 134, 4 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 3 },
  { 134, 2 },
  { 134, 4 },
  { 134, 2 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 1 },
  { 134, 2 },
  { 134, 3 },
  { 134, 2 },
  { 134, 4 },
  { 134, 5 },
  { 134, 4 },
  { 134, 3 },
  { 134, 1 },
  { 134, 1 },
  { 143, 6 },
  { 143, 5 },
  { 142, 6 },
  { 142, 5 },
  { 144, 3 },
  { 144, 1 },
  { 149, 1 },
  { 148, 3 },
  { 148, 1 },
  { 150, 3 },
  { 150, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 152, 1 },
  { 152, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 105, 1 },
  { 147, 4 },
  { 147, 4 },
  { 147, 4 },
  { 131, 1 },
  { 98, 1 },
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
// 844 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy115;
}
// 2549 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 77:
      case 78:
      case 79:
      case 80:
      case 81:
      case 82:
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
      case 138:
      case 166:
      case 209:
      case 210:
      case 217:
      case 225:
      case 236:
// 850 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;
}
// 2583 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 33:
      case 45:
      case 75:
      case 104:
// 854 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-1].minor.yy115, yymsp[0].minor.yy115);
}
// 2596 "parser.c"
        break;
      case 3:
      case 26:
      case 32:
      case 34:
      case 46:
      case 56:
      case 76:
      case 105:
      case 120:
      case 141:
      case 163:
      case 216:
      case 219:
// 858 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(NULL, yymsp[0].minor.yy115);
}
// 2615 "parser.c"
        break;
      case 7:
// 874 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(27,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 2624 "parser.c"
        break;
      case 8:
// 878 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, 0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2634 "parser.c"
        break;
      case 9:
// 882 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy115, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2645 "parser.c"
        break;
      case 10:
// 886 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, 1, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2656 "parser.c"
        break;
      case 11:
// 890 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy115, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-7].minor);
  yy_destructor(30,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2668 "parser.c"
        break;
      case 12:
// 894 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2678 "parser.c"
        break;
      case 13:
// 898 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2689 "parser.c"
        break;
      case 14:
// 902 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2700 "parser.c"
        break;
      case 15:
// 906 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(30,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2712 "parser.c"
        break;
      case 16:
// 910 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
}
// 2719 "parser.c"
        break;
      case 17:
// 914 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 2726 "parser.c"
        break;
      case 18:
// 918 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 2733 "parser.c"
        break;
      case 19:
// 922 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
}
// 2740 "parser.c"
        break;
      case 20:
// 926 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 2747 "parser.c"
        break;
      case 21:
// 930 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[0].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
}
// 2754 "parser.c"
        break;
      case 22:
// 934 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(NULL, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 2761 "parser.c"
        break;
      case 23:
// 938 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
}
// 2768 "parser.c"
        break;
      case 24:
// 942 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_definition(yymsp[-1].minor.yy115, yymsp[0].minor.yy115, yymsp[-2].minor.yy115, status->scanner_state);
}
// 2775 "parser.c"
        break;
      case 27:
// 954 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[0].minor);
}
// 2783 "parser.c"
        break;
      case 28:
// 958 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-2].minor.yy115, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[0].minor);
}
// 2791 "parser.c"
        break;
      case 29:
// 962 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 2800 "parser.c"
        break;
      case 30:
// 966 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_property(yymsp[-4].minor.yy115, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 2809 "parser.c"
        break;
      case 35:
// 986 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 2819 "parser.c"
        break;
      case 36:
// 990 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 2829 "parser.c"
        break;
      case 37:
// 994 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2841 "parser.c"
        break;
      case 38:
// 998 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2853 "parser.c"
        break;
      case 39:
// 1002 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2865 "parser.c"
        break;
      case 40:
// 1006 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2877 "parser.c"
        break;
      case 41:
// 1010 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-6].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2889 "parser.c"
        break;
      case 42:
// 1014 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2901 "parser.c"
        break;
      case 43:
// 1018 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-7].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy115, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2913 "parser.c"
        break;
      case 44:
// 1022 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_class_method(yymsp[-8].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy115, yymsp[-1].minor.yy115, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-7].minor);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2925 "parser.c"
        break;
      case 47:
// 1034 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2933 "parser.c"
        break;
      case 48:
// 1038 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2941 "parser.c"
        break;
      case 49:
// 1042 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2949 "parser.c"
        break;
      case 50:
// 1046 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2957 "parser.c"
        break;
      case 51:
// 1050 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 2965 "parser.c"
        break;
      case 52:
// 1054 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("inline");
  yy_destructor(41,&yymsp[0].minor);
}
// 2973 "parser.c"
        break;
      case 53:
// 1058 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("abstract");
  yy_destructor(34,&yymsp[0].minor);
}
// 2981 "parser.c"
        break;
      case 54:
// 1062 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("final");
  yy_destructor(42,&yymsp[0].minor);
}
// 2989 "parser.c"
        break;
      case 55:
      case 119:
      case 140:
      case 162:
      case 215:
      case 218:
// 1066 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_list(yymsp[-2].minor.yy115, yymsp[0].minor.yy115);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3002 "parser.c"
        break;
      case 57:
// 1074 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3009 "parser.c"
        break;
      case 58:
// 1078 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-1].minor.yy115, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3016 "parser.c"
        break;
      case 59:
// 1082 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-1].minor.yy115, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3023 "parser.c"
        break;
      case 60:
// 1086 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 3031 "parser.c"
        break;
      case 61:
// 1090 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(yymsp[-3].minor.yy115, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 3039 "parser.c"
        break;
      case 62:
// 1094 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_parameter(NULL, yymsp[-3].minor.yy115, yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 3047 "parser.c"
        break;
      case 63:
// 1098 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 3056 "parser.c"
        break;
      case 64:
// 1102 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(43,&yymsp[0].minor);
}
// 3064 "parser.c"
        break;
      case 65:
// 1106 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(44,&yymsp[0].minor);
}
// 3072 "parser.c"
        break;
      case 66:
// 1110 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(45,&yymsp[0].minor);
}
// 3080 "parser.c"
        break;
      case 67:
// 1114 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(46,&yymsp[0].minor);
}
// 3088 "parser.c"
        break;
      case 68:
// 1118 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(47,&yymsp[0].minor);
}
// 3096 "parser.c"
        break;
      case 69:
// 1122 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(48,&yymsp[0].minor);
}
// 3104 "parser.c"
        break;
      case 70:
// 1126 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(49,&yymsp[0].minor);
}
// 3112 "parser.c"
        break;
      case 71:
// 1130 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(50,&yymsp[0].minor);
}
// 3120 "parser.c"
        break;
      case 72:
// 1134 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(51,&yymsp[0].minor);
}
// 3128 "parser.c"
        break;
      case 73:
// 1138 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(52,&yymsp[0].minor);
}
// 3136 "parser.c"
        break;
      case 74:
// 1142 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(53,&yymsp[0].minor);
}
// 3144 "parser.c"
        break;
      case 94:
// 1222 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(54,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3153 "parser.c"
        break;
      case 95:
// 1226 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(55,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3162 "parser.c"
        break;
      case 96:
// 1230 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-2].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3172 "parser.c"
        break;
      case 97:
// 1234 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-5].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3185 "parser.c"
        break;
      case 98:
// 1238 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3195 "parser.c"
        break;
      case 99:
// 1242 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-7].minor.yy115, yymsp[-5].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(56,&yymsp[-8].minor);
  yy_destructor(31,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3208 "parser.c"
        break;
      case 100:
// 1246 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, yymsp[-4].minor.yy115, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3221 "parser.c"
        break;
      case 101:
// 1250 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_if_statement(yymsp[-6].minor.yy115, NULL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(57,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3234 "parser.c"
        break;
      case 102:
// 1254 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3244 "parser.c"
        break;
      case 103:
// 1258 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_switch_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3254 "parser.c"
        break;
      case 106:
// 1270 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(60,&yymsp[-1].minor);
}
// 3263 "parser.c"
        break;
      case 107:
// 1274 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_case_clause(NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(60,&yymsp[-1].minor);
}
// 3272 "parser.c"
        break;
      case 108:
// 1278 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3282 "parser.c"
        break;
      case 109:
// 1282 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_loop_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3292 "parser.c"
        break;
      case 110:
// 1286 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-2].minor.yy115, NULL, status->scanner_state);
  yy_destructor(63,&yymsp[-3].minor);
  yy_destructor(31,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3302 "parser.c"
        break;
      case 111:
// 1290 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_while_statement(yymsp[-3].minor.yy115, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(63,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3312 "parser.c"
        break;
      case 112:
// 1294 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-5].minor);
  yy_destructor(31,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3324 "parser.c"
        break;
      case 113:
// 1298 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_do_while_statement(yymsp[-1].minor.yy115, yymsp[-4].minor.yy115, status->scanner_state);
  yy_destructor(64,&yymsp[-6].minor);
  yy_destructor(31,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3336 "parser.c"
        break;
      case 114:
// 1302 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(65,&yymsp[-6].minor);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3347 "parser.c"
        break;
      case 115:
// 1306 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(65,&yymsp[-7].minor);
  yy_destructor(66,&yymsp[-5].minor);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3359 "parser.c"
        break;
      case 116:
// 1310 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(65,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3371 "parser.c"
        break;
      case 117:
// 1314 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_for_statement(yymsp[-3].minor.yy115, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(65,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(66,&yymsp[-5].minor);
  yy_destructor(67,&yymsp[-4].minor);
  yy_destructor(31,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3384 "parser.c"
        break;
      case 118:
// 1318 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3393 "parser.c"
        break;
      case 121:
// 1331 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("assign");
  yy_destructor(36,&yymsp[0].minor);
}
// 3401 "parser.c"
        break;
      case 122:
// 1336 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("add-assign");
  yy_destructor(69,&yymsp[0].minor);
}
// 3409 "parser.c"
        break;
      case 123:
// 1341 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("sub-assign");
  yy_destructor(70,&yymsp[0].minor);
}
// 3417 "parser.c"
        break;
      case 124:
// 1345 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("mult-assign");
  yy_destructor(71,&yymsp[0].minor);
}
// 3425 "parser.c"
        break;
      case 125:
// 1349 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("div-assign");
  yy_destructor(72,&yymsp[0].minor);
}
// 3433 "parser.c"
        break;
      case 126:
// 1353 "parser.lemon"
{
	yygotominor.yy115 = json_object_new_string("concat-assign");
  yy_destructor(73,&yymsp[0].minor);
}
// 3441 "parser.c"
        break;
      case 127:
// 1358 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy115, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 3448 "parser.c"
        break;
      case 128:
// 1363 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
}
// 3456 "parser.c"
        break;
      case 129:
// 1368 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-2].minor);
}
// 3466 "parser.c"
        break;
      case 130:
// 1373 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(74,&yymsp[-6].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(76,&yymsp[-2].minor);
}
// 3476 "parser.c"
        break;
      case 131:
// 1378 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
}
// 3484 "parser.c"
        break;
      case 132:
// 1383 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy115, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(77,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-2].minor);
}
// 3494 "parser.c"
        break;
      case 133:
// 1388 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy115, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(77,&yymsp[-6].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(76,&yymsp[-2].minor);
}
// 3504 "parser.c"
        break;
      case 134:
// 1393 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy115, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-2].minor);
}
// 3513 "parser.c"
        break;
      case 135:
// 1398 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy115, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(76,&yymsp[-2].minor);
}
// 3522 "parser.c"
        break;
      case 136:
// 1403 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[0].minor);
}
// 3530 "parser.c"
        break;
      case 137:
// 1408 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[0].minor);
}
// 3538 "parser.c"
        break;
      case 139:
// 1416 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_echo_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3547 "parser.c"
        break;
      case 142:
// 1428 "parser.lemon"
{
	yygotominor.yy115 = yymsp[0].minor.yy115;;
}
// 3554 "parser.c"
        break;
      case 143:
// 1433 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(29,&yymsp[0].minor);
}
// 3562 "parser.c"
        break;
      case 144:
// 1438 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(29,&yymsp[0].minor);
}
// 3570 "parser.c"
        break;
      case 145:
// 1443 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_return_statement(yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(81,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3579 "parser.c"
        break;
      case 146:
// 1448 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3593 "parser.c"
        break;
      case 147:
// 1457 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3608 "parser.c"
        break;
      case 148:
// 1466 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3623 "parser.c"
        break;
      case 149:
// 1475 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(76,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3638 "parser.c"
        break;
      case 150:
// 1484 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(85,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3647 "parser.c"
        break;
      case 151:
// 1489 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy115, status->scanner_state);
  yy_destructor(85,&yymsp[-6].minor);
  yy_destructor(24,&yymsp[-5].minor);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3659 "parser.c"
        break;
      case 152:
// 1493 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3668 "parser.c"
        break;
      case 153:
// 1497 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(44,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3677 "parser.c"
        break;
      case 154:
// 1501 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3686 "parser.c"
        break;
      case 155:
// 1505 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3695 "parser.c"
        break;
      case 156:
// 1509 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3704 "parser.c"
        break;
      case 157:
// 1513 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3713 "parser.c"
        break;
      case 158:
// 1517 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3722 "parser.c"
        break;
      case 159:
// 1521 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3731 "parser.c"
        break;
      case 160:
// 1525 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3740 "parser.c"
        break;
      case 161:
// 1529 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3749 "parser.c"
        break;
      case 164:
// 1541 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3756 "parser.c"
        break;
      case 165:
// 1545 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 3764 "parser.c"
        break;
      case 167:
// 1553 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 3772 "parser.c"
        break;
      case 168:
// 1557 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("isset", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 3780 "parser.c"
        break;
      case 169:
// 1561 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3788 "parser.c"
        break;
      case 170:
// 1565 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-equals", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3796 "parser.c"
        break;
      case 171:
// 1569 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3804 "parser.c"
        break;
      case 172:
// 1573 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("not-identical", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3812 "parser.c"
        break;
      case 173:
// 1577 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3820 "parser.c"
        break;
      case 174:
// 1581 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3828 "parser.c"
        break;
      case 175:
// 1585 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("less-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3836 "parser.c"
        break;
      case 176:
// 1589 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3844 "parser.c"
        break;
      case 177:
// 1593 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("list", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 3853 "parser.c"
        break;
      case 178:
// 1597 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("cast", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(26,&yymsp[-1].minor);
}
// 3862 "parser.c"
        break;
      case 179:
// 1601 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-1].minor);
}
// 3870 "parser.c"
        break;
      case 180:
// 1605 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 3878 "parser.c"
        break;
      case 181:
// 1609 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 3886 "parser.c"
        break;
      case 182:
      case 233:
// 1613 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 3896 "parser.c"
        break;
      case 183:
      case 234:
// 1618 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 3906 "parser.c"
        break;
      case 184:
      case 235:
// 1623 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 3916 "parser.c"
        break;
      case 185:
// 1628 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("add", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3924 "parser.c"
        break;
      case 186:
// 1633 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("sub", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3932 "parser.c"
        break;
      case 187:
// 1638 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("concat", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3940 "parser.c"
        break;
      case 188:
// 1643 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("and", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3948 "parser.c"
        break;
      case 189:
// 1648 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("or", yymsp[-2].minor.yy115, yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3956 "parser.c"
        break;
      case 190:
// 1653 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("instanceof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3964 "parser.c"
        break;
      case 191:
// 1658 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy115, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3973 "parser.c"
        break;
      case 192:
// 1663 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("typeof", yymsp[0].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3981 "parser.c"
        break;
      case 193:
// 1668 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 3988 "parser.c"
        break;
      case 194:
      case 222:
      case 226:
// 1673 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3997 "parser.c"
        break;
      case 195:
      case 224:
      case 227:
// 1678 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4006 "parser.c"
        break;
      case 196:
      case 223:
      case 228:
// 1683 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4015 "parser.c"
        break;
      case 197:
// 1688 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4022 "parser.c"
        break;
      case 198:
      case 229:
// 1693 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4030 "parser.c"
        break;
      case 199:
      case 230:
// 1698 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4039 "parser.c"
        break;
      case 200:
      case 232:
// 1703 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(89,&yymsp[0].minor);
}
// 4048 "parser.c"
        break;
      case 201:
      case 231:
// 1708 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(90,&yymsp[0].minor);
}
// 4057 "parser.c"
        break;
      case 202:
// 1713 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-1].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4066 "parser.c"
        break;
      case 203:
// 1718 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_expr("array", yymsp[-1].minor.yy115, NULL, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-2].minor);
  yy_destructor(76,&yymsp[0].minor);
}
// 4075 "parser.c"
        break;
      case 204:
// 1723 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4083 "parser.c"
        break;
      case 205:
// 1728 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 4093 "parser.c"
        break;
      case 206:
// 1733 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(24,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 4103 "parser.c"
        break;
      case 207:
// 1738 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 4112 "parser.c"
        break;
      case 208:
// 1743 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 4121 "parser.c"
        break;
      case 211:
// 1758 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(77,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 4131 "parser.c"
        break;
      case 212:
// 1763 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 4141 "parser.c"
        break;
      case 213:
// 1768 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy115, status->scanner_state);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 4151 "parser.c"
        break;
      case 214:
// 1773 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(40,&yymsp[-1].minor);
  yy_destructor(26,&yymsp[0].minor);
}
// 4161 "parser.c"
        break;
      case 220:
// 1797 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(yymsp[-2].minor.yy115, yymsp[0].minor.yy115, status->scanner_state);
  yy_destructor(60,&yymsp[-1].minor);
}
// 4169 "parser.c"
        break;
      case 221:
// 1801 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_array_item(NULL, yymsp[0].minor.yy115, status->scanner_state);
}
// 4176 "parser.c"
        break;
      case 237:
// 1868 "parser.lemon"
{
	yygotominor.yy115 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4183 "parser.c"
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
// 801 "parser.lemon"


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

// 4259 "parser.c"
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
	parser_status->number_brackets = 0;

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
			case XX_T_FINAL:
				xx_(xx_parser, XX_FINAL, NULL, parser_status);
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
			case XX_T_DO:
				xx_(xx_parser, XX_DO, NULL, parser_status);
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
			case XX_T_DOUBLECOLON:
				xx_(xx_parser, XX_DOUBLECOLON, NULL, parser_status);
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
				parser_status->number_brackets++;
				xx_(xx_parser, XX_BRACKET_OPEN, NULL, parser_status);
				break;
			case XX_T_BRACKET_CLOSE:
				parser_status->number_brackets--;
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
				if (parser_status->number_brackets <= 1) {
					xx_parse_with_token(xx_parser, XX_T_COMMENT, XX_COMMENT, &token, parser_status);
				}
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
			case XX_T_CONSTANT:
				xx_parse_with_token(xx_parser, XX_T_CONSTANT, XX_CONSTANT, &token, parser_status);
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
