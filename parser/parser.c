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


// 838 "parser.c"
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
#define YYNSTATE 572
#define YYNRULE 272
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
 /*     0 */   183,  186,  189,    3,    4,    5,    6,    7,  225,  845,
 /*    10 */     1,    2,  571,    4,    5,    6,    7,  154,  321,  381,
 /*    20 */   179,  234,  238,  254,  176,  101,  323,  229,  103,  540,
 /*    30 */   546,  545,  533,   85,  539,  513,   68,   46,   50,   88,
 /*    40 */   240,  156,  219,  477,   57,   58,   59,   60,   61,   62,
 /*    50 */    63,   64,   65,   66,   67,  322,  217,  213,  215,  178,
 /*    60 */   188,  193,  195,  197,  199,  191,  185,  201,  203,  211,
 /*    70 */   205,  207,  209,   79,  459,   22,  242,  248,  221,   21,
 /*    80 */    88,  273,   88,  447,  135,  563,  241,  243,  244,  245,
 /*    90 */   246,  247,  475,  452,   93,  414,  551,  490,  326,   99,
 /*   100 */   411,  420,  429,  432,  423,  426,  435,  441,  438,  336,
 /*   110 */   444,  136,  138,  140,  230,  149,  270,  350,  251,  160,
 /*   120 */   164,  169,  313,  385,  440,  320,  447,  135,  414,  351,
 /*   130 */   327,  328,  329,  330,  331,  168,  452,  363,  382,  395,
 /*   140 */   399,  408,  315,  411,  420,  429,  432,  423,  426,  435,
 /*   150 */   441,  438,  384,  444,  136,  138,  140,  413,  149,  205,
 /*   160 */   207,  209,  160,  164,  169,  313,  449,  309,  320,  182,
 /*   170 */   447,  135,  201,  203,  211,  205,  207,  209,  355,  471,
 /*   180 */   452,  382,  395,  399,  408,  326,  416,  411,  420,  429,
 /*   190 */   432,  423,  426,  435,  441,  438,   14,  444,  136,  138,
 /*   200 */   140,   17,  149,   32,   46,   50,  160,  164,  169,  313,
 /*   210 */    98,  558,  320,  561,  447,  135,   99,  327,  328,  329,
 /*   220 */   330,  331,   31,  487,  452,  382,  395,  399,  408,  412,
 /*   230 */   419,  411,  420,  429,  432,  423,  426,  435,  441,  438,
 /*   240 */    31,  444,  136,  138,  140,  401,  149,  404,   86,  231,
 /*   250 */   160,  164,  169,  313,  387,  414,  320,  292,  447,  135,
 /*   260 */   383,  388,  261,  263,  262,  152,   29,  172,  452,  382,
 /*   270 */   395,  399,  408,  421,  419,  411,  420,  429,  432,  423,
 /*   280 */   426,  435,  441,  438,  422,  444,  136,  138,  140,  156,
 /*   290 */   149,  477,  424,  419,  160,  164,  169,  313,  249,  414,
 /*   300 */   320,  547,  447,  135,  427,  419,  261,  263,  262,  153,
 /*   310 */   480,  486,  452,  382,  395,  399,  408,  286,  222,  411,
 /*   320 */   420,  429,  432,  423,  426,  435,  441,  438,  425,  444,
 /*   330 */   136,  138,  140,  414,  149,  430,  419,   99,  160,  164,
 /*   340 */   169,  313,  220,  414,  320,  155,  447,  135,  433,  419,
 /*   350 */   261,  263,  262,  139,  495,  491,  452,  382,  395,  399,
 /*   360 */   408,   88,  428,  411,  420,  429,  432,  423,  426,  435,
 /*   370 */   441,  438,  431,  444,  136,  138,  140,  414,  149,  436,
 /*   380 */   419,  293,  160,  164,  169,  313,  387,  414,  320,   96,
 /*   390 */   447,  135,   99,  386,  261,  263,  262,  451,  282,  465,
 /*   400 */   452,  382,  395,  399,  408,  292,  434,  411,  420,  429,
 /*   410 */   432,  423,  426,  435,  441,  438,  437,  444,  136,  138,
 /*   420 */   140,  414,  149,  439,  419,  549,  160,  164,  169,  313,
 /*   430 */   299,  347,  320,   45,  447,  135,  375,  442,  419,  261,
 /*   440 */   263,  262,  260,  144,  452,  382,  395,  399,  408,  137,
 /*   450 */   443,  411,  420,  429,  432,  423,  426,  435,  441,  438,
 /*   460 */   260,  444,  136,  138,  140,  414,  149,   97,  259,  550,
 /*   470 */   160,  164,  169,  313,   52,  347,  320,  281,  447,  135,
 /*   480 */   367,  445,  419,  261,  263,  262,  267,  493,  452,  382,
 /*   490 */   395,  399,  408,   95,  446,  411,  420,  429,  432,  423,
 /*   500 */   426,  435,  441,  438,   31,  444,  136,  138,  140,  497,
 /*   510 */   149,  260,   31,  252,  160,  164,  169,  313,   69,  347,
 /*   520 */   320,   48,  447,  135,  343,   12,  269,  261,  263,  262,
 /*   530 */   489,  501,  452,  382,  395,  399,  408,  275,  505,  411,
 /*   540 */   420,  429,  432,  423,  426,  435,  441,  438,  527,  444,
 /*   550 */   136,  138,  140,  529,  149,   88,  498,   44,  160,  164,
 /*   560 */   169,  313,  455,  530,  320,  500,  447,  135,   94,  572,
 /*   570 */   261,  263,  262,  260,  260,  319,  452,  382,  395,  399,
 /*   580 */   408,   16,  535,  411,  420,  429,  432,  423,  426,  435,
 /*   590 */   441,  438,  536,  444,  136,  138,  140,   17,  149,  290,
 /*   600 */   297,   92,  160,  164,  169,  313,  224,  552,  320,  550,
 /*   610 */   447,  135,  568,   51,  261,  263,  262,   99,  260,  470,
 /*   620 */   452,  382,  395,  399,  408,   91,  565,  411,  420,  429,
 /*   630 */   432,  423,  426,  435,  441,  438,  506,  444,  136,  138,
 /*   640 */   140,  562,  149,  550,  303,  157,  160,  164,  169,  313,
 /*   650 */   396,   27,  320,  158,  447,  135,  181,  237,  261,  263,
 /*   660 */   262,  830,  236,  458,  452,  382,  395,  399,  408,  235,
 /*   670 */   418,  411,  420,  429,  432,  423,  426,  435,  441,  438,
 /*   680 */   415,  444,  136,  138,  140,   25,  149,  417,   15,  232,
 /*   690 */   160,  164,  169,  313,  316,  829,  320,  325,  447,  135,
 /*   700 */   516,  566,  261,  263,  262,  517,   24,  503,  452,  382,
 /*   710 */   395,  399,  408,  161,  518,  411,  420,  429,  432,  423,
 /*   720 */   426,  435,  441,  438,  519,  444,  136,  138,  140,   87,
 /*   730 */   149,  521,  407,  522,  160,  164,  169,  313,  227,  403,
 /*   740 */   320,  524,  447,  135,   28,  523,  226,   68,   46,   50,
 /*   750 */   285,  507,  452,  382,  395,  399,  408,  402,  400,  411,
 /*   760 */   420,  429,  432,  423,  426,  435,  441,  438,   10,  444,
 /*   770 */   136,  138,  140,   84,  149,  175,  223,  111,  160,  164,
 /*   780 */   169,  313,  202,  394,  320,   13,  447,  135,   49,  531,
 /*   790 */   261,  263,  262,  532,   82,  509,  452,  382,  395,  399,
 /*   800 */   408,  392,  166,  411,  420,  429,  432,  423,  426,  435,
 /*   810 */   441,  438,  390,  444,  136,  138,  140,   81,  149,  567,
 /*   820 */     9,  376,  160,  164,  169,  313,  218,  377,  320,  538,
 /*   830 */   447,  135,  537,  652,  261,  263,  262,  373,  314,  482,
 /*   840 */   452,  382,  395,  399,  408,  108,   35,  411,  420,  429,
 /*   850 */   432,  423,  426,  435,  441,  438,  369,  444,  136,  138,
 /*   860 */   140,   56,  149,  368,  360,  365,  160,  164,  169,  313,
 /*   870 */   216,  361,  320,  569,  447,  135,   55,  294,  261,  263,
 /*   880 */   262,  356,  357,  481,  452,  382,  395,  399,  408,  170,
 /*   890 */   295,  411,  420,  429,  432,  423,  426,  435,  441,  438,
 /*   900 */   353,  444,  136,  138,  140,  548,  149,  352,   34,   53,
 /*   910 */   160,  164,  169,  313,  239,  180,  320,   99,  447,  135,
 /*   920 */   344,  345,  261,  263,  262,  570,  553,  148,  452,  382,
 /*   930 */   395,  399,  408,  341,  555,  411,  420,  429,  432,  423,
 /*   940 */   426,  435,  441,  438,  556,  444,  136,  138,  140,  337,
 /*   950 */   149,  173,  332,  557,  160,  164,  169,  313,  462,  300,
 /*   960 */   320,  109,  447,  135,  559,  560,  261,  263,  262,  301,
 /*   970 */   478,  114,  452,  382,  395,  399,  408,  324,  146,  411,
 /*   980 */   420,  429,  432,  423,  426,  435,  441,  438,  474,  444,
 /*   990 */   136,  138,  140,   30,  149,  472,  145,  461,  160,  164,
 /*  1000 */   169,  313,  214,  256,  320,  483,  447,  135,  151,  484,
 /*  1010 */   261,  263,  262,  460,  255,  476,  452,  382,  395,  399,
 /*  1020 */   408,   47,  253,  411,  420,  429,  432,  423,  426,  435,
 /*  1030 */   441,  438,  142,  444,  136,  138,  140,  310,  149,  102,
 /*  1040 */   445,  445,  160,  164,  169,  313,  445,  445,  320,  445,
 /*  1050 */   447,  135,  488,  445,  445,   68,   46,   50,  445,  163,
 /*  1060 */   452,  382,  395,  399,  408,  445,  445,  411,  420,  429,
 /*  1070 */   432,  423,  426,  435,  441,  438,  445,  444,  136,  138,
 /*  1080 */   140,  445,  149,  445,  445,  445,  160,  164,  169,  313,
 /*  1090 */   192,  445,  320,  445,  447,  135,  445,  445,  261,  263,
 /*  1100 */   262,  445,  445,  112,  452,  382,  395,  399,  408,  445,
 /*  1110 */   445,  411,  420,  429,  432,  423,  426,  435,  441,  438,
 /*  1120 */   445,  444,  136,  138,  140,  445,  149,  445,  445,  445,
 /*  1130 */   160,  164,  169,  313,  196,  445,  320,  334,  447,  135,
 /*  1140 */   445,  346,  261,  263,  262,  261,  263,  262,  452,  382,
 /*  1150 */   395,  399,  408,  445,  445,  411,  420,  429,  432,  423,
 /*  1160 */   426,  435,  441,  438,  445,  444,  136,  138,  140,  445,
 /*  1170 */   149,  445,  445,  445,  160,  164,  169,  313,  445,  334,
 /*  1180 */   320,  133,  249,  378,  445,  445,  445,  261,  263,  262,
 /*  1190 */   261,  263,  262,  382,  395,  399,  408,  445,  492,  283,
 /*  1200 */   284,  287,  445,  453,  116,  117,  118,  119,  120,  121,
 /*  1210 */   122,  123,  124,  125,  126,  127,  128,  129,  130,  131,
 /*  1220 */   132,  134,  133,  511,  445,  106,  445,  512,  513,  445,
 /*  1230 */   445,   19,   88,  389,  391,  393,   22,  445,  445,  162,
 /*  1240 */    89,  445,  445,   88,  453,  116,  117,  118,  119,  120,
 /*  1250 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1260 */   131,  132,  134,  133,  445,  445,  445,  510,  104,  525,
 /*  1270 */   533,  445,  539,  513,  389,  391,  393,   88,  445,  445,
 /*  1280 */   113,  445,  445,  445,  445,  453,  116,  117,  118,  119,
 /*  1290 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1300 */   130,  131,  132,  134,  133,  445,   70,   71,   73,   72,
 /*  1310 */    74,  473,  445,  445,  312,  389,  391,  393,  445,  445,
 /*  1320 */   445,  147,  261,  263,  262,  445,  453,  116,  117,  118,
 /*  1330 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1340 */   129,  130,  131,  132,  134,   76,   77,  133,  100,  526,
 /*  1350 */   445,  520,  233,  445,  445,   75,  389,  391,  393,  445,
 /*  1360 */   261,  263,  262,  445,  485,  445,  445,  296,  265,  453,
 /*  1370 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1380 */   126,  127,  128,  129,  130,  131,  132,  134,  133,  106,
 /*  1390 */   445,  541,  105,  544,  545,  533,   88,  512,  513,  389,
 /*  1400 */   391,  393,   88,  445,  445,  469,  445,  445,  445,  445,
 /*  1410 */   453,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1420 */   125,  126,  127,  128,  129,  130,  131,  132,  134,  133,
 /*  1430 */   445,   70,   71,   73,   72,   74,  165,  445,  445,  312,
 /*  1440 */   389,  391,  393,  445,  445,  445,  457,  261,  263,  262,
 /*  1450 */   445,  453,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1460 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  134,
 /*  1470 */    76,   77,  133,  603,  514,  174,  520,  445,  312,  445,
 /*  1480 */    75,  389,  391,  393,  445,  445,  261,  263,  262,  502,
 /*  1490 */   445,  445,  445,  445,  453,  116,  117,  118,  119,  120,
 /*  1500 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1510 */   131,  132,  134,  133,  445,  445,  445,  504,  445,  334,
 /*  1520 */    68,   46,   50,  333,  389,  391,  393,  261,  263,  262,
 /*  1530 */   318,  445,  445,  445,  445,  453,  116,  117,  118,  119,
 /*  1540 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1550 */   130,  131,  132,  134,  133,  445,   70,   71,   73,   72,
 /*  1560 */    74,  445,  445,  445,  334,  389,  391,  393,  374,  445,
 /*  1570 */   445,  143,  261,  263,  262,  445,  453,  116,  117,  118,
 /*  1580 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1590 */   129,  130,  131,  132,  134,   76,   77,  133,  599,  526,
 /*  1600 */   141,  520,  445,  312,  445,   75,  389,  391,  393,  273,
 /*  1610 */   445,  261,  263,  262,  159,  445,  445,  445,  445,  453,
 /*  1620 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1630 */   126,  127,  128,  129,  130,  131,  132,  134,  133,  445,
 /*  1640 */   445,  445,  450,  445,  445,  445,  448,  445,  334,  389,
 /*  1650 */   391,  393,  342,  445,  445,  171,  261,  263,  262,  445,
 /*  1660 */   453,  116,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1670 */   125,  126,  127,  128,  129,  130,  131,  132,  134,  133,
 /*  1680 */   445,   70,   71,   73,   72,   74,  445,  445,  445,  334,
 /*  1690 */   389,  391,  393,  370,  445,  445,  479,  261,  263,  262,
 /*  1700 */   445,  453,  116,  117,  118,  119,  120,  121,  122,  123,
 /*  1710 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  134,
 /*  1720 */    76,   77,  133,  600,  526,  150,  520,  445,  312,  445,
 /*  1730 */    75,  389,  391,  393,  445,  445,  261,  263,  262,  508,
 /*  1740 */   445,  445,  445,  445,  453,  116,  117,  118,  119,  120,
 /*  1750 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1760 */   131,  132,  134,  133,  445,  445,  445,  445,  542,  525,
 /*  1770 */   533,  445,  445,  513,  389,  391,  393,   88,  445,  445,
 /*  1780 */   464,  445,  445,  445,  445,  453,  116,  117,  118,  119,
 /*  1790 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1800 */   130,  131,  132,  134,  133,  445,   70,   71,   73,   72,
 /*  1810 */    74,  445,  445,  445,  334,  389,  391,  393,  366,  445,
 /*  1820 */   445,  167,  261,  263,  262,  445,  453,  116,  117,  118,
 /*  1830 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1840 */   129,  130,  131,  132,  134,   76,   77,  133,   20,   78,
 /*  1850 */   445,  212,  445,  445,  210,   75,  389,  391,  393,  261,
 /*  1860 */   263,  262,  261,  263,  262,  445,  445,  445,  445,  115,
 /*  1870 */   116,  117,  118,  119,  120,  121,  122,  123,  124,  125,
 /*  1880 */   126,  127,  128,  129,  130,  131,  132,  134,  183,  186,
 /*  1890 */   189,  445,  445,  445,  445,  445,  225,  445,  445,  389,
 /*  1900 */   391,  393,   70,   71,   73,   72,   74,  445,  179,  234,
 /*  1910 */   238,  254,  176,  304,  445,  229,  183,  186,  189,  573,
 /*  1920 */   445,  445,  445,  445,  225,  334,  445,  445,  240,  338,
 /*  1930 */   219,  445,  445,  261,  263,  262,  179,  234,  238,  254,
 /*  1940 */   176,   76,   77,  250,   18,   78,  183,  186,  189,  445,
 /*  1950 */   445,   75,    8,  445,  225,   11,  240,   90,  219,  554,
 /*  1960 */   564,  445,  445,  452,  242,  248,  179,  234,  238,  254,
 /*  1970 */   176,  268,   54,  229,  241,  243,  244,  245,  246,  247,
 /*  1980 */    70,   71,   73,   72,   74,  445,  240,  467,  219,  110,
 /*  1990 */   306,   33,  278,  248,  279,  261,  263,  262,  261,  263,
 /*  2000 */   262,  445,  277,  243,  244,  245,  246,  247,   57,   58,
 /*  2010 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   76,
 /*  2020 */    77,  445,  242,  248,   43,  515,  445,  445,  445,   75,
 /*  2030 */   445,  445,  241,  243,  244,  245,  246,  247,  445,  445,
 /*  2040 */   183,  186,  189,  445,    8,  445,  445,   11,  225,   90,
 /*  2050 */   445,  554,  564,  445,  445,  452,  445,  445,  445,  445,
 /*  2060 */   179,  234,  238,  254,  176,  445,  445,  229,  445,  183,
 /*  2070 */   186,  189,  445,   38,  445,  445,  445,  225,  445,  445,
 /*  2080 */   240,  445,  219,   36,   37,   39,   40,   42,   41,  179,
 /*  2090 */   234,  238,  254,  176,  298,  445,  229,  233,  445,  445,
 /*  2100 */   445,  445,  445,  445,  445,  261,  263,  262,  454,  240,
 /*  2110 */   334,  219,  266,  265,  362,  307,  242,  248,  261,  263,
 /*  2120 */   262,  445,  445,  261,  263,  262,  241,  243,  244,  245,
 /*  2130 */   246,  247,  445,   70,   71,   73,   72,   74,  445,   70,
 /*  2140 */    71,   73,   72,   74,  334,  242,  248,  445,  358,  445,
 /*  2150 */   445,  445,  261,  263,  262,  241,  243,  244,  245,  246,
 /*  2160 */   247,  445,  273,  183,  186,  189,  534,  208,  445,  445,
 /*  2170 */   445,  225,   76,   77,  445,  261,  263,  262,   76,   77,
 /*  2180 */   107,  828,   75,  179,  234,  238,  254,  176,   75,  445,
 /*  2190 */   229,  398,  183,  186,  189,  230,  445,  270,  445,  251,
 /*  2200 */   225,  233,  445,  240,  445,  219,  445,  445,  445,  261,
 /*  2210 */   263,  262,  179,  234,  238,  254,  176,  264,  445,  229,
 /*  2220 */   445,  183,  186,  189,  445,  445,  445,  445,  445,  225,
 /*  2230 */   445,  445,  240,  445,  219,  445,  445,  445,  445,  242,
 /*  2240 */   248,  179,  234,  238,  254,  176,  445,  445,  229,  241,
 /*  2250 */   243,  244,  245,  246,  247,   70,   71,   73,   72,   74,
 /*  2260 */   466,  240,  334,  219,  445,  305,  354,  445,  242,  248,
 /*  2270 */   261,  263,  262,  261,  263,  262,  445,  445,  241,  243,
 /*  2280 */   244,  245,  246,  247,  445,   70,   71,   73,   72,   74,
 /*  2290 */   445,  445,  445,  445,   76,   77,  445,  242,  248,  372,
 /*  2300 */   445,  445,  496,  445,   75,  445,  445,  241,  243,  244,
 /*  2310 */   245,  246,  247,  445,  445,  183,  186,  189,  445,  445,
 /*  2320 */   445,  445,  445,  225,   76,   77,  445,  602,  494,   70,
 /*  2330 */    71,   73,   72,   74,   75,  179,  234,  238,  254,  176,
 /*  2340 */   409,  445,  229,  183,  186,  189,  445,  445,  261,  263,
 /*  2350 */   262,  225,  445,  445,  445,  240,  445,  219,  445,  445,
 /*  2360 */   445,  445,  528,  179,  234,  238,  254,  176,   76,   77,
 /*  2370 */   229,   54,  445,  445,  445,  445,  496,  445,   75,  445,
 /*  2380 */   445,  445,  445,  240,  445,  219,  177,  445,   26,  445,
 /*  2390 */    33,  242,  248,  340,  261,  263,  262,  445,  445,  445,
 /*  2400 */   445,  241,  243,  244,  245,  246,  247,   57,   58,   59,
 /*  2410 */    60,   61,   62,   63,   64,   65,   66,   67,  271,  242,
 /*  2420 */   248,  364,  445,  445,  445,  445,  261,  263,  262,  241,
 /*  2430 */   243,  244,  245,  246,  247,  445,  445,  183,  186,  189,
 /*  2440 */   233,  445,  445,  445,  445,  225,  445,  445,  261,  263,
 /*  2450 */   262,  445,  445,  445,  445,  289,  265,  179,  234,  238,
 /*  2460 */   254,  176,  276,  445,  229,  183,  186,  189,  445,  445,
 /*  2470 */   445,  445,  445,  225,  445,  445,  445,  240,  445,  219,
 /*  2480 */   445,  445,  445,  445,  445,  179,  234,  238,  254,  176,
 /*  2490 */   291,  206,  229,  445,  445,  183,  186,  189,  445,  261,
 /*  2500 */   263,  262,  184,  225,  445,  240,  445,  219,  445,  445,
 /*  2510 */   261,  263,  262,  242,  248,  179,  234,  238,  254,  176,
 /*  2520 */   257,   54,  229,  241,  243,  244,  245,  246,  247,   70,
 /*  2530 */    71,   73,   72,   74,  445,  240,  445,  219,  499,  445,
 /*  2540 */    33,  242,  248,  445,  445,  445,  445,  445,  445,  445,
 /*  2550 */   445,  241,  243,  244,  245,  246,  247,   57,   58,   59,
 /*  2560 */    60,   61,   62,   63,   64,   65,   66,   67,   76,   77,
 /*  2570 */   445,  242,  248,  445,  445,  445,   80,  445,   75,  445,
 /*  2580 */   445,  241,  243,  244,  245,  246,  247,  445,  445,  183,
 /*  2590 */   186,  189,  233,  445,  445,  445,  445,  225,  445,  445,
 /*  2600 */   261,  263,  262,  445,  445,  445,  445,  274,  265,  179,
 /*  2610 */   234,  238,  254,  176,  311,  445,  229,  183,  186,  189,
 /*  2620 */   445,  445,  261,  263,  262,  225,  445,  445,  445,  240,
 /*  2630 */   445,  219,  445,  445,  445,  445,  445,  179,  234,  238,
 /*  2640 */   254,  176,  200,  445,  250,   54,  445,  445,  445,  445,
 /*  2650 */   261,  263,  262,  445,  445,  445,  445,  240,  445,  219,
 /*  2660 */   445,  445,   83,  198,   33,  242,  248,  445,  445,  445,
 /*  2670 */   445,  261,  263,  262,  445,  241,  243,  244,  245,  246,
 /*  2680 */   247,   57,   58,   59,   60,   61,   62,   63,   64,   65,
 /*  2690 */    66,   67,  187,  278,  248,  445,  445,  445,  445,  445,
 /*  2700 */   261,  263,  262,  277,  243,  244,  245,  246,  247,  445,
 /*  2710 */   445,  445,  445,  445,  217,  213,  215,  178,  188,  193,
 /*  2720 */   195,  197,  199,  191,  185,  201,  203,  211,  205,  207,
 /*  2730 */   209,  217,  213,  215,  178,  188,  193,  195,  197,  199,
 /*  2740 */   191,  185,  201,  203,  211,  205,  207,  209,  217,  213,
 /*  2750 */   215,  178,  188,  193,  195,  197,  199,  191,  185,  201,
 /*  2760 */   203,  211,  205,  207,  209,  178,  188,  193,  195,  197,
 /*  2770 */   199,  191,  185,  201,  203,  211,  205,  207,  209,  445,
 /*  2780 */   317,  445,  445,  228,  445,  445,  445,  445,  445,  406,
 /*  2790 */   249,  261,  263,  262,  445,  445,  445,  445,  261,  263,
 /*  2800 */   262,  445,  326,  204,  280,  445,  308,  288,  284,  287,
 /*  2810 */   445,  261,  263,  262,  445,  445,  445,  217,  213,  215,
 /*  2820 */   178,  188,  193,  195,  197,  199,  191,  185,  201,  203,
 /*  2830 */   211,  205,  207,  209,  327,  328,  329,  330,  331,  335,
 /*  2840 */   445,  371,  233,  359,  379,  380,  405,  445,  445,  456,
 /*  2850 */   261,  263,  262,  445,  261,  263,  262,  302,  265,  445,
 /*  2860 */   217,  213,  215,  178,  188,  193,  195,  197,  199,  191,
 /*  2870 */   185,  201,  203,  211,  205,  207,  209,  326,  445,  445,
 /*  2880 */   445,  445,  445,  445,  445,  397,  217,  213,  215,  178,
 /*  2890 */   188,  193,  195,  197,  199,  191,  185,  201,  203,  211,
 /*  2900 */   205,  207,  209,  445,  445,  445,  445,  445,  445,  327,
 /*  2910 */   328,  329,  330,  331,  190,  445,  339,  194,  468,  348,
 /*  2920 */   349,  445,  261,  263,  262,  261,  263,  262,  445,  217,
 /*  2930 */   213,  215,  178,  188,  193,  195,  197,  199,  191,  185,
 /*  2940 */   201,  203,  211,  205,  207,  209,  217,  213,  215,  178,
 /*  2950 */   188,  193,  195,  197,  199,  191,  185,  201,  203,  211,
 /*  2960 */   205,  207,  209,   70,   71,   73,   72,   74,  445,  445,
 /*  2970 */   445,  233,  445,  445,  445,  445,  445,  445,  463,  261,
 /*  2980 */   263,  262,  445,  445,  445,  445,  258,  265,  445,  445,
 /*  2990 */   445,  445,  445,  445,  445,  445,  445,  445,  445,  445,
 /*  3000 */   445,  445,   76,   77,  735,  607,  494,  445,  445,  445,
 /*  3010 */   445,  445,   75,  445,  445,  217,  213,  215,  178,  188,
 /*  3020 */   193,  195,  197,  199,  191,  185,  201,  203,  211,  205,
 /*  3030 */   207,  209,  217,  213,  215,  178,  188,  193,  195,  197,
 /*  3040 */   199,  191,  185,  201,  203,  211,  205,  207,  209,  445,
 /*  3050 */   445,  445,  445,  445,  445,  445,  445,  410,  217,  213,
 /*  3060 */   215,  178,  188,  193,  195,  197,  199,  191,  185,  201,
 /*  3070 */   203,  211,  205,  207,  209,  445,  445,  445,  445,  445,
 /*  3080 */   445,  445,   54,  445,  445,  445,  445,  445,  445,  445,
 /*  3090 */   272,  445,  445,  445,  445,  445,  445,  445,  445,  445,
 /*  3100 */   445,   33,  213,  215,  178,  188,  193,  195,  197,  199,
 /*  3110 */   191,  185,  201,  203,  211,  205,  207,  209,   57,   58,
 /*  3120 */    59,   60,   61,   62,   63,   64,   65,   66,   67,   70,
 /*  3130 */    71,   73,   72,   74,   70,   71,   73,   72,   74,  445,
 /*  3140 */   445,  445,  445,  445,  445,  445,  445,  445,  445,  445,
 /*  3150 */   445,  445,  445,  445,  445,  445,  445,  445,  445,  445,
 /*  3160 */   445,  445,  445,  445,  445,  445,  445,  445,   76,   77,
 /*  3170 */   445,  601,  494,   76,   77,  445,  606,  494,   75,  445,
 /*  3180 */   445,  445,  445,   75,  445,   70,   71,   73,   72,   74,
 /*  3190 */    70,   71,   73,   72,   74,  445,   70,   71,   73,   72,
 /*  3200 */    74,   70,   71,   73,   72,   74,  445,  445,  445,  445,
 /*  3210 */   445,  445,  445,  445,  445,  445,  445,  445,  445,  445,
 /*  3220 */   445,  445,  445,  445,   76,   77,  445,  604,  543,   76,
 /*  3230 */    77,  445,  605,  494,   75,   76,   77,  445,  445,   75,
 /*  3240 */    76,   77,  445,   23,  445,   75,  445,  445,  107,  445,
 /*  3250 */    75,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    8,    9,  103,  104,  105,  106,  107,   15,  100,
 /*    10 */   101,  102,  103,  104,  105,  106,  107,   43,  152,  153,
 /*    20 */    27,   28,   29,   30,   31,  111,    6,   34,  114,  115,
 /*    30 */   116,  117,  118,  123,  120,  121,  126,  127,  128,  125,
 /*    40 */    47,   67,   49,   69,   51,   52,   53,   54,   55,   56,
 /*    50 */    57,   58,   59,   60,   61,   35,   10,   11,   12,   13,
 /*    60 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*    70 */    24,   25,   26,  118,    6,  118,   83,   84,   32,  122,
 /*    80 */   125,   49,  125,   34,   35,  109,   93,   94,   95,   96,
 /*    90 */    97,   98,   43,   44,   37,    6,   39,   42,   45,   42,
 /*   100 */    51,   52,   53,   54,   55,   56,   57,   58,   59,   34,
 /*   110 */    61,   62,   63,   64,   82,   66,   84,   42,   86,   70,
 /*   120 */    71,   72,   73,    6,   35,   76,   34,   35,    6,   34,
 /*   130 */    77,   78,   79,   80,   81,   43,   44,   84,   89,   90,
 /*   140 */    91,   92,   74,   51,   52,   53,   54,   55,   56,   57,
 /*   150 */    58,   59,   35,   61,   62,   63,   64,   35,   66,   24,
 /*   160 */    25,   26,   70,   71,   72,   73,   34,   82,   76,   84,
 /*   170 */    34,   35,   21,   22,   23,   24,   25,   26,   83,   43,
 /*   180 */    44,   89,   90,   91,   92,   45,   34,   51,   52,   53,
 /*   190 */    54,   55,   56,   57,   58,   59,   37,   61,   62,   63,
 /*   200 */    64,   42,   66,  126,  127,  128,   70,   71,   72,   73,
 /*   210 */   109,   37,   76,   39,   34,   35,   42,   77,   78,   79,
 /*   220 */    80,   81,    6,   43,   44,   89,   90,   91,   92,  162,
 /*   230 */   163,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   240 */     6,   61,   62,   63,   64,   82,   66,   84,   32,   34,
 /*   250 */    70,   71,   72,   73,  151,    6,   76,   42,   34,   35,
 /*   260 */   157,  158,  159,  160,  161,   43,   32,   43,   44,   89,
 /*   270 */    90,   91,   92,  162,  163,   51,   52,   53,   54,   55,
 /*   280 */    56,   57,   58,   59,   35,   61,   62,   63,   64,   67,
 /*   290 */    66,   69,  162,  163,   70,   71,   72,   73,  151,    6,
 /*   300 */    76,    6,   34,   35,  162,  163,  159,  160,  161,  149,
 /*   310 */   150,   43,   44,   89,   90,   91,   92,  170,  127,   51,
 /*   320 */    52,   53,   54,   55,   56,   57,   58,   59,   35,   61,
 /*   330 */    62,   63,   64,    6,   66,  162,  163,   42,   70,   71,
 /*   340 */    72,   73,  151,    6,   76,  150,   34,   35,  162,  163,
 /*   350 */   159,  160,  161,   35,  118,   43,   44,   89,   90,   91,
 /*   360 */    92,  125,   35,   51,   52,   53,   54,   55,   56,   57,
 /*   370 */    58,   59,   35,   61,   62,   63,   64,    6,   66,  162,
 /*   380 */   163,   34,   70,   71,   72,   73,  151,    6,   76,   39,
 /*   390 */    34,   35,   42,  158,  159,  160,  161,   34,    6,   43,
 /*   400 */    44,   89,   90,   91,   92,   42,   35,   51,   52,   53,
 /*   410 */    54,   55,   56,   57,   58,   59,   35,   61,   62,   63,
 /*   420 */    64,    6,   66,  162,  163,   34,   70,   71,   72,   73,
 /*   430 */    83,  151,   76,   47,   34,   35,  156,  162,  163,  159,
 /*   440 */   160,  161,    6,   43,   44,   89,   90,   91,   92,   35,
 /*   450 */    35,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   460 */     6,   61,   62,   63,   64,    6,   66,  110,   32,  112,
 /*   470 */    70,   71,   72,   73,   45,  151,   76,   85,   34,   35,
 /*   480 */   156,  162,  163,  159,  160,  161,   32,   43,   44,   89,
 /*   490 */    90,   91,   92,  109,   35,   51,   52,   53,   54,   55,
 /*   500 */    56,   57,   58,   59,    6,   61,   62,   63,   64,   34,
 /*   510 */    66,    6,    6,   34,   70,   71,   72,   73,  125,  151,
 /*   520 */    76,   45,   34,   35,  156,   34,   47,  159,  160,  161,
 /*   530 */    32,   43,   44,   89,   90,   91,   92,   32,   32,   51,
 /*   540 */    52,   53,   54,   55,   56,   57,   58,   59,  118,   61,
 /*   550 */    62,   63,   64,   35,   66,  125,   49,   86,   70,   71,
 /*   560 */    72,   73,  151,   45,   76,   42,   34,   35,   34,    0,
 /*   570 */   159,  160,  161,    6,    6,   43,   44,   89,   90,   91,
 /*   580 */    92,  108,   35,   51,   52,   53,   54,   55,   56,   57,
 /*   590 */    58,   59,   45,   61,   62,   63,   64,   42,   66,   32,
 /*   600 */    32,  109,   70,   71,   72,   73,  151,  110,   76,  112,
 /*   610 */    34,   35,   37,   34,  159,  160,  161,   42,    6,   43,
 /*   620 */    44,   89,   90,   91,   92,   34,   38,   51,   52,   53,
 /*   630 */    54,   55,   56,   57,   58,   59,   42,   61,   62,   63,
 /*   640 */    64,  110,   66,  112,   32,  119,   70,   71,   72,   73,
 /*   650 */   151,   35,   76,   68,   34,   35,   34,  164,  159,  160,
 /*   660 */   161,   68,    6,   43,   44,   89,   90,   91,   92,   34,
 /*   670 */   119,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   680 */   163,   61,   62,   63,   64,   49,   66,   45,   34,   49,
 /*   690 */    70,   71,   72,   73,  151,   68,   76,   34,   34,   35,
 /*   700 */    47,   34,  159,  160,  161,   45,   34,   43,   44,   89,
 /*   710 */    90,   91,   92,   42,  119,   51,   52,   53,   54,   55,
 /*   720 */    56,   57,   58,   59,   35,   61,   62,   63,   64,   35,
 /*   730 */    66,   47,   35,   45,   70,   71,   72,   73,   16,   35,
 /*   740 */    76,   35,   34,   35,  123,  119,   34,  126,  127,  128,
 /*   750 */    68,   43,   44,   89,   90,   91,   92,   34,   34,   51,
 /*   760 */    52,   53,   54,   55,   56,   57,   58,   59,   35,   61,
 /*   770 */    62,   63,   64,   35,   66,   35,   32,   42,   70,   71,
 /*   780 */    72,   73,  151,   35,   76,  108,   34,   35,  119,  119,
 /*   790 */   159,  160,  161,   35,   49,   43,   44,   89,   90,   91,
 /*   800 */    92,   35,   42,   51,   52,   53,   54,   55,   56,   57,
 /*   810 */    58,   59,   35,   61,   62,   63,   64,   34,   66,  109,
 /*   820 */    34,   85,   70,   71,   72,   73,  151,  154,   76,   35,
 /*   830 */    34,   35,  119,   34,  159,  160,  161,  154,   34,   43,
 /*   840 */    44,   89,   90,   91,   92,   34,  119,   51,   52,   53,
 /*   850 */    54,   55,   56,   57,   58,   59,  154,   61,   62,   63,
 /*   860 */    64,   16,   66,   85,   34,  154,   70,   71,   72,   73,
 /*   870 */   151,  154,   76,   34,   34,   35,   34,   43,  159,  160,
 /*   880 */   161,   43,  154,   43,   44,   89,   90,   91,   92,   42,
 /*   890 */    49,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*   900 */   154,   61,   62,   63,   64,  112,   66,   43,   45,  119,
 /*   910 */    70,   71,   72,   73,  151,  164,   76,   42,   34,   35,
 /*   920 */    85,  154,  159,  160,  161,  109,  109,   43,   44,   89,
 /*   930 */    90,   91,   92,  154,   38,   51,   52,   53,   54,   55,
 /*   940 */    56,   57,   58,   59,   34,   61,   62,   63,   64,  154,
 /*   950 */    66,   71,  154,  109,   70,   71,   72,   73,  151,   43,
 /*   960 */    76,   49,   34,   35,   34,  109,  159,  160,  161,   49,
 /*   970 */    68,   43,   44,   89,   90,   91,   92,  153,   42,   51,
 /*   980 */    52,   53,   54,   55,   56,   57,   58,   59,   35,   61,
 /*   990 */    62,   63,   64,   35,   66,   71,   65,   74,   70,   71,
 /*  1000 */    72,   73,  151,   49,   76,   65,   34,   35,   42,   42,
 /*  1010 */   159,  160,  161,   34,   34,   43,   44,   89,   90,   91,
 /*  1020 */    92,   34,   49,   51,   52,   53,   54,   55,   56,   57,
 /*  1030 */    58,   59,   42,   61,   62,   63,   64,   34,   66,   43,
 /*  1040 */   171,  171,   70,   71,   72,   73,  171,  171,   76,  171,
 /*  1050 */    34,   35,  123,  171,  171,  126,  127,  128,  171,   43,
 /*  1060 */    44,   89,   90,   91,   92,  171,  171,   51,   52,   53,
 /*  1070 */    54,   55,   56,   57,   58,   59,  171,   61,   62,   63,
 /*  1080 */    64,  171,   66,  171,  171,  171,   70,   71,   72,   73,
 /*  1090 */   151,  171,   76,  171,   34,   35,  171,  171,  159,  160,
 /*  1100 */   161,  171,  171,   43,   44,   89,   90,   91,   92,  171,
 /*  1110 */   171,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  1120 */   171,   61,   62,   63,   64,  171,   66,  171,  171,  171,
 /*  1130 */    70,   71,   72,   73,  151,  171,   76,  151,   34,   35,
 /*  1140 */   171,  155,  159,  160,  161,  159,  160,  161,   44,   89,
 /*  1150 */    90,   91,   92,  171,  171,   51,   52,   53,   54,   55,
 /*  1160 */    56,   57,   58,   59,  171,   61,   62,   63,   64,  171,
 /*  1170 */    66,  171,  171,  171,   70,   71,   72,   73,  171,  151,
 /*  1180 */    76,  107,  151,  155,  171,  171,  171,  159,  160,  161,
 /*  1190 */   159,  160,  161,   89,   90,   91,   92,  171,  124,  168,
 /*  1200 */   169,  170,  171,  129,  130,  131,  132,  133,  134,  135,
 /*  1210 */   136,  137,  138,  139,  140,  141,  142,  143,  144,  145,
 /*  1220 */   146,  147,  107,  116,  171,  118,  171,  120,  121,  171,
 /*  1230 */   171,  113,  125,  159,  160,  161,  118,  171,  171,  124,
 /*  1240 */   122,  171,  171,  125,  129,  130,  131,  132,  133,  134,
 /*  1250 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1260 */   145,  146,  147,  107,  171,  171,  171,  115,  116,  117,
 /*  1270 */   118,  171,  120,  121,  159,  160,  161,  125,  171,  171,
 /*  1280 */   124,  171,  171,  171,  171,  129,  130,  131,  132,  133,
 /*  1290 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1300 */   144,  145,  146,  147,  107,  171,    1,    2,    3,    4,
 /*  1310 */     5,  148,  171,  171,  151,  159,  160,  161,  171,  171,
 /*  1320 */   171,  124,  159,  160,  161,  171,  129,  130,  131,  132,
 /*  1330 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1340 */   143,  144,  145,  146,  147,   40,   41,  107,   43,   44,
 /*  1350 */   171,   46,  151,  171,  171,   50,  159,  160,  161,  171,
 /*  1360 */   159,  160,  161,  171,  124,  171,  171,  166,  167,  129,
 /*  1370 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1380 */   140,  141,  142,  143,  144,  145,  146,  147,  107,  118,
 /*  1390 */   171,  114,  121,  116,  117,  118,  125,  120,  121,  159,
 /*  1400 */   160,  161,  125,  171,  171,  124,  171,  171,  171,  171,
 /*  1410 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1420 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  107,
 /*  1430 */   171,    1,    2,    3,    4,    5,  148,  171,  171,  151,
 /*  1440 */   159,  160,  161,  171,  171,  171,  124,  159,  160,  161,
 /*  1450 */   171,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1460 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  1470 */    40,   41,  107,   43,   44,  148,   46,  171,  151,  171,
 /*  1480 */    50,  159,  160,  161,  171,  171,  159,  160,  161,  124,
 /*  1490 */   171,  171,  171,  171,  129,  130,  131,  132,  133,  134,
 /*  1500 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1510 */   145,  146,  147,  107,  171,  171,  171,  123,  171,  151,
 /*  1520 */   126,  127,  128,  155,  159,  160,  161,  159,  160,  161,
 /*  1530 */   124,  171,  171,  171,  171,  129,  130,  131,  132,  133,
 /*  1540 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1550 */   144,  145,  146,  147,  107,  171,    1,    2,    3,    4,
 /*  1560 */     5,  171,  171,  171,  151,  159,  160,  161,  155,  171,
 /*  1570 */   171,  124,  159,  160,  161,  171,  129,  130,  131,  132,
 /*  1580 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1590 */   143,  144,  145,  146,  147,   40,   41,  107,   43,   44,
 /*  1600 */   148,   46,  171,  151,  171,   50,  159,  160,  161,   49,
 /*  1610 */   171,  159,  160,  161,  124,  171,  171,  171,  171,  129,
 /*  1620 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1630 */   140,  141,  142,  143,  144,  145,  146,  147,  107,  171,
 /*  1640 */   171,  171,   82,  171,  171,  171,   86,  171,  151,  159,
 /*  1650 */   160,  161,  155,  171,  171,  124,  159,  160,  161,  171,
 /*  1660 */   129,  130,  131,  132,  133,  134,  135,  136,  137,  138,
 /*  1670 */   139,  140,  141,  142,  143,  144,  145,  146,  147,  107,
 /*  1680 */   171,    1,    2,    3,    4,    5,  171,  171,  171,  151,
 /*  1690 */   159,  160,  161,  155,  171,  171,  124,  159,  160,  161,
 /*  1700 */   171,  129,  130,  131,  132,  133,  134,  135,  136,  137,
 /*  1710 */   138,  139,  140,  141,  142,  143,  144,  145,  146,  147,
 /*  1720 */    40,   41,  107,   43,   44,  148,   46,  171,  151,  171,
 /*  1730 */    50,  159,  160,  161,  171,  171,  159,  160,  161,  124,
 /*  1740 */   171,  171,  171,  171,  129,  130,  131,  132,  133,  134,
 /*  1750 */   135,  136,  137,  138,  139,  140,  141,  142,  143,  144,
 /*  1760 */   145,  146,  147,  107,  171,  171,  171,  171,  116,  117,
 /*  1770 */   118,  171,  171,  121,  159,  160,  161,  125,  171,  171,
 /*  1780 */   124,  171,  171,  171,  171,  129,  130,  131,  132,  133,
 /*  1790 */   134,  135,  136,  137,  138,  139,  140,  141,  142,  143,
 /*  1800 */   144,  145,  146,  147,  107,  171,    1,    2,    3,    4,
 /*  1810 */     5,  171,  171,  171,  151,  159,  160,  161,  155,  171,
 /*  1820 */   171,  124,  159,  160,  161,  171,  129,  130,  131,  132,
 /*  1830 */   133,  134,  135,  136,  137,  138,  139,  140,  141,  142,
 /*  1840 */   143,  144,  145,  146,  147,   40,   41,  107,   43,   44,
 /*  1850 */   171,  151,  171,  171,  151,   50,  159,  160,  161,  159,
 /*  1860 */   160,  161,  159,  160,  161,  171,  171,  171,  171,  129,
 /*  1870 */   130,  131,  132,  133,  134,  135,  136,  137,  138,  139,
 /*  1880 */   140,  141,  142,  143,  144,  145,  146,  147,    7,    8,
 /*  1890 */     9,  171,  171,  171,  171,  171,   15,  171,  171,  159,
 /*  1900 */   160,  161,    1,    2,    3,    4,    5,  171,   27,   28,
 /*  1910 */    29,   30,   31,   32,  171,   34,    7,    8,    9,    0,
 /*  1920 */   171,  171,  171,  171,   15,  151,  171,  171,   47,  155,
 /*  1930 */    49,  171,  171,  159,  160,  161,   27,   28,   29,   30,
 /*  1940 */    31,   40,   41,   34,   43,   44,    7,    8,    9,  171,
 /*  1950 */   171,   50,   33,  171,   15,   36,   47,   38,   49,   40,
 /*  1960 */    41,  171,  171,   44,   83,   84,   27,   28,   29,   30,
 /*  1970 */    31,   32,   15,   34,   93,   94,   95,   96,   97,   98,
 /*  1980 */     1,    2,    3,    4,    5,  171,   47,  151,   49,   32,
 /*  1990 */   151,   34,   83,   84,   85,  159,  160,  161,  159,  160,
 /*  2000 */   161,  171,   93,   94,   95,   96,   97,   98,   51,   52,
 /*  2010 */    53,   54,   55,   56,   57,   58,   59,   60,   61,   40,
 /*  2020 */    41,  171,   83,   84,   34,   46,  171,  171,  171,   50,
 /*  2030 */   171,  171,   93,   94,   95,   96,   97,   98,  171,  171,
 /*  2040 */     7,    8,    9,  171,   33,  171,  171,   36,   15,   38,
 /*  2050 */   171,   40,   41,  171,  171,   44,  171,  171,  171,  171,
 /*  2060 */    27,   28,   29,   30,   31,  171,  171,   34,  171,    7,
 /*  2070 */     8,    9,  171,   83,  171,  171,  171,   15,  171,  171,
 /*  2080 */    47,  171,   49,   93,   94,   95,   96,   97,   98,   27,
 /*  2090 */    28,   29,   30,   31,   32,  171,   34,  151,  171,  171,
 /*  2100 */   171,  171,  171,  171,  171,  159,  160,  161,   75,   47,
 /*  2110 */   151,   49,  166,  167,  155,  151,   83,   84,  159,  160,
 /*  2120 */   161,  171,  171,  159,  160,  161,   93,   94,   95,   96,
 /*  2130 */    97,   98,  171,    1,    2,    3,    4,    5,  171,    1,
 /*  2140 */     2,    3,    4,    5,  151,   83,   84,  171,  155,  171,
 /*  2150 */   171,  171,  159,  160,  161,   93,   94,   95,   96,   97,
 /*  2160 */    98,  171,   49,    7,    8,    9,   34,  151,  171,  171,
 /*  2170 */   171,   15,   40,   41,  171,  159,  160,  161,   40,   41,
 /*  2180 */    48,   68,   50,   27,   28,   29,   30,   31,   50,  171,
 /*  2190 */    34,   35,    7,    8,    9,   82,  171,   84,  171,   86,
 /*  2200 */    15,  151,  171,   47,  171,   49,  171,  171,  171,  159,
 /*  2210 */   160,  161,   27,   28,   29,   30,   31,  167,  171,   34,
 /*  2220 */   171,    7,    8,    9,  171,  171,  171,  171,  171,   15,
 /*  2230 */   171,  171,   47,  171,   49,  171,  171,  171,  171,   83,
 /*  2240 */    84,   27,   28,   29,   30,   31,  171,  171,   34,   93,
 /*  2250 */    94,   95,   96,   97,   98,    1,    2,    3,    4,    5,
 /*  2260 */    75,   47,  151,   49,  171,  151,  155,  171,   83,   84,
 /*  2270 */   159,  160,  161,  159,  160,  161,  171,  171,   93,   94,
 /*  2280 */    95,   96,   97,   98,  171,    1,    2,    3,    4,    5,
 /*  2290 */   171,  171,  171,  171,   40,   41,  171,   83,   84,   85,
 /*  2300 */   171,  171,   48,  171,   50,  171,  171,   93,   94,   95,
 /*  2310 */    96,   97,   98,  171,  171,    7,    8,    9,  171,  171,
 /*  2320 */   171,  171,  171,   15,   40,   41,  171,   43,   44,    1,
 /*  2330 */     2,    3,    4,    5,   50,   27,   28,   29,   30,   31,
 /*  2340 */   151,  171,   34,    7,    8,    9,  171,  171,  159,  160,
 /*  2350 */   161,   15,  171,  171,  171,   47,  171,   49,  171,  171,
 /*  2360 */   171,  171,   34,   27,   28,   29,   30,   31,   40,   41,
 /*  2370 */    34,   15,  171,  171,  171,  171,   48,  171,   50,  171,
 /*  2380 */   171,  171,  171,   47,  171,   49,  151,  171,   32,  171,
 /*  2390 */    34,   83,   84,   85,  159,  160,  161,  171,  171,  171,
 /*  2400 */   171,   93,   94,   95,   96,   97,   98,   51,   52,   53,
 /*  2410 */    54,   55,   56,   57,   58,   59,   60,   61,  151,   83,
 /*  2420 */    84,   85,  171,  171,  171,  171,  159,  160,  161,   93,
 /*  2430 */    94,   95,   96,   97,   98,  171,  171,    7,    8,    9,
 /*  2440 */   151,  171,  171,  171,  171,   15,  171,  171,  159,  160,
 /*  2450 */   161,  171,  171,  171,  171,  166,  167,   27,   28,   29,
 /*  2460 */    30,   31,   32,  171,   34,    7,    8,    9,  171,  171,
 /*  2470 */   171,  171,  171,   15,  171,  171,  171,   47,  171,   49,
 /*  2480 */   171,  171,  171,  171,  171,   27,   28,   29,   30,   31,
 /*  2490 */    32,  151,   34,  171,  171,    7,    8,    9,  171,  159,
 /*  2500 */   160,  161,  151,   15,  171,   47,  171,   49,  171,  171,
 /*  2510 */   159,  160,  161,   83,   84,   27,   28,   29,   30,   31,
 /*  2520 */    32,   15,   34,   93,   94,   95,   96,   97,   98,    1,
 /*  2530 */     2,    3,    4,    5,  171,   47,  171,   49,   32,  171,
 /*  2540 */    34,   83,   84,  171,  171,  171,  171,  171,  171,  171,
 /*  2550 */   171,   93,   94,   95,   96,   97,   98,   51,   52,   53,
 /*  2560 */    54,   55,   56,   57,   58,   59,   60,   61,   40,   41,
 /*  2570 */   171,   83,   84,  171,  171,  171,   48,  171,   50,  171,
 /*  2580 */   171,   93,   94,   95,   96,   97,   98,  171,  171,    7,
 /*  2590 */     8,    9,  151,  171,  171,  171,  171,   15,  171,  171,
 /*  2600 */   159,  160,  161,  171,  171,  171,  171,  166,  167,   27,
 /*  2610 */    28,   29,   30,   31,  151,  171,   34,    7,    8,    9,
 /*  2620 */   171,  171,  159,  160,  161,   15,  171,  171,  171,   47,
 /*  2630 */   171,   49,  171,  171,  171,  171,  171,   27,   28,   29,
 /*  2640 */    30,   31,  151,  171,   34,   15,  171,  171,  171,  171,
 /*  2650 */   159,  160,  161,  171,  171,  171,  171,   47,  171,   49,
 /*  2660 */   171,  171,   32,  151,   34,   83,   84,  171,  171,  171,
 /*  2670 */   171,  159,  160,  161,  171,   93,   94,   95,   96,   97,
 /*  2680 */    98,   51,   52,   53,   54,   55,   56,   57,   58,   59,
 /*  2690 */    60,   61,  151,   83,   84,  171,  171,  171,  171,  171,
 /*  2700 */   159,  160,  161,   93,   94,   95,   96,   97,   98,  171,
 /*  2710 */   171,  171,  171,  171,   10,   11,   12,   13,   14,   15,
 /*  2720 */    16,   17,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  2730 */    26,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2740 */    19,   20,   21,   22,   23,   24,   25,   26,   10,   11,
 /*  2750 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2760 */    22,   23,   24,   25,   26,   13,   14,   15,   16,   17,
 /*  2770 */    18,   19,   20,   21,   22,   23,   24,   25,   26,  171,
 /*  2780 */    42,  171,  171,  151,  171,  171,  171,  171,  171,   85,
 /*  2790 */   151,  159,  160,  161,  171,  171,  171,  171,  159,  160,
 /*  2800 */   161,  171,   45,  151,  165,  171,   85,  168,  169,  170,
 /*  2810 */   171,  159,  160,  161,  171,  171,  171,   10,   11,   12,
 /*  2820 */    13,   14,   15,   16,   17,   18,   19,   20,   21,   22,
 /*  2830 */    23,   24,   25,   26,   77,   78,   79,   80,   81,   82,
 /*  2840 */   171,   84,  151,   86,   87,   88,  151,  171,  171,   42,
 /*  2850 */   159,  160,  161,  171,  159,  160,  161,  166,  167,  171,
 /*  2860 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2870 */    20,   21,   22,   23,   24,   25,   26,   45,  171,  171,
 /*  2880 */   171,  171,  171,  171,  171,   35,   10,   11,   12,   13,
 /*  2890 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2900 */    24,   25,   26,  171,  171,  171,  171,  171,  171,   77,
 /*  2910 */    78,   79,   80,   81,  151,  171,   84,  151,   42,   87,
 /*  2920 */    88,  171,  159,  160,  161,  159,  160,  161,  171,   10,
 /*  2930 */    11,   12,   13,   14,   15,   16,   17,   18,   19,   20,
 /*  2940 */    21,   22,   23,   24,   25,   26,   10,   11,   12,   13,
 /*  2950 */    14,   15,   16,   17,   18,   19,   20,   21,   22,   23,
 /*  2960 */    24,   25,   26,    1,    2,    3,    4,    5,  171,  171,
 /*  2970 */   171,  151,  171,  171,  171,  171,  171,  171,   42,  159,
 /*  2980 */   160,  161,  171,  171,  171,  171,  166,  167,  171,  171,
 /*  2990 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3000 */   171,  171,   40,   41,   85,   43,   44,  171,  171,  171,
 /*  3010 */   171,  171,   50,  171,  171,   10,   11,   12,   13,   14,
 /*  3020 */    15,   16,   17,   18,   19,   20,   21,   22,   23,   24,
 /*  3030 */    25,   26,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  3040 */    18,   19,   20,   21,   22,   23,   24,   25,   26,  171,
 /*  3050 */   171,  171,  171,  171,  171,  171,  171,   35,   10,   11,
 /*  3060 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  3070 */    22,   23,   24,   25,   26,  171,  171,  171,  171,  171,
 /*  3080 */   171,  171,   15,  171,  171,  171,  171,  171,  171,  171,
 /*  3090 */    85,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3100 */   171,   34,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  3110 */    19,   20,   21,   22,   23,   24,   25,   26,   51,   52,
 /*  3120 */    53,   54,   55,   56,   57,   58,   59,   60,   61,    1,
 /*  3130 */     2,    3,    4,    5,    1,    2,    3,    4,    5,  171,
 /*  3140 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3150 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3160 */   171,  171,  171,  171,  171,  171,  171,  171,   40,   41,
 /*  3170 */   171,   43,   44,   40,   41,  171,   43,   44,   50,  171,
 /*  3180 */   171,  171,  171,   50,  171,    1,    2,    3,    4,    5,
 /*  3190 */     1,    2,    3,    4,    5,  171,    1,    2,    3,    4,
 /*  3200 */     5,    1,    2,    3,    4,    5,  171,  171,  171,  171,
 /*  3210 */   171,  171,  171,  171,  171,  171,  171,  171,  171,  171,
 /*  3220 */   171,  171,  171,  171,   40,   41,  171,   43,   44,   40,
 /*  3230 */    41,  171,   43,   44,   50,   40,   41,  171,  171,   50,
 /*  3240 */    40,   41,  171,   48,  171,   50,  171,  171,   48,  171,
 /*  3250 */    50,
};
#define YY_SHIFT_USE_DFLT (-27)
static short yy_shift_ofst[] = {
 /*     0 */  2011,  569, 1919,  -27,  -27,  -27,  -27,  -27,  786,  733,
 /*    10 */   -27,  491,  159,  -27,  654,  555,  -27, 1901,  -27, 1805,
 /*    20 */   -27,  -27, 3195,  672,  636, 2356,  616,  -27,  234,  958,
 /*    30 */   -27, 3067,  -27,  863, 1990,  -27,  -27,  -27,  -27,  -27,
 /*    40 */   -27,  -27,  -27,  471,  386,  -27,  987,  476, 1990,  -27,
 /*    50 */   579,  429, 1990,  -27,  842,  845,  799,  -27,  -27,  -27,
 /*    60 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*    70 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27, 2138, 2528,
 /*    80 */   783,  745, 2630,  738,  -27,  216,  694,  -27,  -27,  -27,
 /*    90 */   591,   57,  -27,  534,  350,  -27,  391,  295,  -27, 1305,
 /*   100 */   -27,  996,  -27, 1555, 2284,  -27, 3200,  811,  912, 1957,
 /*   110 */   735, 1060,  -27,  928,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   120 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   130 */   -27,  -27,  -27,  -27,  -27,  -27,  414,  -27,  318,  -27,
 /*   140 */  2582,  990,  796,  400,  931,  936,  840,  884,  -27, 2582,
 /*   150 */   966,  222,  -27,  -26,  -27,  -27, 1990,  585, 1104, 1104,
 /*   160 */   671,  972, 1016,  -27, 2582,  760,   49,   92,  -27,  847,
 /*   170 */   136,  224,  880, 2582,  740,  -27, 2582,  -27, 2582,  622,
 /*   180 */   -27,   85, 2582, 2582, 3048, 2582, 2582, 3048, 2582, 2582,
 /*   190 */  3048, 2582,  151, 2582,  151, 2582,  151, 2582,  151, 2582,
 /*   200 */   151, 2582,  135, 2582,  135, 2582,  -27, 2582,  -27, 2582,
 /*   210 */   -27, 2582,  135, 2582, 2752, 2582, 2752, 2582, 3091,   -7,
 /*   220 */    46,  -27,  744, 2582,  -27,  712,  722, 2582,  151,   32,
 /*   230 */   215,  640, 2458, 3048,  635,  656,  622,  -27, 2582,  -27,
 /*   240 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27, 1909, 3048,
 /*   250 */  2113,  479,  973, 1939,  980,  954, 2488,  -27,  436,  -27,
 /*   260 */  2582,  -27,  -27,  -27,  -27,  -27,  454,  -27,  -27,  -27,
 /*   270 */  2582, 3005,  -27, 2430,  505,  -27,  -27,  593,  627,  -27,
 /*   280 */   392,  -27, 2610,  -27,  682, 2582,  -27,  -27,  -27,  567,
 /*   290 */   -27,  -27,  347,  834,  841, 2062,  568,  -27,  -27,  916,
 /*   300 */   920, 1881,  612,  -27,  -27,  151,  151, 2721,  -27, 1003,
 /*   310 */   -27,  151, 3048,  804,   68, 2033, 2738, 1104,  532,  -27,
 /*   320 */   663,   20,  -27,  663,  -27, 2757,  -27,  -27,  -27,  -27,
 /*   330 */   -27,  -27, 2582,  -27, 3048,   75, 2832, 2582,  -27, 2308,
 /*   340 */   140, 2582,  -27,  835,  140, 2582,  -27, 2919,  -27,  -27,
 /*   350 */    95,  864,  140, 2582,  -27,  838,  140, 2582,  -27,  830,
 /*   360 */    53, 2582,  -27, 2336,  140, 2582,  -27,  778,  140, 2582,
 /*   370 */   -27, 2214,  140, 2582,  -27,  736,  140, 2582,  -27,  -27,
 /*   380 */   -27,  -27, 2582,  117,  -27, 2582,  -27, 3048,  -27,  777,
 /*   390 */   -27,  766,  -27,  748,  -27, 2156, 2850,  -27,  -27,  724,
 /*   400 */   163,  723,  704,  -27, 2582, 2704,  697,  -27, 2582, 3022,
 /*   410 */   -27,  152,  122,  -27,  152,  -27,  642, 1990,  -27,  -27,
 /*   420 */   152,  249,  -27,  152,  293,  -27,  152,  327,  -27,  152,
 /*   430 */   337,  -27,  152,  371,  -27,  152,  381,  -27,  152,   89,
 /*   440 */   -27,  152,  415,  -27,  152,  459,  -27, 1560,  132,  973,
 /*   450 */   363,  640,  -27,  -27, 2582, 2807, 1104,  620,  -27,  979,
 /*   460 */   923, 2185, 2936, 1104,  356,  -27, 2582, 2876, 1104,  576,
 /*   470 */   -27,  924, 2582,  953,  -27,  -27,  -27,  902, 1104, 1104,
 /*   480 */   -27,  -27,  940,  967,  180,  268,  -27,  -27,  498,   55,
 /*   490 */   312,  -27,  444,  -27, 2138, 2254,  475,  507, 2506,  523,
 /*   500 */   488,  -27,  664,  -27,  506,  594,  708,  -27,  752,  -27,
 /*   510 */  1430, 3133,  -27,  -27, 1979,  653,  660, 1990,  689,  -27,
 /*   520 */   684,  688, 1990,  706,  -27,  -27, 1979, 2328,  518,  -27,
 /*   530 */  1990,  758,  -27, 2132,  547,  -27, 1990,  794,  -27,  -27,
 /*   540 */  1680, 3184, 2962, 2138, 3189,  -27, 3128,  391,  -27,  -27,
 /*   550 */   -27,  391,  295,  -27,  896,  910,  174,  -27,  930,  875,
 /*   560 */   -27,  391,  295,  -27,  588,  667,  575,  -27,  839,  875,
 /*   570 */   -27,  -27,
};
#define YY_REDUCE_USE_DFLT (-135)
static short yy_reduce_ofst[] = {
 /*     0 */   -91, -135, -100, -135, -135, -135, -135, -135, -135, -135,
 /*    10 */  -135, -135,  677, -135, -135,  473, -135, 1118, -135,  -43,
 /*    20 */  -135, -135,  393, -135, -135,  621, -135, -135, -135, -135,
 /*    30 */  -135,   77, -135, -135,  727, -135, -135, -135, -135, -135,
 /*    40 */  -135, -135, -135, -135, -135, -135, -135, -135,  669, -135,
 /*    50 */  -135, -135,  790, -135, -135, -135, -135, -135, -135, -135,
 /*    60 */  -135, -135, -135, -135, -135, -135, -135, -135, -135, -135,
 /*    70 */  -135, -135, -135, -135, -135, -135, -135, -135,  -45,  393,
 /*    80 */  -135, -135,  -90, -135, -135, -135, -135, -135, -135, -135,
 /*    90 */  -135,  492, -135, -135,  384, -135,  357,  101, -135,  -86,
 /*   100 */  -135, -135, -135, 1152, 1271, -135,  393, -135, -135,  929,
 /*   110 */  -135, 1156, -135, 1740, -135, -135, -135, -135, -135, -135,
 /*   120 */  -135, -135, -135, -135, -135, -135, -135, -135, -135, -135,
 /*   130 */  -135, -135, -135, -135, -135, -135, -135, -135, -135, -135,
 /*   140 */  1452, -135, 1447, 1740, -135, -135, 1197, 1740, -135, 1577,
 /*   150 */  -135,  160, -135,  195, -135, -135,  526, -135, 1490, 1740,
 /*   160 */  -135, 1115, 1740, -135, 1288, -135, 1697, 1740, -135, -135,
 /*   170 */  1531, 1740, -135, 1327, -135, -135, 2235, -135, 2463,  751,
 /*   180 */  -135, -135, 1964, 2351, -135, 1839, 2541, -135, 2114, 2763,
 /*   190 */  -135,  939, -135, 2766, -135,  983, -135, 2512, -135, 2491,
 /*   200 */  -135,  631, -135, 2652, -135, 2340, -135, 2016, -135, 1703,
 /*   210 */  -135, 1700, -135,  851, -135,  719, -135,  675, -135,  191,
 /*   220 */  -135, -135, -135,  455, -135, -135, -135, 2632, -135, -135,
 /*   230 */  -135, -135, 2289, -135, -135, -135,  493, -135,  763, -135,
 /*   240 */  -135, -135, -135, -135, -135, -135, -135, -135, 2639, -135,
 /*   250 */  -135, -135, -135, 1946, -135, -135, 2820, -135, -135, -135,
 /*   260 */  2050, -135, -135, -135, -135, -135, -135, -135, -135, -135,
 /*   270 */  2267, -135, -135, 2441, -135, -135, -135, -135, -135, -135,
 /*   280 */  -135, -135, 1031, -135, -135,  147, -135, -135, -135, -135,
 /*   290 */  -135, -135, -135, -135, -135, 1201, -135, -135, -135, -135,
 /*   300 */  -135, 2691, -135, -135, -135, -135, -135, -135, -135, -135,
 /*   310 */  -135, -135, -135, -135, -135,  543, -135, 1406, 1740, -135,
 /*   320 */  -134, -135, -135,  824, -135,  798, -135, -135, -135, -135,
 /*   330 */  -135, -135, 1368, -135, -135, -135,  795, 1774, -135,  368,
 /*   340 */   779, 1497, -135, -135,  767,  986, -135, -135, -135, -135,
 /*   350 */  -135, -135,  746, 2111, -135, -135,  728, 1993, -135, -135,
 /*   360 */   717, 1959, -135,  324,  711, 1663, -135, -135,  702, 1538,
 /*   370 */  -135,  280,  683, 1413, -135, -135,  673, 1028, -135, -135,
 /*   380 */  -135, -135,  103, -135, -135,  235, -135, -135, -135, -135,
 /*   390 */  -135, -135, -135, -135, -135,  499, -135, -135, -135, -135,
 /*   400 */  -135, -135, -135, -135, 2695, -135, -135, -135, 2189, -135,
 /*   410 */  -135,   67, -135, -135,  517, -135, -135,  551, -135, -135,
 /*   420 */   111, -135, -135,  130, -135, -135,  142, -135, -135,  173,
 /*   430 */  -135, -135,  186, -135, -135,  217, -135, -135,  261, -135,
 /*   440 */  -135,  275, -135, -135,  319, -135, -135, -135, -135, -135,
 /*   450 */  -135, -135, -135, -135,  411, -135, 1322, 1740, -135, -135,
 /*   460 */  -135,  807, -135, 1656, 1740, -135, 1836, -135, 1281, 1740,
 /*   470 */  -135, -135, 1163, -135, -135, -135, -135, -135, 1572, 1740,
 /*   480 */  -135, -135, -135, -135, 1240, 1740, -135, -135, -135, -135,
 /*   490 */  1074, -135, 1740, -135,  236,  393, -135, -135, 1394, -135,
 /*   500 */  1365, -135, 1740, -135, -135, -135, 1615, -135, 1740, -135,
 /*   510 */  1107, 1271, -135, -135,  236, -135, -135,  595, -135, -135,
 /*   520 */  -135, -135,  626, -135, -135, -135,  430,  393, -135, -135,
 /*   530 */   670, -135, -135,  393, -135, -135,  713, -135, -135, -135,
 /*   540 */  1277, 1652, 1271,  430, 1271, -135, 1271,  793, -135, -135,
 /*   550 */  -135,  497,  817, -135, -135, -135,  844, -135, -135,  856,
 /*   560 */  -135,  531,  -24, -135, -135, -135,  710, -135, -135,  816,
 /*   570 */  -135, -135,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   844,  844,  844,  574,  576,  577,  578,  579,  844,  844,
 /*    10 */   580,  844,  844,  581,  844,  844,  582,  844,  597,  844,
 /*    20 */   598,  618,  844,  844,  844,  844,  844,  630,  844,  844,
 /*    30 */   631,  844,  644,  646,  844,  649,  832,  833,  834,  835,
 /*    40 */   836,  837,  838,  844,  844,  839,  844,  647,  844,  650,
 /*    50 */   844,  648,  844,  651,  844,  844,  844,  653,  654,  655,
 /*    60 */   656,  657,  658,  659,  660,  661,  662,  663,  645,  634,
 /*    70 */   636,  637,  638,  639,  640,  641,  642,  643,  844,  844,
 /*    80 */   844,  844,  844,  844,  632,  844,  844,  633,  635,  619,
 /*    90 */   844,  844,  583,  844,  844,  584,  844,  844,  586,  844,
 /*   100 */   592,  844,  593,  844,  844,  616,  844,  844,  844,  844,
 /*   110 */   844,  844,  622,  844,  624,  664,  666,  667,  668,  669,
 /*   120 */   670,  671,  672,  673,  674,  675,  676,  677,  678,  679,
 /*   130 */   680,  681,  682,  683,  684,  685,  844,  686,  844,  687,
 /*   140 */   844,  844,  844,  844,  690,  844,  844,  844,  691,  844,
 /*   150 */   844,  844,  694,  844,  695,  696,  844,  844,  698,  699,
 /*   160 */   844,  844,  844,  702,  844,  844,  844,  844,  704,  844,
 /*   170 */   844,  844,  844,  844,  844,  706,  844,  763,  844,  844,
 /*   180 */   764,  844,  844,  844,  765,  844,  844,  766,  844,  844,
 /*   190 */   767,  844,  771,  844,  772,  844,  773,  844,  774,  844,
 /*   200 */   775,  844,  783,  844,  784,  844,  785,  844,  786,  844,
 /*   210 */   787,  844,  788,  844,  789,  844,  790,  844,  791,  844,
 /*   220 */   844,  776,  844,  844,  777,  844,  844,  844,  778,  795,
 /*   230 */   844,  779,  844,  823,  844,  844,  844,  792,  844,  793,
 /*   240 */   794,  796,  797,  798,  799,  800,  801,  802,  844,  831,
 /*   250 */   795,  844,  780,  844,  844,  805,  844,  806,  844,  807,
 /*   260 */   844,  818,  819,  820,  821,  822,  844,  810,  811,  781,
 /*   270 */   844,  844,  782,  844,  844,  808,  809,  796,  797,  803,
 /*   280 */   844,  804,  844,  824,  844,  844,  826,  827,  825,  844,
 /*   290 */   812,  813,  844,  844,  844,  844,  844,  814,  815,  844,
 /*   300 */   844,  844,  844,  816,  817,  770,  769,  844,  840,  844,
 /*   310 */   841,  768,  842,  844,  844,  844,  844,  844,  844,  707,
 /*   320 */   844,  844,  711,  844,  712,  844,  714,  715,  716,  717,
 /*   330 */   718,  719,  844,  720,  762,  844,  844,  844,  721,  844,
 /*   340 */   844,  844,  724,  844,  844,  844,  725,  844,  731,  732,
 /*   350 */   844,  844,  844,  844,  722,  844,  844,  844,  723,  844,
 /*   360 */   844,  844,  726,  844,  844,  844,  727,  844,  844,  844,
 /*   370 */   728,  844,  844,  844,  729,  844,  844,  844,  730,  733,
 /*   380 */   734,  713,  844,  844,  736,  844,  737,  739,  738,  844,
 /*   390 */   740,  844,  741,  844,  742,  844,  844,  743,  744,  844,
 /*   400 */   844,  844,  844,  745,  844,  844,  844,  746,  844,  844,
 /*   410 */   747,  844,  844,  748,  844,  758,  760,  844,  761,  759,
 /*   420 */   844,  844,  749,  844,  844,  750,  844,  844,  751,  844,
 /*   430 */   844,  752,  844,  844,  753,  844,  844,  754,  844,  844,
 /*   440 */   755,  844,  844,  756,  844,  844,  757,  844,  844,  844,
 /*   450 */   844,  844,  843,  665,  844,  844,  844,  844,  708,  844,
 /*   460 */   844,  844,  844,  844,  844,  709,  844,  844,  844,  844,
 /*   470 */   710,  844,  844,  844,  705,  703,  701,  844,  844,  700,
 /*   480 */   697,  692,  688,  844,  844,  844,  693,  689,  844,  844,
 /*   490 */   844,  623,  844,  625,  844,  844,  844,  844,  844,  844,
 /*   500 */   844,  626,  844,  628,  844,  844,  844,  627,  844,  629,
 /*   510 */   844,  844,  614,  617,  844,  844,  844,  844,  844,  620,
 /*   520 */   844,  844,  844,  844,  621,  608,  844,  844,  844,  610,
 /*   530 */   844,  844,  612,  844,  844,  611,  844,  844,  613,  615,
 /*   540 */   844,  844,  844,  844,  844,  609,  844,  844,  594,  596,
 /*   550 */   595,  844,  844,  585,  844,  844,  844,  587,  844,  844,
 /*   560 */   588,  844,  844,  589,  844,  844,  844,  590,  844,  844,
 /*   570 */   591,  575,
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
 /* 238 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 239 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 240 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 241 */ "xx_mcall_expr ::= IDENTIFIER ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 242 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 243 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 244 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 245 */ "xx_mcall_expr ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 246 */ "xx_common_expr ::= xx_mcall_expr",
 /* 247 */ "xx_common_expr ::= xx_scall_expr",
 /* 248 */ "xx_common_expr ::= xx_fcall_expr",
 /* 249 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 250 */ "xx_call_parameters ::= xx_call_parameter",
 /* 251 */ "xx_call_parameter ::= xx_common_expr",
 /* 252 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 253 */ "xx_array_list ::= xx_array_item",
 /* 254 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 255 */ "xx_array_item ::= xx_array_value",
 /* 256 */ "xx_array_key ::= IDENTIFIER",
 /* 257 */ "xx_array_key ::= STRING",
 /* 258 */ "xx_array_key ::= INTEGER",
 /* 259 */ "xx_array_value ::= xx_common_expr",
 /* 260 */ "xx_literal_expr ::= INTEGER",
 /* 261 */ "xx_literal_expr ::= CHAR",
 /* 262 */ "xx_literal_expr ::= STRING",
 /* 263 */ "xx_literal_expr ::= DOUBLE",
 /* 264 */ "xx_literal_expr ::= NULL",
 /* 265 */ "xx_literal_expr ::= FALSE",
 /* 266 */ "xx_literal_expr ::= TRUE",
 /* 267 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 268 */ "xx_isset_expr ::= IDENTIFIER SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 269 */ "xx_isset_expr ::= IDENTIFIER ARROW IDENTIFIER",
 /* 270 */ "xx_eval_expr ::= xx_common_expr",
 /* 271 */ "xx_comment ::= COMMENT",
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
// 913 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 2364 "parser.c"
      break;
    case 101:
// 926 "parser.lemon"
{ json_object_put((yypminor->yy7)); }
// 2369 "parser.c"
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
// 922 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy7;
}
// 2858 "parser.c"
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
      case 246:
      case 247:
      case 248:
      case 251:
      case 259:
      case 270:
// 928 "parser.lemon"
{
	yygotominor.yy7 = yymsp[0].minor.yy7;
}
// 2896 "parser.c"
        break;
      case 2:
      case 36:
      case 42:
      case 44:
      case 46:
      case 62:
      case 92:
      case 124:
// 932 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(yymsp[-1].minor.yy7, yymsp[0].minor.yy7);
}
// 2910 "parser.c"
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
      case 250:
      case 253:
// 936 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(NULL, yymsp[0].minor.yy7);
}
// 2931 "parser.c"
        break;
      case 8:
// 956 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(33,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2940 "parser.c"
        break;
      case 9:
// 960 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-2].minor);
}
// 2948 "parser.c"
        break;
      case 10:
// 964 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(36,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2957 "parser.c"
        break;
      case 11:
// 968 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy7, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
}
// 2965 "parser.c"
        break;
      case 12:
// 972 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 2974 "parser.c"
        break;
      case 13:
// 976 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 0, 0, NULL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 2983 "parser.c"
        break;
      case 14:
// 980 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy7, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(38,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 2993 "parser.c"
        break;
      case 15:
// 984 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy7, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3002 "parser.c"
        break;
      case 16:
// 988 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3012 "parser.c"
        break;
      case 17:
// 992 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 1, 0, NULL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(40,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
}
// 3022 "parser.c"
        break;
      case 18:
// 996 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy7, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-3].minor);
  yy_destructor(38,&yymsp[-2].minor);
}
// 3031 "parser.c"
        break;
      case 19:
// 1000 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy7, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(41,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
}
// 3041 "parser.c"
        break;
      case 20:
      case 25:
// 1004 "parser.lemon"
{
	yygotominor.yy7 = NULL;
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3051 "parser.c"
        break;
      case 21:
      case 26:
// 1008 "parser.lemon"
{
	yygotominor.yy7 = yymsp[-1].minor.yy7;
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3061 "parser.c"
        break;
      case 22:
      case 72:
      case 140:
      case 165:
      case 186:
      case 249:
      case 252:
// 1012 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_list(yymsp[-2].minor.yy7, yymsp[0].minor.yy7);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3075 "parser.c"
        break;
      case 24:
      case 223:
      case 256:
// 1020 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3084 "parser.c"
        break;
      case 27:
// 1032 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
}
// 3091 "parser.c"
        break;
      case 28:
// 1036 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 3098 "parser.c"
        break;
      case 29:
// 1040 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, yymsp[0].minor.yy7, NULL, status->scanner_state);
}
// 3105 "parser.c"
        break;
      case 30:
// 1044 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
}
// 3112 "parser.c"
        break;
      case 31:
// 1048 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 3119 "parser.c"
        break;
      case 32:
// 1052 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[0].minor.yy7, NULL, yymsp[-1].minor.yy7, status->scanner_state);
}
// 3126 "parser.c"
        break;
      case 33:
// 1056 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(NULL, yymsp[0].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
}
// 3133 "parser.c"
        break;
      case 34:
// 1060 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
}
// 3140 "parser.c"
        break;
      case 35:
// 1064 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_definition(yymsp[-1].minor.yy7, yymsp[0].minor.yy7, yymsp[-2].minor.yy7, status->scanner_state);
}
// 3147 "parser.c"
        break;
      case 38:
// 1080 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(yymsp[-2].minor.yy7, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3155 "parser.c"
        break;
      case 39:
// 1084 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(yymsp[-2].minor.yy7, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3163 "parser.c"
        break;
      case 40:
// 1088 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(yymsp[-4].minor.yy7, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3172 "parser.c"
        break;
      case 41:
// 1092 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_property(yymsp[-4].minor.yy7, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3181 "parser.c"
        break;
      case 48:
// 1120 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3191 "parser.c"
        break;
      case 49:
// 1124 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(46,&yymsp[-4].minor);
  yy_destructor(45,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3201 "parser.c"
        break;
      case 50:
// 1128 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-6].minor.yy7, yymsp[-4].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3213 "parser.c"
        break;
      case 51:
// 1132 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-7].minor.yy7, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3225 "parser.c"
        break;
      case 52:
// 1136 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-7].minor.yy7, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3237 "parser.c"
        break;
      case 53:
// 1140 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-8].minor.yy7, yymsp[-6].minor.yy0, yymsp[-4].minor.yy7, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3249 "parser.c"
        break;
      case 54:
// 1144 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-6].minor.yy7, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3261 "parser.c"
        break;
      case 55:
// 1148 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-7].minor.yy7, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, NULL, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3273 "parser.c"
        break;
      case 56:
// 1152 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-7].minor.yy7, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy7, yymsp[-8].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3285 "parser.c"
        break;
      case 57:
// 1156 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-8].minor.yy7, yymsp[-6].minor.yy0, yymsp[-4].minor.yy7, yymsp[-1].minor.yy7, yymsp[-9].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(32,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3297 "parser.c"
        break;
      case 58:
// 1160 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-5].minor.yy7, yymsp[-3].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3308 "parser.c"
        break;
      case 59:
// 1164 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-6].minor.yy7, yymsp[-4].minor.yy0, yymsp[-2].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3319 "parser.c"
        break;
      case 60:
// 1168 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-5].minor.yy7, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3330 "parser.c"
        break;
      case 61:
// 1172 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_class_method(yymsp[-6].minor.yy7, yymsp[-4].minor.yy0, yymsp[-2].minor.yy7, NULL, yymsp[-7].minor.yy0, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3341 "parser.c"
        break;
      case 64:
// 1184 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 3349 "parser.c"
        break;
      case 65:
// 1188 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3357 "parser.c"
        break;
      case 66:
// 1192 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3365 "parser.c"
        break;
      case 67:
// 1196 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3373 "parser.c"
        break;
      case 68:
// 1200 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3381 "parser.c"
        break;
      case 69:
// 1204 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("inline");
  yy_destructor(50,&yymsp[0].minor);
}
// 3389 "parser.c"
        break;
      case 70:
// 1208 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("abstract");
  yy_destructor(40,&yymsp[0].minor);
}
// 3397 "parser.c"
        break;
      case 71:
// 1212 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("final");
  yy_destructor(41,&yymsp[0].minor);
}
// 3405 "parser.c"
        break;
      case 74:
// 1224 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3412 "parser.c"
        break;
      case 75:
// 1228 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(yymsp[-1].minor.yy7, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3419 "parser.c"
        break;
      case 76:
// 1232 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, yymsp[-1].minor.yy7, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3426 "parser.c"
        break;
      case 77:
// 1236 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3434 "parser.c"
        break;
      case 78:
// 1240 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(yymsp[-3].minor.yy7, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3442 "parser.c"
        break;
      case 79:
// 1244 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_parameter(NULL, yymsp[-3].minor.yy7, yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 3450 "parser.c"
        break;
      case 80:
// 1248 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(15,&yymsp[-2].minor);
  yy_destructor(16,&yymsp[0].minor);
}
// 3459 "parser.c"
        break;
      case 81:
// 1252 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(51,&yymsp[0].minor);
}
// 3467 "parser.c"
        break;
      case 82:
// 1256 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(52,&yymsp[0].minor);
}
// 3475 "parser.c"
        break;
      case 83:
// 1260 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(53,&yymsp[0].minor);
}
// 3483 "parser.c"
        break;
      case 84:
// 1264 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(54,&yymsp[0].minor);
}
// 3491 "parser.c"
        break;
      case 85:
// 1268 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(55,&yymsp[0].minor);
}
// 3499 "parser.c"
        break;
      case 86:
// 1272 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3507 "parser.c"
        break;
      case 87:
// 1276 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(57,&yymsp[0].minor);
}
// 3515 "parser.c"
        break;
      case 88:
// 1280 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(58,&yymsp[0].minor);
}
// 3523 "parser.c"
        break;
      case 89:
// 1284 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(59,&yymsp[0].minor);
}
// 3531 "parser.c"
        break;
      case 90:
// 1288 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(60,&yymsp[0].minor);
}
// 3539 "parser.c"
        break;
      case 91:
// 1292 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(61,&yymsp[0].minor);
}
// 3547 "parser.c"
        break;
      case 113:
// 1380 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 3555 "parser.c"
        break;
      case 114:
// 1384 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(62,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3564 "parser.c"
        break;
      case 115:
// 1388 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3573 "parser.c"
        break;
      case 116:
// 1392 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-2].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3583 "parser.c"
        break;
      case 117:
// 1396 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-5].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3596 "parser.c"
        break;
      case 118:
// 1400 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3606 "parser.c"
        break;
      case 119:
// 1404 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-7].minor.yy7, yymsp[-5].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(64,&yymsp[-8].minor);
  yy_destructor(42,&yymsp[-6].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3619 "parser.c"
        break;
      case 120:
// 1408 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-6].minor.yy7, yymsp[-4].minor.yy7, NULL, status->scanner_state);
  yy_destructor(64,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3632 "parser.c"
        break;
      case 121:
// 1412 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_if_statement(yymsp[-6].minor.yy7, NULL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(64,&yymsp[-7].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3645 "parser.c"
        break;
      case 122:
// 1416 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_switch_statement(yymsp[-2].minor.yy7, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3655 "parser.c"
        break;
      case 123:
// 1420 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_switch_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3665 "parser.c"
        break;
      case 126:
// 1432 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_case_clause(yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[0].minor);
}
// 3674 "parser.c"
        break;
      case 127:
// 1436 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_case_clause(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(67,&yymsp[-3].minor);
  yy_destructor(68,&yymsp[-1].minor);
}
// 3683 "parser.c"
        break;
      case 128:
// 1440 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_case_clause(NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(69,&yymsp[-2].minor);
  yy_destructor(68,&yymsp[-1].minor);
}
// 3692 "parser.c"
        break;
      case 129:
// 1444 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3702 "parser.c"
        break;
      case 130:
// 1448 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_loop_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(70,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3712 "parser.c"
        break;
      case 131:
// 1452 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_while_statement(yymsp[-2].minor.yy7, NULL, status->scanner_state);
  yy_destructor(71,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[-1].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3722 "parser.c"
        break;
      case 132:
// 1456 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_while_statement(yymsp[-3].minor.yy7, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(71,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3732 "parser.c"
        break;
      case 133:
// 1460 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_do_while_statement(yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(72,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3744 "parser.c"
        break;
      case 134:
// 1464 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_do_while_statement(yymsp[-1].minor.yy7, yymsp[-4].minor.yy7, status->scanner_state);
  yy_destructor(72,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3756 "parser.c"
        break;
      case 135:
// 1468 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3767 "parser.c"
        break;
      case 136:
// 1472 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3779 "parser.c"
        break;
      case 137:
// 1476 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(73,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(74,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3791 "parser.c"
        break;
      case 138:
// 1480 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_for_statement(yymsp[-3].minor.yy7, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(73,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(74,&yymsp[-5].minor);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(42,&yymsp[-2].minor);
  yy_destructor(43,&yymsp[0].minor);
}
// 3804 "parser.c"
        break;
      case 139:
// 1484 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3813 "parser.c"
        break;
      case 142:
// 1497 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("assign");
  yy_destructor(45,&yymsp[0].minor);
}
// 3821 "parser.c"
        break;
      case 143:
// 1502 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("add-assign");
  yy_destructor(77,&yymsp[0].minor);
}
// 3829 "parser.c"
        break;
      case 144:
// 1507 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("sub-assign");
  yy_destructor(78,&yymsp[0].minor);
}
// 3837 "parser.c"
        break;
      case 145:
// 1511 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("mult-assign");
  yy_destructor(79,&yymsp[0].minor);
}
// 3845 "parser.c"
        break;
      case 146:
// 1515 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("div-assign");
  yy_destructor(80,&yymsp[0].minor);
}
// 3853 "parser.c"
        break;
      case 147:
// 1519 "parser.lemon"
{
	yygotominor.yy7 = json_object_new_string("concat-assign");
  yy_destructor(81,&yymsp[0].minor);
}
// 3861 "parser.c"
        break;
      case 148:
// 1524 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy7, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 3868 "parser.c"
        break;
      case 149:
// 1529 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy7, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
}
// 3876 "parser.c"
        break;
      case 150:
// 1534 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy7, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3886 "parser.c"
        break;
      case 151:
// 1539 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy7, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3896 "parser.c"
        break;
      case 152:
// 1544 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy7, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3906 "parser.c"
        break;
      case 153:
// 1549 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy7, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3916 "parser.c"
        break;
      case 154:
// 1554 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy7, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
}
// 3924 "parser.c"
        break;
      case 155:
// 1559 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy7, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(86,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3934 "parser.c"
        break;
      case 156:
// 1564 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy7, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(86,&yymsp[-6].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3944 "parser.c"
        break;
      case 157:
// 1569 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy7, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(84,&yymsp[-3].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3953 "parser.c"
        break;
      case 158:
// 1574 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy7, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(85,&yymsp[-2].minor);
}
// 3962 "parser.c"
        break;
      case 159:
// 1579 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(87,&yymsp[0].minor);
}
// 3971 "parser.c"
        break;
      case 160:
// 1584 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-2].minor);
  yy_destructor(88,&yymsp[0].minor);
}
// 3980 "parser.c"
        break;
      case 161:
// 1589 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(87,&yymsp[0].minor);
}
// 3988 "parser.c"
        break;
      case 162:
// 1594 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(88,&yymsp[0].minor);
}
// 3996 "parser.c"
        break;
      case 164:
// 1602 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_echo_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(89,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4005 "parser.c"
        break;
      case 167:
// 1614 "parser.lemon"
{
	yygotominor.yy7 = yymsp[0].minor.yy7;;
}
// 4012 "parser.c"
        break;
      case 168:
// 1619 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4020 "parser.c"
        break;
      case 169:
// 1624 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4028 "parser.c"
        break;
      case 170:
// 1629 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_scall_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(35,&yymsp[0].minor);
}
// 4036 "parser.c"
        break;
      case 171:
// 1634 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_return_statement(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(90,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4045 "parser.c"
        break;
      case 172:
// 1639 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(90,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4054 "parser.c"
        break;
      case 173:
// 1644 "parser.lemon"
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
// 4068 "parser.c"
        break;
      case 174:
// 1653 "parser.lemon"
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
// 4083 "parser.c"
        break;
      case 175:
// 1662 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_throw_exception(yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(92,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4092 "parser.c"
        break;
      case 176:
// 1666 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4101 "parser.c"
        break;
      case 177:
// 1670 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4110 "parser.c"
        break;
      case 178:
// 1674 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(55,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4119 "parser.c"
        break;
      case 179:
// 1678 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4128 "parser.c"
        break;
      case 180:
// 1682 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4137 "parser.c"
        break;
      case 181:
// 1686 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4146 "parser.c"
        break;
      case 182:
// 1690 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(57,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4155 "parser.c"
        break;
      case 183:
// 1694 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(59,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4164 "parser.c"
        break;
      case 184:
// 1698 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(58,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4173 "parser.c"
        break;
      case 185:
// 1702 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 4182 "parser.c"
        break;
      case 188:
// 1714 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 4189 "parser.c"
        break;
      case 189:
// 1718 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(45,&yymsp[-1].minor);
}
// 4197 "parser.c"
        break;
      case 191:
// 1726 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 4205 "parser.c"
        break;
      case 192:
// 1730 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("isset", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4213 "parser.c"
        break;
      case 193:
// 1734 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("require", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4221 "parser.c"
        break;
      case 194:
// 1738 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("clone", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4229 "parser.c"
        break;
      case 195:
// 1742 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("empty", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 4237 "parser.c"
        break;
      case 196:
// 1746 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("equals", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 4245 "parser.c"
        break;
      case 197:
// 1750 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not-equals", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 4253 "parser.c"
        break;
      case 198:
// 1754 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("identical", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 4261 "parser.c"
        break;
      case 199:
// 1758 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("not-identical", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4269 "parser.c"
        break;
      case 200:
// 1762 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("less", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 4277 "parser.c"
        break;
      case 201:
// 1766 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("greater", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4285 "parser.c"
        break;
      case 202:
// 1770 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("less-equal", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 4293 "parser.c"
        break;
      case 203:
// 1774 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 4301 "parser.c"
        break;
      case 204:
// 1778 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("list", yymsp[-1].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4310 "parser.c"
        break;
      case 205:
// 1782 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("cast", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-3].minor);
  yy_destructor(32,&yymsp[-1].minor);
}
// 4319 "parser.c"
        break;
      case 206:
// 1786 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-3].minor);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4328 "parser.c"
        break;
      case 207:
      case 269:
// 1790 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-1].minor);
}
// 4337 "parser.c"
        break;
      case 208:
// 1794 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4345 "parser.c"
        break;
      case 209:
      case 267:
// 1798 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-1].minor);
}
// 4354 "parser.c"
        break;
      case 210:
      case 268:
// 1803 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy7, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4364 "parser.c"
        break;
      case 211:
// 1808 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("add", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 4372 "parser.c"
        break;
      case 212:
// 1813 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("sub", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 4380 "parser.c"
        break;
      case 213:
// 1818 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("mul", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 4388 "parser.c"
        break;
      case 214:
// 1823 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("div", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4396 "parser.c"
        break;
      case 215:
// 1828 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("mod", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4404 "parser.c"
        break;
      case 216:
// 1833 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("concat", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 4412 "parser.c"
        break;
      case 217:
// 1838 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("and", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 4420 "parser.c"
        break;
      case 218:
// 1843 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("or", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 4428 "parser.c"
        break;
      case 219:
// 1848 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("instanceof", yymsp[-2].minor.yy7, yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 4436 "parser.c"
        break;
      case 220:
// 1853 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy7, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4445 "parser.c"
        break;
      case 221:
// 1858 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("typeof", yymsp[0].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 4453 "parser.c"
        break;
      case 222:
// 1863 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4460 "parser.c"
        break;
      case 224:
      case 258:
      case 260:
// 1873 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4469 "parser.c"
        break;
      case 225:
      case 257:
      case 262:
// 1878 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4478 "parser.c"
        break;
      case 226:
      case 261:
// 1883 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4486 "parser.c"
        break;
      case 227:
      case 263:
// 1888 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4494 "parser.c"
        break;
      case 228:
      case 264:
// 1893 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(96,&yymsp[0].minor);
}
// 4503 "parser.c"
        break;
      case 229:
      case 266:
// 1898 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(97,&yymsp[0].minor);
}
// 4512 "parser.c"
        break;
      case 230:
      case 265:
// 1903 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(98,&yymsp[0].minor);
}
// 4521 "parser.c"
        break;
      case 231:
// 1908 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4530 "parser.c"
        break;
      case 232:
// 1913 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_expr("array", yymsp[-1].minor.yy7, NULL, NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(85,&yymsp[0].minor);
}
// 4539 "parser.c"
        break;
      case 233:
// 1918 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 4547 "parser.c"
        break;
      case 234:
// 1923 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4557 "parser.c"
        break;
      case 235:
// 1928 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(30,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4567 "parser.c"
        break;
      case 236:
// 1933 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4576 "parser.c"
        break;
      case 237:
// 1938 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4585 "parser.c"
        break;
      case 238:
// 1943 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(86,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4595 "parser.c"
        break;
      case 239:
// 1948 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(86,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4605 "parser.c"
        break;
      case 240:
// 1953 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(1, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4615 "parser.c"
        break;
      case 241:
// 1958 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(1, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4625 "parser.c"
        break;
      case 242:
// 1963 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(2, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4637 "parser.c"
        break;
      case 243:
// 1968 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(2, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4649 "parser.c"
        break;
      case 244:
// 1973 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(3, yymsp[-7].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy7, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(42,&yymsp[-5].minor);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4661 "parser.c"
        break;
      case 245:
// 1978 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_mcall(3, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[-5].minor);
  yy_destructor(42,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 4673 "parser.c"
        break;
      case 254:
// 2017 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_array_item(yymsp[-2].minor.yy7, yymsp[0].minor.yy7, status->scanner_state);
  yy_destructor(68,&yymsp[-1].minor);
}
// 4681 "parser.c"
        break;
      case 255:
// 2021 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_array_item(NULL, yymsp[0].minor.yy7, status->scanner_state);
}
// 4688 "parser.c"
        break;
      case 271:
// 2086 "parser.lemon"
{
	yygotominor.yy7 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4695 "parser.c"
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
// 879 "parser.lemon"


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

// 4771 "parser.c"
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
