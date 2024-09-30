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
use function array_values;
use function count;
use function current;
use function file_exists;
use function file_get_contents;
use function file_put_contents;
use function is_array;
use function json_decode;
use function json_encode;
use function json_last_error;
use function key;
use function preg_match;

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
     * Is config changed?
     */
    protected bool $changed = false;

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
                    $config->set($matches[1], false, 'optimizations');
                    unset($argv[$i]);
                    continue;
                }

                if (preg_match('/^-f([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->set($matches[1], true, 'optimizations');
                    unset($argv[$i]);
                    continue;
                }

                if (preg_match('/^-W([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->set($matches[1], false, 'warnings');
                    unset($argv[$i]);
                    continue;
                }

                if (preg_match('/^-w([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->set($matches[1], true, 'warnings');
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
}
