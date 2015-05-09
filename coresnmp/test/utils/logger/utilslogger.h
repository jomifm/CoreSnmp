#ifndef CORESIMLOGGER_H
#define CORESIMLOGGER_H

#include <QtCore/QObject>
#include <QtCore/QDebug>

#if QT_VERSION >= 0x050000
#define LogDebug   QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).debug
#define LogInfo    QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).debug
#define LogWarning QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).warning
#define LogError   QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).critical
#define LogCritical   QMessageLogger(__FILE__, __LINE__, Q_FUNC_INFO).fatal
#else
#define LogDebug   qDebug
#define LogInfo    qWarning
#define LogWarning qCritical
#define LogError   qCritical
#define qInstallMessageHandler qInstallMsgHandler
#endif

#ifdef _QT_DEBUG
#define LogFuncionBegin LogDebug() << "Begin"
#define LogFuncionEnd LogDebug() << "End"
#else
#define LogFuncionBegin ;
#define LogFuncionEnd ;
#endif

#define LogConsole(...)  printf(__VA_ARGS__)

class CoreSimLogger : public QObject
{
    Q_OBJECT
public:
    /**
     * Constructor
     * @param
     * @param
     */
    CoreSimLogger(QObject *poParent): QObject(poParent) { }

    /**
     * Destructor
     */
    virtual ~CoreSimLogger() { }

    typedef enum {
        LogLevelDebug, LogLevelInfo, LogLevelWarning, LogLevelError, LogLevelCritical
    } EnumLogLevel;

#if QT_VERSION >= 0x050000
    static void MyLogger(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
#else
    static void MyLogger(QtMsgType type, const char *str)
    {
        QString msg = QString("%1").arg(str);
#endif
        //Declaracion de Variables
        QString strLog = "";
        //EnumLogLevel level;

#if QT_VERSION >= 0x050000
        QString strFunction = context.function;
#else
        QString strFunction = ""; //QString("%1").arg(Q_FUNC_INFO);
#endif

        //Obtiene el Tipo de Log
//        if (type == QtDebugMsg) level = LogLevelDebug;
//        else if (type == QtWarningMsg) level = LogLevelInfo;
//        else if (type == QtCriticalMsg) level = LogLevelWarning;
//        else if (type == QtFatalMsg) level = LogLevelError;
//        else level = LogLevelInfo;

        //Comprueba el Nivel de Log
        //if (LogLevel == LogLevelError && level < LogLevelError) return;
        //else if (LogLevel == LogLevelWarning && level < LogLevelError) return;
        //else if (LogLevel == LogLevelInfo && level < LogLevelInfo) return;

        //Establece la Fecha y Hora
        strLog = QString("%1 %2 ").arg(__DATE__).arg(__TIME__);

        switch (type) {
        case QtDebugMsg:
            strLog += QString ("[Debug] %1 %2").arg(strFunction).arg(msg);
            break;

        case QtWarningMsg:
            strLog += QString ("[Info ] %1 %2").arg(strFunction).arg(msg);
            break;

        case QtCriticalMsg:
            strLog += QString ("[Warn ] %1 %2").arg(strFunction).arg(msg);
            break;

        case QtFatalMsg:
            strLog += QString ("[Error] %1 %2").arg(strFunction).arg(msg);
            abort();
        }

        /*QFile outFile("debuglog.txt");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << strLog << endl;*/

        LogConsole("%s\n", strLog.toStdString().c_str());

        fflush(stdout);
    }
};

#endif // CORESIMLOGGER_H
