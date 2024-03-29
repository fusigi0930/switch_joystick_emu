#include "crpijoyemu.h"
#include <QtGamepad/QGamepad>
#include <QList>
#include <QLoggingCategory>
#include <iostream>
#include <chrono>
#include <QTcpSocket>
#include <QFile>

#include <QDebug>

#define AUP     0
#define ADOWN   1
#define ALEFT   2
#define ARIGHT  3

#define AFLAG_MODIFY(d, a, v) \
    ( \
        v == true ? (d |= (1 << a)) : (d &= ~(1 << a)) \
    )

#define FLOAT_TO_AAXIS(v) static_cast<int8_t>(v * 126.0)

#define DUMP_JSEMU(d) qDebug("%02x %02x %02x %02x %02x %02x %02x %02x %02x", \
    d[0], d[1], d[2], d[3], \
    d[4], d[5], d[6], d[7], d[8])

#define SYNC_DURATION 25
//#define SYNC_DATA \
//    m_mutex.unlock(); \
//    std::this_thread::sleep_for(std::chrono::milliseconds(SYNC_DURATION)); \
//    m_mutex.lock();

#define SYNC_DATA

CRpiJoyEmu::CRpiJoyEmu(QObject *parent) : QObject(parent) {
    m_gamdpad = nullptr;
    m_axis = 0;
    m_lx = m_ly = m_rx = m_ry = 0.0;
    m_thread = nullptr;
    m_quitThread = false;
    m_bRecordJSEvent = false;
    m_bQuitEventThread = false;
    m_szEventFile.clear();
    m_threadRunEvent = nullptr;
    m_timer.setSingleShot(true);
    connect(&m_timer, &QTimer::timeout, this, &CRpiJoyEmu::timerResetJS);
}

CRpiJoyEmu::~CRpiJoyEmu() {
    stop();
    stopRunEvent();

    if (nullptr != m_gamdpad) {
        delete m_gamdpad;
        m_gamdpad = nullptr;
    }
}

void CRpiJoyEmu::timerResetJS() {
    resetReportData();
}

void CRpiJoyEmu::axis(int val) {
    if (0 > val) val = 0;
    if (12 < val) val = 12;

    uint8_t table[12] = {
        AX_NONE,        // 0
        AX_UP,          // AUP, 1
        AX_DOWN,        // ADOWN, 2
        AX_NONE,        // noway, 3
        AX_LEFT,        // ALEFT, 4
        AX_UPLEFT,      // AUP + ALEFT, 5
        AX_DOWNLEFT,    // ADOWN + ALEFT, 6
        AX_NONE,        // noway, 7
        AX_RIGHT,       // ARIGHT, 8
        AX_UPRIGHT,     // AUP + ARIGHT, 9
        AX_DOWNRIGHT,   // ADOWN + ARIGHT, 10
        AX_NONE,        // noway, 11
    };

    SET_AXIS(m_jsemuData, table[val]);

    SYNC_DATA;
}

void CRpiJoyEmu::aaxis(double lx, double ly, double rx, double ry) {
    AAXIS(m_jsemuData, L_AAXIS_X, FLOAT_TO_AAXIS(lx));
    AAXIS(m_jsemuData, L_AAXIS_Y, FLOAT_TO_AAXIS(ly));
    AAXIS(m_jsemuData, R_AAXIS_X, FLOAT_TO_AAXIS(rx));
    AAXIS(m_jsemuData, R_AAXIS_Y, FLOAT_TO_AAXIS(ry));

    SYNC_DATA;
}

void CRpiJoyEmu::resetReportData() {
    memset(m_jsemuData, 0, sizeof(m_jsemuData));
    axis(0);
    aaxis(0.0, 0.0, 0.0, 0.0);
}

void CRpiJoyEmu::slotButtonUp(bool press) {
    AFLAG_MODIFY(m_axis, AUP, press);

    axis(m_axis);
}

void CRpiJoyEmu::slotButtonDown(bool press) {
    AFLAG_MODIFY(m_axis, ADOWN, press);

    axis(m_axis);
}

void CRpiJoyEmu::slotButtonLeft(bool press) {
    AFLAG_MODIFY(m_axis, ALEFT, press);

    axis(m_axis);
}

void CRpiJoyEmu::slotButtonRight(bool press) {
    AFLAG_MODIFY(m_axis, ARIGHT, press);

    axis(m_axis);
}

void CRpiJoyEmu::slotButtonMinus(bool press) {
    true == press ? SET_BUTTON(m_jsemuData, 1, BTN_M) : UNSET_BUTTON(m_jsemuData, 1, BTN_M);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonPlus(bool press) {
    true == press ? SET_BUTTON(m_jsemuData, 1, BTN_P) : UNSET_BUTTON(m_jsemuData, 1, BTN_P);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonA(bool press) {
    true == press ? SET_BUTTON(m_jsemuData, 0, BTN_A) : UNSET_BUTTON(m_jsemuData, 0, BTN_A);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonB(bool press) {
    true == press ? SET_BUTTON(m_jsemuData, 0, BTN_B) : UNSET_BUTTON(m_jsemuData, 0, BTN_B);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonX(bool press) {
    true == press ? SET_BUTTON(m_jsemuData, 0, BTN_X) : UNSET_BUTTON(m_jsemuData, 0, BTN_X);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonY(bool press) {
    true == press ? SET_BUTTON(m_jsemuData, 0, BTN_Y) : UNSET_BUTTON(m_jsemuData, 0, BTN_Y);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonL(bool press) {
    true == press ? SET_BUTTON(m_jsemuData, 0, BTN_L) : UNSET_BUTTON(m_jsemuData, 0, BTN_L);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonR(bool press) {
    true == press ? SET_BUTTON(m_jsemuData, 0, BTN_R) : UNSET_BUTTON(m_jsemuData, 0, BTN_R);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonZL(double value) {
    0.91 < value ? SET_BUTTON(m_jsemuData, 0, BTN_ZL) : UNSET_BUTTON(m_jsemuData, 0, BTN_ZL);

    SYNC_DATA;
}

void CRpiJoyEmu::slotButtonZR(double value) {
    0.91 < value ? SET_BUTTON(m_jsemuData, 0, BTN_ZR) : UNSET_BUTTON(m_jsemuData, 0, BTN_ZR);

    SYNC_DATA;
}

void CRpiJoyEmu::slotAxisLX(double value) {
    m_lx = value;
    aaxis(m_lx, m_ly, m_rx, m_ry);
}

void CRpiJoyEmu::slotAxisLY(double value) {
    m_ly = value;
    aaxis(m_lx, m_ly, m_rx, m_ry);
}

void CRpiJoyEmu::slotAxisRX(double value) {
    m_rx = value;
    aaxis(m_lx, m_ly, m_rx, m_ry);
}

void CRpiJoyEmu::slotAxisRY(double value) {
    m_ry = value;
    aaxis(m_lx, m_ly, m_rx, m_ry);
}

bool CRpiJoyEmu::init() {
    QLoggingCategory::setFilterRules(QStringLiteral("qt.gamepad.debug=true"));

    QGamepadManager *gpad_manager = QGamepadManager::instance();
    QList<int> gpads = gpad_manager->connectedGamepads();

    if (gpads.isEmpty()) {
        return false;
    }

    m_gamdpad = new QGamepad(*gpads.begin(), this);

    //m_mutex.lock();
    resetReportData();

    connect(m_gamdpad, &QGamepad::axisLeftXChanged, this, &CRpiJoyEmu::slotAxisLX);
    connect(m_gamdpad, &QGamepad::axisLeftYChanged, this, &CRpiJoyEmu::slotAxisLY);
    connect(m_gamdpad, &QGamepad::axisRightXChanged, this, &CRpiJoyEmu::slotAxisRX);
    connect(m_gamdpad, &QGamepad::axisRightYChanged, this, &CRpiJoyEmu::slotAxisRY);
    connect(m_gamdpad, &QGamepad::buttonAChanged, this, &CRpiJoyEmu::slotButtonB);
    connect(m_gamdpad, &QGamepad::buttonBChanged, this, &CRpiJoyEmu::slotButtonA);
    connect(m_gamdpad, &QGamepad::buttonXChanged, this, &CRpiJoyEmu::slotButtonY);
    connect(m_gamdpad, &QGamepad::buttonYChanged, this, &CRpiJoyEmu::slotButtonX);
    connect(m_gamdpad, &QGamepad::buttonL1Changed, this, &CRpiJoyEmu::slotButtonL);
    connect(m_gamdpad, &QGamepad::buttonR1Changed, this, &CRpiJoyEmu::slotButtonR);
    connect(m_gamdpad, &QGamepad::buttonL2Changed, this, &CRpiJoyEmu::slotButtonZL);
    connect(m_gamdpad, &QGamepad::buttonR2Changed, this, &CRpiJoyEmu::slotButtonZR);
    connect(m_gamdpad, &QGamepad::buttonSelectChanged, this, &CRpiJoyEmu::slotButtonMinus);
    connect(m_gamdpad, &QGamepad::buttonStartChanged, this, &CRpiJoyEmu::slotButtonPlus);
    connect(m_gamdpad, &QGamepad::buttonUpChanged, this, &CRpiJoyEmu::slotButtonUp);
    connect(m_gamdpad, &QGamepad::buttonDownChanged, this, &CRpiJoyEmu::slotButtonDown);
    connect(m_gamdpad, &QGamepad::buttonLeftChanged, this, &CRpiJoyEmu::slotButtonLeft);
    connect(m_gamdpad, &QGamepad::buttonRightChanged, this, &CRpiJoyEmu::slotButtonRight);

    return true;
}

void CRpiJoyEmu::sendEmuData(QTcpSocket &socket) {
    if (m_bRecordJSEvent) {
        SRecordEvent event;
        //event.time = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count());
        memcpy(event.data, m_jsemuData, REPORT_LENG);
        m_vtRecordData.push_back(event);
    }

    m_jsemuData[8] = QT_CHECKSUM(m_jsemuData);
    m_mutex.lock();
    socket.write(reinterpret_cast<char*>(m_jsemuData), sizeof(m_jsemuData));
    socket.flush();

    m_mutex.unlock();
}

void CRpiJoyEmu::start() {
    if (nullptr != m_thread)
        return;

    m_thread = new std::thread([](CRpiJoyEmu *joyemu)->void {
        if (nullptr == joyemu)
            return;

        if (joyemu->m_ipAddr.isEmpty()) {
            qDebug("ip addr is null");
            return;
        }

        QTcpSocket socket;
        socket.connectToHost(joyemu->m_ipAddr, QT_APP_TRANS_PORT);

        if (!socket.waitForConnected() || QTcpSocket::ConnectedState != socket.state()) {
            qDebug("connect to host timout");
            return;
        }

        qDebug("start sending thread\n");
        while (false == joyemu->m_quitThread) {
            std::this_thread::sleep_for(std::chrono::milliseconds(SYNC_DURATION));
            joyemu->sendEmuData(socket);
        }

        qDebug("quit sending thread\n");
        socket.disconnectFromHost();
        socket.close();
    }, this);

}

void CRpiJoyEmu::stop() {
    if (nullptr != m_thread) {
        m_quitThread = true;
        m_mutex.unlock();

        m_thread->join();
        delete m_thread;
        m_thread = nullptr;
        m_quitThread = false;
    }
}

void CRpiJoyEmu::setIp(QString ip) {
    m_ipAddr = ip;
}

void CRpiJoyEmu::setRecord(bool enable) {
    bool bOri = m_bRecordJSEvent;
    m_bRecordJSEvent = enable;
    if (enable) {
        m_vtRecordData.clear();
    }
    else if (true == bOri && false == enable) {
        QFile file(m_szEventFile);
        if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Unbuffered)) {
            file.write(reinterpret_cast<char*>(&m_vtRecordData[0]), m_vtRecordData.size() * sizeof(SRecordEvent));
            file.flush();
            file.close();
        }
    }
}

void CRpiJoyEmu::runEvents(QList<QString> files) {
    if (nullptr != m_threadRunEvent)
        return;

    if (files.isEmpty())
        return;

    m_eventFiles.clear();
    m_eventFiles = files;

    m_threadRunEvent = new std::thread([](CRpiJoyEmu *joyemu)->void {
        QList<QString>::iterator file = joyemu->m_eventFiles.begin();
        while (false == joyemu->m_bQuitEventThread) {
            QFile f(*file);
            if (!f.open(QIODevice::ReadOnly)) {
                qDebug("open file error");
                return;
            }
            size_t leng = static_cast<size_t>(f.size());
            uint8_t *buff = new uint8_t[leng];

            f.read(reinterpret_cast<char*>(buff), static_cast<qint64>(leng));
            f.close();
            SRecordEvent *event = reinterpret_cast<SRecordEvent *>(buff);
            uint8_t *end_buff = buff + leng;

            std::this_thread::sleep_for(std::chrono::milliseconds(300));

            while (false == joyemu->m_bQuitEventThread) {
                //if (reinterpret_cast<size_t>(event) >= reinterpret_cast<size_t>(end_buff)) {
                //    std::this_thread::sleep_for(std::chrono::milliseconds(300));
                //    event = reinterpret_cast<SRecordEvent*>(buff);
                //}

                SRecordEvent *next = event + 1;
                if (reinterpret_cast<size_t>(next) >= reinterpret_cast<size_t>(end_buff)) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(SYNC_DURATION * 10));
                }
                else {
                    std::this_thread::sleep_for(std::chrono::milliseconds(SYNC_DURATION));
                }

                memcpy(joyemu->m_jsemuData, event->data, REPORT_LENG);
                event++;
            }

            delete [] buff;
            file++;
            if (joyemu->m_eventFiles.end() == file) {
                file = joyemu->m_eventFiles.begin();
            }
        }

    }, this);
}

void CRpiJoyEmu::stopRunEvent() {
    if (nullptr != m_threadRunEvent) {
        m_bQuitEventThread = true;
        m_threadRunEvent->join();
        delete m_threadRunEvent;
        m_threadRunEvent = nullptr;
        m_bQuitEventThread = false;
    }
}

void CRpiJoyEmu::setRecordFileName(QString filename) {
    m_szEventFile = filename;
    if ("file:" == m_szEventFile.left(5)) {
        m_szEventFile = m_szEventFile.mid(8);
    }
    if (".bin" != m_szEventFile.right(4)) {
        m_szEventFile.append(".bin");
    }
    qDebug() << "set filename: " << m_szEventFile;
}
