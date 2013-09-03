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
#define YYNSTATE 493
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
 /*     0 */    89,  154,  158,  152,  190,   86,  311,  318,  147,  733,
 /*    10 */     1,    2,  492,    4,    5,    6,   93,   12,  160,  473,
 /*    20 */   130,  320,  318,  131,  132,  133,  134,  135,  136,  137,
 /*    30 */   138,  139,  140,  141,  247,  126,  128,   88,  102,  106,
 /*    40 */   108,  110,  112,  104,  100,  114,  116,  124,  118,  120,
 /*    50 */   122,  436,   17,  451,  459,  168,  465,  439,  346,  172,
 /*    60 */   435,  313,  374,  161,  162,  351,  163,  164,  165,  166,
 /*    70 */   167,   95,   97,  310,  319,  328,  331,  322,  325,  334,
 /*    80 */   340,  337,  151,  343,   46,   48,   50,  312,   59,  205,
 /*    90 */   197,  198,   70,   74,   79,  219,  194,  200,  226,  210,
 /*   100 */    19,  346,  482,   18,  486,   27,  435,  213,  351,  494,
 /*   110 */   277,  288,  292,    8,  313,  307,  310,  319,  328,  331,
 /*   120 */   322,  325,  334,  340,  337,  253,  343,   46,   48,   50,
 /*   130 */   270,   59,  171,  197,  198,   70,   74,   79,  219,    7,
 /*   140 */   321,  226,   10,  461,  346,  177,  479,  351,  390,    7,
 /*   150 */   462,  351,   10,  277,  288,  292,  479,  351,  307,  310,
 /*   160 */   319,  328,  331,  322,  325,  334,  340,  337,  151,  343,
 /*   170 */    46,   48,   50,  493,   59,    9,  197,  198,   70,   74,
 /*   180 */    79,  219,  207,  200,  226,   14,  177,  346,   11,   64,
 /*   190 */   410,  392,  176,  420,  351,   21,  277,  288,  292,  435,
 /*   200 */    24,  307,  310,  319,  328,  331,  322,  325,  334,  340,
 /*   210 */   337,  151,  343,   46,   48,   50,   66,   59,  376,  197,
 /*   220 */   198,   70,   74,   79,  219,  215,  200,  226,  170,  177,
 /*   230 */   346,  189,  174,  185,  225,  323,  318,  351,  393,  277,
 /*   240 */   288,  292,  326,  318,  307,  310,  319,  328,  331,  322,
 /*   250 */   325,  334,  340,  337,  151,  343,   46,   48,   50,  298,
 /*   260 */    59,  388,  197,  198,   70,   74,   79,  219,  201,  200,
 /*   270 */   226,  329,  318,  346,  173,  174,  185,  426,  332,  318,
 /*   280 */   351,  196,  277,  288,  292,  313,   22,  307,  310,  319,
 /*   290 */   328,  331,  322,  325,  334,  340,  337,  151,  343,   46,
 /*   300 */    48,   50,  358,   59,  195,  197,  198,   70,   74,   79,
 /*   310 */   219,  333,  199,  226,  301,  304,  346,  118,  120,  122,
 /*   320 */   428,  313,  455,  351,   47,  277,  288,  292,   62,  456,
 /*   330 */   307,  310,  319,  328,  331,  322,  325,  334,  340,  337,
 /*   340 */   253,  343,   46,   48,   50,  249,   59,  324,  197,  198,
 /*   350 */    70,   74,   79,  219,  453,   66,  226,  376,   49,  346,
 /*   360 */   435,  144,  313,  432,  313,  221,  351,   52,  277,  288,
 /*   370 */   292,  335,  318,  307,  310,  319,  328,  331,  322,  325,
 /*   380 */   334,  340,  337,  142,  343,   46,   48,   50,  327,   59,
 /*   390 */   330,  197,  198,   70,   74,   79,  219,  227,  276,  226,
 /*   400 */    63,  379,  346,  338,  318,   55,  434,  341,  318,  351,
 /*   410 */    56,  277,  288,  292,  313,   61,  307,  310,  319,  328,
 /*   420 */   331,  322,  325,  334,  340,  337,  282,  343,   46,   48,
 /*   430 */    50,  313,   59,  281,  197,  198,   70,   74,   79,  219,
 /*   440 */   342,  349,  226,  387,  347,  346,  409,  398,  402,   25,
 /*   450 */    65,  196,  351,  196,  277,  288,  292,  336,  229,  307,
 /*   460 */   310,  319,  328,  331,  322,  325,  334,  340,  337,   67,
 /*   470 */   343,   46,   48,   50,  216,   59,  202,  196,  253,   70,
 /*   480 */    74,   79,  219,  262,  228,  226,  197,  198,  346,  344,
 /*   490 */   318,   68,   78,  313,   71,  351,   76,  277,  288,  292,
 /*   500 */   208,   80,  307,  310,  319,  328,  331,  322,  325,  334,
 /*   510 */   340,  337,  354,  343,   46,   48,   50,  313,   59,  339,
 /*   520 */   197,  198,   70,   74,   79,  219,  294,  297,  226,  429,
 /*   530 */    83,  346,  409,  398,  402,   73,  280,   85,  351,   91,
 /*   540 */   277,  288,  292,  345,   90,  307,  310,  319,  328,  331,
 /*   550 */   322,  325,  334,  340,  337,   92,  343,   46,   48,   50,
 /*   560 */    94,   59,  279,  393,  361,   70,   74,   79,  219,   96,
 /*   570 */    98,  226,  197,  198,  346,  145,  149,  150,  375,  155,
 /*   580 */   156,  351,  157,  277,  288,  292,  430,  175,  307,  310,
 /*   590 */   319,  328,  331,  322,  325,  334,  340,  337,   87,  343,
 /*   600 */    46,   48,   50,  716,   59,  717,  197,  198,   70,   74,
 /*   610 */    79,  219,  718,  191,  226,  192,  206,  346,  220,  231,
 /*   620 */   238,  357,  230,  242,  351,  243,  277,  288,  292,  250,
 /*   630 */   255,  307,  310,  319,  328,  331,  322,  325,  334,  340,
 /*   640 */   337,   99,  343,   46,   48,   50,  251,   59,  263,  197,
 /*   650 */   198,   70,   74,   79,  219,  256,  260,  226,  264,  268,
 /*   660 */   346,  272,  271,  285,   82,  314,  287,  351,  293,  277,
 /*   670 */   288,  292,  295,  299,  307,  310,  319,  328,  331,  322,
 /*   680 */   325,  334,  340,  337,  101,  343,   46,   48,   50,  296,
 /*   690 */    59,  300,  197,  198,   70,   74,   79,  219,  302,  303,
 /*   700 */   226,  305,  306,  346,  315,  316,  317,  364,  348,  350,
 /*   710 */   351,  359,  277,  288,  292,  360,  371,  307,  310,  319,
 /*   720 */   328,  331,  322,  325,  334,  340,  337,  366,  343,   46,
 /*   730 */    48,   50,  373,   59,  377,  197,  198,   70,   74,   79,
 /*   740 */   219,  382,  383,  226,  389,  396,  346,  397,  399,  403,
 /*   750 */    58,  400,  401,  351,  407,  277,  288,  292,  408,  404,
 /*   760 */   307,  310,  319,  328,  331,  322,  325,  334,  340,  337,
 /*   770 */   103,  343,   46,   48,   50,  556,   59,  405,  197,  198,
 /*   780 */    70,   74,   79,  219,  422,  423,  226,  443,  425,  346,
 /*   790 */   431,  442,  444,  370,  448,  445,  351,  449,  277,  288,
 /*   800 */   292,  447,  450,  307,  310,  319,  328,  331,  322,  325,
 /*   810 */   334,  340,  337,  105,  343,   46,   48,   50,  458,   59,
 /*   820 */   464,  197,  198,   70,   74,   79,  219,  474,  475,  226,
 /*   830 */   457,  480,  346,  463,  477,  481,  380,  484,  487,  351,
 /*   840 */   488,  277,  288,  292,  490,  397,  307,  310,  319,  328,
 /*   850 */   331,  322,  325,  334,  340,  337,  308,  343,   46,   48,
 /*   860 */    50,  397,   59,  397,  197,  198,   70,   74,   79,  219,
 /*   870 */   397,  397,  226,  397,  397,  346,  397,  397,  397,   54,
 /*   880 */   397,  397,  351,  397,  277,  288,  292,  397,  397,  307,
 /*   890 */   310,  319,  328,  331,  322,  325,  334,  340,  337,  107,
 /*   900 */   343,   46,   48,   50,  397,   59,  397,  197,  198,   70,
 /*   910 */    74,   79,  219,  397,  397,  226,  397,  397,  346,  397,
 /*   920 */   397,  397,  381,  397,  397,  351,  397,  277,  288,  292,
 /*   930 */   397,  397,  307,  310,  319,  328,  331,  322,  325,  334,
 /*   940 */   340,  337,  109,  343,   46,   48,   50,  397,   59,  397,
 /*   950 */   197,  198,   70,   74,   79,  219,  397,  397,  226,  397,
 /*   960 */   397,  346,  394,  398,  402,  369,  397,  397,  351,  397,
 /*   970 */   277,  288,  292,  397,  397,  307,  310,  319,  328,  331,
 /*   980 */   322,  325,  334,  340,  337,  397,  343,   46,   48,   50,
 /*   990 */   397,   59,  397,  397,  111,   70,   74,   79,  219,  397,
 /*  1000 */   397,  226,  197,  198,  346,  397,  397,  397,  386,  397,
 /*  1010 */   397,  351,  397,  277,  288,  292,  397,  397,  307,  310,
 /*  1020 */   319,  328,  331,  322,  325,  334,  340,  337,  113,  343,
 /*  1030 */    46,   48,   50,  397,   59,  397,  197,  198,   70,   74,
 /*  1040 */    79,  219,  397,  397,  226,  397,  397,  346,  397,  397,
 /*  1050 */   397,  385,  397,  397,  351,  397,  277,  288,  292,  397,
 /*  1060 */   397,  307,  310,  319,  328,  331,  322,  325,  334,  340,
 /*  1070 */   337,  115,  343,   46,   48,   50,  397,   59,  397,  197,
 /*  1080 */   198,   70,   74,   79,  219,  397,  397,  226,  397,  397,
 /*  1090 */   346,  397,  411,  412,  414,  413,  415,  351,  397,  277,
 /*  1100 */   288,  292,  397,  397,  307,  310,  319,  328,  331,  322,
 /*  1110 */   325,  334,  340,  337,  397,  343,   46,   48,   50,  397,
 /*  1120 */    59,  397,  460,  397,   70,   74,   79,  219,  417,  397,
 /*  1130 */   226,  397,  397,   20,   45,  416,  418,  232,    3,    4,
 /*  1140 */     5,    6,  277,  288,  292,   57,  397,  307,  186,  397,
 /*  1150 */   352,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1160 */    38,   39,   40,   41,   42,   43,   44,  397,  397,   45,
 /*  1170 */   233,  234,  235,  236,  237,  289,  245,  397,  284,  286,
 /*  1180 */    77,  397,  397,  197,  198,  352,   29,   30,   31,   32,
 /*  1190 */    33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
 /*  1200 */    43,   44,  397,  188,  187,  397,   45,  181,  182,  184,
 /*  1210 */   183,  397,  282,  284,  286,  397,  397,   26,  278,  283,
 /*  1220 */   197,  198,  352,   29,   30,   31,   32,   33,   34,   35,
 /*  1230 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  467,
 /*  1240 */   397,  470,  471,  459,   45,  438,  439,  397,  232,  435,
 /*  1250 */   284,  286,  397,  397,  397,  433,  397,  397,  178,  397,
 /*  1260 */   352,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1270 */    38,   39,   40,   41,   42,   43,   44,  397,  397,  397,
 /*  1280 */    45,  233,  234,  235,  236,  237,  240,  258,  284,  286,
 /*  1290 */   244,  427,  397,  214,  197,  198,  352,   29,   30,   31,
 /*  1300 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1310 */    42,   43,   44,  179,  180,   45,  397,  181,  182,  184,
 /*  1320 */   183,  397,  397,  397,  284,  286,  391,  148,  211,  397,
 /*  1330 */   204,  352,   29,   30,   31,   32,   33,   34,   35,   36,
 /*  1340 */    37,   38,   39,   40,   41,   42,   43,   44,  397,  397,
 /*  1350 */   397,  397,   45,  114,  116,  124,  118,  120,  122,  284,
 /*  1360 */   286,  397,  397,  384,  397,  397,  397,  397,  352,   29,
 /*  1370 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1380 */    40,   41,   42,   43,   44,  397,  397,  179,  180,  397,
 /*  1390 */    45,  181,  182,  184,  183,  232,  284,  286,  397,  397,
 /*  1400 */   397,  224,  397,  397,  397,  397,  352,   29,   30,   31,
 /*  1410 */    32,   33,   34,   35,   36,   37,   38,   39,   40,   41,
 /*  1420 */    42,   43,   44,  397,  397,  397,   45,  397,  233,  234,
 /*  1430 */   235,  236,  237,  397,  284,  286,  397,  378,  397,  397,
 /*  1440 */   397,  397,  352,   29,   30,   31,   32,   33,   34,   35,
 /*  1450 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  397,
 /*  1460 */   437,   45,   19,   51,  438,  439,  218,  397,  435,  397,
 /*  1470 */   284,  286,  368,  397,  197,  198,  397,  352,   29,   30,
 /*  1480 */    31,   32,   33,   34,   35,   36,   37,   38,   39,   40,
 /*  1490 */    41,   42,   43,   44,  468,  451,  459,  397,   45,  439,
 /*  1500 */   397,  397,  435,  397,  397,  284,  286,  397,  397,   53,
 /*  1510 */   397,  397,  397,  397,  352,   29,   30,   31,   32,   33,
 /*  1520 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1530 */    44,  240,  397,  397,  397,  239,   45,  397,  397,  197,
 /*  1540 */   198,  397,  284,  286,  397,  397,  397,  363,  397,  397,
 /*  1550 */   397,  397,  352,   29,   30,   31,   32,   33,   34,   35,
 /*  1560 */    36,   37,   38,   39,   40,   41,   42,   43,   44,  240,
 /*  1570 */   397,  397,   45,  269,  397,  397,  397,  197,  198,  397,
 /*  1580 */   284,  286,  397,   69,  397,  397,  397,  397,  352,   29,
 /*  1590 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1600 */    40,   41,   42,   43,   44,  397,  240,   45,  397,  372,
 /*  1610 */   252,  397,  218,  397,  197,  198,  284,  286,   72,  397,
 /*  1620 */   197,  198,  397,  352,   29,   30,   31,   32,   33,   34,
 /*  1630 */    35,   36,   37,   38,   39,   40,   41,   42,   43,   44,
 /*  1640 */   117,  397,  397,  397,   45,  397,  397,  397,  197,  198,
 /*  1650 */   397,  284,  286,  397,  397,  356,  397,  397,  397,  397,
 /*  1660 */   352,   29,   30,   31,   32,   33,   34,   35,   36,   37,
 /*  1670 */    38,   39,   40,   41,   42,   43,   44,  240,  397,  397,
 /*  1680 */   397,  273,   45,  397,  397,  197,  198,  397,  284,  286,
 /*  1690 */   397,  397,  397,   81,  397,  397,  397,  397,  352,   29,
 /*  1700 */    30,   31,   32,   33,   34,   35,   36,   37,   38,   39,
 /*  1710 */    40,   41,   42,   43,   44,  240,  397,  397,   45,  248,
 /*  1720 */   397,  240,  397,  197,  198,  257,  284,  286,  397,  197,
 /*  1730 */   198,  397,  397,  397,   28,   29,   30,   31,   32,   33,
 /*  1740 */    34,   35,   36,   37,   38,   39,   40,   41,   42,   43,
 /*  1750 */    44,   89,  154,  158,  152,  190,   86,  397,  397,  147,
 /*  1760 */   397,  397,  284,  286,  397,  397,  232,  397,  397,  160,
 /*  1770 */   397,  130,   89,  154,  158,  152,  190,   86,  203,  397,
 /*  1780 */   147,  397,  397,  397,  397,  397,  119,  397,  397,  397,
 /*  1790 */   160,  397,  130,  397,  197,  198,  397,  397,  353,  233,
 /*  1800 */   234,  235,  236,  237,  241,  266,  168,  254,  274,  275,
 /*  1810 */   397,  397,  397,  397,  161,  162,  397,  163,  164,  165,
 /*  1820 */   166,  167,  397,  397,  240,  397,  397,  168,  261,  397,
 /*  1830 */   397,  397,  197,  198,  397,  161,  162,  397,  163,  164,
 /*  1840 */   165,  166,  167,   89,  154,  158,  152,  190,   86,  209,
 /*  1850 */   397,  147,  397,  397,  397,  397,  397,  397,  397,   60,
 /*  1860 */   397,  160,  218,  130,   89,  154,  158,  152,  190,   86,
 /*  1870 */   197,  198,  147,  291,  397,  397,  397,  397,  397,  397,
 /*  1880 */    84,  397,  160,  218,  130,   89,  154,  158,  152,  190,
 /*  1890 */    86,  197,  198,  147,  397,  397,  397,  397,  168,  397,
 /*  1900 */   397,  397,  397,  160,  397,  130,  161,  162,  397,  163,
 /*  1910 */   164,  165,  166,  167,  240,  397,  121,  397,  265,  168,
 /*  1920 */   397,  397,  197,  198,  197,  198,  397,  161,  162,  397,
 /*  1930 */   163,  164,  165,  166,  167,   75,  123,  397,  218,  397,
 /*  1940 */   168,  246,  397,  397,  197,  198,  197,  198,  161,  162,
 /*  1950 */   397,  163,  164,  165,  166,  167,   89,  154,  158,  152,
 /*  1960 */   190,   86,  193,  397,  147,  397,  397,  397,  397,  397,
 /*  1970 */   397,  397,  397,  397,  160,  397,  130,   89,  154,  158,
 /*  1980 */   152,  190,   86,  125,  397,  147,  397,  397,  397,  397,
 /*  1990 */   397,  197,  198,  397,  397,  160,  397,  130,   89,  154,
 /*  2000 */   158,  152,  190,   86,  127,  397,  147,  397,  397,  397,
 /*  2010 */   397,  168,  197,  198,  397,  397,  160,  397,  130,  161,
 /*  2020 */   162,  129,  163,  164,  165,  166,  167,  146,  397,  197,
 /*  2030 */   198,  397,  168,  259,  397,  197,  198,  397,  397,  397,
 /*  2040 */   161,  162,  397,  163,  164,  165,  166,  167,  411,  412,
 /*  2050 */   414,  413,  415,  168,  267,  397,  397,  397,  397,  397,
 /*  2060 */   397,  161,  162,  397,  163,  164,  165,  166,  167,   89,
 /*  2070 */   154,  158,  152,  190,   86,  217,  397,  147,  397,  411,
 /*  2080 */   412,  414,  413,  415,  417,  397,  397,  160,  397,  130,
 /*  2090 */   397,  416,  418,   89,  154,  158,  152,  190,   86,  212,
 /*  2100 */   397,  147,  397,  397,  397,  397,  397,  197,  198,  454,
 /*  2110 */   397,  160,  397,  130,  397,  417,  397,  222,  397,  159,
 /*  2120 */   421,  153,  416,  418,  168,  197,  198,  197,  198,  197,
 /*  2130 */   198,  397,  161,  162,  397,  163,  164,  165,  166,  167,
 /*  2140 */   365,  397,  397,  397,  397,  397,  397,  397,  168,  397,
 /*  2150 */   397,  397,  397,  397,  397,  397,  161,  162,  397,  163,
 /*  2160 */   164,  165,  166,  167,   89,  154,  158,  152,  190,   86,
 /*  2170 */   397,  397,  147,  397,  397,  397,  397,  397,  397,  397,
 /*  2180 */   397,  397,  160,  397,  130,  397,  397,  397,  397,  397,
 /*  2190 */   126,  128,   88,  102,  106,  108,  110,  112,  104,  100,
 /*  2200 */   114,  116,  124,  118,  120,  122,  397,  397,  397,  397,
 /*  2210 */   397,  397,  397,  397,  397,  397,  397,  355,  397,  168,
 /*  2220 */   397,  411,  412,  414,  413,  415,  397,  161,  162,  397,
 /*  2230 */   163,  164,  165,  166,  167,  126,  128,   88,  102,  106,
 /*  2240 */   108,  110,  112,  104,  100,  114,  116,  124,  118,  120,
 /*  2250 */   122,  397,  397,  397,  397,  511,  397,  417,  419,  397,
 /*  2260 */   309,  397,  397,  397,  416,  418,  397,  397,  126,  128,
 /*  2270 */    88,  102,  106,  108,  110,  112,  104,  100,  114,  116,
 /*  2280 */   124,  118,  120,  122,  397,  397,  397,  397,  397,  397,
 /*  2290 */   397,  397,  397,  397,  397,  367,  397,  397,  397,  397,
 /*  2300 */   397,  397,  397,  126,  128,   88,  102,  106,  108,  110,
 /*  2310 */   112,  104,  100,  114,  116,  124,  118,  120,  122,  126,
 /*  2320 */   128,   88,  102,  106,  108,  110,  112,  104,  100,  114,
 /*  2330 */   116,  124,  118,  120,  122,  483,   16,  466,  472,  471,
 /*  2340 */   459,  397,  465,  439,  290,  397,  435,  397,  397,  397,
 /*  2350 */   397,  397,  126,  128,   88,  102,  106,  108,  110,  112,
 /*  2360 */   104,  100,  114,  116,  124,  118,  120,  122,  411,  412,
 /*  2370 */   414,  413,  415,  397,  143,  631,  397,  397,  397,  397,
 /*  2380 */   397,  397,  126,  128,   88,  102,  106,  108,  110,  112,
 /*  2390 */   104,  100,  114,  116,  124,  118,  120,  122,  397,  397,
 /*  2400 */   397,  397,  485,  397,  417,  452,  397,  446,  397,  223,
 /*  2410 */   397,  416,  418,  397,  397,  397,  397,  126,  128,   88,
 /*  2420 */   102,  106,  108,  110,  112,  104,  100,  114,  116,  124,
 /*  2430 */   118,  120,  122,  397,  476,   16,  466,  472,  471,  459,
 /*  2440 */   397,  465,  439,  397,  362,  435,  397,  397,  397,  397,
 /*  2450 */   397,  397,  126,  128,   88,  102,  106,  108,  110,  112,
 /*  2460 */   104,  100,  114,  116,  124,  118,  120,  122,  406,   88,
 /*  2470 */   102,  106,  108,  110,  112,  104,  100,  114,  116,  124,
 /*  2480 */   118,  120,  122,  397,  397,  397,   23,  397,  395,  489,
 /*  2490 */    16,  466,  472,  471,  459,  397,  465,  439,  406,  397,
 /*  2500 */   435,  397,  397,  131,  132,  133,  134,  135,  136,  137,
 /*  2510 */   138,  139,  140,  141,  397,  397,  424,  397,  395,  397,
 /*  2520 */   397,  397,  397,  397,  406,  397,  397,  397,  397,  397,
 /*  2530 */   397,  397,  397,  131,  132,  133,  134,  135,  136,  137,
 /*  2540 */   138,  139,  140,  141,  395,  411,  412,  414,  413,  415,
 /*  2550 */   397,  397,  397,  397,  397,  397,  397,  397,  397,  131,
 /*  2560 */   132,  133,  134,  135,  136,  137,  138,  139,  140,  141,
 /*  2570 */   411,  412,  414,  413,  415,  397,  397,  397,  397,  478,
 /*  2580 */   397,  417,  452,  397,  446,  397,  397,  397,  416,  418,
 /*  2590 */   411,  412,  414,  413,  415,  397,  397,  397,  397,  397,
 /*  2600 */   397,  397,  397,  397,  491,  397,  417,  452,  397,  446,
 /*  2610 */   397,  397,  397,  416,  418,  411,  412,  414,  413,  415,
 /*  2620 */   397,  397,  397,  397,  513,  397,  417,  440,  397,  446,
 /*  2630 */   397,  397,  397,  416,  418,  411,  412,  414,  413,  415,
 /*  2640 */   397,  397,  397,  397,  397,  397,  397,  397,  397,  510,
 /*  2650 */   397,  417,  452,  397,  446,  397,  397,  397,  416,  418,
 /*  2660 */   411,  412,  414,  413,  415,  397,  397,  397,  397,   15,
 /*  2670 */   397,  417,  452,  397,  446,  397,  397,  397,  416,  418,
 /*  2680 */   411,  412,  414,  413,  415,  411,  412,  414,  413,  415,
 /*  2690 */   397,  397,  397,  397,  509,  397,  417,  452,  397,  446,
 /*  2700 */   397,  397,  397,  416,  418,  411,  412,  414,  413,  415,
 /*  2710 */   397,  397,  397,  397,  517,  397,  417,  419,  397,  516,
 /*  2720 */   397,  417,  419,  416,  418,  397,  397,  397,  416,  418,
 /*  2730 */   411,  412,  414,  413,  415,  397,  397,  397,  397,  515,
 /*  2740 */   186,  417,  419,  411,  412,  414,  413,  415,  416,  418,
 /*  2750 */   397,  397,   13,   16,  466,  472,  471,  459,  397,  465,
 /*  2760 */   439,  397,  397,  435,  512,  397,  417,  419,  411,  412,
 /*  2770 */   414,  413,  415,  416,  418,  397,  397,  514,  397,  417,
 /*  2780 */   469,  411,  412,  414,  413,  415,  416,  418,  169,  397,
 /*  2790 */   397,  397,  397,  397,  397,  188,  187,  397,  397,  181,
 /*  2800 */   182,  184,  183,  397,  417,  397,  397,  397,  397,   20,
 /*  2810 */   397,  416,  418,  397,  397,  397,  397,  417,  397,  397,
 /*  2820 */   441,  397,  397,  397,  416,  418,  411,  412,  414,  413,
 /*  2830 */   415,  397,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  2840 */   397,  397,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  2850 */   397,  397,  397,  397,  397,  397,  397,  397,  397,  397,
 /*  2860 */   397,  397,  417,  397,  397,  397,  397,  421,  397,  416,
 /*  2870 */   418,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */    23,   24,   25,   26,   27,   28,  147,  148,   31,   95,
 /*    10 */    96,   97,   98,   99,  100,  101,   31,   34,   41,   36,
 /*    20 */    43,  147,  148,   46,   47,   48,   49,   50,   51,   52,
 /*    30 */    53,   54,   55,   56,  140,    7,    8,    9,   10,   11,
 /*    40 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*    50 */    22,  104,  105,  106,  107,   78,  109,  110,   31,    6,
 /*    60 */   113,    6,   35,   86,   87,   38,   89,   90,   91,   92,
 /*    70 */    93,   86,   87,   46,   47,   48,   49,   50,   51,   52,
 /*    80 */    53,   54,  137,   56,   57,   58,   59,   32,   61,   31,
 /*    90 */   145,  146,   65,   66,   67,   68,  151,  152,   71,   41,
 /*   100 */   107,   31,   34,  110,   36,   35,  113,   79,   38,    0,
 /*   110 */    83,   84,   85,   31,    6,   88,   46,   47,   48,   49,
 /*   120 */    50,   51,   52,   53,   54,  137,   56,   57,   58,   59,
 /*   130 */   142,   61,   79,  145,  146,   65,   66,   67,   68,   30,
 /*   140 */    32,   71,   33,   32,   31,  108,   37,   38,   35,   30,
 /*   150 */    39,   38,   33,   83,   84,   85,   37,   38,   88,   46,
 /*   160 */    47,   48,   49,   50,   51,   52,   53,   54,  137,   56,
 /*   170 */    57,   58,   59,    0,   61,   32,  145,  146,   65,   66,
 /*   180 */    67,   68,  151,  152,   71,   35,  108,   31,   31,   35,
 /*   190 */   113,   35,  155,  107,   38,   31,   83,   84,   85,  113,
 /*   200 */    34,   88,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   210 */    54,  137,   56,   57,   58,   59,   62,   61,   64,  145,
 /*   220 */   146,   65,   66,   67,   68,  151,  152,   71,  150,  108,
 /*   230 */    31,  153,  154,  155,   35,  147,  148,   38,    6,   83,
 /*   240 */    84,   85,  147,  148,   88,   46,   47,   48,   49,   50,
 /*   250 */    51,   52,   53,   54,  137,   56,   57,   58,   59,   31,
 /*   260 */    61,   29,  145,  146,   65,   66,   67,   68,  151,  152,
 /*   270 */    71,  147,  148,   31,  153,  154,  155,   35,  147,  148,
 /*   280 */    38,    6,   83,   84,   85,    6,   43,   88,   46,   47,
 /*   290 */    48,   49,   50,   51,   52,   53,   54,  137,   56,   57,
 /*   300 */    58,   59,    6,   61,   29,  145,  146,   65,   66,   67,
 /*   310 */    68,   32,  152,   71,   86,   87,   31,   20,   21,   22,
 /*   320 */    35,    6,   32,   38,   32,   83,   84,   85,   35,   39,
 /*   330 */    88,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   340 */   137,   56,   57,   58,   59,  142,   61,   32,  145,  146,
 /*   350 */    65,   66,   67,   68,  107,   62,   71,   64,   32,   31,
 /*   360 */   113,  115,    6,   35,    6,   69,   38,   34,   83,   84,
 /*   370 */    85,  147,  148,   88,   46,   47,   48,   49,   50,   51,
 /*   380 */    52,   53,   54,  137,   56,   57,   58,   59,   32,   61,
 /*   390 */    32,  145,  146,   65,   66,   67,   68,  138,  139,   71,
 /*   400 */   135,  136,   31,  147,  148,   60,   35,  147,  148,   38,
 /*   410 */    34,   83,   84,   85,    6,   34,   88,   46,   47,   48,
 /*   420 */    49,   50,   51,   52,   53,   54,  137,   56,   57,   58,
 /*   430 */    59,    6,   61,  144,  145,  146,   65,   66,   67,   68,
 /*   440 */    32,   77,   71,  111,   80,   31,  114,  115,  116,   35,
 /*   450 */   136,    6,   38,    6,   83,   84,   85,   32,    6,   88,
 /*   460 */    46,   47,   48,   49,   50,   51,   52,   53,   54,  108,
 /*   470 */    56,   57,   58,   59,   29,   61,   29,    6,  137,   65,
 /*   480 */    66,   67,   68,  142,   32,   71,  145,  146,   31,  147,
 /*   490 */   148,   63,   35,    6,   34,   38,   34,   83,   84,   85,
 /*   500 */    29,   34,   88,   46,   47,   48,   49,   50,   51,   52,
 /*   510 */    53,   54,  137,   56,   57,   58,   59,    6,   61,   32,
 /*   520 */   145,  146,   65,   66,   67,   68,   77,   78,   71,  111,
 /*   530 */    66,   31,  114,  115,  116,   35,    6,   32,   38,   31,
 /*   540 */    83,   84,   85,   32,  149,   88,   46,   47,   48,   49,
 /*   550 */    50,   51,   52,   53,   54,   78,   56,   57,   58,   59,
 /*   560 */    79,   61,   32,    6,  137,   65,   66,   67,   68,   79,
 /*   570 */    79,   71,  145,  146,   31,   29,   31,   43,   35,   31,
 /*   580 */     6,   38,  149,   83,   84,   85,   29,   63,   88,   46,
 /*   590 */    47,   48,   49,   50,   51,   52,   53,   54,  137,   56,
 /*   600 */    57,   58,   59,   63,   61,   63,  145,  146,   65,   66,
 /*   610 */    67,   68,   63,   31,   71,   43,   43,   31,   31,   31,
 /*   620 */   140,   35,  139,   31,   38,  140,   83,   84,   85,   79,
 /*   630 */    31,   88,   46,   47,   48,   49,   50,   51,   52,   53,
 /*   640 */    54,  137,   56,   57,   58,   59,  140,   61,   79,  145,
 /*   650 */   146,   65,   66,   67,   68,  140,  140,   71,  140,  140,
 /*   660 */    31,  140,   79,   32,   35,  148,   32,   38,   31,   83,
 /*   670 */    84,   85,   31,   79,   88,   46,   47,   48,   49,   50,
 /*   680 */    51,   52,   53,   54,  137,   56,   57,   58,   59,   32,
 /*   690 */    61,   32,  145,  146,   65,   66,   67,   68,   79,   32,
 /*   700 */    71,   79,   32,   31,   31,   39,  108,   35,   31,   31,
 /*   710 */    38,   31,   83,   84,   85,   69,   66,   88,   46,   47,
 /*   720 */    48,   49,   50,   51,   52,   53,   54,  137,   56,   57,
 /*   730 */    58,   59,   32,   61,   63,  145,  146,   65,   66,   67,
 /*   740 */    68,   60,   34,   71,   34,   39,   31,  108,   31,   31,
 /*   750 */    35,   39,  108,   38,   31,   83,   84,   85,   12,   39,
 /*   760 */    88,   46,   47,   48,   49,   50,   51,   52,   53,   54,
 /*   770 */   137,   56,   57,   58,   59,   31,   61,  108,  145,  146,
 /*   780 */    65,   66,   67,   68,   31,   43,   71,   39,   34,   31,
 /*   790 */    34,   41,  108,   35,   39,   32,   38,  108,   83,   84,
 /*   800 */    85,   41,   32,   88,   46,   47,   48,   49,   50,   51,
 /*   810 */    52,   53,   54,  137,   56,   57,   58,   59,   32,   61,
 /*   820 */    32,  145,  146,   65,   66,   67,   68,   31,   34,   71,
 /*   830 */   108,   33,   31,  108,   35,   31,   35,   35,   31,   38,
 /*   840 */    34,   83,   84,   85,   35,  156,   88,   46,   47,   48,
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
 /*   960 */   156,   31,  114,  115,  116,   35,  156,  156,   38,  156,
 /*   970 */    83,   84,   85,  156,  156,   88,   46,   47,   48,   49,
 /*   980 */    50,   51,   52,   53,   54,  156,   56,   57,   58,   59,
 /*   990 */   156,   61,  156,  156,  137,   65,   66,   67,   68,  156,
 /*  1000 */   156,   71,  145,  146,   31,  156,  156,  156,   35,  156,
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
 /*  1120 */    61,  156,   31,  156,   65,   66,   67,   68,   37,  156,
 /*  1130 */    71,  156,  156,   42,  101,   44,   45,   39,   98,   99,
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
 /*  1390 */   101,   90,   91,   92,   93,   39,  145,  146,  156,  156,
 /*  1400 */   156,  112,  156,  156,  156,  156,  117,  118,  119,  120,
 /*  1410 */   121,  122,  123,  124,  125,  126,  127,  128,  129,  130,
 /*  1420 */   131,  132,  133,  156,  156,  156,  101,  156,   72,   73,
 /*  1430 */    74,   75,   76,  156,  145,  146,  156,  112,  156,  156,
 /*  1440 */   156,  156,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1450 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  156,
 /*  1460 */   105,  101,  107,  134,  109,  110,  137,  156,  113,  156,
 /*  1470 */   145,  146,  112,  156,  145,  146,  156,  117,  118,  119,
 /*  1480 */   120,  121,  122,  123,  124,  125,  126,  127,  128,  129,
 /*  1490 */   130,  131,  132,  133,  105,  106,  107,  156,  101,  110,
 /*  1500 */   156,  156,  113,  156,  156,  145,  146,  156,  156,  112,
 /*  1510 */   156,  156,  156,  156,  117,  118,  119,  120,  121,  122,
 /*  1520 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1530 */   133,  137,  156,  156,  156,  141,  101,  156,  156,  145,
 /*  1540 */   146,  156,  145,  146,  156,  156,  156,  112,  156,  156,
 /*  1550 */   156,  156,  117,  118,  119,  120,  121,  122,  123,  124,
 /*  1560 */   125,  126,  127,  128,  129,  130,  131,  132,  133,  137,
 /*  1570 */   156,  156,  101,  141,  156,  156,  156,  145,  146,  156,
 /*  1580 */   145,  146,  156,  112,  156,  156,  156,  156,  117,  118,
 /*  1590 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1600 */   129,  130,  131,  132,  133,  156,  137,  101,  156,  134,
 /*  1610 */   141,  156,  137,  156,  145,  146,  145,  146,  112,  156,
 /*  1620 */   145,  146,  156,  117,  118,  119,  120,  121,  122,  123,
 /*  1630 */   124,  125,  126,  127,  128,  129,  130,  131,  132,  133,
 /*  1640 */   137,  156,  156,  156,  101,  156,  156,  156,  145,  146,
 /*  1650 */   156,  145,  146,  156,  156,  112,  156,  156,  156,  156,
 /*  1660 */   117,  118,  119,  120,  121,  122,  123,  124,  125,  126,
 /*  1670 */   127,  128,  129,  130,  131,  132,  133,  137,  156,  156,
 /*  1680 */   156,  141,  101,  156,  156,  145,  146,  156,  145,  146,
 /*  1690 */   156,  156,  156,  112,  156,  156,  156,  156,  117,  118,
 /*  1700 */   119,  120,  121,  122,  123,  124,  125,  126,  127,  128,
 /*  1710 */   129,  130,  131,  132,  133,  137,  156,  156,  101,  141,
 /*  1720 */   156,  137,  156,  145,  146,  141,  145,  146,  156,  145,
 /*  1730 */   146,  156,  156,  156,  117,  118,  119,  120,  121,  122,
 /*  1740 */   123,  124,  125,  126,  127,  128,  129,  130,  131,  132,
 /*  1750 */   133,   23,   24,   25,   26,   27,   28,  156,  156,   31,
 /*  1760 */   156,  156,  145,  146,  156,  156,   39,  156,  156,   41,
 /*  1770 */   156,   43,   23,   24,   25,   26,   27,   28,   29,  156,
 /*  1780 */    31,  156,  156,  156,  156,  156,  137,  156,  156,  156,
 /*  1790 */    41,  156,   43,  156,  145,  146,  156,  156,   70,   72,
 /*  1800 */    73,   74,   75,   76,   77,   78,   78,   80,   81,   82,
 /*  1810 */   156,  156,  156,  156,   86,   87,  156,   89,   90,   91,
 /*  1820 */    92,   93,  156,  156,  137,  156,  156,   78,  141,  156,
 /*  1830 */   156,  156,  145,  146,  156,   86,   87,  156,   89,   90,
 /*  1840 */    91,   92,   93,   23,   24,   25,   26,   27,   28,   29,
 /*  1850 */   156,   31,  156,  156,  156,  156,  156,  156,  156,  134,
 /*  1860 */   156,   41,  137,   43,   23,   24,   25,   26,   27,   28,
 /*  1870 */   145,  146,   31,   32,  156,  156,  156,  156,  156,  156,
 /*  1880 */   134,  156,   41,  137,   43,   23,   24,   25,   26,   27,
 /*  1890 */    28,  145,  146,   31,  156,  156,  156,  156,   78,  156,
 /*  1900 */   156,  156,  156,   41,  156,   43,   86,   87,  156,   89,
 /*  1910 */    90,   91,   92,   93,  137,  156,  137,  156,  141,   78,
 /*  1920 */   156,  156,  145,  146,  145,  146,  156,   86,   87,  156,
 /*  1930 */    89,   90,   91,   92,   93,  134,  137,  156,  137,  156,
 /*  1940 */    78,   79,  156,  156,  145,  146,  145,  146,   86,   87,
 /*  1950 */   156,   89,   90,   91,   92,   93,   23,   24,   25,   26,
 /*  1960 */    27,   28,   29,  156,   31,  156,  156,  156,  156,  156,
 /*  1970 */   156,  156,  156,  156,   41,  156,   43,   23,   24,   25,
 /*  1980 */    26,   27,   28,  137,  156,   31,  156,  156,  156,  156,
 /*  1990 */   156,  145,  146,  156,  156,   41,  156,   43,   23,   24,
 /*  2000 */    25,   26,   27,   28,  137,  156,   31,  156,  156,  156,
 /*  2010 */   156,   78,  145,  146,  156,  156,   41,  156,   43,   86,
 /*  2020 */    87,  137,   89,   90,   91,   92,   93,  137,  156,  145,
 /*  2030 */   146,  156,   78,   79,  156,  145,  146,  156,  156,  156,
 /*  2040 */    86,   87,  156,   89,   90,   91,   92,   93,    1,    2,
 /*  2050 */     3,    4,    5,   78,   79,  156,  156,  156,  156,  156,
 /*  2060 */   156,   86,   87,  156,   89,   90,   91,   92,   93,   23,
 /*  2070 */    24,   25,   26,   27,   28,   29,  156,   31,  156,    1,
 /*  2080 */     2,    3,    4,    5,   37,  156,  156,   41,  156,   43,
 /*  2090 */   156,   44,   45,   23,   24,   25,   26,   27,   28,  137,
 /*  2100 */   156,   31,  156,  156,  156,  156,  156,  145,  146,   31,
 /*  2110 */   156,   41,  156,   43,  156,   37,  156,  137,  156,  137,
 /*  2120 */    42,  137,   44,   45,   78,  145,  146,  145,  146,  145,
 /*  2130 */   146,  156,   86,   87,  156,   89,   90,   91,   92,   93,
 /*  2140 */    70,  156,  156,  156,  156,  156,  156,  156,   78,  156,
 /*  2150 */   156,  156,  156,  156,  156,  156,   86,   87,  156,   89,
 /*  2160 */    90,   91,   92,   93,   23,   24,   25,   26,   27,   28,
 /*  2170 */   156,  156,   31,  156,  156,  156,  156,  156,  156,  156,
 /*  2180 */   156,  156,   41,  156,   43,  156,  156,  156,  156,  156,
 /*  2190 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*  2200 */    17,   18,   19,   20,   21,   22,  156,  156,  156,  156,
 /*  2210 */   156,  156,  156,  156,  156,  156,  156,   34,  156,   78,
 /*  2220 */   156,    1,    2,    3,    4,    5,  156,   86,   87,  156,
 /*  2230 */    89,   90,   91,   92,   93,    7,    8,    9,   10,   11,
 /*  2240 */    12,   13,   14,   15,   16,   17,   18,   19,   20,   21,
 /*  2250 */    22,  156,  156,  156,  156,   35,  156,   37,   38,  156,
 /*  2260 */    32,  156,  156,  156,   44,   45,  156,  156,    7,    8,
 /*  2270 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*  2280 */    19,   20,   21,   22,  156,  156,  156,  156,  156,  156,
 /*  2290 */   156,  156,  156,  156,  156,   34,  156,  156,  156,  156,
 /*  2300 */   156,  156,  156,    7,    8,    9,   10,   11,   12,   13,
 /*  2310 */    14,   15,   16,   17,   18,   19,   20,   21,   22,    7,
 /*  2320 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*  2330 */    18,   19,   20,   21,   22,  102,  103,  104,  105,  106,
 /*  2340 */   107,  156,  109,  110,   32,  156,  113,  156,  156,  156,
 /*  2350 */   156,  156,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2360 */    15,   16,   17,   18,   19,   20,   21,   22,    1,    2,
 /*  2370 */     3,    4,    5,  156,   29,   79,  156,  156,  156,  156,
 /*  2380 */   156,  156,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2390 */    15,   16,   17,   18,   19,   20,   21,   22,  156,  156,
 /*  2400 */   156,  156,   35,  156,   37,   38,  156,   40,  156,   34,
 /*  2410 */   156,   44,   45,  156,  156,  156,  156,    7,    8,    9,
 /*  2420 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2430 */    20,   21,   22,  156,  102,  103,  104,  105,  106,  107,
 /*  2440 */   156,  109,  110,  156,   34,  113,  156,  156,  156,  156,
 /*  2450 */   156,  156,    7,    8,    9,   10,   11,   12,   13,   14,
 /*  2460 */    15,   16,   17,   18,   19,   20,   21,   22,   11,    9,
 /*  2470 */    10,   11,   12,   13,   14,   15,   16,   17,   18,   19,
 /*  2480 */    20,   21,   22,  156,  156,  156,   29,  156,   31,  102,
 /*  2490 */   103,  104,  105,  106,  107,  156,  109,  110,   11,  156,
 /*  2500 */   113,  156,  156,   46,   47,   48,   49,   50,   51,   52,
 /*  2510 */    53,   54,   55,   56,  156,  156,   29,  156,   31,  156,
 /*  2520 */   156,  156,  156,  156,   11,  156,  156,  156,  156,  156,
 /*  2530 */   156,  156,  156,   46,   47,   48,   49,   50,   51,   52,
 /*  2540 */    53,   54,   55,   56,   31,    1,    2,    3,    4,    5,
 /*  2550 */   156,  156,  156,  156,  156,  156,  156,  156,  156,   46,
 /*  2560 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  2570 */     1,    2,    3,    4,    5,  156,  156,  156,  156,   35,
 /*  2580 */   156,   37,   38,  156,   40,  156,  156,  156,   44,   45,
 /*  2590 */     1,    2,    3,    4,    5,  156,  156,  156,  156,  156,
 /*  2600 */   156,  156,  156,  156,   35,  156,   37,   38,  156,   40,
 /*  2610 */   156,  156,  156,   44,   45,    1,    2,    3,    4,    5,
 /*  2620 */   156,  156,  156,  156,   35,  156,   37,   38,  156,   40,
 /*  2630 */   156,  156,  156,   44,   45,    1,    2,    3,    4,    5,
 /*  2640 */   156,  156,  156,  156,  156,  156,  156,  156,  156,   35,
 /*  2650 */   156,   37,   38,  156,   40,  156,  156,  156,   44,   45,
 /*  2660 */     1,    2,    3,    4,    5,  156,  156,  156,  156,   35,
 /*  2670 */   156,   37,   38,  156,   40,  156,  156,  156,   44,   45,
 /*  2680 */     1,    2,    3,    4,    5,    1,    2,    3,    4,    5,
 /*  2690 */   156,  156,  156,  156,   35,  156,   37,   38,  156,   40,
 /*  2700 */   156,  156,  156,   44,   45,    1,    2,    3,    4,    5,
 /*  2710 */   156,  156,  156,  156,   35,  156,   37,   38,  156,   35,
 /*  2720 */   156,   37,   38,   44,   45,  156,  156,  156,   44,   45,
 /*  2730 */     1,    2,    3,    4,    5,  156,  156,  156,  156,   35,
 /*  2740 */    31,   37,   38,    1,    2,    3,    4,    5,   44,   45,
 /*  2750 */   156,  156,  102,  103,  104,  105,  106,  107,  156,  109,
 /*  2760 */   110,  156,  156,  113,   35,  156,   37,   38,    1,    2,
 /*  2770 */     3,    4,    5,   44,   45,  156,  156,   35,  156,   37,
 /*  2780 */    38,    1,    2,    3,    4,    5,   44,   45,   79,  156,
 /*  2790 */   156,  156,  156,  156,  156,   86,   87,  156,  156,   90,
 /*  2800 */    91,   92,   93,  156,   37,  156,  156,  156,  156,   42,
 /*  2810 */   156,   44,   45,  156,  156,  156,  156,   37,  156,  156,
 /*  2820 */    40,  156,  156,  156,   44,   45,    1,    2,    3,    4,
 /*  2830 */     5,  156,  156,  156,  156,  156,  156,  156,  156,  156,
 /*  2840 */   156,  156,  156,  156,  156,  156,  156,  156,  156,  156,
 /*  2850 */   156,  156,  156,  156,  156,  156,  156,  156,  156,  156,
 /*  2860 */   156,  156,   37,  156,  156,  156,  156,   42,  156,   44,
 /*  2870 */    45,
};
#define YY_SHIFT_USE_DFLT (-24)
static short yy_shift_ofst[] = {
 /*     0 */   119,  173,  109,  -24,  -24,  -24,  -24,   82,  143,  -24,
 /*    10 */   157,  -17, 2634,  150,  -24,  -24, 2659, 2729,  -24, 2767,
 /*    20 */   164,  243, 2457,  166,  414,  -24,   70,  -24,  -24,  -24,
 /*    30 */   -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*    40 */   -24,  -24,  -24,  -24,  -24,  -24,  292,  -24,  326,  -24,
 /*    50 */  2141,  333,  887,  844,  345,  376,  801,  715,  -24, 2141,
 /*    60 */   381,  293,  -24,  154,  -24,  -24, 1301,  428, 1059, 1059,
 /*    70 */   460,  543,  500,  -24, 2141,  462,   27,  457,  -24,  467,
 /*    80 */   758,  629,  464, 2141,  505,  -24, 2141,  -24, 2141,  508,
 /*    90 */   -24,  477,  -15,  481,  -24,  490,  -24,  491,  -24, 1336,
 /*   100 */  2141, 1336, 2141, 1336, 2141, 1336, 2141, 1336, 2141, 1336,
 /*   110 */  2141, 1336, 2141, 1336, 2141,  297, 2141,  297, 2141,  -24,
 /*   120 */  2141,  -24, 2141,  -24, 2141,  297, 2141, 2460, 2141, 2460,
 /*   130 */   -23,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,
 /*   140 */   -24,  -24, 2345,  -24,  546, 2141,  -24, 1250,  545,  534,
 /*   150 */  1749, 2445, 2141,  -24,  548,  574,  508,  -24, 2141,  -24,
 /*   160 */   -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24, 2709,  -24,
 /*   170 */    53,  -24, 1117,  -24,  524, 1227,  -24,  -24,  -24,  -24,
 /*   180 */   -24,  -24,  -24,  -24,  -24,  -24,  540,  542,  549,  -24,
 /*   190 */   582,  572, 1933,  -24,  275,  -24, 2141,  -24,  -24,  -24,
 /*   200 */   -24,  447,  -24,  -24,   58,  573, 1820,  471,  -24,  -24,
 /*   210 */   -24, 2141,   28,  -24, 2046,  445,  -24,  -24, 2445,  587,
 /*   220 */   296, 1728, 2375, 1059,  199,  -24,  588,  452,  -24,  588,
 /*   230 */   -24, 1727,  -24,  -24,  -24,  -24,  -24,  -24, 2141,  -24,
 /*   240 */  2445,  592, 1098, 2141,  -24, 1862, 1356, 2141,  -24,  550,
 /*   250 */  1356, 2141,  -24, 2296,  599, 1209, 2141,  -24, 1954, 1356,
 /*   260 */  2141,  -24,  569, 1356, 2141,  -24, 1975, 1356, 2141,  -24,
 /*   270 */   583, 1356, 2141,  -24,  -24,  -24,  -24, 2141,  530,  -24,
 /*   280 */  2141,  -24, 2445,  -24,  631,  -24,  634,  -24, 1841, 2312,
 /*   290 */   -24,  -24,  637,  449,  641,  657,  -24,  228,  594,  659,
 /*   300 */   -24,  619,  667,  -24,  622,  670,  -24, 2141, 2228,  -24,
 /*   310 */   673,   55,  -24,  673,  -24,  666, 1301,  -24,  -24,  673,
 /*   320 */   108,  -24,  673,  315,  -24,  673,  356,  -24,  673,  358,
 /*   330 */   -24,  673,  279,  -24,  673,  425,  -24,  673,  487,  -24,
 /*   340 */   673,  408,  -24,  673,  511,  -24,  364,  677,  573,  678,
 /*   350 */   534,  -24,  -24, 2141, 2183, 1059,  586,  -24,  680,  646,
 /*   360 */  2070, 2410, 1059,  672,  -24, 2141, 2261, 1059,  930,  -24,
 /*   370 */   650, 2141,  700,  -24,  -24,  -24,  671, 1059, 1059,  -24,
 /*   380 */   -24,  681,  708,  973, 1016,  -24,  -24,  232,  710,  113,
 /*   390 */   -24,  156,  -24, 2513,  -24,  706, 1301,  -24,  717,  712,
 /*   400 */  1301,  -24,  718,  720, 1301,  -24,  723,  746,  744,  -24,
 /*   410 */   -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24,  -24, 2047,
 /*   420 */  2825,  753,  742, 2487,  754,  242,  -24,  285,  -24,  557,
 /*   430 */   756,  328,  -24,  371,  -24,  -24, 2589, 2684,  -24,  -24,
 /*   440 */  2780,  750,  748, 1301,  763,  -24,  760,  755, 1301,  770,
 /*   450 */   -24,  -24, 2780, 2078,  290,  -24, 1301,  786,  -24, 1091,
 /*   460 */   111,  -24, 1301,  788,  -24,  -24, 2614, 2742, 2679, 2047,
 /*   470 */  2704,  -24, 2220,  796,  794, 2544,  799,  -24,  -24,  798,
 /*   480 */   804,   68, 2367,  802,  -24,  -24,  807,  806, 2569,  809,
 /*   490 */   -24,  -24,  -24,
};
#define YY_REDUCE_USE_DFLT (-142)
static short yy_reduce_ofst[] = {
 /*     0 */   -86, -142, 1040, -142, -142, -142, -142, -142, -142, -142,
 /*    10 */  -142, -142, 2650, -142, -142, -142,  -53,   -7, -142,   77,
 /*    20 */  -142, -142,  332, -142, 1105, -142, 1617, -142, -142, -142,
 /*    30 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    40 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*    50 */  1329, -142, 1397, 1617, -142, -142, 1033, 1617, -142, 1725,
 /*    60 */  -142,  265, -142,  314, -142, -142,  361, -142, 1471, 1617,
 /*    70 */  -142, 1506, 1617, -142, 1801, -142, 1068, 1617, -142, -142,
 /*    80 */  1581, 1617, -142, 1746, -142, -142,  461, -142,  504,  395,
 /*    90 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   100 */   547, -142,  633, -142,  676, -142,  762, -142,  805, -142,
 /*   110 */   857, -142,  891, -142,  934, -142, 1503, -142, 1649, -142,
 /*   120 */  1779, -142, 1799, -142, 1846, -142, 1867, -142, 1884, -142,
 /*   130 */   246, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   140 */  -142, -142, -142, -142, -142, 1890, -142, -142, -142, -142,
 /*   150 */   117, -142, 1984, -142, -142, -142,  433, -142, 1982, -142,
 /*   160 */  -142, -142, -142, -142, -142, -142, -142, -142,   78, -142,
 /*   170 */  -142, -142,  121, -142, -142,   37, -142, -142, -142, -142,
 /*   180 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   190 */  -142, -142,  -55, -142, -142, -142,  160, -142, -142, -142,
 /*   200 */  -142, -142, -142, -142, -142, -142,   31, -142, -142, -142,
 /*   210 */  -142, 1962, -142, -142,   74, -142, -142, -142, -142, -142,
 /*   220 */  -142, 1980, -142, 1289, 1617, -142,  259, -142, -142,  483,
 /*   230 */  -142,  480, -142, -142, -142, -142, -142, -142, 1394, -142,
 /*   240 */  -142, -142,  485, 1149, -142,  203, -106, 1578, -142, -142,
 /*   250 */   506, 1469, -142, -142, -142,  515, 1584, -142,  341,  516,
 /*   260 */  1687, -142, -142,  518, 1777, -142,  -12,  519, 1432, -142,
 /*   270 */  -142,  521, 1540, -142, -142, -142, -142, 1075, -142, -142,
 /*   280 */   289, -142, -142, -142, -142, -142, -142, -142, 1038, -142,
 /*   290 */  -142, -142, -142, -142, -142, -142, -142, -142, -142, -142,
 /*   300 */  -142, -142, -142, -142, -142, -142, -142,  719, -142, -142,
 /*   310 */  -141, -142, -142,  517, -142, -142,  598, -142, -142, -126,
 /*   320 */  -142, -142,   88, -142, -142,   95, -142, -142,  124, -142,
 /*   330 */  -142,  131, -142, -142,  224, -142, -142,  256, -142, -142,
 /*   340 */   260, -142, -142,  342, -142, -142, -142, -142, -142, -142,
 /*   350 */  -142, -142, -142,  375, -142, 1543, 1617, -142, -142, -142,
 /*   360 */   427, -142, 1435, 1617, -142,  590, -142, 1360, 1617, -142,
 /*   370 */  -142, 1475, -142, -142, -142, -142, -142, 1325, 1617, -142,
 /*   380 */  -142, -142, -142, 1251, 1617, -142, -142, -142, -142, 1214,
 /*   390 */  -142, 1617, -142,  848, -142, -142,  639, -142, -142, -142,
 /*   400 */   644, -142, -142, -142,  669, -142, -142, -142, -142, -142,
 /*   410 */  -142, -142, -142, -142, -142, -142, -142, -142, -142,   86,
 /*   420 */    77, -142, -142,  418, -142, 1179, -142, 1617, -142, -142,
 /*   430 */  -142, 1143, -142, 1617, -142, -142, 1355,   -7, -142, -142,
 /*   440 */    86, -142, -142,  684, -142, -142, -142, -142,  689, -142,
 /*   450 */  -142, -142,  247,   77, -142, -142,  722, -142, -142,   77,
 /*   460 */  -142, -142,  725, -142, -142, -142, 1136, 1389,   -7,  247,
 /*   470 */    -7, -142,   -7, -142, -142, 2332, -142, -142, -142, -142,
 /*   480 */  -142, -142, 2233, -142, -142, -142, -142, -142, 2387, -142,
 /*   490 */  -142, -142, -142,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */   732,  732,  732,  495,  497,  498,  499,  732,  732,  500,
 /*    10 */   732,  732,  732,  732,  501,  505,  732,  732,  526,  732,
 /*    20 */   732,  732,  732,  732,  732,  530,  732,  532,  568,  570,
 /*    30 */   571,  572,  573,  574,  575,  576,  577,  578,  579,  580,
 /*    40 */   581,  582,  583,  584,  585,  586,  732,  587,  732,  588,
 /*    50 */   732,  732,  732,  732,  591,  732,  732,  732,  592,  732,
 /*    60 */   732,  732,  595,  732,  596,  597,  732,  732,  732,  599,
 /*    70 */   732,  732,  732,  602,  732,  732,  732,  732,  604,  732,
 /*    80 */   732,  732,  732,  732,  732,  606,  732,  660,  732,  732,
 /*    90 */   661,  732,  732,  732,  727,  732,  728,  732,  729,  662,
 /*   100 */   732,  663,  732,  664,  732,  665,  732,  666,  732,  667,
 /*   110 */   732,  668,  732,  669,  732,  676,  732,  677,  732,  678,
 /*   120 */   732,  679,  732,  680,  732,  681,  732,  682,  732,  683,
 /*   130 */   732,  557,  558,  559,  560,  561,  562,  563,  564,  565,
 /*   140 */   566,  567,  732,  670,  732,  732,  671,  688,  732,  672,
 /*   150 */   732,  711,  732,  684,  732,  732,  732,  685,  732,  686,
 /*   160 */   687,  689,  690,  691,  692,  693,  694,  695,  732,  696,
 /*   170 */   732,  697,  732,  712,  732,  732,  714,  719,  720,  721,
 /*   180 */   722,  723,  724,  725,  726,  715,  720,  722,  721,  713,
 /*   190 */   732,  698,  732,  699,  732,  700,  732,  703,  704,  709,
 /*   200 */   710,  732,  707,  708,  732,  673,  732,  732,  705,  706,
 /*   210 */   674,  732,  732,  675,  732,  732,  701,  702,  730,  732,
 /*   220 */   732,  732,  732,  732,  732,  607,  732,  732,  611,  732,
 /*   230 */   612,  732,  614,  615,  616,  617,  618,  619,  732,  620,
 /*   240 */   659,  732,  732,  732,  621,  732,  732,  732,  622,  732,
 /*   250 */   732,  732,  623,  732,  732,  732,  732,  624,  732,  732,
 /*   260 */   732,  625,  732,  732,  732,  626,  732,  732,  732,  627,
 /*   270 */   732,  732,  732,  628,  629,  630,  613,  732,  732,  632,
 /*   280 */   732,  633,  635,  634,  732,  636,  732,  637,  732,  732,
 /*   290 */   638,  639,  732,  732,  732,  732,  640,  732,  732,  732,
 /*   300 */   641,  732,  732,  642,  732,  732,  643,  732,  732,  644,
 /*   310 */   732,  732,  645,  732,  655,  657,  732,  658,  656,  732,
 /*   320 */   732,  646,  732,  732,  647,  732,  732,  648,  732,  732,
 /*   330 */   649,  732,  732,  650,  732,  732,  651,  732,  732,  652,
 /*   340 */   732,  732,  653,  732,  732,  654,  732,  732,  732,  732,
 /*   350 */   732,  731,  569,  732,  732,  732,  732,  608,  732,  732,
 /*   360 */   732,  732,  732,  732,  609,  732,  732,  732,  732,  610,
 /*   370 */   732,  732,  732,  605,  603,  601,  732,  732,  600,  598,
 /*   380 */   593,  589,  732,  732,  732,  594,  590,  732,  732,  732,
 /*   390 */   531,  732,  533,  732,  548,  550,  732,  553,  732,  551,
 /*   400 */   732,  554,  732,  552,  732,  555,  732,  732,  732,  549,
 /*   410 */   538,  540,  541,  542,  543,  544,  545,  546,  547,  732,
 /*   420 */   732,  732,  732,  732,  732,  732,  534,  732,  536,  732,
 /*   430 */   732,  732,  535,  732,  537,  539,  732,  732,  524,  527,
 /*   440 */   732,  732,  732,  732,  732,  528,  732,  732,  732,  732,
 /*   450 */   529,  518,  732,  732,  732,  520,  732,  732,  522,  732,
 /*   460 */   732,  521,  732,  732,  523,  525,  732,  732,  732,  732,
 /*   470 */   732,  519,  732,  732,  732,  732,  732,  502,  506,  732,
 /*   480 */   732,  732,  732,  732,  503,  507,  732,  732,  732,  732,
 /*   490 */   504,  508,  496,
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
  "THROW",         "CHAR",          "DOUBLE",        "NULL",        
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
  "xx_echo_expression",  "xx_mcall_expr",  "xx_scall_expr",  "xx_declare_variable_list",
  "xx_declare_variable",  "xx_isset_expr",  "xx_array_list",  "xx_call_parameters",
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
 /* 146 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 147 */ "xx_unset_statement ::= UNSET IDENTIFIER ARROW IDENTIFIER DOTCOMMA",
 /* 148 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN IDENTIFIER SBRACKET_CLOSE DOTCOMMA",
 /* 149 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN INTEGER SBRACKET_CLOSE DOTCOMMA",
 /* 150 */ "xx_unset_statement ::= UNSET IDENTIFIER SBRACKET_OPEN STRING SBRACKET_CLOSE DOTCOMMA",
 /* 151 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
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
 /* 198 */ "xx_common_expr ::= CHAR",
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
// 2149 "parser.c"
      break;
    case 96:
// 849 "parser.lemon"
{ json_object_put((yypminor->yy103)); }
// 2154 "parser.c"
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
  { 122, 2 },
  { 125, 5 },
  { 125, 6 },
  { 125, 6 },
  { 125, 6 },
  { 126, 3 },
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
  { 147, 3 },
  { 147, 1 },
  { 148, 1 },
  { 148, 3 },
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
  { 151, 3 },
  { 151, 1 },
  { 152, 1 },
  { 150, 3 },
  { 150, 1 },
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
  { 149, 4 },
  { 149, 4 },
  { 149, 4 },
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
// 2610 "parser.c"
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
// 2644 "parser.c"
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
// 2657 "parser.c"
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
// 2676 "parser.c"
        break;
      case 7:
// 875 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(30,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2685 "parser.c"
        break;
      case 8:
// 879 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, 0, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2695 "parser.c"
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
// 2706 "parser.c"
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
// 2717 "parser.c"
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
// 2729 "parser.c"
        break;
      case 12:
// 895 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class(yymsp[-2].minor.yy0, NULL, 0, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 2739 "parser.c"
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
// 2750 "parser.c"
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
// 2761 "parser.c"
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
// 2773 "parser.c"
        break;
      case 16:
// 911 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
}
// 2780 "parser.c"
        break;
      case 17:
// 915 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 2787 "parser.c"
        break;
      case 18:
// 919 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, yymsp[0].minor.yy103, NULL, status->scanner_state);
}
// 2794 "parser.c"
        break;
      case 19:
// 923 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
}
// 2801 "parser.c"
        break;
      case 20:
// 927 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 2808 "parser.c"
        break;
      case 21:
// 931 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[0].minor.yy103, NULL, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2815 "parser.c"
        break;
      case 22:
// 935 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(NULL, yymsp[0].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2822 "parser.c"
        break;
      case 23:
// 939 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
}
// 2829 "parser.c"
        break;
      case 24:
// 943 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_definition(yymsp[-1].minor.yy103, yymsp[0].minor.yy103, yymsp[-2].minor.yy103, status->scanner_state);
}
// 2836 "parser.c"
        break;
      case 27:
// 955 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-2].minor.yy103, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2844 "parser.c"
        break;
      case 28:
// 959 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-2].minor.yy103, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 2852 "parser.c"
        break;
      case 29:
// 963 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-4].minor.yy103, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2861 "parser.c"
        break;
      case 30:
// 967 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_property(yymsp[-4].minor.yy103, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2870 "parser.c"
        break;
      case 35:
// 987 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2880 "parser.c"
        break;
      case 36:
// 991 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(40,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 2890 "parser.c"
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
// 2902 "parser.c"
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
// 2914 "parser.c"
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
// 2926 "parser.c"
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
// 2938 "parser.c"
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
// 2950 "parser.c"
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
// 2962 "parser.c"
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
// 2974 "parser.c"
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
// 2986 "parser.c"
        break;
      case 47:
// 1035 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 2994 "parser.c"
        break;
      case 48:
// 1039 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 3002 "parser.c"
        break;
      case 49:
// 1043 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 3010 "parser.c"
        break;
      case 50:
// 1047 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 3018 "parser.c"
        break;
      case 51:
// 1051 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 3026 "parser.c"
        break;
      case 52:
// 1055 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("inline");
  yy_destructor(44,&yymsp[0].minor);
}
// 3034 "parser.c"
        break;
      case 53:
// 1059 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("abstract");
  yy_destructor(37,&yymsp[0].minor);
}
// 3042 "parser.c"
        break;
      case 54:
// 1063 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("final");
  yy_destructor(45,&yymsp[0].minor);
}
// 3050 "parser.c"
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
// 3063 "parser.c"
        break;
      case 57:
// 1075 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3070 "parser.c"
        break;
      case 58:
// 1079 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(yymsp[-1].minor.yy103, NULL, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3077 "parser.c"
        break;
      case 59:
// 1083 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, yymsp[-1].minor.yy103, yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3084 "parser.c"
        break;
      case 60:
// 1087 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3092 "parser.c"
        break;
      case 61:
// 1091 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(yymsp[-3].minor.yy103, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3100 "parser.c"
        break;
      case 62:
// 1095 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_parameter(NULL, yymsp[-3].minor.yy103, yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3108 "parser.c"
        break;
      case 63:
// 1099 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(11,&yymsp[-2].minor);
  yy_destructor(12,&yymsp[0].minor);
}
// 3117 "parser.c"
        break;
      case 64:
// 1103 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(46,&yymsp[0].minor);
}
// 3125 "parser.c"
        break;
      case 65:
// 1107 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(47,&yymsp[0].minor);
}
// 3133 "parser.c"
        break;
      case 66:
// 1111 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(48,&yymsp[0].minor);
}
// 3141 "parser.c"
        break;
      case 67:
// 1115 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(49,&yymsp[0].minor);
}
// 3149 "parser.c"
        break;
      case 68:
// 1119 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(50,&yymsp[0].minor);
}
// 3157 "parser.c"
        break;
      case 69:
// 1123 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(51,&yymsp[0].minor);
}
// 3165 "parser.c"
        break;
      case 70:
// 1127 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(52,&yymsp[0].minor);
}
// 3173 "parser.c"
        break;
      case 71:
// 1131 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(53,&yymsp[0].minor);
}
// 3181 "parser.c"
        break;
      case 72:
// 1135 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(54,&yymsp[0].minor);
}
// 3189 "parser.c"
        break;
      case 73:
// 1139 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(55,&yymsp[0].minor);
}
// 3197 "parser.c"
        break;
      case 74:
// 1143 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(56,&yymsp[0].minor);
}
// 3205 "parser.c"
        break;
      case 94:
// 1223 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(57,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3214 "parser.c"
        break;
      case 95:
// 1227 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(58,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3223 "parser.c"
        break;
      case 96:
// 1231 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-2].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3233 "parser.c"
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
// 3246 "parser.c"
        break;
      case 98:
// 1239 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_if_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(59,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3256 "parser.c"
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
// 3269 "parser.c"
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
// 3282 "parser.c"
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
// 3295 "parser.c"
        break;
      case 102:
// 1255 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_switch_statement(yymsp[-2].minor.yy103, NULL, status->scanner_state);
  yy_destructor(61,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3305 "parser.c"
        break;
      case 103:
// 1259 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_switch_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(61,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3315 "parser.c"
        break;
      case 106:
// 1271 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_case_clause(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(62,&yymsp[-3].minor);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3324 "parser.c"
        break;
      case 107:
// 1275 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_case_clause(NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(63,&yymsp[-1].minor);
}
// 3333 "parser.c"
        break;
      case 108:
// 1279 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3343 "parser.c"
        break;
      case 109:
// 1283 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_loop_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(65,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3353 "parser.c"
        break;
      case 110:
// 1287 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_while_statement(yymsp[-2].minor.yy103, NULL, status->scanner_state);
  yy_destructor(66,&yymsp[-3].minor);
  yy_destructor(34,&yymsp[-1].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3363 "parser.c"
        break;
      case 111:
// 1291 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_while_statement(yymsp[-3].minor.yy103, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(66,&yymsp[-4].minor);
  yy_destructor(34,&yymsp[-2].minor);
  yy_destructor(35,&yymsp[0].minor);
}
// 3373 "parser.c"
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
// 3385 "parser.c"
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
// 3397 "parser.c"
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
// 3408 "parser.c"
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
// 3420 "parser.c"
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
// 3432 "parser.c"
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
// 3445 "parser.c"
        break;
      case 118:
// 1319 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(71,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3454 "parser.c"
        break;
      case 121:
// 1332 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("assign");
  yy_destructor(39,&yymsp[0].minor);
}
// 3462 "parser.c"
        break;
      case 122:
// 1337 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("add-assign");
  yy_destructor(72,&yymsp[0].minor);
}
// 3470 "parser.c"
        break;
      case 123:
// 1342 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("sub-assign");
  yy_destructor(73,&yymsp[0].minor);
}
// 3478 "parser.c"
        break;
      case 124:
// 1346 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("mult-assign");
  yy_destructor(74,&yymsp[0].minor);
}
// 3486 "parser.c"
        break;
      case 125:
// 1350 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("div-assign");
  yy_destructor(75,&yymsp[0].minor);
}
// 3494 "parser.c"
        break;
      case 126:
// 1354 "parser.lemon"
{
	yygotominor.yy103 = json_object_new_string("concat-assign");
  yy_destructor(76,&yymsp[0].minor);
}
// 3502 "parser.c"
        break;
      case 127:
// 1359 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy103, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 3509 "parser.c"
        break;
      case 128:
// 1364 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
}
// 3517 "parser.c"
        break;
      case 129:
// 1369 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3527 "parser.c"
        break;
      case 130:
// 1374 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy103, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3537 "parser.c"
        break;
      case 131:
// 1379 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
}
// 3545 "parser.c"
        break;
      case 132:
// 1384 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy103, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-5].minor);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3555 "parser.c"
        break;
      case 133:
// 1389 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy103, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-6].minor);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3565 "parser.c"
        break;
      case 134:
// 1394 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy103, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(78,&yymsp[-3].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3574 "parser.c"
        break;
      case 135:
// 1399 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy103, yymsp[-5].minor.yy0, NULL, yymsp[-3].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(78,&yymsp[-4].minor);
  yy_destructor(79,&yymsp[-2].minor);
}
// 3583 "parser.c"
        break;
      case 136:
// 1404 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[0].minor);
}
// 3591 "parser.c"
        break;
      case 137:
// 1409 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(82,&yymsp[0].minor);
}
// 3599 "parser.c"
        break;
      case 139:
// 1417 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_echo_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(83,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3608 "parser.c"
        break;
      case 142:
// 1429 "parser.lemon"
{
	yygotominor.yy103 = yymsp[0].minor.yy103;;
}
// 3615 "parser.c"
        break;
      case 143:
// 1434 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3623 "parser.c"
        break;
      case 144:
// 1439 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(32,&yymsp[0].minor);
}
// 3631 "parser.c"
        break;
      case 145:
// 1444 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_return_statement(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(84,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3640 "parser.c"
        break;
      case 146:
// 1449 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(84,&yymsp[-1].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3649 "parser.c"
        break;
      case 147:
// 1454 "parser.lemon"
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
// 3663 "parser.c"
        break;
      case 148:
// 1463 "parser.lemon"
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
// 3678 "parser.c"
        break;
      case 149:
// 1472 "parser.lemon"
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
// 3693 "parser.c"
        break;
      case 150:
// 1481 "parser.lemon"
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
// 3708 "parser.c"
        break;
      case 151:
// 1490 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_throw_exception(yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(88,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3717 "parser.c"
        break;
      case 152:
// 1494 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(46,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3726 "parser.c"
        break;
      case 153:
// 1498 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(47,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3735 "parser.c"
        break;
      case 154:
// 1502 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3744 "parser.c"
        break;
      case 155:
// 1506 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(51,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3753 "parser.c"
        break;
      case 156:
// 1510 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(48,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3762 "parser.c"
        break;
      case 157:
// 1514 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3771 "parser.c"
        break;
      case 158:
// 1518 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3780 "parser.c"
        break;
      case 159:
// 1522 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(54,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3789 "parser.c"
        break;
      case 160:
// 1526 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(53,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3798 "parser.c"
        break;
      case 161:
// 1530 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(32,&yymsp[0].minor);
}
// 3807 "parser.c"
        break;
      case 164:
// 1542 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 3814 "parser.c"
        break;
      case 165:
// 1546 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 3822 "parser.c"
        break;
      case 167:
// 1554 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 3830 "parser.c"
        break;
      case 168:
// 1558 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("isset", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 3838 "parser.c"
        break;
      case 169:
// 1562 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("equals", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 3846 "parser.c"
        break;
      case 170:
// 1566 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not-equals", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(16,&yymsp[-1].minor);
}
// 3854 "parser.c"
        break;
      case 171:
// 1570 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("identical", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 3862 "parser.c"
        break;
      case 172:
// 1574 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("not-identical", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 3870 "parser.c"
        break;
      case 173:
// 1578 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("less", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 3878 "parser.c"
        break;
      case 174:
// 1582 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("greater", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 3886 "parser.c"
        break;
      case 175:
// 1586 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("less-equal", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 3894 "parser.c"
        break;
      case 176:
// 1590 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 3902 "parser.c"
        break;
      case 177:
// 1594 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("list", yymsp[-1].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 3911 "parser.c"
        break;
      case 178:
// 1598 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("cast", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-3].minor);
  yy_destructor(29,&yymsp[-1].minor);
}
// 3920 "parser.c"
        break;
      case 179:
// 1602 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 3928 "parser.c"
        break;
      case 180:
// 1606 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 3936 "parser.c"
        break;
      case 181:
// 1610 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-1].minor);
}
// 3944 "parser.c"
        break;
      case 182:
// 1615 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), yymsp[-1].minor.yy103, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 3953 "parser.c"
        break;
      case 183:
// 1620 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("add", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(17,&yymsp[-1].minor);
}
// 3961 "parser.c"
        break;
      case 184:
// 1625 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("sub", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 3969 "parser.c"
        break;
      case 185:
// 1630 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("mul", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 3977 "parser.c"
        break;
      case 186:
// 1635 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("div", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 3985 "parser.c"
        break;
      case 187:
// 1640 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("mod", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 3993 "parser.c"
        break;
      case 188:
// 1645 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("concat", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 4001 "parser.c"
        break;
      case 189:
// 1650 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("and", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 4009 "parser.c"
        break;
      case 190:
// 1655 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("or", yymsp[-2].minor.yy103, yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(8,&yymsp[-1].minor);
}
// 4017 "parser.c"
        break;
      case 191:
// 1660 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("instanceof", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 4025 "parser.c"
        break;
      case 192:
// 1665 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy103, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4034 "parser.c"
        break;
      case 193:
// 1670 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("typeof", yymsp[0].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 4042 "parser.c"
        break;
      case 194:
// 1675 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 4049 "parser.c"
        break;
      case 195:
      case 223:
      case 227:
// 1680 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4058 "parser.c"
        break;
      case 196:
      case 225:
      case 228:
// 1685 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4067 "parser.c"
        break;
      case 197:
      case 224:
      case 229:
// 1690 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 4076 "parser.c"
        break;
      case 198:
// 1695 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 4083 "parser.c"
        break;
      case 199:
      case 230:
// 1700 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 4091 "parser.c"
        break;
      case 200:
      case 231:
// 1705 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(91,&yymsp[0].minor);
}
// 4100 "parser.c"
        break;
      case 201:
      case 233:
// 1710 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[0].minor);
}
// 4109 "parser.c"
        break;
      case 202:
      case 232:
// 1715 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 4118 "parser.c"
        break;
      case 203:
// 1720 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-1].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4127 "parser.c"
        break;
      case 204:
// 1725 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array", yymsp[-1].minor.yy103, NULL, NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4136 "parser.c"
        break;
      case 205:
// 1730 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 4144 "parser.c"
        break;
      case 206:
// 1735 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4154 "parser.c"
        break;
      case 207:
// 1740 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_new_instance(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(27,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4164 "parser.c"
        break;
      case 208:
// 1745 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_fcall(yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4173 "parser.c"
        break;
      case 209:
// 1750 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_fcall(yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4182 "parser.c"
        break;
      case 212:
// 1765 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4192 "parser.c"
        break;
      case 213:
// 1770 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_scall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4202 "parser.c"
        break;
      case 214:
// 1775 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall(yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy103, status->scanner_state);
  yy_destructor(77,&yymsp[-4].minor);
  yy_destructor(43,&yymsp[-2].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4212 "parser.c"
        break;
      case 215:
// 1780 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_mcall(yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(77,&yymsp[-3].minor);
  yy_destructor(43,&yymsp[-1].minor);
  yy_destructor(29,&yymsp[0].minor);
}
// 4222 "parser.c"
        break;
      case 221:
// 1804 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_array_item(yymsp[-2].minor.yy103, yymsp[0].minor.yy103, status->scanner_state);
  yy_destructor(63,&yymsp[-1].minor);
}
// 4230 "parser.c"
        break;
      case 222:
// 1808 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_array_item(NULL, yymsp[0].minor.yy103, status->scanner_state);
}
// 4237 "parser.c"
        break;
      case 234:
// 1857 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4246 "parser.c"
        break;
      case 235:
// 1862 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_INTEGER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4255 "parser.c"
        break;
      case 236:
// 1867 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_expr("array-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(79,&yymsp[0].minor);
}
// 4264 "parser.c"
        break;
      case 238:
// 1875 "parser.lemon"
{
	yygotominor.yy103 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 4271 "parser.c"
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

// 4347 "parser.c"
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
