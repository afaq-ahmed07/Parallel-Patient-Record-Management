#ifndef AVL_H
#define AVL_H

#include <iostream>
#include <string>
using namespace std;

struct Patient_Record{
    string id;
    int age;
    char gender;
    string blood_type;
    string date;
    string diagnosis;
};

struct TreeNode {
Patient_Record record;
int height;
TreeNode* left;
TreeNode* right;
TreeNode(const Patient_Record& record = Patient_Record(), TreeNode* left = nullptr, TreeNode* right = nullptr): record(record), left(left), right(right), height(1) {}
};

class PatientAVL {
private:
    TreeNode* root;
    int height(TreeNode* node);
    TreeNode* rotateRight(TreeNode* y);
    TreeNode* rotateLeft(TreeNode* x);
    TreeNode* insert(TreeNode* node, const Patient_Record& record);
    bool remove(TreeNode*& node, const string& id);
    TreeNode* search(TreeNode* node, const string& id);
    void print(TreeNode* node);
    void deleteNode(TreeNode* node);
    int getBalance(TreeNode* node);

public:
    PatientAVL() : root(nullptr) {}

    void insert(const Patient_Record& record);
    bool remove(const string& id);
    Patient_Record* search(const std::string& id);
    TreeNode* get_root();
    void print();
    void deleteTree();
    void set_root(TreeNode* new_root);
    TreeNode* findMin(TreeNode* node);
};

#endif

