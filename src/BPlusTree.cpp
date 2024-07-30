#include "../include/BPlusTree.h"

BPlusTreeNode::BPlusTreeNode(bool leaf) : isLeaf(leaf), next(nullptr) {}

BPlusTree::BPlusTree(int maxKeys) : root(nullptr), maxKeys(maxKeys) {}

void BPlusTree::insert(int key, const std::string& path) {
    if (root == nullptr) {
        root = new BPlusTreeNode(true);
        root->keys.push_back(key);
        root->paths.push_back({path});  // Inicializa un vector de paths
    } else {
        BPlusTreeNode* cursor = root;
        BPlusTreeNode* parent = nullptr;

        while (!cursor->isLeaf) {
            parent = cursor;
            for (int i = 0; i < cursor->keys.size(); i++) {
                if (key < cursor->keys[i]) {
                    cursor = (BPlusTreeNode*)cursor->pointers[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    cursor = (BPlusTreeNode*)cursor->pointers[i + 1];
                    break;
                }
            }
        }

        // Verifica si la clave ya existe
        for (int i = 0; i < cursor->keys.size(); i++) {
            if (cursor->keys[i] == key) {
                // Si la clave existe, verifica si la nueva ruta es diferente
                if (std::find(cursor->paths[i].begin(), cursor->paths[i].end(), path) == cursor->paths[i].end()) {
                    // Si la nueva ruta es diferente, añádela
                    cursor->paths[i].push_back(path);
                }
                return;
            }
        }

        if (cursor->keys.size() < maxKeys) {
            int i = 0;
            while (key > cursor->keys[i] && i < cursor->keys.size()) i++;
            cursor->keys.insert(cursor->keys.begin() + i, key);
            cursor->paths.insert(cursor->paths.begin() + i, {path});  // Inicializa un vector de paths
        } else {
            BPlusTreeNode* newLeaf = new BPlusTreeNode(true);
            std::vector<int> tempKeys = cursor->keys;
            std::vector<std::vector<std::string>> tempPaths = cursor->paths;

            int i = 0;
            while (key > tempKeys[i] && i < tempKeys.size()) i++;
            tempKeys.insert(tempKeys.begin() + i, key);
            tempPaths.insert(tempPaths.begin() + i, {path});  // Inicializa un vector de paths

            cursor->keys.resize((maxKeys + 1) / 2);
            cursor->paths.resize((maxKeys + 1) / 2);
            for (int i = 0; i < (maxKeys + 1) / 2; i++) {
                cursor->keys[i] = tempKeys[i];
                cursor->paths[i] = tempPaths[i];
            }

            for (int i = (maxKeys + 1) / 2, j = 0; i < tempKeys.size(); i++, j++) {
                newLeaf->keys.push_back(tempKeys[i]);
                newLeaf->paths.push_back(tempPaths[i]);
            }

            newLeaf->next = cursor->next;
            cursor->next = newLeaf;

            if (cursor == root) {
                BPlusTreeNode* newRoot = new BPlusTreeNode(false);
                newRoot->keys.push_back(newLeaf->keys[0]);
                newRoot->pointers.push_back(cursor);
                newRoot->pointers.push_back(newLeaf);
                root = newRoot;
            } else {
                insertInternal(newLeaf->keys[0], parent, newLeaf);
            }
        }
    }
}


void BPlusTree::insertInternal(int key, BPlusTreeNode* cursor, BPlusTreeNode* child) {
    if (cursor->keys.size() < maxKeys) {
        int i = 0;
        while (key > cursor->keys[i] && i < cursor->keys.size()) i++;
        cursor->keys.insert(cursor->keys.begin() + i, key);
        cursor->pointers.insert(cursor->pointers.begin() + i + 1, child);
    } else {
        BPlusTreeNode* newInternal = new BPlusTreeNode(false);
        std::vector<int> tempKeys = cursor->keys;
        std::vector<void*> tempPointers = cursor->pointers;

        int i = 0;
        while (key > tempKeys[i] && i < tempKeys.size()) i++;
        tempKeys.insert(tempKeys.begin() + i, key);
        tempPointers.insert(tempPointers.begin() + i + 1, child);

        cursor->keys.resize((maxKeys + 1) / 2);
        cursor->pointers.resize((maxKeys + 1) / 2 + 1);
        for (int i = 0; i < (maxKeys + 1) / 2; i++) {
            cursor->keys[i] = tempKeys[i];
            cursor->pointers[i] = tempPointers[i];
        }
        cursor->pointers[(maxKeys + 1) / 2] = tempPointers[(maxKeys + 1) / 2];

        for (int i = (maxKeys + 1) / 2 + 1, j = 0; i < tempKeys.size(); i++, j++) {
            newInternal->keys.push_back(tempKeys[i]);
            newInternal->pointers.push_back(tempPointers[i]);
        }
        newInternal->pointers.push_back(tempPointers.back());

        if (cursor == root) {
            BPlusTreeNode* newRoot = new BPlusTreeNode(false);
            newRoot->keys.push_back(tempKeys[(maxKeys + 1) / 2]);
            newRoot->pointers.push_back(cursor);
            newRoot->pointers.push_back(newInternal);
            root = newRoot;
        } else {
            insertInternal(tempKeys[(maxKeys + 1) / 2], findParent(root, cursor), newInternal);
        }
    }
}

BPlusTreeNode* BPlusTree::findParent(BPlusTreeNode* cursor, BPlusTreeNode* child) {
    if (cursor->isLeaf || ((BPlusTreeNode*)cursor->pointers[0])->isLeaf) {
        return nullptr;
    }

    for (int i = 0; i < cursor->pointers.size(); i++) {
        if (cursor->pointers[i] == child) {
            return cursor;
        } else {
            BPlusTreeNode* parent = findParent((BPlusTreeNode*)cursor->pointers[i], child);
            if (parent != nullptr) return parent;
        }
    }
    return nullptr;
}

std::string BPlusTree::searchPath(std::string record) {
    int key = stoi(record.substr(0, 2));
    if (root == nullptr) {
        return "";
    } else {
        BPlusTreeNode* cursor = root;
        while (!cursor->isLeaf) {
            for (int i = 0; i < cursor->keys.size(); i++) {
                if (key < cursor->keys[i]) {
                    cursor = (BPlusTreeNode*)cursor->pointers[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    cursor = (BPlusTreeNode*)cursor->pointers[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < cursor->keys.size(); i++) {
            if (cursor->keys[i] == key) {
                std::string allPaths;
                for (const auto& path : cursor->paths[i]) {
                    allPaths += path + " ";
                }
                return allPaths;
            }
        }
        return "";
    }
}



void BPlusTree::printTree() const {
    if (root != nullptr) {
        std::vector<BPlusTreeNode*> nodes = {root};
        while (!nodes.empty()) {
            std::vector<BPlusTreeNode*> nextNodes;
            for (BPlusTreeNode* node : nodes) {
                for (int key : node->keys) {
                    std::cout << key << " ";
                }
                std::cout << "| ";
                if (!node->isLeaf) {
                    for (void* pointer : node->pointers) {
                        nextNodes.push_back((BPlusTreeNode*)pointer);
                    }
                }
            }
            std::cout << std::endl;
            nodes = nextNodes;
        }
    }
}



void BPlusTree::printLeafPaths() {
    if (root != nullptr) {
        BPlusTreeNode* cursor = root;
        while (!cursor->isLeaf) {
            cursor = (BPlusTreeNode*)cursor->pointers[0];
        }
        while (cursor != nullptr) {
            for (const auto& paths : cursor->paths) {
                for (const std::string& path : paths) {
                    std::cout << path << " ";
                }
                std::cout << "| ";
            }
            cursor = cursor->next;
        }
        std::cout << std::endl;
    }
}


std::string BPlusTree::search(int key) {
    std::vector<std::string> paths = getPathsForKey(key);
    if (paths.size()==0) {
        return "empty";
    } else if(paths.size()>1) {
        return "Nfound";
    }
    return paths[0];
}

void BPlusTree::deleteKey(int key) {
    if (root == nullptr) {
        std::cout << "Tree is empty" << std::endl;
        return;
    } else {
        BPlusTreeNode* cursor = root;
        BPlusTreeNode* parent = nullptr;

        while (!cursor->isLeaf) {
            parent = cursor;
            for (int i = 0; i < cursor->keys.size(); i++) {
                if (key < cursor->keys[i]) {
                    cursor = (BPlusTreeNode*)cursor->pointers[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    cursor = (BPlusTreeNode*)cursor->pointers[i + 1];
                    break;
                }
            }
        }

        auto it = std::find(cursor->keys.begin(), cursor->keys.end(), key);
        if (it != cursor->keys.end()) {
            int index = it - cursor->keys.begin();
            cursor->keys.erase(it);
            cursor->paths.erase(cursor->paths.begin() + index);

            if (cursor == root && cursor->keys.empty()) {
                delete cursor;
                root = nullptr;
                return;
            }

            if (cursor->keys.size() < (maxKeys + 1) / 2) {
                deleteEntry(cursor, key, parent);
            }
        }
    }
}

void BPlusTree::deleteEntry(BPlusTreeNode* node, int key, BPlusTreeNode* parent) {
    if (node == root) {
        if (node->keys.empty() && !node->isLeaf) {
            root = (BPlusTreeNode*)node->pointers[0];
            delete node;
        }
        return;
    }

    int separatorIndex = -1;
    for (int i = 0; i < parent->pointers.size(); i++) {
        if (parent->pointers[i] == node) {
            separatorIndex = i;
            break;
        }
    }

    BPlusTreeNode* leftNode = separatorIndex > 0 ? (BPlusTreeNode*)parent->pointers[separatorIndex - 1] : nullptr;
    BPlusTreeNode* rightNode = separatorIndex < parent->pointers.size() - 1 ? (BPlusTreeNode*)parent->pointers[separatorIndex + 1] : nullptr;

    if (leftNode && leftNode->keys.size() > (maxKeys + 1) / 2) {
        redistributeNodes(leftNode, node, parent, separatorIndex - 1);
    } else if (rightNode && rightNode->keys.size() > (maxKeys + 1) / 2) {
        redistributeNodes(node, rightNode, parent, separatorIndex);
    } else {
        if (leftNode) {
            mergeNodes(leftNode, node, parent, separatorIndex - 1);
        } else {
            mergeNodes(node, rightNode, parent, separatorIndex);
        }
    }
}

void BPlusTree::mergeNodes(BPlusTreeNode* leftNode, BPlusTreeNode* rightNode, BPlusTreeNode* parent, int separatorIndex) {
    leftNode->keys.push_back(parent->keys[separatorIndex]);
    leftNode->keys.insert(leftNode->keys.end(), rightNode->keys.begin(), rightNode->keys.end());

    leftNode->paths.insert(leftNode->paths.end(), rightNode->paths.begin(), rightNode->paths.end());

    if (!leftNode->isLeaf) {
        leftNode->pointers.insert(leftNode->pointers.end(), rightNode->pointers.begin(), rightNode->pointers.end());
    }

    parent->keys.erase(parent->keys.begin() + separatorIndex);
    parent->pointers.erase(parent->pointers.begin() + separatorIndex + 1);

    if (leftNode->isLeaf) {
        leftNode->next = rightNode->next;
    }

    delete rightNode;

    if (parent == root && parent->keys.empty()) {
        root = leftNode;
        delete parent;
    }
}

void BPlusTree::redistributeNodes(BPlusTreeNode* leftNode, BPlusTreeNode* rightNode, BPlusTreeNode* parent, int separatorIndex) {
    if (leftNode->keys.size() > rightNode->keys.size()) {
        if (!leftNode->isLeaf) {
            rightNode->keys.insert(rightNode->keys.begin(), parent->keys[separatorIndex]);
            parent->keys[separatorIndex] = leftNode->keys.back();
            leftNode->keys.pop_back();

            rightNode->pointers.insert(rightNode->pointers.begin(), leftNode->pointers.back());
            leftNode->pointers.pop_back();
        } else {
            rightNode->keys.insert(rightNode->keys.begin(), leftNode->keys.back());
            rightNode->paths.insert(rightNode->paths.begin(), leftNode->paths.back());
            leftNode->keys.pop_back();
            leftNode->paths.pop_back();

            parent->keys[separatorIndex] = rightNode->keys.front();
        }
    } else {
        if (!rightNode->isLeaf) {
            leftNode->keys.push_back(parent->keys[separatorIndex]);
            parent->keys[separatorIndex] = rightNode->keys.front();
            rightNode->keys.erase(rightNode->keys.begin());

            leftNode->pointers.push_back(rightNode->pointers.front());
            rightNode->pointers.erase(rightNode->pointers.begin());
        } else {
            leftNode->keys.push_back(rightNode->keys.front());
            leftNode->paths.push_back(rightNode->paths.front());
            rightNode->keys.erase(rightNode->keys.begin());
            rightNode->paths.erase(rightNode->paths.begin());

            parent->keys[separatorIndex] = rightNode->keys.front();
        }
    }
}


std::vector<std::string> BPlusTree::getPathsForKey(int key) {
    if (root == nullptr) {
        return {};
    } else {
        BPlusTreeNode* cursor = root;
        while (!cursor->isLeaf) {
            for (int i = 0; i < cursor->keys.size(); i++) {
                if (key < cursor->keys[i]) {
                    cursor = (BPlusTreeNode*)cursor->pointers[i];
                    break;
                }
                if (i == cursor->keys.size() - 1) {
                    cursor = (BPlusTreeNode*)cursor->pointers[i + 1];
                    break;
                }
            }
        }
        for (int i = 0; i < cursor->keys.size(); i++) {
            if (cursor->keys[i] == key) {
                return cursor->paths[i];
            }
        }
        return {};
    }
}

void BPlusTree::saveTree(const std::string& filename) {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Error: No se pudo abrir el archivo para escritura." << std::endl;
        return;
    }

    // Guardamos el maxKeys
    outFile << maxKeys << std::endl;

    // Usamos un recorrido por niveles para guardar el árbol
    std::queue<BPlusTreeNode*> queue;
    if (root) queue.push(root);

    while (!queue.empty()) {
        int levelSize = queue.size();
        for (int i = 0; i < levelSize; ++i) {
            BPlusTreeNode* node = queue.front();
            queue.pop();

            // Guardamos si es hoja o no
            outFile << (node->isLeaf ? "1" : "0") << " ";

            // Guardamos las claves
            outFile << node->keys.size() << " ";
            for (int key : node->keys) {
                outFile << key << " ";
            }

            // Guardamos las rutas si es un nodo hoja
            if (node->isLeaf) {
                for (const auto& paths : node->paths) {
                    outFile << paths.size() << " ";
                    for (const auto& path : paths) {
                        outFile << path << " ";
                    }
                }
            } else {
                // Si no es hoja, añadimos los hijos a la cola
                for (void* ptr : node->pointers) {
                    queue.push(static_cast<BPlusTreeNode*>(ptr));
                }
            }
            outFile << std::endl;
        }
    }
    outFile.close();
}

void BPlusTree::loadTree(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Error: No se pudo abrir el archivo para lectura." << std::endl;
        return;
    }

    // Limpiamos el árbol existente
    // (Aquí deberías implementar una función para liberar la memoria del árbol actual)

    // Leemos maxKeys
    inFile >> maxKeys;

    std::queue<BPlusTreeNode*> queue;
    std::string line;
    std::getline(inFile, line); // Consumimos el salto de línea

    while (std::getline(inFile, line)) {
        std::istringstream iss(line);
        bool isLeaf;
        iss >> isLeaf;

        BPlusTreeNode* node = new BPlusTreeNode(isLeaf);

        int keyCount;
        iss >> keyCount;
        for (int i = 0; i < keyCount; ++i) {
            int key;
            iss >> key;
            node->keys.push_back(key);
        }

        if (isLeaf) {
            for (int i = 0; i < keyCount; ++i) {
                int pathCount;
                iss >> pathCount;
                std::vector<std::string> paths;
                for (int j = 0; j < pathCount; ++j) {
                    std::string path;
                    iss >> path;
                    paths.push_back(path);
                }
                node->paths.push_back(paths);
            }
        }

        if (queue.empty()) {
            root = node;
        } else {
            BPlusTreeNode* parent = queue.front();
            parent->pointers.push_back(node);
            if (parent->pointers.size() > maxKeys + 1) {
                queue.pop();
            }
        }

        if (!isLeaf) {
            queue.push(node);
        }
    }

    inFile.close();
}