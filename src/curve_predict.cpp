#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;

void constructX(Mat& X, double x, int count, int order) {
	double prev = 1.0;
	//std::cout<<"x:"<<x<<std::endl;
	//std::cout<<count<<" "<<order<<" ";
	for(int i=0; i<order; i++) {
		X.at<double>(count, i) = prev;
		prev = prev*x;
	}
}

void displayMatrix(Mat mat) {
	for(int i=0; i<mat.rows; i++) {
		for(int j=0; j<mat.cols; j++) {
			std::cout<<mat.at<double>(i, j)<<", ";
		}
		std::cout<<std::endl;
	}
	std::cout<<std::endl;
}

bool cmpPoint(Point2f a, Point2f b) {
	return (a.y < b.y);
}

void log_vec_pts(std::string str, std::vector<Point2f> vec) {
	std::cout<<str<<std::endl;
	for(int i=0; i<vec.size(); i++) {
		std::cout<<"("<<vec[i].x<<", "<<vec[i].y<<")"<<", ";
	}
	std::cout<<std::endl;
}

void normaliseToScale(std::vector<Point2f>& vec, double offsetX, double offsetY) {
	sort(vec.begin(), vec.end(), cmpPoint);
	int size = vec.size();
	double max_ele_Y = vec[size - 1].y, min_ele_Y = vec[0].y;
	double scaleY = 1000.0/(max_ele_Y - min_ele_Y);

	//log_vec_pts("vec-raw", vec);

	for(int i=0; i<size; i++) {
		vec[i].x = 10*scaleY*((double)vec[i].x + offsetX);
		vec[i].y = scaleY*((double)vec[i].y + offsetY);
	}

	//log_vec_pts("vec-scaled", vec);
}

void plotPoints(std::vector<Point2f> vec, int plot_rows, int plot_cols) {
	int size = vec.size();
	plot_cols *= 10;
	plot_rows *= 10;
	Mat plot(plot_rows, plot_cols, CV_8UC1, Scalar::all(0));
	std::cout<<"rows : "<<plot_rows<<" cols : "<<plot_cols<<std::endl;
	for(int i=0; i<size; i++) {
		plot.at<uchar>((int)vec[i].y, (int)vec[i].x) = 255;
		std::cout<<(int)vec[i].y<<", "<<(int)vec[i].x<<" "<<plot.at<Vec3b>((int)vec[i].y, (int)vec[i].x)[2]<<std::endl;
	}
	imshow("plot", plot);
	waitKey(0);
}
void plotGraph(std::vector<double>& vals, int XRange[2], int YRange[2]) {	
	int rows = YRange[1] - YRange[0] + 1;
	int cols = XRange[1] - XRange[0] + 1;

	std::vector<Point2f> vec(0);
	for(int i=0; i<cols; i++) {
		double sum = 0;
		double prev = 1.0;
		for(int k=0; k<vals.size(); k++) {
			sum += prev*vals[k];
			prev = prev*i;
		}
		vec.push_back(Point2f(i, sum));
	}

	//TODO : normalise sum
	normaliseToScale(vec, 0.0, cols/2.0);

	//TODO : display it in the image
	plotPoints(vec, rows, cols);
}

int main(int argc, char** argv) {

	Mat img = imread(argv[1], 0);
	//TODO : threshold the image
	int count = 0, order = 7;
	Mat F(order, 1, CV_64F, Scalar::all(0));
	Mat X(order, order, CV_64F, Scalar::all(0)), Y(order, 1, CV_64F, Scalar::all(0));
	Mat X_inv(order, order, CV_64F, Scalar::all(0));

	int rows = img.rows;
	int cols = img.cols;

	//np.where
	//np.zip
	//np.polyfit
	/*def compute(coeff, x):
    sum = 0
    prev = 1
    for i in range(0, coeff.size - 1):
        sum = sum + coeff[i]*prev
        prev = prev*x
    return sum
*/


	double scale = 10.0;

	for(int i=0; i<img.rows; ++i) {
		for(int j=0; j<img.cols; ++j) {
			if(img.at<uchar>(i, j) == 0 && count<order) {
				//std::cout<<count;
				Y.at<double>(count, 0) = (i - rows/2.0)/scale;
				constructX(X, j/scale, count, order);
				++count;
			}
		}
	}

	X_inv = X.inv();
	//displayMatrix(X);
	F = X_inv*Y;

	//std::cout<<"Coeff : "<<std::endl;
	std::vector<double> vec_coeff(0);
	int XRange[] = {0, 500};
	int YRange[] = {0, 500};

	for(int i=0; i<order; i++) {
		double coeff = F.at<double>(i, 0);
		//std::cout<<coeff<<" ";
		vec_coeff.push_back(coeff);
	}

	plotGraph(vec_coeff, XRange, YRange);

	return 0;
}