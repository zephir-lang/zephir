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
use Stub\Issue2699;
use Stub\Traits\Nameable;

/**
 * A persistent array default is one table shared by every instance, and its
 * keys belong to the module for the life of the process. Copying it must never
 * hand a request-scoped copy the right to free them.
 *
 * zend_array_dup()'s immutable branch is a raw bucket memcpy: it addrefs
 * neither keys nor values, and it inherits HASH_FLAG_STATIC_KEYS from the
 * source. So the copy borrows the module's key strings while claiming it owns
 * nothing. Inserting a key that is NOT interned clears the flag on that copy,
 * and destroying it then releases every key it holds, the borrowed ones
 * included.
 *
 * Which is why every test here builds its keys at runtime. A PHP literal
 * offset is interned by the compiler, the flag survives, and nothing is
 * released -- see testALiteralKeyIsTheNegativeControl().
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2699
 */
final class Issue2699Test extends TestCase
{
    /**
     * The control for every test below it.
     *
     * This is the loop #2651 already shipped, and it passes with the defect in
     * place. If it ever fails, the tests below are measuring something else.
     */
    public function testALiteralKeyIsTheNegativeControl(): void
    {
        for ($i = 0; $i < 200; ++$i) {
            $copy        = Issue2699::MAP;
            $copy['lit'] = true;
            unset($copy);

            $churn = array_fill(0, 32, str_repeat('c', 8));
            unset($churn);
        }

        $this->assertSame(['*', 'a'], array_keys(Issue2699::MAP));
        $this->assertSame([true, 1], array_values(Issue2699::MAP));
    }

    public function testCopyOfAnArrayConstantWithARuntimeKeyDoesNotFreeItsKeys(): void
    {
        for ($i = 0; $i < 200; ++$i) {
            $copy = Issue2699::MAP;
            // Built, not written: a literal would be interned and never clear
            // HASH_FLAG_STATIC_KEYS on the copy.
            $copy['component-'.$i] = true;
            unset($copy);

            // Encourage the allocator to reuse anything that was wrongly freed.
            $churn = array_fill(0, 32, str_repeat('c', 8));
            unset($churn);
        }

        $this->assertSame(['*', 'a'], array_keys(Issue2699::MAP));
        $this->assertSame([true, 1], array_values(Issue2699::MAP));
    }

    public function testCopyOfAPropertyDefaultWithARuntimeKeyDoesNotFreeItsKeys(): void
    {
        for ($i = 0; $i < 200; ++$i) {
            $copy                  = (new Issue2699())->getComponents();
            $copy['component-'.$i] = true;
            unset($copy);

            $churn = array_fill(0, 32, str_repeat('c', 8));
            unset($churn);
        }

        $components = (new Issue2699())->getComponents();

        $this->assertSame(['*'], array_keys($components));
        $this->assertSame([true], array_values($components));
    }

    public function testCopyOfANestedDefaultWithARuntimeKeyDoesNotFreeItsKeys(): void
    {
        for ($i = 0; $i < 200; ++$i) {
            $copy = (new Issue2699())->getNested();
            // The outer table separates first, then the inner one: both are
            // persistent tables with borrowed keys.
            $copy['outer']['inner-'.$i] = true;
            $copy['added-'.$i]          = true;
            unset($copy);

            $churn = array_fill(0, 32, str_repeat('c', 8));
            unset($churn);
        }

        $nested = (new Issue2699())->getNested();

        $this->assertSame(['outer'], array_keys($nested));
        $this->assertSame(['inner'], array_keys($nested['outer']));
        $this->assertSame([true], array_values($nested['outer']));
    }

    /**
     * The third consumer of zephir_persist_constant_zval(): a trait property
     * default, which zephir_declare_property_array() stores on the class entry.
     */
    public function testTraitArrayDefaultSurvivesACopyWrittenWithARuntimeKey(): void
    {
        $holder = new class () {
            use Nameable;
        };

        for ($i = 0; $i < 200; ++$i) {
            $meta               = $holder->getMeta();
            $meta['tag-'.$i]    = $i;
            $meta['b']['n'.$i]  = $i;
            unset($meta);

            $churn = array_fill(0, 32, str_repeat('c', 8));
            unset($churn);
        }

        $fresh = new $holder();

        $this->assertSame(['a', 'b'], array_keys($fresh->getMeta()));
        $this->assertSame(['a' => 1, 'b' => [2, 3]], $fresh->getMeta());
    }

    /**
     * The shape reported against Phalcon\Acl\Adapter\Memory: the component name
     * arrives as a method argument, so it is never interned.
     */
    public function testInternalWriteLeavesOtherInstancesIntact(): void
    {
        for ($i = 0; $i < 200; ++$i) {
            $acl = new Issue2699();
            $acl->add('Login-'.$i);
            $acl->add('Index-'.$i);
            unset($acl);

            $churn = array_fill(0, 32, str_repeat('c', 8));
            unset($churn);
        }

        $acl = new Issue2699();
        $acl->add('Login');

        $this->assertSame(['*' => true, 'Login' => true], $acl->getComponents());
        $this->assertSame(['*' => true], (new Issue2699())->getComponents());
    }

    /**
     * The same construct written in plain PHP is the reference behaviour.
     */
    public function testMatchesPlainPhpSemantics(): void
    {
        $php = new class () {
            public const MAP = ['*' => true, 'a' => 1];

            protected array $components = ['*' => true];

            public function add(string $key): void
            {
                $this->components[$key] = true;
            }

            public function getComponents(): array
            {
                return $this->components;
            }
        };

        for ($i = 0; $i < 100; ++$i) {
            $phpCopy                  = (new $php())->getComponents();
            $phpCopy['component-'.$i] = true;
            unset($phpCopy);

            $zephirCopy                  = (new Issue2699())->getComponents();
            $zephirCopy['component-'.$i] = true;
            unset($zephirCopy);
        }

        $phpInstance = new $php();
        $phpInstance->add('Login');

        $zephirInstance = new Issue2699();
        $zephirInstance->add('Login');

        $this->assertSame($phpInstance->getComponents(), $zephirInstance->getComponents());
        $this->assertSame((new $php())->getComponents(), (new Issue2699())->getComponents());
        $this->assertSame($php::MAP, Issue2699::MAP);
    }
}
