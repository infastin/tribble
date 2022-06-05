#include "trb-string.h"

#include "trb-checked.h"
#include "trb-messages.h"
#include "trb-utils.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>

TrbString *trb_string_init0(TrbString *self)
{
	if (self == NULL) {
		self = trb_talloc(TrbString, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the string!");
			return NULL;
		}
	}

	self->data = NULL;
	self->len = 0;
	self->capacity = 0;

	return self;
}

TrbString *trb_string_init(TrbString *self, const char *c_str)
{
	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbString, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the string!");
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

	self->data = malloc(capacity);

	if (self->data == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for the string buffer!");
		return NULL;
	}

	if (c_str != NULL)
		memcpy(self->data, c_str, len);

	self->data[len] = '\0';

	self->len = len;
	self->capacity = capacity;

	return self;
}

TrbString *trb_string_init_len(TrbString *self, const char *str, usize len)
{
	trb_return_val_if_fail(str != NULL, NULL);

	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbString, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the string!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	usize capacity = len ?: 1;

	self->data = malloc(capacity);

	if (self->data == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for the string buffer!");
		return NULL;
	}

	if (len != 0)
		memcpy(self->data, str, len);

	self->data[len] = '\0';

	self->len = len;
	self->capacity = capacity;

	return self;
}

TrbString *trb_string_init_sized(TrbString *self, usize cap)
{
	bool was_allocated = FALSE;

	if (self == NULL) {
		self = trb_talloc(TrbString, 1);

		if (self == NULL) {
			trb_msg_error("couldn't allocate memory for the string!");
			return NULL;
		}

		was_allocated = TRUE;
	}

	self->data = malloc(cap);

	if (self->data == NULL) {
		if (was_allocated)
			free(self);

		trb_msg_error("couldn't allocate memory for the string buffer!");
		return NULL;
	}

	self->data[0] = '\0';

	self->len = 0;
	self->capacity = cap;

	return self;
}

TrbString *trb_string_init_fmt(TrbString *self, const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	char *buf;
	usize len = trb_vstrfmt(&buf, fmt, args);

	if (len == (usize) -1) {
		trb_msg_error("%s", strerror(errno));
		return NULL;
	}

	TrbString *result = trb_string_init_len(self, buf, len);
	free(buf);

	return result;
}

TrbString *trb_string_init_vfmt(TrbString *self, const char *fmt, va_list args)
{
	char *buf;
	usize len = trb_vstrfmt(&buf, fmt, args);

	if (len == (usize) -1) {
		trb_msg_error("%s", strerror(errno));
		return NULL;
	}

	TrbString *result = trb_string_init_len(self, buf, len);
	free(buf);

	return result;
}

static bool __trb_string_newcap(TrbString *self, usize newcap)
{
	usize new_allocated = (newcap >> 3) + (newcap < 9 ? 3 : 6);

	if (trb_chk_add(newcap, new_allocated, &newcap)) {
		trb_msg_error("string capacity overflow!");
		return FALSE;
	}

	char *data = (void *) realloc(self->data, newcap);

	if (data == NULL) {
		trb_msg_error("couldn't reallocate memory for the string buffer!");
		return FALSE;
	}

	self->data = data;
	self->capacity = newcap;

	return TRUE;
}

static bool __trb_string_insert_c(TrbString *self, usize index, char c)
{
	if (trb_chk_add(index, 2, NULL) || trb_chk_add(self->len, 2, NULL)) {
		trb_msg_error("string index/length overflow!");
		return FALSE;
	}

	if (index >= self->len && index + 2 > self->capacity) {
		if (__trb_string_newcap(self, index + 2) == FALSE)
			return FALSE;
	} else if (index < self->len && self->len + 2 > self->capacity) {
		if (__trb_string_newcap(self, self->len + 2) == FALSE)
			return FALSE;
	}

	if (index >= self->len) {
		self->len = index + 1;
	} else {
		memmove(&self->data[index + 1], &self->data[index], self->len - index);
		self->len++;
	}

	self->data[index] = c;
	self->data[self->len] = '\0';

	return TRUE;
}

static bool __trb_string_insert(TrbString *self, usize index, const char *str, usize len)
{
	if (len == 0)
		return TRUE;

	if (
		trb_chk_add(index, len, NULL) ||
		trb_chk_add(index + len, 1, NULL) ||
		trb_chk_add(self->len, len, NULL) ||
		trb_chk_add(self->len + len, 1, NULL)
	) {
		trb_msg_error("string index/length overflow!");
		return FALSE;
	}

	if (index >= self->len && index + len + 1 > self->capacity) {
		if (__trb_string_newcap(self, index + len + 1) == FALSE)
			return FALSE;
	} else if (index < self->len && self->len + len + 1 > self->capacity) {
		if (__trb_string_newcap(self, self->len + len + 1) == FALSE)
			return FALSE;
	}

	if (index >= self->len) {
		self->len = index + len;
	} else {
		memmove(&self->data[index + len], &self->data[index], self->len - index);
		self->len += len;
	}

	self->data[self->len] = '\0';

	if (str == NULL)
		memset(&self->data[index], 0, len);
	else
		memcpy(&self->data[index], str, len);

	return TRUE;
}

static bool __trb_string_insert_vfmt(TrbString *self, usize index, const char *fmt, va_list args)
{
	va_list args_copy;
	va_copy(args_copy, args);

	char *buf;
	usize len = trb_vstrfmt(&buf, fmt, args);
	if (len == (usize) -1) {
		trb_msg_error("%s", strerror(errno));
		return FALSE;
	}

	bool res = __trb_string_insert(self, index, buf, len);
	free(buf);

	return res;
}

bool trb_string_push_back(TrbString *self, const char *c_str)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(c_str != NULL, FALSE);
	return __trb_string_insert(self, self->len, c_str, strlen(c_str));
}

bool trb_string_push_back_len(TrbString *self, const char *str, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_string_insert(self, self->len, str, len);
}

bool trb_string_push_back_fmt(TrbString *self, const char *fmt, ...)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);

	va_list args;
	va_start(args, fmt);
	bool res = __trb_string_insert_vfmt(self, self->len, fmt, args);
	va_end(args);
	return res;
}

bool trb_string_push_back_vfmt(TrbString *self, const char *fmt, va_list args)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);
	return __trb_string_insert_vfmt(self, self->len, fmt, args);
}

bool trb_string_push_back_c(TrbString *self, char c)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_string_insert_c(self, self->len, c);
}

bool trb_string_push_front(TrbString *self, const char *c_str)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(c_str != NULL, FALSE);
	return __trb_string_insert(self, 0, c_str, strlen(c_str));
}

bool trb_string_push_front_len(TrbString *self, const char *str, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_string_insert(self, 0, str, len);
}

bool trb_string_push_front_c(TrbString *self, char c)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_string_insert_c(self, 0, c);
}

bool trb_string_push_front_fmt(TrbString *self, const char *fmt, ...)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);

	va_list args;
	va_start(args, fmt);
	bool res = __trb_string_insert_vfmt(self, 0, fmt, args);
	va_end(args);
	return res;
}

bool trb_string_push_front_vfmt(TrbString *self, const char *fmt, va_list args)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);
	return __trb_string_insert_vfmt(self, 0, fmt, args);
}

bool trb_string_insert(TrbString *self, usize index, const char *c_str)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(c_str != NULL, FALSE);
	return __trb_string_insert(self, index, c_str, strlen(c_str));
}

bool trb_string_insert_len(TrbString *self, usize index, const char *str, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_string_insert(self, index, str, len);
}

bool trb_string_insert_c(TrbString *self, usize index, char c)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_string_insert_c(self, index, c);
}

bool trb_string_insert_fmt(TrbString *self, usize index, const char *fmt, ...)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);

	va_list args;
	va_start(args, fmt);
	bool res = __trb_string_insert_vfmt(self, index, fmt, args);
	va_end(args);
	return res;
}

bool trb_string_insert_vfmt(TrbString *self, usize index, const char *fmt, va_list args)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);
	return __trb_string_insert_vfmt(self, index, fmt, args);
}

static bool __trb_string_erase(TrbString *self, usize index, usize len, char *ret)
{
	if (len == 0)
		return TRUE;

	if (trb_chk_add(index, len, NULL)) {
		trb_msg_error("string index overflow!");
		return FALSE;
	}

	if (index + len > self->len) {
		if (len == 1) {
			trb_msg_warn("character at [%zu] is out of bounds!", index);
		} else {
			trb_msg_warn("range [%zu:%zu] is out of bounds!", index, index + len - 1);
		}

		return FALSE;
	}

	if (ret != NULL)
		memcpy(ret, &self->data[index], len);

	if (index + len != self->len)
		memmove(&self->data[index], &self->data[index + len], (self->len + 1) - len - index);
	else
		self->data[index] = '\0';

	self->len -= len;

	return TRUE;
}

bool trb_string_erase(TrbString *self, usize index, usize len, char *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len < len) {
		trb_msg_warn("array doesn't consist of %zu characters!", len);
		return FALSE;
	}

	return __trb_string_erase(self, index, len, ret);
}

bool trb_string_erase_c(TrbString *self, usize index, char *ret)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->len == 0) {
		trb_msg_warn("string is empty!");
		return FALSE;
	}

	return __trb_string_erase(self, index, 1, ret);
}

static bool __trb_string_overwrite(TrbString *self, usize index, const char *str, usize len)
{
	if (len == 0)
		return TRUE;

	if (trb_chk_add(index, len, NULL) || trb_chk_add(index + len, 1, NULL)) {
		trb_msg_error("string index/length overflow!");
		return FALSE;
	}

	if (index + len + 1 > self->capacity) {
		if (__trb_string_newcap(self, index + len + 1) == FALSE)
			return FALSE;
	}

	if (index + len > self->len) {
		self->len = index + len;
		self->data[self->len] = '\0';
	}

	if (str == NULL)
		memset(&self->data[index], 0, len);
	else
		memcpy(&self->data[index], str, len);

	return TRUE;
}

static bool __trb_string_overwrite_vfmt(TrbString *self, usize index, const char *fmt, va_list args)
{
	va_list args_copy;
	va_copy(args_copy, args);

	char *buf;
	usize len = trb_vstrfmt(&buf, fmt, args);
	if (len == (usize) -1) {
		trb_msg_error("%s", strerror(errno));
		return FALSE;
	}

	bool res = __trb_string_overwrite(self, index, buf, len);
	free(buf);

	return res;
}

bool trb_string_overwrite(TrbString *self, usize index, const char *c_str)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(c_str != NULL, FALSE);
	return __trb_string_overwrite(self, index, c_str, strlen(c_str));
}

bool trb_string_overwrite_len(TrbString *self, usize index, const char *str, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_string_overwrite(self, index, str, len);
}

bool trb_string_overwrite_fmt(TrbString *self, usize index, const char *fmt, ...)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);

	va_list args;
	va_start(args, fmt);
	bool res = __trb_string_overwrite_vfmt(self, index, fmt, args);
	va_end(args);
	return res;
}

bool trb_string_overwrite_vfmt(TrbString *self, usize index, const char *fmt, va_list args)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);
	return __trb_string_overwrite_vfmt(self, index, fmt, args);
}

bool trb_string_overwrite_c(TrbString *self, usize index, char c)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (trb_chk_add(index, 2, NULL)) {
		trb_msg_error("string index overflow!");
		return FALSE;
	}

	if (index + 2 > self->capacity) {
		if (__trb_string_newcap(self, index + 2) == FALSE)
			return FALSE;
	}

	if (index + 1 > self->len) {
		self->len = index + 1;
		self->data[self->len] = '\0';
	}

	self->data[index] = c;

	return TRUE;
}

static bool __trb_string_assign(TrbString *self, const char *str, usize len)
{
	if (len + 1 > self->capacity) {
		if (__trb_string_newcap(self, len + 1) == FALSE)
			return FALSE;
	}

	self->len = len;
	self->data[self->len] = '\0';

	if (str == NULL)
		memset(self->data, 0, len);
	else
		memcpy(self->data, str, len);

	return TRUE;
}

bool trb_string_assign(TrbString *self, const char *c_str)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(c_str != NULL, FALSE);
	return __trb_string_assign(self, c_str, strlen(c_str));
}

bool trb_string_assign_len(TrbString *self, const char *str, usize len)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	return __trb_string_assign(self, str, len);
}

i32 trb_string_cmp(const TrbString *a, const TrbString *b)
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

bool trb_string_assign_fmt(TrbString *self, const char *fmt, ...)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);

	char *buf;
	va_list args;
	va_start(args, fmt);

	usize len = trb_vstrfmt(&buf, fmt, args);
	if (len == (usize) -1) {
		trb_msg_error("%s", strerror(errno));
		return FALSE;
	}

	if (len + 1 > self->capacity) {
		if (__trb_string_newcap(self, len + 1) == FALSE) {
			free(buf);
			return FALSE;
		}
	}

	memcpy(self->data, buf, len + 1);
	self->len = len;

	free(buf);

	return TRUE;
}

bool trb_string_assign_vfmt(TrbString *self, const char *fmt, va_list args)
{
	trb_return_val_if_fail(self != NULL, FALSE);
	trb_return_val_if_fail(fmt != NULL, FALSE);

	char *buf;

	usize len = trb_vstrfmt(&buf, fmt, args);
	if (len == (usize) -1) {
		trb_msg_error("%s", strerror(errno));
		return FALSE;
	}

	if (len + 1 > self->capacity) {
		if (__trb_string_newcap(self, len + 1) == FALSE) {
			free(buf);
			return FALSE;
		}
	}

	memcpy(self->data, buf, len + 1);
	self->len = len;

	free(buf);

	return TRUE;
}

char *trb_string_steal(TrbString *self, usize *len)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->data == NULL) {
		trb_msg_warn("string buffer is NULL!");
		return FALSE;
	}

	char *ret = self->data;

	if (len != NULL)
		*len = self->len;

	self->len = 0;
	self->capacity = 1;

	self->data = calloc(1, 1);

	if (self->data == NULL) {
		self->capacity = 0;
		trb_msg_error("couldn't allocate memory for a new buffer of the string!");
	}

	return ret;
}

char *trb_string_steal0(TrbString *self, usize *len)
{
	trb_return_val_if_fail(self != NULL, FALSE);

	if (self->data == NULL) {
		trb_msg_warn("string buffer is NULL!");
		return FALSE;
	}

	char *ret = self->data;

	if (len != NULL)
		*len = self->len;

	self->data = NULL;
	self->len = 0;
	self->capacity = 0;

	return ret;
}

void trb_string_destroy(TrbString *self)
{
	trb_return_if_fail(self != NULL);

	if (self->data == NULL)
		return;

	free(self->data);

	self->data = NULL;
	self->len = 0;
	self->capacity = 0;
}

void trb_string_free(TrbString *self)
{
	trb_return_if_fail(self != NULL);
	trb_string_destroy(self);
	free(self);
}
