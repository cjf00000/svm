#ifndef __DATA
#define __DATA

#include <vector>

struct Entry {
	int index;
	double value;
};

class Data {
	public:
		void load(const char *fileName, double bias);

		int n() const{
			return data.size();
		}

		int l() const {
			return nlabels;
		}

		virtual int label(int n) const {
			return labels[n];
		}

		int multLabel(int n) const {
			return labels[n];
		}

	public:
		std::vector<std::vector<Entry> > data;
		int nfeatures, nlabels;

	protected:
		std::vector<int> labels;
};

class BinaryData : public Data{
	public:
		void setLabel(int which_label) {
			this->which_label = which_label;
		}

		virtual int label(int n) const { return labels[n]==which_label ? 1 : -1; }

	private:
		int which_label;
};

#endif
