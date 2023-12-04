#pragma once
#include "studiox_includes.h"

#define HIST_RED_BITS 5
#define HIST_GREEN_BITS 6
#define HIST_BLUE_BITS 5

#define HIST_RED_SHIFT   3
#define HIST_GREEN_SHIFT 2
#define HIST_BLUE_SHIFT  3

#define RED_SCALE 2    // scale red distance by this much
#define GREEN_SCALE 3  // scale green distance by this much
#define BLUE_SCALE 1   

typedef struct{
    int  rmin;
    int  rmax;
    int  gmin;
    int  gmax;
    int  bmin;
    int  bmax;
    int  volume;//color volume
    long color_count;//color population
}BOX;


class palette_creater
{
public:
    palette_creater();
    ~palette_creater();

public:
    void CreatePaletteForOnePixelmap(res_info *resource, palette_info *info, int count = -1);
    void CreatePaletteForPixelmaps(res_info *resource, palette_info *info, BOOL recursive = TRUE, int count = -1);

protected:
    BOX *FindLargestColorPop(int numboxes);
    BOX *FindLargestColorVolume(int numboxes);
    void UpdateBox(BOX *boxp);
    void AccumulateSharedHistogram(res_info *info);
    void AccumulateOneMapHistogram(res_info *info);
    int  median_cut(int desired_colors_num);
    void ComputeColor(palette_info *info, BOX *boxp, int index);

private:
    ULONG *histogram;
    BOX   *boxlist;
};

