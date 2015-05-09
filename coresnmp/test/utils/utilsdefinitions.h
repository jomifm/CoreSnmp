#ifndef CORESIMDEFINITIONS_H
#define CORESIMDEFINITIONS_H

#include "test/utils/logger/utilslogger.h"
#if unix
#include <signal.h>
#endif

#include <QtCore/QFile>
#include <QtCore/QCoreApplication>

struct CleanExit
{
    CleanExit() {
        signal(SIGINT, &CleanExit::exitQt);
        signal(SIGTERM, &CleanExit::exitQt);
#ifdef SIGBREAK
        signal(SIGBREAK, &CleanExit::exitQt);
#endif
    }

    static void exitQt(int) {
        QCoreApplication::exit(0);
    }
};

class CoreSimDefinitions : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructor
     * @param QWidget *poParent Widget padre
     */
    CoreSimDefinitions(QObject *poParent = 0) { Q_UNUSED(poParent) }

    /**
     * Destructor
     */
    virtual ~CoreSimDefinitions() { }

signals:

public slots:

};

class MyApplication : public QCoreApplication
{
public:
    /**
     * Constructor
     * @param
     * @param
     */
    MyApplication(int& argc, char ** argv): QCoreApplication(argc, argv) { }

    /**
     * Destructor
     */
    virtual ~MyApplication() { }

    /**
     * Reimplemented from QCoreApplication so we can throw exceptions in slots
     * @param
     * @param
     * @return
     */
    virtual bool notify(QObject *poObjectReceiver, QEvent *poEvent)
    {
        try {
            return QCoreApplication::notify(poObjectReceiver, poEvent);
        }
        catch(std::exception& e) {
            LogError() << "Exception thrown:" << QString(e.what());
        }
        return false;
    }

};

#endif // CORESIMDEFINITIONS_H
