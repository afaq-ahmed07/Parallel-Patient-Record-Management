#include "avl.h"


void PatientAVL::deleteNode(TreeNode* node) {
    if (node == nullptr) {
        return;
    }
    TreeNode* temp = nullptr;
    if (node->left!= nullptr && node->right!= nullptr) {
        temp = findMin(node->right);
    } else {
        temp = node;
    }
    node->record = temp->record;
    if (temp->left!= nullptr) {
        deleteNode(temp->left);
    } else if (temp->right!= nullptr) {
        deleteNode(temp->right);
    } else {
        delete temp;
    }
}

TreeNode* PatientAVL::findMin(TreeNode* node) {
    while (node->left!= nullptr) {
        node = node->left;
    }
    return node;
}

void PatientAVL::deleteTree() {
    TreeNode* root = this->get_root();
    deleteNode(root);
    this->set_root(nullptr);
}

void PatientAVL::set_root(TreeNode* new_root) {
    root = new_root;
}

TreeNode* PatientAVL::insert(TreeNode* node, const Patient_Record& record) {
    if (!node) {
        return new TreeNode(record);
    }
    if (record.id < node->record.id) {
        node->left = insert(node->left, record);
    } else {
        node->right = insert(node->right, record);
    }
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && record.id < node->left->record.id) {
        return rotateRight(node);
    }
    if (balance < -1 && record.id > node->right->record.id) {
        return rotateLeft(node);
    }
    if (balance > 1 && record.id > node->left->record.id) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && record.id < node->right->record.id) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    return node;
}
void PatientAVL::insert(const Patient_Record& record){
   root=insert(root,record);
}

int PatientAVL::height(TreeNode* node) {
    if (!node) {
        return 0;
    }
    return node->height;
}

int PatientAVL::getBalance(TreeNode* node) {
    if (!node) {
        return 0;
    }
    return height(node->left) - height(node->right);
}

TreeNode* PatientAVL::rotateRight(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

TreeNode* PatientAVL::rotateLeft(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

TreeNode* PatientAVL::search(TreeNode* root, const string& id) {
    if (!root || root->record.id == id) {
        return root;
    }

    if (id < root->record.id) {
        return search(root->left, id);
    } else {
        return search(root->right, id);
    }
}

Patient_Record* PatientAVL::search(const string& id) {
    TreeNode* node = search(root, id);
    return (node? &node->record : nullptr);
}

bool PatientAVL::remove(TreeNode* &node, const string& id) {
    if (!node) {
        return false;
    }

    if (id < node->record.id) {
        return remove(node->left, id);
    } else if (id > node->record.id) {
        return remove(node->right, id);
    } else {

        // Case 1: No child
        if (!node->left &&!node->right) {
            node = nullptr;
            return true;
        }

        // Case 2: One child
        if (!node->left) {
            TreeNode* temp = node->right;
            delete node;
            node = temp;
            return true;
        } else if (!node->right) {
            TreeNode* temp = node->left;
            delete node;
            node = temp;
            return true;
        }

        // Case 3: Two children: Get the inorder successor (smallest in the right subtree)
        TreeNode* temp = node->right;
        while (temp->left) {
            temp = temp->left;
        }

        // Copy the inorder successor's data to this node
        node->record = temp->record;

        // Remove the inorder successor node
        return remove(node->right, temp->record.id);
    }
}

bool PatientAVL::remove(const string& id) {
    return remove(root, id);
}



TreeNode* PatientAVL::get_root() {
    if (root!= nullptr) {
        return root;
    } else {
        return nullptr;
    }
}

void PatientAVL::print(TreeNode* node){
    if (node == nullptr) return;

    print(node->left);

    cout << "ID: " << node->record.id << ", Age: " << node->record.age << ", Gender: " << node->record.gender
         << ", Blood Type: " << node->record.blood_type << ", Date: " << node->record.date
         << ", Diagnosis: " << node->record.diagnosis << endl;

    print(node->right);
}

void PatientAVL::print(){
    print(root);
}

