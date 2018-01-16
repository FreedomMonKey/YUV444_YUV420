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
    pel_t cur, right, down, right_down;
    int i, j, k = 0;
    for (i = 0; i < (height << 1); i += 2)
        for (j = 0; j < width; j += 2) {
            cur = YU_src[i * dst_wid + j];
            right = YU_src[i * dst_wid + j + 1];
            down = YU_src[(i + 1) * dst_wid + j];
            right_down = YU_src[(i + 1) * dst_wid + j + 1];
            YU_dst[k] = (cur + right + down + right_down + 2) >> 2;
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

    pel_t *in_image = (pel_t *)malloc(in_size * sizeof(pel_t));
    pel_t *out_image = (pel_t *)malloc(out_size * sizeof(pel_t));

    int frame_cnt;
    for (frame_cnt = 0; frame_cnt < frames; frame_cnt++) {
        fread(in_image, sizeof(pel_t), in_size, in_fp);

        memcpy(out_image, in_image, width * height * sizeof(pel_t));
        yuv_sampUV(in_image, out_image, width, height);

        fwrite(out_image, sizeof(pel_t), out_size, out_fp);
    }

    printf("completed!!!\n");

    free(in_image);
    free(out_image);
    fclose(in_fp);
    fclose(out_fp);
}