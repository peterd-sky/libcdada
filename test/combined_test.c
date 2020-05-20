#include <stdlib.h>

#include "cdada/list.h"
#include "cdada/map.h"
#include "cdada/set.h"
#include "common.h"
#include <string.h>
#include "u552.h"

//Fwd decl
CDADA_LIST_CUSTOM_TYPE_DECL(test_u552_t);
CDADA_MAP_CUSTOM_TYPE_DECL(test_u552_t);
CDADA_SET_CUSTOM_TYPE_DECL(test_u552_t);

int main(int args, char** argv){

	int rv;
	test_u552_t key;
	cdada_list_t* list;
	cdada_map_t* map;
	cdada_set_t* set;

	list = cdada_list_create_custom(test_u552_t);
	TEST_ASSERT(list != NULL);
	map = cdada_map_create_custom(test_u552_t);
	TEST_ASSERT(map != NULL);
	set = cdada_set_create(sizeof(key));
	TEST_ASSERT(set != NULL);

	rv = cdada_list_destroy(list);
	TEST_ASSERT(rv == CDADA_SUCCESS);
	rv = cdada_map_destroy(map);
	TEST_ASSERT(rv == CDADA_SUCCESS);
	rv = cdada_set_destroy(set);
	TEST_ASSERT(rv == CDADA_SUCCESS);

	//Add your test here, and return a code appropriately...
	return rv == 0? EXIT_SUCCESS : EXIT_FAILURE;
}