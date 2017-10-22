/**************************************************************************
 *              Copyright (C), AirM2M Tech. Co., Ltd.
 *
 * Name:    platform_audio.c
 * Author:  lijiaodi
 * Version: V0.1
 * Date:    2017/06/29
 *
 * Description:
 *          Í¼Æ¬½âÂë½Ó¿Ú
 **************************************************************************/

#include "rda_pal.h"
#include "string.h"

#include "platform.h"
#include "platform_malloc.h"
#include "platform_image.h"




int platform_image_decode(const char *filename,image_t *image_info)
{
    int ret;
    T_AMOPENAT_IMAGE_INFO opat_imageinfo;
    int length;
    UINT16 *unicode_path;
    char* actname;
    
        
    if((find_dm_entry( filename, &actname ) ) == -1 )
    {
        return;
    }
    
    length = strlen(actname);

    IVTBL(print)("platform_image_decode %s", actname);
    unicode_path = IVTBL(malloc)((length+1)*sizeof(WCHAR));
    strtows(unicode_path, actname);

    ret = IVTBL(imgs_decode)(unicode_path, &opat_imageinfo);
    IVTBL(free)(unicode_path);


    if(ret == 0)
    {
        image_info->width = opat_imageinfo.width;
        image_info->height= opat_imageinfo.height;
        image_info->format= opat_imageinfo.format;
        image_info->buffer= opat_imageinfo.buffer;
        IVTBL(print)("platform_image_decode %d %d %d ", image_info->width, image_info->height, image_info->format);
    }

    
    return ret;
}

int platform_free_image_decodedata(u16 *buf)
{
    int ret;

    ret = IVTBL(imgs_free_decodedata)(buf);

    IVTBL(print)("platform_free_image_decodedata ret:%d\n",ret);
    return ret;
}

