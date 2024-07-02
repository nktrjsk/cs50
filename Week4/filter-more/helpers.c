#include "helpers.h"
#include "math.h"
#include "stdio.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE pixel = image[i][j];
            int average = round((pixel.rgbtBlue + pixel.rgbtRed + pixel.rgbtGreen) / 3.0);

            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE average;
            int avgRed = 0;
            int avgGreen = 0;
            int avgBlue = 0;

            int counter = 0;
            for (int r = -1; r <= 1; r++)
            {
                for (int c = -1; c <= 1; c++)
                {
                    if ((i + r < 0 || i + r >= height) || (j + c < 0 || j + c >= width))
                    {
                        continue;
                    }

                    RGBTRIPLE pixel = copy[i + r][j + c];
                    avgRed += pixel.rgbtRed;
                    avgGreen += pixel.rgbtGreen;
                    avgBlue += pixel.rgbtBlue;
                    counter++;
                }
            }

            average.rgbtRed = round(avgRed / (float) counter);
            average.rgbtGreen = round(avgGreen / (float) counter);
            average.rgbtBlue = round(avgBlue / (float) counter);

            image[i][j] = average;
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gx_gy_rgb[3][2] = {0};

            for (int r = -1; r <= 1; r++)
            {
                for (int c = -1; c <= 1; c++)
                {
                    if ((i + r < 0 || i + r >= height) || (j + c < 0 || j + c >= width))
                    {
                        continue;
                    }

                    RGBTRIPLE pixel = copy[i + r][j + c];

                    gx_gy_rgb[0][0] += pixel.rgbtRed * gx[r + 1][c + 1];
                    gx_gy_rgb[0][1] += pixel.rgbtRed * gy[r + 1][c + 1];
                    gx_gy_rgb[1][0] += pixel.rgbtGreen * gx[r + 1][c + 1];
                    gx_gy_rgb[1][1] += pixel.rgbtGreen * gy[r + 1][c + 1];
                    gx_gy_rgb[2][0] += pixel.rgbtBlue * gx[r + 1][c + 1];
                    gx_gy_rgb[2][1] += pixel.rgbtBlue * gy[r + 1][c + 1];

                    int red = round(sqrt(gx_gy_rgb[0][0] * gx_gy_rgb[0][0] + gx_gy_rgb[0][1] * gx_gy_rgb[0][1]));
                    int green = round(sqrt(gx_gy_rgb[1][0] * gx_gy_rgb[1][0] + gx_gy_rgb[1][1] * gx_gy_rgb[1][1]));
                    int blue = round(sqrt(gx_gy_rgb[2][0] * gx_gy_rgb[2][0] + gx_gy_rgb[2][1] * gx_gy_rgb[2][1]));

                    RGBTRIPLE result;
                    result.rgbtRed = (red > 255) ? 255 : red;
                    result.rgbtGreen = (green > 255) ? 255 : green;
                    result.rgbtBlue = (blue > 255) ? 255 : blue;

                    image[i][j] = result;
                }
            }
        }
    }
}