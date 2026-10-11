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

use Extension\Issue2761\PhpAccessors;
use PHPUnit\Framework\TestCase;
use ReflectionMethod;
use stdClass;
use Stub\Issue2761\Accessors;
use Stub\Issue2761\Collaborator;
use Throwable;

/**
 * Property shortcut accessors carry the declared property type, or a class
 * named by `@var`, with the same signatures and errors as hand-written PHP.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2761
 */
final class Issue2761Test extends TestCase
{
    /**
     * @dataProvider getterProvider
     */
    public function testGetterReturnTypeMatchesPhp(string $getter): void
    {
        $this->assertSame(
            $this->typeOf(new ReflectionMethod(PhpAccessors::class, $getter), null),
            $this->typeOf(new ReflectionMethod(Accessors::class, $getter), null),
        );
    }

    public function getterProvider(): array
    {
        return [
            'declared string'         => ['getName'],
            'declared class'          => ['getThing'],
            'declared nullable class' => ['getMaybe'],
            'declared nullable int'   => ['getCount'],
            '@var qualified class'    => ['getDocQualified'],
            '@var relative class'     => ['getDocRelative'],
            '@var nullable class'     => ['getDocNullable'],
            'declared beats @var'     => ['getDeclaredWins'],
        ];
    }

    /**
     * @dataProvider setterProvider
     */
    public function testSetterParameterTypeMatchesPhp(string $setter): void
    {
        $this->assertSame(
            $this->typeOf(new ReflectionMethod(PhpAccessors::class, $setter), 0),
            $this->typeOf(new ReflectionMethod(Accessors::class, $setter), 0),
        );
    }

    public function setterProvider(): array
    {
        return [
            'declared string'         => ['setName'],
            'declared class'          => ['setThing'],
            'declared nullable class' => ['setMaybe'],
            'declared nullable int'   => ['setCount'],
            '@var qualified class'    => ['setDocQualified'],
            '@var nullable class'     => ['setDocNullable'],
            'declared beats @var'     => ['setDeclaredWins'],
        ];
    }

    public function testAccessorsRoundTripValues(): void
    {
        $object       = new Accessors();
        $collaborator = new Collaborator();

        $this->assertSame($object, $object->setName('zephir'));
        $this->assertSame('zephir', $object->getName());

        $object->setThing($collaborator);
        $this->assertSame($collaborator, $object->getThing());

        $this->assertNull($object->getMaybe());
        $object->setMaybe($collaborator);
        $this->assertSame($collaborator, $object->getMaybe());
        $object->setMaybe(null);
        $this->assertNull($object->getMaybe());

        $object->setCount(7);
        $this->assertSame(7, $object->getCount());

        $object->setDocQualified($collaborator);
        $this->assertSame($collaborator, $object->getDocQualified());

        $object->setDocNullable($collaborator);
        $this->assertSame($collaborator, $object->getDocNullable());

        $object->setDeclaredWins(3);
        $this->assertSame(3, $object->getDeclaredWins());
    }

    /**
     * @dataProvider uninitializedGetterProvider
     */
    public function testReadingAnUninitializedPropertyFailsAsInPhp(string $getter): void
    {
        $this->assertSame(
            $this->errorOf(static fn () => (new PhpAccessors())->{$getter}()),
            $this->errorOf(static fn () => (new Accessors())->{$getter}()),
        );
    }

    public function uninitializedGetterProvider(): array
    {
        return [
            'class'  => ['getThing'],
            'scalar' => ['getPending'],
        ];
    }

    /**
     * @dataProvider wrongArgumentProvider
     */
    public function testSetterRejectsAWrongTypeAsInPhp(string $setter, $value): void
    {
        $this->assertSame(
            $this->errorOf(static fn () => (new PhpAccessors())->{$setter}($value)),
            $this->errorOf(static fn () => (new Accessors())->{$setter}($value)),
        );
    }

    public function wrongArgumentProvider(): array
    {
        return [
            'string gets int'          => ['setName', 123],
            'class gets stdClass'      => ['setThing', new stdClass()],
            'class gets null'          => ['setThing', null],
            '@var class gets stdClass' => ['setDocQualified', new stdClass()],
            'nullable class gets text' => ['setMaybe', 'text'],
            'nullable int gets text'   => ['setCount', 'text'],
        ];
    }

    /**
     * The getter's return type, or the setter's first parameter type, with
     * the oracle's class name normalised away.
     */
    private function typeOf(ReflectionMethod $method, ?int $parameter): string
    {
        $type = null === $parameter
            ? $method->getReturnType()
            : $method->getParameters()[$parameter]->getType();

        return null === $type ? '(none)' : (string) $type;
    }

    private function errorOf(callable $call): string
    {
        try {
            $call();
        } catch (Throwable $e) {
            // `, called in <file> on line <n>` is only added for a userland callee.
            $message = preg_replace('/, called in .* on line \\d+$/', '', $e->getMessage());

            return get_class($e) . ': ' . str_replace(
                [PhpAccessors::class, Accessors::class],
                'CLASS',
                $message
            );
        }

        return '(no error)';
    }
}
