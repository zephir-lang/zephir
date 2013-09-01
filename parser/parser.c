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
#define YYNOCODE 157
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy103;
  int yy313;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 498
#define YYNRULE 239
#define YYERRORSYMBOL 94
#define YYERRSYMDT yy313
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
 /*     0 */    89,  154,  158,  152,  190,   86,  354,  177,  147,  352,
 /*    10 */   441,   17,  456,  464,    7,  470,  444,   10,  160,  440,
 /*    20 */   130,  484,  356,  131,  132,  133,  134,  135,  136,  137,
 /*    30 */   138,  139,  140,  141,  250,  126,  128,   88,  102,  106,
 /*    40 */   108,  110,  112,  104,  100,  114,  116,  124,  118,  120,
 /*    50 */   122,  473,  456,  464,  176,  168,  444,  143,  351,  440,
 /*    60 */   144,  498,  362,  161,  162,  356,  163,  164,  165,  166,
 /*    70 */   167,  293,  296,  315,  324,  333,  336,  327,  330,  339,
 /*    80 */   345,  342,  142,  348,   46,   48,   50,  177,   59,    8,
 /*    90 */   197,  198,   70,   74,   79,  219,  316,  323,  226,  434,
 /*   100 */   425,  351,  414,  403,  407,   73,  440,    9,  356,  499,
 /*   110 */   277,  288,  291,  325,  323,  306,  315,  324,  333,  336,
 /*   120 */   327,  330,  339,  345,  342,   11,  348,   46,   48,   50,
 /*   130 */   170,   59,  189,  174,  185,   70,   74,   79,  219,    7,
 /*   140 */    14,  226,   10,  466,  351,  309,  484,  356,  379,  307,
 /*   150 */   467,  356,  196,  277,  288,  291,  318,  318,  306,  315,
 /*   160 */   324,  333,  336,  327,  330,  339,  345,  342,  253,  348,
 /*   170 */    46,   48,   50,  249,   59,  313,  197,  198,   70,   74,
 /*   180 */    79,  219,  317,  347,  226,  392,  177,  351,  414,  403,
 /*   190 */   407,   78,  318,  398,  356,  172,  277,  288,  291,  328,
 /*   200 */   323,  306,  315,  324,  333,  336,  327,  330,  339,  345,
 /*   210 */   342,  415,  348,   46,   48,   50,  393,   59,  326,   21,
 /*   220 */   151,   70,   74,   79,  219,  331,  323,  226,  197,  198,
 /*   230 */   351,  173,  174,  185,  369,  199,  196,  356,   22,  277,
 /*   240 */   288,  291,   64,   47,  306,  315,  324,  333,  336,  327,
 /*   250 */   330,  339,  345,  342,  282,  348,   46,   48,   50,  195,
 /*   260 */    59,  281,  197,  198,   70,   74,   79,  219,  171,   66,
 /*   270 */   226,  381,   24,  351,  118,  120,  122,  375,  318,   12,
 /*   280 */   356,  478,  277,  288,  291,   62,   55,  306,  315,  324,
 /*   290 */   333,  336,  327,  330,  339,  345,  342,  253,  348,   46,
 /*   300 */    48,   50,  262,   59,  329,  197,  198,   70,   74,   79,
 /*   310 */   219,  458,   66,  226,  381,   19,  351,  440,   18,   65,
 /*   320 */   380,  440,  487,  356,  491,  277,  288,  291,  227,  276,
 /*   330 */   306,  315,  324,  333,  336,  327,  330,  339,  345,  342,
 /*   340 */   253,  348,   46,   48,   50,  270,   59,   49,  197,  198,
 /*   350 */    70,   74,   79,  219,  334,  323,  226,  337,  323,  351,
 /*   360 */    52,  340,  323,   82,  318,  460,  356,   67,  277,  288,
 /*   370 */   291,  318,  461,  306,  315,  324,  333,  336,  327,  330,
 /*   380 */   339,  345,  342,  366,  348,   46,   48,   50,   93,   59,
 /*   390 */   338,  197,  198,   70,   74,   79,  219,  341,  205,  226,
 /*   400 */   343,  323,  351,   63,  384,   68,   58,   56,  210,  356,
 /*   410 */   196,  277,  288,  291,   61,  318,  306,  315,  324,  333,
 /*   420 */   336,  327,  330,  339,  345,  342,  359,  348,   46,   48,
 /*   430 */    50,  297,   59,  208,  197,  198,   70,   74,   79,  219,
 /*   440 */    71,  344,  226,   95,   97,  351,  346,  323,   83,  385,
 /*   450 */   318,  196,  356,   76,  277,  288,  291,  318,  229,  306,
 /*   460 */   315,  324,  333,  336,  327,  330,  339,  345,  342,  371,
 /*   470 */   348,   46,   48,   50,  202,   59,  332,  197,  198,   70,
 /*   480 */    74,   79,  219,  335,  228,  226,  300,  303,  351,  349,
 /*   490 */   323,   85,  225,  318,  196,  356,   80,  277,  288,  291,
 /*   500 */   280,   91,  306,  315,  324,  333,  336,  327,  330,  339,
 /*   510 */   345,  342,   87,  348,   46,   48,   50,  216,   59,  350,
 /*   520 */   197,  198,   70,   74,   79,  219,  279,   90,  226,   94,
 /*   530 */   145,  351,   92,  149,   96,   54,  150,   98,  356,  155,
 /*   540 */   277,  288,  291,  156,  157,  306,  315,  324,  333,  336,
 /*   550 */   327,  330,  339,  345,  342,   99,  348,   46,   48,   50,
 /*   560 */   363,   59,  175,  197,  198,   70,   74,   79,  219,  721,
 /*   570 */   722,  226,  723,  191,  351,  192,  206,  220,  386,  231,
 /*   580 */   230,  356,  238,  277,  288,  291,  242,  243,  306,  315,
 /*   590 */   324,  333,  336,  327,  330,  339,  345,  342,  101,  348,
 /*   600 */    46,   48,   50,  247,   59,  251,  197,  198,   70,   74,
 /*   610 */    79,  219,  255,  256,  226,  260,  264,  351,  399,  403,
 /*   620 */   407,  374,  268,  221,  356,  398,  277,  288,  291,  263,
 /*   630 */   271,  306,  315,  324,  333,  336,  327,  330,  339,  345,
 /*   640 */   342,  285,  348,   46,   48,   50,  287,   59,  435,  292,
 /*   650 */   103,   70,   74,   79,  219,  272,  294,  226,  197,  198,
 /*   660 */   351,  298,  295,  299,  391,  310,  301,  356,  302,  277,
 /*   670 */   288,  291,  304,  305,  306,  315,  324,  333,  336,  327,
 /*   680 */   330,  339,  345,  342,  105,  348,   46,   48,   50,  308,
 /*   690 */    59,  311,  197,  198,   70,   74,   79,  219,  314,  320,
 /*   700 */   226,  319,  321,  351,  322,  353,  355,  390,  364,  365,
 /*   710 */   356,  376,  277,  288,  291,  378,  382,  306,  315,  324,
 /*   720 */   333,  336,  327,  330,  339,  345,  342,  107,  348,   46,
 /*   730 */    48,   50,  388,   59,  394,  197,  198,   70,   74,   79,
 /*   740 */   219,  387,  401,  226,  404,  402,  351,  405,  408,  409,
 /*   750 */   395,  406,  410,  356,  412,  277,  288,  291,  413,  561,
 /*   760 */   306,  315,  324,  333,  336,  327,  330,  339,  345,  342,
 /*   770 */   109,  348,   46,   48,   50,  427,   59,  428,  197,  198,
 /*   780 */    70,   74,   79,  219,  430,  436,  226,  448,  447,  351,
 /*   790 */   449,  450,  454,  397,  452,  453,  356,  455,  277,  288,
 /*   800 */   291,  462,  463,  306,  315,  324,  333,  336,  327,  330,
 /*   810 */   339,  345,  342,  111,  348,   46,   48,   50,  469,   59,
 /*   820 */   468,  197,  198,   70,   74,   79,  219,  479,  480,  226,
 /*   830 */   482,  485,  351,  486,  489,  492,  431,  493,  397,  356,
 /*   840 */   397,  277,  288,  291,  495,  397,  306,  315,  324,  333,
 /*   850 */   336,  327,  330,  339,  345,  342,  113,  348,   46,   48,
 /*   860 */    50,  397,   59,  397,  197,  198,   70,   74,   79,  219,
 /*   870 */   397,  397,  226,  397,  397,  351,  397,  397,  397,  433,
 /*   880 */   397,  397,  356,  397,  277,  288,  291,  397,  397,  306,
 /*   890 */   315,  324,  333,  336,  327,  330,  339,  345,  342,  115,
 /*   900 */   348,   46,   48,   50,  397,   59,  397,  197,  198,   70,
 /*   910 */    74,   79,  219,  397,  397,  226,  397,  397,  351,  397,
 /*   920 */   397,  397,  437,  397,  397,  356,  397,  277,  288,  291,
 /*   930 */   397,  397,  306,  315,  324,  333,  336,  327,  330,  339,
 /*   940 */   345,  342,  117,  348,   46,   48,   50,  397,   59,  397,
 /*   950 */   197,  198,   70,   74,   79,  219,  397,  397,  226,  397,
 /*   960 */   397,  351,  397,  397,  397,  439,  397,  397,  356,  397,
 /*   970 */   277,  288,  291,  397,  397,  306,  315,  324,  333,  336,
 /*   980 */   327,  330,  339,  345,  342,  289,  348,   46,   48,   50,
 /*   990 */   397,   59,  397,  197,  198,   70,   74,   79,  219,  397,
 /*  1000 */   397,  226,  397,  397,  351,  397,  397,  397,   25,  397,
 /*  1010 */   397,  356,  397,  277,  288,  291,  397,  397,  306,  315,
 /*  1020 */   324,  333,  336,  327,  330,  339,  345,  342,  119,  348,
 /*  1030 */    46,   48,   50,  397,   59,  397,  197,  198,   70,   74,
 /*  1040 */    79,  219,  397,  397,  226,  397,  397,  351,  397,  397,
 /*  1050 */   397,   27,  397,  397,  356,  397,  277,  288,  291,  397,
 /*  1060 */   397,  306,  315,  324,  333,  336,  327,  330,  339,  345,
 /*  1070 */   342,  121,  348,   46,   48,   50,  397,   59,  397,  197,
 /*  1080 */   198,   70,   74,   79,  219,  397,  397,  226,  397,  397,
 /*  1090 */   351,  397,  416,  417,  419,  418,  420,  356,  397,  277,
 /*  1100 */   288,  291,  397,  397,  306,  315,  324,  333,  336,  327,
 /*  1110 */   330,  339,  345,  342,  397,  348,   46,   48,   50,  397,
 /*  1120 */    59,  397,  465,  123,   70,   74,   79,  219,  422,  397,
 /*  1130 */   226,  197,  198,   20,   45,  421,  423,  232,    3,    4,
 /*  1140 */     5,    6,  277,  288,  291,   53,  397,  306,  178,  397,
 /*  1150 */   357,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1160 */    38,   39,   40,   41,   42,   43,   44,  397,  397,   45,
 /*  1170 */   233,  234,  235,  236,  237,  125,  258,  397,  284,  286,
 /*  1180 */   361,  397,  397,  197,  198,  357,   29,   30,   31,   32,
 /*  1190 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1200 */    43,   44,  397,  179,  180,  397,   45,  181,  182,  184,
 /*  1210 */   183,  397,  282,  284,  286,  397,  397,   81,  278,  283,
 /*  1220 */   197,  198,  357,   29,   30,   31,   32,   33,   34,   35,
 /*  1230 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  472,
 /*  1240 */   397,  475,  476,  464,   45,  443,  444,  397,  232,  440,
 /*  1250 */   284,  286,  397,  397,  397,  383,  397,  397,  186,  397,
 /*  1260 */   357,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1270 */    38,   39,   40,   41,   42,   43,   44,  397,  397,  397,
 /*  1280 */    45,  233,  234,  235,  236,  237,  240,  245,  284,  286,
 /*  1290 */   252,   26,  397,  214,  197,  198,  357,   29,   30,   31,
 /*  1300 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1310 */    42,   43,   44,  188,  187,   45,  397,  181,  182,  184,
 /*  1320 */   183,  397,  397,  397,  284,  286,  438,  148,  211,  397,
 /*  1330 */   204,  357,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1340 */    37,   38,   39,   40,   41,   42,   43,   44,  397,  397,
 /*  1350 */   397,  397,   45,  114,  116,  124,  118,  120,  122,  284,
 /*  1360 */   286,  397,  397,  396,  397,  397,  397,  397,  357,   29,
 /*  1370 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1380 */    40,   41,   42,   43,   44,  397,  397,  179,  180,  397,
 /*  1390 */    45,  181,  182,  184,  183,  397,  284,  286,  397,  397,
 /*  1400 */   397,  373,  397,  397,  397,  397,  357,   29,   30,   31,
 /*  1410 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1420 */    42,   43,   44,  397,  397,  397,   45,  738,    1,    2,
 /*  1430 */   497,    4,    5,    6,  284,  286,  397,  368,  397,  397,
 /*  1440 */   397,  397,  357,   29,   30,   31,   32,   33,   34,   35,
 /*  1450 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  397,
 /*  1460 */   442,   45,   19,  397,  443,  444,  232,  397,  440,  397,
 /*  1470 */   284,  286,   57,  397,  397,  397,  397,  357,   29,   30,
 /*  1480 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1490 */    41,   42,   43,   44,  397,  397,  397,  397,   45,  233,
 /*  1500 */   234,  235,  236,  237,  397,  284,  286,  397,  397,  432,
 /*  1510 */   397,  397,  397,  397,  357,   29,   30,   31,   32,   33,
 /*  1520 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1530 */    44,  151,  397,  397,  397,  397,   45,  397,  397,  197,
 /*  1540 */   198,  194,  284,  286,  397,  397,  200,   69,  397,  397,
 /*  1550 */   397,  397,  357,   29,   30,   31,   32,   33,   34,   35,
 /*  1560 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  151,
 /*  1570 */   397,  397,   45,  397,  397,  397,  397,  197,  198,  201,
 /*  1580 */   284,  286,  397,   72,  200,  397,  397,  397,  357,   29,
 /*  1590 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1600 */    40,   41,   42,   43,   44,  397,  240,   45,  397,  397,
 /*  1610 */   239,  397,  397,  397,  197,  198,  284,  286,   77,  397,
 /*  1620 */   397,  397,  397,  357,   29,   30,   31,   32,   33,   34,
 /*  1630 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1640 */   397,  240,  397,  397,   45,  273,  397,  397,  397,  197,
 /*  1650 */   198,  284,  286,  397,  397,  389,  397,  397,  397,  397,
 /*  1660 */   357,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1670 */    38,   39,   40,   41,   42,   43,   44,  151,  397,  397,
 /*  1680 */   397,  397,   45,  397,  397,  197,  198,  207,  284,  286,
 /*  1690 */   397,  397,  200,  224,  397,  397,  397,  397,  357,   29,
 /*  1700 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1710 */    40,   41,   42,   43,   44,  151,  397,  397,   45,  397,
 /*  1720 */   397,  397,  397,  197,  198,  215,  284,  286,  397,  397,
 /*  1730 */   200,  397,  397,  397,   28,   29,   30,   31,   32,   33,
 /*  1740 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1750 */    44,   89,  154,  158,  152,  190,   86,  217,  397,  147,
 /*  1760 */   397,  397,  284,  286,  416,  417,  419,  418,  420,  160,
 /*  1770 */   397,  130,   89,  154,  158,  152,  190,   86,  397,  240,
 /*  1780 */   147,  397,  397,  248,  397,  397,  397,  197,  198,  397,
 /*  1790 */   160,  397,  130,   89,  154,  158,  152,  190,   86,  203,
 /*  1800 */   422,  147,  397,  446,  397,  397,  168,  421,  423,  397,
 /*  1810 */   397,  160,  397,  130,  161,  162,  397,  163,  164,  165,
 /*  1820 */   166,  167,   51,  127,  397,  218,  397,  168,  267,  397,
 /*  1830 */   397,  197,  198,  197,  198,  161,  162,  151,  163,  164,
 /*  1840 */   165,  166,  167,  397,  397,  197,  198,  312,  168,  397,
 /*  1850 */   397,  397,  200,  397,  397,  397,  161,  162,  397,  163,
 /*  1860 */   164,  165,  166,  167,   89,  154,  158,  152,  190,   86,
 /*  1870 */   193,  397,  147,  397,   75,  397,  397,  218,  397,  232,
 /*  1880 */   397,  397,  160,  397,  130,  197,  198,  397,   89,  154,
 /*  1890 */   158,  152,  190,   86,  240,  397,  147,  397,  269,  397,
 /*  1900 */   397,  240,  197,  198,  397,  265,  160,  397,  130,  197,
 /*  1910 */   198,  397,  233,  234,  235,  236,  237,  241,  266,  168,
 /*  1920 */   254,  274,  275,  397,  397,  397,  397,  161,  162,  397,
 /*  1930 */   163,  164,  165,  166,  167,  370,   84,  129,  377,  218,
 /*  1940 */   397,  218,  397,  168,  397,  197,  198,  197,  198,  197,
 /*  1950 */   198,  161,  162,  397,  163,  164,  165,  166,  167,   89,
 /*  1960 */   154,  158,  152,  190,   86,  397,  146,  147,  397,  416,
 /*  1970 */   417,  419,  418,  420,  197,  198,  397,  160,  397,  130,
 /*  1980 */    89,  154,  158,  152,  190,   86,  209,  397,  147,  397,
 /*  1990 */   397,  397,  397,  397,  397,  397,  397,  397,  160,  397,
 /*  2000 */   130,  397,  397,  515,  397,  422,  457,   60,  451,  397,
 /*  2010 */   218,  397,  421,  423,  168,  246,  397,  397,  197,  198,
 /*  2020 */   397,  397,  161,  162,  397,  163,  164,  165,  166,  167,
 /*  2030 */   240,  397,  240,  397,  257,  168,  261,  397,  197,  198,
 /*  2040 */   197,  198,  397,  161,  162,  397,  163,  164,  165,  166,
 /*  2050 */   167,   89,  154,  158,  152,  190,   86,  397,  212,  147,
 /*  2060 */   397,  416,  417,  419,  418,  420,  197,  198,  397,  160,
 /*  2070 */   397,  130,   89,  154,  158,  152,  190,   86,  397,  240,
 /*  2080 */   147,  397,  397,  244,  397,  222,  397,  197,  198,  397,
 /*  2090 */   160,  459,  130,  197,  198,  397,  397,  422,  358,  159,
 /*  2100 */   397,  153,  426,  397,  421,  423,  168,  197,  198,  197,
 /*  2110 */   198,  397,  397,  397,  161,  162,  397,  163,  164,  165,
 /*  2120 */   166,  167,  416,  417,  419,  418,  420,  168,  259,  397,
 /*  2130 */   397,  397,  397,  397,  397,  161,  162,  397,  163,  164,
 /*  2140 */   165,  166,  167,   89,  154,  158,  152,  190,   86,  397,
 /*  2150 */   397,  147,  397,  416,  417,  419,  418,  420,  422,  397,
 /*  2160 */   397,  160,  397,  130,  397,  421,  423,  397,  397,  126,
 /*  2170 */   128,   88,  102,  106,  108,  110,  112,  104,  100,  114,
 /*  2180 */   116,  124,  118,  120,  122,  397,  397,  518,  397,  422,
 /*  2190 */   445,  397,  451,  397,  290,  397,  421,  423,  168,  397,
 /*  2200 */   397,  397,  397,  397,  397,  397,  161,  162,  397,  163,
 /*  2210 */   164,  165,  166,  167,  126,  128,   88,  102,  106,  108,
 /*  2220 */   110,  112,  104,  100,  114,  116,  124,  118,  120,  122,
 /*  2230 */   126,  128,   88,  102,  106,  108,  110,  112,  104,  100,
 /*  2240 */   114,  116,  124,  118,  120,  122,  397,  481,   16,  471,
 /*  2250 */   477,  476,  464,  397,  470,  444,  397,  372,  440,  397,
 /*  2260 */   397,  397,  397,  397,  397,  126,  128,   88,  102,  106,
 /*  2270 */   108,  110,  112,  104,  100,  114,  116,  124,  118,  120,
 /*  2280 */   122,  416,  417,  419,  418,  420,  213,  397,  397,  397,
 /*  2290 */   397,  397,  223,  397,  126,  128,   88,  102,  106,  108,
 /*  2300 */   110,  112,  104,  100,  114,  116,  124,  118,  120,  122,
 /*  2310 */   397,  397,  397,  397,  397,  514,  397,  422,  457,  397,
 /*  2320 */   451,  367,  397,  397,  421,  423,  397,  397,  397,  126,
 /*  2330 */   128,   88,  102,  106,  108,  110,  112,  104,  100,  114,
 /*  2340 */   116,  124,  118,  120,  122,  397,  488,   16,  471,  477,
 /*  2350 */   476,  464,  397,  470,  444,  397,  360,  440,  397,  397,
 /*  2360 */   397,  397,  397,  397,  126,  128,   88,  102,  106,  108,
 /*  2370 */   110,  112,  104,  100,  114,  116,  124,  118,  120,  122,
 /*  2380 */   126,  128,   88,  102,  106,  108,  110,  112,  104,  100,
 /*  2390 */   114,  116,  124,  118,  120,  122,   88,  102,  106,  108,
 /*  2400 */   110,  112,  104,  100,  114,  116,  124,  118,  120,  122,
 /*  2410 */   411,   13,   16,  471,  477,  476,  464,  397,  470,  444,
 /*  2420 */   397,  411,  440,  397,  397,  397,  397,  397,   23,  397,
 /*  2430 */   400,  397,  397,  397,  397,  397,  636,  397,  397,  429,
 /*  2440 */   397,  400,  397,  397,  397,  131,  132,  133,  134,  135,
 /*  2450 */   136,  137,  138,  139,  140,  141,  131,  132,  133,  134,
 /*  2460 */   135,  136,  137,  138,  139,  140,  141,  411,  494,   16,
 /*  2470 */   471,  477,  476,  464,  397,  470,  444,  397,  397,  440,
 /*  2480 */   397,  397,  397,  397,  397,  397,  397,  400,  397,  416,
 /*  2490 */   417,  419,  418,  420,  397,  397,  397,  397,  397,  397,
 /*  2500 */   397,  397,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  2510 */   139,  140,  141,  416,  417,  419,  418,  420,  397,  397,
 /*  2520 */   397,  397,  397,  483,  397,  422,  457,  397,  451,  397,
 /*  2530 */   397,  397,  421,  423,  416,  417,  419,  418,  420,  397,
 /*  2540 */   397,  397,  397,  397,  397,  397,  397,  490,  397,  422,
 /*  2550 */   457,  397,  451,  397,  397,  397,  421,  423,  416,  417,
 /*  2560 */   419,  418,  420,  397,  397,  397,  397,  397,   15,  397,
 /*  2570 */   422,  457,  397,  451,  397,  397,  397,  421,  423,  416,
 /*  2580 */   417,  419,  418,  420,  416,  417,  419,  418,  420,  397,
 /*  2590 */   397,  397,  496,  397,  422,  457,  397,  451,  397,  397,
 /*  2600 */   397,  421,  423,  416,  417,  419,  418,  420,  416,  417,
 /*  2610 */   419,  418,  420,  517,  397,  422,  424,  397,  521,  397,
 /*  2620 */   422,  424,  421,  423,  397,  397,  397,  421,  423,  416,
 /*  2630 */   417,  419,  418,  420,  397,  397,  397,  516,  397,  422,
 /*  2640 */   424,  397,  520,  397,  422,  424,  421,  423,  397,  397,
 /*  2650 */   397,  421,  423,  416,  417,  419,  418,  420,  416,  417,
 /*  2660 */   419,  418,  420,  522,  186,  422,  424,  416,  417,  419,
 /*  2670 */   418,  420,  421,  423,  397,  397,  397,  397,  397,  397,
 /*  2680 */   397,  397,  397,  397,  397,  397,  397,  519,  397,  422,
 /*  2690 */   474,  397,  397,  397,  422,  397,  421,  423,  397,   20,
 /*  2700 */   397,  421,  423,  422,  397,  397,  397,  397,  426,  397,
 /*  2710 */   421,  423,  169,  397,  397,  397,  397,  397,  397,  188,
 /*  2720 */   187,  397,  397,  181,  182,  184,  183,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    23,   24,   25,   26,   27,   28,   77,  108,   31,   80,
 /*    10 */   104,  105,  106,  107,   30,  109,  110,   33,   41,  113,
 /*    20 */    43,   37,   38,   46,   47,   48,   49,   50,   51,   52,
 /*    30 */    53,   54,   55,   56,   79,    7,    8,    9,   10,   11,
 /*    40 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*    50 */    22,  105,  106,  107,  155,   78,  110,   29,   31,  113,
 /*    60 */   115,    0,   35,   86,   87,   38,   89,   90,   91,   92,
 /*    70 */    93,   77,   78,   46,   47,   48,   49,   50,   51,   52,
 /*    80 */    53,   54,  137,   56,   57,   58,   59,  108,   61,   31,
 /*    90 */   145,  146,   65,   66,   67,   68,  148,  149,   71,  111,
 /*   100 */   107,   31,  114,  115,  116,   35,  113,   32,   38,    0,
 /*   110 */    83,   84,   85,  148,  149,   88,   46,   47,   48,   49,
 /*   120 */    50,   51,   52,   53,   54,   31,   56,   57,   58,   59,
 /*   130 */   151,   61,  153,  154,  155,   65,   66,   67,   68,   30,
 /*   140 */    35,   71,   33,   32,   31,   27,   37,   38,   35,   31,
 /*   150 */    39,   38,    6,   83,   84,   85,    6,    6,   88,   46,
 /*   160 */    47,   48,   49,   50,   51,   52,   53,   54,  137,   56,
 /*   170 */    57,   58,   59,  142,   61,   29,  145,  146,   65,   66,
 /*   180 */    67,   68,   32,   32,   71,  111,  108,   31,  114,  115,
 /*   190 */   116,   35,    6,    6,   38,    6,   83,   84,   85,  148,
 /*   200 */   149,   88,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   210 */    54,  113,   56,   57,   58,   59,   29,   61,   32,   31,
 /*   220 */   137,   65,   66,   67,   68,  148,  149,   71,  145,  146,
 /*   230 */    31,  153,  154,  155,   35,  152,    6,   38,   43,   83,
 /*   240 */    84,   85,   35,   32,   88,   46,   47,   48,   49,   50,
 /*   250 */    51,   52,   53,   54,  137,   56,   57,   58,   59,   29,
 /*   260 */    61,  144,  145,  146,   65,   66,   67,   68,   79,   62,
 /*   270 */    71,   64,   34,   31,   20,   21,   22,   35,    6,   34,
 /*   280 */    38,   36,   83,   84,   85,   35,   60,   88,   46,   47,
 /*   290 */    48,   49,   50,   51,   52,   53,   54,  137,   56,   57,
 /*   300 */    58,   59,  142,   61,   32,  145,  146,   65,   66,   67,
 /*   310 */    68,  107,   62,   71,   64,  107,   31,  113,  110,  136,
 /*   320 */    35,  113,   34,   38,   36,   83,   84,   85,  138,  139,
 /*   330 */    88,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   340 */   137,   56,   57,   58,   59,  142,   61,   32,  145,  146,
 /*   350 */    65,   66,   67,   68,  148,  149,   71,  148,  149,   31,
 /*   360 */    34,  148,  149,   35,    6,   32,   38,  108,   83,   84,
 /*   370 */    85,    6,   39,   88,   46,   47,   48,   49,   50,   51,
 /*   380 */    52,   53,   54,  137,   56,   57,   58,   59,   31,   61,
 /*   390 */    32,  145,  146,   65,   66,   67,   68,   32,   31,   71,
 /*   400 */   148,  149,   31,  135,  136,   63,   35,   34,   41,   38,
 /*   410 */     6,   83,   84,   85,   34,    6,   88,   46,   47,   48,
 /*   420 */    49,   50,   51,   52,   53,   54,  137,   56,   57,   58,
 /*   430 */    59,   31,   61,   29,  145,  146,   65,   66,   67,   68,
 /*   440 */    34,   32,   71,   86,   87,   31,  148,  149,   66,   35,
 /*   450 */     6,    6,   38,   34,   83,   84,   85,    6,    6,   88,
 /*   460 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  137,
 /*   470 */    56,   57,   58,   59,   29,   61,   32,  145,  146,   65,
 /*   480 */    66,   67,   68,   32,   32,   71,   86,   87,   31,  148,
 /*   490 */   149,   32,   35,    6,    6,   38,   34,   83,   84,   85,
 /*   500 */     6,   31,   88,   46,   47,   48,   49,   50,   51,   52,
 /*   510 */    53,   54,  137,   56,   57,   58,   59,   29,   61,   32,
 /*   520 */   145,  146,   65,   66,   67,   68,   32,  150,   71,   79,
 /*   530 */    29,   31,   78,   31,   79,   35,   43,   79,   38,   31,
 /*   540 */    83,   84,   85,    6,  150,   88,   46,   47,   48,   49,
 /*   550 */    50,   51,   52,   53,   54,  137,   56,   57,   58,   59,
 /*   560 */     6,   61,   63,  145,  146,   65,   66,   67,   68,   63,
 /*   570 */    63,   71,   63,   31,   31,   43,   43,   31,   35,   31,
 /*   580 */   139,   38,  140,   83,   84,   85,   31,  140,   88,   46,
 /*   590 */    47,   48,   49,   50,   51,   52,   53,   54,  137,   56,
 /*   600 */    57,   58,   59,  140,   61,  140,  145,  146,   65,   66,
 /*   610 */    67,   68,   31,  140,   71,  140,  140,   31,  114,  115,
 /*   620 */   116,   35,  140,   69,   38,    6,   83,   84,   85,   79,
 /*   630 */    79,   88,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   640 */    54,   32,   56,   57,   58,   59,   32,   61,   29,   31,
 /*   650 */   137,   65,   66,   67,   68,  140,   31,   71,  145,  146,
 /*   660 */    31,   79,   32,   32,   35,   31,   79,   38,   32,   83,
 /*   670 */    84,   85,   79,   32,   88,   46,   47,   48,   49,   50,
 /*   680 */    51,   52,   53,   54,  137,   56,   57,   58,   59,   32,
 /*   690 */    61,   43,  145,  146,   65,   66,   67,   68,   32,   31,
 /*   700 */    71,  149,   39,   31,  108,   31,   31,   35,   31,   69,
 /*   710 */    38,   66,   83,   84,   85,   32,   63,   88,   46,   47,
 /*   720 */    48,   49,   50,   51,   52,   53,   54,  137,   56,   57,
 /*   730 */    58,   59,   34,   61,   34,  145,  146,   65,   66,   67,
 /*   740 */    68,   60,   39,   71,   31,  108,   31,   39,   31,   39,
 /*   750 */    35,  108,  108,   38,   31,   83,   84,   85,   12,   31,
 /*   760 */    88,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   770 */   137,   56,   57,   58,   59,   31,   61,   43,  145,  146,
 /*   780 */    65,   66,   67,   68,   34,   34,   71,   39,   41,   31,
 /*   790 */   108,   32,  108,   35,   41,   39,   38,   32,   83,   84,
 /*   800 */    85,  108,   32,   88,   46,   47,   48,   49,   50,   51,
 /*   810 */    52,   53,   54,  137,   56,   57,   58,   59,   32,   61,
 /*   820 */   108,  145,  146,   65,   66,   67,   68,   31,   34,   71,
 /*   830 */    35,   33,   31,   31,   35,   31,   35,   34,  156,   38,
 /*   840 */   156,   83,   84,   85,   35,  156,   88,   46,   47,   48,
 /*   850 */    49,   50,   51,   52,   53,   54,  137,   56,   57,   58,
 /*   860 */    59,  156,   61,  156,  145,  146,   65,   66,   67,   68,
 /*   870 */   156,  156,   71,  156,  156,   31,  156,  156,  156,   35,
 /*   880 */   156,  156,   38,  156,   83,   84,   85,  156,  156,   88,
 /*   890 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  137,
 /*   900 */    56,   57,   58,   59,  156,   61,  156,  145,  146,   65,
 /*   910 */    66,   67,   68,  156,  156,   71,  156,  156,   31,  156,
 /*   920 */   156,  156,   35,  156,  156,   38,  156,   83,   84,   85,
 /*   930 */   156,  156,   88,   46,   47,   48,   49,   50,   51,   52,
 /*   940 */    53,   54,  137,   56,   57,   58,   59,  156,   61,  156,
 /*   950 */   145,  146,   65,   66,   67,   68,  156,  156,   71,  156,
 /*   960 */   156,   31,  156,  156,  156,   35,  156,  156,   38,  156,
 /*   970 */    83,   84,   85,  156,  156,   88,   46,   47,   48,   49,
 /*   980 */    50,   51,   52,   53,   54,  137,   56,   57,   58,   59,
 /*   990 */   156,   61,  156,  145,  146,   65,   66,   67,   68,  156,
 /*  1000 */   156,   71,  156,  156,   31,  156,  156,  156,   35,  156,
 /*  1010 */   156,   38,  156,   83,   84,   85,  156,  156,   88,   46,
 /*  1020 */    47,   48,   49,   50,   51,   52,   53,   54,  137,   56,
 /*  1030 */    57,   58,   59,  156,   61,  156,  145,  146,   65,   66,
 /*  1040 */    67,   68,  156,  156,   71,  156,  156,   31,  156,  156,
 /*  1050 */   156,   35,  156,  156,   38,  156,   83,   84,   85,  156,
 /*  1060 */   156,   88,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  1070 */    54,  137,   56,   57,   58,   59,  156,   61,  156,  145,
 /*  1080 */   146,   65,   66,   67,   68,  156,  156,   71,  156,  156,
 /*  1090 */    31,  156,    1,    2,    3,    4,    5,   38,  156,   83,
 /*  1100 */    84,   85,  156,  156,   88,   46,   47,   48,   49,   50,
 /*  1110 */    51,   52,   53,   54,  156,   56,   57,   58,   59,  156,
 /*  1120 */    61,  156,   31,  137,   65,   66,   67,   68,   37,  156,
 /*  1130 */    71,  145,  146,   42,  101,   44,   45,   39,   98,   99,
 /*  1140 */   100,  101,   83,   84,   85,  112,  156,   88,   31,  156,
 /*  1150 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1160 */   127,  128,  129,  130,  131,  132,  133,  156,  156,  101,
 /*  1170 */    72,   73,   74,   75,   76,  137,   78,  156,  145,  146,
 /*  1180 */   112,  156,  156,  145,  146,  117,  118,  119,  120,  121,
 /*  1190 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1200 */   132,  133,  156,   86,   87,  156,  101,   90,   91,   92,
 /*  1210 */    93,  156,  137,  145,  146,  156,  156,  112,  143,  144,
 /*  1220 */   145,  146,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1230 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  103,
 /*  1240 */   156,  105,  106,  107,  101,  109,  110,  156,   39,  113,
 /*  1250 */   145,  146,  156,  156,  156,  112,  156,  156,   31,  156,
 /*  1260 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1270 */   127,  128,  129,  130,  131,  132,  133,  156,  156,  156,
 /*  1280 */   101,   72,   73,   74,   75,   76,  137,   78,  145,  146,
 /*  1290 */   141,  112,  156,   43,  145,  146,  117,  118,  119,  120,
 /*  1300 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1310 */   131,  132,  133,   86,   87,  101,  156,   90,   91,   92,
 /*  1320 */    93,  156,  156,  156,  145,  146,  112,   77,   78,  156,
 /*  1330 */    80,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1340 */   126,  127,  128,  129,  130,  131,  132,  133,  156,  156,
 /*  1350 */   156,  156,  101,   17,   18,   19,   20,   21,   22,  145,
 /*  1360 */   146,  156,  156,  112,  156,  156,  156,  156,  117,  118,
 /*  1370 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1380 */   129,  130,  131,  132,  133,  156,  156,   86,   87,  156,
 /*  1390 */   101,   90,   91,   92,   93,  156,  145,  146,  156,  156,
 /*  1400 */   156,  112,  156,  156,  156,  156,  117,  118,  119,  120,
 /*  1410 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1420 */   131,  132,  133,  156,  156,  156,  101,   95,   96,   97,
 /*  1430 */    98,   99,  100,  101,  145,  146,  156,  112,  156,  156,
 /*  1440 */   156,  156,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1450 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  156,
 /*  1460 */   105,  101,  107,  156,  109,  110,   39,  156,  113,  156,
 /*  1470 */   145,  146,  112,  156,  156,  156,  156,  117,  118,  119,
 /*  1480 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1490 */   130,  131,  132,  133,  156,  156,  156,  156,  101,   72,
 /*  1500 */    73,   74,   75,   76,  156,  145,  146,  156,  156,  112,
 /*  1510 */   156,  156,  156,  156,  117,  118,  119,  120,  121,  122,
 /*  1520 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1530 */   133,  137,  156,  156,  156,  156,  101,  156,  156,  145,
 /*  1540 */   146,  147,  145,  146,  156,  156,  152,  112,  156,  156,
 /*  1550 */   156,  156,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1560 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  137,
 /*  1570 */   156,  156,  101,  156,  156,  156,  156,  145,  146,  147,
 /*  1580 */   145,  146,  156,  112,  152,  156,  156,  156,  117,  118,
 /*  1590 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1600 */   129,  130,  131,  132,  133,  156,  137,  101,  156,  156,
 /*  1610 */   141,  156,  156,  156,  145,  146,  145,  146,  112,  156,
 /*  1620 */   156,  156,  156,  117,  118,  119,  120,  121,  122,  123,
 /*  1630 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  133,
 /*  1640 */   156,  137,  156,  156,  101,  141,  156,  156,  156,  145,
 /*  1650 */   146,  145,  146,  156,  156,  112,  156,  156,  156,  156,
 /*  1660 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1670 */   127,  128,  129,  130,  131,  132,  133,  137,  156,  156,
 /*  1680 */   156,  156,  101,  156,  156,  145,  146,  147,  145,  146,
 /*  1690 */   156,  156,  152,  112,  156,  156,  156,  156,  117,  118,
 /*  1700 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1710 */   129,  130,  131,  132,  133,  137,  156,  156,  101,  156,
 /*  1720 */   156,  156,  156,  145,  146,  147,  145,  146,  156,  156,
 /*  1730 */   152,  156,  156,  156,  117,  118,  119,  120,  121,  122,
 /*  1740 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1750 */   133,   23,   24,   25,   26,   27,   28,   29,  156,   31,
 /*  1760 */   156,  156,  145,  146,    1,    2,    3,    4,    5,   41,
 /*  1770 */   156,   43,   23,   24,   25,   26,   27,   28,  156,  137,
 /*  1780 */    31,  156,  156,  141,  156,  156,  156,  145,  146,  156,
 /*  1790 */    41,  156,   43,   23,   24,   25,   26,   27,   28,   29,
 /*  1800 */    37,   31,  156,   40,  156,  156,   78,   44,   45,  156,
 /*  1810 */   156,   41,  156,   43,   86,   87,  156,   89,   90,   91,
 /*  1820 */    92,   93,  134,  137,  156,  137,  156,   78,   79,  156,
 /*  1830 */   156,  145,  146,  145,  146,   86,   87,  137,   89,   90,
 /*  1840 */    91,   92,   93,  156,  156,  145,  146,  147,   78,  156,
 /*  1850 */   156,  156,  152,  156,  156,  156,   86,   87,  156,   89,
 /*  1860 */    90,   91,   92,   93,   23,   24,   25,   26,   27,   28,
 /*  1870 */    29,  156,   31,  156,  134,  156,  156,  137,  156,   39,
 /*  1880 */   156,  156,   41,  156,   43,  145,  146,  156,   23,   24,
 /*  1890 */    25,   26,   27,   28,  137,  156,   31,  156,  141,  156,
 /*  1900 */   156,  137,  145,  146,  156,  141,   41,  156,   43,  145,
 /*  1910 */   146,  156,   72,   73,   74,   75,   76,   77,   78,   78,
 /*  1920 */    80,   81,   82,  156,  156,  156,  156,   86,   87,  156,
 /*  1930 */    89,   90,   91,   92,   93,   70,  134,  137,  134,  137,
 /*  1940 */   156,  137,  156,   78,  156,  145,  146,  145,  146,  145,
 /*  1950 */   146,   86,   87,  156,   89,   90,   91,   92,   93,   23,
 /*  1960 */    24,   25,   26,   27,   28,  156,  137,   31,  156,    1,
 /*  1970 */     2,    3,    4,    5,  145,  146,  156,   41,  156,   43,
 /*  1980 */    23,   24,   25,   26,   27,   28,   29,  156,   31,  156,
 /*  1990 */   156,  156,  156,  156,  156,  156,  156,  156,   41,  156,
 /*  2000 */    43,  156,  156,   35,  156,   37,   38,  134,   40,  156,
 /*  2010 */   137,  156,   44,   45,   78,   79,  156,  156,  145,  146,
 /*  2020 */   156,  156,   86,   87,  156,   89,   90,   91,   92,   93,
 /*  2030 */   137,  156,  137,  156,  141,   78,  141,  156,  145,  146,
 /*  2040 */   145,  146,  156,   86,   87,  156,   89,   90,   91,   92,
 /*  2050 */    93,   23,   24,   25,   26,   27,   28,  156,  137,   31,
 /*  2060 */   156,    1,    2,    3,    4,    5,  145,  146,  156,   41,
 /*  2070 */   156,   43,   23,   24,   25,   26,   27,   28,  156,  137,
 /*  2080 */    31,  156,  156,  141,  156,  137,  156,  145,  146,  156,
 /*  2090 */    41,   31,   43,  145,  146,  156,  156,   37,   70,  137,
 /*  2100 */   156,  137,   42,  156,   44,   45,   78,  145,  146,  145,
 /*  2110 */   146,  156,  156,  156,   86,   87,  156,   89,   90,   91,
 /*  2120 */    92,   93,    1,    2,    3,    4,    5,   78,   79,  156,
 /*  2130 */   156,  156,  156,  156,  156,   86,   87,  156,   89,   90,
 /*  2140 */    91,   92,   93,   23,   24,   25,   26,   27,   28,  156,
 /*  2150 */   156,   31,  156,    1,    2,    3,    4,    5,   37,  156,
 /*  2160 */   156,   41,  156,   43,  156,   44,   45,  156,  156,    7,
 /*  2170 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2180 */    18,   19,   20,   21,   22,  156,  156,   35,  156,   37,
 /*  2190 */    38,  156,   40,  156,   32,  156,   44,   45,   78,  156,
 /*  2200 */   156,  156,  156,  156,  156,  156,   86,   87,  156,   89,
 /*  2210 */    90,   91,   92,   93,    7,    8,    9,   10,   11,   12,
 /*  2220 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2230 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2240 */    17,   18,   19,   20,   21,   22,  156,  102,  103,  104,
 /*  2250 */   105,  106,  107,  156,  109,  110,  156,   34,  113,  156,
 /*  2260 */   156,  156,  156,  156,  156,    7,    8,    9,   10,   11,
 /*  2270 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2280 */    22,    1,    2,    3,    4,    5,   79,  156,  156,  156,
 /*  2290 */   156,  156,   34,  156,    7,    8,    9,   10,   11,   12,
 /*  2300 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2310 */   156,  156,  156,  156,  156,   35,  156,   37,   38,  156,
 /*  2320 */    40,   34,  156,  156,   44,   45,  156,  156,  156,    7,
 /*  2330 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2340 */    18,   19,   20,   21,   22,  156,  102,  103,  104,  105,
 /*  2350 */   106,  107,  156,  109,  110,  156,   34,  113,  156,  156,
 /*  2360 */   156,  156,  156,  156,    7,    8,    9,   10,   11,   12,
 /*  2370 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2380 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2390 */    17,   18,   19,   20,   21,   22,    9,   10,   11,   12,
 /*  2400 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2410 */    11,  102,  103,  104,  105,  106,  107,  156,  109,  110,
 /*  2420 */   156,   11,  113,  156,  156,  156,  156,  156,   29,  156,
 /*  2430 */    31,  156,  156,  156,  156,  156,   79,  156,  156,   29,
 /*  2440 */   156,   31,  156,  156,  156,   46,   47,   48,   49,   50,
 /*  2450 */    51,   52,   53,   54,   55,   56,   46,   47,   48,   49,
 /*  2460 */    50,   51,   52,   53,   54,   55,   56,   11,  102,  103,
 /*  2470 */   104,  105,  106,  107,  156,  109,  110,  156,  156,  113,
 /*  2480 */   156,  156,  156,  156,  156,  156,  156,   31,  156,    1,
 /*  2490 */     2,    3,    4,    5,  156,  156,  156,  156,  156,  156,
 /*  2500 */   156,  156,   46,   47,   48,   49,   50,   51,   52,   53,
 /*  2510 */    54,   55,   56,    1,    2,    3,    4,    5,  156,  156,
 /*  2520 */   156,  156,  156,   35,  156,   37,   38,  156,   40,  156,
 /*  2530 */   156,  156,   44,   45,    1,    2,    3,    4,    5,  156,
 /*  2540 */   156,  156,  156,  156,  156,  156,  156,   35,  156,   37,
 /*  2550 */    38,  156,   40,  156,  156,  156,   44,   45,    1,    2,
 /*  2560 */     3,    4,    5,  156,  156,  156,  156,  156,   35,  156,
 /*  2570 */    37,   38,  156,   40,  156,  156,  156,   44,   45,    1,
 /*  2580 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  156,
 /*  2590 */   156,  156,   35,  156,   37,   38,  156,   40,  156,  156,
 /*  2600 */   156,   44,   45,    1,    2,    3,    4,    5,    1,    2,
 /*  2610 */     3,    4,    5,   35,  156,   37,   38,  156,   35,  156,
 /*  2620 */    37,   38,   44,   45,  156,  156,  156,   44,   45,    1,
 /*  2630 */     2,    3,    4,    5,  156,  156,  156,   35,  156,   37,
 /*  2640 */    38,  156,   35,  156,   37,   38,   44,   45,  156,  156,
 /*  2650 */   156,   44,   45,    1,    2,    3,    4,    5,    1,    2,
 /*  2660 */     3,    4,    5,   35,   31,   37,   38,    1,    2,    3,
 /*  2670 */     4,    5,   44,   45,  156,  156,  156,  156,  156,  156,
 /*  2680 */   156,  156,  156,  156,  156,  156,  156,   35,  156,   37,
 /*  2690 */    38,  156,  156,  156,   37,  156,   44,   45,  156,   42,
 /*  2700 */   156,   44,   45,   37,  156,  156,  156,  156,   42,  156,
 /*  2710 */    44,   45,   79,  156,  156,  156,  156,  156,  156,   86,
 /*  2720 */    87,  156,  156,   90,   91,   92,   93,
};
#define YY_SHIFT_USE_DFLT (-72)
static short yy_shift_ofst[] = {
 /*     0 */   -16,   61,  109,  -72,  -72,  -72,  -72,   58,   75,  -72,
 /*    10 */    94,  245, 2533,  105,  -72,  -72, 2280, 2578,  -72, 2657,
 /*    20 */   188,  195, 2399,  238,  973,  -72, 1016,  -72,  -72,  -72,
 /*    30 */   -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,
 /*    40 */   -72,  -72,  -72,  -72,  -72,  -72,  211,  -72,  315,  -72,
 /*    50 */  2120,  326,  543,  500,  226,  373,  414,  371,  -72, 2120,
 /*    60 */   380,  250,  -72,  207,  -72,  -72, 1301,  342, 1059, 1059,
 /*    70 */   406,  285,   70,  -72, 2120,  419,  113,  156,  -72,  462,
 /*    80 */   242,  328,  382, 2120,  459,  -72, 2120,  -72, 2120,  470,
 /*    90 */   -72,  454,  357,  450,  -72,  455,  -72,  458,  -72, 1336,
 /*   100 */  2120, 1336, 2120, 1336, 2120, 1336, 2120, 1336, 2120, 1336,
 /*   110 */  2120, 1336, 2120, 1336, 2120,  254, 2120,  254, 2120,  -72,
 /*   120 */  2120,  -72, 2120,  -72, 2120,  254, 2120, 2387, 2120, 2387,
 /*   130 */   -23,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72,
 /*   140 */   -72,  -72,   28,  -72,  501, 2120,  -72, 1250,  502,  493,
 /*   150 */  1770, 2373, 2120,  -72,  508,  537,  470,  -72, 2120,  -72,
 /*   160 */   -72,  -72,  -72,  -72,  -72,  -72,  -72,  -72, 2633,  -72,
 /*   170 */   189,  -72, 1227,  -72,  499, 1117,  -72,  -72,  -72,  -72,
 /*   180 */   -72,  -72,  -72,  -72,  -72,  -72,  506,  507,  509,  -72,
 /*   190 */   542,  532, 1841,  -72,  230,  -72, 2120,  -72,  -72,  -72,
 /*   200 */   -72,  445,  -72,  -72,  367,  533, 1957,  404,  -72,  -72,
 /*   210 */   -72, 2120, 2207,  -72, 1728,  488,  -72,  -72, 2373,  546,
 /*   220 */   554, 2028, 2258, 1059,  457,  -72,  548,  452,  -72,  548,
 /*   230 */   -72, 1840,  -72,  -72,  -72,  -72,  -72,  -72, 2120,  -72,
 /*   240 */  2373,  555, 1209, 2120,  -72, 1936, 1427, 2120,  -72,  -45,
 /*   250 */  1427, 2120,  -72, 2357,  581, 1098, 2120,  -72, 2049, 1427,
 /*   260 */  2120,  -72,  550, 1427, 2120,  -72, 1749, 1427, 2120,  -72,
 /*   270 */   551, 1427, 2120,  -72,  -72,  -72,  -72, 2120,  494,  -72,
 /*   280 */  2120,  -72, 2373,  -72,  609,  -72,  614,  -72, 2120, 2162,
 /*   290 */   -72,  618,   -6,  625,  630,  -72,  400,  582,  631,  -72,
 /*   300 */   587,  636,  -72,  593,  641,  -72,  118,  657,  -72,  634,
 /*   310 */   648, 2120,  146,  666,  -72,  668,  150,  -72,  668,  -72,
 /*   320 */   663, 1301,  -72,  -72,  668,  186,  -72,  668,  272,  -72,
 /*   330 */   668,  444,  -72,  668,  451,  -72,  668,  358,  -72,  668,
 /*   340 */   365,  -72,  668,  409,  -72,  668,  151,  -72,  668,  487,
 /*   350 */   -72,  -71,  674,  533,  675,  493,  -72,  -72, 2120, 2322,
 /*   360 */  1059,   27,  -72,  677,  640, 1865, 2287, 1059,  199,  -72,
 /*   370 */  2120, 2223, 1059,  586,  -72,  645, 2120,  683,  -72,  -72,
 /*   380 */   -72,  653, 1059, 1059,  -72,  -72,  681,  698,  629,  672,
 /*   390 */   -72,  -72,  187,  700,  715,  -72,  758,  -72, 2456,  -72,
 /*   400 */   703, 1301,  -72,  713,  708, 1301,  -72,  717,  710, 1301,
 /*   410 */   -72,  723,  746,  728,  -72,  -72,  -72,  -72,  -72,  -72,
 /*   420 */   -72,  -72,  -72,  -72, 2121, 2666,  744,  734, 2410,  750,
 /*   430 */   801,  -72,  844,  -72,  619,  751,  887,  -72,  930,  -72,
 /*   440 */   -72, 2152, 2583,  -72,  -72, 1763,  747,  748, 1301,  759,
 /*   450 */   -72,  753,  756, 1301,  765,  -72,  -72, 1763, 2060,  333,
 /*   460 */   -72, 1301,  770,  -72, 1091,  111,  -72, 1301,  786,  -72,
 /*   470 */   -72, 1968, 2652, 2628, 2121, 2607,  -72, 2602,  796,  794,
 /*   480 */  2488,  795,  -72,  -72,  798,  802,  288, 2512,  799,  -72,
 /*   490 */   -72,  804,  803, 2557,  809,  -72,  -72,  -72,
};
#define YY_REDUCE_USE_DFLT (-102)
static short yy_reduce_ofst[] = {
 /*     0 */  1332, -102, 1040, -102, -102, -102, -102, -102, -102, -102,
 /*    10 */  -102, -102, 2309, -102, -102, -102,  -94,  208, -102,   98,
 /*    20 */  -102, -102,   74, -102, 1179, -102, 1617, -102, -102, -102,
 /*    30 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*    40 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*    50 */  1688, -102, 1033, 1617, -102, -102, 1360, 1617, -102, 1873,
 /*    60 */  -102,  268, -102,  183, -102, -102,  259, -102, 1435, 1617,
 /*    70 */  -102, 1471, 1617, -102, 1740, -102, 1506, 1617, -102, -102,
 /*    80 */  1105, 1617, -102, 1802, -102, -102,  375, -102,  418,  377,
 /*    90 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   100 */   461, -102,  513, -102,  547, -102,  590, -102,  633, -102,
 /*   110 */   676, -102,  719, -102,  762, -102,  805, -102,  891, -102,
 /*   120 */   934, -102,  986, -102, 1038, -102, 1686, -102, 1800, -102,
 /*   130 */   -55, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   140 */  -102, -102, -102, -102, -102, 1829, -102, -102, -102, -102,
 /*   150 */  1432, -102, 1964, -102, -102, -102,  394, -102, 1962, -102,
 /*   160 */  -102, -102, -102, -102, -102, -102, -102, -102,  -21, -102,
 /*   170 */  -102, -102,   78, -102, -102, -101, -102, -102, -102, -102,
 /*   180 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   190 */  -102, -102, 1394, -102, -102, -102,   83, -102, -102, -102,
 /*   200 */  -102, -102, -102, -102, -102, -102, 1540, -102, -102, -102,
 /*   210 */  -102, 1921, -102, -102, 1578, -102, -102, -102, -102, -102,
 /*   220 */  -102, 1948, -102, 1581, 1617, -102,  190, -102, -102,  441,
 /*   230 */  -102,  442, -102, -102, -102, -102, -102, -102, 1469, -102,
 /*   240 */  -102, -102,  447, 1942, -102,   31,  463, 1642, -102, -102,
 /*   250 */   465, 1149, -102, -102, -102,  473, 1893, -102,  160,  475,
 /*   260 */  1895, -102, -102,  476, 1764, -102,  203,  482, 1757, -102,
 /*   270 */  -102,  515, 1504, -102, -102, -102, -102, 1075, -102, -102,
 /*   280 */   117, -102, -102, -102, -102, -102, -102, -102,  848, -102,
 /*   290 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   300 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   310 */  -102, 1700, -102, -102, -102,  -52, -102, -102,  552, -102,
 /*   320 */  -102,  596, -102, -102,  -35, -102, -102,   51, -102, -102,
 /*   330 */    77, -102, -102,  206, -102, -102,  209, -102, -102,  213,
 /*   340 */  -102, -102,  252, -102, -102,  298, -102, -102,  341, -102,
 /*   350 */  -102, -102, -102, -102, -102, -102, -102, -102,  289, -102,
 /*   360 */  1068, 1617, -102, -102, -102,  246, -102, 1325, 1617, -102,
 /*   370 */   332, -102, 1289, 1617, -102, -102, 1804, -102, -102, -102,
 /*   380 */  -102, -102, 1143, 1617, -102, -102, -102, -102, 1543, 1617,
 /*   390 */  -102, -102, -102, -102, 1251, -102, 1617, -102,  504, -102,
 /*   400 */  -102,  637, -102, -102, -102,  643, -102, -102, -102,  644,
 /*   410 */  -102, -102, -102, -102, -102, -102, -102, -102, -102, -102,
 /*   420 */  -102, -102, -102, -102,   -7,   98, -102, -102,  -12, -102,
 /*   430 */  1397, -102, 1617, -102, -102, -102, 1214, -102, 1617, -102,
 /*   440 */  -102, 1355,  208, -102, -102,   -7, -102, -102,  682, -102,
 /*   450 */  -102, -102, -102,  684, -102, -102, -102,  204,   98, -102,
 /*   460 */  -102,  693, -102, -102,   98, -102, -102,  712, -102, -102,
 /*   470 */  -102, 1136,  -54,  208,  204,  208, -102,  208, -102, -102,
 /*   480 */  2145, -102, -102, -102, -102, -102, -102, 2244, -102, -102,
 /*   490 */  -102, -102, -102, 2366, -102, -102, -102, -102,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   737,  737,  737,  500,  502,  503,  504,  737,  737,  505,
 /*    10 */   737,  737,  737,  737,  506,  510,  737,  737,  531,  737,
 /*    20 */   737,  737,  737,  737,  737,  535,  737,  537,  573,  575,
 /*    30 */   576,  577,  578,  579,  580,  581,  582,  583,  584,  585,
 /*    40 */   586,  587,  588,  589,  590,  591,  737,  592,  737,  593,
 /*    50 */   737,  737,  737,  737,  596,  737,  737,  737,  597,  737,
 /*    60 */   737,  737,  600,  737,  601,  602,  737,  737,  737,  604,
 /*    70 */   737,  737,  737,  607,  737,  737,  737,  737,  609,  737,
 /*    80 */   737,  737,  737,  737,  737,  611,  737,  665,  737,  737,
 /*    90 */   666,  737,  737,  737,  732,  737,  733,  737,  734,  667,
 /*   100 */   737,  668,  737,  669,  737,  670,  737,  671,  737,  672,
 /*   110 */   737,  673,  737,  674,  737,  681,  737,  682,  737,  683,
 /*   120 */   737,  684,  737,  685,  737,  686,  737,  687,  737,  688,
 /*   130 */   737,  562,  563,  564,  565,  566,  567,  568,  569,  570,
 /*   140 */   571,  572,  737,  675,  737,  737,  676,  693,  737,  677,
 /*   150 */   737,  716,  737,  689,  737,  737,  737,  690,  737,  691,
 /*   160 */   692,  694,  695,  696,  697,  698,  699,  700,  737,  701,
 /*   170 */   737,  702,  737,  717,  737,  737,  719,  724,  725,  726,
 /*   180 */   727,  728,  729,  730,  731,  720,  725,  727,  726,  718,
 /*   190 */   737,  703,  737,  704,  737,  705,  737,  708,  709,  714,
 /*   200 */   715,  737,  712,  713,  737,  678,  737,  737,  710,  711,
 /*   210 */   679,  737,  737,  680,  737,  737,  706,  707,  735,  737,
 /*   220 */   737,  737,  737,  737,  737,  612,  737,  737,  616,  737,
 /*   230 */   617,  737,  619,  620,  621,  622,  623,  624,  737,  625,
 /*   240 */   664,  737,  737,  737,  626,  737,  737,  737,  627,  737,
 /*   250 */   737,  737,  628,  737,  737,  737,  737,  629,  737,  737,
 /*   260 */   737,  630,  737,  737,  737,  631,  737,  737,  737,  632,
 /*   270 */   737,  737,  737,  633,  634,  635,  618,  737,  737,  637,
 /*   280 */   737,  638,  640,  639,  737,  641,  737,  642,  737,  737,
 /*   290 */   643,  737,  737,  737,  737,  644,  737,  737,  737,  645,
 /*   300 */   737,  737,  646,  737,  737,  647,  737,  737,  648,  737,
 /*   310 */   737,  737,  737,  737,  649,  737,  737,  650,  737,  660,
 /*   320 */   662,  737,  663,  661,  737,  737,  651,  737,  737,  652,
 /*   330 */   737,  737,  653,  737,  737,  654,  737,  737,  655,  737,
 /*   340 */   737,  656,  737,  737,  657,  737,  737,  658,  737,  737,
 /*   350 */   659,  737,  737,  737,  737,  737,  736,  574,  737,  737,
 /*   360 */   737,  737,  613,  737,  737,  737,  737,  737,  737,  614,
 /*   370 */   737,  737,  737,  737,  615,  737,  737,  737,  610,  608,
 /*   380 */   606,  737,  737,  605,  603,  598,  594,  737,  737,  737,
 /*   390 */   599,  595,  737,  737,  737,  536,  737,  538,  737,  553,
 /*   400 */   555,  737,  558,  737,  556,  737,  559,  737,  557,  737,
 /*   410 */   560,  737,  737,  737,  554,  543,  545,  546,  547,  548,
 /*   420 */   549,  550,  551,  552,  737,  737,  737,  737,  737,  737,
 /*   430 */   737,  539,  737,  541,  737,  737,  737,  540,  737,  542,
 /*   440 */   544,  737,  737,  529,  532,  737,  737,  737,  737,  737,
 /*   450 */   533,  737,  737,  737,  737,  534,  523,  737,  737,  737,
 /*   460 */   525,  737,  737,  527,  737,  737,  526,  737,  737,  528,
 /*   470 */   530,  737,  737,  737,  737,  737,  524,  737,  737,  737,
 /*   480 */   737,  737,  507,  511,  737,  737,  737,  737,  737,  508,
 /*   490 */   512,  737,  737,  737,  737,  509,  513,  501,
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
  "MUL",           "DIV",           "MOD",           "ISSET",       
  "FETCH",         "TYPEOF",        "INSTANCEOF",    "NEW",         
  "NOT",           "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",  
  "DOTCOMMA",      "CLASS",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "EXTENDS",       "ABSTRACT",      "COMMENT",       "ASSIGN",      
  "CONST",         "CONSTANT",      "FUNCTION",      "PARENTHESES_OPEN",
  "INLINE",        "FINAL",         "TYPE_INTEGER",  "TYPE_UINTEGER",
  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",     "TYPE_UCHAR",  
  "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING",   "TYPE_ARRAY",  
  "TYPE_VAR",      "BREAK",         "CONTINUE",      "IF",          
  "ELSE",          "SWITCH",        "CASE",          "COLON",       
  "DEFAULT",       "LOOP",          "WHILE",         "DO",          
  "FOR",           "IN",            "REVERSE",       "LET",         
  "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",     "DIVASSIGN",   
  "CONCATASSIGN",  "ARROW",         "SBRACKET_OPEN",  "SBRACKET_CLOSE",
  "DOUBLECOLON",   "INCR",          "DECR",          "ECHO",        
  "RETURN",        "UNSET",         "INTEGER",       "STRING",      
  "THROW",         "SCHAR",         "DOUBLE",        "NULL",        
  "TRUE",          "FALSE",         "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_comment",    "xx_class_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",
  "xx_literal_expr",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_visibility",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_mcall_statement",
  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",
  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",
  "xx_switch_statement",  "xx_for_statement",  "xx_eval_expr",  "xx_case_clauses",
  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",
  "xx_assignment_operator",  "xx_assign_expr",  "xx_index_expr",  "xx_echo_expressions",
  "xx_echo_expression",  "xx_mcall_expr",  "xx_scall_expr",  "xx_call_parameters",
  "xx_declare_variable_list",  "xx_declare_variable",  "xx_isset_expr",  "xx_array_list",
  "xx_call_parameter",  "xx_array_item",  "xx_array_key",  "xx_array_value",
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
 /* 182 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 183 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 184 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 185 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 186 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 187 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 188 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 189 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 190 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 191 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 192 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 193 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 194 */ "xx_common_expr ::= CONSTANT",
 /* 195 */ "xx_common_expr ::= IDENTIFIER",
 /* 196 */ "xx_common_expr ::= INTEGER",
 /* 197 */ "xx_common_expr ::= STRING",
 /* 198 */ "xx_common_expr ::= SCHAR",
 /* 199 */ "xx_common_expr ::= DOUBLE",
 /* 200 */ "xx_common_expr ::= NULL",
 /* 201 */ "xx_common_expr ::= TRUE",
 /* 202 */ "xx_common_expr ::= FALSE",
 /* 203 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 204 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 205 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 206 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 207 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 208 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 209 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 210 */ "xx_common_expr ::= xx_mcall_expr",
 /* 211 */ "xx_common_expr ::= xx_scall_expr",
 /* 212 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 213 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 214 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 215 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 216 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 217 */ "xx_call_parameters ::= xx_call_parameter",
 /* 218 */ "xx_call_parameter ::= xx_common_expr",
 /* 219 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 220 */ "xx_array_list ::= xx_array_item",
 /* 221 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 222 */ "xx_array_item ::= xx_array_value",
 /* 223 */ "xx_array_key ::= IDENTIFIER",
 /* 224 */ "xx_array_key ::= STRING",
 /* 225 */ "xx_array_key ::= INTEGER",
 /* 226 */ "xx_array_value ::= xx_literal_expr",
 /* 227 */ "xx_array_value ::= IDENTIFIER",
 /* 228 */ "xx_literal_expr ::= INTEGER",
 /* 229 */ "xx_literal_expr ::= STRING",
 /* 230 */ "xx_literal_expr ::= DOUBLE",
 /* 231 */ "xx_literal_expr ::= NULL",
 /* 232 */ "xx_literal_expr ::= FALSE",
 /* 233 */ "xx_literal_expr ::= TRUE",
 /* 234 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 235 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 236 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 237 */ "xx_eval_expr ::= xx_common_expr",
 /* 238 */ "xx_comment ::= COMMENT",
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
    case 93:
// 836 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2119 "parser.c"
      break;
    case 96:
// 849 "parser.lemon"
{ json_object_put((yypminor->yy103)); }
// 2124 "parser.c"
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
  { 95, 1 },
  { 96, 1 },
  { 97, 2 },
  { 97, 1 },
  { 98, 1 },
  { 98, 1 },
  { 98, 1 },
  { 99, 3 },
  { 100, 5 },
  { 100, 7 },
  { 100, 6 },
  { 100, 8 },
  { 100, 4 },
  { 100, 6 },
  { 100, 5 },
  { 100, 7 },
  { 102, 1 },
  { 102, 1 },
  { 102, 1 },
  { 102, 2 },
  { 102, 2 },
  { 102, 2 },
  { 102, 2 },
  { 102, 3 },
  { 102, 3 },
  { 103, 2 },
  { 103, 1 },
  { 106, 4 },
  { 106, 3 },
  { 106, 6 },
  { 106, 5 },
  { 104, 2 },
  { 104, 1 },
  { 105, 2 },
  { 105, 1 },
  { 109, 6 },
  { 109, 5 },
  { 110, 7 },
  { 110, 8 },
  { 110, 8 },
  { 110, 9 },
  { 110, 8 },
  { 110, 9 },
  { 110, 9 },
  { 110, 10 },
  { 107, 2 },
  { 107, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 113, 1 },
  { 111, 3 },
  { 111, 1 },
  { 114, 1 },
  { 114, 2 },
  { 114, 2 },
  { 114, 3 },
  { 114, 4 },
  { 114, 4 },
  { 116, 3 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 115, 1 },
  { 112, 2 },
  { 112, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 117, 1 },
  { 128, 2 },
  { 129, 2 },
  { 119, 4 },
  { 119, 7 },
  { 119, 5 },
  { 119, 9 },
  { 119, 8 },
  { 119, 8 },
  { 132, 4 },
  { 132, 5 },
  { 135, 2 },
  { 135, 1 },
  { 136, 4 },
  { 136, 3 },
  { 120, 3 },
  { 120, 4 },
  { 130, 4 },
  { 130, 5 },
  { 131, 6 },
  { 131, 7 },
  { 133, 7 },
  { 133, 8 },
  { 133, 9 },
  { 133, 10 },
  { 118, 3 },
  { 138, 3 },
  { 138, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 140, 1 },
  { 139, 3 },
  { 139, 5 },
  { 139, 7 },
  { 139, 8 },
  { 139, 5 },
  { 139, 7 },
  { 139, 8 },
  { 139, 5 },
  { 139, 6 },
  { 139, 2 },
  { 139, 2 },
  { 142, 1 },
  { 121, 3 },
  { 143, 3 },
  { 143, 1 },
  { 144, 1 },
  { 123, 2 },
  { 124, 2 },
  { 122, 3 },
  { 125, 5 },
  { 125, 6 },
  { 125, 6 },
  { 125, 6 },
  { 126, 3 },
  { 126, 7 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 127, 3 },
  { 148, 3 },
  { 148, 1 },
  { 149, 1 },
  { 149, 3 },
  { 141, 1 },
  { 137, 2 },
  { 137, 2 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 4 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 4 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 3 },
  { 137, 2 },
  { 137, 4 },
  { 137, 2 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 1 },
  { 137, 2 },
  { 137, 3 },
  { 137, 2 },
  { 137, 4 },
  { 137, 5 },
  { 137, 4 },
  { 137, 3 },
  { 137, 1 },
  { 137, 1 },
  { 146, 6 },
  { 146, 5 },
  { 145, 6 },
  { 145, 5 },
  { 147, 3 },
  { 147, 1 },
  { 152, 1 },
  { 151, 3 },
  { 151, 1 },
  { 153, 3 },
  { 153, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 155, 1 },
  { 155, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 108, 1 },
  { 150, 4 },
  { 150, 4 },
  { 150, 4 },
  { 134, 1 },
  { 101, 1 },
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
	status->ret = yymsp[0].minor.yy103;
}
// 2580 "parser.c"
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
      case 210:
      case 211:
      case 218:
      case 226:
      case 237:
// 851 "parser.lemon"
{
	yygotominor.yy103 = yymsp[0].minor.yy103;
}
// 2614 "parser.c"
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
	yygotominor.yy103 = xx_ret_list(yymsp[-1].minor.yy103, yymsp[0].minor.yy103);
}
// 2627 "parser.c"
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
      case 217:
      case 220:
// 859 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_list(NULL, yymsp[0].minor.yy103);
}
// 2646 "parser.c"
        break;
      case 7:
// 875 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2655 "parser.c"
        break;
      case 8:
// 879 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, 0, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2665 "parser.c"
        break;
      case 9:
// 883 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy103, 0, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2676 "parser.c"
        break;
      case 10:
// 887 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, 1, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2687 "parser.c"
        break;
      case 11:
// 891 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[-1].minor.yy103, 1, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-7].minor);
  yy_destructor(33,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2699 "parser.c"
        break;
      case 12:
// 895 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2709 "parser.c"
        break;
      case 13:
// 899 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 0, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2720 "parser.c"
        break;
      case 14:
// 903 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 1, NULL, status->scanner_state);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2731 "parser.c"
        break;
      case 15:
// 907 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-4].minor.yy0, NULL, 1, yymsp[-2].minor.yy0, status->scanner_state);
  yy_destructor(37,&yymsp[-6].minor);
  yy_destructor(33,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2743 "parser.c"
        break;
      case 16:
// 911 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
}
// 2750 "parser.c"
        break;
      case 17:
// 915 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 2757 "parser.c"
        break;
      case 18:
// 919 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, yymsp[0].minor.yy103, NULL, status->scanner_state);
}
// 2764 "parser.c"
        break;
      case 19:
// 923 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
}
// 2771 "parser.c"
        break;
      case 20:
// 927 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 2778 "parser.c"
        break;
      case 21:
// 931 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[0].minor.yy103, NULL, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2785 "parser.c"
        break;
      case 22:
// 935 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, yymsp[0].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2792 "parser.c"
        break;
      case 23:
// 939 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2799 "parser.c"
        break;
      case 24:
// 943 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, yymsp[0].minor.yy103, yymsp[-2].minor.yy103, status->scanner_state);
}
// 2806 "parser.c"
        break;
      case 27:
// 955 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-2].minor.yy103, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2814 "parser.c"
        break;
      case 28:
// 959 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-2].minor.yy103, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2822 "parser.c"
        break;
      case 29:
// 963 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-4].minor.yy103, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2831 "parser.c"
        break;
      case 30:
// 967 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-4].minor.yy103, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2840 "parser.c"
        break;
      case 35:
// 987 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2850 "parser.c"
        break;
      case 36:
// 991 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2860 "parser.c"
        break;
      case 37:
// 995 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-6].minor.yy103, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2872 "parser.c"
        break;
      case 38:
// 999 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-7].minor.yy103, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2884 "parser.c"
        break;
      case 39:
// 1003 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-7].minor.yy103, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2896 "parser.c"
        break;
      case 40:
// 1007 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-8].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy103, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-7].minor);
  yy_destructor(43,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2908 "parser.c"
        break;
      case 41:
// 1011 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-6].minor.yy103, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2920 "parser.c"
        break;
      case 42:
// 1015 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-7].minor.yy103, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2932 "parser.c"
        break;
      case 43:
// 1019 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-7].minor.yy103, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy103, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2944 "parser.c"
        break;
      case 44:
// 1023 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_method(yymsp[-8].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy103, yymsp[-1].minor.yy103, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-7].minor);
  yy_destructor(43,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2956 "parser.c"
        break;
      case 47:
// 1035 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2964 "parser.c"
        break;
      case 48:
// 1039 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2972 "parser.c"
        break;
      case 49:
// 1043 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2980 "parser.c"
        break;
      case 50:
// 1047 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2988 "parser.c"
        break;
      case 51:
// 1051 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 2996 "parser.c"
        break;
      case 52:
// 1055 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("inline");
  yy_destructor(44,&yymsp[0].minor);
}
// 3004 "parser.c"
        break;
      case 53:
// 1059 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("abstract");
  yy_destructor(37,&yymsp[0].minor);
}
// 3012 "parser.c"
        break;
      case 54:
// 1063 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("final");
  yy_destructor(45,&yymsp[0].minor);
}
// 3020 "parser.c"
        break;
      case 55:
      case 119:
      case 140:
      case 162:
      case 216:
      case 219:
// 1067 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_list(yymsp[-2].minor.yy103, yymsp[0].minor.yy103);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3033 "parser.c"
        break;
      case 57:
// 1075 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3040 "parser.c"
        break;
      case 58:
// 1079 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(yymsp[-1].minor.yy103, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3047 "parser.c"
        break;
      case 59:
// 1083 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, yymsp[-1].minor.yy103, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3054 "parser.c"
        break;
      case 60:
// 1087 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3062 "parser.c"
        break;
      case 61:
// 1091 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(yymsp[-3].minor.yy103, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3070 "parser.c"
        break;
      case 62:
// 1095 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, yymsp[-3].minor.yy103, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3078 "parser.c"
        break;
      case 63:
// 1099 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 3087 "parser.c"
        break;
      case 64:
// 1103 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(46,&yymsp[0].minor);
}
// 3095 "parser.c"
        break;
      case 65:
// 1107 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(47,&yymsp[0].minor);
}
// 3103 "parser.c"
        break;
      case 66:
// 1111 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(48,&yymsp[0].minor);
}
// 3111 "parser.c"
        break;
      case 67:
// 1115 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(49,&yymsp[0].minor);
}
// 3119 "parser.c"
        break;
      case 68:
// 1119 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(50,&yymsp[0].minor);
}
// 3127 "parser.c"
        break;
      case 69:
// 1123 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(51,&yymsp[0].minor);
}
// 3135 "parser.c"
        break;
      case 70:
// 1127 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(52,&yymsp[0].minor);
}
// 3143 "parser.c"
        break;
      case 71:
// 1131 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(53,&yymsp[0].minor);
}
// 3151 "parser.c"
        break;
      case 72:
// 1135 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(54,&yymsp[0].minor);
}
// 3159 "parser.c"
        break;
      case 73:
// 1139 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(55,&yymsp[0].minor);
}
// 3167 "parser.c"
        break;
      case 74:
// 1143 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3175 "parser.c"
        break;
      case 94:
// 1223 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3184 "parser.c"
        break;
      case 95:
// 1227 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3193 "parser.c"
        break;
      case 96:
// 1231 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-2].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3203 "parser.c"
        break;
      case 97:
// 1235 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-5].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3216 "parser.c"
        break;
      case 98:
// 1239 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3226 "parser.c"
        break;
      case 99:
// 1243 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-7].minor.yy103, yymsp[-5].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(59,&yymsp[-8].minor);
  yy_destructor(34,&yymsp[-6].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3239 "parser.c"
        break;
      case 100:
// 1247 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-6].minor.yy103, yymsp[-4].minor.yy103, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3252 "parser.c"
        break;
      case 101:
// 1251 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-6].minor.yy103, NULL, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3265 "parser.c"
        break;
      case 102:
// 1255 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_switch_statement(yymsp[-2].minor.yy103, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3275 "parser.c"
        break;
      case 103:
// 1259 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_switch_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3285 "parser.c"
        break;
      case 106:
// 1271 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_case_clause(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3294 "parser.c"
        break;
      case 107:
// 1275 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_case_clause(NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3303 "parser.c"
        break;
      case 108:
// 1279 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3313 "parser.c"
        break;
      case 109:
// 1283 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_loop_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3323 "parser.c"
        break;
      case 110:
// 1287 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_while_statement(yymsp[-2].minor.yy103, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3333 "parser.c"
        break;
      case 111:
// 1291 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_while_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3343 "parser.c"
        break;
      case 112:
// 1295 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_do_while_statement(yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(34,&yymsp[-4].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3355 "parser.c"
        break;
      case 113:
// 1299 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_do_while_statement(yymsp[-1].minor.yy103, yymsp[-4].minor.yy103, status->scanner_state);
  yy_destructor(67,&yymsp[-6].minor);
  yy_destructor(34,&yymsp[-5].minor);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3367 "parser.c"
        break;
      case 114:
// 1303 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_for_statement(yymsp[-3].minor.yy103, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(68,&yymsp[-6].minor);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3378 "parser.c"
        break;
      case 115:
// 1307 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_for_statement(yymsp[-3].minor.yy103, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(68,&yymsp[-7].minor);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3390 "parser.c"
        break;
      case 116:
// 1311 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_for_statement(yymsp[-3].minor.yy103, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(68,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3402 "parser.c"
        break;
      case 117:
// 1315 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_for_statement(yymsp[-3].minor.yy103, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(68,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3415 "parser.c"
        break;
      case 118:
// 1319 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3424 "parser.c"
        break;
      case 121:
// 1332 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("assign");
  yy_destructor(39,&yymsp[0].minor);
}
// 3432 "parser.c"
        break;
      case 122:
// 1337 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("add-assign");
  yy_destructor(72,&yymsp[0].minor);
}
// 3440 "parser.c"
        break;
      case 123:
// 1342 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("sub-assign");
  yy_destructor(73,&yymsp[0].minor);
}
// 3448 "parser.c"
        break;
      case 124:
// 1346 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("mult-assign");
  yy_destructor(74,&yymsp[0].minor);
}
// 3456 "parser.c"
        break;
      case 125:
// 1350 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("div-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3464 "parser.c"
        break;
      case 126:
// 1354 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("concat-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3472 "parser.c"
        break;
      case 127:
// 1359 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy103, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 3479 "parser.c"
        break;
      case 128:
// 1364 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
}
// 3487 "parser.c"
        break;
      case 129:
// 1369 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3497 "parser.c"
        break;
      case 130:
// 1374 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy103, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3507 "parser.c"
        break;
      case 131:
// 1379 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3515 "parser.c"
        break;
      case 132:
// 1384 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3525 "parser.c"
        break;
      case 133:
// 1389 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy103, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3535 "parser.c"
        break;
      case 134:
// 1394 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3544 "parser.c"
        break;
      case 135:
// 1399 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3553 "parser.c"
        break;
      case 136:
// 1404 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[0].minor);
}
// 3561 "parser.c"
        break;
      case 137:
// 1409 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[0].minor);
}
// 3569 "parser.c"
        break;
      case 139:
// 1417 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_echo_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3578 "parser.c"
        break;
      case 142:
// 1429 "parser.lemon"
{
	yygotominor.yy103 = yymsp[0].minor.yy103;;
}
// 3585 "parser.c"
        break;
      case 143:
// 1434 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3593 "parser.c"
        break;
      case 144:
// 1439 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3601 "parser.c"
        break;
      case 145:
// 1444 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_return_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3610 "parser.c"
        break;
      case 146:
// 1449 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-4].minor);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3624 "parser.c"
        break;
      case 147:
// 1458 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3639 "parser.c"
        break;
      case 148:
// 1467 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3654 "parser.c"
        break;
      case 149:
// 1476 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3669 "parser.c"
        break;
      case 150:
// 1485 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_throw_exception(yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3678 "parser.c"
        break;
      case 151:
// 1490 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_throw_exception(yymsp[-4].minor.yy0, yymsp[-2].minor.yy103, status->scanner_state);
  yy_destructor(88,&yymsp[-6].minor);
  yy_destructor(27,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3690 "parser.c"
        break;
      case 152:
// 1494 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3699 "parser.c"
        break;
      case 153:
// 1498 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3708 "parser.c"
        break;
      case 154:
// 1502 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3717 "parser.c"
        break;
      case 155:
// 1506 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3726 "parser.c"
        break;
      case 156:
// 1510 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3735 "parser.c"
        break;
      case 157:
// 1514 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3744 "parser.c"
        break;
      case 158:
// 1518 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3753 "parser.c"
        break;
      case 159:
// 1522 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3762 "parser.c"
        break;
      case 160:
// 1526 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3771 "parser.c"
        break;
      case 161:
// 1530 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3780 "parser.c"
        break;
      case 164:
// 1542 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3787 "parser.c"
        break;
      case 165:
// 1546 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3795 "parser.c"
        break;
      case 167:
// 1554 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 3803 "parser.c"
        break;
      case 168:
// 1558 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("isset", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3811 "parser.c"
        break;
      case 169:
// 1562 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("equals", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3819 "parser.c"
        break;
      case 170:
// 1566 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not-equals", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3827 "parser.c"
        break;
      case 171:
// 1570 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("identical", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3835 "parser.c"
        break;
      case 172:
// 1574 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not-identical", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3843 "parser.c"
        break;
      case 173:
// 1578 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("less", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3851 "parser.c"
        break;
      case 174:
// 1582 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("greater", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3859 "parser.c"
        break;
      case 175:
// 1586 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("less-equal", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3867 "parser.c"
        break;
      case 176:
// 1590 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3875 "parser.c"
        break;
      case 177:
// 1594 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("list", yymsp[-1].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3884 "parser.c"
        break;
      case 178:
// 1598 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("cast", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
}
// 3893 "parser.c"
        break;
      case 179:
// 1602 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 3901 "parser.c"
        break;
      case 180:
// 1606 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 3909 "parser.c"
        break;
      case 181:
// 1610 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 3917 "parser.c"
        break;
      case 182:
// 1615 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 3926 "parser.c"
        break;
      case 183:
// 1620 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("add", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3934 "parser.c"
        break;
      case 184:
// 1625 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("sub", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3942 "parser.c"
        break;
      case 185:
// 1630 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("mul", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 3950 "parser.c"
        break;
      case 186:
// 1635 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("div", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3958 "parser.c"
        break;
      case 187:
// 1640 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("mod", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3966 "parser.c"
        break;
      case 188:
// 1645 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("concat", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3974 "parser.c"
        break;
      case 189:
// 1650 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("and", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3982 "parser.c"
        break;
      case 190:
// 1655 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("or", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3990 "parser.c"
        break;
      case 191:
// 1660 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("instanceof", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 3998 "parser.c"
        break;
      case 192:
// 1665 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4007 "parser.c"
        break;
      case 193:
// 1670 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("typeof", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4015 "parser.c"
        break;
      case 194:
// 1675 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4022 "parser.c"
        break;
      case 195:
      case 223:
      case 227:
// 1680 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4031 "parser.c"
        break;
      case 196:
      case 225:
      case 228:
// 1685 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4040 "parser.c"
        break;
      case 197:
      case 224:
      case 229:
// 1690 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4049 "parser.c"
        break;
      case 198:
// 1695 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_SCHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4056 "parser.c"
        break;
      case 199:
      case 230:
// 1700 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4064 "parser.c"
        break;
      case 200:
      case 231:
// 1705 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[0].minor);
}
// 4073 "parser.c"
        break;
      case 201:
      case 233:
// 1710 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4082 "parser.c"
        break;
      case 202:
      case 232:
// 1715 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4091 "parser.c"
        break;
      case 203:
// 1720 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-1].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4100 "parser.c"
        break;
      case 204:
// 1725 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array", yymsp[-1].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4109 "parser.c"
        break;
      case 205:
// 1730 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4117 "parser.c"
        break;
      case 206:
// 1735 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4127 "parser.c"
        break;
      case 207:
// 1740 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4137 "parser.c"
        break;
      case 208:
// 1745 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4146 "parser.c"
        break;
      case 209:
// 1750 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4155 "parser.c"
        break;
      case 212:
// 1765 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4165 "parser.c"
        break;
      case 213:
// 1770 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4175 "parser.c"
        break;
      case 214:
// 1775 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4185 "parser.c"
        break;
      case 215:
// 1780 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4195 "parser.c"
        break;
      case 221:
// 1804 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_array_item(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
}
// 4203 "parser.c"
        break;
      case 222:
// 1808 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_array_item(NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 4210 "parser.c"
        break;
      case 234:
// 1857 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4219 "parser.c"
        break;
      case 235:
// 1862 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4228 "parser.c"
        break;
      case 236:
// 1867 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4237 "parser.c"
        break;
      case 238:
// 1875 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4244 "parser.c"
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

// 4320 "parser.c"
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
			case XX_T_MOD:
				xx_(xx_parser, XX_MOD, NULL, parser_status);
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
