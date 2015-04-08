#include "solver.h"
#include "l2r_l1_svc_dcd.h"
#include "data.h"
#include "model.h"
#include "utils.h"
#include <algorithm>
#include <cassert>

DEFINE_bool(verbose, false, "Verbose output.");

Solver* createSolver(double C, double epsilon, BinaryData &data, std::string type) {
	if (type == "ll3") {
		return new L2R_L1_SVC_DCD(C, epsilon, data, FLAGS_verbose);
	} else {
		assert(0);
		return NULL;
	}
}

MultiModel train(double C, double epsilon, BinaryData &data, std::string type) {
	MultiModel mmodel;

	if (data.l() == 2) {
		data.setLabel(1);
		Solver *solver = createSolver(C, epsilon, data, type);
		mmodel.models.push_back( solver->train() );
		delete solver;
	} else {
		for (int l=0; l<data.l(); l++) {
			data.setLabel(l);
			Solver *solver = createSolver(C, epsilon, data, type);
			mmodel.models.push_back( solver->train() );
			delete solver;
		}
	}

	return mmodel;
}

double score(const std::vector<Entry> &instance, const Model &model) {
	double result = 0;
	for (auto &entry: instance)
		result += model.w[entry.index] * entry.value;

	return result;
}

std::vector<int> predict(const Data &data, const MultiModel &mmodel) {
	std::vector<int> predictions;
	predictions.reserve(data.n());

	if (data.l() == 2) {
		for (auto &instance: data.data) {
			double s = score(instance, mmodel.models[0]);
			predictions.push_back(s>0);
		}
	} else {
		std::vector<double> scores(data.l());
		for (auto &instance: data.data) {
			for (int l=0; l<data.l(); l++)
				scores[l] = score(instance, mmodel.models[l]);

			predictions.push_back(std::max_element(scores.begin(), scores.end()) - scores.begin());
		}
	}

	return predictions;
}

double accuracy(const std::vector<int> &pred, const Data &data) {
	int ncorrect = 0;
	for (int i=0; i<data.n(); i++)
		if (pred[i] == data.multLabel(i))
			ncorrect++;

	return (double)ncorrect / data.n();
}
