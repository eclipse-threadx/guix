#include "palette_creater.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#else
#define STUDIOX_EVAL_BUILD
#endif

palette_creater::palette_creater()
{
    histogram = NULL;
    boxlist = NULL;
}


palette_creater::~palette_creater()
{
}

///////////////////////////////////////////////////////////////////////////////
// Note: 
// The color quantization code is implemented based on Heckbert's Median Cut 
// Algorithm that described in the following paper:
//
// Heckbert, Paul.  "Color Image Quantization for Frame Buffer Display",
// *Proc.SIGGRAPH '82, Computer Graphics v.16 #3 (July 1982), pp 297-304.
//
// The concept behind the median cut algorithm is to partition the color space
// into a three-dimensional rectangle box. Next separated the selected box into 
// two halves recursively untill we get a desired number of boxes.
///////////////////////////////////////////////////////////////////////////////
void palette_creater::CreatePaletteForOnePixelmap(res_info *resource, palette_info *info, int requested_size)
{
    CreatePaletteForPixelmaps(resource, info, FALSE, requested_size);
}


void palette_creater::CreatePaletteForPixelmaps(res_info *resource, palette_info *info, BOOL recursive, int requested_size)
{
    int boxes_num;

    ULONG hist_size = (1 << (HIST_RED_BITS + HIST_GREEN_BITS + HIST_BLUE_BITS));

    histogram = new ULONG[hist_size];
    memset(histogram, 0, hist_size * sizeof(UINT));

    if (requested_size == -1)
    {
        requested_size = 255;
    }

    boxlist = new BOX[requested_size];
    memset(boxlist, 0, requested_size * sizeof(BOX));

    // Accumulate the color histogram of all images under 
    // the specified pixelmap folder, which showing the
    // usage count of each possible color
    if (!recursive)
    {
        AccumulateOneMapHistogram(resource);
    }
    else
    {
        AccumulateSharedHistogram(resource);
    }

    // Initiate one box that containing whole space. 
    // Here we properly decrease the precision of the input color by
    // Remaining 5 bits for red, 6 bits for green and 5 bits for blue.
    // By doing this, we get a smaller histogram size that contains
    // 65536 colors.
    boxlist[0].rmin = 0;
    boxlist[0].rmax = (1 << HIST_RED_BITS) - 1;
    boxlist[0].gmin = 0;
    boxlist[0].gmax = (1 << HIST_GREEN_BITS) - 1;
    boxlist[0].bmin = 0;
    boxlist[0].bmax = (1 << HIST_BLUE_BITS) - 1;

    // Update some parametersi of the first box that containing whole space
    UpdateBox(&boxlist[0]);

    // Process median cut algorithm to produce the desired number of boxes
    boxes_num = median_cut(requested_size);

    info->palette = new GX_COLOR[requested_size];
    memset(info->palette, 0, requested_size * sizeof(GX_COLOR));
    info->total_size = requested_size;
    info->used_size = boxes_num;

    // Calculate the average color of each box which comes to one entry of
    // the optimized palette
    for (int index = 0; index < boxes_num; index++)
    {
        ComputeColor(info, &boxlist[index], index);
    }

    if (histogram)
    {
        delete histogram;
        histogram = NULL;
    }

    if (boxlist)
    {
        delete boxlist;
        boxlist = NULL;
    }
}

// Find the splittable BOX with the largest color population.
BOX *palette_creater::FindLargestColorPop(int numboxes)
{
    BOX *which = NULL;
    long maxc = 0;
    BOX *boxp = boxlist;

    for (int i = 0; i < numboxes; i++)
    {
        if ((boxp->color_count > maxc) && (boxp->volume > 0))
        {
            which = boxp;
            maxc = boxp->color_count;
        }

        boxp++;
    }

    return which;
}

// Find the splittable BOX with the largest color volume.
BOX *palette_creater::FindLargestColorVolume(int numboxes)
{
    BOX *which = NULL;
    long maxc = 0;
    BOX *boxp = boxlist;

    for (int i = 0; i < numboxes; i++)
    {
        if ((boxp->volume > maxc) && (boxp->color_count > 0))
        {
            which = boxp;
            maxc = boxp->volume;
        }

        boxp++;
    }

    return which;
}

void palette_creater::UpdateBox(BOX *boxp)
{
    // Shrink the min and max bounds of a BOX to include only nonzero pixels
    int rmin, rmax;
    int gmin, gmax;
    int bmin, bmax;

    rmin = (1 << HIST_RED_BITS) - 1;
    rmax = 0;
    gmin = (1 << HIST_GREEN_BITS) - 1;
    gmax = 0;
    bmin = (1 << HIST_BLUE_BITS) - 1;
    bmax = 0;

    int r, g, b;
    int color_count = 0;
    ULONG *histp;
    int pos;

    for (r = boxp->rmin; r <= boxp->rmax; r++)
    {
        for (g = boxp->gmin; g <= boxp->gmax; g++)
        {
            for (b = boxp->bmin; b <= boxp->bmax; b++)
            {
                pos = ASSEMBLECOLOR_16BPP(r, g, b);
                histp = &histogram[pos];

                if (*histp != 0)
                {
                    color_count++;

                    if (r < rmin)
                    {
                        rmin = r;
                    }

                    if (r > rmax)
                    {
                        rmax = r;
                    }

                    if (g < gmin)
                    {
                        gmin = g;
                    }

                    if (g > gmax)
                    {
                        gmax = g;
                    }

                    if (b < bmin)
                    {
                        bmin = b;
                    }

                    if (b > bmax)
                    {
                        bmax = b;
                    }
                }
            }
        }
    }

    boxp->color_count = color_count;
    boxp->rmin = rmin;
    boxp->rmax = rmax;
    boxp->gmin = gmin;
    boxp->gmax = gmax;
    boxp->bmin = bmin;
    boxp->bmax = bmax;

    int rdist = ((rmax - rmin) << HIST_RED_SHIFT) * RED_SCALE;
    int gdist = ((gmax - gmin) << HIST_GREEN_SHIFT) * GREEN_SCALE;
    int bdist = ((bmax - bmin) << HIST_BLUE_SHIFT) * BLUE_SCALE;

    boxp->volume = rdist * rdist + gdist * gdist + bdist * bdist;
}

// Accumulate color histogram of all pixelmaps under info that are
// using shared palette
void palette_creater::AccumulateSharedHistogram(res_info *info)
{
    while (info)
    {
        if (info->type == RES_TYPE_PIXELMAP)
        {
            if (info->palette_type == PALETTE_TYPE_SHARED)
            {
                AccumulateOneMapHistogram(info);
            }
        }
        if (info->child)
        {
            AccumulateSharedHistogram(info->child);
        }
        info = info->next;
    }
}

// Accumulate one map color histogram
void palette_creater::AccumulateOneMapHistogram(res_info *info)
{
    if (info->type != RES_TYPE_PIXELMAP)
    {
        return;
    }

    GX_COLOR *getdata;
    int width;
    int height;
    int red, green, blue;
    ULONG *histp;
    USHORT pos;
    res_info *pixinfo;
    
    pixinfo = new res_info(NULL, *info, FALSE);
    pixinfo->output_color_format = GX_COLOR_FORMAT_24XRGB;
    pixinfo->keep_alpha = FALSE;
    pixinfo->compress = FALSE;
    GetOpenProject()->InitializeOnePixelmap(pixinfo, NULL, GX_COLOR_FORMAT_24XRGB);

    GX_PIXELMAP *map;

    for (int index = 0; index < pixinfo->GetPixelmapFrameCount(); index++)
    {
        map = pixinfo->GetPixelmap(index);

        if (map && map->gx_pixelmap_data)
        {
            getdata = (GX_COLOR*)map->gx_pixelmap_data;
            width = map->gx_pixelmap_width;
            height = map->gx_pixelmap_height;

            for (int index = 0; index < width * height; index++)
            {
                red = (*getdata) >> 16;
                green = ((*getdata) & 0xff00) >> 8;
                blue = ((*getdata) & 0xff);

                red >>= HIST_RED_SHIFT;
                green >>= HIST_GREEN_SHIFT;
                blue >>= HIST_BLUE_SHIFT;

                pos = ASSEMBLECOLOR_16BPP(red, green, blue);
                histp = &histogram[pos];

                if (++(*histp) <= 0)
                {
                    (*histp)--;
                }

                getdata++;
            }
        }
    }

    delete pixinfo;
}

// Median cut color quantization algorithm that split one box that containing 
// the whole color space into desired number of boxes.
int palette_creater::median_cut(int desired_colors_num)
{
    int boxes_num = 1;
    BOX *b1, *b2;

    while (boxes_num < desired_colors_num)
    {
        if (boxes_num * 2 <= desired_colors_num)
        {
            // select the box with the largest color population
            b1 = FindLargestColorPop(boxes_num);
        }
        else
        {
            // select the box with the largets color volumn
            b1 = FindLargestColorVolume(boxes_num);
        }

        if (b1 == NULL)
        {
            break;
        }

        b2 = &boxlist[boxes_num];

        /* initiate new BOX. */
        (*b2) = (*b1);

        int rdist = ((b1->rmax - b1->rmin) << HIST_RED_SHIFT) * RED_SCALE;
        int gdist = ((b1->gmax - b1->gmin) << HIST_GREEN_SHIFT) * GREEN_SCALE;
        int bdist = ((b1->bmax - b1->bmin) << HIST_BLUE_SHIFT) * BLUE_SCALE;

        // split the selected box along the longges axis
        if ((rdist >= gdist) && (rdist >= bdist))
        {
            //split BOX along red axis
            b1->rmax = (b1->rmax + b1->rmin) / 2;
            b2->rmin = (b1->rmax + 1);
        }
        else if ((gdist >= rdist) && (gdist >= bdist))
        {
            //split BOX along green axis
            b1->gmax = (b1->gmax + b1->gmin) / 2;
            b2->gmin = (b1->gmax + 1);
        }
        else
        {
            //split BOX along blue axis
            b1->bmax = (b1->bmax + b1->bmin) / 2;
            b2->bmin = (b1->bmax + 1);
        }

        UpdateBox(b1);
        UpdateBox(b2);

        boxes_num++;
    }

    return boxes_num;
}

// Calcualte the average color value of one box
void palette_creater::ComputeColor(palette_info *control, BOX *boxp, int index)
{
    int r, g, b;
    int count;
    ULONG *histp;
    ULONG total = 0;
    ULONG totalred = 0;
    ULONG totalgreen = 0;
    ULONG totalblue = 0;

    for (r = boxp->rmin; r <= boxp->rmax; r++)
    {
        for (g = boxp->gmin; g <= boxp->gmax; g++)
        {
            for (b = boxp->bmin; b <= boxp->bmax; b++)
            {
                histp = &histogram[ASSEMBLECOLOR_16BPP(r, g, b)];
                count = *histp;

                if (count)
                {
                    total += count;

                    totalred += (r << HIST_RED_SHIFT) * count;
                    totalgreen += (g << HIST_GREEN_SHIFT) * count;
                    totalblue += (b << HIST_BLUE_SHIFT) * count;
                }
            }
        }
    }

    if (total)
    {
        r = totalred / total;
        g = totalgreen / total;
        b = totalblue / total;

        control->palette[index] = 0xff000000 | ASSEMBLECOLOR_24BPP(r, g, b);
    }
}