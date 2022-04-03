#include <iostream>
#include <cassert>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <stack>
#include <vector>

#define ADDOK 1
#define ADDFAIl -1
#define GRAPHOK 2
#define GRAPHFAIL -2

typedef std::string Elem_t;

class Node {

public:

    Elem_t data_;
    Node* left_; //NO
    Node* right_; //YES

    Node();
    Node* node_add(Elem_t new_data);
    void node_print(FILE* file, int n);
    void node_dump();
    Node* node_find(Elem_t data);
};

Node::Node():
data_("0"),
right_(nullptr),
left_(nullptr)
{

}

void node_add_str(std::string str, Node* node);

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
    fprintf(stderr, "Data: %s\n", this -> data_.c_str());
    fprintf(stderr, "Left: [0x%p]\n", this -> left_);
    fprintf(stderr, "Right: [0x%p]\n", this -> right_);

    if (this -> right_) this -> right_ -> node_dump();

}

class CTree {

public:

    Node* root_;

    CTree();
    ~CTree();
    void tree_add (Elem_t new_data);
    Node* tree_find (Elem_t data);
    void tree_print(Elem_t n);
    int tree_graph();
    void tree_print();
    void tree_dump();
    void from_str_to_tree(std::string str);
    void akinator_func ();
};

CTree::CTree() : root_ (nullptr) {}

CTree::~CTree() {

    assert (this != nullptr);

    delete root_;
}

void CTree::tree_add(Elem_t new_data) {

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

    fprintf(file, "%s", this -> data_.c_str());

    if (this -> right_) this -> right_ -> node_print(file, n + 1);
    if (this -> left_) this -> left_ -> node_print(file, n + 1);

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

    fprintf(file, "Node_%p [shape = record, label = \"{ Data = %s | Left = 0x%p | Right = 0x%p }\"];\n", elem, elem -> data_.c_str(), elem -> left_, elem -> right_);

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

    if (s.empty()) return "0";

    std::cout << "\n";
    return s;
}

void CTree::from_str_to_tree (std::string str) {

    assert(this != nullptr);

    this -> root_ = new Node;
    node_add_str(str, this -> root_);
}

void node_add_str(std::string str, Node* node) {

    assert(node != nullptr);

    std::stack<Node*> stk;
    //stk.push(node);
    std::string save_str, put_str;
    Node* new_node = nullptr;
    Node* save_root = node;
    int counter = 0;
    int flag = 0;
    int root_counter = 0;

    for (int i = 0; i < str.size(); i++) {

        if (str[i] == '{' && counter != 0) {

            stk.push(node);

            //fprintf(stderr, "\n\n???????????????????????\n\n");

            //save_root -> node_dump();

            //fprintf(stderr, "\n\n???????????????????????\n\n");

            if (node -> right_ == nullptr) {

                node = node -> right_;
                flag = 1;
            }
            else if (node -> left_ == nullptr) {

                node = node -> left_;
                flag = -1;
            }
            else if (node -> right_ && node -> left_) {

                for (int i = 0; i < counter; i++) {

                    if (node == save_root) root_counter++;
                    //if (root_counter > 1) break;
                    if (node -> right_ == nullptr) {

                        node = node -> right_;
                        flag = 1;
                        break;
                    }
                    else if (node -> left_ == nullptr) {

                        node = node -> left_;
                        flag = -1;
                        break;
                    }
                    stk.pop();
                    node = stk.top();

                }
            }
        }

        if ((char(str[i]) >= 'a' && char(str[i]) <= 'z' || char(str[i]) >= 'A' && char(str[i]) <= 'Z') ||
                (char(str[i]) >= '0' && char(str[i]) <= '9') ||  char(str[i]) == '?' || char(str[i]) == '_') {

            save_str += str[i];
        }

        else {

            put_str = save_str;
            if (!put_str.empty()) {

                //std::cout << put_str << "\n";

                if (counter == 0) {
                    node -> data_ = put_str;
                    //std::cout << "Flag: " << flag << "\n";
                    counter++;
                    //std::cout << "Counter: " << counter << "\n";
                    save_str.clear();
                    put_str.clear();

                }
                else {
                    //std::cout << "Flag: " << flag << "\n";
                    new_node = new Node;

                    assert(new_node != nullptr);

                    new_node -> data_ = put_str;
                    node = new_node;
                    //fprintf(stderr, "node: [0x%p]\n", node);
                    save_str.clear();
                    put_str.clear();
                    //fprintf(stderr, "new_node: [0x%p]\n", new_node);
                    counter++;
                    // std::cout << "Counter: " << counter << "\n";
                    if (flag == 1) {
                        stk.top() -> right_ = node;
                    }
                    else if (flag == -1) {
                        stk.top() -> left_ = node;
                    }
                }
            }
        }
        if (str[i] == '}') {
            if (!stk.empty()) {
                node = stk.top();
                stk.pop();
            }
        }

    }
}

void CTree::akinator_func() {

    Node* node = this -> root_;
    assert(node != nullptr);

    std::string s;
    std::vector<Node*> v;
    std::vector<int> v_str_flags;

    while (1) {

        v.push_back(node);

        if(node -> right_ != nullptr && node -> left_ != nullptr) {

            std::cout << "Is he " << node -> data_ << "? [yes/no]\n";
            std::cin >> s;
            if (s == "yes") {

                v_str_flags.push_back(1);
                node = node->right_;
            }
            if (s == "no") {

                v_str_flags.push_back(0);
                node = node->left_;
            }
        }
        else{

            std::cout << "Is it " << node -> data_ << "? [yes/no]\n";
            std::cin >> s;
            if (s == "yes") {

                v_str_flags.push_back(1);

                std::cout << node -> data_ << " is: ";
                for (int i = 0; i < v.size() - 1; i++) {

                    if (i < v.size() - 1) {
                        if (v_str_flags[i] == 0) std::cout << "not " << v[i] -> data_;
                        else std::cout << v[i] -> data_;
                        if (i < v.size() - 2) std::cout << ", ";
                    }

                    else std::cout << v[i] -> data_ << "\n";
                }
            }

            else if (s == "no") {

                v_str_flags.push_back(0);

                std::cout << "Whom did you mean? \n";
                std::string name;
                std::cin >> name;
                std::cout << "What is the difference between " << node -> data_ << " and " << name << "?\n";
                std::string difference;
                std::cin >> difference;
                Node* node_dif = new Node;
                node_dif -> data_ = difference;
                Node* node_name = new Node;
                node_name -> data_ = name;
                if (v_str_flags[v_str_flags.size() - 2] == 1) {

                    v[v.size() - 2] -> right_ = node_dif;
                    node_dif -> right_ = node_name;
                    node_dif -> left_ = v[v.size() - 1];
                }
                else {

                    v[v.size() - 2] -> left_ = node_dif;
                    node_dif -> right_ = node_name;
                    node_dif -> left_ = v[v.size() - 1];
                }
            }
            break;
        }
    }
}

int main() {
    CTree t1;
    /*t1.root_ = new Node;
    t1.root_ -> data_ = "Dimas";
    t1.root_ -> right_ = new Node;
    t1.root_ -> right_ -> data_ = "Vanya";
    t1.root_ -> left_ = new Node;
    t1.root_ -> left_ -> data_ = "Ilya";*/
    //t1.tree_print();
    /*t1.tree_add(5);
    //t1.tree_add(1);
    t1.tree_add(9);
    t1.tree_add(10);
    t1.tree_add(7);
    t1.tree_add(6);
    t1.tree_add(8);*/

    std::string s = file_to_string();

    //std::cout << s << "\n";

    t1.from_str_to_tree(s);

    //t1.tree_print();

    t1.akinator_func();

    t1.tree_print();

    //t1.tree_print();

    //t1.tree_dump();

    //std::cout << s;

    t1.tree_graph();
    return 0;
}
