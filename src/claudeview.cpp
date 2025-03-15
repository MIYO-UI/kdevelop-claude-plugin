#include "claudeview.h"
#include "ui_claudeview.h"
#include "claudeplugin.h"
#include "googleauth.h"
#include "claudeapi.h"

#include <interfaces/icore.h>
#include <interfaces/idocument.h>
#include <interfaces/idocumentcontroller.h>
#include <interfaces/ieditormanager.h>
#include <ktexteditor/view.h>
#include <ktexteditor/document.h>

ClaudeView::ClaudeView(ClaudePlugin* plugin, GoogleAuth* auth, ClaudeAPI* api, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::ClaudeView)
    , m_plugin(plugin)
    , m_googleAuth(auth)
    , m_claudeAPI(api)
{
    ui->setupUi(this);
    setupUI();
    
    // Połącz sygnały
    connect(m_loginButton, &QPushButton::clicked, this, &ClaudeView::onLoginClicked);
    connect(m_logoutButton, &QPushButton::clicked, this, &ClaudeView::onLogoutClicked);
    connect(m_sendButton, &QPushButton::clicked, this, &ClaudeView::onSendMessageClicked);
    
    connect(m_googleAuth, &GoogleAuth::loginSuccessful, this, &ClaudeView::onLoginStatusChanged);
    connect(m_googleAuth, &GoogleAuth::loginFailed, [this](const QString& error) {
        m_statusLabel->setText(tr("Login failed: %1").arg(error));
        updateUIState();
    });
    connect(m_googleAuth, &GoogleAuth::logoutComplete, this, &ClaudeView::onLoginStatusChanged);
    
    connect(m_claudeAPI, &ClaudeAPI::responseReceived, this, &ClaudeView::onResponseReceived);
    connect(m_claudeAPI, &ClaudeAPI::errorOccurred, this, &ClaudeView::onApiError);
    
    // Połącz z sygnałami KDevelop
    connect(KDevelop::ICore::self()->documentController(), &KDevelop::IDocumentController::documentActivated,
            this, &ClaudeView::updateSelectedText);
            
    // Skonfiguruj integrację z edytorem kodu
    setupCodeCompletion();
            
    // Początkowa aktualizacja stanu UI
    updateUIState();
}

ClaudeView::~ClaudeView()
{
    delete ui;
}

void ClaudeView::setupUI()
{
    // Powinienem użyć tutaj ui->xxx dla elementów z UI, ale używam bezpośrednich
    // zmiennych dla demonstracji
    
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    // Status i przyciski logowania
    QHBoxLayout* statusLayout = new QHBoxLayout();
    m_statusLabel = new QLabel(tr("Not logged in"), this);
    m_loginButton = new QPushButton(tr("Login with Google"), this);
    m_logoutButton = new QPushButton(tr("Logout"), this);
    
    statusLayout->addWidget(m_statusLabel);
    statusLayout->addWidget(m_loginButton);
    statusLayout->addWidget(m_logoutButton);
    layout->addLayout(statusLayout);
    
    // Obszar czatu
    m_chatDisplay = new QTextEdit(this);
    m_chatDisplay->setReadOnly(true);
    layout->addWidget(m_chatDisplay);
    
    // Obszar wprowadzania wiadomości
    m_messageInput = new QTextEdit(this);
    m_messageInput->setPlaceholderText(tr("Type your message or question for Claude..."));
    m_messageInput->setMaximumHeight(100);
    layout->addWidget(m_messageInput);
    
    // Przycisk wysyłania
    QHBoxLayout* sendLayout = new QHBoxLayout();
    m_sendButton = new QPushButton(tr("Send"), this);
    sendLayout->addStretch();
    sendLayout->addWidget(m_sendButton);
    layout->addLayout(sendLayout);
    
    setLayout(layout);
}

void ClaudeView::updateUIState()
{
    bool isLoggedIn = m_googleAuth->isAuthenticated();
    
    m_loginButton->setVisible(!isLoggedIn);
    m_logoutButton->setVisible(isLoggedIn);
    m_sendButton->setEnabled(isLoggedIn);
    m_messageInput->setEnabled(isLoggedIn);
    
    if (isLoggedIn) {
        m_statusLabel->setText(tr("Logged in"));
    } else {
        m_statusLabel->setText(tr("Not logged in"));
    }
}

void ClaudeView::onLoginClicked()
{
    m_statusLabel->setText(tr("Logging in..."));
    m_googleAuth->startLogin();
}

void ClaudeView::onLogoutClicked()
{
    m_googleAuth->logout();
}

void ClaudeView::onLoginStatusChanged()
{
    updateUIState();
    
    if (m_googleAuth->isAuthenticated()) {
        m_claudeAPI->createNewConversation();
    }
}

void ClaudeView::onSendMessageClicked()
{
    QString message = m_messageInput->toPlainText();
    if (message.isEmpty()) {
        return;
    }
    
    // Dodaj wiadomość użytkownika do czatu
    m_chatDisplay->append(tr("<b>You:</b> %1").arg(message));
    m_messageInput->clear();
    
    // Wyślij wiadomość do API
    m_claudeAPI->sendMessage(message);
    m_chatDisplay->append(tr("<i>Claude is thinking...</i>"));
}

void ClaudeView::onResponseReceived(const QString& response)
{
    // Usuń "Claude is thinking..." i dodaj odpowiedź
    QTextCursor cursor = m_chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
    cursor.removeSelectedText();
    
    m_chatDisplay->append(tr("<b>Claude:</b> %1").arg(response));
}

void ClaudeView::onApiError(const QString& error)
{
    m_chatDisplay->append(tr("<span style=\"color:red\"><b>Error:</b> %1</span>").arg(error));
}

QString ClaudeView::getSelectedText()
{
    KDevelop::IDocument* doc = KDevelop::ICore::self()->documentController()->activeDocument();
    if (!doc) {
        return QString();
    }
    
    KTextEditor::View* view = qobject_cast<KTextEditor::View*>(doc->activeTextView());
    if (!view) {
        return QString();
    }
    
    if (view->selection()) {
        return view->selectionText();
    }
    
    return QString();
}

void ClaudeView::updateSelectedText()
{
    QString selectedText = getSelectedText();
    if (!selectedText.isEmpty()) {
        m_messageInput->setText(selectedText);
    }
}

void ClaudeView::insertResponseToEditor(const QString& text)
{
    KDevelop::IDocument* doc = KDevelop::ICore::self()->documentController()->activeDocument();
    if (!doc) {
        return;
    }
    
    KTextEditor::View* view = qobject_cast<KTextEditor::View*>(doc->activeTextView());
    if (!view) {
        return;
    }
    
    // Wstaw tekst na pozycji kursora
    view->document()->insertText(view->cursorPosition(), text);
}

void ClaudeView::setupCodeCompletion()
{
    // Podłączenie do sygnałów komplementacji kodu
    connect(KDevelop::ICore::self()->documentController(), &KDevelop::IDocumentController::textDocumentCreated,
            this, [this](KDevelop::IDocument* document) {
                KTextEditor::Document* textDocument = qobject_cast<KTextEditor::Document*>(document->textDocument());
                if (!textDocument) {
                    return;
                }
                
                // Połącz z widokiem dokumentu
                connect(textDocument, &KTextEditor::Document::viewCreated, this, 
                    [this](KTextEditor::Document* document, KTextEditor::View* view) {
                        // Tutaj można dodać niestandardowe akcje do widoku
                        QAction* askClaudeAction = new QAction(tr("Ask Claude"), view);
                        connect(askClaudeAction, &QAction::triggered, this, [this, view]() {
                            if (view->selection()) {
                                QString selectedText = view->selectionText();
                                m_messageInput->setText(tr("Help me understand this code:\n\n") + selectedText);
                            }
                        });
                        
                        view->actionCollection()->addAction("ask_claude", askClaudeAction);
                        view->actionCollection()->setDefaultShortcut(askClaudeAction, Qt::CTRL | Qt::ALT | Qt::Key_C);
                    }
                );
            }
    );
}