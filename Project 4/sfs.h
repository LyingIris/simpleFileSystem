//////////////////////////////////////////////////////////////////
//                                                              //
//  Virtual Disk "Project 4" by Tony Nhan and Sangwon Seo       //
//  Part 2: Simple File System                                  //
//	Header file "sfs.h" for sfs.cpp                             //
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

// Data Constants needed for disk and file system
#define BLOCK_SIZE 4096
#define MAX_BLOCK 4096
#define MAX_DATA_BLOCKS 3500
#define MAX_FILES 64
#define MAX_OPEN_FILES 64
#define FNAME_LENGTH 16

struct FileAccessStatus {

    int opened;
    int wr_ptr;
    int rd_ptr;

};

struct FileDescriptor {

    char                filename[32];
    int                 fat_index;
    size_t                 size;
	FileAccessStatus    fas;

};

struct DirectoryDescriptor {

    FileDescriptor table[MAX_FILE];
    int count;

};

struct fat_node {

    size_t db_index;
    int next;
    int free;

};

struct FileAllocationTable {

    fat_node table[MAX_BLOCK];
    int count;

};

struct freeblocklist {

    int list[MAX_BLOCK];

};

union DiskBlock {
    DirectoryDescriptor dd;
    FileAllocationTable fat;
    freeblocklist fbl;
};

char vd[100][30000];
int fatID = 0;
DirectoryDescriptor root;
FileAllocationTable fat;
static int opened_files = 0;

void FAT_init(FileAllocationTable * fat) {

	for (fatID = 2; fatID < MAX_BLOCK - 1; fatID++) {
		fat->table[fatID].db_index = fatID * sizeof(DiskBlock);
		fat->table[fatID].next = EOF;
		fat->table[fatID].free = 1;
	}
	fat->count = 0;
}

int FAT_getFreeNode(FileAllocationTable * fat) {

	if (++fat->count < MAX_BLOCK - 1) {
		return fat->count;
	}
	else {

		int i;
		for (i = 2; i < MAX_BLOCK - 1; i++) {
			if (fat->table[i].free) {
				return i;
			}
		}
	}
	fat->count--;
	fprintf(stderr, "Error: Cannot get free block in FAT.\n");
	exit(0);
	return -5;
}

char * FAT_getPartFile(FileDescriptor file, FileAllocationTable fat, int length) {

	int curr, next, red = 0;	// this is not a typo, It needs to sound lik read
	int positionOnDisk = 0;
	void * positionOnBuff = 0;
	char buff[length];
	curr = file.fat_index;
	next = fat.table[curr].next;

	for (; red * sizeof(DiskBlock) < length && curr != EOF; curr = fat.table[curr].next, next = fat.table[curr].next, red++) {

		positionOnDisk = fat.table[curr].db_index;
		positionOnBuff = (void *)((int)(&buff) + red * sizeof(DiskBlock));
		read_blocks(positionOnDisk, length, positionOnBuff);
	}
	//printf("D:%i B:%p\n", positionOnDisk, positionOnBuff);
	return strdup(buff);
}

char * FAT_getFullFile(FileDescriptor file, FileAllocationTable fat) {

	return FAT_getPartFile(file, fat, file.size);
}

void DirectoryDescriptor_init(DirectoryDescriptor * root) {

	int i;
	for (i = 0; i < MAX_FILE; i++) {
		root->table[i].size = EMPTY;
	}
}


int DirectoryDescriptor_getFreeSpot(DirectoryDescriptor * root) {

	if (++root->count < MAX_FILE) {
		return root->count;
	}
	else {

		int i;
		for (i = 0; i < MAX_FILE; i++) {
			if (root->table[i].size == EMPTY) {
				return i;
			}
		}
	}
	root->count--;
	fprintf(stderr, "Error: Cannot get free block in DD.\n");
	exit(0);
	return -1;
}

int getIndexOfFileInDirectory(char* name, DirectoryDescriptor * rootDir) {

	int i, fileID = -1;

	// Looks for the file and gets its index.
	for (i = 0; i < MAX_FILE; i++) {
		if (!strcmp(rootDir->table[i].filename, name)) {
			fileID = i;
			break;
		}
	}

	// Checks if the file was correctly closed.
	if (fileID != -1) {
		if (rootDir->table[fileID].fas.opened == 1) {
			//fprintf(stderr, "Warning: '%s' is already open.\n", name);
			return -2;
		}
	}

	return fileID;
}

void FileDescriptor_createFile(char* name, FileDescriptor * file) {

	strcpy(file->filename, name);
	file->fas.opened = 1;
	file->fas.rd_ptr = 0;
	file->fas.wr_ptr = 0;
	file->size = 0;
}

int FileDescriptor_getNextWritable(FileDescriptor * file, FileAllocationTable * fat) {

	int currNode = file->fat_index;

	// Goes to the end of the file.
	while (fat->table[currNode].next != EOF) {
		currNode = fat->table[currNode].next;
		file->fas.wr_ptr = currNode;
	}

	int nextNode = FAT_getFreeNode(fat);
	fat->table[currNode].next = nextNode;
	fat->table[nextNode].next = EOF;

	// Returns -1 if there is no more free spots.
	return nextNode < 0 ? fat->table[nextNode].db_index : -1;
}

int FreeBlockList_getBlock(freeblocklist * fbl) {

	int i;
	for (i = 2; i < MAX_BLOCK - 1; i++) {
		if (fbl->list[i] == 0) {
			fbl->list[i] = 1;
			return i;
		}
	}
	return -1;
}