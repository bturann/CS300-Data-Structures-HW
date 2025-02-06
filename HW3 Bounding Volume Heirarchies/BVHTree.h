#include "AABB.h"
#include "BVHTreeNode.h"
#include <vector>
#include <iostream>
#include <unordered_map>
#ifndef _BVH_TREE
#define _BVH_TREE

class BVHTree {
private:
	BVHTreeNode *root;
	std::unordered_map<std::string, BVHTreeNode *> map;
public:
	BVHTree();
	~BVHTree();
	void printNode(std::ostream &out, BVHTreeNode *, int level);
	void addBVHMember(AABB objectArea, std::string name);
	void moveBVHMember(std::string name, AABB newLocation);
	void removeBVHMember(std::string name);
	std::vector<std::string> getCollidingObjects(AABB object);
	friend std::ostream &operator<<(std::ostream &out, BVHTree &tree);

	bool isLeft(BVHTreeNode* parent, BVHTreeNode* current); //for cheking wheter current is the left child of parent or not, return true if left child
	void setSizes(BVHTreeNode* current); //after adding new node, updates all the ancestors' object areas
	void insertCollidingObjToVector(std::vector<std::string> &vec, BVHTreeNode* current_node, AABB &object);
	void deleteTree(BVHTreeNode* current); //for deleting the entire tree
};

#endif