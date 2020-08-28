
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//int finsert(FILE* file, const char* buffer) {
//
//    long int insert_pos = ftell(file);
//    if (insert_pos < 0) return insert_pos;
//
//    // Grow from the bottom
//    int seek_ret = fseek(file, 0, SEEK_END);
//    if (seek_ret) return seek_ret;
//    long int total_left_to_move = ftell(file);
//    if (total_left_to_move < 0) return total_left_to_move;
//
//    char move_buffer[1024];
//    long int ammount_to_grow = strlen(buffer);
//    if (ammount_to_grow >= sizeof(move_buffer)) return -1;
//
//    total_left_to_move -= insert_pos;
//
//    for (;;) {
//        unsigned int ammount_to_move = sizeof(move_buffer);
//        if (total_left_to_move < ammount_to_move) ammount_to_move = total_left_to_move;
//
//        long int read_pos = insert_pos + total_left_to_move - ammount_to_move;
//
//        seek_ret = fseek(file, read_pos, SEEK_SET);
//        if (seek_ret) return seek_ret;
//        fread(move_buffer, ammount_to_move, 1, file);
//        if (ferror(file)) return ferror(file);
//
//        seek_ret = fseek(file, read_pos + ammount_to_grow, SEEK_SET);
//        if (seek_ret) return seek_ret;
//        fwrite(move_buffer, ammount_to_move, 1, file);
//        if (ferror(file)) return ferror(file);
//
//        total_left_to_move -= ammount_to_move;
//
//        if (!total_left_to_move) break;
//
//    }
//
//    seek_ret = fseek(file, insert_pos, SEEK_SET);
//    if (seek_ret) return seek_ret;
//    fwrite(buffer, ammount_to_grow, 1, file);
//    if (ferror(file)) return ferror(file);
//
//    return 0;
//}

int main()
{
    char filename[] = "org.gnome.Vino.gschema.xml";
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Cannot open file.\n");
        exit(1);
    }

    char string_to_input[] = "<key name='enabled' type='b'>\
<summary> Enable remote access to the desktop</summary>\
<description>\
If true, allows remote access to the desktop via the RFB \
protocol.Users on remote machines may then connect to the\
desktop using a VNC viewer.\
</description>\
<default>true</default>\
</key>";

    /* SEEK_END */
    //fseek(file, -27L, SEEK_END);

    size_t size = 0;
    size_t tail_size = 0;
    size_t size_of_input_string = strlen(string_to_input);

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    printf("size : %d\n", size);
    printf("size_of_input_string : %d\n", size_of_input_string);

    //char* total_buffer = (char*)malloc(size + size_of_input_string);
    char* total_buffer = (char*)malloc(size + size_of_input_string + 2);
    if (total_buffer == NULL)
    {
        printf("ERROR: malloc() failed.\n");
        exit(EXIT_FAILURE);
    }

    rewind(file);

    char* buffer = (char*)malloc(size - 27);
    if (buffer == NULL)
    {
        printf("ERROR: malloc() failed.\n");
        exit(EXIT_FAILURE);
    }

    fread(buffer, size -27, 1, file);       // buffer contain upto -->

    printf("tail_size : %d\n", ftell(file));
    char* tail_buffer = (char*)malloc(27);
    if (tail_buffer == NULL)
    {
        printf("ERROR: malloc() failed.\n");
        exit(EXIT_FAILURE);
    }
    fread(tail_buffer, 27, 1, file);
    printf("tail_buffer : %s\n", tail_buffer);

    memmove(total_buffer, buffer, size - 27);
    
    memmove(&total_buffer[6269], string_to_input, size_of_input_string);
    memmove(&total_buffer[6550], tail_buffer, 27);

    printf("strlen(total_buffer) : %d\n", strlen(total_buffer));

    printf("total_buffer : %s\n", total_buffer);

    FILE* fw = fopen("16.xml", "w");
    if (fw == NULL)
    {
        printf("Cannot open file.\n");
        exit(EXIT_FAILURE);
    }

    fwrite(total_buffer, size + size_of_input_string + 2, 1, fw);

    //fwrite(total_buffer, size + size_of_input_string)
    //rewind(file);

    ////fseek(file, -27L, SEEK_END);



    //rewind(file);

    //printf("ftell() : %d\n", ftell(file));


    //memcpy()
    
    if (fclose(file) != 0)
    {
        exit(1);
    }

    if (fclose(fw) != 0)
    {
        exit(1);
    }


    free(total_buffer);
    free(buffer);

    return 0;
}
