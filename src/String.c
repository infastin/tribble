#include "String.h"

#include "Messages.h"
#include "Utils.h"

#include <errno.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>

String *string_init0(String *string)
{
	if (string == NULL) {
		string = talloc(String, 1);

		if (string == NULL) {
			msg_error("couldn't allocate memory for the string!");
			return NULL;
		}
	}

	string->data = NULL;
	string->len = 0;
	string->capacity = 0;

	return string;
}

String *string_init(String *string, const char *c_str)
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

	usize len = 0;
	usize capacity = 1;

	if (c_str != NULL) {
		len = strlen(c_str);
		capacity = len + 1;
	}

	string->data = malloc(capacity);

	if (string->data == NULL) {
		if (was_allocated)
			free(string);

		msg_error("couldn't allocate memory for the string buffer!");
		return NULL;
	}

	if (c_str != NULL)
		memcpy(string->data, c_str, len);

	string->data[len] = '\0';

	string->len = len;
	string->capacity = capacity;

	return string;
}

String *string_init_len(String *string, const char *str, usize len)
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

	string->data[len] = '\0';

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

	string->data[0] = '\0';

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

	if (index >= string->len && index + 2 > string->capacity) {
		if (__string_newcap(string, index + 2) == FALSE)
			return FALSE;
	} else if (index < string->len && string->len + 2 > string->capacity) {
		if (__string_newcap(string, string->len + 2) == FALSE)
			return FALSE;
	}

	if (index >= string->len) {
		string->len = index + 1;
	} else {
		memmove(&string->data[index + 1], &string->data[index], string->len - index);
		string->len++;
	}

	string->data[index] = c;
	string->data[string->len] = '\0';

	return TRUE;
}

static bool __string_insert(String *string, usize index, const char *str, usize len)
{
	if (len == 0)
		return TRUE;

	if (
		(index > USIZE_MAX - len) ||
		(index + len > USIZE_MAX - 1) ||
		(string->len > USIZE_MAX - len) ||
		(string->len + len > USIZE_MAX - 1)
	) {
		msg_error("string capacity overflow!");
		return FALSE;
	}

	if (index >= string->len && index + len + 1 > string->capacity) {
		if (__string_newcap(string, index + len + 1) == FALSE)
			return FALSE;
	} else if (index < string->len && string->len + len + 1 > string->capacity) {
		if (__string_newcap(string, string->len + len + 1) == FALSE)
			return FALSE;
	}

	if (index >= string->len) {
		string->len = index + len;
	} else {
		memmove(&string->data[index + len], &string->data[index], string->len - index);
		string->len += len;
	}

	string->data[string->len] = '\0';

	if (str == NULL)
		memset(&string->data[index], 0, len);
	else
		memcpy(&string->data[index], str, len);

	return TRUE;
}

static bool __string_insert_vfmt(String *string, usize index, const char *fmt, va_list args)
{
	va_list args_copy;
	va_copy(args_copy, args);

	char *buf;
	usize len = vstrfmt(&buf, fmt, args);
	if (len == -1) {
		msg_error(strerror(errno));
		return FALSE;
	}

	return __string_insert(string, index, buf, len);
}

bool string_push_back(String *string, const char *c_str)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(c_str != NULL, FALSE);
	return __string_insert(string, string->len, c_str, strlen(c_str));
}

bool string_push_back_len(String *string, const char *str, usize len)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert(string, string->len, str, len);
}

bool string_push_back_fmt(String *string, const char *fmt, ...)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);

	va_list args;
	va_start(args, fmt);
	bool res = __string_insert_vfmt(string, string->len, fmt, args);
	va_end(args);
	return res;
}

bool string_push_back_vfmt(String *string, const char *fmt, va_list args)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);
	return __string_insert_vfmt(string, string->len, fmt, args);
}

bool string_push_back_c(String *string, char c)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert_c(string, string->len, c);
}

bool string_push_front(String *string, const char *c_str)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(c_str != NULL, FALSE);
	return __string_insert(string, 0, c_str, strlen(c_str));
}

bool string_push_front_len(String *string, const char *str, usize len)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert(string, 0, str, len);
}

bool string_push_front_c(String *string, char c)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert_c(string, 0, c);
}

bool string_push_front_fmt(String *string, const char *fmt, ...)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);

	va_list args;
	va_start(args, fmt);
	bool res = __string_insert_vfmt(string, 0, fmt, args);
	va_end(args);
	return res;
}

bool string_push_front_vfmt(String *string, const char *fmt, va_list args)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);
	return __string_insert_vfmt(string, 0, fmt, args);
}

bool string_insert(String *string, usize index, const char *c_str)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(c_str != NULL, FALSE);
	return __string_insert(string, index, c_str, strlen(c_str));
}

bool string_insert_len(String *string, usize index, const char *str, usize len)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert(string, index, str, len);
}

bool string_insert_c(String *string, usize index, char c)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_insert_c(string, index, c);
}

bool string_insert_fmt(String *string, usize index, const char *fmt, ...)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);

	va_list args;
	va_start(args, fmt);
	bool res = __string_insert_vfmt(string, index, fmt, args);
	va_end(args);
	return res;
}

bool string_insert_vfmt(String *string, usize index, const char *fmt, va_list args)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);
	return __string_insert_vfmt(string, index, fmt, args);
}

static bool __string_erase(String *string, usize index, usize len, char *ret)
{
	if (len == 0)
		return TRUE;

	if (index > USIZE_MAX - len) {
		msg_error("string index overflow!");
		return FALSE;
	}

	if (index + len > string->len) {
		if (len == 1) {
			msg_warn("character at [%zu] is out of bounds!", index);
		} else {
			msg_warn("range [%zu:%zu] is out of bounds!", index, index + len - 1);
		}

		return FALSE;
	}

	if (ret != NULL)
		memcpy(ret, &string->data[index], len);

	if (index + len != string->len)
		memmove(&string->data[index], &string->data[index + len], (string->len + 1) - len - index);
	else
		string->data[index] = '\0';

	string->len -= len;

	return TRUE;
}

bool string_erase(String *string, usize index, usize len, char *ret)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_erase(string, index, len, ret);
}

bool string_erase_c(String *string, usize index, char *ret)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_erase(string, index, 1, ret);
}

static bool __string_overwrite(String *string, usize index, const char *str, usize len)
{
	if (len == 0)
		return TRUE;

	if (index > USIZE_MAX - len) {
		msg_error("string capacity overflow!");
		return FALSE;
	}

	if (index + len + 1 > string->capacity) {
		if (__string_newcap(string, index + len + 1) == FALSE)
			return FALSE;
	}

	if (index + len > string->len) {
		string->len = index + len;
		string->data[string->len] = '\0';
	}

	if (str == NULL)
		memset(&string->data[index], 0, len);
	else
		memcpy(&string->data[index], str, len);

	return TRUE;
}

static bool __string_overwrite_vfmt(String *string, usize index, const char *fmt, va_list args)
{
	va_list args_copy;
	va_copy(args_copy, args);

	char *buf;
	usize len = vstrfmt(&buf, fmt, args);
	if (len == -1) {
		msg_error(strerror(errno));
		return FALSE;
	}

	return __string_overwrite(string, index, buf, len);
}

bool string_overwrite(String *string, usize index, const char *c_str)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(c_str != NULL, FALSE);
	return __string_overwrite(string, index, c_str, strlen(c_str));
}

bool string_overwrite_len(String *string, usize index, const char *str, usize len)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_overwrite(string, index, str, len);
}

bool string_overwrite_fmt(String *string, usize index, const char *fmt, ...)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);

	va_list args;
	va_start(args, fmt);
	bool res = __string_overwrite_vfmt(string, index, fmt, args);
	va_end(args);
	return res;
}

bool string_overwrite_vfmt(String *string, usize index, const char *fmt, va_list args)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);
	return __string_overwrite_vfmt(string, index, fmt, args);
}

bool string_overwrite_c(String *string, usize index, char c)
{
	return_val_if_fail(string != NULL, FALSE);

	if (index > USIZE_MAX - 1) {
		msg_error("string capacity overflow!");
		return FALSE;
	}

	if (index + 2 > string->capacity) {
		if (__string_newcap(string, index + 2) == FALSE)
			return FALSE;
	}

	if (index + 1 > string->len) {
		string->len = index + 1;
		string->data[string->len] = '\0';
	}

	string->data[index] = c;

	return TRUE;
}

static bool __string_assign(String *string, const char *str, usize len)
{
	if (len + 1 > string->capacity) {
		if (__string_newcap(string, len + 1) == FALSE)
			return FALSE;
	}

	string->len = len;
	string->data[string->len] = '\0';

	if (str == NULL)
		memset(string->data, 0, len);
	else
		memcpy(string->data, str, len);

	return TRUE;
}

bool string_assign(String *string, const char *c_str)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(c_str != NULL, FALSE);
	return __string_assign(string, c_str, strlen(c_str));
}

bool string_assign_len(String *string, const char *str, usize len)
{
	return_val_if_fail(string != NULL, FALSE);
	return __string_assign(string, str, len);
}

i32 string_cmp(const String *a, const String *b)
{
	if (a == NULL && b == NULL)
		return 0;

	if (b == NULL)
		return 1;

	if (a == NULL)
		return -1;

	if (a->len > b->len)
		return 1;

	if (a->len < b->len)
		return -1;

	for (usize i = 0; i < a->len; ++i) {
		if (a->data[i] > b->data[i])
			return 1;
		if (a->data[i] < b->data[i])
			return -1;
	}

	return 0;
}

bool string_fmt(String *string, const char *fmt, ...)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);

	char *buf;
	va_list args;
	va_start(args, fmt);

	usize len = vstrfmt(&buf, fmt, args);
	if (len == -1) {
		msg_error(strerror(errno));
		return FALSE;
	}

	if (len + 1 > string->capacity) {
		if (__string_newcap(string, len + 1) == FALSE) {
			free(buf);
			return FALSE;
		}
	}

	memcpy(string->data, buf, len + 1);
	string->len = len;

	return TRUE;
}

bool string_vfmt(String *string, const char *fmt, va_list args)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(fmt != NULL, FALSE);

	char *buf;

	usize len = vstrfmt(&buf, fmt, args);
	if (len == -1) {
		msg_error(strerror(errno));
		return FALSE;
	}

	if (len + 1 > string->capacity) {
		if (__string_newcap(string, len + 1) == FALSE) {
			free(buf);
			return FALSE;
		}
	}

	memcpy(string->data, buf, len + 1);
	string->len = len;

	return TRUE;
}

bool string_steal(String *string, char **ret, usize *len, bool to_copy)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (string->data == NULL) {
		msg_warn("string buffer is NULL!");
		return FALSE;
	}

	if (to_copy) {
		memcpy(*ret, string->data, string->len + 1);
		free(string->data);
	} else {
		*ret = string->data;
	}

	if (len != NULL)
		*len = string->len;

	string->len = 0;
	string->capacity = 1;

	string->data = malloc(1);

	if (string->data == NULL) {
		string->capacity = 0;
		msg_error("couldn't allocate memory for a new buffer of the string!");
		return FALSE;
	}

	string->data[0] = '\0';

	return TRUE;
}

bool string_steal0(String *string, char **ret, usize *len, bool to_copy)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (string->data == NULL) {
		msg_warn("string buffer is NULL!");
		return FALSE;
	}

	if (to_copy) {
		memcpy(*ret, string->data, string->len + 1);
		free(string->data);
	} else {
		*ret = string->data;
	}

	if (len != NULL)
		*len = string->len;

	string->data = NULL;
	string->len = 0;
	string->capacity = 0;

	return TRUE;
}

bool string_get(String *string, usize index, usize len, char *ret)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (len == 0)
		return TRUE;

	if (index + len > string->len) {
		msg_warn("range [%zu:%zu] is out of bounds!", index, index + len - 1);
		return FALSE;
	}

	memcpy(ret, &string->data[index], len);

	return TRUE;
}

bool string_get_c(String *string, usize index, char *ret)
{
	return_val_if_fail(string != NULL, FALSE);
	return_val_if_fail(ret != NULL, FALSE);

	if (index > string->len) {
		msg_warn("character at [%zu] is out of bounds!", index);
		return FALSE;
	}

	*ret = string->data[index];

	return TRUE;
}

void string_purge(String *string)
{
	return_if_fail(string != NULL);

	if (string->data == NULL)
		return;

	free(string->data);

	string->data = NULL;
	string->len = 0;
	string->capacity = 0;
}

void string_free(String *string)
{
	return_if_fail(string != NULL);
	string_purge(string);
	free(string);
}