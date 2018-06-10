//////////////////////////////////////////////////////////////////
//                                                              //
//  Virtual Disk "Project 4" by Tony Nhan and Sangwon Seo       //
//  Part 2: Simple File System                                  //
//	Compile using: g++ -c -o sfs.o sfs.cpp                      //
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

#include "sfs.h"
#include "disk.o"
#include "disk.h"
#include "disk.cpp"

using namespace std;

// Simple File System Interfaces Prototype
int make_sfs(char *disk_name);
int mount_sfs(char *disk_name);
int umount_sfs(char *disk_name);
int sfs_open(char *file_name);
int sfs_close(int fd);
int sfs_create(char *file_name);
int sfs_delete(char *file_name);
int sfs_read(int fd, void *buf, size_t count);
int sfs_write(int fd, void *buf, size_t count);
int sfs_seek(int fd, int offset);

// Make Disk
int make_sfs(char *disk_name) {

	int fd = open_disk(disk_name);

	if (fd < 0) {
		cout << "ERROR: Fail to make file system." << endl;
		return -1;
	}
	else {
		FAT_init(&fat);
		DirectoryDescriptor_init(&root);
		write_block(fd, 0, (char *)&root);
		write_block(fd, 1, (char *)&fat);
		cout << "Successfully to make file system." << endl;
		close(fd);
		return 0;
	}
}

// Mount Disk
int mount_sfs(char *disk_name) {

	int fd = open(disk_name, O_RDWR | O_APPEND);

	if (fd < 0) {
		cout << "ERROR: Fail to mount disk." << endl;
		return -1;
	}
	else {
		read_block(fd, 0, (char *)&root);
		read_block(fd, 1, (char *)&fat);
		cout << "Successfully to mount disk." << endl;
		return 0;
	}
}

// Unmount Disk
int umount_sfs(char *disk_name) {

	if (close_disk(open_disk(disk_name)) < 0) {
		return -1;
	}
	else {
		opened = 0;
		return 0;
	}
}

// Open File
int sfs_open(char *file_name) {

	if (opened > MAX_OPEN_FILES) {
		cout << "ERROR: Max file opened." << endl;
	}
	else {
		// Open file system
		int fd = open(file_name, O_RDWR | O_APPEND);
		
		if (fd < 0) {
			cout << "ERROR: Fail to open file." << endl;
			return -1;
		}
		else {
			// Increment counter
			opened++;
			cout << "Successfully to open file." << endl;
			return fd;
		}
	}
}

// Close File
int sfs_close(int fd) {

	if (fd < 0) {
		cout << "ERROR: No file exist to open." << endl;
		return -1;
	}
	else {
		
		if (close(fd) < 0) {
			cout << "ERROR: Fail to close file." << endl;
			return -1;
		}
		//Decrement counter
		opened--;
		cout << "Successfully close file" << endl;
		return 0;
	}
}

// Create File
int sfs_create(char *file_name) {
	
	dir_entry file_name;
	void setFile(char* newFile, size_t 1, unsigned int 65) {

	if ( < 0) {
		cout << "ERROR: Fail to create file." << endl;
		return -1;
	}
	else {
		cout << "Successfully to create file." << endl;
		return 0;
	}
}

// Delete File
int sfs_delete(char *file_name) {
	
	f
	if ( < 0) {
		cout << "ERROR: Fail to delete file." << endl;
		return -1;
	}
	else {
		cout << "Successfully to delete file." << endl;
		return 0;
	}
}

// Read In File
int sfs_read(int fd, void *buf, size_t count) {

	if (fd < 0) {
		cout << "ERROR: Fail to read in file." << endl;
		return -1;
	}
	else {
		read(fd, buf, count)
		cout << "Successfully to read in file." << endl;
		return 0;
	}
}

// Write In File
int sfs_write(int fd, void *buf, size_t count) {

	if ( < 0) {
		cout << "ERROR: Fail to write in file." << endl;
		return -1;
	}
	else {
		write(fd, buf, count)
		cout << "Successfully to write in file." << endl;
		return 0;
	}
}

// Seek Location In File
int sfs_seek(int fd, int offset) {

	if (fd < 0) {
		cout << "ERROR: Fail to seek location." << endl;
		return -1;
	}
	else {
		lseek(fd, offset, SEEK_SET);
		cout << "Successfully seek location." << endl;
		return 0;
	}
}