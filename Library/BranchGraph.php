<?php

class BranchGraphNode
{
	protected $_increase = 0;

	protected $_branches = array();

	public function __construct($branch)
	{
		$this->_branch = $branch;
	}

	public function insert(BranchGraphNode $branch)
	{
		if (!in_array($branch, $this->_branches)) {
			$this->_branches[] = $branch;
		}
	}

	public function increase()
	{
		$this->_increase++;
	}

	public function show($padding=0)
	{
		echo str_repeat("    ", $padding), $this->_branch->getUniqueId(), ':' , $this->_increase;
		if (count($this->_branches)) {
			echo ':', PHP_EOL;
			foreach ($this->_branches as $node) {
				$node->show($padding + 1);
			}
		} else {
			echo PHP_EOL;
		}
	}

}

class BranchGraph
{
	protected $_root;

	protected $_branchMap;

	public function addLeaf(Branch $branch)
	{

		if (isset($this->_branchMap[$branch->getUniqueId()])) {
			$branchNode = $this->_branchMap[$branch->getUniqueId()];
		} else {
			$branchNode = new BranchGraphNode($branch);
		}
		$branchNode->increase();

		$oldBranchNode = $branch;
		$tempBranch = $branch->getParentBranch();
		while ($tempBranch) {
			if (isset($this->_branchMap[$tempBranch->getUniqueId()])) {
				$parentBranchNode = $this->_branchMap[$tempBranch->getUniqueId()];
			} else {
				$parentBranchNode = new BranchGraphNode($tempBranch);
				$this->_branchMap[$tempBranch->getUniqueId()] = $parentBranchNode;
			}
			$parentBranchNode->insert($branchNode);
			$branchNode = $parentBranchNode;
			$tempBranch = $tempBranch->getParentBranch();
			if (!$tempBranch) {
				$this->_root = $parentBranchNode;
			}
		}
	}

	public function getRoot()
	{
		return $this->_root;
	}

}