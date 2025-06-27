#include "focuswindow.h"
#include <QApplication>
#include <QFocusEvent>
#include <QCloseEvent>
#include <QDateTime>
#include <QDebug>
#include <QPalette>
#include <QFont>

FocusWindow::FocusWindow(QWidget *parent, const QString &windowId, bool isMainWindow, FocusManager *globalManager)
    : QWidget(parent)
    , m_windowId(windowId)
    , m_isMainWindow(isMainWindow)
    , m_hasFocus(false)
    , m_globalFocusManager(globalManager)
{
    // 设置窗口属性
    setWindowTitle(isMainWindow ? 
        QString("焦点检测器 - %1 (主窗口)").arg(windowId) : 
        QString("焦点检测器 - %1").arg(windowId));
    
    setFixedSize(450, 420);
    setFocusPolicy(Qt::StrongFocus);
    
    // 设置窗口在任务栏中显示
    setWindowFlags(Qt::Window);
    
    setupUI();
    updateBackgroundColor();
    
    // 只连接全局管理器的信号
    if (m_globalFocusManager) {
        connect(m_globalFocusManager, &FocusManager::focusChanged,
                this, &FocusWindow::onFocusChanged);
    }
    
    qDebug() << QString("窗口创建: %1 (主窗口: %2)").arg(windowId).arg(isMainWindow ? "是" : "否");
}

FocusWindow::~FocusWindow()
{
    qDebug() << QString("窗口销毁: %1").arg(m_windowId);
}

void FocusWindow::setupUI()
{
    m_mainLayout = new QVBoxLayout(this);
    m_mainLayout->setSpacing(15);
    m_mainLayout->setContentsMargins(20, 20, 20, 20);
    
    // 标题
    m_titleLabel = new QLabel("焦点状态检测", this);
    QFont titleFont = m_titleLabel->font();
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    m_titleLabel->setStyleSheet("color: #333333;");
    m_mainLayout->addWidget(m_titleLabel);
    
    // 窗口ID区域
    m_windowIdFrame = new QFrame(this);
    m_windowIdFrame->setFrameStyle(QFrame::Box);
    m_windowIdFrame->setLineWidth(1);
    m_windowIdFrame->setFixedHeight(40);
    
    QString windowIdBgColor = m_isMainWindow ? "#E8F5E8" : "#FFF3E0";
    QString windowIdBorderColor = m_isMainWindow ? "#4CAF50" : "#FF9800";
    QString windowIdTextColor = m_isMainWindow ? "#2E7D32" : "#F57C00";
    
    m_windowIdFrame->setStyleSheet(QString(
        "QFrame { background-color: %1; border: 1px solid %2; border-radius: 5px; }")
        .arg(windowIdBgColor, windowIdBorderColor));
    
    QHBoxLayout *windowIdLayout = new QHBoxLayout(m_windowIdFrame);
    m_windowIdLabel = new QLabel(QString("窗口ID: %1%2")
        .arg(m_windowId)
        .arg(m_isMainWindow ? " (主窗口)" : ""), this);
    
    QFont windowIdFont = m_windowIdLabel->font();
    windowIdFont.setBold(m_isMainWindow);
    m_windowIdLabel->setFont(windowIdFont);
    m_windowIdLabel->setStyleSheet(QString("color: %1;").arg(windowIdTextColor));
    m_windowIdLabel->setAlignment(Qt::AlignCenter);
    windowIdLayout->addWidget(m_windowIdLabel);
    
    m_mainLayout->addWidget(m_windowIdFrame);
    
    // 焦点状态指示器
    m_statusFrame = new QFrame(this);
    m_statusFrame->setFrameStyle(QFrame::Box);
    m_statusFrame->setLineWidth(1);
    m_statusFrame->setFixedHeight(50);
    m_statusFrame->setStyleSheet("border-radius: 8px;");
    
    QHBoxLayout *statusLayout = new QHBoxLayout(m_statusFrame);
    m_statusLabel = new QLabel("⚠ 窗口失去焦点", this);
    QFont statusFont = m_statusLabel->font();
    statusFont.setPointSize(12);
    statusFont.setBold(true);
    m_statusLabel->setFont(statusFont);
    m_statusLabel->setStyleSheet("color: white;");
    m_statusLabel->setAlignment(Qt::AlignCenter);
    statusLayout->addWidget(m_statusLabel);
    
    m_mainLayout->addWidget(m_statusFrame);
    
    // 时间记录区域
    m_timeGroupBox = new QGroupBox("焦点时间记录", this);
    QVBoxLayout *timeLayout = new QVBoxLayout(m_timeGroupBox);
    
    // 获得焦点时间
    QHBoxLayout *gainedLayout = new QHBoxLayout();
    QLabel *gainedLabel = new QLabel("获得焦点时间:", this);
    gainedLabel->setFixedWidth(100);
    m_focusGainedEdit = new QLineEdit(this);
    m_focusGainedEdit->setReadOnly(true);
    gainedLayout->addWidget(gainedLabel);
    gainedLayout->addWidget(m_focusGainedEdit);
    timeLayout->addLayout(gainedLayout);
    
    // 失去焦点时间
    QHBoxLayout *lostLayout = new QHBoxLayout();
    QLabel *lostLabel = new QLabel("失去焦点时间:", this);
    lostLabel->setFixedWidth(100);
    m_focusLostEdit = new QLineEdit(this);
    m_focusLostEdit->setReadOnly(true);
    lostLayout->addWidget(lostLabel);
    lostLayout->addWidget(m_focusLostEdit);
    timeLayout->addLayout(lostLayout);
    
    // 焦点切换时间差
    QHBoxLayout *timeDiffLayout = new QHBoxLayout();
    QLabel *timeDiffLabel = new QLabel("焦点切换耗时:", this);
    timeDiffLabel->setFixedWidth(100);
    m_timeDiffEdit = new QLineEdit(this);
    m_timeDiffEdit->setReadOnly(true);
    m_timeDiffEdit->setPlaceholderText("等待焦点切换...");
    m_timeDiffEdit->setStyleSheet(
        "QLineEdit {"
        "    background-color: #FFF8DC;"
        "    border: 1px solid #DDA0DD;"
        "    border-radius: 3px;"
        "    color: #8B4513;"
        "    font-weight: bold;"
        "}");
    timeDiffLayout->addWidget(timeDiffLabel);
    timeDiffLayout->addWidget(m_timeDiffEdit);
    timeLayout->addLayout(timeDiffLayout);
    
    m_mainLayout->addWidget(m_timeGroupBox);
    
    // 按钮区域
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(15);
    
    m_newWindowBtn = new QPushButton("🪟 打开新窗口", this);
    m_newWindowBtn->setFixedSize(140, 35);
    QFont btnFont = m_newWindowBtn->font();
    btnFont.setBold(true);
    m_newWindowBtn->setFont(btnFont);
    m_newWindowBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #4CAF50;"
        "    color: white;"
        "    border: 1px solid #388E3C;"
        "    border-radius: 6px;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #2196F3;"
        "}");
    
    m_closeBtn = new QPushButton(m_isMainWindow ? "❌ 退出程序" : "❌ 关闭窗口", this);
    m_closeBtn->setFixedSize(120, 35);
    m_closeBtn->setFont(btnFont);
    m_closeBtn->setStyleSheet(
        "QPushButton {"
        "    background-color: #F44336;"
        "    color: white;"
        "    border: 1px solid #C62828;"
        "    border-radius: 6px;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #D32F2F;"
        "}");
    
    connect(m_newWindowBtn, &QPushButton::clicked, this, &FocusWindow::onCreateNewWindow);
    connect(m_closeBtn, &QPushButton::clicked, this, &FocusWindow::onCloseWindow);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_newWindowBtn);
    buttonLayout->addWidget(m_closeBtn);
    buttonLayout->addStretch();
    
    m_mainLayout->addLayout(buttonLayout);
}

void FocusWindow::updateBackgroundColor()
{
    QString bgColor = m_hasFocus ? "#E3F2FD" : "#FFFFFF";  // 浅蓝色 : 白色
    QString borderColor = m_hasFocus ? "#2196F3" : "#CCCCCC";
    
    setStyleSheet(QString(
        "FocusWindow { "
        "    background-color: %1; "
        "    border: 2px solid %2; "
        "    border-radius: 10px; "
        "}")
        .arg(bgColor, borderColor));
}

void FocusWindow::updateFocusStatus()
{
    if (m_hasFocus) {
        m_statusLabel->setText("✅ 窗口获得焦点");
        m_statusFrame->setStyleSheet(
            "QFrame { "
            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
            "    stop:0 #4CAF50, stop:1 #66BB6A); "
            "    border: 1px solid #388E3C; "
            "    border-radius: 8px; "
            "}");
    } else {
        m_statusLabel->setText("⚠ 窗口失去焦点");
        m_statusFrame->setStyleSheet(
            "QFrame { "
            "    background: qlineargradient(x1:0, y1:0, x2:1, y2:0, "
            "    stop:0 #FF9800, stop:1 #FFB74D); "
            "    border: 1px solid #F57C00; "
            "    border-radius: 8px; "
            "}");
    }
}

// 焦点事件处理
void FocusWindow::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
    
    if (m_globalFocusManager) {
        m_globalFocusManager->recordFocusGained(m_windowId);
    }
}

void FocusWindow::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    
    if (m_globalFocusManager) {
        m_globalFocusManager->recordFocusLost(m_windowId);
    }
}

void FocusWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::ActivationChange) {
        bool isActive = isActiveWindow();
        if (isActive != m_hasFocus) {
            if (isActive && m_globalFocusManager) {
                m_globalFocusManager->recordFocusGained(m_windowId);
            } else if (!isActive && m_globalFocusManager) {
                m_globalFocusManager->recordFocusLost(m_windowId);
            }
        }
    }
    QWidget::changeEvent(event);
}

void FocusWindow::closeEvent(QCloseEvent *event)
{
    if (m_isMainWindow) {
        // 主窗口关闭时退出整个应用程序
        QApplication::quit();
    }
    event->accept();
}

// 槽函数实现
void FocusWindow::onCreateNewWindow()
{
    static int windowCounter = 1;
    QString newWindowId = QString("Window_%1").arg(++windowCounter);
    
    FocusWindow *newWindow = new FocusWindow(nullptr, newWindowId, false, m_globalFocusManager);
    newWindow->show();
    
    qDebug() << QString("从窗口 %1 创建新窗口 %2").arg(m_windowId, newWindowId);
}

void FocusWindow::onCloseWindow()
{
    close();
}

void FocusWindow::onFocusChanged(const QString &winId, bool hasFocus, const QString &timestamp)
{
    // 只处理自己的焦点变化事件
    if (winId != m_windowId) {
        return;
    }
    
    m_hasFocus = hasFocus;
    updateBackgroundColor();
    updateFocusStatus();
    
    if (hasFocus) {
        // 窗口获得焦点
        m_focusGainedTime = QDateTime::currentDateTime();
        m_focusGainedTimeString = timestamp;
        m_focusGainedEdit->setText(m_focusGainedTimeString);
        
        // 计算并显示时间差
        calculateAndShowTimeDiff();
    } else {
        // 窗口失去焦点
        m_focusLostTime = QDateTime::currentDateTime();
        m_focusLostTimeString = timestamp;
        m_focusLostEdit->setText(m_focusLostTimeString);
    }
}

// 时间差计算辅助方法
qint64 FocusWindow::calculateTimeDifference(const QDateTime &fromTime, const QDateTime &toTime)
{
    return fromTime.msecsTo(toTime);
}

QString FocusWindow::formatTimeDifference(qint64 milliseconds)
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

void FocusWindow::calculateAndShowTimeDiff()
{
    // 计算时间差：窗口获得焦点时间 - 全局最后失去焦点时间
    if (m_globalFocusManager && !m_focusGainedTime.isNull()) {
        QDateTime lastFocusLostTime = m_globalFocusManager->getLastFocusLostTime();
        
        if (!lastFocusLostTime.isNull()) {
            qint64 timeDiff = calculateTimeDifference(lastFocusLostTime, m_focusGainedTime);
            QString timeDiffText = formatTimeDifference(timeDiff);
            
            // 在UI中显示时间差
            m_timeDiffEdit->setText(timeDiffText);
            
            // 根据时间差设置颜色
            QString colorStyle;
            if (timeDiff < 500) {
                colorStyle = "background-color: #C8E6C9; color: #2E7D32;";  // 绿色
            } else if (timeDiff < 2000) {
                colorStyle = "background-color: #FFE0B2; color: #F57C00;";  // 橙色
            } else {
                colorStyle = "background-color: #FFCDD2; color: #C62828;";  // 红色
            }
            
            m_timeDiffEdit->setStyleSheet(QString(
                "QLineEdit {"
                "    %1"
                "    border: 1px solid #DDA0DD;"
                "    border-radius: 3px;"
                "    font-weight: bold;"
                "}").arg(colorStyle));
            
            // 在日志中输出时间差
            qDebug() << QString("[%1] 窗口 %2 焦点切换耗时: %3 (%4毫秒)")
                        .arg(m_focusGainedTimeString, m_windowId, timeDiffText).arg(timeDiff);
        } else {
            m_timeDiffEdit->setText("首次获得焦点");
            m_timeDiffEdit->setStyleSheet(
                "QLineEdit {"
                "    background-color: #E1F5FE;"
                "    border: 1px solid #DDA0DD;"
                "    border-radius: 3px;"
                "    color: #0277BD;"
                "    font-weight: bold;"
                "}");
        }
    }
}

// MOC文件会自动包含 