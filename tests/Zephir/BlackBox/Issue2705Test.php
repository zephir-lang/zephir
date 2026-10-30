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
 * An offset unset on a brace property name, a static property, or below a
 * first offset reaches the container through its write slot and the
 * zephir_array_unset_path() walk, instead of a read that unset a copy. The two
 * shapes that already worked keep their own calls.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2705
 */
final class Issue2705Test extends TestCase
{
    use RunsZephirCommands;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public static function pathShapeProvider(): array
    {
        return [
            'brace literal'    => ['unset this->{"p"}["k"];', 'zephir_fetch_property_write(', 1],
            'brace name'       => ['unset this->{name}[k];', 'zephir_fetch_property_write_zval(', 1],
            'nested property'  => ['unset this->p["a"][k];', 'zephir_fetch_property_write(', 2],
            'brace nested'     => ['unset this->{name}["a"]["b"]["c"];', 'zephir_fetch_property_write_zval(', 3],
            'static'           => ['unset self::sp[k];', 'zephir_fetch_static_property_write_ce(', 1],
            'static nested'    => ['unset self::sp["a"][k];', 'zephir_fetch_static_property_write_ce(', 2],
            'local nested'     => ['unset a["a"][k];', '', 2],
        ];
    }

    /**
     * @dataProvider pathShapeProvider
     */
    public function testTheShapeWalksTheContainerItself(string $statement, string $slotFetch, int $depth): void
    {
        [$exitCode, $stderr, $generated] = $this->generateProject($statement);

        $this->assertSame(0, $exitCode, $stderr);

        $generated = $this->runMethodBody($generated);

        $this->assertMatchesRegularExpression(
            '/zephir_array_unset_path\([^,]+, ' . $depth . ', zephir_unset_offsets\);/',
            $generated
        );
        $this->assertStringContainsString('if (UNEXPECTED(EG(exception))) {', $generated);
        $this->assertStringNotContainsString('zephir_read_property', $generated);
        $this->assertStringNotContainsString('zephir_read_static_property', $generated);

        if ('' !== $slotFetch) {
            $this->assertStringContainsString($slotFetch, $generated);
        }
    }

    public static function unchangedShapeProvider(): array
    {
        return [
            'property offset' => ['unset this->p[k];', 'zephir_unset_property_array(this_ptr, ZEND_STRL("p"), k);'],
            'local offset'    => ['unset a[k];', 'zephir_array_unset(a, k, PH_SEPARATE);'],
            'property'        => ['unset a->p;', 'zephir_unset_property(a, "p");'],
            'named property'  => ['unset a->{name};', 'zephir_unset_property_zval(a, name);'],
        ];
    }

    /**
     * The forms that already reached the container keep their own call, and
     * now stop when it throws.
     *
     * @dataProvider unchangedShapeProvider
     */
    public function testAPlainUnsetKeepsItsCallAndStopsOnAThrow(string $statement, string $expected): void
    {
        [$exitCode, $stderr, $generated] = $this->generateProject($statement);

        $this->assertSame(0, $exitCode, $stderr);

        $generated = $this->runMethodBody($generated);

        $this->assertMatchesRegularExpression(
            '/' . preg_quote($expected, '/') . '\s*if \(UNEXPECTED\(EG\(exception\)\)\) \{/',
            $generated
        );
        $this->assertStringNotContainsString('zephir_array_unset_path', $generated);
    }

    /**
     * The C of `run()` alone: the property initializer reads `p` on its own.
     */
    private function runMethodBody(string $generated): string
    {
        $start = strpos($generated, 'PHP_METHOD(Unsetpath_Sample, run)');
        $this->assertNotFalse($start, 'run() was not generated.');

        $end = strpos($generated, "\n}\n", $start);

        return substr($generated, $start, $end - $start);
    }

    /**
     * @return array{0: int, 1: string, 2: string} exit code, stderr and the generated C
     */
    private function generateProject(string $statement): array
    {
        $name       = 'unsetpath';
        $cwd        = $this->outputDir();
        $projectDir = $cwd . '/' . $name;
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . $name, $cwd)['exitCode']);

        $source = <<<ZEP
            namespace Unsetpath;

            class Sample
            {
                public static sp = [];
                public p = [];

                public function run(var a, var name, var k)
                {
                    {$statement}

                    return a;
                }
            }

            ZEP;
        file_put_contents($projectDir . '/' . $name . '/sample.zep', $source);

        $result    = $this->runZephir('generate --no-ansi', $projectDir);
        $generated = $projectDir . '/ext/' . $name . '/sample.zep.c';

        return [
            $result['exitCode'],
            $result['stderr'],
            is_file($generated) ? (string) file_get_contents($generated) : '',
        ];
    }
}
