/* @Author
Student Name : Emre Saraç
Date : 10.11.2023 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

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
// create a function for adding
void add_emp(int &id, int &size, Employee* new_emp_arr, Employee* emp_arr, int salary, int department) {
    // create a new array and move each employee data
    for (int i = 0; i < size; i++) {
        new_emp_arr[i] = emp_arr[i];
    }
    // set new employee's datum
    
    new_emp_arr[size].set_id(id + 1);
    new_emp_arr[size].set_salary(salary);
    new_emp_arr[size].set_department(department);
    size++;
    id++;
}
// create a function for updating
void update_emp(int &size, Employee* emp_arr, int id, int salary, int department) {
    // get index of the wanted employee
    int index;
    for (index = 0; index < size; index++) {
        if (emp_arr[index].get_id() == id) {
            break;
        }
    }
    // update salary and department values
    emp_arr[index].set_salary(salary);
    emp_arr[index].set_department(department);
}
// create a function for deleting
void delete_emp(int &size, Employee* new_emp_arr, Employee* emp_arr, int id) {
    // decreasing size by 1 and adding elements to another array
    int i;
    for (i = 0; i < size; i++) {
        if (emp_arr[i].get_id() == id) {
            break;
        }
        new_emp_arr[i] = emp_arr[i];
    }
    i++;
    for (int j = i; j < size; j++) {
        new_emp_arr[j-1] = emp_arr[j];
    }
    size--;
}
// create a search function
bool find_id(int size, int id, Employee* emp_arr) {
    for (int i = 0; i < size; i++) {
        if (emp_arr[i].get_id() == id) {
            return true;
        }
    }
    return false;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Wrong command line arguments (True usage: './main hw1.csv operations.txt')" << '\n';
        return EXIT_FAILURE;
    } 

    // take arguments
    string filename = argv[1];
    string operationfilename = argv[2];

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
    // get last element's id
    int id = size;

    // open the file and read header file
    file.open(filename);
    getline(file, header);

    // dynamically create an array
    Employee* emp_arr = new Employee[size];

    // read the file line by line
    int counter = 0;
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

        // set objects
        emp_arr[counter].set_id(id);
        emp_arr[counter].set_salary(salary);
        emp_arr[counter].set_department(department);
        
        counter++;
    }
    file.close();

    // open the operation csv file
    ifstream operation_file(operationfilename);

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
            // get operation and other integers
            if (i == 0) {
                operation = token;
            } else {
                int value = stoi(token);
                values[i-1] = value;
            }
            i++;
        }

        if (operation == "ADD") {
            // get salary and department value from txt file
            int salary = values[0];
            int department = values[1];

            // create new emp array
            Employee* new_emp_arr = new Employee[size + 1];

            // add new emp to array
            add_emp(id, size, new_emp_arr, emp_arr, salary, department);

            // create temp and swap
            Employee* temp = emp_arr;
            emp_arr = new_emp_arr;
            delete[] temp;
        }
        else if (operation == "UPDATE") {
            // get id, salary and department value from txt file
            int id = values[0];
            int salary = values[1];
            int department = values[2];

            // check the validity of the id
            if (!find_id(size, id, emp_arr)) {
                cout << "ERROR: An invalid ID to update";
                continue;
            }
            update_emp(size, emp_arr, id, salary, department);
        }
        else if (operation == "DELETE") {
            // get id value from the txt file
            int id = values[0];

            // check whether there is employee or not 
            if (size == 0) {
                cout << "ERROR: There is no Employee";
                continue;
            }
            // check the validity of the id
            else if(!find_id(size, id, emp_arr)) {
                cout << "ERROR: An invalid ID to delete";
                continue;
            }
            
            // create new emp arr with decreasing size by one
            Employee* new_emp_arr = new Employee[size - 1];
            delete_emp(size, new_emp_arr, emp_arr, id);

            // create temp and swap
            Employee* temp = emp_arr;
            emp_arr = new_emp_arr;
            delete[] temp;
        }
    }
    operation_file.close();

    // create output file for writing into csv
    ofstream outputFile("array_solution.csv");

    outputFile << header << '\n';
    // write data into this file
    for (int i = 0; i < size; i++) {
        outputFile << emp_arr[i].get_id() << ';' << emp_arr[i].get_salary() << ';' << emp_arr[i].get_department() << '\n';
    }
    outputFile.close();

    // deallocate memory
    delete[] emp_arr;
 
    return EXIT_SUCCESS;
}
