<?php

namespace Stubs;

use Fixtures\Ide_Stubs\Interfaces\DiInterfaceExample;

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
class Test
{

    const PROPERTY_EXAMPLE = 'test_property';

    /**
     * @var <DiInterfaceExample>
     */
    private $_container;

    /**
     * @var array
     */
    protected $config;


    /**
     * @param \Fixtures\Ide_Stubs\Interfaces\DiInterfaceExample $container
     * @param array $config
     */
    public function __construct(\Fixtures\Ide_Stubs\Interfaces\DiInterfaceExample $container, array $config) {}

    /**
     * Some useful description
     *
     * @param string $key
     * @return mixed|null
     */
    public function getVar(string $key) {}

}
