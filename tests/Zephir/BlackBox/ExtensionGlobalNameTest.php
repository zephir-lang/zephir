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

use function file_get_contents;
use function file_put_contents;
use function json_decode;
use function json_encode;

/**
 * Every `globals` key in config.json becomes a C member verbatim, so it has to
 * be one. The check meant to enforce that was inverted: it rejected every
 * one-character name and let through `my-setting`, `1abc`, `a.b.c` and names
 * clashing with another member, all of which only failed later, in the C
 * compiler, or were silently merged.
 *
 * A bad key is the user's config, so it is reported as such rather than as an
 * internal error.
 */
final class ExtensionGlobalNameTest extends TestCase
{
    use RunsZephirCommands;

    private const PROJECT = 'globalnames';

    protected function setUp(): void
    {
        $this->setUpZephirRunner();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testShouldAcceptOneCharacterNames(): void
    {
        $result = $this->generate([
            'x'   => ['type' => 'int', 'default' => 1],
            'a.b' => ['type' => 'bool', 'default' => true],
        ]);

        $this->assertSame(0, $result['exitCode'], $result['stderr']);
        $this->assertMatchesRegularExpression('/\tzend_long x;/', $result['header']);
        $this->assertMatchesRegularExpression(
            '/typedef struct _zephir_struct_a \{\s*zend_bool b;\s*\} zephir_struct_a;/',
            $result['header'],
        );
    }

    public function testShouldRejectANameThatIsNotACIdentifier(): void
    {
        $result = $this->generate(['my-setting' => ['type' => 'int', 'default' => 1]]);

        $this->assertConfigError(
            'Extension global "my-setting" is not a valid name: "my-setting" must be a C identifier',
            $result,
        );
    }

    public function testShouldRejectAGlobalNamedLikeAStruct(): void
    {
        $result = $this->generate([
            'db'    => ['type' => 'int', 'default' => 1],
            'db.xx' => ['type' => 'int', 'default' => 1],
        ]);

        $this->assertConfigError(
            'Extension global "db" clashes with the "db.*" globals: both would be the member "db"',
            $result,
        );
    }

    public function testShouldReportAMissingDefaultAsAConfigError(): void
    {
        $result = $this->generate(['no_default' => ['type' => 'int']]);

        $this->assertConfigError('Extension global "no_default" has no default value', $result);
    }

    /**
     * @param array{exitCode: int, stderr: string} $result
     */
    private function assertConfigError(string $message, array $result): void
    {
        $this->assertNotSame(0, $result['exitCode']);
        $this->assertStringContainsString($message, $result['stderr']);
        $this->assertStringNotContainsString('Internal error', $result['stderr']);
    }

    /**
     * @return array{exitCode: int, stderr: string, header: string}
     */
    private function generate(array $globals): array
    {
        $projectDir = $this->outputDir() . '/' . self::PROJECT;
        $this->removePath($projectDir);
        $this->cleanupPath($projectDir);

        $this->assertSame(0, $this->runZephir('init ' . self::PROJECT, $this->outputDir())['exitCode']);

        $config            = json_decode((string) file_get_contents($projectDir . '/config.json'), true);
        $config['globals'] = $globals;
        file_put_contents($projectDir . '/config.json', json_encode($config));

        file_put_contents(
            $projectDir . '/' . self::PROJECT . '/holder.zep',
            "namespace Globalnames;\n\nclass Holder\n{\n}\n",
        );

        $result = $this->runZephir('generate --no-ansi', $projectDir);
        $header = $projectDir . '/ext/php_' . self::PROJECT . '.h';

        return [
            'exitCode' => $result['exitCode'],
            'stderr'   => $result['stderr'],
            'header'   => 0 === $result['exitCode'] ? (string) file_get_contents($header) : '',
        ];
    }
}
