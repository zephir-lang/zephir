
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

typedef struct _xx_parser_token {
	int opcode;
	char *token;
	int token_len;
	int free_flag;
} xx_parser_token;

typedef struct _xx_parser_status {
	int status;
	json_object *ret;
	xx_scanner_state *scanner_state;
	xx_scanner_token *token;
	char *syntax_error;
	unsigned int syntax_error_len;
} xx_parser_status;

#define XX_PARSING_OK 1
#define XX_PARSING_FAILED 0

