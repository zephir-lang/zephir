<?php

namespace Stubs;

use Stubs\Dependency;

/**
 * Test Class Example Description
 *
 * <code>
 * $container = new Di();
 * $config = [
 *    'param' => 42
 * ];
 *
 * $example = new Test($container, $config);
 * </code>
 */
class UseBlock
{

    const PROPERTY_EXAMPLE = 'test_property';

    /**
     * @var <Dependency>
     */
    private $_container;

    /**
     * @var array
     */
    protected $config;


    /**
     * @param \Stubs\Dependency $container
     * @param array $config
     */
    public function __construct(\Stubs\Dependency $container, array $config) {}

    /**
     * Some useful description
     *
     * @param string $key
     * @return mixed|null
     */
    public function getVar(string $key) {}

}
