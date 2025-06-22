#define int const int

#include <kernel/example/test.h>

#define a_img b_img
#define a_img_len b_img_len
#include <kernel/example/test2.h>
#undef a_img
#undef a_img_len

#define a_img c_img
#define a_img_len c_img_len
#include <kernel/example/test3.h>
#undef a_img
#undef a_img_len

#undef int
