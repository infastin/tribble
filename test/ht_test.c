#include "HashTable.h"
#include "Macros.h"
#include "Messages.h"
#include "Rand.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define htb_bucket(ht, buckets, i) ((void *) (((char *) buckets) + (i) * (ht)->bucketsize))
#define htb_key(ht, buckets, i) (htb_bucket(ht, buckets, i))
#define htb_value(ht, buckets, i) ((void *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize))
#define htb_occupied(ht, buckets, i) ((bool *) (((char *) htb_bucket(ht, buckets, i)) + (ht)->keysize + (ht)->valuesize))

#define ht_key(ht, i) (htb_key(ht, (ht)->buckets, i))
#define ht_value(ht, i) (htb_value(ht, (ht)->buckets, i))
#define ht_occupied(ht, i) (htb_occupied(ht, (ht)->buckets, i))

char first_names[32][16] = {
	"Adam",
	"Liam",
	"Adrian",
	"Noah",
	"Anna",
	"Oliver",
	"Charlotte",
	"James",
	"William",
	"Benjamin",
	"Lucas",
	"Henry",
	"Theodore",
	"Elijah",
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

char last_names[32][16] = {
	"Bailey",
	"Berat",
	"Chen",
	"Farquharson",
	"Ferber",
	"Franco",
	"Galinier",
	"Smith",
	"Johnson",
	"Williams",
	"Brown",
	"Jones",
	"Garcia",
	"Miller",
	"Davis",
	"Rodriguez",
	"Martinez",
	"Hernandez",
	"Wilson",
	"Anderson",
	"Han",
	"Lawrence",
	"Lepied",
	"Lopez",
	"Mariotti",
	"Rossi",
	"Urasawa",
	"Taylor",
	"Thomas",
	"Moore",
	"Jackson",
	"Lee"
};

TrbXs128ss state;

char names[32][32] = { 0 };
u32 names_len = 0;

void random_name(char *name)
{
	char *first_name = first_names[trb_xs128ss_next(&state) % 32];
	char *last_name = last_names[trb_xs128ss_next(&state) % 32];

	sprintf(name, "%s %s", first_name, last_name);
}

usize get_name_index(const char *name)
{
	for (u32 i = 0; i < names_len; ++i) {
		if (strcmp(names[i], name) == 0) {
			return i;
		}
	}

	return (usize) -1;
}

void generate_names()
{
	for (u32 i = 0; i < 32; ++i) {
		char name[32] = { 0 };
		do {
			random_name(name);
		} while (get_name_index(name) != (usize) -1);
		memcpy(names[names_len++], name, 32);
	}
}

usize test_hash_func(const void *key, usize, usize)
{
	usize index = get_name_index(key);

	if (index == (usize) -1)
		return trb_xs128ss_next(&state) & 31;

	return ((index + 1) << 1) & 63;
}

bool search_hash_table(TrbHashTable *ht, char *name, u32 pos)
{
	usize slot = pos;

	for (usize i = pos ?: 1;; ++i) {
		if (*ht_occupied(ht, slot) == FALSE)
			return FALSE;

		i32 cmp;
		if (ht->with_data) {
			cmp = ht->cmpd_func(name, ht_key(ht, slot), ht->data);
		} else {
			cmp = ht->cmp_func(name, ht_key(ht, slot));
		}

		if (cmp == 0)
			return TRUE;

		if (i >= ht->slots)
			i = 0;

		slot = (pos + ((i + i * i) >> 1)) & (ht->slots - 1);
	}
}

void test_add_destroy()
{
	TrbHashTable ht;
	trb_hash_table_init(&ht, 32, 8, trb_xs128ss_next(&state), test_hash_func, (TrbCmpFunc) strcmp);

	u32 name_indices[32];

	for (u32 i = 0; i < 8; ++i) {
		u32 name_index = trb_xs128ss_next(&state) & 31;
		char *name = names[name_index];
		trb_hash_table_add(&ht, name, &name_index);
		name_indices[i] = name_index;
	}

	for (u32 i = 0; i < 8; ++i) {
		u32 name_index = name_indices[i];
		char *name = names[name_index];
		u32 expected_slot = (((name_index + 1) << 1) & 63) & (ht.slots - 1);
		assert(search_hash_table(&ht, name, expected_slot));
	}

	for (u32 i = 8; i < 16; ++i) {
		u32 name_index = trb_xs128ss_next(&state) & 31;
		char *name = names[name_index];
		trb_hash_table_add(&ht, name, &name_index);
		name_indices[i] = name_index;
	}

	for (u32 i = 0; i < 16; ++i) {
		u32 name_index = name_indices[i];
		char *name = names[name_index];
		u32 expected_slot = (((name_index + 1) << 1) & 63) & (ht.slots - 1);
		assert(search_hash_table(&ht, name, expected_slot));
	}

	for (u32 i = 16; i < 32; ++i) {
		u32 name_index = trb_xs128ss_next(&state) & 31;
		char *name = names[name_index];
		trb_hash_table_add(&ht, name, &name_index);
		name_indices[i] = name_index;
	}

	for (u32 i = 0; i < 16; ++i) {
		u32 name_index = name_indices[i];
		char *name = names[name_index];
		u32 expected_slot = (((name_index + 1) << 1) & 63) & (ht.slots - 1);
		assert(search_hash_table(&ht, name, expected_slot));
	}

	trb_hash_table_destroy(&ht, NULL, NULL);

	assert(ht.slots == 0);
	assert(ht.used == 0);
	assert(ht.data == NULL);
}

void test_no_value()
{
	TrbHashTable ht;
	trb_hash_table_init(&ht, 32, 0, trb_xs128ss_next(&state), test_hash_func, (TrbCmpFunc) strcmp);

	u32 name_indices[32];

	for (u32 i = 0; i < 8; ++i) {
		u32 name_index = trb_xs128ss_next(&state) & 31;
		char *name = names[name_index];
		trb_hash_table_add(&ht, name, NULL);
		name_indices[i] = name_index;
	}

	for (u32 i = 0; i < 8; ++i) {
		u32 name_index = name_indices[i];
		char *name = names[name_index];
		u32 expected_slot = (((name_index + 1) << 1) & 63) & (ht.slots - 1);
		assert(search_hash_table(&ht, name, expected_slot));
	}

	for (u32 i = 8; i < 16; ++i) {
		u32 name_index = trb_xs128ss_next(&state) & 31;
		char *name = names[name_index];
		trb_hash_table_add(&ht, name, NULL);
		name_indices[i] = name_index;
	}

	for (u32 i = 0; i < 16; ++i) {
		u32 name_index = name_indices[i];
		char *name = names[name_index];
		u32 expected_slot = (((name_index + 1) << 1) & 63) & (ht.slots - 1);
		assert(search_hash_table(&ht, name, expected_slot));
	}

	for (u32 i = 16; i < 32; ++i) {
		u32 name_index = trb_xs128ss_next(&state) & 31;
		char *name = names[name_index];
		trb_hash_table_add(&ht, name, &name_index);
		name_indices[i] = name_index;
	}

	for (u32 i = 0; i < 16; ++i) {
		u32 name_index = name_indices[i];
		char *name = names[name_index];
		u32 expected_slot = (((name_index + 1) << 1) & 63) & (ht.slots - 1);
		assert(search_hash_table(&ht, name, expected_slot));
	}

	trb_hash_table_destroy(&ht, NULL, NULL);
}

int main()
{
	trb_xs128ss_init(&state, 0xdeadbeef);

	generate_names();
	test_add_destroy();
	test_no_value();

	return 0;
}
