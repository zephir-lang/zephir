<?php

/**
 * @link http://www.php.net/manual/en/class.mongoclient.php
 * Class MongoClient
 */
class MongoClient
{
	const VERSION = '';
	const DEFAULT_HOST = "localhost" ;
	const DEFAULT_PORT = 27017 ;
	const RP_PRIMARY = "primary" ;
	const RP_PRIMARY_PREFERRED = "primaryPreferred" ;
	const RP_SECONDARY = "secondary" ;
	const RP_SECONDARY_PREFERRED = "secondaryPreferred" ;
	const RP_NEAREST = "nearest" ;

	/**
	 * @link http://www.php.net/manual/en/mongoclient.selectdb.php
	 * @param string $name
	 * @return MongoDB
	 */
	public function selectDB($name)
	{
		return new MongoDB($this, $name);
	}
}

class MongoDate
{

}

class MongoDB
{
	const PROFILING_OFF = 0 ;
	const PROFILING_SLOW = 1 ;
	const PROFILING_ON = 2 ;

	/**
	 * @link http://www.php.net/manual/en/mongodb.construct.php
	 * @param MongoClient $conn
	 * @param string $name
	 */
	public function __construct(MongoClient $conn , $name) {}
}

class Mongo
	extends MongoClient
{

}

class MongoId
{

}

class MongoRegex
{

}