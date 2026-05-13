#include <stdio.h>

char get_dna_char(int pair) {
    switch (pair) {
        case 0: return 'A';
        case 1: return 'C';
        case 2: return 'G';
        case 3: return 'T';
        default: return 'A';
    }
}

void encode_logic(const char* input_path, const char* output_path) {
    FILE *fin = fopen(input_path, "rb");
    FILE *fout = fopen(output_path, "w");
    if (!fin || !fout) { printf("Error opening files!\n"); return; }

    int ch;
    while ((ch = fgetc(fin)) != EOF) {
        for (int j = 6; j >= 0; j -= 2) {
            fputc(get_dna_char((ch >> j) & 0x03), fout);
        }
    }
    fclose(fin); fclose(fout);
    printf("வெற்றி! கோப்பு DNA-வாக மாற்றப்பட்டது.\n");
}