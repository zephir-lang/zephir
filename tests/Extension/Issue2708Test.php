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

namespace Extension;

use ArrayAccess;
use PHPUnit\Framework\TestCase;
use ReturnTypeWillChange;
use Stub\Issue2708;

/**
 * A literal string array offset, against what PHP does with the same subscript.
 *
 * PHP folds a constant numeric string subscript to an integer key at compile
 * time, so `$a["3"]` is `$a[3]`. Zephir's literal-offset kernel helpers used
 * `zend_hash_str_*`, which does not normalise, while every zval-indexed
 * counterpart used `zend_symtable_*`, which does. The disagreement was visible
 * without leaving Zephir: a constant array literal is emitted as
 * `add_assoc_*_ex`, and php-src implements those with
 * `zend_symtable_str_update`, so the literal stored the integer key that the
 * literal offset then could not find.
 *
 * Every expectation below is PHP executing the same statement, so one test body
 * is correct on 8.0 through 8.5 with no version branching.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2708
 */
final class Issue2708Test extends TestCase
{
    private Issue2708 $subject;

    protected function setUp(): void
    {
        $this->subject = new Issue2708();
    }

    /**
     * The array literal and the literal offset must agree about the key.
     */
    public function testIssetFindsTheKeyTheLiteralStored(): void
    {
        $php = ['3' => 1];

        $this->assertSame(isset($php['3']), $this->subject->issetOnConstantLiteral());
    }

    public function testEmptyFindsTheKeyTheLiteralStored(): void
    {
        $php = ['3' => 1];

        $this->assertSame(empty($php['3']), $this->subject->emptyOnConstantLiteral());
    }

    public function testFetchFindsTheKeyTheLiteralStored(): void
    {
        $php = ['3' => 'found'];

        $this->assertSame($php['3'] ?? null, $this->subject->fetchOnConstantLiteral());
    }

    public function testReadFindsTheKeyTheLiteralStored(): void
    {
        $php = ['3' => 'found'];

        $this->assertSame($php['3'], $this->subject->readOnConstantLiteral());
    }

    /**
     * The line the issue names: the offset PHP removes is the integer key.
     */
    public function testUnsetRemovesTheIntegerKey(): void
    {
        $php = [3 => 'x'];
        unset($php['3']);

        $this->assertSame($php, $this->subject->unsetLiteralOffset([3 => 'x']));
    }

    /**
     * A write under the string spelling has to be readable under the integer
     * one, which is the shape a Zephir program mixing the two would hit.
     */
    public function testWriteUnderStringSpellingReadsBackUnderTheInteger(): void
    {
        $php = [];
        $php['3'] = 'written';

        $this->assertSame($php[3], $this->subject->writeThenReadBack());
    }

    public function testMultiDimensionalWriteUsesTheSameKey(): void
    {
        $php = [];
        $php['3']['4'] = 'deep';

        $this->assertSame($php, $this->subject->writeMultiDimensional());
    }

    /**
     * The key type is observable beyond the key itself. An integer key moves
     * `nNextFreeElement`, so a later append lands past it instead of at zero.
     */
    public function testAppendFollowsTheIntegerKey(): void
    {
        $php = [];
        $php['3'] = 'x';
        $php[] = 'y';

        $this->assertSame($php, $this->subject->appendAfterNumericStringKey());
    }

    /**
     * `array_merge()` renumbers integer keys and preserves string ones, so the
     * merged result reports which one was stored.
     */
    public function testMergeRenumbersTheIntegerKey(): void
    {
        $php = [];
        $php['3'] = 'x';

        $this->assertSame(array_merge($php, ['y']), $this->subject->mergeAfterNumericStringKey());
    }

    /**
     * Which spellings are integer keys is `_zend_handle_numeric_str_ex()`'s
     * rule, not ours: a leading `-` or digit, no leading zero beyond a bare
     * `"0"`, digits only, and no overflow past `ZEND_LONG_MAX`. Asserting
     * against PHP performing the same write inherits the whole rule instead of
     * restating any part of it.
     *
     * @dataProvider literalKeyProvider
     */
    public function testALiteralKeyBecomesWhatPhpMakesOfIt(string $method, string $key): void
    {
        $php = [];
        $php[$key] = 1;

        $this->assertSame($php, $this->subject->$method());
    }

    public static function literalKeyProvider(): array
    {
        return [
            'canonical integer'     => ['writeThree', '3'],
            'zero'                  => ['writeZero', '0'],
            'negative integer'      => ['writeNegativeThree', '-3'],
            'ZEND_LONG_MAX'         => ['writeIntMax', '9223372036854775807'],
            'leading zero'          => ['writeLeadingZero', '03'],
            'leading plus'          => ['writeLeadingPlus', '+3'],
            'negative zero'         => ['writeNegativeZero', '-0'],
            'leading space'         => ['writeLeadingSpace', ' 3'],
            'trailing space'        => ['writeTrailingSpace', '3 '],
            'decimal point'         => ['writeDecimalPoint', '3.0'],
            'past ZEND_LONG_MAX'    => ['writeOverflow', '9223372036854775808'],
            'ordinary string'       => ['writeOrdinaryKey', 'key_a'],
        ];
    }

    /**
     * PHP keeps the original literal beside the folded integer so that an
     * ArrayAccess object is handed the string it was written with; see php bug
     * #63217. Normalising the array path must not reach this one.
     *
     * @dataProvider containerOperationProvider
     */
    public function testAnArrayAccessContainerReceivesTheOriginalString(string $method, string $operation): void
    {
        $spy = new class implements ArrayAccess {
            public array $seen = [];

            public function offsetExists($offset): bool
            {
                $this->seen[] = ['exists', $offset];

                return true;
            }

            #[ReturnTypeWillChange]
            public function offsetGet($offset)
            {
                $this->seen[] = ['get', $offset];

                return 'v';
            }

            public function offsetSet($offset, $value): void
            {
                $this->seen[] = ['set', $offset];
            }

            public function offsetUnset($offset): void
            {
                $this->seen[] = ['unset', $offset];
            }
        };

        $this->subject->$method($spy);

        $this->assertContains(
            [$operation, '3'],
            $spy->seen,
            'An ArrayAccess container must receive the offset as the string it was written with.'
        );
    }

    public static function containerOperationProvider(): array
    {
        return [
            'read'  => ['readOnContainer', 'get'],
            'isset' => ['issetOnContainer', 'exists'],
            'write' => ['writeOnContainer', 'set'],
            'unset' => ['unsetOnContainer', 'unset'],
        ];
    }
}
