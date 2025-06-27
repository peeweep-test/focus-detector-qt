#ifndef FOCUSMANAGER_H
#define FOCUSMANAGER_H

#include <QObject>
#include <QDateTime>
#include <QDebug>

class FocusManager : public QObject
{
    Q_OBJECT

public:
    explicit FocusManager(QObject *parent = nullptr);

public slots:
    // 记录窗口获得焦点
    void recordFocusGained(const QString &windowId);
    
    // 记录窗口失去焦点  
    void recordFocusLost(const QString &windowId);
    
    // 获取当前精确时间字符串
    QString getCurrentTimeString();

signals:
    // 焦点状态变化信号
    void focusChanged(const QString &windowId, bool hasFocus, const QString &timestamp);
    
    // 焦点切换时间差信号（只在真正切换时发出）
    void focusSwitchTimeDiff(const QString &newWindowId, qint64 timeDiffMs, const QString &timeDiffText);

private:
    // 输出日志到控制台
    void logToConsole(const QString &windowId, const QString &action, const QString &timestamp);
    
    // 计算时间差的辅助方法
    qint64 calculateTimeDifference(const QDateTime &fromTime, const QDateTime &toTime);
    QString formatTimeDifference(qint64 milliseconds);
    
    // 记录状态
    QString m_lastFocusedWindowId;  // 最后一个有焦点的窗口ID
    QDateTime m_lastFocusLostTime;  // 最后失去焦点的时间
    bool m_hasFocusedWindow;        // 是否有窗口当前有焦点
};

#endif // FOCUSMANAGER_H 