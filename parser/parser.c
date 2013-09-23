/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 48 "parser.lemon"


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

static json_object *xx_ret_class(xx_parser_token *T, json_object *class_definition, int is_abstract, int is_final,
	xx_parser_token *E, json_object *I, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("class"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	json_object_object_add(ret, "abtract", json_object_new_int(is_abstract));
	json_object_object_add(ret, "final", json_object_new_int(is_final));

	if (E) {
		json_object_object_add(ret, "extends", json_object_new_string(E->token));
	}

	if (I) {
		json_object_object_add(ret, "implements", I);
	}

	if (class_definition) {
		json_object_object_add(ret, "definition", class_definition);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_interface(xx_parser_token *T, json_object *methods_definition, xx_parser_token *E, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("interface"));
	json_object_object_add(ret, "name", json_object_new_string(T->token));

	if (E) {
		json_object_object_add(ret, "extends", json_object_new_string(E->token));
	}

	if (methods_definition) {
		json_object_object_add(ret, "methods", methods_definition);
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

static json_object *xx_ret_interface_definition(json_object *methods, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	if (methods) {
		json_object_object_add(ret, "methods", methods);
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

static json_object *xx_ret_class_method(json_object *visibility, xx_parser_token *T, json_object *parameters,
	json_object *statements, xx_parser_token *D, xx_scanner_state *state)
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

	if (expr) {
		json_object_object_add(ret, "type", json_object_new_string("case"));
		json_object_object_add(ret, "expr", expr);
	} else {
		json_object_object_add(ret, "type", json_object_new_string("default"));
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

static json_object *xx_ret_empty_statement(xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("empty"));

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

static json_object *xx_ret_fcall_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fcall"));
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

static json_object *xx_ret_fcall(int type, xx_parser_token *F, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fcall"));
	json_object_object_add(ret, "name", json_object_new_string(F->token));
	json_object_object_add(ret, "call-type", json_object_new_int(type));

	if (parameters) {
		json_object_object_add(ret, "parameters", parameters);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_mcall(int type, xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
	json_object_object_add(ret, "variable", json_object_new_string(O->token));
	json_object_object_add(ret, "name", json_object_new_string(M->token));
	json_object_object_add(ret, "call-type", json_object_new_int(type));

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


// 839 "parser.c"
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
#define YYNOCODE 172
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy7;
  int yy343;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 579
#define YYNRULE 274
#define YYERRORSYMBOL 99
#define YYERRSYMDT yy343
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
 /*     0 */   183,  186,  189,    3,    4,    5,    6,    7,  225,  854,
 /*    10 */     1,    2,  578,    4,    5,    6,    7,   32,   46,   50,
 /*    20 */   179,  234,  238,  254,  176,  419,  426,  229,  517,  104,
 /*    30 */   532,  540,  575,  546,  520,  261,  106,   99,   88,  105,
 /*    40 */   240,  145,  219,   88,   57,   58,   59,   60,   61,   62,
 /*    50 */    63,   64,   65,   66,   67,  217,  213,  215,  178,  188,
 /*    60 */   193,  195,  197,  199,  191,  185,  201,  203,  211,  205,
 /*    70 */   207,  209,  205,  207,  209,  579,  242,  248,  201,  203,
 /*    80 */   211,  205,  207,  209,  454,  135,  241,  243,  244,  245,
 /*    90 */   246,  247,  261,  144,  459,   85,  421,  252,   68,   46,
 /*   100 */    50,  418,  427,  436,  439,  430,  433,  442,  448,  445,
 /*   110 */   276,  451,  136,  138,  140,  559,  149,  557,  466,  542,
 /*   120 */   160,  164,  169,  320,  394,  438,  327,  454,  135,  543,
 /*   130 */   413,  393,  268,  270,  269,  261,  493,  459,    9,  389,
 /*   140 */   402,  406,  415,  421,  418,  427,  436,  439,  430,  433,
 /*   150 */   442,  448,  445,  502,  451,  136,  138,  140,  536,  149,
 /*   160 */    88,   10,  300,  160,  164,  169,  320,  394,  537,  327,
 /*   170 */   454,  135,  420,  390,  395,  268,  270,  269,  261,  483,
 /*   180 */   459,  554,  389,  402,  406,  415,  322,  418,  427,  436,
 /*   190 */   439,  430,  433,  442,  448,  445,   12,  451,  136,  138,
 /*   200 */   140,  569,  149,  557,  328,  388,  160,  164,  169,  320,
 /*   210 */   354,  306,  327,  454,  135,  374,  231,   99,  268,  270,
 /*   220 */   269,  261,  326,  459,  299,  389,  402,  406,  415,   13,
 /*   230 */   418,  427,  436,  439,  430,  433,  442,  448,  445,   14,
 /*   240 */   451,  136,  138,  140,   17,  149,  316,   79,  182,  160,
 /*   250 */   164,  169,  320,  289,   88,  327,  454,  135,  549,  532,
 /*   260 */   540,  437,  426,  520,  261,  163,  459,   88,  389,  402,
 /*   270 */   406,  415,   15,  418,  427,  436,  439,  430,  433,  442,
 /*   280 */   448,  445,   17,  451,  136,  138,  140,   24,  149,  428,
 /*   290 */   426,  358,  160,  164,  169,  320,  330,   16,  327,  454,
 /*   300 */   135,  518,   97,  106,  557,  519,  520,  261,  478,  459,
 /*   310 */    88,  389,  402,  406,  415,  421,  418,  427,  436,  439,
 /*   320 */   430,  433,  442,  448,  445,  329,  451,  136,  138,  140,
 /*   330 */    69,  149,  288,  452,  426,  160,  164,  169,  320,  249,
 /*   340 */   362,  327,  454,  135,  429,  343,  421,  268,  270,  269,
 /*   350 */   261,  482,  459,  357,  389,  402,  406,  415,  293,  418,
 /*   360 */   427,  436,  439,  430,  433,  442,  448,  445,   25,  451,
 /*   370 */   136,  138,  140,   27,  149,  453,  449,  426,  160,  164,
 /*   380 */   169,  320,  354,   30,  327,  454,  135,  382,  458,  421,
 /*   390 */   268,  270,  269,  261,  500,  459,  299,  389,  402,  406,
 /*   400 */   415,  222,  418,  427,  436,  439,  430,  433,  442,  448,
 /*   410 */   445,   34,  451,  136,  138,  140,   35,  149,  450,  534,
 /*   420 */    44,  160,  164,  169,  320,  220,   88,  327,  454,  135,
 /*   430 */   421,  446,  426,  268,  270,  269,  261,  498,  459,   45,
 /*   440 */   389,  402,  406,  415,   47,  418,  427,  436,  439,  430,
 /*   450 */   433,  442,  448,  445,   48,  451,  136,  138,  140,  447,
 /*   460 */   149,  443,  426,   49,  160,  164,  169,  320,  354,   51,
 /*   470 */   327,  454,  135,  350,   52,  421,  268,  270,  269,  261,
 /*   480 */   494,  459,   53,  389,  402,  406,  415,  392,  418,  427,
 /*   490 */   436,  439,  430,  433,  442,  448,  445,   55,  451,  136,
 /*   500 */   138,  140,   56,  149,  444,  431,  426,  160,  164,  169,
 /*   510 */   320,  318,  659,  327,  454,  135,  391,   81,  421,  268,
 /*   520 */   270,  269,  261,  508,  459,   82,  389,  402,  406,  415,
 /*   530 */   421,  418,  427,  436,  439,  430,  433,  442,  448,  445,
 /*   540 */    84,  451,  136,  138,  140,   87,  149,  441,  440,  426,
 /*   550 */   160,  164,  169,  320,  278,   91,  327,  454,  135,  432,
 /*   560 */    92,  421,  268,  270,  269,  261,  112,  459,   94,  389,
 /*   570 */   402,  406,  415,   95,  418,  427,  436,  439,  430,  433,
 /*   580 */   442,  448,  445,  556,  451,  136,  138,  140,   98,  149,
 /*   590 */   435,  153,  487,  160,  164,  169,  320,  313,  108,  327,
 /*   600 */   454,  135,  408,  102,  411,  268,  270,  269,  261,  510,
 /*   610 */   459,  109,  389,  402,  406,  415,  111,  418,  427,  436,
 /*   620 */   439,  430,  433,  442,  448,  445,  137,  451,  136,  138,
 /*   630 */   140,  139,  149,  434,  426,  142,  160,  164,  169,  320,
 /*   640 */   187,  146,  327,  454,  135,  151,  155,  157,  268,  270,
 /*   650 */   269,  261,  489,  459,  158,  389,  402,  406,  415,  161,
 /*   660 */   418,  427,  436,  439,  430,  433,  442,  448,  445,  166,
 /*   670 */   451,  136,  138,  140,  170,  149,  173,  175,  181,  160,
 /*   680 */   164,  169,  320,  312,  226,  327,  454,  135,  223,  180,
 /*   690 */   227,  268,  270,  269,  261,  472,  459,  232,  389,  402,
 /*   700 */   406,  415,  235,  418,  427,  436,  439,  430,  433,  442,
 /*   710 */   448,  445,  236,  451,  136,  138,  140,  237,  149,  253,
 /*   720 */   255,  256,  160,  164,  169,  320,  177,  262,  327,  454,
 /*   730 */   135,  263,  264,  839,  268,  270,  269,  261,  168,  459,
 /*   740 */   301,  389,  402,  406,  415,  838,  418,  427,  436,  439,
 /*   750 */   430,  433,  442,  448,  445,  292,  451,  136,  138,  140,
 /*   760 */   302,  149,  307,  308,  317,  160,  164,  169,  320,  190,
 /*   770 */   321,  327,  454,  135,  332,  331,  351,  268,  270,  269,
 /*   780 */   261,  488,  459,  339,  389,  402,  406,  415,  344,  418,
 /*   790 */   427,  436,  439,  430,  433,  442,  448,  445,  359,  451,
 /*   800 */   136,  138,  140,  348,  149,  352,  363,  360,  160,  164,
 /*   810 */   169,  320,  224,  364,  327,  454,  135,  367,  368,  375,
 /*   820 */   268,  270,  269,  261,  148,  459,  383,  389,  402,  406,
 /*   830 */   415,  397,  418,  427,  436,  439,  430,  433,  442,  448,
 /*   840 */   445,  399,  451,  136,  138,  140,  372,  149,  376,  401,
 /*   850 */   380,  160,  164,  169,  320,  314,  384,  327,  454,  135,
 /*   860 */   407,  409,  423,  268,  270,  269,  261,  465,  459,  410,
 /*   870 */   389,  402,  406,  415,  414,  418,  427,  436,  439,  430,
 /*   880 */   433,  442,  448,  445,  422,  451,  136,  138,  140,  424,
 /*   890 */   149,  425,  456,  467,  160,  164,  169,  320,  481,  468,
 /*   900 */   327,  454,  135,  495,  485,  490,   68,   46,   50,  261,
 /*   910 */   514,  459,  479,  389,  402,  406,  415,  491,  418,  427,
 /*   920 */   436,  439,  430,  433,  442,  448,  445,  497,  451,  136,
 /*   930 */   138,  140,  504,  149,  507,  505,  513,  160,  164,  169,
 /*   940 */   320,  192,  523,  327,  454,  135,  524,  526,  525,  268,
 /*   950 */   270,  269,  261,  516,  459,  528,  389,  402,  406,  415,
 /*   960 */   529,  418,  427,  436,  439,  430,  433,  442,  448,  445,
 /*   970 */   530,  451,  136,  138,  140,  531,  149,  539,  545,  538,
 /*   980 */   160,  164,  169,  320,  239,  544,  327,  454,  135,  555,
 /*   990 */   560,  563,  268,  270,  269,  261,  114,  459,  562,  389,
 /*  1000 */   402,  406,  415,  566,  418,  427,  436,  439,  430,  433,
 /*  1010 */   442,  448,  445,  564,  451,  136,  138,  140,   99,  149,
 /*  1020 */   567,  572,  570,  160,  164,  169,  320,  194,  573,  327,
 /*  1030 */   454,  135,  574,  576,  577,  268,  270,  269,  261,  172,
 /*  1040 */   459,  447,  389,  402,  406,  415,  447,  418,  427,  436,
 /*  1050 */   439,  430,  433,  442,  448,  445,  447,  451,  136,  138,
 /*  1060 */   140,  447,  149,  447,  447,  447,  160,  164,  169,  320,
 /*  1070 */   196,  447,  327,  454,  135,  447,  447,  447,  268,  270,
 /*  1080 */   269,  261,  477,  459,  447,  389,  402,  406,  415,  447,
 /*  1090 */   418,  427,  436,  439,  430,  433,  442,  448,  445,  447,
 /*  1100 */   451,  136,  138,  140,  447,  149,  447,  447,  447,  160,
 /*  1110 */   164,  169,  320,  462,  447,  327,  454,  135,  447,  447,
 /*  1120 */   447,  268,  270,  269,  261,  447,  459,  447,  389,  402,
 /*  1130 */   406,  415,  333,  418,  427,  436,  439,  430,  433,  442,
 /*  1140 */   448,  445,  447,  451,  136,  138,  140,  447,  149,  447,
 /*  1150 */   447,   19,  160,  164,  169,  320,   22,  447,  327,  133,
 /*  1160 */    89,  447,  447,   88,  334,  335,  336,  337,  338,  469,
 /*  1170 */   333,  389,  402,  406,  415,  260,  464,  268,  270,  269,
 /*  1180 */   447,  460,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1190 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  134,
 /*  1200 */   133,  259,  334,  335,  336,  337,  338,  447,  447,  370,
 /*  1210 */   341,  396,  398,  400,  385,  447,  447,  499,  268,  270,
 /*  1220 */   269,  447,  460,  116,  117,  118,  119,  120,  121,  122,
 /*  1230 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1240 */   134,  133,   96,  447,  548,   99,  551,  552,  540,  447,
 /*  1250 */   519,  520,  396,  398,  400,   88,   28,   31,  492,   68,
 /*  1260 */    46,   50,  447,  460,  116,  117,  118,  119,  120,  121,
 /*  1270 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1280 */   132,  134,  133,  496,   70,   71,   73,   72,   74,  480,
 /*  1290 */   447,  447,  319,  396,  398,  400,  447,  447,  447,  167,
 /*  1300 */   268,  270,  269,  447,  460,  116,  117,  118,  119,  120,
 /*  1310 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1320 */   131,  132,  134,   76,   77,  133,  610,  521,  447,  527,
 /*  1330 */   412,  447,  447,   75,  396,  398,  400,   22,  268,  270,
 /*  1340 */   269,   21,  486,  447,   88,  260,  447,  460,  116,  117,
 /*  1350 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*  1360 */   128,  129,  130,  131,  132,  134,  133,  447,  447,  165,
 /*  1370 */   447,  282,  319,  174,  447,  447,  319,  396,  398,  400,
 /*  1380 */   268,  270,  269,  325,  268,  270,  269,  447,  460,  116,
 /*  1390 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1400 */   127,  128,  129,  130,  131,  132,  134,  133,  447,   70,
 /*  1410 */    71,   73,   72,   74,  447,  447,  447,  341,  396,  398,
 /*  1420 */   400,  365,  447,  447,  476,  268,  270,  269,  447,  460,
 /*  1430 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1440 */   126,  127,  128,  129,  130,  131,  132,  134,   76,   77,
 /*  1450 */   133,  606,  533,  141,  527,  447,  319,  447,   75,  396,
 /*  1460 */   398,  400,  280,  280,  268,  270,  269,  471,  260,  447,
 /*  1470 */   447,  447,  460,  116,  117,  118,  119,  120,  121,  122,
 /*  1480 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1490 */   134,  133,  447,  447,  297,  230,  457,  277,  447,  251,
 /*  1500 */   455,  341,  396,  398,  400,  381,  447,  260,  113,  268,
 /*  1510 */   270,  269,  447,  460,  116,  117,  118,  119,  120,  121,
 /*  1520 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1530 */   132,  134,  133,  304,   70,   71,   73,   72,   74,  447,
 /*  1540 */   447,  447,  341,  396,  398,  400,  361,  447,  447,  162,
 /*  1550 */   268,  270,  269,  447,  460,  116,  117,  118,  119,  120,
 /*  1560 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1570 */   131,  132,  134,   76,   77,  133,  100,  533,  150,  527,
 /*  1580 */   447,  319,  447,   75,  396,  398,  400,  447,  447,  268,
 /*  1590 */   270,  269,  171,  447,  152,  447,  447,  460,  116,  117,
 /*  1600 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*  1610 */   128,  129,  130,  131,  132,  134,  133,  447,  156,  511,
 /*  1620 */   484,  447,   68,   46,   50,  447,  341,  396,  398,  400,
 /*  1630 */   353,  447,  447,  159,  268,  270,  269,  447,  460,  116,
 /*  1640 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1650 */   127,  128,  129,  130,  131,  132,  134,  133,  447,   70,
 /*  1660 */    71,   73,   72,   74,  447,  447,  447,  341,  396,  398,
 /*  1670 */   400,  349,  447,  447,  515,  268,  270,  269,  447,  460,
 /*  1680 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1690 */   126,  127,  128,  129,  130,  131,  132,  134,   76,   77,
 /*  1700 */   133,  607,  533,  447,  527,  447,  474,  447,   75,  396,
 /*  1710 */   398,  400,  447,  447,  268,  270,  269,  147,  447,  154,
 /*  1720 */   447,  447,  460,  116,  117,  118,  119,  120,  121,  122,
 /*  1730 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1740 */   134,  133,  565,  156,  568,  484,  447,   99,  447,  447,
 /*  1750 */   447,  341,  396,  398,  400,  345,  447,  447,  509,  268,
 /*  1760 */   270,  269,  447,  460,  116,  117,  118,  119,  120,  121,
 /*  1770 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1780 */   132,  134,  133,  447,   70,   71,   73,   72,   74,  447,
 /*  1790 */   447,  447,  341,  396,  398,  400,  369,  447,  447,  143,
 /*  1800 */   268,  270,  269,  447,  460,  116,  117,  118,  119,  120,
 /*  1810 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1820 */   131,  132,  134,   76,   77,  133,  614,  501,  447,  198,
 /*  1830 */   447,  447,  200,   75,  396,  398,  400,  268,  270,  269,
 /*  1840 */   268,  270,  269,   31,  260,  447,   31,  115,  116,  117,
 /*  1850 */   118,  119,  120,  121,  122,  123,  124,  125,  126,  127,
 /*  1860 */   128,  129,  130,  131,  132,  134,  183,  186,  189,   29,
 /*  1870 */   310,   93,   86,  558,  225,  233,   99,  396,  398,  400,
 /*  1880 */   447,  447,  447,  268,  270,  269,  179,  234,  238,  254,
 /*  1890 */   176,  272,  447,  229,  183,  186,  189,  447,   31,  447,
 /*  1900 */   260,  261,  225,  341,  447,  447,  240,  340,  219,  260,
 /*  1910 */   447,  268,  270,  269,  179,  234,  238,  254,  176,  447,
 /*  1920 */   447,  229,   54,  341,  512,  447,  274,  377,  447,  261,
 /*  1930 */   447,  268,  270,  269,  240,  266,  219,  416,  447,  506,
 /*  1940 */   323,   33,  242,  248,  379,  268,  270,  269,  268,  270,
 /*  1950 */   269,  447,  241,  243,  244,  245,  246,  247,   57,   58,
 /*  1960 */    59,   60,   61,   62,   63,   64,   65,   66,   67,  341,
 /*  1970 */   242,  248,  347,  373,  447,  447,  447,  268,  270,  269,
 /*  1980 */   241,  243,  244,  245,  246,  247,  447,  447,  183,  186,
 /*  1990 */   189,  447,    8,  447,  447,   11,  225,   90,  447,  561,
 /*  2000 */   571,  447,  447,  459,  447,  447,  447,  447,  179,  234,
 /*  2010 */   238,  254,  176,  283,  447,  229,  183,  186,  189,  580,
 /*  2020 */   447,  447,  447,  261,  225,  447,  447,  447,  240,  447,
 /*  2030 */   219,  447,  447,  447,  447,  447,  179,  234,  238,  254,
 /*  2040 */   176,  447,  447,  229,  405,  447,  183,  186,  189,  447,
 /*  2050 */   447,  261,    8,  447,  225,   11,  240,   90,  219,  561,
 /*  2060 */   571,  280,  447,  459,  242,  248,  179,  234,  238,  254,
 /*  2070 */   176,  447,  447,  229,  241,  243,  244,  245,  246,  247,
 /*  2080 */   837,  261,  447,  447,  447,  333,  240,  447,  219,  447,
 /*  2090 */   447,  249,  242,  248,  230,  447,  277,  447,  251,  268,
 /*  2100 */   270,  269,  241,  243,  244,  245,  246,  247,  290,  291,
 /*  2110 */   294,  447,  447,  447,  473,  447,  447,  334,  335,  336,
 /*  2120 */   337,  338,  242,  248,  346,  447,  447,  355,  356,  447,
 /*  2130 */   447,  447,  241,  243,  244,  245,  246,  247,  447,  447,
 /*  2140 */   183,  186,  189,  447,  202,  447,  447,  218,  225,  447,
 /*  2150 */   447,  447,  268,  270,  269,  268,  270,  269,  447,  447,
 /*  2160 */   179,  234,  238,  254,  176,  267,  447,  229,  183,  186,
 /*  2170 */   189,  447,  447,  447,  447,  261,  225,  447,  447,  447,
 /*  2180 */   240,  447,  219,  447,  447,  447,  447,  447,  179,  234,
 /*  2190 */   238,  254,  176,  447,  447,  229,  447,  447,  183,  186,
 /*  2200 */   189,  447,  447,  261,  204,  447,  225,  447,  240,  447,
 /*  2210 */   219,  447,  268,  270,  269,  447,  242,  248,  179,  234,
 /*  2220 */   238,  254,  176,  275,  447,  229,  241,  243,  244,  245,
 /*  2230 */   246,  247,  447,  261,  447,  447,  461,  447,  240,  447,
 /*  2240 */   219,  183,  186,  189,  242,  248,  447,  447,  447,  225,
 /*  2250 */   447,  447,  447,  447,  241,  243,  244,  245,  246,  247,
 /*  2260 */   447,  179,  234,  238,  254,  176,  447,  447,  229,  447,
 /*  2270 */   183,  186,  189,  447,  242,  248,  261,  447,  225,  447,
 /*  2280 */   447,  240,  447,  219,  241,  243,  244,  245,  246,  247,
 /*  2290 */   179,  234,  238,  254,  176,  311,  447,  229,  447,  206,
 /*  2300 */   447,  447,  447,  447,  447,  261,  447,  268,  270,  269,
 /*  2310 */   240,  447,  219,  183,  186,  189,  184,  242,  248,  371,
 /*  2320 */   447,  225,  447,  447,  268,  270,  269,  241,  243,  244,
 /*  2330 */   245,  246,  247,  179,  234,  238,  254,  176,  305,  447,
 /*  2340 */   229,  447,  183,  186,  189,  447,  242,  248,  261,  447,
 /*  2350 */   225,  447,  447,  240,  447,  219,  241,  243,  244,  245,
 /*  2360 */   246,  247,  179,  234,  238,  254,  176,  447,  447,  250,
 /*  2370 */   447,  183,  186,  189,  447,  447,  447,  261,  216,  225,
 /*  2380 */   447,  447,  240,  447,  219,  447,  268,  270,  269,  242,
 /*  2390 */   248,  179,  234,  238,  254,  176,  257,  447,  229,  241,
 /*  2400 */   243,  244,  245,  246,  247,  447,  261,  447,  447,  447,
 /*  2410 */   447,  240,  447,  219,  183,  186,  189,  403,  285,  248,
 /*  2420 */   286,  447,  225,  447,  447,  268,  270,  269,  284,  243,
 /*  2430 */   244,  245,  246,  247,  179,  234,  238,  254,  176,  298,
 /*  2440 */   447,  229,  447,  183,  186,  189,  447,  242,  248,  261,
 /*  2450 */   447,  225,  447,  447,  240,  447,  219,  241,  243,  244,
 /*  2460 */   245,  246,  247,  179,  234,  238,  254,  176,  447,  447,
 /*  2470 */   229,  447,  183,  186,  189,  447,   43,  447,  261,  208,
 /*  2480 */   225,  447,  447,  240,  447,  219,  447,  268,  270,  269,
 /*  2490 */   242,  248,  179,  234,  238,  254,  176,  447,  447,  250,
 /*  2500 */   241,  243,  244,  245,  246,  247,  447,  261,  233,  447,
 /*  2510 */   447,  447,  240,  447,  219,  447,  268,  270,  269,  242,
 /*  2520 */   248,  447,  447,  265,  271,   38,  447,  447,  447,  241,
 /*  2530 */   243,  244,  245,  246,  247,   36,   37,   39,   40,   42,
 /*  2540 */    41,  447,  210,  447,  447,  214,  447,  447,  285,  248,
 /*  2550 */   268,  270,  269,  268,  270,  269,  212,  447,  284,  243,
 /*  2560 */   244,  245,  246,  247,  268,  270,  269,  447,  447,  217,
 /*  2570 */   213,  215,  178,  188,  193,  195,  197,  199,  191,  185,
 /*  2580 */   201,  203,  211,  205,  207,  209,  178,  188,  193,  195,
 /*  2590 */   197,  199,  191,  185,  201,  203,  211,  205,  207,  209,
 /*  2600 */   228,  470,  447,  447,  447,  447,  447,  447,  268,  270,
 /*  2610 */   269,  447,  217,  213,  215,  178,  188,  193,  195,  197,
 /*  2620 */   199,  191,  185,  201,  203,  211,  205,  207,  209,  447,
 /*  2630 */   333,  447,  447,  447,  447,  447,  447,  417,  217,  213,
 /*  2640 */   215,  178,  188,  193,  195,  197,  199,  191,  185,  201,
 /*  2650 */   203,  211,  205,  207,  209,  447,  447,  447,  447,  447,
 /*  2660 */   447,  447,  334,  335,  336,  337,  338,  342,  447,  378,
 /*  2670 */   475,  366,  386,  387,  447,  447,  447,  447,  447,  447,
 /*  2680 */   447,  217,  213,  215,  178,  188,  193,  195,  197,  199,
 /*  2690 */   191,  185,  201,  203,  211,  205,  207,  209,  101,  447,
 /*  2700 */   447,  103,  547,  553,  552,  540,  447,  546,  520,  447,
 /*  2710 */   447,  447,   88,  324,  447,  447,  447,  447,  447,  447,
 /*  2720 */   447,  447,  447,  447,  217,  213,  215,  178,  188,  193,
 /*  2730 */   195,  197,  199,  191,  185,  201,  203,  211,  205,  207,
 /*  2740 */   209,  217,  213,  215,  178,  188,  193,  195,  197,  199,
 /*  2750 */   191,  185,  201,  203,  211,  205,  207,  209,  217,  213,
 /*  2760 */   215,  178,  188,  193,  195,  197,  199,  191,  185,  201,
 /*  2770 */   203,  211,  205,  207,  209,   70,   71,   73,   72,   74,
 /*  2780 */   447,  447,  447,  447,  447,  447,  447,  447,  447,  447,
 /*  2790 */   463,  447,  249,  447,  447,  447,  447,  447,  447,  315,
 /*  2800 */   268,  270,  269,  447,  447,  447,  287,  447,  447,  295,
 /*  2810 */   291,  294,  447,  447,   76,   77,  279,  611,  550,  447,
 /*  2820 */   447,  447,  447,  447,   75,  447,  447,  217,  213,  215,
 /*  2830 */   178,  188,  193,  195,  197,  199,  191,  185,  201,  203,
 /*  2840 */   211,  205,  207,  209,  447,  447,  447,  447,  447,  447,
 /*  2850 */   447,  447,  404,  217,  213,  215,  178,  188,  193,  195,
 /*  2860 */   197,  199,  191,  185,  201,  203,  211,  205,  207,  209,
 /*  2870 */   217,  213,  215,  178,  188,  193,  195,  197,  199,  191,
 /*  2880 */   185,  201,  203,  211,  205,  207,  209,  447,  447,  447,
 /*  2890 */   447,  447,  221,  217,  213,  215,  178,  188,  193,  195,
 /*  2900 */   197,  199,  191,  185,  201,  203,  211,  205,  207,  209,
 /*  2910 */   213,  215,  178,  188,  193,  195,  197,  199,  191,  185,
 /*  2920 */   201,  203,  211,  205,  207,  209,   54,  447,  742,  447,
 /*  2930 */   447,  447,  447,  447,  447,  447,  447,   54,  447,  447,
 /*  2940 */   447,  447,  447,   83,  447,   33,  447,   70,   71,   73,
 /*  2950 */    72,   74,  447,  447,  110,  447,   33,  447,  447,  447,
 /*  2960 */   447,  447,   57,   58,   59,   60,   61,   62,   63,   64,
 /*  2970 */    65,   66,   67,   57,   58,   59,   60,   61,   62,   63,
 /*  2980 */    64,   65,   66,   67,   54,  447,   76,   77,  447,  613,
 /*  2990 */   501,  447,  447,  447,  447,   54,   75,  447,  447,  447,
 /*  3000 */   447,   26,  447,   33,  447,   70,   71,   73,   72,   74,
 /*  3010 */   447,  447,  447,  447,   33,  447,  447,  447,  447,  447,
 /*  3020 */    57,   58,   59,   60,   61,   62,   63,   64,   65,   66,
 /*  3030 */    67,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*  3040 */    66,   67,  447,  447,   76,   77,  447,  612,  501,   70,
 /*  3050 */    71,   73,   72,   74,   75,  233,  447,  447,  233,  447,
 /*  3060 */   447,  447,  447,  268,  270,  269,  268,  270,  269,  233,
 /*  3070 */   303,  271,  447,  296,  271,  447,  447,  268,  270,  269,
 /*  3080 */   447,  447,  541,  447,  309,  271,  447,  447,   76,   77,
 /*  3090 */    70,   71,   73,   72,   74,  233,  107,  447,   75,  233,
 /*  3100 */   447,  447,  447,  268,  270,  269,  447,  268,  270,  269,
 /*  3110 */   281,  271,  447,  447,  258,  271,  447,  447,  447,  447,
 /*  3120 */   447,  447,  447,  447,  447,  447,  447,  447,  447,   76,
 /*  3130 */    77,  447,  608,  501,   70,   71,   73,   72,   74,   75,
 /*  3140 */   447,  233,  447,  447,  447,  447,  447,  447,  447,  268,
 /*  3150 */   270,  269,  447,  447,  447,  447,  273,  271,  447,  447,
 /*  3160 */   447,  447,  447,  447,  447,  447,  447,  535,  447,  447,
 /*  3170 */   447,  447,  447,   76,   77,   70,   71,   73,   72,   74,
 /*  3180 */   447,  503,  447,   75,  447,  447,  447,  447,  447,  447,
 /*  3190 */   447,  447,  447,  447,  447,  447,  447,  447,  447,  447,
 /*  3200 */   447,  447,  447,  447,  447,  447,  447,  447,  447,  447,
 /*  3210 */   447,  447,  447,  447,   76,   77,  447,   18,   78,   70,
 /*  3220 */    71,   73,   72,   74,   75,  447,  447,  447,  447,  447,
 /*  3230 */   447,  447,  447,  447,  447,  447,  447,  447,  447,  447,
 /*  3240 */   447,  447,  447,  447,  447,  447,  447,  447,  447,  447,
 /*  3250 */   447,  447,  447,  447,  447,  447,  447,  447,   76,   77,
 /*  3260 */   447,  609,  501,   70,   71,   73,   72,   74,   75,   70,
 /*  3270 */    71,   73,   72,   74,  447,  447,  447,  447,  447,  447,
 /*  3280 */   447,  447,  447,  447,  447,  447,  447,  447,  447,  447,
 /*  3290 */   447,  447,  447,  447,  447,  447,  447,  447,  447,  447,
 /*  3300 */   447,  447,   76,   77,  447,   20,   78,  447,   76,   77,
 /*  3310 */   447,  447,   75,  447,  522,  447,  447,  447,   75,  447,
 /*  3320 */    70,   71,   73,   72,   74,   70,   71,   73,   72,   74,
 /*  3330 */   447,   70,   71,   73,   72,   74,   70,   71,   73,   72,
 /*  3340 */    74,  447,   70,   71,   73,   72,   74,  447,  447,  447,
 /*  3350 */   447,  447,  447,  447,  447,  447,  447,  447,  447,   76,
 /*  3360 */    77,  447,  447,  447,   76,   77,  447,  503,  447,   75,
 /*  3370 */    76,   77,   80,  447,   75,   76,   77,  447,   23,  447,
 /*  3380 */    75,   76,   77,  107,  447,   75,  447,  447,  447,  447,
 /*  3390 */   447,   75,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,  103,  104,  105,  106,  107,   15,  100,
 /*    10 */   101,  102,  103,  104,  105,  106,  107,  126,  127,  128,
 /*    20 */    27,   28,   29,   30,   31,  162,  163,   34,  115,  116,
 /*    30 */   117,  118,   37,  120,  121,   42,  118,   42,  125,  121,
 /*    40 */    47,   65,   49,  125,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   10,   11,   12,   13,   14,
 /*    60 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*    70 */    25,   26,   24,   25,   26,    0,   83,   84,   21,   22,
 /*    80 */    23,   24,   25,   26,   34,   35,   93,   94,   95,   96,
 /*    90 */    97,   98,   42,   43,   44,  123,    6,   34,  126,  127,
 /*   100 */   128,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   110 */    47,   61,   62,   63,   64,  110,   66,  112,    6,   35,
 /*   120 */    70,   71,   72,   73,  151,   35,   76,   34,   35,   45,
 /*   130 */    85,  158,  159,  160,  161,   42,   43,   44,   34,   89,
 /*   140 */    90,   91,   92,    6,   51,   52,   53,   54,   55,   56,
 /*   150 */    57,   58,   59,  118,   61,   62,   63,   64,   35,   66,
 /*   160 */   125,   35,   34,   70,   71,   72,   73,  151,   45,   76,
 /*   170 */    34,   35,   35,  157,  158,  159,  160,  161,   42,   43,
 /*   180 */    44,    6,   89,   90,   91,   92,   74,   51,   52,   53,
 /*   190 */    54,   55,   56,   57,   58,   59,   34,   61,   62,   63,
 /*   200 */    64,  110,   66,  112,  152,  153,   70,   71,   72,   73,
 /*   210 */   151,   83,   76,   34,   35,  156,   34,   42,  159,  160,
 /*   220 */   161,   42,   43,   44,   42,   89,   90,   91,   92,  108,
 /*   230 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   37,
 /*   240 */    61,   62,   63,   64,   42,   66,   82,  118,   84,   70,
 /*   250 */    71,   72,   73,    6,  125,   76,   34,   35,  116,  117,
 /*   260 */   118,  162,  163,  121,   42,   43,   44,  125,   89,   90,
 /*   270 */    91,   92,   34,   51,   52,   53,   54,   55,   56,   57,
 /*   280 */    58,   59,   42,   61,   62,   63,   64,   34,   66,  162,
 /*   290 */   163,   34,   70,   71,   72,   73,    6,  108,   76,   34,
 /*   300 */    35,  116,  110,  118,  112,  120,  121,   42,   43,   44,
 /*   310 */   125,   89,   90,   91,   92,    6,   51,   52,   53,   54,
 /*   320 */    55,   56,   57,   58,   59,   35,   61,   62,   63,   64,
 /*   330 */   125,   66,   85,  162,  163,   70,   71,   72,   73,  151,
 /*   340 */    83,   76,   34,   35,   35,   34,    6,  159,  160,  161,
 /*   350 */    42,   43,   44,   42,   89,   90,   91,   92,  170,   51,
 /*   360 */    52,   53,   54,   55,   56,   57,   58,   59,   49,   61,
 /*   370 */    62,   63,   64,   35,   66,   35,  162,  163,   70,   71,
 /*   380 */    72,   73,  151,   35,   76,   34,   35,  156,   34,    6,
 /*   390 */   159,  160,  161,   42,   43,   44,   42,   89,   90,   91,
 /*   400 */    92,  127,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   410 */    59,   45,   61,   62,   63,   64,  119,   66,   35,  118,
 /*   420 */    86,   70,   71,   72,   73,  151,  125,   76,   34,   35,
 /*   430 */     6,  162,  163,  159,  160,  161,   42,   43,   44,   47,
 /*   440 */    89,   90,   91,   92,   34,   51,   52,   53,   54,   55,
 /*   450 */    56,   57,   58,   59,   45,   61,   62,   63,   64,   35,
 /*   460 */    66,  162,  163,  119,   70,   71,   72,   73,  151,   34,
 /*   470 */    76,   34,   35,  156,   45,    6,  159,  160,  161,   42,
 /*   480 */    43,   44,  119,   89,   90,   91,   92,    6,   51,   52,
 /*   490 */    53,   54,   55,   56,   57,   58,   59,   34,   61,   62,
 /*   500 */    63,   64,   16,   66,   35,  162,  163,   70,   71,   72,
 /*   510 */    73,  151,   34,   76,   34,   35,   35,   34,    6,  159,
 /*   520 */   160,  161,   42,   43,   44,   49,   89,   90,   91,   92,
 /*   530 */     6,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   540 */    35,   61,   62,   63,   64,   35,   66,   35,  162,  163,
 /*   550 */    70,   71,   72,   73,  151,   34,   76,   34,   35,   35,
 /*   560 */   109,    6,  159,  160,  161,   42,   43,   44,   34,   89,
 /*   570 */    90,   91,   92,  109,   51,   52,   53,   54,   55,   56,
 /*   580 */    57,   58,   59,   34,   61,   62,   63,   64,  109,   66,
 /*   590 */    35,  149,  150,   70,   71,   72,   73,  151,   34,   76,
 /*   600 */    34,   35,   82,   43,   84,  159,  160,  161,   42,   43,
 /*   610 */    44,   49,   89,   90,   91,   92,   42,   51,   52,   53,
 /*   620 */    54,   55,   56,   57,   58,   59,   35,   61,   62,   63,
 /*   630 */    64,   35,   66,  162,  163,   42,   70,   71,   72,   73,
 /*   640 */   151,   42,   76,   34,   35,   42,  150,  119,  159,  160,
 /*   650 */   161,   42,   43,   44,   68,   89,   90,   91,   92,   42,
 /*   660 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   42,
 /*   670 */    61,   62,   63,   64,   42,   66,   71,   35,   34,   70,
 /*   680 */    71,   72,   73,  151,   34,   76,   34,   35,   32,  164,
 /*   690 */    16,  159,  160,  161,   42,   43,   44,   49,   89,   90,
 /*   700 */    91,   92,   34,   51,   52,   53,   54,   55,   56,   57,
 /*   710 */    58,   59,    6,   61,   62,   63,   64,  164,   66,   49,
 /*   720 */    34,   49,   70,   71,   72,   73,  151,   34,   76,   34,
 /*   730 */    35,   43,   49,   68,  159,  160,  161,   42,   43,   44,
 /*   740 */    43,   89,   90,   91,   92,   68,   51,   52,   53,   54,
 /*   750 */    55,   56,   57,   58,   59,   68,   61,   62,   63,   64,
 /*   760 */    49,   66,   43,   49,   34,   70,   71,   72,   73,  151,
 /*   770 */    34,   76,   34,   35,   34,  153,   85,  159,  160,  161,
 /*   780 */    42,   43,   44,  154,   89,   90,   91,   92,  154,   51,
 /*   790 */    52,   53,   54,   55,   56,   57,   58,   59,   43,   61,
 /*   800 */    62,   63,   64,  154,   66,  154,   43,  154,   70,   71,
 /*   810 */    72,   73,  151,  154,   76,   34,   35,   34,  154,   85,
 /*   820 */   159,  160,  161,   42,   43,   44,   85,   89,   90,   91,
 /*   830 */    92,   35,   51,   52,   53,   54,   55,   56,   57,   58,
 /*   840 */    59,   35,   61,   62,   63,   64,  154,   66,  154,   35,
 /*   850 */   154,   70,   71,   72,   73,  151,  154,   76,   34,   35,
 /*   860 */    34,   34,   34,  159,  160,  161,   42,   43,   44,   35,
 /*   870 */    89,   90,   91,   92,   35,   51,   52,   53,   54,   55,
 /*   880 */    56,   57,   58,   59,  163,   61,   62,   63,   64,   45,
 /*   890 */    66,  119,   34,   34,   70,   71,   72,   73,   35,   74,
 /*   900 */    76,   34,   35,  123,   68,   65,  126,  127,  128,   42,
 /*   910 */    43,   44,   71,   89,   90,   91,   92,   42,   51,   52,
 /*   920 */    53,   54,   55,   56,   57,   58,   59,   42,   61,   62,
 /*   930 */    63,   64,   34,   66,   42,   49,   42,   70,   71,   72,
 /*   940 */    73,  151,   47,   76,   34,   35,   45,   35,  119,  159,
 /*   950 */   160,  161,   42,   43,   44,   47,   89,   90,   91,   92,
 /*   960 */    45,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   970 */   119,   61,   62,   63,   64,   35,   66,   35,   35,  119,
 /*   980 */    70,   71,   72,   73,  151,  119,   76,   34,   35,  112,
 /*   990 */   109,   34,  159,  160,  161,   42,   43,   44,   38,   89,
 /*  1000 */    90,   91,   92,   34,   51,   52,   53,   54,   55,   56,
 /*  1010 */    57,   58,   59,  109,   61,   62,   63,   64,   42,   66,
 /*  1020 */   109,   38,  109,   70,   71,   72,   73,  151,   34,   76,
 /*  1030 */    34,   35,  109,   34,  109,  159,  160,  161,   42,   43,
 /*  1040 */    44,  171,   89,   90,   91,   92,  171,   51,   52,   53,
 /*  1050 */    54,   55,   56,   57,   58,   59,  171,   61,   62,   63,
 /*  1060 */    64,  171,   66,  171,  171,  171,   70,   71,   72,   73,
 /*  1070 */   151,  171,   76,   34,   35,  171,  171,  171,  159,  160,
 /*  1080 */   161,   42,   43,   44,  171,   89,   90,   91,   92,  171,
 /*  1090 */    51,   52,   53,   54,   55,   56,   57,   58,   59,  171,
 /*  1100 */    61,   62,   63,   64,  171,   66,  171,  171,  171,   70,
 /*  1110 */    71,   72,   73,  151,  171,   76,   34,   35,  171,  171,
 /*  1120 */   171,  159,  160,  161,   42,  171,   44,  171,   89,   90,
 /*  1130 */    91,   92,   45,   51,   52,   53,   54,   55,   56,   57,
 /*  1140 */    58,   59,  171,   61,   62,   63,   64,  171,   66,  171,
 /*  1150 */   171,  113,   70,   71,   72,   73,  118,  171,   76,  107,
 /*  1160 */   122,  171,  171,  125,   77,   78,   79,   80,   81,  151,
 /*  1170 */    45,   89,   90,   91,   92,    6,  124,  159,  160,  161,
 /*  1180 */   171,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1190 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  1200 */   107,   32,   77,   78,   79,   80,   81,  171,  171,   84,
 /*  1210 */   151,  159,  160,  161,  155,  171,  171,  124,  159,  160,
 /*  1220 */   161,  171,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1230 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1240 */   147,  107,   39,  171,  114,   42,  116,  117,  118,  171,
 /*  1250 */   120,  121,  159,  160,  161,  125,  123,    6,  124,  126,
 /*  1260 */   127,  128,  171,  129,  130,  131,  132,  133,  134,  135,
 /*  1270 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1280 */   146,  147,  107,   32,    1,    2,    3,    4,    5,  148,
 /*  1290 */   171,  171,  151,  159,  160,  161,  171,  171,  171,  124,
 /*  1300 */   159,  160,  161,  171,  129,  130,  131,  132,  133,  134,
 /*  1310 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1320 */   145,  146,  147,   40,   41,  107,   43,   44,  171,   46,
 /*  1330 */   151,  171,  171,   50,  159,  160,  161,  118,  159,  160,
 /*  1340 */   161,  122,  124,  171,  125,    6,  171,  129,  130,  131,
 /*  1350 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1360 */   142,  143,  144,  145,  146,  147,  107,  171,  171,  148,
 /*  1370 */   171,   32,  151,  148,  171,  171,  151,  159,  160,  161,
 /*  1380 */   159,  160,  161,  124,  159,  160,  161,  171,  129,  130,
 /*  1390 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1400 */   141,  142,  143,  144,  145,  146,  147,  107,  171,    1,
 /*  1410 */     2,    3,    4,    5,  171,  171,  171,  151,  159,  160,
 /*  1420 */   161,  155,  171,  171,  124,  159,  160,  161,  171,  129,
 /*  1430 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1440 */   140,  141,  142,  143,  144,  145,  146,  147,   40,   41,
 /*  1450 */   107,   43,   44,  148,   46,  171,  151,  171,   50,  159,
 /*  1460 */   160,  161,   49,   49,  159,  160,  161,  124,    6,  171,
 /*  1470 */   171,  171,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1480 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1490 */   147,  107,  171,  171,   32,   82,   82,   84,  171,   86,
 /*  1500 */    86,  151,  159,  160,  161,  155,  171,    6,  124,  159,
 /*  1510 */   160,  161,  171,  129,  130,  131,  132,  133,  134,  135,
 /*  1520 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1530 */   146,  147,  107,   32,    1,    2,    3,    4,    5,  171,
 /*  1540 */   171,  171,  151,  159,  160,  161,  155,  171,  171,  124,
 /*  1550 */   159,  160,  161,  171,  129,  130,  131,  132,  133,  134,
 /*  1560 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1570 */   145,  146,  147,   40,   41,  107,   43,   44,  148,   46,
 /*  1580 */   171,  151,  171,   50,  159,  160,  161,  171,  171,  159,
 /*  1590 */   160,  161,  124,  171,   43,  171,  171,  129,  130,  131,
 /*  1600 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1610 */   142,  143,  144,  145,  146,  147,  107,  171,   67,  123,
 /*  1620 */    69,  171,  126,  127,  128,  171,  151,  159,  160,  161,
 /*  1630 */   155,  171,  171,  124,  159,  160,  161,  171,  129,  130,
 /*  1640 */   131,  132,  133,  134,  135,  136,  137,  138,  139,  140,
 /*  1650 */   141,  142,  143,  144,  145,  146,  147,  107,  171,    1,
 /*  1660 */     2,    3,    4,    5,  171,  171,  171,  151,  159,  160,
 /*  1670 */   161,  155,  171,  171,  124,  159,  160,  161,  171,  129,
 /*  1680 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1690 */   140,  141,  142,  143,  144,  145,  146,  147,   40,   41,
 /*  1700 */   107,   43,   44,  171,   46,  171,  151,  171,   50,  159,
 /*  1710 */   160,  161,  171,  171,  159,  160,  161,  124,  171,   43,
 /*  1720 */   171,  171,  129,  130,  131,  132,  133,  134,  135,  136,
 /*  1730 */   137,  138,  139,  140,  141,  142,  143,  144,  145,  146,
 /*  1740 */   147,  107,   37,   67,   39,   69,  171,   42,  171,  171,
 /*  1750 */   171,  151,  159,  160,  161,  155,  171,  171,  124,  159,
 /*  1760 */   160,  161,  171,  129,  130,  131,  132,  133,  134,  135,
 /*  1770 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1780 */   146,  147,  107,  171,    1,    2,    3,    4,    5,  171,
 /*  1790 */   171,  171,  151,  159,  160,  161,  155,  171,  171,  124,
 /*  1800 */   159,  160,  161,  171,  129,  130,  131,  132,  133,  134,
 /*  1810 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1820 */   145,  146,  147,   40,   41,  107,   43,   44,  171,  151,
 /*  1830 */   171,  171,  151,   50,  159,  160,  161,  159,  160,  161,
 /*  1840 */   159,  160,  161,    6,    6,  171,    6,  129,  130,  131,
 /*  1850 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  1860 */   142,  143,  144,  145,  146,  147,    7,    8,    9,   32,
 /*  1870 */    32,   37,   32,   39,   15,  151,   42,  159,  160,  161,
 /*  1880 */   171,  171,  171,  159,  160,  161,   27,   28,   29,   30,
 /*  1890 */    31,  167,  171,   34,    7,    8,    9,  171,    6,  171,
 /*  1900 */     6,   42,   15,  151,  171,  171,   47,  155,   49,    6,
 /*  1910 */   171,  159,  160,  161,   27,   28,   29,   30,   31,  171,
 /*  1920 */   171,   34,   15,  151,   32,  171,   32,  155,  171,   42,
 /*  1930 */   171,  159,  160,  161,   47,   32,   49,  151,  171,   32,
 /*  1940 */   151,   34,   83,   84,   85,  159,  160,  161,  159,  160,
 /*  1950 */   161,  171,   93,   94,   95,   96,   97,   98,   51,   52,
 /*  1960 */    53,   54,   55,   56,   57,   58,   59,   60,   61,  151,
 /*  1970 */    83,   84,   85,  155,  171,  171,  171,  159,  160,  161,
 /*  1980 */    93,   94,   95,   96,   97,   98,  171,  171,    7,    8,
 /*  1990 */     9,  171,   33,  171,  171,   36,   15,   38,  171,   40,
 /*  2000 */    41,  171,  171,   44,  171,  171,  171,  171,   27,   28,
 /*  2010 */    29,   30,   31,   32,  171,   34,    7,    8,    9,    0,
 /*  2020 */   171,  171,  171,   42,   15,  171,  171,  171,   47,  171,
 /*  2030 */    49,  171,  171,  171,  171,  171,   27,   28,   29,   30,
 /*  2040 */    31,  171,  171,   34,   35,  171,    7,    8,    9,  171,
 /*  2050 */   171,   42,   33,  171,   15,   36,   47,   38,   49,   40,
 /*  2060 */    41,   49,  171,   44,   83,   84,   27,   28,   29,   30,
 /*  2070 */    31,  171,  171,   34,   93,   94,   95,   96,   97,   98,
 /*  2080 */    68,   42,  171,  171,  171,   45,   47,  171,   49,  171,
 /*  2090 */   171,  151,   83,   84,   82,  171,   84,  171,   86,  159,
 /*  2100 */   160,  161,   93,   94,   95,   96,   97,   98,  168,  169,
 /*  2110 */   170,  171,  171,  171,   75,  171,  171,   77,   78,   79,
 /*  2120 */    80,   81,   83,   84,   84,  171,  171,   87,   88,  171,
 /*  2130 */   171,  171,   93,   94,   95,   96,   97,   98,  171,  171,
 /*  2140 */     7,    8,    9,  171,  151,  171,  171,  151,   15,  171,
 /*  2150 */   171,  171,  159,  160,  161,  159,  160,  161,  171,  171,
 /*  2160 */    27,   28,   29,   30,   31,   32,  171,   34,    7,    8,
 /*  2170 */     9,  171,  171,  171,  171,   42,   15,  171,  171,  171,
 /*  2180 */    47,  171,   49,  171,  171,  171,  171,  171,   27,   28,
 /*  2190 */    29,   30,   31,  171,  171,   34,  171,  171,    7,    8,
 /*  2200 */     9,  171,  171,   42,  151,  171,   15,  171,   47,  171,
 /*  2210 */    49,  171,  159,  160,  161,  171,   83,   84,   27,   28,
 /*  2220 */    29,   30,   31,   32,  171,   34,   93,   94,   95,   96,
 /*  2230 */    97,   98,  171,   42,  171,  171,   75,  171,   47,  171,
 /*  2240 */    49,    7,    8,    9,   83,   84,  171,  171,  171,   15,
 /*  2250 */   171,  171,  171,  171,   93,   94,   95,   96,   97,   98,
 /*  2260 */   171,   27,   28,   29,   30,   31,  171,  171,   34,  171,
 /*  2270 */     7,    8,    9,  171,   83,   84,   42,  171,   15,  171,
 /*  2280 */   171,   47,  171,   49,   93,   94,   95,   96,   97,   98,
 /*  2290 */    27,   28,   29,   30,   31,   32,  171,   34,  171,  151,
 /*  2300 */   171,  171,  171,  171,  171,   42,  171,  159,  160,  161,
 /*  2310 */    47,  171,   49,    7,    8,    9,  151,   83,   84,   85,
 /*  2320 */   171,   15,  171,  171,  159,  160,  161,   93,   94,   95,
 /*  2330 */    96,   97,   98,   27,   28,   29,   30,   31,   32,  171,
 /*  2340 */    34,  171,    7,    8,    9,  171,   83,   84,   42,  171,
 /*  2350 */    15,  171,  171,   47,  171,   49,   93,   94,   95,   96,
 /*  2360 */    97,   98,   27,   28,   29,   30,   31,  171,  171,   34,
 /*  2370 */   171,    7,    8,    9,  171,  171,  171,   42,  151,   15,
 /*  2380 */   171,  171,   47,  171,   49,  171,  159,  160,  161,   83,
 /*  2390 */    84,   27,   28,   29,   30,   31,   32,  171,   34,   93,
 /*  2400 */    94,   95,   96,   97,   98,  171,   42,  171,  171,  171,
 /*  2410 */   171,   47,  171,   49,    7,    8,    9,  151,   83,   84,
 /*  2420 */    85,  171,   15,  171,  171,  159,  160,  161,   93,   94,
 /*  2430 */    95,   96,   97,   98,   27,   28,   29,   30,   31,   32,
 /*  2440 */   171,   34,  171,    7,    8,    9,  171,   83,   84,   42,
 /*  2450 */   171,   15,  171,  171,   47,  171,   49,   93,   94,   95,
 /*  2460 */    96,   97,   98,   27,   28,   29,   30,   31,  171,  171,
 /*  2470 */    34,  171,    7,    8,    9,  171,   34,  171,   42,  151,
 /*  2480 */    15,  171,  171,   47,  171,   49,  171,  159,  160,  161,
 /*  2490 */    83,   84,   27,   28,   29,   30,   31,  171,  171,   34,
 /*  2500 */    93,   94,   95,   96,   97,   98,  171,   42,  151,  171,
 /*  2510 */   171,  171,   47,  171,   49,  171,  159,  160,  161,   83,
 /*  2520 */    84,  171,  171,  166,  167,   83,  171,  171,  171,   93,
 /*  2530 */    94,   95,   96,   97,   98,   93,   94,   95,   96,   97,
 /*  2540 */    98,  171,  151,  171,  171,  151,  171,  171,   83,   84,
 /*  2550 */   159,  160,  161,  159,  160,  161,  151,  171,   93,   94,
 /*  2560 */    95,   96,   97,   98,  159,  160,  161,  171,  171,   10,
 /*  2570 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2580 */    21,   22,   23,   24,   25,   26,   13,   14,   15,   16,
 /*  2590 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  2600 */   151,   42,  171,  171,  171,  171,  171,  171,  159,  160,
 /*  2610 */   161,  171,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2620 */    18,   19,   20,   21,   22,   23,   24,   25,   26,  171,
 /*  2630 */    45,  171,  171,  171,  171,  171,  171,   35,   10,   11,
 /*  2640 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2650 */    22,   23,   24,   25,   26,  171,  171,  171,  171,  171,
 /*  2660 */   171,  171,   77,   78,   79,   80,   81,   82,  171,   84,
 /*  2670 */    42,   86,   87,   88,  171,  171,  171,  171,  171,  171,
 /*  2680 */   171,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2690 */    19,   20,   21,   22,   23,   24,   25,   26,  111,  171,
 /*  2700 */   171,  114,  115,  116,  117,  118,  171,  120,  121,  171,
 /*  2710 */   171,  171,  125,   42,  171,  171,  171,  171,  171,  171,
 /*  2720 */   171,  171,  171,  171,   10,   11,   12,   13,   14,   15,
 /*  2730 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2740 */    26,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2750 */    19,   20,   21,   22,   23,   24,   25,   26,   10,   11,
 /*  2760 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2770 */    22,   23,   24,   25,   26,    1,    2,    3,    4,    5,
 /*  2780 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  2790 */    42,  171,  151,  171,  171,  171,  171,  171,  171,   85,
 /*  2800 */   159,  160,  161,  171,  171,  171,  165,  171,  171,  168,
 /*  2810 */   169,  170,  171,  171,   40,   41,   85,   43,   44,  171,
 /*  2820 */   171,  171,  171,  171,   50,  171,  171,   10,   11,   12,
 /*  2830 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2840 */    23,   24,   25,   26,  171,  171,  171,  171,  171,  171,
 /*  2850 */   171,  171,   35,   10,   11,   12,   13,   14,   15,   16,
 /*  2860 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  2870 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2880 */    20,   21,   22,   23,   24,   25,   26,  171,  171,  171,
 /*  2890 */   171,  171,   32,   10,   11,   12,   13,   14,   15,   16,
 /*  2900 */    17,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  2910 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2920 */    21,   22,   23,   24,   25,   26,   15,  171,   85,  171,
 /*  2930 */   171,  171,  171,  171,  171,  171,  171,   15,  171,  171,
 /*  2940 */   171,  171,  171,   32,  171,   34,  171,    1,    2,    3,
 /*  2950 */     4,    5,  171,  171,   32,  171,   34,  171,  171,  171,
 /*  2960 */   171,  171,   51,   52,   53,   54,   55,   56,   57,   58,
 /*  2970 */    59,   60,   61,   51,   52,   53,   54,   55,   56,   57,
 /*  2980 */    58,   59,   60,   61,   15,  171,   40,   41,  171,   43,
 /*  2990 */    44,  171,  171,  171,  171,   15,   50,  171,  171,  171,
 /*  3000 */   171,   32,  171,   34,  171,    1,    2,    3,    4,    5,
 /*  3010 */   171,  171,  171,  171,   34,  171,  171,  171,  171,  171,
 /*  3020 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   60,
 /*  3030 */    61,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  3040 */    60,   61,  171,  171,   40,   41,  171,   43,   44,    1,
 /*  3050 */     2,    3,    4,    5,   50,  151,  171,  171,  151,  171,
 /*  3060 */   171,  171,  171,  159,  160,  161,  159,  160,  161,  151,
 /*  3070 */   166,  167,  171,  166,  167,  171,  171,  159,  160,  161,
 /*  3080 */   171,  171,   34,  171,  166,  167,  171,  171,   40,   41,
 /*  3090 */     1,    2,    3,    4,    5,  151,   48,  171,   50,  151,
 /*  3100 */   171,  171,  171,  159,  160,  161,  171,  159,  160,  161,
 /*  3110 */   166,  167,  171,  171,  166,  167,  171,  171,  171,  171,
 /*  3120 */   171,  171,  171,  171,  171,  171,  171,  171,  171,   40,
 /*  3130 */    41,  171,   43,   44,    1,    2,    3,    4,    5,   50,
 /*  3140 */   171,  151,  171,  171,  171,  171,  171,  171,  171,  159,
 /*  3150 */   160,  161,  171,  171,  171,  171,  166,  167,  171,  171,
 /*  3160 */   171,  171,  171,  171,  171,  171,  171,   34,  171,  171,
 /*  3170 */   171,  171,  171,   40,   41,    1,    2,    3,    4,    5,
 /*  3180 */   171,   48,  171,   50,  171,  171,  171,  171,  171,  171,
 /*  3190 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3200 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3210 */   171,  171,  171,  171,   40,   41,  171,   43,   44,    1,
 /*  3220 */     2,    3,    4,    5,   50,  171,  171,  171,  171,  171,
 /*  3230 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3240 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3250 */   171,  171,  171,  171,  171,  171,  171,  171,   40,   41,
 /*  3260 */   171,   43,   44,    1,    2,    3,    4,    5,   50,    1,
 /*  3270 */     2,    3,    4,    5,  171,  171,  171,  171,  171,  171,
 /*  3280 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3290 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3300 */   171,  171,   40,   41,  171,   43,   44,  171,   40,   41,
 /*  3310 */   171,  171,   50,  171,   46,  171,  171,  171,   50,  171,
 /*  3320 */     1,    2,    3,    4,    5,    1,    2,    3,    4,    5,
 /*  3330 */   171,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  3340 */     5,  171,    1,    2,    3,    4,    5,  171,  171,  171,
 /*  3350 */   171,  171,  171,  171,  171,  171,  171,  171,  171,   40,
 /*  3360 */    41,  171,  171,  171,   40,   41,  171,   48,  171,   50,
 /*  3370 */    40,   41,   48,  171,   50,   40,   41,  171,   48,  171,
 /*  3380 */    50,   40,   41,   48,  171,   50,  171,  171,  171,  171,
 /*  3390 */   171,   50,
};
#define YY_SHIFT_USE_DFLT (-25)
static short yy_shift_ofst[] = {
 /*     0 */  1959,   75, 2019,  -25,  -25,  -25,  -25,  -25,  104,  126,
 /*    10 */   -25,  162,  202,  -25,  238,  240,  -25, 3174,  -25, 3262,
 /*    20 */   -25,  -25, 3330,  253,  319, 2969,  338,  -25, 1837,  348,
 /*    30 */   -25, 2980,  -25,  366, 2442,  -25,  -25,  -25,  -25,  -25,
 /*    40 */   -25,  -25,  -25,  334,  392,  -25,  410,  409, 2442,  -25,
 /*    50 */   435,  429, 2442,  -25,  463,  486,  478,  -25,  -25,  -25,
 /*    60 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*    70 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25, 3341, 3324,
 /*    80 */   483,  476, 2911,  505,  -25, 1840,  510,  -25,  -25,  -25,
 /*    90 */   521, 1834,  -25,  534, 1203,  -25,  549,  175,  -25, 1533,
 /*   100 */   -25,  560,  -25, 1408, 3218,  -25, 3335,  564,  562, 2922,
 /*   110 */   574,  523,  -25,  953,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   120 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25,
 /*   130 */   -25,  -25,  -25,  -25,  -25,  -25,  591,  -25,  596,  -25,
 /*   140 */  2436,  593,  609,   50,  -24,  599,  738,  781,  -25, 2436,
 /*   150 */   603, 1551,  -25, 1676,  -25,  -25, 2442,  586, 1082, 1082,
 /*   160 */   617,  136,  222,  -25, 2436,  627,  308,  695,  -25,  632,
 /*   170 */   265,  996,  605, 2436,  642,  -25, 2436,  -25, 2436,  644,
 /*   180 */   -25,  164, 2436, 2436, 2883, 2436, 2436, 2883, 2436, 2436,
 /*   190 */  2883, 2436,   57, 2436,   57, 2436,   57, 2436,   57, 2436,
 /*   200 */    57, 2436,   48, 2436,   48, 2436,  -25, 2436,  -25, 2436,
 /*   210 */   -25, 2436,   48, 2436, 2573, 2436, 2573, 2436, 2899,   -7,
 /*   220 */  2860,  -25,  656, 2436,  -25,  650,  674, 2436,   57, 1413,
 /*   230 */   182,  648, 2407, 2883,  668,  706,  644,  -25, 2436,  -25,
 /*   240 */   -25,  -25,  -25,  -25,  -25,  -25,  -25,  -25, 2335, 2883,
 /*   250 */  2012,   63,  670, 2191,  686,  672, 2364,  -25, 1169,  -25,
 /*   260 */  2436,  693,  688,  683, 2133, 1903,  -25,  -25,  -25,  -25,
 /*   270 */   -25,  -25,  -25, 1894,  -25,  -25,  -25, 2436, 2731,  -25,
 /*   280 */  1981, 1339,  -25,  -25,  665,  677,  -25,  247,  -25, 2465,
 /*   290 */   -25,  687, 2436,  -25,  -25,  -25, 1462,  -25,  -25,  128,
 /*   300 */   697,  711, 2306, 1501,  -25,  -25,  719,  714, 2263, 1838,
 /*   310 */   -25,  -25,   57,   57, 2714,  -25,  730,  -25,   57, 2883,
 /*   320 */   736,  112, 2161, 2671, 1082,  179,  -25,  740,  290,  -25,
 /*   330 */   740,  -25, 2585,  -25,  -25,  -25,  -25,  -25,  -25, 2436,
 /*   340 */   -25, 2883,  311, 2040, 2436,  -25, 1887, 1087, 2436,  -25,
 /*   350 */   691, 1087, 2436,  -25, 2843,  -25,  -25,  257,  755, 1087,
 /*   360 */  2436,  -25,  763, 1087, 2436,  -25,  783, 1125, 2436,  -25,
 /*   370 */  2234, 1087, 2436,  -25,  734, 1087, 2436,  -25, 1859, 1087,
 /*   380 */  2436,  -25,  741, 1087, 2436,  -25,  -25,  -25,  -25, 2436,
 /*   390 */   481,  -25, 2436,  -25, 2883,  -25,  796,  -25,  806,  -25,
 /*   400 */   814,  -25, 2009, 2817,  -25,  -25,  826,  520,  827,  834,
 /*   410 */   -25, 2436,   45,  839,  -25, 2436, 2602,  -25,  828,  137,
 /*   420 */   -25,  828,  -25,  844, 2442,  -25,  -25,  828,  309,  -25,
 /*   430 */   828,  524,  -25,  828,  555,  -25,  828,   90,  -25,  828,
 /*   440 */   512,  -25,  828,  469,  -25,  828,  424,  -25,  828,  383,
 /*   450 */   -25,  828,  340,  -25, 1414,  858,  670,  354,  648,  -25,
 /*   460 */   -25, 2436, 2748, 1082,  824,  -25,  859,  825, 2039, 2559,
 /*   470 */  1082,  652,  -25, 2436, 2628, 1082, 1039,  -25,  841, 2436,
 /*   480 */   863,  -25,  -25,  -25,  836, 1082, 1082,  -25,  -25,  840,
 /*   490 */   875,  437,   93,  -25,  -25, 1251,  885,  394,  -25,  351,
 /*   500 */   -25, 3341, 3319,  898,  886, 1907,  892,  480,  -25,  566,
 /*   510 */   -25, 1892,  894,  867,  -25,  910,  -25, 1283, 2946,  -25,
 /*   520 */   -25, 3268,  895,  901, 2442,  912,  -25,  908,  915, 2442,
 /*   530 */   940,  -25,  -25, 3268, 3133,  123,  -25, 2442,  942,  -25,
 /*   540 */  3048,   84,  -25, 2442,  943,  -25,  -25, 1658, 2774, 1783,
 /*   550 */  3341, 3004,  -25, 3089,  549,  -25,  -25,  -25,  549,  175,
 /*   560 */   -25,  960,  957, 1705,  -25,  969,  976,  -25,  549,  175,
 /*   570 */   -25,  983,  994,   -5,  -25,  999,  976,  -25,  -25,
};
#define YY_REDUCE_USE_DFLT (-138)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -138, -100, -138, -138, -138, -138, -138, -138, -138,
 /*    10 */  -138, -138,  121, -138, -138,  189, -138, 1038, -138, 1219,
 /*    20 */  -138, -138,  205, -138, -138, 1133, -138, -138, -138, -138,
 /*    30 */  -138, -109, -138, -138,  297, -138, -138, -138, -138, -138,
 /*    40 */  -138, -138, -138, -138, -138, -138, -138, -138,  344, -138,
 /*    50 */  -138, -138,  363, -138, -138, -138, -138, -138, -138, -138,
 /*    60 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*    70 */  -138, -138, -138, -138, -138, -138, -138, -138,  129,  205,
 /*    80 */  -138, -138,  -28, -138, -138, -138, -138, -138, -138, -138,
 /*    90 */  -138,  451, -138, -138,  464, -138,  192,  479, -138, 2587,
 /*   100 */  -138, -138, -138,  -87,  -82, -138,  205, -138, -138,  780,
 /*   110 */  -138, 1384, -138, 1718, -138, -138, -138, -138, -138, -138,
 /*   120 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*   130 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*   140 */  1305, -138, 1675, 1718, -138, -138, 1593, 1718, -138, 1430,
 /*   150 */  -138,  442, -138,  496, -138, -138,  528, -138, 1509, 1718,
 /*   160 */  -138, 1425, 1718, -138, 1221, -138, 1175, 1718, -138, -138,
 /*   170 */  1468, 1718, -138, 1225, -138, -138,  575, -138,  360,  525,
 /*   180 */  -138, -138,  704, 2165, -138,  446,  489, -138,  532,  618,
 /*   190 */  -138,  790, -138,  876, -138,  919, -138, 1678, -138, 1681,
 /*   200 */  -138, 1993, -138, 2053, -138, 2148, -138, 2328, -138, 2391,
 /*   210 */  -138, 2405, -138, 2394, -138, 2227, -138, 1996, -138,  274,
 /*   220 */  -138, -138, -138,  661, -138, -138, -138, 2449, -138, -138,
 /*   230 */  -138, -138, 2907, -138, -138, -138,  553, -138,  833, -138,
 /*   240 */  -138, -138, -138, -138, -138, -138, -138, -138, 2641, -138,
 /*   250 */  -138, -138, -138, 2990, -138, -138, 2948, -138, -138, -138,
 /*   260 */  1724, -138, -138, -138, 2357, -138, -138, -138, -138, -138,
 /*   270 */  -138, -138, -138, -138, -138, -138, -138,  403, -138, -138,
 /*   280 */  2944, -138, -138, -138, -138, -138, -138, -138, -138, 1940,
 /*   290 */  -138, -138,  188, -138, -138, -138, -138, -138, -138, -138,
 /*   300 */  -138, -138, 2904, -138, -138, -138, -138, -138, 2918, -138,
 /*   310 */  -138, -138, -138, -138, -138, -138, -138, -138, -138, -138,
 /*   320 */  -138, -138, 1789, -138, 1259, 1718, -138,   52, -138, -138,
 /*   330 */   622, -138,  629, -138, -138, -138, -138, -138, -138, 1752,
 /*   340 */  -138, -138, -138,  634, 1600, -138,  317,  649, 1516, -138,
 /*   350 */  -138,  651, 1475, -138, -138, -138, -138, -138, -138,  653,
 /*   360 */  1391, -138, -138,  659, 1266, -138, -138,  664, 1641, -138,
 /*   370 */    59,  692, 1818, -138, -138,  694, 1772, -138,  231,  696,
 /*   380 */  1350, -138, -138,  702, 1059, -138, -138, -138, -138,   16,
 /*   390 */  -138, -138,  -27, -138, -138, -138, -138, -138, -138, -138,
 /*   400 */  -138, -138, 2266, -138, -138, -138, -138, -138, -138, -138,
 /*   410 */  -138, 1179, -138, -138, -138, 1786, -138, -138, -137, -138,
 /*   420 */  -138,  721, -138, -138,  772, -138, -138,  127, -138, -138,
 /*   430 */   343, -138, -138,  471, -138, -138,   99, -138, -138,  386,
 /*   440 */  -138, -138,  299, -138, -138,  269, -138, -138,  214, -138,
 /*   450 */  -138,  171, -138, -138, -138, -138, -138, -138, -138, -138,
 /*   460 */  -138,  962, -138, 1052, 1718, -138, -138, -138, 1018, -138,
 /*   470 */  1343, 1718, -138, 1555, -138, 1300, 1718, -138, -138, 1141,
 /*   480 */  -138, -138, -138, -138, -138, 1218, 1718, -138, -138, -138,
 /*   490 */  -138, 1134, 1718, -138, -138, -138, -138, 1093, -138, 1718,
 /*   500 */  -138,   35,  205, -138, -138, 1496, -138, 1634, -138, 1718,
 /*   510 */  -138, -138, -138, 1550, -138, 1718, -138,  185,  -82, -138,
 /*   520 */  -138,   35, -138, -138,  829, -138, -138, -138, -138,  851,
 /*   530 */  -138, -138, -138,  301,  205, -138, -138,  860, -138, -138,
 /*   540 */   205, -138, -138,  866, -138, -138, -138, 1130,  142,  -82,
 /*   550 */   301,  -82, -138,  -82,  877, -138, -138, -138,    5,  881,
 /*   560 */  -138, -138, -138,  904, -138, -138,  911, -138,   91,  913,
 /*   570 */  -138, -138, -138,  923, -138, -138,  925, -138, -138,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   853,  853,  853,  581,  583,  584,  585,  586,  853,  853,
 /*    10 */   587,  853,  853,  588,  853,  853,  589,  853,  604,  853,
 /*    20 */   605,  625,  853,  853,  853,  853,  853,  637,  853,  853,
 /*    30 */   638,  853,  651,  653,  853,  656,  841,  842,  843,  844,
 /*    40 */   845,  846,  847,  853,  853,  848,  853,  654,  853,  657,
 /*    50 */   853,  655,  853,  658,  853,  853,  853,  660,  661,  662,
 /*    60 */   663,  664,  665,  666,  667,  668,  669,  670,  652,  641,
 /*    70 */   643,  644,  645,  646,  647,  648,  649,  650,  853,  853,
 /*    80 */   853,  853,  853,  853,  639,  853,  853,  640,  642,  626,
 /*    90 */   853,  853,  590,  853,  853,  591,  853,  853,  593,  853,
 /*   100 */   599,  853,  600,  853,  853,  623,  853,  853,  853,  853,
 /*   110 */   853,  853,  629,  853,  631,  671,  673,  674,  675,  676,
 /*   120 */   677,  678,  679,  680,  681,  682,  683,  684,  685,  686,
 /*   130 */   687,  688,  689,  690,  691,  692,  853,  693,  853,  694,
 /*   140 */   853,  853,  853,  853,  697,  853,  853,  853,  698,  853,
 /*   150 */   853,  853,  701,  853,  702,  703,  853,  853,  705,  706,
 /*   160 */   853,  853,  853,  709,  853,  853,  853,  853,  711,  853,
 /*   170 */   853,  853,  853,  853,  853,  713,  853,  770,  853,  853,
 /*   180 */   771,  853,  853,  853,  772,  853,  853,  773,  853,  853,
 /*   190 */   774,  853,  778,  853,  779,  853,  780,  853,  781,  853,
 /*   200 */   782,  853,  790,  853,  791,  853,  792,  853,  793,  853,
 /*   210 */   794,  853,  795,  853,  796,  853,  797,  853,  798,  853,
 /*   220 */   853,  783,  853,  853,  784,  853,  853,  853,  785,  802,
 /*   230 */   853,  786,  853,  832,  853,  853,  853,  799,  853,  800,
 /*   240 */   801,  803,  804,  805,  806,  807,  808,  809,  853,  840,
 /*   250 */   802,  853,  787,  853,  853,  812,  853,  813,  853,  814,
 /*   260 */   853,  853,  853,  853,  853,  853,  817,  818,  827,  828,
 /*   270 */   829,  831,  830,  853,  819,  820,  788,  853,  853,  789,
 /*   280 */   853,  853,  815,  816,  803,  804,  810,  853,  811,  853,
 /*   290 */   833,  853,  853,  835,  836,  834,  853,  821,  822,  853,
 /*   300 */   853,  853,  853,  853,  823,  824,  853,  853,  853,  853,
 /*   310 */   825,  826,  777,  776,  853,  849,  853,  850,  775,  851,
 /*   320 */   853,  853,  853,  853,  853,  853,  714,  853,  853,  718,
 /*   330 */   853,  719,  853,  721,  722,  723,  724,  725,  726,  853,
 /*   340 */   727,  769,  853,  853,  853,  728,  853,  853,  853,  731,
 /*   350 */   853,  853,  853,  732,  853,  738,  739,  853,  853,  853,
 /*   360 */   853,  729,  853,  853,  853,  730,  853,  853,  853,  733,
 /*   370 */   853,  853,  853,  734,  853,  853,  853,  735,  853,  853,
 /*   380 */   853,  736,  853,  853,  853,  737,  740,  741,  720,  853,
 /*   390 */   853,  743,  853,  744,  746,  745,  853,  747,  853,  748,
 /*   400 */   853,  749,  853,  853,  750,  751,  853,  853,  853,  853,
 /*   410 */   752,  853,  853,  853,  753,  853,  853,  754,  853,  853,
 /*   420 */   755,  853,  765,  767,  853,  768,  766,  853,  853,  756,
 /*   430 */   853,  853,  757,  853,  853,  758,  853,  853,  759,  853,
 /*   440 */   853,  760,  853,  853,  761,  853,  853,  762,  853,  853,
 /*   450 */   763,  853,  853,  764,  853,  853,  853,  853,  853,  852,
 /*   460 */   672,  853,  853,  853,  853,  715,  853,  853,  853,  853,
 /*   470 */   853,  853,  716,  853,  853,  853,  853,  717,  853,  853,
 /*   480 */   853,  712,  710,  708,  853,  853,  707,  704,  699,  695,
 /*   490 */   853,  853,  853,  700,  696,  853,  853,  853,  630,  853,
 /*   500 */   632,  853,  853,  853,  853,  853,  853,  853,  633,  853,
 /*   510 */   635,  853,  853,  853,  634,  853,  636,  853,  853,  621,
 /*   520 */   624,  853,  853,  853,  853,  853,  627,  853,  853,  853,
 /*   530 */   853,  628,  615,  853,  853,  853,  617,  853,  853,  619,
 /*   540 */   853,  853,  618,  853,  853,  620,  622,  853,  853,  853,
 /*   550 */   853,  853,  616,  853,  853,  601,  603,  602,  853,  853,
 /*   560 */   592,  853,  853,  853,  594,  853,  853,  595,  853,  853,
 /*   570 */   596,  853,  853,  853,  597,  853,  853,  598,  582,
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
  "PRIVATE",       "SCOPED",        "COMMA",         "REQUIRE",     
  "CLONE",         "EMPTY",         "INSTANCEOF",    "AND",         
  "OR",            "EQUALS",        "IDENTICAL",     "LESS",        
  "GREATER",       "LESSEQUAL",     "GREATEREQUAL",  "NOTIDENTICAL",
  "NOTEQUALS",     "ADD",           "SUB",           "CONCAT",      
  "MUL",           "DIV",           "MOD",           "ISSET",       
  "FETCH",         "TYPEOF",        "NEW",           "NOT",         
  "PARENTHESES_CLOSE",  "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",    
  "INTERFACE",     "EXTENDS",       "CLASS",         "IMPLEMENTS",  
  "ABSTRACT",      "FINAL",         "BRACKET_OPEN",  "BRACKET_CLOSE",
  "COMMENT",       "ASSIGN",        "CONST",         "CONSTANT",    
  "FUNCTION",      "PARENTHESES_OPEN",  "INLINE",        "TYPE_INTEGER",
  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",    "TYPE_CHAR",   
  "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",     "TYPE_STRING", 
  "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",         "CONTINUE",    
  "IF",            "ELSE",          "SWITCH",        "CASE",        
  "COLON",         "DEFAULT",       "LOOP",          "WHILE",       
  "DO",            "FOR",           "IN",            "REVERSE",     
  "LET",           "ADDASSIGN",     "SUBASSIGN",     "MULASSIGN",   
  "DIVASSIGN",     "CONCATASSIGN",  "ARROW",         "STRING",      
  "SBRACKET_OPEN",  "SBRACKET_CLOSE",  "DOUBLECOLON",   "INCR",        
  "DECR",          "ECHO",          "RETURN",        "UNSET",       
  "THROW",         "INTEGER",       "CHAR",          "DOUBLE",      
  "NULL",          "TRUE",          "FALSE",         "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_interface_def",  "xx_comment",  
  "xx_interface_body",  "xx_class_body",  "xx_implements_list",  "xx_class_definition",
  "xx_implements",  "xx_interface_methods_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",  "xx_literal_expr",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_interface_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_visibility",  "xx_parameter",  "xx_parameter_type",
  "xx_parameter_cast",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_fcall_statement",
  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",
  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",
  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",  "xx_empty_statement",
  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",
  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",
  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",
  "xx_fcall_expr",  "xx_scall_expr",  "xx_declare_variable_list",  "xx_declare_variable",
  "xx_isset_expr",  "xx_array_list",  "xx_call_parameters",  "xx_call_parameter",
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
 /*   6 */ "xx_top_statement ::= xx_interface_def",
 /*   7 */ "xx_top_statement ::= xx_comment",
 /*   8 */ "xx_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*   9 */ "xx_interface_def ::= INTERFACE IDENTIFIER xx_interface_body",
 /*  10 */ "xx_interface_def ::= INTERFACE IDENTIFIER EXTENDS IDENTIFIER xx_interface_body",
 /*  11 */ "xx_class_def ::= CLASS IDENTIFIER xx_class_body",
 /*  12 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  13 */ "xx_class_def ::= CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  14 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  15 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  16 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  17 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  18 */ "xx_class_def ::= FINAL CLASS IDENTIFIER xx_class_body",
 /*  19 */ "xx_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  20 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  21 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  22 */ "xx_implements_list ::= xx_implements_list COMMA xx_implements",
 /*  23 */ "xx_implements_list ::= xx_implements",
 /*  24 */ "xx_implements ::= IDENTIFIER",
 /*  25 */ "xx_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  26 */ "xx_interface_body ::= BRACKET_OPEN xx_interface_methods_definition BRACKET_CLOSE",
 /*  27 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  28 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  29 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  30 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  31 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  32 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  33 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  34 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  35 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  36 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  37 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  38 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  39 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  40 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  41 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  42 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  43 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  44 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  45 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  46 */ "xx_interface_methods_definition ::= xx_interface_methods_definition xx_interface_method_definition",
 /*  47 */ "xx_interface_methods_definition ::= xx_interface_method_definition",
 /*  48 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  49 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  50 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  51 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  52 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  53 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  54 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  55 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  56 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  57 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  58 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  59 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  60 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  61 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  62 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  63 */ "xx_visibility_list ::= xx_visibility",
 /*  64 */ "xx_visibility ::= PUBLIC",
 /*  65 */ "xx_visibility ::= PROTECTED",
 /*  66 */ "xx_visibility ::= PRIVATE",
 /*  67 */ "xx_visibility ::= STATIC",
 /*  68 */ "xx_visibility ::= SCOPED",
 /*  69 */ "xx_visibility ::= INLINE",
 /*  70 */ "xx_visibility ::= ABSTRACT",
 /*  71 */ "xx_visibility ::= FINAL",
 /*  72 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /*  73 */ "xx_parameter_list ::= xx_parameter",
 /*  74 */ "xx_parameter ::= IDENTIFIER",
 /*  75 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /*  76 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /*  77 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /*  78 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /*  79 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /*  80 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /*  81 */ "xx_parameter_type ::= TYPE_INTEGER",
 /*  82 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /*  83 */ "xx_parameter_type ::= TYPE_LONG",
 /*  84 */ "xx_parameter_type ::= TYPE_ULONG",
 /*  85 */ "xx_parameter_type ::= TYPE_CHAR",
 /*  86 */ "xx_parameter_type ::= TYPE_UCHAR",
 /*  87 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /*  88 */ "xx_parameter_type ::= TYPE_BOOL",
 /*  89 */ "xx_parameter_type ::= TYPE_STRING",
 /*  90 */ "xx_parameter_type ::= TYPE_ARRAY",
 /*  91 */ "xx_parameter_type ::= TYPE_VAR",
 /*  92 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /*  93 */ "xx_statement_list ::= xx_statement",
 /*  94 */ "xx_statement ::= xx_let_statement",
 /*  95 */ "xx_statement ::= xx_if_statement",
 /*  96 */ "xx_statement ::= xx_loop_statement",
 /*  97 */ "xx_statement ::= xx_echo_statement",
 /*  98 */ "xx_statement ::= xx_return_statement",
 /*  99 */ "xx_statement ::= xx_fcall_statement",
 /* 100 */ "xx_statement ::= xx_mcall_statement",
 /* 101 */ "xx_statement ::= xx_scall_statement",
 /* 102 */ "xx_statement ::= xx_unset_statement",
 /* 103 */ "xx_statement ::= xx_throw_statement",
 /* 104 */ "xx_statement ::= xx_declare_statement",
 /* 105 */ "xx_statement ::= xx_break_statement",
 /* 106 */ "xx_statement ::= xx_continue_statement",
 /* 107 */ "xx_statement ::= xx_while_statement",
 /* 108 */ "xx_statement ::= xx_do_while_statement",
 /* 109 */ "xx_statement ::= xx_switch_statement",
 /* 110 */ "xx_statement ::= xx_for_statement",
 /* 111 */ "xx_statement ::= xx_comment",
 /* 112 */ "xx_statement ::= xx_empty_statement",
 /* 113 */ "xx_empty_statement ::= DOTCOMMA",
 /* 114 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 115 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 116 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 117 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 118 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 119 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 120 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 121 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 122 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 123 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 124 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 125 */ "xx_case_clauses ::= xx_case_clause",
 /* 126 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 127 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 128 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 129 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 130 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 131 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 132 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 133 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 134 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 135 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 136 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 137 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 138 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 139 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 140 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 141 */ "xx_let_assignments ::= xx_let_assignment",
 /* 142 */ "xx_assignment_operator ::= ASSIGN",
 /* 143 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 144 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 145 */ "xx_assignment_operator ::= MULASSIGN",
 /* 146 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 147 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 148 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 149 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 150 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 151 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 152 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 153 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 154 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 155 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 156 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 157 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 158 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 159 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 160 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 161 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 162 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 163 */ "xx_index_expr ::= xx_common_expr",
 /* 164 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 165 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 166 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 167 */ "xx_echo_expression ::= xx_common_expr",
 /* 168 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 169 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 170 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 171 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 172 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 173 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 174 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE DOTCOMMA",
 /* 175 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 176 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 177 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 178 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 179 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 180 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 181 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 182 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 183 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 184 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 185 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 186 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 187 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 188 */ "xx_declare_variable ::= IDENTIFIER",
 /* 189 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 190 */ "xx_assign_expr ::= xx_common_expr",
 /* 191 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 192 */ "xx_common_expr ::= ISSET xx_isset_expr",
 /* 193 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 194 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 195 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 196 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 197 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 198 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 199 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 200 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 201 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 202 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 203 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 204 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 205 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 206 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 207 */ "xx_common_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 208 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 209 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 210 */ "xx_common_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 211 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 212 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 213 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 214 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 215 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 216 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 217 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 218 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 219 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 220 */ "xx_common_expr ::= FETCH IDENTIFIER COMMA xx_isset_expr",
 /* 221 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 222 */ "xx_common_expr ::= CONSTANT",
 /* 223 */ "xx_common_expr ::= IDENTIFIER",
 /* 224 */ "xx_common_expr ::= INTEGER",
 /* 225 */ "xx_common_expr ::= STRING",
 /* 226 */ "xx_common_expr ::= CHAR",
 /* 227 */ "xx_common_expr ::= DOUBLE",
 /* 228 */ "xx_common_expr ::= NULL",
 /* 229 */ "xx_common_expr ::= TRUE",
 /* 230 */ "xx_common_expr ::= FALSE",
 /* 231 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 232 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 233 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 234 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 235 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 236 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 237 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 238 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 239 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 240 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 241 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 242 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 243 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 244 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 245 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 246 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 247 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 248 */ "xx_common_expr ::= xx_mcall_expr",
 /* 249 */ "xx_common_expr ::= xx_scall_expr",
 /* 250 */ "xx_common_expr ::= xx_fcall_expr",
 /* 251 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 252 */ "xx_call_parameters ::= xx_call_parameter",
 /* 253 */ "xx_call_parameter ::= xx_common_expr",
 /* 254 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 255 */ "xx_array_list ::= xx_array_item",
 /* 256 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 257 */ "xx_array_item ::= xx_array_value",
 /* 258 */ "xx_array_key ::= IDENTIFIER",
 /* 259 */ "xx_array_key ::= STRING",
 /* 260 */ "xx_array_key ::= INTEGER",
 /* 261 */ "xx_array_value ::= xx_common_expr",
 /* 262 */ "xx_literal_expr ::= INTEGER",
 /* 263 */ "xx_literal_expr ::= CHAR",
 /* 264 */ "xx_literal_expr ::= STRING",
 /* 265 */ "xx_literal_expr ::= DOUBLE",
 /* 266 */ "xx_literal_expr ::= NULL",
 /* 267 */ "xx_literal_expr ::= FALSE",
 /* 268 */ "xx_literal_expr ::= TRUE",
 /* 269 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 270 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 271 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 272 */ "xx_eval_expr ::= xx_common_expr",
 /* 273 */ "xx_comment ::= COMMENT",
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
    case 94:
    case 95:
    case 96:
    case 97:
    case 98:
// 914 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2395 "parser.c"
      break;
    case 101:
// 927 "parser.lemon"
{ json_object_put((yypminor->yy7)); }
// 2400 "parser.c"
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
  { 100, 1 },
  { 101, 1 },
  { 102, 2 },
  { 102, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 103, 1 },
  { 104, 3 },
  { 106, 3 },
  { 106, 5 },
  { 105, 3 },
  { 105, 5 },
  { 105, 5 },
  { 105, 7 },
  { 105, 4 },
  { 105, 6 },
  { 105, 6 },
  { 105, 4 },
  { 105, 6 },
  { 109, 2 },
  { 109, 3 },
  { 110, 3 },
  { 110, 1 },
  { 112, 1 },
  { 108, 2 },
  { 108, 3 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 2 },
  { 111, 2 },
  { 111, 2 },
  { 111, 2 },
  { 111, 3 },
  { 111, 3 },
  { 114, 2 },
  { 114, 1 },
  { 117, 4 },
  { 117, 3 },
  { 117, 6 },
  { 117, 5 },
  { 115, 2 },
  { 115, 1 },
  { 116, 2 },
  { 116, 1 },
  { 113, 2 },
  { 113, 1 },
  { 120, 6 },
  { 120, 5 },
  { 121, 7 },
  { 121, 8 },
  { 121, 8 },
  { 121, 9 },
  { 121, 8 },
  { 121, 9 },
  { 121, 9 },
  { 121, 10 },
  { 122, 6 },
  { 122, 7 },
  { 122, 7 },
  { 122, 8 },
  { 118, 2 },
  { 118, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 125, 1 },
  { 123, 3 },
  { 123, 1 },
  { 126, 1 },
  { 126, 2 },
  { 126, 2 },
  { 126, 3 },
  { 126, 4 },
  { 126, 4 },
  { 128, 3 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 127, 1 },
  { 124, 2 },
  { 124, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 147, 1 },
  { 141, 2 },
  { 142, 2 },
  { 131, 4 },
  { 131, 7 },
  { 131, 5 },
  { 131, 9 },
  { 131, 8 },
  { 131, 8 },
  { 145, 4 },
  { 145, 5 },
  { 149, 2 },
  { 149, 1 },
  { 150, 3 },
  { 150, 4 },
  { 150, 3 },
  { 132, 3 },
  { 132, 4 },
  { 143, 4 },
  { 143, 5 },
  { 144, 6 },
  { 144, 7 },
  { 146, 7 },
  { 146, 8 },
  { 146, 9 },
  { 146, 10 },
  { 130, 3 },
  { 152, 3 },
  { 152, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 154, 1 },
  { 153, 3 },
  { 153, 5 },
  { 153, 7 },
  { 153, 7 },
  { 153, 7 },
  { 153, 8 },
  { 153, 5 },
  { 153, 7 },
  { 153, 8 },
  { 153, 5 },
  { 153, 6 },
  { 153, 4 },
  { 153, 4 },
  { 153, 2 },
  { 153, 2 },
  { 156, 1 },
  { 133, 3 },
  { 157, 3 },
  { 157, 1 },
  { 158, 1 },
  { 136, 2 },
  { 135, 2 },
  { 137, 2 },
  { 134, 3 },
  { 134, 2 },
  { 138, 5 },
  { 138, 6 },
  { 139, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 140, 3 },
  { 162, 3 },
  { 162, 1 },
  { 163, 1 },
  { 163, 3 },
  { 155, 1 },
  { 151, 2 },
  { 151, 2 },
  { 151, 2 },
  { 151, 2 },
  { 151, 2 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 4 },
  { 151, 4 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 4 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 3 },
  { 151, 4 },
  { 151, 2 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 151, 2 },
  { 151, 3 },
  { 151, 2 },
  { 151, 4 },
  { 151, 5 },
  { 160, 4 },
  { 160, 3 },
  { 160, 6 },
  { 160, 5 },
  { 161, 6 },
  { 161, 5 },
  { 159, 6 },
  { 159, 5 },
  { 159, 8 },
  { 159, 7 },
  { 159, 8 },
  { 159, 7 },
  { 151, 1 },
  { 151, 1 },
  { 151, 1 },
  { 166, 3 },
  { 166, 1 },
  { 167, 1 },
  { 165, 3 },
  { 165, 1 },
  { 168, 3 },
  { 168, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 170, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 3 },
  { 164, 4 },
  { 164, 3 },
  { 148, 1 },
  { 107, 1 },
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
// 923 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy7;
}
// 2891 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 94:
      case 95:
      case 96:
      case 97:
      case 98:
      case 99:
      case 100:
      case 101:
      case 102:
      case 103:
      case 104:
      case 105:
      case 106:
      case 107:
      case 108:
      case 109:
      case 110:
      case 111:
      case 112:
      case 163:
      case 190:
      case 248:
      case 249:
      case 250:
      case 253:
      case 261:
      case 272:
// 929 "parser.lemon"
{
	yygotominor.yy7 = yymsp[0].minor.yy7;
}
// 2929 "parser.c"
        break;
      case 2:
      case 36:
      case 42:
      case 44:
      case 46:
      case 62:
      case 92:
      case 124:
// 933 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(yymsp[-1].minor.yy7, yymsp[0].minor.yy7);
}
// 2943 "parser.c"
        break;
      case 3:
      case 23:
      case 37:
      case 43:
      case 45:
      case 47:
      case 63:
      case 73:
      case 93:
      case 125:
      case 141:
      case 166:
      case 187:
      case 252:
      case 255:
// 937 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(NULL, yymsp[0].minor.yy7);
}
// 2964 "parser.c"
        break;
      case 8:
// 957 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2973 "parser.c"
        break;
      case 9:
// 961 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2981 "parser.c"
        break;
      case 10:
// 965 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2990 "parser.c"
        break;
      case 11:
// 969 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy7, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 2998 "parser.c"
        break;
      case 12:
// 973 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3007 "parser.c"
        break;
      case 13:
// 977 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 0, 0, NULL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3016 "parser.c"
        break;
      case 14:
// 981 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy7, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3026 "parser.c"
        break;
      case 15:
// 985 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy7, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3035 "parser.c"
        break;
      case 16:
// 989 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3045 "parser.c"
        break;
      case 17:
// 993 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 1, 0, NULL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3055 "parser.c"
        break;
      case 18:
// 997 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy7, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3064 "parser.c"
        break;
      case 19:
// 1001 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3074 "parser.c"
        break;
      case 20:
      case 25:
// 1005 "parser.lemon"
{
	yygotominor.yy7 = NULL;
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3084 "parser.c"
        break;
      case 21:
      case 26:
// 1009 "parser.lemon"
{
	yygotominor.yy7 = yymsp[-1].minor.yy7;
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3094 "parser.c"
        break;
      case 22:
      case 72:
      case 140:
      case 165:
      case 186:
      case 251:
      case 254:
// 1013 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3108 "parser.c"
        break;
      case 24:
      case 223:
      case 258:
// 1021 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3117 "parser.c"
        break;
      case 27:
// 1033 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
}
// 3124 "parser.c"
        break;
      case 28:
// 1037 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 3131 "parser.c"
        break;
      case 29:
// 1041 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, yymsp[0].minor.yy7, NULL, status->scanner_state);
}
// 3138 "parser.c"
        break;
      case 30:
// 1045 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
}
// 3145 "parser.c"
        break;
      case 31:
// 1049 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 3152 "parser.c"
        break;
      case 32:
// 1053 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[0].minor.yy7, NULL, yymsp[-1].minor.yy7, status->scanner_state);
}
// 3159 "parser.c"
        break;
      case 33:
// 1057 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, yymsp[0].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
}
// 3166 "parser.c"
        break;
      case 34:
// 1061 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
}
// 3173 "parser.c"
        break;
      case 35:
// 1065 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, yymsp[0].minor.yy7, yymsp[-2].minor.yy7, status->scanner_state);
}
// 3180 "parser.c"
        break;
      case 38:
// 1081 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(yymsp[-2].minor.yy7, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3188 "parser.c"
        break;
      case 39:
// 1085 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(yymsp[-2].minor.yy7, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3196 "parser.c"
        break;
      case 40:
// 1089 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(yymsp[-4].minor.yy7, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3205 "parser.c"
        break;
      case 41:
// 1093 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(yymsp[-4].minor.yy7, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3214 "parser.c"
        break;
      case 48:
// 1121 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3224 "parser.c"
        break;
      case 49:
// 1125 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3234 "parser.c"
        break;
      case 50:
// 1129 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-6].minor.yy7, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3246 "parser.c"
        break;
      case 51:
// 1133 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-7].minor.yy7, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3258 "parser.c"
        break;
      case 52:
// 1137 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-7].minor.yy7, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3270 "parser.c"
        break;
      case 53:
// 1141 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-8].minor.yy7, yymsp[-6].minor.yy0, yymsp[-4].minor.yy7, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3282 "parser.c"
        break;
      case 54:
// 1145 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-6].minor.yy7, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3294 "parser.c"
        break;
      case 55:
// 1149 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-7].minor.yy7, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3306 "parser.c"
        break;
      case 56:
// 1153 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-7].minor.yy7, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3318 "parser.c"
        break;
      case 57:
// 1157 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-8].minor.yy7, yymsp[-6].minor.yy0, yymsp[-4].minor.yy7, yymsp[-1].minor.yy7, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3330 "parser.c"
        break;
      case 58:
// 1161 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-5].minor.yy7, yymsp[-3].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3341 "parser.c"
        break;
      case 59:
// 1165 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-6].minor.yy7, yymsp[-4].minor.yy0, yymsp[-2].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3352 "parser.c"
        break;
      case 60:
// 1169 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-5].minor.yy7, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3363 "parser.c"
        break;
      case 61:
// 1173 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-6].minor.yy7, yymsp[-4].minor.yy0, yymsp[-2].minor.yy7, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3374 "parser.c"
        break;
      case 64:
// 1185 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3382 "parser.c"
        break;
      case 65:
// 1189 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3390 "parser.c"
        break;
      case 66:
// 1193 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3398 "parser.c"
        break;
      case 67:
// 1197 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3406 "parser.c"
        break;
      case 68:
// 1201 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3414 "parser.c"
        break;
      case 69:
// 1205 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("inline");
  yy_destructor(50,&yymsp[0].minor);
}
// 3422 "parser.c"
        break;
      case 70:
// 1209 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("abstract");
  yy_destructor(40,&yymsp[0].minor);
}
// 3430 "parser.c"
        break;
      case 71:
// 1213 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3438 "parser.c"
        break;
      case 74:
// 1225 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3445 "parser.c"
        break;
      case 75:
// 1229 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(yymsp[-1].minor.yy7, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3452 "parser.c"
        break;
      case 76:
// 1233 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, yymsp[-1].minor.yy7, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3459 "parser.c"
        break;
      case 77:
// 1237 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3467 "parser.c"
        break;
      case 78:
// 1241 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(yymsp[-3].minor.yy7, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3475 "parser.c"
        break;
      case 79:
// 1245 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, yymsp[-3].minor.yy7, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3483 "parser.c"
        break;
      case 80:
// 1249 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 3492 "parser.c"
        break;
      case 81:
// 1253 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(51,&yymsp[0].minor);
}
// 3500 "parser.c"
        break;
      case 82:
// 1257 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(52,&yymsp[0].minor);
}
// 3508 "parser.c"
        break;
      case 83:
// 1261 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(53,&yymsp[0].minor);
}
// 3516 "parser.c"
        break;
      case 84:
// 1265 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(54,&yymsp[0].minor);
}
// 3524 "parser.c"
        break;
      case 85:
// 1269 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(55,&yymsp[0].minor);
}
// 3532 "parser.c"
        break;
      case 86:
// 1273 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3540 "parser.c"
        break;
      case 87:
// 1277 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(57,&yymsp[0].minor);
}
// 3548 "parser.c"
        break;
      case 88:
// 1281 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(58,&yymsp[0].minor);
}
// 3556 "parser.c"
        break;
      case 89:
// 1285 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(59,&yymsp[0].minor);
}
// 3564 "parser.c"
        break;
      case 90:
// 1289 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(60,&yymsp[0].minor);
}
// 3572 "parser.c"
        break;
      case 91:
// 1293 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(61,&yymsp[0].minor);
}
// 3580 "parser.c"
        break;
      case 113:
// 1381 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3588 "parser.c"
        break;
      case 114:
// 1385 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3597 "parser.c"
        break;
      case 115:
// 1389 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3606 "parser.c"
        break;
      case 116:
// 1393 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-2].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3616 "parser.c"
        break;
      case 117:
// 1397 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-5].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3629 "parser.c"
        break;
      case 118:
// 1401 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3639 "parser.c"
        break;
      case 119:
// 1405 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-7].minor.yy7, yymsp[-5].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(64,&yymsp[-8].minor);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3652 "parser.c"
        break;
      case 120:
// 1409 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-6].minor.yy7, yymsp[-4].minor.yy7, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3665 "parser.c"
        break;
      case 121:
// 1413 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-6].minor.yy7, NULL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(64,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3678 "parser.c"
        break;
      case 122:
// 1417 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_switch_statement(yymsp[-2].minor.yy7, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3688 "parser.c"
        break;
      case 123:
// 1421 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_switch_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3698 "parser.c"
        break;
      case 126:
// 1433 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_case_clause(yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3707 "parser.c"
        break;
      case 127:
// 1437 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_case_clause(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-1].minor);
}
// 3716 "parser.c"
        break;
      case 128:
// 1441 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_case_clause(NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[-1].minor);
}
// 3725 "parser.c"
        break;
      case 129:
// 1445 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3735 "parser.c"
        break;
      case 130:
// 1449 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_loop_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3745 "parser.c"
        break;
      case 131:
// 1453 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_while_statement(yymsp[-2].minor.yy7, NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3755 "parser.c"
        break;
      case 132:
// 1457 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_while_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3765 "parser.c"
        break;
      case 133:
// 1461 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_do_while_statement(yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3777 "parser.c"
        break;
      case 134:
// 1465 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_do_while_statement(yymsp[-1].minor.yy7, yymsp[-4].minor.yy7, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3789 "parser.c"
        break;
      case 135:
// 1469 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3800 "parser.c"
        break;
      case 136:
// 1473 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3812 "parser.c"
        break;
      case 137:
// 1477 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(73,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3824 "parser.c"
        break;
      case 138:
// 1481 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(73,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3837 "parser.c"
        break;
      case 139:
// 1485 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3846 "parser.c"
        break;
      case 142:
// 1498 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("assign");
  yy_destructor(45,&yymsp[0].minor);
}
// 3854 "parser.c"
        break;
      case 143:
// 1503 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("add-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3862 "parser.c"
        break;
      case 144:
// 1508 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("sub-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3870 "parser.c"
        break;
      case 145:
// 1512 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("mult-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3878 "parser.c"
        break;
      case 146:
// 1516 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("div-assign");
  yy_destructor(80,&yymsp[0].minor);
}
// 3886 "parser.c"
        break;
      case 147:
// 1520 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("concat-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 3894 "parser.c"
        break;
      case 148:
// 1525 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy7, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 3901 "parser.c"
        break;
      case 149:
// 1530 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy7, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
}
// 3909 "parser.c"
        break;
      case 150:
// 1535 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy7, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3919 "parser.c"
        break;
      case 151:
// 1540 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy7, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3929 "parser.c"
        break;
      case 152:
// 1545 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy7, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3939 "parser.c"
        break;
      case 153:
// 1550 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy7, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3949 "parser.c"
        break;
      case 154:
// 1555 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy7, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
}
// 3957 "parser.c"
        break;
      case 155:
// 1560 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy7, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3967 "parser.c"
        break;
      case 156:
// 1565 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy7, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3977 "parser.c"
        break;
      case 157:
// 1570 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy7, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3986 "parser.c"
        break;
      case 158:
// 1575 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3995 "parser.c"
        break;
      case 159:
// 1580 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 4004 "parser.c"
        break;
      case 160:
// 1585 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 4013 "parser.c"
        break;
      case 161:
// 1590 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 4021 "parser.c"
        break;
      case 162:
// 1595 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 4029 "parser.c"
        break;
      case 164:
// 1603 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_echo_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4038 "parser.c"
        break;
      case 167:
// 1615 "parser.lemon"
{
	yygotominor.yy7 = yymsp[0].minor.yy7;;
}
// 4045 "parser.c"
        break;
      case 168:
// 1620 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4053 "parser.c"
        break;
      case 169:
// 1625 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4061 "parser.c"
        break;
      case 170:
// 1630 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_scall_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4069 "parser.c"
        break;
      case 171:
// 1635 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_return_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4078 "parser.c"
        break;
      case 172:
// 1640 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4087 "parser.c"
        break;
      case 173:
// 1645 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state),
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state),
		status->scanner_state
	);
  yy_destructor(91,&yymsp[-4].minor);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4101 "parser.c"
        break;
      case 174:
// 1654 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_unset_statement(
		xx_ret_literal(XX_T_IDENTIFIER, yymsp[-4].minor.yy0, status->scanner_state),
		yymsp[-2].minor.yy7,
		status->scanner_state
	);
  yy_destructor(91,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4116 "parser.c"
        break;
      case 175:
// 1663 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_throw_exception(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4125 "parser.c"
        break;
      case 176:
// 1667 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4134 "parser.c"
        break;
      case 177:
// 1671 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4143 "parser.c"
        break;
      case 178:
// 1675 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4152 "parser.c"
        break;
      case 179:
// 1679 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4161 "parser.c"
        break;
      case 180:
// 1683 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4170 "parser.c"
        break;
      case 181:
// 1687 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4179 "parser.c"
        break;
      case 182:
// 1691 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4188 "parser.c"
        break;
      case 183:
// 1695 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4197 "parser.c"
        break;
      case 184:
// 1699 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4206 "parser.c"
        break;
      case 185:
// 1703 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4215 "parser.c"
        break;
      case 188:
// 1715 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4222 "parser.c"
        break;
      case 189:
// 1719 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 4230 "parser.c"
        break;
      case 191:
// 1727 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4238 "parser.c"
        break;
      case 192:
// 1731 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("isset", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4246 "parser.c"
        break;
      case 193:
// 1735 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("require", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4254 "parser.c"
        break;
      case 194:
// 1739 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("clone", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4262 "parser.c"
        break;
      case 195:
// 1743 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("empty", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4270 "parser.c"
        break;
      case 196:
// 1747 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("equals", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4278 "parser.c"
        break;
      case 197:
// 1751 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not-equals", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4286 "parser.c"
        break;
      case 198:
// 1755 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("identical", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4294 "parser.c"
        break;
      case 199:
// 1759 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not-identical", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4302 "parser.c"
        break;
      case 200:
// 1763 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("less", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4310 "parser.c"
        break;
      case 201:
// 1767 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("greater", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4318 "parser.c"
        break;
      case 202:
// 1771 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("less-equal", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4326 "parser.c"
        break;
      case 203:
// 1775 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4334 "parser.c"
        break;
      case 204:
// 1779 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("list", yymsp[-1].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4343 "parser.c"
        break;
      case 205:
// 1783 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("cast", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
}
// 4352 "parser.c"
        break;
      case 206:
// 1787 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4361 "parser.c"
        break;
      case 207:
      case 271:
// 1791 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-1].minor);
}
// 4370 "parser.c"
        break;
      case 208:
// 1795 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4378 "parser.c"
        break;
      case 209:
      case 269:
// 1799 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4387 "parser.c"
        break;
      case 210:
      case 270:
// 1804 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4397 "parser.c"
        break;
      case 211:
// 1809 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("add", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4405 "parser.c"
        break;
      case 212:
// 1814 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("sub", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4413 "parser.c"
        break;
      case 213:
// 1819 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("mul", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4421 "parser.c"
        break;
      case 214:
// 1824 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("div", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4429 "parser.c"
        break;
      case 215:
// 1829 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("mod", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4437 "parser.c"
        break;
      case 216:
// 1834 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("concat", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4445 "parser.c"
        break;
      case 217:
// 1839 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("and", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4453 "parser.c"
        break;
      case 218:
// 1844 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("or", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4461 "parser.c"
        break;
      case 219:
// 1849 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("instanceof", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4469 "parser.c"
        break;
      case 220:
// 1854 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4478 "parser.c"
        break;
      case 221:
// 1859 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("typeof", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4486 "parser.c"
        break;
      case 222:
// 1864 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4493 "parser.c"
        break;
      case 224:
      case 260:
      case 262:
// 1874 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4502 "parser.c"
        break;
      case 225:
      case 259:
      case 264:
// 1879 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4511 "parser.c"
        break;
      case 226:
      case 263:
// 1884 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4519 "parser.c"
        break;
      case 227:
      case 265:
// 1889 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4527 "parser.c"
        break;
      case 228:
      case 266:
// 1894 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4536 "parser.c"
        break;
      case 229:
      case 268:
// 1899 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 4545 "parser.c"
        break;
      case 230:
      case 267:
// 1904 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 4554 "parser.c"
        break;
      case 231:
// 1909 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4563 "parser.c"
        break;
      case 232:
// 1914 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array", yymsp[-1].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4572 "parser.c"
        break;
      case 233:
// 1919 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4580 "parser.c"
        break;
      case 234:
// 1924 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4590 "parser.c"
        break;
      case 235:
// 1929 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4600 "parser.c"
        break;
      case 236:
// 1934 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4609 "parser.c"
        break;
      case 237:
// 1939 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4618 "parser.c"
        break;
      case 238:
// 1944 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4629 "parser.c"
        break;
      case 239:
// 1949 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4640 "parser.c"
        break;
      case 240:
// 1954 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4650 "parser.c"
        break;
      case 241:
// 1959 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4660 "parser.c"
        break;
      case 242:
// 1964 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4670 "parser.c"
        break;
      case 243:
// 1969 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4680 "parser.c"
        break;
      case 244:
// 1974 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4692 "parser.c"
        break;
      case 245:
// 1979 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4704 "parser.c"
        break;
      case 246:
// 1984 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4716 "parser.c"
        break;
      case 247:
// 1989 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4728 "parser.c"
        break;
      case 256:
// 2028 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_array_item(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(68,&yymsp[-1].minor);
}
// 4736 "parser.c"
        break;
      case 257:
// 2032 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_array_item(NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 4743 "parser.c"
        break;
      case 273:
// 2097 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4750 "parser.c"
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
// 880 "parser.lemon"


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

// 4826 "parser.c"
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
			case XX_T_INTERFACE:
				xx_(xx_parser, XX_INTERFACE, NULL, parser_status);
				break;
			case XX_T_EXTENDS:
				xx_(xx_parser, XX_EXTENDS, NULL, parser_status);
				break;
			case XX_T_IMPLEMENTS:
				xx_(xx_parser, XX_IMPLEMENTS, NULL, parser_status);
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
			case XX_T_REQUIRE:
				xx_(xx_parser, XX_REQUIRE, NULL, parser_status);
				break;
			case XX_T_CLONE:
				xx_(xx_parser, XX_CLONE, NULL, parser_status);
				break;
			case XX_T_EMPTY:
				xx_(xx_parser, XX_EMPTY, NULL, parser_status);
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
