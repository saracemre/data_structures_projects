/* @Author
Student Name : Emre Saraç
Date : 06.01.2024 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <random>

using namespace std;

const int MAX_INT = 2147483647;

// create an Employee class
class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        Employee(int i, int s, int d) {this->id = i; this->salary = s; this->department = d;}; // constructor
        // setters and getters
        void set_id(int id)                 {this->id = id;};
        void set_salary(int salary)         {this->salary = salary;};
        void set_department(int department) {this->department = department;};
        int get_id()                        {return this->id;};
        int get_salary()                    {return this->salary;};
        int get_department()                {return this->department;};
};

// create a QuadruplySkipList_Node class
class QuadruplySkipList_Node {
    private:
        Employee* data;
        QuadruplySkipList_Node* next;
        QuadruplySkipList_Node* prev;
        QuadruplySkipList_Node* below;
        QuadruplySkipList_Node* above;
public:
    QuadruplySkipList_Node(Employee *data)          {this->data = data; this->next = NULL; this->prev = NULL; this->below = NULL; this->above = NULL;}; // constructor
    // setters and getters
    void set_data(Employee* data)                   {this->data = data;};
    void set_next(QuadruplySkipList_Node* next)     {this->next = next;};
    void set_prev(QuadruplySkipList_Node* prev)     {this->prev = prev;};
    void set_below(QuadruplySkipList_Node* below)   {this->below = below;};
    void set_above(QuadruplySkipList_Node* above)   {this->above = above;};
    Employee* get_data()                            {return this->data;};
    QuadruplySkipList_Node* get_next()              {return this->next;};
    QuadruplySkipList_Node* get_prev()              {return this->prev;};
    QuadruplySkipList_Node* get_below()             {return this->below;};
    QuadruplySkipList_Node* get_above()             {return this->above;};
};

// create a QuadruplySkipList class
class QuadruplySkipList{
    private:
        QuadruplySkipList_Node* head;
        int height;
    public:
        QuadruplySkipList(int height_in);
        ~QuadruplySkipList();
        void insert(Employee* data);
        void remove(int remove_id);
        Employee* search(int search_id);
        void dumpToFile(ofstream& out_file);

        bool empty();
};
// constructor
QuadruplySkipList::QuadruplySkipList(int height_in) {
    this->height = height_in;

    // create a head which is negative
    Employee* head_emp = new Employee(-1, 0, 0);
    QuadruplySkipList_Node* head_node = new QuadruplySkipList_Node(head_emp);
    head = head_node;
    QuadruplySkipList_Node* temp = head;
    // insert height - 1 nodes below the head
    for (int i = 0; i < height-1; i++) {
        QuadruplySkipList_Node* node = new QuadruplySkipList_Node(head_emp);
        temp->set_below(node);
        node->set_above(temp);
        temp = temp->get_below();
    }

    // create a last node which is max_int
    Employee* tail_emp = new Employee(MAX_INT, 0, 0);
    // traverse all nodes in first column and add tails
    QuadruplySkipList_Node* tail_node = new QuadruplySkipList_Node(tail_emp);
    QuadruplySkipList_Node* head_temp = head;
    head_temp->set_next(tail_node);
    tail_node->set_prev(head_temp);
    head_temp = head_temp->get_below();
    while (head_temp != NULL) {
        QuadruplySkipList_Node* node = new QuadruplySkipList_Node(tail_emp);
        head_temp->set_next(node);
        node->set_prev(head_temp);
        tail_node->set_below(node);
        node->set_above(tail_node);

        head_temp = head_temp->get_below();
        tail_node = tail_node->get_below();
    }
}
// destructor
QuadruplySkipList::~QuadruplySkipList() {
    QuadruplySkipList_Node* temp = head;
    while (temp->get_below()) {
        QuadruplySkipList_Node* other_temp = temp->get_below();
        while (temp->get_next()) {
            temp = temp->get_next();
            delete temp->get_prev();
        }
        delete temp;

        temp = other_temp;
    }

    while (temp->get_next()) {
        temp = temp->get_next();
        delete temp->get_prev()->get_data();
        delete temp->get_prev();
    }
    delete temp->get_data();
    delete temp;
}
// insert function
void QuadruplySkipList::insert(Employee* data) {
    // while traversing, store all the previous nodes which can be previous of new nodes
    QuadruplySkipList_Node* temp = head;
    QuadruplySkipList_Node* prev_array[height];
    int index = 0;
    while (temp != NULL) {
        if (data->get_id() > temp->get_next()->get_data()->get_id()) {
            temp = temp->get_next();
        } else {
            prev_array[index] = temp;
            temp = temp->get_below();
            index++;
        }
    }

    // create a new node and insert it with the probability of 1/2 with using previous array
    QuadruplySkipList_Node* new_node = new QuadruplySkipList_Node(data);
    QuadruplySkipList_Node* prev_node = prev_array[height - 1];
    new_node->set_prev(prev_node);
    new_node->set_next(prev_node->get_next());
    prev_node->set_next(new_node);
    new_node->get_next()->set_prev(new_node);
    for (int i = height - 2; i >= 0; i--) {
        // seed the random number generator
        random_device rd;
        mt19937 gen(rd());
        uniform_real_distribution<> dis(0.0, 1.0);

        // generate a random number between 0 and 1
        double probability = dis(gen);

        // add node with 0.5 probability
        if (probability < 0.5) {
            return;
        }
        // add node
        QuadruplySkipList_Node* prev_node = prev_array[i];
        QuadruplySkipList_Node* node = new QuadruplySkipList_Node(data);
        node->set_prev(prev_node);
        node->set_next(prev_node->get_next());
        prev_node->set_next(node);
        node->get_next()->set_prev(node);
        new_node->set_above(node);
        node->set_below(new_node);

        new_node = new_node->get_above();
    }
}
// delete function
void QuadruplySkipList::remove(int remove_id) {
    // check if skip list is empty
    if (this->empty()) {
        cout << "ERROR: There is no Employee\n";
        return;
    }
    // find the node whose id is remove_id
    QuadruplySkipList_Node* temp = head;
    while (temp != NULL) {
        if (remove_id > temp->get_next()->get_data()->get_id()) {
            temp = temp->get_next();
        } else if (remove_id < temp->get_next()->get_data()->get_id()) {
            temp = temp->get_below();
        } else {
            temp = temp->get_next();
            break;
        }
    }
    // check if id is valid
    if (temp == NULL) {
        cout << "ERROR: An invalid ID to delete\n";
        return;
    }

    // delete an employee and delete a node
    delete temp->get_data();
    temp->set_data(NULL);
    while (temp != NULL) {
        temp->get_prev()->set_next(temp->get_next());
        temp->get_next()->set_prev(temp->get_prev());
        temp->set_next(NULL);
        temp->set_prev(NULL);
        temp->set_above(NULL);
        QuadruplySkipList_Node* temp2 = temp;
        temp = temp->get_below();
        temp2->set_below(NULL);

        delete temp2;
    }
}
// search function
Employee* QuadruplySkipList::search(int search_id) {
    // search the node whose id is search_id
    QuadruplySkipList_Node* temp = head;
    while (temp != NULL) {
        if (search_id > temp->get_next()->get_data()->get_id()) {
            temp = temp->get_next();
        } else if (search_id < temp->get_next()->get_data()->get_id()) {
            temp = temp->get_below();
        } else {
            // return searched employee pointer
            temp = temp->get_next();
            return temp->get_data();
        }
    }
    // if temp == NULL, then return NULL
    return NULL;
}
// a function to load data to a file
void QuadruplySkipList::dumpToFile(ofstream& out_file) {
    // write the header
    out_file << "Employee_ID;Salary;Department\n";

    // go down and traverse all nodes 
    QuadruplySkipList_Node* temp = head;
    while (temp->get_below() != NULL) {
        temp = temp->get_below();
    }

    // go to the first node
    temp = temp->get_next();

    // traverse until the max node
    while (temp->get_data()->get_id() != MAX_INT) {
        // write the current node and delete it
        out_file << temp->get_data()->get_id() << ';' << temp->get_data()->get_salary() << ';' << temp->get_data()->get_department() << '\n';
        temp = temp->get_next();
    }    
}
// check whether skip list is empty
bool QuadruplySkipList::empty() {
    // go down of first columns
    QuadruplySkipList_Node* temp = head;
    while (temp->get_below() != NULL) {
        temp = temp->get_below();
    }

    // if next is max node, then the skip list is empty
    if (temp->get_next()->get_data()->get_id() == MAX_INT)
        return true;

    return false;
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

    // create QuadruplySkipList object with fixed height
    QuadruplySkipList* qsl = new QuadruplySkipList(20);

    // create max id varible for adding
    int max_id = 0;

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
        qsl->insert(new_employee);

        // get max id
        max_id = max(max_id, id);
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
            int salary = stoi(tokens[1]);
            int department = stoi(tokens[2]);

            // create a new employee
            Employee* new_employee = new Employee(max_id, salary, department);

            // insert
            qsl->insert(new_employee);
        } 
        else if (operation == "UPDATE") {
            int id = stoi(tokens[1]);
            int salary = stoi(tokens[2]);
            int department = stoi(tokens[3]);

            // get employee
            Employee* employee = qsl->search(id);

            // check whether id is valid or not
            if (employee == NULL) {
                cout << "ERROR: An invalid ID to update\n";
            } else {
                // update data
                employee->set_salary(salary);
                employee->set_department(department);
            }
        }
        else if (operation == "DELETE") {
            int id = stoi(tokens[1]);
            // delete employee data
            qsl->remove(id);
        }
    }
    operation_file.close();

    // open the output file write into it and close it
    ofstream outputFile("quadruply_output.csv");
    qsl->dumpToFile(outputFile);
    outputFile.close();

    delete qsl;

    return EXIT_SUCCESS;
}
