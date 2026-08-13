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

use Zephir\Exception\CompilerException;

use function dirname;
use function escapeshellarg;
use function is_executable;
use function is_file;
use function is_string;
use function realpath;
use function sprintf;
use function trim;

/**
 * The PHP build tools an extension is compiled against.
 *
 * By default `phpize` and `php-config` are taken from the `PATH`, which is what
 * PECL builds do. Passing `--with-php-config=PATH` selects another PHP
 * installation: `configure` is told about the given php-config and `phpize` is
 * taken from the very same directory, because phpize has its prefix baked in at
 * install time. Mixing the two would produce a Makefile with one PHP's headers
 * and another PHP's build macros.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1834
 */
final class PhpToolchain
{
    public const OPTION = '--with-php-config';

    private function __construct(
        private ?string $phpConfig,
        private string $phpize,
    ) {
    }

    /**
     * The toolchain found in the `PATH`.
     */
    public static function default(): self
    {
        return new self(null, 'phpize');
    }

    /**
     * The toolchain rooted at the given php-config.
     *
     * @param mixed $phpConfig Path to php-config; `null` selects the default
     *                         toolchain. `true` is what the CLI parser yields
     *                         for an option used without a value.
     *
     * @throws CompilerException When the option cannot be honored.
     */
    public static function fromPhpConfig(mixed $phpConfig): self
    {
        if (null === $phpConfig) {
            return self::default();
        }

        if (Os::isWindows()) {
            throw new CompilerException(sprintf(
                'The "%s" option is not supported on Windows, where the extension is built with '
                . 'configure.js. Point the %%PHP_DEVPACK%% environment variable at the PHP '
                . 'development pack instead.',
                self::OPTION,
            ));
        }

        if (!is_string($phpConfig) || '' === trim($phpConfig)) {
            throw new CompilerException(sprintf('The "%s" option requires a value.', self::OPTION));
        }

        $phpConfig = trim($phpConfig);
        $resolved  = realpath($phpConfig);

        if (false === $resolved || !is_file($resolved) || !is_executable($resolved)) {
            throw new CompilerException(sprintf(
                'php-config not found or not executable at "%s".',
                $phpConfig,
            ));
        }

        $phpize = dirname($resolved) . '/phpize';

        if (!is_file($phpize) || !is_executable($phpize)) {
            throw new CompilerException(sprintf(
                'phpize not found or not executable at "%s". It must belong to the same PHP '
                . 'installation as the given php-config, otherwise the extension is built with '
                . 'mismatched headers and build macros.',
                $phpize,
            ));
        }

        return new self($resolved, $phpize);
    }

    /**
     * The `configure` option retargeting the build, ready for interpolation.
     *
     * Empty for the default toolchain, so `configure` keeps resolving
     * php-config through the `PATH` exactly as before.
     */
    public function configureOption(): string
    {
        if (null === $this->phpConfig) {
            return '';
        }

        return ' ' . self::OPTION . '=' . escapeshellarg($this->phpConfig);
    }

    /**
     * The php-config command, ready for interpolation into a shell command.
     *
     * Falls back to the one in the `PATH` for the default toolchain, which is
     * the same php-config `configure` would pick up.
     */
    public function phpConfigCommand(): string
    {
        return null === $this->phpConfig ? 'php-config' : escapeshellarg($this->phpConfig);
    }

    /**
     * The phpize command, ready for interpolation into a shell command.
     */
    public function phpizeCommand(): string
    {
        return 'phpize' === $this->phpize ? $this->phpize : escapeshellarg($this->phpize);
    }
}
