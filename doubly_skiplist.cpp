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

// create a DoublySkipList_Node class
class DoublySkipList_Node {
    private:
        Employee* data;
        DoublySkipList_Node* next;
        DoublySkipList_Node* below;
public:
    DoublySkipList_Node(Employee *data)          {this->data = data; this->next = NULL; this->below = NULL;}; // constructor
    // setters and getters
    void set_data(Employee* data)                {this->data = data;};
    void set_next(DoublySkipList_Node* next)     {this->next = next;};
    void set_below(DoublySkipList_Node* below)   {this->below = below;};
    Employee* get_data()                         {return this->data;};
    DoublySkipList_Node* get_next()              {return this->next;};
    DoublySkipList_Node* get_below()             {return this->below;};
};

// create a DoublySkipList class
class DoublySkipList{
    private:
        DoublySkipList_Node* head;
        int height;
    public:
        DoublySkipList(int height_in);
        ~DoublySkipList();
        void insert(Employee* data);
        void remove(int remove_id);
        Employee* search(int search_id);
        void dumpToFile(ofstream& out_file);

        bool empty();
};
// constructor
DoublySkipList::DoublySkipList(int height_in) {
    this->height = height_in;

    // create a head which is negative
    Employee* head_emp = new Employee(-1, 0, 0);
    DoublySkipList_Node* head_node = new DoublySkipList_Node(head_emp);
    head = head_node;
    DoublySkipList_Node* temp = head;
    // insert height - 1 nodes below the head
    for (int i = 0; i < height-1; i++) {
        DoublySkipList_Node* node = new DoublySkipList_Node(head_emp);
        temp->set_below(node);
        temp = temp->get_below();
    }

    // create a last node which is max_int
    Employee* tail_emp = new Employee(MAX_INT, 0, 0);
    // traverse all nodes in first column and add tails
    DoublySkipList_Node* tail_node = new DoublySkipList_Node(tail_emp);
    DoublySkipList_Node* head_temp = head;
    head_temp->set_next(tail_node);
    head_temp = head_temp->get_below();
    while (head_temp != NULL) {
        DoublySkipList_Node* node = new DoublySkipList_Node(tail_emp);
        head_temp->set_next(node);
        tail_node->set_below(node);

        head_temp = head_temp->get_below();
        tail_node = tail_node->get_below();
    }
}
// destructor
DoublySkipList::~DoublySkipList() {
    DoublySkipList_Node* temp = head;
    while (temp->get_below()) {
        DoublySkipList_Node* other_temp = temp->get_below();
        while (temp->get_next()) {
            DoublySkipList_Node* another_temp = temp;
            temp = temp->get_next();
            delete another_temp;
        }
        delete temp;

        temp = other_temp;
    }

    while (temp->get_next()) {
        DoublySkipList_Node* another_temp = temp;
        temp = temp->get_next();
        delete another_temp->get_data();
        delete another_temp;
    }
    delete temp->get_data();
    delete temp;
}
// insert function
void DoublySkipList::insert(Employee* data) {
    // while traversing, store all the previous nodes which can be previous of new nodes
    DoublySkipList_Node* temp = head;
    DoublySkipList_Node* prev_array[height];
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
    DoublySkipList_Node* new_node = new DoublySkipList_Node(data);
    DoublySkipList_Node* prev_node = prev_array[height - 1];
    new_node->set_next(prev_node->get_next());
    prev_node->set_next(new_node);
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
        DoublySkipList_Node* prev_node = prev_array[i];
        DoublySkipList_Node* node = new DoublySkipList_Node(data);
        node->set_next(prev_node->get_next());
        prev_node->set_next(node);
        node->set_below(new_node);

        new_node = prev_node->get_next();
    }
}
// delete function
void DoublySkipList::remove(int remove_id) {
    // check is skip list empty
    if (this->empty()) {
        cout << "ERROR: There is no Employee\n";
        return;
    }
    // find the node whose id is remove_id
    DoublySkipList_Node* temp = head;
    DoublySkipList_Node* prev;
    while (temp != NULL) {
        if (remove_id > temp->get_next()->get_data()->get_id()) {
            temp = temp->get_next();
        } else if (remove_id < temp->get_next()->get_data()->get_id()) {
            temp = temp->get_below();
        } else {
            prev = temp;
            temp = temp->get_next();
            break;
        }
    }
    // check if id is valid
    if (temp == NULL) {
        cout << "ERROR: An invalid ID to delete\n";
        return;
    }

    // delete an employee and delete a nodes
    delete temp->get_data();
    temp->set_data(NULL);
    while (temp != NULL) {
        prev->set_next(temp->get_next());
        temp->set_next(NULL);
        DoublySkipList_Node* temp2 = temp;
        temp = temp->get_below();
        // find previous node of below node
        prev = prev->get_below();
        while (prev != NULL) {
            if (prev->get_next() == temp) {
                break;
            }
            prev = prev->get_next();
        }
        temp2->set_below(NULL);

        delete temp2;
    }
}
// search function
Employee* DoublySkipList::search(int search_id) {
    // search the node whose id is search_id
    DoublySkipList_Node* temp = head;
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
// a function for loading data into a file
void DoublySkipList::dumpToFile(ofstream& out_file) {
    // write the header
    out_file << "Employee_ID;Salary;Department\n";

    // go down and traverse all nodes 
    DoublySkipList_Node* temp = head;
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
// a function checking whether skip list is empty
bool DoublySkipList::empty() {
    // go down of first columns
    DoublySkipList_Node* temp = head;
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

    // create DoublySkipList object with fixed height
    DoublySkipList* qsl = new DoublySkipList(20);

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
                // update employee data
                employee->set_salary(salary);
                employee->set_department(department);
            }
        }
        else if (operation == "DELETE") {
            int id = stoi(tokens[1]);
            // delete employee
            qsl->remove(id);
        }
    }
    operation_file.close();

    // open the output file, write into it and close it
    ofstream outputFile("doubly_output.csv");
    qsl->dumpToFile(outputFile);
    outputFile.close();

    delete qsl;

    return EXIT_SUCCESS;
}
