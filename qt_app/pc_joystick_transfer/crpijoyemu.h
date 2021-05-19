#ifndef CRPIJOYEMU_H
#define CRPIJOYEMU_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include "js_def.h"
#include <QList>
#include <QString>

#include <thread>
#include <mutex>
#include <vector>

QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

struct SRecordEvent {
    //uint64_t time;
    uint8_t data[REPORT_LENG];
};

class CRpiJoyEmu : public QObject
{
    Q_OBJECT
    QGamepad *m_gamdpad;
    uint8_t m_jsemuData[REPORT_LENG+1];
    uint8_t m_axis;

    double m_lx, m_ly, m_rx, m_ry;

    std::thread *m_thread;
    std::mutex m_mutex;
    bool m_quitThread;
    QString m_ipAddr;
    bool m_bRecordJSEvent;
    std::vector<SRecordEvent> m_vtRecordData;

    QString m_szEventFile;
    QList<QString> m_eventFiles;
    bool m_bQuitEventThread;
    std::thread *m_threadRunEvent;

    QTimer m_timer;

public:
    explicit CRpiJoyEmu(QObject *parent = nullptr);
    virtual ~CRpiJoyEmu();

    void axis(int val);
    void aaxis(double lx, double ly, double rx, double ry);
    void resetReportData();
    void sendEmuData(QTcpSocket &socket);

signals:

public slots:
    bool init();
    void start();
    void stop();
    void setIp(QString ip);
    void setRecord(bool enable);
    void runEvents(QList<QString> files);
    void stopRunEvent();
    void timerResetJS();
    void setRecordFileName(QString filename);

    void slotButtonUp(bool press);
    void slotButtonDown(bool press);
    void slotButtonLeft(bool press);
    void slotButtonRight(bool press);
    void slotButtonPlus(bool press);
    void slotButtonMinus(bool press);
    void slotButtonA(bool press);
    void slotButtonB(bool press);
    void slotButtonX(bool press);
    void slotButtonY(bool press);
    void slotButtonL(bool press);
    void slotButtonR(bool press);
    void slotButtonZL(double value);
    void slotButtonZR(double value);
    void slotAxisLX(double value);
    void slotAxisLY(double value);
    void slotAxisRX(double value);
    void slotAxisRY(double value);
};

#endif // CRPIJOYEMU_H
