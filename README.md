# Cpu_Scheduler

Project Overview

This project leverages the Crow web framework to create a web-based interface for CPU scheduling algorithms. Users can upload a text file containing process details, and the application processes this input to determine the most suitable CPU scheduling algorithm. The results are then displayed directly on the web page.

Features

Automatic Algorithm Selection: The program dynamically chooses the most suitable CPU scheduling algorithm based on the input data.
    Multiple Scheduling Algorithms Supported:
        First Come First Serve (FCFS)
        Shortest Job First (SJF)
        Shortest Remaining Time First (SRTF)
        Longest Job First (LJF)
        Round Robin (RR)
        Priority Scheduling
    File Upload: Users can upload an input file with process details.
    Output Display: The scheduling results are displayed on the web page in a user-friendly format.

File Structure

completeScheduling.cpp: Contains all the CPU scheduling algorithms and logic to select the best one. It reads the input file, determines the appropriate scheduling algorithm, executes it, and writes the results to an output file.
main.cpp: Implements the Crow web server to handle file uploads, run the scheduling algorithms, and display the output.
crow_all.h: A single-header version of the Crow library, used to set up the web server.

Prerequisites

Boost Library: Ensure Boost is installed and accessible on your system.

1.)Setup Instructions
  Clone the Repository
2.)Install Boost:
    Download and extract Boost
    Build Boost libraries
3.)Configure Visual Studio
4.)Build the Project
5.)Run the Server
6.)Access the Web App

Usage :
 Upload the input file 
 The output, showing the results of the selected scheduling algorithm, will be displayed 
 directly on the web page.
    
