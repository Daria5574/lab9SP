#include <iostream>
#include <vector>
#include <string>
#include <exception>

#include "cheese.h"

namespace cheese_shop {
    struct Node {
        Cheese data;
        Node* left;
        Node* right;
    };

    class CheeseBinaryTree {
    private:
        Node* root = nullptr;

        Node* insertRecursive(Node* root, const Cheese& cheese) {
            if (root == nullptr) {
                Node* newNode = new Node;
                newNode->data = cheese;
                newNode->left = nullptr;
                newNode->right = nullptr;
                return newNode;
            }

            if (cheese.price + cheese.fatPercent < root->data.price + root->data.fatPercent) {
                root->left = insertRecursive(root->left, cheese);
            }
            else {
                root->right = insertRecursive(root->right, cheese);
            }

            return root;
        }

        Node* findNode(Node* root, const float price, const float fatPercent) {
            if (root == nullptr || (root->data.price == price && root->data.fatPercent == fatPercent)) {
                return root;
            }

            if (price + fatPercent < root->data.price + root->data.fatPercent) {
                return findNode(root->left, price, fatPercent);
            }
            else {
                return findNode(root->right, price, fatPercent);
            }
        }

        Node* findMinNode(Node* node) {
            Node* current = node;

            while (current && current->left != nullptr) {
                current = current->left;
            }

            return current;
        }

        Node* deleteNode(Node* root, const float price, const float fatPercent) {
            if (root == nullptr) {
                return root;
            }

            if (price + fatPercent < root->data.price + root->data.fatPercent) {
                root->left = deleteNode(root->left, price, fatPercent);
            }
            else if (price + fatPercent > root->data.price + root->data.fatPercent) {
                root->right = deleteNode(root->right, price, fatPercent);
            }
            else {
                if (root->left == nullptr) {
                    Node* temp = root->right;
                    delete root;
                    return temp;
                }
                else if (root->right == nullptr) {
                    Node* temp = root->left;
                    delete root;
                    return temp;
                }

                Node* temp = findMinNode(root->right);
                root->data = temp->data;
                root->right = deleteNode(root->right, temp->data.price, temp->data.fatPercent);
            }
            return root;
        }

        void inorderTraversal(Node* root, std::vector<Cheese>& result) {
            if (root != nullptr) {
                inorderTraversal(root->left, result);
                result.push_back(root->data);
                inorderTraversal(root->right, result);
            }
        }

    public:
        void insert(const Cheese& cheese) {
            root = insertRecursive(root, cheese);
        }

        void remove(const float price, const float fatPercent) {
            root = deleteNode(root, price, fatPercent);
        }

        bool search(float price, float fatPercent) {
            Node* result = findNode(root, price, fatPercent);
            return result != nullptr;
        }

        std::vector<Cheese> getAllCheeses() {
            std::vector<Cheese> result;
            inorderTraversal(root, result);
            return result;
        }
    };
}

int main() {
    setlocale(LC_ALL, "Russian");
    cheese_shop::CheeseBinaryTree cheeseTree;

    try {
        cheese_shop::Cheese tartar{ "тартар", "татарстан", 30.5, 1000 };
        cheese_shop::Cheese mozzarella{ "моцарелла", "великобритания", 20.0, 500 };

        cheeseTree.insert(tartar);
        cheeseTree.insert(mozzarella);

        std::cout << "поиск тартара в дереве: " << (cheeseTree.search(1000, 30.5) ? "найден" : "не найден") << std::endl;

        cheeseTree.remove(5.99, 30.5);

        std::vector<cheese_shop::Cheese> allCheeses = cheeseTree.getAllCheeses();

        for (const auto& cheese : allCheeses) {
            std::cout << "\nсыр: " << cheese.brand << ", \nпроизводитель: " << cheese.manufacturer << ", \nжирность: " << cheese.fatPercent << ", \nцена: " << cheese.price << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "исключение: " << e.what() << std::endl;
    }

    return 0;
}