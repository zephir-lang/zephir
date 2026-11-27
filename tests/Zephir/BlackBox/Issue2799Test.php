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
 * A literal array property default is declared on the class, so creating an
 * object runs no code for it. Only an array that reads a constant still needs
 * the object initializer, and a static array keeps its own.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2799
 */
final class Issue2799Test extends TestCase
{
    use GeneratesStubProject;

    protected function setUp(): void
    {
        $this->setUpStubProject();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testLiteralArraysAreDeclaredOnTheClass(): void
    {
        $generated = $this->generateOk('probe.zep', <<<'ZEP'
            namespace Stub;

            class Probe
            {
                public a = [];

                protected b = [1, "k": [2, []], "s": "str", "n": null];
            }
            ZEP);

        $this->assertStringNotContainsString('zephir_init_properties_', $generated);
        $this->assertStringNotContainsString('create_object', $generated);
        $this->assertStringContainsString('zephir_declare_property_array(stub_probe_ce, SL("a"), ', $generated);
        $this->assertStringContainsString('zephir_declare_property_array(stub_probe_ce, SL("b"), ', $generated);
    }

    public function testSubclassRedeclaringALiteralArrayNeedsNoInitializer(): void
    {
        $result = $this->generate([
            'base.zep'  => "namespace Stub;\n\nclass Base\n{\n    protected b = [1];\n}",
            'child.zep' => "namespace Stub;\n\nclass Child extends Base\n{\n    protected b = [2];\n\n    public c = [];\n}",
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);
        $generated = (string) file_get_contents($this->projectDir . '/ext/stub/child.zep.c');
        $this->assertStringNotContainsString('zephir_init_properties_', $generated);
        $this->assertStringContainsString('zephir_declare_property_array(stub_child_ce, SL("b"), ', $generated);
    }

    public function testArrayReadingAConstantKeepsTheInitializer(): void
    {
        $generated = $this->generateOk('probe.zep', <<<'ZEP'
            namespace Stub;

            class Probe
            {
                const K = "k";

                public a = [];

                protected c = [PHP_INT_SIZE, self::K];
            }
            ZEP);

        $this->assertStringContainsString('stub_probe_ce->create_object = zephir_init_properties_Stub_Probe;', $generated);
        $this->assertStringContainsString('zend_declare_property_null(stub_probe_ce, SL("c"), ', $generated);
        $this->assertStringContainsString('zephir_declare_property_array(stub_probe_ce, SL("a"), ', $generated);
        $this->assertStringNotContainsString('ZEND_STRL("a")', $generated);
    }

    public function testStaticArrayKeepsItsInitializer(): void
    {
        $generated = $this->generateOk('probe.zep', <<<'ZEP'
            namespace Stub;

            class Probe
            {
                protected static s = [1];
            }
            ZEP);

        $this->assertStringContainsString('zephir_init_static_properties_Stub_Probe', $generated);
        $this->assertStringNotContainsString('create_object', $generated);
    }
}
