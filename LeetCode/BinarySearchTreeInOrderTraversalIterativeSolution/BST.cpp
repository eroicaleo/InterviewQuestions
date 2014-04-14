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

void BST(Node *root) {
    if (!root) {
	cout << "The tree is empty!" << endl;
	return;
    }

    stack<item> s;
    s.push(make_pair(root, NotVisited));

    while (!s.empty()) {
	item top = s.top();
	Node* n {top.first->left};
	cout << top.first->data << endl;
	if ((top.second == NotVisited) && n) {
	    s.push(make_pair(top.first, NotVisited));
	    top.second = LeftVisited;
	} else {
	    cout << "Node data is: " << top.first->data << endl;
	    n = top.first->right;
	    s.pop();
	    if (n) {
		s.emplace(n, NotVisited);
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
    BST(&vNode[0]);
}
