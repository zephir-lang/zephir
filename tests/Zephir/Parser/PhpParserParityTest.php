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

namespace Zephir\Parser;

use PHPUnit\Framework\TestCase;
use Zephir\Parser\Php\PhpParser;

use function array_flip;
use function basename;
use function explode;
use function file_get_contents;
use function glob;
use function json_encode;
use function trim;

use const JSON_PRETTY_PRINT;

/**
 * Differential (golden) parity test for the pure-PHP parser.
 *
 * For every fixture under tests/Parser/fixtures, the golden `.json` is the
 * exact `json_encode(zephir_parse_file(...), JSON_PRETTY_PRINT)` dump from the
 * reference C extension (regenerate with tests/Parser/generate-golden.php).
 *
 * The pure-PHP parser must reproduce each byte-for-byte. Fixtures listed in
 * tests/Parser/slice-allowlist.txt are the current implemented vertical slice
 * and are asserted as hard pass/fail; the remainder exercise grammar not yet
 * ported and are reported as incomplete (never silently skipped).
 */
final class PhpParserParityTest extends TestCase
{
    private const FIXTURES  = __DIR__ . '/../../Parser/fixtures';
    private const ALLOWLIST = __DIR__ . '/../../Parser/slice-allowlist.txt';

    /**
     * @dataProvider fixtureProvider
     */
    public function testByteIdenticalIr(string $slug, bool $inSlice): void
    {
        $source   = file_get_contents(self::FIXTURES . '/' . $slug . '.zep');
        $expected = file_get_contents(self::FIXTURES . '/' . $slug . '.json');

        $actual = json_encode((new PhpParser())->parse($source, $slug . '.zep'), JSON_PRETTY_PRINT);

        if (!$inSlice) {
            if ($actual === $expected) {
                // Already matches although not yet promoted into the allow-list.
                $this->addToAssertionCount(1);

                return;
            }
            $this->markTestIncomplete("Fixture '{$slug}' exercises grammar outside the current parser slice.");
        }

        $this->assertSame(
            $expected,
            $actual,
            "Pure-PHP parser IR diverges from the C extension golden for '{$slug}'."
        );
    }

    /**
     * @return iterable<string, array{0: string, 1: bool}>
     */
    public static function fixtureProvider(): iterable
    {
        $allow = array_flip(self::loadAllowlist());

        foreach (glob(self::FIXTURES . '/*.zep') as $path) {
            $slug = basename($path, '.zep');
            yield $slug => [$slug, isset($allow[$slug])];
        }
    }

    /**
     * @return list<string>
     */
    private static function loadAllowlist(): array
    {
        $raw   = (string) file_get_contents(self::ALLOWLIST);
        $slugs = [];
        foreach (explode("\n", $raw) as $line) {
            $line = trim($line);
            if ($line !== '') {
                $slugs[] = $line;
            }
        }

        return $slugs;
    }
}
