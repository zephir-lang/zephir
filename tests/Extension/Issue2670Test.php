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

use Issue2670Materializing;
use PHPUnit\Framework\TestCase;
use Stub\Issue2670;

use function memory_get_usage;

/**
 * Kernel paths whose engine API moved in PHP 8.6, each compared against
 * plain PHP on the PHP version the suite runs on.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2670
 */
final class Issue2670Test extends TestCase
{
    use AssertsPhpParity;

    private Issue2670 $test;

    protected function setUp(): void
    {
        $this->test = new Issue2670();
    }

    /**
     * unique_key() of a number converts it to a string, which has to be
     * released after it is appended.
     */
    public function testUniqueKeyOfANumberDoesNotLeak(): void
    {
        $this->test->uniqueKeyLoop('k', 123456789, 10);

        $before = memory_get_usage();
        $this->test->uniqueKeyLoop('k', 123456789, 10000);

        $this->assertSame(0, memory_get_usage() - $before);
    }

    public function testUniqueKeyOfANumberMatchesItsString(): void
    {
        $this->assertSame('k123456789', $this->test->uniqueKeyLoop('k', 123456789, 1));
    }

    /**
     * @dataProvider materializedProvider
     */
    public function testEmptyOfAPropertyIssetMaterializesMatchesPhp(mixed $materialized): void
    {
        $this->assertMatchesPhp(
            fn () => $this->magicTrace(fn (Issue2670Materializing $o) => $this->test->emptyValue($o), $materialized),
            fn () => $this->magicTrace(fn (Issue2670Materializing $o) => empty($o->value), $materialized)
        );
    }

    /**
     * @dataProvider materializedProvider
     */
    public function testIssetOfAPropertyIssetMaterializesMatchesPhp(mixed $materialized): void
    {
        $this->assertMatchesPhp(
            fn () => $this->magicTrace(fn (Issue2670Materializing $o) => $this->test->issetValue($o), $materialized),
            fn () => $this->magicTrace(fn (Issue2670Materializing $o) => isset($o->value), $materialized)
        );
    }

    public function materializedProvider(): array
    {
        return [
            'truthy' => ['set'],
            'falsy'  => [''],
            'null'   => [null],
        ];
    }

    /**
     * The result and the magic methods it took, in order.
     */
    private function magicTrace(callable $probe, mixed $materialized): array
    {
        $object = new Issue2670Materializing($materialized);
        $result = $probe($object);

        return [$result, $object->calls];
    }
}
