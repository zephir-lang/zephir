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
 * The compile-time check of an instance property read follows PHP's rule, the
 * one #2734 gave methods and static properties: the property is reachable from
 * the class the code is written in (for a closure, the class around it), a
 * protected one when that class shares a line of inheritance with the
 * property's root class.
 *
 * A read PHP answers without an Error compiles: `__get()` answers any property
 * the read cannot reach, and `isset`, `empty` and `fetch` fetch the containers
 * of their chain silently. The runtime half is tests/Extension/Issue2784Test.php.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2784
 */
final class Issue2784Test extends TestCase
{
    use RunsZephirCommands;

    private const HIERARCHY = [
        'base.zep'    => <<<'ZEP'
            namespace Stub;

            class Base
            {
                protected prot = 1;

                private priv = 1;
            }
            ZEP,
        'mid.zep'     => "namespace Stub;\n\nclass Mid extends Base\n{\n}\n",
        'sibling.zep' => <<<'ZEP'
            namespace Stub;

            class Sibling extends Base
            {
                protected prot = 2;
            }
            ZEP,
        'magic.zep'   => <<<'ZEP'
            namespace Stub;

            class Magic extends Base
            {
                public function __get(string name)
                {
                    return name;
                }
            }
            ZEP,
    ];

    private const GET = 'public function __get(string name) { return name; } ';

    private string $projectDir = '';

    public static function accessibleProvider(): array
    {
        return [
            'protected property of a parent on a typed receiver' => [[
                'probe.zep' => self::probe('extends Base', 'public function run(<Base> o) { return o->prot; }'),
            ]],
            'protected property of a sibling through the common root' => [[
                'probe.zep' => self::probe('extends Base', 'public function run(<Sibling> o) { return o->prot; }'),
            ]],
            'private property of the scope on a subclass receiver' => [[
                'probe.zep'      => self::probe('', 'private x = 1; public function run(<ProbeChild> o) { return o->x; }'),
                'probechild.zep' => "namespace Stub;\n\nclass ProbeChild extends Probe\n{\n}\n",
            ]],
            'protected property in a closure of a subclass' => [[
                'probe.zep' => self::probe(
                    'extends Base',
                    'public function run() { return function (<Base> o) { return o->prot; }; }'
                ),
            ]],
            'private property of the scope in its closure' => [[
                'probe.zep' => self::probe(
                    '',
                    'private x = 1; public function run() { return function (<Probe> o) { return o->x; }; }'
                ),
            ]],
            'protected property of a receiver with __get' => [[
                'probe.zep' => self::probe('', 'public function run(<Magic> o) { return o->prot; }'),
            ]],
            'private property of a receiver with __get' => [[
                'probe.zep' => self::probe('', 'public function run(<Magic> o) { return o->priv; }'),
            ]],
            'undeclared property of a receiver with __get' => [[
                'probe.zep' => self::probe('', 'public function run(<Magic> o) { return o->virtual; }'),
            ]],
            'private property of a parent through this, with __get' => [[
                'probe.zep' => self::probe(
                    'extends Base',
                    self::GET . 'public function run() { var v; let v = this->priv; return v; }'
                ),
            ]],
            'private property of a parent returned through this, with __get' => [[
                'probe.zep' => self::probe('extends Base', self::GET . 'public function run() { return this->priv; }'),
            ]],
            'undeclared property through this, with __get' => [[
                'probe.zep' => self::probe('', self::GET . 'public function run() { var v; let v = this->virtual; return v; }'),
            ]],
            'undeclared property returned through this, with __get' => [[
                'probe.zep' => self::probe('', self::GET . 'public function run() { return this->virtual; }'),
            ]],
            'isset of an offset of a private property' => [[
                'probe.zep' => self::probe('', 'public function run(<Base> o) { return isset o->priv["k"]; }'),
            ]],
            'isset of a property of a protected property' => [[
                'probe.zep' => self::probe('', 'public function run(<Base> o) { return isset o->prot->x; }'),
            ]],
            'empty of a property of a private property' => [[
                'probe.zep' => self::probe('', 'public function run(<Base> o) { return empty o->priv->x; }'),
            ]],
            'fetch of an offset of a private property' => [[
                'probe.zep' => self::probe('', 'public function run(<Base> o) { var v; return fetch v, o->priv["k"]; }'),
            ]],
            'property written in a function' => [[
                'probe.zep' => "namespace Stub;\n\nclass Probe\n{\n}\n\nfunction probe_write(var o)\n{\n    let o->x = 1;\n}\n",
            ]],
            'isset of an offset of an undeclared property of this' => [[
                'probe.zep' => self::probe('', 'public function run() { return isset this->undeclared["k"]; }'),
            ]],
        ];
    }

    /**
     * @dataProvider accessibleProvider
     */
    public function testReadPhpAllowsCompiles(array $files): void
    {
        $result = $this->generate(self::HIERARCHY + $files);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
    }

    public static function inaccessibleProvider(): array
    {
        $protected = "Attempt to access protected property 'prot' outside of its declared class context: 'Stub\\Base'";
        $private   = "Attempt to access private property 'priv' outside of its declared class context: 'Stub\\Base'";

        return [
            'protected property on an unrelated receiver' => [
                ['probe.zep' => self::probe('', 'public function run(<Base> o) { return o->prot; }')],
                $protected,
            ],
            'protected property inherited by an unrelated receiver' => [
                ['probe.zep' => self::probe('', 'public function run(<Mid> o) { return o->prot; }')],
                $protected,
            ],
            'protected property in a closure of an unrelated class' => [
                ['probe.zep' => self::probe('', 'public function run() { return function (<Base> o) { return o->prot; }; }')],
                $protected,
            ],
            'protected property read in a function' => [
                ['probe.zep' => "namespace Stub;\n\nclass Probe\n{\n}\n\nfunction probe_read(<Base> o)\n{\n    return o->prot;\n}\n"],
                $protected,
            ],
            'protected property as the container of a read' => [
                ['probe.zep' => self::probe('', 'public function run(<Base> o) { return o->prot->x; }')],
                $protected,
            ],
            'private property on an unrelated receiver' => [
                ['probe.zep' => self::probe('', 'public function run(<Base> o) { return o->priv; }')],
                $private,
            ],
            'private property of a parent through this' => [
                ['probe.zep' => self::probe('extends Base', 'public function run() { var v; let v = this->priv; return v; }')],
                $private,
            ],
            'private property of a parent returned through this' => [
                ['probe.zep' => self::probe('extends Base', 'public function run() { return this->priv; }')],
                $private,
            ],
        ];
    }

    /**
     * @dataProvider inaccessibleProvider
     */
    public function testReadPhpRefusesIsACompileError(array $files, string $expected): void
    {
        $result = $this->generate(self::HIERARCHY + $files);
        $output = $result['stdout'] . $result['stderr'];

        $this->assertStringNotContainsString('Fatal error', $output);
        $this->assertStringContainsString($expected, $output);
        $this->assertNotSame(0, $result['exitCode']);
    }

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2784-' . bin2hex(random_bytes(6));
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

    private static function probe(string $extends, string $members): string
    {
        return "namespace Stub;\n\nclass Probe " . $extends . "\n{\n    " . $members . "\n}\n";
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
