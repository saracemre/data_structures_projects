/* @Author
Student Name : Emre Saraç
Date : 17.12.2023 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

// create an Employee class
class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d);
        ~Employee();
        void set_salary(int);
        void set_id(int);
        void set_department(int);
        int get_id();
        int get_salary();
        int get_department();
};
// create constructer
Employee::Employee(int id, int salary, int department) {
    this->id = id;
    this->salary = salary;
    this->department = department;
}
// create destructor
Employee::~Employee() {

}
// create setters and getters
void Employee::set_salary(int salary) {
    this->salary = salary;
}
void Employee::set_id(int id) {
    this->id = id;
}
void Employee::set_department(int department) {
    this->department = department;
}
int Employee::get_id() {
    return this->id;
}
int Employee::get_salary() {
    return this->salary;
}
int Employee::get_department() {
    return this->department;
}

// create a Node class
class Node {
    private:
        Employee* employee;
        Node* left;
        Node* right;
    public:
        Node(Employee*);
        ~Node();
        void set_employee(Employee*);
        void set_left(Node*);
        void set_right(Node*);
        Employee* get_employee();
        Node* get_left();
        Node* get_right();
};
// create constructer
Node::Node(Employee* employee) {
    this->employee = employee;
    this->left = NULL;
    this->right = NULL;
}
// creates destructor
Node::~Node() {
    delete this->get_left();
    delete this->get_right();
    delete this->get_employee();
}
// create setters and getters
void Node::set_employee(Employee* employee) {
    this->employee = employee;
}
void Node::set_left(Node* left) {
    this->left = left;
}
void Node::set_right(Node* right) {
    this->right = right;
}
Employee* Node::get_employee() {
    return this->employee;
}
Node* Node::get_left() {
    return this->left;
}
Node* Node::get_right() {
    return this->right;
}

// create a BinaryTree class
class BinaryTree { 
    private:
        Node *root; 
    public:
        BinaryTree();
        ~BinaryTree();
        void insert(Employee* employee);
        void remove(int id);
        Employee *search(int id);
        int getHeight();
        void printToFile(ofstream &output_file);
        void printToConsole(); 
};
// create constructor
BinaryTree::BinaryTree() {
    this->root = NULL;
}
// create destructor
BinaryTree::~BinaryTree() {
    delete this->root;
}
// add function
void BinaryTree::insert(Employee* employee) {
    
    Node* n = new Node(employee);

    // if root is NULL, then root equals employee node
    if (this->root == NULL) {
        this->root = n;
        return;
    }
    // if root is not NULL, then traverse binary tree and insert the employee node
    else {
        Node* temp = this->root;
        while (true) {
            // check whether employee id is greater
            if (employee->get_id() > temp->get_employee()->get_id()) {
                if (temp->get_right() == NULL) {
                    temp->set_right(n);
                    return;
                }
                temp = temp->get_right();
            } 
            // check whether employee id is less
            else if (employee->get_id() < temp->get_employee()->get_id()) {
                if (temp->get_left() == NULL) {
                    temp->set_left(n);
                    return;
                }
                temp = temp->get_left();
            }
            // if equals, delete n and do not do anything 
            else {
                delete n;
                return;
            }
        }
    }
}
// search function
Employee* BinaryTree::search(int id) {
    // if root equals NULL, return NULL
    if (this->root == NULL) {
        return NULL;
    } else {
        Node* temp = root;
        while (temp != NULL) {
            if (id > temp->get_employee()->get_id()) {
                temp = temp->get_right();
            } else if (id < temp->get_employee()->get_id()) {
                temp = temp->get_left();
            } else {
                return temp->get_employee();
            }
        }
    }
    return NULL;
}
// delete function
void BinaryTree::remove(int id) {
    // check whether root is NULL
    if (this->root == NULL) {
        cout << "ERROR: An invalid ID to delete\n";
        return;
    }

    Node* parent = NULL;
    Node* current = this->root;

    // find node to be removed and parent of that node
    while (current != NULL) {
        if (current->get_employee()->get_id() == id) {
            break;
        }

        parent = current;
        if (id < current->get_employee()->get_id()) {
            current = current->get_left();
        } else if (id > current->get_employee()->get_id()) {
            current = current->get_right();
        }
    }

    // check if id is valid
    if (current == NULL) {
        cout << "ERROR: An invalid ID to delete\n";
        return;
    }

    // 1: node with one child or no child
    // if child is left or no child
    if (current->get_right() == NULL) {
        // get left of the node to be removed and set it's left to NULL
        Node* temp = current->get_left();
        current->set_left(NULL);

        // if node to be removed is root
        if (parent == NULL) {
            this->root = temp;
        } // if node to be removed is left
        else if (parent->get_left() == current) {
            parent->set_left(temp);
        } // if node to be removed is right
        else if (parent->get_right() == current) {
            parent->set_right(temp);
        }

        delete current;
        return;
    }
    // if child is right
    else if (current->get_left() == NULL) {
        // get right of the node to be removed and set it's right to NULL
        Node* temp = current->get_right();
        current->set_right(NULL);

        // if node to be removed is root
        if (parent == NULL) {
            this->root = temp;
        } // if node to be removed is left
        else if (parent->get_left() == current) {
            parent->set_left(temp);
        } // if node to be removed is right
        else if (parent->get_right() == current) {
            parent->set_right(temp);
        }

        delete current;
        return;
    }

    // 2: node with two children
    else {
        // find the smallest node in the right subtree and its parent
        Node* successor = current->get_right();
        Node* successor_parent = current;
        while (successor != NULL && successor->get_left() != NULL) {
            successor_parent = successor;
            successor = successor->get_left();
        }

        // create a new employee which is same as the successor's employee
        Employee* successor_emp = new Employee(successor->get_employee()->get_id(), successor->get_employee()->get_salary(), successor->get_employee()->get_department());

        // delete the successor which has one child (left child) or no child
        // get right of the node to be removed and set it's right to NULL
        Node* successor_temp = successor->get_right();
        successor->set_right(NULL);

        // if node to be removed is left
        if (successor_parent->get_left() == successor) {
            successor_parent->set_left(successor_temp);
        } // if node to be removed is right
        else if (successor_parent->get_right() == successor) {
            successor_parent->set_right(successor_temp);
        }

        delete successor;        

        // delete old employee 
        delete current->get_employee();

        // copy successor to current
        current->set_employee(successor_emp);
        return;
    }
}
// a function fo load data into a file
void BinaryTree::printToFile(ofstream &output_file) {
    string content = "";
    // inorder traverse
    if (this->root == NULL) {
        return;
    }
    stack<Node*> s;
    Node* current = this->root;

    while (current != NULL || !s.empty()) {
        // traverse the leftmost
        while (current != NULL) {
            s.push(current);
            current = current->get_left();
        }

        // get the top and pop it and write it 
        current = s.top();
        s.pop();
        output_file << current->get_employee()->get_id() << ';' << current->get_employee()->get_salary() << ';' << current->get_employee()->get_department() << '\n';

        // go to the right subtree
        current = current->get_right();
    }
}
int main(int argc, char **argv) {

    // take arguments
    string filename = argv[1];
    string operationfilename = argv[2];

    // read the csv file
    ifstream file(filename);

    // read the header line
    string header;
    getline(file, header);

    // create binary tree size
    const int BT_SIZE = 5000;
    // create vector which will store BinaryTree objects (bt_vector[i] = id[i*5000 + 1, (i+1)*5000])
    vector<BinaryTree*> bt_vector;

    // create max id varible for adding
    int max_id = 0;
    int size = 0;

    // read the file line by line and build BinaryTree object
    string line;
    while(getline(file, line)) {
        // create values array, ss and token for each line
        int values[3];
        istringstream ss(line);
        string token;

        // read line with delimeter semicolon
        int i = 0;
        while(getline(ss, token, ';')) {
            // convert string to integer and load into values array
            int value = stoi(token);
            values[i] = value;
            i++;
        }
        int id = values[0];
        int salary = values[1];
        int department = values[2];

        // create Employee object based on values
        Employee* new_employee = new Employee(id, salary, department);

        // insert

        // get index of binary tree and add to it (if size is less, it must be increased)
        int bt_index = (id-1) / BT_SIZE; // (1 id -> bt0, 5000 id -> bt0)
        int vector_size = bt_vector.size();
        for (int j = bt_index; j > vector_size - 1; j--) {
            BinaryTree* new_bt = new BinaryTree();
            bt_vector.push_back(new_bt);
        }
        bt_vector[bt_index]->insert(new_employee);

        // get max id
        max_id = max(max_id, id);
        size++;
    }
    file.close();


    // DO OPERATIONS
    // open the operation csv file
    ifstream operation_file(operationfilename);

    string operation_line;
    // read the operation file line by line
    while(getline(operation_file, line)) {
        // create values array, ss and token for each line
        string tokens[4];
        istringstream ss(line);
        string token;

        int i = 0;
        // read line with delimeter semicolon
        while(getline(ss, token, ';')) {
            // store tokens into array
            tokens[i] = token;
            i++;
        }
        string operation = tokens[0];
        // check what is operation and process
        if (operation == "ADD") {
            max_id++;
            size++;
            int salary = stoi(tokens[1]);
            int department = stoi(tokens[2]);

            // create new employee
            Employee* new_employee = new Employee(max_id, salary, department);

            // insert

            // get binary tree index and insert (if need, expand the vector)
            int bt_index = (max_id-1) / BT_SIZE;
            if (bt_index > (int) bt_vector.size() - 1) {
                bt_vector.push_back(new BinaryTree());
            }
            bt_vector[bt_index]->insert(new_employee);
        } 
        else if (operation == "UPDATE") {
            int id = stoi(tokens[1]);
            int salary = stoi(tokens[2]);
            int department = stoi(tokens[3]);

            // check if index of binary tree is valid
            int bt_index = (id-1) / BT_SIZE;
            if (bt_index > (int) bt_vector.size() - 1) {
                cout << "ERROR: An invalid ID to update\n";
            }
            else {
                // get employee
                Employee* employee = bt_vector[bt_index]->search(id);

                // check whether id is valid or not
                if (employee == NULL) {
                    cout << "ERROR: An invalid ID to update\n";
                } else {
                    // update data
                    employee->set_salary(salary);
                    employee->set_department(department);
                }
            }
            
        }
        else if (operation == "DELETE") {
            int id = stoi(tokens[1]);
            // delete employee data
            // check if there is an employee
            if (size == 0) {
                cout << "ERROR: There is no Employee\n";
            } else {
                // get index of binary tree and check if it is valid
                int bt_index = (id-1) / BT_SIZE;
                if (bt_index > (int) bt_vector.size() - 1) {
                    cout << "ERROR: An invalid ID to delete\n";
                } else {
                    // delete the node
                    bt_vector[bt_index]->remove(id);
                    size--;
                }
            }
        }
    }
    operation_file.close();

    // open the output file, write into it and close it
    
    ofstream outputFile("output.csv");
    // write the header
    outputFile << "Employee_ID;Salary;Department\n";

    // write each binary tree and delete
    for (BinaryTree* bt: bt_vector) {
        bt->printToFile(outputFile);
        delete bt;
    }
    outputFile.close();

    return EXIT_SUCCESS;
}
