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
		case XX_T_CHAR:
			json_object_object_add(ret, "type", json_object_new_string("char"));
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
	if (expr) {
		json_object_object_add(ret, "expr", expr);
	}

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

static json_object *xx_ret_throw_exception(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("throw"));
	if (expr) {
		json_object_object_add(ret, "expr", expr);
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
#define YYNOCODE 158
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy75;
  int yy315;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 485
#define YYNRULE 237
#define YYERRORSYMBOL 94
#define YYERRSYMDT yy315
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
 /*     0 */    93,  158,  162,  156,  194,   90,  339,  322,  151,  471,
 /*    10 */   485,  474,  475,  463,  304,  442,  443,  236,  164,  439,
 /*    20 */   134,  231,  280,  135,  136,  137,  138,  139,  140,  141,
 /*    30 */   142,  143,  144,  145,  130,  132,   92,  106,  110,  112,
 /*    40 */   114,  116,  108,  104,  118,  120,  128,  122,  124,  126,
 /*    50 */   237,  238,  239,  240,  241,  172,  262,  391,   12,  294,
 /*    60 */   413,  402,  406,  165,  166,  350,  167,  168,  169,  170,
 /*    70 */   171,   29,  355,  433,  236,  200,  413,  402,  406,   68,
 /*    80 */   314,  323,  332,  335,  326,  329,  338,  344,  341,    8,
 /*    90 */   347,   50,   52,   54,   13,   63,   16,  190,  212,   74,
 /*   100 */    78,   83,  223,    9,   70,  230,  380,  237,  238,  239,
 /*   110 */   240,  241,   11,  249,  350,   14,  148,  281,  292,  296,
 /*   120 */   396,  355,  311,  118,  120,  128,  122,  124,  126,  314,
 /*   130 */   323,  332,  335,  326,  329,  338,  344,  341,  146,  347,
 /*   140 */    50,   52,   54,   16,   63,  173,  201,  202,   74,   78,
 /*   150 */    83,  223,  192,  191,  230,   15,  185,  186,  188,  187,
 /*   160 */    18,  376,  353,  350,  222,  351,  281,  292,  296,  394,
 /*   170 */   355,  311,  201,  202,  298,  301,  181,  176,  314,  323,
 /*   180 */   332,  335,  326,  329,  338,  344,  341,  155,  347,   50,
 /*   190 */    52,   54,  481,   63,   16,  201,  202,   74,   78,   83,
 /*   200 */   223,  205,  204,  230,  723,    1,    2,  484,    4,    5,
 /*   210 */     6,  362,  350,  397,  414,  281,  292,  296,  373,  355,
 /*   220 */   311,  177,  178,  189,  122,  124,  126,  314,  323,  332,
 /*   230 */   335,  326,  329,  338,  344,  341,  392,  347,   50,   52,
 /*   240 */    54,   25,   63,  181,  182,  424,   74,   78,   83,  223,
 /*   250 */   175,  439,  230,   28,  440,   21,  455,  463,   26,  469,
 /*   260 */   443,  350,  397,  439,  281,  292,  296,  389,  355,  311,
 /*   270 */     3,    4,    5,    6,  225,   51,  314,  323,  332,  335,
 /*   280 */   326,  329,  338,  344,  341,  434,  347,   50,   52,   54,
 /*   290 */   180,   63,   53,  190,   56,   74,   78,   83,  223,  183,
 /*   300 */   184,  230,  465,  185,  186,  188,  187,   59,   55,  466,
 /*   310 */   350,  222,   60,  281,  292,  296,  390,  355,  311,  201,
 /*   320 */   202,  398,  402,  406,   65,  314,  323,  332,  335,  326,
 /*   330 */   329,  338,  344,  341,  257,  347,   50,   52,   54,  274,
 /*   340 */    63,  209,  201,  202,   74,   78,   83,  223,  192,  191,
 /*   350 */   230,  214,  185,  186,  188,  187,  336,  322,   64,  350,
 /*   360 */    71,  222,  281,  292,  296,   86,  355,  311,  236,  201,
 /*   370 */   202,   69,  333,  322,  314,  323,  332,  335,  326,  329,
 /*   380 */   338,  344,  341,  155,  347,   50,   52,   54,   72,   63,
 /*   390 */    75,  201,  202,   74,   78,   83,  223,  219,  204,  230,
 /*   400 */   457,  237,  238,  239,  240,  241,  439,   80,  350,   84,
 /*   410 */   286,  281,  292,  296,   77,  355,  311,  285,  201,  202,
 /*   420 */    89,  330,  322,  314,  323,  332,  335,  326,  329,  338,
 /*   430 */   344,  341,  257,  347,   50,   52,   54,  253,   63,   95,
 /*   440 */   201,  202,   74,   78,   83,  223,  183,  184,  230,  459,
 /*   450 */   185,  186,  188,  187,  181,   79,  460,  350,  222,   87,
 /*   460 */   281,  292,  296,  374,  355,  311,  201,  202,  218,  317,
 /*   470 */   345,  322,  314,  323,  332,  335,  326,  329,  338,  344,
 /*   480 */   341,  257,  347,   50,   52,   54,  266,   63,   94,  201,
 /*   490 */   202,   74,   78,   83,  223,  340,  174,  230,   98,  193,
 /*   500 */   178,  189,  152,  215,   88,  208,  350,  222,   96,  281,
 /*   510 */   292,  296,  379,  355,  311,  201,  202,  100,  315,  322,
 /*   520 */   149,  314,  323,  332,  335,  326,  329,  338,  344,  341,
 /*   530 */   155,  347,   50,   52,   54,  102,   63,  153,  201,  202,
 /*   540 */    74,   78,   83,  223,  286,  203,  230,  327,  322,  159,
 /*   550 */   282,  287,  201,  202,  154,  350,  226,  160,  281,  292,
 /*   560 */   296,  229,  355,  311,  201,  202,  161,  233,  348,  322,
 /*   570 */   314,  323,  332,  335,  326,  329,  338,  344,  341,  312,
 /*   580 */   347,   50,   52,   54,  179,   63,  706,  201,  202,   74,
 /*   590 */    78,   83,  223,  232,  441,  230,   23,  244,  442,  443,
 /*   600 */   707,  273,  439,  708,  350,  201,  202,  281,  292,  296,
 /*   610 */    82,  355,  311,  342,  322,   67,  383,  195,   66,  314,
 /*   620 */   323,  332,  335,  326,  329,  338,  344,  341,  293,  347,
 /*   630 */    50,   52,   54,  224,   63,  196,  201,  202,   74,   78,
 /*   640 */    83,  223,  244,   70,  230,  380,  277,  472,  455,  463,
 /*   650 */   201,  202,  443,  350,  210,  439,  281,  292,  296,  361,
 /*   660 */   355,  311,  486,  324,  322,  235,  234,  246,  314,  323,
 /*   670 */   332,  335,  326,  329,  338,  344,  341,  103,  347,   50,
 /*   680 */    52,   54,  289,   63,  242,  201,  202,   74,   78,   83,
 /*   690 */   223,    7,    7,  230,   10,   10,  477,  477,   23,  355,
 /*   700 */   355,   22,  350,  105,  439,  281,  292,  296,   62,  355,
 /*   710 */   311,  201,  202,  247,  254,  251,  255,  314,  323,  332,
 /*   720 */   335,  326,  329,  338,  344,  341,   91,  347,   50,   52,
 /*   730 */    54,  267,   63,  259,  201,  202,   74,   78,   83,  223,
 /*   740 */    97,  244,  230,  275,  244,  265,  260,  291,  248,  201,
 /*   750 */   202,  350,  201,  202,  281,  292,  296,  378,  355,  311,
 /*   760 */   264,  268,  272,  297,  284,  276,  314,  323,  332,  335,
 /*   770 */   326,  329,  338,  344,  341,  107,  347,   50,   52,   54,
 /*   780 */   299,   63,  303,  201,  202,   74,   78,   83,  223,  244,
 /*   790 */   283,  230,  300,  256,  307,   99,  101,  201,  202,  310,
 /*   800 */   350,  109,  306,  281,  292,  296,  384,  355,  311,  201,
 /*   810 */   202,  319,  309,  317,  318,  314,  323,  332,  335,  326,
 /*   820 */   329,  338,  344,  341,  111,  347,   50,   52,   54,  320,
 /*   830 */    63,  321,  201,  202,   74,   78,   83,  223,  244,  316,
 /*   840 */   230,  364,  269,  352,  354,  363,  201,  202,  375,  350,
 /*   850 */   113,  377,  281,  292,  296,   58,  355,  311,  201,  202,
 /*   860 */   381,  386,  387,  393,  314,  323,  332,  335,  326,  329,
 /*   870 */   338,  344,  341,  115,  347,   50,   52,   54,  400,   63,
 /*   880 */   401,  201,  202,   74,   78,   83,  223,  302,  244,  230,
 /*   890 */   403,  244,  252,  404,  405,  261,  201,  202,  350,  201,
 /*   900 */   202,  281,  292,  296,  438,  355,  311,  407,  408,  409,
 /*   910 */   411,  317,  546,  314,  323,  332,  335,  326,  329,  338,
 /*   920 */   344,  341,  117,  347,   50,   52,   54,  412,   63,  426,
 /*   930 */   201,  202,   74,   78,   83,  223,  244,  337,  230,  448,
 /*   940 */   243,  447,  305,  308,  201,  202,  427,  350,  119,  429,
 /*   950 */   281,  292,  296,   31,  355,  311,  201,  202,  435,  446,
 /*   960 */   317,  449,  314,  323,  332,  335,  326,  329,  338,  344,
 /*   970 */   341,  121,  347,   50,   52,   54,  451,   63,  452,  201,
 /*   980 */   202,   74,   78,   83,  223,  123,  334,  230,  454,  125,
 /*   990 */   462,  468,  453,  201,  202,  461,  350,  201,  202,  281,
 /*  1000 */   292,  296,  436,  355,  311,  478,  467,  479,  480,  317,
 /*  1010 */   482,  314,  323,  332,  335,  326,  329,  338,  344,  341,
 /*  1020 */   358,  347,   50,   52,   54,  483,   63,  396,  201,  202,
 /*  1030 */    74,   78,   83,  223,  127,  331,  230,  396,  129,  396,
 /*  1040 */   396,  396,  201,  202,  396,  350,  201,  202,  281,  292,
 /*  1050 */   296,  432,  355,  311,  396,  396,  396,  396,  396,  396,
 /*  1060 */   314,  323,  332,  335,  326,  329,  338,  344,  341,  365,
 /*  1070 */   347,   50,   52,   54,  396,   63,  396,  201,  202,   74,
 /*  1080 */    78,   83,  223,  131,  200,  230,  396,  370,  396,  396,
 /*  1090 */   396,  201,  202,  396,  350,  201,  202,  281,  292,  296,
 /*  1100 */   430,  355,  311,  396,  396,  396,  396,  220,  396,  314,
 /*  1110 */   323,  332,  335,  326,  329,  338,  344,  341,  133,  347,
 /*  1120 */    50,   52,   54,  396,   63,  396,  201,  202,   74,   78,
 /*  1130 */    83,  223,  216,  396,  230,  396,  163,  396,  396,  396,
 /*  1140 */   201,  202,  396,  350,  201,  202,  281,  292,  296,  385,
 /*  1150 */   355,  311,  396,  396,  396,  396,  317,  396,  314,  323,
 /*  1160 */   332,  335,  326,  329,  338,  344,  341,  150,  347,   50,
 /*  1170 */    52,   54,  396,   63,  396,  201,  202,   74,   78,   83,
 /*  1180 */   223,  157,  328,  230,  396,  396,  396,  396,  396,  201,
 /*  1190 */   202,  396,  350,  396,  396,  281,  292,  296,  368,  355,
 /*  1200 */   311,  396,  396,  396,  396,  317,  396,  314,  323,  332,
 /*  1210 */   335,  326,  329,  338,  344,  341,  317,  347,   50,   52,
 /*  1220 */    54,  396,   63,  396,  317,  317,   74,   78,   83,  223,
 /*  1230 */   396,  346,  230,  396,  396,  396,  396,  396,  396,  396,
 /*  1240 */   350,  396,  349,  396,  281,  292,  296,  355,  396,  311,
 /*  1250 */   343,  325,  396,  396,  396,  314,  323,  332,  335,  326,
 /*  1260 */   329,  338,  344,  341,  396,  347,   50,   52,   54,  396,
 /*  1270 */    63,  396,  200,  155,   74,   78,   83,  223,  396,  396,
 /*  1280 */   230,  201,  202,  396,  200,  396,  396,  211,  204,  396,
 /*  1290 */   396,  396,  281,  292,  296,  199,  396,  311,   49,  396,
 /*  1300 */   396,   17,   20,  470,  476,  475,  463,  206,  469,  443,
 /*  1310 */    73,  396,  439,  396,  396,  356,   33,   34,   35,   36,
 /*  1320 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*  1330 */    47,   48,  396,  396,   49,  396,  396,  396,  396,  396,
 /*  1340 */   396,  396,  396,  288,  290,  396,  437,  396,  396,  396,
 /*  1350 */   396,  356,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1360 */    41,   42,   43,   44,   45,   46,   47,   48,  155,   49,
 /*  1370 */   396,  396,  396,  396,  396,  396,  201,  202,  396,  288,
 /*  1380 */   290,  382,  198,  204,  236,  396,  356,   33,   34,   35,
 /*  1390 */    36,   37,   38,   39,   40,   41,   42,   43,   44,   45,
 /*  1400 */    46,   47,   48,  396,  396,  396,  396,  396,  396,  396,
 /*  1410 */   396,  396,   49,  396,  288,  290,  396,  237,  238,  239,
 /*  1420 */   240,  241,  245,  270,  360,  258,  278,  279,  396,  356,
 /*  1430 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1440 */    43,   44,   45,   46,   47,   48,  396,  396,   49,  396,
 /*  1450 */   396,  396,  396,  396,  396,  396,  396,  288,  290,  396,
 /*  1460 */    81,  396,  396,  396,  396,  356,   33,   34,   35,   36,
 /*  1470 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*  1480 */    47,   48,  396,   49,  396,  396,  396,  396,  396,  396,
 /*  1490 */   396,  396,  396,  288,  290,  388,  396,  396,  396,  396,
 /*  1500 */   356,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1510 */    42,   43,   44,   45,   46,   47,   48,  396,  396,  396,
 /*  1520 */   396,  396,  396,  396,  396,  396,   49,  396,  288,  290,
 /*  1530 */   396,  396,  396,  396,  396,  396,  396,  396,  228,  396,
 /*  1540 */   396,  396,  396,  356,   33,   34,   35,   36,   37,   38,
 /*  1550 */    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*  1560 */   396,  396,   49,  396,  396,  396,  396,  396,  396,  396,
 /*  1570 */   396,  288,  290,  396,  367,  396,  396,  396,  396,  356,
 /*  1580 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1590 */    43,   44,   45,   46,   47,   48,  396,   49,  396,  396,
 /*  1600 */   396,  396,  396,  396,  396,  396,  396,  288,  290,   61,
 /*  1610 */   396,  396,  396,  396,  356,   33,   34,   35,   36,   37,
 /*  1620 */    38,   39,   40,   41,   42,   43,   44,   45,   46,   47,
 /*  1630 */    48,  396,  396,  396,  396,  396,  396,  396,  396,  396,
 /*  1640 */    49,  396,  288,  290,  396,  396,  396,  396,  396,  396,
 /*  1650 */   396,  396,   76,  396,  396,  396,  396,  356,   33,   34,
 /*  1660 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1670 */    45,   46,   47,   48,  396,  396,   49,  396,  396,  396,
 /*  1680 */   396,  396,  396,  396,  396,  288,  290,  396,   57,  396,
 /*  1690 */   396,  396,  396,  356,   33,   34,   35,   36,   37,   38,
 /*  1700 */    39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
 /*  1710 */   396,   49,  396,  396,  396,  396,  396,  396,  396,  396,
 /*  1720 */   396,  288,  290,  372,  396,  396,  396,  396,  356,   33,
 /*  1730 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1740 */    44,   45,   46,   47,   48,  396,  396,  396,  396,  396,
 /*  1750 */   396,  396,  396,  396,   49,  396,  288,  290,  396,  396,
 /*  1760 */   396,  396,  396,  396,  396,  396,   85,  396,  396,  396,
 /*  1770 */   396,  356,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1780 */    41,   42,   43,   44,   45,   46,   47,   48,  396,  396,
 /*  1790 */    49,  396,  396,  396,  396,  396,  396,  396,  396,  288,
 /*  1800 */   290,  396,   30,  396,  396,  396,  396,  356,   33,   34,
 /*  1810 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1820 */    45,   46,   47,   48,  396,   49,  396,  396,  396,  396,
 /*  1830 */   396,  396,  396,  396,  396,  288,  290,  395,  396,  396,
 /*  1840 */   396,  396,  356,   33,   34,   35,   36,   37,   38,   39,
 /*  1850 */    40,   41,   42,   43,   44,   45,   46,   47,   48,  396,
 /*  1860 */   396,  396,  396,  396,  396,  396,  396,  396,   49,  396,
 /*  1870 */   288,  290,  415,  416,  418,  417,  419,  396,  396,  396,
 /*  1880 */   431,  396,  396,  396,  396,  356,   33,   34,   35,   36,
 /*  1890 */    37,   38,   39,   40,   41,   42,   43,   44,   45,   46,
 /*  1900 */    47,   48,   49,  396,  396,  396,  421,  396,  499,  456,
 /*  1910 */   396,  450,  396,  288,  290,  420,  422,  396,  396,   32,
 /*  1920 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1930 */    43,   44,   45,   46,   47,   48,  396,   93,  158,  162,
 /*  1940 */   156,  194,   90,  396,  396,  151,  396,  288,  290,  396,
 /*  1950 */   396,  396,  396,  396,  396,  164,  396,  134,   93,  158,
 /*  1960 */   162,  156,  194,   90,  396,  396,  151,  396,  415,  416,
 /*  1970 */   418,  417,  419,  396,  396,  396,  164,  396,  134,   93,
 /*  1980 */   158,  162,  156,  194,   90,  221,  396,  151,  396,  396,
 /*  1990 */   396,  396,  172,  271,  396,  396,  396,  164,  464,  134,
 /*  2000 */   165,  166,  421,  167,  168,  169,  170,  171,  396,   24,
 /*  2010 */   396,  420,  422,  172,  250,  396,  396,  396,  396,  396,
 /*  2020 */   396,  165,  166,  396,  167,  168,  169,  170,  171,  415,
 /*  2030 */   416,  418,  417,  419,  172,  396,  396,  396,  396,  396,
 /*  2040 */   396,  396,  165,  166,  396,  167,  168,  169,  170,  171,
 /*  2050 */    93,  158,  162,  156,  194,   90,  197,  396,  151,  396,
 /*  2060 */   396,  396,  396,  421,  396,  507,  423,  396,  164,  396,
 /*  2070 */   134,  396,  420,  422,   93,  158,  162,  156,  194,   90,
 /*  2080 */   396,  396,  151,  396,  396,  396,  396,  396,  396,  396,
 /*  2090 */   396,  396,  164,  396,  134,  396,  396,  396,  396,  396,
 /*  2100 */   396,  396,  396,  396,  396,  172,  396,  396,  396,  396,
 /*  2110 */   396,  396,  396,  165,  166,  396,  167,  168,  169,  170,
 /*  2120 */   171,  357,  396,  396,  396,  396,  396,  396,  396,  172,
 /*  2130 */   396,  396,  396,  396,  396,  396,  396,  165,  166,  396,
 /*  2140 */   167,  168,  169,  170,  171,   93,  158,  162,  156,  194,
 /*  2150 */    90,  396,  396,  151,  396,  415,  416,  418,  417,  419,
 /*  2160 */   396,  396,  396,  164,  396,  134,   93,  158,  162,  156,
 /*  2170 */   194,   90,  396,  396,  151,  396,  415,  416,  418,  417,
 /*  2180 */   419,  396,  396,  396,  164,  458,  134,  396,  396,  421,
 /*  2190 */   396,  396,  369,  396,  396,  396,  425,  396,  420,  422,
 /*  2200 */   172,  396,  396,  396,  396,  396,  396,  396,  165,  166,
 /*  2210 */   421,  167,  168,  169,  170,  171,  396,  425,  396,  420,
 /*  2220 */   422,  172,  263,  396,  396,  396,  396,  396,  396,  165,
 /*  2230 */   166,  396,  167,  168,  169,  170,  171,   93,  158,  162,
 /*  2240 */   156,  194,   90,  207,  396,  151,  396,  396,  396,  396,
 /*  2250 */   396,  396,  396,  396,  396,  164,  396,  134,   93,  158,
 /*  2260 */   162,  156,  194,   90,  213,  396,  151,  396,  415,  416,
 /*  2270 */   418,  417,  419,  396,  396,  396,  164,  396,  134,   93,
 /*  2280 */   158,  162,  156,  194,   90,  396,  396,  151,  295,  396,
 /*  2290 */   396,  396,  172,  396,  396,  396,  396,  164,  396,  134,
 /*  2300 */   165,  166,  421,  167,  168,  169,  170,  171,  396,   24,
 /*  2310 */   396,  420,  422,  172,  396,  396,  396,  396,  396,  396,
 /*  2320 */   396,  165,  166,  396,  167,  168,  169,  170,  171,  396,
 /*  2330 */   396,  396,  396,  396,  172,  396,  396,  396,  396,  396,
 /*  2340 */   396,  396,  165,  166,  396,  167,  168,  169,  170,  171,
 /*  2350 */    93,  158,  162,  156,  194,   90,  396,  396,  151,  396,
 /*  2360 */   415,  416,  418,  417,  419,  396,  396,  396,  164,  396,
 /*  2370 */   134,  396,  396,  396,  396,  396,  130,  132,   92,  106,
 /*  2380 */   110,  112,  114,  116,  108,  104,  118,  120,  128,  122,
 /*  2390 */   124,  126,  396,  396,  421,  396,  500,  456,  396,  450,
 /*  2400 */   396,  313,  396,  420,  422,  172,  396,  396,  396,  396,
 /*  2410 */   396,  396,  396,  165,  166,  396,  167,  168,  169,  170,
 /*  2420 */   171,  130,  132,   92,  106,  110,  112,  114,  116,  108,
 /*  2430 */   104,  118,  120,  128,  122,  124,  126,  130,  132,   92,
 /*  2440 */   106,  110,  112,  114,  116,  108,  104,  118,  120,  128,
 /*  2450 */   122,  124,  126,  396,  396,  396,  396,  396,  396,  147,
 /*  2460 */   130,  132,   92,  106,  110,  112,  114,  116,  108,  104,
 /*  2470 */   118,  120,  128,  122,  124,  126,  396,  396,  396,  396,
 /*  2480 */   396,  396,  396,  396,  396,  396,  396,  396,  396,  227,
 /*  2490 */   396,  396,  396,  217,  396,  396,  396,  130,  132,   92,
 /*  2500 */   106,  110,  112,  114,  116,  108,  104,  118,  120,  128,
 /*  2510 */   122,  124,  126,  130,  132,   92,  106,  110,  112,  114,
 /*  2520 */   116,  108,  104,  118,  120,  128,  122,  124,  126,  396,
 /*  2530 */   396,  396,  396,  396,  396,  396,  396,  396,  396,  396,
 /*  2540 */   396,  396,  371,  396,  396,  396,  396,  396,  396,  396,
 /*  2550 */   130,  132,   92,  106,  110,  112,  114,  116,  108,  104,
 /*  2560 */   118,  120,  128,  122,  124,  126,  396,  396,  396,  621,
 /*  2570 */   396,  415,  416,  418,  417,  419,  396,  396,  396,  366,
 /*  2580 */   396,  396,  396,  396,  396,  396,  396,  130,  132,   92,
 /*  2590 */   106,  110,  112,  114,  116,  108,  104,  118,  120,  128,
 /*  2600 */   122,  124,  126,  396,  396,  421,  396,  503,  444,  396,
 /*  2610 */   450,  396,  396,  396,  420,  422,  359,  396,  396,  396,
 /*  2620 */   396,  396,  396,  396,  130,  132,   92,  106,  110,  112,
 /*  2630 */   114,  116,  108,  104,  118,  120,  128,  122,  124,  126,
 /*  2640 */   410,   92,  106,  110,  112,  114,  116,  108,  104,  118,
 /*  2650 */   120,  128,  122,  124,  126,  396,  396,  396,   27,  396,
 /*  2660 */   399,  396,  396,  396,  396,  396,  396,  396,  396,  396,
 /*  2670 */   410,  396,  396,  396,  396,  135,  136,  137,  138,  139,
 /*  2680 */   140,  141,  142,  143,  144,  145,  396,  396,  428,  396,
 /*  2690 */   399,  396,  396,  396,  396,  396,  410,  396,  396,  396,
 /*  2700 */   396,  396,  396,  396,  396,  135,  136,  137,  138,  139,
 /*  2710 */   140,  141,  142,  143,  144,  145,  399,  415,  416,  418,
 /*  2720 */   417,  419,  396,  396,  396,  396,  396,  396,  396,  396,
 /*  2730 */   396,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  2740 */   144,  145,  415,  416,  418,  417,  419,  396,  396,  396,
 /*  2750 */   396,  421,  396,   19,  456,  396,  450,  396,  396,  396,
 /*  2760 */   420,  422,  415,  416,  418,  417,  419,  415,  416,  418,
 /*  2770 */   417,  419,  396,  396,  396,  396,  421,  396,  504,  473,
 /*  2780 */   415,  416,  418,  417,  419,  420,  422,  396,  396,  396,
 /*  2790 */   396,  396,  396,  396,  396,  396,  421,  396,  506,  423,
 /*  2800 */   396,  421,  396,  502,  423,  420,  422,  396,  396,  396,
 /*  2810 */   420,  422,  396,  396,  421,  396,  505,  423,  415,  416,
 /*  2820 */   418,  417,  419,  420,  422,  415,  416,  418,  417,  419,
 /*  2830 */   396,  415,  416,  418,  417,  419,  396,  396,  396,  396,
 /*  2840 */   396,  396,  396,  396,  396,  396,  396,  396,  396,  396,
 /*  2850 */   396,  396,  421,  396,  501,  423,  396,  396,  396,  421,
 /*  2860 */   396,  420,  422,  396,  445,  421,  396,  396,  420,  422,
 /*  2870 */   396,  396,  396,  396,  420,  422,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    23,   24,   25,   26,   27,   28,  148,  149,   31,  104,
 /*    10 */     0,  106,  107,  108,   32,  110,  111,   39,   41,  114,
 /*    20 */    43,  139,  140,   46,   47,   48,   49,   50,   51,   52,
 /*    30 */    53,   54,   55,   56,    7,    8,    9,   10,   11,   12,
 /*    40 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*    50 */    72,   73,   74,   75,   76,   78,   78,  112,  102,   32,
 /*    60 */   115,  116,  117,   86,   87,   31,   89,   90,   91,   92,
 /*    70 */    93,   37,   38,  112,   39,    6,  115,  116,  117,   37,
 /*    80 */    46,   47,   48,   49,   50,   51,   52,   53,   54,   31,
 /*    90 */    56,   57,   58,   59,   34,   61,   36,   31,   29,   65,
 /*   100 */    66,   67,   68,   32,   62,   71,   64,   72,   73,   74,
 /*   110 */    75,   76,   31,   78,   31,   31,  116,   83,   84,   85,
 /*   120 */    37,   38,   88,   17,   18,   19,   20,   21,   22,   46,
 /*   130 */    47,   48,   49,   50,   51,   52,   53,   54,  138,   56,
 /*   140 */    57,   58,   59,   36,   61,   79,  146,  147,   65,   66,
 /*   150 */    67,   68,   86,   87,   71,  102,   90,   91,   92,   93,
 /*   160 */    37,  135,   77,   31,  138,   80,   83,   84,   85,   37,
 /*   170 */    38,   88,  146,  147,   77,   78,  109,    6,   46,   47,
 /*   180 */    48,   49,   50,   51,   52,   53,   54,  138,   56,   57,
 /*   190 */    58,   59,   34,   61,   36,  146,  147,   65,   66,   67,
 /*   200 */    68,  152,  153,   71,   95,   96,   97,   98,   99,  100,
 /*   210 */   101,    6,   31,    6,  114,   83,   84,   85,   37,   38,
 /*   220 */    88,  154,  155,  156,   20,   21,   22,   46,   47,   48,
 /*   230 */    49,   50,   51,   52,   53,   54,   29,   56,   57,   58,
 /*   240 */    59,   31,   61,  109,   31,  108,   65,   66,   67,   68,
 /*   250 */    79,  114,   71,   36,  105,  106,  107,  108,   43,  110,
 /*   260 */   111,   31,    6,  114,   83,   84,   85,   37,   38,   88,
 /*   270 */    98,   99,  100,  101,   69,   32,   46,   47,   48,   49,
 /*   280 */    50,   51,   52,   53,   54,   29,   56,   57,   58,   59,
 /*   290 */   156,   61,   32,   31,   36,   65,   66,   67,   68,   86,
 /*   300 */    87,   71,   32,   90,   91,   92,   93,   60,  135,   39,
 /*   310 */    31,  138,   36,   83,   84,   85,   37,   38,   88,  146,
 /*   320 */   147,  115,  116,  117,   36,   46,   47,   48,   49,   50,
 /*   330 */    51,   52,   53,   54,  138,   56,   57,   58,   59,  143,
 /*   340 */    61,   31,  146,  147,   65,   66,   67,   68,   86,   87,
 /*   350 */    71,   41,   90,   91,   92,   93,  148,  149,  135,   31,
 /*   360 */   109,  138,   83,   84,   85,   37,   38,   88,   39,  146,
 /*   370 */   147,  137,  148,  149,   46,   47,   48,   49,   50,   51,
 /*   380 */    52,   53,   54,  138,   56,   57,   58,   59,   63,   61,
 /*   390 */    36,  146,  147,   65,   66,   67,   68,  152,  153,   71,
 /*   400 */   108,   72,   73,   74,   75,   76,  114,   36,   31,   36,
 /*   410 */   138,   83,   84,   85,   37,   38,   88,  145,  146,  147,
 /*   420 */    32,  148,  149,   46,   47,   48,   49,   50,   51,   52,
 /*   430 */    53,   54,  138,   56,   57,   58,   59,  143,   61,   31,
 /*   440 */   146,  147,   65,   66,   67,   68,   86,   87,   71,   32,
 /*   450 */    90,   91,   92,   93,  109,  135,   39,   31,  138,   66,
 /*   460 */    83,   84,   85,   37,   38,   88,  146,  147,   43,    6,
 /*   470 */   148,  149,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   480 */    54,  138,   56,   57,   58,   59,  143,   61,  150,  146,
 /*   490 */   147,   65,   66,   67,   68,   32,  151,   71,   79,  154,
 /*   500 */   155,  156,   77,   78,  135,   80,   31,  138,   78,   83,
 /*   510 */    84,   85,   37,   38,   88,  146,  147,   79,  148,  149,
 /*   520 */    29,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   530 */   138,   56,   57,   58,   59,   79,   61,   31,  146,  147,
 /*   540 */    65,   66,   67,   68,  138,  153,   71,  148,  149,   31,
 /*   550 */   144,  145,  146,  147,   43,   31,  138,    6,   83,   84,
 /*   560 */    85,   37,   38,   88,  146,  147,  150,    6,  148,  149,
 /*   570 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  138,
 /*   580 */    56,   57,   58,   59,   63,   61,   63,  146,  147,   65,
 /*   590 */    66,   67,   68,   32,  106,   71,  108,  138,  110,  111,
 /*   600 */    63,  142,  114,   63,   31,  146,  147,   83,   84,   85,
 /*   610 */    37,   38,   88,  148,  149,  136,  137,   31,   37,   46,
 /*   620 */    47,   48,   49,   50,   51,   52,   53,   54,  138,   56,
 /*   630 */    57,   58,   59,   31,   61,   43,  146,  147,   65,   66,
 /*   640 */    67,   68,  138,   62,   71,   64,  142,  106,  107,  108,
 /*   650 */   146,  147,  111,   31,   43,  114,   83,   84,   85,   37,
 /*   660 */    38,   88,    0,  148,  149,   31,  140,   31,   46,   47,
 /*   670 */    48,   49,   50,   51,   52,   53,   54,  138,   56,   57,
 /*   680 */    58,   59,   32,   61,  141,  146,  147,   65,   66,   67,
 /*   690 */    68,   30,   30,   71,   33,   33,   35,   35,  108,   38,
 /*   700 */    38,  111,   31,  138,  114,   83,   84,   85,   37,   38,
 /*   710 */    88,  146,  147,  141,   79,  141,  141,   46,   47,   48,
 /*   720 */    49,   50,   51,   52,   53,   54,  138,   56,   57,   58,
 /*   730 */    59,   79,   61,   31,  146,  147,   65,   66,   67,   68,
 /*   740 */    31,  138,   71,   79,  138,  142,  141,   32,  142,  146,
 /*   750 */   147,   31,  146,  147,   83,   84,   85,   37,   38,   88,
 /*   760 */   141,  141,  141,   31,    6,  141,   46,   47,   48,   49,
 /*   770 */    50,   51,   52,   53,   54,  138,   56,   57,   58,   59,
 /*   780 */    31,   61,   79,  146,  147,   65,   66,   67,   68,  138,
 /*   790 */    32,   71,   32,  142,   32,   86,   87,  146,  147,   32,
 /*   800 */    31,  138,   79,   83,   84,   85,   37,   38,   88,  146,
 /*   810 */   147,   31,   79,    6,  149,   46,   47,   48,   49,   50,
 /*   820 */    51,   52,   53,   54,  138,   56,   57,   58,   59,   39,
 /*   830 */    61,  109,  146,  147,   65,   66,   67,   68,  138,   32,
 /*   840 */    71,   69,  142,   31,   31,   31,  146,  147,   66,   31,
 /*   850 */   138,   32,   83,   84,   85,   37,   38,   88,  146,  147,
 /*   860 */    63,   60,   36,   36,   46,   47,   48,   49,   50,   51,
 /*   870 */    52,   53,   54,  138,   56,   57,   58,   59,   39,   61,
 /*   880 */   109,  146,  147,   65,   66,   67,   68,   31,  138,   71,
 /*   890 */    31,  138,  142,   39,  109,  142,  146,  147,   31,  146,
 /*   900 */   147,   83,   84,   85,   37,   38,   88,   31,   39,  109,
 /*   910 */    31,    6,   31,   46,   47,   48,   49,   50,   51,   52,
 /*   920 */    53,   54,  138,   56,   57,   58,   59,   12,   61,   31,
 /*   930 */   146,  147,   65,   66,   67,   68,  138,   32,   71,  109,
 /*   940 */   142,   39,   86,   87,  146,  147,   43,   31,  138,   36,
 /*   950 */    83,   84,   85,   37,   38,   88,  146,  147,   36,   41,
 /*   960 */     6,   32,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   970 */    54,  138,   56,   57,   58,   59,   41,   61,   39,  146,
 /*   980 */   147,   65,   66,   67,   68,  138,   32,   71,   32,  138,
 /*   990 */    32,   32,  109,  146,  147,  109,   31,  146,  147,   83,
 /*  1000 */    84,   85,   37,   38,   88,   33,  109,   31,  102,    6,
 /*  1010 */    31,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  1020 */   138,   56,   57,   58,   59,  102,   61,  157,  146,  147,
 /*  1030 */    65,   66,   67,   68,  138,   32,   71,  157,  138,  157,
 /*  1040 */   157,  157,  146,  147,  157,   31,  146,  147,   83,   84,
 /*  1050 */    85,   37,   38,   88,  157,  157,  157,  157,  157,  157,
 /*  1060 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  138,
 /*  1070 */    56,   57,   58,   59,  157,   61,  157,  146,  147,   65,
 /*  1080 */    66,   67,   68,  138,    6,   71,  157,  138,  157,  157,
 /*  1090 */   157,  146,  147,  157,   31,  146,  147,   83,   84,   85,
 /*  1100 */    37,   38,   88,  157,  157,  157,  157,   29,  157,   46,
 /*  1110 */    47,   48,   49,   50,   51,   52,   53,   54,  138,   56,
 /*  1120 */    57,   58,   59,  157,   61,  157,  146,  147,   65,   66,
 /*  1130 */    67,   68,  138,  157,   71,  157,  138,  157,  157,  157,
 /*  1140 */   146,  147,  157,   31,  146,  147,   83,   84,   85,   37,
 /*  1150 */    38,   88,  157,  157,  157,  157,    6,  157,   46,   47,
 /*  1160 */    48,   49,   50,   51,   52,   53,   54,  138,   56,   57,
 /*  1170 */    58,   59,  157,   61,  157,  146,  147,   65,   66,   67,
 /*  1180 */    68,  138,   32,   71,  157,  157,  157,  157,  157,  146,
 /*  1190 */   147,  157,   31,  157,  157,   83,   84,   85,   37,   38,
 /*  1200 */    88,  157,  157,  157,  157,    6,  157,   46,   47,   48,
 /*  1210 */    49,   50,   51,   52,   53,   54,    6,   56,   57,   58,
 /*  1220 */    59,  157,   61,  157,    6,    6,   65,   66,   67,   68,
 /*  1230 */   157,   32,   71,  157,  157,  157,  157,  157,  157,  157,
 /*  1240 */    31,  157,   32,  157,   83,   84,   85,   38,  157,   88,
 /*  1250 */    32,   32,  157,  157,  157,   46,   47,   48,   49,   50,
 /*  1260 */    51,   52,   53,   54,  157,   56,   57,   58,   59,  157,
 /*  1270 */    61,  157,    6,  138,   65,   66,   67,   68,  157,  157,
 /*  1280 */    71,  146,  147,  157,    6,  157,  157,  152,  153,  157,
 /*  1290 */   157,  157,   83,   84,   85,   29,  157,   88,  101,  157,
 /*  1300 */   157,  103,  104,  105,  106,  107,  108,   29,  110,  111,
 /*  1310 */   113,  157,  114,  157,  157,  118,  119,  120,  121,  122,
 /*  1320 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1330 */   133,  134,  157,  157,  101,  157,  157,  157,  157,  157,
 /*  1340 */   157,  157,  157,  146,  147,  157,  113,  157,  157,  157,
 /*  1350 */   157,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1360 */   127,  128,  129,  130,  131,  132,  133,  134,  138,  101,
 /*  1370 */   157,  157,  157,  157,  157,  157,  146,  147,  157,  146,
 /*  1380 */   147,  113,  152,  153,   39,  157,  118,  119,  120,  121,
 /*  1390 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1400 */   132,  133,  134,  157,  157,  157,  157,  157,  157,  157,
 /*  1410 */   157,  157,  101,  157,  146,  147,  157,   72,   73,   74,
 /*  1420 */    75,   76,   77,   78,  113,   80,   81,   82,  157,  118,
 /*  1430 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1440 */   129,  130,  131,  132,  133,  134,  157,  157,  101,  157,
 /*  1450 */   157,  157,  157,  157,  157,  157,  157,  146,  147,  157,
 /*  1460 */   113,  157,  157,  157,  157,  118,  119,  120,  121,  122,
 /*  1470 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1480 */   133,  134,  157,  101,  157,  157,  157,  157,  157,  157,
 /*  1490 */   157,  157,  157,  146,  147,  113,  157,  157,  157,  157,
 /*  1500 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*  1510 */   128,  129,  130,  131,  132,  133,  134,  157,  157,  157,
 /*  1520 */   157,  157,  157,  157,  157,  157,  101,  157,  146,  147,
 /*  1530 */   157,  157,  157,  157,  157,  157,  157,  157,  113,  157,
 /*  1540 */   157,  157,  157,  118,  119,  120,  121,  122,  123,  124,
 /*  1550 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  1560 */   157,  157,  101,  157,  157,  157,  157,  157,  157,  157,
 /*  1570 */   157,  146,  147,  157,  113,  157,  157,  157,  157,  118,
 /*  1580 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1590 */   129,  130,  131,  132,  133,  134,  157,  101,  157,  157,
 /*  1600 */   157,  157,  157,  157,  157,  157,  157,  146,  147,  113,
 /*  1610 */   157,  157,  157,  157,  118,  119,  120,  121,  122,  123,
 /*  1620 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  133,
 /*  1630 */   134,  157,  157,  157,  157,  157,  157,  157,  157,  157,
 /*  1640 */   101,  157,  146,  147,  157,  157,  157,  157,  157,  157,
 /*  1650 */   157,  157,  113,  157,  157,  157,  157,  118,  119,  120,
 /*  1660 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1670 */   131,  132,  133,  134,  157,  157,  101,  157,  157,  157,
 /*  1680 */   157,  157,  157,  157,  157,  146,  147,  157,  113,  157,
 /*  1690 */   157,  157,  157,  118,  119,  120,  121,  122,  123,  124,
 /*  1700 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  134,
 /*  1710 */   157,  101,  157,  157,  157,  157,  157,  157,  157,  157,
 /*  1720 */   157,  146,  147,  113,  157,  157,  157,  157,  118,  119,
 /*  1730 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1740 */   130,  131,  132,  133,  134,  157,  157,  157,  157,  157,
 /*  1750 */   157,  157,  157,  157,  101,  157,  146,  147,  157,  157,
 /*  1760 */   157,  157,  157,  157,  157,  157,  113,  157,  157,  157,
 /*  1770 */   157,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1780 */   127,  128,  129,  130,  131,  132,  133,  134,  157,  157,
 /*  1790 */   101,  157,  157,  157,  157,  157,  157,  157,  157,  146,
 /*  1800 */   147,  157,  113,  157,  157,  157,  157,  118,  119,  120,
 /*  1810 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1820 */   131,  132,  133,  134,  157,  101,  157,  157,  157,  157,
 /*  1830 */   157,  157,  157,  157,  157,  146,  147,  113,  157,  157,
 /*  1840 */   157,  157,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1850 */   126,  127,  128,  129,  130,  131,  132,  133,  134,  157,
 /*  1860 */   157,  157,  157,  157,  157,  157,  157,  157,  101,  157,
 /*  1870 */   146,  147,    1,    2,    3,    4,    5,  157,  157,  157,
 /*  1880 */   113,  157,  157,  157,  157,  118,  119,  120,  121,  122,
 /*  1890 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1900 */   133,  134,  101,  157,  157,  157,   35,  157,   37,   38,
 /*  1910 */   157,   40,  157,  146,  147,   44,   45,  157,  157,  118,
 /*  1920 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1930 */   129,  130,  131,  132,  133,  134,  157,   23,   24,   25,
 /*  1940 */    26,   27,   28,  157,  157,   31,  157,  146,  147,  157,
 /*  1950 */   157,  157,  157,  157,  157,   41,  157,   43,   23,   24,
 /*  1960 */    25,   26,   27,   28,  157,  157,   31,  157,    1,    2,
 /*  1970 */     3,    4,    5,  157,  157,  157,   41,  157,   43,   23,
 /*  1980 */    24,   25,   26,   27,   28,   29,  157,   31,  157,  157,
 /*  1990 */   157,  157,   78,   79,  157,  157,  157,   41,   31,   43,
 /*  2000 */    86,   87,   35,   89,   90,   91,   92,   93,  157,   42,
 /*  2010 */   157,   44,   45,   78,   79,  157,  157,  157,  157,  157,
 /*  2020 */   157,   86,   87,  157,   89,   90,   91,   92,   93,    1,
 /*  2030 */     2,    3,    4,    5,   78,  157,  157,  157,  157,  157,
 /*  2040 */   157,  157,   86,   87,  157,   89,   90,   91,   92,   93,
 /*  2050 */    23,   24,   25,   26,   27,   28,   29,  157,   31,  157,
 /*  2060 */   157,  157,  157,   35,  157,   37,   38,  157,   41,  157,
 /*  2070 */    43,  157,   44,   45,   23,   24,   25,   26,   27,   28,
 /*  2080 */   157,  157,   31,  157,  157,  157,  157,  157,  157,  157,
 /*  2090 */   157,  157,   41,  157,   43,  157,  157,  157,  157,  157,
 /*  2100 */   157,  157,  157,  157,  157,   78,  157,  157,  157,  157,
 /*  2110 */   157,  157,  157,   86,   87,  157,   89,   90,   91,   92,
 /*  2120 */    93,   70,  157,  157,  157,  157,  157,  157,  157,   78,
 /*  2130 */   157,  157,  157,  157,  157,  157,  157,   86,   87,  157,
 /*  2140 */    89,   90,   91,   92,   93,   23,   24,   25,   26,   27,
 /*  2150 */    28,  157,  157,   31,  157,    1,    2,    3,    4,    5,
 /*  2160 */   157,  157,  157,   41,  157,   43,   23,   24,   25,   26,
 /*  2170 */    27,   28,  157,  157,   31,  157,    1,    2,    3,    4,
 /*  2180 */     5,  157,  157,  157,   41,   31,   43,  157,  157,   35,
 /*  2190 */   157,  157,   70,  157,  157,  157,   42,  157,   44,   45,
 /*  2200 */    78,  157,  157,  157,  157,  157,  157,  157,   86,   87,
 /*  2210 */    35,   89,   90,   91,   92,   93,  157,   42,  157,   44,
 /*  2220 */    45,   78,   79,  157,  157,  157,  157,  157,  157,   86,
 /*  2230 */    87,  157,   89,   90,   91,   92,   93,   23,   24,   25,
 /*  2240 */    26,   27,   28,   29,  157,   31,  157,  157,  157,  157,
 /*  2250 */   157,  157,  157,  157,  157,   41,  157,   43,   23,   24,
 /*  2260 */    25,   26,   27,   28,   29,  157,   31,  157,    1,    2,
 /*  2270 */     3,    4,    5,  157,  157,  157,   41,  157,   43,   23,
 /*  2280 */    24,   25,   26,   27,   28,  157,  157,   31,   32,  157,
 /*  2290 */   157,  157,   78,  157,  157,  157,  157,   41,  157,   43,
 /*  2300 */    86,   87,   35,   89,   90,   91,   92,   93,  157,   42,
 /*  2310 */   157,   44,   45,   78,  157,  157,  157,  157,  157,  157,
 /*  2320 */   157,   86,   87,  157,   89,   90,   91,   92,   93,  157,
 /*  2330 */   157,  157,  157,  157,   78,  157,  157,  157,  157,  157,
 /*  2340 */   157,  157,   86,   87,  157,   89,   90,   91,   92,   93,
 /*  2350 */    23,   24,   25,   26,   27,   28,  157,  157,   31,  157,
 /*  2360 */     1,    2,    3,    4,    5,  157,  157,  157,   41,  157,
 /*  2370 */    43,  157,  157,  157,  157,  157,    7,    8,    9,   10,
 /*  2380 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2390 */    21,   22,  157,  157,   35,  157,   37,   38,  157,   40,
 /*  2400 */   157,   32,  157,   44,   45,   78,  157,  157,  157,  157,
 /*  2410 */   157,  157,  157,   86,   87,  157,   89,   90,   91,   92,
 /*  2420 */    93,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*  2430 */    16,   17,   18,   19,   20,   21,   22,    7,    8,    9,
 /*  2440 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2450 */    20,   21,   22,  157,  157,  157,  157,  157,  157,   29,
 /*  2460 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2470 */    17,   18,   19,   20,   21,   22,  157,  157,  157,  157,
 /*  2480 */   157,  157,  157,  157,  157,  157,  157,  157,  157,   36,
 /*  2490 */   157,  157,  157,   79,  157,  157,  157,    7,    8,    9,
 /*  2500 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2510 */    20,   21,   22,    7,    8,    9,   10,   11,   12,   13,
 /*  2520 */    14,   15,   16,   17,   18,   19,   20,   21,   22,  157,
 /*  2530 */   157,  157,  157,  157,  157,  157,  157,  157,  157,  157,
 /*  2540 */   157,  157,   36,  157,  157,  157,  157,  157,  157,  157,
 /*  2550 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2560 */    17,   18,   19,   20,   21,   22,  157,  157,  157,   79,
 /*  2570 */   157,    1,    2,    3,    4,    5,  157,  157,  157,   36,
 /*  2580 */   157,  157,  157,  157,  157,  157,  157,    7,    8,    9,
 /*  2590 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2600 */    20,   21,   22,  157,  157,   35,  157,   37,   38,  157,
 /*  2610 */    40,  157,  157,  157,   44,   45,   36,  157,  157,  157,
 /*  2620 */   157,  157,  157,  157,    7,    8,    9,   10,   11,   12,
 /*  2630 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2640 */    11,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2650 */    18,   19,   20,   21,   22,  157,  157,  157,   29,  157,
 /*  2660 */    31,  157,  157,  157,  157,  157,  157,  157,  157,  157,
 /*  2670 */    11,  157,  157,  157,  157,   46,   47,   48,   49,   50,
 /*  2680 */    51,   52,   53,   54,   55,   56,  157,  157,   29,  157,
 /*  2690 */    31,  157,  157,  157,  157,  157,   11,  157,  157,  157,
 /*  2700 */   157,  157,  157,  157,  157,   46,   47,   48,   49,   50,
 /*  2710 */    51,   52,   53,   54,   55,   56,   31,    1,    2,    3,
 /*  2720 */     4,    5,  157,  157,  157,  157,  157,  157,  157,  157,
 /*  2730 */   157,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  2740 */    55,   56,    1,    2,    3,    4,    5,  157,  157,  157,
 /*  2750 */   157,   35,  157,   37,   38,  157,   40,  157,  157,  157,
 /*  2760 */    44,   45,    1,    2,    3,    4,    5,    1,    2,    3,
 /*  2770 */     4,    5,  157,  157,  157,  157,   35,  157,   37,   38,
 /*  2780 */     1,    2,    3,    4,    5,   44,   45,  157,  157,  157,
 /*  2790 */   157,  157,  157,  157,  157,  157,   35,  157,   37,   38,
 /*  2800 */   157,   35,  157,   37,   38,   44,   45,  157,  157,  157,
 /*  2810 */    44,   45,  157,  157,   35,  157,   37,   38,    1,    2,
 /*  2820 */     3,    4,    5,   44,   45,    1,    2,    3,    4,    5,
 /*  2830 */   157,    1,    2,    3,    4,    5,  157,  157,  157,  157,
 /*  2840 */   157,  157,  157,  157,  157,  157,  157,  157,  157,  157,
 /*  2850 */   157,  157,   35,  157,   37,   38,  157,  157,  157,   35,
 /*  2860 */   157,   44,   45,  157,   40,   35,  157,  157,   44,   45,
 /*  2870 */   157,  157,  157,  157,   44,   45,
};
#define YY_SHIFT_USE_DFLT (-24)
static short yy_shift_ofst[] = {
 /*     0 */   661,   10,  662,  -24,  -24,  -24,  -24,   58,   71,  -24,
 /*    10 */    81,   60,  -24,   84,  107,  -24, 2716,  123,  -24,  -24,
 /*    20 */  1871, 2766,  -24, 2267,  210,  215, 2629,  217,   34,  -24,
 /*    30 */   916,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*    40 */   -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*    50 */   243,  -24,  260,  -24, 2327,  258, 1112,  818,  247,  276,
 /*    60 */   769,  671,  -24, 2327,  288,  581,  -24,   42,  -24,  -24,
 /*    70 */   360,  325, 1209, 1209,  354,  475,  377,  -24, 2327,  371,
 /*    80 */   720,  573,  -24,  373,  426,  328,  393, 2327,  388,  -24,
 /*    90 */  2327,  -24, 2327,  408,  -24,  430,  709,  419,  -24,  438,
 /*   100 */   -24,  456,  -24,  106, 2327,  106, 2327,  106, 2327,  106,
 /*   110 */  2327,  106, 2327,  106, 2327,  106, 2327,  106, 2327,  204,
 /*   120 */  2327,  204, 2327,  -24, 2327,  -24, 2327,  -24, 2327,  204,
 /*   130 */  2327, 2632, 2327, 2632,  -23,  -24,  -24,  -24,  -24,  -24,
 /*   140 */   -24,  -24,  -24,  -24,  -24,  -24, 2430,  -24,  491, 2327,
 /*   150 */   -24,  425,  506,  511, 2214, 2617, 2327,  -24,  518,  551,
 /*   160 */   408,  -24, 2327,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*   170 */   -24,  -24,   66,  -24,  171,  -24,  262,  -24,  521,  213,
 /*   180 */   -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*   190 */   523,  537,  540,  -24,  586,  592, 2027,  -24, 1266,  -24,
 /*   200 */  2327,  -24,  -24,  -24,  -24, 1278,  -24,  -24,  310,  611,
 /*   210 */  2235,   69,  -24,  -24,  -24, 2327, 2414,  -24, 1956, 1078,
 /*   220 */   -24,  -24, 2617,  602,  205, 2051, 2453, 1209,  524,  -24,
 /*   230 */   634,  561,  -24,  634,  -24, 1345,  -24,  -24,  -24,  -24,
 /*   240 */   -24,  -24, 2327,  -24, 2617,  636,   35, 2327,  -24, 1935,
 /*   250 */   329, 2327,  -24,  635,  329, 2327,  -24, 2490,  702,  -22,
 /*   260 */  2327,  -24, 2143,  329, 2327,  -24,  652,  329, 2327,  -24,
 /*   270 */  1914,  329, 2327,  -24,  664,  329, 2327,  -24,  -24,  -24,
 /*   280 */   -24, 2327,  758,  -24, 2327,  -24, 2617,  -24,  650,  -24,
 /*   290 */   715,  -24, 2256,   27,  -24,  -24,  732,   97,  749,  760,
 /*   300 */   -24,  856,  703,  -18,  -24,  723,  762,  -24,  733,  767,
 /*   310 */   -24, 2327, 2369,  -24,  780,  807,  -24,  780,  -24,  790,
 /*   320 */   360,  -24,  -24,  780, 1219,  -24,  780, 1150,  -24,  780,
 /*   330 */  1003,  -24,  780,  954,  -24,  780,  905,  -24,  780,  463,
 /*   340 */   -24,  780, 1218,  -24,  780, 1199,  -24,  780, 1210,  -24,
 /*   350 */    85,  812,  611,  813,  511,  -24,  -24, 2327, 2580, 1209,
 /*   360 */   622,  -24,  814,  772, 2122, 2543, 1209, 1161,  -24, 2327,
 /*   370 */  2506, 1209,  181,  -24,  782, 2327,  819,  -24,  -24,  -24,
 /*   380 */   797, 1209, 1209,  -24,  -24,  801,  826,  279,  230,  -24,
 /*   390 */   -24,  207,  827,  132,  -24,   83,  -24, 2685,  -24,  839,
 /*   400 */   360,  -24,  859,  854,  360,  -24,  876,  869,  360,  -24,
 /*   410 */   879,  915,  881,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*   420 */   -24,  -24,  -24, 2830, 2175,  898,  903, 2659,  913, 1063,
 /*   430 */   -24, 1014,  -24,  256,  922,  965,  -24,  867,  -24,  -24,
 /*   440 */  2570, 2761,  -24,  -24, 2824,  918,  902,  360,  929,  -24,
 /*   450 */   935,  939,  360,  956,  -24,  -24, 2824, 2154,  417,  -24,
 /*   460 */   360,  958,  -24, 1967,  270,  -24,  360,  959,  -24,  -24,
 /*   470 */  2359, 2741, 2028, 2830, 2779,  -24, 2817,  972,  976,  158,
 /*   480 */   -24,  979,  107,  -24,  -24,
};
#define YY_REDUCE_USE_DFLT (-143)
static short yy_reduce_ofst[] = {
 /*     0 */   109, -143,  172, -143, -143, -143, -143, -143, -143, -143,
 /*    10 */  -143,  -44, -143, -143,   53, -143, 1198, -143, -143, -143,
 /*    20 */   149,  590, -143,  100, -143, -143,  -55, -143, 1689, -143,
 /*    30 */  1801, -143, -143, -143, -143, -143, -143, -143, -143, -143,
 /*    40 */  -143, -143, -143, -143, -143, -143, -143, -143, -143, -143,
 /*    50 */  -143, -143, -143, -143,  173, -143, 1575, 1801, -143, -143,
 /*    60 */  1496, 1801, -143,  223, -143,  479, -143,  234, -143, -143,
 /*    70 */   251, -143, 1197, 1801, -143, 1539, 1801, -143,  320, -143,
 /*    80 */  1347, 1801, -143, -143, 1653, 1801, -143,  369, -143, -143,
 /*    90 */   588, -143,  539,  338, -143, -143, -143, -143, -143, -143,
 /*   100 */  -143, -143, -143, -143,  565, -143,  637, -143,  663, -143,
 /*   110 */   686, -143,  712, -143,  735, -143,  784, -143,  810, -143,
 /*   120 */   833, -143,  847, -143,  851, -143,  896, -143,  900, -143,
 /*   130 */   945, -143,  980, -143,    0, -143, -143, -143, -143, -143,
 /*   140 */  -143, -143, -143, -143, -143, -143, -143, -143, -143, 1029,
 /*   150 */  -143, -143, -143, -143,   49, -143, 1043, -143, -143, -143,
 /*   160 */   416, -143,  998, -143, -143, -143, -143, -143, -143, -143,
 /*   170 */  -143, -143,  345, -143, -143, -143,   67, -143, -143,  134,
 /*   180 */  -143, -143, -143, -143, -143, -143, -143, -143, -143, -143,
 /*   190 */  -143, -143, -143, -143, -143, -143, 1230, -143, -143, -143,
 /*   200 */   392, -143, -143, -143, -143, -143, -143, -143, -143, -143,
 /*   210 */  1135, -143, -143, -143, -143,  994, -143, -143,  245, -143,
 /*   220 */  -143, -143, -143, -143, -143,  418, -143, 1425, 1801, -143,
 /*   230 */  -118, -143, -143,  526, -143,  543, -143, -143, -143, -143,
 /*   240 */  -143, -143,  798, -143, -143, -143,  572,  606, -143,  294,
 /*   250 */   574,  750, -143, -143,  575,  651, -143, -143, -143,  605,
 /*   260 */   753, -143,  343,  619,  603, -143, -143,  620,  700, -143,
 /*   270 */   196,  621,  459, -143, -143,  624,  504, -143, -143, -143,
 /*   280 */  -143,  406, -143, -143,  272, -143, -143, -143, -143, -143,
 /*   290 */  -143, -143,  490, -143, -143, -143, -143, -143, -143, -143,
 /*   300 */  -143, -143, -143, -143, -143, -143, -143, -143, -143, -143,
 /*   310 */  -143,  441, -143, -143,  370, -143, -143,  665, -143, -143,
 /*   320 */   722, -143, -143,  515, -143, -143,  399, -143, -143,  273,
 /*   330 */  -143, -143,  224, -143, -143,  208, -143, -143, -142, -143,
 /*   340 */  -143,  465, -143, -143,  322, -143, -143,  420, -143, -143,
 /*   350 */  -143, -143, -143, -143, -143, -143, -143,  882, -143, 1311,
 /*   360 */  1801, -143, -143, -143,  931, -143, 1461, 1801, -143,  949,
 /*   370 */  -143, 1610, 1801, -143, -143,   26, -143, -143, -143, -143,
 /*   380 */  -143, 1268, 1801, -143, -143, -143, -143, 1382, 1801, -143,
 /*   390 */  -143, -143, -143, 1724, -143, 1801, -143,  206, -143, -143,
 /*   400 */   771, -143, -143, -143,  785, -143, -143, -143,  800, -143,
 /*   410 */  -143, -143, -143, -143, -143, -143, -143, -143, -143, -143,
 /*   420 */  -143, -143, -143,  137,  100, -143, -143,  -39, -143, 1767,
 /*   430 */  -143, 1801, -143, -143, -143, 1233, -143, 1801, -143, -143,
 /*   440 */   488,  590, -143, -143,  137, -143, -143,  830, -143, -143,
 /*   450 */  -143, -143,  883, -143, -143, -143,  292,  100, -143, -143,
 /*   460 */   886, -143, -143,  100, -143, -143,  897, -143, -143, -143,
 /*   470 */   -95,  541,  590,  292,  590, -143,  590, -143, -143,  906,
 /*   480 */  -143, -143,  923, -143, -143,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   722,  722,  722,  487,  489,  490,  491,  722,  722,  492,
 /*    10 */   722,  722,  493,  722,  722,  494,  722,  722,  497,  498,
 /*    20 */   722,  722,  516,  722,  722,  722,  722,  722,  722,  520,
 /*    30 */   722,  522,  558,  560,  561,  562,  563,  564,  565,  566,
 /*    40 */   567,  568,  569,  570,  571,  572,  573,  574,  575,  576,
 /*    50 */   722,  577,  722,  578,  722,  722,  722,  722,  581,  722,
 /*    60 */   722,  722,  582,  722,  722,  722,  585,  722,  586,  587,
 /*    70 */   722,  722,  722,  589,  722,  722,  722,  592,  722,  722,
 /*    80 */   722,  722,  594,  722,  722,  722,  722,  722,  722,  596,
 /*    90 */   722,  650,  722,  722,  651,  722,  722,  722,  717,  722,
 /*   100 */   718,  722,  719,  652,  722,  653,  722,  654,  722,  655,
 /*   110 */   722,  656,  722,  657,  722,  658,  722,  659,  722,  666,
 /*   120 */   722,  667,  722,  668,  722,  669,  722,  670,  722,  671,
 /*   130 */   722,  672,  722,  673,  722,  547,  548,  549,  550,  551,
 /*   140 */   552,  553,  554,  555,  556,  557,  722,  660,  722,  722,
 /*   150 */   661,  678,  722,  662,  722,  701,  722,  674,  722,  722,
 /*   160 */   722,  675,  722,  676,  677,  679,  680,  681,  682,  683,
 /*   170 */   684,  685,  722,  686,  722,  687,  722,  702,  722,  722,
 /*   180 */   704,  709,  710,  711,  712,  713,  714,  715,  716,  705,
 /*   190 */   710,  712,  711,  703,  722,  688,  722,  689,  722,  690,
 /*   200 */   722,  693,  694,  699,  700,  722,  697,  698,  722,  663,
 /*   210 */   722,  722,  695,  696,  664,  722,  722,  665,  722,  722,
 /*   220 */   691,  692,  720,  722,  722,  722,  722,  722,  722,  597,
 /*   230 */   722,  722,  601,  722,  602,  722,  604,  605,  606,  607,
 /*   240 */   608,  609,  722,  610,  649,  722,  722,  722,  611,  722,
 /*   250 */   722,  722,  612,  722,  722,  722,  613,  722,  722,  722,
 /*   260 */   722,  614,  722,  722,  722,  615,  722,  722,  722,  616,
 /*   270 */   722,  722,  722,  617,  722,  722,  722,  618,  619,  620,
 /*   280 */   603,  722,  722,  622,  722,  623,  625,  624,  722,  626,
 /*   290 */   722,  627,  722,  722,  628,  629,  722,  722,  722,  722,
 /*   300 */   630,  722,  722,  722,  631,  722,  722,  632,  722,  722,
 /*   310 */   633,  722,  722,  634,  722,  722,  635,  722,  645,  647,
 /*   320 */   722,  648,  646,  722,  722,  636,  722,  722,  637,  722,
 /*   330 */   722,  638,  722,  722,  639,  722,  722,  640,  722,  722,
 /*   340 */   641,  722,  722,  642,  722,  722,  643,  722,  722,  644,
 /*   350 */   722,  722,  722,  722,  722,  721,  559,  722,  722,  722,
 /*   360 */   722,  598,  722,  722,  722,  722,  722,  722,  599,  722,
 /*   370 */   722,  722,  722,  600,  722,  722,  722,  595,  593,  591,
 /*   380 */   722,  722,  590,  588,  583,  579,  722,  722,  722,  584,
 /*   390 */   580,  722,  722,  722,  521,  722,  523,  722,  538,  540,
 /*   400 */   722,  543,  722,  541,  722,  544,  722,  542,  722,  545,
 /*   410 */   722,  722,  722,  539,  528,  530,  531,  532,  533,  534,
 /*   420 */   535,  536,  537,  722,  722,  722,  722,  722,  722,  722,
 /*   430 */   524,  722,  526,  722,  722,  722,  525,  722,  527,  529,
 /*   440 */   722,  722,  514,  517,  722,  722,  722,  722,  722,  518,
 /*   450 */   722,  722,  722,  722,  519,  508,  722,  722,  722,  510,
 /*   460 */   722,  722,  512,  722,  722,  511,  722,  722,  513,  515,
 /*   470 */   722,  722,  722,  722,  722,  509,  722,  722,  722,  722,
 /*   480 */   495,  722,  722,  496,  488,
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
  "DOTCOMMA",      "CLASS",         "EXTENDS",       "ABSTRACT",    
  "BRACKET_OPEN",  "BRACKET_CLOSE",  "COMMENT",       "ASSIGN",      
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
  "THROW",         "CHAR",          "DOUBLE",        "NULL",        
  "TRUE",          "FALSE",         "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_comment",    "xx_class_body",  "xx_class_definition",
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
  "xx_declare_variable_list",  "xx_declare_variable",  "xx_isset_expr",  "xx_array_list",
  "xx_call_parameters",  "xx_call_parameter",  "xx_array_item",  "xx_array_key",
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
 /*   8 */ "xx_class_def ::= CLASS IDENTIFIER xx_class_body",
 /*   9 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  10 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  11 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  12 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  13 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  14 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  15 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  16 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  17 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  18 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  19 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  20 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  21 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  22 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  23 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  24 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  25 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  26 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  27 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  28 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  29 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  30 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  31 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  32 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  33 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  34 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  35 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  36 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  37 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  38 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  39 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  40 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  41 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  42 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  43 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  44 */ "xx_visibility_list ::= xx_visibility",
 /*  45 */ "xx_visibility ::= PUBLIC",
 /*  46 */ "xx_visibility ::= PROTECTED",
 /*  47 */ "xx_visibility ::= PRIVATE",
 /*  48 */ "xx_visibility ::= STATIC",
 /*  49 */ "xx_visibility ::= SCOPED",
 /*  50 */ "xx_visibility ::= INLINE",
 /*  51 */ "xx_visibility ::= ABSTRACT",
 /*  52 */ "xx_visibility ::= FINAL",
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
 /*  63 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /*  64 */ "xx_parameter_type ::= TYPE_LONG",
 /*  65 */ "xx_parameter_type ::= TYPE_ULONG",
 /*  66 */ "xx_parameter_type ::= TYPE_CHAR",
 /*  67 */ "xx_parameter_type ::= TYPE_UCHAR",
 /*  68 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  69 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  70 */ "xx_parameter_type ::= TYPE_STRING",
 /*  71 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  72 */ "xx_parameter_type ::= TYPE_VAR",
 /*  73 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  74 */ "xx_statement_list ::= xx_statement",
 /*  75 */ "xx_statement ::= xx_let_statement",
 /*  76 */ "xx_statement ::= xx_if_statement",
 /*  77 */ "xx_statement ::= xx_loop_statement",
 /*  78 */ "xx_statement ::= xx_echo_statement",
 /*  79 */ "xx_statement ::= xx_return_statement",
 /*  80 */ "xx_statement ::= xx_mcall_statement",
 /*  81 */ "xx_statement ::= xx_scall_statement",
 /*  82 */ "xx_statement ::= xx_unset_statement",
 /*  83 */ "xx_statement ::= xx_throw_statement",
 /*  84 */ "xx_statement ::= xx_declare_statement",
 /*  85 */ "xx_statement ::= xx_break_statement",
 /*  86 */ "xx_statement ::= xx_continue_statement",
 /*  87 */ "xx_statement ::= xx_while_statement",
 /*  88 */ "xx_statement ::= xx_do_while_statement",
 /*  89 */ "xx_statement ::= xx_switch_statement",
 /*  90 */ "xx_statement ::= xx_for_statement",
 /*  91 */ "xx_statement ::= xx_comment",
 /*  92 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /*  93 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /*  94 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /*  95 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  96 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  97 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  98 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /*  99 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 100 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 101 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 102 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 103 */ "xx_case_clauses ::= xx_case_clause",
 /* 104 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 105 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 106 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 107 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 108 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 109 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 110 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 111 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 112 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 113 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 114 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 115 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 116 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 117 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 118 */ "xx_let_assignments ::= xx_let_assignment",
 /* 119 */ "xx_assignment_operator ::= ASSIGN",
 /* 120 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 121 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 122 */ "xx_assignment_operator ::= MULASSIGN",
 /* 123 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 124 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 125 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 126 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 127 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 128 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 129 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 130 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 131 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 132 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 133 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 134 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 135 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 136 */ "xx_index_expr ::= xx_common_expr",
 /* 137 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 138 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 139 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 140 */ "xx_echo_expression ::= xx_common_expr",
 /* 141 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 142 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 143 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 144 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 145 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 146 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 147 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 148 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 149 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
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
 /* 178 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 179 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 180 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 181 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 182 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 183 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 184 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 185 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 186 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 187 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 188 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 189 */ "xx_common_expr ::= INSTANCEOF xx_common_expr",
 /* 190 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 191 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 192 */ "xx_common_expr ::= CONSTANT",
 /* 193 */ "xx_common_expr ::= IDENTIFIER",
 /* 194 */ "xx_common_expr ::= INTEGER",
 /* 195 */ "xx_common_expr ::= STRING",
 /* 196 */ "xx_common_expr ::= CHAR",
 /* 197 */ "xx_common_expr ::= DOUBLE",
 /* 198 */ "xx_common_expr ::= NULL",
 /* 199 */ "xx_common_expr ::= TRUE",
 /* 200 */ "xx_common_expr ::= FALSE",
 /* 201 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 202 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 203 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 204 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 205 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 206 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 207 */ "xx_common_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 208 */ "xx_common_expr ::= xx_mcall_expr",
 /* 209 */ "xx_common_expr ::= xx_scall_expr",
 /* 210 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 211 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 212 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 213 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 214 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 215 */ "xx_call_parameters ::= xx_call_parameter",
 /* 216 */ "xx_call_parameter ::= xx_common_expr",
 /* 217 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 218 */ "xx_array_list ::= xx_array_item",
 /* 219 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 220 */ "xx_array_item ::= xx_array_value",
 /* 221 */ "xx_array_key ::= IDENTIFIER",
 /* 222 */ "xx_array_key ::= STRING",
 /* 223 */ "xx_array_key ::= INTEGER",
 /* 224 */ "xx_array_value ::= xx_literal_expr",
 /* 225 */ "xx_array_value ::= IDENTIFIER",
 /* 226 */ "xx_literal_expr ::= INTEGER",
 /* 227 */ "xx_literal_expr ::= STRING",
 /* 228 */ "xx_literal_expr ::= DOUBLE",
 /* 229 */ "xx_literal_expr ::= NULL",
 /* 230 */ "xx_literal_expr ::= FALSE",
 /* 231 */ "xx_literal_expr ::= TRUE",
 /* 232 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE",
 /* 233 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE",
 /* 234 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE",
 /* 235 */ "xx_eval_expr ::= xx_common_expr",
 /* 236 */ "xx_comment ::= COMMENT",
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
// 2145 "parser.c"
      break;
    case 96:
// 849 "parser.lemon"
{ json_object_put((yypminor->yy75)); }
// 2150 "parser.c"
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
  { 100, 3 },
  { 100, 5 },
  { 100, 4 },
  { 100, 6 },
  { 102, 3 },
  { 102, 2 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 2 },
  { 103, 2 },
  { 103, 2 },
  { 103, 2 },
  { 103, 3 },
  { 103, 3 },
  { 104, 2 },
  { 104, 1 },
  { 107, 4 },
  { 107, 3 },
  { 107, 6 },
  { 107, 5 },
  { 105, 2 },
  { 105, 1 },
  { 106, 2 },
  { 106, 1 },
  { 110, 6 },
  { 110, 5 },
  { 111, 7 },
  { 111, 8 },
  { 111, 8 },
  { 111, 9 },
  { 111, 8 },
  { 111, 9 },
  { 111, 9 },
  { 111, 10 },
  { 108, 2 },
  { 108, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 114, 1 },
  { 112, 3 },
  { 112, 1 },
  { 115, 1 },
  { 115, 2 },
  { 115, 2 },
  { 115, 3 },
  { 115, 4 },
  { 115, 4 },
  { 117, 3 },
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
  { 113, 2 },
  { 113, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 118, 1 },
  { 129, 2 },
  { 130, 2 },
  { 120, 4 },
  { 120, 7 },
  { 120, 5 },
  { 120, 9 },
  { 120, 8 },
  { 120, 8 },
  { 133, 4 },
  { 133, 5 },
  { 136, 2 },
  { 136, 1 },
  { 137, 4 },
  { 137, 3 },
  { 121, 3 },
  { 121, 4 },
  { 131, 4 },
  { 131, 5 },
  { 132, 6 },
  { 132, 7 },
  { 134, 7 },
  { 134, 8 },
  { 134, 9 },
  { 134, 10 },
  { 119, 3 },
  { 139, 3 },
  { 139, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 140, 3 },
  { 140, 5 },
  { 140, 7 },
  { 140, 8 },
  { 140, 5 },
  { 140, 7 },
  { 140, 8 },
  { 140, 5 },
  { 140, 6 },
  { 140, 2 },
  { 140, 2 },
  { 143, 1 },
  { 122, 3 },
  { 144, 3 },
  { 144, 1 },
  { 145, 1 },
  { 124, 2 },
  { 125, 2 },
  { 123, 3 },
  { 123, 2 },
  { 126, 5 },
  { 126, 6 },
  { 126, 6 },
  { 126, 6 },
  { 127, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 128, 3 },
  { 148, 3 },
  { 148, 1 },
  { 149, 1 },
  { 149, 3 },
  { 142, 1 },
  { 138, 2 },
  { 138, 2 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 4 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 4 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 3 },
  { 138, 2 },
  { 138, 4 },
  { 138, 2 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 2 },
  { 138, 3 },
  { 138, 2 },
  { 138, 4 },
  { 138, 5 },
  { 138, 4 },
  { 138, 3 },
  { 138, 1 },
  { 138, 1 },
  { 147, 6 },
  { 147, 5 },
  { 146, 6 },
  { 146, 5 },
  { 152, 3 },
  { 152, 1 },
  { 153, 1 },
  { 151, 3 },
  { 151, 1 },
  { 154, 3 },
  { 154, 1 },
  { 155, 1 },
  { 155, 1 },
  { 155, 1 },
  { 156, 1 },
  { 156, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 109, 1 },
  { 150, 4 },
  { 150, 4 },
  { 150, 4 },
  { 135, 1 },
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
	status->ret = yymsp[0].minor.yy75;
}
// 2604 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
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
      case 136:
      case 164:
      case 208:
      case 209:
      case 216:
      case 224:
      case 235:
// 851 "parser.lemon"
{
	yygotominor.yy75 = yymsp[0].minor.yy75;
}
// 2638 "parser.c"
        break;
      case 2:
      case 23:
      case 29:
      case 31:
      case 43:
      case 73:
      case 102:
// 855 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_list(yymsp[-1].minor.yy75, yymsp[0].minor.yy75);
}
// 2651 "parser.c"
        break;
      case 3:
      case 24:
      case 30:
      case 32:
      case 44:
      case 54:
      case 74:
      case 103:
      case 118:
      case 139:
      case 161:
      case 215:
      case 218:
// 859 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_list(NULL, yymsp[0].minor.yy75);
}
// 2670 "parser.c"
        break;
      case 7:
// 875 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2679 "parser.c"
        break;
      case 8:
// 879 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy75, 0, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
}
// 2687 "parser.c"
        break;
      case 9:
// 883 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy75, 0, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2696 "parser.c"
        break;
      case 10:
// 887 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy75, 1, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(33,&yymsp[-2].minor);
}
// 2705 "parser.c"
        break;
      case 11:
// 891 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy75, 1, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[-5].minor);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
}
// 2715 "parser.c"
        break;
      case 12:
// 895 "parser.lemon"
{
	yygotominor.yy75 = yymsp[-1].minor.yy75;
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2724 "parser.c"
        break;
      case 13:
// 899 "parser.lemon"
{
	yygotominor.yy75 = NULL;
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2733 "parser.c"
        break;
      case 14:
// 903 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(yymsp[0].minor.yy75, NULL, NULL, status->scanner_state);
}
// 2740 "parser.c"
        break;
      case 15:
// 907 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy75, status->scanner_state);
}
// 2747 "parser.c"
        break;
      case 16:
// 911 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(NULL, yymsp[0].minor.yy75, NULL, status->scanner_state);
}
// 2754 "parser.c"
        break;
      case 17:
// 915 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(yymsp[-1].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
}
// 2761 "parser.c"
        break;
      case 18:
// 919 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(yymsp[-1].minor.yy75, NULL, yymsp[0].minor.yy75, status->scanner_state);
}
// 2768 "parser.c"
        break;
      case 19:
// 923 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(yymsp[0].minor.yy75, NULL, yymsp[-1].minor.yy75, status->scanner_state);
}
// 2775 "parser.c"
        break;
      case 20:
// 927 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(NULL, yymsp[0].minor.yy75, yymsp[-1].minor.yy75, status->scanner_state);
}
// 2782 "parser.c"
        break;
      case 21:
// 931 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(yymsp[-2].minor.yy75, yymsp[0].minor.yy75, yymsp[-1].minor.yy75, status->scanner_state);
}
// 2789 "parser.c"
        break;
      case 22:
// 935 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_definition(yymsp[-1].minor.yy75, yymsp[0].minor.yy75, yymsp[-2].minor.yy75, status->scanner_state);
}
// 2796 "parser.c"
        break;
      case 25:
// 947 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_property(yymsp[-2].minor.yy75, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2804 "parser.c"
        break;
      case 26:
// 951 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_property(yymsp[-2].minor.yy75, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2812 "parser.c"
        break;
      case 27:
// 955 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_property(yymsp[-4].minor.yy75, yymsp[-3].minor.yy0, yymsp[-1].minor.yy75, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2821 "parser.c"
        break;
      case 28:
// 959 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_property(yymsp[-4].minor.yy75, yymsp[-3].minor.yy0, yymsp[-1].minor.yy75, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2830 "parser.c"
        break;
      case 33:
// 979 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy75, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2840 "parser.c"
        break;
      case 34:
// 983 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy75, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2850 "parser.c"
        break;
      case 35:
// 987 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_method(yymsp[-6].minor.yy75, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2862 "parser.c"
        break;
      case 36:
// 991 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_method(yymsp[-7].minor.yy75, yymsp[-5].minor.yy0, yymsp[-3].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2874 "parser.c"
        break;
      case 37:
// 995 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_method(yymsp[-7].minor.yy75, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy75, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2886 "parser.c"
        break;
      case 38:
// 999 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_method(yymsp[-8].minor.yy75, yymsp[-6].minor.yy0, yymsp[-4].minor.yy75, yymsp[-1].minor.yy75, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-7].minor);
  yy_destructor(43,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2898 "parser.c"
        break;
      case 39:
// 1003 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_method(yymsp[-6].minor.yy75, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2910 "parser.c"
        break;
      case 40:
// 1007 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_method(yymsp[-7].minor.yy75, yymsp[-5].minor.yy0, yymsp[-3].minor.yy75, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2922 "parser.c"
        break;
      case 41:
// 1011 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_method(yymsp[-7].minor.yy75, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy75, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2934 "parser.c"
        break;
      case 42:
// 1015 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_class_method(yymsp[-8].minor.yy75, yymsp[-6].minor.yy0, yymsp[-4].minor.yy75, yymsp[-1].minor.yy75, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(42,&yymsp[-7].minor);
  yy_destructor(43,&yymsp[-5].minor);
  yy_destructor(29,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 2946 "parser.c"
        break;
      case 45:
// 1027 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2954 "parser.c"
        break;
      case 46:
// 1031 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 2962 "parser.c"
        break;
      case 47:
// 1035 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 2970 "parser.c"
        break;
      case 48:
// 1039 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 2978 "parser.c"
        break;
      case 49:
// 1043 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 2986 "parser.c"
        break;
      case 50:
// 1047 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("inline");
  yy_destructor(44,&yymsp[0].minor);
}
// 2994 "parser.c"
        break;
      case 51:
// 1051 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("abstract");
  yy_destructor(35,&yymsp[0].minor);
}
// 3002 "parser.c"
        break;
      case 52:
// 1055 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("final");
  yy_destructor(45,&yymsp[0].minor);
}
// 3010 "parser.c"
        break;
      case 53:
      case 117:
      case 138:
      case 160:
      case 214:
      case 217:
// 1059 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_list(yymsp[-2].minor.yy75, yymsp[0].minor.yy75);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3023 "parser.c"
        break;
      case 55:
// 1067 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3030 "parser.c"
        break;
      case 56:
// 1071 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_parameter(yymsp[-1].minor.yy75, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3037 "parser.c"
        break;
      case 57:
// 1075 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_parameter(NULL, yymsp[-1].minor.yy75, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3044 "parser.c"
        break;
      case 58:
// 1079 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3052 "parser.c"
        break;
      case 59:
// 1083 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_parameter(yymsp[-3].minor.yy75, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3060 "parser.c"
        break;
      case 60:
// 1087 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_parameter(NULL, yymsp[-3].minor.yy75, yymsp[-2].minor.yy0, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3068 "parser.c"
        break;
      case 61:
// 1091 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 3077 "parser.c"
        break;
      case 62:
// 1095 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(46,&yymsp[0].minor);
}
// 3085 "parser.c"
        break;
      case 63:
// 1099 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(47,&yymsp[0].minor);
}
// 3093 "parser.c"
        break;
      case 64:
// 1103 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(48,&yymsp[0].minor);
}
// 3101 "parser.c"
        break;
      case 65:
// 1107 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(49,&yymsp[0].minor);
}
// 3109 "parser.c"
        break;
      case 66:
// 1111 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(50,&yymsp[0].minor);
}
// 3117 "parser.c"
        break;
      case 67:
// 1115 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(51,&yymsp[0].minor);
}
// 3125 "parser.c"
        break;
      case 68:
// 1119 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(52,&yymsp[0].minor);
}
// 3133 "parser.c"
        break;
      case 69:
// 1123 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(53,&yymsp[0].minor);
}
// 3141 "parser.c"
        break;
      case 70:
// 1127 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(54,&yymsp[0].minor);
}
// 3149 "parser.c"
        break;
      case 71:
// 1131 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(55,&yymsp[0].minor);
}
// 3157 "parser.c"
        break;
      case 72:
// 1135 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3165 "parser.c"
        break;
      case 92:
// 1215 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3174 "parser.c"
        break;
      case 93:
// 1219 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3183 "parser.c"
        break;
      case 94:
// 1223 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_if_statement(yymsp[-2].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3193 "parser.c"
        break;
      case 95:
// 1227 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_if_statement(yymsp[-5].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3206 "parser.c"
        break;
      case 96:
// 1231 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_if_statement(yymsp[-3].minor.yy75, yymsp[-1].minor.yy75, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3216 "parser.c"
        break;
      case 97:
// 1235 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_if_statement(yymsp[-7].minor.yy75, yymsp[-5].minor.yy75, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(59,&yymsp[-8].minor);
  yy_destructor(36,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3229 "parser.c"
        break;
      case 98:
// 1239 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_if_statement(yymsp[-6].minor.yy75, yymsp[-4].minor.yy75, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3242 "parser.c"
        break;
      case 99:
// 1243 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_if_statement(yymsp[-6].minor.yy75, NULL, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(59,&yymsp[-7].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(60,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3255 "parser.c"
        break;
      case 100:
// 1247 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_switch_statement(yymsp[-2].minor.yy75, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3265 "parser.c"
        break;
      case 101:
// 1251 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_switch_statement(yymsp[-3].minor.yy75, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3275 "parser.c"
        break;
      case 104:
// 1263 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_case_clause(yymsp[-2].minor.yy75, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3284 "parser.c"
        break;
      case 105:
// 1267 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_case_clause(NULL, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3293 "parser.c"
        break;
      case 106:
// 1271 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3303 "parser.c"
        break;
      case 107:
// 1275 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_loop_statement(yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3313 "parser.c"
        break;
      case 108:
// 1279 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_while_statement(yymsp[-2].minor.yy75, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(36,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3323 "parser.c"
        break;
      case 109:
// 1283 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_while_statement(yymsp[-3].minor.yy75, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3333 "parser.c"
        break;
      case 110:
// 1287 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_do_while_statement(yymsp[-1].minor.yy75, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-5].minor);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3345 "parser.c"
        break;
      case 111:
// 1291 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_do_while_statement(yymsp[-1].minor.yy75, yymsp[-4].minor.yy75, status->scanner_state);
  yy_destructor(67,&yymsp[-6].minor);
  yy_destructor(36,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3357 "parser.c"
        break;
      case 112:
// 1295 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_for_statement(yymsp[-3].minor.yy75, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(68,&yymsp[-6].minor);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3368 "parser.c"
        break;
      case 113:
// 1299 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_for_statement(yymsp[-3].minor.yy75, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(68,&yymsp[-7].minor);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3380 "parser.c"
        break;
      case 114:
// 1303 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_for_statement(yymsp[-3].minor.yy75, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(68,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(69,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3392 "parser.c"
        break;
      case 115:
// 1307 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_for_statement(yymsp[-3].minor.yy75, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(68,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(69,&yymsp[-5].minor);
  yy_destructor(70,&yymsp[-4].minor);
  yy_destructor(36,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 3405 "parser.c"
        break;
      case 116:
// 1311 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_statement(yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3414 "parser.c"
        break;
      case 119:
// 1324 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("assign");
  yy_destructor(39,&yymsp[0].minor);
}
// 3422 "parser.c"
        break;
      case 120:
// 1329 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("add-assign");
  yy_destructor(72,&yymsp[0].minor);
}
// 3430 "parser.c"
        break;
      case 121:
// 1334 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("sub-assign");
  yy_destructor(73,&yymsp[0].minor);
}
// 3438 "parser.c"
        break;
      case 122:
// 1338 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("mult-assign");
  yy_destructor(74,&yymsp[0].minor);
}
// 3446 "parser.c"
        break;
      case 123:
// 1342 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("div-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3454 "parser.c"
        break;
      case 124:
// 1346 "parser.lemon"
{
	yygotominor.yy75 = json_object_new_string("concat-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3462 "parser.c"
        break;
      case 125:
// 1351 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy75, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy75, status->scanner_state);
}
// 3469 "parser.c"
        break;
      case 126:
// 1356 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy75, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
}
// 3477 "parser.c"
        break;
      case 127:
// 1361 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy75, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(77,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3487 "parser.c"
        break;
      case 128:
// 1366 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy75, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy75, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(77,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3497 "parser.c"
        break;
      case 129:
// 1371 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy75, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3505 "parser.c"
        break;
      case 130:
// 1376 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy75, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3515 "parser.c"
        break;
      case 131:
// 1381 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy75, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy75, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3525 "parser.c"
        break;
      case 132:
// 1386 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy75, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3534 "parser.c"
        break;
      case 133:
// 1391 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy75, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy75, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3543 "parser.c"
        break;
      case 134:
// 1396 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[0].minor);
}
// 3551 "parser.c"
        break;
      case 135:
// 1401 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[0].minor);
}
// 3559 "parser.c"
        break;
      case 137:
// 1409 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_echo_statement(yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3568 "parser.c"
        break;
      case 140:
// 1421 "parser.lemon"
{
	yygotominor.yy75 = yymsp[0].minor.yy75;;
}
// 3575 "parser.c"
        break;
      case 141:
// 1426 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_mcall_statement(yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3583 "parser.c"
        break;
      case 142:
// 1431 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_scall_statement(yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3591 "parser.c"
        break;
      case 143:
// 1436 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_return_statement(yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3600 "parser.c"
        break;
      case 144:
// 1441 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3609 "parser.c"
        break;
      case 145:
// 1446 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-4].minor);
  yy_destructor(77,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3623 "parser.c"
        break;
      case 146:
// 1455 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3638 "parser.c"
        break;
      case 147:
// 1464 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_INTEGER, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3653 "parser.c"
        break;
      case 148:
// 1473 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_STRING, yymsp[-2].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(85,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3668 "parser.c"
        break;
      case 149:
// 1482 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_throw_exception(yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3677 "parser.c"
        break;
      case 150:
// 1486 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3686 "parser.c"
        break;
      case 151:
// 1490 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3695 "parser.c"
        break;
      case 152:
// 1494 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3704 "parser.c"
        break;
      case 153:
// 1498 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3713 "parser.c"
        break;
      case 154:
// 1502 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3722 "parser.c"
        break;
      case 155:
// 1506 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3731 "parser.c"
        break;
      case 156:
// 1510 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3740 "parser.c"
        break;
      case 157:
// 1514 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3749 "parser.c"
        break;
      case 158:
// 1518 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3758 "parser.c"
        break;
      case 159:
// 1522 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3767 "parser.c"
        break;
      case 162:
// 1534 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3774 "parser.c"
        break;
      case 163:
// 1538 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3782 "parser.c"
        break;
      case 165:
// 1546 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("not", yymsp[0].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 3790 "parser.c"
        break;
      case 166:
// 1550 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("isset", yymsp[0].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3798 "parser.c"
        break;
      case 167:
// 1554 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("equals", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3806 "parser.c"
        break;
      case 168:
// 1558 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("not-equals", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3814 "parser.c"
        break;
      case 169:
// 1562 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("identical", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3822 "parser.c"
        break;
      case 170:
// 1566 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("not-identical", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3830 "parser.c"
        break;
      case 171:
// 1570 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("less", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3838 "parser.c"
        break;
      case 172:
// 1574 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("greater", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3846 "parser.c"
        break;
      case 173:
// 1578 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("less-equal", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3854 "parser.c"
        break;
      case 174:
// 1582 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3862 "parser.c"
        break;
      case 175:
// 1586 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("list", yymsp[-1].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3871 "parser.c"
        break;
      case 176:
// 1590 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("cast", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
}
// 3880 "parser.c"
        break;
      case 177:
// 1594 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 3888 "parser.c"
        break;
      case 178:
// 1598 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 3896 "parser.c"
        break;
      case 179:
// 1602 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 3904 "parser.c"
        break;
      case 180:
// 1607 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy75, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 3913 "parser.c"
        break;
      case 181:
// 1612 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("add", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3921 "parser.c"
        break;
      case 182:
// 1617 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("sub", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3929 "parser.c"
        break;
      case 183:
// 1622 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("mul", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 3937 "parser.c"
        break;
      case 184:
// 1627 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("div", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3945 "parser.c"
        break;
      case 185:
// 1632 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("mod", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3953 "parser.c"
        break;
      case 186:
// 1637 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("concat", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 3961 "parser.c"
        break;
      case 187:
// 1642 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("and", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 3969 "parser.c"
        break;
      case 188:
// 1647 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("or", yymsp[-2].minor.yy75, yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 3977 "parser.c"
        break;
      case 189:
// 1652 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("instanceof", yymsp[0].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 3985 "parser.c"
        break;
      case 190:
// 1657 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy75, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3994 "parser.c"
        break;
      case 191:
// 1662 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("typeof", yymsp[0].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4002 "parser.c"
        break;
      case 192:
// 1667 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4009 "parser.c"
        break;
      case 193:
      case 221:
      case 225:
// 1672 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4018 "parser.c"
        break;
      case 194:
      case 223:
      case 226:
// 1677 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4027 "parser.c"
        break;
      case 195:
      case 222:
      case 227:
// 1682 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4036 "parser.c"
        break;
      case 196:
// 1687 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4043 "parser.c"
        break;
      case 197:
      case 228:
// 1692 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4051 "parser.c"
        break;
      case 198:
      case 229:
// 1697 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[0].minor);
}
// 4060 "parser.c"
        break;
      case 199:
      case 231:
// 1702 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4069 "parser.c"
        break;
      case 200:
      case 230:
// 1707 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4078 "parser.c"
        break;
      case 201:
// 1712 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-1].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4087 "parser.c"
        break;
      case 202:
// 1717 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("array", yymsp[-1].minor.yy75, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4096 "parser.c"
        break;
      case 203:
// 1722 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4104 "parser.c"
        break;
      case 204:
// 1727 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4114 "parser.c"
        break;
      case 205:
// 1732 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4124 "parser.c"
        break;
      case 206:
// 1737 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4133 "parser.c"
        break;
      case 207:
// 1742 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4142 "parser.c"
        break;
      case 210:
// 1757 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4152 "parser.c"
        break;
      case 211:
// 1762 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4162 "parser.c"
        break;
      case 212:
// 1767 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy75, status->scanner_state);
  yy_destructor(77,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4172 "parser.c"
        break;
      case 213:
// 1772 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4182 "parser.c"
        break;
      case 219:
// 1796 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_array_item(yymsp[-2].minor.yy75, yymsp[0].minor.yy75, status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
}
// 4190 "parser.c"
        break;
      case 220:
// 1800 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_array_item(NULL, yymsp[0].minor.yy75, status->scanner_state);
}
// 4197 "parser.c"
        break;
      case 232:
// 1849 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4206 "parser.c"
        break;
      case 233:
// 1854 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4215 "parser.c"
        break;
      case 234:
// 1859 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4224 "parser.c"
        break;
      case 236:
// 1867 "parser.lemon"
{
	yygotominor.yy75 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4231 "parser.c"
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

// 4307 "parser.c"
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
			/*case XX_T_INTERFACE:
				xx_(xx_parser, XX_INTERFACE, NULL, parser_status);
				break;*/
			case XX_T_EXTENDS:
				xx_(xx_parser, XX_EXTENDS, NULL, parser_status);
				break;
			/*case XX_T_IMPLEMENTS:
				xx_(xx_parser, XX_IMPLEMENTS, NULL, parser_status);
				break;*/
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
			case XX_T_CHAR:
				xx_parse_with_token(xx_parser, XX_T_CHAR, XX_CHAR, &token, parser_status);
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
