<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team and contributors                     |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
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
class Config implements \ArrayAccess
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
    public function  offsetGet($key)
    {
        if (!is_array($key)) {
            return $this->offsetExists($key) ? $this->container[$key] : null;
        }

        $namespace = key($key);
        $key = current($key);

        if (!$this->offsetExists($namespace) || is_array($this->container[$namespace])) {
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
        if ($namespace !== null) {
            return $this->offsetGet([$namespace => $key]);
        }

        return $this->offsetGet($key);
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
        if ($namespace !== null) {
            $this->offsetSet([$namespace => $key], $value);
            return;
        }

        $this->offsetSet($key, $value);
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
        return json_encode($this->container, JSON_PRETTY_PRINT);
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

        $this->container = array_merge_recursive($this->container, $config);
    }
}
