#include <stdio.h>
#include <string.h>
#include "reciter.h"
#include "ReciterTabs.h"
#include "debug.h"

unsigned char A, X;
extern int debug;

static unsigned char inputtemp[256];   // secure copy of input tab36096

/* Retrieve flags for character at mem59-1 */
unsigned char Code37055(unsigned char npos, unsigned char mask)
{
	X = npos;
	return tab36376[inputtemp[X]] & mask;
}

unsigned int match(const char * str) {
    while (*str) {
        unsigned char ch = *str;
        A = inputtemp[X++];
        if (A != ch) return 0;
        ++str;
    }
    return 1;
}

unsigned char GetRuleByte(unsigned short mem62, unsigned char Y) {
	unsigned int address = mem62;
	if (mem62 >= 37541) {
		address -= 37541;
		return rules2[address+Y];
	}
	address -= 32000;
	return rules[address+Y];
}

int handle_ch2(unsigned char ch, unsigned char mem) {
    unsigned char tmp;
    X = mem;
    tmp = tab36376[inputtemp[mem]];
    if (ch == ' ') {
        if(tmp & 128) return 1;
    } else if (ch == '#') {
        if(!(tmp & 64)) return 1;
    } else if (ch == '.') {
        if(!(tmp & 8)) return 1;
    } else if (ch == '^') {
        if(!(tmp & 32)) return 1;
    } else return -1;
    return 0;
}


int handle_ch(unsigned char ch, unsigned char mem) {
    unsigned char tmp;
    X = mem;
    tmp = tab36376[inputtemp[X]];
    if (ch == ' ') {
        if ((tmp & 128) != 0) return 1;
    } else if (ch == '#') {
        if ((tmp & 64) == 0) return 1;
    } else if (ch == '.') {
        if((tmp & 8) == 0) return 1;
    } else if (ch == '&') {
        if((tmp & 16) == 0) {
            if (inputtemp[X] != 72) return 1;
            ++X;
        }
    } else if (ch == '^') {
        if ((tmp & 32) == 0) return 1;
    } else if (ch == '+') {
        X = mem;
        ch = inputtemp[X];
        if ((ch != 69) && (ch != 73) && (ch != 89)) return 1;
    } else return -1;
    return 0;
}


int TextToPhonemes(unsigned char *input) {
    unsigned char mem56;      //output position for phonemes
    unsigned char mem57;
    unsigned char mem58;
    unsigned char mem59;
    unsigned char mem60;
    unsigned char mem61;
    unsigned short mem62;     // memory position of current rule

    unsigned char mem64;      // position of '=' or current character
    unsigned char mem65;     // position of ')'
    unsigned char mem66;     // position of '('

    unsigned char Y;

    int r;

    inputtemp[0] = ' ';

    // secure copy of input
    // because input will be overwritten by phonemes
    X = 0;
    do {
        A = input[X] & 127;
        if (A >= 112) A = A & 95;
        else if (A >= 96) A = A & 79;
        inputtemp[++X] = A;
    } while (X < 255);
    inputtemp[255] = 27;
    mem56 = mem61 = 255;

    do {
        do {
            X = ++mem61;
            mem64 = inputtemp[X];
            if (mem64 == '[') {
                X = ++mem56;
                input[X] = 155;
                return 1;
            }

            if (mem64 != '.') break;
            X++;
            A = tab36376[inputtemp[X]] & 1;
            if(A != 0) break;
            mem56++;
            X = mem56;
            A = '.';
            input[X] = '.';
        } while(1);

        mem57 = tab36376[mem64];
        if((mem57&2) != 0) {
            mem62 = 37541;
            while (1) {
                while ((GetRuleByte(++mem62, 0) & 128) == 0);
                Y = 0;
                while(GetRuleByte(mem62, ++Y) != '(');
                mem66 = Y;
                while(GetRuleByte(mem62, ++Y) != ')');
                mem65 = Y;
                while((GetRuleByte(mem62, ++Y) & 127) != '=');
                mem64 = Y;

                mem60 = X = mem61;

                Y = mem66 + 1;

                while(1) {
                    if (GetRuleByte(mem62, Y) != inputtemp[X]) break;
                    if(++Y == mem65) break;
                    mem60 = ++X;
                }

                if (Y == mem65) {
                    mem59 = mem61;

                    while(1) {
                        unsigned char ch;
                        while(1) {
                            mem66--;
                            mem57 = GetRuleByte(mem62, mem66);
                            if ((mem57 & 128) != 0) {
                                mem58 = mem60;
                                break;
                            }
                            X = mem57 & 127;
                            if ((tab36376[X] & 128) == 0) break;
                            if (inputtemp[mem59-1] != mem57) break;
                            --mem59;
                        }

                        ch = mem57;

                        r = handle_ch2(ch, mem59-1);
                        if (r == -1) {
                            switch (ch) {
                            case '&':
                                if (!Code37055(mem59-1,16)) {
                                    if (inputtemp[X] != 'H') r = 1;
                                    else {
                                        A = inputtemp[--X];
                                        if ((A != 'C') && (A != 'S')) r = 1;
                                    }
                                }
                                break;

                            case '@':
                                if(!Code37055(mem59-1,4)) { 
                                    A = inputtemp[X];
                                    if (A != 72) r = 1;
                                    if ((A != 84) && (A != 67) && (A != 83)) r = 1;
                                }
                                break;
                            case '+':
                                X = mem59;
                                A = inputtemp[--X];
                                if ((A != 'E') && (A != 'I') && (A != 'Y')) r = 1;
                                break;
                            case ':':
                                while (Code37055(mem59-1,32)) --mem59;
                                continue;
                            default:
                                return 0;
                            }
                        }

                        if (r == 1) break;

                        mem59 = X;
                    }

                    do {
                        X = mem58+1;
                        if (inputtemp[X] == 'E') {
                            if((tab36376[inputtemp[X+1]] & 128) != 0) {
                                A = inputtemp[++X];
                                if (A == 'L') {
                                    if (inputtemp[++X] != 'Y') break;
                                } else if ((A != 'R') && (A != 'S') && (A != 'D') && !match("FUL")) break;
                            }
                        } else {
                            if (!match("ING")) break;
                            mem58 = X;
                        }

                        r = 0;
                        do {
                            while (1) {
                                Y = mem65 + 1;
                                if(Y == mem64) {
                                    mem61 = mem60;

                                    if (debug) PrintRule(mem62);

                                    while(1) {
                                        mem57 = A = GetRuleByte(mem62, Y);
                                        A = A & 127;
                                        if (A != '=') input[++mem56] = A;
                                        if ((mem57 & 128) != 0) break;
                                        Y++;
                                    }
                                }
                                mem65 = Y;
                                mem57 = GetRuleByte(mem62, Y);
                                if((tab36376[mem57] & 128) == 0) break;
                                if (inputtemp[mem58+1] != mem57) {
                                    r = 1;
                                    break;
                                }
                                ++mem58;
                            }

                            if (r == 0) {
                                A = mem57;
                                if (A == '@') {
                                    if(Code37055(mem58+1, 4) == 0) {
                                        A = inputtemp[X];
                                        if ((A != 82) && (A != 84) && 
                                            (A != 67) && (A != 83)) r = 1;
                                    } else {
                                        r = -2;
                                    }
                                } else if (A == ':') {
                                    while (Code37055(mem58+1, 32)) mem58 = X;
                                    r = -2;
                                } else r = handle_ch(A, mem58+1);
                            }

                            if (r == 1) break;
                            if (r == -2) { 
                                r = 0;
                                continue;
                            }
                            if (r == 0) mem58 = X;
                        } while (r == 0);
                    } while (A == '%');
                    return 0;
                }
            }
        } while (mem57 == 0);

        if(!(mem57 & 128)) return 0;

        // go to the right rules for this character.
        X = mem64 - 'A';
        mem62 = tab37489[X] | (tab37515[X]<<8);
    } while(1);

    return 0;
}
