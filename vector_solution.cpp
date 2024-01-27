/* @Author
Student Name : Emre Saraç
Date : 10.11.2023 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

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
// a function to load a vector
void load_vector(string filename, vector<Employee> &emp_vector) {
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
        // load the vector
        emp_vector.push_back(emp);
    }
    file.close();
}
// check whether id is valid or not
bool contains(int &size, int id, vector<Employee> &emp_vector) {
    for (int i = 0; i < size; i++) {
        if (emp_vector[i].get_id() == id) {
            return true;
        }
    }
    return false;
}
// find index for a wanted Employee
int find_index(int &size, int id, vector<Employee> &emp_vector) {
    Employee emp;
    int index = 0;
    for (index = 0; index < size; index++) {
        emp = emp_vector[index];
        if (emp.get_id() == id) {
            break;
        }
    }
    return index;
}
// add function
void add_emp(int &size, int &max_id, int salary, int department, vector<Employee> &emp_vector) {
    max_id++;

    // create an Employee object
    Employee emp;
    emp.set_id(max_id);
    emp.set_salary(salary);
    emp.set_department(department);

    // push the Employee to vector
    emp_vector.push_back(emp);

    size++;
}
// update function
void update_emp(int id, int salary, int department, int size, vector<Employee> &emp_vector) {
    // set Employee object
    Employee emp;
    emp.set_id(id);
    emp.set_salary(salary);
    emp.set_department(department);

    // find the index of an wanted element
    int index = find_index(size, id, emp_vector);
    emp_vector[index] = emp;
}
// delete function
void delete_emp(int &size, int id, vector<Employee> &emp_vector) {
    int index = find_index(size, id, emp_vector);
    emp_vector.erase(emp_vector.begin() + index);
    size--;
}
// operation function
void operation(string operationfilename, int &size, int &max_id, vector<Employee> &emp_vector) {
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
            add_emp(size, max_id, salary, department, emp_vector);
        }
        else if (operation == "UPDATE") {
            int id = values[0];
            int salary = values[1];
            int department = values[2];

            // check if id is valid or not
            if (!contains(size, id, emp_vector)){
                cout << "ERROR: An invalid ID to update";
            } else {
                update_emp(id, salary, department, size, emp_vector);
            }    
        }
        else if (operation == "DELETE") {
            int id = values[0];

            // check if vector is empty or not
            if (size == 0) {
                cout << "ERROR: There is no Employee";
            } // check validity of id
            else if (!contains(size, id, emp_vector)){
                cout << "ERROR: An invalid ID to delete";
            }
            else {
                delete_emp(size, id, emp_vector);
            }
        }
    }
    operation_file.close();
}
// loading file
void load_file(int &size, vector<Employee> &emp_vector) {
    // create an output file for writing into csv
    ofstream outputFile("vector_solution.csv");

    // write data
    string content = "Employee_ID;Salary;Department\n";
    for (int i = 0; i < size; i++) {
        // convert values to string and then add the content
        content += to_string(emp_vector[i].get_id()) + ';' + to_string(emp_vector[i].get_salary()) + ';' + to_string(emp_vector[i].get_department())  + '\n';
    }
    // write the content to the file
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

    // create a vector with
    vector<Employee> emp_vector;

    // load the vector
    load_vector(filename, emp_vector);

    // get the max id and the size
    int max_id = emp_vector.size();
    int size = emp_vector.size();

    // perform operations
    operation(operationfilename, size, max_id, emp_vector);

    // load file
    load_file(size, emp_vector);
    
    return EXIT_SUCCESS;
}
