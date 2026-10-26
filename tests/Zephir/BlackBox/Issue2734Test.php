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
 * Compile-time visibility checks follow PHP's own rule: a member is reachable
 * from the class the code is written in (for a closure, the class around it),
 * never from the class of the receiver. A protected member also needs the
 * scope and the member's root class to share a line of inheritance, at any
 * depth, and a receiver with `__call()` takes every inaccessible call.
 *
 * The checks used to compare against the receiver and its direct parent, so a
 * grandparent's protected method was rejected even inside the grandparent,
 * while a call from an unrelated class compiled. The runtime half is
 * tests/zept/issue2734_*.zept.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2734
 */
final class Issue2734Test extends TestCase
{
    use RunsZephirCommands;

    private const HIERARCHY = [
        'base.zep'    => <<<'ZEP'
            namespace Stub;

            class Base
            {
                protected static sp = 1;

                private static spp = 1;

                protected function prot() -> int
                {
                    return 1;
                }

                private function priv() -> int
                {
                    return 1;
                }

                protected static function sprot() -> int
                {
                    return 1;
                }

                private static function spriv() -> int
                {
                    return 1;
                }
            }
            ZEP,
        'mid.zep'     => "namespace Stub;\n\nclass Mid extends Base\n{\n}\n",
        'leaf.zep'    => "namespace Stub;\n\nclass Leaf extends Mid\n{\n}\n",
        'deep.zep'    => "namespace Stub;\n\nclass Deep extends Leaf\n{\n}\n",
        'sibling.zep' => <<<'ZEP'
            namespace Stub;

            class Sibling extends Base
            {
                protected static sp = 2;

                protected function prot() -> int
                {
                    return 2;
                }
            }
            ZEP,
        'magic.zep'   => <<<'ZEP'
            namespace Stub;

            class Magic extends Leaf
            {
                public function __call(string name, array args)
                {
                    return name;
                }
            }
            ZEP,
        'protctor.zep' => <<<'ZEP'
            namespace Stub;

            class ProtCtor
            {
                protected function __construct()
                {
                }
            }
            ZEP,
    ];

    private string $projectDir = '';

    public static function accessibleProvider(): array
    {
        return [
            'protected method of a grandparent' => [[
                'probe.zep' => self::probe('extends Leaf', 'public function run(<Leaf> o) { return o->prot(); }'),
            ]],
            'protected method declared three levels up' => [[
                'probe.zep' => self::probe('extends Deep', 'public function run(<Deep> o) { return o->prot(); }'),
            ]],
            'protected method of the scope on a grandchild receiver' => [[
                'probe.zep'     => self::probe(
                    '',
                    'protected function own() { return 1; } public function run(<ProbeLeaf> o) { return o->own(); }'
                ),
                'probemid.zep'  => "namespace Stub;\n\nclass ProbeMid extends Probe\n{\n}\n",
                'probeleaf.zep' => "namespace Stub;\n\nclass ProbeLeaf extends ProbeMid\n{\n}\n",
            ]],
            'protected method of a sibling through the common root' => [[
                'probe.zep' => self::probe('extends Leaf', 'public function run(<Sibling> o) { return o->prot(); }'),
            ]],
            'private method of the scope on a subclass receiver' => [[
                'probe.zep'      => self::probe(
                    '',
                    'private function own() { return 1; } public function run(<ProbeChild> o) { return o->own(); }'
                ),
                'probechild.zep' => "namespace Stub;\n\nclass ProbeChild extends Probe\n{\n}\n",
            ]],
            'closure that does not use this, in a subclass' => [[
                'probe.zep' => self::probe(
                    'extends Leaf',
                    'public function run() { return function (<Leaf> o) { return o->prot(); }; }'
                ),
            ]],
            'inaccessible method of a receiver with __call' => [[
                'probe.zep' => self::probe('', 'public function run(<Magic> o) { return o->prot(); }'),
            ]],
            'private method of a parent through this, with __call' => [[
                'probe.zep' => self::probe(
                    'extends Base',
                    'public function __call(string name, array args) { return name; } '
                    . 'public function run() { return this->priv(); }'
                ),
            ]],
            'protected static method of a grandparent' => [[
                'probe.zep' => self::probe('extends Leaf', 'public function run() { return Base::sprot(); }'),
            ]],
            'protected static property of a grandparent' => [[
                'probe.zep' => self::probe('extends Leaf', 'public function run() { return Base::sp; }'),
            ]],
            'protected static property assigned from a grandchild' => [[
                'probe.zep' => self::probe('extends Leaf', 'public function run() { let Base::sp = 5; }'),
            ]],
            'protected static property redeclared by a sibling' => [[
                'probe.zep' => self::probe('extends Base', 'public function run() { return Sibling::sp; }'),
            ]],
            'private static property through a subclass name' => [[
                'probe.zep'      => self::probe(
                    '',
                    'private static x = 1; public function run() { return ProbeChild::x; }'
                ),
                'probechild.zep' => "namespace Stub;\n\nclass ProbeChild extends Probe\n{\n}\n",
            ]],
            'private static property appended through a subclass name' => [[
                'probe.zep'      => self::probe(
                    '',
                    'private static x = []; public function run() { let ProbeChild::x[] = 1; }'
                ),
                'probechild.zep' => "namespace Stub;\n\nclass ProbeChild extends Probe\n{\n}\n",
            ]],
            'private static property assigned through a subclass name' => [[
                'probe.zep'      => self::probe(
                    '',
                    'private static x = 1; public function run() { let ProbeChild::x = 2; }'
                ),
                'probechild.zep' => "namespace Stub;\n\nclass ProbeChild extends Probe\n{\n}\n",
            ]],
            'protected constructor from a subclass' => [[
                'probe.zep' => self::probe('extends ProtCtor', 'public function run() { return new ProtCtor(); }'),
            ]],
        ];
    }

    /**
     * @dataProvider accessibleProvider
     */
    public function testAccessibleMemberCompiles(array $files): void
    {
        $result = $this->generate(self::HIERARCHY + $files);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
    }

    public static function inaccessibleProvider(): array
    {
        return [
            'protected method on an unrelated receiver' => [
                ['probe.zep' => self::probe('', 'public function run(<Base> o) { return o->prot(); }')],
                "Cannot call protected method 'prot' out of its scope",
            ],
            'protected method inherited by an unrelated receiver' => [
                ['probe.zep' => self::probe('', 'public function run(<Mid> o) { return o->prot(); }')],
                "Cannot call protected method 'prot' out of its scope",
            ],
            'private method on an unrelated receiver' => [
                ['probe.zep' => self::probe('', 'public function run(<Base> o) { return o->priv(); }')],
                "Cannot call private method 'priv' out of its scope",
            ],
            'private method of a parent on a subclass receiver' => [
                ['probe.zep' => self::probe('extends Base', 'public function run(<Leaf> o) { return o->priv(); }')],
                "Cannot call private method 'priv' out of its scope",
            ],
            'private method of a parent through this' => [
                ['probe.zep' => self::probe('extends Base', 'public function run() { return this->priv(); }')],
                "Cannot call private method 'priv' out of its scope",
            ],
            'protected method in a closure of an unrelated class' => [
                ['probe.zep' => self::probe('', 'public function run() { return function (<Base> o) { return o->prot(); }; }')],
                "Cannot call protected method 'prot' out of its scope",
            ],
            'protected static method from an unrelated class' => [
                ['probe.zep' => self::probe('', 'public function run() { return Base::sprot(); }')],
                "Cannot call protected method 'sprot' out of its scope",
            ],
            'private static method from an unrelated class' => [
                ['probe.zep' => self::probe('', 'public function run() { return Base::spriv(); }')],
                "Cannot call private method 'spriv' out of its scope",
            ],
            'protected static property from an unrelated class' => [
                ['probe.zep' => self::probe('', 'public function run() { return Base::sp; }')],
                "Cannot access protected static property 'Stub\\Base::sp' out of its declaring context",
            ],
            'protected static property assigned from an unrelated class' => [
                ['probe.zep' => self::probe('', 'public function run() { let Base::sp = 5; }')],
                "Cannot access protected static property 'Stub\\Base::sp' out of its declaring context",
            ],
            'private static property of a parent through self' => [
                ['probe.zep' => self::probe('extends Base', 'public function run() { return self::spp; }')],
                "Cannot access private static property 'Stub\\Probe::spp' out of its declaring context",
            ],
            'protected constructor from an unrelated class' => [
                ['probe.zep' => self::probe('', 'public function run() { return new ProtCtor(); }')],
                "Cannot call protected method '__construct' out of its scope",
            ],
        ];
    }

    /**
     * @dataProvider inaccessibleProvider
     */
    public function testInaccessibleMemberIsACompileError(array $files, string $expected): void
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

        $this->projectDir = sys_get_temp_dir() . '/zephir-issue2734-' . bin2hex(random_bytes(6));
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
