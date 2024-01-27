/* @Author
Student Name : Emre Saraç
Date : 17.12.2023 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

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

int main(int argc, char **argv) {

    // take arguments
    string filename = argv[1];
    string operationfilename = argv[2];

    // read the csv file
    ifstream file(filename);

    // read the header line
    string header;
    getline(file, header);

    // create a map
    map<int, Employee*> employee_map;

    // store max id
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
        
        // add Employee to the map
        employee_map[id] = new_employee;

        // compare max id and added id
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
            
            // add new employee to the map
            employee_map[max_id] = new_employee;
        } 
        else if (operation == "UPDATE") {
            int id = stoi(tokens[1]);
            int salary = stoi(tokens[2]);
            int department = stoi(tokens[3]);

            // check if id is valid or not
            auto it = employee_map.find(id);
            if (it != employee_map.end()) {
                // update the data
                employee_map[id]->set_salary(salary);
                employee_map[id]->set_department(department);
            } else {
                cout << "ERROR: An invalid ID to update\n";
            }
        }
        else if (operation == "DELETE") {
            int id = stoi(tokens[1]);

            // delete employee data
            // check if map is empty
            if (employee_map.empty()) {
                cout << "ERROR: There is no Employee\n";
            } else {
                // check if id is valid or not
                auto it = employee_map.find(id);
                if (it != employee_map.end()) {
                    delete employee_map[id];
                    employee_map.erase(it);
                } else {
                    cout << "ERROR: An invalid ID to delete\n";
                }
            }
        }
    }
    operation_file.close();

    // open the output file write into it and close it
    
    ofstream outputFile("output.csv");
    // write the header
    outputFile << "Employee_ID;Salary;Department\n";

    for (const auto& pair: employee_map) {
        outputFile << pair.second->get_id() << ';' << pair.second->get_salary() << ';' << pair.second->get_department() << '\n';
        delete employee_map[pair.second->get_id()];
    }

    outputFile.close();

    return EXIT_SUCCESS;
}
