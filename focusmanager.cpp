#include "focusmanager.h"

FocusManager::FocusManager(QObject *parent)
    : QObject(parent)
    , m_hasFocusedWindow(false)
{
    qDebug() << "焦点检测器已启动 - " << getCurrentTimeString();
}

void FocusManager::recordFocusGained(const QString &windowId)
{
    QString timestamp = getCurrentTimeString();
    QDateTime currentTime = QDateTime::currentDateTime();
    
    // 检查是否是真正的焦点切换（不是重复获得焦点）
    bool isRealSwitch = false;
    
    if (!m_hasFocusedWindow) {
        // 之前没有窗口有焦点，现在获得焦点
        isRealSwitch = true;
    } else if (m_lastFocusedWindowId != windowId) {
        // 焦点从一个窗口切换到另一个窗口
        isRealSwitch = true;
    }
    
    // 如果是真正的焦点切换且之前有失去焦点的记录
    if (isRealSwitch && !m_lastFocusLostTime.isNull()) {
        qint64 timeDiff = calculateTimeDifference(m_lastFocusLostTime, currentTime);
        QString timeDiffText = formatTimeDifference(timeDiff);
        
        // 在日志中输出时间差
        qDebug() << QString("[%1] 窗口 %2 获得焦点 - 焦点切换耗时: %3 (%4毫秒)")
                    .arg(timestamp, windowId, timeDiffText).arg(timeDiff);
        
        // 发出时间差信号
        emit focusSwitchTimeDiff(windowId, timeDiff, timeDiffText);
    } else {
        // 普通的焦点获得日志
        logToConsole(windowId, "获得焦点", timestamp);
    }
    
    // 更新状态
    m_lastFocusedWindowId = windowId;
    m_hasFocusedWindow = true;
    
    emit focusChanged(windowId, true, timestamp);
}

void FocusManager::recordFocusLost(const QString &windowId)
{
    QString timestamp = getCurrentTimeString();
    QDateTime currentTime = QDateTime::currentDateTime();
    
    logToConsole(windowId, "失去焦点", timestamp);
    
    // 记录失去焦点的时间，用于计算下次获得焦点的时间差
    m_lastFocusLostTime = currentTime;
    m_hasFocusedWindow = false;
    
    emit focusChanged(windowId, false, timestamp);
}

QString FocusManager::getCurrentTimeString()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    // 返回精确到毫秒的时间字符串
    return currentTime.toString("yyyy-MM-dd hh:mm:ss.zzz");
}

void FocusManager::logToConsole(const QString &windowId, const QString &action, const QString &timestamp)
{
    qDebug() << QString("[%1] 窗口 %2 %3").arg(timestamp, windowId, action);
}

qint64 FocusManager::calculateTimeDifference(const QDateTime &fromTime, const QDateTime &toTime)
{
    return fromTime.msecsTo(toTime);
}

QString FocusManager::formatTimeDifference(qint64 milliseconds)
{
    if (milliseconds < 1000) {
        return QString("%1毫秒").arg(milliseconds);
    } else if (milliseconds < 60000) {
        double seconds = milliseconds / 1000.0;
        return QString("%1秒").arg(seconds, 0, 'f', 2);
    } else {
        qint64 minutes = milliseconds / 60000;
        qint64 remainingMs = milliseconds % 60000;
        double seconds = remainingMs / 1000.0;
        return QString("%1分%2秒").arg(minutes).arg(seconds, 0, 'f', 2);
    }
} 