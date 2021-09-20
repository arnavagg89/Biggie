#include <stdlib.h>
#include "biggie.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "read-str.h"
#include "cs136-trace.h"

// see biggie.h for details

struct biggie {
  bool negative;
  char *digits;
};

struct biggie *biggie_create(const char *s) {
  assert(s);
  int len = strlen(s);
  if (len == 0) {
    return NULL;
  } else if (len > 1) {
    if(s[0] == '0') {
      return NULL;
    }
  }

  if(s[0] == '-') {
    if(s[1] == '0') {
      return NULL;
    }
  }

  for(int i = 0; i < len; i++) {
    if((s[0] == 45 || (s[i] >= 48 && s[i] <= 57))) {
      continue;
    } else {
      return NULL;
    }
  }

  struct biggie *b = malloc(sizeof(struct biggie));
  if(s[0] == '-') {
    b->negative = 1;
    b->digits = malloc(len * sizeof(char));
    b->digits[len - 1] = '\0';
    for(int i = 0, j = len - 1; j >= 1; i++, --j) {
      b->digits[i] = s[j];
    }
  }else {
    b->negative = 0;
    b->digits = malloc((len + 1) * sizeof(char));
    b->digits[len] = '\0';
    for(int i = 0, j = len - 1; j >= 0; i++, --j) {
      b->digits[i] = s[j];
    }
  }
  return b;
}

// see biggie.h for details
void biggie_destroy(struct biggie *big) {
  assert(big);
  free(big->digits);
  free(big);
}

// see biggie.h for details
void biggie_print(const struct biggie *big, bool newline) {
  assert(big);
  if(big->negative) {
    printf("%c", 45);
  }
  for(int i = strlen(big->digits) - 1; i >= 0; --i) {
    printf("%c", big->digits[i]);
  }
  if(newline) {
    printf("\n");
  } 
}

// absol_greater(n, m) returns a bool depending on the magnitude of the digits 
//   in the struct
// note: returns true if "n" is >= to "m"
//       return false if "n" is < than "m"
// time: O(n) where n is the length of digits in struct "n"
bool absol_greater(struct biggie *n, const struct biggie *m) {
  int len1 = strlen(n->digits);
  int len2 = strlen(m->digits);
  if (len1 > len2) {
    return true;
  } else if (len2 > len1) {
    return false;
  } else {
    for (int i = (len1 - 1); i >= 0; i--) {
      if (n->digits[i] > m->digits[i]) {
        return true;
      } else if (n->digits[i] == m->digits[i]) {
        continue;
      } else {
        return false;
      }
    }
    return true;
  }
}

// absol_add(n, m) sums the absolute value of the digits currently stored in 
//   the structs, n and m.
// requires: all biggie parameters are valid (not NULL)
//           n >= m
// time: O(n) where n represents the length of the struct with more digits
void absol_add(struct biggie *n, const struct biggie *m) {
  int nlen = strlen(n->digits);
  int mlen = strlen(m->digits);
  char sum = 0;
  int carry = 0;
  int i = 0;
  
  for( i = 0; i < nlen; i++) { 
    if(i < mlen) {
      sum = n->digits[i] + m->digits[i] + carry - '0' - '0';
      if(sum >= 10) {
        carry = 1;
        n->digits[i] = (sum % 10) + '0';
      } else {
        carry = 0;
        n->digits[i] = sum + '0';
      }
    } else {
      if(carry != 0) {
        sum = n->digits[i] + carry - '0';
        if(sum >= 10) {
          carry = 1;
          n->digits[i] = (sum % 10) + '0';
        } else {
          carry = 0;
          n->digits[i] = sum + '0';
        }
      }
    }
  }

  if(carry != 0) {
    n->digits = realloc(n->digits, (nlen + 2) * sizeof(char));
    nlen += 1;
    n->digits[nlen - 1] = carry + '0';
    n->digits[nlen] = '\0'; 
  }
}

// absol_sub(n, m) subtracts the absolute value of the digits currently 
//   stored in the structs, n and m.
// requires: all biggie parameters are valid (not NULL) 
//           n >= m
// time: O(n) where n represents the length of the struct with more digits
void absol_sub(struct biggie *n, struct biggie *m) {
  int nlen = strlen(n->digits);
  int mlen = strlen(m->digits);
  char subs = 0;
  int borrow = 0;

  for(int i = 0; i < nlen; i++) {
    if (i >= mlen) {
      if(n->digits[i] - '0' < borrow) {
        subs = n->digits[i] - '0' - borrow + 10;
        n->digits[i] = subs + '0';
        borrow = 1;
      } else {
        subs = n->digits[i] - borrow - '0';
        n->digits[i] = subs + '0';
        borrow = 0;
      }
    } else if(n->digits[i] - '0' >= (m->digits[i] -'0' + borrow)) {
      subs = n->digits[i] - m->digits[i] - borrow;    
      n->digits[i] = subs + '0';
      borrow = 0;
    } else {
      subs = n->digits[i] - m->digits[i] - borrow + 10;  
      n->digits[i] = subs + '0';
      borrow = 1;
    }
    n->negative = false;
  }

  while(n->digits[nlen - 1] == '0' && nlen > 1) {
    nlen -= 1;
    n->digits[nlen] = '\0';
  }
}

// see biggie.h for details
void biggie_add(struct biggie *n, const struct biggie *m) {
  assert(n);
  assert(m);
  int mlen = strlen(m->digits);
  int nlen = strlen(n->digits);
  struct biggie *cpy = biggie_copy(m);
  bool comp = absol_greater(n, m);
  if(n->negative == true && m->negative == false) {
    if(comp == false) {
      absol_sub(cpy, n);
      if(mlen > nlen) {
        n->digits = realloc(n->digits, (mlen + 1) * (sizeof(char)));
      }
      n->negative = false;
      strcpy(n->digits, cpy->digits);
    } else {
      absol_sub(n, cpy);
      if(n->negative == true) {
        n->negative = false;  
      } else {
        n->negative = true;
      }
    }
  } else if (n->negative == false && m->negative == true) {
    if(comp == true) {
      absol_sub(n, cpy);
    } else {
      absol_sub(cpy, n);
      n->negative = true;
      if(mlen > nlen) {
        n->digits = realloc(n->digits, (mlen + 1) * (sizeof(char)));
      }
      strcpy(n->digits, cpy->digits);
    }
  } else if (n->negative == true && m->negative == true){
    if(comp == false) {
      absol_add(cpy, n);
      if(mlen > nlen) {
        n->digits = realloc(n->digits, (mlen + 2) * (sizeof(char)));
      }
      strcpy(n->digits, cpy->digits);
    } else {
      absol_add(n, cpy);
    }
    n->negative = true;
  } else {
    if(comp == false) {
      absol_add(cpy, n);
      if(mlen > nlen) {
        n->digits = realloc(n->digits, (mlen + 2) * (sizeof(char)));
      }
      strcpy(n->digits, cpy->digits);
    } else {
      absol_add(n, cpy);
    }
  } 
  biggie_destroy(cpy);
}

// see biggie.h for details
void biggie_sub(struct biggie *n, const struct biggie *m) {
  assert(n);
  assert(m);
  int mlen = strlen(m->digits);
  int nlen = strlen(n->digits);

  struct biggie *cpy = biggie_copy(m);

  bool comp = absol_greater(n, m);

  if(n->negative == true && m->negative == false) {
    if(comp == false) {
      absol_add(cpy, n);
      if(mlen > nlen) {
        n->digits = realloc(n->digits, (mlen + 2) * (sizeof(char)));
      }
      strcpy(n->digits, cpy->digits);
    } else {
      absol_add(n, cpy);
    }
    n->negative = true;
  } else if (n->negative == false && m->negative == true) {
    if(comp == false) {
      absol_add(cpy, n);
      if(mlen > nlen) {
        n->digits = realloc(n->digits, (mlen + 2) * (sizeof(char)));
      }
      strcpy(n->digits, cpy->digits);
    } else {
      absol_add(n, cpy);
    }
    n->negative = false;
  } else if (n->negative == true && m->negative == true) {
    if(comp == false) {
      absol_sub(cpy, n);
      if(mlen > nlen) {
        n->digits = realloc(n->digits, (mlen + 1) * (sizeof(char)));
      }
      n->negative = false;
      strcpy(n->digits, cpy->digits);
    } else {
      absol_sub(n, cpy);
      if(n->negative == true) {
        n->negative = false;  
      } else {
        n->negative = true;
      }
    }
  } else {
    if(comp == true) {
      absol_sub(n, cpy);
    } else {
      absol_sub(cpy, n);
      n->negative = true;
      if(mlen > nlen) {
        n->digits = realloc(n->digits, (mlen + 1) * (sizeof(char)));
      }
      strcpy(n->digits, cpy->digits);
    }
  } 
  biggie_destroy(cpy);
}


// absol_mult(n, m) multiplies the absolute value of the digits currently 
//   stored in the structs, n and m.
// requires: all biggie parameters are valid (not NULL) 
//           n >= m
// time: O(n) where n represents the length of the struct with more digits
void absol_mult(struct biggie *n, const struct biggie *m) {
  int nlen = strlen(n->digits);
  int mlen = strlen(m->digits);
  char mult = 0;
  int carry = 0;
  if (((nlen == 1) && (n->digits[0] == '0')) ||
      ((mlen == 1) && (m->digits[0] == '0'))) {
    n->digits[0] = '0';
    n->digits[1] = '\0';
    n->negative = 0;
    return;
  }

  struct biggie *sum = biggie_copy(n);
  struct biggie *cpy = biggie_copy(n);

  n->digits = realloc(n->digits, (mlen + nlen + 1) * sizeof(char));

  for(int i = 0; i < mlen; i++) { 
    for(int j = 0; j < nlen; j++) {
      mult = (sum->digits[j] - '0') * (m->digits[i] - '0') + carry;
      if(mult >= 10) {
        carry = (mult / 10);
        cpy->digits[j] = (mult % 10) + '0';
      } else {
        carry = 0;
        cpy->digits[j] = mult + '0';
      }
    }
    int carry_len = 0; 
    if(carry != 0) {
      cpy->digits = realloc(cpy->digits, (nlen + 2) * sizeof(char));
      cpy->digits[nlen] = carry + '0';
      carry = 0;
      carry_len = 1;
    }
    cpy->digits[nlen + carry_len] = '\0'; 

    if(i > 0) {
      cpy->digits = realloc(cpy->digits, (nlen + i + carry_len + 2)
                                         * sizeof(char));

      for(int k = nlen + carry_len; k >= 0; k--) { 
        cpy->digits[k + i] = cpy->digits[k];
      }
      for(int k = 0; k < i; k++) {
        cpy->digits[k] = '0';
      }
    }

    cpy->digits[nlen + i + carry_len] = '\0'; 

    if(i > 0) {
      absol_add(cpy, n);
    }
    n->digits = realloc(n->digits, (nlen + i + carry_len + 2) * sizeof(char));

    strcpy(n->digits, cpy->digits);
  }


  biggie_destroy(sum);
  biggie_destroy(cpy);
}

// see biggie.h for details
void biggie_mult(struct biggie *n, const struct biggie *m) {

  assert(n);
  assert(m);
  int mlen = strlen(m->digits);
  int nlen = strlen(n->digits);

  struct biggie *cpy = biggie_copy(m);
  bool comp = absol_greater(n, m);

  if(comp == false) {
    absol_mult(cpy, n);
    n->negative = false;
    n->digits=realloc(n->digits, (mlen + nlen) * 2 * sizeof(char));
    strcpy(n->digits, cpy->digits);
  } else {
    absol_mult(n, cpy);
  }

  if(n->negative && m->negative) {
    n->negative = false;
  } else if(!n->negative && m->negative) {
    n->negative = true;
  }

  biggie_destroy(cpy);
}

// see biggie.h for details
bool biggie_eq(const struct biggie *n, const struct biggie *m) {
  assert(n);
  assert(m);

  int nlen = strlen(n->digits);
  int mlen = strlen(m->digits);
  if((nlen == mlen) && (n->negative == m->negative)) {
    for(int i = 0; i < nlen; i++) {
      if(n->digits[i] == m->digits[i]) {
        continue;
      }else {
        break;
      }
    }
    return true;
  }
  return false;  
}

// see biggie.h for details
bool biggie_gt(const struct biggie *n, const struct biggie *m) {
  assert(n);
  assert(m);
  int nlen = strlen(n->digits);
  int mlen = strlen(m->digits);
  if ((n->negative == m->negative) && (n->negative == false)) {
    if (nlen > mlen) {
      return true;
    } else if (nlen == mlen) {
      for (int i = (nlen - 1); i >= 0; i--) {
        if (n->digits[i] > m->digits[i]) {
          return true;
        } else if (n->digits[i] == m->digits[i]) {
          continue;
        } else {
          return false;
        }
      } return false;
    } else {
      return false;
    }
  } else if ((n->negative == m->negative) && (n->negative == true)) {
    if (nlen > mlen) {
      return false;
    } else if (nlen == mlen) {
      for (int i = (nlen - 1); i >= 0; i--) {
        if (n->digits[i] > m->digits[i]) {
          return false;
        } else if (n->digits[i] == m->digits[i]) {
          continue;
        } else {
          return true;
        }
      }
    } else {
      return true;
    }
  } else {
    if (n->negative == false) {
      return true;
    } else {
      return false;
    }
  }
  return true;
}

// see biggie.h for details
struct biggie *biggie_copy(const struct biggie *n) {
  assert(n);
  struct biggie *num = malloc(sizeof(struct biggie));
  num->negative = n->negative;
  int len = strlen(n->digits);
  char *number = malloc((len + 1) * sizeof(char));
  strcpy(number, n->digits);
  num->digits = number;
  return num;
}
