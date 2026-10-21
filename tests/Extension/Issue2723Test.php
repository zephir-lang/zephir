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
use Stub\Issue2723;
use Stub\Issue2723Magic;

/**
 * Counts how often the code under test evaluated an argument or reached the
 * statement after a call.
 */
final class Issue2723Counter
{
    public int $count = 0;

    public function bump(): int
    {
        return ++$this->count;
    }
}

final class Issue2723Stringable
{
    public function __toString(): string
    {
        return 'target';
    }
}

final class Issue2723Receiver
{
    public function target(): string
    {
        return 'receiver target';
    }
}

/**
 * Plain PHP twin of Stub\Issue2723Base.
 */
class Issue2723MirrorBase
{
    public static function baseTarget(): string
    {
        return 'base';
    }

    public static function baseWithArgument($argument): string
    {
        return 'base argument ' . $argument;
    }

    public static function baseCalledClass(): string
    {
        return get_called_class();
    }

    public function baseInstance(): string
    {
        return 'base instance of ' . get_class($this);
    }
}

/**
 * Plain PHP twin of Stub\Issue2723: the same statements, written as PHP.
 */
class Issue2723Mirror extends Issue2723MirrorBase
{
    public static function target(): string
    {
        return 'target';
    }

    public static function calledClass(): string
    {
        return get_called_class();
    }

    public static function withArgument($argument): string
    {
        return 'argument ' . $argument;
    }

    public function instanceTarget(): string
    {
        return 'instance of ' . get_class($this);
    }

    public function viaParam($className, $name)
    {
        return $className::$name();
    }

    public function viaParamWithArgument($className, $name, $argument)
    {
        return $className::$name($argument);
    }

    public function statementDynamicName($className, $name, $counter)
    {
        $className::$name();

        return $counter->bump();
    }

    public function statementLiteralName($className, $counter)
    {
        $className::target();

        return $counter->bump();
    }

    public function missingClassSkipsArguments($className, $counter)
    {
        return $className::withArgument($counter->bump());
    }

    public function assignToName($className, $name)
    {
        $name = $className::$name();

        return $name;
    }

    public function assignToMethodName($receiver, $name)
    {
        $name = $receiver->$name();

        return $name;
    }

    public function methodViaParam($receiver, $name)
    {
        return $receiver->$name();
    }

    public function selfDynamic($name)
    {
        return self::$name();
    }

    public static function selfDynamicStatic($name)
    {
        return self::$name();
    }

    public function selfDynamicStatement($name, $counter)
    {
        self::$name();

        return $counter->bump();
    }

    public function parentDynamic($name)
    {
        return parent::$name();
    }

    public function parentDynamicWithArgument($name, $argument)
    {
        return parent::$name($argument);
    }

    public function namedDynamic($name)
    {
        return Issue2723MirrorBase::$name();
    }

    public static function namedDynamicStatic($name)
    {
        return Issue2723MirrorBase::$name();
    }

    public function internalDynamic($name, $argument)
    {
        return \DateTime::$name('!Y-m-d', $argument);
    }

    public function selfString()
    {
        return self::{'target'}();
    }

    public function staticString()
    {
        return static::{'calledClass'}();
    }

    public function namedString()
    {
        return Issue2723MirrorBase::{'baseTarget'}();
    }

    public function dynamicClassString($className)
    {
        return $className::{'target'}();
    }
}

class Issue2723MirrorMagic
{
    public function __call(string $name, array $arguments): string
    {
        return '__call ' . $name;
    }

    public static function __callStatic(string $name, array $arguments): string
    {
        return '__callStatic ' . $name;
    }

    public function selfDynamic($name)
    {
        return self::$name();
    }

    public static function selfDynamicStatic($name)
    {
        return self::$name();
    }
}

/**
 * A PHP subclass of each side, so `self::`, `parent::` and named-class calls
 * run with a called class that is not the declaring one. Neither overrides a
 * target: which override `self::` reaches is the separate, literal-call issue.
 */
final class Issue2723Child extends Issue2723
{
}

final class Issue2723MirrorChild extends Issue2723Mirror
{
}

/**
 * Every call goes through the extension and through its plain PHP twin, and
 * the two transcripts have to match once the twin's class names are spelled
 * as the extension's.
 *
 * @issue https://github.com/zephir-lang/zephir/issues/2723
 */
final class Issue2723Test extends TestCase
{
    private const COUNTER = '@counter';

    private const NAMES = [
        'Extension\Issue2723MirrorChild' => 'Extension\Issue2723Child',
        'Extension\Issue2723MirrorMagic' => 'Stub\Issue2723Magic',
        'Extension\Issue2723MirrorBase'  => 'Stub\Issue2723Base',
        'Extension\Issue2723Mirror'      => 'Stub\Issue2723',
    ];

    /**
     * @return iterable<string, array{0: callable, 1: callable}>
     */
    public static function dynamicClassProvider(): iterable
    {
        $names = [
            'string'      => 'target',
            'mixed case'  => 'TaRgEt',
            'undefined'   => 'noSuchMethod',
            'null'        => null,
            'int'         => 1,
            'bool'        => true,
            'array'       => ['target'],
            'stringable'  => new Issue2723Stringable(),
        ];

        foreach ($names as $label => $name) {
            yield 'name ' . $label => [
                static fn () => (new Issue2723())->viaParam(Issue2723::class, $name),
                static fn () => (new Issue2723Mirror())->viaParam(Issue2723Mirror::class, $name),
            ];
        }

        $classes = [
            'object'           => [new Issue2723(), new Issue2723Mirror()],
            'subclass object'  => [new Issue2723Child(), new Issue2723MirrorChild()],
            'leading slash'    => ['\Stub\Issue2723', '\Extension\Issue2723Mirror'],
            'missing'          => ['NoSuchClass2723', 'NoSuchClass2723'],
            'keyword string'   => ['self', 'self'],
            'null'             => [null, null],
            'int'              => [1, 1],
            'array'            => [[], []],
        ];

        foreach ($classes as $label => [$zephirClass, $phpClass]) {
            yield 'class ' . $label => [
                static fn () => (new Issue2723())->viaParam($zephirClass, 'calledClass'),
                static fn () => (new Issue2723Mirror())->viaParam($phpClass, 'calledClass'),
            ];
        }
    }

    /**
     * @dataProvider dynamicClassProvider
     */
    public function testDynamicClassAndName(callable $zephir, callable $php): void
    {
        $this->assertMatchesMirror($zephir, $php);
    }

    public function testNameInEveryVariableKind(): void
    {
        $test = new Issue2723();

        $this->assertSame('target', $test->viaLocal(Issue2723::class, 'target'));
        $this->assertSame('target', $test->viaStringParam(Issue2723::class, 'target'));
        $this->assertSame('target', $test->viaMixedParam(Issue2723::class, 'target'));
        $this->assertSame('target', $test->viaMixedClass(Issue2723::class));
        $this->assertSame('target', $test->assignViaParam(Issue2723::class, 'target'));
        $this->assertSame(
            'receiver target',
            $test->methodViaMixedParam(new Issue2723Receiver(), 'target')
        );
    }

    /**
     * @return iterable<string, array{0: callable, 1: callable}>
     */
    public static function callShapeProvider(): iterable
    {
        $zephir = new Issue2723();
        $php    = new Issue2723Mirror();
        $built  = static fn () => 'tar' . str_repeat('g', 1) . 'et';

        yield 'argument' => [
            static fn () => $zephir->viaParamWithArgument(Issue2723::class, 'withArgument', 'x'),
            static fn () => $php->viaParamWithArgument(Issue2723Mirror::class, 'withArgument', 'x'),
        ];
        yield 'statement' => [
            static fn () => $zephir->statementDynamicName(Issue2723::class, 'target', new Issue2723Counter()),
            static fn () => $php->statementDynamicName(Issue2723Mirror::class, 'target', new Issue2723Counter()),
        ];
        yield 'statement, literal name' => [
            static fn () => $zephir->statementLiteralName(Issue2723::class, new Issue2723Counter()),
            static fn () => $php->statementLiteralName(Issue2723Mirror::class, new Issue2723Counter()),
        ];
        yield 'statement, missing class stops' => [
            static fn () => self::counted(
                static fn ($counter) => $zephir->statementLiteralName('NoSuchClass2723', $counter)
            ),
            static fn () => self::counted(
                static fn ($counter) => $php->statementLiteralName('NoSuchClass2723', $counter)
            ),
        ];
        yield 'statement, bad name stops' => [
            static fn () => self::counted(
                static fn ($counter) => $zephir->statementDynamicName(Issue2723::class, null, $counter)
            ),
            static fn () => self::counted(
                static fn ($counter) => $php->statementDynamicName(Issue2723Mirror::class, null, $counter)
            ),
        ];
        yield 'missing class skips the arguments' => [
            static fn () => self::counted(
                static fn ($counter) => $zephir->missingClassSkipsArguments('NoSuchClass2723', $counter)
            ),
            static fn () => self::counted(
                static fn ($counter) => $php->missingClassSkipsArguments('NoSuchClass2723', $counter)
            ),
        ];
        yield 'result into the name variable' => [
            static fn () => $zephir->assignToName(Issue2723::class, $built()),
            static fn () => $php->assignToName(Issue2723Mirror::class, $built()),
        ];
        yield 'method result into the name variable' => [
            static fn () => $zephir->assignToMethodName(new Issue2723Receiver(), $built()),
            static fn () => $php->assignToMethodName(new Issue2723Receiver(), $built()),
        ];
        yield 'method, null name' => [
            static fn () => $zephir->methodViaParam(new Issue2723Receiver(), null),
            static fn () => $php->methodViaParam(new Issue2723Receiver(), null),
        ];
        yield 'method, array name' => [
            static fn () => $zephir->methodViaParam(new Issue2723Receiver(), ['target']),
            static fn () => $php->methodViaParam(new Issue2723Receiver(), ['target']),
        ];
        yield 'method, null name on null' => [
            static fn () => $zephir->methodViaParam(null, null),
            static fn () => $php->methodViaParam(null, null),
        ];
        yield 'internal class' => [
            static fn () => $zephir->internalDynamic('createFromFormat', '2026-10-19'),
            static fn () => $php->internalDynamic('createFromFormat', '2026-10-19'),
        ];
    }

    /**
     * @dataProvider callShapeProvider
     */
    public function testCallShape(callable $zephir, callable $php): void
    {
        $this->assertMatchesMirror($zephir, $php);
    }

    /**
     * @return iterable<string, array{0: callable, 1: callable}>
     */
    public static function knownClassProvider(): iterable
    {
        $receivers = [
            'own instance'      => [new Issue2723(), new Issue2723Mirror()],
            'subclass instance' => [new Issue2723Child(), new Issue2723MirrorChild()],
        ];

        $calls = [
            'self static'             => ['selfDynamic', ['calledClass']],
            'self instance'           => ['selfDynamic', ['instanceTarget']],
            'self inherited'          => ['selfDynamic', ['baseCalledClass']],
            'self undefined'          => ['selfDynamic', ['noSuchMethod']],
            'self null'               => ['selfDynamic', [null]],
            'self statement'          => ['selfDynamicStatement', ['target', self::COUNTER]],
            'parent static'           => ['parentDynamic', ['baseCalledClass']],
            'parent instance'         => ['parentDynamic', ['baseInstance']],
            'parent undefined'        => ['parentDynamic', ['noSuchMethod']],
            'parent argument'         => ['parentDynamicWithArgument', ['baseWithArgument', 'x']],
            'named static'            => ['namedDynamic', ['baseCalledClass']],
            'named instance'          => ['namedDynamic', ['baseInstance']],
            'named undefined'         => ['namedDynamic', ['noSuchMethod']],
            'self string name'        => ['selfString', []],
            'static string name'      => ['staticString', []],
            'named string name'       => ['namedString', []],
            'constant string name'    => ['constantString', []],
            'dynamic class, string'   => ['dynamicClassString', [Issue2723::class]],
        ];

        foreach ($receivers as $receiverLabel => [$zephir, $php]) {
            foreach ($calls as $callLabel => [$method, $arguments]) {
                $phpMethod = 'constantString' === $method ? 'namedString' : $method;

                yield $callLabel . ', ' . $receiverLabel => [
                    static fn () => $zephir->{$method}(...self::arguments($arguments, false)),
                    static fn () => $php->{$phpMethod}(...self::arguments($arguments, true)),
                ];
            }
        }

        yield 'self from a static method' => [
            static fn () => Issue2723::selfDynamicStatic('calledClass'),
            static fn () => Issue2723Mirror::selfDynamicStatic('calledClass'),
        ];
        yield 'self from a subclass static method' => [
            static fn () => Issue2723Child::selfDynamicStatic('calledClass'),
            static fn () => Issue2723MirrorChild::selfDynamicStatic('calledClass'),
        ];
        yield 'self non-static from a static method' => [
            static fn () => Issue2723::selfDynamicStatic('instanceTarget'),
            static fn () => Issue2723Mirror::selfDynamicStatic('instanceTarget'),
        ];
        yield 'self compound name from a static method' => [
            static fn () => Issue2723::selfDynamicStatic('parent::baseTarget'),
            static fn () => Issue2723Mirror::selfDynamicStatic('parent::baseTarget'),
        ];
        yield 'named non-static from a static method' => [
            static fn () => Issue2723::namedDynamicStatic('baseInstance'),
            static fn () => Issue2723Mirror::namedDynamicStatic('baseInstance'),
        ];
        yield 'named static from a static method' => [
            static fn () => Issue2723Child::namedDynamicStatic('baseCalledClass'),
            static fn () => Issue2723MirrorChild::namedDynamicStatic('baseCalledClass'),
        ];
        yield 'dynamic class, non-static target' => [
            static fn () => (new Issue2723())->viaParam(Issue2723::class, 'instanceTarget'),
            static fn () => (new Issue2723Mirror())->viaParam(Issue2723Mirror::class, 'instanceTarget'),
        ];
        yield 'dynamic class, unrelated non-static target' => [
            static fn () => (new Issue2723())->viaParam(Issue2723Receiver::class, 'target'),
            static fn () => (new Issue2723Mirror())->viaParam(Issue2723Receiver::class, 'target'),
        ];
    }

    /**
     * @dataProvider knownClassProvider
     */
    public function testKnownClassDynamicName(callable $zephir, callable $php): void
    {
        $this->assertMatchesMirror($zephir, $php);
    }

    /**
     * @return iterable<string, array{0: callable, 1: callable}>
     */
    public static function magicProvider(): iterable
    {
        yield '__call from an instance' => [
            static fn () => (new Issue2723Magic())->selfDynamic('NoSuchMethod'),
            static fn () => (new Issue2723MirrorMagic())->selfDynamic('NoSuchMethod'),
        ];
        yield '__callStatic from a static method' => [
            static fn () => Issue2723Magic::selfDynamicStatic('NoSuchMethod'),
            static fn () => Issue2723MirrorMagic::selfDynamicStatic('NoSuchMethod'),
        ];
        yield '__callStatic through a dynamic class' => [
            static fn () => (new Issue2723())->viaParam(Issue2723Magic::class, 'NoSuchMethod'),
            static fn () => (new Issue2723Mirror())->viaParam(Issue2723MirrorMagic::class, 'NoSuchMethod'),
        ];
    }

    /**
     * @dataProvider magicProvider
     */
    public function testMagicFallback(callable $zephir, callable $php): void
    {
        $this->assertMatchesMirror($zephir, $php);
    }

    /**
     * `parent::` names the declaring class's parent, also when the object is a
     * grandchild whose own parent is a different class.
     */
    public function testParentUndefinedFromAGrandchild(): void
    {
        $this->assertMatchesMirror(
            static fn () => (new Issue2723Child())->parentDynamic('noSuchMethod'),
            static fn () => (new Issue2723MirrorChild())->parentDynamic('noSuchMethod')
        );
    }

    public function testParentResultIntoAParameter(): void
    {
        $this->assertSame('base', (new Issue2723())->parentResultIntoParameter('unused'));
    }

    /**
     * Gives each side its own counter, and the mirror its own class.
     */
    private static function arguments(array $arguments, bool $mirror): array
    {
        return array_map(
            static function ($argument) use ($mirror) {
                if (self::COUNTER === $argument) {
                    return new Issue2723Counter();
                }

                return $mirror && Issue2723::class === $argument ? Issue2723Mirror::class : $argument;
            },
            $arguments
        );
    }

    /**
     * Runs a call with a fresh counter and reports how far it got.
     */
    private static function counted(callable $call): string
    {
        $counter = new Issue2723Counter();

        try {
            $result = var_export($call($counter), true);
        } catch (\Throwable $e) {
            $result = get_class($e) . ': ' . $e->getMessage();
        }

        return $result . ' after ' . $counter->count . ' bumps';
    }

    private function assertMatchesMirror(callable $zephir, callable $php): void
    {
        $names = self::NAMES;
        foreach (self::NAMES as $mirror => $extension) {
            // var_export() doubles the backslash inside a returned string.
            $names[str_replace('\\', '\\\\', $mirror)] = str_replace('\\', '\\\\', $extension);
        }

        $this->assertSame(
            strtr($this->transcript($php), $names),
            $this->transcript($zephir),
            'The extension must behave exactly as PHP does'
        );
    }

    /**
     * The diagnostics raised and the outcome, readable so that the mirror's
     * class names can be respelled inside returned strings too.
     */
    private function transcript(callable $subject): string
    {
        $diagnostics = [];

        set_error_handler(static function (int $code, string $message) use (&$diagnostics): bool {
            $diagnostics[] = $message;

            return true;
        });

        try {
            $result = var_export($subject(), true);
        } catch (\Throwable $e) {
            $result = get_class($e) . ': ' . $e->getMessage();
        } finally {
            restore_error_handler();
        }

        return implode(' ; ', $diagnostics) . ' => ' . $result;
    }
}
