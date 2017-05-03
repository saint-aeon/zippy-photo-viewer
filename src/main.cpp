#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickImageProvider>
#include <QQmlContext>
#include <QQuickTextureFactory>
#include <QQuickImageResponse>
#include <QQuickTextureFactory>
#include <QSharedPointer>
#include <QAbstractItemModel>
#include <QHash>
#include <QObject>
#include <QQuickWindow>
#include <QAbstractNativeEventFilter>

#include <quazipfile.h>

#include "zip/archive.hpp"
#include "zipimageprovider.hpp"
#include "zipitemmodel.hpp"

class EventFilter : public QAbstractNativeEventFilter
{
public:
    bool nativeEventFilter(const QByteArray &eventType, void *message, long* result) override
    {
        if (eventType == "windows_generic_MSG")
        {
            MSG* msg = reinterpret_cast<MSG*>(message);
            switch (msg->message)
            {
            case WM_NCCALCSIZE:
                *result = 0;
                return true;
            }
        }

        return false;
    }
};


int main(int argc, char *argv[])
{
    QString zipFile = "";
    if (argc > 1)
    {
        zipFile = argv[1];
    }

//    QString zipFile = "C:/Users/d10dd/Desktop/Test1.zip";
    auto archive = QSharedPointer<Zip::Archive>::create(zipFile);
    ZipImageProvider provider(archive);
    ZipItemModel model(archive);


    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;
    EventFilter eventFilter;
    app.installNativeEventFilter(&eventFilter);

    engine.rootContext()->setContextProperty("archive", archive.data());
    engine.rootContext()->setContextProperty("myModel", &model);
    engine.addImageProvider(QLatin1String("zipimageprovider"), &provider);
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    QQuickWindow* window;
    window = qobject_cast<QQuickWindow*>(engine.rootObjects().value(0));
    window->show();



    return app.exec();
}

// OPENING PASSWORD PROTECTED FILES
//    QuaZip zip("C:/Users/d10dd/Desktop/Test3.zip");
//    bool res = zip.open(QuaZip::mdUnzip);
//    qDebug() << res;
//    zip.goToFirstFile();

//    QuaZipFile file(&zip);
//    QuaZipFileInfo info{};
//    file.getFileInfo(&info);
//    qDebug() << info.flags;
//    bool res2 = file.open(QIODevice::ReadOnly, "password");
//    qDebug() << res2;
//    auto data = file.readAll();
//    qDebug() << data.size();
//    file.close();
//    zip.close();

//    QFile file2("C:/Users/d10dd/Desktop/result.jpg");
//    file2.open(QIODevice::WriteOnly);
//    file2.write(data);
//    file2.close();
