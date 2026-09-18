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
use PHPUnit\Framework\TestCase;
use RecursiveDirectoryIterator;
use RecursiveIteratorIterator;

/**
 * A generated function that opens a Zephir memory frame has to close it, or the
 * frame, the method globals and every zval registered in the frame are retained
 * for the life of the process - once per call, silently.
 *
 * So read it back out of the committed extension. This walks the C the compiler
 * actually emitted for `stub/` and asks one coarse question of each function:
 * if it opens a frame, does it close it anywhere at all? That is deliberately
 * weaker than "on every path", which would mean re-implementing the compiler's
 * flow analysis in a test; it is still enough to catch a whole body shape whose
 * close was never emitted, which is how #2716 reached a release.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2716
 */
final class MemoryFrameBalanceTest extends TestCase
{
    private const GROW = 'zephir_memory_grow_stack(';

    /**
     * Kernel constructs that restore the frame unconditionally, by prefix.
     *
     * Deliberately not read out of `ext/kernel/*.h`: `zephir_check_call_status`
     * and friends restore too, but only on their failure branch, so deriving
     * the list would accept a body that leaks on its normal path. Adding a new
     * unconditional macro to the kernel means adding it here; the symptom is a
     * failure naming a function that is in fact fine.
     */
    private const RESTORING = [
        'ZEPHIR_MM_RESTORE',
        'RETURN_MM',                // RETURN_MM(), RETURN_MM_NULL(), RETURN_MM_BOOL(), ...
        'RETURN_CTOR',
        'RETURN_CCTOR',
        'RETURN_NCTOR',
        'RETURN_LCTOR',
        'RETURN_THIS',              // RETURN_THIS() and RETURN_THIS_ZVAL()
        'ZEPHIR_THROW_EXCEPTION_',  // _STR, _ZVAL, _DEBUG_STR, _DEBUG_ZVAL
    ];

    public function testEveryGeneratedFunctionThatOpensAMemoryFrameClosesIt(): void
    {
        $offenders = [];

        foreach ($this->generatedSources() as $path => $source) {
            foreach ($this->functionBodies($source) as $signature => $body) {
                if (!str_contains($body, self::GROW)) {
                    continue;
                }

                foreach (self::RESTORING as $construct) {
                    if (str_contains($body, $construct)) {
                        continue 2;
                    }
                }

                $offenders[] = $path . ': ' . $signature;
            }
        }

        $this->assertSame(
            [],
            $offenders,
            'These generated functions open a memory frame and never close it, so every call '
            . 'retains the frame, the method globals and every zval registered in it.'
        );
    }

    /**
     * Each `PHP_METHOD(...)`/`PHP_FUNCTION(...)` definition in a generated file,
     * keyed by its signature. Definitions sit at file scope, so the body ends at
     * the first closing brace in column zero.
     *
     * @return array<string, string>
     */
    private function functionBodies(string $source): array
    {
        $pattern = '/^((?:PHP_METHOD|PHP_FUNCTION)\([^)]*\))\s*\n\{\n(.*?)^\}/ms';

        if (!preg_match_all($pattern, $source, $matches, PREG_SET_ORDER)) {
            return [];
        }

        $bodies = [];
        foreach ($matches as $match) {
            $bodies[$match[1]] = $match[2];
        }

        return $bodies;
    }

    /**
     * @return array<string, string>
     */
    private function generatedSources(): array
    {
        $extDir = dirname(__DIR__, 2) . '/ext';

        $this->assertDirectoryExists($extDir, 'The committed generated extension is missing.');

        $sources  = [];
        $iterator = new RecursiveIteratorIterator(
            new RecursiveDirectoryIterator($extDir, FilesystemIterator::SKIP_DOTS)
        );

        foreach ($iterator as $file) {
            if ($file->isFile() && str_ends_with($file->getFilename(), '.zep.c')) {
                $sources[$file->getPathname()] = (string) file_get_contents($file->getPathname());
            }
        }

        $this->assertNotEmpty($sources, 'No generated .zep.c files were found to check.');

        return $sources;
    }
}
