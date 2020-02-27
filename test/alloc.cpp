#include "../src/ustring.h"

#include <stdio.h>

int main() {
	ustring s1("®");
	ustring s2(s1);
	ustring s3, s4;
	s3 = s2;
	s4 = "Hello World!";
	
	printf("%s\n", s4.buffer);
	printf("%s has size %zu and length %zu.\n", s3.buffer, s3.size, s3.length);

	return 0;
}

