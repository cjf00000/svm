#include "l2r_l1_svc_dcd.h"
#include "model.h"
#include "data.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <numeric>

L2R_L1_SVC_DCD::L2R_L1_SVC_DCD(double C, double epsilon, const Data &data, bool verbose): C(C), epsilon(epsilon), data(data), verbose(verbose) {
	q_diag.resize(data.n());

	for (int i=0; i<data.n(); i++)
		for (auto entry: data.data[i])
			q_diag[i] += entry.value * entry.value;
}

Model L2R_L1_SVC_DCD::train(int max_iter) {
	Model model;
	model.w.resize(data.nfeatures);
	model.alpha.resize(data.n());

	double M_bar = infty;
	double m_bar = -infty;

	std::vector<int> A(data.n());
	std::vector<int> Anew(data.n());
	std::iota(A.begin(), A.end(), 0);

	for (int iter=0; iter<max_iter; iter++) {
		double M = -infty;
		double m = infty;

		std::shuffle(A.begin(), A.end(), generator);
		Anew.clear();

		for (int i: A) {
			int y = data.label(i);

			// Compute gradient
			double g = 0;
			for (auto entry: data.data[i]) 
				g += model.w[entry.index] * entry.value;
			g = g * y - 1;

			// Compute projected gradient && shrinkage
			double pg = 0;
			if (model.alpha[i] == 0) {
				if (g <= M_bar) Anew.push_back(i);
				if (g<0) pg = g;
			} else if (model.alpha[i] == C) {
				if (g >= m_bar) Anew.push_back(i);
				if (g>0) pg = g;
			} else {
				Anew.push_back(i);
				pg = g;
			}

			// Update M
			M = std::max(M, pg);
			m = std::min(m, pg);

			// Update alpha, w
			if (pg != 0) {
				double old_alpha = model.alpha[i];
				model.alpha[i] = std::min(std::max(old_alpha - g/q_diag[i], 0.0), C);
				double num = (model.alpha[i] - old_alpha) * y;

				for (auto entry: data.data[i])
					model.w[entry.index] += num * entry.value;
			}
		}

		// Check convergence
		if (M - m < epsilon) {
			if (A.size() == data.data.size())
				break;
			else {
				A.resize(data.n());
				std::iota(A.begin(), A.end(), 0);
				M_bar = infty;
				m_bar = -infty;
			}
		} else {
			A = Anew;
		}

		// Update M bar
		M_bar = M<=0 ? infty : M;
		m_bar = m>=0 ? -infty : m;

		if (verbose) {
			int nSV = std::count_if(model.alpha.begin(), model.alpha.end(), 
						[&](double alpha){ return alpha>0 && alpha<C; });

			// 1/2 ||w||^2 + C \sum_i max(0, 1 - yw^T x)
			double primal_f = 0.5 * std::inner_product(model.w.begin(), model.w.end(),
								   model.w.begin(), 0.0);
			for (int i=0; i<data.n(); i++) {
				double pred = 0;
				for (auto entry: data.data[i]) pred += model.w[entry.index] * entry.value;
				primal_f += std::max(0., 1 - data.label(i) * pred);
			}

			// 1/2 alpha^T Q alpha - e^T alpha = 1/2 ||w||^2 - e^T alpha
			double dual_f = 0.5 * std::inner_product(model.w.begin(), model.w.end(),
								   model.w.begin(), 0.0);
			dual_f -= std::accumulate(model.alpha.begin(), model.alpha.end(), 0.0);

			LI << "Iteration " << iter << "\tnSV = " << nSV << " primal f = " << primal_f
			     << "\tdual f = " << dual_f << "\tactive set size = " << A.size();
		}
	}

	return model;
}
