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
 * C `long` is 64-bit under LP64 (Linux, macOS) and 32-bit under LLP64
 * (Windows x64), while `zend_long` is `int64_t` on both. A kernel prototype
 * that spells `long` therefore truncates every PHP integer on Windows, and
 * nothing on a Linux or macOS CI leg can observe it.
 *
 * This test is that observation: it reads the headers rather than running
 * them, so the portability contract is enforced from any platform.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2666
 */
final class KernelPortabilityTest extends TestCase
{
    /**
     * `long long` and `unsigned long long` are exactly 64-bit everywhere, so
     * they carry no data-model dependency and are allowed.
     */
    private const ALLOWED = ['long long', 'unsigned long long'];

    public function kernelHeaderProvider(): array
    {
        $headers = glob($this->kernelDir() . '/*.h');
        self::assertNotEmpty($headers);

        $cases = [];
        foreach ($headers as $header) {
            $cases[basename($header)] = [$header];
        }

        return $cases;
    }

    /**
     * @dataProvider kernelHeaderProvider
     */
    public function testKernelHeadersUseNoPlatformVariantIntegerType(string $header): void
    {
        $this->assertSame(
            [],
            $this->platformVariantTypes((string) file_get_contents($header)),
            sprintf(
                '%s uses a C `long`, whose width follows the data model. '
                . 'Use `zend_long`/`zend_ulong` for a PHP integer.',
                basename($header),
            ),
        );
    }

    /**
     * Every `long` token that is neither part of `zend_long`/`zend_ulong`
     * (blocked by the word boundary) nor one of the fixed-width spellings.
     *
     * @return list<string>
     */
    private function platformVariantTypes(string $source): array
    {
        $source = $this->stripComments($source);

        preg_match_all(
            '/\b(?:unsigned\s+)?long(?:\s+long)?\b/',
            $source,
            $matches,
            PREG_OFFSET_CAPTURE,
        );

        $found = [];
        foreach ($matches[0] as [$match, $offset]) {
            $normalised = preg_replace('/\s+/', ' ', $match);
            if (in_array($normalised, self::ALLOWED, true)) {
                continue;
            }

            $line = substr_count(substr($source, 0, $offset), "\n") + 1;
            $found[] = $line . ': ' . $normalised;
        }

        return $found;
    }

    /**
     * Block and line comments only; the headers contain no string literal
     * holding the word `long`.
     */
    private function stripComments(string $source): string
    {
        // Keep the newlines so reported line numbers stay accurate.
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
