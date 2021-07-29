#ifndef WISH_TAGGED_PTR
#define WISH_TAGGED_PTR
#include <numeric>

#ifdef LOCKFREE_COMPRESSION
#include "tagged_ptr_compress.h"
#else
#include "tagged_ptr_dcas.h"
#endif

#endif