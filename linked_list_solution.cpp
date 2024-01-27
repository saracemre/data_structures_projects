/* @Author
Student Name : Emre Saraç
Date : 10.11.2023 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// create a Node class
class Node {
    private:
        int id;
        int salary;
        int department;
        Node* next;
    public:
        Node(int, int, int);
        ~Node();
        void set_salary(int);
        void set_id(int);
        void set_department(int);
        void set_next(Node*);
        int get_id();
        int get_salary();
        int get_department();
        Node* get_next();
};

// create a SingleLinkedList class
class SingleLinkedList {
    private:
        Node* head;
        Node* tail;
    public:
        SingleLinkedList();
        ~SingleLinkedList();
        Node* get_head();
        Node* get_tail();
        void set_head(Node*);
        void set_tail(Node*);
        void add_node(int, int, int);
        void remove_node(int);
        bool contains(int);
        Node* search(int);
        void print_list();
};

// create constructer
Node::Node(int id, int salary, int department) {
    this->id = id;
    this->salary = salary;
    this->department = department;
    this->next = NULL;
}
// create deconstructer
Node::~Node() {

} // functions for Node
Node* SingleLinkedList::get_head() {
    return this->head;
}
Node* SingleLinkedList::get_tail() {
    return this->tail;
}
void SingleLinkedList::set_head(Node* head) {
    this->head = head;
}
void SingleLinkedList::set_tail(Node* tail) {
    this->tail = tail;
}
void Node::set_salary(int salary) {
    this->salary = salary;
}
void Node::set_id(int id) {
    this->id = id;
}
void Node::set_department(int department) {
    this->department = department;
}
void Node::set_next(Node* next) {
    this->next = next;
}
int Node::get_id() {
    return this->id;
}
int Node::get_salary() {
    return this->salary;
}
int Node::get_department() {
    return this->department;
}
Node* Node::get_next() {
    return this->next;
}

// create constructor for SingleLinkedList
SingleLinkedList::SingleLinkedList() {
    head = NULL;
    tail = NULL;
}
SingleLinkedList::~SingleLinkedList() {

}
// functions for SingleLinkedList
void SingleLinkedList::add_node(int id, int salary, int department) {
    Node* new_node = new Node(id, salary, department);
    // if head equals to NULL, then we assign the new_node to head
    if (head == NULL) {
        head = new_node;
        tail = new_node;
    } else {
        tail->set_next(new_node);
        tail = tail->get_next();
    }
}
// a function for removing node
void SingleLinkedList::remove_node(int id) {
    Node* prev = this->search(id);
    if (head == tail) {
        delete head;
        head=NULL;
        tail=NULL;
    } else {
        if (prev == NULL) {
            Node* new_node = new Node(-1, -1, -1);
            new_node->set_next(head->get_next());
            delete head;
            head = new_node->get_next();
            delete new_node;
        } else {
            Node* n = prev->get_next();
            Node* next = n->get_next();
            n->set_next(NULL);
            delete n;
            prev->set_next(next);
        }
    }
}
// check if id is valid
bool SingleLinkedList::contains(int id) {
    Node* n = head;
    while (n != NULL) {
        if (n->get_id() == id) {
            return true;
        }
        n = n->get_next();
    }
    return false;
}
// get the previous of the searched
Node* SingleLinkedList::search(int id) {
    Node* n = head;
    if (head == NULL) {
        return NULL;
    }
    while (n->get_next() != NULL) {
        if (n->get_next()->get_id() == id) {
            return n;
        }
        n = n->get_next();
    }
    return NULL;
}
// a function for printing to console
void SingleLinkedList::print_list() {
    Node* n = head;
    while (n != NULL) {
        cout << n->get_id() << ';' << n->get_salary() << ';' << n->get_department() << '\n';
        n = n->get_next();
    }
}
// a function for loading file into a linked list
void load_sll(string filename, SingleLinkedList &sll) {
    // read the data of the csv file
    ifstream file(filename);

    // read the header line
    string header;
    getline(file, header);

    string line;
    // read the file line by line
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

        // add nodes
        sll.add_node(id, salary, department);
    }
    file.close();
}
// a function for getting size 
int get_size(string filename) {
    // read the csv file
    ifstream file(filename);

    // read the header line
    string header;
    getline(file, header);

    // get the number of employees
    int size = 0;
    string line;
    while (getline(file, line)) {
        size++;
    }
    file.close();

    return size;
}
// a function for performing operations 
void operation(string operationfilename, int &size, int &max_id, SingleLinkedList &sll) {
    // open the operation csv file
    ifstream operation_file(operationfilename);

    string line;
    // read the operation file line by line
    while(getline(operation_file, line)) {
        // create values array, ss and token for each line
        string operation;
        int values[3];
        istringstream ss(line);
        string token;

        // read line with delimeter semicolon
        int i = 0;
        while(getline(ss, token, ';')) {
            // get operation and other values
            if (i == 0) {
                operation = token;
            } else {
                int value = stoi(token);
                values[i-1] = value;
            }
            i++;
        }
        // check what is operation and process
        if (operation == "ADD") {
            size++;
            max_id++;
            int salary = values[0];
            int department = values[1];
            // add node to linked list
            sll.add_node(max_id, salary, department);
        } 
        else if (operation == "UPDATE") {
            int id = values[0];
            int salary = values[1];
            int department = values[2];
            // check whether id is valid or not
            if (sll.contains(id) == false) {
                cout << "ERROR: An invalid ID to update";
            } else {
                // search the node according to id, and update it
                Node* found_node = sll.search(id)->get_next();
                found_node->set_salary(salary);
                found_node->set_department(department);
            }
        } 
        else if (operation == "DELETE") {
            int id = values[0];
            // check if id is invalid and if linked list is empty
            if (sll.contains(id) == false) {
                cout << "ERROR: An invalid ID to delete";
            } else if (size == 0) {
                cout << "ERROR: There is no Employee";
            } else {
                sll.remove_node(id);
                size--;
            }
        }
    }
    // close the file
    operation_file.close();
}
// a function for loading data into a file
void load_file(string outputFileName, SingleLinkedList &sll) {
    // create an output file for writing into csv
    ofstream outputFile(outputFileName);
    // write the header
    outputFile << "Employee_ID;Salary;Department" << '\n';
    // write data and delete nodes one by one
    Node* n = sll.get_head();
    while (n != NULL) {
        Node* head = sll.get_head();
        outputFile << n->get_id() << ';' << n->get_salary() << ';' << n->get_department() << '\n';
        n = n->get_next();
        head->set_next(NULL);
        delete head;
        sll.set_head(n);
    }
    outputFile.close();
}

int main(int argc, char **argv) {
    // control the arguments
    if (argc != 3) {
        cerr << "Wrong command line arguments (True usage: './main hw1.csv operations.txt')" << '\n';
        return EXIT_FAILURE;
    }

    // get arguments
    string filename = argv[1];
    string operationfilename = argv[2];

    // we define SingleLinkedList and load file into linked list
    SingleLinkedList sll;
    load_sll(filename, sll);

    // get max id
    int max_id = sll.get_tail()->get_id();

    // get size of the employees
    int size = get_size(filename);

    // do operations
    operation(operationfilename, size, max_id, sll);

    // load a changed linked list into a file
    load_file("linked_list_solution.csv", sll);
    
    return EXIT_SUCCESS;
}
