#include "focusmanager.h"

FocusManager::FocusManager(QObject *parent)
    : QObject(parent)
{
    qDebug() << "焦点检测器已启动 - " << getCurrentTimeString();
}

void FocusManager::recordFocusGained(const QString &windowId)
{
    QString timestamp = getCurrentTimeString();
    
    // 检查是否是重复的焦点获得事件
    if (m_currentFocusedWindow == windowId) {
        qDebug() << QString("[%1] 窗口 %2 重复获得焦点，忽略此事件").arg(timestamp, windowId);
        return;
    }
    
    logToConsole(windowId, "获得焦点", timestamp);
    
    // 更新当前有焦点的窗口
    m_currentFocusedWindow = windowId;
    
    emit focusChanged(windowId, true, timestamp);
}

void FocusManager::recordFocusLost(const QString &windowId)
{
    QString timestamp = getCurrentTimeString();
    QDateTime currentTime = QDateTime::currentDateTime();
    
    logToConsole(windowId, "失去焦点", timestamp);
    
    // 更新全局最后失去焦点的时间
    m_lastFocusLostTime = currentTime;
    
    // 清空当前有焦点的窗口ID
    m_currentFocusedWindow.clear();
    
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