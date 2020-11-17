#ifndef Q_INCLUDES_H_
#define Q_INCLUDES_H_

// C INCLUDES
#include <fcntl.h>
#include <unistd.h>

// C++ INCLUDES
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>

// QT INCLUDES
#include <qmainwindow.h>
#include <QImage>
#include <QObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QWidget>
#include <QString>
#include <QtMultimedia/QMediaPlayer>
#include <QtMultimedia/QMediaPlaylist>
#include <QUrl>
#include <QTreeWidget>
#include <QStringList>
#include <QInputDialog>

// WARN
#include <QDebug>

// TAGLIB
#include "../../3dparty/lib1/id3v2tag.h"
#include "../../3dparty/lib1/mpegfile.h"
#include "../../3dparty/lib1/id3v2frame.h"
#include "../../3dparty/lib1/id3v2header.h"
#include "../../3dparty/lib1/attachedpictureframe.h"
#include "../../3dparty/lib1/tag.h"
#include "../../3dparty/lib1/tfile.h"
#include "../../3dparty/lib1/trueaudiofile.h"
#include "../../3dparty/lib1/fileref.h"

// RapidJSON
#include "../../3dparty/rapidjson/document.h"
#include "../../3dparty/rapidjson/stringbuffer.h"
#include "../../3dparty/rapidjson/prettywriter.h"

#endif // Q_INCLUDES_H_
