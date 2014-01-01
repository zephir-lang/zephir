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

static json_object *xx_ret_require_statement(json_object *expr, xx_scanner_state *state)
{
	json_object *ret = json_object_new_object();

	json_object_object_add(ret, "type", json_object_new_string("require"));
	json_object_object_add(ret, "expr", expr);

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


// 957 "parser.c"
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
#define YYNOCODE 193
#define YYACTIONTYPE unsigned short int
#define xx_TOKENTYPE xx_parser_token*
typedef union {
  xx_TOKENTYPE yy0;
  json_object* yy31;
  int yy385;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define xx_ARG_SDECL xx_parser_status *status;
#define xx_ARG_PDECL ,xx_parser_status *status
#define xx_ARG_FETCH xx_parser_status *status = yypParser->status
#define xx_ARG_STORE yypParser->status = status
#define YYNSTATE 689
#define YYNRULE 334
#define YYERRORSYMBOL 107
#define YYERRSYMDT yy385
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
 /*     0 */   492,  314,  244,  247,  238,  241,  260,  262,  270,  264,
 /*    10 */   266,  268,   10,  288,  264,  266,  268,   82,  257,  253,
 /*    20 */    78,  109, 1006,  257,  253,  232,  296,  301,  315,  229,
 /*    30 */   689,  311,  124,   81,  292,  188,  621,  154,  293,  625,
 /*    40 */   647,   25,  322,  162,  538,  139,  623,  310,   24,  282,
 /*    50 */    11,  124,  124,  501,  510,  519,  522,  513,  516,  525,
 /*    60 */   531,  528,   13,  534,  189,  191,  193,  416,  202,  462,
 /*    70 */   435,   73,  213,  217,  222,  396,  406,  641,  403,   71,
 /*    80 */    72,   74,   75,   77,   76,  304,  636,  642,  294,  473,
 /*    90 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   100 */   492,  372,  244,  247,  238,  241,    3,    4,    5,    6,
 /*   110 */     7,    8,  405,  288, 1024,    1,    2,  688,    4,    5,
 /*   120 */     6,    7,    8,  629,  205,  232,  296,  301,  315,  229,
 /*   130 */   635,  311,   60,  630,  292,  188,   14,  622,   84,   92,
 /*   140 */   156,   98,  322,  164,  538,  125,  623,  310,  362,  282,
 /*   150 */   209,  124,  564,  501,  510,  519,  522,  513,  516,  525,
 /*   160 */   531,  528,  632,  534,  189,  191,  193,  423,  202,  635,
 /*   170 */   118,   43,  213,  217,  222,  396,   84,   92,  403,   98,
 /*   180 */    66,  409,   15,   84,   92,  304,   67,   18,   44,  473,
 /*   190 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   200 */   492,  361,  244,  247,  238,  241,   36,   37,   39,   40,
 /*   210 */    66,   61,  575,  288,  652,  410,  411,  412,  413,  414,
 /*   220 */   415,  635,  161,  436,  437,  232,  296,  301,  315,  229,
 /*   230 */   421,  311,  430,  435,  292,  188,  659,  664,  625,  647,
 /*   240 */   156,  119,  322,  569,  538,  623,  155,  310,   16,  282,
 /*   250 */   124,  124,  587,  501,  510,  519,  522,  513,  516,  525,
 /*   260 */   531,  528,  583,  534,  189,  191,  193,  434,  202,  649,
 /*   270 */   257,  253,  213,  217,  222,  396,  635,  581,  403,  650,
 /*   280 */   149,  409,   88,   84,   92,  304,   98,   85,   18,  473,
 /*   290 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   300 */   492,  321,  244,  247,  238,  241,   31,   17,   34,   58,
 /*   310 */    37,   39,   40,  288,  314,  410,  411,  412,  413,  414,
 /*   320 */   415,  206,  567,  603,  207,  232,  296,  301,  315,  229,
 /*   330 */   609,  311,  320,  599,  292,  188,   84,   92,  449,   98,
 /*   340 */   455,  435,  322,  197,  538,   30,  321,  310,   59,  282,
 /*   350 */   209,  293,  564,  501,  510,  519,  522,  513,  516,  525,
 /*   360 */   531,  528,  254,  534,  189,  191,  193,  451,  202,  147,
 /*   370 */   376,  667,  213,  217,  222,  396,   62,  327,  403,   65,
 /*   380 */   114,  409,  143,  117,  668,  304,  325,  149,   99,  473,
 /*   390 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   400 */   492,   21,  244,  247,  238,  241,   63,  321,   34,   58,
 /*   410 */    37,   39,   40,  288,  316,  410,  411,  412,  413,  414,
 /*   420 */   415,  120,  347,  333,  123,  232,  296,  301,  315,  229,
 /*   430 */   146,  311,  334,  149,  292,  188,  321,  377,  337,   23,
 /*   440 */   339,   25,  322,  568,  538,  321,  125,  310,  126,  282,
 /*   450 */   404,  472,  124,  501,  510,  519,  522,  513,  516,  525,
 /*   460 */   531,  528,  546,  534,  189,  191,  193,  344,  202,  256,
 /*   470 */   321,   28,  213,  217,  222,  396,  353,  321,  403,  329,
 /*   480 */   331,  330,  300,   27,  321,  304,  373,  332,  383,  473,
 /*   490 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   500 */   492,  355,  244,  247,  238,  241,  256,  321,  370,  675,
 /*   510 */   321,  678,   32,  288,  149,  374,  329,  331,  330,  300,
 /*   520 */   439,   35,  408,  369,  332,  232,  296,  301,  315,  229,
 /*   530 */   467,  311,  420,  431,  292,  188,  433,   42,  381,  398,
 /*   540 */   438,  387,  322,  201,  538,  456,   66,  310,  433,  282,
 /*   550 */   502,  509,  476,  501,  510,  519,  522,  513,  516,  525,
 /*   560 */   531,  528,   38,  534,  189,  191,  193,  463,  202,  256,
 /*   570 */   433,  443,  213,  217,  222,  396,  504,  582,  403,  329,
 /*   580 */   331,  330,  300,  637,  644,  304,  354,  332,  475,  473,
 /*   590 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   600 */   492,  504,  244,  247,  238,  241,  256,  511,  509,  514,
 /*   610 */   509,   45,  503,  288,  504,  504,  329,  331,  330,  300,
 /*   620 */   615,  517,  509,  319,  332,  232,  296,  301,  315,  229,
 /*   630 */   611,  311,  520,  509,  292,  188,   66,  512,  523,  509,
 /*   640 */   504,  504,  322,  563,  538,  526,  509,  310,  504,  282,
 /*   650 */   515,  518,  504,  501,  510,  519,  522,  513,  516,  525,
 /*   660 */   531,  528,   46,  534,  189,  191,  193,  610,  202,  256,
 /*   670 */   529,  509,  213,  217,  222,  396,  521,  524,  403,  329,
 /*   680 */   331,  330,  300,   69,  527,  304,  326,  332,  530,  473,
 /*   690 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   700 */   492,  504,  244,  247,  238,  241,  256,  532,  509,  535,
 /*   710 */   509,  594,  504,  288,   64,  685,  329,  331,  330,  300,
 /*   720 */   149,   70,  124,  336,  332,  232,  296,  301,  315,  229,
 /*   730 */   627,  311,  639,   79,  292,  188,  669,  533,  667,   80,
 /*   740 */    83,  124,  322,  216,  538,   87,   86,  310,  536,  282,
 /*   750 */    89,   91,  641,  501,  510,  519,  522,  513,  516,  525,
 /*   760 */   531,  528,  642,  534,  189,  191,  193,   90,  202,  256,
 /*   770 */    93,   94,  213,  217,  222,  396,  638,   95,  403,  329,
 /*   780 */   331,  330,  300,   97,   43,  304,  343,  332,  111,  473,
 /*   790 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   800 */   492,  112,  244,  247,  238,  241,  256,  679,  116,  667,
 /*   810 */   122,  129,  130,  288,  132,  131,  329,  331,  330,  300,
 /*   820 */   136,  134,  135,  352,  332,  232,  296,  301,  315,  229,
 /*   830 */   137,  311,  141,  144,  292,  188,  142,  666,  145,  148,
 /*   840 */   152,  158,  322,  562,  538,  159,  190,  310,  192,  282,
 /*   850 */   198,  195,  199,  501,  510,  519,  522,  513,  516,  525,
 /*   860 */   531,  528,  204,  534,  189,  191,  193,  208,  202,  256,
 /*   870 */   210,  211,  213,  217,  222,  396,  214,  219,  403,  329,
 /*   880 */   331,  330,  300,  223,  228,  304,  380,  332,  226,  473,
 /*   890 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*   900 */   492,  255,  244,  247,  238,  241,  256,  286,  289,  290,
 /*   910 */   295,  298,  297,  288,  317,  323,  329,  331,  330,  300,
 /*   920 */   335,  340,  324,  386,  332,  232,  296,  301,  315,  229,
 /*   930 */   348,  311,  341,  342,  292,  188,  349,  350, 1008, 1007,
 /*   940 */   365,  378,  322,  221,  538,  384,  379,  310,  385,  282,
 /*   950 */   397,  407,  440,  501,  510,  519,  522,  513,  516,  525,
 /*   960 */   531,  528,  428,  534,  189,  191,  193,  444,  202,  469,
 /*   970 */   425,  230,  213,  217,  222,  396,  448,  468,  403,  481,
 /*   980 */   409,  329,  331,  330,  300,  304,  483,  441,  445,  473,
 /*   990 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1000 */   492,  485,  244,  247,  238,  241,  453,  487,  460,  470,
 /*  1010 */   506,  505,  507,  288,  410,  411,  412,  413,  414,  415,
 /*  1020 */   547,  548,  559,  508,  561,  232,  296,  301,  315,  229,
 /*  1030 */   565,  311,  570,  571,  292,  188,  577,  589,  596,  597,
 /*  1040 */   605,  617,  322,  558,  538,  645,  633,  310,  634,  282,
 /*  1050 */   631,  739,  740,  501,  510,  519,  522,  513,  516,  525,
 /*  1060 */   531,  528,  640,  534,  189,  191,  193,  643,  202,  646,
 /*  1070 */   655,  478,  213,  217,  222,  396,  651,  654,  403,  474,
 /*  1080 */   479,  329,  331,  330,  300,  304,  653,  656,  665,  473,
 /*  1090 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1100 */   492,  672,  244,  247,  238,  241,  673,  670,  674,  676,
 /*  1110 */   149,  682,  115,  288,   34,   58,   37,   39,   40,  677,
 /*  1120 */   680,  683,  684,  686,  528,  232,  296,  301,  315,  229,
 /*  1130 */   687,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1140 */   528,  528,  322,  225,  538,  528,  528,  310,  528,  282,
 /*  1150 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1160 */   531,  528,  528,  534,  189,  191,  193,  194,  202,  528,
 /*  1170 */   395,  528,  213,  217,  222,  396,  528,  528,  403,  528,
 /*  1180 */   329,  331,  330,  300,  528,  304,  528,  528,  528,  473,
 /*  1190 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1200 */   492,  528,  244,  247,  238,  241,  658,  528,  661,  528,
 /*  1210 */   662,  647,  528,  288,  528,  528,  125,  623,  528,  528,
 /*  1220 */   528,  528,  124,  528,  528,  232,  296,  301,  315,  229,
 /*  1230 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1240 */   528,  528,  322,  402,  538,  528,  528,  310,  528,  282,
 /*  1250 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1260 */   531,  528,  528,  534,  189,  191,  193,  203,  202,  528,
 /*  1270 */   395,  528,  213,  217,  222,  396,  528,  528,  403,  528,
 /*  1280 */   329,  331,  330,  300,  528,  304,  528,  528,  528,  473,
 /*  1290 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1300 */   492,  528,  244,  247,  238,  241,  528,  528,  528,  528,
 /*  1310 */   528,  528,  121,  288,   34,   58,   37,   39,   40,  528,
 /*  1320 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  1330 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1340 */   528,  528,  322,  545,  538,  528,  528,  310,  528,  282,
 /*  1350 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1360 */   531,  528,  528,  534,  189,  191,  193,  218,  202,  528,
 /*  1370 */   395,  528,  213,  217,  222,  396,  528,  528,  403,  528,
 /*  1380 */   329,  331,  330,  300,  528,  304,  528,  528,  528,  473,
 /*  1390 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1400 */   492,  528,  244,  247,  238,  241,  256,  528,  528,  528,
 /*  1410 */   528,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  1420 */   528,  528,  528,  528,  346,  232,  296,  301,  315,  229,
 /*  1430 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1440 */   528,  528,  322,  552,  538,  528,  528,  310,  528,  282,
 /*  1450 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1460 */   531,  528,  528,  534,  189,  191,  193,  227,  202,  528,
 /*  1470 */   395,  528,  213,  217,  222,  396,  528,  528,  403,  528,
 /*  1480 */   329,  331,  330,  300,  528,  304,  528,  528,  528,  473,
 /*  1490 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1500 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  1510 */   417,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  1520 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  1530 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1540 */   528,  285,  322,  557,  538,  528,  528,  310,  528,  282,
 /*  1550 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1560 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  283,
 /*  1570 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  1580 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  1590 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1600 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  1610 */   422,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  1620 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  1630 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1640 */   528,  528,  322,  574,  538,  528,  528,  310,  528,  282,
 /*  1650 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1660 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  429,
 /*  1670 */   528,  528,  213,  217,  222,  396,  427,  528,  403,  329,
 /*  1680 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  1690 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1700 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  1710 */   426,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  1720 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  1730 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1740 */   528,  528,  322,  573,  538,  528,  528,  310,  528,  282,
 /*  1750 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1760 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  528,
 /*  1770 */   478,  528,  213,  217,  222,  396,  528,  528,  403,  477,
 /*  1780 */   329,  331,  330,  300,  528,  304,  528,  528,  528,  473,
 /*  1790 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1800 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  1810 */   432,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  1820 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  1830 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1840 */   528,  528,  322,  578,  538,  528,  528,  310,  528,  282,
 /*  1850 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1860 */   531,  528,  528,  534,  189,  191,  193,  560,  202,  528,
 /*  1870 */   395,  528,  213,  217,  222,  396,  528,  528,  403,  528,
 /*  1880 */   329,  331,  330,  300,  528,  304,  528,  528,  528,  473,
 /*  1890 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  1900 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  1910 */   442,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  1920 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  1930 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  1940 */   528,  528,  322,  580,  538,  528,  528,  310,  528,  282,
 /*  1950 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  1960 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  394,
 /*  1970 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  1980 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  1990 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2000 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  2010 */   446,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  2020 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2030 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2040 */   528,  528,  322,  584,  538,  528,  528,  310,  528,  282,
 /*  2050 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2060 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  233,
 /*  2070 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2080 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2090 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2100 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  2110 */   450,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  2120 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2130 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2140 */   528,  528,  322,  586,  538,  528,  528,  310,  528,  282,
 /*  2150 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2160 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  393,
 /*  2170 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2180 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2190 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2200 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  2210 */   454,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  2220 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2230 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2240 */   528,  528,  322,  590,  538,  528,  528,  310,  528,  282,
 /*  2250 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2260 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  236,
 /*  2270 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2280 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2290 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2300 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  2310 */   457,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  2320 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2330 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2340 */   528,  528,  322,  592,  538,  528,  528,  310,  528,  282,
 /*  2350 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2360 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  392,
 /*  2370 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2380 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2390 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2400 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  2410 */   461,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  2420 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2430 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2440 */   528,  528,  322,  600,  538,  528,  528,  310,  528,  282,
 /*  2450 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2460 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  239,
 /*  2470 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2480 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2490 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2500 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  2510 */   464,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  2520 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2530 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2540 */   528,  528,  322,  602,  538,  528,  528,  310,  528,  282,
 /*  2550 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2560 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  391,
 /*  2570 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2580 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2590 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2600 */   492,  528,  244,  247,  238,  241,  418,  528,  528,  528,
 /*  2610 */   471,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  2620 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2630 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2640 */   528,  528,  322,  606,  538,  528,  528,  310,  528,  282,
 /*  2650 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2660 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  242,
 /*  2670 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2680 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2690 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2700 */   492,  528,  244,  247,  238,  241,  528,  528,  528,  528,
 /*  2710 */   528,  528,  576,  288,   34,   58,   37,   39,   40,  528,
 /*  2720 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2730 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2740 */   528,  528,  322,  608,  538,  528,  528,  310,  528,  282,
 /*  2750 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2760 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  390,
 /*  2770 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2780 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2790 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2800 */   492,  528,  244,  247,  238,  241,  528,  528,  528,  528,
 /*  2810 */   528,  528,  588,  288,   34,   58,   37,   39,   40,  528,
 /*  2820 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2830 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2840 */   528,  528,  322,  612,  538,  528,  528,  310,  528,  282,
 /*  2850 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2860 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  245,
 /*  2870 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2880 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2890 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  2900 */   492,  528,  244,  247,  238,  241,  528,  528,  528,  528,
 /*  2910 */   528,  528,  604,  288,   34,   58,   37,   39,   40,  528,
 /*  2920 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  2930 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  2940 */   528,  528,  322,  614,  538,  528,  528,  310,  528,  282,
 /*  2950 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  2960 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  389,
 /*  2970 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  2980 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  2990 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  3000 */   492,  528,  244,  247,  238,  241,  528,  528,  528,  528,
 /*  3010 */   528,  528,  616,  288,   34,   58,   37,   39,   40,  528,
 /*  3020 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  3030 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  3040 */   528,  528,  322,  618,  538,  528,  528,  310,  528,  282,
 /*  3050 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  3060 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  248,
 /*  3070 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  3080 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  3090 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  3100 */   492,  528,  244,  247,  238,  241,  250,  528,  528,  528,
 /*  3110 */   528,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  3120 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  3130 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  3140 */   528,  528,  322,  620,  538,  528,  528,  310,  528,  282,
 /*  3150 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  3160 */   531,  528,  528,  534,  189,  191,  193,  528,  202,  252,
 /*  3170 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  3180 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  3190 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  3200 */   492,  528,  244,  247,  238,  241,  258,  528,  528,  528,
 /*  3210 */   528,  528,  528,  288,  528,  528,  329,  331,  330,  300,
 /*  3220 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  3230 */   528,  311,  528,  528,  292,  188,  528,  528,  528,  528,
 /*  3240 */   528,  528,  322,  528,  538,  528,  528,  310,  528,  282,
 /*  3250 */   528,  528,  528,  501,  510,  519,  522,  513,  516,  525,
 /*  3260 */   531,  528,  528,  534,  189,  191,  193,  690,  202,  261,
 /*  3270 */   528,  528,  213,  217,  222,  396,  528,  528,  403,  329,
 /*  3280 */   331,  330,  300,  528,  528,  304,  528,  528,  528,  473,
 /*  3290 */   488,  495,  498,  303,  305,  306,  307,  308,  309,  539,
 /*  3300 */   235,  528,  244,  247,  238,  241,  528,    9,  528,  528,
 /*  3310 */    12,  528,  140,  288,  671,  681,  528,  528,  538,  528,
 /*  3320 */   528,  528,  528,  528,  528,  232,  296,  301,  315,  229,
 /*  3330 */   528,  311,  528,  528,  292,  528,  100,  101,  103,  102,
 /*  3340 */   104,  528,  322,  528,  528,  528,  528,  310,  528,  282,
 /*  3350 */   528,  528,  528,   47,   48,   49,   50,   51,   52,   53,
 /*  3360 */    54,   55,   56,   57,  528,  528,  528,  528,  312,  186,
 /*  3370 */   166,  528,  528,  539,  528,  528,  628,  528,  329,  331,
 /*  3380 */   330,  300,  106,  107,  360,  304,  528,  368,  364,  367,
 /*  3390 */   595,  163,  105,  303,  305,  306,  307,  308,  309,  528,
 /*  3400 */   540,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  3410 */   176,  177,  178,  179,  180,  181,  182,  183,  184,  185,
 /*  3420 */   187,  186,  166,  528,  537,  528,  528,  528,  528,  528,
 /*  3430 */   528,  528,  528,  528,  480,  482,  484,  486,  100,  101,
 /*  3440 */   103,  102,  104,  196,  528,  528,  528,  528,  528,  528,
 /*  3450 */   528,  528,  540,  167,  168,  169,  170,  171,  172,  173,
 /*  3460 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  183,
 /*  3470 */   184,  185,  187,  186,  166,  528,  537,  528,  648,  528,
 /*  3480 */   528,  528,  528,  528,  106,  107,  480,  482,  484,  486,
 /*  3490 */   528,  528,  157,  528,  105,  200,  100,  101,  103,  102,
 /*  3500 */   104,  528,  528,  528,  540,  167,  168,  169,  170,  171,
 /*  3510 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
 /*  3520 */   182,  183,  184,  185,  187,  186,  166,  528,  537,  528,
 /*  3530 */   528,  528,  528,  528,  528,  528,  528,  528,  480,  482,
 /*  3540 */   484,  486,  106,  107,  528,  722,  660,  212,  528,  528,
 /*  3550 */   528,  528,  105,  528,  528,  528,  540,  167,  168,  169,
 /*  3560 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  3570 */   180,  181,  182,  183,  184,  185,  187,  186,  166,  528,
 /*  3580 */   537,  528,  528,  528,  528,  528,  528,  528,  528,  528,
 /*  3590 */   480,  482,  484,  486,  100,  101,  103,  102,  104,  215,
 /*  3600 */   100,  101,  103,  102,  104,  528,  528,  528,  540,  167,
 /*  3610 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  3620 */   178,  179,  180,  181,  182,  183,  184,  185,  187,  186,
 /*  3630 */   166,  528,  537,  528,  528,  528,  528,  528,  528,  528,
 /*  3640 */   106,  107,  480,  482,  484,  486,  106,  107,   26,  528,
 /*  3650 */   105,  220,  528,  528,  110,  528,  105,  528,  528,  528,
 /*  3660 */   540,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  3670 */   176,  177,  178,  179,  180,  181,  182,  183,  184,  185,
 /*  3680 */   187,  186,  166,  528,  537,  528,  528,  528,  528,  528,
 /*  3690 */   528,  528,  528,  528,  480,  482,  484,  486,  100,  101,
 /*  3700 */   103,  102,  104,  224,  528,  100,  101,  103,  102,  104,
 /*  3710 */   528,  528,  540,  167,  168,  169,  170,  171,  172,  173,
 /*  3720 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  183,
 /*  3730 */   184,  185,  187,  186,  166,  528,  537,  528,  528,  528,
 /*  3740 */   528,  528,  528,  528,  106,  107,  480,  482,  484,  486,
 /*  3750 */   128,  106,  107,  528,  105,  401,  528,  528,  528,  157,
 /*  3760 */   528,  105,  528,  528,  540,  167,  168,  169,  170,  171,
 /*  3770 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
 /*  3780 */   182,  183,  184,  185,  187,  186,  166,  528,  537,  151,
 /*  3790 */   528,  528,  153,  657,  663,  528,  662,  647,  480,  482,
 /*  3800 */   484,  486,  139,  623,  528,  528,  528,  544,  124,  100,
 /*  3810 */   101,  103,  102,  104,  528,  528,  540,  167,  168,  169,
 /*  3820 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  3830 */   180,  181,  182,  183,  184,  185,  187,  186,  166,  528,
 /*  3840 */   537,  528,  528,  528,  528,  528,  528,  528,  528,  528,
 /*  3850 */   480,  482,  484,  486,  528,  106,  107,  528,  528,  551,
 /*  3860 */   528,  528,  528,  595,  528,  105,  528,  528,  540,  167,
 /*  3870 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  3880 */   178,  179,  180,  181,  182,  183,  184,  185,  187,  186,
 /*  3890 */   166,  528,  537,  528,  528,  528,  528,  528,  528,  528,
 /*  3900 */   528,  528,  480,  482,  484,  486,  100,  101,  103,  102,
 /*  3910 */   104,  556,  528,  528,  528,  528,  528,  528,  528,  528,
 /*  3920 */   540,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  3930 */   176,  177,  178,  179,  180,  181,  182,  183,  184,  185,
 /*  3940 */   187,  186,  166,  528,  537,  528,  528,  528,  312,  528,
 /*  3950 */   528,  528,  106,  107,  480,  482,  484,  486,  329,  331,
 /*  3960 */   330,  300,  105,  566,  528,  528,  528,  363,  364,  367,
 /*  3970 */   528,  528,  540,  167,  168,  169,  170,  171,  172,  173,
 /*  3980 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  183,
 /*  3990 */   184,  185,  187,  186,  166,  528,  537,  528,  528,   20,
 /*  4000 */   528,   22,  528,  138,  528,   25,  480,  482,  484,  486,
 /*  4010 */   139,  528,  126,  528,  528,  572,  124,  528,  528,  528,
 /*  4020 */   528,  528,  528,  528,  540,  167,  168,  169,  170,  171,
 /*  4030 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
 /*  4040 */   182,  183,  184,  185,  187,  186,  166,  528,  537,  528,
 /*  4050 */   528,  528,  312,  528,  528,  528,  528,  528,  480,  482,
 /*  4060 */   484,  486,  329,  331,  330,  300,  528,  579,  528,  528,
 /*  4070 */   528,  528,  528,  366,  528,  528,  540,  167,  168,  169,
 /*  4080 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  4090 */   180,  181,  182,  183,  184,  185,  187,  186,  166,  528,
 /*  4100 */   537,  528,  528,  528,  263,  528,  528,  528,  528,  528,
 /*  4110 */   480,  482,  484,  486,  329,  331,  330,  300,  528,  585,
 /*  4120 */   528,  528,  528,  528,  528,  528,  528,  528,  540,  167,
 /*  4130 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  4140 */   178,  179,  180,  181,  182,  183,  184,  185,  187,  186,
 /*  4150 */   166,  528,  537,  528,  528,  528,  265,  528,  528,  528,
 /*  4160 */   528,  528,  480,  482,  484,  486,  329,  331,  330,  300,
 /*  4170 */   528,  591,  528,  528,  528,  528,  528,  528,  528,  528,
 /*  4180 */   540,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  4190 */   176,  177,  178,  179,  180,  181,  182,  183,  184,  185,
 /*  4200 */   187,  186,  166,  528,  537,  528,  528,  528,  267,  528,
 /*  4210 */   528,  528,  528,  528,  480,  482,  484,  486,  329,  331,
 /*  4220 */   330,  300,  528,  601,  528,  528,  528,  528,  528,  528,
 /*  4230 */   528,  528,  540,  167,  168,  169,  170,  171,  172,  173,
 /*  4240 */   174,  175,  176,  177,  178,  179,  180,  181,  182,  183,
 /*  4250 */   184,  185,  187,  186,  166,  528,  537,  528,  528,  528,
 /*  4260 */   269,  528,  528,  528,  528,  528,  480,  482,  484,  486,
 /*  4270 */   329,  331,  330,  300,  528,  607,  528,  528,  528,  528,
 /*  4280 */   528,  528,  528,  528,  540,  167,  168,  169,  170,  171,
 /*  4290 */   172,  173,  174,  175,  176,  177,  178,  179,  180,  181,
 /*  4300 */   182,  183,  184,  185,  187,  186,  166,  528,  537,  528,
 /*  4310 */   528,  528,  271,  528,  528,  528,  528,  528,  480,  482,
 /*  4320 */   484,  486,  329,  331,  330,  300,  528,  613,  528,  528,
 /*  4330 */   528,  528,  528,  528,  528,  528,  540,  167,  168,  169,
 /*  4340 */   170,  171,  172,  173,  174,  175,  176,  177,  178,  179,
 /*  4350 */   180,  181,  182,  183,  184,  185,  187,  186,  166,  528,
 /*  4360 */   537,  528,  528,  528,  273,  528,  528,  528,  528,  528,
 /*  4370 */   480,  482,  484,  486,  329,  331,  330,  300,  528,  619,
 /*  4380 */   528,  100,  101,  103,  102,  104,  528,  528,  540,  167,
 /*  4390 */   168,  169,  170,  171,  172,  173,  174,  175,  176,  177,
 /*  4400 */   178,  179,  180,  181,  182,  183,  184,  185,  187,  186,
 /*  4410 */   166,  528,  537,  528,  528,  528,  528,  528,  528,  528,
 /*  4420 */   528,  528,  480,  482,  484,  486,  528,  106,  107,  528,
 /*  4430 */    19,  127,  528,  133,  528,  528,  528,  105,  528,  528,
 /*  4440 */   165,  167,  168,  169,  170,  171,  172,  173,  174,  175,
 /*  4450 */   176,  177,  178,  179,  180,  181,  182,  183,  184,  185,
 /*  4460 */   187,  528,  528,  528,  537,  235,  528,  244,  247,  238,
 /*  4470 */   241,  275,  528,  528,  480,  482,  484,  486,  288,  528,
 /*  4480 */   528,  329,  331,  330,  300,  528,  528,  528,  528,  528,
 /*  4490 */   232,  296,  301,  315,  229,  375,  311,  528,  235,  292,
 /*  4500 */   244,  247,  238,  241,  528,  528,  528,  322,  277,  528,
 /*  4510 */   528,  288,  310,  528,  282,  528,  528,  528,  329,  331,
 /*  4520 */   330,  300,  528,  232,  296,  301,  315,  229,  371,  311,
 /*  4530 */   528,  528,  292,  528,  528,  528,  528,  528,  528,  528,
 /*  4540 */   322,    9,  528,  279,   12,  310,  140,  282,  671,  681,
 /*  4550 */   304,  528,  538,  329,  331,  330,  300,  528,  303,  305,
 /*  4560 */   306,  307,  308,  309,  276,  272,  274,  528,  528,  231,
 /*  4570 */   237,  243,  246,  249,  251,  240,  234,  260,  262,  270,
 /*  4580 */   264,  266,  268,  304,  528,  528,  528,  528,  528,  257,
 /*  4590 */   253,  303,  305,  306,  307,  308,  309,  528,  281,  235,
 /*  4600 */   528,  244,  247,  238,  241,  528,  528,  539,  329,  331,
 /*  4610 */   330,  300,  288,  528,  528,  528,  528,  528,  528,  528,
 /*  4620 */   528,  528,  528,  528,  232,  296,  301,  315,  229,  528,
 /*  4630 */   311,  528,  235,  313,  244,  247,  238,  241,  528,  528,
 /*  4640 */   528,  322,  287,  528,  528,  288,  310,  528,  282,  528,
 /*  4650 */   528,  359,  329,  331,  330,  300,  528,  232,  296,  301,
 /*  4660 */   315,  229,  356,  311,  528,  291,  292,  528,  528,  528,
 /*  4670 */   528,  528,  528,  528,  322,  329,  331,  330,  300,  310,
 /*  4680 */   528,  282,  528,  528,  358,  528,  528,  528,  528,  528,
 /*  4690 */   528,  528,  357,  305,  306,  307,  308,  309,  528,  272,
 /*  4700 */   274,  528,  528,  231,  237,  243,  246,  249,  251,  240,
 /*  4710 */   234,  260,  262,  270,  264,  266,  268,  304,  528,  528,
 /*  4720 */   528,  528,  528,  257,  253,  303,  305,  306,  307,  308,
 /*  4730 */   309,  299,  528,  235,  528,  244,  247,  238,  241,  528,
 /*  4740 */   302,  329,  331,  330,  300,  528,  288,  528,  528,  528,
 /*  4750 */   329,  331,  330,  300,  528,  528,  528,  528,  232,  296,
 /*  4760 */   301,  315,  229,  318,  311,  528,  235,  292,  244,  247,
 /*  4770 */   238,  241,  528,  399,  528,  322,  528,  528,  528,  288,
 /*  4780 */   310,  528,  282,  329,  331,  330,  300,  528,  528,  528,
 /*  4790 */   528,  232,  296,  301,  315,  229,  328,  311,  528,  489,
 /*  4800 */   292,  528,  528,  528,  528,  528,  528,  528,  322,  329,
 /*  4810 */   331,  330,  300,  310,  528,  282,  528,  528,  304,  528,
 /*  4820 */   528,  528,  528,  528,  528,  528,  303,  305,  306,  307,
 /*  4830 */   308,  309,  231,  237,  243,  246,  249,  251,  240,  234,
 /*  4840 */   260,  262,  270,  264,  266,  268,  528,  528,  528,  528,
 /*  4850 */   528,  304,  257,  253,  528,  528,  528,  528,  528,  303,
 /*  4860 */   305,  306,  307,  308,  309,  493,  528,  235,  528,  244,
 /*  4870 */   247,  238,  241,  528,  496,  329,  331,  330,  300,  528,
 /*  4880 */   288,  528,  528,  528,  329,  331,  330,  300,  528,  528,
 /*  4890 */   528,  528,  232,  296,  301,  315,  229,  338,  311,  528,
 /*  4900 */   235,  292,  244,  247,  238,  241,  528,  528,  528,  322,
 /*  4910 */   528,  499,  528,  288,  310,  528,  282,  528,  528,  458,
 /*  4920 */   419,  329,  331,  330,  300,  232,  296,  301,  315,  229,
 /*  4930 */   345,  311,  528,  409,  292,  528,  528,  528,  528,  528,
 /*  4940 */   528,  528,  322,  528,  542,  528,  528,  310,  528,  282,
 /*  4950 */   528,  528,  304,  528,  329,  331,  330,  300,  528,  528,
 /*  4960 */   303,  305,  306,  307,  308,  309,  528,  410,  411,  412,
 /*  4970 */   413,  414,  415,  528,  447,  465,  466,  528,  528,  549,
 /*  4980 */   528,  528,  528,  528,  528,  304,  528,  528,  528,  329,
 /*  4990 */   331,  330,  300,  303,  305,  306,  307,  308,  309,  554,
 /*  5000 */   528,  235,  528,  244,  247,  238,  241,  528,  528,  329,
 /*  5010 */   331,  330,  300,  528,  288,  528,  528,  528,  528,  528,
 /*  5020 */   528,  528,  528,  528,  528,  528,  232,  296,  301,  315,
 /*  5030 */   229,  351,  311,  528,  235,  292,  244,  247,  238,  241,
 /*  5040 */   528,  528,  528,  322,  528,  528,  528,  288,  310,  528,
 /*  5050 */   282,  528,  528,  528,  528,  528,  528,  528,  528,  232,
 /*  5060 */   296,  301,  315,  229,  382,  311,  528,  528,  292,  528,
 /*  5070 */   528,  528,  528,  528,  528,  528,  322,  528,  528,  528,
 /*  5080 */   528,  310,  528,  282,  528,  528,  304,  528,  528,  528,
 /*  5090 */   528,  528,  528,  528,  303,  305,  306,  307,  308,  309,
 /*  5100 */   100,  101,  103,  102,  104,  528,  528,  528,  528,  528,
 /*  5110 */   528,  528,  528,  528,  528,  528,  528,  528,  528,  304,
 /*  5120 */   528,  528,  528,  528,  528,  528,  528,  303,  305,  306,
 /*  5130 */   307,  308,  309,  528,  528,  235,  528,  244,  247,  238,
 /*  5140 */   241,  528,  528,  528,  528,  528,  106,  107,  288,  726,
 /*  5150 */   127,  528,  133,  528,  528,  528,  105,  528,  528,  528,
 /*  5160 */   232,  296,  301,  315,  229,  388,  311,  528,  235,  292,
 /*  5170 */   244,  247,  238,  241,  528,  528,  528,  322,  528,  528,
 /*  5180 */   528,  288,  310,  528,  282,  528,  528,  528,  528,  528,
 /*  5190 */   528,  528,  528,  232,  296,  301,  315,  229,  528,  311,
 /*  5200 */   528,  528,  292,  528,  528,  528,  528,  528,  528,  528,
 /*  5210 */   322,  528,  528,  528,  528,  310,  528,  282,  528,  528,
 /*  5220 */   304,  528,  528,  528,  528,  528,  528,  528,  303,  305,
 /*  5230 */   306,  307,  308,  309,  100,  101,  103,  102,  104,  528,
 /*  5240 */   528,  528,  528,  528,  528,  541,  528,  528,  528,  528,
 /*  5250 */   528,  528,  528,  304,  528,  528,  528,  528,  528,  528,
 /*  5260 */   528,  303,  305,  306,  307,  308,  309,  528,  528,  235,
 /*  5270 */   528,  244,  247,  238,  241,  528,  528,  528,  528,  528,
 /*  5280 */   106,  107,  288,  150,  626,  528,  133,  528,  528,  528,
 /*  5290 */   105,  528,  528,  528,  232,  296,  301,  315,  229,  528,
 /*  5300 */   311,  528,  528,  292,  235,  528,  244,  247,  238,  241,
 /*  5310 */   528,  322,  528,  528,  528,  528,  310,  288,  282,  528,
 /*  5320 */   528,  424,  528,  528,  528,  528,  528,  528,   96,  232,
 /*  5330 */   296,  301,  315,  229,  528,  311,  528,  528,  292,  528,
 /*  5340 */   528,  528,  528,  528,  528,   29,  322,  528,  528,   68,
 /*  5350 */   528,  310,  528,  282,  304,  528,  452,  528,  528,  528,
 /*  5360 */   528,  528,  303,  305,  306,  307,  308,  309,   47,   48,
 /*  5370 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  100,
 /*  5380 */   101,  103,  102,  104,  528,  528,  528,  528,  528,  304,
 /*  5390 */   528,  528,  528,  528,  528,  528,  528,  303,  305,  306,
 /*  5400 */   307,  308,  309,  528,  528,  235,  528,  244,  247,  238,
 /*  5410 */   241,  528,  528,  528,  528,  528,  528,  528,  288,  528,
 /*  5420 */   528,  528,  528,  528,  528,  106,  107,  528,  728,  108,
 /*  5430 */   232,  296,  301,  315,  229,  105,  311,  528,  235,  292,
 /*  5440 */   244,  247,  238,  241,  528,  528,  528,  322,  528,  528,
 /*  5450 */   528,  288,  310,  528,  282,  528,  528,  459,  528,  528,
 /*  5460 */   528,  528,  528,  232,  296,  301,  315,  229,   41,  311,
 /*  5470 */   528,  528,  292,  491,  528,  528,  528,  528,  528,  528,
 /*  5480 */   322,  528,  528,  528,  528,  310,  528,  282,  528,  528,
 /*  5490 */   304,  528,  528,  528,  528,  528,  528,  528,  303,  305,
 /*  5500 */   306,  307,  308,  309,  528,  528,   33,  528,   47,   48,
 /*  5510 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  528,
 /*  5520 */   528,  528,  528,  304,  528,  528,  528,  528,  528,  528,
 /*  5530 */   528,  303,  305,  306,  307,  308,  309,  528,  528,  235,
 /*  5540 */   528,  244,  247,  238,  241,  528,  528,  528,  528,  528,
 /*  5550 */   528,  528,  288,  528,  528,  528,  528,  528,  528,  528,
 /*  5560 */   528,  528,  528,  528,  232,  296,  301,  315,  229,  528,
 /*  5570 */   311,  528,  235,  292,  244,  247,  238,  241,  528,  528,
 /*  5580 */   528,  322,  528,  528,  528,  288,  310,  528,  282,  528,
 /*  5590 */   528,  528,  528,  528,  528,  528,  528,  232,  296,  301,
 /*  5600 */   315,  229,   96,  311,  528,  528,  292,  528,  528,  528,
 /*  5610 */   528,  528,  528,  528,  322,  528,  553,  528,  528,  310,
 /*  5620 */   528,  282,  528,   68,  304,  528,  528,  528,  528,  528,
 /*  5630 */   528,  528,  303,  305,  306,  307,  308,  309,  528,  528,
 /*  5640 */   528,  528,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  5650 */    55,   56,   57,  528,  528,  528,  528,  304,  528,  528,
 /*  5660 */   528,  528,  528,  528,  528,  303,  305,  306,  307,  308,
 /*  5670 */   309,  528,  528,  235,  528,  244,  247,  238,  241,  528,
 /*  5680 */   528,  528,  528,  528,  528,  528,  288,  528,  528,  528,
 /*  5690 */   528,  528,  528,  528,  528,  528,  528,  528,  232,  296,
 /*  5700 */   301,  315,  229,  528,  311,  528,  528,  313,  528,  528,
 /*  5710 */   528,  528,  528,  528,  528,  322,  528,  528,  528,  528,
 /*  5720 */   310,  528,  282,  528,  278,  528,  528,  528,  528,  276,
 /*  5730 */   272,  274,  528,  528,  231,  237,  243,  246,  249,  251,
 /*  5740 */   240,  234,  260,  262,  270,  264,  266,  268,  528,  528,
 /*  5750 */   528,  528,  528,  528,  257,  253,  528,  528,  358,  528,
 /*  5760 */   528,  528,  528,  528,  528,  528,  357,  305,  306,  307,
 /*  5770 */   308,  309,  528,  528,  528,  259,  278,  528,  528,  528,
 /*  5780 */   528,  276,  272,  274,  528,  528,  231,  237,  243,  246,
 /*  5790 */   249,  251,  240,  234,  260,  262,  270,  264,  266,  268,
 /*  5800 */   528,  528,  528,  278,  528,  528,  257,  253,  276,  272,
 /*  5810 */   274,  528,  528,  231,  237,  243,  246,  249,  251,  240,
 /*  5820 */   234,  260,  262,  270,  264,  266,  268,  528,  528,  528,
 /*  5830 */   528,  528,  284,  257,  253,  528,  528,  528,  528,  528,
 /*  5840 */   528,  278,  528,  528,  528,  280,  276,  272,  274,  528,
 /*  5850 */   528,  231,  237,  243,  246,  249,  251,  240,  234,  260,
 /*  5860 */   262,  270,  264,  266,  268,  528,  528,  528,  528,  528,
 /*  5870 */   528,  257,  253,  278,  528,  528,  528,  528,  276,  272,
 /*  5880 */   274,  528,  400,  231,  237,  243,  246,  249,  251,  240,
 /*  5890 */   234,  260,  262,  270,  264,  266,  268,  528,  528,  528,
 /*  5900 */   278,  528,  528,  257,  253,  276,  272,  274,  528,  528,
 /*  5910 */   231,  237,  243,  246,  249,  251,  240,  234,  260,  262,
 /*  5920 */   270,  264,  266,  268,  897,  528,  528,  528,  528,  528,
 /*  5930 */   257,  253,  278,  528,  490,  528,  528,  276,  272,  274,
 /*  5940 */   528,  528,  231,  237,  243,  246,  249,  251,  240,  234,
 /*  5950 */   260,  262,  270,  264,  266,  268,  528,  528,  528,  528,
 /*  5960 */   528,  528,  257,  253,  278,  528,  494,  528,  528,  276,
 /*  5970 */   272,  274,  528,  528,  231,  237,  243,  246,  249,  251,
 /*  5980 */   240,  234,  260,  262,  270,  264,  266,  268,  528,  528,
 /*  5990 */   528,  528,  528,  528,  257,  253,  278,  528,  497,  528,
 /*  6000 */   528,  276,  272,  274,  528,  528,  231,  237,  243,  246,
 /*  6010 */   249,  251,  240,  234,  260,  262,  270,  264,  266,  268,
 /*  6020 */   528,  528,  528,  528,  528,  528,  257,  253,  278,  528,
 /*  6030 */   500,  528,  528,  276,  272,  274,  528,  528,  231,  237,
 /*  6040 */   243,  246,  249,  251,  240,  234,  260,  262,  270,  264,
 /*  6050 */   266,  268,  528,  528,  528,  528,  528,  528,  257,  253,
 /*  6060 */   278,  528,  528,  528,  528,  276,  272,  274,  528,  543,
 /*  6070 */   231,  237,  243,  246,  249,  251,  240,  234,  260,  262,
 /*  6080 */   270,  264,  266,  268,  528,  528,  528,  528,  528,  528,
 /*  6090 */   257,  253,  278,  528,  528,  528,  528,  276,  272,  274,
 /*  6100 */   528,  550,  231,  237,  243,  246,  249,  251,  240,  234,
 /*  6110 */   260,  262,  270,  264,  266,  268,  528,  528,  528,  528,
 /*  6120 */   528,  528,  257,  253,  278,  528,  528,  528,  528,  276,
 /*  6130 */   272,  274,   96,  555,  231,  237,  243,  246,  249,  251,
 /*  6140 */   240,  234,  260,  262,  270,  264,  266,  268,  528,  113,
 /*  6150 */    96,  528,  528,   68,  257,  253,  528,  528,  528,  528,
 /*  6160 */   528,  528,  528,  528,  528,  528,   96,  160,  528,  528,
 /*  6170 */   528,   68,   47,   48,   49,   50,   51,   52,   53,   54,
 /*  6180 */    55,   56,   57,  598,  528,  528,  528,   68,   41,  528,
 /*  6190 */    47,   48,   49,   50,   51,   52,   53,   54,   55,   56,
 /*  6200 */    57,  100,  101,  103,  102,  104,   47,   48,   49,   50,
 /*  6210 */    51,   52,   53,   54,   55,   56,   57,  100,  101,  103,
 /*  6220 */   102,  104,  528,  528,  528,  528,  528,  528,   47,   48,
 /*  6230 */    49,   50,   51,   52,   53,   54,   55,   56,   57,  100,
 /*  6240 */   101,  103,  102,  104,  528,  528,  528,  106,  107,  528,
 /*  6250 */   717,  626,  528,  133,  528,  528,  528,  105,  528,  528,
 /*  6260 */   528,  528,  528,  106,  107,  528,  721,  624,  528,  133,
 /*  6270 */   528,  528,  528,  105,  528,  100,  101,  103,  102,  104,
 /*  6280 */   100,  101,  103,  102,  104,  106,  107,  528,  718,  626,
 /*  6290 */   528,  133,  528,  528,  528,  105,  528,  100,  101,  103,
 /*  6300 */   102,  104,  100,  101,  103,  102,  104,  528,  528,  528,
 /*  6310 */   528,  528,  528,  528,  528,  528,  528,  528,  528,  528,
 /*  6320 */   528,  106,  107,  528,  727,  108,  106,  107,  528,  720,
 /*  6330 */   593,  105,  528,  528,  528,  528,  105,  528,  100,  101,
 /*  6340 */   103,  102,  104,  106,  107,  528,  724,  593,  106,  107,
 /*  6350 */   528,  725,  593,  105,  528,  528,  528,  528,  105,  528,
 /*  6360 */   100,  101,  103,  102,  104,  528,  528,  528,  528,  528,
 /*  6370 */   528,  528,  528,  528,  528,  528,  528,  528,  528,  528,
 /*  6380 */   528,  528,  528,  528,  106,  107,  528,  723,  593,  528,
 /*  6390 */   528,  528,  528,  528,  105,  528,  528,  528,  528,  528,
 /*  6400 */   528,  528,  528,  528,  528,  528,  106,  107,  528,  719,
 /*  6410 */   593,  528,  528,  528,  528,  528,  105,
};
static YYCODETYPE yy_lookahead[] = {
 /*     0 */     7,   56,    9,   10,   11,   12,   26,   27,   28,   29,
 /*    10 */    30,   31,   41,   20,   29,   30,   31,   38,   38,   39,
 /*    20 */    41,  128,   77,   38,   39,   32,   33,   34,   35,   36,
 /*    30 */     0,   38,  139,   54,   41,   42,  124,  125,   93,  127,
 /*    40 */   128,  128,   49,   50,   51,  133,  134,   54,  135,   56,
 /*    50 */    42,  139,  139,   60,   61,   62,   63,   64,   65,   66,
 /*    60 */    67,   68,   41,   70,   71,   72,   73,  173,   75,  175,
 /*    70 */   176,   92,   79,   80,   81,   82,    6,   41,   85,  100,
 /*    80 */   101,  102,  103,  104,  105,   92,   50,   51,   41,   96,
 /*    90 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   100 */     7,   54,    9,   10,   11,   12,  111,  112,  113,  114,
 /*   110 */   115,  116,   42,   20,  108,  109,  110,  111,  112,  113,
 /*   120 */   114,  115,  116,   42,   50,   32,   33,   34,   35,   36,
 /*   130 */    49,   38,  136,   52,   41,   42,  117,  125,  142,  143,
 /*   140 */   128,  145,   49,   50,   51,  133,  134,   54,    6,   56,
 /*   150 */    76,  139,   78,   60,   61,   62,   63,   64,   65,   66,
 /*   160 */    67,   68,   42,   70,   71,   72,   73,   38,   75,   49,
 /*   170 */   136,   21,   79,   80,   81,   82,  142,  143,   85,  145,
 /*   180 */     6,   52,   44,  142,  143,   92,  145,   49,   38,   96,
 /*   190 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   200 */     7,   59,    9,   10,   11,   12,  141,  142,  143,  144,
 /*   210 */     6,   37,   39,   20,   42,   86,   87,   88,   89,   90,
 /*   220 */    91,   49,   49,   94,   95,   32,   33,   34,   35,   36,
 /*   230 */   173,   38,  175,  176,   41,   42,  125,    6,  127,  128,
 /*   240 */   128,   37,   49,   50,   51,  134,  134,   54,   41,   56,
 /*   250 */   139,  139,   39,   60,   61,   62,   63,   64,   65,   66,
 /*   260 */    67,   68,   49,   70,   71,   72,   73,   38,   75,   42,
 /*   270 */    38,   39,   79,   80,   81,   82,   49,  136,   85,   52,
 /*   280 */    49,   52,   36,  142,  143,   92,  145,   41,   49,   96,
 /*   290 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   300 */     7,    6,    9,   10,   11,   12,  138,  117,  140,  141,
 /*   310 */   142,  143,  144,   20,   56,   86,   87,   88,   89,   90,
 /*   320 */    91,  168,  169,   39,   50,   32,   33,   34,   35,   36,
 /*   330 */   136,   38,   37,   49,   41,   42,  142,  143,  173,  145,
 /*   340 */   175,  176,   49,   50,   51,   39,    6,   54,   42,   56,
 /*   350 */    76,   93,   78,   60,   61,   62,   63,   64,   65,   66,
 /*   360 */    67,   68,   41,   70,   71,   72,   73,   38,   75,  119,
 /*   370 */    49,  121,   79,   80,   81,   82,   39,   37,   85,   42,
 /*   380 */    39,   52,   44,   42,   46,   92,   56,   49,  139,   96,
 /*   390 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   400 */     7,   50,    9,   10,   11,   12,  138,    6,  140,  141,
 /*   410 */   142,  143,  144,   20,   41,   86,   87,   88,   89,   90,
 /*   420 */    91,   39,   49,   93,   42,   32,   33,   34,   35,   36,
 /*   430 */    46,   38,   41,   49,   41,   42,    6,   41,   37,  126,
 /*   440 */    49,  128,   49,   50,   51,    6,  133,   54,  135,   56,
 /*   450 */   171,  172,  139,   60,   61,   62,   63,   64,   65,   66,
 /*   460 */    67,   68,    6,   70,   71,   72,   73,   37,   75,  170,
 /*   470 */     6,   56,   79,   80,   81,   82,   37,    6,   85,  180,
 /*   480 */   181,  182,  183,   41,    6,   92,  187,  188,   92,   96,
 /*   490 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   500 */     7,   37,    9,   10,   11,   12,  170,    6,   37,   44,
 /*   510 */     6,   46,   42,   20,   49,   37,  180,  181,  182,  183,
 /*   520 */    41,   16,   41,  187,  188,   32,   33,   34,   35,   36,
 /*   530 */    49,   38,   41,  173,   41,   42,  176,   41,   37,   83,
 /*   540 */    49,   37,   49,   50,   51,  173,    6,   54,  176,   56,
 /*   550 */   184,  185,    6,   60,   61,   62,   63,   64,   65,   66,
 /*   560 */    67,   68,   36,   70,   71,   72,   73,  173,   75,  170,
 /*   570 */   176,   92,   79,   80,   81,   82,    6,   37,   85,  180,
 /*   580 */   181,  182,  183,  131,  132,   92,  187,  188,   42,   96,
 /*   590 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   600 */     7,    6,    9,   10,   11,   12,  170,  184,  185,  184,
 /*   610 */   185,   59,   42,   20,    6,    6,  180,  181,  182,  183,
 /*   620 */    39,  184,  185,  187,  188,   32,   33,   34,   35,   36,
 /*   630 */    49,   38,  184,  185,   41,   42,    6,   42,  184,  185,
 /*   640 */     6,    6,   49,   50,   51,  184,  185,   54,    6,   56,
 /*   650 */    42,   42,    6,   60,   61,   62,   63,   64,   65,   66,
 /*   660 */    67,   68,   21,   70,   71,   72,   73,   37,   75,  170,
 /*   670 */   184,  185,   79,   80,   81,   82,   42,   42,   85,  180,
 /*   680 */   181,  182,  183,   52,   42,   92,  187,  188,   42,   96,
 /*   690 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   700 */     7,    6,    9,   10,   11,   12,  170,  184,  185,  184,
 /*   710 */   185,  128,    6,   20,   42,   44,  180,  181,  182,  183,
 /*   720 */    49,  129,  139,  187,  188,   32,   33,   34,   35,   36,
 /*   730 */   128,   38,    6,   93,   41,   42,  119,   42,  121,   54,
 /*   740 */    59,  139,   49,   50,   51,  129,   52,   54,   42,   56,
 /*   750 */    41,  129,   41,   60,   61,   62,   63,   64,   65,   66,
 /*   760 */    67,   68,   51,   70,   71,   72,   73,   52,   75,  170,
 /*   770 */    41,   52,   79,   80,   81,   82,   50,  129,   85,  180,
 /*   780 */   181,  182,  183,   41,   21,   92,  187,  188,   41,   96,
 /*   790 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   800 */     7,   56,    9,   10,   11,   12,  170,  119,   42,  121,
 /*   810 */    42,   54,   52,   20,   42,  129,  180,  181,  182,  183,
 /*   820 */   129,   54,   52,  187,  188,   32,   33,   34,   35,   36,
 /*   830 */    42,   38,   41,   41,   41,   42,  118,   41,  118,  118,
 /*   840 */    50,   41,   49,   50,   51,   56,   42,   54,   42,   56,
 /*   850 */    74,   49,   49,   60,   61,   62,   63,   64,   65,   66,
 /*   860 */    67,   68,   49,   70,   71,   72,   73,  169,   75,  170,
 /*   870 */   129,   77,   79,   80,   81,   82,   49,   49,   85,  180,
 /*   880 */   181,  182,  183,   49,   42,   92,  187,  188,   80,   96,
 /*   890 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*   900 */     7,   56,    9,   10,   11,   12,  170,   37,   41,   21,
 /*   910 */    56,    6,   41,   20,   56,   41,  180,  181,  182,  183,
 /*   920 */    56,   41,   50,  187,  188,   32,   33,   34,   35,   36,
 /*   930 */    41,   38,   50,   56,   41,   42,   50,   56,   77,   77,
 /*   940 */    77,   50,   49,   50,   51,   50,   56,   54,   56,   56,
 /*   950 */    41,  172,   50,   60,   61,   62,   63,   64,   65,   66,
 /*   960 */    67,   68,   59,   70,   71,   72,   73,   50,   75,   50,
 /*   970 */   173,  170,   79,   80,   81,   82,   41,   41,   85,   42,
 /*   980 */    52,  180,  181,  182,  183,   92,   42,  173,  173,   96,
 /*   990 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1000 */     7,   42,    9,   10,   11,   12,  173,   42,  173,  173,
 /*  1010 */    41,  185,   52,   20,   86,   87,   88,   89,   90,   91,
 /*  1020 */    41,   83,   80,  129,   42,   32,   33,   34,   35,   36,
 /*  1030 */    77,   38,   74,   49,   41,   42,   49,   49,   41,   56,
 /*  1040 */    49,   49,   49,   50,   51,  130,  130,   54,   42,   56,
 /*  1050 */   129,   42,   42,   60,   61,   62,   63,   64,   65,   66,
 /*  1060 */    67,   68,  132,   70,   71,   72,   73,   41,   75,   42,
 /*  1070 */   130,  170,   79,   80,   81,   82,  129,   42,   85,  178,
 /*  1080 */   179,  180,  181,  182,  183,   92,  130,   42,  121,   96,
 /*  1090 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1100 */     7,   45,    9,   10,   11,   12,   41,  118,  118,   41,
 /*  1110 */    49,   45,  138,   20,  140,  141,  142,  143,  144,  118,
 /*  1120 */   118,   41,  118,   41,  192,   32,   33,   34,   35,   36,
 /*  1130 */   118,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1140 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1150 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1160 */    67,   68,  192,   70,   71,   72,   73,  167,   75,  192,
 /*  1170 */   170,  192,   79,   80,   81,   82,  192,  192,   85,  192,
 /*  1180 */   180,  181,  182,  183,  192,   92,  192,  192,  192,   96,
 /*  1190 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1200 */     7,  192,    9,   10,   11,   12,  123,  192,  125,  192,
 /*  1210 */   127,  128,  192,   20,  192,  192,  133,  134,  192,  192,
 /*  1220 */   192,  192,  139,  192,  192,   32,   33,   34,   35,   36,
 /*  1230 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1240 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1250 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1260 */    67,   68,  192,   70,   71,   72,   73,  167,   75,  192,
 /*  1270 */   170,  192,   79,   80,   81,   82,  192,  192,   85,  192,
 /*  1280 */   180,  181,  182,  183,  192,   92,  192,  192,  192,   96,
 /*  1290 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1300 */     7,  192,    9,   10,   11,   12,  192,  192,  192,  192,
 /*  1310 */   192,  192,  138,   20,  140,  141,  142,  143,  144,  192,
 /*  1320 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  1330 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1340 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1350 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1360 */    67,   68,  192,   70,   71,   72,   73,  167,   75,  192,
 /*  1370 */   170,  192,   79,   80,   81,   82,  192,  192,   85,  192,
 /*  1380 */   180,  181,  182,  183,  192,   92,  192,  192,  192,   96,
 /*  1390 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1400 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  1410 */   192,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  1420 */   192,  192,  192,  192,  188,   32,   33,   34,   35,   36,
 /*  1430 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1440 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1450 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1460 */    67,   68,  192,   70,   71,   72,   73,  167,   75,  192,
 /*  1470 */   170,  192,   79,   80,   81,   82,  192,  192,   85,  192,
 /*  1480 */   180,  181,  182,  183,  192,   92,  192,  192,  192,   96,
 /*  1490 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1500 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  1510 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  1520 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  1530 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1540 */   192,  142,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1550 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1560 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  1570 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  1580 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  1590 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1600 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  1610 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  1620 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  1630 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1640 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1650 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1660 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  1670 */   192,  192,   79,   80,   81,   82,  177,  192,   85,  180,
 /*  1680 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  1690 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1700 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  1710 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  1720 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  1730 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1740 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1750 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1760 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  192,
 /*  1770 */   170,  192,   79,   80,   81,   82,  192,  192,   85,  179,
 /*  1780 */   180,  181,  182,  183,  192,   92,  192,  192,  192,   96,
 /*  1790 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1800 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  1810 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  1820 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  1830 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1840 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1850 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1860 */    67,   68,  192,   70,   71,   72,   73,  167,   75,  192,
 /*  1870 */   170,  192,   79,   80,   81,   82,  192,  192,   85,  192,
 /*  1880 */   180,  181,  182,  183,  192,   92,  192,  192,  192,   96,
 /*  1890 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  1900 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  1910 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  1920 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  1930 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  1940 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  1950 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  1960 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  1970 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  1980 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  1990 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2000 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  2010 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  2020 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2030 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2040 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2050 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2060 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2070 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2080 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2090 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2100 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  2110 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  2120 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2130 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2140 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2150 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2160 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2170 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2180 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2190 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2200 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  2210 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  2220 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2230 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2240 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2250 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2260 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2270 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2280 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2290 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2300 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  2310 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  2320 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2330 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2340 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2350 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2360 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2370 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2380 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2390 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2400 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  2410 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  2420 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2430 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2440 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2450 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2460 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2470 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2480 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2490 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2500 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  2510 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  2520 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2530 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2540 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2550 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2560 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2570 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2580 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2590 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2600 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  2610 */   174,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  2620 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2630 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2640 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2650 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2660 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2670 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2680 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2690 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2700 */     7,  192,    9,   10,   11,   12,  192,  192,  192,  192,
 /*  2710 */   192,  192,  138,   20,  140,  141,  142,  143,  144,  192,
 /*  2720 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2730 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2740 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2750 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2760 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2770 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2780 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2790 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2800 */     7,  192,    9,   10,   11,   12,  192,  192,  192,  192,
 /*  2810 */   192,  192,  138,   20,  140,  141,  142,  143,  144,  192,
 /*  2820 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2830 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2840 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2850 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2860 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2870 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2880 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2890 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  2900 */     7,  192,    9,   10,   11,   12,  192,  192,  192,  192,
 /*  2910 */   192,  192,  138,   20,  140,  141,  142,  143,  144,  192,
 /*  2920 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  2930 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  2940 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  2950 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  2960 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  2970 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  2980 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  2990 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3000 */     7,  192,    9,   10,   11,   12,  192,  192,  192,  192,
 /*  3010 */   192,  192,  138,   20,  140,  141,  142,  143,  144,  192,
 /*  3020 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  3030 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  3040 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  3050 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  3060 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  3070 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  3080 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  3090 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3100 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  3110 */   192,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  3120 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  3130 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  3140 */   192,  192,   49,   50,   51,  192,  192,   54,  192,   56,
 /*  3150 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  3160 */    67,   68,  192,   70,   71,   72,   73,  192,   75,  170,
 /*  3170 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  3180 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  3190 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3200 */     7,  192,    9,   10,   11,   12,  170,  192,  192,  192,
 /*  3210 */   192,  192,  192,   20,  192,  192,  180,  181,  182,  183,
 /*  3220 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  3230 */   192,   38,  192,  192,   41,   42,  192,  192,  192,  192,
 /*  3240 */   192,  192,   49,  192,   51,  192,  192,   54,  192,   56,
 /*  3250 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  3260 */    67,   68,  192,   70,   71,   72,   73,    0,   75,  170,
 /*  3270 */   192,  192,   79,   80,   81,   82,  192,  192,   85,  180,
 /*  3280 */   181,  182,  183,  192,  192,   92,  192,  192,  192,   96,
 /*  3290 */    97,   98,   99,  100,  101,  102,  103,  104,  105,  106,
 /*  3300 */     7,  192,    9,   10,   11,   12,  192,   40,  192,  192,
 /*  3310 */    43,  192,   45,   20,   47,   48,  192,  192,   51,  192,
 /*  3320 */   192,  192,  192,  192,  192,   32,   33,   34,   35,   36,
 /*  3330 */   192,   38,  192,  192,   41,  192,    1,    2,    3,    4,
 /*  3340 */     5,  192,   49,  192,  192,  192,  192,   54,  192,   56,
 /*  3350 */   192,  192,  192,   60,   61,   62,   63,   64,   65,   66,
 /*  3360 */    67,   68,   69,   70,  192,  192,  192,  192,  170,  115,
 /*  3370 */   116,  192,  192,  106,  192,  192,   41,  192,  180,  181,
 /*  3380 */   182,  183,   47,   48,  186,   92,  192,  189,  190,  191,
 /*  3390 */    55,  137,   57,  100,  101,  102,  103,  104,  105,  192,
 /*  3400 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3410 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3420 */   166,  115,  116,  192,  170,  192,  192,  192,  192,  192,
 /*  3430 */   192,  192,  192,  192,  180,  181,  182,  183,    1,    2,
 /*  3440 */     3,    4,    5,  137,  192,  192,  192,  192,  192,  192,
 /*  3450 */   192,  192,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  3460 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3470 */   164,  165,  166,  115,  116,  192,  170,  192,   41,  192,
 /*  3480 */   192,  192,  192,  192,   47,   48,  180,  181,  182,  183,
 /*  3490 */   192,  192,   55,  192,   57,  137,    1,    2,    3,    4,
 /*  3500 */     5,  192,  192,  192,  146,  147,  148,  149,  150,  151,
 /*  3510 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  3520 */   162,  163,  164,  165,  166,  115,  116,  192,  170,  192,
 /*  3530 */   192,  192,  192,  192,  192,  192,  192,  192,  180,  181,
 /*  3540 */   182,  183,   47,   48,  192,   50,   51,  137,  192,  192,
 /*  3550 */   192,  192,   57,  192,  192,  192,  146,  147,  148,  149,
 /*  3560 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3570 */   160,  161,  162,  163,  164,  165,  166,  115,  116,  192,
 /*  3580 */   170,  192,  192,  192,  192,  192,  192,  192,  192,  192,
 /*  3590 */   180,  181,  182,  183,    1,    2,    3,    4,    5,  137,
 /*  3600 */     1,    2,    3,    4,    5,  192,  192,  192,  146,  147,
 /*  3610 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3620 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  115,
 /*  3630 */   116,  192,  170,  192,  192,  192,  192,  192,  192,  192,
 /*  3640 */    47,   48,  180,  181,  182,  183,   47,   48,   55,  192,
 /*  3650 */    57,  137,  192,  192,   55,  192,   57,  192,  192,  192,
 /*  3660 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3670 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3680 */   166,  115,  116,  192,  170,  192,  192,  192,  192,  192,
 /*  3690 */   192,  192,  192,  192,  180,  181,  182,  183,    1,    2,
 /*  3700 */     3,    4,    5,  137,  192,    1,    2,    3,    4,    5,
 /*  3710 */   192,  192,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  3720 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3730 */   164,  165,  166,  115,  116,  192,  170,  192,  192,  192,
 /*  3740 */   192,  192,  192,  192,   47,   48,  180,  181,  182,  183,
 /*  3750 */    53,   47,   48,  192,   57,  137,  192,  192,  192,   55,
 /*  3760 */   192,   57,  192,  192,  146,  147,  148,  149,  150,  151,
 /*  3770 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  3780 */   162,  163,  164,  165,  166,  115,  116,  192,  170,  120,
 /*  3790 */   192,  192,  123,  124,  125,  192,  127,  128,  180,  181,
 /*  3800 */   182,  183,  133,  134,  192,  192,  192,  137,  139,    1,
 /*  3810 */     2,    3,    4,    5,  192,  192,  146,  147,  148,  149,
 /*  3820 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  3830 */   160,  161,  162,  163,  164,  165,  166,  115,  116,  192,
 /*  3840 */   170,  192,  192,  192,  192,  192,  192,  192,  192,  192,
 /*  3850 */   180,  181,  182,  183,  192,   47,   48,  192,  192,  137,
 /*  3860 */   192,  192,  192,   55,  192,   57,  192,  192,  146,  147,
 /*  3870 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  3880 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  115,
 /*  3890 */   116,  192,  170,  192,  192,  192,  192,  192,  192,  192,
 /*  3900 */   192,  192,  180,  181,  182,  183,    1,    2,    3,    4,
 /*  3910 */     5,  137,  192,  192,  192,  192,  192,  192,  192,  192,
 /*  3920 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  3930 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  3940 */   166,  115,  116,  192,  170,  192,  192,  192,  170,  192,
 /*  3950 */   192,  192,   47,   48,  180,  181,  182,  183,  180,  181,
 /*  3960 */   182,  183,   57,  137,  192,  192,  192,  189,  190,  191,
 /*  3970 */   192,  192,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  3980 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  3990 */   164,  165,  166,  115,  116,  192,  170,  192,  192,  122,
 /*  4000 */   192,  124,  192,  126,  192,  128,  180,  181,  182,  183,
 /*  4010 */   133,  192,  135,  192,  192,  137,  139,  192,  192,  192,
 /*  4020 */   192,  192,  192,  192,  146,  147,  148,  149,  150,  151,
 /*  4030 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  4040 */   162,  163,  164,  165,  166,  115,  116,  192,  170,  192,
 /*  4050 */   192,  192,  170,  192,  192,  192,  192,  192,  180,  181,
 /*  4060 */   182,  183,  180,  181,  182,  183,  192,  137,  192,  192,
 /*  4070 */   192,  192,  192,  191,  192,  192,  146,  147,  148,  149,
 /*  4080 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  4090 */   160,  161,  162,  163,  164,  165,  166,  115,  116,  192,
 /*  4100 */   170,  192,  192,  192,  170,  192,  192,  192,  192,  192,
 /*  4110 */   180,  181,  182,  183,  180,  181,  182,  183,  192,  137,
 /*  4120 */   192,  192,  192,  192,  192,  192,  192,  192,  146,  147,
 /*  4130 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4140 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  115,
 /*  4150 */   116,  192,  170,  192,  192,  192,  170,  192,  192,  192,
 /*  4160 */   192,  192,  180,  181,  182,  183,  180,  181,  182,  183,
 /*  4170 */   192,  137,  192,  192,  192,  192,  192,  192,  192,  192,
 /*  4180 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  4190 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4200 */   166,  115,  116,  192,  170,  192,  192,  192,  170,  192,
 /*  4210 */   192,  192,  192,  192,  180,  181,  182,  183,  180,  181,
 /*  4220 */   182,  183,  192,  137,  192,  192,  192,  192,  192,  192,
 /*  4230 */   192,  192,  146,  147,  148,  149,  150,  151,  152,  153,
 /*  4240 */   154,  155,  156,  157,  158,  159,  160,  161,  162,  163,
 /*  4250 */   164,  165,  166,  115,  116,  192,  170,  192,  192,  192,
 /*  4260 */   170,  192,  192,  192,  192,  192,  180,  181,  182,  183,
 /*  4270 */   180,  181,  182,  183,  192,  137,  192,  192,  192,  192,
 /*  4280 */   192,  192,  192,  192,  146,  147,  148,  149,  150,  151,
 /*  4290 */   152,  153,  154,  155,  156,  157,  158,  159,  160,  161,
 /*  4300 */   162,  163,  164,  165,  166,  115,  116,  192,  170,  192,
 /*  4310 */   192,  192,  170,  192,  192,  192,  192,  192,  180,  181,
 /*  4320 */   182,  183,  180,  181,  182,  183,  192,  137,  192,  192,
 /*  4330 */   192,  192,  192,  192,  192,  192,  146,  147,  148,  149,
 /*  4340 */   150,  151,  152,  153,  154,  155,  156,  157,  158,  159,
 /*  4350 */   160,  161,  162,  163,  164,  165,  166,  115,  116,  192,
 /*  4360 */   170,  192,  192,  192,  170,  192,  192,  192,  192,  192,
 /*  4370 */   180,  181,  182,  183,  180,  181,  182,  183,  192,  137,
 /*  4380 */   192,    1,    2,    3,    4,    5,  192,  192,  146,  147,
 /*  4390 */   148,  149,  150,  151,  152,  153,  154,  155,  156,  157,
 /*  4400 */   158,  159,  160,  161,  162,  163,  164,  165,  166,  115,
 /*  4410 */   116,  192,  170,  192,  192,  192,  192,  192,  192,  192,
 /*  4420 */   192,  192,  180,  181,  182,  183,  192,   47,   48,  192,
 /*  4430 */    50,   51,  192,   53,  192,  192,  192,   57,  192,  192,
 /*  4440 */   146,  147,  148,  149,  150,  151,  152,  153,  154,  155,
 /*  4450 */   156,  157,  158,  159,  160,  161,  162,  163,  164,  165,
 /*  4460 */   166,  192,  192,  192,  170,    7,  192,    9,   10,   11,
 /*  4470 */    12,  170,  192,  192,  180,  181,  182,  183,   20,  192,
 /*  4480 */   192,  180,  181,  182,  183,  192,  192,  192,  192,  192,
 /*  4490 */    32,   33,   34,   35,   36,   37,   38,  192,    7,   41,
 /*  4500 */     9,   10,   11,   12,  192,  192,  192,   49,  170,  192,
 /*  4510 */   192,   20,   54,  192,   56,  192,  192,  192,  180,  181,
 /*  4520 */   182,  183,  192,   32,   33,   34,   35,   36,   37,   38,
 /*  4530 */   192,  192,   41,  192,  192,  192,  192,  192,  192,  192,
 /*  4540 */    49,   40,  192,  170,   43,   54,   45,   56,   47,   48,
 /*  4550 */    92,  192,   51,  180,  181,  182,  183,  192,  100,  101,
 /*  4560 */   102,  103,  104,  105,   13,   14,   15,  192,  192,   18,
 /*  4570 */    19,   20,   21,   22,   23,   24,   25,   26,   27,   28,
 /*  4580 */    29,   30,   31,   92,  192,  192,  192,  192,  192,   38,
 /*  4590 */    39,  100,  101,  102,  103,  104,  105,  192,  170,    7,
 /*  4600 */   192,    9,   10,   11,   12,  192,  192,  106,  180,  181,
 /*  4610 */   182,  183,   20,  192,  192,  192,  192,  192,  192,  192,
 /*  4620 */   192,  192,  192,  192,   32,   33,   34,   35,   36,  192,
 /*  4630 */    38,  192,    7,   41,    9,   10,   11,   12,  192,  192,
 /*  4640 */   192,   49,  170,  192,  192,   20,   54,  192,   56,  192,
 /*  4650 */   192,   59,  180,  181,  182,  183,  192,   32,   33,   34,
 /*  4660 */    35,   36,   37,   38,  192,  170,   41,  192,  192,  192,
 /*  4670 */   192,  192,  192,  192,   49,  180,  181,  182,  183,   54,
 /*  4680 */   192,   56,  192,  192,   92,  192,  192,  192,  192,  192,
 /*  4690 */   192,  192,  100,  101,  102,  103,  104,  105,  192,   14,
 /*  4700 */    15,  192,  192,   18,   19,   20,   21,   22,   23,   24,
 /*  4710 */    25,   26,   27,   28,   29,   30,   31,   92,  192,  192,
 /*  4720 */   192,  192,  192,   38,   39,  100,  101,  102,  103,  104,
 /*  4730 */   105,  170,  192,    7,  192,    9,   10,   11,   12,  192,
 /*  4740 */   170,  180,  181,  182,  183,  192,   20,  192,  192,  192,
 /*  4750 */   180,  181,  182,  183,  192,  192,  192,  192,   32,   33,
 /*  4760 */    34,   35,   36,   37,   38,  192,    7,   41,    9,   10,
 /*  4770 */    11,   12,  192,  170,  192,   49,  192,  192,  192,   20,
 /*  4780 */    54,  192,   56,  180,  181,  182,  183,  192,  192,  192,
 /*  4790 */   192,   32,   33,   34,   35,   36,   37,   38,  192,  170,
 /*  4800 */    41,  192,  192,  192,  192,  192,  192,  192,   49,  180,
 /*  4810 */   181,  182,  183,   54,  192,   56,  192,  192,   92,  192,
 /*  4820 */   192,  192,  192,  192,  192,  192,  100,  101,  102,  103,
 /*  4830 */   104,  105,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  4840 */    26,   27,   28,   29,   30,   31,  192,  192,  192,  192,
 /*  4850 */   192,   92,   38,   39,  192,  192,  192,  192,  192,  100,
 /*  4860 */   101,  102,  103,  104,  105,  170,  192,    7,  192,    9,
 /*  4870 */    10,   11,   12,  192,  170,  180,  181,  182,  183,  192,
 /*  4880 */    20,  192,  192,  192,  180,  181,  182,  183,  192,  192,
 /*  4890 */   192,  192,   32,   33,   34,   35,   36,   37,   38,  192,
 /*  4900 */     7,   41,    9,   10,   11,   12,  192,  192,  192,   49,
 /*  4910 */   192,  170,  192,   20,   54,  192,   56,  192,  192,   38,
 /*  4920 */    39,  180,  181,  182,  183,   32,   33,   34,   35,   36,
 /*  4930 */    37,   38,  192,   52,   41,  192,  192,  192,  192,  192,
 /*  4940 */   192,  192,   49,  192,  170,  192,  192,   54,  192,   56,
 /*  4950 */   192,  192,   92,  192,  180,  181,  182,  183,  192,  192,
 /*  4960 */   100,  101,  102,  103,  104,  105,  192,   86,   87,   88,
 /*  4970 */    89,   90,   91,  192,   93,   94,   95,  192,  192,  170,
 /*  4980 */   192,  192,  192,  192,  192,   92,  192,  192,  192,  180,
 /*  4990 */   181,  182,  183,  100,  101,  102,  103,  104,  105,  170,
 /*  5000 */   192,    7,  192,    9,   10,   11,   12,  192,  192,  180,
 /*  5010 */   181,  182,  183,  192,   20,  192,  192,  192,  192,  192,
 /*  5020 */   192,  192,  192,  192,  192,  192,   32,   33,   34,   35,
 /*  5030 */    36,   37,   38,  192,    7,   41,    9,   10,   11,   12,
 /*  5040 */   192,  192,  192,   49,  192,  192,  192,   20,   54,  192,
 /*  5050 */    56,  192,  192,  192,  192,  192,  192,  192,  192,   32,
 /*  5060 */    33,   34,   35,   36,   37,   38,  192,  192,   41,  192,
 /*  5070 */   192,  192,  192,  192,  192,  192,   49,  192,  192,  192,
 /*  5080 */   192,   54,  192,   56,  192,  192,   92,  192,  192,  192,
 /*  5090 */   192,  192,  192,  192,  100,  101,  102,  103,  104,  105,
 /*  5100 */     1,    2,    3,    4,    5,  192,  192,  192,  192,  192,
 /*  5110 */   192,  192,  192,  192,  192,  192,  192,  192,  192,   92,
 /*  5120 */   192,  192,  192,  192,  192,  192,  192,  100,  101,  102,
 /*  5130 */   103,  104,  105,  192,  192,    7,  192,    9,   10,   11,
 /*  5140 */    12,  192,  192,  192,  192,  192,   47,   48,   20,   50,
 /*  5150 */    51,  192,   53,  192,  192,  192,   57,  192,  192,  192,
 /*  5160 */    32,   33,   34,   35,   36,   37,   38,  192,    7,   41,
 /*  5170 */     9,   10,   11,   12,  192,  192,  192,   49,  192,  192,
 /*  5180 */   192,   20,   54,  192,   56,  192,  192,  192,  192,  192,
 /*  5190 */   192,  192,  192,   32,   33,   34,   35,   36,  192,   38,
 /*  5200 */   192,  192,   41,  192,  192,  192,  192,  192,  192,  192,
 /*  5210 */    49,  192,  192,  192,  192,   54,  192,   56,  192,  192,
 /*  5220 */    92,  192,  192,  192,  192,  192,  192,  192,  100,  101,
 /*  5230 */   102,  103,  104,  105,    1,    2,    3,    4,    5,  192,
 /*  5240 */   192,  192,  192,  192,  192,   84,  192,  192,  192,  192,
 /*  5250 */   192,  192,  192,   92,  192,  192,  192,  192,  192,  192,
 /*  5260 */   192,  100,  101,  102,  103,  104,  105,  192,  192,    7,
 /*  5270 */   192,    9,   10,   11,   12,  192,  192,  192,  192,  192,
 /*  5280 */    47,   48,   20,   50,   51,  192,   53,  192,  192,  192,
 /*  5290 */    57,  192,  192,  192,   32,   33,   34,   35,   36,  192,
 /*  5300 */    38,  192,  192,   41,    7,  192,    9,   10,   11,   12,
 /*  5310 */   192,   49,  192,  192,  192,  192,   54,   20,   56,  192,
 /*  5320 */   192,   59,  192,  192,  192,  192,  192,  192,   20,   32,
 /*  5330 */    33,   34,   35,   36,  192,   38,  192,  192,   41,  192,
 /*  5340 */   192,  192,  192,  192,  192,   37,   49,  192,  192,   41,
 /*  5350 */   192,   54,  192,   56,   92,  192,   59,  192,  192,  192,
 /*  5360 */   192,  192,  100,  101,  102,  103,  104,  105,   60,   61,
 /*  5370 */    62,   63,   64,   65,   66,   67,   68,   69,   70,    1,
 /*  5380 */     2,    3,    4,    5,  192,  192,  192,  192,  192,   92,
 /*  5390 */   192,  192,  192,  192,  192,  192,  192,  100,  101,  102,
 /*  5400 */   103,  104,  105,  192,  192,    7,  192,    9,   10,   11,
 /*  5410 */    12,  192,  192,  192,  192,  192,  192,  192,   20,  192,
 /*  5420 */   192,  192,  192,  192,  192,   47,   48,  192,   50,   51,
 /*  5430 */    32,   33,   34,   35,   36,   57,   38,  192,    7,   41,
 /*  5440 */     9,   10,   11,   12,  192,  192,  192,   49,  192,  192,
 /*  5450 */   192,   20,   54,  192,   56,  192,  192,   59,  192,  192,
 /*  5460 */   192,  192,  192,   32,   33,   34,   35,   36,   20,   38,
 /*  5470 */   192,  192,   41,   42,  192,  192,  192,  192,  192,  192,
 /*  5480 */    49,  192,  192,  192,  192,   54,  192,   56,  192,  192,
 /*  5490 */    92,  192,  192,  192,  192,  192,  192,  192,  100,  101,
 /*  5500 */   102,  103,  104,  105,  192,  192,   58,  192,   60,   61,
 /*  5510 */    62,   63,   64,   65,   66,   67,   68,   69,   70,  192,
 /*  5520 */   192,  192,  192,   92,  192,  192,  192,  192,  192,  192,
 /*  5530 */   192,  100,  101,  102,  103,  104,  105,  192,  192,    7,
 /*  5540 */   192,    9,   10,   11,   12,  192,  192,  192,  192,  192,
 /*  5550 */   192,  192,   20,  192,  192,  192,  192,  192,  192,  192,
 /*  5560 */   192,  192,  192,  192,   32,   33,   34,   35,   36,  192,
 /*  5570 */    38,  192,    7,   41,    9,   10,   11,   12,  192,  192,
 /*  5580 */   192,   49,  192,  192,  192,   20,   54,  192,   56,  192,
 /*  5590 */   192,  192,  192,  192,  192,  192,  192,   32,   33,   34,
 /*  5600 */    35,   36,   20,   38,  192,  192,   41,  192,  192,  192,
 /*  5610 */   192,  192,  192,  192,   49,  192,   84,  192,  192,   54,
 /*  5620 */   192,   56,  192,   41,   92,  192,  192,  192,  192,  192,
 /*  5630 */   192,  192,  100,  101,  102,  103,  104,  105,  192,  192,
 /*  5640 */   192,  192,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  5650 */    68,   69,   70,  192,  192,  192,  192,   92,  192,  192,
 /*  5660 */   192,  192,  192,  192,  192,  100,  101,  102,  103,  104,
 /*  5670 */   105,  192,  192,    7,  192,    9,   10,   11,   12,  192,
 /*  5680 */   192,  192,  192,  192,  192,  192,   20,  192,  192,  192,
 /*  5690 */   192,  192,  192,  192,  192,  192,  192,  192,   32,   33,
 /*  5700 */    34,   35,   36,  192,   38,  192,  192,   41,  192,  192,
 /*  5710 */   192,  192,  192,  192,  192,   49,  192,  192,  192,  192,
 /*  5720 */    54,  192,   56,  192,    8,  192,  192,  192,  192,   13,
 /*  5730 */    14,   15,  192,  192,   18,   19,   20,   21,   22,   23,
 /*  5740 */    24,   25,   26,   27,   28,   29,   30,   31,  192,  192,
 /*  5750 */   192,  192,  192,  192,   38,   39,  192,  192,   92,  192,
 /*  5760 */   192,  192,  192,  192,  192,  192,  100,  101,  102,  103,
 /*  5770 */   104,  105,  192,  192,  192,   59,    8,  192,  192,  192,
 /*  5780 */   192,   13,   14,   15,  192,  192,   18,   19,   20,   21,
 /*  5790 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  5800 */   192,  192,  192,    8,  192,  192,   38,   39,   13,   14,
 /*  5810 */    15,  192,  192,   18,   19,   20,   21,   22,   23,   24,
 /*  5820 */    25,   26,   27,   28,   29,   30,   31,  192,  192,  192,
 /*  5830 */   192,  192,   37,   38,   39,  192,  192,  192,  192,  192,
 /*  5840 */   192,    8,  192,  192,  192,   77,   13,   14,   15,  192,
 /*  5850 */   192,   18,   19,   20,   21,   22,   23,   24,   25,   26,
 /*  5860 */    27,   28,   29,   30,   31,  192,  192,  192,  192,  192,
 /*  5870 */   192,   38,   39,    8,  192,  192,  192,  192,   13,   14,
 /*  5880 */    15,  192,   49,   18,   19,   20,   21,   22,   23,   24,
 /*  5890 */    25,   26,   27,   28,   29,   30,   31,  192,  192,  192,
 /*  5900 */     8,  192,  192,   38,   39,   13,   14,   15,  192,  192,
 /*  5910 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  5920 */    28,   29,   30,   31,   59,  192,  192,  192,  192,  192,
 /*  5930 */    38,   39,    8,  192,   42,  192,  192,   13,   14,   15,
 /*  5940 */   192,  192,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  5950 */    26,   27,   28,   29,   30,   31,  192,  192,  192,  192,
 /*  5960 */   192,  192,   38,   39,    8,  192,   42,  192,  192,   13,
 /*  5970 */    14,   15,  192,  192,   18,   19,   20,   21,   22,   23,
 /*  5980 */    24,   25,   26,   27,   28,   29,   30,   31,  192,  192,
 /*  5990 */   192,  192,  192,  192,   38,   39,    8,  192,   42,  192,
 /*  6000 */   192,   13,   14,   15,  192,  192,   18,   19,   20,   21,
 /*  6010 */    22,   23,   24,   25,   26,   27,   28,   29,   30,   31,
 /*  6020 */   192,  192,  192,  192,  192,  192,   38,   39,    8,  192,
 /*  6030 */    42,  192,  192,   13,   14,   15,  192,  192,   18,   19,
 /*  6040 */    20,   21,   22,   23,   24,   25,   26,   27,   28,   29,
 /*  6050 */    30,   31,  192,  192,  192,  192,  192,  192,   38,   39,
 /*  6060 */     8,  192,  192,  192,  192,   13,   14,   15,  192,   49,
 /*  6070 */    18,   19,   20,   21,   22,   23,   24,   25,   26,   27,
 /*  6080 */    28,   29,   30,   31,  192,  192,  192,  192,  192,  192,
 /*  6090 */    38,   39,    8,  192,  192,  192,  192,   13,   14,   15,
 /*  6100 */   192,   49,   18,   19,   20,   21,   22,   23,   24,   25,
 /*  6110 */    26,   27,   28,   29,   30,   31,  192,  192,  192,  192,
 /*  6120 */   192,  192,   38,   39,    8,  192,  192,  192,  192,   13,
 /*  6130 */    14,   15,   20,   49,   18,   19,   20,   21,   22,   23,
 /*  6140 */    24,   25,   26,   27,   28,   29,   30,   31,  192,   37,
 /*  6150 */    20,  192,  192,   41,   38,   39,  192,  192,  192,  192,
 /*  6160 */   192,  192,  192,  192,  192,  192,   20,   37,  192,  192,
 /*  6170 */   192,   41,   60,   61,   62,   63,   64,   65,   66,   67,
 /*  6180 */    68,   69,   70,   37,  192,  192,  192,   41,   20,  192,
 /*  6190 */    60,   61,   62,   63,   64,   65,   66,   67,   68,   69,
 /*  6200 */    70,    1,    2,    3,    4,    5,   60,   61,   62,   63,
 /*  6210 */    64,   65,   66,   67,   68,   69,   70,    1,    2,    3,
 /*  6220 */     4,    5,  192,  192,  192,  192,  192,  192,   60,   61,
 /*  6230 */    62,   63,   64,   65,   66,   67,   68,   69,   70,    1,
 /*  6240 */     2,    3,    4,    5,  192,  192,  192,   47,   48,  192,
 /*  6250 */    50,   51,  192,   53,  192,  192,  192,   57,  192,  192,
 /*  6260 */   192,  192,  192,   47,   48,  192,   50,   51,  192,   53,
 /*  6270 */   192,  192,  192,   57,  192,    1,    2,    3,    4,    5,
 /*  6280 */     1,    2,    3,    4,    5,   47,   48,  192,   50,   51,
 /*  6290 */   192,   53,  192,  192,  192,   57,  192,    1,    2,    3,
 /*  6300 */     4,    5,    1,    2,    3,    4,    5,  192,  192,  192,
 /*  6310 */   192,  192,  192,  192,  192,  192,  192,  192,  192,  192,
 /*  6320 */   192,   47,   48,  192,   50,   51,   47,   48,  192,   50,
 /*  6330 */    51,   57,  192,  192,  192,  192,   57,  192,    1,    2,
 /*  6340 */     3,    4,    5,   47,   48,  192,   50,   51,   47,   48,
 /*  6350 */   192,   50,   51,   57,  192,  192,  192,  192,   57,  192,
 /*  6360 */     1,    2,    3,    4,    5,  192,  192,  192,  192,  192,
 /*  6370 */   192,  192,  192,  192,  192,  192,  192,  192,  192,  192,
 /*  6380 */   192,  192,  192,  192,   47,   48,  192,   50,   51,  192,
 /*  6390 */   192,  192,  192,  192,   57,  192,  192,  192,  192,  192,
 /*  6400 */   192,  192,  192,  192,  192,  192,   47,   48,  192,   50,
 /*  6410 */    51,  192,  192,  192,  192,  192,   57,
};
#define YY_SHIFT_USE_DFLT (-56)
static short yy_shift_ofst[] = {
 /*     0 */  4501,   30, 3267,  -56,  -56,  -56,  -56,  -56,  -56,  -29,
 /*    10 */     8,  -56,   21,  138,  -56,  207,  239,  -56, 4380,  -56,
 /*    20 */   351,  -56, 5099, 5378,  -56, 3593,  442,  415, 5308,  306,
 /*    30 */  5448,  470,  -56,  -56,  505, 6168,  -56,  526,  -56,  -56,
 /*    40 */   -56,  496,  150,  -56,  552,  641,  -56,  -56,  -56,  -56,
 /*    50 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,
 /*    60 */   174,  337, 5448,  672,  -56,  -56, 5582,  -56,  631,  -21,
 /*    70 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  640,  685,
 /*    80 */   -56,  -56,  681,  -56,  246,  694,  -21,  -56,  709,  715,
 /*    90 */   -21,  -56,  729,  719,  -21,  -56,  742,  763,  -56,  -56,
 /*   100 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56, 3905, 3599,
 /*   110 */   747,  745, 6112,  341, 5448,  766,  -56,  -56,  204,  382,
 /*   120 */  5448,  768,  -56,  -56,  -56,  -56,  -56, 3697,  757,  760,
 /*   130 */   -21,  772,  -56,  767,  770,  -21,  788,  -56, 6274,  -56,
 /*   140 */   791,  338,  -56,  792,  384,  -56,  796,  231,  -56, 5233,
 /*   150 */   -56,  790,  -56, 6200, 6279,  -56, 3704,  800,  789, 6130,
 /*   160 */   173,   -7,  -56,   93,  -56,  -56,  -56,  -56,  -56,  -56,
 /*   170 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,
 /*   180 */   -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  -56,  804,
 /*   190 */   -56,  806,  -56, 5565,  802,  193,  293,  776,  803,  393,
 /*   200 */   493,  -56, 5565,  813,   74,  -56,  274,  -56,  -56,  -21,
 /*   210 */   794, 3193, 3193,  827,  593,  693,  -56, 5565,  828,  793,
 /*   220 */   893,  -56,  834,  993, 1093,  808, 5565,  842,  -56, 5565,
 /*   230 */   232, 5565, 5565,  232, 5565, 5565, 6116, 5565, 5565, 4551,
 /*   240 */  5565, 5565, 4551, 5565, 5565, 4551, 5565, 5565, 4551, 5565,
 /*   250 */   -20, 5565,  -20,  321,  845, 4458, 6116, 5565, 5716,  -56,
 /*   260 */  5565,  -15, 5565,  -15, 5565,  232, 5565,  232, 5565,  232,
 /*   270 */  5565,  -15, 5565, 4814, 5565, 4814, 5565, 4685, 5565, 5768,
 /*   280 */  5565, 6116, 3293, 5795,  -56,  870, 5565,  232,  867,  888,
 /*   290 */  5565,  -20,  258,   47,  854, 4491,  871,  905, 5565,  232,
 /*   300 */   -56, 5565,  232,  -56,  -56,  -56,  -56,  -56,  -56,  -56,
 /*   310 */   -56, 4592, 6116,  -55, 4625,  373,  858, 4726,  -56,  295,
 /*   320 */   -56, 5565,  874,  872,  330, 4759,  340,  -56,  -56,  -56,
 /*   330 */   -56,  -56,  -56,  391,  864, 4860,  401,  -56,  -56,  880,
 /*   340 */   882,  877, 4893,  430,  -56,  -56,  -56,  889,  886,  881,
 /*   350 */  4994,  -56,  439,  -56,  464,  -56,  -56,  861,  862,  -56,
 /*   360 */   142,  -56, 5666,  -56,  863, 5565,  -56,  -56,  -56,  471,
 /*   370 */   -56,  -56,  -56,  478,  -56,  -56,  396,  891,  890, 5027,
 /*   380 */   501,  -56,  -56,  895,  892, 5128,  504,  -56,  -56,  -20,
 /*   390 */   -20,  -20,  -20,  -20,  -20, 6116,  909,  456, 5161, 5833,
 /*   400 */  3193, 1193,  -56,  481,   70,  -56,  481,  -56, 4881,  -56,
 /*   410 */   -56,  -56,  -56,  -56,  -56,  -56, 5565,  -56, 6116,  491,
 /*   420 */   129, 5565,  -56, 5262,  928, 5565,  -56,  903,  -56, 5865,
 /*   430 */   229, 5565,  -56,  -56, 5565,  -56,  -56,  -56,  479,  902,
 /*   440 */   928, 5565,  -56,  917,  928, 5565,  -56,  935,  329, 5565,
 /*   450 */   -56, 5297,  928, 5565,  -56,  229, 5565,  -56, 5398,  928,
 /*   460 */  5565,  -56,  229, 5565,  -56,  -56,  -56,  936,  919,  928,
 /*   470 */  5565,  -56,  -56, 5565,  546,  -56, 5565,  -56, 6116,  -56,
 /*   480 */   937,  -56,  944,  -56,  959,  -56,  965,  -56, 5431, 5892,
 /*   490 */   -56,  -56, 5565, 5924,  -56, 5565, 5956,  -56, 5565, 5988,
 /*   500 */   -56,  969,  570,  -56,  969,  -56,  960,  -21,  -56,  -56,
 /*   510 */   969,  595,  -56,  969,  608,  -56,  969,  609,  -56,  969,
 /*   520 */   634,  -56,  969,  635,  -56,  969,  642,  -56,  969,  646,
 /*   530 */   -56,  969,  695,  -56,  969,  706,  -56, 6116,  -56,  -56,
 /*   540 */   -56, 5565, 6020, 3193, 1293,  -56,  979,  938, 5532, 6052,
 /*   550 */  3193, 1393,  -56, 5565, 6084, 3193, 1493,  -56,  942, 5565,
 /*   560 */   982,  -56,  -56,  -56,  953, 3193, 3193,  -56,  -56,  958,
 /*   570 */   984, 1593, 1693,  -56,  -56, 5448,  987, 1793,  -56, 1893,
 /*   580 */   -56,  540,  213, 1993,  -56, 2093,  -56, 5448,  988, 2193,
 /*   590 */   -56, 2293,  -56, 3905, 3808,  997,  983, 6146,  284, 2393,
 /*   600 */   -56, 2493,  -56, 5448,  991, 2593,  -56, 2693,  -56,  630,
 /*   610 */   581, 2793,  -56, 2893,  -56, 5448,  992, 2993,  -56, 3093,
 /*   620 */   -56, 6216, 6296,  -56, 3697,  -56, 3697, 3335,   81,  -56,
 /*   630 */   -21,  120,  -56, 1006,  -56,   36, 1009,  726, 1010,  711,
 /*   640 */   -56,  -56, 1026,  -56,  -56, 1027,  -56, 3437,  227,  -56,
 /*   650 */   -21,  172,  -56, 1035,  -56, 1045,  -56, 6238, 3495, 6301,
 /*   660 */  3905, 6337,  -56, 6359,  796,  -56,  -56,  -56,  796,  231,
 /*   670 */   -56, 1056, 1065,  465,  -56, 1068, 1061,  -56,  796,  231,
 /*   680 */   -56, 1066, 1080,  671,  -56, 1082, 1061,  -56,  -56,
};
#define YY_REDUCE_USE_DFLT (-108)
static short yy_reduce_ofst[] = {
 /*     0 */     6, -108,   -5, -108, -108, -108, -108, -108, -108, -108,
 /*    10 */  -108, -108, -108,   19, -108, -108,  190, -108, 3877, -108,
 /*    20 */  -108, -108,  313,  -87, -108,  249, -108, -108,   -4, -108,
 /*    30 */   168, -108, -108, -108, -108,   65, -108, -108, -108, -108,
 /*    40 */  -108, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*    50 */  -108, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*    60 */  -108, -108,  268, -108, -108, -108,   41, -108, -108,  592,
 /*    70 */  -108, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*    80 */  -108, -108, -108, -108, -108, -108,  616, -108, -108, -108,
 /*    90 */   622, -108, -108, -108,  648, -108, -108, -108, -108, -108,
 /*   100 */  -108, -108, -108, -108, -108, -108, -108, -108, -107,  249,
 /*   110 */  -108, -108,   34, -108,  974, -108, -108, -108, -108, -108,
 /*   120 */  1174, -108, -108, -108, -108, -108, -108, -107, -108, -108,
 /*   130 */   686, -108, -108, -108, -108,  691, -108, -108,  -87, -108,
 /*   140 */  -108,  718, -108, -108,  720, -108,  250,  721, -108, 3669,
 /*   150 */  -108, -108, -108,  -88,  112, -108,  249, -108, -108,  141,
 /*   160 */  -108, 3254, -108, 4294, -108, -108, -108, -108, -108, -108,
 /*   170 */  -108, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*   180 */  -108, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*   190 */  -108, -108, -108, 1000, -108, 3306, 4294, -108, -108, 3358,
 /*   200 */  4294, -108, 1100, -108,  153, -108,  698, -108, -108,  741,
 /*   210 */  -108, 3410, 4294, -108, 3462, 4294, -108, 1200, -108, 3514,
 /*   220 */  4294, -108, -108, 3566, 4294, -108, 1300, -108, -108,  801,
 /*   230 */  -108, 1799, 1899, -108, 1999, 2099, -108, 2199, 2299, -108,
 /*   240 */  2399, 2499, -108, 2599, 2699, -108, 2799, 2899, -108, 2936,
 /*   250 */  -108, 2999, -108, -108, -108,  299, -108, 3036, -108, -108,
 /*   260 */  3099, -108, 3934, -108, 3986, -108, 4038, -108, 4090, -108,
 /*   270 */  4142, -108, 4194, -108, 4301, -108, 4338, -108, 4373, -108,
 /*   280 */  4428, -108, 1399, -108, -108, -108, 4472, -108, -108, -108,
 /*   290 */  4495, -108, -108, -108, -108,  336, -108, -108, 4561, -108,
 /*   300 */  -108, 4570, -108, -108, -108, -108, -108, -108, -108, -108,
 /*   310 */  -108, 3198, -108, -108,  399, -108, -108,  436, -108, -108,
 /*   320 */  -108, 1236, -108, -108, -108,  499, -108, -108, -108, -108,
 /*   330 */  -108, -108, -108, -108, -108,  536, -108, -108, -108, -108,
 /*   340 */  -108, -108,  599, -108, -108, -108, -108, -108, -108, -108,
 /*   350 */   636, -108, -108, -108, -108, -108, -108, -108, -108, -108,
 /*   360 */  -108, -108, 3778, -108, -108, 3882, -108, -108, -108, -108,
 /*   370 */  -108, -108, -108, -108, -108, -108, -108, -108, -108,  699,
 /*   380 */  -108, -108, -108, -108, -108,  736, -108, -108, -108, -108,
 /*   390 */  -108, -108, -108, -108, -108, -108, -108, -108, 4603, -108,
 /*   400 */  3618, 4294, -108,  279, -108, -108,  779, -108, -106, -108,
 /*   410 */  -108, -108, -108, -108, -108, -108, 1336, -108, -108, -108,
 /*   420 */    57, 1436, -108, 1499,  797, 1536, -108, -108, -108, -108,
 /*   430 */   360, 1636, -108, -108, 1499, -108, -108, -108, -108, -108,
 /*   440 */   814, 1736, -108, -108,  815, 1836, -108, -108,  165, 1936,
 /*   450 */  -108, 1499,  833, 2036, -108,  372, 2136, -108, 1499,  835,
 /*   460 */  2236, -108,  394, 2336, -108, -108, -108, -108, -108,  836,
 /*   470 */  2436, -108, -108,  901, -108, -108, 1600, -108, -108, -108,
 /*   480 */  -108, -108, -108, -108, -108, -108, -108, -108, 4629, -108,
 /*   490 */  -108, -108, 4695, -108, -108, 4704, -108, -108, 4741, -108,
 /*   500 */  -108,  366, -108, -108,  826, -108, -108,  894, -108, -108,
 /*   510 */   423, -108, -108,  425, -108, -108,  437, -108, -108,  448,
 /*   520 */  -108, -108,  454, -108, -108,  461, -108, -108,  486, -108,
 /*   530 */  -108,  523, -108, -108,  525, -108, -108, -108, -108, -108,
 /*   540 */  -108, 4774, -108, 3670, 4294, -108, -108, -108, 4809, -108,
 /*   550 */  3722, 4294, -108, 4829, -108, 3774, 4294, -108, -108, 1700,
 /*   560 */  -108, -108, -108, -108, -108, 3826, 4294, -108, -108, -108,
 /*   570 */  -108, 3878, 4294, -108, -108, 2574, -108, 3930, -108, 4294,
 /*   580 */  -108, -108, -108, 3982, -108, 4294, -108, 2674, -108, 4034,
 /*   590 */  -108, 4294, -108,  583,  249, -108, -108,  194, -108, 4086,
 /*   600 */  -108, 4294, -108, 2774, -108, 4138, -108, 4294, -108, -108,
 /*   610 */  -108, 4190, -108, 4294, -108, 2874, -108, 4242, -108, 4294,
 /*   620 */  -108,   12,  112, -108,  583, -108,  602,  249,  915, -108,
 /*   630 */   921,  916, -108, -108, -108,  452, -108, -108, -108,  930,
 /*   640 */  -108, -108, -108, -108, -108, -108, -108,  249,  940, -108,
 /*   650 */   947,  956, -108, -108, -108, -108, -108, 1083,  111,  112,
 /*   660 */   602,  112, -108,  112,  967, -108, -108, -108,  617,  989,
 /*   670 */  -108, -108, -108,  990, -108, -108, 1001, -108,  688, 1002,
 /*   680 */  -108, -108, -108, 1004, -108, -108, 1012, -108, -108,
};
static YYACTIONTYPE yy_default[] = {
 /*     0 */  1023, 1023, 1023,  691,  693,  694,  695,  696,  697, 1023,
 /*    10 */  1023,  698, 1023, 1023,  699, 1023, 1023,  700, 1023,  715,
 /*    20 */  1023,  716, 1023, 1023,  749, 1023, 1023, 1023, 1023, 1023,
 /*    30 */  1023, 1023,  769,  787,  788, 1023,  789,  791,  792,  793,
 /*    40 */   794, 1023, 1023,  805, 1023, 1023,  806,  807,  808,  809,
 /*    50 */   810,  811,  812,  813,  814,  815,  816,  817,  790,  773,
 /*    60 */  1023, 1023, 1023, 1023,  770,  774, 1023,  795,  797, 1023,
 /*    70 */   801, 1010, 1011, 1012, 1013, 1014, 1015, 1016, 1023, 1023,
 /*    80 */  1017, 1018, 1023, 1019, 1023,  798, 1023,  802, 1023,  799,
 /*    90 */  1023,  803, 1023,  800, 1023,  804, 1023, 1023,  796,  777,
 /*   100 */   779,  780,  781,  782,  783,  784,  785,  786, 1023, 1023,
 /*   110 */  1023, 1023, 1023, 1023, 1023, 1023,  771,  775, 1023, 1023,
 /*   120 */  1023, 1023,  772,  776,  778,  745,  750, 1023, 1023, 1023,
 /*   130 */  1023, 1023,  751, 1023, 1023, 1023, 1023,  752, 1023,  746,
 /*   140 */  1023, 1023,  701, 1023, 1023,  702, 1023, 1023,  704, 1023,
 /*   150 */   710, 1023,  711, 1023, 1023,  747, 1023, 1023, 1023, 1023,
 /*   160 */  1023, 1023,  753, 1023,  755,  818,  820,  821,  822,  823,
 /*   170 */   824,  825,  826,  827,  828,  829,  830,  831,  832,  833,
 /*   180 */   834,  835,  836,  837,  838,  839,  840,  841,  842, 1023,
 /*   190 */   843, 1023,  844, 1023, 1023, 1023, 1023,  847, 1023, 1023,
 /*   200 */  1023,  848, 1023, 1023, 1023,  851, 1023,  852,  853, 1023,
 /*   210 */  1023,  855,  856, 1023, 1023, 1023,  859, 1023, 1023, 1023,
 /*   220 */  1023,  861, 1023, 1023, 1023, 1023, 1023, 1023,  863, 1023,
 /*   230 */   926, 1023, 1023,  927, 1023, 1023,  928, 1023, 1023,  929,
 /*   240 */  1023, 1023,  930, 1023, 1023,  931, 1023, 1023,  932, 1023,
 /*   250 */   939, 1023,  940, 1023,  944, 1023, 1001, 1023, 1023,  949,
 /*   260 */  1023,  950, 1023,  951, 1023,  952, 1023,  953, 1023,  954,
 /*   270 */  1023,  955, 1023,  956, 1023,  957, 1023,  958, 1023, 1023,
 /*   280 */  1023,  998, 1023, 1023,  941, 1023, 1023,  942, 1023, 1023,
 /*   290 */  1023,  943,  962, 1023,  947, 1023, 1023, 1023, 1023,  959,
 /*   300 */   960, 1023,  961,  963,  964,  965,  966,  967,  968,  969,
 /*   310 */   970, 1023, 1009,  962, 1023, 1023,  973, 1023,  974, 1023,
 /*   320 */   975, 1023, 1023, 1023, 1023, 1023, 1023,  981,  982,  995,
 /*   330 */   996,  997, 1000, 1023, 1023, 1023, 1023,  985,  986, 1023,
 /*   340 */  1023, 1023, 1023, 1023,  987,  988,  999, 1023, 1023,  976,
 /*   350 */  1023,  977, 1023,  978, 1023,  979,  980,  963,  964,  971,
 /*   360 */  1023,  972, 1023, 1002, 1023, 1023, 1004, 1005, 1003, 1023,
 /*   370 */   983,  984,  948, 1023,  989,  990, 1023, 1023,  945, 1023,
 /*   380 */  1023,  991,  992, 1023,  946, 1023, 1023,  993,  994,  938,
 /*   390 */   937,  936,  935,  934,  933, 1020, 1023, 1023, 1023, 1023,
 /*   400 */  1023, 1023,  864, 1023, 1023,  868, 1023,  869, 1023,  871,
 /*   410 */   872,  873,  874,  875,  876,  877, 1023,  878,  925, 1023,
 /*   420 */  1023, 1023,  879, 1023, 1023, 1023,  882, 1023,  891, 1023,
 /*   430 */  1023, 1023,  883,  889, 1023,  890,  892,  893, 1023, 1023,
 /*   440 */  1023, 1023,  880, 1023, 1023, 1023,  881, 1023, 1023, 1023,
 /*   450 */   884, 1023, 1023, 1023,  885, 1023, 1023,  886, 1023, 1023,
 /*   460 */  1023,  887, 1023, 1023,  888,  894,  895, 1023, 1023, 1023,
 /*   470 */  1023,  896,  870, 1023, 1023,  898, 1023,  899,  901,  900,
 /*   480 */   995,  902,  997,  903,  996,  904,  960,  905, 1023, 1023,
 /*   490 */   906,  907, 1023, 1023,  908, 1023, 1023,  909, 1023, 1023,
 /*   500 */   910, 1023, 1023,  911, 1023,  921,  923, 1023,  924,  922,
 /*   510 */  1023, 1023,  912, 1023, 1023,  913, 1023, 1023,  914, 1023,
 /*   520 */  1023,  915, 1023, 1023,  916, 1023, 1023,  917, 1023, 1023,
 /*   530 */   918, 1023, 1023,  919, 1023, 1023,  920, 1023, 1021, 1022,
 /*   540 */   819, 1023, 1023, 1023, 1023,  865, 1023, 1023, 1023, 1023,
 /*   550 */  1023, 1023,  866, 1023, 1023, 1023, 1023,  867, 1023, 1023,
 /*   560 */  1023,  862,  860,  858, 1023, 1023,  857,  854,  849,  845,
 /*   570 */  1023, 1023, 1023,  850,  846, 1023, 1023, 1023,  761, 1023,
 /*   580 */   763, 1023, 1023, 1023,  754, 1023,  756, 1023, 1023, 1023,
 /*   590 */   762, 1023,  764, 1023, 1023, 1023, 1023, 1023, 1023, 1023,
 /*   600 */   757, 1023,  759, 1023, 1023, 1023,  765, 1023,  767, 1023,
 /*   610 */  1023, 1023,  758, 1023,  760, 1023, 1023, 1023,  766, 1023,
 /*   620 */   768, 1023, 1023,  748, 1023,  729, 1023, 1023, 1023,  731,
 /*   630 */  1023, 1023,  733, 1023,  737, 1023, 1023, 1023, 1023, 1023,
 /*   640 */   741,  743, 1023,  744,  742, 1023,  735, 1023, 1023,  732,
 /*   650 */  1023, 1023,  734, 1023,  738, 1023,  736, 1023, 1023, 1023,
 /*   660 */  1023, 1023,  730, 1023, 1023,  712,  714,  713, 1023, 1023,
 /*   670 */   703, 1023, 1023, 1023,  705, 1023, 1023,  706, 1023, 1023,
 /*   680 */   707, 1023, 1023, 1023,  708, 1023, 1023,  709,  692,
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
  "xx_require_statement",  "xx_fetch_statement",  "xx_fcall_statement",  "xx_mcall_statement",
  "xx_scall_statement",  "xx_unset_statement",  "xx_throw_statement",  "xx_declare_statement",
  "xx_break_statement",  "xx_continue_statement",  "xx_while_statement",  "xx_do_while_statement",
  "xx_switch_statement",  "xx_for_statement",  "xx_empty_statement",  "xx_eval_expr",
  "xx_case_clauses",  "xx_case_clause",  "xx_common_expr",  "xx_let_assignments",
  "xx_let_assignment",  "xx_assignment_operator",  "xx_assign_expr",  "xx_array_offset_list",
  "xx_array_offset",  "xx_index_expr",  "xx_echo_expressions",  "xx_echo_expression",
  "xx_mcall_expr",  "xx_fcall_expr",  "xx_scall_expr",  "xx_fetch_expr",
  "xx_declare_variable_list",  "xx_declare_variable",  "xx_array_list",  "xx_call_parameters",
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
 /* 137 */ "xx_statement ::= xx_require_statement",
 /* 138 */ "xx_statement ::= xx_fetch_statement",
 /* 139 */ "xx_statement ::= xx_fcall_statement",
 /* 140 */ "xx_statement ::= xx_mcall_statement",
 /* 141 */ "xx_statement ::= xx_scall_statement",
 /* 142 */ "xx_statement ::= xx_unset_statement",
 /* 143 */ "xx_statement ::= xx_throw_statement",
 /* 144 */ "xx_statement ::= xx_declare_statement",
 /* 145 */ "xx_statement ::= xx_break_statement",
 /* 146 */ "xx_statement ::= xx_continue_statement",
 /* 147 */ "xx_statement ::= xx_while_statement",
 /* 148 */ "xx_statement ::= xx_do_while_statement",
 /* 149 */ "xx_statement ::= xx_switch_statement",
 /* 150 */ "xx_statement ::= xx_for_statement",
 /* 151 */ "xx_statement ::= xx_comment",
 /* 152 */ "xx_statement ::= xx_empty_statement",
 /* 153 */ "xx_empty_statement ::= DOTCOMMA",
 /* 154 */ "xx_break_statement ::= BREAK DOTCOMMA",
 /* 155 */ "xx_continue_statement ::= CONTINUE DOTCOMMA",
 /* 156 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 157 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 158 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 159 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 160 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE ELSE BRACKET_OPEN BRACKET_CLOSE",
 /* 161 */ "xx_if_statement ::= IF xx_eval_expr BRACKET_OPEN BRACKET_CLOSE ELSE BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 162 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 163 */ "xx_switch_statement ::= SWITCH xx_eval_expr BRACKET_OPEN xx_case_clauses BRACKET_CLOSE",
 /* 164 */ "xx_case_clauses ::= xx_case_clauses xx_case_clause",
 /* 165 */ "xx_case_clauses ::= xx_case_clause",
 /* 166 */ "xx_case_clause ::= CASE xx_literal_expr COLON",
 /* 167 */ "xx_case_clause ::= CASE xx_literal_expr COLON xx_statement_list",
 /* 168 */ "xx_case_clause ::= DEFAULT COLON xx_statement_list",
 /* 169 */ "xx_loop_statement ::= LOOP BRACKET_OPEN BRACKET_CLOSE",
 /* 170 */ "xx_loop_statement ::= LOOP BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 171 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN BRACKET_CLOSE",
 /* 172 */ "xx_while_statement ::= WHILE xx_eval_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 173 */ "xx_do_while_statement ::= DO BRACKET_OPEN BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 174 */ "xx_do_while_statement ::= DO BRACKET_OPEN xx_statement_list BRACKET_CLOSE WHILE xx_eval_expr DOTCOMMA",
 /* 175 */ "xx_for_statement ::= FOR IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 176 */ "xx_for_statement ::= FOR IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 177 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 178 */ "xx_for_statement ::= FOR IDENTIFIER COMMA IDENTIFIER IN REVERSE xx_common_expr BRACKET_OPEN xx_statement_list BRACKET_CLOSE",
 /* 179 */ "xx_let_statement ::= LET xx_let_assignments DOTCOMMA",
 /* 180 */ "xx_let_assignments ::= xx_let_assignments COMMA xx_let_assignment",
 /* 181 */ "xx_let_assignments ::= xx_let_assignment",
 /* 182 */ "xx_assignment_operator ::= ASSIGN",
 /* 183 */ "xx_assignment_operator ::= ADDASSIGN",
 /* 184 */ "xx_assignment_operator ::= SUBASSIGN",
 /* 185 */ "xx_assignment_operator ::= MULASSIGN",
 /* 186 */ "xx_assignment_operator ::= DIVASSIGN",
 /* 187 */ "xx_assignment_operator ::= CONCATASSIGN",
 /* 188 */ "xx_assignment_operator ::= MODASSIGN",
 /* 189 */ "xx_let_assignment ::= IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 190 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 191 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 192 */ "xx_let_assignment ::= IDENTIFIER ARROW BRACKET_OPEN STRING BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 193 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 194 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 195 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_assignment_operator xx_assign_expr",
 /* 196 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 197 */ "xx_let_assignment ::= IDENTIFIER DOUBLECOLON IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 198 */ "xx_let_assignment ::= IDENTIFIER SBRACKET_OPEN SBRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 199 */ "xx_let_assignment ::= IDENTIFIER xx_array_offset_list xx_assignment_operator xx_assign_expr",
 /* 200 */ "xx_array_offset_list ::= xx_array_offset_list xx_array_offset",
 /* 201 */ "xx_array_offset_list ::= xx_array_offset",
 /* 202 */ "xx_array_offset ::= SBRACKET_OPEN xx_index_expr SBRACKET_CLOSE",
 /* 203 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER INCR",
 /* 204 */ "xx_let_assignment ::= IDENTIFIER ARROW IDENTIFIER DECR",
 /* 205 */ "xx_let_assignment ::= IDENTIFIER INCR",
 /* 206 */ "xx_let_assignment ::= IDENTIFIER DECR",
 /* 207 */ "xx_let_assignment ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE xx_assignment_operator xx_assign_expr",
 /* 208 */ "xx_index_expr ::= xx_common_expr",
 /* 209 */ "xx_echo_statement ::= ECHO xx_echo_expressions DOTCOMMA",
 /* 210 */ "xx_echo_expressions ::= xx_echo_expressions COMMA xx_echo_expression",
 /* 211 */ "xx_echo_expressions ::= xx_echo_expression",
 /* 212 */ "xx_echo_expression ::= xx_common_expr",
 /* 213 */ "xx_mcall_statement ::= xx_mcall_expr DOTCOMMA",
 /* 214 */ "xx_fcall_statement ::= xx_fcall_expr DOTCOMMA",
 /* 215 */ "xx_scall_statement ::= xx_scall_expr DOTCOMMA",
 /* 216 */ "xx_fetch_statement ::= xx_fetch_expr DOTCOMMA",
 /* 217 */ "xx_return_statement ::= RETURN xx_common_expr DOTCOMMA",
 /* 218 */ "xx_return_statement ::= RETURN DOTCOMMA",
 /* 219 */ "xx_require_statement ::= REQUIRE xx_common_expr DOTCOMMA",
 /* 220 */ "xx_unset_statement ::= UNSET xx_common_expr DOTCOMMA",
 /* 221 */ "xx_throw_statement ::= THROW xx_common_expr DOTCOMMA",
 /* 222 */ "xx_declare_statement ::= TYPE_INTEGER xx_declare_variable_list DOTCOMMA",
 /* 223 */ "xx_declare_statement ::= TYPE_UINTEGER xx_declare_variable_list DOTCOMMA",
 /* 224 */ "xx_declare_statement ::= TYPE_CHAR xx_declare_variable_list DOTCOMMA",
 /* 225 */ "xx_declare_statement ::= TYPE_UCHAR xx_declare_variable_list DOTCOMMA",
 /* 226 */ "xx_declare_statement ::= TYPE_LONG xx_declare_variable_list DOTCOMMA",
 /* 227 */ "xx_declare_statement ::= TYPE_ULONG xx_declare_variable_list DOTCOMMA",
 /* 228 */ "xx_declare_statement ::= TYPE_DOUBLE xx_declare_variable_list DOTCOMMA",
 /* 229 */ "xx_declare_statement ::= TYPE_STRING xx_declare_variable_list DOTCOMMA",
 /* 230 */ "xx_declare_statement ::= TYPE_BOOL xx_declare_variable_list DOTCOMMA",
 /* 231 */ "xx_declare_statement ::= TYPE_VAR xx_declare_variable_list DOTCOMMA",
 /* 232 */ "xx_declare_variable_list ::= xx_declare_variable_list COMMA xx_declare_variable",
 /* 233 */ "xx_declare_variable_list ::= xx_declare_variable",
 /* 234 */ "xx_declare_variable ::= IDENTIFIER",
 /* 235 */ "xx_declare_variable ::= IDENTIFIER ASSIGN xx_literal_expr",
 /* 236 */ "xx_assign_expr ::= xx_common_expr",
 /* 237 */ "xx_common_expr ::= NOT xx_common_expr",
 /* 238 */ "xx_common_expr ::= ISSET xx_common_expr",
 /* 239 */ "xx_common_expr ::= REQUIRE xx_common_expr",
 /* 240 */ "xx_common_expr ::= CLONE xx_common_expr",
 /* 241 */ "xx_common_expr ::= EMPTY xx_common_expr",
 /* 242 */ "xx_common_expr ::= LIKELY xx_common_expr",
 /* 243 */ "xx_common_expr ::= UNLIKELY xx_common_expr",
 /* 244 */ "xx_common_expr ::= xx_common_expr EQUALS xx_common_expr",
 /* 245 */ "xx_common_expr ::= xx_common_expr NOTEQUALS xx_common_expr",
 /* 246 */ "xx_common_expr ::= xx_common_expr IDENTICAL xx_common_expr",
 /* 247 */ "xx_common_expr ::= xx_common_expr NOTIDENTICAL xx_common_expr",
 /* 248 */ "xx_common_expr ::= xx_common_expr LESS xx_common_expr",
 /* 249 */ "xx_common_expr ::= xx_common_expr GREATER xx_common_expr",
 /* 250 */ "xx_common_expr ::= xx_common_expr LESSEQUAL xx_common_expr",
 /* 251 */ "xx_common_expr ::= xx_common_expr GREATEREQUAL xx_common_expr",
 /* 252 */ "xx_common_expr ::= PARENTHESES_OPEN xx_common_expr PARENTHESES_CLOSE",
 /* 253 */ "xx_common_expr ::= PARENTHESES_OPEN xx_parameter_type PARENTHESES_CLOSE xx_common_expr",
 /* 254 */ "xx_common_expr ::= LESS IDENTIFIER GREATER xx_common_expr",
 /* 255 */ "xx_common_expr ::= xx_common_expr ARROW IDENTIFIER",
 /* 256 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 257 */ "xx_common_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE",
 /* 258 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER",
 /* 259 */ "xx_common_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 260 */ "xx_common_expr ::= xx_common_expr SBRACKET_OPEN xx_common_expr SBRACKET_CLOSE",
 /* 261 */ "xx_common_expr ::= xx_common_expr ADD xx_common_expr",
 /* 262 */ "xx_common_expr ::= xx_common_expr SUB xx_common_expr",
 /* 263 */ "xx_common_expr ::= xx_common_expr MUL xx_common_expr",
 /* 264 */ "xx_common_expr ::= xx_common_expr DIV xx_common_expr",
 /* 265 */ "xx_common_expr ::= xx_common_expr MOD xx_common_expr",
 /* 266 */ "xx_common_expr ::= xx_common_expr CONCAT xx_common_expr",
 /* 267 */ "xx_common_expr ::= xx_common_expr AND xx_common_expr",
 /* 268 */ "xx_common_expr ::= xx_common_expr OR xx_common_expr",
 /* 269 */ "xx_common_expr ::= xx_common_expr INSTANCEOF xx_common_expr",
 /* 270 */ "xx_fetch_expr ::= FETCH IDENTIFIER COMMA xx_common_expr",
 /* 271 */ "xx_common_expr ::= xx_fetch_expr",
 /* 272 */ "xx_common_expr ::= TYPEOF xx_common_expr",
 /* 273 */ "xx_common_expr ::= IDENTIFIER",
 /* 274 */ "xx_common_expr ::= INTEGER",
 /* 275 */ "xx_common_expr ::= STRING",
 /* 276 */ "xx_common_expr ::= CHAR",
 /* 277 */ "xx_common_expr ::= DOUBLE",
 /* 278 */ "xx_common_expr ::= NULL",
 /* 279 */ "xx_common_expr ::= TRUE",
 /* 280 */ "xx_common_expr ::= FALSE",
 /* 281 */ "xx_common_expr ::= CONSTANT",
 /* 282 */ "xx_common_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 283 */ "xx_common_expr ::= SBRACKET_OPEN xx_array_list SBRACKET_CLOSE",
 /* 284 */ "xx_common_expr ::= NEW IDENTIFIER",
 /* 285 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 286 */ "xx_common_expr ::= NEW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 287 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE",
 /* 288 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 289 */ "xx_common_expr ::= NEW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 290 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 291 */ "xx_fcall_expr ::= IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 292 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 293 */ "xx_fcall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 294 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 295 */ "xx_scall_expr ::= IDENTIFIER DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 296 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 297 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 298 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 299 */ "xx_scall_expr ::= BRACKET_OPEN IDENTIFIER BRACKET_CLOSE DOUBLECOLON BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 300 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 301 */ "xx_mcall_expr ::= xx_common_expr ARROW IDENTIFIER PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 302 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 303 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN IDENTIFIER BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 304 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN xx_call_parameters PARENTHESES_CLOSE",
 /* 305 */ "xx_mcall_expr ::= xx_common_expr ARROW BRACKET_OPEN STRING BRACKET_CLOSE PARENTHESES_OPEN PARENTHESES_CLOSE",
 /* 306 */ "xx_common_expr ::= xx_mcall_expr",
 /* 307 */ "xx_common_expr ::= xx_scall_expr",
 /* 308 */ "xx_common_expr ::= xx_fcall_expr",
 /* 309 */ "xx_common_expr ::= xx_common_expr QUESTION xx_common_expr COLON xx_common_expr",
 /* 310 */ "xx_call_parameters ::= xx_call_parameters COMMA xx_call_parameter",
 /* 311 */ "xx_call_parameters ::= xx_call_parameter",
 /* 312 */ "xx_call_parameter ::= xx_common_expr",
 /* 313 */ "xx_array_list ::= xx_array_list COMMA xx_array_item",
 /* 314 */ "xx_array_list ::= xx_array_item",
 /* 315 */ "xx_array_item ::= xx_array_key COLON xx_array_value",
 /* 316 */ "xx_array_item ::= xx_array_value",
 /* 317 */ "xx_array_key ::= IDENTIFIER",
 /* 318 */ "xx_array_key ::= STRING",
 /* 319 */ "xx_array_key ::= INTEGER",
 /* 320 */ "xx_array_value ::= xx_common_expr",
 /* 321 */ "xx_literal_expr ::= INTEGER",
 /* 322 */ "xx_literal_expr ::= CHAR",
 /* 323 */ "xx_literal_expr ::= STRING",
 /* 324 */ "xx_literal_expr ::= DOUBLE",
 /* 325 */ "xx_literal_expr ::= NULL",
 /* 326 */ "xx_literal_expr ::= FALSE",
 /* 327 */ "xx_literal_expr ::= TRUE",
 /* 328 */ "xx_literal_expr ::= IDENTIFIER DOUBLECOLON CONSTANT",
 /* 329 */ "xx_literal_expr ::= CONSTANT",
 /* 330 */ "xx_literal_expr ::= SBRACKET_OPEN SBRACKET_CLOSE",
 /* 331 */ "xx_eval_expr ::= xx_common_expr",
 /* 332 */ "xx_comment ::= COMMENT",
 /* 333 */ "xx_cblock ::= CBLOCK",
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
// 1036 "parser.lemon"
{
	/*if ((yypminor->yy0)) {
		if ((yypminor->yy0)->free_flag) {
			efree((yypminor->yy0)->token);
		}
		efree((yypminor->yy0));
	}*/
}
// 3223 "parser.c"
      break;
    case 109:
// 1049 "parser.lemon"
{ json_object_put((yypminor->yy31)); }
// 3228 "parser.c"
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
  { 146, 1 },
  { 166, 1 },
  { 160, 2 },
  { 161, 2 },
  { 148, 4 },
  { 148, 7 },
  { 148, 5 },
  { 148, 9 },
  { 148, 8 },
  { 148, 8 },
  { 164, 4 },
  { 164, 5 },
  { 168, 2 },
  { 168, 1 },
  { 169, 3 },
  { 169, 4 },
  { 169, 3 },
  { 149, 3 },
  { 149, 4 },
  { 162, 4 },
  { 162, 5 },
  { 163, 6 },
  { 163, 7 },
  { 165, 7 },
  { 165, 8 },
  { 165, 9 },
  { 165, 10 },
  { 147, 3 },
  { 171, 3 },
  { 171, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 173, 1 },
  { 172, 3 },
  { 172, 5 },
  { 172, 7 },
  { 172, 7 },
  { 172, 7 },
  { 172, 6 },
  { 172, 5 },
  { 172, 7 },
  { 172, 6 },
  { 172, 5 },
  { 172, 4 },
  { 175, 2 },
  { 175, 1 },
  { 176, 3 },
  { 172, 4 },
  { 172, 4 },
  { 172, 2 },
  { 172, 2 },
  { 172, 5 },
  { 177, 1 },
  { 150, 3 },
  { 178, 3 },
  { 178, 1 },
  { 179, 1 },
  { 155, 2 },
  { 154, 2 },
  { 156, 2 },
  { 153, 2 },
  { 151, 3 },
  { 151, 2 },
  { 152, 3 },
  { 157, 3 },
  { 158, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 159, 3 },
  { 184, 3 },
  { 184, 1 },
  { 185, 1 },
  { 185, 3 },
  { 174, 1 },
  { 170, 2 },
  { 170, 2 },
  { 170, 2 },
  { 170, 2 },
  { 170, 2 },
  { 170, 2 },
  { 170, 2 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 4 },
  { 170, 4 },
  { 170, 3 },
  { 170, 5 },
  { 170, 5 },
  { 170, 3 },
  { 170, 3 },
  { 170, 4 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 170, 3 },
  { 183, 4 },
  { 170, 1 },
  { 170, 2 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 2 },
  { 170, 3 },
  { 170, 2 },
  { 170, 4 },
  { 170, 5 },
  { 170, 4 },
  { 170, 6 },
  { 170, 7 },
  { 181, 4 },
  { 181, 3 },
  { 181, 6 },
  { 181, 5 },
  { 182, 6 },
  { 182, 5 },
  { 182, 8 },
  { 182, 7 },
  { 182, 10 },
  { 182, 9 },
  { 180, 6 },
  { 180, 5 },
  { 180, 8 },
  { 180, 7 },
  { 180, 8 },
  { 180, 7 },
  { 170, 1 },
  { 170, 1 },
  { 170, 1 },
  { 170, 5 },
  { 187, 3 },
  { 187, 1 },
  { 188, 1 },
  { 186, 3 },
  { 186, 1 },
  { 189, 3 },
  { 189, 1 },
  { 190, 1 },
  { 190, 1 },
  { 190, 1 },
  { 191, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 1 },
  { 129, 3 },
  { 129, 1 },
  { 129, 2 },
  { 167, 1 },
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
// 1045 "parser.lemon"
{
	status->ret = yymsp[0].minor.yy31;
}
// 3779 "parser.c"
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
      case 152:
      case 208:
      case 236:
      case 271:
      case 306:
      case 307:
      case 308:
      case 312:
      case 320:
      case 331:
// 1051 "parser.lemon"
{
	yygotominor.yy31 = yymsp[0].minor.yy31;
}
// 3822 "parser.c"
        break;
      case 2:
      case 40:
      case 56:
      case 58:
      case 60:
      case 88:
      case 129:
      case 164:
      case 200:
// 1055 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_list(yymsp[-1].minor.yy31, yymsp[0].minor.yy31);
}
// 3837 "parser.c"
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
      case 165:
      case 181:
      case 201:
      case 211:
      case 233:
      case 311:
      case 314:
// 1059 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_list(NULL, yymsp[0].minor.yy31);
}
// 3861 "parser.c"
        break;
      case 9:
// 1084 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_namespace(yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(40,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 3870 "parser.c"
        break;
      case 10:
// 1088 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_interface(yymsp[-1].minor.yy0, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(43,&yymsp[-2].minor);
}
// 3878 "parser.c"
        break;
      case 11:
// 1092 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_interface(yymsp[-3].minor.yy0, yymsp[0].minor.yy31, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(43,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3887 "parser.c"
        break;
      case 12:
// 1096 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy31, 0, 0, NULL, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3895 "parser.c"
        break;
      case 13:
// 1100 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy31, 0, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3904 "parser.c"
        break;
      case 14:
// 1104 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy31, 0, 0, NULL, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3913 "parser.c"
        break;
      case 15:
// 1108 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-5].minor.yy0, yymsp[0].minor.yy31, 0, 0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(45,&yymsp[-6].minor);
  yy_destructor(44,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3923 "parser.c"
        break;
      case 16:
// 1112 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy31, 1, 0, NULL, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3932 "parser.c"
        break;
      case 17:
// 1116 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy31, 1, 0, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3942 "parser.c"
        break;
      case 18:
// 1120 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy31, 1, 0, NULL, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(47,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(46,&yymsp[-2].minor);
}
// 3952 "parser.c"
        break;
      case 19:
// 1124 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-1].minor.yy0, yymsp[0].minor.yy31, 0, 1, NULL, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-3].minor);
  yy_destructor(45,&yymsp[-2].minor);
}
// 3961 "parser.c"
        break;
      case 20:
// 1128 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class(yymsp[-3].minor.yy0, yymsp[0].minor.yy31, 0, 1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(48,&yymsp[-5].minor);
  yy_destructor(45,&yymsp[-4].minor);
  yy_destructor(44,&yymsp[-2].minor);
}
// 3971 "parser.c"
        break;
      case 21:
      case 50:
// 1132 "parser.lemon"
{
	yygotominor.yy31 = NULL;
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3981 "parser.c"
        break;
      case 22:
      case 51:
// 1136 "parser.lemon"
{
	yygotominor.yy31 = yymsp[-1].minor.yy31;
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 3991 "parser.c"
        break;
      case 23:
      case 52:
      case 106:
      case 180:
      case 210:
      case 232:
      case 310:
      case 313:
// 1140 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_list(yymsp[-2].minor.yy31, yymsp[0].minor.yy31);
  yy_destructor(6,&yymsp[-1].minor);
}
// 4006 "parser.c"
        break;
      case 25:
      case 273:
      case 317:
// 1148 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state);
}
// 4015 "parser.c"
        break;
      case 26:
// 1152 "parser.lemon"
{
  yygotominor.yy31 = NULL;
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4024 "parser.c"
        break;
      case 27:
// 1156 "parser.lemon"
{
  yygotominor.yy31 = yymsp[-1].minor.yy31;
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4033 "parser.c"
        break;
      case 28:
// 1160 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
}
// 4040 "parser.c"
        break;
      case 29:
// 1164 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(NULL, NULL, yymsp[0].minor.yy31, status->scanner_state);
}
// 4047 "parser.c"
        break;
      case 30:
// 1168 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(NULL, yymsp[0].minor.yy31, NULL, status->scanner_state);
}
// 4054 "parser.c"
        break;
      case 31:
// 1172 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(yymsp[-1].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
}
// 4061 "parser.c"
        break;
      case 32:
// 1176 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(yymsp[-1].minor.yy31, NULL, yymsp[0].minor.yy31, status->scanner_state);
}
// 4068 "parser.c"
        break;
      case 33:
// 1180 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(yymsp[0].minor.yy31, NULL, yymsp[-1].minor.yy31, status->scanner_state);
}
// 4075 "parser.c"
        break;
      case 34:
// 1184 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(NULL, yymsp[0].minor.yy31, yymsp[-1].minor.yy31, status->scanner_state);
}
// 4082 "parser.c"
        break;
      case 35:
// 1188 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(yymsp[-2].minor.yy31, yymsp[0].minor.yy31, yymsp[-1].minor.yy31, status->scanner_state);
}
// 4089 "parser.c"
        break;
      case 36:
// 1192 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_definition(yymsp[-1].minor.yy31, yymsp[0].minor.yy31, yymsp[-2].minor.yy31, status->scanner_state);
}
// 4096 "parser.c"
        break;
      case 37:
// 1196 "parser.lemon"
{
  yygotominor.yy31 = xx_ret_interface_definition(NULL, yymsp[0].minor.yy31, status->scanner_state);
}
// 4103 "parser.c"
        break;
      case 38:
// 1200 "parser.lemon"
{
  yygotominor.yy31 = xx_ret_interface_definition(yymsp[0].minor.yy31, NULL, status->scanner_state);
}
// 4110 "parser.c"
        break;
      case 39:
// 1204 "parser.lemon"
{
  yygotominor.yy31 = xx_ret_interface_definition(yymsp[0].minor.yy31, yymsp[-1].minor.yy31, status->scanner_state);
}
// 4117 "parser.c"
        break;
      case 42:
// 1217 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_property(yymsp[-2].minor.yy31, yymsp[-1].minor.yy0, NULL, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4125 "parser.c"
        break;
      case 43:
// 1221 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_property(yymsp[-2].minor.yy31, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4133 "parser.c"
        break;
      case 44:
// 1225 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_property(yymsp[-4].minor.yy31, yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, yymsp[-5].minor.yy0, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4142 "parser.c"
        break;
      case 45:
// 1229 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_property(yymsp[-4].minor.yy31, yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4151 "parser.c"
        break;
      case 46:
// 1233 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_property(yymsp[-3].minor.yy31, yymsp[-2].minor.yy0, NULL, yymsp[-4].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4159 "parser.c"
        break;
      case 47:
// 1237 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_property(yymsp[-3].minor.yy31, yymsp[-2].minor.yy0, NULL, NULL, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4167 "parser.c"
        break;
      case 48:
// 1241 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_property(yymsp[-5].minor.yy31, yymsp[-4].minor.yy0, yymsp[-2].minor.yy31, yymsp[-6].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4176 "parser.c"
        break;
      case 49:
// 1245 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_property(yymsp[-5].minor.yy31, yymsp[-4].minor.yy0, yymsp[-2].minor.yy31, NULL, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(52,&yymsp[-3].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4185 "parser.c"
        break;
      case 54:
// 1265 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_property_shortcut(NULL, yymsp[0].minor.yy0, status->scanner_state);
}
// 4192 "parser.c"
        break;
      case 55:
// 1269 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_property_shortcut(yymsp[-1].minor.yy0, yymsp[0].minor.yy0, status->scanner_state);
}
// 4199 "parser.c"
        break;
      case 62:
// 1298 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, yymsp[-5].minor.yy0, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4209 "parser.c"
        break;
      case 63:
// 1302 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_const(yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, NULL, status->scanner_state);
  yy_destructor(53,&yymsp[-4].minor);
  yy_destructor(52,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4219 "parser.c"
        break;
      case 64:
// 1307 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-6].minor.yy31, yymsp[-4].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4231 "parser.c"
        break;
      case 65:
// 1311 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-7].minor.yy31, yymsp[-5].minor.yy0, yymsp[-3].minor.yy31, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4243 "parser.c"
        break;
      case 66:
// 1315 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-7].minor.yy31, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4255 "parser.c"
        break;
      case 67:
// 1319 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-8].minor.yy31, yymsp[-6].minor.yy0, yymsp[-4].minor.yy31, yymsp[-1].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4267 "parser.c"
        break;
      case 68:
// 1323 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-6].minor.yy31, yymsp[-4].minor.yy0, NULL, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4279 "parser.c"
        break;
      case 69:
// 1327 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-7].minor.yy31, yymsp[-5].minor.yy0, yymsp[-3].minor.yy31, NULL, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4291 "parser.c"
        break;
      case 70:
// 1331 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-7].minor.yy31, yymsp[-5].minor.yy0, NULL, yymsp[-1].minor.yy31, yymsp[-8].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4303 "parser.c"
        break;
      case 71:
// 1335 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-8].minor.yy31, yymsp[-6].minor.yy0, yymsp[-4].minor.yy31, yymsp[-1].minor.yy31, yymsp[-9].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4315 "parser.c"
        break;
      case 72:
// 1339 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-8].minor.yy31, yymsp[-6].minor.yy0, NULL, NULL, NULL, yymsp[-2].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4328 "parser.c"
        break;
      case 73:
// 1343 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-9].minor.yy31, yymsp[-7].minor.yy0, yymsp[-5].minor.yy31, NULL, NULL, yymsp[-2].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4341 "parser.c"
        break;
      case 74:
// 1347 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-9].minor.yy31, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy31, NULL, yymsp[-3].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4354 "parser.c"
        break;
      case 75:
// 1351 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-10].minor.yy31, yymsp[-8].minor.yy0, yymsp[-6].minor.yy31, yymsp[-1].minor.yy31, NULL, yymsp[-3].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-9].minor);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4367 "parser.c"
        break;
      case 76:
// 1355 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-8].minor.yy31, yymsp[-6].minor.yy0, NULL, NULL, yymsp[-9].minor.yy0, yymsp[-2].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4380 "parser.c"
        break;
      case 77:
// 1359 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-9].minor.yy31, yymsp[-7].minor.yy0, yymsp[-5].minor.yy31, NULL, yymsp[-10].minor.yy0, yymsp[-2].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-4].minor);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4393 "parser.c"
        break;
      case 78:
// 1363 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-9].minor.yy31, yymsp[-7].minor.yy0, NULL, yymsp[-1].minor.yy31, yymsp[-10].minor.yy0, yymsp[-3].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-8].minor);
  yy_destructor(56,&yymsp[-6].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4406 "parser.c"
        break;
      case 79:
// 1367 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-10].minor.yy31, yymsp[-8].minor.yy0, yymsp[-6].minor.yy31, yymsp[-1].minor.yy31, yymsp[-11].minor.yy0, yymsp[-3].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-9].minor);
  yy_destructor(56,&yymsp[-7].minor);
  yy_destructor(37,&yymsp[-5].minor);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4419 "parser.c"
        break;
      case 80:
// 1372 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-7].minor.yy31, yymsp[-5].minor.yy0, NULL, NULL, NULL, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4431 "parser.c"
        break;
      case 81:
// 1376 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-8].minor.yy31, yymsp[-6].minor.yy0, yymsp[-4].minor.yy31, NULL, NULL, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4443 "parser.c"
        break;
      case 82:
// 1380 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-7].minor.yy31, yymsp[-5].minor.yy0, NULL, NULL, yymsp[-8].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-6].minor);
  yy_destructor(56,&yymsp[-4].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4455 "parser.c"
        break;
      case 83:
// 1384 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-8].minor.yy31, yymsp[-6].minor.yy0, yymsp[-4].minor.yy31, NULL, yymsp[-9].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(55,&yymsp[-7].minor);
  yy_destructor(56,&yymsp[-5].minor);
  yy_destructor(37,&yymsp[-3].minor);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4467 "parser.c"
        break;
      case 84:
// 1388 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-5].minor.yy31, yymsp[-3].minor.yy0, NULL, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4478 "parser.c"
        break;
      case 85:
// 1392 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-6].minor.yy31, yymsp[-4].minor.yy0, yymsp[-2].minor.yy31, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4489 "parser.c"
        break;
      case 86:
// 1396 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-5].minor.yy31, yymsp[-3].minor.yy0, NULL, NULL, yymsp[-6].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4500 "parser.c"
        break;
      case 87:
// 1400 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_class_method(yymsp[-6].minor.yy31, yymsp[-4].minor.yy0, yymsp[-2].minor.yy31, NULL, yymsp[-7].minor.yy0, NULL, status->scanner_state);
  yy_destructor(55,&yymsp[-5].minor);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4511 "parser.c"
        break;
      case 90:
// 1413 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("public");
  yy_destructor(1,&yymsp[0].minor);
}
// 4519 "parser.c"
        break;
      case 91:
// 1417 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("protected");
  yy_destructor(2,&yymsp[0].minor);
}
// 4527 "parser.c"
        break;
      case 92:
// 1421 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("private");
  yy_destructor(4,&yymsp[0].minor);
}
// 4535 "parser.c"
        break;
      case 93:
// 1425 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("static");
  yy_destructor(3,&yymsp[0].minor);
}
// 4543 "parser.c"
        break;
      case 94:
// 1429 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("scoped");
  yy_destructor(5,&yymsp[0].minor);
}
// 4551 "parser.c"
        break;
      case 95:
// 1433 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("inline");
  yy_destructor(57,&yymsp[0].minor);
}
// 4559 "parser.c"
        break;
      case 96:
// 1437 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("abstract");
  yy_destructor(47,&yymsp[0].minor);
}
// 4567 "parser.c"
        break;
      case 97:
// 1441 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("final");
  yy_destructor(48,&yymsp[0].minor);
}
// 4575 "parser.c"
        break;
      case 98:
// 1446 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_return_type(1, NULL, status->scanner_state);
  yy_destructor(58,&yymsp[0].minor);
}
// 4583 "parser.c"
        break;
      case 99:
// 1450 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_return_type(0, yymsp[0].minor.yy31, status->scanner_state);
}
// 4590 "parser.c"
        break;
      case 100:
// 1454 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_list(yymsp[-2].minor.yy31, yymsp[0].minor.yy31);
  yy_destructor(16,&yymsp[-1].minor);
}
// 4598 "parser.c"
        break;
      case 102:
// 1462 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_return_type_item(yymsp[0].minor.yy31, NULL, 0, 0, status->scanner_state);
}
// 4605 "parser.c"
        break;
      case 103:
// 1466 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_return_type_item(yymsp[-1].minor.yy31, NULL, 1, 0, status->scanner_state);
  yy_destructor(36,&yymsp[0].minor);
}
// 4613 "parser.c"
        break;
      case 104:
// 1470 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy31, 0, 0, status->scanner_state);
}
// 4620 "parser.c"
        break;
      case 105:
// 1474 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_return_type_item(NULL, yymsp[0].minor.yy31, 0, 1, status->scanner_state);
}
// 4627 "parser.c"
        break;
      case 108:
// 1487 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_parameter(NULL, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4634 "parser.c"
        break;
      case 109:
// 1491 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_parameter(yymsp[-1].minor.yy31, NULL, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4641 "parser.c"
        break;
      case 110:
// 1495 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_parameter(yymsp[-2].minor.yy31, NULL, yymsp[0].minor.yy0, NULL, 1, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 4649 "parser.c"
        break;
      case 111:
// 1499 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_parameter(NULL, yymsp[-1].minor.yy31, yymsp[0].minor.yy0, NULL, 0, status->scanner_state);
}
// 4656 "parser.c"
        break;
      case 112:
// 1503 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_parameter(NULL, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy31, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4664 "parser.c"
        break;
      case 113:
// 1507 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_parameter(yymsp[-3].minor.yy31, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy31, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4672 "parser.c"
        break;
      case 114:
// 1511 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_parameter(yymsp[-4].minor.yy31, NULL, yymsp[-2].minor.yy0, yymsp[0].minor.yy31, 1, status->scanner_state);
  yy_destructor(36,&yymsp[-3].minor);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4681 "parser.c"
        break;
      case 115:
// 1515 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_parameter(NULL, yymsp[-3].minor.yy31, yymsp[-2].minor.yy0, yymsp[0].minor.yy31, 0, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 4689 "parser.c"
        break;
      case 116:
// 1519 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-2].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 4698 "parser.c"
        break;
      case 117:
// 1523 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_IDENTIFIER, yymsp[-3].minor.yy0, status->scanner_state);
  yy_destructor(20,&yymsp[-4].minor);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[-1].minor);
  yy_destructor(21,&yymsp[0].minor);
}
// 4709 "parser.c"
        break;
      case 118:
// 1527 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_INTEGER);
  yy_destructor(60,&yymsp[0].minor);
}
// 4717 "parser.c"
        break;
      case 119:
// 1531 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_UINTEGER);
  yy_destructor(61,&yymsp[0].minor);
}
// 4725 "parser.c"
        break;
      case 120:
// 1535 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_LONG);
  yy_destructor(62,&yymsp[0].minor);
}
// 4733 "parser.c"
        break;
      case 121:
// 1539 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_ULONG);
  yy_destructor(63,&yymsp[0].minor);
}
// 4741 "parser.c"
        break;
      case 122:
// 1543 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_CHAR);
  yy_destructor(64,&yymsp[0].minor);
}
// 4749 "parser.c"
        break;
      case 123:
// 1547 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_UCHAR);
  yy_destructor(65,&yymsp[0].minor);
}
// 4757 "parser.c"
        break;
      case 124:
// 1551 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_DOUBLE);
  yy_destructor(66,&yymsp[0].minor);
}
// 4765 "parser.c"
        break;
      case 125:
// 1555 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_BOOL);
  yy_destructor(67,&yymsp[0].minor);
}
// 4773 "parser.c"
        break;
      case 126:
// 1559 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_STRING);
  yy_destructor(68,&yymsp[0].minor);
}
// 4781 "parser.c"
        break;
      case 127:
// 1563 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_ARRAY);
  yy_destructor(69,&yymsp[0].minor);
}
// 4789 "parser.c"
        break;
      case 128:
// 1567 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_type(XX_TYPE_VAR);
  yy_destructor(70,&yymsp[0].minor);
}
// 4797 "parser.c"
        break;
      case 153:
// 1669 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_empty_statement(status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 4805 "parser.c"
        break;
      case 154:
// 1673 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_break_statement(status->scanner_state);
  yy_destructor(71,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4814 "parser.c"
        break;
      case 155:
// 1677 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_continue_statement(status->scanner_state);
  yy_destructor(72,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4823 "parser.c"
        break;
      case 156:
// 1681 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_if_statement(yymsp[-2].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4833 "parser.c"
        break;
      case 157:
// 1685 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_if_statement(yymsp[-5].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4846 "parser.c"
        break;
      case 158:
// 1689 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_if_statement(yymsp[-3].minor.yy31, yymsp[-1].minor.yy31, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4856 "parser.c"
        break;
      case 159:
// 1693 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_if_statement(yymsp[-7].minor.yy31, yymsp[-5].minor.yy31, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(73,&yymsp[-8].minor);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4869 "parser.c"
        break;
      case 160:
// 1697 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_if_statement(yymsp[-6].minor.yy31, yymsp[-4].minor.yy31, NULL, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(74,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4882 "parser.c"
        break;
      case 161:
// 1701 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_if_statement(yymsp[-6].minor.yy31, NULL, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(73,&yymsp[-7].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(74,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4895 "parser.c"
        break;
      case 162:
// 1705 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_switch_statement(yymsp[-2].minor.yy31, NULL, status->scanner_state);
  yy_destructor(75,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4905 "parser.c"
        break;
      case 163:
// 1709 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_switch_statement(yymsp[-3].minor.yy31, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(75,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4915 "parser.c"
        break;
      case 166:
// 1721 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_case_clause(yymsp[-1].minor.yy31, NULL, status->scanner_state);
  yy_destructor(76,&yymsp[-2].minor);
  yy_destructor(77,&yymsp[0].minor);
}
// 4924 "parser.c"
        break;
      case 167:
// 1725 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_case_clause(yymsp[-2].minor.yy31, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(76,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 4933 "parser.c"
        break;
      case 168:
// 1729 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_case_clause(NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(78,&yymsp[-2].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 4942 "parser.c"
        break;
      case 169:
// 1733 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_loop_statement(NULL, status->scanner_state);
  yy_destructor(79,&yymsp[-2].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4952 "parser.c"
        break;
      case 170:
// 1737 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_loop_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(79,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4962 "parser.c"
        break;
      case 171:
// 1741 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_while_statement(yymsp[-2].minor.yy31, NULL, status->scanner_state);
  yy_destructor(80,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-1].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4972 "parser.c"
        break;
      case 172:
// 1745 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_while_statement(yymsp[-3].minor.yy31, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(80,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 4982 "parser.c"
        break;
      case 173:
// 1749 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_do_while_statement(yymsp[-1].minor.yy31, NULL, status->scanner_state);
  yy_destructor(81,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 4994 "parser.c"
        break;
      case 174:
// 1753 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_do_while_statement(yymsp[-1].minor.yy31, yymsp[-4].minor.yy31, status->scanner_state);
  yy_destructor(81,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(80,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5006 "parser.c"
        break;
      case 175:
// 1757 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_for_statement(yymsp[-3].minor.yy31, NULL, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(82,&yymsp[-6].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5017 "parser.c"
        break;
      case 176:
// 1761 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_for_statement(yymsp[-3].minor.yy31, NULL, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(82,&yymsp[-7].minor);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5029 "parser.c"
        break;
      case 177:
// 1765 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_for_statement(yymsp[-3].minor.yy31, yymsp[-7].minor.yy0, yymsp[-5].minor.yy0, 0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(82,&yymsp[-8].minor);
  yy_destructor(6,&yymsp[-6].minor);
  yy_destructor(83,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5041 "parser.c"
        break;
      case 178:
// 1769 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_for_statement(yymsp[-3].minor.yy31, yymsp[-8].minor.yy0, yymsp[-6].minor.yy0, 1, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(82,&yymsp[-9].minor);
  yy_destructor(6,&yymsp[-7].minor);
  yy_destructor(83,&yymsp[-5].minor);
  yy_destructor(84,&yymsp[-4].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5054 "parser.c"
        break;
      case 179:
// 1773 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(85,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5063 "parser.c"
        break;
      case 182:
// 1786 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("assign");
  yy_destructor(52,&yymsp[0].minor);
}
// 5071 "parser.c"
        break;
      case 183:
// 1791 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("add-assign");
  yy_destructor(86,&yymsp[0].minor);
}
// 5079 "parser.c"
        break;
      case 184:
// 1796 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("sub-assign");
  yy_destructor(87,&yymsp[0].minor);
}
// 5087 "parser.c"
        break;
      case 185:
// 1800 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("mult-assign");
  yy_destructor(88,&yymsp[0].minor);
}
// 5095 "parser.c"
        break;
      case 186:
// 1804 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("div-assign");
  yy_destructor(89,&yymsp[0].minor);
}
// 5103 "parser.c"
        break;
      case 187:
// 1808 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("concat-assign");
  yy_destructor(90,&yymsp[0].minor);
}
// 5111 "parser.c"
        break;
      case 188:
// 1812 "parser.lemon"
{
	yygotominor.yy31 = json_object_new_string("mod-assign");
  yy_destructor(91,&yymsp[0].minor);
}
// 5119 "parser.c"
        break;
      case 189:
// 1817 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("variable", yymsp[-1].minor.yy31, yymsp[-2].minor.yy0, NULL, NULL, yymsp[0].minor.yy31, status->scanner_state);
}
// 5126 "parser.c"
        break;
      case 190:
// 1822 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("object-property", yymsp[-1].minor.yy31, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
}
// 5134 "parser.c"
        break;
      case 191:
// 1827 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("variable-dynamic-object-property", yymsp[-1].minor.yy31, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 5144 "parser.c"
        break;
      case 192:
// 1832 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("string-dynamic-object-property", yymsp[-1].minor.yy31, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 5154 "parser.c"
        break;
      case 193:
// 1837 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("object-property-append", yymsp[-1].minor.yy31, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5164 "parser.c"
        break;
      case 194:
// 1842 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("object-property-array-index", yymsp[-1].minor.yy31, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy31, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
}
// 5172 "parser.c"
        break;
      case 195:
// 1847 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("static-property", yymsp[-1].minor.yy31, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(93,&yymsp[-3].minor);
}
// 5180 "parser.c"
        break;
      case 196:
// 1852 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("static-property-append", yymsp[-1].minor.yy31, yymsp[-6].minor.yy0, yymsp[-4].minor.yy0, NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(93,&yymsp[-5].minor);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5190 "parser.c"
        break;
      case 197:
// 1857 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("static-property-array-index", yymsp[-1].minor.yy31, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-2].minor.yy31, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(93,&yymsp[-4].minor);
}
// 5198 "parser.c"
        break;
      case 198:
// 1862 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("variable-append", yymsp[-1].minor.yy31, yymsp[-4].minor.yy0, NULL, NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(38,&yymsp[-3].minor);
  yy_destructor(59,&yymsp[-2].minor);
}
// 5207 "parser.c"
        break;
      case 199:
// 1867 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("array-index", yymsp[-1].minor.yy31, yymsp[-3].minor.yy0, NULL, yymsp[-2].minor.yy31, yymsp[0].minor.yy31, status->scanner_state);
}
// 5214 "parser.c"
        break;
      case 202:
// 1879 "parser.lemon"
{
	yygotominor.yy31 = yymsp[-1].minor.yy31;
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5223 "parser.c"
        break;
      case 203:
// 1884 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("object-property-incr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(94,&yymsp[0].minor);
}
// 5232 "parser.c"
        break;
      case 204:
// 1889 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("object-property-decr", NULL, yymsp[-3].minor.yy0, yymsp[-1].minor.yy0, NULL, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-2].minor);
  yy_destructor(95,&yymsp[0].minor);
}
// 5241 "parser.c"
        break;
      case 205:
// 1894 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("incr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(94,&yymsp[0].minor);
}
// 5249 "parser.c"
        break;
      case 206:
// 1899 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("decr", NULL, yymsp[-1].minor.yy0, NULL, NULL, NULL, status->scanner_state);
  yy_destructor(95,&yymsp[0].minor);
}
// 5257 "parser.c"
        break;
      case 207:
// 1904 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_let_assignment("dynamic-variable", yymsp[-1].minor.yy31, yymsp[-3].minor.yy0, NULL, NULL, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
}
// 5266 "parser.c"
        break;
      case 209:
// 1912 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_echo_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(96,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5275 "parser.c"
        break;
      case 212:
// 1924 "parser.lemon"
{
	yygotominor.yy31 = yymsp[0].minor.yy31;;
}
// 5282 "parser.c"
        break;
      case 213:
// 1929 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_mcall_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5290 "parser.c"
        break;
      case 214:
// 1934 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_fcall_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5298 "parser.c"
        break;
      case 215:
// 1939 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_scall_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5306 "parser.c"
        break;
      case 216:
// 1944 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_fetch_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(42,&yymsp[0].minor);
}
// 5314 "parser.c"
        break;
      case 217:
// 1949 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_return_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(97,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5323 "parser.c"
        break;
      case 218:
// 1954 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_return_statement(NULL, status->scanner_state);
  yy_destructor(97,&yymsp[-1].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5332 "parser.c"
        break;
      case 219:
// 1959 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_require_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(7,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5341 "parser.c"
        break;
      case 220:
// 1964 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_unset_statement(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(98,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5350 "parser.c"
        break;
      case 221:
// 1969 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_throw_exception(yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(99,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5359 "parser.c"
        break;
      case 222:
// 1973 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_INTEGER, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(60,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5368 "parser.c"
        break;
      case 223:
// 1977 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_UINTEGER, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(61,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5377 "parser.c"
        break;
      case 224:
// 1981 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_CHAR, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(64,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5386 "parser.c"
        break;
      case 225:
// 1985 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_UCHAR, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(65,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5395 "parser.c"
        break;
      case 226:
// 1989 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_LONG, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(62,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5404 "parser.c"
        break;
      case 227:
// 1993 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_ULONG, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(63,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5413 "parser.c"
        break;
      case 228:
// 1997 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_DOUBLE, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(66,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5422 "parser.c"
        break;
      case 229:
// 2001 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_STRING, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(68,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5431 "parser.c"
        break;
      case 230:
// 2005 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_BOOL, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(67,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5440 "parser.c"
        break;
      case 231:
// 2009 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_statement(XX_T_TYPE_VAR, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(70,&yymsp[-2].minor);
  yy_destructor(42,&yymsp[0].minor);
}
// 5449 "parser.c"
        break;
      case 234:
// 2021 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_variable(yymsp[0].minor.yy0, NULL, status->scanner_state);
}
// 5456 "parser.c"
        break;
      case 235:
// 2025 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_declare_variable(yymsp[-2].minor.yy0, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(52,&yymsp[-1].minor);
}
// 5464 "parser.c"
        break;
      case 237:
// 2033 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("not", yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(36,&yymsp[-1].minor);
}
// 5472 "parser.c"
        break;
      case 238:
// 2037 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("isset", yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(32,&yymsp[-1].minor);
}
// 5480 "parser.c"
        break;
      case 239:
// 2041 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("require", yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(7,&yymsp[-1].minor);
}
// 5488 "parser.c"
        break;
      case 240:
// 2045 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("clone", yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(11,&yymsp[-1].minor);
}
// 5496 "parser.c"
        break;
      case 241:
// 2049 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("empty", yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(12,&yymsp[-1].minor);
}
// 5504 "parser.c"
        break;
      case 242:
// 2053 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("likely", yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(9,&yymsp[-1].minor);
}
// 5512 "parser.c"
        break;
      case 243:
// 2057 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("unlikely", yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(10,&yymsp[-1].minor);
}
// 5520 "parser.c"
        break;
      case 244:
// 2061 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("equals", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(18,&yymsp[-1].minor);
}
// 5528 "parser.c"
        break;
      case 245:
// 2065 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("not-equals", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(25,&yymsp[-1].minor);
}
// 5536 "parser.c"
        break;
      case 246:
// 2069 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("identical", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(19,&yymsp[-1].minor);
}
// 5544 "parser.c"
        break;
      case 247:
// 2073 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("not-identical", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(24,&yymsp[-1].minor);
}
// 5552 "parser.c"
        break;
      case 248:
// 2077 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("less", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-1].minor);
}
// 5560 "parser.c"
        break;
      case 249:
// 2081 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("greater", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5568 "parser.c"
        break;
      case 250:
// 2085 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("less-equal", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(22,&yymsp[-1].minor);
}
// 5576 "parser.c"
        break;
      case 251:
// 2089 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("greater-equal", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(23,&yymsp[-1].minor);
}
// 5584 "parser.c"
        break;
      case 252:
// 2093 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("list", yymsp[-1].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5593 "parser.c"
        break;
      case 253:
// 2097 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("cast", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-3].minor);
  yy_destructor(37,&yymsp[-1].minor);
}
// 5602 "parser.c"
        break;
      case 254:
// 2101 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("type-hint", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(20,&yymsp[-3].minor);
  yy_destructor(21,&yymsp[-1].minor);
}
// 5611 "parser.c"
        break;
      case 255:
// 2105 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("property-access", yymsp[-2].minor.yy31, xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-1].minor);
}
// 5619 "parser.c"
        break;
      case 256:
// 2109 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("property-dynamic-access", yymsp[-4].minor.yy31, xx_ret_literal(XX_T_IDENTIFIER, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5629 "parser.c"
        break;
      case 257:
// 2113 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("property-string-access", yymsp[-4].minor.yy31, xx_ret_literal(XX_T_STRING, yymsp[-1].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5639 "parser.c"
        break;
      case 258:
// 2117 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("static-property-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-1].minor);
}
// 5647 "parser.c"
        break;
      case 259:
      case 328:
// 2121 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("static-constant-access", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), xx_ret_literal(XX_T_IDENTIFIER, yymsp[0].minor.yy0, status->scanner_state), NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-1].minor);
}
// 5656 "parser.c"
        break;
      case 260:
// 2130 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("array-access", yymsp[-3].minor.yy31, yymsp[-1].minor.yy31, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5665 "parser.c"
        break;
      case 261:
// 2135 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("add", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(26,&yymsp[-1].minor);
}
// 5673 "parser.c"
        break;
      case 262:
// 2140 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("sub", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(27,&yymsp[-1].minor);
}
// 5681 "parser.c"
        break;
      case 263:
// 2145 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("mul", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(29,&yymsp[-1].minor);
}
// 5689 "parser.c"
        break;
      case 264:
// 2150 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("div", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(30,&yymsp[-1].minor);
}
// 5697 "parser.c"
        break;
      case 265:
// 2155 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("mod", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(31,&yymsp[-1].minor);
}
// 5705 "parser.c"
        break;
      case 266:
// 2160 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("concat", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(28,&yymsp[-1].minor);
}
// 5713 "parser.c"
        break;
      case 267:
// 2165 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("and", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(14,&yymsp[-1].minor);
}
// 5721 "parser.c"
        break;
      case 268:
// 2170 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("or", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(15,&yymsp[-1].minor);
}
// 5729 "parser.c"
        break;
      case 269:
// 2175 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("instanceof", yymsp[-2].minor.yy31, yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(13,&yymsp[-1].minor);
}
// 5737 "parser.c"
        break;
      case 270:
// 2180 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("fetch", xx_ret_literal(XX_T_IDENTIFIER, yymsp[-2].minor.yy0, status->scanner_state), yymsp[0].minor.yy31, NULL, status->scanner_state);
  yy_destructor(33,&yymsp[-3].minor);
  yy_destructor(6,&yymsp[-1].minor);
}
// 5746 "parser.c"
        break;
      case 272:
// 2190 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("typeof", yymsp[0].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(34,&yymsp[-1].minor);
}
// 5754 "parser.c"
        break;
      case 274:
      case 319:
      case 321:
// 2200 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_INTEGER, yymsp[0].minor.yy0, status->scanner_state);
}
// 5763 "parser.c"
        break;
      case 275:
      case 318:
      case 323:
// 2205 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_STRING, yymsp[0].minor.yy0, status->scanner_state);
}
// 5772 "parser.c"
        break;
      case 276:
      case 322:
// 2210 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_CHAR, yymsp[0].minor.yy0, status->scanner_state);
}
// 5780 "parser.c"
        break;
      case 277:
      case 324:
// 2215 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_DOUBLE, yymsp[0].minor.yy0, status->scanner_state);
}
// 5788 "parser.c"
        break;
      case 278:
      case 325:
// 2220 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_NULL, NULL, status->scanner_state);
  yy_destructor(103,&yymsp[0].minor);
}
// 5797 "parser.c"
        break;
      case 279:
      case 327:
// 2225 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_TRUE, NULL, status->scanner_state);
  yy_destructor(104,&yymsp[0].minor);
}
// 5806 "parser.c"
        break;
      case 280:
      case 326:
// 2230 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_FALSE, NULL, status->scanner_state);
  yy_destructor(105,&yymsp[0].minor);
}
// 5815 "parser.c"
        break;
      case 281:
      case 329:
// 2235 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_literal(XX_T_CONSTANT, yymsp[0].minor.yy0, status->scanner_state);
}
// 5823 "parser.c"
        break;
      case 282:
// 2240 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5832 "parser.c"
        break;
      case 283:
// 2245 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("array", yymsp[-1].minor.yy31, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-2].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 5841 "parser.c"
        break;
      case 284:
// 2250 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_new_instance(0, yymsp[0].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-1].minor);
}
// 5849 "parser.c"
        break;
      case 285:
// 2255 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_new_instance(0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5859 "parser.c"
        break;
      case 286:
// 2260 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_new_instance(0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(35,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5869 "parser.c"
        break;
      case 287:
// 2265 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_new_instance(1, yymsp[-1].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-3].minor);
  yy_destructor(49,&yymsp[-2].minor);
  yy_destructor(50,&yymsp[0].minor);
}
// 5879 "parser.c"
        break;
      case 288:
// 2270 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_new_instance(1, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(35,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5891 "parser.c"
        break;
      case 289:
// 2275 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_new_instance(1, yymsp[-4].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(35,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5903 "parser.c"
        break;
      case 290:
// 2280 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_fcall(1, yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5912 "parser.c"
        break;
      case 291:
// 2285 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_fcall(1, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5921 "parser.c"
        break;
      case 292:
// 2290 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_fcall(2, yymsp[-4].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5932 "parser.c"
        break;
      case 293:
// 2295 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_fcall(2, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5943 "parser.c"
        break;
      case 294:
// 2300 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_scall(0, yymsp[-5].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(93,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5953 "parser.c"
        break;
      case 295:
// 2305 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_scall(0, yymsp[-4].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(93,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5963 "parser.c"
        break;
      case 296:
// 2310 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_scall(1, yymsp[-6].minor.yy0, yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(49,&yymsp[-7].minor);
  yy_destructor(50,&yymsp[-5].minor);
  yy_destructor(93,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5975 "parser.c"
        break;
      case 297:
// 2315 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_scall(1, yymsp[-5].minor.yy0, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-6].minor);
  yy_destructor(50,&yymsp[-4].minor);
  yy_destructor(93,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 5987 "parser.c"
        break;
      case 298:
// 2320 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_scall(1, yymsp[-8].minor.yy0, yymsp[-4].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(49,&yymsp[-9].minor);
  yy_destructor(50,&yymsp[-7].minor);
  yy_destructor(93,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6001 "parser.c"
        break;
      case 299:
// 2325 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_scall(1, yymsp[-7].minor.yy0, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(49,&yymsp[-8].minor);
  yy_destructor(50,&yymsp[-6].minor);
  yy_destructor(93,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6015 "parser.c"
        break;
      case 300:
// 2330 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_mcall(1, yymsp[-5].minor.yy31, yymsp[-3].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(39,&yymsp[-4].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6025 "parser.c"
        break;
      case 301:
// 2335 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_mcall(1, yymsp[-4].minor.yy31, yymsp[-2].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6035 "parser.c"
        break;
      case 302:
// 2340 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_mcall(2, yymsp[-7].minor.yy31, yymsp[-4].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6047 "parser.c"
        break;
      case 303:
// 2345 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_mcall(2, yymsp[-6].minor.yy31, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6059 "parser.c"
        break;
      case 304:
// 2350 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_mcall(3, yymsp[-7].minor.yy31, yymsp[-4].minor.yy0, yymsp[-1].minor.yy31, status->scanner_state);
  yy_destructor(39,&yymsp[-6].minor);
  yy_destructor(49,&yymsp[-5].minor);
  yy_destructor(50,&yymsp[-3].minor);
  yy_destructor(56,&yymsp[-2].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6071 "parser.c"
        break;
      case 305:
// 2355 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_mcall(3, yymsp[-6].minor.yy31, yymsp[-3].minor.yy0, NULL, status->scanner_state);
  yy_destructor(39,&yymsp[-5].minor);
  yy_destructor(49,&yymsp[-4].minor);
  yy_destructor(50,&yymsp[-2].minor);
  yy_destructor(56,&yymsp[-1].minor);
  yy_destructor(37,&yymsp[0].minor);
}
// 6083 "parser.c"
        break;
      case 309:
// 2375 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_expr("ternary", yymsp[-4].minor.yy31, yymsp[-2].minor.yy31, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(8,&yymsp[-3].minor);
  yy_destructor(77,&yymsp[-1].minor);
}
// 6092 "parser.c"
        break;
      case 315:
// 2399 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_array_item(yymsp[-2].minor.yy31, yymsp[0].minor.yy31, status->scanner_state);
  yy_destructor(77,&yymsp[-1].minor);
}
// 6100 "parser.c"
        break;
      case 316:
// 2403 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_array_item(NULL, yymsp[0].minor.yy31, status->scanner_state);
}
// 6107 "parser.c"
        break;
      case 330:
// 2459 "parser.lemon"
{
  yygotominor.yy31 = xx_ret_expr("empty-array", NULL, NULL, NULL, status->scanner_state);
  yy_destructor(38,&yymsp[-1].minor);
  yy_destructor(59,&yymsp[0].minor);
}
// 6116 "parser.c"
        break;
      case 332:
// 2467 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_comment(yymsp[0].minor.yy0, status->scanner_state);
}
// 6123 "parser.c"
        break;
      case 333:
// 2471 "parser.lemon"
{
	yygotominor.yy31 = xx_ret_cblock(yymsp[0].minor.yy0, status->scanner_state);
}
// 6130 "parser.c"
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
// 1002 "parser.lemon"


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

// 6206 "parser.c"
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
 | Copyright (c) 2013-2014 Zephir Team and contributors                          |
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
