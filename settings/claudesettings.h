#pragma once

#include <QObject>
#include <KCoreConfigSkeleton>

class ClaudeSettings : public KCoreConfigSkeleton
{
    Q_OBJECT

public:
    static ClaudeSettings* self();
    
    void setApiKey(const QString& apiKey);
    QString apiKey() const;
    
    void setModelName(const QString& modelName);
    QString modelName() const;
    
    void setAutoSendSelection(bool enable);
    bool autoSendSelection() const;
    
    void setInsertResponseAtCursor(bool enable);
    bool insertResponseAtCursor() const;
    
    void save();

private:
    ClaudeSettings();
    ~ClaudeSettings() override;
    
    static ClaudeSettings* m_instance;
    
    class ClaudeSettingsPrivate;
    ClaudeSettingsPrivate* const d;
};