#include "data.h"
#include "utils.h"
#include <fstream>
#include <sstream>
#include <string>
#include <locale>

// Label index starts from 0
// feature index starts from 1
void Data::load(const char *fileName, double bias) {
	std::ifstream fin(fileName);
	CHECK(!fin.fail());

	int offset = bias>=0 ? 0 : 1;
	nfeatures = 0;
	nlabels = 0;
	for (std::string line; std::getline(fin, line); ) {
		std::istringstream sin(line);

		int label;
		sin >> label;

		labels.push_back(label);
		nlabels = std::max(label+1, nlabels);

		std::vector<Entry> doc;
		int index;
		double value;

		if (bias >= 0) 
			doc.push_back( Entry{0, bias} );

		while (sin >> index) {
			sin.ignore(1, ':');
			sin >> value;

			doc.push_back( Entry{index-offset, value} );

			nfeatures = std::max(nfeatures, index);
		}

		data.push_back(doc);
	}

	if (bias>=0)
		nfeatures++;
}
