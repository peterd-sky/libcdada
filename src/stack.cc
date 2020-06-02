#include "cdada/stack.h"
#include "cdada/__common_internal.h"
#include "cdada/__stack_internal.h"

#include <stdlib.h>
#include <string.h>

using namespace std;

cdada_stack_t* __cdada_stack_create(const uint16_t val_size,
						__cdada_stack_ops_t* ops){

	__cdada_stack_int_t* m = NULL;

	if(unlikely(val_size == 0))
		return m;

	m = (__cdada_stack_int_t*)malloc(sizeof(__cdada_stack_int_t));
	memset(m, 0, sizeof(__cdada_stack_int_t));
	m->magic_num = CDADA_MAGIC;
	m->user_val_len = val_size;

	try{
		//Custom type
		if(ops){
			m->val_len = m->user_val_len = val_size;
			m->ops = ops;
			(*m->ops->create)(m);
			return m;
		}

		if(val_size == 1){
			m->stack.u8 = new stack<uint8_t>();
			m->val_len = 1;
		}else if(val_size == 2){
			m->stack.u16 = new stack<uint16_t>();
			m->val_len = 2;
		}else if(val_size > 2 && val_size <= 4){
			m->stack.u32 = new stack<uint32_t>();
			m->val_len = 4;
		}else if(val_size > 4 && val_size <= 8){
			m->stack.u64 = new stack<uint64_t>();
			m->val_len = 8;
		}else if(val_size > 8 && val_size <= 16){
			m->stack.u128 = new stack<cdada_u128_t>();
			m->val_len = 16;
		}else if(val_size > 16 && val_size <= 32){
			m->stack.u256 = new stack<cdada_u256_t>();
			m->val_len = 32;
		}else if(val_size > 32 && val_size <= 64){
			m->stack.u512 = new stack<cdada_u512_t>();
			m->val_len = 64;
		}else if(val_size > 64 && val_size <= 128){
			m->stack.u1024 = new stack<cdada_u1024_t>();
			m->val_len = 128;
		}else if(val_size > 128 && val_size <= 256){
			m->stack.u2048 = new stack<cdada_u2048_t>();
			m->val_len = 256;
		}else{
			goto ROLLBACK;
		}
	}catch(bad_alloc& e){
		goto ROLLBACK;
	}catch(...){
		CDADA_ASSERT(0);
		goto ROLLBACK;
	}

	return m;

ROLLBACK:
	free(m);
	return NULL;
}

/**
* Destroy a stack structure
*/
int cdada_stack_destroy(cdada_stack_t* stack){

	__cdada_stack_int_t* m = (__cdada_stack_int_t*)stack;

	CDADA_CHECK_MAGIC(m);

	try{
		int c = m->ops? 0 : m->val_len;
		switch(c){
			case 1:
				delete m->stack.u8;
				break;
			case 2:
				delete m->stack.u16;
				break;
			case 4:
				delete m->stack.u32;
				break;
			case 8:
				delete m->stack.u64;
				break;
			case 16:
				delete m->stack.u128;
				break;
			case 32:
				delete m->stack.u256;
				break;
			case 64:
				delete m->stack.u512;
				break;
			case 128:
				delete m->stack.u1024;
				break;
			case 256:
				delete m->stack.u2048;
				break;
			case 0:
				CDADA_ASSERT(m->ops);
				(*m->ops->destroy)(m);
				break;
			default:
				CDADA_ASSERT(0);
				return CDADA_E_UNKNOWN;
		}
	}catch(...){
		CDADA_ASSERT(0);
		return CDADA_E_UNKNOWN;
	}

	m->magic_num = 0x0;
	free(m);

	return CDADA_SUCCESS;
}

int cdada_stack_push(cdada_stack_t* stack, const void* val){

	__cdada_stack_int_t* m = (__cdada_stack_int_t*)stack;

	CDADA_CHECK_MAGIC(m);

	if(unlikely(!val))
		return CDADA_E_INVALID;

	try{
		int c = m->ops? 0 : m->val_len;
		switch(c){
			case 1:
				return cdada_stack_push_u<uint8_t>(m, m->stack.u8,
								val);
			case 2:
				return cdada_stack_push_u<uint16_t>(m,
								m->stack.u16,
								val);
			case 4:
				return cdada_stack_push_u<uint32_t>(m,
								m->stack.u32,
								val);
			case 8:
				return cdada_stack_push_u<uint64_t>(m,
								m->stack.u64,
								val);
			case 16:
				return cdada_stack_push_u<cdada_u128_t>(m,
								m->stack.u128,
								val);
			case 32:
				return cdada_stack_push_u<cdada_u256_t>(m,
								m->stack.u256,
								val);
			case 64:
				return cdada_stack_push_u<cdada_u512_t>(m,
								m->stack.u512,
								val);
			case 128:
				return cdada_stack_push_u<cdada_u1024_t>(m,
								m->stack.u1024,
								val);
			case 256:
				return cdada_stack_push_u<cdada_u2048_t>(m,
								m->stack.u2048,
								val);
			case 0:
				CDADA_ASSERT(m->ops);
				return (*m->ops->push)(m, val);
			default:
				break;
		}
	}catch(bad_alloc& e){
		return CDADA_E_MEM;
	}catch(...){}

	CDADA_ASSERT(0);
	return CDADA_E_UNKNOWN;
}

int cdada_stack_pop(cdada_stack_t* stack){

	__cdada_stack_int_t* m = (__cdada_stack_int_t*)stack;

	CDADA_CHECK_MAGIC(m);

	try{
		int c = m->ops? 0 : m->val_len;
		switch(c){
			case 1:
				return cdada_stack_pop_u<uint8_t>(m,
								m->stack.u8);
			case 2:
				return cdada_stack_pop_u<uint16_t>(m,
								m->stack.u16);
			case 4:
				return cdada_stack_pop_u<uint32_t>(m,
								m->stack.u32);
			case 8:
				return cdada_stack_pop_u<uint64_t>(m,
								m->stack.u64);
			case 16:
				return cdada_stack_pop_u<cdada_u128_t>(m,
								m->stack.u128);
			case 32:
				return cdada_stack_pop_u<cdada_u256_t>(m,
								m->stack.u256);
			case 64:
				return cdada_stack_pop_u<cdada_u512_t>(m,
								m->stack.u512);
			case 128:
				return cdada_stack_pop_u<cdada_u1024_t>(m,
								m->stack.u1024);
			case 256:
				return cdada_stack_pop_u<cdada_u2048_t>(m,
								m->stack.u2048);
			case 0:
				CDADA_ASSERT(m->ops);
				return (*m->ops->pop)(m);
			default:
				break;
		}
	}catch(...){}

	CDADA_ASSERT(0);
	return CDADA_E_UNKNOWN;
}

uint32_t cdada_stack_size(const cdada_stack_t* stack){

	__cdada_stack_int_t* m = (__cdada_stack_int_t*)stack;

	if(unlikely(!m || m->magic_num != CDADA_MAGIC))
		return 0;

	try{
		int c = m->ops? 0 : m->val_len;
		switch(c){
			case 1:
				return m->stack.u8->size();
			case 2:
				return m->stack.u16->size();
			case 4:
				return m->stack.u32->size();
			case 8:
				return m->stack.u64->size();
			case 16:
				return m->stack.u128->size();
			case 32:
				return m->stack.u256->size();
			case 64:
				return m->stack.u512->size();
			case 128:
				return m->stack.u1024->size();
			case 256:
				return m->stack.u2048->size();
			case 0:
				CDADA_ASSERT(m->ops);
				return (*m->ops->size)(m);
			default:
				break;
		}
	}catch(...){}

	CDADA_ASSERT(0);
	return 0;
}

int cdada_stack_top(const cdada_stack_t* stack, void* val){

	__cdada_stack_int_t* m = (__cdada_stack_int_t*)stack;

	CDADA_CHECK_MAGIC(m);

	if(unlikely(!val))
		return CDADA_E_INVALID;

	try{
		int c = m->ops? 0 : m->val_len;
		switch(c){
			case 1:
				return cdada_stack_top_u<uint8_t>(m, m->stack.u8,
								val);
			case 2:
				return cdada_stack_top_u<uint16_t>(m,
								m->stack.u16,
								val);
			case 4:
				return cdada_stack_top_u<uint32_t>(m,
								m->stack.u32,
								val);
			case 8:
				return cdada_stack_top_u<uint64_t>(m,
								m->stack.u64,
								val);
			case 16:
				return cdada_stack_top_u<cdada_u128_t>(m,
								m->stack.u128,
								val);
			case 32:
				return cdada_stack_top_u<cdada_u256_t>(m,
								m->stack.u256,
								val);
			case 64:
				return cdada_stack_top_u<cdada_u512_t>(m,
								m->stack.u512,
								val);
			case 128:
				return cdada_stack_top_u<cdada_u1024_t>(m,
								m->stack.u1024,
								val);
			case 256:
				return cdada_stack_top_u<cdada_u2048_t>(m,
								m->stack.u2048,
								val);
			case 0:
				CDADA_ASSERT(m->ops);
				return (*m->ops->top)(m, val);
			default:
				break;
		}
	}catch(bad_alloc& e){
		return CDADA_E_MEM;
	}catch(...){}

	CDADA_ASSERT(0);
	return CDADA_E_UNKNOWN;
}

bool cdada_stack_empty(const cdada_stack_t* stack){

	__cdada_stack_int_t* m = (__cdada_stack_int_t*)stack;

	if(unlikely(!m || m->magic_num != CDADA_MAGIC))
		return false;

	try{
		int c = m->ops? 0 : m->val_len;
		switch(c){
			case 1:
				return cdada_stack_empty_u<uint8_t>(m,
								m->stack.u8);
			case 2:
				return cdada_stack_empty_u<uint16_t>(m,
								m->stack.u16);
			case 4:
				return cdada_stack_empty_u<uint32_t>(m,
								m->stack.u32);
			case 8:
				return cdada_stack_empty_u<uint64_t>(m,
								m->stack.u64);
			case 16:
				return cdada_stack_empty_u<cdada_u128_t>(m,
								m->stack.u128);
			case 32:
				return cdada_stack_empty_u<cdada_u256_t>(m,
								m->stack.u256);
			case 64:
				return cdada_stack_empty_u<cdada_u512_t>(m,
								m->stack.u512);
			case 128:
				return cdada_stack_empty_u<cdada_u1024_t>(m,
								m->stack.u1024);
			case 256:
				return cdada_stack_empty_u<cdada_u2048_t>(m,
								m->stack.u2048);
			case 0:
				CDADA_ASSERT(m->ops);
				return (*m->ops->empty)(m);
			default:
				break;
		}
	}catch(...){}

	CDADA_ASSERT(0);
	return false;
}