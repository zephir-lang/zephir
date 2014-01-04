<?php

/**
 * Class Memcache
 * @link http://se2.php.net/manual/ru/class.memcache.php
 */
class Memcache
{
	public function connect($host, $port, $timeout)
	{

	}

	public function pconnect($host, $port)
	{

	}

	public function addserver($host, $port = 11211, $persistent = null, $weight = null, $timeout = null, $retry_interval = null, $status = null, callable $failure_callback = null, $timeoutms = null) {}

	public function setserverparams() {}

	public function setfailurecallback() {}

	public function getserverstatus() {}

	public function findserver() {}

	public function getversion() {}

	public function add($key, $var, $flag, $expire ) {}

	public function set() {}

	public function replace() {}

	public function cas() {}

	public function append() {}

	public function prepend() {}

	public function get($key, &$flags = null ) {}

	public function delete($key, $timeout = 0) {}

	public function getstats() {}

	public function getExtendedStats($type, $slabid, $limit=100) {}

	public function setcompressthreshold() {}

	public function increment() {}

	public function decrement() {}

	public function close() {}

	public function flush() {}
}