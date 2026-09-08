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

namespace Zephir\Test\Passes;

use PHPUnit\Framework\TestCase;
use Zephir\Parser\Parser;
use Zephir\Passes\DefiniteAssignmentPass;
use Zephir\StatementsBlock;

/**
 * A local the user declared without a value keeps the IS_UNDEF its declaration
 * left behind until something writes to it, and PHP evaluates a read of an
 * unset variable as null. This pass decides which declarations have to be
 * registered with the memory frame up front so that read produces null.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2679
 */
final class DefiniteAssignmentPassTest extends TestCase
{
    private string $tempFile;

    protected function setUp(): void
    {
        $this->tempFile = sys_get_temp_dir() . '/zephir_definite_assignment_' . uniqid('', true) . '.zep';
    }

    protected function tearDown(): void
    {
        if (is_file($this->tempFile)) {
            unlink($this->tempFile);
        }
    }

    public function testAConditionalWriteIsNotEnough(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            if a { let x = 1; }
            return x;
        ');
    }

    public function testAnUnconditionalWriteIsEnough(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            let x = 1;
            return x;
        ');
    }

    public function testADeclarationWithAValueIsNotACandidate(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x = 1;
            if a { let x = 2; }
            return x;
        ');
    }

    public function testBothArmsOfAnIfElseCount(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            if a { let x = 1; } else { let x = 2; }
            return x;
        ');
    }

    public function testAnElseIfChainWithAFinalElseCounts(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            if a { let x = 1; } elseif b { let x = 2; } else { let x = 3; }
            return x;
        ');
    }

    public function testAnElseIfChainWithoutAFinalElseIsNotEnough(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            if a { let x = 1; } elseif b { let x = 2; }
            return x;
        ');
    }

    public function testOnlyOneArmOfAnIfElseIsNotEnough(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            if a { let x = 1; } else { let b = false; }
            return x;
        ');
    }

    /**
     * The compiler's own `conditional-initialization` warning misses this
     * shape because it only fires on a single init branch.
     */
    public function testTwoSeparateConditionalWritesAreStillNotEnough(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            if a { let x = 1; }
            if b { let x = 2; }
            return x;
        ');
    }

    public function testAnUnconditionalWriteAfterAConditionalOneIsEnough(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            if a { let x = 1; }
            let x = 2;
            return x;
        ');
    }

    public function testAReadInsideTheBranchThatWroteIsFine(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            if a { let x = 1; return x; }
            return 0;
        ');
    }

    public function testALoopBodyMayNotRun(): void
    {
        $this->assertRequiresNull('x', '
            var x, item;
            for item in data { let x = item; }
            return x;
        ');
    }

    public function testAWhileBodyMayNotRun(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            while a { let x = 1; }
            return x;
        ');
    }

    public function testADoWhileBodyAlwaysRuns(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            do { let x = 1; } while a;
            return x;
        ');
    }

    public function testALoopVariableIsAssignedInsideTheBody(): void
    {
        $this->assertDoesNotRequireNull('item', '
            var x, item;
            for item in data { let x = item; }
            return x;
        ');
    }

    public function testASwitchWithoutADefaultLeavesAGap(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            switch a {
                case true:
                    let x = 1;
                    break;
            }
            return x;
        ');
    }

    public function testASwitchWithADefaultCoversEveryValue(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            switch a {
                case true:
                    let x = 1;
                    break;
                default:
                    let x = 2;
                    break;
            }
            return x;
        ');
    }

    public function testASwitchWithADefaultThatSkipsTheWriteStillLeavesAGap(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            switch a {
                case true:
                    let x = 1;
                    break;
                default:
                    let b = false;
                    break;
            }
            return x;
        ');
    }

    /**
     * An exception can leave the try block before the assignment runs.
     */
    public function testATryBlockAssignmentIsNotCertain(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            try { let x = 1; } catch \Exception, e { let b = false; }
            return x;
        ');
    }

    /**
     * Control only gets past the try/catch two ways: the try ran to the end,
     * or a catch did. A catch that always throws is not one of them, so the
     * try's assignment is the only way out and therefore certain.
     */
    public function testATryWhoseCatchAlwaysThrowsKeepsTheTryAssignment(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x, e;
            try { let x = 1; } catch \\Exception, e { throw new \\RuntimeException("no"); }
            return x;
        ');
    }

    public function testTheCatchVariableIsAssignedInsideTheCatch(): void
    {
        $this->assertDoesNotRequireNull('e', '
            var x;
            try { let x = 1; } catch \Exception, e { return e; }
            return x;
        ');
    }

    public function testAReadBeforeTheOnlyWriteCounts(): void
    {
        $this->assertRequiresNull('x', '
            var x, t;
            let t = typeof x;
            let x = 1;
            return t;
        ');
    }

    /**
     * `fetch` writes its target on both paths, the value on a hit and
     * ZVAL_NULL on a miss, so the target is an unconditional write. It also
     * must not be registered with the memory frame: a read-only fetch borrows
     * its value without taking a reference.
     */
    public function testAFetchInAConditionAssignsItsTargetEitherWay(): void
    {
        $this->assertDoesNotRequireNull('v', '
            var v;
            if fetch v, data["k"] { let b = true; }
            return v;
        ');
    }

    public function testAStandaloneFetchAssignsItsTarget(): void
    {
        $this->assertDoesNotRequireNull('v', '
            var v;
            fetch v, data["k"];
            return v;
        ');
    }

    public function testAnUpdateThroughTheVariableReadsItFirst(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            let x[] = 1;
            return x;
        ');
    }

    public function testACompoundAssignmentReadsTheVariableFirst(): void
    {
        $this->assertRequiresNull('x', '
            var x;
            let x += 1;
            return x;
        ');
    }

    public function testADeclaredLocalThatIsNeverReadNeedsNothing(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            if a { let x = 1; }
            return 0;
        ');
    }

    /**
     * A closure body is a scope of its own, so its statements say nothing
     * about the enclosing method's locals.
     */
    public function testAClosureBodyIsNotWalked(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x, fnc;
            let fnc = function() { var x; return x; };
            let x = 1;
            return x;
        ');
    }

    /**
     * The `use (...)` clause does belong to this scope: the value is captured
     * where the closure is created.
     */
    public function testACaptureIsAReadAtTheClosureCreation(): void
    {
        $this->assertRequiresNull('x', '
            var x, fnc;
            if a { let x = 1; }
            let fnc = function() use (x) { return x; };
            return fnc();
        ');
    }

    /**
     * A property name is itself a `variable` node in the AST, so a generic
     * walk of `this->x` reads a local named "x" that is not there.
     */
    public function testAPropertyNameIsNotAReadOfALocalWithThatName(): void
    {
        $this->assertDoesNotRequireNull('image', '
            var image;
            let image = this->image;
            return image;
        ');
    }

    public function testAStaticPropertyNameIsNotAReadOfALocalWithThatName(): void
    {
        $this->assertDoesNotRequireNull('someNull', '
            var someNull;
            let someNull = self::someNull;
            return someNull;
        ');
    }

    /**
     * `this->{name}` really does read a local, unlike the two above.
     */
    public function testADynamicPropertyNameIsARead(): void
    {
        $this->assertRequiresNull('name', '
            var name, out;
            if a { let name = "p"; }
            let out = this->{name};
            return out;
        ');
    }

    public function testAnArrayIndexIsARead(): void
    {
        $this->assertRequiresNull('k', '
            var k, out;
            if a { let k = "p"; }
            let out = data[k];
            return out;
        ');
    }

    /**
     * Empty clauses fall through into the next one, exactly as in PHP, so the
     * write in the clause they land on covers them.
     */
    public function testFallingThroughClausesShareTheClauseTheyLandOn(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            switch a {
                case 1:
                case 2:
                    let x = "aaa";
                    break;
                default:
                    let x = "bbb";
                    break;
            }
            return x;
        ');
    }

    /**
     * A local nothing ever writes to is #2654's case, not this one: the
     * declaration itself starts it at IS_NULL, which costs nothing, whereas
     * registering it with the memory frame costs a ZEPHIR_INIT_VAR.
     */
    public function testALocalNothingEverWritesToIsLeftToTheDeclaration(): void
    {
        $this->assertDoesNotRequireNull('x', '
            var x;
            return x;
        ');
    }

    private function analyse(string $body): DefiniteAssignmentPass
    {
        $source = "namespace Test;\n\nclass Probe\n{\n"
            . "    public function run(bool a, bool b, array data)\n    {\n"
            . $body
            . "\n    }\n}\n";

        file_put_contents($this->tempFile, $source);

        $ir = (new Parser())->parse($this->tempFile);

        $statements = null;
        foreach ($ir as $node) {
            if ('class' === $node['type']) {
                $statements = $node['definition']['methods'][0]['statements'];
                break;
            }
        }

        $this->assertNotNull($statements, 'The probe did not parse into a method body.');

        $pass = new DefiniteAssignmentPass();
        $pass->pass(new StatementsBlock($statements));

        return $pass;
    }

    private function assertDoesNotRequireNull(string $name, string $body): void
    {
        $this->assertFalse(
            $this->analyse($body)->requiresNullInitialization($name),
            sprintf('"%s" is definitely assigned at every read, so it needs no initialization.', $name)
        );
    }

    private function assertRequiresNull(string $name, string $body): void
    {
        $this->assertTrue(
            $this->analyse($body)->requiresNullInitialization($name),
            sprintf('"%s" is read where nothing has assigned it, so it has to start at null.', $name)
        );
    }
}
