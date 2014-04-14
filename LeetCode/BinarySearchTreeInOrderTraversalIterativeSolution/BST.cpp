#include <iostream>
#include <stack>
#include <vector>

using namespace std;

enum NodeStatus {NotVisited, LeftVisited, RightVisited};

struct Node {
    int data = 0;
    Node *left = NULL;
    Node *right = NULL;
};

typedef pair<Node*, NodeStatus> item;

void BST2(Node *root) {
    stack<Node *> s;
    Node* current = root;
    while(!s.empty() || current) {
	if (current) {
	    s.push(current);
	    current = current->left;
	} else {
	    current = s.top();
	    cout << "Node data is: " << current->data << endl;
	    s.pop();
	    current = current->right;
	}
    }
    return;
}

void BST(Node *root) {
    if (!root) {
	cout << "The tree is empty!" << endl;
	return;
    }

    stack<item> s;
    s.push(make_pair(root, NotVisited));

    while (!s.empty()) {
	item &top = s.top();
	Node* n {top.first->left};
	if ((top.second == NotVisited) && n) {
	    s.push(make_pair(n, NotVisited));
	    top.second = LeftVisited;
	} else {
	    cout << "Node data is: " << top.first->data << endl;
	    n = top.first->right;
	    s.pop();
	    if (n) {
		s.push(make_pair(n, NotVisited));
	    }
	}
    }

    return;
}

int main() {
    vector<Node> vNode(7);
    for (size_t i = 0; i < vNode.size(); ++i) {
	vNode[i].data = i;
	if (i < 3) {
	    vNode[i].left = &vNode[2*i+1];
	    vNode[i].right = &vNode[2*i+2];
	}
    }
    // BST(&vNode[0]);
    BST2(&vNode[0]);
}
