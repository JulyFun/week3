#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;


void on_mouse(int event, int x, int y, int flags, void* param);

Point pt[4];
Mat roi, img;
Mat src;

int main() {
    src = imread("car.jpg", 1);
    // imshow("ori", src);
    namedWindow("signroi");
    imshow("signroi", src);
    setMouseCallback("signroi", on_mouse, 0);
    waitKey();
    return 0;
}

int cnt = 0;

void doPerspectiveTransform(Mat input, Mat& output, Point* pt) {
    std::vector<Point2f> srcQuad(4), dstQuad(4);
    output = input.clone();
    srcQuad[0].x = pt[0].x, srcQuad[1].x = pt[1].x, srcQuad[2].x = pt[2].x, srcQuad[3].x = pt[3].x;
    srcQuad[0].y = pt[0].y, srcQuad[1].y = pt[1].y, srcQuad[2].y = pt[2].y, srcQuad[3].y = pt[3].y;
    // src是原图中要处理的四边形

    dstQuad[0].x = 0, dstQuad[1].x = 0, dstQuad[2].x = 600, dstQuad[3].x = 600;
    dstQuad[0].y = 0, dstQuad[1].y = 200, dstQuad[2].y = 200, dstQuad[3].y = 0;
    // dst就是最终目标图像方框
    // x是宽，y是高，上下左右相对位置必须和上面的四边形一致

    Mat warp_matrix = getPerspectiveTransform(srcQuad, dstQuad);
    // 这个mat才是[变换矩阵]

    warpPerspective(input, output, warp_matrix, Size(600, 200));
    // 上面的这个size必须和dst大小一致
    // 上面的这个size也是最终生成图像大小
}

void on_mouse(int event, int x, int y, int flags, void* param) {
    img = src.clone();
    switch (event) {
    case EVENT_LBUTTONUP: {
        pt[cnt].x = x;
        pt[cnt].y = y;
        
        namedWindow("signroi");
        for (int i = 0; i <= cnt; ++i) {
            circle(img, pt[i], 5, Scalar(0, 0, 255), -1);
        }
        imshow("signroi", img);
        if (cnt == 0) {
            destroyWindow("result");
        }
        if (cnt == 3) {
            namedWindow("result");
            Mat res = img.clone();
            doPerspectiveTransform(src, res, pt);
            imshow("result", res);
            cnt = 0;
        }
        else
            ++cnt;
    }
    }
}
