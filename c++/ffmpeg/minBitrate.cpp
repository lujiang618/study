
#include <stdint.h>
#include <math.h>
#include <iostream>

uint32_t EstimateMinBitRate(int width, int height, int fps, float level)
{
    double pixelsPerSecond = static_cast<double>(width) * height * fps;

    double bpp_factor = 0.10 * 1.0; // bits per pixel baseline

    // 计算码率（bps）
    double bitrate = pixelsPerSecond * bpp_factor;

    // 限制最大码率不超过Level允许值（近似ITU-T上限）
    double maxLevelBitrate = 0;
    if (level <= 4.0)
        maxLevelBitrate = 20000000;
    else if (level <= 4.1)
        maxLevelBitrate = 50000000;
    else if (level <= 4.2)
        maxLevelBitrate = 50000000;
    else if (level <= 5.0)
        maxLevelBitrate = 135000000;
    else if (level <= 5.1)
        maxLevelBitrate = 240000000;
    else if (level <= 5.2)
        maxLevelBitrate = 240000000;
    else if (level <= 6.0)
        maxLevelBitrate = 240000000;
    else if (level <= 6.1)
        maxLevelBitrate = 480000000;
    else
        maxLevelBitrate = 800000000;

    bitrate = std::min(bitrate, maxLevelBitrate);


    return static_cast<uint32_t>(bitrate);
}


int main()
{
    int width = 2560;
    int height = 1440;
    int fps = 30;
    float level = 4.1;

    uint32_t bitrate = EstimateMinBitRate(width, height, fps, level);

    printf("Estimated minimum bit rate: %u bps\n", bitrate);
}