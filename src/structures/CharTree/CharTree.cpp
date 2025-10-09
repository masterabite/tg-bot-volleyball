#include "CharTree.h"

CharTree::CharTree() {
    root = new node(' ');
}

CharTree::~CharTree() {
    destroy_node(root);
}

void CharTree::clear() {
    if (root == nullptr) return;
    
    for (auto& [key, value]: root->children) {
        destroy_node(value);
    }

    root->children.clear();
}

void CharTree::destroy_node(node* n) {
    for (auto& [key, value]: n->children) {
        destroy_node(value);
    }
    delete n;
}

bool CharTree::add_key(const std::string& str) {
    if (str.empty()) return false;

    node* n = root;

    for (char c: str) {
        if (n->children.find(c) == n->children.end()) {
            n->children[c] = new node(c);
        }
        n = n->children[c];
    }

    if (n->isKey) return false;
    
    n->isKey = true;
    return true;
}

bool CharTree::check_key(const std::string& str) {
    node* n = root;
    for (char c: str) {
        if (n->children.find(c) == n->children.end()) return false;
        n = n->children[c];
    }
    return n->isKey;
}