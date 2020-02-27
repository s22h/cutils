/**
	@file ustring.c
	@author s22h
	@date 27 Feb 2020
	@brief A unicode-aware string
	@version 0.1.0
 */

#include "ustring.h"
#include "unicode.h"

#include <cstring>

ustring::ustring() {
	buffer = nullptr;
	size = 0;
	length = 0;
}

ustring::ustring(const char *chars) {
	length = utf8_strlen((uint8_t*) chars, &size);
	buffer = new uint8_t[size + 1];
	buffer[size] = 0;
	memcpy(buffer, chars, size);
}

ustring::ustring(ustring& other) {
	size = other.size;
	length = other.length;
	buffer = new uint8_t[size + 1];
	buffer[size] = 0;
	memcpy(buffer, other.buffer, size);
}

ustring::~ustring() {
	if (buffer) {
		delete[] buffer;
		length = 0;
		size = 0;
	}
}

ustring& ustring::operator=(const char *chars) {
	if (buffer) delete[] buffer;

	length = utf8_strlen((uint8_t*) chars, &size);
	buffer = new uint8_t[size + 1];
	buffer[size] = 0;
	memcpy(buffer, chars, size);

	return *this;
}

ustring& ustring::operator=(ustring& other) {
	if (buffer) delete[] buffer;

	size = other.size;
	length = other.length;
	buffer = new uint8_t[size + 1];
	buffer[size] = 0;
	memcpy(buffer, other.buffer, size);

	return *this;
}

