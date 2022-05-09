#include "String.h"

#include "Messages.h"

#include <memory.h>
#include <stdlib.h>
#include <string.h>

String *string_init(String *string, const char *str, usize len)
{
	return_val_if_fail(str != NULL, NULL);

	bool was_allocated = FALSE;

	if (string == NULL) {
		string = talloc(String, 1);

		if (string == NULL) {
			msg_error("couldn't allocate memory for the string!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	usize capacity = (len != 0) ? len : 1;

	string->data = malloc(capacity);

	if (string->data == NULL) {
		if (was_allocated)
			free(string);

		msg_error("couldn't allocate memory for the string buffer!");
		return NULL;
	}

	if (len != 0)
		memcpy(string->data, str, len);

	string->len = len;
	string->capacity = capacity;

	return string;
}

String *string_init_sized(String *string, usize cap)
{
	bool was_allocated = FALSE;

	if (string == NULL) {
		string = talloc(String, 1);

		if (string == NULL) {
			msg_error("couldn't allocate memory for the string!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	string->data = malloc(cap);

	if (string->data == NULL) {
		if (was_allocated)
			free(string);

		msg_error("couldn't allocate memory for the string buffer!");
		return NULL;
	}

	string->len = 0;
	string->capacity = cap;

	return string;
}

static bool __string_newcap(String *string, usize newcap)
{
	usize new_allocated = (newcap >> 3) + (newcap < 9 ? 3 : 6);

	if (newcap > USIZE_MAX - new_allocated) {
		msg_error("string capacity overflow!");
		return FALSE;
	}

	newcap += new_allocated;

	char *data = (void *) realloc(string->data, newcap);

	if (data == NULL) {
		msg_error("couldn't reallocate memory for the string buffer!");
		return FALSE;
	}

	string->data = data;
	string->capacity = newcap;

	return TRUE;
}

static bool __string_insert_c(String *string, usize index, char c)
{
	if (index > USIZE_MAX - 1 || string->len > USIZE_MAX - 1) {
		msg_error("string capacity overflow!");
		return FALSE;
	}

	if (index + 1 > string->capacity) {
		if (__string_newcap(string, index + 1) == FALSE)
			return FALSE;
	} else if (string->len + 1 > string->capacity) {
		if (__string_newcap(string, string->capacity + 1) == FALSE)
			return FALSE;
	}

	if (index >= string->len) {
		string->len = index + 1;
	} else {
		memmove(&string->data[index + 1], &string->data[index], string->len - index);
		string->len++;
	}

	string->data[index] = c;

	return TRUE;
}

static bool __string_insert(String *string, usize index, const char *str, usize len)
{
	if (len == 0)
		return TRUE;

	if ((index > USIZE_MAX - len) || (string->len > USIZE_MAX - len)) {
		msg_error("string capacity overflow!");
		return FALSE;
	}

	if (index + len > string->capacity) {
		if (__string_newcap(string, index + len) == FALSE)
			return FALSE;
	} else if (string->len + len > string->capacity) {
		if (__string_newcap(string, string->len + len) == FALSE)
			return FALSE;
	}

	if (index >= string->len) {
		string->len = index + len;
	} else {
		string->len += len;
		memmove(&string->data[index + len], &string->data[index], string->len - index);
	}

	if (str == NULL)
		memset(&string->data[index], 0, len);
	else
		memcpy(&string->data[index], str, len);

	return TRUE;
}

bool string_push_back(String *string, const char *str, usize len)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert(string, string->len - 1, str, len);
}

bool string_push_back_c(String *string, char c)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert_c(string, string->len - 1, c);
}

bool string_push_front(String *string, const char *str, usize len)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert(string, 0, str, len);
}

bool string_push_front_c(String *string, char c)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert_c(string, 0, c);
}

bool string_insert(String *string, usize index, const char *str, usize len)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert(string, index, str, len);
}

bool string_insert_c(String *string, usize index, char c)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert_c(string, index, c);
}
