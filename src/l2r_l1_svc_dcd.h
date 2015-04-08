#ifndef __L2R_L1_SVC_DCD
#define __L2R_L1_SVC_DCD

// L2 regularized L1 loss support vector classification
// Solved by Liblinear algorithm (dual coordinate descent) (Hsieh et. al, ICML08)
// Optimize
// f(alpha) = 1/2 alpha^T D alpha - e^T alpha

#include <vector>
#include "solver.h"

class Model;
class Data;

class L2R_L1_SVC_DCD : public Solver {
	public:
		L2R_L1_SVC_DCD(double C, double epsilon, const Data &data, bool verbose=false);

		Model train(int max_iter = 1000);

	private:
		double C;
		double epsilon;
		const Data &data;
		std::vector<double> q_diag;
		bool verbose;
};

#endif
