<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Extension\Optimizers;

use PHPUnit\Framework\TestCase;
use Stub\Optimizers\Substr;

/**
 * Exhaustive differential gate for zephir_substr().
 *
 * The hand-written cases in SubstrTest document intent; this one proves
 * totality by comparing every (subject, from, length) combination against
 * native substr(). It exists because the kernel spent ten years diverging
 * from PHP on ~24% of its input space without a single test noticing.
 *
 * Not a data-provider test on purpose: 20k PHPUnit cases would dominate the
 * suite's runtime and output. One assertion, full divergence list on failure.
 */
final class SubstrParityTest extends TestCase
{
    /**
     * Subjects PHP accepts and coerces. Arrays and bare objects are excluded:
     * PHP 8 raises TypeError for those, while Zephir routes them through
     * zend_make_printable_zval. That delta is pre-existing and out of scope.
     */
    private const SUBJECTS = [
        "''"         => '',
        "'a'"        => 'a',
        "'abcdef'"   => 'abcdef',
        "'GetPosts'" => 'GetPosts',
        'binary'     => "\x00ab",
        'long20'     => 'abcdefghijklmnopqrst',
        'null'       => null,
        'true'       => true,
        'false'      => false,
        'int0'       => 0,
        'int12345'   => 12345,
        'float1.5'   => 1.5,
    ];

    public function testMatchesNativeSubstrForEveryCombination(): void
    {
        $test       = new Substr();
        $divergent  = [];
        $combos     = 0;

        foreach (self::SUBJECTS as $name => $subject) {
            // This file declares strict_types, so native substr() would reject a
            // non-string subject outright. Cast first: (string) reproduces exactly
            // the weak-mode coercion PHP applies to a string parameter
            // (null -> "", false -> "", true -> "1", 1.5 -> "1.5"), which is the
            // behaviour the kernel is being held to.
            $native = (string) $subject;

            // Span is driven by the coerced length so the sweep always straddles
            // the len-1 / len / len+1 boundary where the off-by-one lived.
            $span  = strlen($native) + 4;
            $froms = array_merge(range(-$span, $span), [PHP_INT_MIN, PHP_INT_MAX]);
            $lens  = array_merge(range(-$span, $span), [PHP_INT_MIN, PHP_INT_MAX]);

            foreach ($froms as $from) {
                ++$combos;
                $this->collect(
                    $divergent,
                    $name,
                    $from,
                    null,
                    $test->testTwoArguments($subject, $from),
                    substr($native, $from)
                );

                foreach ($lens as $len) {
                    ++$combos;
                    $this->collect(
                        $divergent,
                        $name,
                        $from,
                        $len,
                        $test->testThreeArguments($subject, $from, $len),
                        substr($native, $from, $len)
                    );
                }
            }
        }

        $this->assertSame(
            [],
            $divergent,
            sprintf(
                "zephir_substr() diverges from native substr() in %d of %d combinations:\n%s",
                count($divergent),
                $combos,
                implode("\n", array_slice($divergent, 0, 40))
            )
        );
    }

    /**
     * @param list<string> $divergent
     */
    private function collect(array &$divergent, string $name, int $from, ?int $len, $actual, $expected): void
    {
        if ($actual === $expected) {
            return;
        }

        $divergent[] = sprintf(
            '  %s from=%s length=%s -> zephir=%s php=%s',
            $name,
            $from,
            $len ?? '-',
            var_export($actual, true),
            var_export($expected, true)
        );
    }
}
