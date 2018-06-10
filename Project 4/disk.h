//////////////////////////////////////////////////////////////////
//                                                              //
//  Virtual Disk "Project 4" by Tony Nhan and Sangwon Seo       //
//  Part 1: Virtual Disk                                        //
//	Header file "disk.h" for disk.cpp                           //
//                                                              //
//////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

// Data Constants needed for disk
#define BLOCK_SIZE 4096
#define MAX_BLOCK 4096
#define MAX_DATA_BLOCKS 3500
#define MAX_FILES 64
#define MAX_OPEN_FILES 64
#define FNAME_LENGTH 16