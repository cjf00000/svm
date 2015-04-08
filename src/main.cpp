#include "data.h"
#include "model.h"
#include "utils.h"
#include "solver.h"

DEFINE_double(C, 1.0, "cost");
DEFINE_double(epsilon, 1e-1, "Convergence criteria");
DEFINE_double(bias, -1, "Bias, <0 means no bias");
DEFINE_string(type, "ll3", "Type of algorithm\n"
		"\tll3=L2R L1 loss SVC dual coordinate descent (liblinear");

DEFINE_string(train_file, "specify", "training file in LibSVM format");
DEFINE_string(test_file, "specify", "testing file in LibSVM format");

int main(int argc, char **argv) {
	google::InitGoogleLogging(argv[0]);
	google::ParseCommandLineFlags(&argc, &argv, false);
	FLAGS_logtostderr = true;
	FLAGS_colorlogtostderr = true;
	if (argc == 1) print_help();
	else print_flags();

	BinaryData train_data;
	train_data.load(FLAGS_train_file.c_str(), FLAGS_bias);
	LI << "Training data load from " << FLAGS_train_file << " "
	   << train_data.n() << " instances,"
	   << train_data.nfeatures << " features,"
	   << train_data.nlabels << " labels.";

	BinaryData test_data;
	test_data.load(FLAGS_test_file.c_str(), FLAGS_bias);
	LI << "Testing data load from " << FLAGS_test_file << " "
	   << test_data.n() << " instances,"
	   << test_data.nfeatures << " features,"
	   << test_data.nlabels << " labels.";

	MultiModel model = train(FLAGS_C, FLAGS_epsilon, train_data, FLAGS_type);
	auto results = predict(test_data, model);

	LI << "Accuracy: " << accuracy(results, test_data);

	return 0;
}
