#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    //check if command is valid
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    //open file
    FILE *input_file = fopen(argv[1], "r");

    //check if input_file is valid
    if (input_file == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }

    //declare variables
    //blocks of 512
    unsigned char buffer[512];

    //count images found
    int count_image = 0;

    //pointer to found images
    FILE *output_file = NULL;

    //allocate memory
    char *filename = malloc(8 * sizeof(char));

    //read file
    while (fread(buffer, sizeof(char), 512, input_file))
    {
        //check first 4 bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            //close files after opened
            if (count_image > 0)
            {
                fclose(output_file);
            }

            //create filename
            sprintf(filename, "%03i.jpg", count_image);

            //open file to write
            output_file = fopen(filename, "w");

            //track number of images
            count_image++;
        }

        //check if not used
        if (output_file != NULL)
        {
            fwrite(buffer, sizeof(char), 512, output_file);
        }
    }

    //free memory
    free(filename);

    //close files
    fclose(input_file);
    fclose(output_file);

    return 0;
}