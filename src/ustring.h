/**
	@file ustring.h
	@author s22h
	@date 27 Feb 2020
	@brief A unicode-aware string
	@version 0.1.0
 */

#ifndef S22H_USTRING_H_
#define S22H_USTRING_H_

#include <cstdlib>
#include <cstdint>

class ustring {
public:
	// NUL-terminated byte buffer
	uint8_t *buffer;
	// the number of unicode characters (runes)
	size_t length;
	// the byte size of the buffer - 1 (excluding the NUL character)
	size_t size;
	ustring();
	ustring(const char *chars);
	ustring(ustring& other);
	ustring& operator=(const char *chars);
	ustring& operator=(ustring& other);
	//bool operator==(ustring& other);
	~ustring();
};

#endif

