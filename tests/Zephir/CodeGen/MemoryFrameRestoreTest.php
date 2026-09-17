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

/**
 * Converting a parameter opens a Zephir memory frame, and whatever opens one
 * has to close it. The frame is opened on `getMustGrownStack()` alone, so the
 * close has to be emitted for every body that control can run off the end of -
 * including a body that holds nothing to run.
 *
 * The guard cases matter as much: a body that always leaves the method has
 * already restored the frame on its way out, and a second restore would be
 * dead code at best.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2716
 */
final class MemoryFrameRestoreTest extends TestCase
{
    use CompilesZephirSource;

    protected function setUp(): void
    {
        $this->setUpCodeGen('zephir_issue2716_test_', ['stub/issue2716']);
    }

    protected function tearDown(): void
    {
        $this->tearDownCodeGen();
    }

    /**
     * Every shape here converts a parameter, so every one of them opens a
     * frame, and none of them can leave the method any way but by running off
     * the end of its body.
     *
     * @return array<string, array{0: string}>
     */
    public static function reachesEndProvider(): array
    {
        return [
            'no body, array parameter with a default' => ['__construct'],
            'no body, string parameter'               => ['emptyWithString'],
            'comment-only body, array parameter'      => ['commentOnlyWithArray'],
            'comment-only body, string default'       => ['commentOnlyWithStringDefault'],
            'body is a try that swallows its throw'   => ['swallowedThrow'],
        ];
    }

    /**
     * @dataProvider reachesEndProvider
     */
    public function testAFrameThatIsOpenedIsClosed(string $method): void
    {
        $body = $this->bodyOf($method);

        $this->assertStringContainsString(
            'zephir_memory_grow_stack(',
            $body,
            $method . ' is expected to open a memory frame; the fixture no longer exercises the defect.'
        );
        $this->assertStringContainsString(
            'ZEPHIR_MM_RESTORE();',
            $body,
            $method . ' opens a memory frame and never closes it, so the frame, the method globals '
            . 'and the converted parameter are retained for every call.'
        );
    }

    /**
     * Nothing to convert, so nothing to open, so nothing to close.
     */
    public function testAMethodThatOpensNoFrameClosesNone(): void
    {
        $body = $this->bodyOf('noParameters');

        $this->assertStringNotContainsString('zephir_memory_grow_stack(', $body);
        $this->assertStringNotContainsString('ZEPHIR_MM_RESTORE();', $body);
    }

    /**
     * `RETURN_MM_LONG()` restores on its way out, so the end of the body is
     * unreachable and a trailing restore would be dead code.
     */
    public function testABodyThatAlwaysReturnsIsNotGivenASecondRestore(): void
    {
        $body = $this->bodyOf('returnsEarly');

        $this->assertStringContainsString('zephir_memory_grow_stack(', $body);
        $this->assertStringNotContainsString('ZEPHIR_MM_RESTORE();', $body);
    }

    /**
     * A generator creator's body is synthesized by generatorCreatorCode(),
     * which already ends in `ZEPHIR_MM_RESTORE(); return;`. Its statements are
     * detached, which makes it look body-less to everything downstream.
     */
    public function testAGeneratorCreatorKeepsExactlyOneRestore(): void
    {
        $this->assertSame(
            1,
            substr_count($this->bodyOf('producesValues'), 'ZEPHIR_MM_RESTORE();'),
            'The generator creator emits its own restore; a second one lands after its `return`.'
        );
    }

    /**
     * Declaring a return type on an empty body stays accepted: `{}` is how an
     * interface-shaped stub is written in Zephir, and stub/oo/oonativeimplements.zep
     * alone has ten of them.
     */
    public function testAnEmptyBodyWithAReturnTypeStillCompiles(): void
    {
        $this->assertStringContainsString(
            'PHP_METHOD(Stub_Issue2716_Frames, emptyWithReturnType)',
            $this->compileFixture()
        );
    }

    private function bodyOf(string $method): string
    {
        $signature = 'PHP_METHOD(Stub_Issue2716_Frames, ' . $method . ')';
        $body      = $this->methodBody($this->compileFixture(), $signature);

        $this->assertNotSame('', $body, 'Could not locate ' . $signature . ' in the generated C.');

        return $body;
    }

    private function compileFixture(): string
    {
        $zep = <<<'ZEP'
namespace Stub\Issue2716;

class Frames
{
    public function __construct(array options = [])
    {
    }

    public function emptyWithString(string name) -> void
    {
    }

    public function commentOnlyWithArray(array items) -> void
    {
        // nothing to do
    }

    public function commentOnlyWithStringDefault(string name = "x") -> void
    {
        // nothing to do
    }

    public function swallowedThrow(array items) -> void
    {
        try {
            throw new \Exception("swallowed");
        }
    }

    public function noParameters() -> void
    {
    }

    public function emptyWithReturnType() -> int
    {
    }

    public function returnsEarly(array items) -> int
    {
        return 1;
    }

    public function producesValues(array items)
    {
        yield 1;
    }
}
ZEP;

        $this->compileSource('Stub\\Issue2716\\Frames', 'stub/issue2716/frames.zep', $zep);

        return $this->generatedC('stub/issue2716/frames.zep');
    }
}
