
/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2016 Zephir Team and contributors                     |
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

#ifndef PHP_ZEPHIR_XX_H
#define PHP_ZEPHIR_XX_H

/* List of tokens and their names */
typedef struct _xx_token_names {
	unsigned int code;
	char *name;
} xx_token_names;

typedef struct _xx_memory_manager {
	zval ***slots;
	int number;
} xx_memory_manager;

/* Active token state */
typedef struct _xx_scanner_state {
	int active_token;
	char* start;
	char* end;
	unsigned int start_length;
	int mode;
	unsigned int active_line;
	unsigned int active_char;
	unsigned int class_line;
	unsigned int class_char;
	unsigned int method_line;
	unsigned int method_char;
	char *active_file;	
} xx_scanner_state;

/* Extra information tokens */
typedef struct _xx_scanner_token {
	int opcode;
	char *value;
	int len;
} xx_scanner_token;

typedef struct _xx_parser_token {
	int opcode;
	char *token;
	int token_len;
	int free_flag;
} xx_parser_token;

typedef struct _xx_parser_status {
	int status;
#if PHP_VERSION_ID < 70000
	zval *ret;
#else
	zval ret;
#endif
	xx_scanner_state *scanner_state;
	xx_scanner_token *token;
	char *syntax_error;
	unsigned int syntax_error_len;
	unsigned int number_brackets;
} xx_parser_status;

#define XX_PARSING_OK 1
#define XX_PARSING_FAILED 0

void parser_track_variable(xx_scanner_state *state, zval **var);
int parser_is_tracked(xx_scanner_state *state, zval **var);
void parser_free_variable(xx_scanner_state *state, zval **var);
int xx_get_token(xx_scanner_state *state, xx_scanner_token *token);

#endif
