#include "recogize.h"
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "include/easypr.h"
#include "include/easypr/util/switch.hpp"
#include <QDebug>

using namespace easypr;
using namespace cv;

Recogize::Recogize()
{
}

bool Recogize::rec(QString p, QString f)
{
    QString source = p+f;
    String path = String((const char *)source.toLocal8Bit());
    cout<<path;
    Mat src = imread(path);
    CPlateRecognize pr;
    pr.setLifemode(true);
    pr.setDebug(false);
    pr.setMaxPlates(1);
    //pr.setDetectType(PR_DETECT_COLOR | PR_DETECT_SOBEL);
    pr.setDetectType(easypr::PR_DETECT_CMSER);
    //vector<string> plateVec;
    vector<CPlate> plateVec;
    int result = pr.plateRecognize(src, plateVec);
    //int result = pr.plateRecognizeAsText(src, plateVec);
    if (result == 0) {
      size_t num = plateVec.size();
      for (size_t j = 0; j < num; j++) {
        String pstr = plateVec[j].getPlateStr();
        plateStr = QString::fromLocal8Bit(pstr.c_str());
        plateColor = plateStr.section(":",0,0);
        plateNum = plateStr.section(":",1,1);
        resultPath = "E://result//"+f;
        String rpath = String((const char *)resultPath.toLocal8Bit());
        Mat plateMat = plateVec[j].getPlateMat();
        imwrite(rpath,plateMat);
      }
      return true;
    }
    if (result != 0) {
        cout << "result:" << result << endl;
        return false;
    }
}
