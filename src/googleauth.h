#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QWebEngineView>
#include <QOAuth2AuthorizationCodeFlow>
#include <QOAuthHttpServerReplyHandler>

class GoogleAuth : public QObject
{
    Q_OBJECT

public:
    explicit GoogleAuth(QObject* parent = nullptr);
    ~GoogleAuth() override;

    void startLogin();
    bool isAuthenticated() const;
    QString accessToken() const;
    void logout();

signals:
    void loginSuccessful();
    void loginFailed(const QString& error);
    void logoutComplete();

private slots:
    void handleAuthGranted();
    void handleTokenReceived(const QString& token);
    void handleAuthError(const QString& error);

private:
    QOAuth2AuthorizationCodeFlow* m_oauth2;
    QOAuthHttpServerReplyHandler* m_replyHandler;
    QNetworkAccessManager* m_networkManager;
    QString m_accessToken;
    bool m_authenticated;

    void setupOAuth();
};