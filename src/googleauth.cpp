#include "googleauth.h"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrlQuery>
#include <QRandomGenerator>

GoogleAuth::GoogleAuth(QObject* parent)
    : QObject(parent)
    , m_authenticated(false)
{
    m_networkManager = new QNetworkAccessManager(this);
    setupOAuth();
}

GoogleAuth::~GoogleAuth()
{
}

void GoogleAuth::setupOAuth()
{
    // Inicjalizacja OAuth 2.0
    m_oauth2 = new QOAuth2AuthorizationCodeFlow(this);
    m_oauth2->setAuthorizationUrl(QUrl("https://accounts.google.com/o/oauth2/auth"));
    m_oauth2->setTokenUrl(QUrl("https://oauth2.googleapis.com/token"));
    m_oauth2->setClientIdentifier("YOUR_CLIENT_ID"); // Zastąp własnym ID klienta Google
    m_oauth2->setClientIdentifierSharedKey("YOUR_CLIENT_SECRET"); // Zastąp własnym kluczem tajnym
    
    // Ustaw zakres uprawnień
    m_oauth2->setScope("openid email profile");
    
    // Skonfiguruj obsługę odpowiedzi serwera
    m_replyHandler = new QOAuthHttpServerReplyHandler(8080, this);
    m_oauth2->setReplyHandler(m_replyHandler);
    
    // Połącz sygnały
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::authorizeWithBrowser, 
            &QDesktopServices::openUrl);
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::granted, 
            this, &GoogleAuth::handleAuthGranted);
    connect(m_oauth2, &QOAuth2AuthorizationCodeFlow::error,
            [this](const QString& error) {
                handleAuthError(error);
            });
}

void GoogleAuth::startLogin()
{
    m_oauth2->grant();
}

void GoogleAuth::handleAuthGranted()
{
    m_accessToken = m_oauth2->token();
    m_authenticated = true;
    emit loginSuccessful();
}

void GoogleAuth::handleTokenReceived(const QString& token)
{
    m_accessToken = token;
    m_authenticated = true;
    emit loginSuccessful();
}

void GoogleAuth::handleAuthError(const QString& error)
{
    m_authenticated = false;
    emit loginFailed(error);
}

bool GoogleAuth::isAuthenticated() const
{
    return m_authenticated;
}

QString GoogleAuth::accessToken() const
{
    return m_accessToken;
}

void GoogleAuth::logout()
{
    // Wylogowanie z Google
    QUrl logoutUrl("https://accounts.google.com/logout");
    QNetworkRequest request(logoutUrl);
    
    m_networkManager->get(request);
    
    m_accessToken.clear();
    m_authenticated = false;
    
    emit logoutComplete();
}