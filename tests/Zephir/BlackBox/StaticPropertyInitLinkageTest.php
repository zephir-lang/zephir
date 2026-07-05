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

namespace Zephir\Test\BlackBox;

use PHPUnit\Framework\TestCase;

/**
 * A static property initialized with an array literal makes Zephir emit a
 * `zephir_init_static_properties_<Class>()` helper. Its declaration must NOT be
 * emitted into the per-class header: a single-file (concatenated) build statifies
 * the `.c` definition but leaves header prototypes untouched, so a header prototype
 * yields "static declaration follows non-static declaration". The declaration is
 * instead emitted at file scope in the project `.c`, next to the RINIT call, so the
 * definition and its declaration share one translation unit. Regression test for #2601.
 */
final class StaticPropertyInitLinkageTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-2601-' . bin2hex(random_bytes(6));
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

    public function testStaticArrayInitializerIsDeclaredInProjectFileNotHeader(): void
    {
        file_put_contents($this->projectDir . '/stub/staticprop.zep', <<<'ZEP'
namespace Stub;

class StaticProp
{
    public static someEmptyArray = [];
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);
        $this->assertSame(0, $result['exitCode'], $result['stdout'] . $result['stderr']);

        $header  = (string) file_get_contents($this->projectDir . '/ext/stub/staticprop.zep.h');
        $source  = (string) file_get_contents($this->projectDir . '/ext/stub/staticprop.zep.c');
        $project = (string) file_get_contents($this->projectDir . '/ext/stub.c');

        // The definition still lives in the class .c.
        $this->assertMatchesRegularExpression(
            '/^void zephir_init_static_properties_Stub_StaticProp\(\)$/m',
            $source,
            'the static-initializer definition must remain in the class .zep.c',
        );

        // Its prototype must NOT be emitted in the per-class header — that is the
        // non-static declaration a single-file build cannot statify.
        $this->assertDoesNotMatchRegularExpression(
            '/^(?:void|int) zephir_init_static_properties_Stub_StaticProp\(\);/m',
            $header,
            'the static-initializer prototype must not leak into the per-class header',
        );

        // It is instead forward-declared at file scope in the project .c ...
        $this->assertMatchesRegularExpression(
            '/^void zephir_init_static_properties_Stub_StaticProp\(\);$/m',
            $project,
            'the static-initializer must be forward-declared at file scope in the project .c',
        );

        // ... before PHP_RINIT_FUNCTION, and the RINIT call is still emitted.
        $declPos  = strpos($project, "\nvoid zephir_init_static_properties_Stub_StaticProp();");
        $rinitPos = strpos($project, 'PHP_RINIT_FUNCTION');
        $this->assertNotFalse($declPos, 'forward declaration not found at file scope');
        $this->assertNotFalse($rinitPos);
        $this->assertLessThan($rinitPos, $declPos, 'forward declaration must precede PHP_RINIT_FUNCTION');

        // The RINIT call to the initializer must still be emitted (tab-indented, inside RINIT).
        $this->assertMatchesRegularExpression(
            '/\tzephir_init_static_properties_Stub_StaticProp\(\);/',
            $project,
            'the RINIT call to the static initializer must still be emitted',
        );
    }
}
