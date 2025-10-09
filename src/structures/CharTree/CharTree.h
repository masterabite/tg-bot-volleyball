#pragma once
#ifndef CHARTREE_H
#define CHARTREE_H

#include <iostream>
#include <chrono>
#include <vector>

#include <unordered_map>

class CharTree
{
public:
     CharTree();
    ~CharTree();

    void clear();
    
    bool add_key    (const std::string& str);
    bool check_key  (const std::string& str);

private:

    struct node {   
        char val;
        bool isKey;
        std::unordered_map<char, node*> children;  
        node(char _val): val(_val), isKey(false) {};
    };

    void destroy_node(node* n);

	node* root;
};


#endif // !CHARTREE_H

