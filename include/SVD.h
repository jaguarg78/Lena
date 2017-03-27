/*
 * SVD.h
 *
 *  Created on: Mar 25, 2017
 *      Author: gianni
 */

#ifndef SVD_H_
#define SVD_H_

#include <eigen3/Eigen/Dense>

namespace Lena {
	class SVD /* : public Eigen::JacobiSVD */ {
	public:
		SVD(const Eigen::MatrixXd& rBlock);
		virtual ~SVD() throw();

		const Eigen::MatrixXd& matrixU() const;
		const Eigen::VectorXd& singularValues() const;
		const Eigen::MatrixXd& matrixV() const;

		unsigned rows() const;
		unsigned cols() const;
	private:
		void process(const Eigen::MatrixXd& rBlock);

		unsigned         _uiRows;
		unsigned         _uiCols;
		Eigen::MatrixXd  _matU;
		Eigen::VectorXd  _vecS;
		Eigen::MatrixXd  _matV;
	};
};

#endif /* SVD_H_ */
