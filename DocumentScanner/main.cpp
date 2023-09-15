#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

Mat imgOrig, imgGray, imgCanny, imgThres, imgDil, imgErode, imgWarp, imgCrop, imgBlur;
vector<Point> initPoints, docPoints;

float w = 420, h = 596;

Mat preProcessing(Mat img) {
	cvtColor(img, imgGray, COLOR_BGR2GRAY);
	GaussianBlur(imgGray, imgBlur, Size(3, 3), 3, 0);
	Canny(imgBlur, imgCanny, 25, 75);

	Mat kernel = getStructuringElement(MORPH_RECT, Size(3, 3));
	dilate(imgCanny, imgDil, kernel);
	//erode(imgDil, imgErode, kernel);

	return imgDil;
}

vector<Point> getContours(Mat img) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(img, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());

	vector<Point> biggest;
	int maxArea = 0;

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);
		cout << area << "\n";

		string objType;

		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			if (area > maxArea && conPoly[i].size() == 4) {
				//drawContours(imgOrig, conPoly, i, Scalar(255, 0, 255), 5);
				biggest = { conPoly[i][0], conPoly[i][1] , conPoly[i][2] , conPoly[i][3] };
				maxArea = area;
			}

			/*drawContours(imgOrig, conPoly, i, Scalar(255, 0, 255), 2);*/
		}
	}

	return biggest;
}

void drawPoints(vector<Point> points, Scalar color) {
	for (int i = 0; i < points.size(); i++) {
		circle(imgOrig, points[i], 10, color, FILLED);
		putText(imgOrig, to_string(i), points[i], FONT_HERSHEY_PLAIN, 5, color, 5);
	}
}

vector<Point> reorder(vector<Point> points) {
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;

	for (int i = 0; i < 4; i++) {
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}

	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);  // 0
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);  // 1
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);  // 2
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);  // 3
	
	return newPoints;
}

Mat getWarp(Mat img, vector<Point> points, float w, float h) {
	Point2f src[4] = {points[0], points[1] , points[2] , points[3] };
	Point2f dst[4] = { {0.0f, 0.0f}, {w, 0.0f}, {0.0f, h}, {w, h} };

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, Point(w, h));

	return imgWarp;
}

int main() {
	string path = "Resources/doc-img.jpg";
	imgOrig = imread(path);
	//resize(imgOrig, imgOrig, Size(), 0.25, 0.25);

	// Preprocessing
	imgThres = preProcessing(imgOrig);
	
	// Get contours - biggest
	initPoints = getContours(imgThres);
	//drawPoints(initPoints, Scalar(0, 0, 255));
	docPoints = reorder(initPoints);
	//drawPoints(initPoints, Scalar(0, 255, 0));

	// Warping
	imgWarp = getWarp(imgOrig, docPoints, w, h);

	// Cropping
	int cropVal = 5;
	Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
	imgCrop = imgWarp(roi);

	imshow("Image - Original", imgOrig);
	imshow("Image - Dilated", imgThres);
	imshow("Image - Warped", imgWarp);
	imshow("Image - Cropped", imgCrop);
	waitKey(0);
	
	return 0;
}
