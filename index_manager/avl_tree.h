#pragma once
#include<json/json.h>
using namespace std;
namespace AvlTreeNameSpace {


    string convertToBinary(int num);
    


    int binaryToInt(string binary);

    struct RecordInfo {
    public:
        int offset, recordSize;
        RecordInfo(int offset, int recordSize) : offset(offset), recordSize(recordSize) {}

        const Json::Value toJson() const {
            Json::Value record;
            record["offset"] = convertToBinary(this->offset), record["recordSize"] = convertToBinary(this->recordSize);
            return record;
        }


        static RecordInfo jsonToRecord(Json::Value record) {
            return RecordInfo(binaryToInt(record["offset"].asString()), binaryToInt(record["recordSize"].asString()));
        }
    };


    struct Key {
    public:
        const int key;
        const RecordInfo info;
        Key(int key, RecordInfo info) : key(key), info(info) {}

        const Json::Value toJson() const {
            Json::Value record;
            record["key"] = convertToBinary(this->key), record["info"] = this->info.toJson();
            return record;
        }

        static Key jsonToRecord(Json::Value record) {
            return Key(binaryToInt(record["key"].asString()), RecordInfo::jsonToRecord(record["info"]));
        }

        static int getJsonSize() {
            Key key = Key(-1, RecordInfo(-1, -1));
            return key.toJson().toStyledString().size();
        }
    };


    struct AvlTreeNode : Key {
        int height;
        AvlTreeNode* leftChild, *rightChild;

        AvlTreeNode(int key, RecordInfo info) : Key(key, info) {
            this->height = 1;
            this->leftChild = nullptr;
            this->rightChild = nullptr;
        }

        const Json::Value toJson() {
            Json::Value record;
            record["leftChild"] = leftChild, record["rightChild"] = rightChild;
        }

        static int getJsonSize() {

        }
    };


    class AvlTree {
    private:
        AvlTreeNode* root = nullptr;
        int size;


        int getHeight(const AvlTreeNode* currNode) {
            return currNode != nullptr ? currNode->height : 0;
        }

        const AvlTreeNode* get(const AvlTreeNode* currNode, const int key) const {
            if (currNode == nullptr) return nullptr;

            if (key == currNode->key)
                return currNode; // Base-case: `key` has been found and a pointer pointing to the AvlTreeNode is returned.

            // Base-case: Reached to a child of a leaf-node which means that the `key` is not found and a nullptr is returned in that case.


            if (currNode->key > key)
                return this->get(currNode->leftChild,
                    key); // Recurse on the left-subtree in case the `key` is smaller than the `currentNode` key.

            return this->get(currNode->rightChild, key); // Otherwise recurse on the right-subtree.
        };

        const AvlTreeNode* rightRotate(AvlTreeNode* a) {
            AvlTreeNode* b = a->leftChild, * c = b->leftChild;
            a->leftChild = b->rightChild;
            b->rightChild = a;



            // Updating the height of the nodes belonging to the set {a, b}
            for (AvlTreeNode* node : { a, b }) {
                node->height = max(getHeight(node->leftChild), getHeight(node->rightChild)) + 1;
            }
            return b;
        }

        const AvlTreeNode* leftRotate(AvlTreeNode* a) {
            AvlTreeNode* b = a->rightChild;
            a->rightChild = b->leftChild;
            b->leftChild = a;


            // Updating the height of the nodes belonging to the set {a, b}
            for (AvlTreeNode* node : { a, b }) {
                node->height = max(getHeight(node->leftChild), getHeight(node->rightChild)) + 1;
            }
            return b;
        };


        const AvlTreeNode* insert(AvlTreeNode* currNode, const int key, const RecordInfo& info) {
            if (currNode == nullptr) {
                this->size += sizeof(AvlTreeNode);
                return new AvlTreeNode(
                    key, info); // Base-case: Found an empty place to place a new AvlTreeNode with the passed key.
            }


            if (currNode->key == key) { // Base-case: Found a duplicate key, so we end up updating the `RecordInfo` in this node.
                return new AvlTreeNode(key, info);
            }
                

            // The newly created AvlTreeNode is a potential candidate to be on the left-subtree of the `currNode`
            // if and only if current_node's key is greater than the passed `key`
            // otherwise it will be a potential candidate on the right-subtree.

            if (currNode->key > key)
                currNode->leftChild = const_cast<AvlTreeNode*>(insert(currNode->leftChild, key, info));
            else currNode->rightChild = const_cast<AvlTreeNode*>(insert(currNode->rightChild, key, info));

            currNode->height = max(getHeight(currNode->leftChild), getHeight(currNode->rightChild)) + 1;

            // If the current_node's balance has reached -2, it implies that the subtree containing the node `currNode`
            // as it's root_node is heavy on the right so, we need to left-rotate the subtree to ensure the self-balancing property.

            int balance = getHeight(currNode->leftChild) - getHeight(currNode->rightChild);
            if (balance == -2) {
                if (getHeight(currNode->rightChild->leftChild) > getHeight(currNode->rightChild->rightChild))
                    currNode->rightChild = const_cast<AvlTreeNode*>(rightRotate(currNode->rightChild));
                return const_cast<AvlTreeNode*>(leftRotate(currNode));
            }

            // If the current_node's height has reached 2, it implies that the subtree containing the node `currNode`
            // as it's root_node is two-level's heavy on the left side so, we need to right -rotate to ensure that the overall tree
            // is balanced or psuedo-balanced. (meaning that for every node it's either that the left-subtree's height equals the right-subtree's height or
            // the left -subtree's height is greater than the right one's only by one level).

            if (balance == 2) {
                if (getHeight(currNode->leftChild->leftChild) < getHeight(currNode->leftChild->rightChild))
                    currNode->leftChild = const_cast<AvlTreeNode*>(leftRotate(currNode->leftChild));
                return const_cast<AvlTreeNode*>(rightRotate(currNode));
            }
            return currNode;
        }


        void inorderTraversel(AvlTreeNode* currNode, vector<Key*>& nodes) {
            if (currNode == nullptr) return;

            inorderTraversel(currNode->leftChild, nodes);

            nodes.push_back(currNode);

            inorderTraversel(currNode->rightChild, nodes);
        }

    public:
        
        const void insert(int key, RecordInfo info) {
            if (this->root == nullptr) return void(this->root = new AvlTreeNode(key, info));
            this->root = const_cast<AvlTreeNode*>(this->insert(this->root, key, info));
        }

        const AvlTreeNode* getRoot() const {
            return this->root;
        }

        const AvlTreeNode* get(int key) const {
            return get(root, key);
        }


        const vector<Key*> getAllKeys() {
            vector<Key*> nodes;
            inorderTraversel(const_cast<AvlTreeNode*>(getRoot()), nodes);
            return nodes;
        }


        const int getSize() const {
            return this->size;
        }
    };
}

