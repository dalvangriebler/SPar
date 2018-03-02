/* ***************************************************************************
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 *  As a special exception, you may use this file as part of a free software
 *  library without restriction.  Specifically, if other files instantiate
 *  templates or use macros or inline functions from this file, or you compile
 *  this file and link it with other files to produce an executable, this
 *  file does not by itself cause the resulting executable to be covered by
 *  the GNU General Public License.  This exception does not however
 *  invalidate any other reasons why the executable file might be covered by
 *  the GNU General Public License.
 *
 ****************************************************************************
 *  Authors: Dalvan Griebler <dalvangriebler@gmail.com>
 *         
 *  Copyright: GNU General Public License
 *  Description: The Sobel Application reads BMP images and 
 *  apply the filter sobel and save the image filtered in the root directory.
 *  File Name: sobel.cpp
 *  Version: 1.0 (14/07/2016)
 *  Compilation Command: spar -std=c++1y -spar_file sobel.cpp -o exe
 *	Exacution Command: ./exe 800x600/
*/

#include <cassert>
#include <iostream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <chrono>
#include <dirent.h>

#define BYTES_BLOCK	6000000
#define DESTINATION_DIR	"filtered/"


#define ABS(x)			((x<0)?(-x):(x))
#define ROUND_INT(x)		((x<0)?(int)(x-0.5):(int)(x+0.5))

bool getImageInfo(const char *filename, int &width, int &height, long &leng, unsigned int &header_size){ 
	FILE *f;
	if ((f=fopen(filename,"rb")) == NULL){
		std::cerr << "Error in fopen: " << filename << " -> " << strerror(errno) << std::endl;
		return false;
	}
	fseek(f,0,SEEK_END);
	long len=ftell(f);
	leng=len;
	fseek(f,0,SEEK_SET);
	if (len<24){
		fclose(f);
		std::cerr << filename << " is less than 24 bytes"  << std::endl;
		return false;
	}	
    unsigned char buf[24]; 
    if (fread(buf,1,24,f) != 24) {fputs ("Reading error",stderr); exit (3);}
	fclose(f);
	// BMP: first two bytes say BM, http://en.wikipedia.org/wiki/BMP_file_format 
	if (buf[0]=='B' && buf[1]=='M'){
		header_size = 54;
		width = (buf[18]) + (buf[19]<<8);
		height = (buf[22]) + (buf[23]<<8);
		return true;
	}
	return false;
}
void completMatrixSobelH(char buffer[3][3]){
    buffer[0][0] = 1;
	buffer[0][1] = 2;
	buffer[0][2] = 1;
	buffer[1][0] = 0;
	buffer[1][1] = 0;
	buffer[1][2] = 0;
	buffer[2][0] = -1;
	buffer[2][1] = -2;
	buffer[2][2] = -1;
}
void completMatrixSobelV(char buffer[3][3]){
    buffer[0][0] = -1;
	buffer[0][1] = 0;
	buffer[0][2] = 1;
	buffer[1][0] = -2;
	buffer[1][1] = 0;
	buffer[1][2] = 2;
	buffer[2][0] = -1;
	buffer[2][1] = 0;
	buffer[2][2] = 1;
}
unsigned char sobel(unsigned char buffer[3][3]){
	int gx=0, gy=0, sum=0;
	char sobel_x[3][3], sobel_y[3][3];
	completMatrixSobelH(sobel_x);
	completMatrixSobelV(sobel_y);
	unsigned char x,y;
	for(y=0;y<3;y++){
		for(x=0;x<3;x++){
			gy += ((short int)buffer[y][x] * (short int)sobel_y[y][x]);
			gx += ((short int)buffer[y][x] * (short int)sobel_x[y][x]);
		}
	}
	sum = ABS(gy) + ABS(gx);
	if(sum > 255) sum = 255;
	return (255-sum);
}

unsigned int tot_img=0, tot_not=0;

int process(int argc, char *argv[]){
	char lcomand[256];
	std::strcpy(lcomand, "mkdir -p ");
	std::strcat(lcomand, DESTINATION_DIR);
	if (system(lcomand)){
		std::cerr << "Error in: " << lcomand << " -> " << strerror(errno) << std::endl;
		return -1;
	}
	std::string filepath=argv[1];
	DIR *dptr = opendir ((char *)(filepath.c_str()));
    if (dptr == NULL) {
        std::cerr << "Argument error: " << filepath << " -> " << strerror(errno) << std::endl;
        exit (3);
    }
    struct dirent *dfptr;
    //SPar annotation
	[[spar::ToStream,spar::Input(dptr,dfptr,filepath,tot_img,tot_not), spar::Output(tot_img,tot_not)]]
	while((dfptr = readdir(dptr)) != NULL){	//stream comes from a directory
		const std::string &filename(dfptr->d_name);
		char *name = new char [256];
		std::strcpy (name, filepath.c_str());
		if (filepath.at(filepath.length()-1) != '/') std::strcat (name, "/");
		std::strcat (name, filename.c_str());
		char *newname = new char [256];
		std::strcpy (newname, DESTINATION_DIR);
		std::strcat (newname, filename.c_str());
		if (filename.substr(filename.find_last_of(".") + 1) == "bmp"){ // ignores files that are not .bmp extensions
			tot_img++;
			//SPar annotation
			[[spar::Stage, spar::Input(name,newname), spar::Replicate(2)]]{
				int width, height;
				unsigned int header_size;
				long len;
				if (getImageInfo(name, width, height, len, header_size) == false){
					std::cerr << "Error in getImageInfo()!!" << std::endl;
					exit (3);
				}
				unsigned char *filtered_image = (unsigned char *) malloc(height*width);
				unsigned char *image = (unsigned char *) malloc(width*height);
				unsigned char *all_bytes = (unsigned char *) malloc(len+3);
				uint32_t padbytes = (width*3)%4;
				if(padbytes!=0){
					padbytes = 4-padbytes;
				}
				//---------------------------------------------------------------------------------------------------
				//starts to read the image
				FILE *fptr;
				if((fptr=fopen(name,"r"))==NULL){
					std::cerr << "Error in fopen: " << name << " -> " << strerror(errno) << std::endl;
					exit (3);
				}else{
					size_t tot=len;
					size_t rest = tot/BYTES_BLOCK;
					size_t block=0;
					if (rest != 0) block=tot/rest;
					if (block==0){
						block=tot;
						if(fread(all_bytes,sizeof(unsigned char),block,fptr) != block) {fputs ("Reading error\n",stderr); exit (3);}
					}else{
						size_t l;
						for (l = 0; l < rest-1; ++l){
							if(fread(&all_bytes[block*l],sizeof(unsigned char),block,fptr) != block) {fputs ("Reading error\n",stderr); exit (3);}
						}
						if(fread(&all_bytes[block*l],sizeof(unsigned char),(block+(tot%rest)),fptr) != (block+(tot%rest))) {fputs ("Reading error\n",stderr); exit (3);}
					}
					fclose(fptr);					
					//---------------------------------------------------------------------------------------------------
					for (int y=0, p=0, a=header_size; y < height; ++y){
						for (int x = 0; x < width; ++x, p++){
							for (int z = 0; z < 3; ++z){
								if (padbytes != 0 && padbytes !=3 && x >= width-pow(padbytes,3)*3){
									
								}else{
									image[p] = all_bytes[a];
									a++;
								}	
							}																									
						}
						for(uint32_t t=0;t<padbytes;t++,a++){
							all_bytes[a] = 0;						
						}
					}
					//---------------------------------------------------------------------------------------------------
					unsigned char image_buffer[3][3];
					for(int y=1;y<height-1;y++){
						for(int x=1;x<width-1;x++){
							for(int v=0;v<3;v++){
								for(int u=0;u<3;u++){
									image_buffer[v][u] = image[(((y+v-1)*width)+(x+u-1))];
									filtered_image[((y*width)+x)] = sobel(image_buffer);								
								}
							}	
						}
					}
					free(image);
					//---------------------------------------------------------------------------------------------------
					for (int y=0, a=header_size; y < height; ++y){
						for (int x = 0; x < width; ++x){
							for (int z = 0; z < 3; ++z){
								if (padbytes != 0 && padbytes !=3 && x >= width-pow(padbytes,3)*3){
									
								}else{
									all_bytes[a] = filtered_image[((y*width)+x)];
									a++;
								}
							}
						}
						for(uint32_t t=0;t<padbytes;t++,a++){
							all_bytes[a] = 0;
						}
					}
					free(filtered_image);
					//---------------------------------------------------------------------------------------------------
					//starts to write the image
					FILE *fptr2;
					if((fptr2=fopen(newname,"w"))==NULL){
						std::cerr << "Error in fopen: " << newname << " -> " << strerror(errno) << std::endl;
						exit (3);
					}else{
						size_t tot=len;
						size_t rest = tot/BYTES_BLOCK;
						size_t block=0;
						if (rest != 0) block=tot/rest;
						if (block==0){
							block=tot;
							if(fwrite(all_bytes,sizeof(unsigned char),block,fptr2) != block) {fputs ("Writing error\n",stderr); exit (3);}
						}else{
							size_t l;
							for (l = 0; l < rest-1; ++l){
								if(fwrite(&all_bytes[block*l],sizeof(unsigned char),block,fptr2) != block) {fputs ("Writing error\n",stderr); exit (3);}	
							}
							if(fwrite(&all_bytes[block*l],sizeof(unsigned char),(block+(tot%rest)),fptr2) != (block+(tot%rest))) {fputs ("Writing error\n",stderr); exit (3);}
						}				
						fclose(fptr2);
					}
					free(all_bytes);
				}
			}
		}else{
			tot_not++;				
		}
	}
	closedir(dptr);
   	return 0;
}
int main(int argc, char *argv[]){
	if (argc <= 1 ) {
		std::cerr << "use: " << argv[0] << " <directory of images> " << std::endl;
		return -1;
	}	
	auto t_start = std::chrono::high_resolution_clock::now();
	if (process(argc, argv) != 0){
		return -1;
	}
	auto t_end = std::chrono::high_resolution_clock::now();
	std::cout << "Execution Time(seconds): "  <<  std::chrono::duration<double>(t_end-t_start).count() << " | Total Images: " << tot_img << " | Total Not: " << tot_not << std::endl;
	return 0;
}
