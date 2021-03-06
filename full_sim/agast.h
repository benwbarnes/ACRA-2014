//
//    agast7s - AGAST, an adaptive and generic corner detector based on the
//              accelerated segment test for a 12 pixel mask in square format
//
//    Copyright (C) 2010  Elmar Mair
//    All rights reserved.
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions are met:
//        * Redistributions of source code must retain the above copyright
//          notice, this list of conditions and the following disclaimer.
//        * Redistributions in binary form must reproduce the above copyright
//          notice, this list of conditions and the following disclaimer in the
//          documentation and/or other materials provided with the distribution.
//        * Neither the name of the <organization> nor the
//          names of its contributors may be used to endorse or promote products
//          derived from this software without specific prior written permission.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//    DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
//    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef AGAST7_12S_H
#define AGAST7_12S_H

#include <stdint.h>
#include <vector>
#include <opencv2/opencv.hpp>

class AstDetector
{
	public:
		AstDetector():xsize(0),ysize(0),b(-1) {}
		AstDetector(int width, int height, int thr):xsize(width),ysize(height),b(thr) {}
		virtual ~AstDetector(){;}
		virtual std::vector<struct CvPoint>& detect(const unsigned char* im)=0;
		virtual int get_borderWidth()=0;
		std::vector<struct CvPoint>& nms(const unsigned char* im);
		std::vector<struct CvPoint>& processImage(const unsigned char* im) {detect(im); return nms(im);}
		std::vector<struct CvPoint>& get_corners_all(){return corners_all;}
		std::vector<struct CvPoint>& get_corners_nms(){return corners_nms;}
		void set_threshold(int b_){b=b_;}
		void set_imageSize(int xsize_, int ysize_){xsize=xsize_; ysize=ysize_; init_pattern();}

	protected:
		virtual void init_pattern()=0;
		virtual int cornerScore(const unsigned char* p)=0;
		void score(const unsigned char* i);
		std::vector<struct CvPoint>& nonMaximumSuppression();

		std::vector<struct CvPoint> corners_all;
		std::vector<struct CvPoint> corners_nms;
		std::vector<int> scores;
		std::vector<int> nmsFlags;
		int xsize, ysize;
		int b;
};

class AgastDetector7_12s : public AstDetector
{
	public:
		AgastDetector7_12s():AstDetector(){;}
		AgastDetector7_12s(int width, int height, int thr):AstDetector(width, height, thr){init_pattern();};
		~AgastDetector7_12s(){}
		std::vector<struct CvPoint>& detect(const unsigned char* im);
		std::vector<struct CvPoint>& nms(const unsigned char* im);
		int get_borderWidth(){return borderWidth;}

	private:
		static const int borderWidth=2;
		int_fast16_t s_offset0;
		int_fast16_t s_offset1;
		int_fast16_t s_offset2;
		int_fast16_t s_offset3;
		int_fast16_t s_offset4;
		int_fast16_t s_offset5;
		int_fast16_t s_offset6;
		int_fast16_t s_offset7;
		int_fast16_t s_offset8;
		int_fast16_t s_offset9;
		int_fast16_t s_offset10;
		int_fast16_t s_offset11;


		int cornerScore(const unsigned char* p);
		void init_pattern()
		{
			s_offset0=(-2)+(0)*xsize;
			s_offset1=(-2)+(-1)*xsize;
			s_offset2=(-1)+(-2)*xsize;
			s_offset3=(0)+(-2)*xsize;
			s_offset4=(1)+(-2)*xsize;
			s_offset5=(2)+(-1)*xsize;
			s_offset6=(2)+(0)*xsize;
			s_offset7=(2)+(1)*xsize;
			s_offset8=(1)+(2)*xsize;
			s_offset9=(0)+(2)*xsize;
			s_offset10=(-1)+(2)*xsize;
			s_offset11=(-2)+(1)*xsize;
		}
};

#endif /* AGASTDETECTOR_H */
