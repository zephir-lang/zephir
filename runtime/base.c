
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

const zephir_token_names zephir_tokens[] =
{
	{ ZEPHIR_T_INTEGER,             "INTEGER" },
	{ ZEPHIR_T_DOUBLE,              "DOUBLE" },
	{ ZEPHIR_T_STRING,              "STRING" },
	{ ZEPHIR_T_IDENTIFIER,    		"IDENTIFIER" },
	{ ZEPHIR_T_AT,                  "@" },
	{ ZEPHIR_T_COMMA,               "," },
	{ ZEPHIR_T_ASSIGN,              "=" },
	{ ZEPHIR_T_COLON,               ":" },
	{ ZEPHIR_T_PARENTHESES_OPEN,    "(" },
	{ ZEPHIR_T_PARENTHESES_CLOSE,   ")" },
	{ ZEPHIR_T_BRACKET_OPEN,        "{" },
	{ ZEPHIR_T_BRACKET_CLOSE,       "}" },
	{ ZEPHIR_T_SBRACKET_OPEN,       "[" },
	{ ZEPHIR_T_SBRACKET_CLOSE,      "]" },
	{  0, NULL }
};

/**
 * Wrapper to alloc memory within the parser
 */
static void *zephir_wrapper_alloc(size_t bytes){
	return emalloc(bytes);
}

/**
 * Wrapper to free memory within the parser
 */
static void zephir_wrapper_free(void *pointer){
	efree(pointer);
}

/**
 * Creates a parser_token to be passed to the parser
 */
static void zephir_parse_with_token(void* zephir_parser, int opcode, int parsercode, zephir_scanner_token *token, zephir_parser_status *parser_status){

	zephir_parser_token *pToken;

	pToken = emalloc(sizeof(zephir_parser_token));
	pToken->opcode = opcode;
	pToken->token = token->value;
	pToken->token_len = token->len;
	pToken->free_flag = 1;

	zephir_(zephir_parser, parsercode, pToken, parser_status);

	token->value = NULL;
	token->len = 0;
}

/**
 * Creates an error message when it's triggered by the scanner
 */
static void zephir_scanner_error_msg(zephir_parser_status *parser_status){

	/*char *error, *error_part;
	ZEPHIR_scanner_state *state = parser_status->scanner_state;

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
 * Parses a program returning an intermediate array representation
 */
int zephir_parse_program(zval **return_value, char *program, unsigned int program_length, const char *file_path, zval **error_msg TSRMLS_DC) {

	char *error;
	zephir_scanner_state *state;
	zephir_scanner_token token;
	int scanner_status, status = SUCCESS, start_lines;
	zephir_parser_status *parser_status = NULL;
	void* zephir_parser;

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
	zephir_parser = zephir_Alloc(zephir_wrapper_alloc);

	parser_status = emalloc(sizeof(zephir_parser_status));
	state = emalloc(sizeof(zephir_scanner_state));

	parser_status->status = ZEPHIR_PARSING_OK;
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

	while (0 <= (scanner_status = zephir_get_token(state, &token))) {

		state->active_token = token.opcode;

		state->start_length = (program + program_length - state->start);

		switch (token.opcode) {

			case ZEPHIR_T_IGNORE:
				break;

			case ZEPHIR_T_NAMESPACE:
				zephir_(zephir_parser, ZEPHIR_NAMESPACE, NULL, parser_status);
				break;
			case ZEPHIR_T_ABSTRACT:
				zephir_(zephir_parser, ZEPHIR_ABSTRACT, NULL, parser_status);
				break;
			case ZEPHIR_T_CLASS:
				zephir_(zephir_parser, ZEPHIR_CLASS, NULL, parser_status);
				break;
			case ZEPHIR_T_INTERFACE:
				zephir_(zephir_parser, ZEPHIR_INTERFACE, NULL, parser_status);
				break;
			case ZEPHIR_T_EXTENDS:
				zephir_(zephir_parser, ZEPHIR_EXTENDS, NULL, parser_status);
				break;
			case ZEPHIR_T_IMPLEMENTS:
				zephir_(zephir_parser, ZEPHIR_IMPLEMENTS, NULL, parser_status);
				break;
			case ZEPHIR_T_PUBLIC:
				zephir_(zephir_parser, ZEPHIR_PUBLIC, NULL, parser_status);
				break;
			case ZEPHIR_T_PROTECTED:
				zephir_(zephir_parser, ZEPHIR_PROTECTED, NULL, parser_status);
				break;
			case ZEPHIR_T_PRIVATE:
				zephir_(zephir_parser, ZEPHIR_PRIVATE, NULL, parser_status);
				break;
			case ZEPHIR_T_STATIC:
				zephir_(zephir_parser, ZEPHIR_STATIC, NULL, parser_status);
				break;
			case ZEPHIR_T_INLINE:
				zephir_(zephir_parser, ZEPHIR_INLINE, NULL, parser_status);
				break;
			case ZEPHIR_T_FINAL:
				zephir_(zephir_parser, ZEPHIR_FINAL, NULL, parser_status);
				break;
			case ZEPHIR_T_FUNCTION:
				zephir_(zephir_parser, ZEPHIR_FUNCTION, NULL, parser_status);
				break;
			case ZEPHIR_T_LET:
				zephir_(zephir_parser, ZEPHIR_LET, NULL, parser_status);
				break;
			case ZEPHIR_T_ECHO:
				zephir_(zephir_parser, ZEPHIR_ECHO, NULL, parser_status);
				break;
			case ZEPHIR_T_RETURN:
				zephir_(zephir_parser, ZEPHIR_RETURN, NULL, parser_status);
				break;
			case ZEPHIR_T_REQUIRE:
				zephir_(zephir_parser, ZEPHIR_REQUIRE, NULL, parser_status);
				break;
			case ZEPHIR_T_CLONE:
				zephir_(zephir_parser, ZEPHIR_CLONE, NULL, parser_status);
				break;
			case ZEPHIR_T_EMPTY:
				zephir_(zephir_parser, ZEPHIR_EMPTY, NULL, parser_status);
				break;
			case ZEPHIR_T_IF:
				zephir_(zephir_parser, ZEPHIR_IF, NULL, parser_status);
				break;
			case ZEPHIR_T_ELSE:
				zephir_(zephir_parser, ZEPHIR_ELSE, NULL, parser_status);
				break;
			case ZEPHIR_T_LOOP:
				zephir_(zephir_parser, ZEPHIR_LOOP, NULL, parser_status);
				break;
			case ZEPHIR_T_CONTINUE:
				zephir_(zephir_parser, ZEPHIR_CONTINUE, NULL, parser_status);
				break;
			case ZEPHIR_T_BREAK:
				zephir_(zephir_parser, ZEPHIR_BREAK, NULL, parser_status);
				break;
			case ZEPHIR_T_WHILE:
				zephir_(zephir_parser, ZEPHIR_WHILE, NULL, parser_status);
				break;
			case ZEPHIR_T_DO:
				zephir_(zephir_parser, ZEPHIR_DO, NULL, parser_status);
				break;
			case ZEPHIR_T_NEW:
				zephir_(zephir_parser, ZEPHIR_NEW, NULL, parser_status);
				break;
			case ZEPHIR_T_CONST:
				zephir_(zephir_parser, ZEPHIR_CONST, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPEOF:
				zephir_(zephir_parser, ZEPHIR_TYPEOF, NULL, parser_status);
				break;
			case ZEPHIR_T_INSTANCEOF:
				zephir_(zephir_parser, ZEPHIR_INSTANCEOF, NULL, parser_status);
				break;
			case ZEPHIR_T_ISSET:
				zephir_(zephir_parser, ZEPHIR_ISSET, NULL, parser_status);
				break;
			case ZEPHIR_T_UNSET:
				zephir_(zephir_parser, ZEPHIR_UNSET, NULL, parser_status);
				break;
			case ZEPHIR_T_THROW:
				zephir_(zephir_parser, ZEPHIR_THROW, NULL, parser_status);
				break;
			case ZEPHIR_T_FOR:
				zephir_(zephir_parser, ZEPHIR_FOR, NULL, parser_status);
				break;
			case ZEPHIR_T_IN:
				zephir_(zephir_parser, ZEPHIR_IN, NULL, parser_status);
				break;
			case ZEPHIR_T_REVERSE:
				zephir_(zephir_parser, ZEPHIR_REVERSE, NULL, parser_status);
				break;
			case ZEPHIR_T_USE:
				zephir_(zephir_parser, ZEPHIR_USE, NULL, parser_status);
				break;
			case ZEPHIR_T_AS:
				zephir_(zephir_parser, ZEPHIR_AS, NULL, parser_status);
				break;
			case ZEPHIR_T_TRY:
				zephir_(zephir_parser, ZEPHIR_TRY, NULL, parser_status);
				break;
			case ZEPHIR_T_CATCH:
				zephir_(zephir_parser, ZEPHIR_CATCH, NULL, parser_status);
				break;

			case ZEPHIR_T_DOTCOMMA:
				zephir_(zephir_parser, ZEPHIR_DOTCOMMA, NULL, parser_status);
				break;
			case ZEPHIR_T_COMMA:
				zephir_(zephir_parser, ZEPHIR_COMMA, NULL, parser_status);
				break;
			case ZEPHIR_T_ASSIGN:
				zephir_(zephir_parser, ZEPHIR_ASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_ADDASSIGN:
				zephir_(zephir_parser, ZEPHIR_ADDASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_SUBASSIGN:
				zephir_(zephir_parser, ZEPHIR_SUBASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_DIVASSIGN:
				zephir_(zephir_parser, ZEPHIR_DIVASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_MULASSIGN:
				zephir_(zephir_parser, ZEPHIR_MULASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_CONCATASSIGN:
				zephir_(zephir_parser, ZEPHIR_CONCATASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_MODASSIGN:
				zephir_(zephir_parser, ZEPHIR_MODASSIGN, NULL, parser_status);
				break;
			case ZEPHIR_T_EQUALS:
				zephir_(zephir_parser, ZEPHIR_EQUALS, NULL, parser_status);
				break;
			case ZEPHIR_T_NOTEQUALS:
				zephir_(zephir_parser, ZEPHIR_NOTEQUALS, NULL, parser_status);
				break;
			case ZEPHIR_T_IDENTICAL:
				zephir_(zephir_parser, ZEPHIR_IDENTICAL, NULL, parser_status);
				break;
			case ZEPHIR_T_NOTIDENTICAL:
				zephir_(zephir_parser, ZEPHIR_NOTIDENTICAL, NULL, parser_status);
				break;
			case ZEPHIR_T_LESS:
				zephir_(zephir_parser, ZEPHIR_LESS, NULL, parser_status);
				break;
			case ZEPHIR_T_GREATER:
				zephir_(zephir_parser, ZEPHIR_GREATER, NULL, parser_status);
				break;
			case ZEPHIR_T_LESSEQUAL:
				zephir_(zephir_parser, ZEPHIR_LESSEQUAL, NULL, parser_status);
				break;
			case ZEPHIR_T_GREATEREQUAL:
				zephir_(zephir_parser, ZEPHIR_GREATEREQUAL, NULL, parser_status);
				break;
			case ZEPHIR_T_QUESTION:
				zephir_(zephir_parser, ZEPHIR_QUESTION, NULL, parser_status);
				break;
			case ZEPHIR_T_COLON:
				zephir_(zephir_parser, ZEPHIR_COLON, NULL, parser_status);
				break;
			case ZEPHIR_T_ARROW:
				zephir_(zephir_parser, ZEPHIR_ARROW, NULL, parser_status);
				break;
			case ZEPHIR_T_DOUBLECOLON:
				zephir_(zephir_parser, ZEPHIR_DOUBLECOLON, NULL, parser_status);
				break;
			case ZEPHIR_T_NOT:
				zephir_(zephir_parser, ZEPHIR_NOT, NULL, parser_status);
				break;
			case ZEPHIR_T_FETCH:
				zephir_(zephir_parser, ZEPHIR_FETCH, NULL, parser_status);
				break;
			case ZEPHIR_T_SWITCH:
				zephir_(zephir_parser, ZEPHIR_SWITCH, NULL, parser_status);
				break;
			case ZEPHIR_T_CASE:
				zephir_(zephir_parser, ZEPHIR_CASE, NULL, parser_status);
				break;
			case ZEPHIR_T_DEFAULT:
				zephir_(zephir_parser, ZEPHIR_DEFAULT, NULL, parser_status);
				break;

			case ZEPHIR_T_PARENTHESES_OPEN:
				zephir_(zephir_parser, ZEPHIR_PARENTHESES_OPEN, NULL, parser_status);
				break;
			case ZEPHIR_T_PARENTHESES_CLOSE:
				zephir_(zephir_parser, ZEPHIR_PARENTHESES_CLOSE, NULL, parser_status);
				break;

			case ZEPHIR_T_BRACKET_OPEN:
				parser_status->number_brackets++;
				zephir_(zephir_parser, ZEPHIR_BRACKET_OPEN, NULL, parser_status);
				break;
			case ZEPHIR_T_BRACKET_CLOSE:
				parser_status->number_brackets--;
				zephir_(zephir_parser, ZEPHIR_BRACKET_CLOSE, NULL, parser_status);
				break;

			case ZEPHIR_T_SBRACKET_OPEN:
				zephir_(zephir_parser, ZEPHIR_SBRACKET_OPEN, NULL, parser_status);
				break;
			case ZEPHIR_T_SBRACKET_CLOSE:
				zephir_(zephir_parser, ZEPHIR_SBRACKET_CLOSE, NULL, parser_status);
				break;

			case ZEPHIR_T_NULL:
				zephir_(zephir_parser, ZEPHIR_NULL, NULL, parser_status);
				break;
			case ZEPHIR_T_TRUE:
				zephir_(zephir_parser, ZEPHIR_TRUE, NULL, parser_status);
				break;
			case ZEPHIR_T_FALSE:
				zephir_(zephir_parser, ZEPHIR_FALSE, NULL, parser_status);
				break;
			case ZEPHIR_T_COMMENT:
				if (parser_status->number_brackets <= 1) {
                    zephir_parse_with_token(zephir_parser, ZEPHIR_T_COMMENT, ZEPHIR_COMMENT, &token, parser_status);
				}
				break;
			case ZEPHIR_T_CBLOCK:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_CBLOCK, ZEPHIR_CBLOCK, &token, parser_status);
				break;
			case ZEPHIR_T_TYPE_INTEGER:
				zephir_(zephir_parser, ZEPHIR_TYPE_INTEGER, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_UINTEGER:
				zephir_(zephir_parser, ZEPHIR_TYPE_UINTEGER, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_CHAR:
				zephir_(zephir_parser, ZEPHIR_TYPE_CHAR, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_UCHAR:
				zephir_(zephir_parser, ZEPHIR_TYPE_UCHAR, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_LONG:
				zephir_(zephir_parser, ZEPHIR_TYPE_LONG, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_ULONG:
				zephir_(zephir_parser, ZEPHIR_TYPE_ULONG, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_DOUBLE:
				zephir_(zephir_parser, ZEPHIR_TYPE_DOUBLE, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_STRING:
				zephir_(zephir_parser, ZEPHIR_TYPE_STRING, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_BOOL:
				zephir_(zephir_parser, ZEPHIR_TYPE_BOOL, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_ARRAY:
				zephir_(zephir_parser, ZEPHIR_TYPE_ARRAY, NULL, parser_status);
				break;
			case ZEPHIR_T_TYPE_VAR:
				zephir_(zephir_parser, ZEPHIR_TYPE_VAR, NULL, parser_status);
				break;
            case ZEPHIR_T_TYPE_OBJECT:
                zephir_(zephir_parser, ZEPHIR_TYPE_OBJECT, NULL, parser_status);
                break;
            case ZEPHIR_T_TYPE_RESOURCE:
                zephir_(zephir_parser, ZEPHIR_TYPE_RESOURCE, NULL, parser_status);
                break;
            case ZEPHIR_T_TYPE_CALLABLE:
                zephir_(zephir_parser, ZEPHIR_TYPE_CALLABLE, NULL, parser_status);
                break;

			case ZEPHIR_T_ADD:
				zephir_(zephir_parser, ZEPHIR_ADD, NULL, parser_status);
				break;
			case ZEPHIR_T_SUB:
				zephir_(zephir_parser, ZEPHIR_SUB, NULL, parser_status);
				break;
			case ZEPHIR_T_MUL:
				zephir_(zephir_parser, ZEPHIR_MUL, NULL, parser_status);
				break;
			case ZEPHIR_T_DIV:
				zephir_(zephir_parser, ZEPHIR_DIV, NULL, parser_status);
				break;
			case ZEPHIR_T_MOD:
				zephir_(zephir_parser, ZEPHIR_MOD, NULL, parser_status);
				break;
			case ZEPHIR_T_DOT:
				zephir_(zephir_parser, ZEPHIR_CONCAT, NULL, parser_status);
				break;
			case ZEPHIR_T_INCR:
				zephir_(zephir_parser, ZEPHIR_INCR, NULL, parser_status);
				break;
			case ZEPHIR_T_DECR:
				zephir_(zephir_parser, ZEPHIR_DECR, NULL, parser_status);
				break;
			case ZEPHIR_T_AND:
				zephir_(zephir_parser, ZEPHIR_AND, NULL, parser_status);
				break;
			case ZEPHIR_T_OR:
				zephir_(zephir_parser, ZEPHIR_OR, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_AND:
				zephir_(zephir_parser, ZEPHIR_BITWISE_AND, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_OR:
				zephir_(zephir_parser, ZEPHIR_BITWISE_OR, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_XOR:
				zephir_(zephir_parser, ZEPHIR_BITWISE_XOR, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_SHIFTLEFT:
				zephir_(zephir_parser, ZEPHIR_BITWISE_SHIFTLEFT, NULL, parser_status);
				break;
			case ZEPHIR_T_BITWISE_SHIFTRIGHT:
				zephir_(zephir_parser, ZEPHIR_BITWISE_SHIFTRIGHT, NULL, parser_status);
				break;
			case ZEPHIR_T_INTEGER:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_INTEGER, ZEPHIR_INTEGER, &token, parser_status);
				break;
			case ZEPHIR_T_DOUBLE:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_DOUBLE, ZEPHIR_DOUBLE, &token, parser_status);
				break;
			case ZEPHIR_T_STRING:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_STRING, ZEPHIR_STRING, &token, parser_status);
				break;
			case ZEPHIR_T_CHAR:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_CHAR, ZEPHIR_CHAR, &token, parser_status);
				break;
			case ZEPHIR_T_IDENTIFIER:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_IDENTIFIER, ZEPHIR_IDENTIFIER, &token, parser_status);
				break;
			case ZEPHIR_T_CONSTANT:
				zephir_parse_with_token(zephir_parser, ZEPHIR_T_CONSTANT, ZEPHIR_CONSTANT, &token, parser_status);
				break;

			case ZEPHIR_T_VOID:
				zephir_(zephir_parser, ZEPHIR_VOID, NULL, parser_status);
				break;
			case ZEPHIR_T_LIKELY:
				zephir_(zephir_parser, ZEPHIR_LIKELY, NULL, parser_status);
				break;
			case ZEPHIR_T_UNLIKELY:
				zephir_(zephir_parser, ZEPHIR_UNLIKELY, NULL, parser_status);
				break;

			default:
				parser_status->status = ZEPHIR_PARSING_FAILED;
				if (!*error_msg) {
                    unsigned int length = sizeof(char) * 48; // Z_STRLEN_P(state->active_file);
					error = emalloc(length);
					snprintf(error, length, "Scanner: unknown opcode %d on in %s line %d", token.opcode, state->active_file, state->active_line);
					ALLOC_INIT_ZVAL(*error_msg);
					ZVAL_STRING(*error_msg, error, 1);
					efree(error);
				}
				break;
		}

		if (parser_status->status != ZEPHIR_PARSING_OK) {
			status = FAILURE;
			break;
		}

		state->end = state->start;
	}

	if (status != FAILURE) {
		switch (scanner_status) {
			case ZEPHIR_SCANNER_RETCODE_ERR:
			case ZEPHIR_SCANNER_RETCODE_IMPOSSIBLE:
				{
					char *x = emalloc(sizeof(char) * 1024);
					if (state->start) {
						snprintf(x, 1024, "Scanner error: %d %s", scanner_status, state->start);
					} else {
						snprintf(x, 1024, "Scanner error: %d", scanner_status);
					}
					status = FAILURE;
				}
				break;
			default:
				zephir_(zephir_parser, 0, NULL, parser_status);
		}
	}

	state->active_token = 0;
	state->start = NULL;

	if (parser_status->status != ZEPHIR_PARSING_OK) {
		status = FAILURE;
		if (parser_status->syntax_error) {
			if (!*error_msg) {
				ALLOC_INIT_ZVAL(*error_msg);
				ZVAL_STRING(*error_msg, parser_status->syntax_error, 1);
			}
			efree(parser_status->syntax_error);
		}
	}

	zephir_Free(zephir_parser, zephir_wrapper_free);

	if (status != FAILURE) {
		if (parser_status->status == ZEPHIR_PARSING_OK) {
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
		//zend_print_zval_r(parser_status->ret, 0 TSRMLS_CC);
		*return_value = parser_status->ret;
	}

	//efree(Z_STRVAL(processed_comment));*/

	efree(parser_status);
	efree(state);

	return status;
}
