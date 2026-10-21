<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Zephir\Code\Builder;

use Zephir\Exception\InvalidArgumentException;
use Zephir\Types\Types;

use function addcslashes;
use function array_key_exists;
use function count;
use function explode;
use function in_array;
use function is_float;
use function preg_match;
use function sprintf;
use function str_starts_with;
use function strrpos;
use function substr;
use function var_export;

/**
 * One entry of the `globals` section of config.json.
 *
 * This is the single place that decides, for a given global, what its C member
 * type is, whether it is exposed as a php.ini directive, and whether it still
 * needs a compiled-in default. Both emitters use it: plain globals through
 * Compiler::processExtensionGlobals() and dotted ones through Struct. Keeping
 * the decision in one object is what stops the two from drifting, which is how
 * `int` and `double` globals ended up with no directive at all (see #2449).
 */
final class ExtensionGlobal
{
    /**
     * Update handler per Zephir type.
     *
     * Every one of these is exported by the engine unchanged from PHP 8.0 to
     * 8.5, except `zephir_OnUpdateChar`, which the kernel supplies because
     * php-src has no stock handler writing a single `char` (ext/soap's
     * OnUpdateCacheMode is the same idea).
     *
     * A type absent from this map gets no directive: `hash` cannot be
     * expressed as an ini string, and the engine stores ini values as
     * zend_string only.
     */
    private const INI_HANDLERS = [
        Types::T_BOOL    => 'OnUpdateBool',
        Types::T_BOOLEAN => 'OnUpdateBool',
        Types::T_INT     => 'OnUpdateLong',
        Types::T_INTEGER => 'OnUpdateLong',
        Types::T_LONG    => 'OnUpdateLong',
        Types::T_UINT    => 'OnUpdateLongGEZero',
        Types::T_ULONG   => 'OnUpdateLongGEZero',
        Types::T_DOUBLE  => 'OnUpdateReal',
        Types::T_STRING  => 'OnUpdateString',
        Types::T_CHAR    => 'zephir_OnUpdateChar',
        Types::T_UCHAR   => 'zephir_OnUpdateChar',
    ];

    /**
     * C type per Zephir type.
     *
     * All four integer types are engine-width on purpose. A C `int` is 4 bytes
     * and OnUpdateLong writes a `zend_long` through the member address, so a
     * narrower member would corrupt whatever follows it; `long` additionally
     * changes width between data models (#2666).
     */
    private const C_TYPES = [
        Types::T_BOOL    => 'zend_bool',
        Types::T_BOOLEAN => 'zend_bool',
        Types::T_INT     => 'zend_long',
        Types::T_INTEGER => 'zend_long',
        Types::T_LONG    => 'zend_long',
        Types::T_UINT    => 'zend_ulong',
        Types::T_ULONG   => 'zend_ulong',
        Types::T_DOUBLE  => 'double',
        Types::T_CHAR    => 'char',
        Types::T_UCHAR   => 'unsigned char',
        // A plain C string pointer, which is what OnUpdateString writes. It
        // points either into the ini entry, which must never be freed, or at
        // the copy globals_set() owns (see ownedMember()).
        Types::T_STRING  => 'char *',
        Types::T_HASH    => 'HashTable *',
    ];

    /**
     * Prefix of the member holding the copy globals_set() writes into a
     * `string` global; see ownedMember().
     */
    private const OWNED_PREFIX = 'zephir_owned_';

    /**
     * Members the kernel declares in every extension's module globals, see
     * templates/engine/php_project.h. A global or a struct of the same name
     * would declare the member twice.
     */
    private const KERNEL_MEMBERS = ['initialized', 'fcache', 'scache', 'pcache', 'cache_enabled', 'recursive_lock'];

    /**
     * @param string $member     C member path, `my_setting` or `db.my_setting`
     * @param array  $definition the global's config.json block
     *
     * @throws InvalidArgumentException
     */
    public function __construct(private string $member, private array $definition)
    {
        if (!array_key_exists('type', $definition)) {
            throw new InvalidArgumentException(
                sprintf('Extension global "%s" has no type', $member)
            );
        }

        if (!array_key_exists('default', $definition)) {
            throw new InvalidArgumentException(
                sprintf('Extension global "%s" has no default value', $member)
            );
        }
    }

    /**
     * Rejects a `globals` key that cannot become a C member.
     *
     * A key is `member` or `struct.member`, and each part is emitted verbatim
     * as a C identifier. Anything past the second dot used to be dropped, so
     * `a.b.c` and `a.b.d` silently became the same member.
     *
     * @throws InvalidArgumentException
     */
    public static function assertValidName(string $name): void
    {
        $parts = explode('.', $name);

        if (count($parts) > 2) {
            throw self::invalidName($name, 'only "member" or "struct.member" is supported');
        }

        foreach ($parts as $part) {
            if (1 !== preg_match('/^[A-Za-z_][A-Za-z0-9_]*$/', $part)) {
                throw self::invalidName(
                    $name,
                    sprintf(
                        '"%s" must be a C identifier (letters, digits and underscores, not starting with a digit)',
                        $part
                    )
                );
            }

            if (str_starts_with($part, self::OWNED_PREFIX)) {
                throw self::invalidName($name, sprintf('the "%s" prefix is reserved', self::OWNED_PREFIX));
            }
        }

        if (in_array($parts[0], self::KERNEL_MEMBERS, true)) {
            throw self::invalidName($name, sprintf('"%s" is a member the Zephir kernel declares', $parts[0]));
        }
    }

    /**
     * The C type a Zephir global type is declared as.
     *
     * @throws InvalidArgumentException
     */
    public static function cTypeOf(string $type, ?string $member = null): string
    {
        if (!isset(self::C_TYPES[$type])) {
            throw new InvalidArgumentException(
                null === $member
                    ? 'Unknown global type: ' . $type
                    : sprintf('Unknown type "%s" for extension global "%s"', $type, $member)
            );
        }

        return self::C_TYPES[$type];
    }

    /**
     * The C type of the struct member.
     *
     * @throws InvalidArgumentException
     */
    public function cType(): string
    {
        return self::cTypeOf($this->definition['type'], $this->member);
    }

    /**
     * Whether this global is exposed as a php.ini directive.
     */
    public function isIniCapable(): bool
    {
        return isset(self::INI_HANDLERS[$this->definition['type']]);
    }

    /**
     * Whether the global is initialized once per process instead of per request.
     */
    public function isModule(): bool
    {
        return !empty($this->definition['module']);
    }

    /**
     * The directive name, defaulting to `<extension>.<member path>`.
     */
    public function iniName(string $namespace): string
    {
        return $this->definition['ini-entry']['name'] ?? $namespace . '.' . $this->member;
    }

    /**
     * Where the directive may be changed from.
     */
    public function iniScope(): string
    {
        return $this->definition['ini-entry']['scope'] ?? 'PHP_INI_ALL';
    }

    /**
     * The `PHP_INI_BEGIN()` line for this global, or '' when it has none.
     *
     * The macros expand with their own trailing comma, so the caller joins
     * these with a newline and nothing else.
     *
     * @throws InvalidArgumentException
     */
    public function iniEntry(string $namespace): string
    {
        if (!$this->isIniCapable()) {
            return '';
        }

        $macro = match ($this->definition['type']) {
            Types::T_BOOL, Types::T_BOOLEAN => 'STD_PHP_INI_BOOLEAN',
            default                         => 'STD_PHP_INI_ENTRY',
        };

        return sprintf(
            '%s("%s", "%s", %s, %s, %s, zend_%s_globals, %s_globals)',
            $macro,
            $this->iniName($namespace),
            $this->iniDefault(),
            $this->iniScope(),
            self::INI_HANDLERS[$this->definition['type']],
            $this->member,
            $namespace,
            $namespace,
        );
    }

    /**
     * The compiled-in assignment of the default, or '' when there is none.
     *
     * An INI-backed global deliberately has none. REGISTER_INI_ENTRIES() runs
     * every on_modify handler at MINIT with the php.ini value (or the entry's
     * own default when php.ini is silent), so the member is already seeded
     * before any user code runs. Assigning a literal on top of that is exactly
     * what discarded the php.ini value on every request (#2449), and for a
     * string it also built a fresh zend_string per request for a value the
     * ini entry already owns.
     *
     * @throws InvalidArgumentException
     */
    public function cDefault(string $namespace): string
    {
        if ($this->isIniCapable()) {
            return '';
        }

        /* Validates the type, and names the global when it is unknown. */
        $this->cType();

        /* The caller indents; see %INIT_GLOBALS% in templates/engine/project.c. */
        return sprintf(
            '%s_globals->%s = %s;',
            $namespace,
            $this->member,
            $this->definition['default'],
        );
    }

    /**
     * Path of the member that owns what globals_set() wrote, or null when the
     * global needs none.
     *
     * Only a `string` global needs one. Its `char *` member is pointed into the
     * ini entry by OnUpdateString, and that value belongs to the engine, so
     * the member alone cannot tell whether it may be freed. The copy lives
     * next to the member, in the same struct (#2725).
     */
    public function ownedMember(): ?string
    {
        if (Types::T_STRING !== $this->definition['type']) {
            return null;
        }

        $separator = strrpos($this->member, '.');
        if (false === $separator) {
            return self::OWNED_PREFIX . $this->member;
        }

        return substr($this->member, 0, $separator + 1) . self::OWNED_PREFIX . substr($this->member, $separator + 1);
    }

    /**
     * The GINIT assignment that starts the owned copy empty, or '' when there
     * is none. Thread globals are malloc()ed, not zeroed, under ZTS.
     */
    public function ownedInit(string $namespace): string
    {
        $owned = $this->ownedMember();
        if (null === $owned) {
            return '';
        }

        return sprintf('%s_globals->%s = NULL;', $namespace, $owned);
    }

    /**
     * The GSHUTDOWN call that frees the owned copy, or '' when there is none.
     */
    public function ownedRelease(string $namespace): string
    {
        $owned = $this->ownedMember();
        if (null === $owned) {
            return '';
        }

        return sprintf('zephir_globals_release_string(&%s_globals->%s);', $namespace, $owned);
    }

    private static function invalidName(string $name, string $reason): InvalidArgumentException
    {
        return new InvalidArgumentException(sprintf('Extension global "%s" is not a valid name: %s', $name, $reason));
    }

    /**
     * The default rendered as the C string literal the ini entry carries.
     *
     * It must be a literal: STD_PHP_INI_* takes `sizeof(default) - 1` as the
     * value length. It must also never be NULL, because the numeric handlers
     * dereference `new_value` without a guard.
     */
    private function iniDefault(): string
    {
        $default = $this->definition['default'];

        return match ($this->definition['type']) {
            Types::T_BOOL, Types::T_BOOLEAN => true === $default ? '1' : '0',
            // var_export() round-trips a float exactly and independently of
            // the `precision` ini setting, so the generated C stays the same
            // whichever PHP ran the compiler.
            Types::T_DOUBLE                 => is_float($default) ? var_export($default, true) : (string) $default,
            default                         => addcslashes((string) $default, "\"\\"),
        };
    }
}
