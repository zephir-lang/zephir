<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir;

/**
 * Zephir\Config
 *
 * Manages compiler global configuration.
 *
 * @package Zephir
 */
class Config implements \ArrayAccess, \JsonSerializable
{
    /**
     * Default configuration for project
     *
     * @var array
     */
    private $container = [
        'stubs' => [
            'path' => 'ide/%version%/%namespace%/',
            'stubs-run-after-generate' => false,
        ],
        'api' => [
            'path' => 'doc/%version%',
            'theme' => [
                'name'    => 'zephir',
                'options' => [
                    'github'           => null,
                    'analytics'        => null,
                    'main_color'       => '#3E6496',
                    'link_color'       => '#3E6496',
                    'link_hover_color' => '#5F9AE7'
                ]
            ]
        ],
        'warnings' => [
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
            'conditional-initialization'         => true
        ],
        'optimizations' => [
            'static-type-inference'              => true,
            'static-type-inference-second-pass'  => true,
            'local-context-pass'                 => true,
            'constant-folding'                   => true,
            'static-constant-class-folding'      => true,
            'call-gatherer-pass'                 => true,
            'check-invalid-reads'                => false,
            'internal-call-transformation'       => false
        ],
        'namespace'   => '',
        'name'        => '',
        'description' => '',
        'author'      => '',
        'version'     => '0.0.1',
        'verbose'     => false,
        'requires'    => [
            'extensions'    => []
        ]
    ];

    /**
     * Is config changed?
     *
     * @var bool
     */
    protected $changed = false;

    /**
     * Config constructor.
     *
     * @throws Exception
     */
    public function __construct()
    {
        $this->populate();
        $this->changed = false;

        $this->registerShutdownFunction();
    }

    /**
     * Factory method to create a Config instence from the $_SERVER['argv'].
     *
     * @return Config
     */
    public static function fromServer()
    {
        $config = new self();

        /**
         * Change configurations flags
         */
        if ($_SERVER['argc'] >= 2) {
            for ($i = 2; $i < $_SERVER['argc']; $i++) {
                $parameter = $_SERVER['argv'][$i];

                if (preg_match('/^-fno-([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->set($matches[1], false, 'optimizations');
                    continue;
                }

                if (preg_match('/^-f([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->set($matches[1], true, 'optimizations');
                }

                if (preg_match('/^-W([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->set($matches[1], false, 'warnings');
                    continue;
                }

                if (preg_match('/^-w([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->set($matches[1], true, 'warnings');
                    continue;
                }

                if (preg_match('/^--([a-z0-9\-]+)$/', $parameter, $matches)) {
                    $config->set($matches[1], true, 'extra');
                    continue;
                }

                if (preg_match('/^--([a-z0-9\-]+)=(.*)$/', $parameter, $matches)) {
                    $config->set($matches[1], $matches[2], 'extra');
                    continue;
                }

                switch ($parameter) {
                    case '-w':
                        $config->set('silent', true);
                        break;

                    case '-v':
                        $config->set('verbose', true);
                        break;

                    case '-V':
                        $config->set('verbose', false);
                        break;

                    default:
                        break;
                }
            }
        }

        return $config;
    }

    /**
     * Allows to check whether an $key is defined.
     *
     * @param mixed $key
     * @return bool
     */
    public function offsetExists($key)
    {
        return isset($this->container[$key]) || array_key_exists($key, $this->container);
    }

    /**
     * Gets a $key from the internal container.
     *
     * @param mixed $key
     * @return mixed|null
     */
    public function offsetGet($key)
    {
        if (!is_array($key)) {
            return $this->offsetExists($key) ? $this->container[$key] : null;
        }

        $namespace = key($key);
        $key = current($key);

        if (!$this->offsetExists($namespace) || !is_array($this->container[$namespace])) {
            return null;
        }

        if (isset($this->container[$namespace][$key]) || array_key_exists($key, $this->container[$namespace])) {
            return $this->container[$namespace][$key];
        }

        return null;
    }

    /**
     * Sets a configuration value.
     *
     * @param mixed $key
     * @param mixed $value
     */
    public function offsetSet($key, $value)
    {
        if (!is_array($key)) {
            $this->container[$key] = $value;
            $this->changed = true;
            return;
        }

        $namespace = key($key);
        $key = current($key);

        if (!array_key_exists($namespace, $this->container)) {
            $this->container[$namespace] = [];
        }

        $this->container[$namespace][$key] = $value;
        $this->changed = true;
    }

    /**
     * Unsets a $key from internal container.
     *
     * @param mixed $key
     */
    public function offsetUnset($key)
    {
        unset($this->container[$key]);

        $this->changed = true;
    }

    /**
     * Retrieves a configuration setting.
     *
     * @param mixed $key
     * @param mixed $namespace
     * @return mixed|null
     */
    public function get($key, $namespace = null)
    {
        return $namespace !== null ? $this->offsetGet([$namespace => $key]) : $this->offsetGet($key);
    }

    /**
     * Changes a configuration setting
     *
     * @param mixed $key
     * @param mixed $value
     * @param mixed $namespace
     */
    public function set($key, $value, $namespace = null)
    {
        $namespace !== null ? $this->offsetSet([$namespace => $key], $value) : $this->offsetSet($key, $value);
    }

    /**
     * Writes the configuration if it has been changed
     */
    public function dumpToFile()
    {
        if ($this->changed && !file_exists('config.json')) {
            file_put_contents('config.json', $this);
        }
    }

    /**
     * Returns JSON representation of the project config.
     *
     * @return string
     */
    public function __toString()
    {
        return json_encode($this, JSON_PRETTY_PRINT);
    }

    /**
     * Specify data which should be serialized to JSON
     *
     * @return array
     */
    public function jsonSerialize()
    {
        return $this->container;
    }

    /**
     * Registers shutdown function.
     *
     * @return void
     */
    public function registerShutdownFunction()
    {
        register_shutdown_function([$this, 'dumpToFile']);
    }

    /**
     * Populate project configuration.
     *
     * @throws Exception
     */
    protected function populate()
    {
        if (!file_exists('config.json')) {
            return;
        }

        $config = json_decode(file_get_contents('config.json'), true);
        if (!is_array($config)) {
            throw new Exception(
                'The config.json file is not valid or there is no Zephir extension initialized in this directory.'
            );
        }

        foreach ($config as $key => $configSection) {
            $this->offsetSet($key, $configSection);
        }
    }
}
