#include "claudeapi.h"
#include "googleauth.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>

ClaudeAPI::ClaudeAPI(GoogleAuth* auth, QObject* parent)
    : QObject(parent)
    , m_auth(auth)
{
    m_networkManager = new QNetworkAccessManager(this);
    m_apiKey = ""; // Wartość domyślna, zazwyczaj wczytywana z konfiguracji
}

ClaudeAPI::~ClaudeAPI()
{
}

QNetworkRequest ClaudeAPI::createAuthorizedRequest(const QUrl& url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    
    // Używamy tokenu z uwierzytelniania Google do wymiany na token API Claude
    if (m_auth->isAuthenticated()) {
        request.setRawHeader("Authorization", QString("Bearer %1").arg(m_apiKey).toUtf8());
        request.setRawHeader("X-Google-Token", m_auth->accessToken().toUtf8());
    }
    
    return request;
}

void ClaudeAPI::createNewConversation()
{
    // Tworzenie nowej konwersacji z Claude API
    QUrl url("https://api.anthropic.com/v1/messages");
    QNetworkRequest request = createAuthorizedRequest(url);
    
    QJsonObject jsonObject;
    jsonObject["model"] = "claude-3-opus-20240229";
    
    QByteArray jsonData = QJsonDocument(jsonObject).toJson();
    
    QNetworkReply* reply = m_networkManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleNewConversationResponse(reply);
    });
}

void ClaudeAPI::sendMessage(const QString& message)
{
    // Wysyłanie wiadomości do Claude API
    QUrl url("https://api.anthropic.com/v1/messages");
    QNetworkRequest request = createAuthorizedRequest(url);
    
    QJsonObject jsonObject;
    jsonObject["model"] = "claude-3-opus-20240229";
    jsonObject["max_tokens"] = 4096;
    
    QJsonArray messagesArray;
    QJsonObject messageObject;
    messageObject["role"] = "user";
    messageObject["content"] = message;
    messagesArray.append(messageObject);
    
    jsonObject["messages"] = messagesArray;
    
    if (!m_currentConversationId.isEmpty()) {
        jsonObject["conversation_id"] = m_currentConversationId;
    }
    
    QByteArray jsonData = QJsonDocument(jsonObject).toJson();
    
    QNetworkReply* reply = m_networkManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        handleMessageResponse(reply);
    });
}

void ClaudeAPI::handleNewConversationResponse(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        
        if (jsonObj.contains("conversation_id")) {
            m_currentConversationId = jsonObj["conversation_id"].toString();
            emit conversationCreated(m_currentConversationId);
        }
    } else {
        emit errorOccurred(reply->errorString());
    }
    
    reply->deleteLater();
}

void ClaudeAPI::handleMessageResponse(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        
        if (jsonObj.contains("content")) {
            QJsonArray contentArray = jsonObj["content"].toArray();
            QString responseText;
            
            for (const QJsonValue& content : contentArray) {
                if (content.toObject()["type"].toString() == "text") {
                    responseText += content.toObject()["text"].toString();
                }
            }
            
            emit responseReceived(responseText);
        }
    } else {
        emit errorOccurred(reply->errorString());
    }
    
    reply->deleteLater();
}

void ClaudeAPI::handleAuthResponse(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        QJsonObject jsonObj = jsonDoc.object();
        
        if (jsonObj.contains("api_key")) {
            m_apiKey = jsonObj["api_key"].toString();
        }
    } else {
        emit errorOccurred(reply->errorString());
    }
    
    reply->deleteLater();
}