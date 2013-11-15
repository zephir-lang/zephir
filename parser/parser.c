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
#define YYNOCODE 191
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy235;
  int yy381;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 677
#define YYNRULE 329
#define YYERRORSYMBOL 106
#define YYERRSYMDT yy381
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
 /*     0 */   232,  318,  241,  244,  235,  238,    3,    4,    5,    6,
 /*    10 */     7,    8,  492,  285, 1007,    1,    2,  676,    4,    5,
 /*    20 */     6,    7,    8,  637,  202,  229,  293,  298,  312,  226,
 /*    30 */   623,  308,  371,  638,  289,  185,  609,  152,   30,  613,
 /*    40 */   635,   59,  319,  546,  526,  137,  611,  307,  524,  279,
 /*    50 */   206,  122,  552,  489,  498,  507,  510,  501,  504,  513,
 /*    60 */   519,  516,  620,  522,  186,  188,  190,  419,  199,  623,
 /*    70 */   116,  406,  210,  214,  219,  393,   82,   90,  400,   96,
 /*    80 */   445,  406,  451,  431,  301,  490,  497,  593,  464,  479,
 /*    90 */   483,  486,  300,  302,  303,  304,  305,  306,  527,  232,
 /*   100 */   591,  241,  244,  235,  238,  407,  408,  409,  410,  411,
 /*   110 */   587,  417,  285,  426,  431,  407,  408,  409,  410,  411,
 /*   120 */   374,  677,  432,  433,  229,  293,  298,  312,  226,  412,
 /*   130 */   308,  458,  431,  289,  185,  261,  263,  265,  318,  322,
 /*   140 */   629,  319,  545,  526,  254,  250,  307,   11,  279,  624,
 /*   150 */   630,  318,  489,  498,  507,  510,  501,  504,  513,  519,
 /*   160 */   516,   15,  522,  186,  188,  190,   18,  199,  253,  317,
 /*   170 */   380,  210,  214,  219,  393,  330,  313,  400,  326,  328,
 /*   180 */   327,  297,  378,  301,  344,  351,  329,  464,  479,  483,
 /*   190 */   486,  300,  302,  303,  304,  305,  306,  527,  232,  311,
 /*   200 */   241,  244,  235,  238,  257,  259,  267,  261,  263,  265,
 /*   210 */   459,  285,  569,  429,  499,  497,  254,  250,   82,   90,
 /*   220 */   990,   96,  251,  229,  293,  298,  312,  226,  582,  308,
 /*   230 */   373,  640,  289,  185,  647,  290,  613,  635,  623,  122,
 /*   240 */   319,  580,  526,  611,  663,  307,  666,  279,  122,  147,
 /*   250 */    66,  489,  498,  507,  510,  501,  504,  513,  519,  516,
 /*   260 */    10,  522,  186,  188,  190,  430,  199,  617,  492,  154,
 /*   270 */   210,  214,  219,  393,  623,  153,  400,  618,   13,  406,
 /*   280 */   122,  117,  301,  492,  311,   14,  464,  479,  483,  486,
 /*   290 */   300,  302,  303,  304,  305,  306,  527,  232,   16,  241,
 /*   300 */   244,  235,  238,  604,  491,   34,   58,   37,   39,   40,
 /*   310 */   285,  502,  497,  407,  408,  409,  410,  411,   18,  500,
 /*   320 */   290,  204,  229,  293,  298,  312,  226,  118,  308,   60,
 /*   330 */   121,  289,  185,  318,  610,   82,   90,  154,   96,  319,
 /*   340 */   578,  526,  123,  611,  307,   17,  279,  206,  122,  552,
 /*   350 */   489,  498,  507,  510,  501,  504,  513,  519,  516,   97,
 /*   360 */   522,  186,  188,  190,  384,  199,  253,  505,  497,  210,
 /*   370 */   214,  219,  393,  627,   21,  400,  326,  328,  327,  297,
 /*   380 */   492,  301,  492,  383,  329,  464,  479,  483,  486,  300,
 /*   390 */   302,  303,  304,  305,  306,  527,  232,   27,  241,  244,
 /*   400 */   235,  238,  253,   36,   37,   39,   40,   82,   90,  285,
 /*   410 */    67,   43,  326,  328,  327,  297,  506,  626,  503,  377,
 /*   420 */   329,  229,  293,  298,  312,  226,  144,  308,   44,  147,
 /*   430 */   289,  185,  508,  497,   28,   23,  615,   25,  319,  562,
 /*   440 */   526,  673,  123,  307,  124,  279,  147,  122,  122,  489,
 /*   450 */   498,  507,  510,  501,  504,  513,  519,  516,  534,  522,
 /*   460 */   186,  188,  190,  452,  199,  253,  429,   32,  210,  214,
 /*   470 */   219,  393,  203,  555,  400,  326,  328,  327,  297,  492,
 /*   480 */   301,   35,  349,  329,  464,  479,  483,  486,  300,  302,
 /*   490 */   303,  304,  305,  306,  527,  232,  597,  241,  244,  235,
 /*   500 */   238,  469,   82,   90,  403,   96,  386,  435,  285,  465,
 /*   510 */   470,  326,  328,  327,  297,  509,  326,  328,  327,  297,
 /*   520 */   229,  293,  298,  312,  226,   42,  308,  254,  250,  289,
 /*   530 */   185,  141,  107,  656,   25,  395,  147,  319,  574,  526,
 /*   540 */   402,   24,  307,  122,  279,  122,  625,  632,  489,  498,
 /*   550 */   507,  510,  501,  504,  513,  519,  516,  439,  522,  186,
 /*   560 */   188,  190,   45,  199,  253,  511,  497,  210,  214,  219,
 /*   570 */   393,  401,  463,  400,  326,  328,  327,  297,  667,  301,
 /*   580 */   655,  340,  329,  464,  479,  483,  486,  300,  302,  303,
 /*   590 */   304,  305,  306,  527,  232,  492,  241,  244,  235,  238,
 /*   600 */   646,  652,  649,  492,  650,  635,  416,  285,  514,  497,
 /*   610 */   123,  611,   62,  291,  434,   65,  122,   38,   64,  229,
 /*   620 */   293,  298,  312,  226,   46,  308,  369,   86,  289,  185,
 /*   630 */    66,  512,   83,  517,  497,  492,  319,  572,  526,  515,
 /*   640 */    69,  307,  427,  279,  147,  429,   66,  489,  498,  507,
 /*   650 */   510,  501,  504,  513,  519,  516,   70,  522,  186,  188,
 /*   660 */   190,  598,  199,  253,  492,   85,  210,  214,  219,  393,
 /*   670 */    66,  518,  400,  326,  328,  327,  297,   61,  301,   80,
 /*   680 */   333,  329,  464,  479,  483,  486,  300,  302,  303,  304,
 /*   690 */   305,  306,  527,  232,  575,  241,  244,  235,  238,  253,
 /*   700 */   521,  570,  520,  497,  571,   79,  285,  523,  497,  326,
 /*   710 */   328,  327,  297,  603,  318,  563,  323,  329,  229,  293,
 /*   720 */   298,  312,  226,  599,  308,  159,  331,  289,  185,  657,
 /*   730 */   145,  655,  655,  629,  336,  319,  561,  526,  318,  318,
 /*   740 */   307,  112,  279,  630,  115,  350,  489,  498,  507,  510,
 /*   750 */   501,  504,  513,  519,  516,   84,  522,  186,  188,  190,
 /*   760 */   359,  199,  253,  467,   87,  210,  214,  219,  393,  352,
 /*   770 */   341,  400,  326,  328,  327,  297,  318,  301,   88,  316,
 /*   780 */   329,  464,  479,  483,  486,  300,  302,  303,  304,  305,
 /*   790 */   306,  527,  232,  318,  241,  244,  235,  238,  253,  466,
 /*   800 */   318,   89,   91,   92,   95,  285,   93,  334,  326,  328,
 /*   810 */   327,  297,   43,  358,  109,  366,  329,  229,  293,  298,
 /*   820 */   312,  226,  110,  308,  324,  114,  289,  185,  120,  127,
 /*   830 */   128,  367,  129,  132,  319,  568,  526,  130,  134,  307,
 /*   840 */   133,  279,  135,  139,  140,  489,  498,  507,  510,  501,
 /*   850 */   504,  513,  519,  516,  142,  522,  186,  188,  190,  447,
 /*   860 */   199,  143,  146,  654,  210,  214,  219,  393,  150,  187,
 /*   870 */   400,  156,  157,  406,  189,  192,  301,  195,  196,  205,
 /*   880 */   464,  479,  483,  486,  300,  302,  303,  304,  305,  306,
 /*   890 */   527,  232,  201,  241,  244,  235,  238,  113,  211,   34,
 /*   900 */    58,   37,   39,   40,  285,  207,  216,  407,  408,  409,
 /*   910 */   410,  411,  208,  220,  223,  225,  229,  293,  298,  312,
 /*   920 */   226,  252,  308,  283,  286,  289,  185,  287,  292,  294,
 /*   930 */   295,  314,  321,  319,  566,  526,  320,  332,  307,  337,
 /*   940 */   279,  339,  338,  345,  489,  498,  507,  510,  501,  504,
 /*   950 */   513,  519,  516,  346,  522,  186,  188,  190,  375,  199,
 /*   960 */   253,  347,  394,  210,  214,  219,  393,  992,  376,  400,
 /*   970 */   326,  328,  327,  297,  991,  301,  362,  370,  329,  464,
 /*   980 */   479,  483,  486,  300,  302,  303,  304,  305,  306,  527,
 /*   990 */   232,  381,  241,  244,  235,  238,  469,  405,  382,  424,
 /*  1000 */   436,  404,  440,  285,  421,  468,  326,  328,  327,  297,
 /*  1010 */   437,  441,  444,  449,  472,  229,  293,  298,  312,  226,
 /*  1020 */   474,  308,  456,  476,  289,  185,  478,  494,  493,  495,
 /*  1030 */   535,  536,  319,  588,  526,  496,  549,  307,  547,  279,
 /*  1040 */   553,  558,  559,  489,  498,  507,  510,  501,  504,  513,
 /*  1050 */   519,  516,  565,  522,  186,  188,  190,  577,  199,  425,
 /*  1060 */   584,  585,  210,  214,  219,  393,  423,  605,  400,  326,
 /*  1070 */   328,  327,  297,  633,  301,  619,  622,  727,  464,  479,
 /*  1080 */   483,  486,  300,  302,  303,  304,  305,  306,  527,  232,
 /*  1090 */   621,  241,  244,  235,  238,  414,  728,  628,  631,  460,
 /*  1100 */   634,  639,  285,  643,  641,  326,  328,  327,  297,  642,
 /*  1110 */   644,  660,  653,  658,  229,  293,  298,  312,  226,  661,
 /*  1120 */   308,  662,  664,  289,  185,  147,  665,  668,  670,  671,
 /*  1130 */   672,  319,  218,  526,  674,  521,  307,  675,  279,  521,
 /*  1140 */   521,  521,  489,  498,  507,  510,  501,  504,  513,  519,
 /*  1150 */   516,  521,  522,  186,  188,  190,  224,  199,  521,  392,
 /*  1160 */   521,  210,  214,  219,  393,  521,  521,  400,  521,  326,
 /*  1170 */   328,  327,  297,  301,  521,  521,  521,  464,  479,  483,
 /*  1180 */   486,  300,  302,  303,  304,  305,  306,  527,  232,  521,
 /*  1190 */   241,  244,  235,  238,  414,  521,  521,  521,  457,  521,
 /*  1200 */   521,  285,  521,  521,  326,  328,  327,  297,  521,  521,
 /*  1210 */   521,  521,  521,  229,  293,  298,  312,  226,  521,  308,
 /*  1220 */   521,  521,  289,  185,  521,  521,  521,  521,  521,  521,
 /*  1230 */   319,  198,  526,  521,  521,  307,  521,  279,  521,  521,
 /*  1240 */   521,  489,  498,  507,  510,  501,  504,  513,  519,  516,
 /*  1250 */   521,  522,  186,  188,  190,  521,  199,  253,  521,  521,
 /*  1260 */   210,  214,  219,  393,  521,  521,  400,  326,  328,  327,
 /*  1270 */   297,  521,  301,  521,  521,  343,  464,  479,  483,  486,
 /*  1280 */   300,  302,  303,  304,  305,  306,  527,  232,  521,  241,
 /*  1290 */   244,  235,  238,  119,  521,   34,   58,   37,   39,   40,
 /*  1300 */   285,   31,  521,   34,   58,   37,   39,   40,  521,  521,
 /*  1310 */   521,  521,  229,  293,  298,  312,  226,  521,  308,  521,
 /*  1320 */   521,  289,  185,  521,  521,  521,  521,  521,  521,  319,
 /*  1330 */   556,  526,  521,  521,  307,  521,  279,  521,  521,  521,
 /*  1340 */   489,  498,  507,  510,  501,  504,  513,  519,  516,  521,
 /*  1350 */   522,  186,  188,  190,  191,  199,  521,  392,  521,  210,
 /*  1360 */   214,  219,  393,  521,  521,  400,  521,  326,  328,  327,
 /*  1370 */   297,  301,  521,  521,  521,  464,  479,  483,  486,  300,
 /*  1380 */   302,  303,  304,  305,  306,  527,  232,  521,  241,  244,
 /*  1390 */   235,  238,  414,  521,  521,  521,  453,  521,  521,  285,
 /*  1400 */   521,  521,  326,  328,  327,  297,  521,  521,  521,  521,
 /*  1410 */   521,  229,  293,  298,  312,  226,  521,  308,  521,  521,
 /*  1420 */   289,  185,  521,  521,  521,  521,  521,  521,  319,  194,
 /*  1430 */   526,  521,  521,  307,  521,  279,  521,  521,  521,  489,
 /*  1440 */   498,  507,  510,  501,  504,  513,  519,  516,  521,  522,
 /*  1450 */   186,  188,  190,  200,  199,  521,  392,  521,  210,  214,
 /*  1460 */   219,  393,  521,  521,  400,  521,  326,  328,  327,  297,
 /*  1470 */   301,  521,  521,  521,  464,  479,  483,  486,  300,  302,
 /*  1480 */   303,  304,  305,  306,  527,  232,  521,  241,  244,  235,
 /*  1490 */   238,  414,  521,  521,  521,  450,  521,  521,  285,  521,
 /*  1500 */   521,  326,  328,  327,  297,  521,  521,  521,  521,  521,
 /*  1510 */   229,  293,  298,  312,  226,  521,  308,  521,  521,  289,
 /*  1520 */   185,  521,  521,  521,  521,  521,  521,  319,  222,  526,
 /*  1530 */   521,  521,  307,  521,  279,  521,  521,  521,  489,  498,
 /*  1540 */   507,  510,  501,  504,  513,  519,  516,  521,  522,  186,
 /*  1550 */   188,  190,  548,  199,  521,  392,  521,  210,  214,  219,
 /*  1560 */   393,  521,  521,  400,  521,  326,  328,  327,  297,  301,
 /*  1570 */   521,  521,  521,  464,  479,  483,  486,  300,  302,  303,
 /*  1580 */   304,  305,  306,  527,  232,  521,  241,  244,  235,  238,
 /*  1590 */   414,  521,  521,  521,  446,  521,  521,  285,  521,  521,
 /*  1600 */   326,  328,  327,  297,  521,  521,  521,  521,  521,  229,
 /*  1610 */   293,  298,  312,  226,  521,  308,  521,  521,  289,  185,
 /*  1620 */   521,  521,  521,  521,  521,  521,  319,  557,  526,  521,
 /*  1630 */   521,  307,  521,  279,  521,  521,  521,  489,  498,  507,
 /*  1640 */   510,  501,  504,  513,  519,  516,  521,  522,  186,  188,
 /*  1650 */   190,  215,  199,  521,  392,  521,  210,  214,  219,  393,
 /*  1660 */   521,  521,  400,  521,  326,  328,  327,  297,  301,  521,
 /*  1670 */   521,  521,  464,  479,  483,  486,  300,  302,  303,  304,
 /*  1680 */   305,  306,  527,  232,  521,  241,  244,  235,  238,  414,
 /*  1690 */   521,  521,  521,  442,  521,  521,  285,  521,  521,  326,
 /*  1700 */   328,  327,  297,  521,  521,  521,  521,  521,  229,  293,
 /*  1710 */   298,  312,  226,  521,  308,  521,  521,  289,  185,  521,
 /*  1720 */   521,  521,  521,  521,  521,  319,  550,  526,  521,  521,
 /*  1730 */   307,  521,  279,  521,  521,  521,  489,  498,  507,  510,
 /*  1740 */   501,  504,  513,  519,  516,  521,  522,  186,  188,  190,
 /*  1750 */   521,  199,  230,  521,  521,  210,  214,  219,  393,  521,
 /*  1760 */   521,  400,  326,  328,  327,  297,  521,  301,  521,  521,
 /*  1770 */   521,  464,  479,  483,  486,  300,  302,  303,  304,  305,
 /*  1780 */   306,  527,  232,  521,  241,  244,  235,  238,  414,  521,
 /*  1790 */   521,  521,  438,  521,  521,  285,  521,  521,  326,  328,
 /*  1800 */   327,  297,  521,  521,  521,  521,  521,  229,  293,  298,
 /*  1810 */   312,  226,  521,  308,  521,  521,  289,  185,  521,  521,
 /*  1820 */   521,  521,  521,  521,  319,  399,  526,  521,  521,  307,
 /*  1830 */   521,  279,  521,  521,  521,  489,  498,  507,  510,  501,
 /*  1840 */   504,  513,  519,  516,  521,  522,  186,  188,  190,  521,
 /*  1850 */   199,  388,  521,  521,  210,  214,  219,  393,  521,  521,
 /*  1860 */   400,  326,  328,  327,  297,  521,  301,  521,  521,  521,
 /*  1870 */   464,  479,  483,  486,  300,  302,  303,  304,  305,  306,
 /*  1880 */   527,  232,  521,  241,  244,  235,  238,  414,  521,  521,
 /*  1890 */   521,  428,  521,  521,  285,  521,  521,  326,  328,  327,
 /*  1900 */   297,  521,  521,  521,  521,  521,  229,  293,  298,  312,
 /*  1910 */   226,  521,  308,  521,  521,  289,  185,  521,  521,  521,
 /*  1920 */   521,  521,  521,  319,  213,  526,  521,  521,  307,  521,
 /*  1930 */   279,  521,  521,  521,  489,  498,  507,  510,  501,  504,
 /*  1940 */   513,  519,  516,  521,  522,  186,  188,  190,  521,  199,
 /*  1950 */   480,  521,  521,  210,  214,  219,  393,  521,  521,  400,
 /*  1960 */   326,  328,  327,  297,  521,  301,  521,  521,  521,  464,
 /*  1970 */   479,  483,  486,  300,  302,  303,  304,  305,  306,  527,
 /*  1980 */   232,  521,  241,  244,  235,  238,  414,  521,  521,  521,
 /*  1990 */   422,  521,  521,  285,  521,  521,  326,  328,  327,  297,
 /*  2000 */   521,  521,  521,  521,  521,  229,  293,  298,  312,  226,
 /*  2010 */   521,  308,  521,  521,  289,  185,  521,  521,  521,  521,
 /*  2020 */   521,  521,  319,  162,  526,  521,  521,  307,  521,  279,
 /*  2030 */   521,  521,  521,  489,  498,  507,  510,  501,  504,  513,
 /*  2040 */   519,  516,  521,  522,  186,  188,  190,  521,  199,  284,
 /*  2050 */   521,  521,  210,  214,  219,  393,  521,  521,  400,  326,
 /*  2060 */   328,  327,  297,  521,  301,  521,  521,  521,  464,  479,
 /*  2070 */   483,  486,  300,  302,  303,  304,  305,  306,  527,  232,
 /*  2080 */   521,  241,  244,  235,  238,  414,  521,  521,  521,  418,
 /*  2090 */   521,  521,  285,  521,  521,  326,  328,  327,  297,  521,
 /*  2100 */   521,  521,  521,  521,  229,  293,  298,  312,  226,  521,
 /*  2110 */   308,  521,  521,  289,  185,  521,  521,  521,  521,  521,
 /*  2120 */   521,  319,  160,  526,  521,  521,  307,  521,  279,  521,
 /*  2130 */   521,  521,  489,  498,  507,  510,  501,  504,  513,  519,
 /*  2140 */   516,  521,  522,  186,  188,  190,  521,  199,  272,  521,
 /*  2150 */   521,  210,  214,  219,  393,  521,  521,  400,  326,  328,
 /*  2160 */   327,  297,  521,  301,  521,  521,  521,  464,  479,  483,
 /*  2170 */   486,  300,  302,  303,  304,  305,  306,  527,  232,  521,
 /*  2180 */   241,  244,  235,  238,  414,  521,  521,  521,  413,  521,
 /*  2190 */   521,  285,  521,  521,  326,  328,  327,  297,  521,  521,
 /*  2200 */   521,  521,  521,  229,  293,  298,  312,  226,  521,  308,
 /*  2210 */   521,  521,  289,  185,  521,  521,  521,  521,  521,  521,
 /*  2220 */   319,  551,  526,  521,  521,  307,  521,  279,  521,  521,
 /*  2230 */   521,  489,  498,  507,  510,  501,  504,  513,  519,  516,
 /*  2240 */   521,  522,  186,  188,  190,  521,  199,  278,  521,  521,
 /*  2250 */   210,  214,  219,  393,  521,  521,  400,  326,  328,  327,
 /*  2260 */   297,  521,  301,  521,  521,  521,  464,  479,  483,  486,
 /*  2270 */   300,  302,  303,  304,  305,  306,  527,  232,  521,  241,
 /*  2280 */   244,  235,  238,  564,  521,   34,   58,   37,   39,   40,
 /*  2290 */   285,   63,  521,   34,   58,   37,   39,   40,  521,  521,
 /*  2300 */   521,  521,  229,  293,  298,  312,  226,  521,  308,  521,
 /*  2310 */   521,  289,  185,  521,  521,  521,  521,  521,  521,  319,
 /*  2320 */   608,  526,  521,  521,  307,  521,  279,  521,  521,  521,
 /*  2330 */   489,  498,  507,  510,  501,  504,  513,  519,  516,  521,
 /*  2340 */   522,  186,  188,  190,  521,  199,  239,  521,  521,  210,
 /*  2350 */   214,  219,  393,  521,  521,  400,  326,  328,  327,  297,
 /*  2360 */   521,  301,  521,  521,  521,  464,  479,  483,  486,  300,
 /*  2370 */   302,  303,  304,  305,  306,  527,  232,  521,  241,  244,
 /*  2380 */   235,  238,  576,  521,   34,   58,   37,   39,   40,  285,
 /*  2390 */   592,  521,   34,   58,   37,   39,   40,  521,  521,  521,
 /*  2400 */   521,  229,  293,  298,  312,  226,  521,  308,  521,  521,
 /*  2410 */   289,  185,  521,  521,  521,  521,  521,  521,  319,  606,
 /*  2420 */   526,  521,  521,  307,  521,  279,  521,  521,  521,  489,
 /*  2430 */   498,  507,  510,  501,  504,  513,  519,  516,  521,  522,
 /*  2440 */   186,  188,  190,  521,  199,  484,  521,  521,  210,  214,
 /*  2450 */   219,  393,  521,  521,  400,  326,  328,  327,  297,  521,
 /*  2460 */   301,  521,  521,  521,  464,  479,  483,  486,  300,  302,
 /*  2470 */   303,  304,  305,  306,  527,  232,  521,  241,  244,  235,
 /*  2480 */   238,  258,  521,  521,  521,  521,  521,  521,  285,  521,
 /*  2490 */   521,  326,  328,  327,  297,  521,  521,  521,  521,  521,
 /*  2500 */   229,  293,  298,  312,  226,  521,  308,  521,  521,  289,
 /*  2510 */   185,  521,  521,  521,  521,  521,  521,  319,  602,  526,
 /*  2520 */   521,  521,  307,  521,  279,  521,  521,  521,  489,  498,
 /*  2530 */   507,  510,  501,  504,  513,  519,  516,  521,  522,  186,
 /*  2540 */   188,  190,  521,  199,  390,  521,  521,  210,  214,  219,
 /*  2550 */   393,  521,  521,  400,  326,  328,  327,  297,  521,  301,
 /*  2560 */   521,  521,  521,  464,  479,  483,  486,  300,  302,  303,
 /*  2570 */   304,  305,  306,  527,  232,  521,  241,  244,  235,  238,
 /*  2580 */   391,  521,  521,  521,  521,  521,  521,  285,  521,  521,
 /*  2590 */   326,  328,  327,  297,  521,  521,  521,  521,  521,  229,
 /*  2600 */   293,  298,  312,  226,  521,  308,  521,  521,  289,  185,
 /*  2610 */   521,  521,  521,  521,  521,  521,  319,  600,  526,  521,
 /*  2620 */   521,  307,  521,  279,  521,  521,  521,  489,  498,  507,
 /*  2630 */   510,  501,  504,  513,  519,  516,  521,  522,  186,  188,
 /*  2640 */   190,  521,  199,  236,  521,  521,  210,  214,  219,  393,
 /*  2650 */   521,  521,  400,  326,  328,  327,  297,  521,  301,  521,
 /*  2660 */   521,  521,  464,  479,  483,  486,  300,  302,  303,  304,
 /*  2670 */   305,  306,  527,  232,  521,  241,  244,  235,  238,  389,
 /*  2680 */   521,  521,  521,  521,  521,  521,  285,  521,  521,  326,
 /*  2690 */   328,  327,  297,  521,  521,  521,  521,  521,  229,  293,
 /*  2700 */   298,  312,  226,  521,  308,  521,  521,  289,  185,  521,
 /*  2710 */   521,  521,  521,  521,  521,  319,  533,  526,  521,  521,
 /*  2720 */   307,  521,  279,  521,  521,  521,  489,  498,  507,  510,
 /*  2730 */   501,  504,  513,  519,  516,  521,  522,  186,  188,  190,
 /*  2740 */   521,  199,  233,  521,  521,  210,  214,  219,  393,  521,
 /*  2750 */   521,  400,  326,  328,  327,  297,  521,  301,  521,  521,
 /*  2760 */   521,  464,  479,  483,  486,  300,  302,  303,  304,  305,
 /*  2770 */   306,  527,  232,  521,  241,  244,  235,  238,  542,  521,
 /*  2780 */   521,  521,  521,  521,  521,  285,  521,  521,  326,  328,
 /*  2790 */   327,  297,  521,  521,  521,  521,  521,  229,  293,  298,
 /*  2800 */   312,  226,  521,  308,  521,  521,  289,  185,  521,  521,
 /*  2810 */   521,  521,  521,  521,  319,  596,  526,  521,  521,  307,
 /*  2820 */   521,  279,  521,  521,  521,  489,  498,  507,  510,  501,
 /*  2830 */   504,  513,  519,  516,  521,  522,  186,  188,  190,  521,
 /*  2840 */   199,  268,  521,  521,  210,  214,  219,  393,  521,  521,
 /*  2850 */   400,  326,  328,  327,  297,  521,  301,  521,  521,  521,
 /*  2860 */   464,  479,  483,  486,  300,  302,  303,  304,  305,  306,
 /*  2870 */   527,  232,  521,  241,  244,  235,  238,  387,  521,  521,
 /*  2880 */   521,  521,  521,  521,  285,  521,  521,  326,  328,  327,
 /*  2890 */   297,  521,  521,  521,  521,  521,  229,  293,  298,  312,
 /*  2900 */   226,  521,  308,  521,  521,  289,  185,  521,  521,  521,
 /*  2910 */   521,  521,  521,  319,  594,  526,  521,  521,  307,  521,
 /*  2920 */   279,  521,  521,  521,  489,  498,  507,  510,  501,  504,
 /*  2930 */   513,  519,  516,  521,  522,  186,  188,  190,  521,  199,
 /*  2940 */   242,  521,  521,  210,  214,  219,  393,  521,  521,  400,
 /*  2950 */   326,  328,  327,  297,  521,  301,  521,  521,  521,  464,
 /*  2960 */   479,  483,  486,  300,  302,  303,  304,  305,  306,  527,
 /*  2970 */   232,  521,  241,  244,  235,  238,  537,  521,  521,  521,
 /*  2980 */   521,  521,  521,  285,  521,  521,  326,  328,  327,  297,
 /*  2990 */   521,  521,  521,  521,  521,  229,  293,  298,  312,  226,
 /*  3000 */   521,  308,  521,  521,  289,  185,  521,  521,  521,  521,
 /*  3010 */   521,  521,  319,  540,  526,  521,  521,  307,  521,  279,
 /*  3020 */   521,  521,  521,  489,  498,  507,  510,  501,  504,  513,
 /*  3030 */   519,  516,  521,  522,  186,  188,  190,  521,  199,  264,
 /*  3040 */   521,  521,  210,  214,  219,  393,  521,  521,  400,  326,
 /*  3050 */   328,  327,  297,  521,  301,  521,  521,  521,  464,  479,
 /*  3060 */   483,  486,  300,  302,  303,  304,  305,  306,  527,  232,
 /*  3070 */   521,  241,  244,  235,  238,  266,  521,  521,  521,  521,
 /*  3080 */   521,  521,  285,  521,  521,  326,  328,  327,  297,  521,
 /*  3090 */   521,  521,  521,  521,  229,  293,  298,  312,  226,  521,
 /*  3100 */   308,  521,  521,  289,  185,  521,  521,  521,  521,  521,
 /*  3110 */   521,  319,  590,  526,  521,  521,  307,  521,  279,  521,
 /*  3120 */   521,  521,  489,  498,  507,  510,  501,  504,  513,  519,
 /*  3130 */   516,  521,  522,  186,  188,  190,  521,  199,  245,  521,
 /*  3140 */   521,  210,  214,  219,  393,  521,  521,  400,  326,  328,
 /*  3150 */   327,  297,  521,  301,  521,  521,  521,  464,  479,  483,
 /*  3160 */   486,  300,  302,  303,  304,  305,  306,  527,  232,  521,
 /*  3170 */   241,  244,  235,  238,  276,  521,  521,  521,  521,  521,
 /*  3180 */   521,  285,  521,  521,  326,  328,  327,  297,  521,  521,
 /*  3190 */   521,  521,  521,  229,  293,  298,  312,  226,  521,  308,
 /*  3200 */   521,  521,  289,  185,  521,  521,  521,  521,  521,  521,
 /*  3210 */   319,  282,  526,  521,  521,  307,  521,  279,  521,  521,
 /*  3220 */   521,  489,  498,  507,  510,  501,  504,  513,  519,  516,
 /*  3230 */   521,  522,  186,  188,  190,  521,  199,  521,  280,  521,
 /*  3240 */   210,  214,  219,  393,  521,  521,  400,  521,  326,  328,
 /*  3250 */   327,  297,  301,  521,  521,  521,  464,  479,  483,  486,
 /*  3260 */   300,  302,  303,  304,  305,  306,  527,  232,  521,  241,
 /*  3270 */   244,  235,  238,  521,    9,  521,  521,   12,  521,  138,
 /*  3280 */   285,  659,  669,  521,   78,  526,  521,  521,  521,  521,
 /*  3290 */   521,  521,  229,  293,  298,  312,  226,   81,  308,  521,
 /*  3300 */   521,  289,  521,  521,  521,  521,  521,  521,  521,  319,
 /*  3310 */   521,  521,  521,  521,  307,  521,  279,  521,  521,  521,
 /*  3320 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  3330 */    57,  521,  521,  521,   73,  521,  183,  164,  521,  527,
 /*  3340 */   521,  521,   71,   72,   74,   75,   77,   76,  521,  521,
 /*  3350 */   521,  301,  521,  521,  521,  521,  521,  521,  212,  300,
 /*  3360 */   302,  303,  304,  305,  306,  521,  521,  528,  165,  166,
 /*  3370 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  3380 */   177,  178,  179,  180,  181,  182,  184,  183,  164,  521,
 /*  3390 */   525,  149,  521,  521,  151,  645,  651,  521,  650,  635,
 /*  3400 */   471,  473,  475,  477,  137,  611,  521,  521,  521,  217,
 /*  3410 */   122,   98,   99,  101,  100,  102,  521,  521,  528,  165,
 /*  3420 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  3430 */   176,  177,  178,  179,  180,  181,  182,  184,  183,  164,
 /*  3440 */   521,  525,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  3450 */   521,  471,  473,  475,  477,  521,  521,  104,  105,  521,
 /*  3460 */   539,  521,  521,  126,  521,  521,  521,  103,  521,  528,
 /*  3470 */   165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  3480 */   175,  176,  177,  178,  179,  180,  181,  182,  184,  183,
 /*  3490 */   164,  521,  525,  521,  521,   20,  521,   22,  521,  136,
 /*  3500 */   521,   25,  471,  473,  475,  477,  137,  521,  124,  521,
 /*  3510 */   521,  567,  122,   98,   99,  101,  100,  102,  521,  521,
 /*  3520 */   528,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  3530 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  184,
 /*  3540 */   183,  164,  521,  525,  521,  521,  521,  521,  521,  521,
 /*  3550 */   521,  521,  521,  471,  473,  475,  477,  521,  521,  104,
 /*  3560 */   105,  521,  607,  521,  521,  521,  521,  583,  521,  103,
 /*  3570 */   521,  528,  165,  166,  167,  168,  169,  170,  171,  172,
 /*  3580 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  3590 */   184,  183,  164,  521,  525,  521,  521,  521,  309,  521,
 /*  3600 */   521,  521,  521,  521,  471,  473,  475,  477,  326,  328,
 /*  3610 */   327,  297,  521,  560,  357,  521,  521,  365,  361,  364,
 /*  3620 */   521,  521,  528,  165,  166,  167,  168,  169,  170,  171,
 /*  3630 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
 /*  3640 */   182,  184,  183,  164,  521,  525,  521,  521,  521,  521,
 /*  3650 */   309,  521,  521,  521,  521,  471,  473,  475,  477,  521,
 /*  3660 */   326,  328,  327,  297,  595,  521,   98,   99,  101,  100,
 /*  3670 */   102,  363,  521,  528,  165,  166,  167,  168,  169,  170,
 /*  3680 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  3690 */   181,  182,  184,  183,  164,  521,  525,  521,  521,  521,
 /*  3700 */   521,  521,  521,  521,  521,  521,  471,  473,  475,  477,
 /*  3710 */   521,  521,  104,  105,  521,  573,  521,  521,  521,  521,
 /*  3720 */   108,  521,  103,  521,  528,  165,  166,  167,  168,  169,
 /*  3730 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  3740 */   180,  181,  182,  184,  183,  164,  521,  525,  521,  521,
 /*  3750 */   521,  262,  521,  521,  521,  521,  521,  471,  473,  475,
 /*  3760 */   477,  326,  328,  327,  297,  521,  221,  521,   98,   99,
 /*  3770 */   101,  100,  102,  521,  521,  528,  165,  166,  167,  168,
 /*  3780 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  3790 */   179,  180,  181,  182,  184,  183,  164,  521,  525,  521,
 /*  3800 */   521,  521,  521,  521,  521,  521,  521,  521,  471,  473,
 /*  3810 */   475,  477,  521,  521,  104,  105,  521,  398,  521,  521,
 /*  3820 */   521,  521,  155,  521,  103,  521,  528,  165,  166,  167,
 /*  3830 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  3840 */   178,  179,  180,  181,  182,  184,  183,  164,  521,  525,
 /*  3850 */   521,  521,  521,  260,  521,  521,  521,  521,  521,  471,
 /*  3860 */   473,  475,  477,  326,  328,  327,  297,  521,  601,  521,
 /*  3870 */    98,   99,  101,  100,  102,  521,  521,  528,  165,  166,
 /*  3880 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  3890 */   177,  178,  179,  180,  181,  182,  184,  183,  164,  521,
 /*  3900 */   525,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  3910 */   471,  473,  475,  477,  521,  521,  104,  105,  521,  197,
 /*  3920 */   521,  521,  521,  521,   26,  521,  103,  521,  528,  165,
 /*  3930 */   166,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  3940 */   176,  177,  178,  179,  180,  181,  182,  184,  183,  164,
 /*  3950 */   521,  525,  521,  521,  521,  296,  521,  521,  521,  521,
 /*  3960 */   521,  471,  473,  475,  477,  326,  328,  327,  297,  521,
 /*  3970 */   589,  521,   98,   99,  101,  100,  102,  521,  521,  528,
 /*  3980 */   165,  166,  167,  168,  169,  170,  171,  172,  173,  174,
 /*  3990 */   175,  176,  177,  178,  179,  180,  181,  182,  184,  183,
 /*  4000 */   164,  521,  525,  521,  521,  521,  521,  521,  521,  521,
 /*  4010 */   521,  521,  471,  473,  475,  477,  521,  521,  104,  105,
 /*  4020 */   521,  579,  521,  521,  521,  521,  521,  521,  103,  521,
 /*  4030 */   528,  165,  166,  167,  168,  169,  170,  171,  172,  173,
 /*  4040 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  184,
 /*  4050 */   183,  164,  521,  525,  521,  521,  521,  309,  521,  521,
 /*  4060 */   521,  521,  521,  471,  473,  475,  477,  326,  328,  327,
 /*  4070 */   297,  521,  193,  521,  521,  521,  360,  361,  364,  521,
 /*  4080 */   521,  528,  165,  166,  167,  168,  169,  170,  171,  172,
 /*  4090 */   173,  174,  175,  176,  177,  178,  179,  180,  181,  182,
 /*  4100 */   184,  183,  164,  521,  525,  521,  521,  521,  396,  521,
 /*  4110 */   521,  521,  521,  521,  471,  473,  475,  477,  326,  328,
 /*  4120 */   327,  297,  521,  161,  521,  521,  521,  521,  521,  521,
 /*  4130 */   521,  521,  528,  165,  166,  167,  168,  169,  170,  171,
 /*  4140 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
 /*  4150 */   182,  184,  183,  164,  521,  525,  521,  521,  521,  270,
 /*  4160 */   521,  521,  521,  521,  521,  471,  473,  475,  477,  326,
 /*  4170 */   328,  327,  297,  521,  544,  521,  521,  521,  521,  521,
 /*  4180 */   521,  521,  521,  528,  165,  166,  167,  168,  169,  170,
 /*  4190 */   171,  172,  173,  174,  175,  176,  177,  178,  179,  180,
 /*  4200 */   181,  182,  184,  183,  164,  521,  525,  521,  521,  521,
 /*  4210 */   227,  521,  521,  521,  521,  521,  471,  473,  475,  477,
 /*  4220 */   326,  328,  327,  297,  521,  532,  521,  521,  521,  521,
 /*  4230 */   521,  521,  521,  521,  528,  165,  166,  167,  168,  169,
 /*  4240 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  4250 */   180,  181,  182,  184,  183,  164,  521,  525,  521,  521,
 /*  4260 */   521,  299,  521,  521,  521,  521,  521,  471,  473,  475,
 /*  4270 */   477,  326,  328,  327,  297,  521,  554,  521,  521,  521,
 /*  4280 */   521,  521,  521,  521,  521,  528,  165,  166,  167,  168,
 /*  4290 */   169,  170,  171,  172,  173,  174,  175,  176,  177,  178,
 /*  4300 */   179,  180,  181,  182,  184,  183,  164,  521,  525,  521,
 /*  4310 */   521,  521,  247,  521,  521,  521,  521,  521,  471,  473,
 /*  4320 */   475,  477,  326,  328,  327,  297,  521,  209,  521,   98,
 /*  4330 */    99,  101,  100,  102,  521,  521,  528,  165,  166,  167,
 /*  4340 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  4350 */   178,  179,  180,  181,  182,  184,  183,  164,  521,  525,
 /*  4360 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  471,
 /*  4370 */   473,  475,  477,  521,  521,  104,  105,  521,  705,  614,
 /*  4380 */   521,  131,  521,  521,  521,  103,  521,  163,  165,  166,
 /*  4390 */   167,  168,  169,  170,  171,  172,  173,  174,  175,  176,
 /*  4400 */   177,  178,  179,  180,  181,  182,  184,  521,  521,  521,
 /*  4410 */   525,  232,  521,  241,  244,  235,  238,  530,  521,  521,
 /*  4420 */   471,  473,  475,  477,  285,  521,  521,  326,  328,  327,
 /*  4430 */   297,  521,  521,  521,  521,  521,  229,  293,  298,  312,
 /*  4440 */   226,  521,  308,  521,  232,  289,  241,  244,  235,  238,
 /*  4450 */   521,  521,  521,  319,  255,  521,  521,  285,  307,  521,
 /*  4460 */   279,  521,  521,  521,  326,  328,  327,  297,  521,  229,
 /*  4470 */   293,  298,  312,  226,  379,  308,  521,  521,  289,  521,
 /*  4480 */   521,  521,  521,  521,  521,  521,  319,  288,  529,  521,
 /*  4490 */   521,  307,  521,  279,  521,  301,  521,  326,  328,  327,
 /*  4500 */   297,  521,  521,  300,  302,  303,  304,  305,  306,  273,
 /*  4510 */   269,  271,  521,  521,  228,  234,  240,  243,  246,  248,
 /*  4520 */   237,  231,  257,  259,  267,  261,  263,  265,  301,  521,
 /*  4530 */   521,  521,  521,  521,  254,  250,  300,  302,  303,  304,
 /*  4540 */   305,  306,  521,  521,  232,  521,  241,  244,  235,  238,
 /*  4550 */   249,  521,  521,  521,  274,  521,  521,  285,  521,  521,
 /*  4560 */   326,  328,  327,  297,  326,  328,  327,  297,  521,  229,
 /*  4570 */   293,  298,  312,  226,  348,  308,  521,  232,  289,  241,
 /*  4580 */   244,  235,  238,  521,  487,  521,  319,  521,  521,  521,
 /*  4590 */   285,  307,  521,  279,  326,  328,  327,  297,  521,  521,
 /*  4600 */   521,  521,  229,  293,  298,  312,  226,  368,  308,  521,
 /*  4610 */   521,  289,  521,  521,  521,  521,  521,  521,  521,  319,
 /*  4620 */   521,  521,  521,  521,  307,  521,  279,  521,  301,  521,
 /*  4630 */   521,  521,  521,  521,  521,  521,  300,  302,  303,  304,
 /*  4640 */   305,  306,  521,  269,  271,  521,  521,  228,  234,  240,
 /*  4650 */   243,  246,  248,  237,  231,  257,  259,  267,  261,  263,
 /*  4660 */   265,  301,  521,  521,  521,  521,  521,  254,  250,  300,
 /*  4670 */   302,  303,  304,  305,  306,  521,  521,  232,  521,  241,
 /*  4680 */   244,  235,  238,  521,   98,   99,  101,  100,  102,  521,
 /*  4690 */   285,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  4700 */   521,  521,  229,  293,  298,  312,  226,  315,  308,  521,
 /*  4710 */   232,  289,  241,  244,  235,  238,  521,  521,  521,  319,
 /*  4720 */   521,  521,  521,  285,  307,  521,  279,  521,  521,  521,
 /*  4730 */   104,  105,  521,  715,  106,  229,  293,  298,  312,  226,
 /*  4740 */   103,  308,  521,  521,  289,  521,  521,  521,  521,  521,
 /*  4750 */   521,  521,  319,  521,  521,  521,  521,  307,  521,  279,
 /*  4760 */   521,  301,  455,  521,  521,  521,  521,  521,  521,  300,
 /*  4770 */   302,  303,  304,  305,  306,  521,  228,  234,  240,  243,
 /*  4780 */   246,  248,  237,  231,  257,  259,  267,  261,  263,  265,
 /*  4790 */   521,  521,  521,  521,  301,  521,  254,  250,  521,  521,
 /*  4800 */   521,  521,  300,  302,  303,  304,  305,  306,  521,  521,
 /*  4810 */   232,  521,  241,  244,  235,  238,  521,  521,  521,  521,
 /*  4820 */   521,  521,  521,  285,  521,  521,  521,  521,  521,  521,
 /*  4830 */   521,  521,  521,  521,  521,  229,  293,  298,  312,  226,
 /*  4840 */   342,  308,  521,  232,  289,  241,  244,  235,  238,  521,
 /*  4850 */   521,  521,  319,  521,  521,  521,  285,  307,  521,  279,
 /*  4860 */   521,  521,  521,  521,  521,  521,  521,  521,  229,  293,
 /*  4870 */   298,  312,  226,  372,  308,  521,  521,  289,  678,  521,
 /*  4880 */   521,  521,  521,  521,  521,  319,  521,  521,  521,  521,
 /*  4890 */   307,  521,  279,  521,  301,  521,  521,  521,  521,  521,
 /*  4900 */   521,  521,  300,  302,  303,  304,  305,  306,   98,   99,
 /*  4910 */   101,  100,  102,  521,  521,  521,  521,  521,    9,  521,
 /*  4920 */   521,   12,  521,  138,  521,  659,  669,  301,  521,  526,
 /*  4930 */   521,  521,  521,  521,  521,  300,  302,  303,  304,  305,
 /*  4940 */   306,  521,  521,  232,  521,  241,  244,  235,  238,  521,
 /*  4950 */   521,  521,  521,  521,  104,  105,  285,   19,  125,  521,
 /*  4960 */   131,  521,  521,  521,  103,  521,  521,  521,  229,  293,
 /*  4970 */   298,  312,  226,  335,  308,  521,  232,  289,  241,  244,
 /*  4980 */   235,  238,  521,  527,  521,  319,  521,  521,  521,  285,
 /*  4990 */   307,  521,  279,  521,  521,  521,  521,  521,  521,  521,
 /*  5000 */   521,  229,  293,  298,  312,  226,   41,  308,  521,  521,
 /*  5010 */   289,  482,  521,  521,  521,  521,  521,  521,  319,  521,
 /*  5020 */   521,  521,  521,  307,  521,  279,  521,  301,  521,  521,
 /*  5030 */   521,  521,  521,  521,  521,  300,  302,  303,  304,  305,
 /*  5040 */   306,  521,  521,  521,   33,  521,   47,   48,   49,   50,
 /*  5050 */    51,   52,   53,   54,   55,   56,   57,  521,  521,  521,
 /*  5060 */   301,  521,  521,  521,  521,  521,  521,  521,  300,  302,
 /*  5070 */   303,  304,  305,  306,  521,  521,  232,  521,  241,  244,
 /*  5080 */   235,  238,  521,  521,  521,  521,  521,  521,  521,  285,
 /*  5090 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  5100 */   521,  229,  293,  298,  312,  226,  353,  308,  521,  232,
 /*  5110 */   289,  241,  244,  235,  238,  521,  521,  521,  319,  521,
 /*  5120 */   521,  521,  285,  307,  521,  279,  521,  521,  454,  415,
 /*  5130 */   521,  521,  521,  521,  229,  293,  298,  312,  226,  521,
 /*  5140 */   308,  521,  406,  289,  521,  521,  521,  521,  521,  521,
 /*  5150 */   521,  319,  521,  521,  521,  521,  307,  521,  279,  521,
 /*  5160 */   301,  521,   98,   99,  101,  100,  102,  521,  300,  302,
 /*  5170 */   303,  304,  305,  306,  521,  521,  407,  408,  409,  410,
 /*  5180 */   411,  521,  443,  461,  462,  521,  541,  521,  521,  521,
 /*  5190 */   521,  521,  521,  301,  232,  521,  241,  244,  235,  238,
 /*  5200 */   521,  300,  302,  303,  304,  305,  306,  285,  104,  105,
 /*  5210 */   521,  709,  612,  521,  131,  521,  521,  521,  103,  229,
 /*  5220 */   293,  298,  312,  226,  521,  308,  521,  232,  289,  241,
 /*  5230 */   244,  235,  238,  521,  521,  521,  319,  521,  521,  521,
 /*  5240 */   285,  307,  521,  279,  521,  521,  420,  521,  521,  521,
 /*  5250 */   521,  521,  229,  293,  298,  312,  226,   41,  308,  521,
 /*  5260 */   521,  310,  521,  521,  521,  521,  521,  521,  521,  319,
 /*  5270 */   521,  521,  521,  521,  307,  521,  279,  521,  301,  356,
 /*  5280 */   521,  521,  521,  521,  521,  521,  300,  302,  303,  304,
 /*  5290 */   305,  306,   98,   99,  101,  100,  102,   47,   48,   49,
 /*  5300 */    50,   51,   52,   53,   54,   55,   56,   57,  521,  521,
 /*  5310 */   521,  355,  521,  521,  521,  521,  521,  521,  521,  354,
 /*  5320 */   302,  303,  304,  305,  306,  521,  521,  232,  521,  241,
 /*  5330 */   244,  235,  238,  521,  521,  521,  521,  521,  104,  105,
 /*  5340 */   285,  714,  125,  521,  131,  521,  521,  521,  103,  521,
 /*  5350 */   521,  521,  229,  293,  298,  312,  226,  521,  308,  521,
 /*  5360 */   232,  289,  241,  244,  235,  238,  521,  521,  521,  319,
 /*  5370 */   521,  521,  521,  285,  307,  521,  279,  521,  521,  448,
 /*  5380 */   521,  521,  521,  521,  521,  229,  293,  298,  312,  226,
 /*  5390 */   325,  308,  521,  521,  289,  521,  521,  521,  521,  521,
 /*  5400 */   521,  521,  319,  521,  521,  521,  521,  307,  521,  279,
 /*  5410 */   521,  301,  521,  521,  521,  521,  521,  521,  521,  300,
 /*  5420 */   302,  303,  304,  305,  306,   98,   99,  101,  100,  102,
 /*  5430 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  5440 */   521,  521,  521,  521,  301,  521,  521,  521,  521,  521,
 /*  5450 */   521,  521,  300,  302,  303,  304,  305,  306,  521,  521,
 /*  5460 */   232,  521,  241,  244,  235,  238,  521,  521,  521,  521,
 /*  5470 */   521,  104,  105,  285,  148,  614,  521,  131,  521,  521,
 /*  5480 */   521,  103,  521,  521,  521,  229,  293,  298,  312,  226,
 /*  5490 */   385,  308,  521,  232,  289,  241,  244,  235,  238,  521,
 /*  5500 */   521,  521,  319,  521,  521,  521,  285,  307,  521,  279,
 /*  5510 */   521,  521,  521,  521,  521,  521,  521,  521,  229,  293,
 /*  5520 */   298,  312,  226,  521,  308,  521,  521,  289,  521,  521,
 /*  5530 */   521,  521,  521,  521,  521,  319,  521,  521,  521,  521,
 /*  5540 */   307,  521,  279,  521,  301,  521,  521,  521,  521,  521,
 /*  5550 */   521,  521,  300,  302,  303,  304,  305,  306,   98,   99,
 /*  5560 */   101,  100,  102,  521,  521,  521,  521,  521,  521,  521,
 /*  5570 */   521,  521,  521,  521,  521,  521,  521,  301,  521,  521,
 /*  5580 */   521,  521,  521,  521,  521,  300,  302,  303,  304,  305,
 /*  5590 */   306,  521,  521,  232,  521,  241,  244,  235,  238,  521,
 /*  5600 */   521,  521,  521,  521,  104,  105,  285,  706,  614,  521,
 /*  5610 */   131,  521,  521,  521,  103,  521,  521,  521,  229,  293,
 /*  5620 */   298,  312,  226,  521,  308,  521,  521,  310,  521,  521,
 /*  5630 */   521,  521,  521,  521,  521,  319,  521,  521,  521,  521,
 /*  5640 */   307,  521,  279,  521,  275,  521,  521,  521,  521,  273,
 /*  5650 */   269,  271,  521,  521,  228,  234,  240,  243,  246,  248,
 /*  5660 */   237,  231,  257,  259,  267,  261,  263,  265,  521,  521,
 /*  5670 */   521,  521,  521,  521,  254,  250,  521,  355,  481,  521,
 /*  5680 */   521,  521,  521,  521,  521,  354,  302,  303,  304,  305,
 /*  5690 */   306,  521,  521,  521,  275,  521,  521,  521,  521,  273,
 /*  5700 */   269,  271,  521,  521,  228,  234,  240,  243,  246,  248,
 /*  5710 */   237,  231,  257,  259,  267,  261,  263,  265,  521,  521,
 /*  5720 */   521,  521,  521,  521,  254,  250,  275,  521,  488,  521,
 /*  5730 */   521,  273,  269,  271,  521,  521,  228,  234,  240,  243,
 /*  5740 */   246,  248,  237,  231,  257,  259,  267,  261,  263,  265,
 /*  5750 */   521,  521,  521,  521,  521,  521,  254,  250,  275,  521,
 /*  5760 */   521,  521,  521,  273,  269,  271,  521,  397,  228,  234,
 /*  5770 */   240,  243,  246,  248,  237,  231,  257,  259,  267,  261,
 /*  5780 */   263,  265,  521,  521,  521,  275,  521,  281,  254,  250,
 /*  5790 */   273,  269,  271,  521,  521,  228,  234,  240,  243,  246,
 /*  5800 */   248,  237,  231,  257,  259,  267,  261,  263,  265,  521,
 /*  5810 */   521,  521,  521,  521,  521,  254,  250,  275,  521,  521,
 /*  5820 */   521,  521,  273,  269,  271,  521,  543,  228,  234,  240,
 /*  5830 */   243,  246,  248,  237,  231,  257,  259,  267,  261,  263,
 /*  5840 */   265,  521,  521,  521,  521,  521,  521,  254,  250,  275,
 /*  5850 */   521,  521,  521,  521,  273,  269,  271,  521,  531,  228,
 /*  5860 */   234,  240,  243,  246,  248,  237,  231,  257,  259,  267,
 /*  5870 */   261,  263,  265,  521,  521,  521,  275,  521,  521,  254,
 /*  5880 */   250,  273,  269,  271,  521,  521,  228,  234,  240,  243,
 /*  5890 */   246,  248,  237,  231,  257,  259,  267,  261,  263,  265,
 /*  5900 */   882,  521,  521,  275,  521,  521,  254,  250,  273,  269,
 /*  5910 */   271,  521,  521,  228,  234,  240,  243,  246,  248,  237,
 /*  5920 */   231,  257,  259,  267,  261,  263,  265,  521,  521,  521,
 /*  5930 */   521,  521,  521,  254,  250,  521,  521,  485,  521,  521,
 /*  5940 */   521,  521,  275,  521,  521,  277,  521,  273,  269,  271,
 /*  5950 */   521,  521,  228,  234,  240,  243,  246,  248,  237,  231,
 /*  5960 */   257,  259,  267,  261,  263,  265,  521,  521,  521,  521,
 /*  5970 */   521,  521,  254,  250,  275,  521,  521,  521,  521,  273,
 /*  5980 */   269,  271,  521,  538,  228,  234,  240,  243,  246,  248,
 /*  5990 */   237,  231,  257,  259,  267,  261,  263,  265,  521,  521,
 /*  6000 */   521,  275,  521,  521,  254,  250,  273,  269,  271,   94,
 /*  6010 */   521,  228,  234,  240,  243,  246,  248,  237,  231,  257,
 /*  6020 */   259,  267,  261,  263,  265,  256,  111,   94,  521,  521,
 /*  6030 */    68,  254,  250,  521,  521,  521,  521,  521,  521,  521,
 /*  6040 */   521,  521,  521,   94,  586,  521,  521,  521,   68,   47,
 /*  6050 */    48,   49,   50,   51,   52,   53,   54,   55,   56,   57,
 /*  6060 */   158,  521,  521,  521,   68,  521,  521,   47,   48,   49,
 /*  6070 */    50,   51,   52,   53,   54,   55,   56,   57,   94,  521,
 /*  6080 */   521,  521,  521,   47,   48,   49,   50,   51,   52,   53,
 /*  6090 */    54,   55,   56,   57,   94,   29,  521,  521,  521,   68,
 /*  6100 */   521,   98,   99,  101,  100,  102,   98,   99,  101,  100,
 /*  6110 */   102,  521,  521,  521,  521,   68,  521,  521,   47,   48,
 /*  6120 */    49,   50,   51,   52,   53,   54,   55,   56,   57,   98,
 /*  6130 */    99,  101,  100,  102,   47,   48,   49,   50,   51,   52,
 /*  6140 */    53,   54,   55,   56,   57,  521,  521,  104,  105,  521,
 /*  6150 */   716,  106,  104,  105,  521,  711,  581,  103,  521,  521,
 /*  6160 */   521,  521,  103,  521,   98,   99,  101,  100,  102,  636,
 /*  6170 */   521,  521,  521,  521,  521,  104,  105,   98,   99,  101,
 /*  6180 */   100,  102,  521,  155,  521,  103,  521,  521,  521,  521,
 /*  6190 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  6200 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  6210 */   104,  105,  521,  713,  581,   98,   99,  101,  100,  102,
 /*  6220 */   103,  521,  521,  104,  105,  521,  708,  581,   98,   99,
 /*  6230 */   101,  100,  102,  103,  521,  521,  521,  521,  521,  521,
 /*  6240 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  6250 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  6260 */   521,  104,  105,  521,  707,  581,   98,   99,  101,  100,
 /*  6270 */   102,  103,  521,  521,  104,  105,  521,  710,  648,   98,
 /*  6280 */    99,  101,  100,  102,  103,  521,  521,  521,  521,  521,
 /*  6290 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  6300 */   521,  521,  521,  521,  521,  521,  521,  521,  521,  521,
 /*  6310 */   521,  521,  104,  105,  521,  712,  581,  521,  521,  616,
 /*  6320 */   521,  521,  103,  521,  521,  104,  105,  521,  521,  521,
 /*  6330 */   521,  521,  521,  583,  521,  103,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,    6,    9,   10,   11,   12,  110,  111,  112,  113,
 /*    10 */   114,  115,    6,   20,  107,  108,  109,  110,  111,  112,
 /*    20 */   113,  114,  115,   42,   50,   32,   33,   34,   35,   36,
 /*    30 */    49,   38,   37,   52,   41,   42,  123,  124,   39,  126,
 /*    40 */   127,   42,   49,   50,   51,  132,  133,   54,   42,   56,
 /*    50 */    76,  138,   78,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   68,   42,   70,   71,   72,   73,   38,   75,   49,
 /*    70 */   135,   52,   79,   80,   81,   82,  141,  142,   85,  144,
 /*    80 */   171,   52,  173,  174,   91,  182,  183,   49,   95,   96,
 /*    90 */    97,   98,   99,  100,  101,  102,  103,  104,  105,    7,
 /*   100 */    39,    9,   10,   11,   12,   86,   87,   88,   89,   90,
 /*   110 */    49,  171,   20,  173,  174,   86,   87,   88,   89,   90,
 /*   120 */    41,    0,   93,   94,   32,   33,   34,   35,   36,  171,
 /*   130 */    38,  173,  174,   41,   42,   29,   30,   31,    6,   56,
 /*   140 */    41,   49,   50,   51,   38,   39,   54,   42,   56,   50,
 /*   150 */    51,    6,   60,   61,   62,   63,   64,   65,   66,   67,
 /*   160 */    68,   44,   70,   71,   72,   73,   49,   75,  168,   37,
 /*   170 */    91,   79,   80,   81,   82,   92,   41,   85,  178,  179,
 /*   180 */   180,  181,   37,   91,   49,  185,  186,   95,   96,   97,
 /*   190 */    98,   99,  100,  101,  102,  103,  104,  105,    7,   56,
 /*   200 */     9,   10,   11,   12,   26,   27,   28,   29,   30,   31,
 /*   210 */   171,   20,  135,  174,  182,  183,   38,   39,  141,  142,
 /*   220 */    77,  144,   41,   32,   33,   34,   35,   36,  127,   38,
 /*   230 */    49,   42,   41,   42,  124,   92,  126,  127,   49,  138,
 /*   240 */    49,   50,   51,  133,   44,   54,   46,   56,  138,   49,
 /*   250 */     6,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*   260 */    41,   70,   71,   72,   73,   38,   75,   42,    6,  127,
 /*   270 */    79,   80,   81,   82,   49,  133,   85,   52,   41,   52,
 /*   280 */   138,   37,   91,    6,   56,  116,   95,   96,   97,   98,
 /*   290 */    99,  100,  101,  102,  103,  104,  105,    7,   41,    9,
 /*   300 */    10,   11,   12,  137,   42,  139,  140,  141,  142,  143,
 /*   310 */    20,  182,  183,   86,   87,   88,   89,   90,   49,   42,
 /*   320 */    92,   50,   32,   33,   34,   35,   36,   39,   38,  135,
 /*   330 */    42,   41,   42,    6,  124,  141,  142,  127,  144,   49,
 /*   340 */    50,   51,  132,  133,   54,  116,   56,   76,  138,   78,
 /*   350 */    60,   61,   62,   63,   64,   65,   66,   67,   68,  138,
 /*   360 */    70,   71,   72,   73,   37,   75,  168,  182,  183,   79,
 /*   370 */    80,   81,   82,    6,   50,   85,  178,  179,  180,  181,
 /*   380 */     6,   91,    6,  185,  186,   95,   96,   97,   98,   99,
 /*   390 */   100,  101,  102,  103,  104,  105,    7,   41,    9,   10,
 /*   400 */    11,   12,  168,  140,  141,  142,  143,  141,  142,   20,
 /*   410 */   144,   21,  178,  179,  180,  181,   42,   50,   42,  185,
 /*   420 */   186,   32,   33,   34,   35,   36,   46,   38,   38,   49,
 /*   430 */    41,   42,  182,  183,   56,  125,  127,  127,   49,   50,
 /*   440 */    51,   44,  132,   54,  134,   56,   49,  138,  138,   60,
 /*   450 */    61,   62,   63,   64,   65,   66,   67,   68,    6,   70,
 /*   460 */    71,   72,   73,  171,   75,  168,  174,   42,   79,   80,
 /*   470 */    81,   82,  166,  167,   85,  178,  179,  180,  181,    6,
 /*   480 */    91,   16,  185,  186,   95,   96,   97,   98,   99,  100,
 /*   490 */   101,  102,  103,  104,  105,    7,  135,    9,   10,   11,
 /*   500 */    12,  168,  141,  142,    6,  144,  168,   41,   20,  176,
 /*   510 */   177,  178,  179,  180,  181,   42,  178,  179,  180,  181,
 /*   520 */    32,   33,   34,   35,   36,   41,   38,   38,   39,   41,
 /*   530 */    42,   44,  127,   46,  127,   83,   49,   49,   50,   51,
 /*   540 */    42,  134,   54,  138,   56,  138,  130,  131,   60,   61,
 /*   550 */    62,   63,   64,   65,   66,   67,   68,   91,   70,   71,
 /*   560 */    72,   73,   59,   75,  168,  182,  183,   79,   80,   81,
 /*   570 */    82,  169,  170,   85,  178,  179,  180,  181,  118,   91,
 /*   580 */   120,  185,  186,   95,   96,   97,   98,   99,  100,  101,
 /*   590 */   102,  103,  104,  105,    7,    6,    9,   10,   11,   12,
 /*   600 */   122,    6,  124,    6,  126,  127,   41,   20,  182,  183,
 /*   610 */   132,  133,   39,   41,   49,   42,  138,   36,   42,   32,
 /*   620 */    33,   34,   35,   36,   21,   38,   54,   36,   41,   42,
 /*   630 */     6,   42,   41,  182,  183,    6,   49,   50,   51,   42,
 /*   640 */    52,   54,  171,   56,   49,  174,    6,   60,   61,   62,
 /*   650 */    63,   64,   65,   66,   67,   68,  128,   70,   71,   72,
 /*   660 */    73,   37,   75,  168,    6,  128,   79,   80,   81,   82,
 /*   670 */     6,   42,   85,  178,  179,  180,  181,   37,   91,   54,
 /*   680 */   185,  186,   95,   96,   97,   98,   99,  100,  101,  102,
 /*   690 */   103,  104,  105,    7,   39,    9,   10,   11,   12,  168,
 /*   700 */    42,   37,  182,  183,   49,   92,   20,  182,  183,  178,
 /*   710 */   179,  180,  181,   39,    6,   39,  185,  186,   32,   33,
 /*   720 */    34,   35,   36,   49,   38,   49,   41,   41,   42,  118,
 /*   730 */   118,  120,  120,   41,   49,   49,   50,   51,    6,    6,
 /*   740 */    54,   39,   56,   51,   42,   37,   60,   61,   62,   63,
 /*   750 */    64,   65,   66,   67,   68,   52,   70,   71,   72,   73,
 /*   760 */     6,   75,  168,    6,   41,   79,   80,   81,   82,   37,
 /*   770 */    37,   85,  178,  179,  180,  181,    6,   91,   52,  185,
 /*   780 */   186,   95,   96,   97,   98,   99,  100,  101,  102,  103,
 /*   790 */   104,  105,    7,    6,    9,   10,   11,   12,  168,   42,
 /*   800 */     6,  128,   41,   52,   41,   20,  128,   37,  178,  179,
 /*   810 */   180,  181,   21,   59,   41,  185,  186,   32,   33,   34,
 /*   820 */    35,   36,   56,   38,   37,   42,   41,   42,   42,   54,
 /*   830 */    52,   37,  128,   54,   49,   50,   51,   42,  128,   54,
 /*   840 */    52,   56,   42,   41,  117,   60,   61,   62,   63,   64,
 /*   850 */    65,   66,   67,   68,   41,   70,   71,   72,   73,   38,
 /*   860 */    75,  117,  117,   41,   79,   80,   81,   82,   50,   42,
 /*   870 */    85,   41,   56,   52,   42,   49,   91,   74,   49,  167,
 /*   880 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*   890 */   105,    7,   49,    9,   10,   11,   12,  137,   49,  139,
 /*   900 */   140,  141,  142,  143,   20,  128,   49,   86,   87,   88,
 /*   910 */    89,   90,   77,   49,   80,   42,   32,   33,   34,   35,
 /*   920 */    36,   56,   38,   37,   41,   41,   42,   21,   56,   41,
 /*   930 */     6,   56,   50,   49,   50,   51,   41,   56,   54,   41,
 /*   940 */    56,   56,   50,   41,   60,   61,   62,   63,   64,   65,
 /*   950 */    66,   67,   68,   50,   70,   71,   72,   73,   50,   75,
 /*   960 */   168,   56,   41,   79,   80,   81,   82,   77,   56,   85,
 /*   970 */   178,  179,  180,  181,   77,   91,   77,  185,  186,   95,
 /*   980 */    96,   97,   98,   99,  100,  101,  102,  103,  104,  105,
 /*   990 */     7,   50,    9,   10,   11,   12,  168,   41,   56,   59,
 /*  1000 */    50,  170,   50,   20,  171,  177,  178,  179,  180,  181,
 /*  1010 */   171,  171,   41,  171,   42,   32,   33,   34,   35,   36,
 /*  1020 */    42,   38,  171,   42,   41,   42,   42,   41,  183,   52,
 /*  1030 */    41,   83,   49,   50,   51,  128,   42,   54,   80,   56,
 /*  1040 */    77,   74,   49,   60,   61,   62,   63,   64,   65,   66,
 /*  1050 */    67,   68,   49,   70,   71,   72,   73,   49,   75,  168,
 /*  1060 */    41,   56,   79,   80,   81,   82,  175,   49,   85,  178,
 /*  1070 */   179,  180,  181,  129,   91,  128,   42,   42,   95,   96,
 /*  1080 */    97,   98,   99,  100,  101,  102,  103,  104,  105,    7,
 /*  1090 */   129,    9,   10,   11,   12,  168,   42,  131,   41,  172,
 /*  1100 */    42,  128,   20,  129,  129,  178,  179,  180,  181,   42,
 /*  1110 */    42,   45,  120,  117,   32,   33,   34,   35,   36,   41,
 /*  1120 */    38,  117,   41,   41,   42,   49,  117,  117,   45,   41,
 /*  1130 */   117,   49,   50,   51,   41,  190,   54,  117,   56,  190,
 /*  1140 */   190,  190,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  1150 */    68,  190,   70,   71,   72,   73,  165,   75,  190,  168,
 /*  1160 */   190,   79,   80,   81,   82,  190,  190,   85,  190,  178,
 /*  1170 */   179,  180,  181,   91,  190,  190,  190,   95,   96,   97,
 /*  1180 */    98,   99,  100,  101,  102,  103,  104,  105,    7,  190,
 /*  1190 */     9,   10,   11,   12,  168,  190,  190,  190,  172,  190,
 /*  1200 */   190,   20,  190,  190,  178,  179,  180,  181,  190,  190,
 /*  1210 */   190,  190,  190,   32,   33,   34,   35,   36,  190,   38,
 /*  1220 */   190,  190,   41,   42,  190,  190,  190,  190,  190,  190,
 /*  1230 */    49,   50,   51,  190,  190,   54,  190,   56,  190,  190,
 /*  1240 */   190,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*  1250 */   190,   70,   71,   72,   73,  190,   75,  168,  190,  190,
 /*  1260 */    79,   80,   81,   82,  190,  190,   85,  178,  179,  180,
 /*  1270 */   181,  190,   91,  190,  190,  186,   95,   96,   97,   98,
 /*  1280 */    99,  100,  101,  102,  103,  104,  105,    7,  190,    9,
 /*  1290 */    10,   11,   12,  137,  190,  139,  140,  141,  142,  143,
 /*  1300 */    20,  137,  190,  139,  140,  141,  142,  143,  190,  190,
 /*  1310 */   190,  190,   32,   33,   34,   35,   36,  190,   38,  190,
 /*  1320 */   190,   41,   42,  190,  190,  190,  190,  190,  190,   49,
 /*  1330 */    50,   51,  190,  190,   54,  190,   56,  190,  190,  190,
 /*  1340 */    60,   61,   62,   63,   64,   65,   66,   67,   68,  190,
 /*  1350 */    70,   71,   72,   73,  165,   75,  190,  168,  190,   79,
 /*  1360 */    80,   81,   82,  190,  190,   85,  190,  178,  179,  180,
 /*  1370 */   181,   91,  190,  190,  190,   95,   96,   97,   98,   99,
 /*  1380 */   100,  101,  102,  103,  104,  105,    7,  190,    9,   10,
 /*  1390 */    11,   12,  168,  190,  190,  190,  172,  190,  190,   20,
 /*  1400 */   190,  190,  178,  179,  180,  181,  190,  190,  190,  190,
 /*  1410 */   190,   32,   33,   34,   35,   36,  190,   38,  190,  190,
 /*  1420 */    41,   42,  190,  190,  190,  190,  190,  190,   49,   50,
 /*  1430 */    51,  190,  190,   54,  190,   56,  190,  190,  190,   60,
 /*  1440 */    61,   62,   63,   64,   65,   66,   67,   68,  190,   70,
 /*  1450 */    71,   72,   73,  165,   75,  190,  168,  190,   79,   80,
 /*  1460 */    81,   82,  190,  190,   85,  190,  178,  179,  180,  181,
 /*  1470 */    91,  190,  190,  190,   95,   96,   97,   98,   99,  100,
 /*  1480 */   101,  102,  103,  104,  105,    7,  190,    9,   10,   11,
 /*  1490 */    12,  168,  190,  190,  190,  172,  190,  190,   20,  190,
 /*  1500 */   190,  178,  179,  180,  181,  190,  190,  190,  190,  190,
 /*  1510 */    32,   33,   34,   35,   36,  190,   38,  190,  190,   41,
 /*  1520 */    42,  190,  190,  190,  190,  190,  190,   49,   50,   51,
 /*  1530 */   190,  190,   54,  190,   56,  190,  190,  190,   60,   61,
 /*  1540 */    62,   63,   64,   65,   66,   67,   68,  190,   70,   71,
 /*  1550 */    72,   73,  165,   75,  190,  168,  190,   79,   80,   81,
 /*  1560 */    82,  190,  190,   85,  190,  178,  179,  180,  181,   91,
 /*  1570 */   190,  190,  190,   95,   96,   97,   98,   99,  100,  101,
 /*  1580 */   102,  103,  104,  105,    7,  190,    9,   10,   11,   12,
 /*  1590 */   168,  190,  190,  190,  172,  190,  190,   20,  190,  190,
 /*  1600 */   178,  179,  180,  181,  190,  190,  190,  190,  190,   32,
 /*  1610 */    33,   34,   35,   36,  190,   38,  190,  190,   41,   42,
 /*  1620 */   190,  190,  190,  190,  190,  190,   49,   50,   51,  190,
 /*  1630 */   190,   54,  190,   56,  190,  190,  190,   60,   61,   62,
 /*  1640 */    63,   64,   65,   66,   67,   68,  190,   70,   71,   72,
 /*  1650 */    73,  165,   75,  190,  168,  190,   79,   80,   81,   82,
 /*  1660 */   190,  190,   85,  190,  178,  179,  180,  181,   91,  190,
 /*  1670 */   190,  190,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  1680 */   103,  104,  105,    7,  190,    9,   10,   11,   12,  168,
 /*  1690 */   190,  190,  190,  172,  190,  190,   20,  190,  190,  178,
 /*  1700 */   179,  180,  181,  190,  190,  190,  190,  190,   32,   33,
 /*  1710 */    34,   35,   36,  190,   38,  190,  190,   41,   42,  190,
 /*  1720 */   190,  190,  190,  190,  190,   49,   50,   51,  190,  190,
 /*  1730 */    54,  190,   56,  190,  190,  190,   60,   61,   62,   63,
 /*  1740 */    64,   65,   66,   67,   68,  190,   70,   71,   72,   73,
 /*  1750 */   190,   75,  168,  190,  190,   79,   80,   81,   82,  190,
 /*  1760 */   190,   85,  178,  179,  180,  181,  190,   91,  190,  190,
 /*  1770 */   190,   95,   96,   97,   98,   99,  100,  101,  102,  103,
 /*  1780 */   104,  105,    7,  190,    9,   10,   11,   12,  168,  190,
 /*  1790 */   190,  190,  172,  190,  190,   20,  190,  190,  178,  179,
 /*  1800 */   180,  181,  190,  190,  190,  190,  190,   32,   33,   34,
 /*  1810 */    35,   36,  190,   38,  190,  190,   41,   42,  190,  190,
 /*  1820 */   190,  190,  190,  190,   49,   50,   51,  190,  190,   54,
 /*  1830 */   190,   56,  190,  190,  190,   60,   61,   62,   63,   64,
 /*  1840 */    65,   66,   67,   68,  190,   70,   71,   72,   73,  190,
 /*  1850 */    75,  168,  190,  190,   79,   80,   81,   82,  190,  190,
 /*  1860 */    85,  178,  179,  180,  181,  190,   91,  190,  190,  190,
 /*  1870 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  1880 */   105,    7,  190,    9,   10,   11,   12,  168,  190,  190,
 /*  1890 */   190,  172,  190,  190,   20,  190,  190,  178,  179,  180,
 /*  1900 */   181,  190,  190,  190,  190,  190,   32,   33,   34,   35,
 /*  1910 */    36,  190,   38,  190,  190,   41,   42,  190,  190,  190,
 /*  1920 */   190,  190,  190,   49,   50,   51,  190,  190,   54,  190,
 /*  1930 */    56,  190,  190,  190,   60,   61,   62,   63,   64,   65,
 /*  1940 */    66,   67,   68,  190,   70,   71,   72,   73,  190,   75,
 /*  1950 */   168,  190,  190,   79,   80,   81,   82,  190,  190,   85,
 /*  1960 */   178,  179,  180,  181,  190,   91,  190,  190,  190,   95,
 /*  1970 */    96,   97,   98,   99,  100,  101,  102,  103,  104,  105,
 /*  1980 */     7,  190,    9,   10,   11,   12,  168,  190,  190,  190,
 /*  1990 */   172,  190,  190,   20,  190,  190,  178,  179,  180,  181,
 /*  2000 */   190,  190,  190,  190,  190,   32,   33,   34,   35,   36,
 /*  2010 */   190,   38,  190,  190,   41,   42,  190,  190,  190,  190,
 /*  2020 */   190,  190,   49,   50,   51,  190,  190,   54,  190,   56,
 /*  2030 */   190,  190,  190,   60,   61,   62,   63,   64,   65,   66,
 /*  2040 */    67,   68,  190,   70,   71,   72,   73,  190,   75,  168,
 /*  2050 */   190,  190,   79,   80,   81,   82,  190,  190,   85,  178,
 /*  2060 */   179,  180,  181,  190,   91,  190,  190,  190,   95,   96,
 /*  2070 */    97,   98,   99,  100,  101,  102,  103,  104,  105,    7,
 /*  2080 */   190,    9,   10,   11,   12,  168,  190,  190,  190,  172,
 /*  2090 */   190,  190,   20,  190,  190,  178,  179,  180,  181,  190,
 /*  2100 */   190,  190,  190,  190,   32,   33,   34,   35,   36,  190,
 /*  2110 */    38,  190,  190,   41,   42,  190,  190,  190,  190,  190,
 /*  2120 */   190,   49,   50,   51,  190,  190,   54,  190,   56,  190,
 /*  2130 */   190,  190,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  2140 */    68,  190,   70,   71,   72,   73,  190,   75,  168,  190,
 /*  2150 */   190,   79,   80,   81,   82,  190,  190,   85,  178,  179,
 /*  2160 */   180,  181,  190,   91,  190,  190,  190,   95,   96,   97,
 /*  2170 */    98,   99,  100,  101,  102,  103,  104,  105,    7,  190,
 /*  2180 */     9,   10,   11,   12,  168,  190,  190,  190,  172,  190,
 /*  2190 */   190,   20,  190,  190,  178,  179,  180,  181,  190,  190,
 /*  2200 */   190,  190,  190,   32,   33,   34,   35,   36,  190,   38,
 /*  2210 */   190,  190,   41,   42,  190,  190,  190,  190,  190,  190,
 /*  2220 */    49,   50,   51,  190,  190,   54,  190,   56,  190,  190,
 /*  2230 */   190,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*  2240 */   190,   70,   71,   72,   73,  190,   75,  168,  190,  190,
 /*  2250 */    79,   80,   81,   82,  190,  190,   85,  178,  179,  180,
 /*  2260 */   181,  190,   91,  190,  190,  190,   95,   96,   97,   98,
 /*  2270 */    99,  100,  101,  102,  103,  104,  105,    7,  190,    9,
 /*  2280 */    10,   11,   12,  137,  190,  139,  140,  141,  142,  143,
 /*  2290 */    20,  137,  190,  139,  140,  141,  142,  143,  190,  190,
 /*  2300 */   190,  190,   32,   33,   34,   35,   36,  190,   38,  190,
 /*  2310 */   190,   41,   42,  190,  190,  190,  190,  190,  190,   49,
 /*  2320 */    50,   51,  190,  190,   54,  190,   56,  190,  190,  190,
 /*  2330 */    60,   61,   62,   63,   64,   65,   66,   67,   68,  190,
 /*  2340 */    70,   71,   72,   73,  190,   75,  168,  190,  190,   79,
 /*  2350 */    80,   81,   82,  190,  190,   85,  178,  179,  180,  181,
 /*  2360 */   190,   91,  190,  190,  190,   95,   96,   97,   98,   99,
 /*  2370 */   100,  101,  102,  103,  104,  105,    7,  190,    9,   10,
 /*  2380 */    11,   12,  137,  190,  139,  140,  141,  142,  143,   20,
 /*  2390 */   137,  190,  139,  140,  141,  142,  143,  190,  190,  190,
 /*  2400 */   190,   32,   33,   34,   35,   36,  190,   38,  190,  190,
 /*  2410 */    41,   42,  190,  190,  190,  190,  190,  190,   49,   50,
 /*  2420 */    51,  190,  190,   54,  190,   56,  190,  190,  190,   60,
 /*  2430 */    61,   62,   63,   64,   65,   66,   67,   68,  190,   70,
 /*  2440 */    71,   72,   73,  190,   75,  168,  190,  190,   79,   80,
 /*  2450 */    81,   82,  190,  190,   85,  178,  179,  180,  181,  190,
 /*  2460 */    91,  190,  190,  190,   95,   96,   97,   98,   99,  100,
 /*  2470 */   101,  102,  103,  104,  105,    7,  190,    9,   10,   11,
 /*  2480 */    12,  168,  190,  190,  190,  190,  190,  190,   20,  190,
 /*  2490 */   190,  178,  179,  180,  181,  190,  190,  190,  190,  190,
 /*  2500 */    32,   33,   34,   35,   36,  190,   38,  190,  190,   41,
 /*  2510 */    42,  190,  190,  190,  190,  190,  190,   49,   50,   51,
 /*  2520 */   190,  190,   54,  190,   56,  190,  190,  190,   60,   61,
 /*  2530 */    62,   63,   64,   65,   66,   67,   68,  190,   70,   71,
 /*  2540 */    72,   73,  190,   75,  168,  190,  190,   79,   80,   81,
 /*  2550 */    82,  190,  190,   85,  178,  179,  180,  181,  190,   91,
 /*  2560 */   190,  190,  190,   95,   96,   97,   98,   99,  100,  101,
 /*  2570 */   102,  103,  104,  105,    7,  190,    9,   10,   11,   12,
 /*  2580 */   168,  190,  190,  190,  190,  190,  190,   20,  190,  190,
 /*  2590 */   178,  179,  180,  181,  190,  190,  190,  190,  190,   32,
 /*  2600 */    33,   34,   35,   36,  190,   38,  190,  190,   41,   42,
 /*  2610 */   190,  190,  190,  190,  190,  190,   49,   50,   51,  190,
 /*  2620 */   190,   54,  190,   56,  190,  190,  190,   60,   61,   62,
 /*  2630 */    63,   64,   65,   66,   67,   68,  190,   70,   71,   72,
 /*  2640 */    73,  190,   75,  168,  190,  190,   79,   80,   81,   82,
 /*  2650 */   190,  190,   85,  178,  179,  180,  181,  190,   91,  190,
 /*  2660 */   190,  190,   95,   96,   97,   98,   99,  100,  101,  102,
 /*  2670 */   103,  104,  105,    7,  190,    9,   10,   11,   12,  168,
 /*  2680 */   190,  190,  190,  190,  190,  190,   20,  190,  190,  178,
 /*  2690 */   179,  180,  181,  190,  190,  190,  190,  190,   32,   33,
 /*  2700 */    34,   35,   36,  190,   38,  190,  190,   41,   42,  190,
 /*  2710 */   190,  190,  190,  190,  190,   49,   50,   51,  190,  190,
 /*  2720 */    54,  190,   56,  190,  190,  190,   60,   61,   62,   63,
 /*  2730 */    64,   65,   66,   67,   68,  190,   70,   71,   72,   73,
 /*  2740 */   190,   75,  168,  190,  190,   79,   80,   81,   82,  190,
 /*  2750 */   190,   85,  178,  179,  180,  181,  190,   91,  190,  190,
 /*  2760 */   190,   95,   96,   97,   98,   99,  100,  101,  102,  103,
 /*  2770 */   104,  105,    7,  190,    9,   10,   11,   12,  168,  190,
 /*  2780 */   190,  190,  190,  190,  190,   20,  190,  190,  178,  179,
 /*  2790 */   180,  181,  190,  190,  190,  190,  190,   32,   33,   34,
 /*  2800 */    35,   36,  190,   38,  190,  190,   41,   42,  190,  190,
 /*  2810 */   190,  190,  190,  190,   49,   50,   51,  190,  190,   54,
 /*  2820 */   190,   56,  190,  190,  190,   60,   61,   62,   63,   64,
 /*  2830 */    65,   66,   67,   68,  190,   70,   71,   72,   73,  190,
 /*  2840 */    75,  168,  190,  190,   79,   80,   81,   82,  190,  190,
 /*  2850 */    85,  178,  179,  180,  181,  190,   91,  190,  190,  190,
 /*  2860 */    95,   96,   97,   98,   99,  100,  101,  102,  103,  104,
 /*  2870 */   105,    7,  190,    9,   10,   11,   12,  168,  190,  190,
 /*  2880 */   190,  190,  190,  190,   20,  190,  190,  178,  179,  180,
 /*  2890 */   181,  190,  190,  190,  190,  190,   32,   33,   34,   35,
 /*  2900 */    36,  190,   38,  190,  190,   41,   42,  190,  190,  190,
 /*  2910 */   190,  190,  190,   49,   50,   51,  190,  190,   54,  190,
 /*  2920 */    56,  190,  190,  190,   60,   61,   62,   63,   64,   65,
 /*  2930 */    66,   67,   68,  190,   70,   71,   72,   73,  190,   75,
 /*  2940 */   168,  190,  190,   79,   80,   81,   82,  190,  190,   85,
 /*  2950 */   178,  179,  180,  181,  190,   91,  190,  190,  190,   95,
 /*  2960 */    96,   97,   98,   99,  100,  101,  102,  103,  104,  105,
 /*  2970 */     7,  190,    9,   10,   11,   12,  168,  190,  190,  190,
 /*  2980 */   190,  190,  190,   20,  190,  190,  178,  179,  180,  181,
 /*  2990 */   190,  190,  190,  190,  190,   32,   33,   34,   35,   36,
 /*  3000 */   190,   38,  190,  190,   41,   42,  190,  190,  190,  190,
 /*  3010 */   190,  190,   49,   50,   51,  190,  190,   54,  190,   56,
 /*  3020 */   190,  190,  190,   60,   61,   62,   63,   64,   65,   66,
 /*  3030 */    67,   68,  190,   70,   71,   72,   73,  190,   75,  168,
 /*  3040 */   190,  190,   79,   80,   81,   82,  190,  190,   85,  178,
 /*  3050 */   179,  180,  181,  190,   91,  190,  190,  190,   95,   96,
 /*  3060 */    97,   98,   99,  100,  101,  102,  103,  104,  105,    7,
 /*  3070 */   190,    9,   10,   11,   12,  168,  190,  190,  190,  190,
 /*  3080 */   190,  190,   20,  190,  190,  178,  179,  180,  181,  190,
 /*  3090 */   190,  190,  190,  190,   32,   33,   34,   35,   36,  190,
 /*  3100 */    38,  190,  190,   41,   42,  190,  190,  190,  190,  190,
 /*  3110 */   190,   49,   50,   51,  190,  190,   54,  190,   56,  190,
 /*  3120 */   190,  190,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  3130 */    68,  190,   70,   71,   72,   73,  190,   75,  168,  190,
 /*  3140 */   190,   79,   80,   81,   82,  190,  190,   85,  178,  179,
 /*  3150 */   180,  181,  190,   91,  190,  190,  190,   95,   96,   97,
 /*  3160 */    98,   99,  100,  101,  102,  103,  104,  105,    7,  190,
 /*  3170 */     9,   10,   11,   12,  168,  190,  190,  190,  190,  190,
 /*  3180 */   190,   20,  190,  190,  178,  179,  180,  181,  190,  190,
 /*  3190 */   190,  190,  190,   32,   33,   34,   35,   36,  190,   38,
 /*  3200 */   190,  190,   41,   42,  190,  190,  190,  190,  190,  190,
 /*  3210 */    49,  141,   51,  190,  190,   54,  190,   56,  190,  190,
 /*  3220 */   190,   60,   61,   62,   63,   64,   65,   66,   67,   68,
 /*  3230 */   190,   70,   71,   72,   73,  190,   75,  190,  168,  190,
 /*  3240 */    79,   80,   81,   82,  190,  190,   85,  190,  178,  179,
 /*  3250 */   180,  181,   91,  190,  190,  190,   95,   96,   97,   98,
 /*  3260 */    99,  100,  101,  102,  103,  104,  105,    7,  190,    9,
 /*  3270 */    10,   11,   12,  190,   40,  190,  190,   43,  190,   45,
 /*  3280 */    20,   47,   48,  190,   41,   51,  190,  190,  190,  190,
 /*  3290 */   190,  190,   32,   33,   34,   35,   36,   54,   38,  190,
 /*  3300 */   190,   41,  190,  190,  190,  190,  190,  190,  190,   49,
 /*  3310 */   190,  190,  190,  190,   54,  190,   56,  190,  190,  190,
 /*  3320 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  3330 */    70,  190,  190,  190,   91,  190,  114,  115,  190,  105,
 /*  3340 */   190,  190,   99,  100,  101,  102,  103,  104,  190,  190,
 /*  3350 */   190,   91,  190,  190,  190,  190,  190,  190,  136,   99,
 /*  3360 */   100,  101,  102,  103,  104,  190,  190,  145,  146,  147,
 /*  3370 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3380 */   158,  159,  160,  161,  162,  163,  164,  114,  115,  190,
 /*  3390 */   168,  119,  190,  190,  122,  123,  124,  190,  126,  127,
 /*  3400 */   178,  179,  180,  181,  132,  133,  190,  190,  190,  136,
 /*  3410 */   138,    1,    2,    3,    4,    5,  190,  190,  145,  146,
 /*  3420 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  3430 */   157,  158,  159,  160,  161,  162,  163,  164,  114,  115,
 /*  3440 */   190,  168,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  3450 */   190,  178,  179,  180,  181,  190,  190,   47,   48,  190,
 /*  3460 */   136,  190,  190,   53,  190,  190,  190,   57,  190,  145,
 /*  3470 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3480 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  114,
 /*  3490 */   115,  190,  168,  190,  190,  121,  190,  123,  190,  125,
 /*  3500 */   190,  127,  178,  179,  180,  181,  132,  190,  134,  190,
 /*  3510 */   190,  136,  138,    1,    2,    3,    4,    5,  190,  190,
 /*  3520 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  3530 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  3540 */   114,  115,  190,  168,  190,  190,  190,  190,  190,  190,
 /*  3550 */   190,  190,  190,  178,  179,  180,  181,  190,  190,   47,
 /*  3560 */    48,  190,  136,  190,  190,  190,  190,   55,  190,   57,
 /*  3570 */   190,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  3580 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3590 */   164,  114,  115,  190,  168,  190,  190,  190,  168,  190,
 /*  3600 */   190,  190,  190,  190,  178,  179,  180,  181,  178,  179,
 /*  3610 */   180,  181,  190,  136,  184,  190,  190,  187,  188,  189,
 /*  3620 */   190,  190,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  3630 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  3640 */   163,  164,  114,  115,  190,  168,  190,  190,  190,  190,
 /*  3650 */   168,  190,  190,  190,  190,  178,  179,  180,  181,  190,
 /*  3660 */   178,  179,  180,  181,  136,  190,    1,    2,    3,    4,
 /*  3670 */     5,  189,  190,  145,  146,  147,  148,  149,  150,  151,
 /*  3680 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  3690 */   162,  163,  164,  114,  115,  190,  168,  190,  190,  190,
 /*  3700 */   190,  190,  190,  190,  190,  190,  178,  179,  180,  181,
 /*  3710 */   190,  190,   47,   48,  190,  136,  190,  190,  190,  190,
 /*  3720 */    55,  190,   57,  190,  145,  146,  147,  148,  149,  150,
 /*  3730 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  3740 */   161,  162,  163,  164,  114,  115,  190,  168,  190,  190,
 /*  3750 */   190,  168,  190,  190,  190,  190,  190,  178,  179,  180,
 /*  3760 */   181,  178,  179,  180,  181,  190,  136,  190,    1,    2,
 /*  3770 */     3,    4,    5,  190,  190,  145,  146,  147,  148,  149,
 /*  3780 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3790 */   160,  161,  162,  163,  164,  114,  115,  190,  168,  190,
 /*  3800 */   190,  190,  190,  190,  190,  190,  190,  190,  178,  179,
 /*  3810 */   180,  181,  190,  190,   47,   48,  190,  136,  190,  190,
 /*  3820 */   190,  190,   55,  190,   57,  190,  145,  146,  147,  148,
 /*  3830 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  3840 */   159,  160,  161,  162,  163,  164,  114,  115,  190,  168,
 /*  3850 */   190,  190,  190,  168,  190,  190,  190,  190,  190,  178,
 /*  3860 */   179,  180,  181,  178,  179,  180,  181,  190,  136,  190,
 /*  3870 */     1,    2,    3,    4,    5,  190,  190,  145,  146,  147,
 /*  3880 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3890 */   158,  159,  160,  161,  162,  163,  164,  114,  115,  190,
 /*  3900 */   168,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  3910 */   178,  179,  180,  181,  190,  190,   47,   48,  190,  136,
 /*  3920 */   190,  190,  190,  190,   55,  190,   57,  190,  145,  146,
 /*  3930 */   147,  148,  149,  150,  151,  152,  153,  154,  155,  156,
 /*  3940 */   157,  158,  159,  160,  161,  162,  163,  164,  114,  115,
 /*  3950 */   190,  168,  190,  190,  190,  168,  190,  190,  190,  190,
 /*  3960 */   190,  178,  179,  180,  181,  178,  179,  180,  181,  190,
 /*  3970 */   136,  190,    1,    2,    3,    4,    5,  190,  190,  145,
 /*  3980 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3990 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  114,
 /*  4000 */   115,  190,  168,  190,  190,  190,  190,  190,  190,  190,
 /*  4010 */   190,  190,  178,  179,  180,  181,  190,  190,   47,   48,
 /*  4020 */   190,  136,  190,  190,  190,  190,  190,  190,   57,  190,
 /*  4030 */   145,  146,  147,  148,  149,  150,  151,  152,  153,  154,
 /*  4040 */   155,  156,  157,  158,  159,  160,  161,  162,  163,  164,
 /*  4050 */   114,  115,  190,  168,  190,  190,  190,  168,  190,  190,
 /*  4060 */   190,  190,  190,  178,  179,  180,  181,  178,  179,  180,
 /*  4070 */   181,  190,  136,  190,  190,  190,  187,  188,  189,  190,
 /*  4080 */   190,  145,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  4090 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  4100 */   164,  114,  115,  190,  168,  190,  190,  190,  168,  190,
 /*  4110 */   190,  190,  190,  190,  178,  179,  180,  181,  178,  179,
 /*  4120 */   180,  181,  190,  136,  190,  190,  190,  190,  190,  190,
 /*  4130 */   190,  190,  145,  146,  147,  148,  149,  150,  151,  152,
 /*  4140 */   153,  154,  155,  156,  157,  158,  159,  160,  161,  162,
 /*  4150 */   163,  164,  114,  115,  190,  168,  190,  190,  190,  168,
 /*  4160 */   190,  190,  190,  190,  190,  178,  179,  180,  181,  178,
 /*  4170 */   179,  180,  181,  190,  136,  190,  190,  190,  190,  190,
 /*  4180 */   190,  190,  190,  145,  146,  147,  148,  149,  150,  151,
 /*  4190 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  4200 */   162,  163,  164,  114,  115,  190,  168,  190,  190,  190,
 /*  4210 */   168,  190,  190,  190,  190,  190,  178,  179,  180,  181,
 /*  4220 */   178,  179,  180,  181,  190,  136,  190,  190,  190,  190,
 /*  4230 */   190,  190,  190,  190,  145,  146,  147,  148,  149,  150,
 /*  4240 */   151,  152,  153,  154,  155,  156,  157,  158,  159,  160,
 /*  4250 */   161,  162,  163,  164,  114,  115,  190,  168,  190,  190,
 /*  4260 */   190,  168,  190,  190,  190,  190,  190,  178,  179,  180,
 /*  4270 */   181,  178,  179,  180,  181,  190,  136,  190,  190,  190,
 /*  4280 */   190,  190,  190,  190,  190,  145,  146,  147,  148,  149,
 /*  4290 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  4300 */   160,  161,  162,  163,  164,  114,  115,  190,  168,  190,
 /*  4310 */   190,  190,  168,  190,  190,  190,  190,  190,  178,  179,
 /*  4320 */   180,  181,  178,  179,  180,  181,  190,  136,  190,    1,
 /*  4330 */     2,    3,    4,    5,  190,  190,  145,  146,  147,  148,
 /*  4340 */   149,  150,  151,  152,  153,  154,  155,  156,  157,  158,
 /*  4350 */   159,  160,  161,  162,  163,  164,  114,  115,  190,  168,
 /*  4360 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  178,
 /*  4370 */   179,  180,  181,  190,  190,   47,   48,  190,   50,   51,
 /*  4380 */   190,   53,  190,  190,  190,   57,  190,  145,  146,  147,
 /*  4390 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4400 */   158,  159,  160,  161,  162,  163,  164,  190,  190,  190,
 /*  4410 */   168,    7,  190,    9,   10,   11,   12,  168,  190,  190,
 /*  4420 */   178,  179,  180,  181,   20,  190,  190,  178,  179,  180,
 /*  4430 */   181,  190,  190,  190,  190,  190,   32,   33,   34,   35,
 /*  4440 */    36,  190,   38,  190,    7,   41,    9,   10,   11,   12,
 /*  4450 */   190,  190,  190,   49,  168,  190,  190,   20,   54,  190,
 /*  4460 */    56,  190,  190,  190,  178,  179,  180,  181,  190,   32,
 /*  4470 */    33,   34,   35,   36,   37,   38,  190,  190,   41,  190,
 /*  4480 */   190,  190,  190,  190,  190,  190,   49,  168,   84,  190,
 /*  4490 */   190,   54,  190,   56,  190,   91,  190,  178,  179,  180,
 /*  4500 */   181,  190,  190,   99,  100,  101,  102,  103,  104,   13,
 /*  4510 */    14,   15,  190,  190,   18,   19,   20,   21,   22,   23,
 /*  4520 */    24,   25,   26,   27,   28,   29,   30,   31,   91,  190,
 /*  4530 */   190,  190,  190,  190,   38,   39,   99,  100,  101,  102,
 /*  4540 */   103,  104,  190,  190,    7,  190,    9,   10,   11,   12,
 /*  4550 */   168,  190,  190,  190,  168,  190,  190,   20,  190,  190,
 /*  4560 */   178,  179,  180,  181,  178,  179,  180,  181,  190,   32,
 /*  4570 */    33,   34,   35,   36,   37,   38,  190,    7,   41,    9,
 /*  4580 */    10,   11,   12,  190,  168,  190,   49,  190,  190,  190,
 /*  4590 */    20,   54,  190,   56,  178,  179,  180,  181,  190,  190,
 /*  4600 */   190,  190,   32,   33,   34,   35,   36,   37,   38,  190,
 /*  4610 */   190,   41,  190,  190,  190,  190,  190,  190,  190,   49,
 /*  4620 */   190,  190,  190,  190,   54,  190,   56,  190,   91,  190,
 /*  4630 */   190,  190,  190,  190,  190,  190,   99,  100,  101,  102,
 /*  4640 */   103,  104,  190,   14,   15,  190,  190,   18,   19,   20,
 /*  4650 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  4660 */    31,   91,  190,  190,  190,  190,  190,   38,   39,   99,
 /*  4670 */   100,  101,  102,  103,  104,  190,  190,    7,  190,    9,
 /*  4680 */    10,   11,   12,  190,    1,    2,    3,    4,    5,  190,
 /*  4690 */    20,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  4700 */   190,  190,   32,   33,   34,   35,   36,   37,   38,  190,
 /*  4710 */     7,   41,    9,   10,   11,   12,  190,  190,  190,   49,
 /*  4720 */   190,  190,  190,   20,   54,  190,   56,  190,  190,  190,
 /*  4730 */    47,   48,  190,   50,   51,   32,   33,   34,   35,   36,
 /*  4740 */    57,   38,  190,  190,   41,  190,  190,  190,  190,  190,
 /*  4750 */   190,  190,   49,  190,  190,  190,  190,   54,  190,   56,
 /*  4760 */   190,   91,   59,  190,  190,  190,  190,  190,  190,   99,
 /*  4770 */   100,  101,  102,  103,  104,  190,   18,   19,   20,   21,
 /*  4780 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  4790 */   190,  190,  190,  190,   91,  190,   38,   39,  190,  190,
 /*  4800 */   190,  190,   99,  100,  101,  102,  103,  104,  190,  190,
 /*  4810 */     7,  190,    9,   10,   11,   12,  190,  190,  190,  190,
 /*  4820 */   190,  190,  190,   20,  190,  190,  190,  190,  190,  190,
 /*  4830 */   190,  190,  190,  190,  190,   32,   33,   34,   35,   36,
 /*  4840 */    37,   38,  190,    7,   41,    9,   10,   11,   12,  190,
 /*  4850 */   190,  190,   49,  190,  190,  190,   20,   54,  190,   56,
 /*  4860 */   190,  190,  190,  190,  190,  190,  190,  190,   32,   33,
 /*  4870 */    34,   35,   36,   37,   38,  190,  190,   41,    0,  190,
 /*  4880 */   190,  190,  190,  190,  190,   49,  190,  190,  190,  190,
 /*  4890 */    54,  190,   56,  190,   91,  190,  190,  190,  190,  190,
 /*  4900 */   190,  190,   99,  100,  101,  102,  103,  104,    1,    2,
 /*  4910 */     3,    4,    5,  190,  190,  190,  190,  190,   40,  190,
 /*  4920 */   190,   43,  190,   45,  190,   47,   48,   91,  190,   51,
 /*  4930 */   190,  190,  190,  190,  190,   99,  100,  101,  102,  103,
 /*  4940 */   104,  190,  190,    7,  190,    9,   10,   11,   12,  190,
 /*  4950 */   190,  190,  190,  190,   47,   48,   20,   50,   51,  190,
 /*  4960 */    53,  190,  190,  190,   57,  190,  190,  190,   32,   33,
 /*  4970 */    34,   35,   36,   37,   38,  190,    7,   41,    9,   10,
 /*  4980 */    11,   12,  190,  105,  190,   49,  190,  190,  190,   20,
 /*  4990 */    54,  190,   56,  190,  190,  190,  190,  190,  190,  190,
 /*  5000 */   190,   32,   33,   34,   35,   36,   20,   38,  190,  190,
 /*  5010 */    41,   42,  190,  190,  190,  190,  190,  190,   49,  190,
 /*  5020 */   190,  190,  190,   54,  190,   56,  190,   91,  190,  190,
 /*  5030 */   190,  190,  190,  190,  190,   99,  100,  101,  102,  103,
 /*  5040 */   104,  190,  190,  190,   58,  190,   60,   61,   62,   63,
 /*  5050 */    64,   65,   66,   67,   68,   69,   70,  190,  190,  190,
 /*  5060 */    91,  190,  190,  190,  190,  190,  190,  190,   99,  100,
 /*  5070 */   101,  102,  103,  104,  190,  190,    7,  190,    9,   10,
 /*  5080 */    11,   12,  190,  190,  190,  190,  190,  190,  190,   20,
 /*  5090 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  5100 */   190,   32,   33,   34,   35,   36,   37,   38,  190,    7,
 /*  5110 */    41,    9,   10,   11,   12,  190,  190,  190,   49,  190,
 /*  5120 */   190,  190,   20,   54,  190,   56,  190,  190,   38,   39,
 /*  5130 */   190,  190,  190,  190,   32,   33,   34,   35,   36,  190,
 /*  5140 */    38,  190,   52,   41,  190,  190,  190,  190,  190,  190,
 /*  5150 */   190,   49,  190,  190,  190,  190,   54,  190,   56,  190,
 /*  5160 */    91,  190,    1,    2,    3,    4,    5,  190,   99,  100,
 /*  5170 */   101,  102,  103,  104,  190,  190,   86,   87,   88,   89,
 /*  5180 */    90,  190,   92,   93,   94,  190,   84,  190,  190,  190,
 /*  5190 */   190,  190,  190,   91,    7,  190,    9,   10,   11,   12,
 /*  5200 */   190,   99,  100,  101,  102,  103,  104,   20,   47,   48,
 /*  5210 */   190,   50,   51,  190,   53,  190,  190,  190,   57,   32,
 /*  5220 */    33,   34,   35,   36,  190,   38,  190,    7,   41,    9,
 /*  5230 */    10,   11,   12,  190,  190,  190,   49,  190,  190,  190,
 /*  5240 */    20,   54,  190,   56,  190,  190,   59,  190,  190,  190,
 /*  5250 */   190,  190,   32,   33,   34,   35,   36,   20,   38,  190,
 /*  5260 */   190,   41,  190,  190,  190,  190,  190,  190,  190,   49,
 /*  5270 */   190,  190,  190,  190,   54,  190,   56,  190,   91,   59,
 /*  5280 */   190,  190,  190,  190,  190,  190,   99,  100,  101,  102,
 /*  5290 */   103,  104,    1,    2,    3,    4,    5,   60,   61,   62,
 /*  5300 */    63,   64,   65,   66,   67,   68,   69,   70,  190,  190,
 /*  5310 */   190,   91,  190,  190,  190,  190,  190,  190,  190,   99,
 /*  5320 */   100,  101,  102,  103,  104,  190,  190,    7,  190,    9,
 /*  5330 */    10,   11,   12,  190,  190,  190,  190,  190,   47,   48,
 /*  5340 */    20,   50,   51,  190,   53,  190,  190,  190,   57,  190,
 /*  5350 */   190,  190,   32,   33,   34,   35,   36,  190,   38,  190,
 /*  5360 */     7,   41,    9,   10,   11,   12,  190,  190,  190,   49,
 /*  5370 */   190,  190,  190,   20,   54,  190,   56,  190,  190,   59,
 /*  5380 */   190,  190,  190,  190,  190,   32,   33,   34,   35,   36,
 /*  5390 */    37,   38,  190,  190,   41,  190,  190,  190,  190,  190,
 /*  5400 */   190,  190,   49,  190,  190,  190,  190,   54,  190,   56,
 /*  5410 */   190,   91,  190,  190,  190,  190,  190,  190,  190,   99,
 /*  5420 */   100,  101,  102,  103,  104,    1,    2,    3,    4,    5,
 /*  5430 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  5440 */   190,  190,  190,  190,   91,  190,  190,  190,  190,  190,
 /*  5450 */   190,  190,   99,  100,  101,  102,  103,  104,  190,  190,
 /*  5460 */     7,  190,    9,   10,   11,   12,  190,  190,  190,  190,
 /*  5470 */   190,   47,   48,   20,   50,   51,  190,   53,  190,  190,
 /*  5480 */   190,   57,  190,  190,  190,   32,   33,   34,   35,   36,
 /*  5490 */    37,   38,  190,    7,   41,    9,   10,   11,   12,  190,
 /*  5500 */   190,  190,   49,  190,  190,  190,   20,   54,  190,   56,
 /*  5510 */   190,  190,  190,  190,  190,  190,  190,  190,   32,   33,
 /*  5520 */    34,   35,   36,  190,   38,  190,  190,   41,  190,  190,
 /*  5530 */   190,  190,  190,  190,  190,   49,  190,  190,  190,  190,
 /*  5540 */    54,  190,   56,  190,   91,  190,  190,  190,  190,  190,
 /*  5550 */   190,  190,   99,  100,  101,  102,  103,  104,    1,    2,
 /*  5560 */     3,    4,    5,  190,  190,  190,  190,  190,  190,  190,
 /*  5570 */   190,  190,  190,  190,  190,  190,  190,   91,  190,  190,
 /*  5580 */   190,  190,  190,  190,  190,   99,  100,  101,  102,  103,
 /*  5590 */   104,  190,  190,    7,  190,    9,   10,   11,   12,  190,
 /*  5600 */   190,  190,  190,  190,   47,   48,   20,   50,   51,  190,
 /*  5610 */    53,  190,  190,  190,   57,  190,  190,  190,   32,   33,
 /*  5620 */    34,   35,   36,  190,   38,  190,  190,   41,  190,  190,
 /*  5630 */   190,  190,  190,  190,  190,   49,  190,  190,  190,  190,
 /*  5640 */    54,  190,   56,  190,    8,  190,  190,  190,  190,   13,
 /*  5650 */    14,   15,  190,  190,   18,   19,   20,   21,   22,   23,
 /*  5660 */    24,   25,   26,   27,   28,   29,   30,   31,  190,  190,
 /*  5670 */   190,  190,  190,  190,   38,   39,  190,   91,   42,  190,
 /*  5680 */   190,  190,  190,  190,  190,   99,  100,  101,  102,  103,
 /*  5690 */   104,  190,  190,  190,    8,  190,  190,  190,  190,   13,
 /*  5700 */    14,   15,  190,  190,   18,   19,   20,   21,   22,   23,
 /*  5710 */    24,   25,   26,   27,   28,   29,   30,   31,  190,  190,
 /*  5720 */   190,  190,  190,  190,   38,   39,    8,  190,   42,  190,
 /*  5730 */   190,   13,   14,   15,  190,  190,   18,   19,   20,   21,
 /*  5740 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  5750 */   190,  190,  190,  190,  190,  190,   38,   39,    8,  190,
 /*  5760 */   190,  190,  190,   13,   14,   15,  190,   49,   18,   19,
 /*  5770 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  5780 */    30,   31,  190,  190,  190,    8,  190,   37,   38,   39,
 /*  5790 */    13,   14,   15,  190,  190,   18,   19,   20,   21,   22,
 /*  5800 */    23,   24,   25,   26,   27,   28,   29,   30,   31,  190,
 /*  5810 */   190,  190,  190,  190,  190,   38,   39,    8,  190,  190,
 /*  5820 */   190,  190,   13,   14,   15,  190,   49,   18,   19,   20,
 /*  5830 */    21,   22,   23,   24,   25,   26,   27,   28,   29,   30,
 /*  5840 */    31,  190,  190,  190,  190,  190,  190,   38,   39,    8,
 /*  5850 */   190,  190,  190,  190,   13,   14,   15,  190,   49,   18,
 /*  5860 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  5870 */    29,   30,   31,  190,  190,  190,    8,  190,  190,   38,
 /*  5880 */    39,   13,   14,   15,  190,  190,   18,   19,   20,   21,
 /*  5890 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  5900 */    59,  190,  190,    8,  190,  190,   38,   39,   13,   14,
 /*  5910 */    15,  190,  190,   18,   19,   20,   21,   22,   23,   24,
 /*  5920 */    25,   26,   27,   28,   29,   30,   31,  190,  190,  190,
 /*  5930 */   190,  190,  190,   38,   39,  190,  190,   42,  190,  190,
 /*  5940 */   190,  190,    8,  190,  190,   77,  190,   13,   14,   15,
 /*  5950 */   190,  190,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  5960 */    26,   27,   28,   29,   30,   31,  190,  190,  190,  190,
 /*  5970 */   190,  190,   38,   39,    8,  190,  190,  190,  190,   13,
 /*  5980 */    14,   15,  190,   49,   18,   19,   20,   21,   22,   23,
 /*  5990 */    24,   25,   26,   27,   28,   29,   30,   31,  190,  190,
 /*  6000 */   190,    8,  190,  190,   38,   39,   13,   14,   15,   20,
 /*  6010 */   190,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  6020 */    27,   28,   29,   30,   31,   59,   37,   20,  190,  190,
 /*  6030 */    41,   38,   39,  190,  190,  190,  190,  190,  190,  190,
 /*  6040 */   190,  190,  190,   20,   37,  190,  190,  190,   41,   60,
 /*  6050 */    61,   62,   63,   64,   65,   66,   67,   68,   69,   70,
 /*  6060 */    37,  190,  190,  190,   41,  190,  190,   60,   61,   62,
 /*  6070 */    63,   64,   65,   66,   67,   68,   69,   70,   20,  190,
 /*  6080 */   190,  190,  190,   60,   61,   62,   63,   64,   65,   66,
 /*  6090 */    67,   68,   69,   70,   20,   37,  190,  190,  190,   41,
 /*  6100 */   190,    1,    2,    3,    4,    5,    1,    2,    3,    4,
 /*  6110 */     5,  190,  190,  190,  190,   41,  190,  190,   60,   61,
 /*  6120 */    62,   63,   64,   65,   66,   67,   68,   69,   70,    1,
 /*  6130 */     2,    3,    4,    5,   60,   61,   62,   63,   64,   65,
 /*  6140 */    66,   67,   68,   69,   70,  190,  190,   47,   48,  190,
 /*  6150 */    50,   51,   47,   48,  190,   50,   51,   57,  190,  190,
 /*  6160 */   190,  190,   57,  190,    1,    2,    3,    4,    5,   41,
 /*  6170 */   190,  190,  190,  190,  190,   47,   48,    1,    2,    3,
 /*  6180 */     4,    5,  190,   55,  190,   57,  190,  190,  190,  190,
 /*  6190 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  6200 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  6210 */    47,   48,  190,   50,   51,    1,    2,    3,    4,    5,
 /*  6220 */    57,  190,  190,   47,   48,  190,   50,   51,    1,    2,
 /*  6230 */     3,    4,    5,   57,  190,  190,  190,  190,  190,  190,
 /*  6240 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  6250 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  6260 */   190,   47,   48,  190,   50,   51,    1,    2,    3,    4,
 /*  6270 */     5,   57,  190,  190,   47,   48,  190,   50,   51,    1,
 /*  6280 */     2,    3,    4,    5,   57,  190,  190,  190,  190,  190,
 /*  6290 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  6300 */   190,  190,  190,  190,  190,  190,  190,  190,  190,  190,
 /*  6310 */   190,  190,   47,   48,  190,   50,   51,  190,  190,   41,
 /*  6320 */   190,  190,   57,  190,  190,   47,   48,  190,  190,  190,
 /*  6330 */   190,  190,  190,   55,  190,   57,
};
#define YY_SHIFT_USE_DFLT (-27)
static short yy_shift_ofst[] = {
 /*     0 */  3234,  121, 4878,  -27,  -27,  -27,  -27,  -27,  -27,  219,
 /*    10 */   105,  -27,  237,  117,  -27,  257,  269,  -27, 4907,  -27,
 /*    20 */   324,  -27, 5291, 6100,  -27, 3869,  356,  378, 6058,   -1,
 /*    30 */  4986,  425,  -27,  -27,  465, 5237,  -27,  581,  -27,  -27,
 /*    40 */   -27,  484,  390,  -27,  503,  603,  -27,  -27,  -27,  -27,
 /*    50 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*    60 */   640,  573, 4986,  576,  -27,  -27, 6074,  -27,  588, 3243,
 /*    70 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  613,  625,
 /*    80 */   -27,  -27,  591,  703, 3243,  -27,  723,  726, 3243,  -27,
 /*    90 */   761,  751, 3243,  -27,  763,  791,  -27,  -27,  -27,  -27,
 /*   100 */   -27,  -27,  -27,  -27,  -27,  -27, 3971, 3665,  773,  766,
 /*   110 */  5989,  702, 4986,  783,  -27,  -27,  244,  288, 4986,  786,
 /*   120 */   -27,  -27,  -27,  -27,  -27, 3410,  775,  778, 3243,  795,
 /*   130 */   -27,  779,  788, 3243,  800,  -27, 4683,  -27,  802,  487,
 /*   140 */   -27,  813,  380,  -27,  822,  595,  -27, 5424,  -27,  818,
 /*   150 */   -27, 4328, 6176,  -27, 3767,  830,  816, 6023,  676, 2072,
 /*   160 */   -27, 1973,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   170 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   180 */   -27,  -27,  -27,  -27,  -27,  -27,  827,  -27,  832,  -27,
 /*   190 */  5486,  826, 1577, 1379,  803,  829, 1280, 1181,  -27, 5486,
 /*   200 */   843,  -26,  -27,  271,  -27,  -27, 3243,  835, 3161, 3161,
 /*   210 */   849, 2171, 1874,  -27, 5486,  857, 1676, 1082,  -27,  864,
 /*   220 */    -7, 1478,  834, 5486,  873,  -27, 5486,  489, 5486, 5486,
 /*   230 */   489, 5486, 5486, 5993, 5486, 5486, 4496, 5486, 5486, 4496,
 /*   240 */  5486, 5486, 4496, 5486, 5486, 4496, 5486,  178, 5486,  178,
 /*   250 */   181,  865, 4836, 5993, 5486, 5966,  -27, 5486,  106, 5486,
 /*   260 */   106, 5486,  489, 5486,  489, 5486,  489, 5486,  106, 5486,
 /*   270 */  4758, 5486, 4758, 5486, 4629, 5486, 5868, 5486, 5993, 3260,
 /*   280 */  5750,  -27,  886, 5486,  489,  883,  906, 5486,  178,  228,
 /*   290 */   572,  872, 4570,  888,  924, 5486,  489,  -27, 5486,  489,
 /*   300 */   -27,  -27,  -27,  -27,  -27,  -27,  -27,  -27, 5220, 5993,
 /*   310 */   143, 5069,  135,  875, 4670,  -27,  132,  -27, 5486,  895,
 /*   320 */   882,   83, 5353,  787,  -27,  -27,  -27,  -27,  -27,  -27,
 /*   330 */   685,  881, 4936,  770,  -27,  -27,  898,  892,  885, 4803,
 /*   340 */   733,  -27,  -27,  -27,  902,  903,  905, 4537,  -27,  708,
 /*   350 */   -27,  732,  -27,  -27,  890,  897,  -27,  754,  -27, 5586,
 /*   360 */   -27,  899, 5486,  -27,  -27,  -27,  794,  -27,  -27,  -27,
 /*   370 */    -5,  -27,  -27,   79,  908,  912, 4437,  145,  -27,  -27,
 /*   380 */   941,  942, 5453,  327,  -27,  -27,  178,  178,  178,  178,
 /*   390 */   178,  178, 5993,  921,  452, 4404, 5718, 3161, 1775,  -27,
 /*   400 */   956,  498,  -27,  956,  -27, 5090,  -27,  -27,  -27,  -27,
 /*   410 */   -27,  -27, 5486,  -27, 5993,  565,   29, 5486,  -27, 5187,
 /*   420 */    19, 5486,  -27,  940,  -27, 5841,  227, 5486,  -27,  -27,
 /*   430 */  5486,  -27,  -27,  -27,  466,  950,   19, 5486,  -27,  952,
 /*   440 */    19, 5486,  -27,  971,  821, 5486,  -27, 5320,   19, 5486,
 /*   450 */   -27,  227, 5486,  -27, 4703,   19, 5486,  -27,  227, 5486,
 /*   460 */   -27,  -27,  -27,  -27, 5486,  757,  -27, 5486,  -27, 5993,
 /*   470 */   -27,  972,  -27,  978,  -27,  981,  -27,  984,  -27, 4969,
 /*   480 */  5636,  -27,  -27, 5486, 5895,  -27, 5486, 5686,  -27,  986,
 /*   490 */   262,  -27,  986,  -27,  977, 3243,  -27,  -27,  986,  277,
 /*   500 */   -27,  986,  376,  -27,  986,  374,  -27,  986,  473,  -27,
 /*   510 */   986,  589,  -27,  986,  597,  -27,  986,  629,  -27,  986,
 /*   520 */   658,  -27,  986,    6,  -27, 5993,  -27,  -27,  -27, 5486,
 /*   530 */  5809, 3161, 2666,  -27,  989,  948, 5102, 5934, 3161, 2963,
 /*   540 */   -27, 5486, 5777, 3161,   92,  -27,  958, 5486,  994,  -27,
 /*   550 */   -27,  -27,  963, 3161, 3161,  -27,  -27,  967,  993,  389,
 /*   560 */   686,  -27,  -27, 4986, 1003,  884,  -27,  785,  -27,  664,
 /*   570 */   655,  587,  -27,  488,  -27, 4986, 1008,  290,  -27,  191,
 /*   580 */   -27, 3971, 3512, 1019, 1005, 6007,   61,  983,  -27, 3062,
 /*   590 */   -27, 4986,   38, 2864,  -27, 2765,  -27,  624,  674, 2567,
 /*   600 */   -27, 2468,  -27, 4986, 1018, 2369,  -27, 2270,  -27, 5161,
 /*   610 */  6265,  -27, 3410,  -27, 3410, 6278,  225,  -27, 3243,   20,
 /*   620 */   -27, 1034,  -27,   99, 1035,  367, 1054,  692,  -27,  -27,
 /*   630 */  1057,  -27,  -27, 1058,  -27, 6128,  -19,  -27, 3243,  189,
 /*   640 */   -27, 1067,  -27, 1068,  -27, 5557, 6227, 6163, 3971, 6105,
 /*   650 */   -27, 6214,  822,  -27,  -27,  -27,  822,  595,  -27, 1066,
 /*   660 */  1078,  200,  -27, 1081, 1076,  -27,  822,  595,  -27, 1083,
 /*   670 */  1088,  397,  -27, 1093, 1076,  -27,  -27,
};
#define YY_REDUCE_USE_DFLT (-105)
static short yy_reduce_ofst[] = {
 /*     0 */   -93, -105, -104, -105, -105, -105, -105, -105, -105, -105,
 /*    10 */  -105, -105, -105,  169, -105, -105,  229, -105, 3374, -105,
 /*    20 */  -105, -105,  310,  407, -105,  221, -105, -105,  194, -105,
 /*    30 */  1164, -105, -105, -105, -105,  263, -105, -105, -105, -105,
 /*    40 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*    50 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*    60 */  -105, -105, 2154, -105, -105, -105,  266, -105, -105,  528,
 /*    70 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*    80 */  -105, -105, -105, -105,  537, -105, -105, -105,  673, -105,
 /*    90 */  -105, -105,  678, -105, -105, -105, -105, -105, -105, -105,
 /*   100 */  -105, -105, -105, -105, -105, -105,  405,  221, -105, -105,
 /*   110 */   -65, -105,  760, -105, -105, -105, -105, -105, 1156, -105,
 /*   120 */  -105, -105, -105, -105, -105,  405, -105, -105,  704, -105,
 /*   130 */  -105, -105, -105,  710, -105, -105,  407, -105, -105,  727,
 /*   140 */  -105, -105,  744, -105,  612,  745, -105, 3272, -105, -105,
 /*   150 */  -105,  -87,  142, -105,  221, -105, -105,   77, -105, 3987,
 /*   160 */  -105, 4242, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   170 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   180 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, -105,
 /*   190 */  1189, -105, 3936, 4242, -105, -105, 3783, 4242, -105, 1288,
 /*   200 */  -105,  306, -105,  712, -105, -105,  777, -105, 4191, 4242,
 /*   210 */  -105, 3222, 4242, -105, 1486, -105, 3273, 4242, -105, -105,
 /*   220 */  3630, 4242, -105,  991, -105, -105, 4042, -105, 2412, 1584,
 /*   230 */  -105, 2376, 2574, -105, 2511, 2475, -105, 1683, 2178, -105,
 /*   240 */  2709, 2772, -105,  338, 2970, -105, 4144, -105, 4382, -105,
 /*   250 */  -105, -105,  792, -105, 4286, -105, -105, 2313, -105, 3685,
 /*   260 */  -105, 3583, -105, 2871, -105, 2907, -105, 2673, -105, 3991,
 /*   270 */  -105, 1980, -105, 4386, -105, 3006, -105, 2079, -105, 3070,
 /*   280 */  -105, -105, -105, 1881, -105, -105, -105, 4319, -105, -105,
 /*   290 */  -105, -105,  630, -105, -105, 3787, -105, -105, 4093, -105,
 /*   300 */  -105, -105, -105, -105, -105, -105, -105, -105, 3430, -105,
 /*   310 */  -105,    0, -105, -105,  594, -105, -105, -105, 1089, -105,
 /*   320 */  -105, -105,  531, -105, -105, -105, -105, -105, -105, -105,
 /*   330 */  -105, -105,  495, -105, -105, -105, -105, -105, -105,  396,
 /*   340 */  -105, -105, -105, -105, -105, -105, -105,  297, -105, -105,
 /*   350 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, 3889,
 /*   360 */  -105, -105, 3482, -105, -105, -105, -105, -105, -105, -105,
 /*   370 */  -105, -105, -105, -105, -105, -105,  234, -105, -105, -105,
 /*   380 */  -105, -105,  198, -105, -105, -105, -105, -105, -105, -105,
 /*   390 */  -105, -105, -105, -105, -105, 3940, -105, 3681, 4242, -105,
 /*   400 */   402, -105, -105,  831, -105,  -42, -105, -105, -105, -105,
 /*   410 */  -105, -105, 2016, -105, -105, -105,  -60, 1917, -105,  891,
 /*   420 */   833, 1818, -105, -105, -105, -105,  471, 1719, -105, -105,
 /*   430 */   891, -105, -105, -105, -105, -105,  839, 1620, -105, -105,
 /*   440 */   840, 1521, -105, -105,  -91, 1422, -105,  891,  842, 1323,
 /*   450 */  -105,  292, 1224, -105,  891,  851, 1026, -105,   39,  927,
 /*   460 */  -105, -105, -105, -105,  333, -105, -105,  828, -105, -105,
 /*   470 */  -105, -105, -105, -105, -105, -105, -105, -105, -105, 1782,
 /*   480 */  -105, -105, -105, 2277, -105, -105, 4416, -105, -105,  -97,
 /*   490 */  -105, -105,  845, -105, -105,  907, -105, -105,   32, -105,
 /*   500 */  -105,  129, -105, -105,  185, -105, -105,  250, -105, -105,
 /*   510 */   383, -105, -105,  426, -105, -105,  451, -105, -105,  520,
 /*   520 */  -105, -105,  525, -105, -105, -105, -105, -105, -105, 4249,
 /*   530 */  -105, 4089, 4242, -105, -105, -105, 2808, -105, 3324, 4242,
 /*   540 */  -105, 2610, -105, 4038, 4242, -105, -105, 1387, -105, -105,
 /*   550 */  -105, -105, -105, 4140, 4242, -105, -105, -105, -105, 3477,
 /*   560 */  4242, -105, -105, 2146, -105, 3375, -105, 4242, -105, -105,
 /*   570 */  -105, 3579, -105, 4242, -105, 2245, -105, 3885, -105, 4242,
 /*   580 */  -105,  101,  221, -105, -105,  361, -105, 3834, -105, 4242,
 /*   590 */  -105, 2253, -105, 3528, -105, 4242, -105, -105, -105, 3732,
 /*   600 */  -105, 4242, -105,  166, -105, 3426, -105, 4242, -105,  210,
 /*   610 */   142, -105,  101, -105,  309,  221,  944, -105,  947,  961,
 /*   620 */  -105, -105, -105,  416, -105, -105, -105,  966, -105, -105,
 /*   630 */  -105, -105, -105, -105, -105,  221,  974, -105,  973,  975,
 /*   640 */  -105, -105, -105, -105, -105,  478,  110,  142,  309,  142,
 /*   650 */  -105,  142,  992, -105, -105, -105,  611,  996, -105, -105,
 /*   660 */  -105, 1004, -105, -105, 1009, -105,  460, 1010, -105, -105,
 /*   670 */  -105, 1013, -105, -105, 1020, -105, -105,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1006, 1006, 1006,  679,  681,  682,  683,  684,  685, 1006,
 /*    10 */  1006,  686, 1006, 1006,  687, 1006, 1006,  688, 1006,  703,
 /*    20 */  1006,  704, 1006, 1006,  737, 1006, 1006, 1006, 1006, 1006,
 /*    30 */  1006, 1006,  757,  775,  776, 1006,  777,  779,  780,  781,
 /*    40 */   782, 1006, 1006,  793, 1006, 1006,  794,  795,  796,  797,
 /*    50 */   798,  799,  800,  801,  802,  803,  804,  805,  778,  761,
 /*    60 */  1006, 1006, 1006, 1006,  758,  762, 1006,  783,  785, 1006,
 /*    70 */   789,  994,  995,  996,  997,  998,  999, 1000, 1006, 1006,
 /*    80 */  1001, 1002, 1006,  786, 1006,  790, 1006,  787, 1006,  791,
 /*    90 */  1006,  788, 1006,  792, 1006, 1006,  784,  765,  767,  768,
 /*   100 */   769,  770,  771,  772,  773,  774, 1006, 1006, 1006, 1006,
 /*   110 */  1006, 1006, 1006, 1006,  759,  763, 1006, 1006, 1006, 1006,
 /*   120 */   760,  764,  766,  733,  738, 1006, 1006, 1006, 1006, 1006,
 /*   130 */   739, 1006, 1006, 1006, 1006,  740, 1006,  734, 1006, 1006,
 /*   140 */   689, 1006, 1006,  690, 1006, 1006,  692, 1006,  698, 1006,
 /*   150 */   699, 1006, 1006,  735, 1006, 1006, 1006, 1006, 1006, 1006,
 /*   160 */   741, 1006,  743,  806,  808,  809,  810,  811,  812,  813,
 /*   170 */   814,  815,  816,  817,  818,  819,  820,  821,  822,  823,
 /*   180 */   824,  825,  826,  827,  828,  829, 1006,  830, 1006,  831,
 /*   190 */  1006, 1006, 1006, 1006,  834, 1006, 1006, 1006,  835, 1006,
 /*   200 */  1006, 1006,  838, 1006,  839,  840, 1006, 1006,  842,  843,
 /*   210 */  1006, 1006, 1006,  846, 1006, 1006, 1006, 1006,  848, 1006,
 /*   220 */  1006, 1006, 1006, 1006, 1006,  850, 1006,  910, 1006, 1006,
 /*   230 */   911, 1006, 1006,  912, 1006, 1006,  913, 1006, 1006,  914,
 /*   240 */  1006, 1006,  915, 1006, 1006,  916, 1006,  923, 1006,  924,
 /*   250 */  1006,  928, 1006,  985, 1006, 1006,  933, 1006,  934, 1006,
 /*   260 */   935, 1006,  936, 1006,  937, 1006,  938, 1006,  939, 1006,
 /*   270 */   940, 1006,  941, 1006,  942, 1006, 1006, 1006,  982, 1006,
 /*   280 */  1006,  925, 1006, 1006,  926, 1006, 1006, 1006,  927,  946,
 /*   290 */  1006,  931, 1006, 1006, 1006, 1006,  943,  944, 1006,  945,
 /*   300 */   947,  948,  949,  950,  951,  952,  953,  954, 1006,  993,
 /*   310 */   946, 1006, 1006,  957, 1006,  958, 1006,  959, 1006, 1006,
 /*   320 */  1006, 1006, 1006, 1006,  965,  966,  979,  980,  981,  984,
 /*   330 */  1006, 1006, 1006, 1006,  969,  970, 1006, 1006, 1006, 1006,
 /*   340 */  1006,  971,  972,  983, 1006, 1006,  960, 1006,  961, 1006,
 /*   350 */   962, 1006,  963,  964,  947,  948,  955, 1006,  956, 1006,
 /*   360 */   986, 1006, 1006,  988,  989,  987, 1006,  967,  968,  932,
 /*   370 */  1006,  973,  974, 1006, 1006,  929, 1006, 1006,  975,  976,
 /*   380 */  1006,  930, 1006, 1006,  977,  978,  922,  921,  920,  919,
 /*   390 */   918,  917, 1003, 1006, 1006, 1006, 1006, 1006, 1006,  851,
 /*   400 */  1006, 1006,  855, 1006,  856, 1006,  858,  859,  860,  861,
 /*   410 */   862,  863, 1006,  864,  909, 1006, 1006, 1006,  865, 1006,
 /*   420 */  1006, 1006,  868, 1006,  877, 1006, 1006, 1006,  869,  875,
 /*   430 */  1006,  876,  878,  879, 1006, 1006, 1006, 1006,  866, 1006,
 /*   440 */  1006, 1006,  867, 1006, 1006, 1006,  870, 1006, 1006, 1006,
 /*   450 */   871, 1006, 1006,  872, 1006, 1006, 1006,  873, 1006, 1006,
 /*   460 */   874,  880,  881,  857, 1006, 1006,  883, 1006,  884,  886,
 /*   470 */   885,  979,  887,  981,  888,  980,  889,  944,  890, 1006,
 /*   480 */  1006,  891,  892, 1006, 1006,  893, 1006, 1006,  894, 1006,
 /*   490 */  1006,  895, 1006,  905,  907, 1006,  908,  906, 1006, 1006,
 /*   500 */   896, 1006, 1006,  897, 1006, 1006,  898, 1006, 1006,  899,
 /*   510 */  1006, 1006,  900, 1006, 1006,  901, 1006, 1006,  902, 1006,
 /*   520 */  1006,  903, 1006, 1006,  904, 1006, 1004, 1005,  807, 1006,
 /*   530 */  1006, 1006, 1006,  852, 1006, 1006, 1006, 1006, 1006, 1006,
 /*   540 */   853, 1006, 1006, 1006, 1006,  854, 1006, 1006, 1006,  849,
 /*   550 */   847,  845, 1006, 1006,  844,  841,  836,  832, 1006, 1006,
 /*   560 */  1006,  837,  833, 1006, 1006, 1006,  749, 1006,  751, 1006,
 /*   570 */  1006, 1006,  742, 1006,  744, 1006, 1006, 1006,  750, 1006,
 /*   580 */   752, 1006, 1006, 1006, 1006, 1006, 1006, 1006,  745, 1006,
 /*   590 */   747, 1006, 1006, 1006,  753, 1006,  755, 1006, 1006, 1006,
 /*   600 */   746, 1006,  748, 1006, 1006, 1006,  754, 1006,  756, 1006,
 /*   610 */  1006,  736, 1006,  717, 1006, 1006, 1006,  719, 1006, 1006,
 /*   620 */   721, 1006,  725, 1006, 1006, 1006, 1006, 1006,  729,  731,
 /*   630 */  1006,  732,  730, 1006,  723, 1006, 1006,  720, 1006, 1006,
 /*   640 */   722, 1006,  726, 1006,  724, 1006, 1006, 1006, 1006, 1006,
 /*   650 */   718, 1006, 1006,  700,  702,  701, 1006, 1006,  691, 1006,
 /*   660 */  1006, 1006,  693, 1006, 1006,  694, 1006, 1006,  695, 1006,
 /*   670 */  1006, 1006,  696, 1006, 1006,  697,  680,
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
  "MULASSIGN",     "DIVASSIGN",     "CONCATASSIGN",  "STRING",      
  "DOUBLECOLON",   "INCR",          "DECR",          "ECHO",        
  "RETURN",        "UNSET",         "THROW",         "INTEGER",     
  "CHAR",          "DOUBLE",        "NULL",          "TRUE",        
  "FALSE",         "CBLOCK",        "error",         "program",     
  "xx_language",   "xx_top_statement_list",  "xx_top_statement",  "xx_namespace_def",
  "xx_class_def",  "xx_interface_def",  "xx_comment",    "xx_cblock",   
  "xx_interface_body",  "xx_class_body",  "xx_implements_list",  "xx_class_definition",
  "xx_implements",  "xx_interface_definition",  "xx_class_properties_definition",  "xx_class_consts_definition",
  "xx_class_methods_definition",  "xx_interface_methods_definition",  "xx_class_property_definition",  "xx_visibility_list",
  "xx_literal_expr",  "xx_class_property_shortcuts",  "xx_class_property_shortcuts_list",  "xx_class_property_shortcut",
  "xx_class_const_definition",  "xx_class_method_definition",  "xx_interface_method_definition",  "xx_parameter_list",
  "xx_statement_list",  "xx_method_return_type",  "xx_visibility",  "xx_method_return_type_list",
  "xx_method_return_type_item",  "xx_parameter_type",  "xx_parameter_cast",  "xx_parameter_cast_collection",
  "xx_parameter",  "xx_statement",  "xx_let_statement",  "xx_if_statement",
  "xx_loop_statement",  "xx_echo_statement",  "xx_return_statement",  "xx_fetch_statement",
  "xx_fcall_statement",  "xx_mcall_statement",  "xx_scall_statement",  "xx_unset_statement",
  "xx_throw_statement",  "xx_declare_statement",  "xx_break_statement",  "xx_continue_statement",
  "xx_while_statement",  "xx_do_while_statement",  "xx_switch_statement",  "xx_for_statement",
  "xx_empty_statement",  "xx_eval_expr",  "xx_case_clauses",  "xx_case_clause",
  "xx_common_expr",  "xx_let_assignments",  "xx_let_assignment",  "xx_assignment_operator",
  "xx_assign_expr",  "xx_array_offset_list",  "xx_array_offset",  "xx_index_expr",
  "xx_echo_expressions",  "xx_echo_expression",  "xx_mcall_expr",  "xx_fcall_expr",
  "xx_scall_expr",  "xx_fetch_expr",  "xx_declare_variable_list",  "xx_declare_variable",
  "xx_array_list",  "xx_call_parameters",  "xx_call_parameter",  "xx_array_item",
  "xx_array_key",  "xx_array_value",
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
 /* 187 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 188 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 193 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 194 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 195 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 196 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 197 */ "xx_let_assignment ::= IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 198 */ "xx_array_offset_list ::= xx_array_offset_list xx_array_offset",
 /* 199 */ "xx_array_offset_list ::= xx_array_offset",
 /* 200 */ "xx_array_offset ::= SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE",
 /* 201 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 202 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 203 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 204 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 205 */ "xx_index_expr ::= xx_common_expr",
 /* 206 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 207 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 208 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 209 */ "xx_echo_expression ::= xx_common_expr",
 /* 210 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 211 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 212 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 213 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 214 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 215 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 216 */ "xx_unset_statement ::= UNSET xx_common_expr DOTCOMMA",
 /* 217 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 218 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 219 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 220 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 221 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 222 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 223 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 224 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 225 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 226 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 227 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 228 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 229 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 230 */ "xx_declare_variable ::= IDENTIFIER",
 /* 231 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 232 */ "xx_assign_expr ::= xx_common_expr",
 /* 233 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 234 */ "xx_common_expr ::= ISSET xx_common_expr",
 /* 235 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 236 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 237 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 238 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 239 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 240 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 241 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 242 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 243 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 245 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 246 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 247 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 248 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 249 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 250 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 251 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 252 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 253 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 254 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 255 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 256 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 257 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 258 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 259 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 260 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 261 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 262 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 263 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 264 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 265 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 266 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_common_expr",
 /* 267 */ "xx_common_expr ::= xx_fetch_expr",
 /* 268 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 269 */ "xx_common_expr ::= IDENTIFIER",
 /* 270 */ "xx_common_expr ::= INTEGER",
 /* 271 */ "xx_common_expr ::= STRING",
 /* 272 */ "xx_common_expr ::= CHAR",
 /* 273 */ "xx_common_expr ::= DOUBLE",
 /* 274 */ "xx_common_expr ::= NULL",
 /* 275 */ "xx_common_expr ::= TRUE",
 /* 276 */ "xx_common_expr ::= FALSE",
 /* 277 */ "xx_common_expr ::= CONSTANT",
 /* 278 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 279 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 280 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 281 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 282 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 283 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 284 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 285 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 286 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 287 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 288 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 289 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 290 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 291 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 292 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 293 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 294 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 295 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 296 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 297 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 298 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 299 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 300 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 301 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 302 */ "xx_common_expr ::= xx_mcall_expr",
 /* 303 */ "xx_common_expr ::= xx_scall_expr",
 /* 304 */ "xx_common_expr ::= xx_fcall_expr",
 /* 305 */ "xx_common_expr ::= xx_common_expr QUESTION xx_common_expr COLON xx_common_expr",
 /* 306 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 307 */ "xx_call_parameters ::= xx_call_parameter",
 /* 308 */ "xx_call_parameter ::= xx_common_expr",
 /* 309 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 310 */ "xx_array_list ::= xx_array_item",
 /* 311 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 312 */ "xx_array_item ::= xx_array_value",
 /* 313 */ "xx_array_key ::= IDENTIFIER",
 /* 314 */ "xx_array_key ::= STRING",
 /* 315 */ "xx_array_key ::= INTEGER",
 /* 316 */ "xx_array_value ::= xx_common_expr",
 /* 317 */ "xx_literal_expr ::= INTEGER",
 /* 318 */ "xx_literal_expr ::= CHAR",
 /* 319 */ "xx_literal_expr ::= STRING",
 /* 320 */ "xx_literal_expr ::= DOUBLE",
 /* 321 */ "xx_literal_expr ::= NULL",
 /* 322 */ "xx_literal_expr ::= FALSE",
 /* 323 */ "xx_literal_expr ::= TRUE",
 /* 324 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 325 */ "xx_literal_expr ::= CONSTANT",
 /* 326 */ "xx_eval_expr ::= xx_common_expr",
 /* 327 */ "xx_comment ::= COMMENT",
 /* 328 */ "xx_cblock ::= CBLOCK",
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
// 1022 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3184 "parser.c"
      break;
    case 108:
// 1035 "parser.lemon"
{ json_object_put((yypminor->yy235)); }
// 3189 "parser.c"
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
  { 107, 1 },
  { 108, 1 },
  { 109, 2 },
  { 109, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 110, 1 },
  { 111, 3 },
  { 113, 3 },
  { 113, 5 },
  { 112, 3 },
  { 112, 5 },
  { 112, 5 },
  { 112, 7 },
  { 112, 4 },
  { 112, 6 },
  { 112, 6 },
  { 112, 4 },
  { 112, 6 },
  { 117, 2 },
  { 117, 3 },
  { 118, 3 },
  { 118, 1 },
  { 120, 1 },
  { 116, 2 },
  { 116, 3 },
  { 119, 1 },
  { 119, 1 },
  { 119, 1 },
  { 119, 2 },
  { 119, 2 },
  { 119, 2 },
  { 119, 2 },
  { 119, 3 },
  { 119, 3 },
  { 121, 1 },
  { 121, 1 },
  { 121, 2 },
  { 122, 2 },
  { 122, 1 },
  { 126, 4 },
  { 126, 3 },
  { 126, 6 },
  { 126, 5 },
  { 126, 5 },
  { 126, 4 },
  { 126, 7 },
  { 126, 6 },
  { 129, 2 },
  { 129, 3 },
  { 130, 3 },
  { 130, 1 },
  { 131, 1 },
  { 131, 2 },
  { 123, 2 },
  { 123, 1 },
  { 124, 2 },
  { 124, 1 },
  { 125, 2 },
  { 125, 1 },
  { 132, 6 },
  { 132, 5 },
  { 133, 7 },
  { 133, 8 },
  { 133, 8 },
  { 133, 9 },
  { 133, 8 },
  { 133, 9 },
  { 133, 9 },
  { 133, 10 },
  { 133, 9 },
  { 133, 10 },
  { 133, 10 },
  { 133, 11 },
  { 133, 10 },
  { 133, 11 },
  { 133, 11 },
  { 133, 12 },
  { 134, 8 },
  { 134, 9 },
  { 134, 9 },
  { 134, 10 },
  { 134, 6 },
  { 134, 7 },
  { 134, 7 },
  { 134, 8 },
  { 127, 2 },
  { 127, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 138, 1 },
  { 137, 1 },
  { 137, 1 },
  { 139, 3 },
  { 139, 1 },
  { 140, 1 },
  { 140, 2 },
  { 140, 1 },
  { 140, 1 },
  { 135, 3 },
  { 135, 1 },
  { 144, 1 },
  { 144, 2 },
  { 144, 3 },
  { 144, 2 },
  { 144, 3 },
  { 144, 4 },
  { 144, 5 },
  { 144, 4 },
  { 142, 3 },
  { 143, 5 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 141, 1 },
  { 136, 2 },
  { 136, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 145, 1 },
  { 164, 1 },
  { 158, 2 },
  { 159, 2 },
  { 147, 4 },
  { 147, 7 },
  { 147, 5 },
  { 147, 9 },
  { 147, 8 },
  { 147, 8 },
  { 162, 4 },
  { 162, 5 },
  { 166, 2 },
  { 166, 1 },
  { 167, 3 },
  { 167, 4 },
  { 167, 3 },
  { 148, 3 },
  { 148, 4 },
  { 160, 4 },
  { 160, 5 },
  { 161, 6 },
  { 161, 7 },
  { 163, 7 },
  { 163, 8 },
  { 163, 9 },
  { 163, 10 },
  { 146, 3 },
  { 169, 3 },
  { 169, 1 },
  { 171, 1 },
  { 171, 1 },
  { 171, 1 },
  { 171, 1 },
  { 171, 1 },
  { 171, 1 },
  { 170, 3 },
  { 170, 5 },
  { 170, 7 },
  { 170, 7 },
  { 170, 7 },
  { 170, 6 },
  { 170, 5 },
  { 170, 7 },
  { 170, 6 },
  { 170, 5 },
  { 170, 4 },
  { 173, 2 },
  { 173, 1 },
  { 174, 3 },
  { 170, 4 },
  { 170, 4 },
  { 170, 2 },
  { 170, 2 },
  { 175, 1 },
  { 149, 3 },
  { 176, 3 },
  { 176, 1 },
  { 177, 1 },
  { 153, 2 },
  { 152, 2 },
  { 154, 2 },
  { 151, 2 },
  { 150, 3 },
  { 150, 2 },
  { 155, 3 },
  { 156, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 157, 3 },
  { 182, 3 },
  { 182, 1 },
  { 183, 1 },
  { 183, 3 },
  { 172, 1 },
  { 168, 2 },
  { 168, 2 },
  { 168, 2 },
  { 168, 2 },
  { 168, 2 },
  { 168, 2 },
  { 168, 2 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 4 },
  { 168, 4 },
  { 168, 3 },
  { 168, 5 },
  { 168, 5 },
  { 168, 3 },
  { 168, 3 },
  { 168, 4 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 168, 3 },
  { 181, 4 },
  { 168, 1 },
  { 168, 2 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 2 },
  { 168, 3 },
  { 168, 2 },
  { 168, 4 },
  { 168, 5 },
  { 168, 4 },
  { 168, 6 },
  { 168, 7 },
  { 179, 4 },
  { 179, 3 },
  { 179, 6 },
  { 179, 5 },
  { 180, 6 },
  { 180, 5 },
  { 180, 8 },
  { 180, 7 },
  { 180, 10 },
  { 180, 9 },
  { 178, 6 },
  { 178, 5 },
  { 178, 8 },
  { 178, 7 },
  { 178, 8 },
  { 178, 7 },
  { 168, 1 },
  { 168, 1 },
  { 168, 1 },
  { 168, 5 },
  { 185, 3 },
  { 185, 1 },
  { 186, 1 },
  { 184, 3 },
  { 184, 1 },
  { 187, 3 },
  { 187, 1 },
  { 188, 1 },
  { 188, 1 },
  { 188, 1 },
  { 189, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 1 },
  { 128, 3 },
  { 128, 1 },
  { 165, 1 },
  { 114, 1 },
  { 115, 1 },
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
	status->ret = yymsp[0].minor.yy235;
}
// 3735 "parser.c"
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
      case 205:
      case 232:
      case 267:
      case 302:
      case 303:
      case 304:
      case 308:
      case 316:
      case 326:
// 1037 "parser.lemon"
{
	yygotominor.yy235 = yymsp[0].minor.yy235;
}
// 3777 "parser.c"
        break;
      case 2:
      case 40:
      case 56:
      case 58:
      case 60:
      case 88:
      case 129:
      case 163:
      case 198:
// 1041 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(yymsp[-1].minor.yy235, yymsp[0].minor.yy235);
}
// 3792 "parser.c"
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
      case 199:
      case 208:
      case 229:
      case 307:
      case 310:
// 1045 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(NULL, yymsp[0].minor.yy235);
}
// 3816 "parser.c"
        break;
      case 9:
// 1070 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 3825 "parser.c"
        break;
      case 10:
// 1074 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3833 "parser.c"
        break;
      case 11:
// 1078 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3842 "parser.c"
        break;
      case 12:
// 1082 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3850 "parser.c"
        break;
      case 13:
// 1086 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3859 "parser.c"
        break;
      case 14:
// 1090 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 0, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3868 "parser.c"
        break;
      case 15:
// 1094 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy235, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(45,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3878 "parser.c"
        break;
      case 16:
// 1098 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3887 "parser.c"
        break;
      case 17:
// 1102 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3897 "parser.c"
        break;
      case 18:
// 1106 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 1, 0, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3907 "parser.c"
        break;
      case 19:
// 1110 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy235, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3916 "parser.c"
        break;
      case 20:
// 1114 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy235, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3926 "parser.c"
        break;
      case 21:
      case 50:
// 1118 "parser.lemon"
{
	yygotominor.yy235 = NULL;
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3936 "parser.c"
        break;
      case 22:
      case 51:
// 1122 "parser.lemon"
{
	yygotominor.yy235 = yymsp[-1].minor.yy235;
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3946 "parser.c"
        break;
      case 23:
      case 52:
      case 106:
      case 179:
      case 207:
      case 228:
      case 306:
      case 309:
// 1126 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(yymsp[-2].minor.yy235, yymsp[0].minor.yy235);
  yy_destructor(6,&yymsp[-1].minor);
}
// 3961 "parser.c"
        break;
      case 25:
      case 269:
      case 313:
// 1134 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 3970 "parser.c"
        break;
      case 26:
// 1138 "parser.lemon"
{
  yygotominor.yy235 = NULL;
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3979 "parser.c"
        break;
      case 27:
// 1142 "parser.lemon"
{
  yygotominor.yy235 = yymsp[-1].minor.yy235;
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3988 "parser.c"
        break;
      case 28:
// 1146 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
}
// 3995 "parser.c"
        break;
      case 29:
// 1150 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 4002 "parser.c"
        break;
      case 30:
// 1154 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, yymsp[0].minor.yy235, NULL, status->scanner_state);
}
// 4009 "parser.c"
        break;
      case 31:
// 1158 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
}
// 4016 "parser.c"
        break;
      case 32:
// 1162 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 4023 "parser.c"
        break;
      case 33:
// 1166 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[0].minor.yy235, NULL, yymsp[-1].minor.yy235, status->scanner_state);
}
// 4030 "parser.c"
        break;
      case 34:
// 1170 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(NULL, yymsp[0].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
}
// 4037 "parser.c"
        break;
      case 35:
// 1174 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
}
// 4044 "parser.c"
        break;
      case 36:
// 1178 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_definition(yymsp[-1].minor.yy235, yymsp[0].minor.yy235, yymsp[-2].minor.yy235, status->scanner_state);
}
// 4051 "parser.c"
        break;
      case 37:
// 1182 "parser.lemon"
{
  yygotominor.yy235 = xx_ret_interface_definition(NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 4058 "parser.c"
        break;
      case 38:
// 1186 "parser.lemon"
{
  yygotominor.yy235 = xx_ret_interface_definition(yymsp[0].minor.yy235, NULL, status->scanner_state);
}
// 4065 "parser.c"
        break;
      case 39:
// 1190 "parser.lemon"
{
  yygotominor.yy235 = xx_ret_interface_definition(yymsp[0].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
}
// 4072 "parser.c"
        break;
      case 42:
// 1203 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-2].minor.yy235, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4080 "parser.c"
        break;
      case 43:
// 1207 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-2].minor.yy235, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4088 "parser.c"
        break;
      case 44:
// 1211 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-4].minor.yy235, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4097 "parser.c"
        break;
      case 45:
// 1215 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-4].minor.yy235, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4106 "parser.c"
        break;
      case 46:
// 1219 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4114 "parser.c"
        break;
      case 47:
// 1223 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4122 "parser.c"
        break;
      case 48:
// 1227 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-5].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, yymsp[-6].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4131 "parser.c"
        break;
      case 49:
// 1231 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_property(yymsp[-5].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4140 "parser.c"
        break;
      case 54:
// 1251 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4147 "parser.c"
        break;
      case 55:
// 1255 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4154 "parser.c"
        break;
      case 62:
// 1284 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4164 "parser.c"
        break;
      case 63:
// 1288 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4174 "parser.c"
        break;
      case 64:
// 1293 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-6].minor.yy235, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4186 "parser.c"
        break;
      case 65:
// 1297 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, yymsp[-3].minor.yy235, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4198 "parser.c"
        break;
      case 66:
// 1301 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4210 "parser.c"
        break;
      case 67:
// 1305 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy235, yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4222 "parser.c"
        break;
      case 68:
// 1309 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-6].minor.yy235, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4234 "parser.c"
        break;
      case 69:
// 1313 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, yymsp[-3].minor.yy235, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4246 "parser.c"
        break;
      case 70:
// 1317 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy235, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4258 "parser.c"
        break;
      case 71:
// 1321 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy235, yymsp[-1].minor.yy235, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4270 "parser.c"
        break;
      case 72:
// 1325 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4283 "parser.c"
        break;
      case 73:
// 1329 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-9].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy235, NULL, NULL, yymsp[-2].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4296 "parser.c"
        break;
      case 74:
// 1333 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-9].minor.yy235, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy235, NULL, yymsp[-3].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4309 "parser.c"
        break;
      case 75:
// 1337 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-10].minor.yy235, yymsp[-8].minor.yy0, yymsp[-6].minor.yy235, yymsp[-1].minor.yy235, NULL, yymsp[-3].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-9].minor);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4322 "parser.c"
        break;
      case 76:
// 1341 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4335 "parser.c"
        break;
      case 77:
// 1345 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-9].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy235, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4348 "parser.c"
        break;
      case 78:
// 1349 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-9].minor.yy235, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy235, yymsp[-10].minor.yy0, yymsp[-3].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4361 "parser.c"
        break;
      case 79:
// 1353 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-10].minor.yy235, yymsp[-8].minor.yy0, yymsp[-6].minor.yy235, yymsp[-1].minor.yy235, yymsp[-11].minor.yy0, yymsp[-3].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-9].minor);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4374 "parser.c"
        break;
      case 80:
// 1358 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4386 "parser.c"
        break;
      case 81:
// 1362 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy235, NULL, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4398 "parser.c"
        break;
      case 82:
// 1366 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-7].minor.yy235, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4410 "parser.c"
        break;
      case 83:
// 1370 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-8].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy235, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4422 "parser.c"
        break;
      case 84:
// 1374 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-5].minor.yy235, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4433 "parser.c"
        break;
      case 85:
// 1378 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-6].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4444 "parser.c"
        break;
      case 86:
// 1382 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-5].minor.yy235, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4455 "parser.c"
        break;
      case 87:
// 1386 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_class_method(yymsp[-6].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy235, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4466 "parser.c"
        break;
      case 90:
// 1399 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4474 "parser.c"
        break;
      case 91:
// 1403 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4482 "parser.c"
        break;
      case 92:
// 1407 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4490 "parser.c"
        break;
      case 93:
// 1411 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4498 "parser.c"
        break;
      case 94:
// 1415 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4506 "parser.c"
        break;
      case 95:
// 1419 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("inline");
  yy_destructor(57,&yymsp[0].minor);
}
// 4514 "parser.c"
        break;
      case 96:
// 1423 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("abstract");
  yy_destructor(47,&yymsp[0].minor);
}
// 4522 "parser.c"
        break;
      case 97:
// 1427 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("final");
  yy_destructor(48,&yymsp[0].minor);
}
// 4530 "parser.c"
        break;
      case 98:
// 1432 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[0].minor);
}
// 4538 "parser.c"
        break;
      case 99:
// 1436 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type(0, yymsp[0].minor.yy235, status->scanner_state);
}
// 4545 "parser.c"
        break;
      case 100:
// 1440 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_list(yymsp[-2].minor.yy235, yymsp[0].minor.yy235);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4553 "parser.c"
        break;
      case 102:
// 1448 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(yymsp[0].minor.yy235, NULL, 0, 0, status->scanner_state);
}
// 4560 "parser.c"
        break;
      case 103:
// 1452 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(yymsp[-1].minor.yy235, NULL, 1, 0, status->scanner_state);
  yy_destructor(36,&yymsp[0].minor);
}
// 4568 "parser.c"
        break;
      case 104:
// 1456 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy235, 0, 0, status->scanner_state);
}
// 4575 "parser.c"
        break;
      case 105:
// 1460 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy235, 0, 1, status->scanner_state);
}
// 4582 "parser.c"
        break;
      case 108:
// 1473 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4589 "parser.c"
        break;
      case 109:
// 1477 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-1].minor.yy235, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4596 "parser.c"
        break;
      case 110:
// 1481 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-2].minor.yy235, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 4604 "parser.c"
        break;
      case 111:
// 1485 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, yymsp[-1].minor.yy235, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4611 "parser.c"
        break;
      case 112:
// 1489 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4619 "parser.c"
        break;
      case 113:
// 1493 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-3].minor.yy235, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4627 "parser.c"
        break;
      case 114:
// 1497 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(yymsp[-4].minor.yy235, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 1, status->scanner_state);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4636 "parser.c"
        break;
      case 115:
// 1501 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_parameter(NULL, yymsp[-3].minor.yy235, yymsp[-2].minor.yy0, yymsp[0].minor.yy235, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4644 "parser.c"
        break;
      case 116:
// 1505 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 4653 "parser.c"
        break;
      case 117:
// 1509 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 4664 "parser.c"
        break;
      case 118:
// 1513 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4672 "parser.c"
        break;
      case 119:
// 1517 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(61,&yymsp[0].minor);
}
// 4680 "parser.c"
        break;
      case 120:
// 1521 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4688 "parser.c"
        break;
      case 121:
// 1525 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(63,&yymsp[0].minor);
}
// 4696 "parser.c"
        break;
      case 122:
// 1529 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4704 "parser.c"
        break;
      case 123:
// 1533 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(65,&yymsp[0].minor);
}
// 4712 "parser.c"
        break;
      case 124:
// 1537 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(66,&yymsp[0].minor);
}
// 4720 "parser.c"
        break;
      case 125:
// 1541 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(67,&yymsp[0].minor);
}
// 4728 "parser.c"
        break;
      case 126:
// 1545 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(68,&yymsp[0].minor);
}
// 4736 "parser.c"
        break;
      case 127:
// 1549 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(69,&yymsp[0].minor);
}
// 4744 "parser.c"
        break;
      case 128:
// 1553 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(70,&yymsp[0].minor);
}
// 4752 "parser.c"
        break;
      case 152:
// 1651 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4760 "parser.c"
        break;
      case 153:
// 1655 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4769 "parser.c"
        break;
      case 154:
// 1659 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(72,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4778 "parser.c"
        break;
      case 155:
// 1663 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-2].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4788 "parser.c"
        break;
      case 156:
// 1667 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-5].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4801 "parser.c"
        break;
      case 157:
// 1671 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4811 "parser.c"
        break;
      case 158:
// 1675 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-7].minor.yy235, yymsp[-5].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(73,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4824 "parser.c"
        break;
      case 159:
// 1679 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-6].minor.yy235, yymsp[-4].minor.yy235, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4837 "parser.c"
        break;
      case 160:
// 1683 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_if_statement(yymsp[-6].minor.yy235, NULL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4850 "parser.c"
        break;
      case 161:
// 1687 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_switch_statement(yymsp[-2].minor.yy235, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4860 "parser.c"
        break;
      case 162:
// 1691 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_switch_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4870 "parser.c"
        break;
      case 165:
// 1703 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(77,&yymsp[0].minor);
}
// 4879 "parser.c"
        break;
      case 166:
// 1707 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(76,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 4888 "parser.c"
        break;
      case 167:
// 1711 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_case_clause(NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 4897 "parser.c"
        break;
      case 168:
// 1715 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4907 "parser.c"
        break;
      case 169:
// 1719 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_loop_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4917 "parser.c"
        break;
      case 170:
// 1723 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_while_statement(yymsp[-2].minor.yy235, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4927 "parser.c"
        break;
      case 171:
// 1727 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_while_statement(yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4937 "parser.c"
        break;
      case 172:
// 1731 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_do_while_statement(yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4949 "parser.c"
        break;
      case 173:
// 1735 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_do_while_statement(yymsp[-1].minor.yy235, yymsp[-4].minor.yy235, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4961 "parser.c"
        break;
      case 174:
// 1739 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_for_statement(yymsp[-3].minor.yy235, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4972 "parser.c"
        break;
      case 175:
// 1743 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_for_statement(yymsp[-3].minor.yy235, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(82,&yymsp[-7].minor);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4984 "parser.c"
        break;
      case 176:
// 1747 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_for_statement(yymsp[-3].minor.yy235, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(82,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4996 "parser.c"
        break;
      case 177:
// 1751 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_for_statement(yymsp[-3].minor.yy235, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(82,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5009 "parser.c"
        break;
      case 178:
// 1755 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(85,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5018 "parser.c"
        break;
      case 181:
// 1768 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("assign");
  yy_destructor(52,&yymsp[0].minor);
}
// 5026 "parser.c"
        break;
      case 182:
// 1773 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("add-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 5034 "parser.c"
        break;
      case 183:
// 1778 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("sub-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 5042 "parser.c"
        break;
      case 184:
// 1782 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("mult-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 5050 "parser.c"
        break;
      case 185:
// 1786 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("div-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5058 "parser.c"
        break;
      case 186:
// 1790 "parser.lemon"
{
	yygotominor.yy235 = json_object_new_string("concat-assign");
  yy_destructor(90,&yymsp[0].minor);
}
// 5066 "parser.c"
        break;
      case 187:
// 1795 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy235, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 5073 "parser.c"
        break;
      case 188:
// 1800 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
}
// 5081 "parser.c"
        break;
      case 189:
// 1805 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 5091 "parser.c"
        break;
      case 190:
// 1810 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 5101 "parser.c"
        break;
      case 191:
// 1815 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5111 "parser.c"
        break;
      case 192:
// 1820 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
}
// 5119 "parser.c"
        break;
      case 193:
// 1825 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(92,&yymsp[-3].minor);
}
// 5127 "parser.c"
        break;
      case 194:
// 1830 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy235, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(92,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5137 "parser.c"
        break;
      case 195:
// 1835 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy235, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(92,&yymsp[-4].minor);
}
// 5145 "parser.c"
        break;
      case 196:
// 1840 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy235, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5154 "parser.c"
        break;
      case 197:
// 1845 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy235, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
}
// 5161 "parser.c"
        break;
      case 200:
// 1857 "parser.lemon"
{
	yygotominor.yy235 = yymsp[-1].minor.yy235;
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5170 "parser.c"
        break;
      case 201:
// 1862 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(93,&yymsp[0].minor);
}
// 5179 "parser.c"
        break;
      case 202:
// 1867 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(94,&yymsp[0].minor);
}
// 5188 "parser.c"
        break;
      case 203:
// 1872 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[0].minor);
}
// 5196 "parser.c"
        break;
      case 204:
// 1877 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 5204 "parser.c"
        break;
      case 206:
// 1885 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_echo_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(95,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5213 "parser.c"
        break;
      case 209:
// 1897 "parser.lemon"
{
	yygotominor.yy235 = yymsp[0].minor.yy235;;
}
// 5220 "parser.c"
        break;
      case 210:
// 1902 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5228 "parser.c"
        break;
      case 211:
// 1907 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5236 "parser.c"
        break;
      case 212:
// 1912 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5244 "parser.c"
        break;
      case 213:
// 1917 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fetch_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5252 "parser.c"
        break;
      case 214:
// 1922 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(96,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5261 "parser.c"
        break;
      case 215:
// 1927 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(96,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5270 "parser.c"
        break;
      case 216:
// 1932 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_unset_statement(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5279 "parser.c"
        break;
      case 217:
// 1937 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_throw_exception(yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(98,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5288 "parser.c"
        break;
      case 218:
// 1941 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5297 "parser.c"
        break;
      case 219:
// 1945 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5306 "parser.c"
        break;
      case 220:
// 1949 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5315 "parser.c"
        break;
      case 221:
// 1953 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5324 "parser.c"
        break;
      case 222:
// 1957 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5333 "parser.c"
        break;
      case 223:
// 1961 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5342 "parser.c"
        break;
      case 224:
// 1965 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5351 "parser.c"
        break;
      case 225:
// 1969 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5360 "parser.c"
        break;
      case 226:
// 1973 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5369 "parser.c"
        break;
      case 227:
// 1977 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5378 "parser.c"
        break;
      case 230:
// 1989 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5385 "parser.c"
        break;
      case 231:
// 1993 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 5393 "parser.c"
        break;
      case 233:
// 2001 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5401 "parser.c"
        break;
      case 234:
// 2005 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("isset", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
}
// 5409 "parser.c"
        break;
      case 235:
// 2009 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("require", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5417 "parser.c"
        break;
      case 236:
// 2013 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("clone", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5425 "parser.c"
        break;
      case 237:
// 2017 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("empty", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5433 "parser.c"
        break;
      case 238:
// 2021 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("likely", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5441 "parser.c"
        break;
      case 239:
// 2025 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("unlikely", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5449 "parser.c"
        break;
      case 240:
// 2029 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("equals", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5457 "parser.c"
        break;
      case 241:
// 2033 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not-equals", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5465 "parser.c"
        break;
      case 242:
// 2037 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("identical", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5473 "parser.c"
        break;
      case 243:
// 2041 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("not-identical", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5481 "parser.c"
        break;
      case 244:
// 2045 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("less", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5489 "parser.c"
        break;
      case 245:
// 2049 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("greater", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5497 "parser.c"
        break;
      case 246:
// 2053 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("less-equal", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5505 "parser.c"
        break;
      case 247:
// 2057 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5513 "parser.c"
        break;
      case 248:
// 2061 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("list", yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5522 "parser.c"
        break;
      case 249:
// 2065 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("cast", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
}
// 5531 "parser.c"
        break;
      case 250:
// 2069 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5540 "parser.c"
        break;
      case 251:
// 2073 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-access", yymsp[-2].minor.yy235, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5548 "parser.c"
        break;
      case 252:
// 2077 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy235, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5558 "parser.c"
        break;
      case 253:
// 2081 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy235, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5568 "parser.c"
        break;
      case 254:
// 2085 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(92,&yymsp[-1].minor);
}
// 5576 "parser.c"
        break;
      case 255:
      case 324:
// 2089 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(92,&yymsp[-1].minor);
}
// 5585 "parser.c"
        break;
      case 256:
// 2098 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("array-access", yymsp[-3].minor.yy235, yymsp[-1].minor.yy235, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5594 "parser.c"
        break;
      case 257:
// 2103 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("add", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5602 "parser.c"
        break;
      case 258:
// 2108 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("sub", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5610 "parser.c"
        break;
      case 259:
// 2113 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("mul", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5618 "parser.c"
        break;
      case 260:
// 2118 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("div", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5626 "parser.c"
        break;
      case 261:
// 2123 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("mod", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5634 "parser.c"
        break;
      case 262:
// 2128 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("concat", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5642 "parser.c"
        break;
      case 263:
// 2133 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("and", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5650 "parser.c"
        break;
      case 264:
// 2138 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("or", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 5658 "parser.c"
        break;
      case 265:
// 2143 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("instanceof", yymsp[-2].minor.yy235, yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5666 "parser.c"
        break;
      case 266:
// 2148 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy235, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5675 "parser.c"
        break;
      case 268:
// 2158 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("typeof", yymsp[0].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5683 "parser.c"
        break;
      case 270:
      case 315:
      case 317:
// 2168 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5692 "parser.c"
        break;
      case 271:
      case 314:
      case 319:
// 2173 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5701 "parser.c"
        break;
      case 272:
      case 318:
// 2178 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5709 "parser.c"
        break;
      case 273:
      case 320:
// 2183 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5717 "parser.c"
        break;
      case 274:
      case 321:
// 2188 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(102,&yymsp[0].minor);
}
// 5726 "parser.c"
        break;
      case 275:
      case 323:
// 2193 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5735 "parser.c"
        break;
      case 276:
      case 322:
// 2198 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(104,&yymsp[0].minor);
}
// 5744 "parser.c"
        break;
      case 277:
      case 325:
// 2203 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5752 "parser.c"
        break;
      case 278:
// 2208 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5761 "parser.c"
        break;
      case 279:
// 2213 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("array", yymsp[-1].minor.yy235, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5770 "parser.c"
        break;
      case 280:
// 2218 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5778 "parser.c"
        break;
      case 281:
// 2223 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5788 "parser.c"
        break;
      case 282:
// 2228 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5798 "parser.c"
        break;
      case 283:
// 2233 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5808 "parser.c"
        break;
      case 284:
// 2238 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5820 "parser.c"
        break;
      case 285:
// 2243 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(35,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5832 "parser.c"
        break;
      case 286:
// 2248 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5841 "parser.c"
        break;
      case 287:
// 2253 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5850 "parser.c"
        break;
      case 288:
// 2258 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5861 "parser.c"
        break;
      case 289:
// 2263 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5872 "parser.c"
        break;
      case 290:
// 2268 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(92,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5882 "parser.c"
        break;
      case 291:
// 2273 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(92,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5892 "parser.c"
        break;
      case 292:
// 2278 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(92,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5904 "parser.c"
        break;
      case 293:
// 2283 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(92,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5916 "parser.c"
        break;
      case 294:
// 2288 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(49,&yymsp[-9].minor);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(92,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5930 "parser.c"
        break;
      case 295:
// 2293 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-8].minor);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(92,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5944 "parser.c"
        break;
      case 296:
// 2298 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(1, yymsp[-5].minor.yy235, yymsp[-3].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5954 "parser.c"
        break;
      case 297:
// 2303 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(1, yymsp[-4].minor.yy235, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5964 "parser.c"
        break;
      case 298:
// 2308 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(2, yymsp[-7].minor.yy235, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5976 "parser.c"
        break;
      case 299:
// 2313 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(2, yymsp[-6].minor.yy235, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5988 "parser.c"
        break;
      case 300:
// 2318 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(3, yymsp[-7].minor.yy235, yymsp[-4].minor.yy0, yymsp[-1].minor.yy235, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6000 "parser.c"
        break;
      case 301:
// 2323 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_mcall(3, yymsp[-6].minor.yy235, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6012 "parser.c"
        break;
      case 305:
// 2343 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_expr("ternary", yymsp[-4].minor.yy235, yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 6021 "parser.c"
        break;
      case 311:
// 2367 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_array_item(yymsp[-2].minor.yy235, yymsp[0].minor.yy235, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 6029 "parser.c"
        break;
      case 312:
// 2371 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_array_item(NULL, yymsp[0].minor.yy235, status->scanner_state);
}
// 6036 "parser.c"
        break;
      case 327:
// 2431 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6043 "parser.c"
        break;
      case 328:
// 2435 "parser.lemon"
{
	yygotominor.yy235 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 6050 "parser.c"
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

// 6126 "parser.c"
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
