#include "dialog.h"
#include <QApplication>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include "include/easypr.h"
#include "include/easypr/util/switch.hpp"
#include "include/easypr/core/plate.hpp"
#include <QDebug>
#include <clientwidget.h>
#include <serverwidget.h>

using namespace easypr;
using namespace cv;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClientWidget c;
    c.show();
    ServerWidget s;
    s.show();

    return a.exec();
}
