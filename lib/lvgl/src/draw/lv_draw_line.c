/**
 * @file lv_draw_line.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdbool.h>
#include "../core/lv_refr.h"
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

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

LV_ATTRIBUTE_FAST_MEM void lv_draw_line_dsc_init(lv_draw_line_dsc_t * dsc)
{
    lv_memset_00(dsc, sizeof(lv_draw_line_dsc_t));
    dsc->width = 1;
    dsc->opa = LV_OPA_COVER;
    dsc->color = lv_color_black();
}

LV_ATTRIBUTE_FAST_MEM void lv_draw_line(const lv_point_t * point1, const lv_point_t * point2, const lv_area_t * clip,
                                        const lv_draw_line_dsc_t * dsc)
{
    if(dsc->width == 0) return;
    if(dsc->opa <= LV_OPA_MIN) return;

    const lv_draw_backend_t * backend = lv_draw_backend_get();
    backend->draw_line(point1, point2, clip, dsc);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
