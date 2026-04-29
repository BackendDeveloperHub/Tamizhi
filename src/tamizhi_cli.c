#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_banner() {
    printf("----------------------------------------\n");
    printf("   தமிழி (Tamizhi) Compiler V0.1       \n");
    printf("   Open Source @ Backend Developer Hub \n");
    printf("   Author: Prabakaran                  \n");
    printf("----------------------------------------\n\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_banner();
        printf("பயன்பாடு: tamizhi run <filename.tz>\n");
        return 1;
    }

    char *command = argv[1];
    char *filename = argv[2];

    if (strcmp(command, "run") == 0) {
        char base_name[256];
        strcpy(base_name, filename);
        char *dot = strrchr(base_name, '.');
        if (dot) *dot = '\0'; // .tz எக்ஸ்டென்ஷனை நீக்க

        char cmd[1024];

        // 1. LLVM IR உருவாக்குதல்
        printf("[1/3] தமிழி ஆய்வு செய்கிறது...\n");
        sprintf(cmd, "./tamizhi_core %s > %s.ll", filename, base_name);
        if (system(cmd) != 0) {
            printf("தவறு: தமிழி கோப்பில் பிழை உள்ளது!\n");
            return 1;
        }

        // 2. Clang மூலம் பைனரி உருவாக்குதல்
        printf("[2/3] மெஷின் கோடாக மாற்றுகிறது...\n");
        sprintf(cmd, "clang -x ir %s.ll -o %s_bin", base_name, base_name);
        system(cmd);

        // 3. பைனரியை இயக்குதல்
        printf("[3/3] இயங்குகிறது:\n\n");
        sprintf(cmd, "./%s_bin", base_name);
        system(cmd);

        // Cleanup (Optional)
        // sprintf(cmd, "rm %s.ll %s_bin", base_name, base_name);
        // system(cmd);
    } else {
        printf("தவறான கட்டளை: %s\n", command);
    }

    return 0;
}
