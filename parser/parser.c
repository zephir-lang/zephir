/** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
// 52 "parser.lemon"


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

	json_object_object_add(ret, "abstract", json_object_new_int(is_abstract));
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

static json_object *xx_ret_interface(xx_parser_token *T, json_object *interface_definition, xx_parser_token *E, xx_scanner_state *state)
{
  json_object *ret = json_object_new_object();

  json_object_object_add(ret, "type", json_object_new_string("interface"));
  json_object_object_add(ret, "name", json_object_new_string(T->token));

  if (E) {
    json_object_object_add(ret, "extends", json_object_new_string(E->token));
  }

  if (interface_definition) {
    json_object_object_add(ret, "definition", interface_definition);
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

static json_object *xx_ret_interface_definition(json_object *methods, json_object *constants, xx_scanner_state *state)
{
  json_object *ret = json_object_new_object();

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

static json_object *xx_ret_class_property(json_object *visibility, xx_parser_token *T,
		json_object *default_value, xx_parser_token *D, json_object *shortcuts, xx_scanner_state *state)
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

	if (shortcuts) {
		json_object_object_add(ret, "shortcuts", shortcuts);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_property_shortcut(xx_parser_token *C, xx_parser_token *D, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("shortcut"));
	if (C) {
		json_object_object_add(ret, "docblock", json_object_new_string(C->token));
	}
	json_object_object_add(ret, "name", json_object_new_string(D->token));

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
	json_object *statements, xx_parser_token *D, json_object *return_type, xx_scanner_state *state)
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

	if (return_type) {
		json_object_object_add(ret, "return-type", return_type);
	}

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_parameter(json_object *type, json_object *cast, xx_parser_token *N, json_object *default_value,
	int mandatory, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("parameter"));
	json_object_object_add(ret, "name", json_object_new_string(N->token));

	if (type) {
		json_object_object_add(ret, "data-type", type);
		json_object_object_add(ret, "mandatory", json_object_new_int(mandatory));
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

static json_object *xx_ret_return_type(int is_void, json_object *return_type_list, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return-type"));

	json_object_object_add(ret, "list", return_type_list);

	json_object_object_add(ret, "void", json_object_new_int(is_void));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}

static json_object *xx_ret_return_type_item(json_object *type, json_object *cast, int mandatory, int collection, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("return-type-parameter"));

	if (type) {
		json_object_object_add(ret, "data-type", type);
		json_object_object_add(ret, "mandatory", json_object_new_int(mandatory));
	}

	if (cast) {
		json_object_object_add(ret, "cast", cast);
		json_object_object_add(ret, "collection", json_object_new_int(collection));
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
		case XX_TYPE_LONG:
			return json_object_new_string("long");
		case XX_TYPE_ULONG:
			return json_object_new_string("ulong");
		case XX_TYPE_STRING:
			return json_object_new_string("string");
		case XX_TYPE_CHAR:
			return json_object_new_string("char");
		case XX_TYPE_ARRAY:
			return json_object_new_string("array");
		case XX_TYPE_VAR:
			return json_object_new_string("variable");
		default:
			fprintf(stderr, "unknown type?\n");
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

static json_object *xx_ret_fetch_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("fetch"));
	json_object_object_add(ret, "expr", expr);

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

static json_object *xx_ret_unset_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("unset"));

	json_object_object_add(ret, "expr", expr);

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

static json_object *xx_ret_new_instance(int dynamic, xx_parser_token *T, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("new"));
	json_object_object_add(ret, "class", json_object_new_string(T->token));
	json_object_object_add(ret, "dynamic", json_object_new_int(dynamic));

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

static json_object *xx_ret_mcall(int type, json_object *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("mcall"));
	json_object_object_add(ret, "variable", O);
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

static json_object *xx_ret_scall(int dynamic_class, xx_parser_token *O, xx_parser_token *M, json_object *parameters, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("scall"));
	json_object_object_add(ret, "dynamic-class", json_object_new_int(dynamic_class));
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

static json_object *xx_ret_cblock(xx_parser_token *T, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("cblock"));
	json_object_object_add(ret, "value", json_object_new_string(T->token));

	json_object_object_add(ret, "file", json_object_new_string(state->active_file));
	json_object_object_add(ret, "line", json_object_new_int(state->active_line));
	json_object_object_add(ret, "char", json_object_new_int(state->active_char));

	return ret;
}


// 943 "parser.c"
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
#define YYNOCODE 192
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy129;
  int yy383;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 678
#define YYNRULE 330
#define YYERRORSYMBOL 107
#define YYERRSYMDT yy383
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
 /*     0 */   232,  311,  241,  244,  235,  238,  593,  678,   34,   58,
 /*    10 */    37,   39,   40,  285, 1009,    1,    2,  677,    4,    5,
 /*    20 */     6,    7,    8,  618,  417,  229,  293,  298,  312,  226,
 /*    30 */   624,  308,  435,  619,  289,  185,  610,  152,  290,  614,
 /*    40 */   636,   25,  319,  547,  527,  137,  612,  307,   24,  279,
 /*    50 */   311,  122,  122,  490,  499,  508,  511,  502,  505,  514,
 /*    60 */   520,  517,  641,  523,  186,  188,  190,  420,  199,  624,
 /*    70 */   116,  992,  210,  214,  219,  393,   82,   90,  400,   96,
 /*    80 */   658,  406,  656,   82,   90,  301,   67,  290,  291,  465,
 /*    90 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   100 */   232,  369,  241,  244,  235,  238,  113,  621,   34,   58,
 /*   110 */    37,   39,   40,  285,  624,  407,  408,  409,  410,  411,
 /*   120 */   412,  254,  250,  433,  434,  229,  293,  298,  312,  226,
 /*   130 */   413,  308,  459,  432,  289,  185,  261,  263,  265,  254,
 /*   140 */   250,  638,  319,  546,  527,  254,  250,  307,  624,  279,
 /*   150 */   436,  639,  493,  490,  499,  508,  511,  502,  505,  514,
 /*   160 */   520,  517,  594,  523,  186,  188,  190,  418,  199,  427,
 /*   170 */   432,  468,  210,  214,  219,  393,  154,  570,  400,  145,
 /*   180 */   406,  656,  153,   82,   90,  301,   96,  122,  522,  465,
 /*   190 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   200 */   232,  440,  241,  244,  235,  238,   31,  467,   34,   58,
 /*   210 */    37,   39,   40,  285,  407,  408,  409,  410,  411,  412,
 /*   220 */    78,   36,   37,   39,   40,  229,  293,  298,  312,  226,
 /*   230 */    10,  308,  322,   81,  289,  185,    3,    4,    5,    6,
 /*   240 */     7,    8,  319,  581,  527,  653,   11,  307,  446,  279,
 /*   250 */   452,  432,  564,  490,  499,  508,  511,  502,  505,  514,
 /*   260 */   520,  517,  159,  523,  186,  188,  190,  630,  199,  330,
 /*   270 */    66,   73,  210,  214,  219,  393,  625,  631,  400,   71,
 /*   280 */    72,   74,   75,   77,   76,  301,  401,  464,  147,  465,
 /*   290 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   300 */   232,  117,  241,  244,  235,  238,  257,  259,  267,  261,
 /*   310 */   263,  265,  453,  285,  598,  430,  403,   13,  254,  250,
 /*   320 */    82,   90,  204,   96,   14,  229,  293,  298,  312,  226,
 /*   330 */   144,  308,   60,  147,  289,  185,   16,  611,   82,   90,
 /*   340 */   154,   96,  319,  579,  527,  123,  612,  307,  206,  279,
 /*   350 */   553,  122,  402,  490,  499,  508,  511,  502,  505,  514,
 /*   360 */   520,  517,   18,  523,  186,  188,  190,  448,  199,  616,
 /*   370 */   318,  535,  210,  214,  219,  393,   17,  141,  400,  657,
 /*   380 */   122,  406,  147,   97,  118,  301,  202,  121,  628,  465,
 /*   390 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   400 */   232,  384,  241,  244,  235,  238,  428,  460,  191,  430,
 /*   410 */   430,  392,  206,  285,  553,  407,  408,  409,  410,  411,
 /*   420 */   412,  326,  328,  327,  297,  229,  293,  298,  312,  226,
 /*   430 */   112,  308,  627,  115,  289,  185,  648,  374,  614,  636,
 /*   440 */   107,   28,  319,  563,  527,  612,  318,  307,  395,  279,
 /*   450 */   122,  122,  630,  490,  499,  508,  511,  502,  505,  514,
 /*   460 */   520,  517,  631,  523,  186,  188,  190,  431,  199,  592,
 /*   470 */   318,  359,  210,  214,  219,  393,  318,  378,  400,  588,
 /*   480 */    27,  406,  491,  498,   30,  301,  493,   59,  380,  465,
 /*   490 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   500 */   232,  371,  241,  244,  235,  238,  577,  367,   34,   58,
 /*   510 */    37,   39,   40,  285,  318,  407,  408,  409,  410,  411,
 /*   520 */   412,  318,  492,  604,  358,  229,  293,  298,  312,  226,
 /*   530 */    32,  308,   86,  600,  289,  185,  664,   83,  667,  500,
 /*   540 */   498,  147,  319,  575,  527,  352,  318,  307,   21,  279,
 /*   550 */   503,  498,  350,  490,  499,  508,  511,  502,  505,  514,
 /*   560 */   520,  517,  674,  523,  186,  188,  190,  147,  199,  253,
 /*   570 */   318,   38,  210,  214,  219,  393,  318,  324,  400,  326,
 /*   580 */   328,  327,  297,   42,  318,  301,  383,  329,  493,  465,
 /*   590 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   600 */   232,  341,  241,  244,  235,  238,  605,  334,   34,   58,
 /*   610 */    37,   39,   40,  285,  119,  317,   34,   58,   37,   39,
 /*   620 */    40,  668,  331,  656,  501,  229,  293,  298,  312,  226,
 /*   630 */   336,  308,  506,  498,  289,  185,   66,  313,   35,   23,
 /*   640 */   583,   25,  319,  573,  527,  344,  123,  307,  124,  279,
 /*   650 */    45,  122,  122,  490,  499,  508,  511,  502,  505,  514,
 /*   660 */   520,  517,   46,  523,  186,  188,  190,  599,  199,  253,
 /*   670 */   493,   43,  210,  214,  219,  393,  251,   69,  400,  326,
 /*   680 */   328,  327,  297,  493,  373,  301,  377,  329,   44,  465,
 /*   690 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   700 */   232,  493,  241,  244,  235,  238,  504,  509,  498,  512,
 /*   710 */   498,   64,  565,  285,   34,   58,   37,   39,   40,  507,
 /*   720 */   203,  556,  515,  498,   70,  229,  293,  298,  312,  226,
 /*   730 */    62,  308,   79,   65,  289,  185,   66,  510,  518,  498,
 /*   740 */   493,  493,  319,  562,  527,  521,  498,  307,  493,  279,
 /*   750 */   524,  498,   66,  490,  499,  508,  511,  502,  505,  514,
 /*   760 */   520,  517,   80,  523,  186,  188,  190,   61,  199,  253,
 /*   770 */   493,   84,  210,  214,  219,  393,  519,  513,  400,  326,
 /*   780 */   328,  327,  297,  571,  516,  301,  349,  329,   85,  465,
 /*   790 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   800 */   232,  576,  241,  244,  235,  238,  525,  626,  633,  531,
 /*   810 */    87,  572,   63,  285,   34,   58,   37,   39,   40,  326,
 /*   820 */   328,  327,  297,   88,   91,  229,  293,  298,  312,  226,
 /*   830 */    89,  308,   15,   92,  289,  185,   93,   18,   95,   43,
 /*   840 */   109,  110,  319,  569,  527,  114,  120,  307,  127,  279,
 /*   850 */   128,  129,  130,  490,  499,  508,  511,  502,  505,  514,
 /*   860 */   520,  517,  132,  523,  186,  188,  190,  133,  199,  253,
 /*   870 */   134,  135,  210,  214,  219,  393,  139,  140,  400,  326,
 /*   880 */   328,  327,  297,  142,  143,  301,  340,  329,  655,  465,
 /*   890 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*   900 */   232,  150,  241,  244,  235,  238,  156,  146,  157,  187,
 /*   910 */   189,  192,  195,  285,  208,  196,  205,  225,  201,  207,
 /*   920 */   211,  216,  220,  223,  283,  229,  293,  298,  312,  226,
 /*   930 */   252,  308,  286,  287,  289,  185,  292,  294,  295,  314,
 /*   940 */   320,  332,  319,  567,  527,  321,  337,  307,  338,  279,
 /*   950 */   339,  345,  346,  490,  499,  508,  511,  502,  505,  514,
 /*   960 */   520,  517,  347,  523,  186,  188,  190,  994,  199,  253,
 /*   970 */   375,  376,  210,  214,  219,  393,  381,  382,  400,  326,
 /*   980 */   328,  327,  297,  394,  993,  301,  333,  329,  362,  465,
 /*   990 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1000 */   232,  405,  241,  244,  235,  238,  422,  404,  425,  437,
 /*  1010 */   445,  438,  441,  285,  473,  475,  442,  450,  457,  477,
 /*  1020 */   479,  495,  496,  536,  494,  229,  293,  298,  312,  226,
 /*  1030 */   497,  308,  548,  537,  289,  185,  550,  559,  554,  560,
 /*  1040 */   566,  578,  319,  589,  527,  585,  606,  307,  634,  279,
 /*  1050 */   623,  586,  620,  490,  499,  508,  511,  502,  505,  514,
 /*  1060 */   520,  517,  728,  523,  186,  188,  190,  622,  199,  253,
 /*  1070 */   729,  629,  210,  214,  219,  393,  632,  644,  400,  326,
 /*  1080 */   328,  327,  297,  635,  640,  301,  323,  329,  642,  465,
 /*  1090 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1100 */   232,  643,  241,  244,  235,  238,  645,  654,  661,  662,
 /*  1110 */   665,  147,  671,  285,  659,  672,  663,  666,  675,  669,
 /*  1120 */   673,  676,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1130 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1140 */   523,  523,  319,  218,  527,  523,  523,  307,  523,  279,
 /*  1150 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1160 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  253,
 /*  1170 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  1180 */   328,  327,  297,  523,  523,  301,  316,  329,  523,  465,
 /*  1190 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1200 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  1210 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  1220 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1230 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1240 */   523,  523,  319,  198,  527,  523,  523,  307,  523,  279,
 /*  1250 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1260 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  253,
 /*  1270 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  1280 */   328,  327,  297,  523,  523,  301,  370,  329,  523,  465,
 /*  1290 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1300 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  1310 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  1320 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1330 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1340 */   523,  523,  319,  557,  527,  523,  523,  307,  523,  279,
 /*  1350 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1360 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  253,
 /*  1370 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  1380 */   328,  327,  297,  523,  523,  301,  351,  329,  523,  465,
 /*  1390 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1400 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  1410 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  1420 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1430 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1440 */   523,  523,  319,  194,  527,  523,  523,  307,  523,  279,
 /*  1450 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1460 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  253,
 /*  1470 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  1480 */   328,  327,  297,  523,  523,  301,  366,  329,  523,  465,
 /*  1490 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1500 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  1510 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  1520 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1530 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1540 */   523,  523,  319,  222,  527,  523,  523,  307,  523,  279,
 /*  1550 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1560 */   520,  517,  523,  523,  186,  188,  190,  549,  199,  523,
 /*  1570 */   392,  523,  210,  214,  219,  393,  523,  523,  400,  523,
 /*  1580 */   326,  328,  327,  297,  523,  301,  523,  523,  523,  465,
 /*  1590 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1600 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  1610 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  1620 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1630 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1640 */   523,  523,  319,  558,  527,  523,  523,  307,  523,  279,
 /*  1650 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1660 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  426,
 /*  1670 */   523,  523,  210,  214,  219,  393,  424,  523,  400,  326,
 /*  1680 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  1690 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1700 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  1710 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  1720 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1730 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1740 */   523,  523,  319,  551,  527,  523,  523,  307,  523,  279,
 /*  1750 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1760 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  253,
 /*  1770 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  1780 */   328,  327,  297,  523,  523,  301,  523,  343,  523,  465,
 /*  1790 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1800 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  1810 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  1820 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1830 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1840 */   523,  523,  319,  399,  527,  523,  523,  307,  523,  279,
 /*  1850 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1860 */   520,  517,  523,  523,  186,  188,  190,  200,  199,  523,
 /*  1870 */   392,  523,  210,  214,  219,  393,  523,  523,  400,  523,
 /*  1880 */   326,  328,  327,  297,  523,  301,  523,  523,  523,  465,
 /*  1890 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  1900 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  1910 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  1920 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  1930 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  1940 */   523,  523,  319,  213,  527,  523,  523,  307,  523,  279,
 /*  1950 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  1960 */   520,  517,  523,  523,  186,  188,  190,  215,  199,  523,
 /*  1970 */   392,  523,  210,  214,  219,  393,  523,  523,  400,  523,
 /*  1980 */   326,  328,  327,  297,  523,  301,  523,  523,  523,  465,
 /*  1990 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2000 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2010 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2020 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2030 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2040 */   523,  523,  319,  162,  527,  523,  523,  307,  523,  279,
 /*  2050 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2060 */   520,  517,  523,  523,  186,  188,  190,  224,  199,  523,
 /*  2070 */   392,  523,  210,  214,  219,  393,  523,  523,  400,  523,
 /*  2080 */   326,  328,  327,  297,  523,  301,  523,  523,  523,  465,
 /*  2090 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2100 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2110 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2120 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2130 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2140 */   523,  523,  319,  160,  527,  523,  523,  307,  523,  279,
 /*  2150 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2160 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  523,
 /*  2170 */   470,  523,  210,  214,  219,  393,  523,  523,  400,  469,
 /*  2180 */   326,  328,  327,  297,  523,  301,  523,  523,  523,  465,
 /*  2190 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2200 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2210 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2220 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2230 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2240 */   523,  523,  319,  552,  527,  523,  523,  307,  523,  279,
 /*  2250 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2260 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  255,
 /*  2270 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  2280 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  2290 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2300 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2310 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2320 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2330 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2340 */   523,  523,  319,  609,  527,  523,  523,  307,  523,  279,
 /*  2350 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2360 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  390,
 /*  2370 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  2380 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  2390 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2400 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2410 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2420 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2430 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2440 */   523,  523,  319,  607,  527,  523,  523,  307,  523,  279,
 /*  2450 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2460 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  268,
 /*  2470 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  2480 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  2490 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2500 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2510 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2520 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2530 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2540 */   523,  523,  319,  603,  527,  523,  523,  307,  523,  279,
 /*  2550 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2560 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  264,
 /*  2570 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  2580 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  2590 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2600 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2610 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2620 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2630 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2640 */   523,  523,  319,  601,  527,  523,  523,  307,  523,  279,
 /*  2650 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2660 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  274,
 /*  2670 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  2680 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  2690 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2700 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2710 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2720 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2730 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2740 */   523,  523,  319,  534,  527,  523,  523,  307,  523,  279,
 /*  2750 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2760 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  278,
 /*  2770 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  2780 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  2790 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2800 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2810 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2820 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2830 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2840 */   523,  523,  319,  597,  527,  523,  523,  307,  523,  279,
 /*  2850 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2860 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  288,
 /*  2870 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  2880 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  2890 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  2900 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  2910 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  2920 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  2930 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  2940 */   523,  523,  319,  595,  527,  523,  523,  307,  523,  279,
 /*  2950 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  2960 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  262,
 /*  2970 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  2980 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  2990 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  3000 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  3010 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  3020 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  3030 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  3040 */   523,  523,  319,  541,  527,  523,  523,  307,  523,  279,
 /*  3050 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  3060 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  249,
 /*  3070 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  3080 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  3090 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  3100 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  3110 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  3120 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  3130 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  3140 */   523,  523,  319,  591,  527,  523,  523,  307,  523,  279,
 /*  3150 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  3160 */   520,  517,  523,  523,  186,  188,  190,  523,  199,  230,
 /*  3170 */   523,  523,  210,  214,  219,  393,  523,  523,  400,  326,
 /*  3180 */   328,  327,  297,  523,  523,  301,  523,  523,  523,  465,
 /*  3190 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  3200 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  3210 */   523,  523,  523,  285,  523,  523,  523,  523,  523,  523,
 /*  3220 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  3230 */   523,  308,  523,  523,  289,  185,  523,  523,  523,  523,
 /*  3240 */   523,  523,  319,  282,  527,  523,  523,  307,  523,  279,
 /*  3250 */   523,  523,  523,  490,  499,  508,  511,  502,  505,  514,
 /*  3260 */   520,  517,  523,  523,  186,  188,  190,  679,  199,  523,
 /*  3270 */   280,  523,  210,  214,  219,  393,  523,  523,  400,  523,
 /*  3280 */   326,  328,  327,  297,  523,  301,  523,  523,  523,  465,
 /*  3290 */   480,  484,  487,  300,  302,  303,  304,  305,  306,  528,
 /*  3300 */   232,  523,  241,  244,  235,  238,  523,    9,  523,  523,
 /*  3310 */    12,  523,  138,  285,  660,  670,  523,  523,  527,  523,
 /*  3320 */   523,  523,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  3330 */   523,  308,  523,  260,  289,  523,   98,   99,  101,  100,
 /*  3340 */   102,  523,  319,  326,  328,  327,  297,  307,  523,  279,
 /*  3350 */   523,  523,  523,   47,   48,   49,   50,   51,   52,   53,
 /*  3360 */    54,   55,   56,   57,  523,  523,  523,  523,  309,  183,
 /*  3370 */   164,  523,  523,  528,  523,  523,  617,  523,  326,  328,
 /*  3380 */   327,  297,  104,  105,  357,  301,  523,  365,  361,  364,
 /*  3390 */   584,  212,  103,  300,  302,  303,  304,  305,  306,  523,
 /*  3400 */   529,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  3410 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  184,
 /*  3420 */   183,  164,  523,  526,  149,  523,  523,  151,  646,  652,
 /*  3430 */   523,  651,  636,  472,  474,  476,  478,  137,  612,  523,
 /*  3440 */   523,  523,  217,  122,   98,   99,  101,  100,  102,  523,
 /*  3450 */   523,  529,  165,  166,  167,  168,  169,  170,  171,  172,
 /*  3460 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  3470 */   184,  183,  164,  523,  526,  523,  523,  523,  647,  523,
 /*  3480 */   650,  523,  651,  636,  472,  474,  476,  478,  123,  612,
 /*  3490 */   104,  105,  523,  540,  122,  523,  126,  523,  523,  523,
 /*  3500 */   103,  523,  529,  165,  166,  167,  168,  169,  170,  171,
 /*  3510 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
 /*  3520 */   182,  184,  183,  164,  523,  526,  523,  523,   20,  523,
 /*  3530 */    22,  523,  136,  523,   25,  472,  474,  476,  478,  137,
 /*  3540 */   523,  124,  523,  523,  568,  122,   98,   99,  101,  100,
 /*  3550 */   102,  523,  523,  529,  165,  166,  167,  168,  169,  170,
 /*  3560 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  3570 */   181,  182,  184,  183,  164,  523,  526,  523,  523,  523,
 /*  3580 */   523,  523,  523,  523,  523,  523,  472,  474,  476,  478,
 /*  3590 */   523,  523,  104,  105,  523,  608,  523,  523,  523,  523,
 /*  3600 */   108,  523,  103,  523,  529,  165,  166,  167,  168,  169,
 /*  3610 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  3620 */   180,  181,  182,  184,  183,  164,  523,  526,  523,  523,
 /*  3630 */   523,  415,  523,  523,  523,  461,  523,  472,  474,  476,
 /*  3640 */   478,  326,  328,  327,  297,  523,  561,  523,   98,   99,
 /*  3650 */   101,  100,  102,  523,  523,  529,  165,  166,  167,  168,
 /*  3660 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  3670 */   179,  180,  181,  182,  184,  183,  164,  523,  526,  523,
 /*  3680 */   523,  523,  523,  523,  523,  523,  523,  523,  472,  474,
 /*  3690 */   476,  478,  523,  523,  104,  105,  523,  596,  523,  523,
 /*  3700 */   523,  523,  584,  523,  103,  523,  529,  165,  166,  167,
 /*  3710 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  3720 */   178,  179,  180,  181,  182,  184,  183,  164,  523,  526,
 /*  3730 */   523,  523,  523,  415,  523,  523,  523,  423,  523,  472,
 /*  3740 */   474,  476,  478,  326,  328,  327,  297,  523,  574,  523,
 /*  3750 */    98,   99,  101,  100,  102,  523,  523,  529,  165,  166,
 /*  3760 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  3770 */   177,  178,  179,  180,  181,  182,  184,  183,  164,  523,
 /*  3780 */   526,  523,  523,  523,  523,  523,  523,  523,  523,  523,
 /*  3790 */   472,  474,  476,  478,  523,  523,  104,  105,  523,  221,
 /*  3800 */   523,  523,  523,  523,  155,  523,  103,  523,  529,  165,
 /*  3810 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  3820 */   176,  177,  178,  179,  180,  181,  182,  184,  183,  164,
 /*  3830 */   523,  526,  523,  523,  523,  415,  523,  523,  523,  454,
 /*  3840 */   523,  472,  474,  476,  478,  326,  328,  327,  297,  523,
 /*  3850 */   398,  523,   98,   99,  101,  100,  102,  523,  523,  529,
 /*  3860 */   165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  3870 */   175,  176,  177,  178,  179,  180,  181,  182,  184,  183,
 /*  3880 */   164,  523,  526,  523,  523,  523,  523,  523,  523,  523,
 /*  3890 */   523,  523,  472,  474,  476,  478,  523,  523,  104,  105,
 /*  3900 */   523,  602,  523,  523,  523,  523,   26,  523,  103,  523,
 /*  3910 */   529,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  3920 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  184,
 /*  3930 */   183,  164,  523,  526,  523,  523,  523,  309,  523,  523,
 /*  3940 */   523,  523,  523,  472,  474,  476,  478,  326,  328,  327,
 /*  3950 */   297,  523,  197,  523,  523,  523,  360,  361,  364,  523,
 /*  3960 */   523,  529,  165,  166,  167,  168,  169,  170,  171,  172,
 /*  3970 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  3980 */   184,  183,  164,  523,  526,  523,  523,  523,  415,  523,
 /*  3990 */   523,  523,  429,  523,  472,  474,  476,  478,  326,  328,
 /*  4000 */   327,  297,  523,  590,  523,   98,   99,  101,  100,  102,
 /*  4010 */   523,  523,  529,  165,  166,  167,  168,  169,  170,  171,
 /*  4020 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
 /*  4030 */   182,  184,  183,  164,  523,  526,  523,  523,  523,  523,
 /*  4040 */   523,  523,  523,  523,  523,  472,  474,  476,  478,  523,
 /*  4050 */   523,  104,  105,  523,  580,  523,  523,  523,  523,  523,
 /*  4060 */   523,  103,  523,  529,  165,  166,  167,  168,  169,  170,
 /*  4070 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  4080 */   181,  182,  184,  183,  164,  523,  526,  523,  523,  523,
 /*  4090 */   523,  415,  523,  523,  523,  451,  472,  474,  476,  478,
 /*  4100 */   470,  326,  328,  327,  297,  193,  523,  523,  466,  471,
 /*  4110 */   326,  328,  327,  297,  529,  165,  166,  167,  168,  169,
 /*  4120 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  4130 */   180,  181,  182,  184,  183,  164,  523,  526,  523,  523,
 /*  4140 */   523,  415,  523,  523,  523,  414,  523,  472,  474,  476,
 /*  4150 */   478,  326,  328,  327,  297,  523,  161,  523,  523,  523,
 /*  4160 */   523,  523,  523,  523,  523,  529,  165,  166,  167,  168,
 /*  4170 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  4180 */   179,  180,  181,  182,  184,  183,  164,  523,  526,  523,
 /*  4190 */   523,  523,  415,  523,  523,  523,  439,  523,  472,  474,
 /*  4200 */   476,  478,  326,  328,  327,  297,  523,  545,  523,  523,
 /*  4210 */   523,  523,  523,  523,  523,  523,  529,  165,  166,  167,
 /*  4220 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  4230 */   178,  179,  180,  181,  182,  184,  183,  164,  523,  526,
 /*  4240 */   523,  523,  523,  415,  523,  523,  523,  419,  523,  472,
 /*  4250 */   474,  476,  478,  326,  328,  327,  297,  523,  533,  523,
 /*  4260 */   523,  523,  523,  523,  523,  523,  523,  529,  165,  166,
 /*  4270 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  4280 */   177,  178,  179,  180,  181,  182,  184,  183,  164,  523,
 /*  4290 */   526,  523,  523,  523,  415,  523,  523,  523,  447,  523,
 /*  4300 */   472,  474,  476,  478,  326,  328,  327,  297,  523,  555,
 /*  4310 */   523,  523,  523,  523,  523,  523,  523,  523,  529,  165,
 /*  4320 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  4330 */   176,  177,  178,  179,  180,  181,  182,  184,  183,  164,
 /*  4340 */   523,  526,  523,  523,  523,  415,  523,  523,  523,  458,
 /*  4350 */   523,  472,  474,  476,  478,  326,  328,  327,  297,  523,
 /*  4360 */   209,  523,   98,   99,  101,  100,  102,  523,  523,  529,
 /*  4370 */   165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  4380 */   175,  176,  177,  178,  179,  180,  181,  182,  184,  183,
 /*  4390 */   164,  523,  526,  523,  523,  523,  523,  523,  523,  523,
 /*  4400 */   523,  523,  472,  474,  476,  478,  523,  523,  104,  105,
 /*  4410 */   523,  148,  615,  523,  131,  523,  523,  523,  103,  523,
 /*  4420 */   163,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  4430 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  184,
 /*  4440 */   523,  523,  523,  526,  232,  523,  241,  244,  235,  238,
 /*  4450 */   247,  523,  523,  472,  474,  476,  478,  285,  523,  523,
 /*  4460 */   326,  328,  327,  297,  523,  523,  523,  523,  523,  229,
 /*  4470 */   293,  298,  312,  226,  523,  308,  523,  232,  289,  241,
 /*  4480 */   244,  235,  238,  523,  523,  523,  319,  523,  309,  523,
 /*  4490 */   285,  307,  523,  279,  523,  523,  523,  523,  326,  328,
 /*  4500 */   327,  297,  229,  293,  298,  312,  226,  379,  308,  363,
 /*  4510 */   523,  289,  523,  523,  523,  523,  523,  523,  523,  319,
 /*  4520 */     9,  530,  396,   12,  307,  138,  279,  660,  670,  301,
 /*  4530 */   523,  527,  326,  328,  327,  297,  523,  300,  302,  303,
 /*  4540 */   304,  305,  306,  273,  269,  271,  523,  523,  228,  234,
 /*  4550 */   240,  243,  246,  248,  237,  231,  257,  259,  267,  261,
 /*  4560 */   263,  265,  301,  523,  523,  523,  523,  523,  254,  250,
 /*  4570 */   300,  302,  303,  304,  305,  306,  523,  523,  232,  523,
 /*  4580 */   241,  244,  235,  238,  415,  523,  528,  523,  443,  523,
 /*  4590 */   523,  285,  523,  523,  326,  328,  327,  297,  523,  523,
 /*  4600 */   523,  523,  523,  229,  293,  298,  312,  226,  348,  308,
 /*  4610 */   523,  232,  289,  241,  244,  235,  238,  523,  270,  523,
 /*  4620 */   319,  523,  523,  523,  285,  307,  523,  279,  326,  328,
 /*  4630 */   327,  297,  523,  523,  523,  523,  229,  293,  298,  312,
 /*  4640 */   226,  368,  308,  523,  523,  289,  523,  523,  523,  523,
 /*  4650 */   523,  523,  523,  319,  523,  523,  299,  523,  307,  523,
 /*  4660 */   279,  523,  523,  301,  523,  523,  326,  328,  327,  297,
 /*  4670 */   523,  300,  302,  303,  304,  305,  306,  523,  269,  271,
 /*  4680 */   523,  523,  228,  234,  240,  243,  246,  248,  237,  231,
 /*  4690 */   257,  259,  267,  261,  263,  265,  301,  523,  523,  523,
 /*  4700 */   523,  523,  254,  250,  300,  302,  303,  304,  305,  306,
 /*  4710 */   227,  523,  232,  523,  241,  244,  235,  238,  296,  523,
 /*  4720 */   326,  328,  327,  297,  523,  285,  523,  523,  326,  328,
 /*  4730 */   327,  297,  523,  523,  523,  523,  523,  229,  293,  298,
 /*  4740 */   312,  226,  315,  308,  523,  276,  289,  232,  523,  241,
 /*  4750 */   244,  235,  238,  245,  319,  326,  328,  327,  297,  307,
 /*  4760 */   285,  279,  523,  326,  328,  327,  297,  523,  523,  523,
 /*  4770 */   523,  523,  229,  293,  298,  312,  226,  523,  308,  523,
 /*  4780 */   523,  289,  523,  523,  523,  523,  523,  523,  523,  319,
 /*  4790 */   386,  523,  523,  523,  307,  523,  279,  301,  523,  456,
 /*  4800 */   326,  328,  327,  297,  523,  300,  302,  303,  304,  305,
 /*  4810 */   306,  523,  523,  228,  234,  240,  243,  246,  248,  237,
 /*  4820 */   231,  257,  259,  267,  261,  263,  265,  523,  523,  523,
 /*  4830 */   523,  523,  301,  254,  250,  523,  523,  523,  523,  523,
 /*  4840 */   300,  302,  303,  304,  305,  306,  481,  523,  232,  523,
 /*  4850 */   241,  244,  235,  238,  388,  523,  326,  328,  327,  297,
 /*  4860 */   523,  285,  523,  523,  326,  328,  327,  297,  523,  523,
 /*  4870 */   523,  523,  523,  229,  293,  298,  312,  226,  342,  308,
 /*  4880 */   523,  232,  289,  241,  244,  235,  238,  523,  523,  523,
 /*  4890 */   319,  523,  242,  523,  285,  307,  523,  279,  523,  523,
 /*  4900 */   455,  416,  326,  328,  327,  297,  229,  293,  298,  312,
 /*  4910 */   226,  372,  308,  523,  406,  289,  523,  523,  523,  523,
 /*  4920 */   523,  523,  523,  319,  387,  523,  523,  523,  307,  523,
 /*  4930 */   279,  523,  523,  301,  326,  328,  327,  297,  523,  523,
 /*  4940 */   523,  300,  302,  303,  304,  305,  306,  523,  407,  408,
 /*  4950 */   409,  410,  411,  412,  523,  444,  462,  463,  523,  485,
 /*  4960 */   523,  523,  523,  523,  523,  523,  301,  523,  523,  326,
 /*  4970 */   328,  327,  297,  523,  300,  302,  303,  304,  305,  306,
 /*  4980 */   239,  523,  232,  523,  241,  244,  235,  238,  266,  523,
 /*  4990 */   326,  328,  327,  297,  523,  285,  523,  523,  326,  328,
 /*  5000 */   327,  297,  523,  523,  523,  523,  523,  229,  293,  298,
 /*  5010 */   312,  226,  335,  308,  523,  232,  289,  241,  244,  235,
 /*  5020 */   238,  523,  258,  523,  319,  523,  488,  523,  285,  307,
 /*  5030 */   523,  279,  326,  328,  327,  297,  326,  328,  327,  297,
 /*  5040 */   229,  293,  298,  312,  226,   41,  308,  523,  284,  289,
 /*  5050 */   483,  523,  523,  523,  523,  523,  523,  319,  326,  328,
 /*  5060 */   327,  297,  307,  523,  279,  523,  523,  301,  523,  523,
 /*  5070 */   523,  523,  523,  523,  523,  300,  302,  303,  304,  305,
 /*  5080 */   306,  523,  523,   33,  523,   47,   48,   49,   50,   51,
 /*  5090 */    52,   53,   54,   55,   56,   57,  523,  523,  523,  523,
 /*  5100 */   301,  523,  523,  523,  523,  523,  523,  523,  300,  302,
 /*  5110 */   303,  304,  305,  306,  523,  523,  232,  523,  241,  244,
 /*  5120 */   235,  238,  236,   98,   99,  101,  100,  102,  523,  285,
 /*  5130 */   523,  523,  326,  328,  327,  297,  523,  523,  523,  523,
 /*  5140 */   523,  229,  293,  298,  312,  226,  353,  308,  523,  232,
 /*  5150 */   289,  241,  244,  235,  238,  523,  523,  523,  319,  523,
 /*  5160 */   523,  523,  285,  307,  523,  279,  523,  523,  523,  104,
 /*  5170 */   105,  523,  711,  649,  229,  293,  298,  312,  226,  103,
 /*  5180 */   308,  523,  391,  289,  523,   98,   99,  101,  100,  102,
 /*  5190 */   523,  319,  326,  328,  327,  297,  307,  523,  279,  523,
 /*  5200 */   523,  301,  523,  523,  523,  523,  523,  523,  523,  300,
 /*  5210 */   302,  303,  304,  305,  306,   98,   99,  101,  100,  102,
 /*  5220 */   523,  523,  523,  523,  523,  637,  542,  523,  523,  523,
 /*  5230 */   523,  104,  105,  523,  301,  523,  523,  523,  523,  155,
 /*  5240 */   523,  103,  300,  302,  303,  304,  305,  306,  523,  523,
 /*  5250 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  5260 */   523,  104,  105,  285,  715,  125,  523,  131,  523,  523,
 /*  5270 */   523,  103,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  5280 */   523,  308,  523,  233,  289,  232,  523,  241,  244,  235,
 /*  5290 */   238,  523,  319,  326,  328,  327,  297,  307,  285,  279,
 /*  5300 */   523,  523,  421,  523,  523,  523,  523,  523,  523,   94,
 /*  5310 */   229,  293,  298,  312,  226,  523,  308,  523,  523,  310,
 /*  5320 */   523,  523,  523,  523,  523,  523,   29,  319,  389,  523,
 /*  5330 */    68,  523,  307,  523,  279,  301,  523,  356,  326,  328,
 /*  5340 */   327,  297,  523,  300,  302,  303,  304,  305,  306,   47,
 /*  5350 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  5360 */    98,   99,  101,  100,  102,  523,  523,  523,  523,  523,
 /*  5370 */   355,  523,  523,  523,  523,  523,  523,  523,  354,  302,
 /*  5380 */   303,  304,  305,  306,  538,  523,  232,  523,  241,  244,
 /*  5390 */   235,  238,  523,  523,  326,  328,  327,  297,  523,  285,
 /*  5400 */   523,  523,  523,  523,  523,  523,  104,  105,  523,  717,
 /*  5410 */   106,  229,  293,  298,  312,  226,  103,  308,  523,  232,
 /*  5420 */   289,  241,  244,  235,  238,  523,  523,  523,  319,  523,
 /*  5430 */   523,  523,  285,  307,  523,  279,  523,  523,  449,  523,
 /*  5440 */   523,  523,  523,  523,  229,  293,  298,  312,  226,  325,
 /*  5450 */   308,  523,  523,  289,  523,  523,  523,  523,  523,  523,
 /*  5460 */   523,  319,  272,  523,  523,  523,  307,  523,  279,  523,
 /*  5470 */   523,  301,  326,  328,  327,  297,  523,  523,  523,  300,
 /*  5480 */   302,  303,  304,  305,  306,   98,   99,  101,  100,  102,
 /*  5490 */   523,  523,  523,  523,  523,  523,  543,  523,  523,  523,
 /*  5500 */   523,  523,  523,  523,  301,  523,  326,  328,  327,  297,
 /*  5510 */   523,  523,  300,  302,  303,  304,  305,  306,  523,  523,
 /*  5520 */   232,  523,  241,  244,  235,  238,  523,  523,  523,  523,
 /*  5530 */   523,  104,  105,  285,  707,  615,  523,  131,  523,  523,
 /*  5540 */   523,  103,  523,  523,  523,  229,  293,  298,  312,  226,
 /*  5550 */   385,  308,  523,  232,  289,  241,  244,  235,  238,  523,
 /*  5560 */   523,  523,  319,  523,  523,  523,  285,  307,  523,  279,
 /*  5570 */   523,  523,  523,  523,  523,  523,  523,  523,  229,  293,
 /*  5580 */   298,  312,  226,   94,  308,  523,  523,  289,  523,  523,
 /*  5590 */   523,  523,  523,  523,  523,  319,  523,  523,  523,  523,
 /*  5600 */   307,  523,  279,  523,   68,  301,  523,  523,  523,  523,
 /*  5610 */   523,  523,  523,  300,  302,  303,  304,  305,  306,  523,
 /*  5620 */   523,  523,  523,   47,   48,   49,   50,   51,   52,   53,
 /*  5630 */    54,   55,   56,   57,  523,  523,  523,  523,  301,  523,
 /*  5640 */   523,  523,  523,  523,  523,  523,  300,  302,  303,  304,
 /*  5650 */   305,  306,  523,  523,  232,  523,  241,  244,  235,  238,
 /*  5660 */   523,  523,  523,  523,  523,  523,  523,  285,  523,  523,
 /*  5670 */   523,  523,  523,  523,  523,  523,  523,  523,  523,  229,
 /*  5680 */   293,  298,  312,  226,  523,  308,  523,  523,  310,  523,
 /*  5690 */   523,  523,  523,  523,  523,  523,  319,  523,  523,  523,
 /*  5700 */   523,  307,  523,  279,  523,  275,  523,  523,  523,  523,
 /*  5710 */   273,  269,  271,  523,  523,  228,  234,  240,  243,  246,
 /*  5720 */   248,  237,  231,  257,  259,  267,  261,  263,  265,   98,
 /*  5730 */    99,  101,  100,  102,  523,  254,  250,  523,  523,  355,
 /*  5740 */   523,  523,  523,  523,  523,  523,  523,  354,  302,  303,
 /*  5750 */   304,  305,  306,  523,  523,  523,  523,  523,  523,  523,
 /*  5760 */   523,  523,  523,  523,  523,  523,  523,  523,  523,  523,
 /*  5770 */   523,  523,  523,  523,  277,  104,  105,  523,   19,  125,
 /*  5780 */   523,  131,  523,  275,  523,  103,  523,  523,  273,  269,
 /*  5790 */   271,  523,  523,  228,  234,  240,  243,  246,  248,  237,
 /*  5800 */   231,  257,  259,  267,  261,  263,  265,  523,  523,  523,
 /*  5810 */   523,  523,  523,  254,  250,  275,  523,  523,  523,  523,
 /*  5820 */   273,  269,  271,  523,  397,  228,  234,  240,  243,  246,
 /*  5830 */   248,  237,  231,  257,  259,  267,  261,  263,  265,  523,
 /*  5840 */   523,  523,  275,  523,  523,  254,  250,  273,  269,  271,
 /*  5850 */   523,  523,  228,  234,  240,  243,  246,  248,  237,  231,
 /*  5860 */   257,  259,  267,  261,  263,  265,  256,  523,  523,  275,
 /*  5870 */   523,  281,  254,  250,  273,  269,  271,  523,  523,  228,
 /*  5880 */   234,  240,  243,  246,  248,  237,  231,  257,  259,  267,
 /*  5890 */   261,  263,  265,  523,  523,  523,  275,  523,  523,  254,
 /*  5900 */   250,  273,  269,  271,  523,  523,  228,  234,  240,  243,
 /*  5910 */   246,  248,  237,  231,  257,  259,  267,  261,  263,  265,
 /*  5920 */   884,  523,  523,  523,  523,  523,  254,  250,  275,  523,
 /*  5930 */   489,  523,  523,  273,  269,  271,  523,  523,  228,  234,
 /*  5940 */   240,  243,  246,  248,  237,  231,  257,  259,  267,  261,
 /*  5950 */   263,  265,  523,  523,  523,  523,  523,  523,  254,  250,
 /*  5960 */   275,  523,  486,  523,  523,  273,  269,  271,  523,  523,
 /*  5970 */   228,  234,  240,  243,  246,  248,  237,  231,  257,  259,
 /*  5980 */   267,  261,  263,  265,  523,  523,  523,  523,  523,  523,
 /*  5990 */   254,  250,  275,  523,  482,  523,  523,  273,  269,  271,
 /*  6000 */   523,  523,  228,  234,  240,  243,  246,  248,  237,  231,
 /*  6010 */   257,  259,  267,  261,  263,  265,  523,  523,  523,  523,
 /*  6020 */   523,  523,  254,  250,  275,  523,  523,  523,  523,  273,
 /*  6030 */   269,  271,  523,  532,  228,  234,  240,  243,  246,  248,
 /*  6040 */   237,  231,  257,  259,  267,  261,  263,  265,  523,  523,
 /*  6050 */   523,  523,  523,  523,  254,  250,  275,  523,  523,  523,
 /*  6060 */   523,  273,  269,  271,  523,  544,  228,  234,  240,  243,
 /*  6070 */   246,  248,  237,  231,  257,  259,  267,  261,  263,  265,
 /*  6080 */   523,  523,  523,  523,  523,  523,  254,  250,  275,  523,
 /*  6090 */   523,  523,  523,  273,  269,  271,   94,  539,  228,  234,
 /*  6100 */   240,  243,  246,  248,  237,  231,  257,  259,  267,  261,
 /*  6110 */   263,  265,  523,  111,   94,  523,  523,   68,  254,  250,
 /*  6120 */   523,  523,  523,  523,  523,  523,  523,  523,  523,  523,
 /*  6130 */    94,  587,  523,  523,  523,   68,   47,   48,   49,   50,
 /*  6140 */    51,   52,   53,   54,   55,   56,   57,  158,  523,  523,
 /*  6150 */   523,   68,   41,  523,   47,   48,   49,   50,   51,   52,
 /*  6160 */    53,   54,   55,   56,   57,   98,   99,  101,  100,  102,
 /*  6170 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  6180 */    57,   98,   99,  101,  100,  102,  523,  523,  523,  523,
 /*  6190 */   523,  523,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  6200 */    55,   56,   57,   98,   99,  101,  100,  102,  523,  523,
 /*  6210 */   523,  104,  105,  523,  706,  615,  523,  131,  523,  523,
 /*  6220 */   523,  103,  523,  523,  523,  523,  523,  104,  105,  523,
 /*  6230 */   710,  613,  523,  131,  523,  523,  523,  103,  523,   98,
 /*  6240 */    99,  101,  100,  102,   98,   99,  101,  100,  102,  104,
 /*  6250 */   105,  523,  712,  582,  523,  523,  523,  523,  523,  103,
 /*  6260 */   523,   98,   99,  101,  100,  102,   98,   99,  101,  100,
 /*  6270 */   102,  523,  523,  523,  523,  523,  523,  523,  523,  523,
 /*  6280 */   523,  523,  523,  523,  523,  104,  105,  523,  716,  106,
 /*  6290 */   104,  105,  523,  714,  582,  103,  523,  523,  523,  523,
 /*  6300 */   103,  523,   98,   99,  101,  100,  102,  104,  105,  523,
 /*  6310 */   709,  582,  104,  105,  523,  708,  582,  103,  523,  523,
 /*  6320 */   523,  523,  103,  523,  523,  523,  523,  523,  523,  523,
 /*  6330 */   523,  523,  523,  523,  523,  523,  523,  523,  523,  523,
 /*  6340 */   523,  523,  523,  523,  523,  523,  523,  523,  104,  105,
 /*  6350 */   523,  713,  582,  523,  523,  523,  523,  523,  103,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,   56,    9,   10,   11,   12,  138,    0,  140,  141,
 /*    10 */   142,  143,  144,   20,  108,  109,  110,  111,  112,  113,
 /*    20 */   114,  115,  116,   42,   41,   32,   33,   34,   35,   36,
 /*    30 */    49,   38,   49,   52,   41,   42,  124,  125,   93,  127,
 /*    40 */   128,  128,   49,   50,   51,  133,  134,   54,  135,   56,
 /*    50 */    56,  139,  139,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   68,   42,   70,   71,   72,   73,   38,   75,   49,
 /*    70 */   136,   77,   79,   80,   81,   82,  142,  143,   85,  145,
 /*    80 */   119,   52,  121,  142,  143,   92,  145,   93,   41,   96,
 /*    90 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   100 */     7,   54,    9,   10,   11,   12,  138,   42,  140,  141,
 /*   110 */   142,  143,  144,   20,   49,   86,   87,   88,   89,   90,
 /*   120 */    91,   38,   39,   94,   95,   32,   33,   34,   35,   36,
 /*   130 */   172,   38,  174,  175,   41,   42,   29,   30,   31,   38,
 /*   140 */    39,   42,   49,   50,   51,   38,   39,   54,   49,   56,
 /*   150 */    41,   52,    6,   60,   61,   62,   63,   64,   65,   66,
 /*   160 */    67,   68,   49,   70,   71,   72,   73,  172,   75,  174,
 /*   170 */   175,    6,   79,   80,   81,   82,  128,  136,   85,  119,
 /*   180 */    52,  121,  134,  142,  143,   92,  145,  139,   42,   96,
 /*   190 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   200 */     7,   92,    9,   10,   11,   12,  138,   42,  140,  141,
 /*   210 */   142,  143,  144,   20,   86,   87,   88,   89,   90,   91,
 /*   220 */    41,  141,  142,  143,  144,   32,   33,   34,   35,   36,
 /*   230 */    41,   38,   56,   54,   41,   42,  111,  112,  113,  114,
 /*   240 */   115,  116,   49,   50,   51,    6,   42,   54,  172,   56,
 /*   250 */   174,  175,   39,   60,   61,   62,   63,   64,   65,   66,
 /*   260 */    67,   68,   49,   70,   71,   72,   73,   41,   75,   93,
 /*   270 */     6,   92,   79,   80,   81,   82,   50,   51,   85,  100,
 /*   280 */   101,  102,  103,  104,  105,   92,  170,  171,   49,   96,
 /*   290 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   300 */     7,   37,    9,   10,   11,   12,   26,   27,   28,   29,
 /*   310 */    30,   31,  172,   20,  136,  175,    6,   41,   38,   39,
 /*   320 */   142,  143,   50,  145,  117,   32,   33,   34,   35,   36,
 /*   330 */    46,   38,  136,   49,   41,   42,   41,  125,  142,  143,
 /*   340 */   128,  145,   49,   50,   51,  133,  134,   54,   76,   56,
 /*   350 */    78,  139,   42,   60,   61,   62,   63,   64,   65,   66,
 /*   360 */    67,   68,   49,   70,   71,   72,   73,   38,   75,  128,
 /*   370 */     6,    6,   79,   80,   81,   82,  117,   44,   85,   46,
 /*   380 */   139,   52,   49,  139,   39,   92,   50,   42,    6,   96,
 /*   390 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   400 */     7,   37,    9,   10,   11,   12,  172,  172,  166,  175,
 /*   410 */   175,  169,   76,   20,   78,   86,   87,   88,   89,   90,
 /*   420 */    91,  179,  180,  181,  182,   32,   33,   34,   35,   36,
 /*   430 */    39,   38,   50,   42,   41,   42,  125,   41,  127,  128,
 /*   440 */   128,   56,   49,   50,   51,  134,    6,   54,   83,   56,
 /*   450 */   139,  139,   41,   60,   61,   62,   63,   64,   65,   66,
 /*   460 */    67,   68,   51,   70,   71,   72,   73,   38,   75,   39,
 /*   470 */     6,    6,   79,   80,   81,   82,    6,   37,   85,   49,
 /*   480 */    41,   52,  183,  184,   39,   92,    6,   42,   92,   96,
 /*   490 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   500 */     7,   37,    9,   10,   11,   12,  138,   37,  140,  141,
 /*   510 */   142,  143,  144,   20,    6,   86,   87,   88,   89,   90,
 /*   520 */    91,    6,   42,   39,   59,   32,   33,   34,   35,   36,
 /*   530 */    42,   38,   36,   49,   41,   42,   44,   41,   46,  183,
 /*   540 */   184,   49,   49,   50,   51,   37,    6,   54,   50,   56,
 /*   550 */   183,  184,   37,   60,   61,   62,   63,   64,   65,   66,
 /*   560 */    67,   68,   44,   70,   71,   72,   73,   49,   75,  169,
 /*   570 */     6,   36,   79,   80,   81,   82,    6,   37,   85,  179,
 /*   580 */   180,  181,  182,   41,    6,   92,  186,  187,    6,   96,
 /*   590 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   600 */     7,   37,    9,   10,   11,   12,  138,   37,  140,  141,
 /*   610 */   142,  143,  144,   20,  138,   37,  140,  141,  142,  143,
 /*   620 */   144,  119,   41,  121,   42,   32,   33,   34,   35,   36,
 /*   630 */    49,   38,  183,  184,   41,   42,    6,   41,   16,  126,
 /*   640 */   128,  128,   49,   50,   51,   49,  133,   54,  135,   56,
 /*   650 */    59,  139,  139,   60,   61,   62,   63,   64,   65,   66,
 /*   660 */    67,   68,   21,   70,   71,   72,   73,   37,   75,  169,
 /*   670 */     6,   21,   79,   80,   81,   82,   41,   52,   85,  179,
 /*   680 */   180,  181,  182,    6,   49,   92,  186,  187,   38,   96,
 /*   690 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   700 */     7,    6,    9,   10,   11,   12,   42,  183,  184,  183,
 /*   710 */   184,   42,  138,   20,  140,  141,  142,  143,  144,   42,
 /*   720 */   167,  168,  183,  184,  129,   32,   33,   34,   35,   36,
 /*   730 */    39,   38,   93,   42,   41,   42,    6,   42,  183,  184,
 /*   740 */     6,    6,   49,   50,   51,  183,  184,   54,    6,   56,
 /*   750 */   183,  184,    6,   60,   61,   62,   63,   64,   65,   66,
 /*   760 */    67,   68,   54,   70,   71,   72,   73,   37,   75,  169,
 /*   770 */     6,   52,   79,   80,   81,   82,   42,   42,   85,  179,
 /*   780 */   180,  181,  182,   37,   42,   92,  186,  187,  129,   96,
 /*   790 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   800 */     7,   39,    9,   10,   11,   12,   42,  131,  132,  169,
 /*   810 */    41,   49,  138,   20,  140,  141,  142,  143,  144,  179,
 /*   820 */   180,  181,  182,   52,   41,   32,   33,   34,   35,   36,
 /*   830 */   129,   38,   44,   52,   41,   42,  129,   49,   41,   21,
 /*   840 */    41,   56,   49,   50,   51,   42,   42,   54,   54,   56,
 /*   850 */    52,  129,   42,   60,   61,   62,   63,   64,   65,   66,
 /*   860 */    67,   68,   54,   70,   71,   72,   73,   52,   75,  169,
 /*   870 */   129,   42,   79,   80,   81,   82,   41,  118,   85,  179,
 /*   880 */   180,  181,  182,   41,  118,   92,  186,  187,   41,   96,
 /*   890 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   900 */     7,   50,    9,   10,   11,   12,   41,  118,   56,   42,
 /*   910 */    42,   49,   74,   20,   77,   49,  168,   42,   49,  129,
 /*   920 */    49,   49,   49,   80,   37,   32,   33,   34,   35,   36,
 /*   930 */    56,   38,   41,   21,   41,   42,   56,   41,    6,   56,
 /*   940 */    41,   56,   49,   50,   51,   50,   41,   54,   50,   56,
 /*   950 */    56,   41,   50,   60,   61,   62,   63,   64,   65,   66,
 /*   960 */    67,   68,   56,   70,   71,   72,   73,   77,   75,  169,
 /*   970 */    50,   56,   79,   80,   81,   82,   50,   56,   85,  179,
 /*   980 */   180,  181,  182,   41,   77,   92,  186,  187,   77,   96,
 /*   990 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1000 */     7,   41,    9,   10,   11,   12,  172,  171,   59,   50,
 /*  1010 */    41,  172,   50,   20,   42,   42,  172,  172,  172,   42,
 /*  1020 */    42,   41,   52,   41,  184,   32,   33,   34,   35,   36,
 /*  1030 */   129,   38,   80,   83,   41,   42,   42,   74,   77,   49,
 /*  1040 */    49,   49,   49,   50,   51,   41,   49,   54,  130,   56,
 /*  1050 */    42,   56,  129,   60,   61,   62,   63,   64,   65,   66,
 /*  1060 */    67,   68,   42,   70,   71,   72,   73,  130,   75,  169,
 /*  1070 */    42,  132,   79,   80,   81,   82,   41,  130,   85,  179,
 /*  1080 */   180,  181,  182,   42,  129,   92,  186,  187,  130,   96,
 /*  1090 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1100 */     7,   42,    9,   10,   11,   12,   42,  121,   45,   41,
 /*  1110 */    41,   49,   45,   20,  118,   41,  118,  118,   41,  118,
 /*  1120 */   118,  118,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1130 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1140 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1150 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1160 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  1170 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  1180 */   180,  181,  182,  191,  191,   92,  186,  187,  191,   96,
 /*  1190 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1200 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  1210 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  1220 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1230 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1240 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1250 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1260 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  1270 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  1280 */   180,  181,  182,  191,  191,   92,  186,  187,  191,   96,
 /*  1290 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1300 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  1310 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  1320 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1330 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1340 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1350 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1360 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  1370 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  1380 */   180,  181,  182,  191,  191,   92,  186,  187,  191,   96,
 /*  1390 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1400 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  1410 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  1420 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1430 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1440 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1450 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1460 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  1470 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  1480 */   180,  181,  182,  191,  191,   92,  186,  187,  191,   96,
 /*  1490 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1500 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  1510 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  1520 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1530 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1540 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1550 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1560 */    67,   68,  191,   70,   71,   72,   73,  166,   75,  191,
 /*  1570 */   169,  191,   79,   80,   81,   82,  191,  191,   85,  191,
 /*  1580 */   179,  180,  181,  182,  191,   92,  191,  191,  191,   96,
 /*  1590 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1600 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  1610 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  1620 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1630 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1640 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1650 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1660 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  1670 */   191,  191,   79,   80,   81,   82,  176,  191,   85,  179,
 /*  1680 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  1690 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1700 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  1710 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  1720 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1730 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1740 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1750 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1760 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  1770 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  1780 */   180,  181,  182,  191,  191,   92,  191,  187,  191,   96,
 /*  1790 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1800 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  1810 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  1820 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1830 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1840 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1850 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1860 */    67,   68,  191,   70,   71,   72,   73,  166,   75,  191,
 /*  1870 */   169,  191,   79,   80,   81,   82,  191,  191,   85,  191,
 /*  1880 */   179,  180,  181,  182,  191,   92,  191,  191,  191,   96,
 /*  1890 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1900 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  1910 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  1920 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  1930 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  1940 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  1950 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  1960 */    67,   68,  191,   70,   71,   72,   73,  166,   75,  191,
 /*  1970 */   169,  191,   79,   80,   81,   82,  191,  191,   85,  191,
 /*  1980 */   179,  180,  181,  182,  191,   92,  191,  191,  191,   96,
 /*  1990 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2000 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2010 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2020 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2030 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2040 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2050 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2060 */    67,   68,  191,   70,   71,   72,   73,  166,   75,  191,
 /*  2070 */   169,  191,   79,   80,   81,   82,  191,  191,   85,  191,
 /*  2080 */   179,  180,  181,  182,  191,   92,  191,  191,  191,   96,
 /*  2090 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2100 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2110 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2120 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2130 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2140 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2150 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2160 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  191,
 /*  2170 */   169,  191,   79,   80,   81,   82,  191,  191,   85,  178,
 /*  2180 */   179,  180,  181,  182,  191,   92,  191,  191,  191,   96,
 /*  2190 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2200 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2210 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2220 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2230 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2240 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2250 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2260 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  2270 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  2280 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  2290 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2300 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2310 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2320 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2330 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2340 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2350 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2360 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  2370 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  2380 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  2390 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2400 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2410 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2420 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2430 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2440 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2450 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2460 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  2470 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  2480 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  2490 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2500 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2510 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2520 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2530 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2540 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2550 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2560 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  2570 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  2580 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  2590 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2600 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2610 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2620 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2630 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2640 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2650 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2660 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  2670 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  2680 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  2690 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2700 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2710 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2720 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2730 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2740 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2750 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2760 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  2770 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  2780 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  2790 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2800 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2810 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2820 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2830 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2840 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2850 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2860 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  2870 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  2880 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  2890 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2900 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  2910 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  2920 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  2930 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  2940 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  2950 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  2960 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  2970 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  2980 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  2990 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3000 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  3010 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  3020 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  3030 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  3040 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  3050 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  3060 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  3070 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  3080 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  3090 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3100 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  3110 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  3120 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  3130 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  3140 */   191,  191,   49,   50,   51,  191,  191,   54,  191,   56,
 /*  3150 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  3160 */    67,   68,  191,   70,   71,   72,   73,  191,   75,  169,
 /*  3170 */   191,  191,   79,   80,   81,   82,  191,  191,   85,  179,
 /*  3180 */   180,  181,  182,  191,  191,   92,  191,  191,  191,   96,
 /*  3190 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3200 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  3210 */   191,  191,  191,   20,  191,  191,  191,  191,  191,  191,
 /*  3220 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  3230 */   191,   38,  191,  191,   41,   42,  191,  191,  191,  191,
 /*  3240 */   191,  191,   49,  142,   51,  191,  191,   54,  191,   56,
 /*  3250 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  3260 */    67,   68,  191,   70,   71,   72,   73,    0,   75,  191,
 /*  3270 */   169,  191,   79,   80,   81,   82,  191,  191,   85,  191,
 /*  3280 */   179,  180,  181,  182,  191,   92,  191,  191,  191,   96,
 /*  3290 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3300 */     7,  191,    9,   10,   11,   12,  191,   40,  191,  191,
 /*  3310 */    43,  191,   45,   20,   47,   48,  191,  191,   51,  191,
 /*  3320 */   191,  191,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  3330 */   191,   38,  191,  169,   41,  191,    1,    2,    3,    4,
 /*  3340 */     5,  191,   49,  179,  180,  181,  182,   54,  191,   56,
 /*  3350 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  3360 */    67,   68,   69,   70,  191,  191,  191,  191,  169,  115,
 /*  3370 */   116,  191,  191,  106,  191,  191,   41,  191,  179,  180,
 /*  3380 */   181,  182,   47,   48,  185,   92,  191,  188,  189,  190,
 /*  3390 */    55,  137,   57,  100,  101,  102,  103,  104,  105,  191,
 /*  3400 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3410 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3420 */   115,  116,  191,  169,  120,  191,  191,  123,  124,  125,
 /*  3430 */   191,  127,  128,  179,  180,  181,  182,  133,  134,  191,
 /*  3440 */   191,  191,  137,  139,    1,    2,    3,    4,    5,  191,
 /*  3450 */   191,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  3460 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  3470 */   165,  115,  116,  191,  169,  191,  191,  191,  123,  191,
 /*  3480 */   125,  191,  127,  128,  179,  180,  181,  182,  133,  134,
 /*  3490 */    47,   48,  191,  137,  139,  191,   53,  191,  191,  191,
 /*  3500 */    57,  191,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  3510 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3520 */   164,  165,  115,  116,  191,  169,  191,  191,  122,  191,
 /*  3530 */   124,  191,  126,  191,  128,  179,  180,  181,  182,  133,
 /*  3540 */   191,  135,  191,  191,  137,  139,    1,    2,    3,    4,
 /*  3550 */     5,  191,  191,  146,  147,  148,  149,  150,  151,  152,
 /*  3560 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  3570 */   163,  164,  165,  115,  116,  191,  169,  191,  191,  191,
 /*  3580 */   191,  191,  191,  191,  191,  191,  179,  180,  181,  182,
 /*  3590 */   191,  191,   47,   48,  191,  137,  191,  191,  191,  191,
 /*  3600 */    55,  191,   57,  191,  146,  147,  148,  149,  150,  151,
 /*  3610 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  3620 */   162,  163,  164,  165,  115,  116,  191,  169,  191,  191,
 /*  3630 */   191,  169,  191,  191,  191,  173,  191,  179,  180,  181,
 /*  3640 */   182,  179,  180,  181,  182,  191,  137,  191,    1,    2,
 /*  3650 */     3,    4,    5,  191,  191,  146,  147,  148,  149,  150,
 /*  3660 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3670 */   161,  162,  163,  164,  165,  115,  116,  191,  169,  191,
 /*  3680 */   191,  191,  191,  191,  191,  191,  191,  191,  179,  180,
 /*  3690 */   181,  182,  191,  191,   47,   48,  191,  137,  191,  191,
 /*  3700 */   191,  191,   55,  191,   57,  191,  146,  147,  148,  149,
 /*  3710 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3720 */   160,  161,  162,  163,  164,  165,  115,  116,  191,  169,
 /*  3730 */   191,  191,  191,  169,  191,  191,  191,  173,  191,  179,
 /*  3740 */   180,  181,  182,  179,  180,  181,  182,  191,  137,  191,
 /*  3750 */     1,    2,    3,    4,    5,  191,  191,  146,  147,  148,
 /*  3760 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  3770 */   159,  160,  161,  162,  163,  164,  165,  115,  116,  191,
 /*  3780 */   169,  191,  191,  191,  191,  191,  191,  191,  191,  191,
 /*  3790 */   179,  180,  181,  182,  191,  191,   47,   48,  191,  137,
 /*  3800 */   191,  191,  191,  191,   55,  191,   57,  191,  146,  147,
 /*  3810 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3820 */   158,  159,  160,  161,  162,  163,  164,  165,  115,  116,
 /*  3830 */   191,  169,  191,  191,  191,  169,  191,  191,  191,  173,
 /*  3840 */   191,  179,  180,  181,  182,  179,  180,  181,  182,  191,
 /*  3850 */   137,  191,    1,    2,    3,    4,    5,  191,  191,  146,
 /*  3860 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  3870 */   157,  158,  159,  160,  161,  162,  163,  164,  165,  115,
 /*  3880 */   116,  191,  169,  191,  191,  191,  191,  191,  191,  191,
 /*  3890 */   191,  191,  179,  180,  181,  182,  191,  191,   47,   48,
 /*  3900 */   191,  137,  191,  191,  191,  191,   55,  191,   57,  191,
 /*  3910 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3920 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3930 */   115,  116,  191,  169,  191,  191,  191,  169,  191,  191,
 /*  3940 */   191,  191,  191,  179,  180,  181,  182,  179,  180,  181,
 /*  3950 */   182,  191,  137,  191,  191,  191,  188,  189,  190,  191,
 /*  3960 */   191,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  3970 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  3980 */   165,  115,  116,  191,  169,  191,  191,  191,  169,  191,
 /*  3990 */   191,  191,  173,  191,  179,  180,  181,  182,  179,  180,
 /*  4000 */   181,  182,  191,  137,  191,    1,    2,    3,    4,    5,
 /*  4010 */   191,  191,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  4020 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  4030 */   164,  165,  115,  116,  191,  169,  191,  191,  191,  191,
 /*  4040 */   191,  191,  191,  191,  191,  179,  180,  181,  182,  191,
 /*  4050 */   191,   47,   48,  191,  137,  191,  191,  191,  191,  191,
 /*  4060 */   191,   57,  191,  146,  147,  148,  149,  150,  151,  152,
 /*  4070 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  4080 */   163,  164,  165,  115,  116,  191,  169,  191,  191,  191,
 /*  4090 */   191,  169,  191,  191,  191,  173,  179,  180,  181,  182,
 /*  4100 */   169,  179,  180,  181,  182,  137,  191,  191,  177,  178,
 /*  4110 */   179,  180,  181,  182,  146,  147,  148,  149,  150,  151,
 /*  4120 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  4130 */   162,  163,  164,  165,  115,  116,  191,  169,  191,  191,
 /*  4140 */   191,  169,  191,  191,  191,  173,  191,  179,  180,  181,
 /*  4150 */   182,  179,  180,  181,  182,  191,  137,  191,  191,  191,
 /*  4160 */   191,  191,  191,  191,  191,  146,  147,  148,  149,  150,
 /*  4170 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4180 */   161,  162,  163,  164,  165,  115,  116,  191,  169,  191,
 /*  4190 */   191,  191,  169,  191,  191,  191,  173,  191,  179,  180,
 /*  4200 */   181,  182,  179,  180,  181,  182,  191,  137,  191,  191,
 /*  4210 */   191,  191,  191,  191,  191,  191,  146,  147,  148,  149,
 /*  4220 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  4230 */   160,  161,  162,  163,  164,  165,  115,  116,  191,  169,
 /*  4240 */   191,  191,  191,  169,  191,  191,  191,  173,  191,  179,
 /*  4250 */   180,  181,  182,  179,  180,  181,  182,  191,  137,  191,
 /*  4260 */   191,  191,  191,  191,  191,  191,  191,  146,  147,  148,
 /*  4270 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  4280 */   159,  160,  161,  162,  163,  164,  165,  115,  116,  191,
 /*  4290 */   169,  191,  191,  191,  169,  191,  191,  191,  173,  191,
 /*  4300 */   179,  180,  181,  182,  179,  180,  181,  182,  191,  137,
 /*  4310 */   191,  191,  191,  191,  191,  191,  191,  191,  146,  147,
 /*  4320 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4330 */   158,  159,  160,  161,  162,  163,  164,  165,  115,  116,
 /*  4340 */   191,  169,  191,  191,  191,  169,  191,  191,  191,  173,
 /*  4350 */   191,  179,  180,  181,  182,  179,  180,  181,  182,  191,
 /*  4360 */   137,  191,    1,    2,    3,    4,    5,  191,  191,  146,
 /*  4370 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  4380 */   157,  158,  159,  160,  161,  162,  163,  164,  165,  115,
 /*  4390 */   116,  191,  169,  191,  191,  191,  191,  191,  191,  191,
 /*  4400 */   191,  191,  179,  180,  181,  182,  191,  191,   47,   48,
 /*  4410 */   191,   50,   51,  191,   53,  191,  191,  191,   57,  191,
 /*  4420 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  4430 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4440 */   191,  191,  191,  169,    7,  191,    9,   10,   11,   12,
 /*  4450 */   169,  191,  191,  179,  180,  181,  182,   20,  191,  191,
 /*  4460 */   179,  180,  181,  182,  191,  191,  191,  191,  191,   32,
 /*  4470 */    33,   34,   35,   36,  191,   38,  191,    7,   41,    9,
 /*  4480 */    10,   11,   12,  191,  191,  191,   49,  191,  169,  191,
 /*  4490 */    20,   54,  191,   56,  191,  191,  191,  191,  179,  180,
 /*  4500 */   181,  182,   32,   33,   34,   35,   36,   37,   38,  190,
 /*  4510 */   191,   41,  191,  191,  191,  191,  191,  191,  191,   49,
 /*  4520 */    40,   84,  169,   43,   54,   45,   56,   47,   48,   92,
 /*  4530 */   191,   51,  179,  180,  181,  182,  191,  100,  101,  102,
 /*  4540 */   103,  104,  105,   13,   14,   15,  191,  191,   18,   19,
 /*  4550 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  4560 */    30,   31,   92,  191,  191,  191,  191,  191,   38,   39,
 /*  4570 */   100,  101,  102,  103,  104,  105,  191,  191,    7,  191,
 /*  4580 */     9,   10,   11,   12,  169,  191,  106,  191,  173,  191,
 /*  4590 */   191,   20,  191,  191,  179,  180,  181,  182,  191,  191,
 /*  4600 */   191,  191,  191,   32,   33,   34,   35,   36,   37,   38,
 /*  4610 */   191,    7,   41,    9,   10,   11,   12,  191,  169,  191,
 /*  4620 */    49,  191,  191,  191,   20,   54,  191,   56,  179,  180,
 /*  4630 */   181,  182,  191,  191,  191,  191,   32,   33,   34,   35,
 /*  4640 */    36,   37,   38,  191,  191,   41,  191,  191,  191,  191,
 /*  4650 */   191,  191,  191,   49,  191,  191,  169,  191,   54,  191,
 /*  4660 */    56,  191,  191,   92,  191,  191,  179,  180,  181,  182,
 /*  4670 */   191,  100,  101,  102,  103,  104,  105,  191,   14,   15,
 /*  4680 */   191,  191,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4690 */    26,   27,   28,   29,   30,   31,   92,  191,  191,  191,
 /*  4700 */   191,  191,   38,   39,  100,  101,  102,  103,  104,  105,
 /*  4710 */   169,  191,    7,  191,    9,   10,   11,   12,  169,  191,
 /*  4720 */   179,  180,  181,  182,  191,   20,  191,  191,  179,  180,
 /*  4730 */   181,  182,  191,  191,  191,  191,  191,   32,   33,   34,
 /*  4740 */    35,   36,   37,   38,  191,  169,   41,    7,  191,    9,
 /*  4750 */    10,   11,   12,  169,   49,  179,  180,  181,  182,   54,
 /*  4760 */    20,   56,  191,  179,  180,  181,  182,  191,  191,  191,
 /*  4770 */   191,  191,   32,   33,   34,   35,   36,  191,   38,  191,
 /*  4780 */   191,   41,  191,  191,  191,  191,  191,  191,  191,   49,
 /*  4790 */   169,  191,  191,  191,   54,  191,   56,   92,  191,   59,
 /*  4800 */   179,  180,  181,  182,  191,  100,  101,  102,  103,  104,
 /*  4810 */   105,  191,  191,   18,   19,   20,   21,   22,   23,   24,
 /*  4820 */    25,   26,   27,   28,   29,   30,   31,  191,  191,  191,
 /*  4830 */   191,  191,   92,   38,   39,  191,  191,  191,  191,  191,
 /*  4840 */   100,  101,  102,  103,  104,  105,  169,  191,    7,  191,
 /*  4850 */     9,   10,   11,   12,  169,  191,  179,  180,  181,  182,
 /*  4860 */   191,   20,  191,  191,  179,  180,  181,  182,  191,  191,
 /*  4870 */   191,  191,  191,   32,   33,   34,   35,   36,   37,   38,
 /*  4880 */   191,    7,   41,    9,   10,   11,   12,  191,  191,  191,
 /*  4890 */    49,  191,  169,  191,   20,   54,  191,   56,  191,  191,
 /*  4900 */    38,   39,  179,  180,  181,  182,   32,   33,   34,   35,
 /*  4910 */    36,   37,   38,  191,   52,   41,  191,  191,  191,  191,
 /*  4920 */   191,  191,  191,   49,  169,  191,  191,  191,   54,  191,
 /*  4930 */    56,  191,  191,   92,  179,  180,  181,  182,  191,  191,
 /*  4940 */   191,  100,  101,  102,  103,  104,  105,  191,   86,   87,
 /*  4950 */    88,   89,   90,   91,  191,   93,   94,   95,  191,  169,
 /*  4960 */   191,  191,  191,  191,  191,  191,   92,  191,  191,  179,
 /*  4970 */   180,  181,  182,  191,  100,  101,  102,  103,  104,  105,
 /*  4980 */   169,  191,    7,  191,    9,   10,   11,   12,  169,  191,
 /*  4990 */   179,  180,  181,  182,  191,   20,  191,  191,  179,  180,
 /*  5000 */   181,  182,  191,  191,  191,  191,  191,   32,   33,   34,
 /*  5010 */    35,   36,   37,   38,  191,    7,   41,    9,   10,   11,
 /*  5020 */    12,  191,  169,  191,   49,  191,  169,  191,   20,   54,
 /*  5030 */   191,   56,  179,  180,  181,  182,  179,  180,  181,  182,
 /*  5040 */    32,   33,   34,   35,   36,   20,   38,  191,  169,   41,
 /*  5050 */    42,  191,  191,  191,  191,  191,  191,   49,  179,  180,
 /*  5060 */   181,  182,   54,  191,   56,  191,  191,   92,  191,  191,
 /*  5070 */   191,  191,  191,  191,  191,  100,  101,  102,  103,  104,
 /*  5080 */   105,  191,  191,   58,  191,   60,   61,   62,   63,   64,
 /*  5090 */    65,   66,   67,   68,   69,   70,  191,  191,  191,  191,
 /*  5100 */    92,  191,  191,  191,  191,  191,  191,  191,  100,  101,
 /*  5110 */   102,  103,  104,  105,  191,  191,    7,  191,    9,   10,
 /*  5120 */    11,   12,  169,    1,    2,    3,    4,    5,  191,   20,
 /*  5130 */   191,  191,  179,  180,  181,  182,  191,  191,  191,  191,
 /*  5140 */   191,   32,   33,   34,   35,   36,   37,   38,  191,    7,
 /*  5150 */    41,    9,   10,   11,   12,  191,  191,  191,   49,  191,
 /*  5160 */   191,  191,   20,   54,  191,   56,  191,  191,  191,   47,
 /*  5170 */    48,  191,   50,   51,   32,   33,   34,   35,   36,   57,
 /*  5180 */    38,  191,  169,   41,  191,    1,    2,    3,    4,    5,
 /*  5190 */   191,   49,  179,  180,  181,  182,   54,  191,   56,  191,
 /*  5200 */   191,   92,  191,  191,  191,  191,  191,  191,  191,  100,
 /*  5210 */   101,  102,  103,  104,  105,    1,    2,    3,    4,    5,
 /*  5220 */   191,  191,  191,  191,  191,   41,   84,  191,  191,  191,
 /*  5230 */   191,   47,   48,  191,   92,  191,  191,  191,  191,   55,
 /*  5240 */   191,   57,  100,  101,  102,  103,  104,  105,  191,  191,
 /*  5250 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  5260 */   191,   47,   48,   20,   50,   51,  191,   53,  191,  191,
 /*  5270 */   191,   57,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  5280 */   191,   38,  191,  169,   41,    7,  191,    9,   10,   11,
 /*  5290 */    12,  191,   49,  179,  180,  181,  182,   54,   20,   56,
 /*  5300 */   191,  191,   59,  191,  191,  191,  191,  191,  191,   20,
 /*  5310 */    32,   33,   34,   35,   36,  191,   38,  191,  191,   41,
 /*  5320 */   191,  191,  191,  191,  191,  191,   37,   49,  169,  191,
 /*  5330 */    41,  191,   54,  191,   56,   92,  191,   59,  179,  180,
 /*  5340 */   181,  182,  191,  100,  101,  102,  103,  104,  105,   60,
 /*  5350 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*  5360 */     1,    2,    3,    4,    5,  191,  191,  191,  191,  191,
 /*  5370 */    92,  191,  191,  191,  191,  191,  191,  191,  100,  101,
 /*  5380 */   102,  103,  104,  105,  169,  191,    7,  191,    9,   10,
 /*  5390 */    11,   12,  191,  191,  179,  180,  181,  182,  191,   20,
 /*  5400 */   191,  191,  191,  191,  191,  191,   47,   48,  191,   50,
 /*  5410 */    51,   32,   33,   34,   35,   36,   57,   38,  191,    7,
 /*  5420 */    41,    9,   10,   11,   12,  191,  191,  191,   49,  191,
 /*  5430 */   191,  191,   20,   54,  191,   56,  191,  191,   59,  191,
 /*  5440 */   191,  191,  191,  191,   32,   33,   34,   35,   36,   37,
 /*  5450 */    38,  191,  191,   41,  191,  191,  191,  191,  191,  191,
 /*  5460 */   191,   49,  169,  191,  191,  191,   54,  191,   56,  191,
 /*  5470 */   191,   92,  179,  180,  181,  182,  191,  191,  191,  100,
 /*  5480 */   101,  102,  103,  104,  105,    1,    2,    3,    4,    5,
 /*  5490 */   191,  191,  191,  191,  191,  191,  169,  191,  191,  191,
 /*  5500 */   191,  191,  191,  191,   92,  191,  179,  180,  181,  182,
 /*  5510 */   191,  191,  100,  101,  102,  103,  104,  105,  191,  191,
 /*  5520 */     7,  191,    9,   10,   11,   12,  191,  191,  191,  191,
 /*  5530 */   191,   47,   48,   20,   50,   51,  191,   53,  191,  191,
 /*  5540 */   191,   57,  191,  191,  191,   32,   33,   34,   35,   36,
 /*  5550 */    37,   38,  191,    7,   41,    9,   10,   11,   12,  191,
 /*  5560 */   191,  191,   49,  191,  191,  191,   20,   54,  191,   56,
 /*  5570 */   191,  191,  191,  191,  191,  191,  191,  191,   32,   33,
 /*  5580 */    34,   35,   36,   20,   38,  191,  191,   41,  191,  191,
 /*  5590 */   191,  191,  191,  191,  191,   49,  191,  191,  191,  191,
 /*  5600 */    54,  191,   56,  191,   41,   92,  191,  191,  191,  191,
 /*  5610 */   191,  191,  191,  100,  101,  102,  103,  104,  105,  191,
 /*  5620 */   191,  191,  191,   60,   61,   62,   63,   64,   65,   66,
 /*  5630 */    67,   68,   69,   70,  191,  191,  191,  191,   92,  191,
 /*  5640 */   191,  191,  191,  191,  191,  191,  100,  101,  102,  103,
 /*  5650 */   104,  105,  191,  191,    7,  191,    9,   10,   11,   12,
 /*  5660 */   191,  191,  191,  191,  191,  191,  191,   20,  191,  191,
 /*  5670 */   191,  191,  191,  191,  191,  191,  191,  191,  191,   32,
 /*  5680 */    33,   34,   35,   36,  191,   38,  191,  191,   41,  191,
 /*  5690 */   191,  191,  191,  191,  191,  191,   49,  191,  191,  191,
 /*  5700 */   191,   54,  191,   56,  191,    8,  191,  191,  191,  191,
 /*  5710 */    13,   14,   15,  191,  191,   18,   19,   20,   21,   22,
 /*  5720 */    23,   24,   25,   26,   27,   28,   29,   30,   31,    1,
 /*  5730 */     2,    3,    4,    5,  191,   38,   39,  191,  191,   92,
 /*  5740 */   191,  191,  191,  191,  191,  191,  191,  100,  101,  102,
 /*  5750 */   103,  104,  105,  191,  191,  191,  191,  191,  191,  191,
 /*  5760 */   191,  191,  191,  191,  191,  191,  191,  191,  191,  191,
 /*  5770 */   191,  191,  191,  191,   77,   47,   48,  191,   50,   51,
 /*  5780 */   191,   53,  191,    8,  191,   57,  191,  191,   13,   14,
 /*  5790 */    15,  191,  191,   18,   19,   20,   21,   22,   23,   24,
 /*  5800 */    25,   26,   27,   28,   29,   30,   31,  191,  191,  191,
 /*  5810 */   191,  191,  191,   38,   39,    8,  191,  191,  191,  191,
 /*  5820 */    13,   14,   15,  191,   49,   18,   19,   20,   21,   22,
 /*  5830 */    23,   24,   25,   26,   27,   28,   29,   30,   31,  191,
 /*  5840 */   191,  191,    8,  191,  191,   38,   39,   13,   14,   15,
 /*  5850 */   191,  191,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  5860 */    26,   27,   28,   29,   30,   31,   59,  191,  191,    8,
 /*  5870 */   191,   37,   38,   39,   13,   14,   15,  191,  191,   18,
 /*  5880 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  5890 */    29,   30,   31,  191,  191,  191,    8,  191,  191,   38,
 /*  5900 */    39,   13,   14,   15,  191,  191,   18,   19,   20,   21,
 /*  5910 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  5920 */    59,  191,  191,  191,  191,  191,   38,   39,    8,  191,
 /*  5930 */    42,  191,  191,   13,   14,   15,  191,  191,   18,   19,
 /*  5940 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  5950 */    30,   31,  191,  191,  191,  191,  191,  191,   38,   39,
 /*  5960 */     8,  191,   42,  191,  191,   13,   14,   15,  191,  191,
 /*  5970 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  5980 */    28,   29,   30,   31,  191,  191,  191,  191,  191,  191,
 /*  5990 */    38,   39,    8,  191,   42,  191,  191,   13,   14,   15,
 /*  6000 */   191,  191,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  6010 */    26,   27,   28,   29,   30,   31,  191,  191,  191,  191,
 /*  6020 */   191,  191,   38,   39,    8,  191,  191,  191,  191,   13,
 /*  6030 */    14,   15,  191,   49,   18,   19,   20,   21,   22,   23,
 /*  6040 */    24,   25,   26,   27,   28,   29,   30,   31,  191,  191,
 /*  6050 */   191,  191,  191,  191,   38,   39,    8,  191,  191,  191,
 /*  6060 */   191,   13,   14,   15,  191,   49,   18,   19,   20,   21,
 /*  6070 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  6080 */   191,  191,  191,  191,  191,  191,   38,   39,    8,  191,
 /*  6090 */   191,  191,  191,   13,   14,   15,   20,   49,   18,   19,
 /*  6100 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  6110 */    30,   31,  191,   37,   20,  191,  191,   41,   38,   39,
 /*  6120 */   191,  191,  191,  191,  191,  191,  191,  191,  191,  191,
 /*  6130 */    20,   37,  191,  191,  191,   41,   60,   61,   62,   63,
 /*  6140 */    64,   65,   66,   67,   68,   69,   70,   37,  191,  191,
 /*  6150 */   191,   41,   20,  191,   60,   61,   62,   63,   64,   65,
 /*  6160 */    66,   67,   68,   69,   70,    1,    2,    3,    4,    5,
 /*  6170 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  6180 */    70,    1,    2,    3,    4,    5,  191,  191,  191,  191,
 /*  6190 */   191,  191,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  6200 */    68,   69,   70,    1,    2,    3,    4,    5,  191,  191,
 /*  6210 */   191,   47,   48,  191,   50,   51,  191,   53,  191,  191,
 /*  6220 */   191,   57,  191,  191,  191,  191,  191,   47,   48,  191,
 /*  6230 */    50,   51,  191,   53,  191,  191,  191,   57,  191,    1,
 /*  6240 */     2,    3,    4,    5,    1,    2,    3,    4,    5,   47,
 /*  6250 */    48,  191,   50,   51,  191,  191,  191,  191,  191,   57,
 /*  6260 */   191,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  6270 */     5,  191,  191,  191,  191,  191,  191,  191,  191,  191,
 /*  6280 */   191,  191,  191,  191,  191,   47,   48,  191,   50,   51,
 /*  6290 */    47,   48,  191,   50,   51,   57,  191,  191,  191,  191,
 /*  6300 */    57,  191,    1,    2,    3,    4,    5,   47,   48,  191,
 /*  6310 */    50,   51,   47,   48,  191,   50,   51,   57,  191,  191,
 /*  6320 */   191,  191,   57,  191,  191,  191,  191,  191,  191,  191,
 /*  6330 */   191,  191,  191,  191,  191,  191,  191,  191,  191,  191,
 /*  6340 */   191,  191,  191,  191,  191,  191,  191,  191,   47,   48,
 /*  6350 */   191,   50,   51,  191,  191,  191,  191,  191,   57,
};
#define YY_SHIFT_USE_DFLT (-56)
static short yy_shift_ofst[] = {
 /*     0 */  4480,    7, 3267,  -56,  -56,  -56,  -56,  -56,  -56,  189,
 /*    10 */   204,  -56,  276,  788,  -56,  295,  313,  -56, 5728,  -56,
 /*    20 */   498,  -56, 5214, 5359,  -56, 3851,  439,  385, 5289,  445,
 /*    30 */  5025,  488,  -56,  -56,  622, 6132,  -56,  535,  -56,  -56,
 /*    40 */   -56,  542,  650,  -56,  591,  641,  -56,  -56,  -56,  -56,
 /*    50 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,
 /*    60 */   730,  691, 5025,  669,  -56,  -56, 5563,  -56,  625,  179,
 /*    70 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  639,  708,
 /*    80 */   -56,  -56,  496,  719,  179,  -56,  769,  771,  179,  -56,
 /*    90 */   783,  781,  179,  -56,  797,  818,  -56,  -56,  -56,  -56,
 /*   100 */   -56,  -56,  -56,  -56,  -56,  -56, 4004, 3545,  799,  785,
 /*   110 */  6076,  391, 5025,  803,  -56,  -56,  264,  345, 5025,  804,
 /*   120 */   -56,  -56,  -56,  -56,  -56, 3443,  794,  798,  179,  810,
 /*   130 */   -56,  808,  815,  179,  829,  -56, 6238,  -56,  835,  333,
 /*   140 */   -56,  842,  284,  -56,  847,  239,  -56, 4361,  -56,  851,
 /*   150 */   -56, 6164, 6260,  -56, 3749,  865,  852, 6110,  213, 2093,
 /*   160 */   -56, 1993,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,
 /*   170 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,
 /*   180 */   -56,  -56,  -56,  -56,  -56,  -56,  867,  -56,  868,  -56,
 /*   190 */  5546,  862, 1593, 1393,  838,  866, 1293, 1193,  -56, 5546,
 /*   200 */   869,  336,  -56,  272,  -56,  -56,  179,  837, 3193, 3193,
 /*   210 */   871, 2193, 1893,  -56, 5546,  872, 1693, 1093,  -56,  873,
 /*   220 */    -7, 1493,  843, 5546,  875,  -56, 5546,   83, 5546, 5546,
 /*   230 */   101, 5546, 5546, 6080, 5546, 5546, 4530, 5546, 5546, 4530,
 /*   240 */  5546, 5546, 4530, 5546, 5546, 4530, 5546,  280, 5546,  280,
 /*   250 */   635,  874, 4874, 6080, 5546, 5807,  -56, 5546,  107, 5546,
 /*   260 */   107, 5546,   83, 5546,   83, 5546,   83, 5546,  107, 5546,
 /*   270 */  4795, 5546, 4795, 5546, 4664, 5546, 5697, 5546, 6080, 3293,
 /*   280 */  5834,  -56,  887, 5546,   83,  891,  912, 5546,  280,  -55,
 /*   290 */    47,  880, 4604,  896,  932, 5546,   83,  -56, 5546,   83,
 /*   300 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56, 5278, 6080,
 /*   310 */    -6, 5109,  596,  883, 4705,  -56,  578,  -56, 5546,  899,
 /*   320 */   895,  176, 5412,  540,  -56,  -56,  -56,  -56,  -56,  -56,
 /*   330 */   581,  885, 4975,  570,  -56,  -56,  905,  898,  894, 4841,
 /*   340 */   564,  -56,  -56,  -56,  910,  902,  906, 4571,  -56,  515,
 /*   350 */   -56,  508,  -56,  -56,  890,  907,  -56,  465,  -56, 5647,
 /*   360 */   -56,  911, 5546,  -56,  -56,  -56,  470,  -56,  -56,  -56,
 /*   370 */   464,  -56,  -56,  396,  920,  915, 4470,  440,  -56,  -56,
 /*   380 */   926,  921, 5513,  364,  -56,  -56,  280,  280,  280,  280,
 /*   390 */   280,  280, 6080,  942,  365, 4437, 5775, 3193, 1793,  -56,
 /*   400 */   960,  310,  -56,  960,  -56, 4862,  -56,  -56,  -56,  -56,
 /*   410 */   -56,  -56,  -56, 5546,  -56, 6080,  -17,   29, 5546,  -56,
 /*   420 */  5243,  128, 5546,  -56,  949,  -56, 5861,  429, 5546,  -56,
 /*   430 */   -56, 5546,  -56,  -56,  -56,  109,  959,  128, 5546,  -56,
 /*   440 */   962,  128, 5546,  -56,  969,  329, 5546,  -56, 5379,  128,
 /*   450 */  5546,  -56,  429, 5546,  -56, 4740,  128, 5546,  -56,  429,
 /*   460 */  5546,  -56,  -56,  -56,  -56, 5546,  165,  -56, 5546,  -56,
 /*   470 */  6080,  -56,  972,  -56,  973,  -56,  977,  -56,  978,  -56,
 /*   480 */  5008, 5952,  -56,  -56, 5546, 5920,  -56, 5546, 5888,  -56,
 /*   490 */   980,  480,  -56,  980,  -56,  970,  179,  -56,  -56,  980,
 /*   500 */   582,  -56,  980,  664,  -56,  980,  677,  -56,  980,  695,
 /*   510 */   -56,  980,  735,  -56,  980,  742,  -56,  980,  734,  -56,
 /*   520 */   980,  146,  -56,  980,  764,  -56, 6080,  -56,  -56,  -56,
 /*   530 */  5546, 5984, 3193, 2693,  -56,  982,  950, 5142, 6048, 3193,
 /*   540 */  2993,  -56, 5546, 6016, 3193,   93,  -56,  952, 5546,  994,
 /*   550 */   -56,  -56,  -56,  961, 3193, 3193,  -56,  -56,  963,  990,
 /*   560 */   393,  693,  -56,  -56, 5025,  991,  893,  -56,  793,  -56,
 /*   570 */   746,  762,  593,  -56,  493,  -56, 5025,  992,  293,  -56,
 /*   580 */   193,  -56, 4004, 3647, 1004,  995, 6094,  430,  993,  -56,
 /*   590 */  3093,  -56, 5025,  113, 2893,  -56, 2793,  -56,  630,  484,
 /*   600 */  2593,  -56, 2493,  -56, 5025,  997, 2393,  -56, 2293,  -56,
 /*   610 */  6180, 6301,  -56, 3443,  -56, 3443, 3335,  -19,  -56,  179,
 /*   620 */    65,  -56, 1008,  -56,  226, 1020,  382, 1028,  411,  -56,
 /*   630 */   -56, 1035,  -56,  -56, 1041,  -56, 5184,   99,  -56,  179,
 /*   640 */    20,  -56, 1059,  -56, 1064,  -56, 5484, 5122, 6243, 4004,
 /*   650 */  6202,  -56, 6265,  847,  -56,  -56,  -56,  847,  239,  -56,
 /*   660 */  1063, 1068,  492,  -56, 1069, 1062,  -56,  847,  239,  -56,
 /*   670 */  1067, 1074,  518,  -56, 1077, 1062,  -56,  -56,
};
#define YY_REDUCE_USE_DFLT (-133)
static short yy_reduce_ofst[] = {
 /*     0 */   -94, -133,  125, -133, -133, -133, -133, -133, -133, -133,
 /*    10 */  -133, -133, -133,  207, -133, -133,  259, -133, 3406, -133,
 /*    20 */  -133, -133,  513,  -87, -133,  244, -133, -133,  196, -133,
 /*    30 */    68, -133, -133, -133, -133,   80, -133, -133, -133, -133,
 /*    40 */  -133, -133, -133, -133, -133, -133, -133, -133, -133, -133,
 /*    50 */  -133, -133, -133, -133, -133, -133, -133, -133, -133, -133,
 /*    60 */  -133, -133,  674, -133, -133, -133,  -59, -133, -133,  595,
 /*    70 */  -133, -133, -133, -133, -133, -133, -133, -133, -133, -133,
 /*    80 */  -133, -133, -133, -133,  659, -133, -133, -133,  701, -133,
 /*    90 */  -133, -133,  707, -133, -133, -133, -133, -133, -133, -133,
 /*   100 */  -133, -133, -133, -133, -133, -133,  312,  244, -133, -133,
 /*   110 */   -66, -133,  -32, -133, -133, -133, -133, -133,  476, -133,
 /*   120 */  -133, -133, -133, -133, -133,  312, -133, -133,  722, -133,
 /*   130 */  -133, -133, -133,  741, -133, -133,  -87, -133, -133,  759,
 /*   140 */  -133, -133,  766, -133,   60,  789, -133, 3304, -133, -133,
 /*   150 */  -133,  -88,   48, -133,  244, -133, -133,   41, -133, 4019,
 /*   160 */  -133, 4274, -133, -133, -133, -133, -133, -133, -133, -133,
 /*   170 */  -133, -133, -133, -133, -133, -133, -133, -133, -133, -133,
 /*   180 */  -133, -133, -133, -133, -133, -133, -133, -133, -133, -133,
 /*   190 */   242, -133, 3968, 4274, -133, -133, 3815, 4274, -133, 1701,
 /*   200 */  -133,  553, -133,  748, -133, -133,  790, -133, 4223, 4274,
 /*   210 */  -133, 3254, 4274, -133, 1801, -133, 3305, 4274, -133, -133,
 /*   220 */  3662, 4274, -133, 1901, -133, -133, 4541, -133, 5013, 3000,
 /*   230 */  -133, 2200, 5114, -133, 5159, 4953, -133, 4685, 4811, -133,
 /*   240 */  4755, 4723, -133, 4621, 4584, -133, 4281, -133, 2900, -133,
 /*   250 */  -133, -133, 1100, -133, 2100, -133, -133, 4853, -133, 3164,
 /*   260 */  -133, 2800, -133, 2400, -133, 4819, -133, 2300, -133, 4449,
 /*   270 */  -133, 5293, -133, 2500, -133, 4576, -133, 2600, -133, 3101,
 /*   280 */  -133, -133, -133, 4879, -133, -133, -133, 2700, -133, -133,
 /*   290 */  -133, -133, 1300, -133, -133, 4549, -133, -133, 4487, -133,
 /*   300 */  -133, -133, -133, -133, -133, -133, -133, -133, 3199, -133,
 /*   310 */  -133, 1200, -133, -133, 1000, -133, -133, -133, 1600, -133,
 /*   320 */  -133, -133,  900, -133, -133, -133, -133, -133, -133, -133,
 /*   330 */  -133, -133,  800, -133, -133, -133, -133, -133, -133,  700,
 /*   340 */  -133, -133, -133, -133, -133, -133, -133,  600, -133, -133,
 /*   350 */  -133, -133, -133, -133, -133, -133, -133, -133, -133, 3768,
 /*   360 */  -133, -133, 4319, -133, -133, -133, -133, -133, -133, -133,
 /*   370 */  -133, -133, -133, -133, -133, -133,  500, -133, -133, -133,
 /*   380 */  -133, -133,  400, -133, -133, -133, -133, -133, -133, -133,
 /*   390 */  -133, -133, -133, -133, -133, 4353, -133, 3713, 4274, -133,
 /*   400 */   116, -133, -133,  836, -133,  -42, -133, -133, -133, -133,
 /*   410 */  -133, -133, -133, 3972, -133, -133, -133,   -5, 4074, -133,
 /*   420 */  1500,  834, 3564, -133, -133, -133, -133,  234, 3819, -133,
 /*   430 */  -133, 1500, -133, -133, -133, -133, -133,  839, 4023, -133,
 /*   440 */  -133,  844, 4415, -133, -133,   76, 4125, -133, 1500,  845,
 /*   450 */  3922, -133,  140, 3666, -133, 1500,  846, 4176, -133,  235,
 /*   460 */  3462, -133, -133, -133, -133, 3931, -133, -133, 2001, -133,
 /*   470 */  -133, -133, -133, -133, -133, -133, -133, -133, -133, -133,
 /*   480 */  4677, -133, -133, -133, 4790, -133, -133, 4857, -133, -133,
 /*   490 */   299, -133, -133,  840, -133, -133,  901, -133, -133,  356,
 /*   500 */  -133, -133,  367, -133, -133,  449, -133, -133,  524, -133,
 /*   510 */  -133,  526, -133, -133,  539, -133, -133,  555, -133, -133,
 /*   520 */   562, -133, -133,  567, -133, -133, -133, -133, -133, -133,
 /*   530 */   640, -133, 4121, 4274, -133, -133, -133, 5215, -133, 3356,
 /*   540 */  4274, -133, 5327, -133, 4070, 4274, -133, -133, 1401, -133,
 /*   550 */  -133, -133, -133, -133, 4172, 4274, -133, -133, -133, -133,
 /*   560 */  3509, 4274, -133, -133,  574, -133, 3407, -133, 4274, -133,
 /*   570 */  -133, -133, 3611, -133, 4274, -133,  368, -133, 3917, -133,
 /*   580 */  4274, -133,  512,  244, -133, -133,  178, -133, 3866, -133,
 /*   590 */  4274, -133, -132, -133, 3560, -133, 4274, -133, -133, -133,
 /*   600 */  3764, -133, 4274, -133,  468, -133, 3458, -133, 4274, -133,
 /*   610 */   212,   48, -133,  512, -133,  241,  244,  918, -133,  923,
 /*   620 */   937, -133, -133, -133,  676, -133, -133, -133,  939, -133,
 /*   630 */  -133, -133, -133, -133, -133, -133,  244,  947, -133,  955,
 /*   640 */   958, -133, -133, -133, -133, -133, 3355,  311,   48,  241,
 /*   650 */    48, -133,   48,  986, -133, -133, -133,  -39,  996, -133,
 /*   660 */  -133, -133,  998, -133, -133,  999, -133,  502, 1001, -133,
 /*   670 */  -133, -133, 1002, -133, -133, 1003, -133, -133,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1008, 1008, 1008,  680,  682,  683,  684,  685,  686, 1008,
 /*    10 */  1008,  687, 1008, 1008,  688, 1008, 1008,  689, 1008,  704,
 /*    20 */  1008,  705, 1008, 1008,  738, 1008, 1008, 1008, 1008, 1008,
 /*    30 */  1008, 1008,  758,  776,  777, 1008,  778,  780,  781,  782,
 /*    40 */   783, 1008, 1008,  794, 1008, 1008,  795,  796,  797,  798,
 /*    50 */   799,  800,  801,  802,  803,  804,  805,  806,  779,  762,
 /*    60 */  1008, 1008, 1008, 1008,  759,  763, 1008,  784,  786, 1008,
 /*    70 */   790,  996,  997,  998,  999, 1000, 1001, 1002, 1008, 1008,
 /*    80 */  1003, 1004, 1008,  787, 1008,  791, 1008,  788, 1008,  792,
 /*    90 */  1008,  789, 1008,  793, 1008, 1008,  785,  766,  768,  769,
 /*   100 */   770,  771,  772,  773,  774,  775, 1008, 1008, 1008, 1008,
 /*   110 */  1008, 1008, 1008, 1008,  760,  764, 1008, 1008, 1008, 1008,
 /*   120 */   761,  765,  767,  734,  739, 1008, 1008, 1008, 1008, 1008,
 /*   130 */   740, 1008, 1008, 1008, 1008,  741, 1008,  735, 1008, 1008,
 /*   140 */   690, 1008, 1008,  691, 1008, 1008,  693, 1008,  699, 1008,
 /*   150 */   700, 1008, 1008,  736, 1008, 1008, 1008, 1008, 1008, 1008,
 /*   160 */   742, 1008,  744,  807,  809,  810,  811,  812,  813,  814,
 /*   170 */   815,  816,  817,  818,  819,  820,  821,  822,  823,  824,
 /*   180 */   825,  826,  827,  828,  829,  830, 1008,  831, 1008,  832,
 /*   190 */  1008, 1008, 1008, 1008,  835, 1008, 1008, 1008,  836, 1008,
 /*   200 */  1008, 1008,  839, 1008,  840,  841, 1008, 1008,  843,  844,
 /*   210 */  1008, 1008, 1008,  847, 1008, 1008, 1008, 1008,  849, 1008,
 /*   220 */  1008, 1008, 1008, 1008, 1008,  851, 1008,  912, 1008, 1008,
 /*   230 */   913, 1008, 1008,  914, 1008, 1008,  915, 1008, 1008,  916,
 /*   240 */  1008, 1008,  917, 1008, 1008,  918, 1008,  925, 1008,  926,
 /*   250 */  1008,  930, 1008,  987, 1008, 1008,  935, 1008,  936, 1008,
 /*   260 */   937, 1008,  938, 1008,  939, 1008,  940, 1008,  941, 1008,
 /*   270 */   942, 1008,  943, 1008,  944, 1008, 1008, 1008,  984, 1008,
 /*   280 */  1008,  927, 1008, 1008,  928, 1008, 1008, 1008,  929,  948,
 /*   290 */  1008,  933, 1008, 1008, 1008, 1008,  945,  946, 1008,  947,
 /*   300 */   949,  950,  951,  952,  953,  954,  955,  956, 1008,  995,
 /*   310 */   948, 1008, 1008,  959, 1008,  960, 1008,  961, 1008, 1008,
 /*   320 */  1008, 1008, 1008, 1008,  967,  968,  981,  982,  983,  986,
 /*   330 */  1008, 1008, 1008, 1008,  971,  972, 1008, 1008, 1008, 1008,
 /*   340 */  1008,  973,  974,  985, 1008, 1008,  962, 1008,  963, 1008,
 /*   350 */   964, 1008,  965,  966,  949,  950,  957, 1008,  958, 1008,
 /*   360 */   988, 1008, 1008,  990,  991,  989, 1008,  969,  970,  934,
 /*   370 */  1008,  975,  976, 1008, 1008,  931, 1008, 1008,  977,  978,
 /*   380 */  1008,  932, 1008, 1008,  979,  980,  924,  923,  922,  921,
 /*   390 */   920,  919, 1005, 1008, 1008, 1008, 1008, 1008, 1008,  852,
 /*   400 */  1008, 1008,  856, 1008,  857, 1008,  859,  860,  861,  862,
 /*   410 */   863,  864,  865, 1008,  866,  911, 1008, 1008, 1008,  867,
 /*   420 */  1008, 1008, 1008,  870, 1008,  879, 1008, 1008, 1008,  871,
 /*   430 */   877, 1008,  878,  880,  881, 1008, 1008, 1008, 1008,  868,
 /*   440 */  1008, 1008, 1008,  869, 1008, 1008, 1008,  872, 1008, 1008,
 /*   450 */  1008,  873, 1008, 1008,  874, 1008, 1008, 1008,  875, 1008,
 /*   460 */  1008,  876,  882,  883,  858, 1008, 1008,  885, 1008,  886,
 /*   470 */   888,  887,  981,  889,  983,  890,  982,  891,  946,  892,
 /*   480 */  1008, 1008,  893,  894, 1008, 1008,  895, 1008, 1008,  896,
 /*   490 */  1008, 1008,  897, 1008,  907,  909, 1008,  910,  908, 1008,
 /*   500 */  1008,  898, 1008, 1008,  899, 1008, 1008,  900, 1008, 1008,
 /*   510 */   901, 1008, 1008,  902, 1008, 1008,  903, 1008, 1008,  904,
 /*   520 */  1008, 1008,  905, 1008, 1008,  906, 1008, 1006, 1007,  808,
 /*   530 */  1008, 1008, 1008, 1008,  853, 1008, 1008, 1008, 1008, 1008,
 /*   540 */  1008,  854, 1008, 1008, 1008, 1008,  855, 1008, 1008, 1008,
 /*   550 */   850,  848,  846, 1008, 1008,  845,  842,  837,  833, 1008,
 /*   560 */  1008, 1008,  838,  834, 1008, 1008, 1008,  750, 1008,  752,
 /*   570 */  1008, 1008, 1008,  743, 1008,  745, 1008, 1008, 1008,  751,
 /*   580 */  1008,  753, 1008, 1008, 1008, 1008, 1008, 1008, 1008,  746,
 /*   590 */  1008,  748, 1008, 1008, 1008,  754, 1008,  756, 1008, 1008,
 /*   600 */  1008,  747, 1008,  749, 1008, 1008, 1008,  755, 1008,  757,
 /*   610 */  1008, 1008,  737, 1008,  718, 1008, 1008, 1008,  720, 1008,
 /*   620 */  1008,  722, 1008,  726, 1008, 1008, 1008, 1008, 1008,  730,
 /*   630 */   732, 1008,  733,  731, 1008,  724, 1008, 1008,  721, 1008,
 /*   640 */  1008,  723, 1008,  727, 1008,  725, 1008, 1008, 1008, 1008,
 /*   650 */  1008,  719, 1008, 1008,  701,  703,  702, 1008, 1008,  692,
 /*   660 */  1008, 1008, 1008,  694, 1008, 1008,  695, 1008, 1008,  696,
 /*   670 */  1008, 1008, 1008,  697, 1008, 1008,  698,  681,
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
  "QUESTION",      "LIKELY",        "UNLIKELY",      "CLONE",       
  "EMPTY",         "INSTANCEOF",    "AND",           "OR",          
  "BITWISE_OR",    "BITWISE_AND",   "EQUALS",        "IDENTICAL",   
  "LESS",          "GREATER",       "LESSEQUAL",     "GREATEREQUAL",
  "NOTIDENTICAL",  "NOTEQUALS",     "ADD",           "SUB",         
  "CONCAT",        "MUL",           "DIV",           "MOD",         
  "ISSET",         "FETCH",         "TYPEOF",        "NEW",         
  "NOT",           "PARENTHESES_CLOSE",  "SBRACKET_OPEN",  "ARROW",       
  "NAMESPACE",     "IDENTIFIER",    "DOTCOMMA",      "INTERFACE",   
  "EXTENDS",       "CLASS",         "IMPLEMENTS",    "ABSTRACT",    
  "FINAL",         "BRACKET_OPEN",  "BRACKET_CLOSE",  "COMMENT",     
  "ASSIGN",        "CONST",         "CONSTANT",      "FUNCTION",    
  "PARENTHESES_OPEN",  "INLINE",        "VOID",          "SBRACKET_CLOSE",
  "TYPE_INTEGER",  "TYPE_UINTEGER",  "TYPE_LONG",     "TYPE_ULONG",  
  "TYPE_CHAR",     "TYPE_UCHAR",    "TYPE_DOUBLE",   "TYPE_BOOL",   
  "TYPE_STRING",   "TYPE_ARRAY",    "TYPE_VAR",      "BREAK",       
  "CONTINUE",      "IF",            "ELSE",          "SWITCH",      
  "CASE",          "COLON",         "DEFAULT",       "LOOP",        
  "WHILE",         "DO",            "FOR",           "IN",          
  "REVERSE",       "LET",           "ADDASSIGN",     "SUBASSIGN",   
  "MULASSIGN",     "DIVASSIGN",     "CONCATASSIGN",  "MODASSIGN",   
  "STRING",        "DOUBLECOLON",   "INCR",          "DECR",        
  "ECHO",          "RETURN",        "UNSET",         "THROW",       
  "INTEGER",       "CHAR",          "DOUBLE",        "NULL",        
  "TRUE",          "FALSE",         "CBLOCK",        "error",       
  "program",       "xx_language",   "xx_top_statement_list",  "xx_top_statement",
  "xx_namespace_def",  "xx_class_def",  "xx_interface_def",  "xx_comment",  
  "xx_cblock",     "xx_interface_body",  "xx_class_body",  "xx_implements_list",
  "xx_class_definition",  "xx_implements",  "xx_interface_definition",  "xx_class_properties_definition",
  "xx_class_consts_definition",  "xx_class_methods_definition",  "xx_interface_methods_definition",  "xx_class_property_definition",
  "xx_visibility_list",  "xx_literal_expr",  "xx_class_property_shortcuts",  "xx_class_property_shortcuts_list",
  "xx_class_property_shortcut",  "xx_class_const_definition",  "xx_class_method_definition",  "xx_interface_method_definition",
  "xx_parameter_list",  "xx_statement_list",  "xx_method_return_type",  "xx_visibility",
  "xx_method_return_type_list",  "xx_method_return_type_item",  "xx_parameter_type",  "xx_parameter_cast",
  "xx_parameter_cast_collection",  "xx_parameter",  "xx_statement",  "xx_let_statement",
  "xx_if_statement",  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",
  "xx_fetch_statement",  "xx_fcall_statement",  "xx_mcall_statement",  "xx_scall_statement",
  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",
  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",  "xx_switch_statement",
  "xx_for_statement",  "xx_empty_statement",  "xx_eval_expr",  "xx_case_clauses",
  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",
  "xx_assignment_operator",  "xx_assign_expr",  "xx_array_offset_list",  "xx_array_offset",
  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",
  "xx_fcall_expr",  "xx_scall_expr",  "xx_fetch_expr",  "xx_declare_variable_list",
  "xx_declare_variable",  "xx_array_list",  "xx_call_parameters",  "xx_call_parameter",
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
 /*   8 */ "xx_top_statement ::= xx_cblock",
 /*   9 */ "xx_namespace_def ::= NAMESPACE IDENTIFIER DOTCOMMA",
 /*  10 */ "xx_interface_def ::= INTERFACE IDENTIFIER xx_interface_body",
 /*  11 */ "xx_interface_def ::= INTERFACE IDENTIFIER EXTENDS IDENTIFIER xx_interface_body",
 /*  12 */ "xx_class_def ::= CLASS IDENTIFIER xx_class_body",
 /*  13 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  14 */ "xx_class_def ::= CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  15 */ "xx_class_def ::= CLASS IDENTIFIER EXTENDS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  16 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER xx_class_body",
 /*  17 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  18 */ "xx_class_def ::= ABSTRACT CLASS IDENTIFIER IMPLEMENTS xx_implements_list xx_class_body",
 /*  19 */ "xx_class_def ::= FINAL CLASS IDENTIFIER xx_class_body",
 /*  20 */ "xx_class_def ::= FINAL CLASS IDENTIFIER EXTENDS IDENTIFIER xx_class_body",
 /*  21 */ "xx_class_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  22 */ "xx_class_body ::= BRACKET_OPEN xx_class_definition BRACKET_CLOSE",
 /*  23 */ "xx_implements_list ::= xx_implements_list COMMA xx_implements",
 /*  24 */ "xx_implements_list ::= xx_implements",
 /*  25 */ "xx_implements ::= IDENTIFIER",
 /*  26 */ "xx_interface_body ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  27 */ "xx_interface_body ::= BRACKET_OPEN xx_interface_definition BRACKET_CLOSE",
 /*  28 */ "xx_class_definition ::= xx_class_properties_definition",
 /*  29 */ "xx_class_definition ::= xx_class_consts_definition",
 /*  30 */ "xx_class_definition ::= xx_class_methods_definition",
 /*  31 */ "xx_class_definition ::= xx_class_properties_definition xx_class_methods_definition",
 /*  32 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition",
 /*  33 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition",
 /*  34 */ "xx_class_definition ::= xx_class_consts_definition xx_class_methods_definition",
 /*  35 */ "xx_class_definition ::= xx_class_properties_definition xx_class_consts_definition xx_class_methods_definition",
 /*  36 */ "xx_class_definition ::= xx_class_consts_definition xx_class_properties_definition xx_class_methods_definition",
 /*  37 */ "xx_interface_definition ::= xx_class_consts_definition",
 /*  38 */ "xx_interface_definition ::= xx_interface_methods_definition",
 /*  39 */ "xx_interface_definition ::= xx_class_consts_definition xx_interface_methods_definition",
 /*  40 */ "xx_class_properties_definition ::= xx_class_properties_definition xx_class_property_definition",
 /*  41 */ "xx_class_properties_definition ::= xx_class_property_definition",
 /*  42 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  43 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER DOTCOMMA",
 /*  44 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  45 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr DOTCOMMA",
 /*  46 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER xx_class_property_shortcuts DOTCOMMA",
 /*  47 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER xx_class_property_shortcuts DOTCOMMA",
 /*  48 */ "xx_class_property_definition ::= COMMENT xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr xx_class_property_shortcuts DOTCOMMA",
 /*  49 */ "xx_class_property_definition ::= xx_visibility_list IDENTIFIER ASSIGN xx_literal_expr xx_class_property_shortcuts DOTCOMMA",
 /*  50 */ "xx_class_property_shortcuts ::= BRACKET_OPEN BRACKET_CLOSE",
 /*  51 */ "xx_class_property_shortcuts ::= BRACKET_OPEN xx_class_property_shortcuts_list BRACKET_CLOSE",
 /*  52 */ "xx_class_property_shortcuts_list ::= xx_class_property_shortcuts_list COMMA xx_class_property_shortcut",
 /*  53 */ "xx_class_property_shortcuts_list ::= xx_class_property_shortcut",
 /*  54 */ "xx_class_property_shortcut ::= IDENTIFIER",
 /*  55 */ "xx_class_property_shortcut ::= COMMENT IDENTIFIER",
 /*  56 */ "xx_class_consts_definition ::= xx_class_consts_definition xx_class_const_definition",
 /*  57 */ "xx_class_consts_definition ::= xx_class_const_definition",
 /*  58 */ "xx_class_methods_definition ::= xx_class_methods_definition xx_class_method_definition",
 /*  59 */ "xx_class_methods_definition ::= xx_class_method_definition",
 /*  60 */ "xx_interface_methods_definition ::= xx_interface_methods_definition xx_interface_method_definition",
 /*  61 */ "xx_interface_methods_definition ::= xx_interface_method_definition",
 /*  62 */ "xx_class_const_definition ::= COMMENT CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  63 */ "xx_class_const_definition ::= CONST CONSTANT ASSIGN xx_literal_expr DOTCOMMA",
 /*  64 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  65 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  66 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  67 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  68 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  69 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN BRACKET_CLOSE",
 /*  70 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  71 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  72 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  73 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  74 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  75 */ "xx_class_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  76 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  77 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN BRACKET_CLOSE",
 /*  78 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  79 */ "xx_class_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /*  80 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  81 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  82 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  83 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE ARROW xx_method_return_type DOTCOMMA",
 /*  84 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  85 */ "xx_interface_method_definition ::= xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  86 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE DOTCOMMA",
 /*  87 */ "xx_interface_method_definition ::= COMMENT xx_visibility_list FUNCTION IDENTIFIER PARENTHESES_OPEN xx_parameter_list PARENTHESES_CLOSE DOTCOMMA",
 /*  88 */ "xx_visibility_list ::= xx_visibility_list xx_visibility",
 /*  89 */ "xx_visibility_list ::= xx_visibility",
 /*  90 */ "xx_visibility ::= PUBLIC",
 /*  91 */ "xx_visibility ::= PROTECTED",
 /*  92 */ "xx_visibility ::= PRIVATE",
 /*  93 */ "xx_visibility ::= STATIC",
 /*  94 */ "xx_visibility ::= SCOPED",
 /*  95 */ "xx_visibility ::= INLINE",
 /*  96 */ "xx_visibility ::= ABSTRACT",
 /*  97 */ "xx_visibility ::= FINAL",
 /*  98 */ "xx_method_return_type ::= VOID",
 /*  99 */ "xx_method_return_type ::= xx_method_return_type_list",
 /* 100 */ "xx_method_return_type_list ::= xx_method_return_type_list BITWISE_OR xx_method_return_type_item",
 /* 101 */ "xx_method_return_type_list ::= xx_method_return_type_item",
 /* 102 */ "xx_method_return_type_item ::= xx_parameter_type",
 /* 103 */ "xx_method_return_type_item ::= xx_parameter_type NOT",
 /* 104 */ "xx_method_return_type_item ::= xx_parameter_cast",
 /* 105 */ "xx_method_return_type_item ::= xx_parameter_cast_collection",
 /* 106 */ "xx_parameter_list ::= xx_parameter_list COMMA xx_parameter",
 /* 107 */ "xx_parameter_list ::= xx_parameter",
 /* 108 */ "xx_parameter ::= IDENTIFIER",
 /* 109 */ "xx_parameter ::= xx_parameter_type IDENTIFIER",
 /* 110 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER",
 /* 111 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER",
 /* 112 */ "xx_parameter ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 113 */ "xx_parameter ::= xx_parameter_type IDENTIFIER ASSIGN xx_literal_expr",
 /* 114 */ "xx_parameter ::= xx_parameter_type NOT IDENTIFIER ASSIGN xx_literal_expr",
 /* 115 */ "xx_parameter ::= xx_parameter_cast IDENTIFIER ASSIGN xx_literal_expr",
 /* 116 */ "xx_parameter_cast ::= LESS IDENTIFIER GREATER",
 /* 117 */ "xx_parameter_cast_collection ::= LESS IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE GREATER",
 /* 118 */ "xx_parameter_type ::= TYPE_INTEGER",
 /* 119 */ "xx_parameter_type ::= TYPE_UINTEGER",
 /* 120 */ "xx_parameter_type ::= TYPE_LONG",
 /* 121 */ "xx_parameter_type ::= TYPE_ULONG",
 /* 122 */ "xx_parameter_type ::= TYPE_CHAR",
 /* 123 */ "xx_parameter_type ::= TYPE_UCHAR",
 /* 124 */ "xx_parameter_type ::= TYPE_DOUBLE",
 /* 125 */ "xx_parameter_type ::= TYPE_BOOL",
 /* 126 */ "xx_parameter_type ::= TYPE_STRING",
 /* 127 */ "xx_parameter_type ::= TYPE_ARRAY",
 /* 128 */ "xx_parameter_type ::= TYPE_VAR",
 /* 129 */ "xx_statement_list ::= xx_statement_list xx_statement",
 /* 130 */ "xx_statement_list ::= xx_statement",
 /* 131 */ "xx_statement ::= xx_cblock",
 /* 132 */ "xx_statement ::= xx_let_statement",
 /* 133 */ "xx_statement ::= xx_if_statement",
 /* 134 */ "xx_statement ::= xx_loop_statement",
 /* 135 */ "xx_statement ::= xx_echo_statement",
 /* 136 */ "xx_statement ::= xx_return_statement",
 /* 137 */ "xx_statement ::= xx_fetch_statement",
 /* 138 */ "xx_statement ::= xx_fcall_statement",
 /* 139 */ "xx_statement ::= xx_mcall_statement",
 /* 140 */ "xx_statement ::= xx_scall_statement",
 /* 141 */ "xx_statement ::= xx_unset_statement",
 /* 142 */ "xx_statement ::= xx_throw_statement",
 /* 143 */ "xx_statement ::= xx_declare_statement",
 /* 144 */ "xx_statement ::= xx_break_statement",
 /* 145 */ "xx_statement ::= xx_continue_statement",
 /* 146 */ "xx_statement ::= xx_while_statement",
 /* 147 */ "xx_statement ::= xx_do_while_statement",
 /* 148 */ "xx_statement ::= xx_switch_statement",
 /* 149 */ "xx_statement ::= xx_for_statement",
 /* 150 */ "xx_statement ::= xx_comment",
 /* 151 */ "xx_statement ::= xx_empty_statement",
 /* 152 */ "xx_empty_statement ::= DOTCOMMA",
 /* 153 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 154 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 155 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 156 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 157 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 158 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 159 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 160 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 161 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 162 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 163 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 164 */ "xx_case_clauses ::= xx_case_clause",
 /* 165 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 166 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 167 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 168 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 169 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 170 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 171 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 172 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 173 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 174 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 175 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 176 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 177 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 178 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 179 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 180 */ "xx_let_assignments ::= xx_let_assignment",
 /* 181 */ "xx_assignment_operator ::= ASSIGN",
 /* 182 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 183 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 184 */ "xx_assignment_operator ::= MULASSIGN",
 /* 185 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 186 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 187 */ "xx_assignment_operator ::= MODASSIGN",
 /* 188 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 193 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 194 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 195 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 196 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 197 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 198 */ "xx_let_assignment ::= IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 199 */ "xx_array_offset_list ::= xx_array_offset_list xx_array_offset",
 /* 200 */ "xx_array_offset_list ::= xx_array_offset",
 /* 201 */ "xx_array_offset ::= SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE",
 /* 202 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 203 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 204 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 205 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 206 */ "xx_index_expr ::= xx_common_expr",
 /* 207 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 208 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 209 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 210 */ "xx_echo_expression ::= xx_common_expr",
 /* 211 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 212 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 213 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 214 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 215 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 216 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 217 */ "xx_unset_statement ::= UNSET xx_common_expr DOTCOMMA",
 /* 218 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 219 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 220 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 221 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 222 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 223 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 224 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 225 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 226 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 227 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 228 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 229 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 230 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 231 */ "xx_declare_variable ::= IDENTIFIER",
 /* 232 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 233 */ "xx_assign_expr ::= xx_common_expr",
 /* 234 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 235 */ "xx_common_expr ::= ISSET xx_common_expr",
 /* 236 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 237 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 238 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 239 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 240 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 245 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 246 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 247 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 248 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 249 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 250 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 251 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 252 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 253 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 254 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 255 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 256 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 257 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 258 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 259 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 260 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 261 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 262 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 263 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 264 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 265 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 266 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 267 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_common_expr",
 /* 268 */ "xx_common_expr ::= xx_fetch_expr",
 /* 269 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 270 */ "xx_common_expr ::= IDENTIFIER",
 /* 271 */ "xx_common_expr ::= INTEGER",
 /* 272 */ "xx_common_expr ::= STRING",
 /* 273 */ "xx_common_expr ::= CHAR",
 /* 274 */ "xx_common_expr ::= DOUBLE",
 /* 275 */ "xx_common_expr ::= NULL",
 /* 276 */ "xx_common_expr ::= TRUE",
 /* 277 */ "xx_common_expr ::= FALSE",
 /* 278 */ "xx_common_expr ::= CONSTANT",
 /* 279 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 280 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 281 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 282 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 283 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 284 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 285 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 286 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 287 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 288 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 289 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 290 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 291 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 292 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 293 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 294 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 295 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 296 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 297 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 298 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 299 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 300 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 301 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 302 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 303 */ "xx_common_expr ::= xx_mcall_expr",
 /* 304 */ "xx_common_expr ::= xx_scall_expr",
 /* 305 */ "xx_common_expr ::= xx_fcall_expr",
 /* 306 */ "xx_common_expr ::= xx_common_expr QUESTION xx_common_expr COLON xx_common_expr",
 /* 307 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 308 */ "xx_call_parameters ::= xx_call_parameter",
 /* 309 */ "xx_call_parameter ::= xx_common_expr",
 /* 310 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 311 */ "xx_array_list ::= xx_array_item",
 /* 312 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 313 */ "xx_array_item ::= xx_array_value",
 /* 314 */ "xx_array_key ::= IDENTIFIER",
 /* 315 */ "xx_array_key ::= STRING",
 /* 316 */ "xx_array_key ::= INTEGER",
 /* 317 */ "xx_array_value ::= xx_common_expr",
 /* 318 */ "xx_literal_expr ::= INTEGER",
 /* 319 */ "xx_literal_expr ::= CHAR",
 /* 320 */ "xx_literal_expr ::= STRING",
 /* 321 */ "xx_literal_expr ::= DOUBLE",
 /* 322 */ "xx_literal_expr ::= NULL",
 /* 323 */ "xx_literal_expr ::= FALSE",
 /* 324 */ "xx_literal_expr ::= TRUE",
 /* 325 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 326 */ "xx_literal_expr ::= CONSTANT",
 /* 327 */ "xx_eval_expr ::= xx_common_expr",
 /* 328 */ "xx_comment ::= COMMENT",
 /* 329 */ "xx_cblock ::= CBLOCK",
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
    case 99:
    case 100:
    case 101:
    case 102:
    case 103:
    case 104:
    case 105:
    case 106:
// 1022 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3190 "parser.c"
      break;
    case 109:
// 1035 "parser.lemon"
{ json_object_put((yypminor->yy129)); }
// 3195 "parser.c"
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
  { 108, 1 },
  { 109, 1 },
  { 110, 2 },
  { 110, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 111, 1 },
  { 112, 3 },
  { 114, 3 },
  { 114, 5 },
  { 113, 3 },
  { 113, 5 },
  { 113, 5 },
  { 113, 7 },
  { 113, 4 },
  { 113, 6 },
  { 113, 6 },
  { 113, 4 },
  { 113, 6 },
  { 118, 2 },
  { 118, 3 },
  { 119, 3 },
  { 119, 1 },
  { 121, 1 },
  { 117, 2 },
  { 117, 3 },
  { 120, 1 },
  { 120, 1 },
  { 120, 1 },
  { 120, 2 },
  { 120, 2 },
  { 120, 2 },
  { 120, 2 },
  { 120, 3 },
  { 120, 3 },
  { 122, 1 },
  { 122, 1 },
  { 122, 2 },
  { 123, 2 },
  { 123, 1 },
  { 127, 4 },
  { 127, 3 },
  { 127, 6 },
  { 127, 5 },
  { 127, 5 },
  { 127, 4 },
  { 127, 7 },
  { 127, 6 },
  { 130, 2 },
  { 130, 3 },
  { 131, 3 },
  { 131, 1 },
  { 132, 1 },
  { 132, 2 },
  { 124, 2 },
  { 124, 1 },
  { 125, 2 },
  { 125, 1 },
  { 126, 2 },
  { 126, 1 },
  { 133, 6 },
  { 133, 5 },
  { 134, 7 },
  { 134, 8 },
  { 134, 8 },
  { 134, 9 },
  { 134, 8 },
  { 134, 9 },
  { 134, 9 },
  { 134, 10 },
  { 134, 9 },
  { 134, 10 },
  { 134, 10 },
  { 134, 11 },
  { 134, 10 },
  { 134, 11 },
  { 134, 11 },
  { 134, 12 },
  { 135, 8 },
  { 135, 9 },
  { 135, 9 },
  { 135, 10 },
  { 135, 6 },
  { 135, 7 },
  { 135, 7 },
  { 135, 8 },
  { 128, 2 },
  { 128, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 139, 1 },
  { 138, 1 },
  { 138, 1 },
  { 140, 3 },
  { 140, 1 },
  { 141, 1 },
  { 141, 2 },
  { 141, 1 },
  { 141, 1 },
  { 136, 3 },
  { 136, 1 },
  { 145, 1 },
  { 145, 2 },
  { 145, 3 },
  { 145, 2 },
  { 145, 3 },
  { 145, 4 },
  { 145, 5 },
  { 145, 4 },
  { 143, 3 },
  { 144, 5 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 142, 1 },
  { 137, 2 },
  { 137, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 146, 1 },
  { 165, 1 },
  { 159, 2 },
  { 160, 2 },
  { 148, 4 },
  { 148, 7 },
  { 148, 5 },
  { 148, 9 },
  { 148, 8 },
  { 148, 8 },
  { 163, 4 },
  { 163, 5 },
  { 167, 2 },
  { 167, 1 },
  { 168, 3 },
  { 168, 4 },
  { 168, 3 },
  { 149, 3 },
  { 149, 4 },
  { 161, 4 },
  { 161, 5 },
  { 162, 6 },
  { 162, 7 },
  { 164, 7 },
  { 164, 8 },
  { 164, 9 },
  { 164, 10 },
  { 147, 3 },
  { 170, 3 },
  { 170, 1 },
  { 172, 1 },
  { 172, 1 },
  { 172, 1 },
  { 172, 1 },
  { 172, 1 },
  { 172, 1 },
  { 172, 1 },
  { 171, 3 },
  { 171, 5 },
  { 171, 7 },
  { 171, 7 },
  { 171, 7 },
  { 171, 6 },
  { 171, 5 },
  { 171, 7 },
  { 171, 6 },
  { 171, 5 },
  { 171, 4 },
  { 174, 2 },
  { 174, 1 },
  { 175, 3 },
  { 171, 4 },
  { 171, 4 },
  { 171, 2 },
  { 171, 2 },
  { 176, 1 },
  { 150, 3 },
  { 177, 3 },
  { 177, 1 },
  { 178, 1 },
  { 154, 2 },
  { 153, 2 },
  { 155, 2 },
  { 152, 2 },
  { 151, 3 },
  { 151, 2 },
  { 156, 3 },
  { 157, 3 },
  { 158, 3 },
  { 158, 3 },
  { 158, 3 },
  { 158, 3 },
  { 158, 3 },
  { 158, 3 },
  { 158, 3 },
  { 158, 3 },
  { 158, 3 },
  { 158, 3 },
  { 183, 3 },
  { 183, 1 },
  { 184, 1 },
  { 184, 3 },
  { 173, 1 },
  { 169, 2 },
  { 169, 2 },
  { 169, 2 },
  { 169, 2 },
  { 169, 2 },
  { 169, 2 },
  { 169, 2 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 4 },
  { 169, 4 },
  { 169, 3 },
  { 169, 5 },
  { 169, 5 },
  { 169, 3 },
  { 169, 3 },
  { 169, 4 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 169, 3 },
  { 182, 4 },
  { 169, 1 },
  { 169, 2 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 2 },
  { 169, 3 },
  { 169, 2 },
  { 169, 4 },
  { 169, 5 },
  { 169, 4 },
  { 169, 6 },
  { 169, 7 },
  { 180, 4 },
  { 180, 3 },
  { 180, 6 },
  { 180, 5 },
  { 181, 6 },
  { 181, 5 },
  { 181, 8 },
  { 181, 7 },
  { 181, 10 },
  { 181, 9 },
  { 179, 6 },
  { 179, 5 },
  { 179, 8 },
  { 179, 7 },
  { 179, 8 },
  { 179, 7 },
  { 169, 1 },
  { 169, 1 },
  { 169, 1 },
  { 169, 5 },
  { 186, 3 },
  { 186, 1 },
  { 187, 1 },
  { 185, 3 },
  { 185, 1 },
  { 188, 3 },
  { 188, 1 },
  { 189, 1 },
  { 189, 1 },
  { 189, 1 },
  { 190, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 3 },
  { 129, 1 },
  { 166, 1 },
  { 115, 1 },
  { 116, 1 },
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
// 1031 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy129;
}
// 3742 "parser.c"
        break;
      case 1:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 131:
      case 132:
      case 133:
      case 134:
      case 135:
      case 136:
      case 137:
      case 138:
      case 139:
      case 140:
      case 141:
      case 142:
      case 143:
      case 144:
      case 145:
      case 146:
      case 147:
      case 148:
      case 149:
      case 150:
      case 151:
      case 206:
      case 233:
      case 268:
      case 303:
      case 304:
      case 305:
      case 309:
      case 317:
      case 327:
// 1037 "parser.lemon"
{
	yygotominor.yy129 = yymsp[0].minor.yy129;
}
// 3784 "parser.c"
        break;
      case 2:
      case 40:
      case 56:
      case 58:
      case 60:
      case 88:
      case 129:
      case 163:
      case 199:
// 1041 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_list(yymsp[-1].minor.yy129, yymsp[0].minor.yy129);
}
// 3799 "parser.c"
        break;
      case 3:
      case 24:
      case 41:
      case 53:
      case 57:
      case 59:
      case 61:
      case 89:
      case 101:
      case 107:
      case 130:
      case 164:
      case 180:
      case 200:
      case 209:
      case 230:
      case 308:
      case 311:
// 1045 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_list(NULL, yymsp[0].minor.yy129);
}
// 3823 "parser.c"
        break;
      case 9:
// 1070 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 3832 "parser.c"
        break;
      case 10:
// 1074 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3840 "parser.c"
        break;
      case 11:
// 1078 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy129, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3849 "parser.c"
        break;
      case 12:
// 1082 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy129, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3857 "parser.c"
        break;
      case 13:
// 1086 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy129, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3866 "parser.c"
        break;
      case 14:
// 1090 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy129, 0, 0, NULL, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3875 "parser.c"
        break;
      case 15:
// 1094 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy129, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(45,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3885 "parser.c"
        break;
      case 16:
// 1098 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy129, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3894 "parser.c"
        break;
      case 17:
// 1102 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy129, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3904 "parser.c"
        break;
      case 18:
// 1106 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy129, 1, 0, NULL, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3914 "parser.c"
        break;
      case 19:
// 1110 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy129, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3923 "parser.c"
        break;
      case 20:
// 1114 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy129, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3933 "parser.c"
        break;
      case 21:
      case 50:
// 1118 "parser.lemon"
{
	yygotominor.yy129 = NULL;
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3943 "parser.c"
        break;
      case 22:
      case 51:
// 1122 "parser.lemon"
{
	yygotominor.yy129 = yymsp[-1].minor.yy129;
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3953 "parser.c"
        break;
      case 23:
      case 52:
      case 106:
      case 179:
      case 208:
      case 229:
      case 307:
      case 310:
// 1126 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_list(yymsp[-2].minor.yy129, yymsp[0].minor.yy129);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3968 "parser.c"
        break;
      case 25:
      case 270:
      case 314:
// 1134 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3977 "parser.c"
        break;
      case 26:
// 1138 "parser.lemon"
{
  yygotominor.yy129 = NULL;
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3986 "parser.c"
        break;
      case 27:
// 1142 "parser.lemon"
{
  yygotominor.yy129 = yymsp[-1].minor.yy129;
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3995 "parser.c"
        break;
      case 28:
// 1146 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
}
// 4002 "parser.c"
        break;
      case 29:
// 1150 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy129, status->scanner_state);
}
// 4009 "parser.c"
        break;
      case 30:
// 1154 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(NULL, yymsp[0].minor.yy129, NULL, status->scanner_state);
}
// 4016 "parser.c"
        break;
      case 31:
// 1158 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(yymsp[-1].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
}
// 4023 "parser.c"
        break;
      case 32:
// 1162 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(yymsp[-1].minor.yy129, NULL, yymsp[0].minor.yy129, status->scanner_state);
}
// 4030 "parser.c"
        break;
      case 33:
// 1166 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(yymsp[0].minor.yy129, NULL, yymsp[-1].minor.yy129, status->scanner_state);
}
// 4037 "parser.c"
        break;
      case 34:
// 1170 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(NULL, yymsp[0].minor.yy129, yymsp[-1].minor.yy129, status->scanner_state);
}
// 4044 "parser.c"
        break;
      case 35:
// 1174 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(yymsp[-2].minor.yy129, yymsp[0].minor.yy129, yymsp[-1].minor.yy129, status->scanner_state);
}
// 4051 "parser.c"
        break;
      case 36:
// 1178 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_definition(yymsp[-1].minor.yy129, yymsp[0].minor.yy129, yymsp[-2].minor.yy129, status->scanner_state);
}
// 4058 "parser.c"
        break;
      case 37:
// 1182 "parser.lemon"
{
  yygotominor.yy129 = xx_ret_interface_definition(NULL, yymsp[0].minor.yy129, status->scanner_state);
}
// 4065 "parser.c"
        break;
      case 38:
// 1186 "parser.lemon"
{
  yygotominor.yy129 = xx_ret_interface_definition(yymsp[0].minor.yy129, NULL, status->scanner_state);
}
// 4072 "parser.c"
        break;
      case 39:
// 1190 "parser.lemon"
{
  yygotominor.yy129 = xx_ret_interface_definition(yymsp[0].minor.yy129, yymsp[-1].minor.yy129, status->scanner_state);
}
// 4079 "parser.c"
        break;
      case 42:
// 1203 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_property(yymsp[-2].minor.yy129, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4087 "parser.c"
        break;
      case 43:
// 1207 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_property(yymsp[-2].minor.yy129, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4095 "parser.c"
        break;
      case 44:
// 1211 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_property(yymsp[-4].minor.yy129, yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4104 "parser.c"
        break;
      case 45:
// 1215 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_property(yymsp[-4].minor.yy129, yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4113 "parser.c"
        break;
      case 46:
// 1219 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_property(yymsp[-3].minor.yy129, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4121 "parser.c"
        break;
      case 47:
// 1223 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_property(yymsp[-3].minor.yy129, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4129 "parser.c"
        break;
      case 48:
// 1227 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_property(yymsp[-5].minor.yy129, yymsp[-4].minor.yy0, yymsp[-2].minor.yy129, yymsp[-6].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4138 "parser.c"
        break;
      case 49:
// 1231 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_property(yymsp[-5].minor.yy129, yymsp[-4].minor.yy0, yymsp[-2].minor.yy129, NULL, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4147 "parser.c"
        break;
      case 54:
// 1251 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4154 "parser.c"
        break;
      case 55:
// 1255 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4161 "parser.c"
        break;
      case 62:
// 1284 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4171 "parser.c"
        break;
      case 63:
// 1288 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4181 "parser.c"
        break;
      case 64:
// 1293 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-6].minor.yy129, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4193 "parser.c"
        break;
      case 65:
// 1297 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-7].minor.yy129, yymsp[-5].minor.yy0, yymsp[-3].minor.yy129, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4205 "parser.c"
        break;
      case 66:
// 1301 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-7].minor.yy129, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4217 "parser.c"
        break;
      case 67:
// 1305 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-8].minor.yy129, yymsp[-6].minor.yy0, yymsp[-4].minor.yy129, yymsp[-1].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4229 "parser.c"
        break;
      case 68:
// 1309 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-6].minor.yy129, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4241 "parser.c"
        break;
      case 69:
// 1313 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-7].minor.yy129, yymsp[-5].minor.yy0, yymsp[-3].minor.yy129, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4253 "parser.c"
        break;
      case 70:
// 1317 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-7].minor.yy129, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy129, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4265 "parser.c"
        break;
      case 71:
// 1321 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-8].minor.yy129, yymsp[-6].minor.yy0, yymsp[-4].minor.yy129, yymsp[-1].minor.yy129, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4277 "parser.c"
        break;
      case 72:
// 1325 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-8].minor.yy129, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4290 "parser.c"
        break;
      case 73:
// 1329 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-9].minor.yy129, yymsp[-7].minor.yy0, yymsp[-5].minor.yy129, NULL, NULL, yymsp[-2].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4303 "parser.c"
        break;
      case 74:
// 1333 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-9].minor.yy129, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy129, NULL, yymsp[-3].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4316 "parser.c"
        break;
      case 75:
// 1337 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-10].minor.yy129, yymsp[-8].minor.yy0, yymsp[-6].minor.yy129, yymsp[-1].minor.yy129, NULL, yymsp[-3].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-9].minor);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4329 "parser.c"
        break;
      case 76:
// 1341 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-8].minor.yy129, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4342 "parser.c"
        break;
      case 77:
// 1345 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-9].minor.yy129, yymsp[-7].minor.yy0, yymsp[-5].minor.yy129, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4355 "parser.c"
        break;
      case 78:
// 1349 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-9].minor.yy129, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy129, yymsp[-10].minor.yy0, yymsp[-3].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4368 "parser.c"
        break;
      case 79:
// 1353 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-10].minor.yy129, yymsp[-8].minor.yy0, yymsp[-6].minor.yy129, yymsp[-1].minor.yy129, yymsp[-11].minor.yy0, yymsp[-3].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-9].minor);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4381 "parser.c"
        break;
      case 80:
// 1358 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-7].minor.yy129, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4393 "parser.c"
        break;
      case 81:
// 1362 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-8].minor.yy129, yymsp[-6].minor.yy0, yymsp[-4].minor.yy129, NULL, NULL, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4405 "parser.c"
        break;
      case 82:
// 1366 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-7].minor.yy129, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4417 "parser.c"
        break;
      case 83:
// 1370 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-8].minor.yy129, yymsp[-6].minor.yy0, yymsp[-4].minor.yy129, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4429 "parser.c"
        break;
      case 84:
// 1374 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-5].minor.yy129, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4440 "parser.c"
        break;
      case 85:
// 1378 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-6].minor.yy129, yymsp[-4].minor.yy0, yymsp[-2].minor.yy129, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4451 "parser.c"
        break;
      case 86:
// 1382 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-5].minor.yy129, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4462 "parser.c"
        break;
      case 87:
// 1386 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_class_method(yymsp[-6].minor.yy129, yymsp[-4].minor.yy0, yymsp[-2].minor.yy129, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4473 "parser.c"
        break;
      case 90:
// 1399 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4481 "parser.c"
        break;
      case 91:
// 1403 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4489 "parser.c"
        break;
      case 92:
// 1407 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4497 "parser.c"
        break;
      case 93:
// 1411 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4505 "parser.c"
        break;
      case 94:
// 1415 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4513 "parser.c"
        break;
      case 95:
// 1419 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("inline");
  yy_destructor(57,&yymsp[0].minor);
}
// 4521 "parser.c"
        break;
      case 96:
// 1423 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("abstract");
  yy_destructor(47,&yymsp[0].minor);
}
// 4529 "parser.c"
        break;
      case 97:
// 1427 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("final");
  yy_destructor(48,&yymsp[0].minor);
}
// 4537 "parser.c"
        break;
      case 98:
// 1432 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[0].minor);
}
// 4545 "parser.c"
        break;
      case 99:
// 1436 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_return_type(0, yymsp[0].minor.yy129, status->scanner_state);
}
// 4552 "parser.c"
        break;
      case 100:
// 1440 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_list(yymsp[-2].minor.yy129, yymsp[0].minor.yy129);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4560 "parser.c"
        break;
      case 102:
// 1448 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_return_type_item(yymsp[0].minor.yy129, NULL, 0, 0, status->scanner_state);
}
// 4567 "parser.c"
        break;
      case 103:
// 1452 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_return_type_item(yymsp[-1].minor.yy129, NULL, 1, 0, status->scanner_state);
  yy_destructor(36,&yymsp[0].minor);
}
// 4575 "parser.c"
        break;
      case 104:
// 1456 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy129, 0, 0, status->scanner_state);
}
// 4582 "parser.c"
        break;
      case 105:
// 1460 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy129, 0, 1, status->scanner_state);
}
// 4589 "parser.c"
        break;
      case 108:
// 1473 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4596 "parser.c"
        break;
      case 109:
// 1477 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_parameter(yymsp[-1].minor.yy129, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4603 "parser.c"
        break;
      case 110:
// 1481 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_parameter(yymsp[-2].minor.yy129, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 4611 "parser.c"
        break;
      case 111:
// 1485 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_parameter(NULL, yymsp[-1].minor.yy129, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4618 "parser.c"
        break;
      case 112:
// 1489 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy129, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4626 "parser.c"
        break;
      case 113:
// 1493 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_parameter(yymsp[-3].minor.yy129, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy129, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4634 "parser.c"
        break;
      case 114:
// 1497 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_parameter(yymsp[-4].minor.yy129, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy129, 1, status->scanner_state);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4643 "parser.c"
        break;
      case 115:
// 1501 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_parameter(NULL, yymsp[-3].minor.yy129, yymsp[-2].minor.yy0, yymsp[0].minor.yy129, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4651 "parser.c"
        break;
      case 116:
// 1505 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 4660 "parser.c"
        break;
      case 117:
// 1509 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 4671 "parser.c"
        break;
      case 118:
// 1513 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4679 "parser.c"
        break;
      case 119:
// 1517 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(61,&yymsp[0].minor);
}
// 4687 "parser.c"
        break;
      case 120:
// 1521 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4695 "parser.c"
        break;
      case 121:
// 1525 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(63,&yymsp[0].minor);
}
// 4703 "parser.c"
        break;
      case 122:
// 1529 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4711 "parser.c"
        break;
      case 123:
// 1533 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(65,&yymsp[0].minor);
}
// 4719 "parser.c"
        break;
      case 124:
// 1537 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(66,&yymsp[0].minor);
}
// 4727 "parser.c"
        break;
      case 125:
// 1541 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(67,&yymsp[0].minor);
}
// 4735 "parser.c"
        break;
      case 126:
// 1545 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(68,&yymsp[0].minor);
}
// 4743 "parser.c"
        break;
      case 127:
// 1549 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(69,&yymsp[0].minor);
}
// 4751 "parser.c"
        break;
      case 128:
// 1553 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(70,&yymsp[0].minor);
}
// 4759 "parser.c"
        break;
      case 152:
// 1651 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4767 "parser.c"
        break;
      case 153:
// 1655 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4776 "parser.c"
        break;
      case 154:
// 1659 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(72,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4785 "parser.c"
        break;
      case 155:
// 1663 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_if_statement(yymsp[-2].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4795 "parser.c"
        break;
      case 156:
// 1667 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_if_statement(yymsp[-5].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4808 "parser.c"
        break;
      case 157:
// 1671 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_if_statement(yymsp[-3].minor.yy129, yymsp[-1].minor.yy129, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4818 "parser.c"
        break;
      case 158:
// 1675 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_if_statement(yymsp[-7].minor.yy129, yymsp[-5].minor.yy129, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(73,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4831 "parser.c"
        break;
      case 159:
// 1679 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_if_statement(yymsp[-6].minor.yy129, yymsp[-4].minor.yy129, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4844 "parser.c"
        break;
      case 160:
// 1683 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_if_statement(yymsp[-6].minor.yy129, NULL, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4857 "parser.c"
        break;
      case 161:
// 1687 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_switch_statement(yymsp[-2].minor.yy129, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4867 "parser.c"
        break;
      case 162:
// 1691 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_switch_statement(yymsp[-3].minor.yy129, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4877 "parser.c"
        break;
      case 165:
// 1703 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_case_clause(yymsp[-1].minor.yy129, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(77,&yymsp[0].minor);
}
// 4886 "parser.c"
        break;
      case 166:
// 1707 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_case_clause(yymsp[-2].minor.yy129, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(76,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 4895 "parser.c"
        break;
      case 167:
// 1711 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_case_clause(NULL, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 4904 "parser.c"
        break;
      case 168:
// 1715 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4914 "parser.c"
        break;
      case 169:
// 1719 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_loop_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4924 "parser.c"
        break;
      case 170:
// 1723 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_while_statement(yymsp[-2].minor.yy129, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4934 "parser.c"
        break;
      case 171:
// 1727 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_while_statement(yymsp[-3].minor.yy129, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4944 "parser.c"
        break;
      case 172:
// 1731 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_do_while_statement(yymsp[-1].minor.yy129, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4956 "parser.c"
        break;
      case 173:
// 1735 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_do_while_statement(yymsp[-1].minor.yy129, yymsp[-4].minor.yy129, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4968 "parser.c"
        break;
      case 174:
// 1739 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_for_statement(yymsp[-3].minor.yy129, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4979 "parser.c"
        break;
      case 175:
// 1743 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_for_statement(yymsp[-3].minor.yy129, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(82,&yymsp[-7].minor);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4991 "parser.c"
        break;
      case 176:
// 1747 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_for_statement(yymsp[-3].minor.yy129, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(82,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5003 "parser.c"
        break;
      case 177:
// 1751 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_for_statement(yymsp[-3].minor.yy129, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(82,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5016 "parser.c"
        break;
      case 178:
// 1755 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(85,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5025 "parser.c"
        break;
      case 181:
// 1768 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("assign");
  yy_destructor(52,&yymsp[0].minor);
}
// 5033 "parser.c"
        break;
      case 182:
// 1773 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("add-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 5041 "parser.c"
        break;
      case 183:
// 1778 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("sub-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 5049 "parser.c"
        break;
      case 184:
// 1782 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("mult-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 5057 "parser.c"
        break;
      case 185:
// 1786 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("div-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5065 "parser.c"
        break;
      case 186:
// 1790 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("concat-assign");
  yy_destructor(90,&yymsp[0].minor);
}
// 5073 "parser.c"
        break;
      case 187:
// 1794 "parser.lemon"
{
	yygotominor.yy129 = json_object_new_string("mod-assign");
  yy_destructor(91,&yymsp[0].minor);
}
// 5081 "parser.c"
        break;
      case 188:
// 1799 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy129, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy129, status->scanner_state);
}
// 5088 "parser.c"
        break;
      case 189:
// 1804 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy129, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
}
// 5096 "parser.c"
        break;
      case 190:
// 1809 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy129, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 5106 "parser.c"
        break;
      case 191:
// 1814 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy129, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 5116 "parser.c"
        break;
      case 192:
// 1819 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy129, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5126 "parser.c"
        break;
      case 193:
// 1824 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy129, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy129, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
}
// 5134 "parser.c"
        break;
      case 194:
// 1829 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy129, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(93,&yymsp[-3].minor);
}
// 5142 "parser.c"
        break;
      case 195:
// 1834 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy129, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(93,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5152 "parser.c"
        break;
      case 196:
// 1839 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy129, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy129, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(93,&yymsp[-4].minor);
}
// 5160 "parser.c"
        break;
      case 197:
// 1844 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy129, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5169 "parser.c"
        break;
      case 198:
// 1849 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy129, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy129, yymsp[0].minor.yy129, status->scanner_state);
}
// 5176 "parser.c"
        break;
      case 201:
// 1861 "parser.lemon"
{
	yygotominor.yy129 = yymsp[-1].minor.yy129;
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5185 "parser.c"
        break;
      case 202:
// 1866 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(94,&yymsp[0].minor);
}
// 5194 "parser.c"
        break;
      case 203:
// 1871 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(95,&yymsp[0].minor);
}
// 5203 "parser.c"
        break;
      case 204:
// 1876 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 5211 "parser.c"
        break;
      case 205:
// 1881 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(95,&yymsp[0].minor);
}
// 5219 "parser.c"
        break;
      case 207:
// 1889 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_echo_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(96,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5228 "parser.c"
        break;
      case 210:
// 1901 "parser.lemon"
{
	yygotominor.yy129 = yymsp[0].minor.yy129;;
}
// 5235 "parser.c"
        break;
      case 211:
// 1906 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_mcall_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5243 "parser.c"
        break;
      case 212:
// 1911 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_fcall_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5251 "parser.c"
        break;
      case 213:
// 1916 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_scall_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5259 "parser.c"
        break;
      case 214:
// 1921 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_fetch_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5267 "parser.c"
        break;
      case 215:
// 1926 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_return_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5276 "parser.c"
        break;
      case 216:
// 1931 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(97,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5285 "parser.c"
        break;
      case 217:
// 1936 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_unset_statement(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(98,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5294 "parser.c"
        break;
      case 218:
// 1941 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_throw_exception(yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(99,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5303 "parser.c"
        break;
      case 219:
// 1945 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5312 "parser.c"
        break;
      case 220:
// 1949 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5321 "parser.c"
        break;
      case 221:
// 1953 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5330 "parser.c"
        break;
      case 222:
// 1957 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5339 "parser.c"
        break;
      case 223:
// 1961 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5348 "parser.c"
        break;
      case 224:
// 1965 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5357 "parser.c"
        break;
      case 225:
// 1969 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5366 "parser.c"
        break;
      case 226:
// 1973 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5375 "parser.c"
        break;
      case 227:
// 1977 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5384 "parser.c"
        break;
      case 228:
// 1981 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5393 "parser.c"
        break;
      case 231:
// 1993 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5400 "parser.c"
        break;
      case 232:
// 1997 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 5408 "parser.c"
        break;
      case 234:
// 2005 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("not", yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5416 "parser.c"
        break;
      case 235:
// 2009 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("isset", yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
}
// 5424 "parser.c"
        break;
      case 236:
// 2013 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("require", yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5432 "parser.c"
        break;
      case 237:
// 2017 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("clone", yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5440 "parser.c"
        break;
      case 238:
// 2021 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("empty", yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5448 "parser.c"
        break;
      case 239:
// 2025 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("likely", yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5456 "parser.c"
        break;
      case 240:
// 2029 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("unlikely", yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5464 "parser.c"
        break;
      case 241:
// 2033 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("equals", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5472 "parser.c"
        break;
      case 242:
// 2037 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("not-equals", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5480 "parser.c"
        break;
      case 243:
// 2041 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("identical", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5488 "parser.c"
        break;
      case 244:
// 2045 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("not-identical", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5496 "parser.c"
        break;
      case 245:
// 2049 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("less", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5504 "parser.c"
        break;
      case 246:
// 2053 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("greater", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5512 "parser.c"
        break;
      case 247:
// 2057 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("less-equal", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5520 "parser.c"
        break;
      case 248:
// 2061 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5528 "parser.c"
        break;
      case 249:
// 2065 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("list", yymsp[-1].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5537 "parser.c"
        break;
      case 250:
// 2069 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("cast", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
}
// 5546 "parser.c"
        break;
      case 251:
// 2073 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5555 "parser.c"
        break;
      case 252:
// 2077 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("property-access", yymsp[-2].minor.yy129, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5563 "parser.c"
        break;
      case 253:
// 2081 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy129, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5573 "parser.c"
        break;
      case 254:
// 2085 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy129, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5583 "parser.c"
        break;
      case 255:
// 2089 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-1].minor);
}
// 5591 "parser.c"
        break;
      case 256:
      case 325:
// 2093 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-1].minor);
}
// 5600 "parser.c"
        break;
      case 257:
// 2102 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("array-access", yymsp[-3].minor.yy129, yymsp[-1].minor.yy129, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5609 "parser.c"
        break;
      case 258:
// 2107 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("add", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5617 "parser.c"
        break;
      case 259:
// 2112 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("sub", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5625 "parser.c"
        break;
      case 260:
// 2117 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("mul", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5633 "parser.c"
        break;
      case 261:
// 2122 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("div", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5641 "parser.c"
        break;
      case 262:
// 2127 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("mod", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5649 "parser.c"
        break;
      case 263:
// 2132 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("concat", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5657 "parser.c"
        break;
      case 264:
// 2137 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("and", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5665 "parser.c"
        break;
      case 265:
// 2142 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("or", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 5673 "parser.c"
        break;
      case 266:
// 2147 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("instanceof", yymsp[-2].minor.yy129, yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5681 "parser.c"
        break;
      case 267:
// 2152 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy129, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5690 "parser.c"
        break;
      case 269:
// 2162 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("typeof", yymsp[0].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5698 "parser.c"
        break;
      case 271:
      case 316:
      case 318:
// 2172 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5707 "parser.c"
        break;
      case 272:
      case 315:
      case 320:
// 2177 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5716 "parser.c"
        break;
      case 273:
      case 319:
// 2182 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5724 "parser.c"
        break;
      case 274:
      case 321:
// 2187 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5732 "parser.c"
        break;
      case 275:
      case 322:
// 2192 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5741 "parser.c"
        break;
      case 276:
      case 324:
// 2197 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(104,&yymsp[0].minor);
}
// 5750 "parser.c"
        break;
      case 277:
      case 323:
// 2202 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(105,&yymsp[0].minor);
}
// 5759 "parser.c"
        break;
      case 278:
      case 326:
// 2207 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5767 "parser.c"
        break;
      case 279:
// 2212 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5776 "parser.c"
        break;
      case 280:
// 2217 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("array", yymsp[-1].minor.yy129, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5785 "parser.c"
        break;
      case 281:
// 2222 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5793 "parser.c"
        break;
      case 282:
// 2227 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5803 "parser.c"
        break;
      case 283:
// 2232 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5813 "parser.c"
        break;
      case 284:
// 2237 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5823 "parser.c"
        break;
      case 285:
// 2242 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5835 "parser.c"
        break;
      case 286:
// 2247 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(35,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5847 "parser.c"
        break;
      case 287:
// 2252 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5856 "parser.c"
        break;
      case 288:
// 2257 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5865 "parser.c"
        break;
      case 289:
// 2262 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5876 "parser.c"
        break;
      case 290:
// 2267 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5887 "parser.c"
        break;
      case 291:
// 2272 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(93,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5897 "parser.c"
        break;
      case 292:
// 2277 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5907 "parser.c"
        break;
      case 293:
// 2282 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(93,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5919 "parser.c"
        break;
      case 294:
// 2287 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(93,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5931 "parser.c"
        break;
      case 295:
// 2292 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(49,&yymsp[-9].minor);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(93,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5945 "parser.c"
        break;
      case 296:
// 2297 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-8].minor);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(93,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5959 "parser.c"
        break;
      case 297:
// 2302 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_mcall(1, yymsp[-5].minor.yy129, yymsp[-3].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5969 "parser.c"
        break;
      case 298:
// 2307 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_mcall(1, yymsp[-4].minor.yy129, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5979 "parser.c"
        break;
      case 299:
// 2312 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_mcall(2, yymsp[-7].minor.yy129, yymsp[-4].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5991 "parser.c"
        break;
      case 300:
// 2317 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_mcall(2, yymsp[-6].minor.yy129, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6003 "parser.c"
        break;
      case 301:
// 2322 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_mcall(3, yymsp[-7].minor.yy129, yymsp[-4].minor.yy0, yymsp[-1].minor.yy129, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6015 "parser.c"
        break;
      case 302:
// 2327 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_mcall(3, yymsp[-6].minor.yy129, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6027 "parser.c"
        break;
      case 306:
// 2347 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_expr("ternary", yymsp[-4].minor.yy129, yymsp[-2].minor.yy129, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 6036 "parser.c"
        break;
      case 312:
// 2371 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_array_item(yymsp[-2].minor.yy129, yymsp[0].minor.yy129, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 6044 "parser.c"
        break;
      case 313:
// 2375 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_array_item(NULL, yymsp[0].minor.yy129, status->scanner_state);
}
// 6051 "parser.c"
        break;
      case 328:
// 2435 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6058 "parser.c"
        break;
      case 329:
// 2439 "parser.lemon"
{
	yygotominor.yy129 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 6065 "parser.c"
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
// 988 "parser.lemon"


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

// 6141 "parser.c"
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
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
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
			case XX_T_DIVASSIGN:
				xx_(xx_parser, XX_DIVASSIGN, NULL, parser_status);
				break;
			case XX_T_MULASSIGN:
				xx_(xx_parser, XX_MULASSIGN, NULL, parser_status);
				break;
			case XX_T_CONCATASSIGN:
				xx_(xx_parser, XX_CONCATASSIGN, NULL, parser_status);
				break;
			case XX_T_MODASSIGN:
				xx_(xx_parser, XX_MODASSIGN, NULL, parser_status);
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
			case XX_T_QUESTION:
				xx_(xx_parser, XX_QUESTION, NULL, parser_status);
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
			case XX_T_CBLOCK:
				xx_parse_with_token(xx_parser, XX_T_CBLOCK, XX_CBLOCK, &token, parser_status);
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
			case XX_T_BITWISE_AND:
				xx_(xx_parser, XX_BITWISE_AND, NULL, parser_status);
				break;
			case XX_T_BITWISE_OR:
				xx_(xx_parser, XX_BITWISE_OR, NULL, parser_status);
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

			case XX_T_VOID:
				xx_(xx_parser, XX_VOID, NULL, parser_status);
				break;
			case XX_T_LIKELY:
				xx_(xx_parser, XX_LIKELY, NULL, parser_status);
				break;
			case XX_T_UNLIKELY:
				xx_(xx_parser, XX_UNLIKELY, NULL, parser_status);
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
