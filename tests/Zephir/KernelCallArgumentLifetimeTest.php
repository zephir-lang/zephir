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

use PHPUnit\Framework\TestCase;

/**
 * A zval the kernel built for a userland call has to outlive every call that
 * receives it. Releasing it in between is not a crash, so nothing observes it
 * by accident: the freed slot is handed straight to the next allocation, which
 * is the method-name string of the very next call, and the callee silently
 * receives that string as its argument.
 *
 * This test is that observation, read out of the C rather than run: for every
 * `zval_ptr_dtor(&x)` it checks that `&x` is not passed to a later call in the
 * same function without being re-initialised first.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2656
 */
final class KernelCallArgumentLifetimeTest extends TestCase
{
    /**
     * Macros and functions that hand an argument to userland or to the engine.
     */
    private const CALL = '(?:ZEPHIR_(?:RETURN_)?CALL_[A-Z_]+|zephir_call_[a-z_]+)';

    /**
     * Anything that gives a released local a fresh value, after which passing
     * it on is correct again.
     */
    private const REINIT = '(?:ZVAL_[A-Z_]+|array_init(?:_size)?|object_init_ex|zephir_create_array)';

    public function kernelSourceProvider(): array
    {
        $sources = glob($this->kernelDir() . '/*.c');
        self::assertNotEmpty($sources);

        $cases = [];
        foreach ($sources as $source) {
            $cases[basename($source)] = [$source];
        }

        return $cases;
    }

    /**
     * @dataProvider kernelSourceProvider
     */
    public function testNoReleasedLocalIsPassedToALaterCall(string $source): void
    {
        $this->assertSame(
            [],
            $this->releasedThenPassed((string) file_get_contents($source)),
            sprintf(
                '%s releases a local and then hands it to a call. The callee '
                . 'reads freed memory, and the next allocation, which is the '
                . "call's own method-name string, overwrites it first.",
                basename($source),
            ),
        );
    }

    /**
     * Guards the guard: the shape it is meant to catch must actually trip it.
     */
    public function testTheGuardCatchesTheReportedShape(): void
    {
        $shape = <<<'C'
            int probe(zval *fetched, const zval *arr, char *index, uint32_t index_length)
            {
            	zval exist, offset;
            	ZVAL_UNDEF(&exist);
            	ZVAL_STRINGL(&offset, index, index_length);

            	ZEPHIR_CALL_METHOD_WITHOUT_OBSERVE(&exist, (zval *)arr, "offsetexists", NULL, 0, &offset);
            	zval_ptr_dtor(&offset);
            	if (zend_is_true(&exist)) {
            		ZEPHIR_CALL_METHOD_WITHOUT_OBSERVE(fetched, (zval *)arr, "offsetget", NULL, 0, &offset);
            		return 1;
            	}

            	return 0;
            }
            C;

        // The body delimiters are column-zero braces, as in every kernel file.
        $shape = (string) preg_replace('/^ {12}/m', '', $shape);

        $this->assertSame(['8: offset'], $this->releasedThenPassed($shape));
    }

    /**
     * Re-initialising a released local makes passing it on correct again, so
     * that shape must not be reported.
     */
    public function testTheGuardAllowsAReinitialisedLocal(): void
    {
        $shape = <<<'C'
            int probe(zval *arr, char *index, uint32_t index_length)
            {
            	zval offset;
            	ZVAL_STRINGL(&offset, index, index_length);
            	ZEPHIR_CALL_METHOD_WITHOUT_OBSERVE(NULL, arr, "offsetunset", NULL, 0, &offset);
            	zval_ptr_dtor(&offset);

            	ZVAL_LONG(&offset, 0);
            	ZEPHIR_CALL_METHOD_WITHOUT_OBSERVE(NULL, arr, "offsetunset", NULL, 0, &offset);
            	zval_ptr_dtor(&offset);

            	return 1;
            }
            C;

        $shape = (string) preg_replace('/^ {12}/m', '', $shape);

        $this->assertSame([], $this->releasedThenPassed($shape));
    }

    /**
     * @return list<string> "<line>: <local>" per finding
     */
    private function releasedThenPassed(string $source): array
    {
        $source = $this->stripComments($source);

        $found = [];
        foreach ($this->functionBodies($source) as [$firstLine, $body]) {
            preg_match_all('/\bzval_ptr_dtor\(\s*&(\w+)\s*\)/', $body, $releases, PREG_OFFSET_CAPTURE);

            foreach ($releases[1] as $index => [$local, $_]) {
                [$match, $offset] = $releases[0][$index];
                $tail             = substr($body, $offset + strlen($match));

                // Everything after a fresh value is a different lifetime.
                if (preg_match('/\b' . self::REINIT . '\s*\(\s*&' . preg_quote($local, '/') . '\b/', $tail, $m, PREG_OFFSET_CAPTURE)) {
                    $tail = substr($tail, 0, $m[0][1]);
                }

                if (!preg_match('/\b' . self::CALL . '\s*\([^;]*&' . preg_quote($local, '/') . '\b/', $tail)) {
                    continue;
                }

                $line    = $firstLine + substr_count(substr($body, 0, $offset), "\n");
                $found[] = $line . ': ' . $local;
            }
        }

        return $found;
    }

    /**
     * Function bodies, delimited by a brace alone on a line at column zero.
     * Every kernel definition is written that way and nothing nested is, so
     * this needs no C parser.
     *
     * @return list<array{int, string}> first line number (1-based) and body
     */
    private function functionBodies(string $source): array
    {
        $lines  = explode("\n", $source);
        $count  = count($lines);
        $bodies = [];

        for ($i = 0; $i < $count; ++$i) {
            if ('{' !== $lines[$i]) {
                continue;
            }

            $end = $i + 1;
            while ($end < $count && '}' !== $lines[$end]) {
                ++$end;
            }

            $bodies[] = [$i + 1, implode("\n", array_slice($lines, $i, $end - $i + 1))];
            $i        = $end;
        }

        return $bodies;
    }

    /**
     * Block and line comments only, with the newlines kept so reported line
     * numbers stay accurate.
     */
    private function stripComments(string $source): string
    {
        return (string) preg_replace_callback(
            '#/\*.*?\*/|//[^\n]*#s',
            static fn (array $m): string => str_repeat("\n", substr_count($m[0], "\n")),
            $source,
        );
    }

    private function kernelDir(): string
    {
        return dirname(__DIR__, 2) . '/kernel';
    }
}
