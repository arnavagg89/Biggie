// This module provides support for arbitrarily large ints,
// affectionately known as "biggies"


#include <stdbool.h>

// NOTE: the running times are logn becuase the number of digits
//       in a number n is log_10(n)

// The following applies to all functions:
// requires: all biggie parameters are valid (not NULL) 

struct biggie;

// biggie_create(s) creates a biggie with the value represented in s
//   or returns NULL if s is not a valid integer representation
// requires: s is valid (not NULL)
// effects: may allocate memory (client must call biggie_destroy)
// time: O(n), where n is the length of s
struct biggie *biggie_create(const char *s);

// biggie_destroy(big) destroys big
// effects: big is no longer valid
// time: O(1)
void biggie_destroy(struct biggie *big);

// biggie_print() prints the biggie to output followed by
//   an optional newline (trailing \n is printed if newline is true)
// effects: prints output
// time: O(logn)
void biggie_print(const struct biggie *big, bool newline);

// biggie_add(n, m) modifies n so that n = n + m
// effects: modifies n
// time: O(logn + logm)
void biggie_add(struct biggie *n, const struct biggie *m);

// biggie_sub(n, m) modifies n so that n = n - m
// effects: modifies n
// time: O(logn + logm)
void biggie_sub(struct biggie *n, const struct biggie *m);

// biggie_mult(n, m) modifies n so that n = n * m
// effects: modifies n
// time: O(logn * logm)
void biggie_mult(struct biggie *n, const struct biggie *m);

// biggie_eq(n, m) determines if n == m
// time: O(logn + logm)
bool biggie_eq(const struct biggie *n, const struct biggie *m);

// biggie_gt(n, m) determines if n > m
// time: O(logn + logm)
bool biggie_gt(const struct biggie *n, const struct biggie *m);

// biggie_copy(n) returns a new biggie, equal to n
// effects: allocates memory (client must call biggie_destroy)
// time: O(logn)
struct biggie *biggie_copy(const struct biggie *n);