#include "Tribble.h"

int main()
{
	char word[] = "ABCDABD";
	usize len = sizeof(word) - 1;
	char table[len];

	table[0] = 0;

	for (usize i = 1, d = 1, j = 0; i < len; ++i) {
		if (word[i] == word[j]) {
			table[i] = d++;
			j++;
		} else {
			table[i] = d;
			j = 0;
			d = 0;
		}
	}

	char text[] = "ABC ABCDAB ABCDABCDABDE";
	usize tlen = sizeof(text) - 1;

	usize start = 0;

	for (usize i = 0, j = 0; i < tlen;) {
		if (text[i] == word[j]) {
			i++;
			j++;

			if (j == len)
				break;
		} else {
			if (j == 0) {
				start = ++i;
			} else {
				j = table[j];
				start = i - j;
			}
		}
	}

	printf("%zu: %s\n", start, &text[start]);

	return 0;
}
