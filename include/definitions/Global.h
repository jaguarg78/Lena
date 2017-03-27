/*
 * Global.h
 *
 *  Created on: Jan 17, 2016
 *      Author: gianni
 */

#ifndef INCLUDE_GLOBAL_H_
#define INCLUDE_GLOBAL_H_

#include <iostream>
#include <iomanip>
#include <climits>

#include <eigen3/Eigen/Dense>

#include "external/GLogger.h"

enum WMProcess_Types {
	TYPE_I,
	TYPE_II,
	TYPE_III,
	TYPE_IV,
	TYPE_NUMS
};

/*
 * Pixel structures
 */
typedef struct {
    unsigned char   byteBlue;           // 1 byte
    unsigned char   byteGreen;          // 1 byte
    unsigned char   byteRed;            // 1 byte
} Pixel;

typedef struct {
    Pixel           pixel;              // 3 bytes
    unsigned char   byteUnused;         // 1 byte
} PixelTable;

/*
 * Supported Depths
 */
#define BIT_COUNT_BINARY_IMAGE       1
#define BIT_COUNT_8_BIT_IMAGE        8
#define BIT_COUNT_RGB_IMAGE          sizeof(Pixel) * CHAR_BIT

/*
 * Normalized channel values boundaries
 */
#define PIXEL_CHANNEL_MAX            255
#define PIXEL_CHANNEL_MIN            0

#define DATA_SIZE_PER_BLOCK_DEFAULT  1
#define REDUDANCY_DEFAULT            1
#define ITERATIONS_DEFAULT           1
#define BLOCK_DIMENSION_DEFAULT      4

#define REDUDANCY_DEFAULT_S          3

#define BLOCK_DIMENSION_DEFAULT_43   8
#define PROTECTED_COLUMNS_DEFAULT_43 1
#define MIN_PROTECTED_COLUMNS        PROTECTED_COLUMNS_DEFAULT_43

#define DATA_SIZE_PER_BLOCK_18       2

#define PADDING_ADJUSTMENT_BYTES     4

namespace Utilities {
	struct SVD {
		Eigen::MatrixXd  matU;
		Eigen::MatrixXd  matS;
		Eigen::MatrixXd  matV;
	};

	/* Debug functions */
	namespace Debug {
		const std::string getValuesFromVector(const std::vector<unsigned char>& vecInput,
											  unsigned int uiOffset,
											  unsigned int uiWidth);

		template<typename T>
		const std::string getString(T& t) {
			std::ostringstream oss;
			oss << t;

			return oss.str();
		}
	}
}

#endif /* INCLUDE_GLOBAL_H_ */
