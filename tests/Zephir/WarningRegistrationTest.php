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

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use ReflectionClass;
use Zephir\Config;

/**
 * A warning is printed only when its key is registered: the formatter reads
 * `warnings.<key>` and a key nobody registered reads back as null, which is
 * falsy, which means "off". Six warnings shipped for years that no setting
 * could ever turn on, and nothing said so.
 *
 * These tests close the loop by going the other way: they read the keys the
 * compiler really emits out of the source, and the keys `config.json` really
 * carries out of the file, and insist all three lists agree.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2727
 */
final class WarningRegistrationTest extends TestCase
{
    private string $pwd;

    protected function setUp(): void
    {
        $this->pwd = getcwd();
    }

    protected function tearDown(): void
    {
        if (getcwd() !== $this->pwd) {
            chdir($this->pwd);
        }
    }

    public function testEveryEmittedWarningKeyIsRegistered(): void
    {
        $known = (new Config())->getKnownWarnings();
        $emitted = $this->emittedWarningKeys();

        $this->assertNotEmpty($emitted, 'The source scan found no warning keys at all');

        $unregistered = array_values(array_diff(array_keys($emitted), $known));

        $this->assertSame(
            [],
            $unregistered,
            sprintf(
                "These warning keys are emitted but not registered, so they can never print:\n%s",
                implode("\n", array_map(
                    fn (string $key): string => sprintf('  %s (%s)', $key, $emitted[$key]),
                    $unregistered
                ))
            )
        );
    }

    /**
     * `deprecated-strict-type` was registered in the defaults and missing from
     * this repo's own `config.json`, which used to replace the whole section,
     * so the warning was dead for every build run from the repo root.
     */
    public function testConfigJsonRegistersTheSameWarningKeysAsTheDefaults(): void
    {
        $config = json_decode(file_get_contents($this->projectRoot() . '/config.json'), true);

        $this->assertArrayHasKey('warnings', $config);

        $known = (new Config())->getKnownWarnings();
        $inFile = array_keys($config['warnings']);
        sort($inFile);

        $this->assertSame($known, $inFile);
    }

    /**
     * Every registered key must have somewhere that emits it, or the key is a
     * promise the compiler does not keep.
     */
    public function testEveryRegisteredWarningKeyIsEmittedSomewhere(): void
    {
        $emitted = array_keys($this->emittedWarningKeys());

        $unused = array_values(array_diff((new Config())->getKnownWarnings(), $emitted));

        $this->assertSame(
            ['invalid-return-type'],
            $unused,
            'A registered warning key that nothing emits can never fire'
        );
    }

    /**
     * Warning keys the compiler emits, mapped to the file that emits them.
     *
     * Two shapes reach the logger: a literal first element of the context
     * array, and a property (`$warningName`, `$warningType`) whose value the
     * subclasses override. The second shape is why a plain grep for the four
     * keys in the bug report missed `non-valid-require-once`.
     *
     * @return array<string, string>
     */
    private function emittedWarningKeys(): array
    {
        $keys = [];
        $dynamic = [];

        foreach ($this->sourceFiles() as $file) {
            $source = file_get_contents($file);

            preg_match_all('/->warning\s*\((.*?)\)\s*;/s', $source, $calls);

            foreach ($calls[1] as $arguments) {
                if (preg_match('/\[\s*\'([a-z0-9][a-z0-9-]*)\'\s*,/s', $arguments, $literal)) {
                    $keys[$literal[1]] ??= $this->relative($file);
                    continue;
                }

                if (preg_match('/\[\s*\$this->(warning[A-Za-z]+)\s*,/s', $arguments, $property)) {
                    $dynamic[$property[1]][] = $this->className($source);
                }
            }
        }

        foreach ($dynamic as $property => $holders) {
            foreach ($this->classesUsing($property, $holders) as $class => $key) {
                $keys[$key] ??= $this->relative((new ReflectionClass($class))->getFileName());
            }
        }

        return $keys;
    }

    /**
     * Every value a warning-key property takes, across the classes that pass
     * it to the logger and everything deriving from them.
     *
     * @param string[] $holders
     *
     * @return array<string, string>
     */
    private function classesUsing(string $property, array $holders): array
    {
        $values = [];

        foreach ($this->sourceFiles() as $file) {
            $class = $this->className(file_get_contents($file));

            if (null === $class || !class_exists($class)) {
                continue;
            }

            $isHolder = false;
            foreach ($holders as $holder) {
                if ($class === $holder || is_subclass_of($class, $holder)) {
                    $isHolder = true;
                    break;
                }
            }

            if (!$isHolder) {
                continue;
            }

            $default = (new ReflectionClass($class))->getDefaultProperties()[$property] ?? null;

            if (is_string($default) && '' !== $default) {
                $values[$class] = $default;
            }
        }

        return $values;
    }

    private function className(string $source): ?string
    {
        if (!preg_match('/^namespace\s+([^;]+);/m', $source, $namespace)) {
            return null;
        }

        if (!preg_match('/^(?:final\s+|abstract\s+)?class\s+(\w+)/m', $source, $class)) {
            return null;
        }

        return trim($namespace[1]) . '\\' . $class[1];
    }

    /**
     * @return string[]
     */
    private function sourceFiles(): array
    {
        static $files = null;

        if (null !== $files) {
            return $files;
        }

        $files = [];
        $directory = new \RecursiveDirectoryIterator($this->projectRoot() . '/src');

        foreach (new \RecursiveIteratorIterator($directory) as $file) {
            if ($file->isFile() && 'php' === $file->getExtension()) {
                $files[] = $file->getPathname();
            }
        }

        sort($files);

        return $files;
    }

    private function projectRoot(): string
    {
        return realpath($this->pwd . '/' . \constant('ZEPHIRPATH'));
    }

    private function relative(string $file): string
    {
        return str_replace($this->projectRoot() . '/', '', $file);
    }
}
