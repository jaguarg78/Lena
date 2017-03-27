/*
 * SVD.cpp
 *
 *  Created on: Mar 25, 2017
 *      Author: gianni
 */

#include "SVD.h"

extern "C" {
    /* DGESVD prototype */
    extern void dgesvd_(char*   pcJobU,     // Defines the computing options of U
                        char*   pcJobVt,    // Defines the computing options of U
                        int*    piRows,     // Number of Matrix' Rows (m)
                        int*    piColumns,  // Number of Matrix' Columns (n)
                        double* pdMatrixA,  // m x n Matrix A as an array
                        int*    piLeadA,    // Leading Dimension of A (m)
                        double* pdMatrixS,  // m Vector S
                        double* pdMatrixU,  // m x n Matrix U as an array
                        int*    piLeadV,    // Leading Dimension of U (n)
                        double* pdMatrixV,  // m x n Matrix Vt as an array
                        int*    piLeadU,    // Leading Dimension of V (m)
                        double* pdWork,     // Workspace as an array
                        int*    piLeadWS,   // LD of Workspace >= max(3*min(m,n)+max(m,n), 5*min(m,n)).
                                            // -1 calculates optimal Workspace size.
                        int*    piInfo);    // 0 Success.
                                            // < 0 = -j. jth parameter argument illegal value
                                            // > 0. Algorithm did not converge
}

namespace Lena {
	SVD::SVD(const Eigen::MatrixXd& rBlock) : _uiRows(rBlock.rows()),
	                                          _uiCols(rBlock.cols()) {
		process(rBlock);
	}

	SVD::~SVD() throw() {
		// TODO Auto-generated destructor stub
	}

	void SVD::process(const Eigen::MatrixXd& matA) {
		int iSize = matA.rows();
					int iLeadWS = -1;
					int iInfo;
					double dOptWS;
					Eigen::MatrixXd mATemp = matA;
		#ifdef _DEBUG
					std::cout << "Matrix A" << std::endl;
					std::cout << matA << std::endl;
					std::cout << "Matrix A copy" << std::endl;
					std::cout << mATemp << std::endl;
		#endif
					_matU.resize(iSize, iSize);
					_vecS.resize(iSize);
					_matV.resize(iSize, iSize);

					Eigen::MatrixXd tempV(iSize, iSize);
					/* Getting Optimal Workspace size */
					::dgesvd_(const_cast<char *>(std::string("All").c_str()),
							  const_cast<char *>(std::string("All").c_str()),
							  &iSize,
							  &iSize,
							  mATemp.data(),
							  &iSize,
							  _vecS.data(),
							  _matU.data(),
							  &iSize,
							  tempV.data(),
							  &iSize,
							  &dOptWS,
							  &iLeadWS,
							  &iInfo);
					iLeadWS = (int) dOptWS;

					double *pdWS = new double[iLeadWS * sizeof(double)];
					::dgesvd_(const_cast<char *>(std::string("All").c_str()),
							  const_cast<char *>(std::string("All").c_str()),
							  &iSize,
							  &iSize,
							  mATemp.data(),
							  &iSize,
							  _vecS.data(),
							  _matU.data(),
							  &iSize,
							  tempV.data(),
							  &iSize,
							  pdWS,
							  &iLeadWS,
							  &iInfo);
					_matV = tempV.transpose();
		#if _DEBUG
					std::cout << "Matrix U" << std::endl;
					std::cout << svd.matU << std::endl;
					std::cout << "Vector S" << std::endl;
					std::cout << svd.matS << std::endl;
					std::cout << "Matrix V" << std::endl;
					std::cout << svd.matV << std::endl;
					std::cout << "Matrix A Result" << std::endl;
					std::cout << svd.matU * svd.matS * svd.matV.transpose() << std::endl;
		#endif
	}

	unsigned SVD::rows() const {
	    return _uiRows;
	}
    unsigned SVD::cols() const {
        return _uiCols;
    }

	const Eigen::MatrixXd& SVD::matrixU() const {
		return _matU;
	}

	const Eigen::VectorXd& SVD::singularValues() const {
		return _vecS;
	}

	const Eigen::MatrixXd& SVD::matrixV() const {
		return _matV;
	}
}
