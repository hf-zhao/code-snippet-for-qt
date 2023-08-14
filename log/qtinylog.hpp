#ifndef QTINYLOG_HPP
#define QTINYLOG_HPP
#include <QtGlobal>
#include <QDateTime>
#include <QTextStream>
#include <QThread>
#include <QMutex>
/**
 * @file qtinylog.hpp
 * @brief a tiny log output
 *
 * @details Through the custom log output, it is convenient to realize the program debugging
 *
 * @date 2023-08-13 12:16
 *
 */

QMutex mtx;
QTextStream out(stdout);
QHash<Qt::HANDLE, int> threadList;
int threadId = 1;

/**
 * @brief Colorful log
 *
 */
void defaultLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    out.setEncoding(QStringConverter::System);
    // timestamp
    QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
    switch (type) {
    case QtDebugMsg:
        out << QString("\033[34m[%1][DEBUG] \"%2<%3>%4\": %5")
                   .arg(timestamp, context.file, QString::number(context.line), context.function, msg) << "\033[0m\n";
        break;
    case QtInfoMsg:
        out << QString("\033[32m[%1][INFO] \"%2<%3>%4\": %5")
                   .arg(timestamp, context.file, QString::number(context.line), context.function, msg) << "\033[0m\n";
        break;
    case QtWarningMsg:
        out << QString("\033[33m[%1][WARNING] \"%2<%3>%4\": %5")
                   .arg(timestamp, context.file, QString::number(context.line), context.function, msg) << "\033[0m\n";
        break;
    case QtCriticalMsg:
        out << QString("\033[38;5;208m[%1][CRITICAL] \"%2<%3>%4\": %5")
                   .arg(timestamp, context.file, QString::number(context.line), context.function, msg) << "\033[0m\n";
        break;
    case QtFatalMsg:
        out << QString("\033[31m[%1][FATAL] \"%2<%3>%4\": %5")
                   .arg(timestamp, context.file, QString::number(context.line), context.function, msg) << "\033[0m\n";
        break;
    }
    out.flush();
}

/**
 * @brief Colorful thread
 *
 */
void threadLogHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(type)
    if (threadList.isEmpty()) {
        threadList.insert(QThread::currentThreadId(), ++threadId);
    }
    if (!threadList.contains(QThread::currentThreadId())) {
        threadList.insert(QThread::currentThreadId(), ++threadId);
    }
    QString timestamp = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz"); // timestamp
    mtx.lock();
    out.setEncoding(QStringConverter::System);
    out << QString("\033[1;3%1m[%2][%3] \"%4<%5>%6\": %7")
               .arg(QString::number(threadList.value(QThread::currentThreadId())),
                    timestamp,
                    QString::number(reinterpret_cast<quintptr>(QThread::currentThreadId())),
                    context.file,
                    QString::number(context.line),
                    context.function,
                    msg) << "\033[0m\n";
    out.flush();
    mtx.unlock();
}

#endif
