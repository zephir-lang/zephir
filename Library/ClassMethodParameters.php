<?php

class ClassMethodParameters
{

	public function __construct(array $parameters)
	{
		$this->_parameters = $parameters;
	}

	public function getParameters()
	{
		return $this->_parameters;
	}

}