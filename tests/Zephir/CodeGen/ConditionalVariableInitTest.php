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

namespace Zephir\Test\CodeGen;

use PHPUnit\Framework\TestCase;
use Psr\Log\NullLogger;
use Zephir\Backend\Backend;
use Zephir\Backend\StringsManager;
use Zephir\Compiler;
use Zephir\Compiler\CompilerFileFactory;
use Zephir\Config;
use Zephir\FileSystem\HardDisk;
use Zephir\Os;
use Zephir\Parser\Manager;
use Zephir\Parser\Parser;

/**
 * A local whose only assignment sits inside a conditional has to be registered
 * with the memory frame at its declaration, so a path that skips the
 * assignment reads null the way PHP does.
 *
 * The registration is one ZEPHIR_INIT_VAR at the top of the body rather than
 * an IS_NULL in the declaration block, because IS_UNDEF at declaration is what
 * makes a first write observe the slot. With the slot already registered every
 * later write reads Variable::$variantInits and downgrades itself to
 * ZEPHIR_INIT_NVAR or ZEPHIR_OBS_NVAR, which observe only while the target is
 * still undefined, so it is registered exactly once. Registering it twice is
 * not a leak but an abort: see kernel/memory.c zephir_do_memory_observe().
 *
 * The guard tests matter as much as the positive ones. Every extra
 * registration forces a memory frame on a method that may not have needed one.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2679
 */
final class ConditionalVariableInitTest extends TestCase
{
    private Compiler $compiler;
    private CompilerFileFactory $compilerFileFactory;
    private string $originalCwd;
    private string $tempDir;

    protected function setUp(): void
    {
        if (Os::isWindows()) {
            $this->markTestSkipped('Code generation tests do not run on Windows.');
        }

        $this->originalCwd = getcwd();

        $this->tempDir = sys_get_temp_dir() . '/zephir_issue2679_test_' . uniqid('', true);
        mkdir($this->tempDir . '/ext/stub/issue2679', 0755, true);
        mkdir($this->tempDir . '/stub/issue2679', 0755, true);

        file_put_contents(
            $this->tempDir . '/config.json',
            json_encode(['namespace' => 'stub'], JSON_PRETTY_PRINT)
        );

        chdir($this->tempDir);

        $config = new Config();
        $disk   = new HardDisk($this->tempDir . '/.zephir');
        $disk->initialize();
        $disk->makeDirectory('.');
        $logger  = new NullLogger();
        $backend = new Backend($config, ZEPHIRPATH . 'kernel', ZEPHIRPATH . 'templates');

        $this->compilerFileFactory = new CompilerFileFactory($config, $disk, $logger);

        $this->compiler = new Compiler(
            $config,
            $backend,
            new Manager(new Parser()),
            $disk,
            $this->compilerFileFactory
        );
        $this->compiler->setLogger($logger);
    }

    protected function tearDown(): void
    {
        chdir($this->originalCwd);

        if (is_dir($this->tempDir)) {
            $iterator = new \RecursiveIteratorIterator(
                new \RecursiveDirectoryIterator(
                    $this->tempDir,
                    \FilesystemIterator::SKIP_DOTS
                ),
                \RecursiveIteratorIterator::CHILD_FIRST
            );
            foreach ($iterator as $file) {
                $file->isDir() ? rmdir($file->getRealPath()) : unlink($file->getRealPath());
            }
            rmdir($this->tempDir);
        }
    }

    /**
     * Each case names the local and the first branch of the method, so the
     * registration can be pinned to the declaration rather than merely being
     * present somewhere.
     *
     * @return array<string, array{0: string, 1: string, 2: string}>
     */
    public static function conditionalProvider(): array
    {
        return [
            'if without else'        => ['conditional', 'x', 'if (flag)'],
            'two separate ifs'       => ['multiBranch', 'x', 'if (a)'],
            'loop that may not run'  => ['loopOnly', 'x', 'zephir_is_iterable('],
            'switch without default' => ['switchWithoutDefault', 'x', 'if (n == 1)'],
            'assignment inside try'  => ['tryOnly', 'x', 'try_start_1'],
            'string local'           => ['stringLocal', 's', 'if (flag)'],
        ];
    }

    /**
     * The mirror image: the local is definitely assigned at every read, so its
     * registration has to stay where the assignment put it.
     *
     * @return array<string, array{0: string, 1: string, 2: string}>
     */
    public static function definitelyAssignedProvider(): array
    {
        return [
            'unconditional write after a conditional one' => [
                'conditionalThenReassign',
                'x',
                'if (flag)',
            ],
            'switch with default' => [
                'switchWithDefault',
                'x',
                'if (n == 1)',
            ],
        ];
    }

    /**
     * @dataProvider conditionalProvider
     */
    public function testConditionallyAssignedLocalIsRegisteredAtItsDeclaration(
        string $method,
        string $name,
        string $firstBranch
    ): void {
        $body         = $this->methodBody($this->compileFixture(), $method);
        $registration = 'ZEPHIR_INIT_VAR(&' . $name . ');';

        $this->assertStringContainsString(
            'ZVAL_UNDEF(&' . $name . ');',
            $body,
            sprintf(
                '%s() must keep "%s" undefined in the declaration block, which is what lets the '
                . 'registration below observe it.',
                $method,
                $name
            )
        );
        $this->assertSame(
            1,
            substr_count($body, $registration),
            sprintf(
                '%s() must register "%s" exactly once; a second observe of the same address '
                . 'aborts in a debug build.',
                $method,
                $name
            )
        );
        $this->assertLessThan(
            strpos($body, $firstBranch),
            strpos($body, $registration),
            sprintf(
                '%s() must register "%s" before "%s", or a path that skips the assignment still '
                . 'reads IS_UNDEF.',
                $method,
                $name,
                $firstBranch
            )
        );
    }

    /**
     * @dataProvider definitelyAssignedProvider
     */
    public function testDefinitelyAssignedLocalIsLeftAlone(
        string $method,
        string $name,
        string $firstBranch
    ): void {
        $body         = $this->methodBody($this->compileFixture(), $method);
        $registration = 'ZEPHIR_INIT_VAR(&' . $name . ');';

        $this->assertGreaterThan(
            strpos($body, $firstBranch),
            strpos($body, $registration),
            sprintf(
                '%s() pays for a registration of "%s" it does not need: every one of them forces '
                . 'a memory frame on a method that may not have had one.',
                $method,
                $name
            )
        );
    }

    /**
     * An if/else that assigns in both arms already has its registration
     * hoisted out of the branch by the compiler, so position says nothing
     * here. What an added registration would leave behind is the downgrade of
     * that hoisted one to its N form.
     */
    public function testExhaustiveIfElseKeepsItsHoistedRegistration(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'exhaustiveIfElse');

        $this->assertSame(1, substr_count($body, 'ZEPHIR_INIT_VAR(&x);'));
        $this->assertStringNotContainsString(
            'ZEPHIR_INIT_NVAR(&x);',
            $body,
            'A second registration was added ahead of the one the if/else already hoists.'
        );
    }

    /**
     * `fetch` writes its target on both paths, the value on a hit and
     * ZVAL_NULL on a miss, so its target is already a valid zval. Touching it
     * would also be unsafe: a read-only fetch borrows its value without taking
     * a reference, so a registered slot would free a reference this method
     * never owned.
     */
    public function testFetchTargetIsNotRegistered(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'fetchCase');

        $this->assertStringContainsString(
            'zephir_memory_observe(&v);',
            $body,
            'The fetch target must keep its single unconditional observe.'
        );
        $this->assertStringNotContainsString(
            'ZEPHIR_INIT_VAR(&v);',
            $body,
            'Registering a fetch target ahead of the fetch adds a second observe.'
        );
    }

    /**
     * A local nothing ever writes to is #2654's case: the declaration itself
     * starts it at IS_NULL, which needs no memory frame at all.
     */
    public function testNeverAssignedLocalStillStartsAsNull(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'neverAssigned');

        $this->assertStringContainsString('ZVAL_NULL(&x);', $body);
        $this->assertStringNotContainsString('ZEPHIR_INIT_VAR(&x);', $body);
    }

    /**
     * The write is conditional but the read is not a zval read: a native local
     * has no IS_UNDEF to hand to userland and is not in the memory frame.
     */
    public function testNativeLocalIsUntouched(): void
    {
        $body = $this->methodBody($this->compileFixture(), 'nativeLocal');

        $this->assertStringContainsString('zend_long i = 0;', $body);
        $this->assertStringNotContainsString('ZEPHIR_INIT_VAR(&i);', $body);
    }

    private function compileFixture(): string
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2679;

class Branch
{
    public function conditional(bool flag)
    {
        var x;

        if flag {
            let x = "set";
        }

        return x;
    }

    public function multiBranch(bool a, bool b)
    {
        var x;

        if a {
            let x = "a";
        }

        if b {
            let x = "b";
        }

        return x;
    }

    public function loopOnly(array items)
    {
        var x, item;

        for item in items {
            let x = item;
        }

        return x;
    }

    public function switchWithoutDefault(int n)
    {
        var x;

        switch n {
            case 1:
                let x = "one";
                break;
        }

        return x;
    }

    public function switchWithDefault(int n)
    {
        var x;

        switch n {
            case 1:
                let x = "one";
                break;

            default:
                let x = "other";
                break;
        }

        return x;
    }

    public function tryOnly(bool boom)
    {
        var x;

        try {
            if boom {
                throw new \RuntimeException("boom");
            }

            let x = "ok";
        } catch \RuntimeException {
            let boom = true;
        }

        return x;
    }

    public function stringLocal(bool flag)
    {
        string s;

        if flag {
            let s = "set";
        }

        return s;
    }

    public function exhaustiveIfElse(bool flag)
    {
        var x;

        if flag {
            let x = "yes";
        } else {
            let x = "no";
        }

        return x;
    }

    public function conditionalThenReassign(bool flag)
    {
        var x;

        if flag {
            let x = "first";
        }

        let x = "second";

        return x;
    }

    public function fetchCase(array data)
    {
        var v;
        bool found = false;

        if fetch v, data["k"] {
            let found = true;
        }

        return [found, v];
    }

    public function fetchGuarded(array data) -> string
    {
        var v;

        if fetch v, data["k"] {
            return (string) v;
        }

        return "missing";
    }

    public function neverAssigned()
    {
        var x;

        return x;
    }

    public function nativeLocal(bool flag) -> int
    {
        int i;

        if flag {
            let i = 7;
        }

        return i;
    }
}
ZEP;
        file_put_contents($this->tempDir . '/stub/issue2679/branch.zep', $zep);

        $compilerFile = $this->compilerFileFactory->create(
            'Stub\\Issue2679\\Branch',
            'stub/issue2679/branch.zep'
        );
        $compilerFile->preCompile($this->compiler);
        $compilerFile->compile($this->compiler, new StringsManager());

        return file_get_contents($this->tempDir . '/ext/stub/issue2679/branch.zep.c');
    }

    /**
     * Returns the C body of a single generated method.
     */
    private function methodBody(string $generated, string $method): string
    {
        $marker   = 'PHP_METHOD(Stub_Issue2679_Branch, ' . $method . ')';
        $startPos = strpos($generated, $marker);

        $this->assertNotFalse($startPos, 'Could not locate ' . $marker . ' in the generated C.');

        $endPos = strpos($generated, 'PHP_METHOD(', $startPos + strlen($marker));

        return false === $endPos
            ? substr($generated, $startPos)
            : substr($generated, $startPos, $endPos - $startPos);
    }
}
