# Parallel Patient Record Management System

## Overview

This project is a Parallel Patient Record Management System implemented in C++. It leverages the Message Passing Interface (MPI) for parallel processing and uses an AVL tree to store and search patient records efficiently. The system supports CRUD (Create, Read, Update, Delete) operations on patient records.

## Features

- **Parallel Processing**: Utilizes MPI to perform operations on patient records in parallel.
- **AVL Tree**: Ensures balanced and efficient searching, insertion, and deletion of records.
- **CRUD Operations**: 
  - **Create**: Add new patient records.
  - **Read**: Retrieve patient records.
  - **Update**: Modify existing patient records.
  - **Delete**: Remove patient records.
  
## Requirements

- C++11 or higher
- MPI library (e.g., OpenMPI or MPICH)

## Installation

1. **Install MPI library:**

    For OpenMPI:
    ```sh
    sudo apt-get install libopenmpi-dev openmpi-bin
    ```

    For MPICH:
    ```sh
    sudo apt-get install mpich
    ```

2. **Clone the repository:**

    ```sh
    git clone https://github.com/yourusername/patient-record-management.git
    cd patient-record-management
    ```

3. **Compile the project:**

    ```sh
    mpic++ -std=c++11 main.cpp avl_tree.cpp patient_record.cpp -o patient_record_management
    ```

## Usage

1. **Run the application:**

    ```sh
    mpirun -np <number_of_processes> ./patient_record_management
    ```

    Replace `<number_of_processes>` with the number of processes you want to use.

2. **Perform CRUD operations:**

    - The application will prompt for different operations.
    - Follow the prompts to create, read, update, or delete patient records.

## Project Structure

- `main.cpp`: The main entry point of the application.
- `avl.cpp` / `avl.h`: Contains the implementation of the AVL tree.
- `patient_record.txt`: Contains the patient records.
