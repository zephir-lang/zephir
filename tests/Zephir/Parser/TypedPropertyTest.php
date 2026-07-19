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
 * Parser support for typed class/trait properties (issue #2608):
 * a builtin/array/nullable/class type may precede the property name and is
 * carried on the property node as `data-type` / `nullable` / `cast`, mirroring
 * how method parameters already encode their types.
 */
final class TypedPropertyTest extends TestCase
{
    /** @return array<string, array> property nodes keyed by name */
    private function properties(string $body): array
    {
        $src = "namespace T;\nclass C {\n{$body}\n}\n";
        $ir  = (new PhpParser())->parse($src, 'test.zep');

        $this->assertNotSame('error', $ir['type'] ?? null, 'source must parse without error');

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

    public function testBuiltinScalarAndArrayTypes(): void
    {
        $props = $this->properties(
            "    public array orgs = [];\n" .
            "    protected bool valid = false;\n" .
            "    public int count = 0;\n" .
            "    public double ratio = 1.5;\n" .
            "    public string label = \"x\";"
        );

        $this->assertSame('array', $props['orgs']['data-type'] ?? null);
        $this->assertSame('bool', $props['valid']['data-type'] ?? null);
        $this->assertSame('int', $props['count']['data-type'] ?? null);
        $this->assertSame('double', $props['ratio']['data-type'] ?? null);
        $this->assertSame('string', $props['label']['data-type'] ?? null);
        $this->assertArrayNotHasKey('nullable', $props['orgs']);
    }

    public function testNullableType(): void
    {
        $props = $this->properties('    private ?string val = "";');

        $this->assertSame('string', $props['val']['data-type'] ?? null);
        $this->assertSame(1, $props['val']['nullable'] ?? null);
        $this->assertSame('string', $props['val']['default']['type'] ?? null);
    }

    public function testClassNameType(): void
    {
        $props = $this->properties('    public <Redis> conn;');

        $this->assertArrayHasKey('cast', $props['conn']);
        $this->assertSame('Redis', $props['conn']['cast']['value'] ?? null);
        $this->assertArrayNotHasKey('default', $props['conn']);
    }

    public function testNullableClassNameType(): void
    {
        $props = $this->properties('    public ?<Redis> conn = null;');

        $this->assertArrayHasKey('cast', $props['conn']);
        $this->assertSame(1, $props['conn']['nullable'] ?? null);
    }

    public function testUntypedPropertyStillHasNoDataType(): void
    {
        $props = $this->properties('    public plain = 1;');

        $this->assertArrayNotHasKey('data-type', $props['plain']);
        $this->assertArrayNotHasKey('cast', $props['plain']);
        $this->assertSame('int', $props['plain']['default']['type'] ?? null);
    }
}
