#include<bits/stdc++.h>
using namespace std;
struct range
{
	int low, high;
};

struct Node
{
	range *r;
	int max;
	Node *left, *right, *parent;
};

range *newrange(int low, int high)
{
	range *r = new range();
	r->low = low;
	r->high = high;
	return r;
}

class intervalTree
{
private :
	Node *root;

public:
	intervalTree(range *r)
	{
		root = new Node();
		root->r = r;
		root->max = max(r->low, r->high);
		root->left = NULL;
		root->right = NULL;
		root->parent = NULL;
	}
	
	Node* newNode(range *r)
	{
		Node *node = new Node();
		node->r = r;
		node->max = max(r->low, r->high);
		node->left = NULL;
		node->right = NULL;
		return node;
	}

	void insert(range *r)
	{
		Node *currentNode = newNode(r);
		insert(root, currentNode);
	}
	//to insert nodes in the interval tree
	void insert(Node *root, Node *currentNode)
	{
		// if presently no node is there in tree
		if (!this->root)
		{
			this->root = currentNode;
			this->root->parent = NULL;
		}
		else
		{
			root->max = max(root->max, currentNode->max);     //as max needs to be updated each time
			// for moving to the left child
			if (currentNode->r->low < root->r->low)
			{
				//insert the node if it is null
				if (!root->left)
				{
					root->left = currentNode;
					currentNode->parent = root;
				}
				else
				{
					insert(root->left, currentNode);
				}
			}
			else
			{	//for moving to right child
				// insert the node if it is null
				if(!root->right)
				{
					root->right = currentNode;
					currentNode->parent = root;
				}
				else
				{
					insert(root->right, currentNode);
				}
			}
		}
	}
	//if the interval of any node exactly matches with the required interval
	bool doMatch(range *a, range *b)
	{
		if (a->low == b->low && a->high == b->high)
			return true;
		return false;
	}

	void Delete(range *r)
	{
		Delete(root, r);
	}
	//to check if it is a left child or not
	bool isLeftChild(Node *t)
	{
		if (t->parent->left == t)
			return true;
		return false;
	}

	void Delete(Node *root, range *r)
	{
		if(!root)
		{
			cout<<"No such range is there"<<endl;
			return;
		}

		if (doMatch(root->r, r))
		{
			if (root->right)
			{	//replace with the min of right subtree
				Node *replace = root;
				Node *temp = root->right;
				while(temp)
				{
					replace = temp;
					temp = temp->left;
				}
				//delete replace
				if (isLeftChild(replace))
				{
					replace->parent->left = replace->right;
					if (replace->right) replace->right->parent = replace->parent;
				}else
				{
					replace->parent->right = replace->right;
					if (replace->right) replace->right->parent = replace->parent;
				} 
				
				temp = replace->parent;
				while(temp!=root)
				{
					if (temp->left) temp->max = root->left->max;
					if (temp->right) temp->max = max(root->max, root->right->max);
					temp = temp->parent;
				}
				//replacing max with replace
				root->r = replace->r;
				root->max = replace->max;
				if (root->left) root->max = max(root->max, root->left->max);
				if (root->right) root->max = max(root->max, root->right->max);
				}else{//if right subtree is empty
				if (isLeftChild(root)){
					root->parent->left = root->left;
					if (root->left) root->left->parent = root->parent;
				}else
				{
					root->parent->right = root->left;
					if (root->left) root->left->parent = root->parent;
				} 
			}

			Node *temp = root->parent;
			while(temp != NULL)
			{
				temp->max = temp->r->high;
				if (temp->left) temp->max = max(temp->left->max, temp->max);
				if (temp->right) temp->max = max(temp->right->max, temp->max);
				temp = temp->parent;
			}

		}
		else
		{
			if (r->low < root->r->low)
				Delete(root->left, r);
			else Delete(root->right, r);
		}

	}
	//to find if there is a overlap or not
	bool doOverlap(range *a,range*b)
	{
		if (a->high < b->low || a->low > b->high)
			return false;
		return true;
	}

	range* search(range *r)
	{
		return search(root, r);
	}

	range* search(Node *root,range*r)
	{
		if (!root)
			return NULL;
		//if there is an overlap return that node
		if (doOverlap(root->r, r))
			return root->r;
		
		//if there was no overlap go to left child under this condition
		if (root->left && r->low <= root->left->max)
			return search(root->left, r);
		else 
			//else go to right child
			return search(root->right, r);
	}

	void traverseTree()
	{
		traverseTree(root);
	}

	void traverseTree(Node *root)
	{
		if (!root)
			return;
		traverseTree(root->left);
		cout<<root->r->low<<","<<root->r->high<<",("<<root->max<<")"<<"\n";
		traverseTree(root->right);
	}
	
};

int main()
{
	intervalTree it(newrange(15,20));
	range intr[] = {{15, 20}, {10, 30}, {17, 19},{5, 20}, {12, 15}, {30, 40}};
    
    for (int i = 0; i < 6; i++)
		it.insert(newrange(intr[i].low, intr[i].high));
		
	cout<<"Inorder traversal of tree is:\n";
	it.traverseTree();
	cout<<endl;
	
	//on deleting a node
	it.Delete(newrange(17,19));
	
	cout<<"Inorder traversal of tree after deletion is:\n";
	it.traverseTree();
	cout<<endl;
	
	//example 1
	range *found = it.search(newrange(14, 16));
	if(found)
		cout<<"This interval overlaps with ("<<found->low<<","<<found->high<<")"<<endl;
	else cout<<"No Overlap for this interval";
	
	//example 2
	found = it.search(newrange(21, 23));
	if(found)
		cout<<"This interval overlaps with ("<<found->low<<","<<found->high<<")"<<endl;
	else cout<<"No Overlap for this interval";
	
}
