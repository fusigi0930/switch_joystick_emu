#ifndef CRPIJOYEMU_H
#define CRPIJOYEMU_H

#include <QObject>
#include <QTcpSocket>
#include "js_def.h"

#include <thread>
#include <mutex>

QT_BEGIN_NAMESPACE
class QGamepad;
QT_END_NAMESPACE

class CRpiJoyEmu : public QObject
{
    Q_OBJECT
    QGamepad *m_gamdpad;
    uint8_t m_jsemuData[REPORT_LENG];
    uint8_t m_axis;

    double m_lx, m_ly, m_rx, m_ry;

    std::thread *m_thread;
    std::mutex m_mutex;
    bool m_quitThread;
    QString m_ipAddr;

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
