/**
 * @file lv_draw_img.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "lv_draw_img.h"
#include "lv_img_cache.h"
#include "../hal/lv_hal_disp.h"
#include "../misc/lv_log.h"
#include "../core/lv_refr.h"
#include "../misc/lv_mem.h"
#include "../misc/lv_math.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
LV_ATTRIBUTE_FAST_MEM static lv_res_t lv_img_draw_core(const lv_area_t * coords, const lv_area_t * clip_area,
                                                       const void * src,
                                                       const lv_draw_img_dsc_t * draw_dsc);

static void show_error(const lv_area_t * coords, const lv_area_t * clip_area, const char * msg);
static void draw_cleanup(_lv_img_cache_entry_t * cache);

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void lv_draw_img_dsc_init(lv_draw_img_dsc_t * dsc)
{
    lv_memset_00(dsc, sizeof(lv_draw_img_dsc_t));
    dsc->recolor = lv_color_black();
    dsc->opa = LV_OPA_COVER;
    dsc->zoom = LV_IMG_ZOOM_NONE;
    dsc->antialias = LV_COLOR_DEPTH > 8 ? 1 : 0;
}

/**
 * Draw an image
 * @param coords the coordinates of the image
 * @param mask the image will be drawn only in this area
 * @param src pointer to a lv_color_t array which contains the pixels of the image
 * @param dsc pointer to an initialized `lv_draw_img_dsc_t` variable
 */
void lv_draw_img(const lv_area_t * coords, const lv_area_t * mask, const void * src, const lv_draw_img_dsc_t * dsc)
{
    if(src == NULL) {
        LV_LOG_WARN("Image draw: src is NULL");
        show_error(coords, mask, "No\ndata");
        return;
    }

    if(dsc->opa <= LV_OPA_MIN) return;

    const lv_draw_backend_t * backend = lv_draw_backend_get();

    lv_res_t res;
    if(backend->draw_img_core) {
        res = backend->draw_img_core(coords, mask, src, dsc);
    }
    else {
        res = lv_img_draw_core(coords, mask, src, dsc);
    }

    if(res == LV_RES_INV) {
        LV_LOG_WARN("Image draw error");
        show_error(coords, mask, "No\ndata");
        return;
    }
}

/**
 * Get the pixel size of a color format in bits
 * @param cf a color format (`LV_IMG_CF_...`)
 * @return the pixel size in bits
 */
uint8_t lv_img_cf_get_px_size(lv_img_cf_t cf)
{
    uint8_t px_size = 0;

    switch(cf) {
        case LV_IMG_CF_UNKNOWN:
        case LV_IMG_CF_RAW:
            px_size = 0;
            break;
        case LV_IMG_CF_TRUE_COLOR:
        case LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED:
            px_size = LV_COLOR_SIZE;
            break;
        case LV_IMG_CF_TRUE_COLOR_ALPHA:
            px_size = LV_IMG_PX_SIZE_ALPHA_BYTE << 3;
            break;
        case LV_IMG_CF_INDEXED_1BIT:
        case LV_IMG_CF_ALPHA_1BIT:
            px_size = 1;
            break;
        case LV_IMG_CF_INDEXED_2BIT:
        case LV_IMG_CF_ALPHA_2BIT:
            px_size = 2;
            break;
        case LV_IMG_CF_INDEXED_4BIT:
        case LV_IMG_CF_ALPHA_4BIT:
            px_size = 4;
            break;
        case LV_IMG_CF_INDEXED_8BIT:
        case LV_IMG_CF_ALPHA_8BIT:
            px_size = 8;
            break;
        default:
            px_size = 0;
            break;
    }

    return px_size;
}

/**
 * Check if a color format is chroma keyed or not
 * @param cf a color format (`LV_IMG_CF_...`)
 * @return true: chroma keyed; false: not chroma keyed
 */
bool lv_img_cf_is_chroma_keyed(lv_img_cf_t cf)
{
    bool is_chroma_keyed = false;

    switch(cf) {
        case LV_IMG_CF_TRUE_COLOR_CHROMA_KEYED:
        case LV_IMG_CF_RAW_CHROMA_KEYED:
        case LV_IMG_CF_INDEXED_1BIT:
        case LV_IMG_CF_INDEXED_2BIT:
        case LV_IMG_CF_INDEXED_4BIT:
        case LV_IMG_CF_INDEXED_8BIT:
            is_chroma_keyed = true;
            break;

        default:
            is_chroma_keyed = false;
            break;
    }

    return is_chroma_keyed;
}

/**
 * Check if a color format has alpha channel or not
 * @param cf a color format (`LV_IMG_CF_...`)
 * @return true: has alpha channel; false: doesn't have alpha channel
 */
bool lv_img_cf_has_alpha(lv_img_cf_t cf)
{
    bool has_alpha = false;

    switch(cf) {
        case LV_IMG_CF_TRUE_COLOR_ALPHA:
        case LV_IMG_CF_RAW_ALPHA:
        case LV_IMG_CF_INDEXED_1BIT:
        case LV_IMG_CF_INDEXED_2BIT:
        case LV_IMG_CF_INDEXED_4BIT:
        case LV_IMG_CF_INDEXED_8BIT:
        case LV_IMG_CF_ALPHA_1BIT:
        case LV_IMG_CF_ALPHA_2BIT:
        case LV_IMG_CF_ALPHA_4BIT:
        case LV_IMG_CF_ALPHA_8BIT:
            has_alpha = true;
            break;
        default:
            has_alpha = false;
            break;
    }

    return has_alpha;
}

/**
 * Get the type of an image source
 * @param src pointer to an image source:
 *  - pointer to an 'lv_img_t' variable (image stored internally and compiled into the code)
 *  - a path to a file (e.g. "S:/folder/image.bin")
 *  - or a symbol (e.g. LV_SYMBOL_CLOSE)
 * @return type of the image source LV_IMG_SRC_VARIABLE/FILE/SYMBOL/UNKNOWN
 */
lv_img_src_t lv_img_src_get_type(const void * src)
{
    lv_img_src_t img_src_type = LV_IMG_SRC_UNKNOWN;

    if(src == NULL) return img_src_type;
    const uint8_t * u8_p = src;

    /*The first byte shows the type of the image source*/
    if(u8_p[0] >= 0x20 && u8_p[0] <= 0x7F) {
        img_src_type = LV_IMG_SRC_FILE; /*If it's an ASCII character then it's file name*/
    }
    else if(u8_p[0] >= 0x80) {
        img_src_type = LV_IMG_SRC_SYMBOL; /*Symbols begins after 0x7F*/
    }
    else {
        img_src_type = LV_IMG_SRC_VARIABLE; /*`lv_img_dsc_t` is draw to the first byte < 0x20*/
    }

    if(LV_IMG_SRC_UNKNOWN == img_src_type) {
        LV_LOG_WARN("lv_img_src_get_type: unknown image type");
    }

    return img_src_type;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

LV_ATTRIBUTE_FAST_MEM static lv_res_t lv_img_draw_core(const lv_area_t * coords, const lv_area_t * clip_area,
                                                       const void * src,
                                                       const lv_draw_img_dsc_t * draw_dsc)
{
    if(draw_dsc->opa <= LV_OPA_MIN) return LV_RES_OK;

    _lv_img_cache_entry_t * cdsc = _lv_img_cache_open(src, draw_dsc->recolor, draw_dsc->frame_id);

    if(cdsc == NULL) return LV_RES_INV;


    const lv_draw_backend_t * backend = lv_draw_backend_get();
    bool chroma_keyed = lv_img_cf_is_chroma_keyed(cdsc->dec_dsc.header.cf);
    bool alpha_byte   = lv_img_cf_has_alpha(cdsc->dec_dsc.header.cf);

    if(cdsc->dec_dsc.error_msg != NULL) {
        LV_LOG_WARN("Image draw error");

        show_error(coords, clip_area, cdsc->dec_dsc.error_msg);
    }
    /*The decoder could open the image and gave the entire uncompressed image.
     *Just draw it!*/
    else if(cdsc->dec_dsc.img_data) {
        lv_area_t map_area_rot;
        lv_area_copy(&map_area_rot, coords);
        if(draw_dsc->angle || draw_dsc->zoom != LV_IMG_ZOOM_NONE) {
            int32_t w = lv_area_get_width(coords);
            int32_t h = lv_area_get_height(coords);

            _lv_img_buf_get_transformed_area(&map_area_rot, w, h, draw_dsc->angle, draw_dsc->zoom, &draw_dsc->pivot);

            map_area_rot.x1 += coords->x1;
            map_area_rot.y1 += coords->y1;
            map_area_rot.x2 += coords->x1;
            map_area_rot.y2 += coords->y1;
        }

        lv_area_t mask_com; /*Common area of mask and coords*/
        bool union_ok;
        union_ok = _lv_area_intersect(&mask_com, clip_area, &map_area_rot);
        /*Out of mask. There is nothing to draw so the image is drawn successfully.*/
        if(union_ok == false) {
            draw_cleanup(cdsc);
            return LV_RES_OK;
        }

        backend->draw_img(coords, &mask_com, cdsc->dec_dsc.img_data, draw_dsc, chroma_keyed, alpha_byte);
    }
    /*The whole uncompressed image is not available. Try to read it line-by-line*/
    else {
        lv_area_t mask_com; /*Common area of mask and coords*/
        bool union_ok;
        union_ok = _lv_area_intersect(&mask_com, clip_area, coords);
        /*Out of mask. There is nothing to draw so the image is drawn successfully.*/
        if(union_ok == false) {
            draw_cleanup(cdsc);
            return LV_RES_OK;
        }

        int32_t width = lv_area_get_width(&mask_com);

        uint8_t  * buf = lv_mem_buf_get(lv_area_get_width(&mask_com) *
                                        LV_IMG_PX_SIZE_ALPHA_BYTE);  /*+1 because of the possible alpha byte*/

        lv_area_t line;
        lv_area_copy(&line, &mask_com);
        lv_area_set_height(&line, 1);
        int32_t x = mask_com.x1 - coords->x1;
        int32_t y = mask_com.y1 - coords->y1;
        int32_t row;
        lv_res_t read_res;
        for(row = mask_com.y1; row <= mask_com.y2; row++) {
            lv_area_t mask_line;
            union_ok = _lv_area_intersect(&mask_line, clip_area, &line);
            if(union_ok == false) continue;

            read_res = lv_img_decoder_read_line(&cdsc->dec_dsc, x, y, width, buf);
            if(read_res != LV_RES_OK) {
                lv_img_decoder_close(&cdsc->dec_dsc);
                LV_LOG_WARN("Image draw can't read the line");
                lv_mem_buf_release(buf);
                draw_cleanup(cdsc);
                return LV_RES_INV;
            }

            backend->draw_img(&line, &mask_line, buf, draw_dsc, chroma_keyed, alpha_byte);
            line.y1++;
            line.y2++;
            y++;
        }
        lv_mem_buf_release(buf);
    }

    draw_cleanup(cdsc);
    return LV_RES_OK;
}


static void show_error(const lv_area_t * coords, const lv_area_t * clip_area, const char * msg)
{
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = lv_color_white();
    lv_draw_rect(coords, clip_area, &rect_dsc);

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    lv_draw_label(coords, clip_area, &label_dsc, msg, NULL);
}

static void draw_cleanup(_lv_img_cache_entry_t * cache)
{
    /*Automatically close images with no caching*/
#if LV_IMG_CACHE_DEF_SIZE == 0
    lv_img_decoder_close(&cache->dec_dsc);
#else
    LV_UNUSED(cache);
#endif
}
