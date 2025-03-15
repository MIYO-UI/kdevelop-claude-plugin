#pragma once

#include <QWidget>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

namespace Ui {
class ClaudeView;
}

class ClaudePlugin;
class GoogleAuth;
class ClaudeAPI;

class ClaudeView : public QWidget
{
    Q_OBJECT

public:
    explicit ClaudeView(ClaudePlugin* plugin, GoogleAuth* auth, ClaudeAPI* api, QWidget* parent = nullptr);
    ~ClaudeView() override;

private slots:
    void onLoginClicked();
    void onLogoutClicked();
    void onSendMessageClicked();
    void onLoginStatusChanged();
    void onResponseReceived(const QString& response);
    void onApiError(const QString& error);
    void updateSelectedText();
    void insertResponseToEditor(const QString& text);

private:
    Ui::ClaudeView* ui;
    ClaudePlugin* m_plugin;
    GoogleAuth* m_googleAuth;
    ClaudeAPI* m_claudeAPI;
    
    QTextEdit* m_chatDisplay;
    QTextEdit* m_messageInput;
    QPushButton* m_sendButton;
    QPushButton* m_loginButton;
    QPushButton* m_logoutButton;
    QLabel* m_statusLabel;
    
    QString getSelectedText();
    void setupUI();
    void updateUIState();
    void setupCodeCompletion();
};