#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <string>

using namespace cv;
using namespace std;

int main()
{
    Mat img0 = imread("./image.png", IMREAD_COLOR);
    Mat img_gray;
    cvtColor(img0, img_gray, COLOR_RGB2GRAY);

    Mat img_hsv;
    cvtColor(img0, img_hsv, COLOR_RGB2HSV);

    Mat img_med;
    medianBlur(img_gray, img_med, 5);

    Mat img_Canny;
    Canny(img_med, img_Canny, 0, 255);

    vector<vector<Point>> contours;
    findContours(img_Canny, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat mask = Mat::zeros(img0.rows, img0.cols, CV_8UC1);
    drawContours(mask, contours, -1, Scalar(255), FILLED);

    RotatedRect rot_rect1 = minAreaRect(contours[0]);
    Point center1 = rot_rect1.center;

    // Ищем все окружности на картинке
    vector<Vec3f> circles;
    HoughCircles(mask, circles, HOUGH_GRADIENT, 1,
        mask.rows / 32,  // change this value to detect circles with different distances to each other
        10, 21, 0, 0 // change the last two parameters
   // (min_radius & max_radius) to detect larger circles
    );

    //Наносим точки, окружности и координаты на изначальную картинку
    for (size_t i = 0; i < circles.size(); i++)
    {
        Vec3i c = circles[i];
        Point center = Point(c[0], c[1]);
        // circle center
        circle(img0, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
        // circle outline
        int radius = c[2];
        circle(img0, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
        int x = c[0];
        int y = c[1];
        string coordinates = to_string(x) + "," + to_string(y);
        center.x += 10;
        center.y -= 10;
        putText(img0, coordinates, center, 4, 0.5, (0, 250, 0));
    }

    // Вывод результата работы
    namedWindow("Display window", WINDOW_AUTOSIZE);
    imshow("Display window", img0);
    waitKey(0);

    return 0;
}