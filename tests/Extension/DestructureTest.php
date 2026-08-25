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

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Destructure;

/**
 * Destructuring assignment: let [a, b, c] = expr;
 *
 * @see https://github.com/zephir-lang/zephir/issues/2496
 */
final class DestructureTest extends TestCase
{
    private Destructure $test;

    protected function setUp(): void
    {
        $this->test = new Destructure();
    }

    /**
     * @dataProvider destructureProvider
     */
    public function testShouldDestructure(array $expected, string $method): void
    {
        $this->assertSame($expected, $this->test->$method());
    }

    public function destructureProvider(): array
    {
        return [
            [[1, 2, 3],            'basic'],
            [[1, 3],               'skipped'],
            [[1, 2],               'literal'],
            [[10, 20, 1],          'fromCall'],
            [[2, 1],               'swap'],
            [[1, 2],               'fewerTargets'],
            [[[1, 2], [3, 4]],     'nestedValue'],
            [[5, 'x'],             'typedTargets'],
            [['z', 'y'],           'keyedSource'],
            [[3, 7],               'inLoop'],
            [[1, 2, 9],            'chained'],
            [[7, 8],               'fromProperty'],
        ];
    }

    public function testShouldSkipTrailingSlot(): void
    {
        $this->assertSame(1, $this->test->trailingSkip());
    }

    /**
     * A slot past the end of the source array yields null, matching PHP's
     * list(). The diagnostic is Zephir's own array-read notice ("Undefined
     * index"), not PHP's "Undefined array key" warning, so it is suppressed
     * here — PHPUnit 9.5 promotes notices to failures.
     */
    public function testShouldAssignNullToMissingSlot(): void
    {
        $this->assertSame([1, 2, null], @$this->test->missingSlot());
    }
}
