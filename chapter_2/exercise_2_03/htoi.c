#include <stdio.h>
#include <ctype.h>

int htoi(const char s[]);
int main() {
    char x[]="0xFFA234\0";
    printf("%ld",htoi(x));
    return 0;
}

/* htoi: convert hexadecimal string s to integer */
int htoi(const char s[]) {
    int i = 0;
    int n = 0;
    int hexdigit;

    if (s[i] == '0' && (s[i + 1] == 'x' || s[i + 1] == 'X')) {
        i = 2;
    }

    while (s[i] != '\0') {
        if (isdigit(s[i])) {
            hexdigit = s[i] - '0';
        } else if (s[i] >= 'a' && s[i] <= 'f') {
            hexdigit = s[i] - 'a' + 10;
        } else if (s[i] >= 'A' && s[i] <= 'F') {
            hexdigit = s[i] - 'A' + 10;
        }

        n = 16 * n + hexdigit;
        i++;
    }
    return n;
}
