#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#include "opencv2/core/core_c.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;
using namespace std;

vector<string> getFilenames(int n) {
    vector<string> filenames;
    for (int i = 1; i <= n; ++i) {
        stringstream ss;
        ss << "images/" << i << ".jpg";
        filenames.push_back(ss.str());
    }
    return filenames;
}


// don't touch this unless you exactly know what you're doing
// yes, 60 is magic number
void createArffFile(vector<vector<double> > features) {
    ofstream arff;
    arff.open("banana.arff");
    arff << "@RELATION banana" << endl << endl;

    for (size_t i = 0; i < features[0].size(); ++i) {
        arff << "@ATTRIBUTE feature" << i << " NUMERIC" << endl;
    }
    arff << "@ATTRIBUTE class {Banana, noBanana}" << endl << endl;

    arff << "@DATA" << endl;
    for (size_t i = 0; i < features.size(); ++i) {
        for (size_t j = 0; j < features[0].size(); ++j) {
            arff << features[i][j] << ",";
        }
        if (i < 60) {
            arff << "Banana" << endl;
        }
        else {
            arff << "noBanana" << endl;
        }
    }
    arff.close();
}

double getFeature2(Mat &pic) {
    std::vector<cv::Mat> rgb;
    split(pic, rgb);
    float c, m, y, k;
    double max = -1;

    const int rows((int) pic.rows);
    const int cols((int)pic.cols);
    for (int i = (int)rows - 1; i >= 0; --i) {
        for (int j = cols - 1; j >= 0; --j) {
            float r = (int)rgb[2].at<uchar>(i, j) / 255.;
            float g = (int)rgb[1].at<uchar>(i, j) / 255.;
            float b = (int)rgb[0].at<uchar>(i, j) / 255.;
            float k = std::min(std::min(1 - r, 1 - g), 1 - b);

            c = (1 - r - k) / (1 - k) * 255.0f;
            m = (1 - g - k) / (1 - k) * 255.0f;
            y = (1 - b - k) / (1 - k) * 255.0f;

            
            if (c == 0)
                c = 1;
            if (m == 0)
                m = 1;

            if (y / c + y / m > max) {
                max = y / c + y / m;
            }
        }
    }

    return max / 512;
}


int main(int argc, const char* argv[]) {
    int numOfPics = 200;
    vector<string> filenames = getFilenames(numOfPics);


    vector<vector<double> > features(numOfPics);

    int counter = 0;
    for (string filename : filenames) {
        // opening picture
        Mat currentPic = imread(filename, 1);

        // filling features data
        features[counter].push_back(getFeature2(currentPic));

        counter++; // do not erase this line:)
    }

    createArffFile(features);
}
