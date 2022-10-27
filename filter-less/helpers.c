#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //TODO
    //look for each pixel [height][width]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //define colors
            int red = image[i][j].rgbtRed;
            int green = image[i][j].rgbtGreen;
            int blue = image[i][j].rgbtBlue;

            //calculate average
            float rgbaverage = (round(red) + round(blue) + round(green)) / 3;

            //round average
            rgbaverage = round(rgbaverage);

            //set color to the average
            image[i][j].rgbtRed = rgbaverage;
            image[i][j].rgbtGreen = rgbaverage;
            image[i][j].rgbtBlue = rgbaverage;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //TODO
    //declare sepia colors
    int sepiared;
    int sepiagreen;
    int sepiablue;

    //look for each pixel [height][width]
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //define colors
            float red = image[i][j].rgbtRed;
            float green = image[i][j].rgbtGreen;
            float blue = image[i][j].rgbtBlue;

            //define sepia colors
            sepiared = round(0.393 * red + 0.769 * green + 0.189 * blue);
            sepiagreen = round(0.349 * red + 0.686 * green + 0.168 * blue);
            sepiablue = round(0.272 * red + 0.534 * green + 0.131 * blue);

            //limit to 255
            if (sepiared > 255)
            {
                sepiared = 255;
            }

            if (sepiagreen > 255)
            {
                sepiagreen = 255;
            }

            if (sepiablue > 255)
            {
                sepiablue = 255;
            }

            //update values
            image[i][j].rgbtRed = sepiared;
            image[i][j].rgbtGreen = sepiagreen;
            image[i][j].rgbtBlue = sepiablue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //TODO
    //look for each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - (j + 1)];
            image[i][width - (j + 1)] = temp;
        }

    }



    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //TODO
    //create copy
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }

    //look through each pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalred, totalgreen, totalblue;
            totalred = totalgreen = totalblue = 0;
            float counter = 0.00;

            //look for neighbors
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int currentx = i + x;
                    int currenty = j + y;

                    //check if neighbor is valid
                    if (currentx < 0 || currentx > (height - 1) || currenty < 0 || currenty > (width - 1))
                    {
                        continue;
                    }

                    //get values of valid neighbors
                    totalred += image[currentx][currenty].rgbtRed;
                    totalgreen += image[currentx][currenty].rgbtGreen;
                    totalblue += image[currentx][currenty].rgbtBlue;

                    counter++;
                }

                //calculate average of neigbors
                temp[i][j].rgbtRed = round(totalred / counter);
                temp[i][j].rgbtGreen = round(totalgreen / counter);
                temp[i][j].rgbtBlue = round(totalblue / counter);
            }
        }
    }

    //copy new pixels to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = temp[i][j].rgbtRed;
            image[i][j].rgbtGreen = temp[i][j].rgbtGreen;
            image[i][j].rgbtBlue = temp[i][j].rgbtBlue;
        }
    }
    return;
}
