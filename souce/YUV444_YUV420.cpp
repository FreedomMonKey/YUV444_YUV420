#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include<string.h>

typedef uint8_t        pel_t;

void yuv_sampUV(pel_t *src, pel_t *dst, int width, int height)
{
    int dst_wid = width;
    int dst_hei = height;
    pel_t *YU_src = src + width * height;
    pel_t *YU_dst = dst + dst_wid * dst_hei;
    int i, j, k = 0;
    for (i = 0; i < (height << 1); i += 2)
        for (j = 0; j < width; j += 2) {
            YU_dst[k] = (YU_src[i * dst_wid + j] + YU_src[i * dst_wid + j + 1] + YU_src[(i + 1) * dst_wid + j] + YU_src[(i + 1) * dst_wid + j + 1]) >> 2;
            k++;
        }
}


int main(int argc, char** argv)
{
    FILE *in_fp = fopen("b.yuv", "rb");
    FILE *out_fp = fopen("out.yuv", "wb");
    int width = 1920;
    int height = 1080;
    int frames = 1;

    int in_size = width * height * 3;
    int out_size = width * height * 3 / 2;

    uint8_t *in_image = (uint8_t *)malloc(in_size * sizeof(pel_t));
    uint8_t *out_image = (uint8_t *)malloc(out_size * sizeof(pel_t));

    uint8_t *Y = in_image;
    uint8_t *U = in_image + width * height;
    uint8_t *V = in_image + width * height * 5 / 4;

    int frame_cnt, r, c;
    for (frame_cnt = 0; frame_cnt < frames; frame_cnt++) {
        fread(in_image, sizeof(uint8_t), in_size, in_fp);

        memcpy(out_image, in_image, width * height * sizeof(pel_t));
        yuv_sampUV(in_image, out_image, width, height);

        fwrite(out_image, sizeof(uint8_t), out_size, out_fp);
    }

    printf("completed!!!\n");

    free(in_image);
    free(out_image);
    fclose(in_fp);
    fclose(out_fp);
}