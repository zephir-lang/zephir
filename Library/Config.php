<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2014 Zephir Team and contributors                     |
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
 * Config
 *
 * Manages compiler global configuration
 */
class Config
{
    protected $_config = array(
        'stubs' => array(
            'path' => 'ide/%version%/%namespace%/',
            'stubs-run-after-generate' => false,
        ),
        'warnings' => array(
            'unused-variable'                    => true,
            'unused-variable-external'           => false,
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
        ),
        'optimizations' => array(
            'static-type-inference'             => true,
            'static-type-inference-second-pass' => true,
            'local-context-pass'                => true,
            'constant-folding'                  => true,
            'static-constant-class-folding'     => true,
            'call-gatherer-pass'                => true,
            'check-invalid-reads'               => false
        ),
        'namespace'   => '',
        'name'        => '',
        'description' => '',
        'author'      => '',
        'version'     => '0.0.1',
        'verbose'     => false,
        'requires' => array(
            'extensions'    => array()
        )
    );

    protected $_changed = false;

    /**
     * Config constructor
     *
     */
    public function __construct()
    {
        if (file_exists('config.json')) {

            $config = json_decode(file_get_contents('config.json'), true);
            if (!is_array($config)) {
                throw new Exception("config.json is not valid or there is no Zephir extension initialized in this directory");
            }

            foreach ($config as $key => $configSection) {
                if (!is_array($configSection)) {
                    $this->_config[$key] = $configSection;
                } else {
                    foreach ($configSection as $subKey => $subValue) {
                        $this->_config[$key][$subKey] = $subValue;
                    }
                }
            }
        }
    }

    /**
     * Retrieves a configuration setting
     *
     * @param $key
     * @param null $namespace
     * @return mixed
     */
    public function get($key, $namespace = null)
    {
        if ($namespace !== null) {
            if (isset($this->_config[$namespace][$key])) {
                return $this->_config[$namespace][$key];
            } else {
                //new \Exception('Option [' . $namespace . '][' . $key . '] not exists');
            }
        } else {
            if (isset($this->_config[$key])) {
                return $this->_config[$key];
            } else {
                //new \Exception('Option [' . $key . '] not exists');
            }
        }

        return null;
    }

    /**
     * Changes a configuration setting
     *
     * @param $key
     * @param $value
     * @param null $namespace
     */
    public function set($key, $value, $namespace = null)
    {
        if ($namespace !== null) {
            $this->_config[$namespace][$key] = $value;
        } else {
            $this->_config[$key] = $value;
        }

        $this->_changed = true;
    }

    /**
     * Writes the configuration if it has been changed
     */
    public function saveOnExit()
    {
        if ($this->_changed && !file_exists('config.json')) {
            if (defined('JSON_PRETTY_PRINT')) {
                $config = json_encode($this->_config, JSON_PRETTY_PRINT);
            } else {
                $config = json_encode($this->_config);
            }
            file_put_contents('config.json', $config);
        }
    }
}
