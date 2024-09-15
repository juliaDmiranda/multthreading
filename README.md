# Multithread Project with Thread Synchronization

This project was developed as part of the course **Special Topics in Networks and Distributed Systems**. It aims to synchronize threads for performing reading, processing, and writing operations using **mutexes** to ensure proper synchronization. The code is written in **C**.

This project aimed to study and gain hands-on experience with this kind of implementation.

## Description

The program reads numbers from an input file `e.txt`, reverses them, and writes the reversed numbers to an output file named `s.txt`. The threads are organized as follows:

- **Reading Thread**: Reads numbers from the input file.
- **Processing Thread**: Reverses the numbers read.
- **Writing Thread**: Writes the reversed numbers to the output file.

### Project Structure

- **Mutexes** coordinate thread access to shared memory and ensure that reading, processing, and writing happen in the correct order.

### How to run

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/multithread-project.git


2. Compile the code:
   ```bash
    gcc -pthread -o multithread main.c

3. Run the program:
   ```bash
   ./multithread
