#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>
#include <cmath>
#include <cstdlib>

#define ADDOK 1
#define ADDFAIl -1
#define GRAPHOK 2
#define GRAPHFAIL -2

typedef int Elem_t;

class Node {

public:

    int data_;
    Node* left_; //NO
    Node* right_; //YES

    Node* node_add(Elem_t new_data);
    void node_print(FILE* file, int n);
    void node_dump();
    Node* node_find(Elem_t data);
};

Node* Node::node_find(Elem_t data) {

    assert(this != nullptr);

    if (this -> data_ == data)
        return this;

    if (this -> left_) return this -> left_ -> node_find(data);
    if (this -> right_) return this -> right_ ->node_find(data);
}

Node* Node::node_add(Elem_t new_data) {

    if (this == nullptr) {

        Node* new_node = new Node;

        assert(new_node != nullptr);

        new_node -> data_ = new_data;
        new_node -> left_ = new_node -> right_ = nullptr;

        return new_node;
    }

    if (new_data < this -> data_) {
        if (this -> left_) this -> left_ -> node_add(new_data);
        else this -> left_ = this -> left_ -> node_add(new_data);
    }
    if (new_data > this -> data_) {
        if (this -> right_) this -> right_ -> node_add(new_data);
        else this -> right_ = this -> right_ -> node_add(new_data);
    }
}

void Node::node_dump() {

    assert(this);

    if (this -> left_) this -> left_ -> node_dump();

    fprintf(stderr, "----------------------\n");
    fprintf(stderr, "Node: [0x%p]\n", this);
    fprintf(stderr, "Data: %d\n", this -> data_);
    fprintf(stderr, "Left: [0x%p]\n", this -> left_);
    fprintf(stderr, "Right: [0x%p]\n", this -> right_);

    if (this -> right_) this -> right_ -> node_dump();

}

class CTree {

public:

    Node* root_;

    CTree();
    ~CTree();
    int tree_add (Elem_t new_data);
    Node* tree_find (Elem_t data);
    void tree_print(Elem_t n);
    int tree_graph();
    void tree_print();
    void tree_dump();
    void from_str_to_tree(std::string str);
};

CTree::CTree() : root_ (nullptr) {}

CTree::~CTree() {

    assert (this != nullptr);

    delete root_;
}

int CTree::tree_add(Elem_t new_data) {

    if (this -> root_ == nullptr)
        this -> root_ = this -> root_ -> node_add(new_data);

    else {

        this -> root_ -> node_add(new_data);
    }
}

void Node::node_print(FILE* file, int n) {

    assert (this);

    fprintf(file, "\n");
    fprintf(file, "%*s", n, "{ ");

    fprintf(file, "%d", this -> data_);

    if (this -> left_) this -> left_ -> node_print(file, n + 1);
    if (this -> right_) this -> right_ -> node_print(file, n + 1);

    if (this -> left_ == nullptr && this -> right_ == nullptr) fprintf(file,  " }");
    else fprintf(file,  "%*s", n, " }");

    fprintf(file, "\n");

}

void CTree::tree_print() {

    FILE* tree_file = fopen("tree_file.txt", "w");

    this -> root_ -> node_print(tree_file, 0);

    fclose(tree_file);
}

void CTree::tree_dump() {

    assert(this);

    fprintf(stderr, "Tree: [0x%p]\n", this);
    fprintf(stderr, "Root of tree: [0x%p]\n", this -> root_);

    if (this->root_) {
        this->root_->node_dump();
    }
}

void fprintf_node (Node* elem, FILE* file) {

    assert(elem != nullptr);
    assert(file != nullptr);

    if (elem -> left_) fprintf_node(elem -> left_, file);

    fprintf(file, "Node_%p [shape = record, label = \"{ Data = %d | Left = 0x%p | Right = 0x%p }\"];\n", elem, elem -> data_, elem -> left_, elem -> right_);

    if (elem -> right_) fprintf_node(elem -> right_, file);
}

void fprintf_links (Node* elem, FILE* file) {

    assert(elem != nullptr);
    assert(file != nullptr);

    if(elem -> left_) fprintf_links(elem -> left_, file);

    if (elem -> left_) fprintf(file, "Node_%p->Node_%p;\n", elem, elem -> left_);
    if (elem -> right_) fprintf(file, "Node_%p->Node_%p;\n", elem, elem -> right_);

    if(elem -> right_) fprintf_links(elem -> right_, file);
}

int CTree::tree_graph() {

    Node* elem = this -> root_;

    std::vector<Node* > v;

    if (elem == nullptr)
        return GRAPHFAIL;

    FILE* tree_dump = fopen("tree_dump.txt", "w");
    fprintf(tree_dump, "digraph {\n");

    fprintf_node(this -> root_, tree_dump);
    fprintf_links(this -> root_, tree_dump);

    fprintf(tree_dump, "}");
    fclose(tree_dump);

    system("dot -Tpng -otree_dump.png tree_dump.txt");
    //system("start ~/tree_dump.png");

    return GRAPHOK;
}

Node* CTree::tree_find(Elem_t data) {

    assert(this != nullptr);

    Node* elem = this -> root_;

    if (root_ == nullptr)
        return nullptr;

    return this -> root_ -> node_find(data);
}

std::string file_to_string() {

    FILE* tree_file = fopen("tree_file.txt", "r");

    char str[1000];
    std::string s;

    while (1) {

        char* estr = fgets(str, sizeof(str), tree_file);
        if (estr == nullptr)
            break;
        char* save_char = strchr(str, '\n');
        if (save_char != nullptr)
            *save_char = ' ';

        s += str;

        //fprintf(stderr, "%s", str);
    }

    fclose(tree_file);

    return s;
}

void CTree::from_str_to_tree (std::string str) {

    std::string save_str, put_str;

    for (int i = 0; i < str.size(); i++) {

        if ((char(str[i]) >= 'a' && char(str[i]) <= 'z' || char(str[i]) >= 'A' && char(str[i]) <= 'Z') || char(str[i]) >= '0' && char(str[i]) <= '9') {

            save_str += str[i];
        }
        else {
            put_str = save_str;
            if (!put_str.empty()) {
                std::cout << put_str << " ";
                this -> tree_add(stoi(put_str));
            }
            save_str.clear();
            put_str.clear();
        }
    }
}

int main() {
    CTree t1;
    /*t1.tree_add(5);
    t1.tree_add(1);
    t1.tree_add(9);
    t1.tree_add(10);
    t1.tree_add(7);
    t1.tree_add(6);
    t1.tree_add(8);
    t1.tree_print();*/

    std::string s = file_to_string();

    t1.from_str_to_tree(s);

    t1.tree_dump();

    //std::cout << s;

    t1.tree_graph();
    return 0;
}
