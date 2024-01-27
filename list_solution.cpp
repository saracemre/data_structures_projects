/* @Author
Student Name : Emre Saraç
Date : 10.11.2023 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <list>

using namespace std;

// create an Employee class
class Employee {
    private:
        int id;
        int salary;
        int department;
    public:
        Employee();
        ~Employee();
        void set_salary(int);
        void set_id(int);
        void set_department(int);
        int get_id();
        int get_salary();
        int get_department();
};
// create constructer
Employee::Employee() {

}
// create deconstructer
Employee::~Employee() {
    
}
// setters and getters
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
// a function to load a list
void load_list(string filename, list<Employee> &emp_list) {
    // read the csv file
    ifstream file(filename);

    // read the header line
    string header;
    getline(file, header);

    // read the file line by line
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
        // get data
        int id = values[0];
        int salary = values[1];
        int department = values[2];

        // create an Employee object
        Employee emp;
        emp.set_id(id);
        emp.set_salary(salary);
        emp.set_department(department);
        // load the list
        emp_list.push_back(emp);
    }
    file.close();
}
// check whether id is valid or not
bool contains(int &size, int id, list<Employee> &emp_list) {
    for (Employee &emp: emp_list) {
        if (emp.get_id() == id) {
            return true;
        }
    }
    return false;
}
// add function
void add_emp(int &size, int &max_id, int salary, int department, list<Employee> &emp_list) {
    max_id++;

    // create an Employee object
    Employee emp;
    emp.set_id(max_id);
    emp.set_salary(salary);
    emp.set_department(department);

    // push the Employee to list
    emp_list.push_back(emp);

    size++;
}
// update function
void update_emp(int id, int salary, int department, int size, list<Employee> &emp_list) {
    // set Employee object
    Employee updated_emp;
    updated_emp.set_id(id);
    updated_emp.set_salary(salary);
    updated_emp.set_department(department);

    // search for wanted employee
    for (Employee &emp: emp_list) {
        if (emp.get_id() == id) {
            emp = updated_emp;
            break;
        }
    }
}
// delete function
void delete_emp(int &size, int id, list<Employee> &emp_list) {
    // search for wanted employee
    for (auto it = emp_list.begin(); it != emp_list.end(); ++it) {
        if (it->get_id() == id) {
            emp_list.erase(it);
            break;
        }
    }
    size--;
}
// operation function
void operation(string operationfilename, int &size, int &max_id, list<Employee> &emp_list) {
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
            int salary = values[0];
            int department = values[1];
            add_emp(size, max_id, salary, department, emp_list);
        }
        else if (operation == "UPDATE") {
            int id = values[0];
            int salary = values[1];
            int department = values[2];

            // check if id is valid or not
            if (!contains(size, id, emp_list)){
                cout << "ERROR: An invalid ID to update";
            } else {
                update_emp(id, salary, department, size, emp_list);
            }    
        }
        else if (operation == "DELETE") {
            int id = values[0];
          
            // check if is is valid or not
            if (size == 0) {
                cout << "ERROR: There is no Employee";
            } 
            else if (!contains(size, id, emp_list)){
                cout << "ERROR: An invalid ID to delete";
            }else {
                delete_emp(size, id, emp_list);
            }
        }
    }
    operation_file.close();
}
// load the file
void load_file(int &size, list<Employee> &emp_list) {
    // create an output file for writing into csv
    ofstream outputFile("list_solution.csv");

    // create a content
    string content = "Employee_ID;Salary;Department\n";

    for (auto it = emp_list.begin(); it != emp_list.end(); ++it) {
        // convert values to strings and then load into content
        content += to_string(it->get_id()) + ';' + to_string(it->get_salary()) + ';' + to_string(it->get_department())  + '\n';
    }
    // write the content to the output file
    outputFile << content;
    
    outputFile.close();
}


int main(int argc, char **argv) {

    if (argc != 3) {
        cerr << "Wrong command line arguments (True usage: './main <csv_filename> <operation_filename>')" << '\n';
        return EXIT_FAILURE;
    } 
    // take arguments
    string filename = argv[1];
    string operationfilename = argv[2];

    // create a list
    list<Employee> emp_list;

    // load the list
    load_list(filename, emp_list);

    // get the max id and the size
    int max_id = emp_list.size();
    int size = emp_list.size();
    
    // do operations
    operation(operationfilename, size, max_id, emp_list);

    // load file
    load_file(size, emp_list);
    
    return EXIT_SUCCESS;
}
