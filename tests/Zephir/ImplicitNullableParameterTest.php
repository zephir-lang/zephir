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

namespace Zephir\Test;

use FilesystemIterator;
use PhpToken;
use PHPUnit\Framework\TestCase;
use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;
use SplFileInfo;

/**
 * A parameter with a non-nullable type and a `null` default is deprecated as of
 * PHP 8.4, and the deprecation is emitted by the *compiler*, so it fires when
 * the class file is compiled rather than when the function runs.
 *
 * For a lazily autoloaded exception that moment is the first `throw new ...`,
 * which is how `IllegalOperationException` and `InvalidTypeException` came to
 * replace every compile error they were supposed to carry: the CLI's error
 * handler turned the deprecation into an `ErrorException` thrown out of the
 * `new` expression.
 *
 * This test reads the sources rather than running them, for two reasons. A
 * class is compiled once per process, so an in-process check passes as soon as
 * anything else has already autoloaded the class. And the deprecation does not
 * exist before PHP 8.4, so a check that waits for PHP to emit it is vacuous on
 * four of the six supported versions, while reading the declaration holds
 * identically on all of them.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2663
 */
final class ImplicitNullableParameterTest extends TestCase
{
    /**
     * A promoted constructor parameter needs no type of its own, so a bare
     * modifier sits exactly where a type would. PHP rejects a *typed* promoted
     * parameter with a `null` default outright ("Cannot use null as default
     * value for parameter $x of type array"), so a modifier in that position
     * always means the parameter is untyped and carries no deprecation.
     */
    private const MODIFIERS = ['public', 'protected', 'private', 'readonly'];

    /**
     * The project's own PHP, plus the CLI entry script.
     *
     * `ext/` is left out: `run-tests.php` and `build/gen_stub.php` come from
     * php-src and are replaced wholesale on every update. `ide/` is generated
     * and `vendor/` is not ours.
     */
    private const TARGETS = ['src', 'tests', 'prototypes', 'templates', 'config', 'zephir'];

    public function phpSourceProvider(): array
    {
        $cases = [];
        foreach (self::TARGETS as $target) {
            $cases[$target] = [$target];
        }

        return $cases;
    }

    /**
     * @dataProvider phpSourceProvider
     */
    public function testDeclaresNoImplicitlyNullableParameter(string $target): void
    {
        $files = $this->phpFiles($this->projectRoot() . '/' . $target);
        self::assertNotEmpty($files, $target . ' matched no PHP file to scan.');

        $found = [];
        foreach ($files as $file) {
            $relative = str_replace($this->projectRoot() . '/', '', $file);
            foreach ($this->implicitlyNullableParameters((string) file_get_contents($file)) as $entry) {
                $found[] = $relative . ':' . $entry;
            }
        }

        $this->assertSame(
            [],
            $found,
            'A parameter with a non-nullable type and a `null` default is deprecated on PHP 8.4+, '
            . 'and the deprecation replaces whatever error the enclosing code was reporting. '
            . 'Write the type as `?Type`.',
        );
    }

    public function knownBadSignatureProvider(): array
    {
        return [
            'array'          => ['array $x = null'],
            'class name'     => ['Variable $x = null'],
            'qualified name' => ['\Zephir\Variable\Variable $x = null'],
            'scalar'         => ['string $x = null'],
            'by reference'   => ['array &$x = null'],
            'uppercase null' => ['array $x = NULL'],
            'second of two'  => ['string $a, array $x = null'],
        ];
    }

    /**
     * The positive control. Every assertion in the scan above is that nothing
     * was found, so a scanner that silently matched nothing would satisfy all
     * of them.
     *
     * @dataProvider knownBadSignatureProvider
     */
    public function testScannerFlagsAKnownBadSignature(string $declaration): void
    {
        $source = "<?php\n\nclass Probe\n{\n    public function m(" . $declaration . ") {}\n}\n";

        $this->assertCount(
            1,
            $this->implicitlyNullableParameters($source),
            $declaration . ' is implicitly nullable and must be reported.',
        );
    }

    public function acceptableSourceProvider(): array
    {
        return [
            'explicit nullable'    => ['function m(?array $x = null) {}'],
            'spaced nullable'      => ['function m(? array $x = null) {}'],
            'union with null'      => ['function m(array|null $x = null) {}'],
            'null first in union'  => ['function m(null|array $x = null) {}'],
            'mixed'                => ['function m(mixed $x = null) {}'],
            'qualified nullable'   => ['function m(?\Zephir\Variable\Variable $x = null) {}'],
            'nullable by ref'      => ['function m(?array &$x = null) {}'],
            'untyped'              => ['function m($x = null) {}'],
            'untyped promoted'     => ['class C { public function __construct(private $x = null) {} }'],
            'arrow function'       => ['$f = fn (?array $x = null) => $x;'],
            'assignment statement' => ['function m() { return $x = null; }'],
            'typed property'       => ['class C { private ?array $x = null; }'],
            'string default'       => ["function m(\$s = 'array \$x = null') {}"],
            'comment in list'      => ['function m(/* array $x = null */ $y = null) {}'],
            'call in body'         => ['function m() { other("array $x = null"); }'],
        ];
    }

    /**
     * The negative control: the forms that are already correct, plus the
     * contexts that merely look like a parameter. Without these, tightening the
     * scanner into silence would go unnoticed.
     *
     * @dataProvider acceptableSourceProvider
     */
    public function testScannerAcceptsExplicitlyNullableAndUnrelatedForms(string $source): void
    {
        $this->assertSame([], $this->implicitlyNullableParameters("<?php\n" . $source));
    }

    /**
     * A blanked stretch keeps its newlines, so a reported line number is the
     * line the declaration is really on.
     */
    public function testReportsTheDeclarationLine(): void
    {
        $source = "<?php\n\n\n\nfunction m(array \$x = null) {}\n";

        $this->assertSame(['5: array $x = null'], $this->implicitlyNullableParameters($source));
    }

    /**
     * @return list<string> `<line>: <declaration>` for each offender
     */
    private function implicitlyNullableParameters(string $source): array
    {
        $subject = $this->parameterListsOnly($source);
        $name    = '\\\\?[A-Za-z_]\w*(?:\\\\[A-Za-z_]\w*)*';

        preg_match_all(
            '/(?<nullable>\?\s*)?(?<type>' . $name . '(?:\s*[|&]\s*\??' . $name . ')*)'
            . '\s+&?\s*\$(?<param>[A-Za-z_]\w*)\s*=\s*\\\\?null\b/i',
            $subject,
            $matches,
            PREG_OFFSET_CAPTURE | PREG_SET_ORDER,
        );

        $found = [];
        foreach ($matches as $match) {
            if (($match['nullable'][0] ?? '') !== '') {
                continue;
            }

            if (!$this->isNonNullableType((string) preg_replace('/\s+/', '', $match['type'][0]))) {
                continue;
            }

            $line = substr_count(substr($subject, 0, $match[0][1]), "\n") + 1;

            $found[] = $line . ': ' . preg_replace('/\s+/', ' ', trim($match[0][0]));
        }

        return $found;
    }

    /**
     * Whether a type declaration forbids `null`, which is what makes a `null`
     * default an implicit widening.
     */
    private function isNonNullableType(string $type): bool
    {
        $parts = array_map('strtolower', explode('|', $type));

        if (in_array('mixed', $parts, true) || in_array('null', $parts, true)) {
            return false;
        }

        foreach ($parts as $part) {
            // A `?` on any union member, and a modifier standing in for a type.
            if (str_starts_with($part, '?') || in_array(ltrim($part, '\\'), self::MODIFIERS, true)) {
                return false;
            }
        }

        return true;
    }

    /**
     * Blank every byte that is not inside a parameter list, keeping each
     * blanked stretch's newlines so reported line numbers stay accurate.
     *
     * Restricting the search this way is what keeps `private $property = null`
     * and `return $x = null` out of the results without having to parse a
     * parameter list. Comments and string bodies are blanked as well, because
     * `src/Stubs/Generator.php` and `tests/Zephir/Stubs/GeneratorTest.php` both
     * carry `string $name = null` as literal string data.
     */
    private function parameterListsOnly(string $source): string
    {
        $out     = '';
        $pending = false;
        $depth   = 0;

        foreach (PhpToken::tokenize($source) as $token) {
            $text = $token->text;

            if ($depth === 0) {
                $out .= str_repeat("\n", substr_count($text, "\n"));

                if ($token->is([T_FUNCTION, T_FN])) {
                    $pending = true;
                } elseif ($pending && $text === '(') {
                    $pending = false;
                    $depth   = 1;
                }

                continue;
            }

            if ($token->is([T_COMMENT, T_DOC_COMMENT, T_CONSTANT_ENCAPSED_STRING, T_ENCAPSED_AND_WHITESPACE])) {
                $out .= str_repeat("\n", substr_count($text, "\n"));

                continue;
            }

            $out .= $text;

            if ($text === '(') {
                ++$depth;
            } elseif ($text === ')') {
                --$depth;
            }
        }

        return $out;
    }

    /**
     * @return list<string>
     */
    private function phpFiles(string $path): array
    {
        if (!is_dir($path)) {
            return [$path];
        }

        $files    = [];
        $iterator = new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($path, FilesystemIterator::SKIP_DOTS),
        );

        foreach ($iterator as $entry) {
            /** @var SplFileInfo $entry */
            if ($entry->getExtension() === 'php') {
                $files[] = $entry->getPathname();
            }
        }

        sort($files);

        return $files;
    }

    private function projectRoot(): string
    {
        return dirname(__DIR__, 2);
    }
}
