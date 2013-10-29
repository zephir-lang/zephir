PHP_ARG_ENABLE(%PROJECT_LOWER%, whether to enable %PROJECT_LOWER%, [ --enable-%PROJECT_LOWER%   Enable %PROJECT_CAMELIZE%])

if test "$PHP_%PROJECT_UPPER%" = "yes"; then
	AC_DEFINE(HAVE_%PROJECT_UPPER%, 1, [Whether you have %PROJECT_CAMELIZE%])
	%PROJECT_LOWER%_sources="%PROJECT_LOWER%.c kernel/main.c kernel/memory.c kernel/exception.c kernel/hash.c kernel/debug.c kernel/backtrace.c kernel/object.c kernel/array.c kernel/string.c kernel/fcall.c kernel/alternative/fcall.c kernel/file.c kernel/operators.c kernel/concat.c %FILES_COMPILED%"
	PHP_NEW_EXTENSION(%PROJECT_LOWER%, $%PROJECT_LOWER%_sources, $ext_shared)
fi
