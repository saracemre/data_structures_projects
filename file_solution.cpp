/* @Author
Student Name : Emre Saraç
Date : 26.10.2023 */

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
    if (argc != 3) {
        cerr << "Wrong command line arguments (True usage: './main hw1.csv operations.txt')" << "\n";
        return EXIT_FAILURE;
    } 
    // get file names
    char* filename = argv[1];
    string operationfilename = argv[2];

    // read the csv file
    ifstream file(filename);

    // read the header line
    string header;
    getline(file, header);

    // create output file for writing into csv
    ofstream outputFile("file_solution.csv");

    int max_id;
    string line;
    string content;
    while(getline(file, line)) {
        // create id, ss and token for each line
        int id;
        istringstream ss(line);
        string token;

        // read line with delimeter semicolon
        int i = 0;
        while(getline(ss, token, ';')) {
            // convert string to integer and load into values array
            int value = stoi(token);
            if (i == 0) {
                id = value;
            }
            i++;
        }
        // get last id    
        max_id = id;

        // adding line to content
        content += line + "\n";
    }
    // write all content into the output file
    outputFile.write(content.c_str(), content.size());
    // close files
    file.close();
    outputFile.close();
    
    // read the operation file line by line
    ifstream operation_file(operationfilename);
    while(getline(operation_file, line)) {
        // create values, ss and token for each line
        string operation;
        int id, salary, department;
        istringstream ss(line);
        string token;

        // read line with delimeter semicolon
        int i = 0;
        while(getline(ss, token, ';')) {
            // get operation and other integers
            if (i == 0) {
                operation = token;
            } // if operation == ADD and token is the second one, then get salary
            else if (operation == "ADD" and i == 1) {
                salary = stoi(token);
            } // if operation == ADD and token is the third one, then get department
            else if (operation == "ADD" and i == 2) {
                department = stoi(token);
            } // if operation == UPDATE and token is the second one, then get id
            else if (operation == "UPDATE" and i == 1) {
                id = stoi(token);
            } // if operation == UPDATE and token is the third one, then get salary
            else if (operation == "UPDATE" and i == 2) {
                salary = stoi(token);
            } // if operation == UPDATE and token is the forth one, then get department
            else if (operation == "UPDATE" and i == 3) {
                department = stoi(token);
            } // if operation == DELETE and token is the second one, then get id
            else if (operation == "DELETE" and i == 1) {
                id = stoi(token);
            }
            i++;
        }

        // check operation and operate 
        if (operation == "ADD") {
            ofstream outputAddFile("file_solution.csv", ios::app);
            // increase id and write all data about employee
            max_id++;
            outputAddFile << max_id << ';' << salary << ';' << department << '\n';
            outputAddFile.close();
        }
        else if (operation == "UPDATE") {
            // open input file
            ifstream inFile("file_solution.csv");

            // create output file
            ofstream outFile("temp.csv");

            // is valid id flag
            bool is_valid = false;

            // read the file line by line
            string dline;
            string content_with_update;
            while(getline(inFile, dline)) {
                // create values, ss and token for each line
                int found_id = 0;
                istringstream ss(dline);
                string token;

                // read line with delimeter semicolon
                int j = 0;
                while(getline(ss, token, ';')) {
                    // convert string to integer
                    int value = stoi(token);
                    if (j == 0) {
                        found_id = value;
                    }
                    j++;
                }
                // if found id is equal to id in the operation file, update the employee
                if (found_id == id) {
                    dline = to_string(id) + ';' + to_string(salary) + ';' + to_string(department);
                    is_valid = true;
                }
                // load content with lines
                content_with_update += dline + "\n";
            }
            // print error message if id is not valid
            if (is_valid == false) {
                cout << "ERROR: An invalid ID to update\n";
            }
            // write content to the file and close
            outFile.write(content_with_update.c_str(), content_with_update.size());
            inFile.close();
            outFile.close();
            
            // rename file
            rename("temp.csv", "file_solution.csv");
        }
        else if (operation == "DELETE") {
            // open input file
            ifstream inFile("file_solution.csv");

            // create output file
            ofstream outFile("temp.csv");

            // read the file line by line
            string dline;
            string content_with_delete;
            // create id and is_valid for checking validity of id
            int found_id = 0;
            bool is_valid = false;
            while(getline(inFile, dline)) {
                // create ss and token for each line
                istringstream ss(dline);
                string token;

                // read line with delimeter semicolon
                int j = 0;
                while(getline(ss, token, ';')) {
                    // convert string to integer and assign values
                    int value = stoi(token);
                    if (j == 0) {
                        found_id = value;
                    }
                    j++;
                }

                // if found id is equal to id in the operation file, do not write it into file
                if (found_id == id) {
                    is_valid = true;
                    continue;
                } else {
                    content_with_delete += dline + "\n";
                }        
            }
            if (found_id == 0) {
                cout << "ERROR: There is no Employee\n";
            } if (is_valid == false) {
                cout << "ERROR: An invalid id to delete\n";
            }

            outFile.write(content_with_delete.c_str(), content_with_delete.size());
            inFile.close();
            outFile.close();

            rename("temp.csv", "file_solution.csv");
        }
    }
    operation_file.close();

    // write header and whole other content and close files
    ifstream infile("file_solution.csv");
    ofstream temp("temp.csv");
    string content_with_header = header + '\n';
    while(getline(infile, line)) {
        content_with_header += line + '\n';
    }
    temp.write(content_with_header.c_str(), content_with_header.size());
    infile.close();
    temp.close();

    rename("temp.csv", filename);

    return EXIT_SUCCESS;
}
