#include <stdio.h>
#include "curl/curl.h"
#include "cJSON.h"
#include <math.h>

#define JSON_TXT    "jSon.txt"
#define BUFSIZE     1024*4
#define APIURL
#define APIKEY
#define APISECRET

int parseJson(char *jSonText)
{
    cJSON *cJson = cJSON_Parse(jSonText);
    cJSON *c1=NULL,*c2=NULL,*c3=NULL,*c4=NULL,*c5=NULL,*c6=NULL;
    if(!cJson){
        printf("cJson_parse error!\n");
        return -1;
    }
    c1 = cJSON_GetObjectItem(cJson,"img_id");
    if(!c1)
        printf("get img_id error!\n");
    else if(c1->type == cJSON_String)
        printf("img_id : %s\n",c1->valuestring);

    c1 = cJSON_GetObjectItem(cJson,"img_height");
    if(!c1)
        printf("get img_height error!\n");
    else if(c1->type == cJSON_Number)
        printf("img_height : %d\n",c1->valueint);

    c1 = cJSON_GetObjectItem(cJson,"img_width");
    if(!c1)
        printf("get img_weight error!\n");
    else if(c1->type == cJSON_Number)
        printf("img_width : %d\n",c1->valueint);

    c1 = cJSON_GetObjectItem(cJson,"session_id");
    if(!c1)
        printf("get session_id error!\n");
    else if(c1->type == cJSON_String)
        printf("session_id %s\n",c1->valuestring);

    c1 = cJSON_GetObjectItem(cJson,"face");
    if(!c1)
        printf("get face error!\n");
    else if(c1->type == cJSON_Array)
    {
        int c1_size = cJSON_GetArraySize(c1);
        int i = 0;
     //   printf("%d\n",c1_size);
        for(i=0;i<c1_size;i++)
        {
            c2 = cJSON_GetArrayItem(c1,i);
            if(!c2)
            {
                printf("get c1 item error!\n");
                break;
            }
            c3 = cJSON_GetObjectItem(c2,"attribute");
            if(!c3)
                printf("get attribute error!\n");
            else if(c3->type == cJSON_Object)
            {
                c4 = cJSON_GetObjectItem(c3,"age");
                if(!c4)
                    printf("get age error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"range");
                    if(!c5)
                        printf("get age range error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("age range : %d\n",c5->valueint);

                    c5 = cJSON_GetObjectItem(c4,"value");
                    if(!c5)
                        printf("get age value error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("age value : %d\n",c5->valueint);
                }

                c4 = cJSON_GetObjectItem(c3,"gender");
                if(!c4)
                    printf("get gender error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"confidence");
                    if(!c5)
                        printf("get gender confidence error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("gender confidence : %f\n",c5->valuedouble);

                    c5 = cJSON_GetObjectItem(c4,"value");
                    if(!c5)
                        printf("get gender value error!\n");
                    else if(c5->type == cJSON_String)
                        printf("gender value : %s\n",c5->valuestring);
                }

                c4 = cJSON_GetObjectItem(c3,"glass");
                if(!c4)
                    printf("get glass error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"confidence");
                    if(!c5)
                        printf("get glass confidence error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("glass confidence : %f\n",c5->valuedouble);

                    c5 = cJSON_GetObjectItem(c4,"value");
                    if(!c5)
                        printf("get gender value error!\n");
                    else if(c5->type == cJSON_String)
                        printf("gender value : %s\n",c5->valuestring);
                }

                c4 = cJSON_GetObjectItem(c3,"pose");
                if(!c4)
                    printf("get pose error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"pitch_angle");
                    if(!c5)
                        printf("get pose pitch_angle error!\n");
                    else if(c5->type == cJSON_Object)
                    {
                        c6 = cJSON_GetObjectItem(c5,"value");
                        if(!c6)
                            printf("get pose pitch_angle value error!\n");
                        else if(c6->type == cJSON_Number)
                            printf("pose pitch_angle : %f\n",c6->valuedouble);
                    }

                    c5 = cJSON_GetObjectItem(c4,"roll_angle");
                    if(!c5)
                        printf("get pose roll_angle error!\n");
                    else if(c5->type == cJSON_Object)
                    {
                        c6 = cJSON_GetObjectItem(c5,"value");
                        if(!c6)
                            printf("get pose roll_angle value error!\n");
                        else if(c6->type == cJSON_Number)
                            printf("pose roll_angle : %f\n",c6->valuedouble);
                    }

                    c5 = cJSON_GetObjectItem(c4,"yaw_angle");
                    if(!c5)
                        printf("get pose yaw_angle error!\n");
                    else if(c5->type == cJSON_Object)
                    {
                        c6 = cJSON_GetObjectItem(c5,"value");
                        if(!c6)
                            printf("get pose roll_angle value error!\n");
                        else if(c6->type == cJSON_Number)
                            printf("pose roll_angle : %f\n",c6->valuedouble);
                    }
                }

                c4 = cJSON_GetObjectItem(c3,"race");
                if(!c4)
                    printf("get race error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"confidence");
                    if(!c5)
                        printf("get race confidence error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("race confidence : %f\n",c5->valuedouble);

                    c5 = cJSON_GetObjectItem(c4,"value");
                    if(!c5)
                        printf("get race value error!\n");
                    else if(c5->type == cJSON_String)
                        printf("race value : %s\n",c5->valuestring);
                }

                c4 = cJSON_GetObjectItem(c3,"smiling");
                if(!c4)
                    printf("get smiling error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"value");
                    if(!c5)
                        printf("get smiling value error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("smiling value : %f\n",c5->valuedouble);
                }
            }
            c3 = cJSON_GetObjectItem(c2,"face_id");
            if(!c3)
                printf("get face_id error!\n");
            else if(c3->type == cJSON_String)
                printf("face_id : %s\n",c3->valuestring);

            c3 = cJSON_GetObjectItem(c2,"position");
            if(!c3)
                printf("get position error!\n");
            else if(c3->type == cJSON_Object)
            {
                c4 = cJSON_GetObjectItem(c3,"center");
                if(!c4)
                    printf("get center error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"x");
                    if(!c5)
                        printf("get center x error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("center x : %f\n",c5->valuedouble);

                    c5 = cJSON_GetObjectItem(c4,"y");
                    if(!c5)
                        printf("get center y error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("center y : %f\n",c5->valuedouble);
                }
                c4 = cJSON_GetObjectItem(c3,"eye_left");
                if(!c4)
                    printf("get eye_left error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"x");
                    if(!c5)
                        printf("get eye_left x error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("eye_left x : %f\n",c5->valuedouble);
                    c5 = cJSON_GetObjectItem(c4,"y");
                    if(!c5)
                        printf("get eye_left y error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("eye_left y : %f\n",c5->valuedouble);
                }
                c4 = cJSON_GetObjectItem(c3,"eye_right");
                if(!c4)
                    printf("get eye_right error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"x");
                    if(!c5)
                        printf("get eye_right x error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("eye_right x : %f\n",c5->valuedouble);
                    c5 = cJSON_GetObjectItem(c4,"y");
                    if(!c5)
                        printf("get eye_right y error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("eye_right y : %f\n",c5->valuedouble);
                }

                c4 = cJSON_GetObjectItem(c3,"mouth_left");
                if(!c4)
                    printf("get mouth_left error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"x");
                    if(!c5)
                        printf("get mouth_left x error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("mouth_left x : %f\n",c5->valuedouble);
                    c5 = cJSON_GetObjectItem(c4,"y");
                    if(!c5)
                        printf("get mouth_left y error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("mouth_left y : %f\n",c5->valuedouble);
                }


                c4 = cJSON_GetObjectItem(c3,"mouth_right");
                if(!c4)
                    printf("get mouth_right error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"x");
                    if(!c5)
                        printf("get mouth_right x error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("mouth_right x : %f\n",c5->valuedouble);
                    c5 = cJSON_GetObjectItem(c4,"y");
                    if(!c5)
                        printf("get mouth_right y error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("mouth_right y : %f\n",c5->valuedouble);
                }


                c4 = cJSON_GetObjectItem(c3,"nose");
                if(!c4)
                    printf("get mouth_left error!\n");
                else if(c4->type == cJSON_Object)
                {
                    c5 = cJSON_GetObjectItem(c4,"x");
                    if(!c5)
                        printf("get nose x error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("nose x : %f\n",c5->valuedouble);
                    c5 = cJSON_GetObjectItem(c4,"y");
                    if(!c5)
                        printf("get nose y error!\n");
                    else if(c5->type == cJSON_Number)
                        printf("nose y : %f\n",c5->valuedouble);
                }
                c4 = cJSON_GetObjectItem(c3,"height");
                if(!c4)
                    printf("get height error!\n");
                else if(c4->type == cJSON_Number)
                    printf("height : %f\n",c4->valuedouble);
                c4 = cJSON_GetObjectItem(c3,"width");
                if(!c4)
                    printf("get width error!\n");
                else if(c4->type == cJSON_Number)
                    printf("width : %f\n",c4->valuedouble);

            }

        }
    }

    cJSON_Delete(cJson);
    return 0;
}


int main(void)
{
    int ret;


    FILE *fd = NULL;
    char txtBuffer[BUFSIZE] = {0};
    fd = fopen(JSON_TET,"r");
    if(fd < 0){
        printf("fopen error!\n");
        return -1;
    }

    ret = fread(txtBuffer,BUFSIZE,1,fd);
    if(ret < 0){
        printf("fread error!\n");
        return -1;
    }
    parseJson(txtBuffer);
    fclose(fd);
    return 0;
}

