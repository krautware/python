#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

bool fields_build = false;
#define MAX_SOLUTIONS 10000
int positions[MAX_SOLUTIONS];
uint64_t fields[64] = {0};
char letters[] = "abcdefgh";

int build_fields(void){
    uint64_t pattern = 1, pattern2;
    uint64_t rows[8] = {0};
    uint64_t lines[8] = {0};
    uint64_t diagonal_elements[8] = {0};
    uint64_t r_diagonal_vector = 0, l_diagonal_vector = 0;
    int i, j, count;
    const int LEFT = 7;
    const int RIGHT = 9;

    if(fields_build)
        return -1;
    
    /* build the row vectors*/
    for(uint64_t row = 0xff, i = 0; i < 8; i++){
        rows[i] = row;
        row <<= 8;
    }

    /* build the line vectors */
    for(uint64_t line = 0, i = 0; i < 8; i++){
            line = pattern;
            for(int j = 0; j < 7; j++){
                line <<= 8;
                line |= pattern;
            }
            pattern <<= 1;
            lines[i] = line;
    }


    /* build the diagonals */

    /* firstly build the list of fields and bitpattern 
       r_diagonal_vector contains the bitpattern of the diagonal A1-H8
       l_diagonal_vector the same of diagonal H1-A8 */
    diagonal_elements[0] = 0, r_diagonal_vector = 1, pattern = 1, l_diagonal_vector = 128, pattern2 = 128;
    for(i = 1; i < 8; i++){
        diagonal_elements[i] = diagonal_elements[i-1] + 9;
        pattern <<= 9, pattern2 <<= 7;
        r_diagonal_vector |= pattern;
        l_diagonal_vector |= pattern2;
    }

    /* And now set the pattern in the table of fields
       diagonal_elements contain the fields of the diagonal A1-H8
       the correspomdig fields of the diagonal H8-A1 are computed by the formula
       56 - diagonal_elements[i] + 2*i with i in [0..7] */
    for(i = 0; i < 8; i++){
        fields[diagonal_elements[i]] |= r_diagonal_vector;
        fields[56 - diagonal_elements[i] + 2*i] |= l_diagonal_vector;
    }

    /* And now all the other diagonals! Starting with B1-A2 (corresponding H7-G8)
       B1-H7 (A2-G8), C1-A3 ... and so on til G1-A7 */
    
    for(i = 1; i < 7; i++){
        diagonal_elements[7] = 0, l_diagonal_vector=0, r_diagonal_vector = 0, pattern = 0;
        int end = i;
        for(int k = 0, offset = LEFT; k < 2; ++k, offset = RIGHT, end = 7 -i){
            diagonal_elements[0] = i;
            l_diagonal_vector |= (1 << i), pattern |= (1 << i);
            for(count = 1, j = end;j > 0;j--, count++){
                diagonal_elements[count] = diagonal_elements[count - 1] + offset;
                l_diagonal_vector |= (pattern <<= offset);
            }
            diagonal_elements[count] = 0;
            r_diagonal_vector = l_diagonal_vector << (63 - diagonal_elements[count - 1] - diagonal_elements[0]);
           
            /* And now set the fields in the table with the bitpattern */
            for(j = 0; j < count; j++){
                fields[diagonal_elements[j]] |= l_diagonal_vector;
                fields[63 - diagonal_elements[j]] |= r_diagonal_vector;
            }
            l_diagonal_vector = r_diagonal_vector = pattern = 0;
        }
    } 

	/* Finally set the bitpatterns for rows and lines */
	for(i = 0; i < 64; i++){
		fields[i] |= (lines[(i & 7)]|rows[(i >> 3)]);
	} 
    fields_build = true;
    return 0;
}

bool check_board(int a, int b, int c, int d, int e){
    bool res = false;

    if(fields_build == false)
        build_fields();
    if((fields[a] | fields[b] | fields[c] | fields[d] | fields[e]) == UINT64_MAX)
        res = true;
    return res;
}

char *toBinaryString(uint64_t n, char *s)
{
    uint64_t p = 1;
    int i;
    char *t = s;

    p <<= 63;
    for(i = 0; i < 64; i++){
        *s = (n & p) ? '1' : '0';
        s++, p >>= 1;
        if(7 == (i & 7)){
            *s++ = '-';
        } 
    }

    return (s = t);

}
int print_board(int from, int til){
    int i, j;
    char header[] ={"| A | B | C | D | E | F | G | H |"};
    char *line = "____________________________________";
    char s[80] = {0};
    
    union {
        uint64_t t;
        char a[8];
    } z;
    
    
    for(i = 0; i < 64; i++){
        z.t = fields[i];
        printf("Feld: %c%d\n", letters[(i & 7)], (i>>3) + 1) ;
        printf("%2d= %s\n", i, toBinaryString(fields[i], (char *) &s));
        printf("  %s    \n", header);
        printf("%s\n", line);
        for(j = 7; j >= 0; j--){
            printf("%d | ", j + 1);
            printf("%c | %c | %c | %c | %c | %c | %c | %c |\n",  
                    z.a[j] & 0x01 ? ((((j<<3)) == i) ? 'D' :'x') : ' ',
                    z.a[j] & 0x02 ? ((((j<<3) + 1) == i) ? 'D' :'x') : ' ',
                    z.a[j] & 0x04 ? ((((j<<3) + 2) == i) ? 'D' :'x') : ' ',
                    z.a[j] & 0x08 ? ((((j<<3) + 3) == i) ? 'D' :'x') : ' ',
                    z.a[j] & 0x10 ? ((((j<<3) + 4) == i) ? 'D' :'x') : ' ',
                    z.a[j] & 0x20 ? ((((j<<3) + 5) == i) ? 'D' :'x') : ' ',
                    z.a[j] & 0x40 ? ((((j<<3) + 6) == i) ? 'D' :'x') : ' ',
                    z.a[j] & 0x80 ? ((((j<<3) + 7) == i) ? 'D' :'x') : ' ');
            printf("%s\n", line);
        }
        printf("  %s    \n", header);

        /* getchar(); */

        
    }    
    return 0;
}

char *extract_position(int p, char *buf)
{
    int res[5], i;
    char *s = buf;
    static int count = 0;

    ++count;

    for(i = 4; i >= 0; --i){
        res[i] = p & 63;
        p >>= 6;
        if(count == 1){
            printf("%d ", res[i]);
        }
    }

    for(i = 0; i  < 5; ++i){
        sprintf(s, "%c%d ", letters[res[i]&7], (res[i]>>3) + 1);
        if(count == 1){
            printf("Feldnr: %d -> buf = %s - s = %s len(buf) = %lu - len(s) %lu\n", 
                        res[1], buf, s, strlen(buf), strlen(s));
        }
        s += strlen(s);
    }
    if(count == 1){
        printf("Puffer = %s\n", buf);
    }
    *(buf + strlen(buf) - 1) = 0;

    return buf;
    
}
int main(int argc, char *argv[])
{
    int count = 0; 
    int hit = 0;
    int res, i;
    int hit_array[10000];
    clock_t start, end;
    double time_taken;
    FILE *fp;
    char buf[20] = {0};
    
    start = clock();
    build_fields();
    for(int d1 = 0;d1 < 60; d1++){
        for(int d2 = d1 + 1; d2 < 61; d2++){
            for(int d3 = d2 + 1; d3 < 62; d3++){
                for(int d4 = d3 +1; d4 < 63; d4++){
                    for(int d5 = d4 + 1;d5 < 64; d5++){
                        count++;
                        if(check_board(d1, d2, d3, d4, d5)){
                            hit_array[hit] = count;
                            if(hit == 0){
                                printf("%d %d %d %d %d\n", d5, d4, d3, d2, d1);
                            }
                            if(hit < MAX_SOLUTIONS){
                                res = d1;
                                res <<= 6; res += d2;
                                res <<= 6; res += d3;
                                res <<= 6; res += d4;
                                res <<= 6; res += d5;
                                positions[hit] = res;
                            } 
                            hit++;
                        }                            
                    }
                }
            }
        }
    }
    
    /* Dump the positions vector into file */
    if((fp = fopen("positions.txt", "w")) == NULL){
        printf("Kann Datei nicht zum Schreiben öffnen!\n");
    }
    else{
        printf("Speichere den Ergebnisvektor unter positions.txt\n");
        for(i = 0; i < ((hit < MAX_SOLUTIONS) ? hit : (hit + 1)); ++i){
            /* printf("%s\n", extract_position(positions[i], (char *) &buf)); */
            fprintf(fp, "%s\n", extract_position(positions[i], (char *) &buf));
        }
        fclose(fp);
    }
    printf("Zahl der Durchläufe: %d\nZahl der Treffer: %d\n", count, hit);
    end = clock();
    time_taken = ((double) end - (double) start)/CLOCKS_PER_SEC;
    printf("Gesamtlaufzeit: %f Sekunden\n", time_taken);
    exit(0);
}
