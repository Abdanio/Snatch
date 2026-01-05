#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <direct.h>
    #include <io.h>
    #include <windows.h>
    #define MKDIR(path) _mkdir(path)
    #define CLEAR_SCREEN "cls"
    #define F_OK 0
    #define access _access
    #define YTDLP_BIN "yt-dlp.exe"
    #define FFMPEG_BIN "ffmpeg.exe"
    #define LOCAL_CMD_PREFIX ".\\"
    
    // Windows Console Colors
    #define COLOR_RESET 7
    #define COLOR_GREEN 10
    #define COLOR_CYAN 11
    #define COLOR_RED 12
    #define COLOR_YELLOW 14
    #define COLOR_MAGENTA 13
    #define COLOR_BRIGHT_WHITE 15
#else
    #include <sys/stat.h>
    #include <unistd.h>
    #define MKDIR(path) mkdir(path, 0777)
    #define CLEAR_SCREEN "clear"
    #define YTDLP_BIN "yt-dlp"
    #define FFMPEG_BIN "ffmpeg"
    #define LOCAL_CMD_PREFIX "./"
    
    // ANSI Colors for Linux/macOS
    #define COLOR_RESET "\033[0m"
    #define COLOR_GREEN "\033[1;32m"
    #define COLOR_CYAN "\033[1;36m"
    #define COLOR_RED "\033[1;31m"
    #define COLOR_YELLOW "\033[1;33m"
    #define COLOR_MAGENTA "\033[1;35m"
    #define COLOR_BRIGHT_WHITE "\033[1;37m"
#endif

#define MAX_CMD 2048
#define MAX_URL 1024
#define MAX_PATH_LEN 512

// Global variable to store the command to run yt-dlp
char ytdlp_executable[MAX_PATH_LEN];

#ifdef _WIN32
void set_color(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
#else
void set_color(const char* color) {
    printf("%s", color);
}
#endif

void print_colored(int color, const char *text) {
    #ifdef _WIN32
        set_color(color);
        printf("%s", text);
        set_color(COLOR_RESET);
    #else
        printf("%s%s%s", color, text, COLOR_RESET);
    #endif
}

void print_banner() {
    system(CLEAR_SCREEN);
    printf("\n");
    print_colored(COLOR_CYAN, "   _____ _   _    _  _______ _____ _    _ \n");
    print_colored(COLOR_CYAN, "  / ____| \\ | |  / \\|__   __/ ____| |  | |\n");
    print_colored(COLOR_MAGENTA, "  | (___ |  \\| | / _ \\  | | | |    | |__| |\n");
    print_colored(COLOR_MAGENTA, "   \\___ \\| . ` |/ ___ \\ | | | |    |  __  |\n");
    print_colored(COLOR_YELLOW, "   ____) | |\\  / /   \\ \\| | | |____| |  | |\n");
    print_colored(COLOR_YELLOW, "  |_____/|_| \\_/_/     \\_\\_|  \\_____|_|  |_|\n");
    printf("\n");
    print_colored(COLOR_GREEN, "       >> Audio Downloader v1.0 - Fast & Powerful <<\n");
    print_colored(COLOR_BRIGHT_WHITE, "  ========================================================\n");
    print_colored(COLOR_CYAN, "   [+] YouTube");
    printf(" | ");
    print_colored(COLOR_MAGENTA, "[+] SoundCloud");
    printf(" | ");
    print_colored(COLOR_GREEN, "[+] Spotify Search\n");
    print_colored(COLOR_BRIGHT_WHITE, "  ========================================================\n\n");
}

void get_input(char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

bool is_spotify(const char *url) {
    return strstr(url, "spotify.com") != NULL || strstr(url, "open.spotify.com") != NULL;
}

void check_dependencies() {
    print_colored(COLOR_CYAN, " [i] Checking system dependencies...\n");

    bool ytdlp_ok = false;
    bool ffmpeg_ok = false;

    // Check yt-dlp
    if (access(YTDLP_BIN, F_OK) == 0) {
        sprintf(ytdlp_executable, "%s%s", LOCAL_CMD_PREFIX, YTDLP_BIN);
        ytdlp_ok = true;
        print_colored(COLOR_GREEN, " [OK] ");
        printf("Found local %s\n", YTDLP_BIN);
    } else {
        int res = system("yt-dlp --version >nul 2>&1");
        #ifndef _WIN32
        res = system("yt-dlp --version >/dev/null 2>&1");
        #endif
        if (res == 0) {
            strcpy(ytdlp_executable, "yt-dlp");
            ytdlp_ok = true;
            print_colored(COLOR_GREEN, " [OK] ");
            printf("Found global yt-dlp\n");
        }
    }

    // Check ffmpeg
    if (access(FFMPEG_BIN, F_OK) == 0) {
        ffmpeg_ok = true;
        print_colored(COLOR_GREEN, " [OK] ");
        printf("Found local %s\n", FFMPEG_BIN);
    } else {
        int res = system("ffmpeg -version >nul 2>&1");
        #ifndef _WIN32
        res = system("ffmpeg -version >/dev/null 2>&1");
        #endif
        if (res == 0) {
            ffmpeg_ok = true;
            print_colored(COLOR_GREEN, " [OK] ");
            printf("Found global ffmpeg\n");
        }
    }

    // If anything is missing, run auto-setup
    if (!ytdlp_ok || !ffmpeg_ok) {
        printf("\n");
        print_colored(COLOR_YELLOW, " [!] Missing dependencies detected.\n");
        print_colored(COLOR_CYAN, " [*] Starting auto-setup to download missing files...\n");
        print_colored(COLOR_CYAN, " [*] This may take a few minutes. Please wait.\n\n");

        #ifdef _WIN32
            // Create setup.ps1
            FILE *fp = fopen("setup.ps1", "w");
            if (fp) {
                fprintf(fp, "Write-Host \" [Setup] Initializing...\"\n");
                
                // yt-dlp download logic
                if (!ytdlp_ok) {
                    fprintf(fp, "Write-Host \" [Setup] Downloading yt-dlp.exe...\"\n");
                    fprintf(fp, "Invoke-WebRequest -Uri \"https://github.com/yt-dlp/yt-dlp/releases/latest/download/yt-dlp.exe\" -OutFile \"yt-dlp.exe\"\n");
                }

                // ffmpeg download logic
                if (!ffmpeg_ok) {
                    fprintf(fp, "Write-Host \" [Setup] Downloading ffmpeg (approx 25MB)...\"\n");
                    fprintf(fp, "$url = \"https://www.gyan.dev/ffmpeg/builds/ffmpeg-release-essentials.zip\"\n");
                    fprintf(fp, "$zip = \"ffmpeg.zip\"\n");
                    fprintf(fp, "Invoke-WebRequest -Uri $url -OutFile $zip\n");
                    
                    fprintf(fp, "Write-Host \" [Setup] Extracting ffmpeg...\"\n");
                    fprintf(fp, "Expand-Archive -Path $zip -DestinationPath \"ffmpeg_temp\" -Force\n");
                    
                    fprintf(fp, "$binPath = Get-ChildItem -Path \"ffmpeg_temp\" -Recurse -Filter \"ffmpeg.exe\" | Select-Object -First 1\n");
                    fprintf(fp, "if ($binPath) {\n");
                    fprintf(fp, "    Move-Item -Path $binPath.FullName -Destination \".\\ffmpeg.exe\" -Force\n");
                    fprintf(fp, "    Write-Host \" [Setup] ffmpeg installed.\"\n");
                    fprintf(fp, "}\n");
                    
                    fprintf(fp, "Remove-Item -Path $zip -Force\n");
                    fprintf(fp, "Remove-Item -Path \"ffmpeg_temp\" -Recurse -Force\n");
                }
                
                fprintf(fp, "Write-Host \" [Setup] Done!\"\n");
                fclose(fp);

                // Run PowerShell script
                system("powershell -ExecutionPolicy Bypass -File setup.ps1");
                
                // Cleanup script
                remove("setup.ps1");

                // Re-check to confirm
                if (access(YTDLP_BIN, F_OK) == 0) {
                    sprintf(ytdlp_executable, "%s%s", LOCAL_CMD_PREFIX, YTDLP_BIN);
                    print_colored(COLOR_GREEN, " [OK] yt-dlp is ready.\n");
                }
                if (access(FFMPEG_BIN, F_OK) == 0) {
                    print_colored(COLOR_GREEN, " [OK] ffmpeg is ready.\n");
                }
            } else {
                printf(" [!] Error: Could not create setup script.\n");
            }
        #else
            printf(" [!] Auto-setup is currently only supported on Windows.\n");
            printf(" [!] Please install 'yt-dlp' and 'ffmpeg' manually.\n");
            exit(1);
        #endif
    }
    
    printf("------------------------------------------------------------\n");
}

int main() {
    char url[MAX_URL];
    char output_folder[MAX_PATH_LEN];
    char command[MAX_CMD];
    char final_url[MAX_URL];
    
    // Run dependency check first
    check_dependencies();

    print_banner();

    // 1. Input URL
    print_colored(COLOR_CYAN, " [>] ");
    get_input("Paste Link (YouTube/SoundCloud/Spotify): ", url, MAX_URL);
    
    if (strlen(url) == 0) {
        printf(" [!] Error: URL cannot be empty.\n");
        return 1;
    }

    // 2. Input Output Folder
    print_colored(COLOR_CYAN, " [+] ");
    get_input("Output Folder (default: 'downloads'): ", output_folder, MAX_PATH_LEN);
    if (strlen(output_folder) == 0) {
        strcpy(output_folder, "downloads");
    }

    // Create output directory
    MKDIR(output_folder);

    // 3. Process URL
    if (is_spotify(url)) {
        printf("\n");
        print_colored(COLOR_MAGENTA, " [~] Spotify link detected.\n");
        print_colored(COLOR_CYAN, " [i] SNATCH v1.0 uses YouTube Music search for Spotify links.\n");
        
        char query[MAX_URL];
        get_input(" [?] Enter 'Artist - Title' to search: ", query, MAX_URL);
        
        if (strlen(query) == 0) {
            printf(" [!] Error: Search query cannot be empty.\n");
            return 1;
        }
        
        // Construct ytsearch command
        sprintf(final_url, "ytsearch1:\"%s\"", query);
    } else {
        strcpy(final_url, url);
    }

    // 4. Construct Command
    printf("\n");
    print_colored(COLOR_BRIGHT_WHITE, "  ========================================================\n");
    print_colored(COLOR_GREEN, "  [*] Initializing SNATCH engine...\n");
    print_colored(COLOR_CYAN, "  [>] Target: ");
    printf("%s\n", final_url);
    print_colored(COLOR_CYAN, "  [+] Output: ");
    printf("%s\n", output_folder);
    print_colored(COLOR_BRIGHT_WHITE, "  ========================================================\n\n");
    print_colored(COLOR_YELLOW, "  [>>] Downloading & Converting... Please wait\n\n");

    // Use the determined ytdlp_executable
    // Added --quiet --no-warnings to clean up output, kept --progress
    sprintf(command, "%s -x --audio-format mp3 --audio-quality 0 --add-metadata --embed-thumbnail --progress --quiet --no-warnings --console-title -o \"%s/%%(title)s.%%(ext)s\" \"%s\"", ytdlp_executable, output_folder, final_url);

    // 5. Execute
    int result = system(command);

    if (result == 0) {
        printf("\n");
        print_colored(COLOR_BRIGHT_WHITE, "  ========================================================\n");
        print_colored(COLOR_GREEN, "  [SUCCESS] Download complete!\n");
        print_colored(COLOR_CYAN, "  [FILE] Saved in '");
        print_colored(COLOR_YELLOW, output_folder);
        print_colored(COLOR_CYAN, "' folder.\n");
        print_colored(COLOR_BRIGHT_WHITE, "  ========================================================\n");
    } else {
        printf("\n");
        print_colored(COLOR_BRIGHT_WHITE, "  ========================================================\n");
        print_colored(COLOR_RED, "  [ERROR] Download failed.\n");
        print_colored(COLOR_YELLOW, "  [TIP] If it's an ffmpeg issue, make sure ffmpeg.exe is in this folder.\n");
        print_colored(COLOR_BRIGHT_WHITE, "  ========================================================\n");
    }

    printf("\n Press Enter to exit...");
    getchar();

    return 0;
}
