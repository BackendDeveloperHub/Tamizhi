#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "codegen.h"

int main(int argc, char *argv[]) {
    // 1. பயன்பாட்டு முறை சரிபார்ப்பு
    if (argc < 2) {
        fprintf(stderr, "பயன்பாடு: tamizhi <filename.tz> அல்லது tamizhi run <filename.tz>\n");
        return 1;
    }

    char *target_file;

    // 2. 'run' என்ற வார்த்தை இருந்தால் அடுத்த ஆர்குமெண்ட்டை கோப்பாக எடு
    if (strcmp(argv[1], "run") == 0) {
        if (argc < 3) {
            fprintf(stderr, "பிழை: கோப்புப் பெயரை உள்ளிடவும்!\n");
            return 1;
        }
        target_file = argv[2];
    } else {
        target_file = argv[1];
    }

    // 3. தமிழி கோப்பைத் திறத்தல்
    FILE *file = fopen(target_file, "r");
    if (!file) {
        perror("கோப்பைத் திறக்க முடியவில்லை");
        fprintf(stderr, "தேடப்பட்ட கோப்பு: %s\n", target_file);
        return 1;
    }

    // 4. LLVM Backend Engine தயார் செய்தல்
    tamizhi_codegen_init();
    tamizhi_generate_entry();

    fprintf(stderr, "--- தமிழி கம்பைலர் (v0.1) ---\n");

    // 5. Parser-ஐ இயக்குதல்
    parse(file); 

    // 6. LLVM IR மற்றும் DNA-VM பணிகளை முடித்தல்
    tamizhi_codegen_finish();

    // 7. கோப்பை மூடுதல்
    fclose(file);
    fprintf(stderr, "\nதொகுப்பு மற்றும் ஆய்வு முடிந்தது.\n");

    return 0;
}
