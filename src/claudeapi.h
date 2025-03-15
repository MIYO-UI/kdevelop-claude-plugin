#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>

class GoogleAuth;

class ClaudeAPI : public QObject
{
    Q_OBJECT

public:
    explicit ClaudeAPI(GoogleAuth* auth, QObject* parent = nullptr);
    ~ClaudeAPI() override;

    void sendMessage(const QString& message);
    void createNewConversation();

signals:
    void responseReceived(const QString& response);
    void errorOccurred(const QString& error);
    void conversationCreated(const QString& conversationId);

private slots:
    void handleAuthResponse(QNetworkReply* reply);
    void handleMessageResponse(QNetworkReply* reply);
    void handleNewConversationResponse(QNetworkReply* reply);

private:
    GoogleAuth* m_auth;
    QNetworkAccessManager* m_networkManager;
    QString m_currentConversationId;
    QString m_apiKey;

    QNetworkRequest createAuthorizedRequest(const QUrl& url);
};