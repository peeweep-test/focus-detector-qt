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
    , m_localFocusManager(new FocusManager(this))
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
    
    // 连接信号
    if (m_localFocusManager) {
        connect(m_localFocusManager, &FocusManager::focusChanged,
                this, &FocusWindow::onFocusChanged);
        connect(m_localFocusManager, &FocusManager::focusSwitchTimeDiff,
                this, &FocusWindow::onFocusSwitchTimeDiff);
    }
    
    // 如果有全局管理器，也连接时间差信号
    if (m_globalFocusManager) {
        connect(m_globalFocusManager, &FocusManager::focusSwitchTimeDiff,
                this, &FocusWindow::onFocusSwitchTimeDiff);
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
    
    // 添加弹性空间
    m_mainLayout->addStretch();
}

void FocusWindow::updateBackgroundColor()
{
    QString bgColor = m_hasFocus ? "#E6F3FF" : "white";
    setStyleSheet(QString("FocusWindow { background-color: %1; }").arg(bgColor));
}

void FocusWindow::updateFocusStatus()
{
    if (m_hasFocus) {
        m_statusLabel->setText("✓ 窗口已获得焦点");
        m_statusFrame->setStyleSheet("QFrame { background-color: #4CAF50; border-radius: 8px; }");
    } else {
        m_statusLabel->setText("⚠ 窗口失去焦点");
        m_statusFrame->setStyleSheet("QFrame { background-color: #FF9800; border-radius: 8px; }");
    }
}

void FocusWindow::focusInEvent(QFocusEvent *event)
{
    QWidget::focusInEvent(event);
    m_hasFocus = true;
    updateBackgroundColor();
    updateFocusStatus();
    
    if (m_globalFocusManager) {
        m_globalFocusManager->recordFocusGained(m_windowId);
    }
    if (m_localFocusManager) {
        m_localFocusManager->recordFocusGained(m_windowId);
    }
}

void FocusWindow::focusOutEvent(QFocusEvent *event)
{
    QWidget::focusOutEvent(event);
    m_hasFocus = false;
    updateBackgroundColor();
    updateFocusStatus();
    
    if (m_globalFocusManager) {
        m_globalFocusManager->recordFocusLost(m_windowId);
    }
    if (m_localFocusManager) {
        m_localFocusManager->recordFocusLost(m_windowId);
    }
}

void FocusWindow::changeEvent(QEvent *event)
{
    QWidget::changeEvent(event);
    
    if (event->type() == QEvent::ActivationChange) {
        bool nowActive = isActiveWindow();
        if (nowActive != m_hasFocus) {
            m_hasFocus = nowActive;
            updateBackgroundColor();
            updateFocusStatus();
            
            if (m_globalFocusManager) {
                if (nowActive) {
                    m_globalFocusManager->recordFocusGained(m_windowId);
                } else {
                    m_globalFocusManager->recordFocusLost(m_windowId);
                }
            }
            if (m_localFocusManager) {
                if (nowActive) {
                    m_localFocusManager->recordFocusGained(m_windowId);
                } else {
                    m_localFocusManager->recordFocusLost(m_windowId);
                }
            }
        }
    }
}

void FocusWindow::closeEvent(QCloseEvent *event)
{
    if (m_isMainWindow) {
        qDebug() << "主窗口关闭，退出程序";
        QApplication::quit();
    } else {
        qDebug() << QString("关闭窗口: %1").arg(m_windowId);
        event->accept();
    }
}

void FocusWindow::onCreateNewWindow()
{
    qDebug() << QString("从窗口 %1 创建新窗口").arg(m_windowId);
    
    QString newWindowId = QString("窗口-%1").arg(QDateTime::currentMSecsSinceEpoch() % 10000);
    FocusWindow *newWindow = new FocusWindow(nullptr, newWindowId, false, m_globalFocusManager);
    newWindow->show();
    
    qDebug() << QString("新窗口创建成功: %1").arg(newWindowId);
}

void FocusWindow::onCloseWindow()
{
    close();
}

void FocusWindow::onFocusChanged(const QString &winId, bool hasFocus, const QString &timestamp)
{
    if (winId == m_windowId) {
        if (hasFocus) {
            m_focusGainedTime = timestamp;
            m_focusGainedEdit->setText(m_focusGainedTime);
        } else {
            m_focusLostTime = timestamp;
            m_focusLostEdit->setText(m_focusLostTime);
        }
    }
}

void FocusWindow::onFocusSwitchTimeDiff(const QString &windowId, qint64 timeDiffMs, const QString &timeDiffText)
{
    // 只有当这个窗口获得焦点时才更新时间差显示
    if (windowId == m_windowId) {
        m_timeDiffEdit->setText(timeDiffText);
        
        // 根据时间差长短设置不同颜色
        QString color;
        if (timeDiffMs <= 400) {
            color = "#228B22";  // 绿色 - 快速切换
        } else if (timeDiffMs <= 1000) {
            color = "#FF8C00";  // 橙色 - 中等切换
        } else {
            color = "#DC143C";  // 红色 - 慢速切换
        }
        
        m_timeDiffEdit->setStyleSheet(QString(
            "QLineEdit {"
            "    background-color: #FFF8DC;"
            "    border: 1px solid %1;"
            "    border-radius: 3px;"
            "    color: %1;"
            "    font-weight: bold;"
            "}").arg(color));
        
        qDebug() << QString("窗口 %1 显示时间差: %2").arg(windowId, timeDiffText);
    }
}

// MOC文件会自动包含 