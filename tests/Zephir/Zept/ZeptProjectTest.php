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

namespace Zephir\Test\Zept;

use PHPUnit\Framework\TestCase;
use Zephir\Zept\ZeptParseException;
use Zephir\Zept\ZeptProject;

final class ZeptProjectTest extends TestCase
{
    private function zept(array $files): \Zephir\Zept\ZeptFile
    {
        return new \Zephir\Zept\ZeptFile('t', $files, 'echo "x";', 'x', false);
    }

    public function testDerivesExtensionNamespaceFromFirstSegmentLowercased(): void
    {
        $project = new ZeptProject($this->zept(['namespace Zept; class Greeter {}']));

        $this->assertSame('zept', $project->namespace());
    }

    public function testMapsClassToZephirLayoutPath(): void
    {
        $project = new ZeptProject($this->zept(['namespace Zept;' . "\n" . 'class Greeter {}']));

        $sources = $project->sources();

        $this->assertArrayHasKey('zept/greeter.zep', $sources);
        $this->assertStringContainsString('class Greeter', $sources['zept/greeter.zep']);
    }

    public function testMapsNestedNamespaceToSubdirectory(): void
    {
        $project = new ZeptProject($this->zept(['namespace Zept\\Sub; class Foo {}']));

        $sources = $project->sources();

        $this->assertArrayHasKey('zept/sub/foo.zep', $sources);
        $this->assertSame('zept', $project->namespace());
    }

    public function testSupportsInterfaceDeclarations(): void
    {
        $project = new ZeptProject($this->zept(['namespace Zept; interface Speaks {}']));

        $this->assertArrayHasKey('zept/speaks.zep', $project->sources());
    }

    public function testCollectsAllFilesSharingTheNamespace(): void
    {
        $project = new ZeptProject($this->zept([
            'namespace Zept; class First {}',
            'namespace Zept; class Second {}',
        ]));

        $sources = $project->sources();

        $this->assertCount(2, $sources);
        $this->assertArrayHasKey('zept/first.zep', $sources);
        $this->assertArrayHasKey('zept/second.zep', $sources);
    }

    public function testThrowsWhenFileHasNoNamespace(): void
    {
        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('namespace');

        (new ZeptProject($this->zept(['class Greeter {}'])))->namespace();
    }

    public function testThrowsWhenFilesDisagreeOnExtensionNamespace(): void
    {
        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('same extension namespace');

        (new ZeptProject($this->zept([
            'namespace Zept; class A {}',
            'namespace Other; class B {}',
        ])))->namespace();
    }

    public function testThrowsWhenFileHasNoTypeDeclaration(): void
    {
        $this->expectException(ZeptParseException::class);
        $this->expectExceptionMessage('class or interface');

        (new ZeptProject($this->zept(['namespace Zept;'])))->sources();
    }
}
