#include "utility.h"

#include <QDir>
#include <QDirIterator>

/**
 * @brief GetFolderImages
 * @param path folder path
 * @param string_list
 * @param sub_dir
 * @return
 *
 * 将文件夹内所有文件路径加载到数组中
 */
int GetFolderImages(const QString path, QStringList &string_list, bool sub_dir)
{
    string_list.clear();
    int result = 0;
//    QString s = QDir::currentPath();
    QDir dir(path);
    if(!dir.exists())
    {
        return result;
    }

    QStringList filters;
    //用于设置文件名称过滤器，只为filters格式（后缀为.jpeg等图片格式）
    filters<<QString("*.jpeg")<<QString("*.jpg")<<QString("*.png")<<QString("*.tiff")<<QString("*.gif")<<QString("*.bmp")
          <<QString("*.mov")<<QString("*.mp4");

    QDirIterator *dir_iterator = NULL;
    if(sub_dir)
         //定义迭代器并设置过滤器,sub_dir若为true遍历子目录，若为false不遍历子目录。
        dir_iterator = new QDirIterator(path,filters,QDir::Files | QDir::NoSymLinks,    QDirIterator::Subdirectories);
    else
        dir_iterator = new QDirIterator(path,filters,QDir::Files | QDir::NoSymLinks);

    while(dir_iterator->hasNext())
    {
        dir_iterator->next();
        QFileInfo file_info = dir_iterator->fileInfo();
        QString absolute_file_path = file_info.absoluteFilePath();
        string_list.append(absolute_file_path);
        ++result;
    }
    if(dir_iterator!=NULL)
        delete dir_iterator;
  //该函数返回的图片路径储存在QStringList &string_list中，可取出数据调用。
    return result;
}


int GetOneImage(const QString path, QStringList &string_list, bool sub_dir)
{

}
