#ifndef __SOLVER
#define __SOLVER

#include <string>
#include <vector>

class Model;
class Data;
class BinaryData;
struct Entry;

class Solver {
	public:
		virtual ~Solver() {};
		virtual Model train(int max_iter = 1000) = 0;
};

struct MultiModel {
	std::vector<Model> models;
};

Solver* createSolver(double C, double epsilon, BinaryData &data, std::string type);
MultiModel train(double C, double epsilon, BinaryData &data, std::string type);
double score(const std::vector<Entry> &instance, const Model &model);
std::vector<int> predict(const Data &data, const MultiModel &mmodel);
double accuracy(const std::vector<int> &pred, const Data &data);

#endif
