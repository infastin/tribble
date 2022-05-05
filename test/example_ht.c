#include "Hash.h"
#include "HashTable.h"
#include "Macros.h"
#include "Polynom.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char first_names[][16] = {
	"Adam",
	"Adrian",
	"Anna",
	"Charlotte",
	"Frédérique",
	"Ilaria",
	"Jakub",
	"Jennyfer",
	"Julia",
	"Justin",
	"Mario",
	"Miriam",
	"Mohamed",
	"Nourimane",
	"Owen",
	"Peter",
	"Petra",
	"Rachid",
	"Rebecca",
	"Sarah",
	"Thibault",
	"Wolfgang",
};

char last_names[][16] = {
	"Bailey",
	"Berat",
	"Chen",
	"Farquharson",
	"Ferber",
	"Franco",
	"Galinier",
	"Han",
	"Lawrence",
	"Lepied",
	"Lopez",
	"Mariotti",
	"Rossi",
	"Urasawa",
	"Zwickelman",
};

void random_name(char *name)
{
	char *first_name = first_names[rand() % 22];
	char *last_name = last_names[rand() % 15];

	sprintf(name, "%s %s", first_name, last_name);
}

void hexDump(void *addr, uint32_t len)
{
	char *memory = (char *) addr;

	for (uint32_t i = 0; i < len; ++i) {
		if (i != 0 && i % 37 == 0)
			printf("\n");

		printf("%02x ", memory[i]);
	}
}

int main(int argc, char *argv[])
{
	HashTable ht;
	ht_init(&ht, 32, 4, jhash, (CmpFunc) strcmp);

	srand(time(0));

	char name[32];
	uint age;

	for (int i = 0; i < 100; ++i) {
		random_name(name);
		age = rand() % 80;
		ht_add(&ht, name, &age);
	}

	hexDump(ht.buckets, ht.slots * 37);

	uint ret;
	ht_lookup(&ht, name, &ret);

	printf("%s: (%d == %d)", name, age, ret);

	return 0;
}
