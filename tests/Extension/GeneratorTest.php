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

use Error;
use Exception;
use Iterator;
use PHPUnit\Framework\TestCase;
use ReflectionClass;
use RuntimeException;
use Stub\Generators;

/**
 * Generator methods (issue #1849): every observable behavior is asserted
 * against what the equivalent PHP generator produces.
 */
final class GeneratorTest extends TestCase
{
    public function testReturnsExtensionScopedGeneratorImplementingIterator(): void
    {
        $it = (new Generators())->countTo(1);

        $this->assertSame('Stub\Generator', get_class($it));
        $this->assertInstanceOf(Iterator::class, $it);
        $this->assertFalse($it instanceof \Generator, 'PHP\'s \Generator is final: extensions cannot produce it');
    }

    public function testLazyStart(): void
    {
        $test = new Generators();
        $it   = $test->countTo(2);

        $this->assertSame(0, $test->sideEffects, 'body must not run at creation');
        $this->assertTrue($it->valid());
        $this->assertSame(1, $test->sideEffects, 'first observation primes the body');
    }

    public function testValuesAndAutoKeys(): void
    {
        $out = [];
        foreach ((new Generators())->countTo(3) as $k => $v) {
            $out[$k] = $v;
        }

        $this->assertSame([1, 2, 3], $out);
    }

    public function testExplicitAndMixedKeysFollowArrayAppendRule(): void
    {
        $out = [];
        foreach ((new Generators())->keysMix() as $k => $v) {
            $out[] = [$k, $v];
        }

        $this->assertSame([['a', 1], [10, 2], [11, 3], ['z', 4], [12, 5]], $out);
    }

    public function testBareYield(): void
    {
        $out = [];
        foreach ((new Generators())->bare() as $k => $v) {
            $out[] = [$k, $v];
        }

        $this->assertSame([[0, null], [1, null]], $out);
    }

    public function testNestedArrayLoops(): void
    {
        $out = iterator_to_array((new Generators())->nested([[1, 2], [3], [4, 5]]), false);

        $this->assertSame([1, 2, 3, 4, 5], $out);
    }

    public function testIndependentInterleavedInstances(): void
    {
        $test = new Generators();
        $a    = $test->countTo(2);
        $b    = $test->countTo(2);

        $this->assertSame(1, $a->current());
        $this->assertSame(1, $b->current());
        $a->next();
        $this->assertSame(2, $a->current());
        $this->assertSame(1, $b->current());
    }

    public function testZephirForConsumesGenerator(): void
    {
        $test = new Generators();

        $this->assertSame([1, 2, 3], $test->consume($test->range4(3)));
    }

    public function testGeneratorRelayingAnotherGenerator(): void
    {
        $test = new Generators();
        $out  = [];
        foreach ($test->relay($test->range4(3)) as $k => $v) {
            $out[$k] = $v;
        }

        $this->assertSame([1, 2, 3], $out, 'relay re-keys with its own auto keys');
    }

    public function testGetReturn(): void
    {
        $g = (new Generators())->withReturn();

        try {
            $g->getReturn();
            $this->fail('getReturn before completion must throw');
        } catch (Exception $e) {
            $this->assertSame("Cannot get return value of a generator that hasn't returned", $e->getMessage());
        }

        foreach ($g as $v) {
        }

        $this->assertSame('done', $g->getReturn());
    }

    public function testRewindRules(): void
    {
        $g = (new Generators())->three();
        $g->rewind();
        $g->rewind();
        $this->assertSame(1, $g->current());

        $g->next();

        try {
            $g->rewind();
            $this->fail('rewind after advancing must throw');
        } catch (Exception $e) {
            $this->assertSame('Cannot rewind a generator that was already run', $e->getMessage());
        }
    }

    public function testTraversingClosedGeneratorThrows(): void
    {
        $g = (new Generators())->three();
        foreach ($g as $v) {
        }

        $this->expectException(Exception::class);
        $this->expectExceptionMessage('Cannot traverse an already closed generator');
        foreach ($g as $v) {
        }
    }

    public function testExhaustion(): void
    {
        $g = (new Generators())->countTo(1);
        foreach ($g as $v) {
        }

        $this->assertFalse($g->valid());
        $this->assertNull($g->current());
        $this->assertNull($g->key());
        $g->next();
        $this->assertFalse($g->valid());
    }

    public function testStaticGeneratorWithTypedLocalsAcrossSuspension(): void
    {
        $out = [];
        foreach (Generators::seq(3, 0.5, true, 'end') as $v) {
            $out[] = $v;
        }

        $this->assertSame([0.5, 1.0, 1.5, 'end'], $out);
    }

    public function testSendResumesAndReturnsCurrent(): void
    {
        $g = (new Generators())->three();

        $this->assertSame(2, $g->send('ignored'), 'send on a fresh generator primes then resumes (PHP behavior)');
        $this->assertSame(3, $g->send('ignored'));
        $this->assertSame(3, $g->current());
    }

    public function testExceptionFromBodyClosesGenerator(): void
    {
        $g = (new Generators())->boom();
        $this->assertSame(1, $g->current());

        try {
            $g->next();
            $this->fail('body exception must propagate');
        } catch (RuntimeException $e) {
            $this->assertSame('boom', $e->getMessage());
        }

        $this->assertFalse($g->valid());

        try {
            $g->getReturn();
            $this->fail('getReturn after an exception-closed generator must throw');
        } catch (Exception $e) {
            $this->assertSame("Cannot get return value of a generator that hasn't returned", $e->getMessage());
        }
    }

    public function testSnapshotSemanticsForPropertyIteration(): void
    {
        $test        = new Generators();
        $test->items = [1, 2, 3];

        $g = $test->overItems();
        $this->assertSame(1, $g->current());

        $test->items = [9];

        $this->assertSame([1, 2, 3], iterator_to_array($g, false), 'iteration uses the snapshot at loop entry');
    }

    public function testReverseArrayIteration(): void
    {
        $out = iterator_to_array((new Generators())->arrReverse([1, 2, 3]), false);

        $this->assertSame([3, 2, 1], $out);
    }

    public function testStepMethodsInvisibleToReflection(): void
    {
        $reflection = new ReflectionClass(Generators::class);

        $this->assertFalse($reflection->hasMethod('zephir_gen_step_countTo'));

        $stepLike = array_filter(
            $reflection->getMethods(),
            static fn ($m) => str_starts_with($m->getName(), 'zephir_gen_step_')
        );
        $this->assertSame([], $stepLike, 'internal step methods must not surface in reflection');
    }

    public function testGeneratorObjectIsNotCloneable(): void
    {
        $g = (new Generators())->three();

        $this->expectException(Error::class);
        clone $g;
    }
}
