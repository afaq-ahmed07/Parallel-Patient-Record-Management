#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <mpi.h>
#include "avl.h"
#include <unistd.h>
#include <cstdlib>
using namespace std;

Patient_Record get_patient_record(const string &line)
{
    Patient_Record patient;
    stringstream s_stream(line);
    getline(s_stream, patient.id, ',');
    s_stream >> patient.age;
    s_stream.ignore();
    s_stream >> patient.gender;
    s_stream.ignore();
    getline(s_stream, patient.blood_type, ',');
    getline(s_stream, patient.date, ',');
    getline(s_stream, patient.diagnosis);
    return patient;
}

void log_file(const string &message)
{
    ofstream logFile;

    logFile.open("log.txt", ios::app);
    if (!logFile)
    {
        cerr << "Failed to open the log file for appending." << std::endl;
        return;
    }
    logFile << message;
    logFile << endl;
    logFile.close();
}
int extractPatientID(const string &patientID)
{
    if (patientID.size() <= 1)
        return 0;

    try
    {
        return stoi(patientID.substr(1));
    }
    catch (const invalid_argument &e)
    {
        cerr << "Invalid patient ID format: " << patientID << endl;
        return 0;
    }
}

void read_file(const vector<string>& filenames, vector<string>& string_rec)
{
    for (const auto& filename : filenames)
    {
        ifstream file(filename);
        string line;
        while (getline(file, line))
        {
            string_rec.push_back(line);
        }

        if (file.bad())
        {
            std::cerr << "Error: An error occurred while reading file " << filename << std::endl;
        }

        file.close();
    }
}
string record_to_string(const Patient_Record &record)
{
    stringstream s_stream;
    s_stream << record.id << ",";
    s_stream << record.age << ",";
    s_stream << record.gender << ",";
    s_stream << record.blood_type << ",";
    s_stream << record.date << ",";
    s_stream << record.diagnosis;
    return s_stream.str();
}
void inorderTraversal(TreeNode *root, ofstream &file)
{
    if (root != NULL)
    {
        inorderTraversal(root->left, file);           
        file << record_to_string(root->record) << endl;
        inorderTraversal(root->right, file);           
    }
}
void create_backup(PatientAVL &patient_avl)
{
    ofstream file("backup.txt", ios::app);
    if (!file.is_open())
    {
        cout << "Error opening file!" << endl;
        return;
    }
    inorderTraversal(patient_avl.get_root(), file);

    file.close();
}
Patient_Record string_to_obj(const string &serialized)
{
    Patient_Record record;
    stringstream s_stream(serialized);
    getline(s_stream, record.id, ',');
    s_stream >> record.age;
    s_stream.ignore();
    s_stream >> record.gender;
    s_stream.ignore();
    getline(s_stream, record.blood_type, ',');
    getline(s_stream, record.date, ',');
    getline(s_stream, record.diagnosis);
    return record;
}
void add_patient(PatientAVL &patient_avl, const Patient_Record &new_record)
{
    string message;
    patient_avl.insert(new_record);
    cout << "Patient record added successfully." << endl;
    message = "Patient record with ID '" + new_record.id + "' added successfully.";
    log_file(message);
}

bool del_patient(PatientAVL &patient_avl, const string &del_id)
{
    string message;
    if (patient_avl.remove(del_id))
    {
        cout << "Patient record with ID '" << del_id << "' deleted successfully." << endl;
        message = "Patient record with ID '" + del_id + "' deleted successfully.";
        log_file(message);
        return true;
    }
    else
    {
        cout << "Patient record with ID '" << del_id << "' not found." << endl;
        message = "Patient record with ID '" + del_id + "' not found.";
        log_file(message);
        return false;
    }
}

bool search_patient(PatientAVL &patient_avl, string search_id)
{
    Patient_Record *node = patient_avl.search(search_id);
    string message;
    if (node != nullptr)
    {
        cout << "Patient record with ID '" << search_id << "' found successfully." << endl;
        message = "Patient record with ID '" + search_id + "' found successfully.";
        log_file(message);
        cout << "Patient ID:" << search_id << endl;
        cout << "Patient Age:" << node->age << endl;
        cout << "Patient Gender:" << node->gender << endl;
        cout << "Patient Blood Type:" << node->blood_type << endl;
        cout << "Patient Date:" << node->date << endl;
        cout << "Patient Diagnosis:" << node->diagnosis << endl;
        return true;
    }
    else
    {
        cout << "Patient record with ID '" << search_id << "' not found." << endl;
        message = "Patient record with ID '" + search_id + "' not found.";
        log_file(message);
        return false;
    }
}

bool update_patient(PatientAVL &patient_avl, const Patient_Record &new_record)
{
    Patient_Record *existing_record = patient_avl.search(new_record.id);
    string message;
    if (existing_record != nullptr)
    {
        existing_record->age = new_record.age;
        existing_record->gender = new_record.gender;
        existing_record->blood_type = new_record.blood_type;
        existing_record->date = new_record.date;
        existing_record->diagnosis = new_record.diagnosis;

        cout << "Patient record with ID '" << new_record.id << "' updated successfully." << endl;
        message = "Patient record with ID '" + new_record.id + "' updated successfully.";
        log_file(message);
        return true;
    }
    else
    {
        cout << "Patient record with ID '" << new_record.id << "' not found." << endl;
        message = "Patient record with ID '" + new_record.id + "' not found.";
        log_file(message);
        return false;
    }
}

int print_menu()
{
    system("clear");
    int choice;
    cout<<endl<<endl<<endl;
    cout << "\t\t\t\t\t\t\t\t1. Add a Patient\n";
    cout << "\t\t\t\t\t\t\t\t2. Update a Patient\n";
    cout << "\t\t\t\t\t\t\t\t3. Delete a Patient\n";
    cout << "\t\t\t\t\t\t\t\t4. Search a Patient\n";
    cout << "\t\t\t\t\t\t\t\t5. Create a Backup\n";
    cout << "\t\t\t\t\t\t\t\t6. Recover from Backup\n";
    cout<<"\t\t\t\t\t\t\t\tEnter Choice:";
    cin >> choice;
    return choice;
}
int main()
{
    int rank, size, choice, file_count=3;
    vector<string>file_arr(file_count);
    file_arr.push_back("Patient_Records.txt");
    file_arr.push_back("Additional_Patients.txt");
    file_arr.push_back("Extra_Patient_Records.txt");
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int record_size;
    vector<string> string_records;
    PatientAVL patient_avl;
    if (rank == 0)
    {
        read_file(file_arr, string_records);
        record_size = string_records.size() / size;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&record_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
    
    if (rank==0){
    
        for (int i = 0; i < record_size; ++i)
        {
            if (i + record_size < string_records.size()){
            const auto &serial = string_records[i + record_size];
            const char *buffer = serial.c_str();
            int buffer_size = serial.size() + 1;
            MPI_Send(buffer, buffer_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            }
            auto &str = string_records[i];
            patient_avl.insert(string_to_obj(str));
        }
        int pat_id;
        string str_id;
        const char *cstr;
        int str_length;
        Patient_Record new_record;
        string str_record;
        do
        {
            choice = print_menu();
            MPI_Send(&choice, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            switch (choice)
            {
            case 1:
            {
                cout << "Enter patient ID (P0000): ";
                cin >> new_record.id;
                cout << "Enter patient age: ";
                cin >> new_record.age;
                cout << "Enter patient gender (M/F): ";
                cin >> new_record.gender;
                cout << "Enter patient blood type: ";
                cin >> new_record.blood_type;
                cout << "Enter diagnosis date (MM/DD/YYYY): ";
                cin >> new_record.date;
                cout << "Enter diagnosis: ";
                cin.ignore();
                getline(cin, new_record.diagnosis);
                string str_record = record_to_string(new_record);
                const char *buff = str_record.c_str();
                int buff_size = str_record.size() + 1;
                MPI_Send(buff, buff_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
            }
            break;
            case 2:
            {
                cout << "Enter the ID of the patient you want to update: ";
                cin >> str_id;
                pat_id = extractPatientID(str_id);
                new_record.id = str_id;
                cout << "Enter patient age: ";
                cin >> new_record.age;
                cout << "Enter patient gender (M/F): ";
                cin >> new_record.gender;
                cout << "Enter patient blood type: ";
                cin >> new_record.blood_type;
                cout << "Enter diagnosis date (MM/DD/YYYY): ";
                cin >> new_record.date;
                cout << "Enter diagnosis: ";
                cin.ignore();
                getline(cin, new_record.diagnosis);
                string str_record = record_to_string(new_record);
                const char *buff = str_record.c_str();
                int buff_size = str_record.size() + 1;
                MPI_Send(buff, buff_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                update_patient(patient_avl, new_record);
            }
            break;
            case 3:
                cout << "Enter ID to Delete (P0000):";
                cin >> str_id;
                cstr = str_id.c_str();
                str_length = str_id.length() + 1;
                MPI_Send(cstr, str_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                del_patient(patient_avl, str_id);
                break;
            case 4:
                cout << "Enter ID to search (P0000):";
                cin >> str_id;
                pat_id = extractPatientID(str_id);
                cstr = str_id.c_str();
                str_length = str_id.length() + 1;
                MPI_Send(cstr, str_length, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                search_patient(patient_avl, str_id);

                break;
            case 5:
                create_backup(patient_avl);
                break;
            case 6:
            {
                ifstream file("backup.txt");

                if (file.is_open())
                {
                    patient_avl.deleteTree();
                    string_records.clear();
                    file_arr.clear();
                    file_arr.push_back("backup.txt");
                    read_file(file_arr, string_records);
                    record_size = string_records.size() / size;
                    MPI_Bcast(&record_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
                    for (int i = 0; i < record_size; ++i)
                    {
                       if (i + record_size < string_records.size()){
                        const auto &serial = string_records[i + record_size];
                        const char *buffer = serial.c_str();
                         int buffer_size = serial.size() + 1;
                         MPI_Send(buffer, buffer_size, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
                       }
                        auto &str = string_records[i];
                        patient_avl.insert(string_to_obj(str));
                    }
                }
            }
            break;
            default:
                cout << "Invalid Choice\n";
                break;
            }
            sleep(7);
        } while (choice != 7);
        
     }

     if(rank==1){
        PatientAVL patient_avl;
        Patient_Record new_record;
        string id;
        for (int i = 0; i < record_size; ++i)
        {
            char buffer[1024];
            MPI_Recv(buffer, sizeof(buffer), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            string recv_records(buffer);
            Patient_Record record = string_to_obj(recv_records);
            patient_avl.insert(record);
        }
         while (choice != 7)
        {
            MPI_Recv(&choice, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (choice <= 2)
            {
                char buffer[1024];
                MPI_Recv(buffer, sizeof(buffer), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                string recv_records(buffer);
                new_record = string_to_obj(recv_records);
            }
            else if (choice > 2 && choice < 5)
            {
                char buffer[1024];
                MPI_Recv(buffer, sizeof(buffer), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                buffer[sizeof(buffer) - 1] = '\0';
                string str_id(buffer);
                id = str_id;
            }
            switch (choice)
            {
            case 1:
                add_patient(patient_avl, new_record);

                break;
            case 2:
                update_patient(patient_avl, new_record);
                break;
            case 3:
                del_patient(patient_avl, id);
                break;
            case 4:
                search_patient(patient_avl, id);

                break;
            case 5:
                create_backup(patient_avl);
                cout << "Backup completed successfully!" << endl;
                break;
            case 6:
                patient_avl.deleteTree();
                 for (int i = 0; i < record_size; ++i)
                 {
                     char buffer[1024];
                     MPI_Recv(buffer, sizeof(buffer), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                     string recv_records(buffer);
                     Patient_Record record = string_to_obj(recv_records);
                     patient_avl.insert(record);
                }
                cout<<"Recovery Successful"<<endl;
                 break;
           default:
                cout << "Invalid Choice\n";
                break;
            }
        }
    }
    MPI_Finalize();
    return 0;
}

