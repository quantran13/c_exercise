#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "hacking.h"
#define FILENAME  "/tmp/notes"

int print_notes(int, int, char *);
int find_user_note(int, int);
int search_note(char *, char *);

int main(int argc, char **argv)
{
	int userid, fd, printing = 1;
	char searchstring[100];

	if (argc == 2)
		strcpy(searchstring, argv[1]);
	else
		searchstring[0] = 0;

	userid = getuid();
	fd = open(FILENAME, O_RDONLY);
	if (fd == -1)
		fatal("in main() while opening file for reading");

	while (printing == 1) 
		printing = print_notes(fd, userid, searchstring);
	printf("-------[ end of note data ]-------\n\n");
	close(fd);

	return 0;
}

int print_notes(int fd, int uid, char *searchstring)
{
	int len_note;
	char note_buffer[100];

	len_note = find_user_note(fd, uid);
	if (len_note == -1)
		return 0;
	
	read(fd, note_buffer, len_note);
	note_buffer[len_note] = 0;
	
	if (search_note(note_buffer, searchstring))
		printf("%s", note_buffer);

	return 1;
}

int find_user_note(int fd, int user_uid)
{
	int length, note_uid = -1;
	unsigned char byte;

	while (note_uid != user_uid) {
		if (read(fd, &note_uid, 4) != 4)
			return -1;
		if(read(fd, &byte, 1) != 1)
			return -1;

		byte = 0;
		length = 0;

		while (byte != '\n') { 
			if (read(fd, &byte, 1) != 1) 
				return -1;   
			length++;
      }
	}

	lseek(fd, length * - 1, SEEK_CUR);
	printf("[DEBUG] found a %d byte note for user id %d\n", length, note_uid);
	
	return length;
}

int search_note(char *note, char *keyword) 
{
	int i, keyword_length, match = 0;

	keyword_length = strlen(keyword);
	if (keyword_length == 0) 
		return 1;            

	for (i=0; i < strlen(note); i++) { 
		if (note[i] == keyword[match]) 
			match++;  
		else {      
			if(note[i] == keyword[0]) 
				match = 1;
			else
				match = 0; 
		}

		if(match == keyword_length) 
			return 1;  
	}
   
	return 0; 
}
