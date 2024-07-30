#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <fstream>
#include <queue>
#include <sstream>

class BPlusTreeNode {
public:
    bool isLeaf;
    std::vector<int> keys;
    std::vector<void*> pointers;
    BPlusTreeNode* next;  // Usado para que los nodos hoja apunten al siguiente nodo hoja
   std::vector<std::vector<std::string>> paths;  // Almacena las rutas asociadas a las claves


    BPlusTreeNode(bool leaf);
};

class BPlusTree {
public:
    BPlusTree(int maxKeys);

    void insert(int key, const std::string& path);
    std::string search(int key);
    void printTree() const;
    void printNode(BPlusTreeNode* node) const;
    void printLeafPaths();
    std::string searchPath(std::string record);
    std::vector<std::string> getPathsForKey(int key);

    void deleteKey(int key);

    void saveTree(const std::string& filename);
    void loadTree(const std::string& filename);


private:
    BPlusTreeNode* root;
    int maxKeys;

    void insertInternal(int key, BPlusTreeNode* cursor, BPlusTreeNode* child);
    BPlusTreeNode* findParent(BPlusTreeNode* cursor, BPlusTreeNode* child);

    void deleteEntry(BPlusTreeNode* node, int key, BPlusTreeNode* parent);
    void mergeNodes(BPlusTreeNode* leftNode, BPlusTreeNode* rightNode, BPlusTreeNode* parent, int separatorIndex);
    void redistributeNodes(BPlusTreeNode* leftNode, BPlusTreeNode* rightNode, BPlusTreeNode* parent, int separatorIndex);
};

#endif // BPLUSTREE_H
