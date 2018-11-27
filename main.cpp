#include <QCoreApplication>
#include <QJSEngine>
#include <QQmlEngine>
#include <QString>
#include "generic_interface.h"
#include "qt_jse_plugin_global.h"
#include <dlfcn.h>
#include <QDebug>
#include <QCommandLineParser>
//typedef int (*function_handle)(int);
typedef Generic_Interface * (*function_handle)(void);
int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QCoreApplication::setApplicationName(argv[0]);
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("load dynamic library");
    //QString plugin_path
    
    QCommandLineOption libraryOption(QLatin1String("plugin"),
                                    QLatin1String("pluggin file path."),
                                    QLatin1String("pluggin"));
    parser.addOption(libraryOption);

    parser.process(app);

    QString plugin_path = parser.value(libraryOption);
    
    void* handle = dlopen( plugin_path.toStdString().c_str(), RTLD_LAZY);
    
    if(handle == nullptr)
    {
        exit(-1);
    }
    QJSEngine engine;
    
    qDebug() << plugin_path;
    function_handle pluginIfc = (function_handle)dlsym(handle, "CreatePlugin");
    Generic_Interface *commonClass = (Generic_Interface*)pluginIfc();\
    commonClass->Abstract(&engine);
    //return app.exec();
}

