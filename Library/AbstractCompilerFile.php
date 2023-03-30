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

namespace Zephir;

abstract class AbstractCompilerFile
{
    /**
     * @var string|null
     */
    protected ?string $compiledFile = null;

    /**
     * @var bool
     */
    protected bool $external = false;

    /**
     * Returns the path to the compiled file.
     *
     * @return string|null
     */
    public function getCompiledFile(): ?string
    {
        return $this->compiledFile;
    }

    /**
     * @return bool
     */
    public function isExternal(): bool
    {
        return $this->external;
    }

    /**
     * Sets if the class belongs to an external dependency or not.
     *
     * @param mixed $external
     */
    public function setIsExternal($external): void
    {
        $this->external = (bool) $external;
    }

    /**
     * Compiles the file.
     *
     * @param Compiler $compiler
     * @param StringsManager $stringsManager
     */
    abstract public function compile(Compiler $compiler, StringsManager $stringsManager): void;
}
