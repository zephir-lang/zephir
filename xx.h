
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

