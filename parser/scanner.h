
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

#define XX_SCANNER_RETCODE_EOF -1
#define XX_SCANNER_RETCODE_ERR -2
#define XX_SCANNER_RETCODE_IMPOSSIBLE -3

/** Modes */
#define XX_T_IGNORE 297

/* Literals & Identifiers */
#define XX_T_INTEGER 301
#define XX_T_DOUBLE 302
#define XX_T_STRING 303
#define XX_T_NULL 304
#define XX_T_FALSE 305
#define XX_T_TRUE 306
#define XX_T_IDENTIFIER 307
#define XX_T_ARRAY 308
#define XX_T_CHAR 309

#define XX_T_TYPE_INTEGER 310
#define XX_T_TYPE_DOUBLE 311
#define XX_T_TYPE_BOOL 312
#define XX_T_TYPE_STRING 313
#define XX_T_TYPE_VAR 314
#define XX_T_TYPE_LONG 315
#define XX_T_TYPE_ULONG 316
#define XX_T_TYPE_CHAR 317
#define XX_T_TYPE_UCHAR 318
#define XX_T_TYPE_UINTEGER 319

#define XX_T_NAMESPACE 350
#define XX_T_CLASS 351
#define XX_T_PUBLIC 352
#define XX_T_PROTECTED 353
#define XX_T_EXTENDS 354
#define XX_T_FUNCTION 355
#define XX_T_LET 356
#define XX_T_COMMENT 357
#define XX_T_ECHO 358
#define XX_T_CONST 359
#define XX_T_ABSTRACT 360
#define XX_T_IMPLEMENTS 361
#define XX_T_INTERFACE 362
#define XX_T_IF 363
#define XX_T_ELSE 364
#define XX_T_WHILE 365
#define XX_T_NEW 366
#define XX_T_RETURN 367
#define XX_T_LOOP 368
#define XX_T_BREAK 369
#define XX_T_CONTINUE 370
#define XX_T_INSTANCEOF 371
#define XX_T_TYPEOF 372
#define XX_T_ISSET 373
#define XX_T_UNSET 374
#define XX_T_THROW 375
#define XX_T_FOR 376
#define XX_T_IN 377
#define XX_T_FETCH 378
#define XX_T_SWITCH 379
#define XX_T_CASE 380
#define XX_T_DEFAULT 381
#define XX_T_REVERSE 382
#define XX_T_PRIVATE 383
#define XX_T_STATIC 384
#define XX_T_INLINE 385
#define XX_T_FINAL 386
#define XX_T_CONSTANT 387
#define XX_T_DO 388
#define XX_T_REQUIRE 389

/* Operators */
#define XX_T_AT '@'
#define XX_T_DOT '.'
#define XX_T_COMMA ','
#define XX_T_ASSIGN '='
#define XX_T_LESS '<'
#define XX_T_GREATER '>'
#define XX_T_COLON ':'
#define XX_T_DOTCOMMA ';'
#define XX_T_BRACKET_OPEN '{'
#define XX_T_BRACKET_CLOSE '}'
#define XX_T_SBRACKET_OPEN '['
#define XX_T_SBRACKET_CLOSE ']'
#define XX_T_PARENTHESES_OPEN '('
#define XX_T_PARENTHESES_CLOSE ')'
#define XX_T_ARROW 400
#define XX_T_EQUALS 401
#define XX_T_IDENTICAL 402
#define XX_T_ADD '+'
#define XX_T_SUB '-'
#define XX_T_MUL '*'
#define XX_T_DIV '/'
#define XX_T_MOD '%'
#define XX_T_INCR 403
#define XX_T_DECR 404
#define XX_T_NOTEQUALS 405
#define XX_T_NOTIDENTICAL 406
#define XX_T_NOT 407
#define XX_T_GREATEREQUAL 408
#define XX_T_LESSEQUAL 409
#define XX_T_ADDASSIGN 410
#define XX_T_SUBASSIGN 411
#define XX_T_MULASSIGN 412
#define XX_T_DIVASSIGN 413
#define XX_T_CONCATASSIGN 414
#define XX_T_AND 415
#define XX_T_OR 416
#define XX_T_DOUBLECOLON 417

/* List of tokens and their names */
typedef struct _xx_token_names {
	unsigned int code;
	char *name;
} xx_token_names;

/* Active token state */
typedef struct _xx_scanner_state {
	int active_token;
	char* start;
	char* end;
	unsigned int start_length;
	int mode;
	unsigned int active_line;
	unsigned int active_char;
	char *active_file;
} xx_scanner_state;

/* Extra information tokens */
typedef struct _xx_scanner_token {
	int opcode;
	char *value;
	int len;
} xx_scanner_token;

int xx_get_token(xx_scanner_state *s, xx_scanner_token *token);

extern const xx_token_names xx_tokens[];
