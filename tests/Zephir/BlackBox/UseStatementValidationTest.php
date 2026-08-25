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
 * Regression coverage for `use` statements referencing classes that don't
 * exist at compile time. They used to register into the alias manager
 * without any validation, then produced the cryptic runtime
 * `Fatal error: During class fetch` when the alias was actually referenced.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2435
 */
final class UseStatementValidationTest extends TestCase
{
    use RunsZephirCommands;

    private string $projectDir;

    protected function setUp(): void
    {
        $this->setUpZephirRunner();

        $this->projectDir = sys_get_temp_dir() . '/zephir-use-validation-' . bin2hex(random_bytes(6));
        $this->cleanupPath($this->projectDir);
        $this->setUpZephirProject();
    }

    protected function tearDown(): void
    {
        $this->tearDownZephirRunner();
    }

    public function testTypoUseStatementProducesCompileTimeWarning(): void
    {
        $this->writeZep('typo.zep', <<<'ZEP'
namespace Stub;

use Phalcon\Views\Exception as ViewException;

class Typo
{
    public function noop()
    {
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode'], 'Warning must not fail the build');
        $this->assertStringContainsString('Phalcon\\Views\\Exception', $result['stdout'] . $result['stderr']);
        $this->assertStringContainsString('does not exist at compile time', $result['stdout'] . $result['stderr']);
    }

    public function testValidUseStatementProducesNoWarning(): void
    {
        $this->writeZep('valid.zep', <<<'ZEP'
namespace Stub;

use stdClass;
use Exception;

class Valid
{
    public function noop()
    {
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $output = $result['stdout'] . $result['stderr'];
        $this->assertStringNotContainsString('stdClass', $output);
        $this->assertStringNotContainsString('Exception', $output);
        $this->assertStringNotContainsString('does not exist at compile time', $output);
    }

    public function testChainedStaticReturnDoesNotWarn(): void
    {
        // Regression: a method declared `-> <static>` whose result is
        // immediately chained used to surface a bogus warning
        //   `Class "Foo\static" does not exist at compile time`
        // because MethodCall.php's chained-call type tracker ran the
        // return-class-type through getFullName() and namespace-prefixed the
        // reserved `static` keyword. See #2505 / Phalcon report.
        $this->writeZep('fluent.zep', <<<'ZEP'
namespace Stub;

class Fluent
{
    public function makeStatic() -> <static>
    {
        return new static();
    }

    public function noop() -> bool
    {
        return true;
    }

    public function caller() -> bool
    {
        return this->makeStatic()->noop();
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $output = $result['stdout'] . $result['stderr'];
        $this->assertStringNotContainsString('Stub\\static', $output);
        $this->assertStringNotContainsString('Stub\\self', $output);
        $this->assertStringNotContainsString('does not exist at compile time', $output);
    }

    public function testIntraExtensionUseDoesNotWarn(): void
    {
        // Regression: file alphabetical order forces the importing file
        // (`aconsumer.zep`) to be preCompiled BEFORE the file declaring the
        // imported class (`zlater.zep`). Validation must run after every
        // file has been preCompiled, not per-file at parse time, otherwise
        // legitimate intra-extension `use` statements falsely warn.
        $this->writeZep('aconsumer.zep', <<<'ZEP'
namespace Stub;

use Stub\ZLater;

class AConsumer
{
    public function noop(<ZLater> z)
    {
    }
}
ZEP);
        $this->writeZep('zlater.zep', <<<'ZEP'
namespace Stub;

class ZLater
{
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $output = $result['stdout'] . $result['stderr'];
        $this->assertStringNotContainsString('Stub\\ZLater', $output);
        $this->assertStringNotContainsString('does not exist at compile time', $output);
    }

    public function testOptionalExtensionUseDoesNotWarn(): void
    {
        // Regression: the bundled prototypes declare the classes of extensions
        // that are not loaded in the running PHP, but they were required after
        // the `use` validation ran. An extension built without redis/memcached
        // therefore warned about classes Zephir itself provides.
        // See https://github.com/phalcon/cphalcon/issues/17517.
        $this->writeZep('optional.zep', <<<'ZEP'
namespace Stub;

use Memcached;
use Redis;
use RedisCluster;
use RedisException;

class Optional
{
    public function noop()
    {
    }
}
ZEP);

        $result = $this->runZephir('generate --no-ansi', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $output = $result['stdout'] . $result['stderr'];
        $this->assertStringNotContainsString('does not exist at compile time', $output);
    }

    public function testWnoFlagSuppressesTheWarning(): void
    {
        $this->writeZep('typo.zep', <<<'ZEP'
namespace Stub;

use Phalcon\Views\Exception as ViewException;

class Typo
{
    public function noop()
    {
    }
}
ZEP);

        // Zephir's flag convention: `-W<name>` disables the warning,
        // `-w<name>` enables it (the opposite of GCC's `-Wno-<name>`).
        $result = $this->runZephir('generate --no-ansi -Wnonexistent-class', $this->projectDir);

        $this->assertSame(0, $result['exitCode']);
        $this->assertStringNotContainsString(
            'does not exist at compile time',
            $result['stdout'] . $result['stderr']
        );
    }

    private function setUpZephirProject(): void
    {
        mkdir($this->projectDir . '/stub', 0777, true);
        file_put_contents(
            $this->projectDir . '/config.json',
            json_encode(['namespace' => 'stub', 'name' => 'stub'])
        );
    }

    private function writeZep(string $relativePath, string $content): void
    {
        $full = $this->projectDir . '/stub/' . $relativePath;
        if (!is_dir(dirname($full))) {
            mkdir(dirname($full), 0777, true);
        }
        file_put_contents($full, $content);
    }
}
