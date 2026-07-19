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

namespace Zephir\Test\Parser;

use PHPUnit\Framework\TestCase;
use Zephir\Parser\Php\PhpParser;

/**
 * Pure-PHP parser support for union types (issue #2613): a `type | type | ...`
 * declaration on a parameter or property is carried as an ordered `data-types`
 * member list. Each member is `['data-type' => <name>]` or `['cast' => <node>]`,
 * matching the C parser byte-for-byte.
 */
final class UnionTypeTest extends TestCase
{
    private function parse(string $body): array
    {
        $ir = (new PhpParser())->parse("namespace T;\n{$body}\n", 'test.zep');
        $this->assertNotSame('error', $ir['type'] ?? null, 'source must parse without error');

        return $ir;
    }

    /** @return list<string> flattened member labels (`int`, `<Foo>`, ...) */
    private function memberLabels(array $node): array
    {
        return array_map(
            static fn (array $m): string => isset($m['cast']) ? ('<' . $m['cast']['value'] . '>') : $m['data-type'],
            $node['data-types']
        );
    }

    private function firstMethodParams(array $ir): array
    {
        foreach ($ir as $node) {
            if (($node['type'] ?? '') === 'class') {
                return $node['definition']['methods'][0]['parameters'];
            }
        }

        return [];
    }

    private function properties(array $ir): array
    {
        $props = [];
        foreach ($ir as $node) {
            if (($node['type'] ?? '') === 'class') {
                foreach ($node['definition']['properties'] ?? [] as $p) {
                    $props[$p['name']] = $p;
                }
            }
        }

        return $props;
    }

    public function testScalarUnionParameter(): void
    {
        $ir     = $this->parse("class C {\n public function f(int|float a) -> void {}\n}");
        $params = $this->firstMethodParams($ir);

        // A union parameter is bound as an internal `variable` with the members
        // carried in `data-types`; no top-level `cast` is emitted.
        $this->assertSame('variable', $params[0]['data-type']);
        $this->assertSame(0, $params[0]['mandatory']);
        $this->assertArrayNotHasKey('cast', $params[0]);
        $this->assertSame(['int', 'double'], $this->memberLabels($params[0]));
    }

    public function testClassAndNullUnionParameter(): void
    {
        $ir     = $this->parse("class C {\n public function f(string|<Foo>|null a) -> void {}\n}");
        $params = $this->firstMethodParams($ir);

        $this->assertSame('variable', $params[0]['data-type']);
        $this->assertSame(['string', '<Foo>', 'null'], $this->memberLabels($params[0]));
    }

    public function testScalarUnionProperty(): void
    {
        $ir    = $this->parse("class C {\n public int|float num = 1;\n}");
        $props = $this->properties($ir);

        $this->assertArrayNotHasKey('data-type', $props['num']);
        $this->assertArrayNotHasKey('cast', $props['num']);
        $this->assertSame(['int', 'double'], $this->memberLabels($props['num']));
    }

    public function testNullableAndClassUnionProperty(): void
    {
        $ir    = $this->parse("class C {\n public int|string|null note;\n public string|<Bar> tag;\n}");
        $props = $this->properties($ir);

        $this->assertSame(['int', 'string', 'null'], $this->memberLabels($props['note']));
        $this->assertSame(['string', '<Bar>'], $this->memberLabels($props['tag']));
    }

    public function testSingleTypeIsNotAUnion(): void
    {
        $ir    = $this->parse("class C {\n public int count = 0;\n public function f(int a) -> void {}\n}");
        $props = $this->properties($ir);

        // A lone type keeps the existing single-type shape — no `data-types`.
        $this->assertArrayNotHasKey('data-types', $props['count']);
        $this->assertSame('int', $props['count']['data-type']);
        $this->assertArrayNotHasKey('data-types', $this->firstMethodParams($ir)[0]);
    }
}
