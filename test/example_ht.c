#include "Hash.h"
#include "HashTable.h"
#include "Macros.h"

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

void random_name(char *name, usize size)
{
	char *first_name = first_names[rand() % 22];
	char *last_name = last_names[rand() % 15];

	snprintf(name, size, "%s %s", first_name, last_name);
}

#define NAME_SIZE 27

int main(int argc, char *argv[])
{
	HashTable ht;
	ht_init(&ht, NAME_SIZE, 8, 0xdeadbeef, jhash, (CmpFunc) strcmp);

	srand(time(0));

	char name[NAME_SIZE] = { 0 };
	u64 age;

	for (int i = 0; i < 100; ++i) {
		random_name(name, NAME_SIZE);
		age = rand() % 80;
		ht_add(&ht, name, &age);
	}

	struct Person {
		char name[NAME_SIZE];
		u64 age;
	};

	usize len = 0;
	struct Person buf[ht.used];

	ht_remove_all(&ht, paddingof(struct Person, name, age), buf, &len);

	for (int i = 0; i < len; ++i) {
		struct Person persona = buf[i];

		printf("%s: %lu\n", persona.name, persona.age);
	}

	ht_destroy(&ht, NULL, NULL);

	return 0;
}
