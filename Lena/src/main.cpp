/*
 * main.cpp
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */


#include <iostream>


#include "BMPData.h"
#include "GenericImage.h"
#include "TestImageData.h"
#include "Utilities.h"
#include "WMProcess.h"

#define SHOWN_PIXELS    12
#define INIT_PIXEL      30

int main(int argc, char *argv[]) {
	std::cout << "Starting Lena....." << std::endl;

//	if (argc != 3) {
//		std::cerr << "Error. File name is missing...." << std::endl;
//		return 1;
//	}

	try {
		BMPData inputImage("images/lena_GS.bmp");
		inputImage.init();
		std::cout << inputImage << std::endl;

		BMPData logoImage("images/Test_32x32_BI.bmp");
		logoImage.init();
		std::cout << logoImage << std::endl;

		BMPData hostImage(inputImage);
		std::cout << hostImage << std::endl;

        BMPData logoOutImage(32, 32);
        std::cout << logoOutImage << std::endl;
//
//		BMPData testImage("images/rgb-Base_48x48.bmp");
//		testImage.init();
//        std::cout << testImage << std::endl;

//        BMPData testImage("images/test.bmp");
//        testImage.init();
//        std::cout << testImage << std::endl;
//        unsigned char *pucData = const_cast<unsigned char *>(testImage.getData());
//        for (unsigned int  i = 0; i < testImage.getDataSize(); i++) {
//            if ((i * CHAR_BIT) % testImage.getWidth() == 0) {
//                std::cout << std::endl << "[" << std::dec << i << "]";
//            }
//            std::cout << std::hex << (int) pucData[i] << " ";
//        }
//        std::cout << std::dec << std::endl;
//
//        std::cout << "************** //////////// 1 **********" << std::endl;
//        BMPData testImage2("images/Test_32x32_BI.bmp");
//        testImage2.init();
//        std::cout << testImage2 << std::endl;
//        unsigned char *pucData2 = const_cast<unsigned char *>(testImage2.getData());
//        for (unsigned int  i = 0; i < testImage2.getDataSize(); i++) {
//            if ((i * CHAR_BIT) % testImage2.getWidth() == 0) {
//                std::cout << std::endl << "[" << std::dec << i << "]";
//            }
//            std::cout << std::hex << (int) pucData2[i] << " ";
//        }
//        std::cout << std::dec << std::endl;
//
//        std::cout << "************** //////////// 2 **********" << std::endl;
//        GenericImage image("images/Test_32x32_BI.bmp");
//        std::vector<unsigned char> imageData(image.getDataSize());
//        std::cout << "Array Size: " << imageData.size() << std::endl;
//        std::cout << "Copied amount of pixels: " << image.getData(imageData) << std::endl;
//
//        for (unsigned int  i = 0; i < imageData.size(); i++) {
//            if ((i * CHAR_BIT) % image.getWidth() == 0) {
//                std::cout << std::endl << "[" << std::dec << i << "]";
//            }
//
//            std::cout << std::hex << (int) imageData[i] << " ";
//        }
//        std::cout << std::endl;
//
//        std::vector<PixelTable> pColormap;
//        PixelTable pixel0;
//        pixel0.pixel.byteRed = 0x00;
//        pixel0.pixel.byteGreen = 0x00;
//        pixel0.pixel.byteBlue = 0x00;
//        pixel0.byteUnused = 0x00;
//        PixelTable pixel1;
//        pixel1.pixel.byteRed = 0xFF;
//        pixel1.pixel.byteGreen = 0xFF;
//        pixel1.pixel.byteBlue = 0xFF;
//        pixel1.byteUnused = 0x00;
//        pColormap.push_back(pixel0);
//        pColormap.push_back(pixel1);
//        BMPData output(imageData, 32, 32, &pColormap);
//        output.create("images/test.bmp", true);
//
//
//
//        std::cout << "****************** image 2 ************" << std::endl;
//
//        GenericImage image2("images/test.bmp");
//        std::vector<unsigned char> imageData2(image2.getDataSize());
//        std::cout << "Array Size: " << imageData2.size() << std::endl;
//        std::cout << "Copied amount of pixels: " << image2.getData(imageData2) << std::endl;
//
//        for (unsigned int  i = 0; i < imageData2.size(); i++) {
//            if ((i * CHAR_BIT) % image2.getWidth() == 0) {
//                std::cout << std::endl << "[" << std::dec << i << "]";
//            }
//
//            std::cout << std::hex << (int) imageData2[i] << " ";
//        }
//        std::cout << std::endl;

//        std::vector<unsigned char> imageOutput(image.getDataSize());
//        for (unsigned int i = 0; i < imageData.size(); i++) {
//            unsigned char ucByteData = imageData.data()[i];
//            unsigned char ucNewByte = 0x0;
//            for (unsigned int j = 0; j < CHAR_BIT; j++) {
//                ucNewByte |= (ucByteData & 0x80) >> (CHAR_BIT - (j + 1));
////                std::cout << (int) ucByteData << "/" << (int) ucNewByte << " ";
//                ucByteData <<= 1;
//            }
////            std::cout << "->" << (int) ucNewByte << std::endl;
//            imageOutput.data()[image.getDataSize() - (i + 1)] = ucNewByte;
//        }
//
//        std::cout << "***************************" << std::endl;
//        for (unsigned int i = 0; i < imageOutput.size(); i++) {
//            if (i % 4 == 0) {
//                std::cout << std::endl << "[" << std::dec << i / 4 << "] ";
//            }
//
//            std::cout << std::hex << (int) imageOutput.data()[i] << " ";
//        }

//	    /********************************/
//	    BMPData inputImage("images/Test_32x32_BI.bmp");
//	    inputImage.init();
//	    std::cout << inputImage << std::endl;
//
//	    std::vector<unsigned char> dataVector(const_cast<unsigned char *>(inputImage.getData()),
//	                                          const_cast<unsigned char *>(inputImage.getData()) + inputImage.getDataSize());
//
//	    BMPData outputImage(dataVector, 32, 32, BMPData::TYPE_BINARY);
//	    outputImage.create("images/test2.bmp", true);
//
//	    BMPData inputImage2("images/rgb-Base_48x48.bmp");
//        inputImage2.init();
//        std::cout << inputImage2 << std::endl;
//
//        std::vector<unsigned char> dataVector2(const_cast<unsigned char *>(inputImage2.getData()),
//                                               const_cast<unsigned char *>(inputImage2.getData()) + inputImage2.getDataSize());
//
//        BMPData outputImage2(dataVector2, 48, 48, BMPData::TYPE_RGB888);
//        outputImage2.create("images/test.bmp", true);
//
//	    BMPData inputImage3("images/test.bmp");
//	    inputImage3.init();
//        std::cout << inputImage3 << std::endl;

//        BMPData inputImage3("images/lena_GS.bmp");
//        inputImage3.init();
//        std::cout << inputImage3 << std::endl;
//
//        std::vector<unsigned char> dataVector3(const_cast<unsigned char *>(inputImage3.getData()),
//                                               const_cast<unsigned char *>(inputImage3.getData()) + inputImage3.getDataSize());

//        GenericImage::createRGB888Image("images/test100.jpg", 512, 512, dataVector3.data(), "JPG", 100);
//        GenericImage::createRGB888Image("images/test090.jpg", 512, 512, dataVector3.data(), "JPG", 90);
//        GenericImage::createRGB888Image("images/test080.jpg", 512, 512, dataVector3.data(), "JPG", 80);
//        GenericImage::createRGB888Image("images/test070.jpg", 512, 512, dataVector3.data(), "JPG", 70);
//        GenericImage::createRGB888Image("images/test060.jpg", 512, 512, dataVector3.data(), "JPG", 60);
//        GenericImage::createRGB888Image("images/test050.jpg", 512, 512, dataVector3.data(), "JPG", 50);
//        GenericImage::createRGB888Image("images/test040.jpg", 512, 512, dataVector3.data(), "JPG", 40);
//        GenericImage::createRGB888Image("images/test030.jpg", 512, 512, dataVector3.data(), "JPG", 30);
//        GenericImage::createRGB888Image("images/test020.jpg", 512, 512, dataVector3.data(), "JPG", 20);
//        GenericImage::createRGB888Image("images/test010.jpg", 512, 512, dataVector3.data(), "JPG", 10);

//	      std::vector<unsigned char> vec1;
//	      vec1.push_back(43);
//	      vec1.push_back(43);
//	      vec1.push_back(43);
//	      vec1.push_back(43);
//	      vec1.push_back(43);
//	      vec1.push_back(43);
//	      vec1.push_back(54);
//	      vec1.push_back(54);
//	      vec1.push_back(54);
//	      vec1.push_back(51);
//	      vec1.push_back(51);
//	      vec1.push_back(51);
//	      TestImageData test1("test1",
//	                          2,
//	                          2,
//	                          vec1);
//	      std::cout << test1 << std::endl;
//
//	      std::vector<unsigned char> vec2;
//        vec2.push_back(38);
//        vec2.push_back(38);
//        vec2.push_back(38);
//        vec2.push_back(38);
//        vec2.push_back(38);
//        vec2.push_back(38);
//        vec2.push_back(59);
//        vec2.push_back(59);
//        vec2.push_back(59);
//        vec2.push_back(54);
//        vec2.push_back(54);
//        vec2.push_back(54);
//        TestImageData test2("test2",
//                            2,
//                            2,
//                            vec2);
//        std::cout << test2 << std::endl;
//
//        std::cout << Utilities::PSNR_RGB(test1, test2) << std::endl;

//		  SWatermark wm(4, 4, 20, 3);
//		  wm.embedLogo(inputImage, logoImage);
//        std::cout << "The End!!!" << std::endl;

//		  BMPData inputRGB("images/lena_rgb.bmp");
//		  inputRGB.init();
//		  std::cout << inputRGB << std::endl;
//
//		  BMPData outputRGB_1(inputRGB);
//		  Utilities::toGrayScale(outputRGB_1, Utilities::GRAY_MEAN);
//		  outputRGB_1.create("images/test_1.bmp");
//
//		  BMPData outputRGB_2(inputRGB);
//        Utilities::toGrayScale(outputRGB_2, Utilities::GRAY_LIGHTNESS);
//        outputRGB_2.create("images/test_2.bmp");
//
//        BMPData outputRGB_3(****BCR***inputRGB);
//        Utilities::toGrayScale(outputRGB_3, Utilities::GRAY_LUMINOSITY);
//        outputRGB_3.create("images/test_3.bmp");


        SProcess::WMProcess_17 wm_17(20);
        wm_17.embedLogo(hostImage, logoImage);
        wm_17.extractLogo(logoOutImage, hostImage);
//
//        SProcess::WMProcess_35 wm_35(20);
//        wm_35.embedLogo(hostImage, logoImage);
//        wm_35.extractLogo(logoOutImage, hostImage);
//
//        SProcess::WMProcess_74 wm_74(10., 8);
//        wm_74.embedLogo(hostImage, logoImage);
//        wm_74.extractLogo(logoOutImage, hostImage);
//
//        UProcess::WMProcess_43 wm_43(1, BLOCK_DIMENSION_DEFAULT_43, 1, 3);
//        UProcess::WMProcess_43 wm_43(1);
//        wm_43.embedLogo(hostImage, logoImage);
//        wm_43.extractLogo(logoOutImage, hostImage);
//
//        UProcess::WMProcess_09 wm_09(.002);
//        wm_09.embedLogo(hostImage, logoImage);
//        wm_09.extractLogo(hostImage, logoImage);
//
//        UVProcess::WMProcess_18 wm_18(.002);
//        wm_18.embedLogo(hostImage, logoImage);
//        wm_18.extractLogo(logoOutImage, hostImage);

//		Eigen::MatrixXd test(4,4);
//		test << 43, 43, 54, 51,
//		        43, 43, 54, 51,
//		        42, 42, 49, 46,
//		        46, 46, 49, 49;
//		Utilities::getSVD(test);

//        std::cout << "Ouput File: Data" << std::endl;
//        std::cout << hostImage << std::endl;

//        std::cout << "PSNR : " << Utilities::PSNR_RGB(inputImage, hostImage) << std::endl;
        std::cout << "BCR: " << Utilities::BCR(logoImage, logoOutImage) << std::endl;
        logoOutImage.create(std::string("output/images/logoOutput.bmp"));
        std::cout << "The End!!!" << std::endl;
	} catch (std::exception& e) {
		std::cerr << "Error. " << e.what() << std::endl;
	} catch (...) {
	    std::cerr << "Generic Error" << std::endl;
	}

	return 0;
}


