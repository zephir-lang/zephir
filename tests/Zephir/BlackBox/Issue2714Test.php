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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * A parent class the compiler cannot locate is only a `nonexistent-class`
 * warning, because a class may extend one that exists only at runtime. The
 * compiler keeps it as a `DefinitionRuntime`, which has no methods, constants
 * or properties to check, and every `parent::` access that treated it as a
 * full definition died with a PHP fatal error instead of a compile error.
 *
 * `let parent::x = v` never compiled at all: the lookup it went through
 * tested the parent's *name* with `instanceof`, so it reported "does not
 * extend any class" for every class. The runtime half is
 * tests/zept/issue2714_parent_static_property.zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2714
 */
final class Issue2714Test extends TestCase
{
    use RunsZephirCommands;

    public static function unresolvedParentAccessProvider(): array
    {
        return [
            'method call' => [
                'public function run() { return parent::nope(); }',
                'Cannot call method "nope" on parent because class Other\Missing does not exist',
            ],
            'constructor call' => [
                'public function __construct() { parent::__construct(); }',
                'Cannot call method "__construct" on parent because class Other\Missing does not exist',
            ],
            'constant' => [
                'public function run() { return parent::FOO; }',
                'Cannot find constant called "FOO" on parent because class Other\Missing does not exist',
            ],
            'class constant' => [
                'const A = parent::B;',
                'Cannot find constant called "B" on parent because class Other\Missing does not exist',
            ],
            'static property read' => [
                'public function run() { return parent::x; }',
                'Cannot access static property "x" on parent because class Other\Missing does not exist',
            ],
            'static property append' => [
                'public function run() { let parent::x[] = 1; }',
                'Cannot assign static property "x" on parent because class Other\Missing does not exist',
            ],
            'static property element' => [
                'public function run() { let parent::x[1] = 1; }',
                'Cannot assign static property "x" on parent because class Other\Missing does not exist',
            ],
            'static property assign' => [
                'public function run() { let parent::x = 1; }',
                'Cannot assign static property "x" on parent because class Other\Missing does not exist',
            ],
            'static property compound assign' => [
                'public function run() { let parent::x -= 1; }',
                'Cannot assign static property "x" on parent because class Other\Missing does not exist',
            ],
        ];
    }

    /**
     * @dataProvider unresolvedParentAccessProvider
     */
    public function testParentAccessOnUnresolvedParentIsACompileError(string $member, string $expected): void
    {
        $result = $this->generate([
            'probe.zep' => "namespace Stub;\n\nclass Probe extends \\Other\\Missing\n{\n    {$member}\n}\n",
        ]);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringNotContainsString('Fatal error', $output);
        $this->assertStringContainsString($expected, $output);
        $this->assertNotSame(0, $result['exitCode']);
    }

    public function testArrayPropertyOnUnresolvedParentCompiles(): void
    {
        $result = $this->generate([
            'probe.zep' => <<<'ZEP'
                namespace Stub;

                class Probe extends \Other\Missing
                {
                    protected items = [1];

                    protected static shared = [2];
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');
        $this->assertStringContainsString('zephir_get_internal_ce(SL("other\\\\missing"))', $generated);
        $this->assertStringContainsString('zephir_init_properties_Stub_Probe', $generated);
    }

    public function testParentClassOnUnresolvedParentFoldsToItsName(): void
    {
        // PHP resolves parent::class to the parent's name, which the compiler
        // knows even when it cannot see the class itself.
        $result = $this->generate([
            'probe.zep' => <<<'ZEP'
                namespace Stub;

                class Probe extends \Other\Missing
                {
                    public function run() -> string
                    {
                        return parent::class;
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringContainsString(
            'RETURN_STRING("Other\\\\Missing")',
            (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c')
        );
    }

    public function testParentStaticPropertyAssignOnResolvedParentCompiles(): void
    {
        $result = $this->generate([
            'base.zep' => <<<'ZEP'
                namespace Stub;

                class Base
                {
                    public static x = 1;
                }
                ZEP,
            'probe.zep' => <<<'ZEP'
                namespace Stub;

                class Probe extends Base
                {
                    public static function run() -> void
                    {
                        let parent::x = 5;
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $this->assertStringContainsString(
            'zephir_update_static_property_ce(stub_base_ce, ZEND_STRL("x")',
            (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c')
        );
    }

    public static function parentKeywordWithoutParentProvider(): array
    {
        return [
            'new parent' => ['public function run() { return new parent(); }'],
            'instanceof parent' => ['public function run(var o) { return o instanceof parent; }'],
        ];
    }

    /**
     * @dataProvider parentKeywordWithoutParentProvider
     */
    public function testParentKeywordWithoutParentIsACompileError(string $member): void
    {
        $result = $this->generate([
            'probe.zep' => "namespace Stub;\n\nclass Probe\n{\n    {$member}\n}\n",
        ]);
        $output = $result['stdout'] . $result['stderr'];

        // PHP's own compile error, zend_compile.c.
        $this->assertStringContainsString('Cannot use "parent" when current class scope has no parent', $output);
        $this->assertNotSame(0, $result['exitCode']);
    }

    public function testNewParentAndInstanceOfNameTheParentAndCurrentClass(): void
    {
        $result = $this->generate([
            'base.zep' => <<<'ZEP'
                namespace Stub;

                class Base
                {
                }
                ZEP,
            'probe.zep' => <<<'ZEP'
                namespace Stub;

                class Probe extends Base
                {
                    public function make()
                    {
                        return new parent();
                    }

                    public function isParent(var o) -> bool
                    {
                        return o instanceof parent;
                    }

                    public function isSelf(var o) -> bool
                    {
                        return o instanceof self;
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');
        $this->assertStringNotContainsString('Stub\\\\parent', $generated);
        $this->assertStringNotContainsString('Stub\\\\self', $generated);
        $this->assertStringContainsString('object_init_ex(return_value, stub_base_ce)', $generated);
        $this->assertStringContainsString('zephir_instance_of_ev(o, stub_base_ce)', $generated);
        $this->assertStringContainsString('zephir_instance_of_ev(o, stub_probe_ce)', $generated);
    }

    public function testNewParentAndInstanceOfParentOnInternalParentUseItsClassEntry(): void
    {
        $result = $this->generate([
            'probe.zep' => <<<'ZEP'
                namespace Stub;

                class Probe extends \Exception
                {
                    public function make()
                    {
                        return new parent("m", 3);
                    }

                    public function isParent(var o) -> bool
                    {
                        return o instanceof parent;
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');
        $this->assertStringNotContainsString('parent_ce', $generated);
        $this->assertStringContainsString('object_init_ex(return_value, zend_ce_exception)', $generated);
        // The same check `o instanceof \Exception` compiles to.
        $this->assertStringContainsString('zephir_is_instance_of(o, SL("Exception"))', $generated);
    }

    public function testInstanceOfStaticUsesTheCalledClass(): void
    {
        $result = $this->generate([
            'probe.zep' => <<<'ZEP'
                namespace Stub;

                class Probe
                {
                    public static function isStatic(var o) -> bool
                    {
                        return o instanceof static;
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');
        $this->assertStringNotContainsString('Stub\\\\static', $generated);
        $this->assertStringContainsString('zephir_instance_of_ev(o, zend_get_called_scope(execute_data))', $generated);
    }

    public function testNewParentOnUnresolvedParentIsLookedUpAtRuntime(): void
    {
        // As in PHP, a parent the compiler cannot see is fetched by name when
        // the code runs, like any other class it cannot see.
        $result = $this->generate([
            'probe.zep' => <<<'ZEP'
                namespace Stub;

                class Probe extends \Other\Missing
                {
                    public function make()
                    {
                        return new parent();
                    }
                }
                ZEP,
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/probe.zep.c');
        $this->assertStringNotContainsString('Stub\\\\parent', $generated);
        $this->assertStringContainsString('SL("Other\\\\Missing")', $generated);
    }

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2714-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    /**
     * @param array<string, string> $files
     *
     * @return array{exitCode: int, stdout: string, stderr: string}
     */
    private function generate(array $files): array
    {
        foreach ($files as $name => $source) {
            file_put_contents($this->projectDir . '/stub/' . $name, $source);
        }

        return $this->runZephir('generate --no-ansi', $this->projectDir);
    }
}
