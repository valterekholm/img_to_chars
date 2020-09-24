#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

// SIGWINCH is called when the window is resized.
void handle_winch(int sig) {
    signal(SIGWINCH, SIG_IGN);

    // Reinitialize the window to update data structures.
    endwin();
    initscr();
    refresh();
    clear();

    char tmp[128];
    sprintf(tmp, "%dx%d", COLS, LINES);

    // Approximate the center
    int x = COLS / 2 - strlen(tmp) / 2;
    int y = LINES / 2 - 1;

    mvaddstr(y, x, tmp);
    refresh();

    signal(SIGWINCH, handle_winch);
}

unsigned int readBMP(char* filename, unsigned char** data)
{
    printf("readBMP with filename %s", filename);
    int i;
    FILE* f = fopen(filename, "rb");
    unsigned char info[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

    printf("%s", info);



    // extract image height and width from header
    int width, height;
    memcpy(&width, info + 18, sizeof(int));
    memcpy(&height, info + 22, sizeof(int));

    printf("w: %d, h: %d", width, height);

    int heightSign = 1;
    if (height < 0) {
        heightSign = -1;
    }

    int size = 3 * width * abs(height);
    printf("Size: %d", size);
    *data = NULL;
    *data =  (unsigned char*)malloc(size);//char[size]; // allocate 3 bytes per pixel
    printf("did malloc");
    if(*data==NULL){
        free(data);
        printf("\nERROR: Memory allocation did not complete successfully!");
    }
    fread(*data, sizeof(unsigned char), size, f); // read the rest of the data at once
    //free it later in calling function?
    for(int i=0; i<size; i++){
        printf("%d , ", *data[i]);
    }
    fclose(f);

    if (heightSign == 1) {
        for (i = 0; i < size; i += 3)
        {
            //code to flip the image data here....
        }
    }
    return size;
}

int main(int argc, char *argv[]) {
    int useC = 0, width = 0, height = 0;

    printf("main");

    if (useC>0) {
        initscr();
        // COLS/LINES are now set
        width = COLS;
        height = LINES;

        signal(SIGWINCH, handle_winch);

        while (getch() != 27) {
            /* Nada */
        }

        endwin();
    }
    else{
    }

    unsigned char* data;

    readBMP("test.bmp", &data);

    //printf("size: %d, data: %s", size, data);
    free(data);
    return(0);
}
