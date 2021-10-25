/* 
    LAB1 ES01 RAINERI ANDREA ANGELO S280848
    ALGORITMI E STRUTTURE DATI
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>

char *cercaRegexp(char *src, char *regexp);
int checkSingleChar(char *r, char c);
int regExLen(char *regex);

int main(int argc, char const *argv[])
{
    printf("%s trovata in: %s", argv[1], cercaRegexp(argv[2], argv[1]));
    return 0;
}

char *cercaRegexp(char *src, char *regexp) {
    char *s, *r;
    s = src;
    int mismatch = 0;

    //  Recursion terminal condition: the regex requires more characters available in the string to check
    if (strlen(src) < regExLen(regexp)) {
        return NULL;
    }

    for (r = regexp; (r -  regexp) < strlen(regexp) && !mismatch; r++) {
        if (*r == '[') {
            if (*(r+1) == '^') {
                // Check for [^]
                mismatch = (checkSingleChar((r+2), *s)) ? 1 : 0;
            } else {
                // Check for []
                mismatch = (checkSingleChar(r+1, *s)) ? 0 : 1;
            }
            for ( ; *r !=  ']'; r++);
        } else if (*r == '.') {
            // Check for . --> any char will match
            mismatch = 0;
        } else if (*r == '/') {
            if (isupper(*(r+1))) {
                // Check for /A --> uppercase char
                mismatch = (isupper(*s)) ? 0 : 1; 
            } else {
                // Check for /a --> lowercase char
                mismatch = (islower(*s)) ? 0 : 1;
            }
            r++;
        } else {
            // Check for simple char --> src and regex must match perfectly
            mismatch = (*s == *r) ? 0 : 1;
        }
        s++;
    }

    if (mismatch) {
        // src does not match regex, check starting with the following char of src
        return cercaRegexp(src+1, regexp);
    } else {
        // match found, return pointer to the first occurence
        return src;
    }
}

int checkSingleChar(char *r, char c) {
    int i = 0;

    while (r[i] != ']') {
        if (r[i] == c)
            return 1;
        i++;
    }

    return 0;
}

int regExLen(char *regex) {
    int len = 0, i, j;

    for (i = 0; i < strlen(regex); i++) {
        if (regex[i] == '[') {
            for (j = i; regex[j] != ']'; j++);
            i = j;
        } else if (regex[i] == '/') {
            i++;
        }
        len++;
    }

    return len;
}
