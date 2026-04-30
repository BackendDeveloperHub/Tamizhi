#include <stdio.h>

int dna_to_pair(char dna) {
    switch (dna) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default: return 0;
    }
}

void decode_logic(const char* dna_path, const char* output_path) {
    FILE *fin = fopen(dna_path, "r");
    FILE *fout = fopen(output_path, "wb");
    if (!fin || !fout) { printf("Error opening files!\n"); return; }

    int dna_chars[4];
    while ((dna_chars[0] = fgetc(fin)) != EOF) {
        dna_chars[1] = fgetc(fin);
        dna_chars[2] = fgetc(fin);
        dna_chars[3] = fgetc(fin);

        unsigned char ch = 0;
        ch |= (dna_to_pair(dna_chars[0]) << 6);
        ch |= (dna_to_pair(dna_chars[1]) << 4);
        ch |= (dna_to_pair(dna_chars[2]) << 2);
        ch |= (dna_to_pair(dna_chars[3]));
        fputc(ch, fout);
    }
    fclose(fin); fclose(fout);
    printf("வெற்றி! அசல் கோப்பு மீட்டெடுக்கப்பட்டது.\n");
}