#include<cstring>
#include<iostream>
#include "YUVConverter.h"
void YUV422toYUV420(unsigned char *yuv422, unsigned char *yuv420,  int Y_width, int Y_height)
{
    int UV_width_422 = Y_width >> 1;   //422采样的U和V信号在行方向是隔一个采样一次
    int UV_height_422 = Y_height;      //422采样的U和V信号在列方向是全采样的
 
    int UV_width_420 = Y_width >> 1;   //420采样的U和V信号在行方向是隔一个采样一次
    int UV_height_420 = Y_height >> 1; //420采样的U和V信号在列方向是隔一个采样一次
 
    int Ylen = Y_width * Y_height;     //有多少像素就有多少个Y分量
 
    /* YUV422P的存储方式：
       YYYYYYYY
       YYYYYYYY
       UUUUUUUU
       VVVVVVVV
    */
    unsigned char *p_Y422 = yuv422 ;
    unsigned char *p_U422 = p_Y422 + Ylen ;
    unsigned char *p_V422 = p_U422 + Ylen / 2 ;
    
    /* YUV420P的存储方式：
       YYYYYYYY
       YYYYYYYY
       UUUU
       VVVV
    */
    unsigned char *p_Y420 = yuv420 ;
    unsigned char *p_U420 = p_Y420 + Ylen ;
    unsigned char *p_V420 = p_U420 + Ylen / 4;
 
 
    //Y分量都是一样的，直接复制
    memcpy(p_Y420, p_Y422, Ylen);
 
 
    /* 对于一个4*4的YUV422来说，U分量的采样结果是:
     * U0  X  U2  X
     * U4  X  U6  X
     * U8  X  U10 X
     * U12 X  U14 X
     *
     * 而同样大小的YUV420,U分量的采样结果：
     * U0  X  U2  X
     * X   X  X   X
     * U8  X  U10 X
     * X   X  X   X
     * 
     * 在plane的存储方式下，这两种采样U分量的存放区别是：
     * YUV422P:
     * U0 U2|U4 U6|U8 U10|U12 U14
     * 
     * YUV420P:
     * U0 U2|U8 U10
     * 用|隔开不表示在存储中被隔开，仅表示这是一个UV_width_422宽度
     */
     
     
    int k = 0 ;
    unsigned char u1, u2 , v1, v2 ;
    for ( int i = 0 ; i < UV_height_422 ; i += 2)
    {
        for ( int j = 0 ; j < UV_width_422 ; j++)
        {
            // 取422p相邻行的两个U分量的平均，作为420取样的U分量
            u1 = *(p_U422 + i * UV_width_422 + j);
            u2 = *(p_U422 + (i + 1) * UV_width_422 + j);
            // 取422p相邻行的两个V分量的平均，作为420取样的V分量
            v1 = *(p_V422 + i * UV_width_422 + j);
            v2 = *(p_V422 + (i + 1) * UV_width_422 + j);
 
            *(p_U420 + k * UV_width_420 + j) = ((u1 + u2) >> 1);
            *(p_V420 + k * UV_width_420 + j) = ((v1 + v2) >> 1);
        }
        k++;
    }
}

void YUV420toYUV422(unsigned char *yuv420, unsigned char *yuv422, int Y_width, int Y_height) {
	int UV_width_422 = Y_width>>1;   //422色度信号U和V宽度
	int UV_height_422 = Y_height;    //422色度信号U和V高度 

	int UV_width_420 = Y_width>>1;   //420色度信号U和V宽度
	int UV_height_420 = Y_height>>1; //420色度信号U和V高度 

	int Ylen = Y_width * Y_height;

	unsigned char *p_Y420 = yuv420 ;   
	unsigned char *p_U420 = p_Y420 + Ylen ;   
	unsigned char *p_V420 = p_U420 + Ylen / 4 ;   

	unsigned char *p_Y422 = yuv422 ;   
	unsigned char *p_U422 = p_Y422 + Ylen ;   
	unsigned char *p_V422 = p_U422 + Ylen / 2 ;   
    
	//Y
	memcpy(p_Y422, p_Y420, Ylen); 
    
    for (int uv = 0; uv < UV_height_420; uv++) {
		//U
		memcpy(p_U422 + uv*Y_width,					p_U420 + uv*UV_width_422, UV_width_422);
		memcpy(p_U422 + uv*Y_width + UV_width_422,	p_U420 + uv*UV_width_422, UV_width_422);
    }
    for (int uv = 0; uv < UV_height_420; uv++) {
		//V
		memcpy(p_V422 + uv*Y_width,					p_V420 + uv*UV_width_422, UV_width_422);
		memcpy(p_V422 + uv*Y_width + UV_width_422,	p_V420 + uv*UV_width_422, UV_width_422);
	}
    
}