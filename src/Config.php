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

use ArrayAccess;
use JsonSerializable;
use ReturnTypeWillChange;

use function array_key_exists;
use function array_keys;
use function array_merge;
use function array_values;
use function count;
use function current;
use function file_exists;
use function file_get_contents;
use function file_put_contents;
use function in_array;
use function is_array;
use function json_decode;
use function json_encode;
use function json_last_error;
use function key;
use function levenshtein;
use function preg_match;
use function sort;
use function strlen;

use const JSON_ERROR_CTRL_CHAR;
use const JSON_ERROR_DEPTH;
use const JSON_ERROR_NONE;
use const JSON_ERROR_STATE_MISMATCH;
use const JSON_ERROR_SYNTAX;
use const JSON_ERROR_UTF8;
use const JSON_PRETTY_PRINT;

/**
 * Manages compiler global configuration.
 */
class Config implements ArrayAccess, JsonSerializable
{
    /**
     * Every top-level setting Zephir reads.
     *
     * A config.json key outside this list has no effect at all, and used to
     * be swallowed without a word, which is how someone spent a build cycle
     * on an `ini` section that never existed (#2449). Only the top level is
     * checked: several sections legitimately carry keys the compiler no
     * longer reads.
     */
    private const KNOWN_SETTINGS = [
        'api',
        'author',
        'backend',
        'constants-sources',
        'description',
        'destructors',
        'extension-name',
        'external-dependencies',
        'extra',
        'extra-cflags',
        'extra-classes',
        'extra-libs',
        'extra-sources',
        'globals',
        'info',
        'initializers',
        'kernel-classes',
        'name',
        'namespace',
        'optimizations',
        'optimizer-dirs',
        'package-dependencies',
        'prototype-dir',
        'requires',
        'silent',
        'stubs',
        'verbose',
        'version',
        'warnings',
    ];

    /**
     * Sections whose file contents are merged into the defaults rather than
     * replacing them.
     *
     * A `warnings` block that omits a key used to delete it, so the key read
     * back as null, which is falsy, which the formatter took for "off". That
     * is how `deprecated-strict-type` went dead for every build run from this
     * repo's own root (#2727).
     *
     * `optimizations` is deliberately absent: cphalcon's config.json omits
     * `call-gatherer-pass`, so merging would switch it on and change the
     * generated C. That one needs its own change and its own validation.
     */
    private const MERGED_SECTIONS = ['warnings'];

    /**
     * Sections whose keys are checked against the ones Zephir reads.
     *
     * A misspelling here is inert, exactly like a misspelled top-level
     * setting, and used to be just as silent.
     */
    private const VALIDATED_SECTIONS = ['warnings', 'optimizations'];

    /**
     * Is config changed?
     */
    protected bool $changed = false;

    /**
     * The built-in configuration, captured before config.json is read.
     *
     * `populate()` overwrites whole sections of `$container`, so the container
     * cannot answer "is this a setting Zephir knows about?" once a project
     * file has been loaded. This snapshot can, and it is the single registry
     * of warning keys (#2727).
     */
    private array $defaults;

    /**
     * Settings found in config.json that Zephir does not read, mapped to the
     * closest setting it does read, or null when nothing is close.
     *
     * @var array<string, string|null>
     */
    private array $unknownSettings = [];

    /**
     * Keys found inside a validated section that Zephir does not read, per
     * section, mapped to the closest key it does read or null when there is
     * none. Fed by config.json and by the -W/-w/-f/-fno- flags alike.
     *
     * @var array<string, array<string, string|null>>
     */
    private array $unknownSectionKeys = [];

    /**
     * Default configuration for project.
     */
    private array $container = [
        'stubs'         => [
            'path'                     => 'ide/%version%/%namespace%/',
            'stubs-run-after-generate' => false,
            'banner'                   => '',
        ],
        'api'           => [
            'path'  => 'doc/%version%',
            'theme' => [
                'name'    => 'zephir',
                'options' => [
                    'github'           => null,
                    'analytics'        => null,
                    'main_color'       => '#3E6496',
                    'link_color'       => '#3E6496',
                    'link_hover_color' => '#5F9AE7',
                ],
            ],
        ],
        'warnings'      => [
            'unused-variable'                    => true,
            'unused-variable-external'           => false,
            'unassigned-variable'                => true,
            'possible-wrong-parameter'           => true,
            'possible-wrong-parameter-undefined' => false,
            'nonexistent-function'               => true,
            'nonexistent-class'                  => true,
            'non-valid-isset'                    => true,
            'non-array-update'                   => true,
            'non-valid-objectupdate'             => true,
            'non-valid-fetch'                    => true,
            'invalid-array-index'                => true,
            'non-array-append'                   => true,
            'invalid-return-type'                => true,
            'unreachable-code'                   => true,
            'nonexistent-constant'               => true,
            'not-supported-magic-constant'       => true,
            'non-valid-decrement'                => true,
            'non-valid-increment'                => true,
            'non-valid-clone'                    => true,
            'non-valid-new'                      => true,
            'non-array-access'                   => true,
            'invalid-reference'                  => true,
            'invalid-typeof-comparison'          => true,
            'conditional-initialization'         => true,
            'deprecated-strict-type'             => true,
            'non-valid-unset'                    => true,
            'non-valid-require'                  => true,
            'non-valid-require-once'             => true,
            'extra-parentheses'                  => true,
            'missing-optimizer'                  => false,
        ],
        'optimizations' => [
            'static-type-inference'             => true,
            'static-type-inference-second-pass' => true,
            'local-context-pass'                => true,
            'constant-folding'                  => true,
            'static-constant-class-folding'     => true,
            'call-gatherer-pass'                => true,
            'check-invalid-reads'               => false,
            'internal-call-transformation'      => false,
        ],
        'extra'         => [
            'indent'         => 'spaces',
            'export-classes' => false,
        ],
        /**
         * Hand-written kernel classes a project can opt into. Each one is
         * registered as <Ns>\<Name> and compiled out entirely when false, so
         * an extension never gains a class it did not ask for.
         */
        'kernel-classes' => [
            'buffer' => false,
        ],
        'namespace'     => '',
        'name'          => '',
        'description'   => '',
        'author'        => 'Phalcon Team',
        'version'       => '0.0.1',
        'verbose'       => false,
        'requires'      => [
            'extensions' => [],
        ],
    ];

    /**
     * @throws Exception
     */
    public function __construct()
    {
        $this->defaults = $this->container;

        $this->populate();
    }

    /**
     * Returns JSON representation of the project config.
     */
    public function __toString()
    {
        return (string)json_encode($this, JSON_PRETTY_PRINT);
    }

    /**
     * Writes the configuration if it has been changed.
     */
    public function dumpToFile(): void
    {
        file_put_contents('config.json', $this);
    }

    /**
     * Factory method to create a Config instance from the $_SERVER['argv'].
     */
    public static function fromServer(): self
    {
        $config = new self();

        /**
         * Change configurations flags
         */
        if ($_SERVER['argc'] >= 2) {
            $argv = $_SERVER['argv'];

            for ($i = 1; $i < $_SERVER['argc']; ++$i) {
                $parameter = $argv[$i];

                if (preg_match('/^-fno-([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->setFlag('optimizations', $matches[1], false);
                    unset($argv[$i]);
                    continue;
                }

                if (preg_match('/^-f([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->setFlag('optimizations', $matches[1], true);
                    unset($argv[$i]);
                    continue;
                }

                if (preg_match('/^-W([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->setFlag('warnings', $matches[1], false);
                    unset($argv[$i]);
                    continue;
                }

                if (preg_match('/^-w([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->setFlag('warnings', $matches[1], true);
                    unset($argv[$i]);
                    continue;
                }

                if (preg_match('/^--([a-z0-9\-]+)$/', $parameter, $matches)) {
                    // Only known options
                    if (null !== $config->get($matches[1], 'extra')) {
                        $config->set($matches[1], true, 'extra');
                        unset($argv[$i]);
                    }

                    continue;
                }

                if (preg_match('/^--([a-z0-9\-]+)=(.*)$/', $parameter, $matches)) {
                    // Only known options
                    if (null !== $config->get($matches[1], 'extra')) {
                        $config->set($matches[1], $matches[2], 'extra');
                        unset($argv[$i]);
                    }

                    continue;
                }

                switch ($parameter) {
                    case '-q':
                    case '--quiet':
                        $config->set('silent', true);
                        break;
                    case '-v':
                    case '--verbose':
                        $config->set('verbose', true);
                        break;
                    case '-V':
                        $config->set('verbose', false);
                        break;
                    default:
                        break;
                }
            }

            $_SERVER['argv'] = array_values($argv);
            $_SERVER['argc'] = count($argv);
        }

        return $config;
    }

    /**
     * Retrieves a configuration setting.
     *
     * @param mixed $key
     * @param mixed $namespace
     *
     * @return mixed|null
     */
    public function get($key, $namespace = null)
    {
        return null !== $namespace ? $this->offsetGet([$namespace => $key]) : $this->offsetGet($key);
    }

    /**
     * Settings found in config.json that Zephir does not read.
     *
     * The value is the closest known setting, or null when there is none.
     * They are still kept in the container, so a project carrying private
     * keys keeps building.
     *
     * @return array<string, string|null>
     */
    public function getUnknownSettings(): array
    {
        return $this->unknownSettings;
    }

    /**
     * Keys inside a section that Zephir does not read, per section.
     *
     * They are still kept, so a project carrying private keys keeps building,
     * and the CLI reports them rather than letting a typo pass for a setting.
     *
     * @return array<string, array<string, string|null>>
     */
    public function getUnknownSectionKeys(): array
    {
        return $this->unknownSectionKeys;
    }

    /**
     * Every warning key the compiler recognizes, sorted.
     *
     * Read from the defaults rather than the container: a project's
     * `warnings` block may add keys of its own, and those are not warnings
     * the compiler knows how to raise.
     *
     * @return string[]
     */
    public function getKnownWarnings(): array
    {
        $keys = array_keys($this->defaults['warnings']);
        sort($keys);

        return $keys;
    }

    /**
     * Whether a warning key is one the compiler recognizes.
     *
     * The formatter needs this to tell "turned off" from "never registered".
     * Reading the flag alone cannot: both read back as falsy.
     */
    public function isKnownWarning(string $key): bool
    {
        return array_key_exists($key, $this->defaults['warnings']);
    }

    /**
     * Specify data which should be serialized to JSON.
     */
    public function jsonSerialize(): array
    {
        return $this->container;
    }

    /**
     * Allows to check whether a $key is defined.
     *
     * @param mixed $offset
     *
     * @return bool
     */
    public function offsetExists($offset): bool
    {
        return isset($this->container[$offset]) || array_key_exists($offset, $this->container);
    }

    /**
     * Gets a $key from the internal container.
     *
     * @param mixed $offset
     *
     * @return mixed|null
     */
    #[ReturnTypeWillChange]
    public function offsetGet($offset)
    {
        if (!is_array($offset)) {
            return $this->offsetExists($offset) ? $this->container[$offset] : null;
        }

        $namespace = key($offset);
        $offset    = current($offset);

        if (!$this->offsetExists($namespace) || !is_array($this->container[$namespace])) {
            return null;
        }

        if (isset($this->container[$namespace][$offset]) || array_key_exists($offset, $this->container[$namespace])) {
            return $this->container[$namespace][$offset];
        }

        return null;
    }

    /**
     * Sets a configuration value.
     *
     * @param mixed $offset
     * @param mixed $value
     */
    #[ReturnTypeWillChange]
    public function offsetSet($offset, $value): void
    {
        if (!is_array($offset)) {
            $this->container[$offset] = $value;

            return;
        }

        $namespace = key($offset);
        $offset    = current($offset);

        if (!array_key_exists($namespace, $this->container)) {
            $this->container[$namespace] = [];
        }

        $this->container[$namespace][$offset] = $value;
    }

    /**
     * Unsets a $key from internal container.
     *
     * @param mixed $offset
     *
     * @deprecated
     *
     */
    #[ReturnTypeWillChange]
    public function offsetUnset($offset): void
    {
        unset($this->container[$offset]);
    }

    /**
     * Changes a configuration setting.
     *
     * @param mixed $key
     * @param mixed $value
     * @param mixed $namespace
     */
    public function set($key, $value, $namespace = null): void
    {
        null !== $namespace ? $this->offsetSet([$namespace => $key], $value) : $this->offsetSet($key, $value);
    }

    /**
     * Populate project configuration.
     *
     * @throws Exception
     */
    protected function populate(): void
    {
        if (!file_exists('config.json')) {
            return;
        }

        $config  = json_decode(file_get_contents('config.json'), true);
        $message = 'The config.json file is invalid';

        switch (json_last_error()) {
            case JSON_ERROR_NONE:
                foreach ($config as $key => $configSection) {
                    if (!in_array((string) $key, self::KNOWN_SETTINGS, true)) {
                        $this->unknownSettings[$key] = $this->closest((string) $key, self::KNOWN_SETTINGS);
                    }

                    if (is_array($configSection) && in_array((string) $key, self::VALIDATED_SECTIONS, true)) {
                        $this->collectUnknownSectionKeys((string) $key, $configSection);
                    }

                    if (is_array($configSection) && in_array((string) $key, self::MERGED_SECTIONS, true)) {
                        $this->container[$key] = array_merge($this->defaults[$key], $configSection);
                        continue;
                    }

                    $this->offsetSet($key, $configSection);
                }

                return;
            case JSON_ERROR_DEPTH:
                $message = "$message: Maximum stack depth exceeded";
                break;
            case JSON_ERROR_STATE_MISMATCH:
                $message = "$message: Underflow or the modes mismatch";
                break;
            case JSON_ERROR_CTRL_CHAR:
                $message = "$message: Unexpected control character found";
                break;
            case JSON_ERROR_SYNTAX:
                $message = "$message: Syntax error, malformed JSON";
                break;
            case JSON_ERROR_UTF8:
                $message = "$message: Malformed UTF-8 characters, possibly incorrectly encoded";
                break;
            default:
                break;
        }

        throw new Exception($message);
    }

    /**
     * The name closest to a misspelled one, or null when nothing is close.
     *
     * A suggestion is only useful for an actual typo. `ini` is seven edits
     * away from `globals`, and guessing there would send the reader somewhere
     * worse than the documentation.
     *
     * @param string[] $candidates
     */
    private function closest(string $key, array $candidates): ?string
    {
        $closest  = null;
        $distance = 1 + (int) (strlen($key) / 2);

        foreach ($candidates as $name) {
            $score = levenshtein($key, $name);

            if ($score < $distance) {
                $distance = $score;
                $closest  = $name;
            }
        }

        return $closest;
    }

    /**
     * Records the keys of one config.json section that Zephir does not read.
     */
    private function collectUnknownSectionKeys(string $section, array $values): void
    {
        $candidates = array_keys($this->defaults[$section]);

        foreach (array_keys($values) as $key) {
            if (array_key_exists((string) $key, $this->defaults[$section])) {
                continue;
            }

            $this->unknownSectionKeys[$section][(string) $key] = $this->closest((string) $key, $candidates);
        }
    }

    /**
     * Sets a `-W`/`-w`/`-f`/`-fno-` flag.
     *
     * An unrecognized key is reported instead of being created: the entry
     * would be one nothing ever reads, so the flag silently did nothing and
     * the reader had no way to tell (#2727).
     */
    private function setFlag(string $section, string $key, bool $value): void
    {
        if (!array_key_exists($key, $this->defaults[$section])) {
            $this->unknownSectionKeys[$section][$key] = $this->closest(
                $key,
                array_keys($this->defaults[$section])
            );

            return;
        }

        $this->set($key, $value, $section);
    }
}
