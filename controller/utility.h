#ifndef UTILITY_H
#define UTILITY_H
#include <QString>
#include <QStringList>

 int GetFolderImages(const QString path, QStringList &string_list, bool sub_dir);
 int GetOneImage(const QString path, QStringList &string_list, bool sub_dir );

#endif // UTILITY_H
