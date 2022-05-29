#include "Messages.h"

#include "Utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static const char *message_flag_to_color(TrbMessageFlags flag)
{
	switch (flag) {
	case TRB_MESSAGE_ERROR:
		return "\033[1;31m";
	case TRB_MESSAGE_CRITICAL:
		return "\033[1;35m";
	case TRB_MESSAGE_WARNING:
		return "\033[1;33m";
	case TRB_MESSAGE_INFO:
		return "\033[1;36m";
	case TRB_MESSAGE_DEBUG:
		return "\033[1;34m";
	case TRB_MESSAGE_PRINT:
	default:
		return "\033[1;32m";
	}

	return "";
}

static const char *message_flag_to_prefix(TrbMessageFlags flag, bool *to_stdout)
{
	*to_stdout = true;

	switch (flag) {
	case TRB_MESSAGE_ERROR:
		*to_stdout = false;
		return "ERROR";
	case TRB_MESSAGE_CRITICAL:
		*to_stdout = false;
		return "CRITICAL";
	case TRB_MESSAGE_WARNING:
		*to_stdout = false;
		return "WARNING";
	case TRB_MESSAGE_INFO:
		return "INFO";
	case TRB_MESSAGE_DEBUG:
		return "DEBUG";
	case TRB_MESSAGE_PRINT:
	default:
		return "MESSAGE";
	}

	return "";
}

void trb_message(TrbMessageFlags flag, const char *msg, ...)
{
	const char *color;
	const char *prefix;
	const char *reset;
	bool to_stdout;
	FILE *stream;

	color = message_flag_to_color(flag);
	prefix = message_flag_to_prefix(flag, &to_stdout);
	reset = "\033[0m";

	if (to_stdout)
		stream = stdout;
	else
		stream = stderr;

	va_list args;
	va_start(args, msg);
	char *parsed = trb_strdup_vfmt(msg, args);
	va_end(args);

	if (parsed == NULL) {
		perror("message");
		return;
	}

	fprintf(stream, "** %s%s%s: %s\n", color, prefix, reset, parsed);
	free(parsed);
}

void trb_message_func(TrbMessageFlags flag, const char *file, usize line, const char *func, const char *msg, ...)
{
	va_list args;
	va_start(args, msg);
	char *parsed = trb_strdup_vfmt(msg, args);
	va_end(args);

	if (parsed == NULL) {
		perror("message_func");
		return;
	}

	const char *white = "\033[1;37m";
	const char *reset = "\033[0m";

	trb_message(flag, "%s%s:%zu:%s%s: %s", white, file, line, func, reset, parsed);

	free(parsed);
}

void trb_return_if_fail_warning(const char *file, usize line, const char *func, const char *expr)
{
	const char *white = "\033[1;37m";
	const char *reset = "\033[0m";

	trb_message(TRB_MESSAGE_WARNING, "%s%s:%zu:%s%s: assertion '%s' is failed!", white, file, line, func, reset, expr);
}

void trb_exit_if_fail_critical(const char *file, usize line, const char *func, const char *expr)
{
	const char *white = "\033[1;37m";
	const char *reset = "\033[0m";

	trb_message(TRB_MESSAGE_CRITICAL, "%s%s:%zu:%s%s: assertion '%s' is failed!", white, file, line, func, reset, expr);
}
