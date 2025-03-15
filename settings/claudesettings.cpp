#include "claudesettings.h"

#include <KConfigGroup>
#include <QSettings>

class ClaudeSettings::ClaudeSettingsPrivate
{
public:
    QString apiKey;
    QString modelName;
    bool autoSendSelection;
    bool insertResponseAtCursor;
};

ClaudeSettings* ClaudeSettings::m_instance = nullptr;

ClaudeSettings* ClaudeSettings::self()
{
    if (!m_instance) {
        m_instance = new ClaudeSettings();
    }
    return m_instance;
}

ClaudeSettings::ClaudeSettings()
    : KCoreConfigSkeleton()
    , d(new ClaudeSettingsPrivate)
{
    setCurrentGroup(QStringLiteral("ClaudePlugin"));
    
    addItemString(QStringLiteral("ApiKey"), d->apiKey, QString());
    addItemString(QStringLiteral("ModelName"), d->modelName, QStringLiteral("claude-3-opus-20240229"));
    addItemBool(QStringLiteral("AutoSendSelection"), d->autoSendSelection, false);
    addItemBool(QStringLiteral("InsertResponseAtCursor"), d->insertResponseAtCursor, false);
    
    load();
}

ClaudeSettings::~ClaudeSettings()
{
    delete d;
}

void ClaudeSettings::setApiKey(const QString& apiKey)
{
    d->apiKey = apiKey;
}

QString ClaudeSettings::apiKey() const
{
    return d->apiKey;
}

void ClaudeSettings::setModelName(const QString& modelName)
{
    d->modelName = modelName;
}

QString ClaudeSettings::modelName() const
{
    return d->modelName;
}

void ClaudeSettings::setAutoSendSelection(bool enable)
{
    d->autoSendSelection = enable;
}

bool ClaudeSettings::autoSendSelection() const
{
    return d->autoSendSelection;
}

void ClaudeSettings::setInsertResponseAtCursor(bool enable)
{
    d->insertResponseAtCursor = enable;
}

bool ClaudeSettings::insertResponseAtCursor() const
{
    return d->insertResponseAtCursor;
}

void ClaudeSettings::save()
{
    KCoreConfigSkeleton::save();
}