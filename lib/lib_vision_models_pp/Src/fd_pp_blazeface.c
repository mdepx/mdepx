/*---------------------------------------------------------------------------------------------
 * Copyright (c) 2025 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file in
 * the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *--------------------------------------------------------------------------------------------*/

// #include "mpe_pp_loc.h"
#include "fd_blazeface_pp_if.h"
#include "vision_models_pp.h"


/* Offsets to access face detect blaze face input data */
#define AI_FD_BLAZEFACE_PP_XCENTER      (0)
#define AI_FD_BLAZEFACE_PP_YCENTER      (1)
#define AI_FD_BLAZEFACE_PP_WIDTHREL     (2)
#define AI_FD_BLAZEFACE_PP_HEIGHTREL    (3)
#define AI_FD_BLAZEFACE_PP_KEYPOINTS    (4)

/* Can't be removed if qsort is not re-written... */
static int32_t AI_FD_PP_SORT_CLASS;


static int32_t fd_nms_comparator(const void *pa, const void *pb)
{
    fd_pp_outBuffer_t a = *(fd_pp_outBuffer_t *)pa;
    fd_pp_outBuffer_t b = *(fd_pp_outBuffer_t *)pb;

    float32_t diff = 0.0;
    float32_t a_weighted_conf = 0.0;
    float32_t b_weighted_conf = 0.0;

    if (a.class_index == AI_FD_PP_SORT_CLASS)
    {
        a_weighted_conf = a.conf;
    }
    else
    {
         a_weighted_conf = 0.0;
    }

    if (b.class_index == AI_FD_PP_SORT_CLASS)
    {
        b_weighted_conf = b.conf;
    }
    else
    {
         b_weighted_conf = 0.0;
    }

    diff = a_weighted_conf - b_weighted_conf;

    if (diff < 0) return 1;
    else if (diff > 0) return -1;
    return 0;
}

static int32_t fd_pp_nmsFiltering_centroid(fd_pp_out_t *pOutput,
                                           fd_blazeface_pp_static_param_t *pInput_static_param)
{
    int32_t j, k, limit_counter, detections_per_class;
    for (k = 0; k < pInput_static_param->nb_classes; ++k)
    {
        limit_counter = 0;
        detections_per_class = 0;
        AI_FD_PP_SORT_CLASS = k;


        /* Counts the number of detections with class k */
        for (int32_t i = 0; i < pInput_static_param->nb_detect ; i ++)
        {
            if(pOutput->pOutBuff[i].class_index == k)
            {
                detections_per_class++;
            }
        }

        if (detections_per_class > 0)
        {
            /* Sorts detections based on class k */
            qsort(pOutput->pOutBuff,
                  pInput_static_param->nb_detect,
                  sizeof(fd_pp_outBuffer_t),
                  fd_nms_comparator);

            for (int32_t i = 0; i < detections_per_class ; i ++)
            {
                if (pOutput->pOutBuff[i].conf == 0) continue;
                float32_t *a = &(pOutput->pOutBuff[i].x_center);
                for (j = i + 1; j < detections_per_class; j ++)
                {
                    if (pOutput->pOutBuff[j].conf == 0) continue;
                    float32_t *b = &(pOutput->pOutBuff[j].x_center);
                    if (vision_models_box_iou(a, b) > pInput_static_param->iou_threshold)
                    {
                        pOutput->pOutBuff[j].conf = 0;
                    }
                }
            }

            /* Limits detections count */
            for (int32_t i = 0; i < detections_per_class; i++)
            {
                if ((limit_counter < pInput_static_param->max_boxes_limit) &&
                    (pOutput->pOutBuff[i].conf != 0))
                {
                    limit_counter++;
                }
                else
                {
                    pOutput->pOutBuff[i].conf = 0;
                }
            }
        }
    }
    return (AI_FD_PP_ERROR_NO);
}


static int32_t fd_pp_scoreFiltering_centroid(fd_pp_out_t *pOutput,
                                             fd_blazeface_pp_static_param_t *pInput_static_param)
{
    int32_t det_count = 0;

    for (int32_t i = 0; i < pInput_static_param->nb_detect; i++)
    {
        if (pOutput->pOutBuff[i].conf >= pInput_static_param->conf_threshold)
        {
            pOutput->pOutBuff[det_count].x_center    = pOutput->pOutBuff[i].x_center;
            pOutput->pOutBuff[det_count].y_center    = pOutput->pOutBuff[i].y_center;
            pOutput->pOutBuff[det_count].width       = pOutput->pOutBuff[i].width;
            pOutput->pOutBuff[det_count].height      = pOutput->pOutBuff[i].height;
            pOutput->pOutBuff[det_count].conf        = pOutput->pOutBuff[i].conf;
            pOutput->pOutBuff[det_count].class_index = pOutput->pOutBuff[i].class_index;
            for (int32_t j = 0; j < pInput_static_param->nb_keypoints; j++)
            {
                pOutput->pOutBuff[det_count].pKeyPoints[j].x    = pOutput->pOutBuff[i].pKeyPoints[j].x;
                pOutput->pOutBuff[det_count].pKeyPoints[j].y    = pOutput->pOutBuff[i].pKeyPoints[j].y;
            }
            det_count++;
        }
    }
    pOutput->nb_detect = det_count;

    return (AI_FD_PP_ERROR_NO);
}

static int32_t fd_pp_level_decode_and_store_is8(int8_t *pRawBoxes,
                                         int8_t *pProbas,
                                         fd_pp_out_t *pOutput,
                                         const float32_t *pAnchors,
                                         uint32_t inDetection,
                                         fd_blazeface_pp_static_param_t *pInput_static_param,
                                         float32_t raw_scale,
                                         int8_t raw_zp,
                                         float32_t proba_scale,
                                         int8_t proba_zp)

{
  float32_t inv_size = 1.0f / pInput_static_param->in_size;
  int32_t boxe_stride = pInput_static_param->nb_keypoints * 2 + AI_FD_BLAZEFACE_PP_KEYPOINTS;


  int32_t det_count = pInput_static_param->nb_detect;
  fd_pp_outBuffer_t *pOutBuff = (fd_pp_outBuffer_t *)pOutput->pOutBuff;

  if ( 1 == pInput_static_param->nb_classes) {

    float32_t computedThreshold = -logf( 1 / pInput_static_param->conf_threshold - 1);
    int8_t threshold_s8  = (int8_t)(computedThreshold / proba_scale + proba_zp);

    for (int32_t det = 0; det < (int32_t)inDetection; ++det)
    {
      if ( *pProbas > threshold_s8) {
        /* read and activate objectness */
        float32_t dequant;
        dequant = (float32_t)((int32_t)*pProbas - proba_zp) * proba_scale;
        float32_t prob = vision_models_sigmoid_f(dequant);
        pOutBuff[det_count].conf = prob;
        pOutBuff[det_count].class_index = 0;

        dequant = (float32_t)((int32_t)pRawBoxes[AI_FD_BLAZEFACE_PP_XCENTER] - raw_zp) * raw_scale;
        pOutBuff[det_count].x_center   = dequant * inv_size + pAnchors[0];

        dequant = (float32_t)((int32_t)pRawBoxes[AI_FD_BLAZEFACE_PP_YCENTER] - raw_zp) * raw_scale;
        pOutBuff[det_count].y_center   = dequant * inv_size + pAnchors[1];

        dequant = (float32_t)((int32_t)pRawBoxes[AI_FD_BLAZEFACE_PP_WIDTHREL] - raw_zp) * raw_scale;
        pOutBuff[det_count].width      = dequant * inv_size;

        dequant = (float32_t)((int32_t)pRawBoxes[AI_FD_BLAZEFACE_PP_HEIGHTREL] - raw_zp) * raw_scale;
        pOutBuff[det_count].height     = dequant * inv_size;

        for (uint32_t j = 0; j <  pInput_static_param->nb_keypoints; j++)
        {
            dequant = (float32_t)((int32_t)pRawBoxes[AI_FD_BLAZEFACE_PP_KEYPOINTS + 2*j + 0] - raw_zp) * raw_scale;
            pOutBuff[det_count].pKeyPoints[j].x = dequant * inv_size + pAnchors[0];
            dequant = (float32_t)((int32_t)pRawBoxes[AI_FD_BLAZEFACE_PP_KEYPOINTS + 2*j + 1] - raw_zp) * raw_scale;
            pOutBuff[det_count].pKeyPoints[j].y = dequant * inv_size + pAnchors[1];

        } // for anch

        det_count++;
      } // if threshold
      pProbas++;
      pRawBoxes+=boxe_stride;
      pAnchors+=2;

    } // for det
  } // if nb_classes == 1
  pInput_static_param->nb_detect = det_count;

  return det_count;

}

static int32_t fd_pp_getNNBoxes_centroid_is8(fd_blazeface_pp_in_t *pInput,
                                             fd_pp_out_t *pOut,
                                             fd_blazeface_pp_static_param_t *pInput_static_param)
{
  int32_t error   = AI_FD_PP_ERROR_NO;

  if (pInput_static_param->nb_classes != 1)
  {
    error = AI_FD_PP_ERROR;
  }
  if (pOut->pOutBuff == NULL)
  {
    error   = AI_FD_PP_ERROR;
  }
  pInput_static_param->nb_detect = 0;

  if (error == AI_FD_PP_ERROR_NO)
  {
    int8_t *pRawBoxes, *pProbas;
    float32_t *pAnchors;
    uint32_t inDetections;
    float32_t boxeScale, probaScale;
    int8_t boxeZp, probaZp;

    //out 0
    boxeScale    = pInput_static_param->boxe_0_scale;
    boxeZp       = pInput_static_param->boxe_0_zero_point;
    probaScale   = pInput_static_param->proba_0_scale;
    probaZp      = pInput_static_param->proba_0_zero_point;
    inDetections = pInput_static_param->nb_detections_0;
    pRawBoxes    = (int8_t *)pInput->pRawDetections_0;
    pProbas      = (int8_t *)pInput->pScores_0;
    pAnchors     = (float32_t *)pInput_static_param->pAnchors_0;

    fd_pp_level_decode_and_store_is8(pRawBoxes, pProbas, pOut, pAnchors, inDetections, pInput_static_param, boxeScale, boxeZp, probaScale, probaZp);

    //out 1
    boxeScale    = pInput_static_param->boxe_1_scale;
    boxeZp       = pInput_static_param->boxe_1_zero_point;
    probaScale   = pInput_static_param->proba_1_scale;
    probaZp      = pInput_static_param->proba_1_zero_point;
    inDetections = pInput_static_param->nb_detections_1;
    pRawBoxes    = (int8_t *)pInput->pRawDetections_1;
    pProbas      = (int8_t *)pInput->pScores_1;
    pAnchors     = (float32_t *)pInput_static_param->pAnchors_1;

    fd_pp_level_decode_and_store_is8(pRawBoxes, pProbas, pOut, pAnchors, inDetections, pInput_static_param, boxeScale, boxeZp, probaScale, probaZp);

  }
  return (error);
}



/* ----------------------       Exported routines      ---------------------- */

int32_t fd_blazeface_pp_reset(fd_blazeface_pp_static_param_t *pInput_static_param)
{
  return (AI_FD_PP_ERROR_NO);
}

int32_t fd_blazeface_pp_process_int8(fd_blazeface_pp_in_t *pInput,
                              fd_pp_out_t *pOutput,
                              fd_blazeface_pp_static_param_t *pInput_static_param)
{
  int32_t error   = AI_FD_PP_ERROR_NO;

  /* Call Get NN boxes first */
  error = fd_pp_getNNBoxes_centroid_is8(pInput,
                                        pOutput,
                                        pInput_static_param);

  if (error != AI_FD_PP_ERROR_NO) return (error);

  /* Then NMS */
  error = fd_pp_nmsFiltering_centroid(pOutput,
                                      pInput_static_param);

  if (error != AI_FD_PP_ERROR_NO) return (error);

  /* And score re-filtering */
  error = fd_pp_scoreFiltering_centroid(pOutput,
                                        pInput_static_param);

  return (error);
}
