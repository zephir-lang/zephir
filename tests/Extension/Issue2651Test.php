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

use PHPUnit\Framework\TestCase;
use Stub\Issue2651;

/**
 * A persistent array default -- a typed property default, a trait property
 * default or an array class constant -- is one table shared by every instance.
 * It must be copied on the first write, exactly like PHP's own immutable array
 * defaults, instead of being mutated in place.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2651
 */
final class Issue2651Test extends TestCase
{
    public function testEmptyTypedArrayDefaultIsNotSharedBetweenInstances(): void
    {
        $copy             = (new Issue2651())->toArray();
        $copy['X-Leak']   = 'polluted';

        $this->assertSame([], (new Issue2651())->toArray());
    }

    public function testPopulatedTypedArrayDefaultIsNotSharedBetweenInstances(): void
    {
        $copy      = (new Issue2651())->getPreset();
        $copy['z'] = 'three';

        $this->assertSame(['x' => 'one', 'y' => 'two'], (new Issue2651())->getPreset());
    }

    public function testNestedTypedArrayDefaultIsNotSharedBetweenInstances(): void
    {
        $copy                     = (new Issue2651())->getNested();
        $copy['outer']['inner']   = 'overwritten';
        $copy['outer']['added']   = 'new';

        $this->assertSame(['outer' => ['inner' => 'deep']], (new Issue2651())->getNested());
    }

    public function testInternalWriteDoesNotLeakIntoOtherInstances(): void
    {
        $first = new Issue2651();
        $first->addHeader('X-First', 'yes');

        $this->assertSame(['X-First' => 'yes'], $first->toArray());
        $this->assertSame([], (new Issue2651())->toArray());
    }

    public function testArrayClassConstantIsNotMutableThroughACopy(): void
    {
        $copy      = Issue2651::MAP;
        $copy['c'] = 'third';

        $this->assertSame(['a' => 'first', 'b' => 'second'], Issue2651::MAP);
    }

    public function testTraitArrayDefaultIsNotSharedWhenMutatedFromUserland(): void
    {
        $holder = new class () {
            use \Stub\Traits\Nameable;
        };

        $tags   = $holder->getTags();
        $tags[] = 4;

        $meta        = $holder->getMeta();
        $meta['b'][] = 99;
        $meta['c']   = 'added';

        $fresh = new $holder();

        $this->assertSame([1, 2, 3], $fresh->getTags());
        $this->assertSame(['a' => 1, 'b' => [2, 3]], $fresh->getMeta());
    }

    /**
     * Positive control for the second half of #2651.
     *
     * Separating the shared default calls zend_array_dup(), whose immutable
     * branch is a raw memcpy of the buckets -- no addref on keys or values.
     * Destroying that copy then releases them. Unless the persistent strings are
     * non-refcounted and the table carries HASH_FLAG_STATIC_KEYS, the first
     * cycle frees strings the shared table still points at, and every later read
     * is a use-after-free.
     */
    public function testRepeatedSeparationDoesNotFreeTheSharedDefault(): void
    {
        for ($i = 0; $i < 200; ++$i) {
            $preset       = (new Issue2651())->getPreset();
            $preset['z']  = 'three';
            unset($preset);

            $constant      = Issue2651::MAP;
            $constant['c'] = 'third';
            unset($constant);

            // Encourage the allocator to reuse anything that was wrongly freed.
            $churn = array_fill(0, 32, str_repeat('c', 8));
            unset($churn);
        }

        $preset = (new Issue2651())->getPreset();

        $this->assertSame(['x', 'y'], array_keys($preset));
        $this->assertSame(['one', 'two'], array_values($preset));
        $this->assertSame(['a' => 'first', 'b' => 'second'], Issue2651::MAP);
    }

    /**
     * The same construct written in plain PHP is the reference behaviour.
     */
    public function testMatchesPlainPhpSemantics(): void
    {
        $php = new class () {
            public const MAP = ['a' => 'first', 'b' => 'second'];

            protected array $headers = [];
            protected array $preset  = ['x' => 'one', 'y' => 'two'];

            public function toArray(): array
            {
                return $this->headers;
            }

            public function getPreset(): array
            {
                return $this->preset;
            }
        };

        $phpCopy           = $php->toArray();
        $phpCopy['X-Leak'] = 'polluted';

        $zephirCopy           = (new Issue2651())->toArray();
        $zephirCopy['X-Leak'] = 'polluted';

        $this->assertSame((new $php())->toArray(), (new Issue2651())->toArray());
        $this->assertSame((new $php())->getPreset(), (new Issue2651())->getPreset());
    }
}
