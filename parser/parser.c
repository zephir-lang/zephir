/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 45 "parser.lemon"


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
#define YYNOCODE 156
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy175;
  int yy311;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 500
#define YYNRULE 240
#define YYERRORSYMBOL 93
#define YYERRSYMDT yy311
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
 /*     0 */    89,  152,  156,  150,  188,   86,  348,  325,  145,  438,
 /*    10 */   443,   17,  458,  466,  500,  472,  446,  234,  158,  442,
 /*    20 */   128,   63,  386,  129,  130,  131,  132,  133,  134,  135,
 /*    30 */   136,  137,  138,  139,  124,  126,   88,  102,  106,  108,
 /*    40 */   110,  112,  104,  100,  114,  116,  122,  118,  120,    8,
 /*    50 */   235,  236,  237,  238,  239,  166,  247,  175,  353,  489,
 /*    60 */   369,  493,   73,  159,  160,  358,  161,  162,  163,  164,
 /*    70 */   165,  320,    9,  317,  326,  335,  338,  329,  332,  341,
 /*    80 */   347,  344,  149,  350,   46,   48,   50,  365,   59,  203,
 /*    90 */   195,  196,   70,   74,   79,  221,  328,  197,  228,  208,
 /*   100 */   168,  353,  187,  172,  183,  376,    7,  320,  358,   10,
 /*   110 */   279,  290,  293,  486,  358,  308,  317,  326,  335,  338,
 /*   120 */   329,  332,  341,  347,  344,  255,  350,   46,   48,   50,
 /*   130 */   272,   59,  346,  195,  196,   70,   74,   79,  221,  184,
 /*   140 */   474,  228,  477,  478,  466,   19,  445,  446,   18,  223,
 /*   150 */   442,  442,  353,  279,  290,  293,  382,   11,  308,  358,
 /*   160 */   741,    1,    2,  499,    4,    5,    6,  317,  326,  335,
 /*   170 */   338,  329,  332,  341,  347,  344,  284,  350,   46,   48,
 /*   180 */    50,   14,   59,  283,  195,  196,   70,   74,   79,  221,
 /*   190 */   318,  325,  228,  356,  186,  185,  354,  210,  179,  180,
 /*   200 */   182,  181,  427,  353,  279,  290,  293,  227,  442,  308,
 /*   210 */   358,  417,  234,  114,  116,  122,  118,  120,  317,  326,
 /*   220 */   335,  338,  329,  332,  341,  347,  344,  255,  350,   46,
 /*   230 */    48,   50,  251,   59,   21,  195,  196,   70,   74,   79,
 /*   240 */   221,  176,   24,  228,  460,  235,  236,  237,  238,  239,
 /*   250 */   442,  260,  212,  214,  353,  279,  290,  293,   25,  284,
 /*   260 */   308,  358,  401,  405,  409,  280,  285,  195,  196,  317,
 /*   270 */   326,  335,  338,  329,  332,  341,  347,  344,  255,  350,
 /*   280 */    46,   48,   50,  264,   59,   47,  195,  196,   70,   74,
 /*   290 */    79,  221,  229,  278,  228,  194,  177,  178,  118,  120,
 /*   300 */   179,  180,  182,  181,   22,  353,  279,  290,  293,   27,
 /*   310 */   436,  308,  358,  416,  405,  409,   49,  218,  234,   52,
 /*   320 */   317,  326,  335,  338,  329,  332,  341,  347,  344,  111,
 /*   330 */   350,   46,   48,   50,   12,   59,  480,  195,  196,   70,
 /*   340 */    74,   79,  221,  177,  178,  228,  194,  179,  180,  182,
 /*   350 */   181,  235,  236,  237,  238,  239,  353,  279,  290,  293,
 /*   360 */   441,  242,  308,  358,  170,  259,   55,  216,  315,  195,
 /*   370 */   196,  317,  326,  335,  338,  329,  332,  341,  347,  344,
 /*   380 */   157,  350,   46,   48,   50,   56,   59,   61,  195,  196,
 /*   390 */    70,   74,   79,  221,  311,  444,  228,   19,  309,  445,
 /*   400 */   446,  146,  209,  442,  202,  295,  298,  353,  279,  290,
 /*   410 */   293,  439,  242,  308,  358,   67,  241,  351,  325,  175,
 /*   420 */   195,  196,  317,  326,  335,  338,  329,  332,  341,  347,
 /*   430 */   344,  125,  350,   46,   48,   50,  169,   59,  468,  195,
 /*   440 */   196,   70,   74,   79,  221,  469,  105,  228,  475,  458,
 /*   450 */   466,   65,  299,  446,  195,  196,  442,  175,  353,  279,
 /*   460 */   290,  293,  435,  242,  308,  358,  174,  254,  342,  325,
 /*   470 */    68,  195,  196,  317,  326,  335,  338,  329,  332,  341,
 /*   480 */   347,  344,  462,  350,   46,   48,   50,   71,   59,  463,
 /*   490 */    64,  149,   70,   74,   79,  221,  282,   76,  228,  195,
 /*   500 */   196,  314,  171,  172,  183,   80,  198,  302,  305,  353,
 /*   510 */   279,  290,  293,  433,  242,  308,  358,   66,  267,  383,
 /*   520 */    83,  281,  195,  196,  317,  326,  335,  338,  329,  332,
 /*   530 */   341,  347,  344,  127,  350,   46,   48,   50,   90,   59,
 /*   540 */   320,  195,  196,   70,   74,   79,  221,   60,  242,  228,
 /*   550 */   220,   85,  250,   91,   93,   92,  195,  196,  195,  196,
 /*   560 */   353,  279,  290,  293,  399,  319,  308,  358,  394,  400,
 /*   570 */    94,  416,  405,  409,  320,  317,  326,  335,  338,  329,
 /*   580 */   332,  341,  347,  344,   96,  350,   46,   48,   50,   98,
 /*   590 */    59,  437,   62,  149,   70,   74,   79,  221,  320,  349,
 /*   600 */   228,  195,  196,  192,    3,    4,    5,    6,  198,   95,
 /*   610 */    97,  353,  279,  290,  293,  397,  242,  308,  358,   66,
 /*   620 */   271,  383,  143,  352,  195,  196,  317,  326,  335,  338,
 /*   630 */   329,  332,  341,  347,  344,  113,  350,   46,   48,   50,
 /*   640 */   148,   59,  147,  195,  196,   70,   74,   79,  221,  379,
 /*   650 */   242,  228,  220,  153,  263,  345,  325,  154,  195,  196,
 /*   660 */   195,  196,  353,  279,  290,  293,  392,  242,  308,  358,
 /*   670 */   155,  275,  339,  325,  189,  195,  196,  317,  326,  335,
 /*   680 */   338,  329,  332,  341,  347,  344,  151,  350,   46,   48,
 /*   690 */    50,  173,   59,  724,  195,  196,   70,   74,   79,  221,
 /*   700 */    75,  242,  228,  220,  725,  246,  336,  325,  726,  195,
 /*   710 */   196,  195,  196,  353,  279,  290,  293,  393,   87,  308,
 /*   720 */   358,  190,  400,  333,  325,  211,  195,  196,  317,  326,
 /*   730 */   335,  338,  329,  332,  341,  347,  344,  115,  350,   46,
 /*   740 */    48,   50,  204,   59,  395,  195,  196,   70,   74,   79,
 /*   750 */   221,   84,  103,  228,  220,  330,  325,  327,  325,  213,
 /*   760 */   195,  196,  195,  196,  353,  279,  290,  293,   82,  361,
 /*   770 */   308,  358,  215,  194,  222,  233,  232,  195,  196,  317,
 /*   780 */   326,  335,  338,  329,  332,  341,  347,  344,  373,  350,
 /*   790 */    46,   48,   50,  240,   59,  193,  195,  196,   70,   74,
 /*   800 */    79,  221,   51,  107,  228,  220,  244,  245,  252,  249,
 /*   810 */   253,  195,  196,  195,  196,  353,  279,  290,  293,  377,
 /*   820 */   117,  308,  358,  257,  194,  258,  262,  265,  195,  196,
 /*   830 */   317,  326,  335,  338,  329,  332,  341,  347,  344,  144,
 /*   840 */   350,   46,   48,   50,  266,   59,  200,  195,  196,   70,
 /*   850 */    74,   79,  221,  224,  194,  228,  119,  270,  273,  287,
 /*   860 */   274,  195,  196,  289,  195,  196,  353,  279,  290,  293,
 /*   870 */   388,  291,  308,  358,  294,  296,  206,  297,  300,  195,
 /*   880 */   196,  317,  326,  335,  338,  329,  332,  341,  347,  344,
 /*   890 */   109,  350,   46,   48,   50,  301,   59,  304,  195,  196,
 /*   900 */    70,   74,   79,  221,  368,  303,  228,  121,  306,  307,
 /*   910 */   310,  312,  195,  196,  313,  195,  196,  353,  279,  290,
 /*   920 */   293,   54,  101,  308,  358,  316,  322,  321,  323,  324,
 /*   930 */   195,  196,  317,  326,  335,  338,  329,  332,  341,  347,
 /*   940 */   344,   99,  350,   46,   48,   50,  355,   59,  231,  195,
 /*   950 */   196,   70,   74,   79,  221,  123,  320,  228,  357,  366,
 /*   960 */   367,  380,  378,  195,  196,  384,  389,  390,  353,  279,
 /*   970 */   290,  293,  387,  230,  308,  358,  320,  396,  403,  406,
 /*   980 */   404,  343,  320,  317,  326,  335,  338,  329,  332,  341,
 /*   990 */   347,  344,  320,  350,   46,   48,   50,  407,   59,  320,
 /*  1000 */   408,  340,   70,   74,   79,  221,  411,  337,  228,  410,
 /*  1010 */   414,  412,  415,  563,  429,  430,  450,  334,  432,  353,
 /*  1020 */   279,  290,  293,   58,  331,  308,  358,  449,  451,  452,
 /*  1030 */   454,  456,  455,  457,  317,  326,  335,  338,  329,  332,
 /*  1040 */   341,  347,  344,  464,  350,   46,   48,   50,  465,   59,
 /*  1050 */   470,  471,  481,   70,   74,   79,  221,  482,  488,  228,
 /*  1060 */   484,  487,  494,  397,  491,  497,  495,  397,  397,  397,
 /*  1070 */   353,  279,  290,  293,   78,  397,  308,  358,  397,  397,
 /*  1080 */   397,  397,  397,  397,  397,  317,  326,  335,  338,  329,
 /*  1090 */   332,  341,  347,  344,  397,  350,   46,   48,   50,  397,
 /*  1100 */    59,  397,  397,  397,   70,   74,   79,  221,  397,  397,
 /*  1110 */   228,  397,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  1120 */   397,  353,  279,  290,  293,  381,  397,  308,  358,  397,
 /*  1130 */   397,  397,  397,  397,  397,  397,  317,  326,  335,  338,
 /*  1140 */   329,  332,  341,  347,  344,  397,  350,   46,   48,   50,
 /*  1150 */   397,   59,  397,  397,  397,   70,   74,   79,  221,  397,
 /*  1160 */   397,  228,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  1170 */   397,  397,  353,  279,  290,  293,  371,  397,  308,  358,
 /*  1180 */   397,  397,  397,  397,  397,  397,  397,  317,  326,  335,
 /*  1190 */   338,  329,  332,  341,  347,  344,  397,  350,   46,   48,
 /*  1200 */    50,  397,   59,  397,  397,  397,   70,   74,   79,  221,
 /*  1210 */   397,  397,  228,  397,  397,  397,  397,  397,  397,  397,
 /*  1220 */   397,  397,  397,  353,  279,  290,  293,  364,  397,  308,
 /*  1230 */   358,  418,  419,  421,  420,  422,  397,  397,  317,  326,
 /*  1240 */   335,  338,  329,  332,  341,  347,  344,  397,  350,   46,
 /*  1250 */    48,   50,  397,   59,  184,  397,  397,   70,   74,   79,
 /*  1260 */   221,  397,  397,  228,  492,  397,  424,  459,  397,  453,
 /*  1270 */   397,  353,  397,  423,  425,  279,  290,  293,  358,  397,
 /*  1280 */   308,  397,  397,  397,  397,  397,  317,  326,  335,  338,
 /*  1290 */   329,  332,  341,  347,  344,  397,  350,   46,   48,   50,
 /*  1300 */   397,   59,  167,  501,  397,   70,   74,   79,  221,  186,
 /*  1310 */   185,  228,  149,  179,  180,  182,  181,  397,  397,  397,
 /*  1320 */   195,  196,  205,  279,  290,  293,   45,  198,  308,  397,
 /*  1330 */   397,  397,    7,  397,  397,   10,  397,  434,  397,  486,
 /*  1340 */   358,  397,  359,   29,   30,   31,   32,   33,   34,   35,
 /*  1350 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  397,
 /*  1360 */   397,   45,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  1370 */   286,  288,   26,  397,  397,  397,  397,  359,   29,   30,
 /*  1380 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1390 */    41,   42,   43,   44,  149,  397,  397,   45,  397,  397,
 /*  1400 */   397,  397,  195,  196,  217,  286,  288,  397,  391,  198,
 /*  1410 */   397,  397,  397,  359,   29,   30,   31,   32,   33,   34,
 /*  1420 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1430 */   397,  397,  397,  397,  397,  397,  397,  397,   45,  397,
 /*  1440 */   397,  286,  288,  418,  419,  421,  420,  422,  397,   77,
 /*  1450 */   397,  397,  397,  397,  359,   29,   30,   31,   32,   33,
 /*  1460 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1470 */    44,  397,  397,   45,  397,  397,  485,  397,  424,  459,
 /*  1480 */   397,  453,  286,  288,  226,  423,  425,  397,  397,  359,
 /*  1490 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1500 */    39,   40,   41,   42,   43,   44,  397,  397,  397,   45,
 /*  1510 */   397,  397,  397,  397,  397,  397,  397,  286,  288,  397,
 /*  1520 */   385,  142,  397,  397,  397,  359,   29,   30,   31,   32,
 /*  1530 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1540 */    43,   44,  397,  140,  397,  397,  397,  397,  397,  397,
 /*  1550 */    45,  195,  196,  286,  288,  418,  419,  421,  420,  422,
 /*  1560 */   397,  375,  397,  397,  397,  397,  359,   29,   30,   31,
 /*  1570 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1580 */    42,   43,   44,  397,  397,   45,  397,  397,   15,  397,
 /*  1590 */   424,  459,  397,  453,  286,  288,   57,  423,  425,  397,
 /*  1600 */   397,  359,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1610 */    37,   38,   39,   40,   41,   42,   43,   44,  149,  397,
 /*  1620 */   397,   45,  397,  397,  397,  397,  195,  196,  199,  286,
 /*  1630 */   288,  397,  370,  198,  397,  397,  397,  359,   29,   30,
 /*  1640 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1650 */    41,   42,   43,   44,  397,  397,  397,  397,  397,  397,
 /*  1660 */   397,  397,   45,  397,  397,  286,  288,  418,  419,  421,
 /*  1670 */   420,  422,  397,   69,  397,  397,  397,  397,  359,   29,
 /*  1680 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1690 */    40,   41,   42,   43,   44,  397,  397,   45,  397,  397,
 /*  1700 */   517,  397,  424,  459,  397,  453,  286,  288,  440,  423,
 /*  1710 */   425,  397,  397,  359,   29,   30,   31,   32,   33,   34,
 /*  1720 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1730 */   397,  397,  397,   45,  397,  397,  397,  397,  397,  397,
 /*  1740 */   397,  286,  288,  397,   81,  397,  397,  397,  397,  359,
 /*  1750 */    29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
 /*  1760 */    39,   40,   41,   42,   43,   44,  397,  397,  397,  397,
 /*  1770 */   397,  397,  397,  397,   45,  397,  397,  286,  288,  418,
 /*  1780 */   419,  421,  420,  422,  397,  398,  397,  397,  397,  397,
 /*  1790 */   359,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1800 */    38,   39,   40,   41,   42,   43,   44,  397,  397,   45,
 /*  1810 */   397,  397,  520,  397,  424,  447,  397,  453,  286,  288,
 /*  1820 */    53,  423,  425,  397,  397,  359,   29,   30,   31,   32,
 /*  1830 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1840 */    43,   44,  397,  397,  397,   45,  397,  397,  397,  397,
 /*  1850 */   397,  397,  397,  286,  288,  397,  363,  397,  397,  397,
 /*  1860 */   397,  359,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1870 */    37,   38,   39,   40,   41,   42,   43,   44,  397,  397,
 /*  1880 */   397,  397,  397,  397,  397,  397,   45,  397,  397,  286,
 /*  1890 */   288,  418,  419,  421,  420,  422,  397,   72,  397,  397,
 /*  1900 */   397,  397,  359,   29,   30,   31,   32,   33,   34,   35,
 /*  1910 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1920 */   397,  397,  397,  397,  516,  397,  424,  459,  397,  453,
 /*  1930 */   286,  288,  397,  423,  425,   28,   29,   30,   31,   32,
 /*  1940 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1950 */    43,   44,  397,   89,  152,  156,  150,  188,   86,  201,
 /*  1960 */   397,  145,  413,  286,  288,  397,  397,  397,  397,  397,
 /*  1970 */   397,  158,  397,  128,   89,  152,  156,  150,  188,   86,
 /*  1980 */   207,  402,  145,  418,  419,  421,  420,  422,  397,  397,
 /*  1990 */   397,  397,  158,  397,  128,  397,  129,  130,  131,  132,
 /*  2000 */   133,  134,  135,  136,  137,  138,  139,  397,  166,  397,
 /*  2010 */   397,  397,  397,  397,  397,  397,  159,  160,  424,  161,
 /*  2020 */   162,  163,  164,  165,  397,  423,  425,  397,  397,  166,
 /*  2030 */   397,  397,  397,  397,  397,  397,  397,  159,  160,  397,
 /*  2040 */   161,  162,  163,  164,  165,   89,  152,  156,  150,  188,
 /*  2050 */    86,  219,  397,  145,  397,  234,  397,  397,  397,  397,
 /*  2060 */   397,  397,  397,  158,  397,  128,   89,  152,  156,  150,
 /*  2070 */   188,   86,  397,  397,  145,  397,  397,  397,  397,  397,
 /*  2080 */   397,  397,  397,  397,  158,  397,  128,  397,  235,  236,
 /*  2090 */   237,  238,  239,  243,  268,  397,  256,  276,  277,  397,
 /*  2100 */   166,  397,  397,  397,  397,  397,  397,  397,  159,  160,
 /*  2110 */   397,  161,  162,  163,  164,  165,  397,  397,  397,  397,
 /*  2120 */   397,  166,  248,   89,  152,  156,  150,  188,   86,  159,
 /*  2130 */   160,  145,  161,  162,  163,  164,  165,  397,  397,  397,
 /*  2140 */   397,  158,  397,  128,  397,  397,  397,   89,  152,  156,
 /*  2150 */   150,  188,   86,  397,  397,  145,  397,  397,  397,  397,
 /*  2160 */   397,  397,  397,  397,  397,  158,  397,  128,  397,  496,
 /*  2170 */    16,  473,  479,  478,  466,  397,  472,  446,  166,  269,
 /*  2180 */   442,  397,  397,  397,  397,  397,  159,  160,  397,  161,
 /*  2190 */   162,  163,  164,  165,  360,  397,  490,   16,  473,  479,
 /*  2200 */   478,  466,  166,  472,  446,  397,  397,  442,  397,  397,
 /*  2210 */   159,  160,  397,  161,  162,  163,  164,  165,  397,  397,
 /*  2220 */   397,  397,  397,  397,  397,   89,  152,  156,  150,  188,
 /*  2230 */    86,  397,  397,  145,  397,  418,  419,  421,  420,  422,
 /*  2240 */   397,  397,  397,  158,  397,  128,  397,  397,  397,   89,
 /*  2250 */   152,  156,  150,  188,   86,  397,  397,  145,  397,  397,
 /*  2260 */   397,  397,  397,  397,  397,  397,  397,  158,  498,  128,
 /*  2270 */   424,  459,  397,  453,  397,  397,  397,  423,  425,  397,
 /*  2280 */   166,  261,  397,  397,  397,  397,  397,  397,  159,  160,
 /*  2290 */   397,  161,  162,  163,  164,  165,  372,  397,  483,   16,
 /*  2300 */   473,  479,  478,  466,  166,  472,  446,  397,  397,  442,
 /*  2310 */   397,  397,  159,  160,  397,  161,  162,  163,  164,  165,
 /*  2320 */   397,  418,  419,  421,  420,  422,  397,   89,  152,  156,
 /*  2330 */   150,  188,   86,  191,  397,  145,  397,  418,  419,  421,
 /*  2340 */   420,  422,  397,  397,  397,  158,  397,  128,  397,  397,
 /*  2350 */    89,  152,  156,  150,  188,   86,  424,  397,  145,  448,
 /*  2360 */   397,  397,  397,  423,  425,  397,  467,  397,  158,  397,
 /*  2370 */   128,  397,  424,  397,  397,  397,  397,   20,  397,  423,
 /*  2380 */   425,  397,  166,  397,  397,  397,  397,  397,  397,  397,
 /*  2390 */   159,  160,  397,  161,  162,  163,  164,  165,  397,  397,
 /*  2400 */   397,  397,  397,  397,  397,  166,  397,  418,  419,  421,
 /*  2410 */   420,  422,  397,  159,  160,  397,  161,  162,  163,  164,
 /*  2420 */   165,  124,  126,   88,  102,  106,  108,  110,  112,  104,
 /*  2430 */   100,  114,  116,  122,  118,  120,  397,  397,  397,  397,
 /*  2440 */   519,  397,  424,  426,  397,  292,  397,  397,  397,  423,
 /*  2450 */   425,  397,  397,  124,  126,   88,  102,  106,  108,  110,
 /*  2460 */   112,  104,  100,  114,  116,  122,  118,  120,  397,  397,
 /*  2470 */    13,   16,  473,  479,  478,  466,  397,  472,  446,  362,
 /*  2480 */   397,  442,  397,  397,  397,  397,  397,  124,  126,   88,
 /*  2490 */   102,  106,  108,  110,  112,  104,  100,  114,  116,  122,
 /*  2500 */   118,  120,  124,  126,   88,  102,  106,  108,  110,  112,
 /*  2510 */   104,  100,  114,  116,  122,  118,  120,  397,  397,  397,
 /*  2520 */   397,  397,  397,  397,  397,  397,  397,  397,  225,  397,
 /*  2530 */   397,  397,  397,  397,  397,  397,  124,  126,   88,  102,
 /*  2540 */   106,  108,  110,  112,  104,  100,  114,  116,  122,  118,
 /*  2550 */   120,  397,  418,  419,  421,  420,  422,  141,  638,  397,
 /*  2560 */   397,  397,  397,  397,  397,  124,  126,   88,  102,  106,
 /*  2570 */   108,  110,  112,  104,  100,  114,  116,  122,  118,  120,
 /*  2580 */   397,  461,  397,  397,  397,  397,  397,  424,  397,  397,
 /*  2590 */   397,  374,  428,  397,  423,  425,  397,  397,  397,  124,
 /*  2600 */   126,   88,  102,  106,  108,  110,  112,  104,  100,  114,
 /*  2610 */   116,  122,  118,  120,  413,  397,  397,  397,  397,  397,
 /*  2620 */   397,  397,  397,  397,  397,  413,  397,  397,  397,  397,
 /*  2630 */   397,  431,  397,  402,  397,  397,  397,  397,  397,  397,
 /*  2640 */   397,  397,   23,  397,  402,  397,  397,  397,  129,  130,
 /*  2650 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  129,
 /*  2660 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  2670 */    88,  102,  106,  108,  110,  112,  104,  100,  114,  116,
 /*  2680 */   122,  118,  120,  418,  419,  421,  420,  422,  418,  419,
 /*  2690 */   421,  420,  422,  397,  397,  397,  397,  397,  397,  397,
 /*  2700 */   397,  397,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  2710 */   397,  397,  397,  397,  397,  397,  523,  397,  424,  426,
 /*  2720 */   397,  518,  397,  424,  426,  423,  425,  397,  397,  397,
 /*  2730 */   423,  425,  418,  419,  421,  420,  422,  418,  419,  421,
 /*  2740 */   420,  422,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  2750 */   397,  397,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  2760 */   397,  397,  397,  397,  397,  522,  397,  424,  426,  397,
 /*  2770 */   524,  397,  424,  426,  423,  425,  397,  397,  397,  423,
 /*  2780 */   425,  418,  419,  421,  420,  422,  418,  419,  421,  420,
 /*  2790 */   422,  397,  418,  419,  421,  420,  422,  397,  397,  397,
 /*  2800 */   397,  397,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  2810 */   397,  397,  397,  397,  521,  397,  424,  476,  397,  397,
 /*  2820 */   397,  424,  397,  423,  425,  397,   20,  424,  423,  425,
 /*  2830 */   397,  397,  428,  397,  423,  425,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    22,   23,   24,   25,   26,   27,  147,  148,   30,   33,
 /*    10 */   103,  104,  105,  106,    0,  108,  109,   38,   40,  112,
 /*    20 */    42,  134,  135,   45,   46,   47,   48,   49,   50,   51,
 /*    30 */    52,   53,   54,   55,    7,    8,    9,   10,   11,   12,
 /*    40 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   30,
 /*    50 */    71,   72,   73,   74,   75,   77,   77,  107,   30,   33,
 /*    60 */    33,   35,   34,   85,   86,   37,   88,   89,   90,   91,
 /*    70 */    92,    6,   31,   45,   46,   47,   48,   49,   50,   51,
 /*    80 */    52,   53,  136,   55,   56,   57,   58,    6,   60,   30,
 /*    90 */   144,  145,   64,   65,   66,   67,   31,  151,   70,   40,
 /*   100 */   150,   30,  152,  153,  154,   34,   29,    6,   37,   32,
 /*   110 */    82,   83,   84,   36,   37,   87,   45,   46,   47,   48,
 /*   120 */    49,   50,   51,   52,   53,  136,   55,   56,   57,   58,
 /*   130 */   141,   60,   31,  144,  145,   64,   65,   66,   67,   30,
 /*   140 */   102,   70,  104,  105,  106,  106,  108,  109,  109,   68,
 /*   150 */   112,  112,   30,   82,   83,   84,   34,   30,   87,   37,
 /*   160 */    94,   95,   96,   97,   98,   99,  100,   45,   46,   47,
 /*   170 */    48,   49,   50,   51,   52,   53,  136,   55,   56,   57,
 /*   180 */    58,   34,   60,  143,  144,  145,   64,   65,   66,   67,
 /*   190 */   147,  148,   70,   76,   85,   86,   79,   30,   89,   90,
 /*   200 */    91,   92,  106,   30,   82,   83,   84,   34,  112,   87,
 /*   210 */    37,  112,   38,   17,   18,   19,   20,   21,   45,   46,
 /*   220 */    47,   48,   49,   50,   51,   52,   53,  136,   55,   56,
 /*   230 */    57,   58,  141,   60,   30,  144,  145,   64,   65,   66,
 /*   240 */    67,   30,   33,   70,  106,   71,   72,   73,   74,   75,
 /*   250 */   112,   77,   85,   86,   30,   82,   83,   84,   34,  136,
 /*   260 */    87,   37,  113,  114,  115,  142,  143,  144,  145,   45,
 /*   270 */    46,   47,   48,   49,   50,   51,   52,   53,  136,   55,
 /*   280 */    56,   57,   58,  141,   60,   31,  144,  145,   64,   65,
 /*   290 */    66,   67,  137,  138,   70,    6,   85,   86,   20,   21,
 /*   300 */    89,   90,   91,   92,   42,   30,   82,   83,   84,   34,
 /*   310 */   110,   87,   37,  113,  114,  115,   31,   28,   38,   33,
 /*   320 */    45,   46,   47,   48,   49,   50,   51,   52,   53,  136,
 /*   330 */    55,   56,   57,   58,   33,   60,   35,  144,  145,   64,
 /*   340 */    65,   66,   67,   85,   86,   70,    6,   89,   90,   91,
 /*   350 */    92,   71,   72,   73,   74,   75,   30,   82,   83,   84,
 /*   360 */    34,  136,   87,   37,    6,  140,   59,   42,   28,  144,
 /*   370 */   145,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   380 */   136,   55,   56,   57,   58,   33,   60,   33,  144,  145,
 /*   390 */    64,   65,   66,   67,   26,  104,   70,  106,   30,  108,
 /*   400 */   109,   76,   77,  112,   79,   76,   77,   30,   82,   83,
 /*   410 */    84,   34,  136,   87,   37,  107,  140,  147,  148,  107,
 /*   420 */   144,  145,   45,   46,   47,   48,   49,   50,   51,   52,
 /*   430 */    53,  136,   55,   56,   57,   58,   78,   60,   31,  144,
 /*   440 */   145,   64,   65,   66,   67,   38,  136,   70,  104,  105,
 /*   450 */   106,  135,   30,  109,  144,  145,  112,  107,   30,   82,
 /*   460 */    83,   84,   34,  136,   87,   37,  154,  140,  147,  148,
 /*   470 */    62,  144,  145,   45,   46,   47,   48,   49,   50,   51,
 /*   480 */    52,   53,   31,   55,   56,   57,   58,   33,   60,   38,
 /*   490 */    34,  136,   64,   65,   66,   67,    6,   33,   70,  144,
 /*   500 */   145,  146,  152,  153,  154,   33,  151,   85,   86,   30,
 /*   510 */    82,   83,   84,   34,  136,   87,   37,   61,  140,   63,
 /*   520 */    65,   31,  144,  145,   45,   46,   47,   48,   49,   50,
 /*   530 */    51,   52,   53,  136,   55,   56,   57,   58,  149,   60,
 /*   540 */     6,  144,  145,   64,   65,   66,   67,  133,  136,   70,
 /*   550 */   136,   31,  140,   30,   30,   77,  144,  145,  144,  145,
 /*   560 */    30,   82,   83,   84,   34,   31,   87,   37,  110,    6,
 /*   570 */    78,  113,  114,  115,    6,   45,   46,   47,   48,   49,
 /*   580 */    50,   51,   52,   53,   78,   55,   56,   57,   58,   78,
 /*   590 */    60,   28,   34,  136,   64,   65,   66,   67,    6,   31,
 /*   600 */    70,  144,  145,  146,   97,   98,   99,  100,  151,   85,
 /*   610 */    86,   30,   82,   83,   84,   34,  136,   87,   37,   61,
 /*   620 */   140,   63,   28,   31,  144,  145,   45,   46,   47,   48,
 /*   630 */    49,   50,   51,   52,   53,  136,   55,   56,   57,   58,
 /*   640 */    42,   60,   30,  144,  145,   64,   65,   66,   67,  133,
 /*   650 */   136,   70,  136,   30,  140,  147,  148,    6,  144,  145,
 /*   660 */   144,  145,   30,   82,   83,   84,   34,  136,   87,   37,
 /*   670 */   149,  140,  147,  148,   30,  144,  145,   45,   46,   47,
 /*   680 */    48,   49,   50,   51,   52,   53,  136,   55,   56,   57,
 /*   690 */    58,   62,   60,   62,  144,  145,   64,   65,   66,   67,
 /*   700 */   133,  136,   70,  136,   62,  140,  147,  148,   62,  144,
 /*   710 */   145,  144,  145,   30,   82,   83,   84,   34,  136,   87,
 /*   720 */    37,   42,    6,  147,  148,   78,  144,  145,   45,   46,
 /*   730 */    47,   48,   49,   50,   51,   52,   53,  136,   55,   56,
 /*   740 */    57,   58,   42,   60,   28,  144,  145,   64,   65,   66,
 /*   750 */    67,  133,  136,   70,  136,  147,  148,  147,  148,   78,
 /*   760 */   144,  145,  144,  145,   30,   82,   83,   84,   34,  136,
 /*   770 */    87,   37,   78,    6,   30,   30,  138,  144,  145,   45,
 /*   780 */    46,   47,   48,   49,   50,   51,   52,   53,  136,   55,
 /*   790 */    56,   57,   58,  139,   60,   28,  144,  145,   64,   65,
 /*   800 */    66,   67,  133,  136,   70,  136,   30,  139,   78,  139,
 /*   810 */   139,  144,  145,  144,  145,   30,   82,   83,   84,   34,
 /*   820 */   136,   87,   37,   30,    6,  139,  139,   78,  144,  145,
 /*   830 */    45,   46,   47,   48,   49,   50,   51,   52,   53,  136,
 /*   840 */    55,   56,   57,   58,  139,   60,   28,  144,  145,   64,
 /*   850 */    65,   66,   67,  136,    6,   70,  136,  139,   78,   31,
 /*   860 */   139,  144,  145,   31,  144,  145,   30,   82,   83,   84,
 /*   870 */    34,  136,   87,   37,   30,   30,   28,   31,   78,  144,
 /*   880 */   145,   45,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   890 */   136,   55,   56,   57,   58,   31,   60,   31,  144,  145,
 /*   900 */    64,   65,   66,   67,  136,   78,   70,  136,   78,   31,
 /*   910 */    31,   30,  144,  145,   42,  144,  145,   30,   82,   83,
 /*   920 */    84,   34,  136,   87,   37,   31,   30,  148,   38,  107,
 /*   930 */   144,  145,   45,   46,   47,   48,   49,   50,   51,   52,
 /*   940 */    53,  136,   55,   56,   57,   58,   30,   60,    6,  144,
 /*   950 */   145,   64,   65,   66,   67,  136,    6,   70,   30,   30,
 /*   960 */    68,   31,   65,  144,  145,   62,   59,   33,   30,   82,
 /*   970 */    83,   84,   34,   31,   87,   37,    6,   33,   38,   30,
 /*   980 */   107,   31,    6,   45,   46,   47,   48,   49,   50,   51,
 /*   990 */    52,   53,    6,   55,   56,   57,   58,   38,   60,    6,
 /*  1000 */   107,   31,   64,   65,   66,   67,   38,   31,   70,   30,
 /*  1010 */    30,  107,   12,   30,   30,   42,   38,   31,   33,   30,
 /*  1020 */    82,   83,   84,   34,   31,   87,   37,   40,  107,   31,
 /*  1030 */    40,  107,   38,   31,   45,   46,   47,   48,   49,   50,
 /*  1040 */    51,   52,   53,  107,   55,   56,   57,   58,   31,   60,
 /*  1050 */   107,   31,   30,   64,   65,   66,   67,   33,   30,   70,
 /*  1060 */    34,   32,   30,  155,   34,   34,   33,  155,  155,  155,
 /*  1070 */    30,   82,   83,   84,   34,  155,   87,   37,  155,  155,
 /*  1080 */   155,  155,  155,  155,  155,   45,   46,   47,   48,   49,
 /*  1090 */    50,   51,   52,   53,  155,   55,   56,   57,   58,  155,
 /*  1100 */    60,  155,  155,  155,   64,   65,   66,   67,  155,  155,
 /*  1110 */    70,  155,  155,  155,  155,  155,  155,  155,  155,  155,
 /*  1120 */   155,   30,   82,   83,   84,   34,  155,   87,   37,  155,
 /*  1130 */   155,  155,  155,  155,  155,  155,   45,   46,   47,   48,
 /*  1140 */    49,   50,   51,   52,   53,  155,   55,   56,   57,   58,
 /*  1150 */   155,   60,  155,  155,  155,   64,   65,   66,   67,  155,
 /*  1160 */   155,   70,  155,  155,  155,  155,  155,  155,  155,  155,
 /*  1170 */   155,  155,   30,   82,   83,   84,   34,  155,   87,   37,
 /*  1180 */   155,  155,  155,  155,  155,  155,  155,   45,   46,   47,
 /*  1190 */    48,   49,   50,   51,   52,   53,  155,   55,   56,   57,
 /*  1200 */    58,  155,   60,  155,  155,  155,   64,   65,   66,   67,
 /*  1210 */   155,  155,   70,  155,  155,  155,  155,  155,  155,  155,
 /*  1220 */   155,  155,  155,   30,   82,   83,   84,   34,  155,   87,
 /*  1230 */    37,    1,    2,    3,    4,    5,  155,  155,   45,   46,
 /*  1240 */    47,   48,   49,   50,   51,   52,   53,  155,   55,   56,
 /*  1250 */    57,   58,  155,   60,   30,  155,  155,   64,   65,   66,
 /*  1260 */    67,  155,  155,   70,   34,  155,   36,   37,  155,   39,
 /*  1270 */   155,   30,  155,   43,   44,   82,   83,   84,   37,  155,
 /*  1280 */    87,  155,  155,  155,  155,  155,   45,   46,   47,   48,
 /*  1290 */    49,   50,   51,   52,   53,  155,   55,   56,   57,   58,
 /*  1300 */   155,   60,   78,    0,  155,   64,   65,   66,   67,   85,
 /*  1310 */    86,   70,  136,   89,   90,   91,   92,  155,  155,  155,
 /*  1320 */   144,  145,  146,   82,   83,   84,  100,  151,   87,  155,
 /*  1330 */   155,  155,   29,  155,  155,   32,  155,  111,  155,   36,
 /*  1340 */    37,  155,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1350 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  155,
 /*  1360 */   155,  100,  155,  155,  155,  155,  155,  155,  155,  155,
 /*  1370 */   144,  145,  111,  155,  155,  155,  155,  116,  117,  118,
 /*  1380 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1390 */   129,  130,  131,  132,  136,  155,  155,  100,  155,  155,
 /*  1400 */   155,  155,  144,  145,  146,  144,  145,  155,  111,  151,
 /*  1410 */   155,  155,  155,  116,  117,  118,  119,  120,  121,  122,
 /*  1420 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1430 */   155,  155,  155,  155,  155,  155,  155,  155,  100,  155,
 /*  1440 */   155,  144,  145,    1,    2,    3,    4,    5,  155,  111,
 /*  1450 */   155,  155,  155,  155,  116,  117,  118,  119,  120,  121,
 /*  1460 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1470 */   132,  155,  155,  100,  155,  155,   34,  155,   36,   37,
 /*  1480 */   155,   39,  144,  145,  111,   43,   44,  155,  155,  116,
 /*  1490 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1500 */   127,  128,  129,  130,  131,  132,  155,  155,  155,  100,
 /*  1510 */   155,  155,  155,  155,  155,  155,  155,  144,  145,  155,
 /*  1520 */   111,  114,  155,  155,  155,  116,  117,  118,  119,  120,
 /*  1530 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1540 */   131,  132,  155,  136,  155,  155,  155,  155,  155,  155,
 /*  1550 */   100,  144,  145,  144,  145,    1,    2,    3,    4,    5,
 /*  1560 */   155,  111,  155,  155,  155,  155,  116,  117,  118,  119,
 /*  1570 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1580 */   130,  131,  132,  155,  155,  100,  155,  155,   34,  155,
 /*  1590 */    36,   37,  155,   39,  144,  145,  111,   43,   44,  155,
 /*  1600 */   155,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1610 */   125,  126,  127,  128,  129,  130,  131,  132,  136,  155,
 /*  1620 */   155,  100,  155,  155,  155,  155,  144,  145,  146,  144,
 /*  1630 */   145,  155,  111,  151,  155,  155,  155,  116,  117,  118,
 /*  1640 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1650 */   129,  130,  131,  132,  155,  155,  155,  155,  155,  155,
 /*  1660 */   155,  155,  100,  155,  155,  144,  145,    1,    2,    3,
 /*  1670 */     4,    5,  155,  111,  155,  155,  155,  155,  116,  117,
 /*  1680 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*  1690 */   128,  129,  130,  131,  132,  155,  155,  100,  155,  155,
 /*  1700 */    34,  155,   36,   37,  155,   39,  144,  145,  111,   43,
 /*  1710 */    44,  155,  155,  116,  117,  118,  119,  120,  121,  122,
 /*  1720 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1730 */   155,  155,  155,  100,  155,  155,  155,  155,  155,  155,
 /*  1740 */   155,  144,  145,  155,  111,  155,  155,  155,  155,  116,
 /*  1750 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1760 */   127,  128,  129,  130,  131,  132,  155,  155,  155,  155,
 /*  1770 */   155,  155,  155,  155,  100,  155,  155,  144,  145,    1,
 /*  1780 */     2,    3,    4,    5,  155,  111,  155,  155,  155,  155,
 /*  1790 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1800 */   126,  127,  128,  129,  130,  131,  132,  155,  155,  100,
 /*  1810 */   155,  155,   34,  155,   36,   37,  155,   39,  144,  145,
 /*  1820 */   111,   43,   44,  155,  155,  116,  117,  118,  119,  120,
 /*  1830 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1840 */   131,  132,  155,  155,  155,  100,  155,  155,  155,  155,
 /*  1850 */   155,  155,  155,  144,  145,  155,  111,  155,  155,  155,
 /*  1860 */   155,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1870 */   125,  126,  127,  128,  129,  130,  131,  132,  155,  155,
 /*  1880 */   155,  155,  155,  155,  155,  155,  100,  155,  155,  144,
 /*  1890 */   145,    1,    2,    3,    4,    5,  155,  111,  155,  155,
 /*  1900 */   155,  155,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1910 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  100,
 /*  1920 */   155,  155,  155,  155,   34,  155,   36,   37,  155,   39,
 /*  1930 */   144,  145,  155,   43,   44,  116,  117,  118,  119,  120,
 /*  1940 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1950 */   131,  132,  155,   22,   23,   24,   25,   26,   27,   28,
 /*  1960 */   155,   30,   11,  144,  145,  155,  155,  155,  155,  155,
 /*  1970 */   155,   40,  155,   42,   22,   23,   24,   25,   26,   27,
 /*  1980 */    28,   30,   30,    1,    2,    3,    4,    5,  155,  155,
 /*  1990 */   155,  155,   40,  155,   42,  155,   45,   46,   47,   48,
 /*  2000 */    49,   50,   51,   52,   53,   54,   55,  155,   77,  155,
 /*  2010 */   155,  155,  155,  155,  155,  155,   85,   86,   36,   88,
 /*  2020 */    89,   90,   91,   92,  155,   43,   44,  155,  155,   77,
 /*  2030 */   155,  155,  155,  155,  155,  155,  155,   85,   86,  155,
 /*  2040 */    88,   89,   90,   91,   92,   22,   23,   24,   25,   26,
 /*  2050 */    27,   28,  155,   30,  155,   38,  155,  155,  155,  155,
 /*  2060 */   155,  155,  155,   40,  155,   42,   22,   23,   24,   25,
 /*  2070 */    26,   27,  155,  155,   30,  155,  155,  155,  155,  155,
 /*  2080 */   155,  155,  155,  155,   40,  155,   42,  155,   71,   72,
 /*  2090 */    73,   74,   75,   76,   77,  155,   79,   80,   81,  155,
 /*  2100 */    77,  155,  155,  155,  155,  155,  155,  155,   85,   86,
 /*  2110 */   155,   88,   89,   90,   91,   92,  155,  155,  155,  155,
 /*  2120 */   155,   77,   78,   22,   23,   24,   25,   26,   27,   85,
 /*  2130 */    86,   30,   88,   89,   90,   91,   92,  155,  155,  155,
 /*  2140 */   155,   40,  155,   42,  155,  155,  155,   22,   23,   24,
 /*  2150 */    25,   26,   27,  155,  155,   30,  155,  155,  155,  155,
 /*  2160 */   155,  155,  155,  155,  155,   40,  155,   42,  155,  101,
 /*  2170 */   102,  103,  104,  105,  106,  155,  108,  109,   77,   78,
 /*  2180 */   112,  155,  155,  155,  155,  155,   85,   86,  155,   88,
 /*  2190 */    89,   90,   91,   92,   69,  155,  101,  102,  103,  104,
 /*  2200 */   105,  106,   77,  108,  109,  155,  155,  112,  155,  155,
 /*  2210 */    85,   86,  155,   88,   89,   90,   91,   92,  155,  155,
 /*  2220 */   155,  155,  155,  155,  155,   22,   23,   24,   25,   26,
 /*  2230 */    27,  155,  155,   30,  155,    1,    2,    3,    4,    5,
 /*  2240 */   155,  155,  155,   40,  155,   42,  155,  155,  155,   22,
 /*  2250 */    23,   24,   25,   26,   27,  155,  155,   30,  155,  155,
 /*  2260 */   155,  155,  155,  155,  155,  155,  155,   40,   34,   42,
 /*  2270 */    36,   37,  155,   39,  155,  155,  155,   43,   44,  155,
 /*  2280 */    77,   78,  155,  155,  155,  155,  155,  155,   85,   86,
 /*  2290 */   155,   88,   89,   90,   91,   92,   69,  155,  101,  102,
 /*  2300 */   103,  104,  105,  106,   77,  108,  109,  155,  155,  112,
 /*  2310 */   155,  155,   85,   86,  155,   88,   89,   90,   91,   92,
 /*  2320 */   155,    1,    2,    3,    4,    5,  155,   22,   23,   24,
 /*  2330 */    25,   26,   27,   28,  155,   30,  155,    1,    2,    3,
 /*  2340 */     4,    5,  155,  155,  155,   40,  155,   42,  155,  155,
 /*  2350 */    22,   23,   24,   25,   26,   27,   36,  155,   30,   39,
 /*  2360 */   155,  155,  155,   43,   44,  155,   30,  155,   40,  155,
 /*  2370 */    42,  155,   36,  155,  155,  155,  155,   41,  155,   43,
 /*  2380 */    44,  155,   77,  155,  155,  155,  155,  155,  155,  155,
 /*  2390 */    85,   86,  155,   88,   89,   90,   91,   92,  155,  155,
 /*  2400 */   155,  155,  155,  155,  155,   77,  155,    1,    2,    3,
 /*  2410 */     4,    5,  155,   85,   86,  155,   88,   89,   90,   91,
 /*  2420 */    92,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2430 */    16,   17,   18,   19,   20,   21,  155,  155,  155,  155,
 /*  2440 */    34,  155,   36,   37,  155,   31,  155,  155,  155,   43,
 /*  2450 */    44,  155,  155,    7,    8,    9,   10,   11,   12,   13,
 /*  2460 */    14,   15,   16,   17,   18,   19,   20,   21,  155,  155,
 /*  2470 */   101,  102,  103,  104,  105,  106,  155,  108,  109,   33,
 /*  2480 */   155,  112,  155,  155,  155,  155,  155,    7,    8,    9,
 /*  2490 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2500 */    20,   21,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2510 */    15,   16,   17,   18,   19,   20,   21,  155,  155,  155,
 /*  2520 */   155,  155,  155,  155,  155,  155,  155,  155,   33,  155,
 /*  2530 */   155,  155,  155,  155,  155,  155,    7,    8,    9,   10,
 /*  2540 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2550 */    21,  155,    1,    2,    3,    4,    5,   28,   78,  155,
 /*  2560 */   155,  155,  155,  155,  155,    7,    8,    9,   10,   11,
 /*  2570 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2580 */   155,   30,  155,  155,  155,  155,  155,   36,  155,  155,
 /*  2590 */   155,   33,   41,  155,   43,   44,  155,  155,  155,    7,
 /*  2600 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2610 */    18,   19,   20,   21,   11,  155,  155,  155,  155,  155,
 /*  2620 */   155,  155,  155,  155,  155,   11,  155,  155,  155,  155,
 /*  2630 */   155,   28,  155,   30,  155,  155,  155,  155,  155,  155,
 /*  2640 */   155,  155,   28,  155,   30,  155,  155,  155,   45,   46,
 /*  2650 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   45,
 /*  2660 */    46,   47,   48,   49,   50,   51,   52,   53,   54,   55,
 /*  2670 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2680 */    19,   20,   21,    1,    2,    3,    4,    5,    1,    2,
 /*  2690 */     3,    4,    5,  155,  155,  155,  155,  155,  155,  155,
 /*  2700 */   155,  155,  155,  155,  155,  155,  155,  155,  155,  155,
 /*  2710 */   155,  155,  155,  155,  155,  155,   34,  155,   36,   37,
 /*  2720 */   155,   34,  155,   36,   37,   43,   44,  155,  155,  155,
 /*  2730 */    43,   44,    1,    2,    3,    4,    5,    1,    2,    3,
 /*  2740 */     4,    5,  155,  155,  155,  155,  155,  155,  155,  155,
 /*  2750 */   155,  155,  155,  155,  155,  155,  155,  155,  155,  155,
 /*  2760 */   155,  155,  155,  155,  155,   34,  155,   36,   37,  155,
 /*  2770 */    34,  155,   36,   37,   43,   44,  155,  155,  155,   43,
 /*  2780 */    44,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  2790 */     5,  155,    1,    2,    3,    4,    5,  155,  155,  155,
 /*  2800 */   155,  155,  155,  155,  155,  155,  155,  155,  155,  155,
 /*  2810 */   155,  155,  155,  155,   34,  155,   36,   37,  155,  155,
 /*  2820 */   155,   36,  155,   43,   44,  155,   41,   36,   43,   44,
 /*  2830 */   155,  155,   41,  155,   43,   44,
};
#define YY_SHIFT_USE_DFLT (-25)
static short yy_shift_ofst[] = {
 /*     0 */    77,   14, 1303,  -25,  -25,  -25,  -25,   19,   41,  -25,
 /*    10 */   127,  301, 1554,  147,  -25,  -25, 1890, 2406,  -25, 2785,
 /*    20 */   204,  262, 2614,  209,  224,  -25,  275,  -25,  -25,  -25,
 /*    30 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*    40 */   -25,  -25,  -25,  -25,  -25,  -25,  254,  -25,  285,  -25,
 /*    50 */  2328,  286,  836,  887,  307,  352,  938,  989,  -25, 2328,
 /*    60 */   354,  558,  -25,  456,  -25,  -25,  258,  408, 1241, 1241,
 /*    70 */   454,  122,   28,  -25, 2328,  464, 1091, 1040,  -25,  472,
 /*    80 */   785,  734,  455, 2328,  520,  -25, 2328,  -25, 2328,  523,
 /*    90 */   -25,  478,  524,  492,  -25,  506,  -25,  511,  -25,  196,
 /*   100 */  2328,  196, 2328,  196, 2328,  196, 2328,  196, 2328,  196,
 /*   110 */  2328,  196, 2328,  196, 2328,  278, 2328,  278, 2328,  -25,
 /*   120 */  2328,  -25, 2328,  278, 2328, 2661, 2328, 2661,  -22,  -25,
 /*   130 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   140 */  2529,  -25,  594, 2328,  -25,  325,  612,  598, 1931, 2592,
 /*   150 */  2328,  -25,  623,  651,  523,  -25, 2328,  -25,  -25,  -25,
 /*   160 */   -25,  -25,  -25,  -25,  -25,  -25, 1224,  -25,  358,  -25,
 /*   170 */   109,  -25,  629,  211,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   180 */   -25,  -25,  -25,  -25,  631,  642,  646,  -25,  644,  679,
 /*   190 */  2305,  -25,  767,  -25, 2328,  -25,  -25,  -25,  -25,  818,
 /*   200 */   -25,  -25,   59,  700, 1952,  848,  -25,  -25,  -25,  167,
 /*   210 */   647,  -25,  681,  -25,  694,  -25, 2023,  289,  -25,  -25,
 /*   220 */  2592,  744,   81, 2125, 2495, 1241,  173,  -25,  745,  942,
 /*   230 */   -25,  745,  -25, 2017,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   240 */  2328,  -25, 2592,  776,  -21, 2328,  -25, 2044,  280, 2328,
 /*   250 */   -25,  730,  280, 2328,  -25, 2480,  793,  174, 2328,  -25,
 /*   260 */  2203,  280, 2328,  -25,  749,  280, 2328,  -25, 2101,  280,
 /*   270 */  2328,  -25,  780,  280, 2328,  -25,  -25,  -25,  -25, 2328,
 /*   280 */   490,  -25, 2328,  -25, 2592,  -25,  828,  -25,  832,  -25,
 /*   290 */  2328, 2414,  -25,  844,  329,  845,  846,  -25,  422,  800,
 /*   300 */   864,  -25,  827,  866,  -25,  830,  878,  -25,  368,  879,
 /*   310 */   -25,  881,  872, 2328,  340,  894,  -25,  896,  534,  -25,
 /*   320 */   896,  -25,  890,  258,  -25,  -25,  896,   65,  -25,  896,
 /*   330 */   993,  -25,  896,  986,  -25,  896,  976,  -25,  896,  970,
 /*   340 */   -25,  896,  950,  -25,  896,  101,  -25,  896,  568,  -25,
 /*   350 */   896,  592,  -25,  117,  916,  700,  928,  598,  -25,  -25,
 /*   360 */  2328, 2446, 1241, 1193,  -25,  929,  892, 2227,   27, 1241,
 /*   370 */  1142,  -25, 2328, 2558, 1241,   71,  -25,  897, 2328,  930,
 /*   380 */   -25,  -25,  -25,  903, 1241, 1241,  -25,  -25,  907,  934,
 /*   390 */   683,  632,  -25,  -25,  716,  944,  581,  -25,  530,  -25,
 /*   400 */  1951,  -25,  940,  258,  -25,  949,  959,  258,  -25,  979,
 /*   410 */   968,  258,  -25,  980, 1000,  983,  -25,  -25,  -25,  -25,
 /*   420 */   -25,  -25,  -25,  -25,  -25,  -25, 1982, 2791,  984,  973,
 /*   430 */  2603,  985,  479,  -25,  428,  -25,  563,  -24,  377,  -25,
 /*   440 */   326,  -25,  -25, 1778, 2682,  -25,  -25, 2320,  987,  978,
 /*   450 */   258,  998,  -25,  990,  994,  258, 1002,  -25,  -25, 2320,
 /*   460 */  2551,  451,  -25,  258, 1017,  -25, 2336,  407,  -25,  258,
 /*   470 */  1020,  -25,  -25, 1666, 2780, 2736, 1982, 2731,  -25, 2687,
 /*   480 */  1022, 1024, 1442, 1026,  -25,  -25, 1029, 1028,   26, 1230,
 /*   490 */  1030,  -25,  -25, 1032, 1033, 2234, 1031,  -25,  -25,  -25,
};
#define YY_REDUCE_USE_DFLT (-142)
static short yy_reduce_ofst[] = {
 /*     0 */    66, -142,  507, -142, -142, -142, -142, -142, -142, -142,
 /*    10 */  -142, -142, 2369, -142, -142, -142,  -93,   39, -142,   99,
 /*    20 */  -142, -142,  458, -142, 1261, -142, 1819, -142, -142, -142,
 /*    30 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    40 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    50 */   669, -142, 1709, 1819, -142, -142, 1485, 1819, -142,  414,
 /*    60 */  -142, -113, -142,  316, -142, -142,  308, -142, 1562, 1819,
 /*    70 */  -142, 1786, 1819, -142,  567, -142, 1338, 1819, -142, -142,
 /*    80 */  1633, 1819, -142,  618, -142, -142,  582, -142,  805,  389,
 /*    90 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   100 */   786, -142,  616, -142,  310, -142,  667, -142,  754, -142,
 /*   110 */   193, -142,  499, -142,  601, -142,  684, -142,  720, -142,
 /*   120 */   771, -142,  819, -142,  295, -142,  397, -142, 1407, -142,
 /*   130 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   140 */  -142, -142, -142,  703, -142, -142, -142, -142, 1482, -142,
 /*   150 */   550, -142, -142, -142,  521, -142,  244, -142, -142, -142,
 /*   160 */  -142, -142, -142, -142, -142, -142,  -50, -142, -142, -142,
 /*   170 */   350, -142, -142,  312, -142, -142, -142, -142, -142, -142,
 /*   180 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   190 */   457, -142, -142, -142,  -54, -142, -142, -142, -142, -142,
 /*   200 */  -142, -142, -142, -142, 1176, -142, -142, -142, -142, -142,
 /*   210 */  -142, -142, -142, -142, -142, -142, 1258, -142, -142, -142,
 /*   220 */  -142, -142, -142,  717, -142, 1373, 1819, -142,  155, -142,
 /*   230 */  -142,  638, -142,  654, -142, -142, -142, -142, -142, -142,
 /*   240 */   276, -142, -142, -142,  668,  565, -142,   91,  670,  412,
 /*   250 */  -142, -142,  671,  327, -142, -142, -142,  686,  225, -142,
 /*   260 */   142,  687,  514, -142, -142,  705,  378, -142,  -11,  718,
 /*   270 */   480, -142, -142,  721,  531, -142, -142, -142, -142,  123,
 /*   280 */  -142, -142,   40, -142, -142, -142, -142, -142, -142, -142,
 /*   290 */   735, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   300 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   310 */  -142, -142, -142,  355, -142, -142, -142,   43, -142, -142,
 /*   320 */   779, -142, -142,  822, -142, -142,  610, -142, -142,  608,
 /*   330 */  -142, -142,  576, -142, -142,  559, -142, -142,  525, -142,
 /*   340 */  -142,  321, -142, -142,  508, -142, -142, -141, -142, -142,
 /*   350 */   270, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   360 */   633, -142, 1745, 1819, -142, -142, -142,  768, -142, 1521,
 /*   370 */  1819, -142,  652, -142, 1450, 1819, -142, -142,  516, -142,
 /*   380 */  -142, -142, -142, -142, 1409, 1819, -142, -142, -142, -142,
 /*   390 */  1297, 1819, -142, -142, -142, -142, 1674, -142, 1819, -142,
 /*   400 */   149, -142, -142,  873, -142, -142, -142,  893, -142, -142,
 /*   410 */  -142,  904, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   420 */  -142, -142, -142, -142, -142, -142,   96,   99, -142, -142,
 /*   430 */   200, -142, 1226, -142, 1819, -142, -142, -142, 1597, -142,
 /*   440 */  1819, -142, -142,  291,   39, -142, -142,   96, -142, -142,
 /*   450 */   921, -142, -142, -142, -142,  924, -142, -142, -142,  138,
 /*   460 */    99, -142, -142,  936, -142, -142,   99, -142, -142,  943,
 /*   470 */  -142, -142, -142,   38,  344,   39,  138,   39, -142,   39,
 /*   480 */  -142, -142, 2197, -142, -142, -142, -142, -142, -142, 2095,
 /*   490 */  -142, -142, -142, -142, -142, 2068, -142, -142, -142, -142,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   740,  740,  740,  502,  504,  505,  506,  740,  740,  507,
 /*    10 */   740,  740,  740,  740,  508,  512,  740,  740,  533,  740,
 /*    20 */   740,  740,  740,  740,  740,  537,  740,  539,  575,  577,
 /*    30 */   578,  579,  580,  581,  582,  583,  584,  585,  586,  587,
 /*    40 */   588,  589,  590,  591,  592,  593,  740,  594,  740,  595,
 /*    50 */   740,  740,  740,  740,  598,  740,  740,  740,  599,  740,
 /*    60 */   740,  740,  602,  740,  603,  604,  740,  740,  740,  606,
 /*    70 */   740,  740,  740,  609,  740,  740,  740,  740,  611,  740,
 /*    80 */   740,  740,  740,  740,  740,  613,  740,  667,  740,  740,
 /*    90 */   668,  740,  740,  740,  735,  740,  736,  740,  737,  669,
 /*   100 */   740,  670,  740,  671,  740,  672,  740,  673,  740,  674,
 /*   110 */   740,  675,  740,  676,  740,  685,  740,  686,  740,  687,
 /*   120 */   740,  688,  740,  689,  740,  690,  740,  691,  740,  564,
 /*   130 */   565,  566,  567,  568,  569,  570,  571,  572,  573,  574,
 /*   140 */   740,  677,  740,  740,  678,  696,  740,  679,  740,  719,
 /*   150 */   740,  692,  740,  740,  740,  693,  740,  694,  695,  697,
 /*   160 */   698,  699,  700,  701,  702,  703,  740,  704,  740,  705,
 /*   170 */   740,  720,  740,  740,  722,  727,  728,  729,  730,  731,
 /*   180 */   732,  733,  734,  723,  728,  730,  729,  721,  740,  706,
 /*   190 */   740,  707,  740,  708,  740,  711,  712,  717,  718,  740,
 /*   200 */   715,  716,  740,  680,  740,  740,  713,  714,  681,  740,
 /*   210 */   740,  682,  740,  683,  740,  684,  740,  740,  709,  710,
 /*   220 */   738,  740,  740,  740,  740,  740,  740,  614,  740,  740,
 /*   230 */   618,  740,  619,  740,  621,  622,  623,  624,  625,  626,
 /*   240 */   740,  627,  666,  740,  740,  740,  628,  740,  740,  740,
 /*   250 */   629,  740,  740,  740,  630,  740,  740,  740,  740,  631,
 /*   260 */   740,  740,  740,  632,  740,  740,  740,  633,  740,  740,
 /*   270 */   740,  634,  740,  740,  740,  635,  636,  637,  620,  740,
 /*   280 */   740,  639,  740,  640,  642,  641,  740,  643,  740,  644,
 /*   290 */   740,  740,  645,  740,  740,  740,  740,  646,  740,  740,
 /*   300 */   740,  647,  740,  740,  648,  740,  740,  649,  740,  740,
 /*   310 */   650,  740,  740,  740,  740,  740,  651,  740,  740,  652,
 /*   320 */   740,  662,  664,  740,  665,  663,  740,  740,  653,  740,
 /*   330 */   740,  654,  740,  740,  655,  740,  740,  656,  740,  740,
 /*   340 */   657,  740,  740,  658,  740,  740,  659,  740,  740,  660,
 /*   350 */   740,  740,  661,  740,  740,  740,  740,  740,  739,  576,
 /*   360 */   740,  740,  740,  740,  615,  740,  740,  740,  740,  740,
 /*   370 */   740,  616,  740,  740,  740,  740,  617,  740,  740,  740,
 /*   380 */   612,  610,  608,  740,  740,  607,  605,  600,  596,  740,
 /*   390 */   740,  740,  601,  597,  740,  740,  740,  538,  740,  540,
 /*   400 */   740,  555,  557,  740,  560,  740,  558,  740,  561,  740,
 /*   410 */   559,  740,  562,  740,  740,  740,  556,  545,  547,  548,
 /*   420 */   549,  550,  551,  552,  553,  554,  740,  740,  740,  740,
 /*   430 */   740,  740,  740,  541,  740,  543,  740,  740,  740,  542,
 /*   440 */   740,  544,  546,  740,  740,  531,  534,  740,  740,  740,
 /*   450 */   740,  740,  535,  740,  740,  740,  740,  536,  525,  740,
 /*   460 */   740,  740,  527,  740,  740,  529,  740,  740,  528,  740,
 /*   470 */   740,  530,  532,  740,  740,  740,  740,  740,  526,  740,
 /*   480 */   740,  740,  740,  740,  509,  513,  740,  740,  740,  740,
 /*   490 */   740,  510,  514,  740,  740,  740,  740,  511,  515,  503,
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
  "MUL",           "DIV",           "ISSET",         "FETCH",       
  "TYPEOF",        "INSTANCEOF",    "NEW",           "NOT",         
  "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",  "EXTENDS",     
  "ABSTRACT",      "COMMENT",       "ASSIGN",        "CONST",       
  "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",  "INLINE",      
  "FINAL",         "TYPE_INTEGER",  "TYPE_UINTEGER",  "TYPE_LONG",   
  "TYPE_ULONG",    "TYPE_CHAR",     "TYPE_UCHAR",    "TYPE_DOUBLE", 
  "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",    "TYPE_VAR",    
  "BREAK",         "CONTINUE",      "IF",            "ELSE",        
  "SWITCH",        "CASE",          "COLON",         "DEFAULT",     
  "LOOP",          "WHILE",         "DO",            "FOR",         
  "IN",            "REVERSE",       "LET",           "ADDASSIGN",   
  "SUBASSIGN",     "MULASSIGN",     "DIVASSIGN",     "CONCATASSIGN",
  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "DOUBLECOLON", 
  "INCR",          "DECR",          "ECHO",          "RETURN",      
  "UNSET",         "INTEGER",       "STRING",        "THROW",       
  "SCHAR",         "DOUBLE",        "NULL",          "TRUE",        
  "FALSE",         "error",         "program",       "xx_language", 
  "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",  "xx_class_def",
  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",  "xx_literal_expr",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",  "xx_statement_list",
  "xx_visibility",  "xx_parameter",  "xx_parameter_type",  "xx_parameter_cast",
  "xx_statement",  "xx_let_statement",  "xx_if_statement",  "xx_loop_statement",
  "xx_echo_statement",  "xx_return_statement",  "xx_mcall_statement",  "xx_scall_statement",
  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",
  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",  "xx_switch_statement",
  "xx_for_statement",  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",
  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",
  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",
  "xx_mcall_expr",  "xx_scall_expr",  "xx_call_parameters",  "xx_declare_variable_list",
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
 /* 187 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 188 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 189 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 190 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 191 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 192 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 193 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 194 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 195 */ "xx_common_expr ::= CONSTANT",
 /* 196 */ "xx_common_expr ::= IDENTIFIER",
 /* 197 */ "xx_common_expr ::= INTEGER",
 /* 198 */ "xx_common_expr ::= STRING",
 /* 199 */ "xx_common_expr ::= SCHAR",
 /* 200 */ "xx_common_expr ::= DOUBLE",
 /* 201 */ "xx_common_expr ::= NULL",
 /* 202 */ "xx_common_expr ::= TRUE",
 /* 203 */ "xx_common_expr ::= FALSE",
 /* 204 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 205 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 206 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 207 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 208 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 209 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 210 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 211 */ "xx_common_expr ::= xx_mcall_expr",
 /* 212 */ "xx_common_expr ::= xx_scall_expr",
 /* 213 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 214 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 215 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 216 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 217 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 218 */ "xx_call_parameters ::= xx_call_parameter",
 /* 219 */ "xx_call_parameter ::= xx_common_expr",
 /* 220 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 221 */ "xx_array_list ::= xx_array_item",
 /* 222 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 223 */ "xx_array_item ::= xx_array_value",
 /* 224 */ "xx_array_key ::= IDENTIFIER",
 /* 225 */ "xx_array_key ::= STRING",
 /* 226 */ "xx_array_key ::= INTEGER",
 /* 227 */ "xx_array_value ::= xx_literal_expr",
 /* 228 */ "xx_array_value ::= IDENTIFIER",
 /* 229 */ "xx_literal_expr ::= INTEGER",
 /* 230 */ "xx_literal_expr ::= STRING",
 /* 231 */ "xx_literal_expr ::= DOUBLE",
 /* 232 */ "xx_literal_expr ::= NULL",
 /* 233 */ "xx_literal_expr ::= FALSE",
 /* 234 */ "xx_literal_expr ::= TRUE",
 /* 235 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 236 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 237 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 238 */ "xx_eval_expr ::= xx_common_expr",
 /* 239 */ "xx_comment ::= COMMENT",
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
    case 91:
    case 92:
// 836 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2141 "parser.c"
      break;
    case 95:
// 849 "parser.lemon"
{ json_object_put((yypminor->yy175)); }
// 2146 "parser.c"
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
  { 94, 1 },
  { 95, 1 },
  { 96, 2 },
  { 96, 1 },
  { 97, 1 },
  { 97, 1 },
  { 97, 1 },
  { 98, 3 },
  { 99, 5 },
  { 99, 7 },
  { 99, 6 },
  { 99, 8 },
  { 99, 4 },
  { 99, 6 },
  { 99, 5 },
  { 99, 7 },
  { 101, 1 },
  { 101, 1 },
  { 101, 1 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 2 },
  { 101, 3 },
  { 101, 3 },
  { 102, 2 },
  { 102, 1 },
  { 105, 4 },
  { 105, 3 },
  { 105, 6 },
  { 105, 5 },
  { 103, 2 },
  { 103, 1 },
  { 104, 2 },
  { 104, 1 },
  { 108, 6 },
  { 108, 5 },
  { 109, 7 },
  { 109, 8 },
  { 109, 8 },
  { 109, 9 },
  { 109, 8 },
  { 109, 9 },
  { 109, 9 },
  { 109, 10 },
  { 106, 2 },
  { 106, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 112, 1 },
  { 110, 3 },
  { 110, 1 },
  { 113, 1 },
  { 113, 2 },
  { 113, 2 },
  { 113, 3 },
  { 113, 4 },
  { 113, 4 },
  { 115, 3 },
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
  { 111, 2 },
  { 111, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 116, 1 },
  { 127, 2 },
  { 128, 2 },
  { 118, 4 },
  { 118, 7 },
  { 118, 5 },
  { 118, 9 },
  { 118, 8 },
  { 118, 8 },
  { 131, 4 },
  { 131, 5 },
  { 134, 2 },
  { 134, 1 },
  { 135, 4 },
  { 135, 3 },
  { 119, 3 },
  { 119, 4 },
  { 129, 4 },
  { 129, 5 },
  { 130, 6 },
  { 130, 7 },
  { 132, 7 },
  { 132, 8 },
  { 132, 9 },
  { 132, 10 },
  { 117, 3 },
  { 137, 3 },
  { 137, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 138, 3 },
  { 138, 5 },
  { 138, 7 },
  { 138, 8 },
  { 138, 5 },
  { 138, 7 },
  { 138, 8 },
  { 138, 5 },
  { 138, 6 },
  { 138, 2 },
  { 138, 2 },
  { 141, 1 },
  { 120, 3 },
  { 142, 3 },
  { 142, 1 },
  { 143, 1 },
  { 122, 2 },
  { 123, 2 },
  { 121, 3 },
  { 124, 5 },
  { 124, 6 },
  { 124, 6 },
  { 124, 6 },
  { 125, 3 },
  { 125, 7 },
  { 126, 3 },
  { 126, 3 },
  { 126, 3 },
  { 126, 3 },
  { 126, 3 },
  { 126, 3 },
  { 126, 3 },
  { 126, 3 },
  { 126, 3 },
  { 126, 3 },
  { 147, 3 },
  { 147, 1 },
  { 148, 1 },
  { 148, 3 },
  { 140, 1 },
  { 136, 2 },
  { 136, 2 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 4 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 4 },
  { 136, 4 },
  { 136, 4 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 3 },
  { 136, 2 },
  { 136, 4 },
  { 136, 2 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 1 },
  { 136, 2 },
  { 136, 3 },
  { 136, 2 },
  { 136, 4 },
  { 136, 5 },
  { 136, 4 },
  { 136, 3 },
  { 136, 1 },
  { 136, 1 },
  { 145, 6 },
  { 145, 5 },
  { 144, 6 },
  { 144, 5 },
  { 146, 3 },
  { 146, 1 },
  { 151, 1 },
  { 150, 3 },
  { 150, 1 },
  { 152, 3 },
  { 152, 1 },
  { 153, 1 },
  { 153, 1 },
  { 153, 1 },
  { 154, 1 },
  { 154, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 107, 1 },
  { 149, 4 },
  { 149, 4 },
  { 149, 4 },
  { 133, 1 },
  { 100, 1 },
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
// 845 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy175;
}
// 2603 "parser.c"
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
      case 211:
      case 212:
      case 219:
      case 227:
      case 238:
// 851 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;
}
// 2637 "parser.c"
        break;
      case 2:
      case 25:
      case 31:
      case 33:
      case 45:
      case 75:
      case 104:
// 855 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-1].minor.yy175, yymsp[0].minor.yy175);
}
// 2650 "parser.c"
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
      case 218:
      case 221:
// 859 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(NULL, yymsp[0].minor.yy175);
}
// 2669 "parser.c"
        break;
      case 7:
// 875 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2678 "parser.c"
        break;
      case 8:
// 879 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 0, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2688 "parser.c"
        break;
      case 9:
// 883 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy175, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2699 "parser.c"
        break;
      case 10:
// 887 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, 1, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2710 "parser.c"
        break;
      case 11:
// 891 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy175, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-7].minor);
  yy_destructor(32,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2722 "parser.c"
        break;
      case 12:
// 895 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2732 "parser.c"
        break;
      case 13:
// 899 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2743 "parser.c"
        break;
      case 14:
// 903 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2754 "parser.c"
        break;
      case 15:
// 907 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(32,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2766 "parser.c"
        break;
      case 16:
// 911 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
}
// 2773 "parser.c"
        break;
      case 17:
// 915 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2780 "parser.c"
        break;
      case 18:
// 919 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 2787 "parser.c"
        break;
      case 19:
// 923 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
}
// 2794 "parser.c"
        break;
      case 20:
// 927 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 2801 "parser.c"
        break;
      case 21:
// 931 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[0].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2808 "parser.c"
        break;
      case 22:
// 935 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(NULL, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2815 "parser.c"
        break;
      case 23:
// 939 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
}
// 2822 "parser.c"
        break;
      case 24:
// 943 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_definition(yymsp[-1].minor.yy175, yymsp[0].minor.yy175, yymsp[-2].minor.yy175, status->scanner_state);
}
// 2829 "parser.c"
        break;
      case 27:
// 955 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(31,&yymsp[0].minor);
}
// 2837 "parser.c"
        break;
      case 28:
// 959 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-2].minor.yy175, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[0].minor);
}
// 2845 "parser.c"
        break;
      case 29:
// 963 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2854 "parser.c"
        break;
      case 30:
// 967 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_property(yymsp[-4].minor.yy175, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2863 "parser.c"
        break;
      case 35:
// 987 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2873 "parser.c"
        break;
      case 36:
// 991 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 2883 "parser.c"
        break;
      case 37:
// 995 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2895 "parser.c"
        break;
      case 38:
// 999 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2907 "parser.c"
        break;
      case 39:
// 1003 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2919 "parser.c"
        break;
      case 40:
// 1007 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2931 "parser.c"
        break;
      case 41:
// 1011 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-6].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2943 "parser.c"
        break;
      case 42:
// 1015 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2955 "parser.c"
        break;
      case 43:
// 1019 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-7].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy175, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(41,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2967 "parser.c"
        break;
      case 44:
// 1023 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_class_method(yymsp[-8].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy175, yymsp[-1].minor.yy175, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(41,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 2979 "parser.c"
        break;
      case 47:
// 1035 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2987 "parser.c"
        break;
      case 48:
// 1039 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2995 "parser.c"
        break;
      case 49:
// 1043 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3003 "parser.c"
        break;
      case 50:
// 1047 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3011 "parser.c"
        break;
      case 51:
// 1051 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3019 "parser.c"
        break;
      case 52:
// 1055 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("inline");
  yy_destructor(43,&yymsp[0].minor);
}
// 3027 "parser.c"
        break;
      case 53:
// 1059 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("abstract");
  yy_destructor(36,&yymsp[0].minor);
}
// 3035 "parser.c"
        break;
      case 54:
// 1063 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("final");
  yy_destructor(44,&yymsp[0].minor);
}
// 3043 "parser.c"
        break;
      case 55:
      case 119:
      case 140:
      case 162:
      case 217:
      case 220:
// 1067 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_list(yymsp[-2].minor.yy175, yymsp[0].minor.yy175);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3056 "parser.c"
        break;
      case 57:
// 1075 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3063 "parser.c"
        break;
      case 58:
// 1079 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-1].minor.yy175, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3070 "parser.c"
        break;
      case 59:
// 1083 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-1].minor.yy175, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3077 "parser.c"
        break;
      case 60:
// 1087 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 3085 "parser.c"
        break;
      case 61:
// 1091 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(yymsp[-3].minor.yy175, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 3093 "parser.c"
        break;
      case 62:
// 1095 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_parameter(NULL, yymsp[-3].minor.yy175, yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 3101 "parser.c"
        break;
      case 63:
// 1099 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 3110 "parser.c"
        break;
      case 64:
// 1103 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(45,&yymsp[0].minor);
}
// 3118 "parser.c"
        break;
      case 65:
// 1107 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(46,&yymsp[0].minor);
}
// 3126 "parser.c"
        break;
      case 66:
// 1111 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(47,&yymsp[0].minor);
}
// 3134 "parser.c"
        break;
      case 67:
// 1115 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(48,&yymsp[0].minor);
}
// 3142 "parser.c"
        break;
      case 68:
// 1119 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(49,&yymsp[0].minor);
}
// 3150 "parser.c"
        break;
      case 69:
// 1123 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(50,&yymsp[0].minor);
}
// 3158 "parser.c"
        break;
      case 70:
// 1127 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(51,&yymsp[0].minor);
}
// 3166 "parser.c"
        break;
      case 71:
// 1131 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(52,&yymsp[0].minor);
}
// 3174 "parser.c"
        break;
      case 72:
// 1135 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(53,&yymsp[0].minor);
}
// 3182 "parser.c"
        break;
      case 73:
// 1139 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(54,&yymsp[0].minor);
}
// 3190 "parser.c"
        break;
      case 74:
// 1143 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(55,&yymsp[0].minor);
}
// 3198 "parser.c"
        break;
      case 94:
// 1223 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3207 "parser.c"
        break;
      case 95:
// 1227 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3216 "parser.c"
        break;
      case 96:
// 1231 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-2].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3226 "parser.c"
        break;
      case 97:
// 1235 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-5].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3239 "parser.c"
        break;
      case 98:
// 1239 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3249 "parser.c"
        break;
      case 99:
// 1243 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-7].minor.yy175, yymsp[-5].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(58,&yymsp[-8].minor);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3262 "parser.c"
        break;
      case 100:
// 1247 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, yymsp[-4].minor.yy175, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3275 "parser.c"
        break;
      case 101:
// 1251 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_if_statement(yymsp[-6].minor.yy175, NULL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(58,&yymsp[-7].minor);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3288 "parser.c"
        break;
      case 102:
// 1255 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3298 "parser.c"
        break;
      case 103:
// 1259 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_switch_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(60,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3308 "parser.c"
        break;
      case 106:
// 1271 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(62,&yymsp[-1].minor);
}
// 3317 "parser.c"
        break;
      case 107:
// 1275 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_case_clause(NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(62,&yymsp[-1].minor);
}
// 3326 "parser.c"
        break;
      case 108:
// 1279 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3336 "parser.c"
        break;
      case 109:
// 1283 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_loop_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3346 "parser.c"
        break;
      case 110:
// 1287 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-2].minor.yy175, NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-1].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3356 "parser.c"
        break;
      case 111:
// 1291 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_while_statement(yymsp[-3].minor.yy175, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(65,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3366 "parser.c"
        break;
      case 112:
// 1295 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3378 "parser.c"
        break;
      case 113:
// 1299 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_do_while_statement(yymsp[-1].minor.yy175, yymsp[-4].minor.yy175, status->scanner_state);
  yy_destructor(66,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-3].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3390 "parser.c"
        break;
      case 114:
// 1303 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-6].minor);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3401 "parser.c"
        break;
      case 115:
// 1307 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-7].minor);
  yy_destructor(68,&yymsp[-5].minor);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3413 "parser.c"
        break;
      case 116:
// 1311 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(68,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3425 "parser.c"
        break;
      case 117:
// 1315 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_for_statement(yymsp[-3].minor.yy175, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(67,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(68,&yymsp[-5].minor);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[0].minor);
}
// 3438 "parser.c"
        break;
      case 118:
// 1319 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3447 "parser.c"
        break;
      case 121:
// 1332 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("assign");
  yy_destructor(38,&yymsp[0].minor);
}
// 3455 "parser.c"
        break;
      case 122:
// 1337 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("add-assign");
  yy_destructor(71,&yymsp[0].minor);
}
// 3463 "parser.c"
        break;
      case 123:
// 1342 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("sub-assign");
  yy_destructor(72,&yymsp[0].minor);
}
// 3471 "parser.c"
        break;
      case 124:
// 1346 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("mult-assign");
  yy_destructor(73,&yymsp[0].minor);
}
// 3479 "parser.c"
        break;
      case 125:
// 1350 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("div-assign");
  yy_destructor(74,&yymsp[0].minor);
}
// 3487 "parser.c"
        break;
      case 126:
// 1354 "parser.lemon"
{
	yygotominor.yy175 = json_object_new_string("concat-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3495 "parser.c"
        break;
      case 127:
// 1359 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy175, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 3502 "parser.c"
        break;
      case 128:
// 1364 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(76,&yymsp[-3].minor);
}
// 3510 "parser.c"
        break;
      case 129:
// 1369 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(76,&yymsp[-5].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(78,&yymsp[-2].minor);
}
// 3520 "parser.c"
        break;
      case 130:
// 1374 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(76,&yymsp[-6].minor);
  yy_destructor(77,&yymsp[-4].minor);
  yy_destructor(78,&yymsp[-2].minor);
}
// 3530 "parser.c"
        break;
      case 131:
// 1379 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
}
// 3538 "parser.c"
        break;
      case 132:
// 1384 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy175, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(79,&yymsp[-5].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(78,&yymsp[-2].minor);
}
// 3548 "parser.c"
        break;
      case 133:
// 1389 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy175, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(79,&yymsp[-6].minor);
  yy_destructor(77,&yymsp[-4].minor);
  yy_destructor(78,&yymsp[-2].minor);
}
// 3558 "parser.c"
        break;
      case 134:
// 1394 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy175, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(78,&yymsp[-2].minor);
}
// 3567 "parser.c"
        break;
      case 135:
// 1399 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy175, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(77,&yymsp[-4].minor);
  yy_destructor(78,&yymsp[-2].minor);
}
// 3576 "parser.c"
        break;
      case 136:
// 1404 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[0].minor);
}
// 3584 "parser.c"
        break;
      case 137:
// 1409 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[0].minor);
}
// 3592 "parser.c"
        break;
      case 139:
// 1417 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_echo_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3601 "parser.c"
        break;
      case 142:
// 1429 "parser.lemon"
{
	yygotominor.yy175 = yymsp[0].minor.yy175;;
}
// 3608 "parser.c"
        break;
      case 143:
// 1434 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(31,&yymsp[0].minor);
}
// 3616 "parser.c"
        break;
      case 144:
// 1439 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(31,&yymsp[0].minor);
}
// 3624 "parser.c"
        break;
      case 145:
// 1444 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_return_statement(yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3633 "parser.c"
        break;
      case 146:
// 1449 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3647 "parser.c"
        break;
      case 147:
// 1458 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(84,&yymsp[-5].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(78,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3662 "parser.c"
        break;
      case 148:
// 1467 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(84,&yymsp[-5].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(78,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3677 "parser.c"
        break;
      case 149:
// 1476 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(84,&yymsp[-5].minor);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(78,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3692 "parser.c"
        break;
      case 150:
// 1485 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3701 "parser.c"
        break;
      case 151:
// 1490 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy175, status->scanner_state);
  yy_destructor(87,&yymsp[-6].minor);
  yy_destructor(26,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(28,&yymsp[-1].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3713 "parser.c"
        break;
      case 152:
// 1494 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3722 "parser.c"
        break;
      case 153:
// 1498 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3731 "parser.c"
        break;
      case 154:
// 1502 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3740 "parser.c"
        break;
      case 155:
// 1506 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3749 "parser.c"
        break;
      case 156:
// 1510 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3758 "parser.c"
        break;
      case 157:
// 1514 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3767 "parser.c"
        break;
      case 158:
// 1518 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3776 "parser.c"
        break;
      case 159:
// 1522 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3785 "parser.c"
        break;
      case 160:
// 1526 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3794 "parser.c"
        break;
      case 161:
// 1530 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(31,&yymsp[0].minor);
}
// 3803 "parser.c"
        break;
      case 164:
// 1542 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3810 "parser.c"
        break;
      case 165:
// 1546 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
}
// 3818 "parser.c"
        break;
      case 167:
// 1554 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 3826 "parser.c"
        break;
      case 168:
// 1558 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("isset", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3834 "parser.c"
        break;
      case 169:
// 1562 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3842 "parser.c"
        break;
      case 170:
// 1566 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-equals", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3850 "parser.c"
        break;
      case 171:
// 1570 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3858 "parser.c"
        break;
      case 172:
// 1574 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("not-identical", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3866 "parser.c"
        break;
      case 173:
// 1578 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3874 "parser.c"
        break;
      case 174:
// 1582 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3882 "parser.c"
        break;
      case 175:
// 1586 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("less-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3890 "parser.c"
        break;
      case 176:
// 1590 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3898 "parser.c"
        break;
      case 177:
// 1594 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("list", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 3907 "parser.c"
        break;
      case 178:
// 1598 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("cast", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-3].minor);
  yy_destructor(28,&yymsp[-1].minor);
}
// 3916 "parser.c"
        break;
      case 179:
// 1602 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-1].minor);
}
// 3924 "parser.c"
        break;
      case 180:
// 1606 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-1].minor);
}
// 3932 "parser.c"
        break;
      case 181:
// 1610 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-1].minor);
}
// 3940 "parser.c"
        break;
      case 182:
      case 235:
// 1614 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(78,&yymsp[0].minor);
}
// 3950 "parser.c"
        break;
      case 183:
      case 236:
// 1619 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(78,&yymsp[0].minor);
}
// 3960 "parser.c"
        break;
      case 184:
      case 237:
// 1624 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(78,&yymsp[0].minor);
}
// 3970 "parser.c"
        break;
      case 185:
// 1629 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("add", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3978 "parser.c"
        break;
      case 186:
// 1634 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("sub", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3986 "parser.c"
        break;
      case 187:
// 1639 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("mul", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 3994 "parser.c"
        break;
      case 188:
// 1644 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("div", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4002 "parser.c"
        break;
      case 189:
// 1649 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("concat", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4010 "parser.c"
        break;
      case 190:
// 1654 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("and", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4018 "parser.c"
        break;
      case 191:
// 1659 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("or", yymsp[-2].minor.yy175, yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4026 "parser.c"
        break;
      case 192:
// 1664 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("instanceof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4034 "parser.c"
        break;
      case 193:
// 1669 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy175, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4043 "parser.c"
        break;
      case 194:
// 1674 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("typeof", yymsp[0].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4051 "parser.c"
        break;
      case 195:
// 1679 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4058 "parser.c"
        break;
      case 196:
      case 224:
      case 228:
// 1684 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4067 "parser.c"
        break;
      case 197:
      case 226:
      case 229:
// 1689 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4076 "parser.c"
        break;
      case 198:
      case 225:
      case 230:
// 1694 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4085 "parser.c"
        break;
      case 199:
// 1699 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4092 "parser.c"
        break;
      case 200:
      case 231:
// 1704 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4100 "parser.c"
        break;
      case 201:
      case 232:
// 1709 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(90,&yymsp[0].minor);
}
// 4109 "parser.c"
        break;
      case 202:
      case 234:
// 1714 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[0].minor);
}
// 4118 "parser.c"
        break;
      case 203:
      case 233:
// 1719 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4127 "parser.c"
        break;
      case 204:
// 1724 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
  yy_destructor(78,&yymsp[0].minor);
}
// 4136 "parser.c"
        break;
      case 205:
// 1729 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_expr("array", yymsp[-1].minor.yy175, NULL, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(78,&yymsp[0].minor);
}
// 4145 "parser.c"
        break;
      case 206:
// 1734 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4153 "parser.c"
        break;
      case 207:
// 1739 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 4163 "parser.c"
        break;
      case 208:
// 1744 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(26,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 4173 "parser.c"
        break;
      case 209:
// 1749 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 4182 "parser.c"
        break;
      case 210:
// 1754 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 4191 "parser.c"
        break;
      case 213:
// 1769 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(79,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 4201 "parser.c"
        break;
      case 214:
// 1774 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 4211 "parser.c"
        break;
      case 215:
// 1779 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy175, status->scanner_state);
  yy_destructor(76,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 4221 "parser.c"
        break;
      case 216:
// 1784 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(28,&yymsp[0].minor);
}
// 4231 "parser.c"
        break;
      case 222:
// 1808 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(yymsp[-2].minor.yy175, yymsp[0].minor.yy175, status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
}
// 4239 "parser.c"
        break;
      case 223:
// 1812 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_array_item(NULL, yymsp[0].minor.yy175, status->scanner_state);
}
// 4246 "parser.c"
        break;
      case 239:
// 1879 "parser.lemon"
{
	yygotominor.yy175 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4253 "parser.c"
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
// 802 "parser.lemon"


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

// 4329 "parser.c"
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
			case XX_T_MUL:
				xx_(xx_parser, XX_MUL, NULL, parser_status);
				break;
			case XX_T_DIV:
				xx_(xx_parser, XX_DIV, NULL, parser_status);
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
