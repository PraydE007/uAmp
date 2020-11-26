#include <QApplication>
#include "src/mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
//    QFile File("app/res/themes/darkTheme.qss");
//    File.open(QFile::ReadOnly);
//    QString StyleSheet = QLatin1String(File.readAll());
//
//    app.setStyleSheet(StyleSheet);
    MainWindow mW;
    mW.show();
    return app.exec();
}
