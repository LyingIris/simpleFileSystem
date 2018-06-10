//////////////////////////////////////////////////////////////////
//                                                              //
//  Virtual Disk "Project 4" by Tony Nhan and Sangwon Seo       //
//  Part 1: Virtual Disk                                        //
//	Compile using: g++ -c -o disk.o disk.cpp                    //
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

#include "disk.h"

using namespace std;


// Virtual Disk Interfaces Prototype
int create_disk(char* filename, size_t nbytes);
int open_disk(char* filename);
int read_block(int disk, int block_num, char *buf);
int write_block(int disk, int block_num, char *buf);
int close_disk(int disk);

// Create Disk Function
int create_disk(char* filename, size_t nbytes) {
	
	// Create and open file to act as disk
	int fd = open(filename, O_RDWR | O_APPEND | O_CREAT, 00777);

	if (fd < 0) {
		cout << "ERROR: Fail to create disk " << filename << endl;
		return -1;
	}
	else {
		cout << "Disk " << filename << " successfully created" << endl;
    	// Open file and make space for disk
    	ofstream x(filename);
    	x.seekp(nbytes);
    	x << " ";
    	x.close();

    	// Close the file
    	close(fd);
		return 0;
	}
}

// Open Disk Function
int open_disk(char* filename) {

	// Open file to be ready for read and write
	int fd = open(filename, O_RDWR | O_APPEND);

	if (fd < 0) {
		cout << "ERROR: Fail to mount disk: " << filename << endl;
		return -1;
	}
	else {
		cout << "Disk " << filename << " successfully mounted." << endl;

		// Return file descriptor when success
		return fd;
	}
}

// Read Block Function
int read_block(int disk, int block_num, char *buf) {

	// Check for existing file descriptor
	if (disk < 0) {
		cout << "ERROR: No disk have been mounted." << endl;			
		return -1;
	}	// Check if block number bigger than allow max block
	else if (block_num > MAX_BLOCK) {
		cout << "ERROR: block number (" << block_num << ") is too big! Abort" << endl;
		return -1;
	}
	else {
		// Locate the block to read
		if (lseek(disk, block_num * BLOCK_SIZE, SEEK_SET) < 0) {
			cout << "ERROR: Fail to read disk block." << endl;
			return -1;
		}
		// Read the block
		if (read(disk, buf, BLOCK_SIZE) != BLOCK_SIZE) {
			cout << "ERROR: Fail to read disk block." << endl;
			return -1;
		}

		cout << "Successfully to read disk block." << endl;
		return 0;
	}
}

// Write Block Function
int write_block(int disk, int block_num, char *buf) {

	// Check for existing file descriptor
	if (disk < 0) {
		cout << "ERROR: No disk have been mounted." << endl;
		return -1;
	}	// Check if block number bigger than allow max block
	else if (block_num > MAX_BLOCK) {
		cout << "ERROR: block number (" << block_num << ") is too big! Abort" << endl;
		return -1;
	}
	else {
		//  Locate the block to write
		if (lseek(disk, block_num * BLOCK_SIZE, SEEK_SET) < 0) {
			cout << "ERROR: Fail to write disk block." << endl;
			return -1;
		}
		// Write to the block
		if (write(disk, buf, BLOCK_SIZE) != BLOCK_SIZE) {
			cout << "ERROR: Fail to write disk block." << endl;
			return -1;
		}

		cout << "Successfully to write disk block." << endl;
		return 0;
	}
}

// Close Disk Function
int close_disk(int disk) {
	
	// Check if disk been mounted
	if (disk < 0) {
		cout << "ERROR: No disk have been mounted." << endl;
		return -1;
	}
	else {
		
		if (close(disk) < 0) {
			cout << "ERROR: Fail to unmount disk." << endl;
			return -1;
		}
		// Close the disk
		cout << "Successfully unmount disk." << endl;
		return 0;
	}
}