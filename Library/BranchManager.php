<?php

class BranchManager
{
	protected $_currentBranch;

	protected $_level = -1;

	protected $_uniqueId = 1;

	public function addBranch(Branch $branch)
	{
		if ($this->_currentBranch) {
			$branch->setParentBranch($this->_currentBranch);
			$this->_currentBranch = $branch;
		} else {
			$this->_currentBranch = $branch;
		}

		$branch->setUniqueId($this->_uniqueId);
		$branch->setLevel($this->_level);

		$this->_level++;
		$this->_uniqueId++;
	}

	public function getCurrentBranch()
	{
		return $this->_currentBranch;
	}

	public function removeBranch(Branch $branch)
	{
		$parentBranch = $branch->getParentBranch();
		$this->_currentBranch = $parentBranch;
		$this->_level--;
	}
}