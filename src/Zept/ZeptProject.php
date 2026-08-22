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

namespace Zephir\Zept;

/**
 * Translates a parsed {@see ZeptFile} into the on-disk project layout Zephir
 * expects: an extension namespace and one `.zep` source per `--FILE--`, keyed
 * by the path Zephir derives from the fully-qualified class name.
 *
 * Mirrors Zephir's own convention (see the repo's `config.json` + `stub/`):
 * `namespace Stub; class Foo` lives at `stub/foo.zep`, and the extension
 * namespace is the lower-cased first namespace segment.
 *
 * This class is pure (no I/O), which keeps the path/namespace logic unit
 * testable without compiling anything.
 */
final class ZeptProject
{
    private const NAMESPACE_RE = '/^\s*namespace\s+([A-Za-z_][A-Za-z0-9_]*(?:\\\\[A-Za-z_][A-Za-z0-9_]*)*)\s*;/m';

    /**
     * A declaration has to begin a statement, so it is preceded by a line
     * start, a `;` or a `}` -- never by prose. `class`, `interface` and `trait`
     * are ordinary English words, and this pattern is matched against the whole
     * `--FILE--` body, comments included, taking the first hit. Without the
     * prefix, "a private method of the class being compiled" in a docblock won
     * and derived `stub/being.zep`; the case then failed deep in the compiler
     * with `Unexpected class name 'Stub\Foo' in file: 'stub/being.zep'`, which
     * says nothing about the comment that caused it.
     */
    private const TYPE_RE = '/(?:^|[;}])\s*(?:final\s+|abstract\s+)*(?:class|interface|trait)\s+([A-Za-z_][A-Za-z0-9_]*)/m';

    public function __construct(private ZeptFile $zept)
    {
    }

    /**
     * The extension namespace: the lower-cased first segment of the namespace
     * shared by every `--FILE--`.
     */
    public function namespace(): string
    {
        $extensionNs = null;

        foreach ($this->zept->files as $source) {
            $first = strtolower(explode('\\', $this->namespaceOf($source))[0]);
            if ($extensionNs === null) {
                $extensionNs = $first;
            } elseif ($extensionNs !== $first) {
                throw ZeptParseException::in(
                    $this->zept->test,
                    'all --FILE-- sections must share the same extension namespace'
                );
            }
        }

        return (string) $extensionNs;
    }

    /**
     * @return array<string, string> Relative `.zep` path => Zephir source.
     */
    public function sources(): array
    {
        $this->namespace(); // validate namespace agreement up front

        $sources = [];
        foreach ($this->zept->files as $source) {
            $fqcn = $this->namespaceOf($source) . '\\' . $this->typeNameOf($source);
            $path = strtolower(str_replace('\\', '/', $fqcn)) . '.zep';
            $sources[$path] = $source;
        }

        return $sources;
    }

    private function namespaceOf(string $source): string
    {
        if (preg_match(self::NAMESPACE_RE, $source, $m) !== 1) {
            throw ZeptParseException::in(
                $this->zept->test,
                'every --FILE-- must declare a namespace'
            );
        }

        return $m[1];
    }

    private function typeNameOf(string $source): string
    {
        if (preg_match(self::TYPE_RE, $source, $m) !== 1) {
            throw ZeptParseException::in(
                $this->zept->test,
                'every --FILE-- must declare a class, interface or trait'
            );
        }

        return $m[1];
    }
}
