/****************************************

	CS 300 DATA STRUCTURES - HOMEWORK III
	
	Author: Berkay Barýþ Turan
	Student ID: 28132
	Date: 01.12.2021

*****************************************/
#include "BVHTree.h"

BVHTree::BVHTree() {
	root = NULL;
}

BVHTree::~BVHTree() {
	deleteTree(root);
	root = NULL;
	map.clear();
}

void BVHTree::deleteTree(BVHTreeNode* current) {
	if(current == NULL)
		return;
	deleteTree(current->leftChild);
	deleteTree(current->rightChild);

	delete current;
	current = NULL;
}

bool BVHTree::isLeft(BVHTreeNode* parent, BVHTreeNode* current) {
	return(parent->leftChild==current);
}

void BVHTree::setSizes(BVHTreeNode* current) {
	BVHTreeNode* ptr = current->parent;
	while(ptr != NULL)
	{
		AABB branch_area = ptr->leftChild->aabb + ptr->rightChild->aabb;
		ptr->aabb = branch_area;
		ptr = ptr->parent;
	}
}

void BVHTree::addBVHMember(AABB objectArea, std::string name) {

	BVHTreeNode *newNode = new BVHTreeNode(objectArea,name,true);
	map[name] = newNode;

	if(root == NULL) //if the tree is empty
	{
		root = newNode;
	}

	else if(root->leftChild == NULL) //checking whether there is only one node in the tree
	{
		AABB branch_area = newNode->aabb + root->aabb;
		BVHTreeNode *branch = new BVHTreeNode(branch_area,"branch",false);

		branch->rightChild = root;
		branch->leftChild = newNode;

		root->parent = branch;
		newNode->parent = branch;

		root = branch;
	}

	else //if there are more than one node in the tree
	{
		BVHTreeNode* branchNode = root;

		while(!(branchNode->isLeaf))
		{
			int increaseInRightTreeSize = AABB::unionArea(newNode->aabb, branchNode->rightChild->aabb) - branchNode->rightChild->aabb.getArea();
			int increaseInLeftTreeSize = AABB::unionArea(newNode->aabb, branchNode->leftChild->aabb) - branchNode->leftChild->aabb.getArea();

			increaseInRightTreeSize < increaseInLeftTreeSize ? branchNode = branchNode->rightChild : branchNode = branchNode->leftChild;
		}

		AABB branch_area = newNode->aabb + branchNode->aabb;
		BVHTreeNode* new_branch = new BVHTreeNode(branch_area,"branch",false);
		BVHTreeNode* grand_parent = branchNode->parent;

		new_branch->leftChild = newNode;
		newNode->parent = new_branch;

		isLeft(grand_parent,branchNode) ? grand_parent->leftChild = new_branch : grand_parent->rightChild = new_branch;

		new_branch->parent = grand_parent;
		branchNode->parent = new_branch;
		new_branch->rightChild = branchNode;
		
		setSizes(new_branch);
	}
}

void BVHTree::removeBVHMember(std::string name) {

	if(map.find(name) == map.end())
	{
		return;
	}

	else //if name exists in the map
	{
		BVHTreeNode *toRemove = map[name];
		BVHTreeNode *parent_branch = toRemove->parent;
		BVHTreeNode *sibling_node;

		if(parent_branch == NULL)
		{
			delete toRemove;
			root = NULL;
			map.erase(name);
		}

		else
		{
			isLeft(parent_branch,toRemove) ? sibling_node = parent_branch->rightChild : sibling_node = parent_branch->leftChild;

			if(parent_branch->parent == NULL)
			{
				root = sibling_node;
				sibling_node->parent = NULL;
			}

			else
				isLeft(parent_branch->parent,parent_branch) ? parent_branch->parent->leftChild = sibling_node : parent_branch->parent->rightChild = sibling_node;

			sibling_node->parent = parent_branch->parent;

			delete toRemove;
			delete parent_branch;
			toRemove = NULL;
			parent_branch = NULL;

			map.erase(name);
			setSizes(sibling_node);
		}
	}
}

void BVHTree::moveBVHMember(std::string name, AABB newLocation) {
	if(map.find(name) == map.end())
	{
		return;
	}

	else //if name exists in the map
	{
		BVHTreeNode *toMove = map[name];
		BVHTreeNode *parent_Move = toMove->parent;

		if(parent_Move != NULL)
		{
			if((parent_Move->aabb.maxX >= newLocation.maxX)&&(parent_Move->aabb.maxY >= newLocation.maxY)&&
				(parent_Move->aabb.minX <= newLocation.minX)&&(parent_Move->aabb.minY <= newLocation.minY))
			{
				toMove->aabb = newLocation;
			}

			else
			{
				removeBVHMember(name);
				addBVHMember(newLocation,name);
			}
		}
		else //start of changed lines
		{
			removeBVHMember(name);
			addBVHMember(newLocation,name);
		} //end of changed lines
	}
}

void BVHTree::insertCollidingObjToVector(std::vector<std::string> &vec, BVHTreeNode* current_node, AABB &object) {
	if(current_node == NULL)
		return;

	if(current_node->isLeaf && current_node->aabb.collide(object))
		vec.push_back(current_node->name);

	else
	{
		if(current_node->aabb.collide(object))
		{
			insertCollidingObjToVector(vec, current_node->leftChild, object);
			insertCollidingObjToVector(vec, current_node->rightChild, object);
		}
	}
}

std::vector<std::string> BVHTree::getCollidingObjects(AABB object) {
	std::vector<std::string> vec;
	insertCollidingObjToVector(vec,root,object);
	return vec;	
}

void BVHTree::printNode(std::ostream &out, BVHTreeNode *node, int level) {
	if (root == nullptr) return;
	for (int i = 0; i < level; i++) {
		out << "  ";
	}
	if (!node->isLeaf) {
		out << "+ branch || ";
		node->aabb.printAABB(out);
		out << std::endl;
		printNode(out, node->rightChild, level + 1);
		printNode(out, node->leftChild, level + 1);
	}
	else {
		out << "- ";
		if (node->parent) {
			if (node->parent->rightChild == node)
				out << "R ";
			else
				out << "L ";
		}
		out << "- leaf: " << node->name << " || ";
		node->aabb.printAABB(out);
		out << std::endl;
	}
}

std::ostream &operator<<(std::ostream &out, BVHTree &tree) {
	tree.printNode(out, tree.root, 0);
	return out;
}