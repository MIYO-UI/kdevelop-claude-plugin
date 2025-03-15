#pragma once

#include <interfaces/iplugin.h>
#include <QVariantList>

namespace KDevelop {
class IToolViewFactory;
}

class ClaudeView;
class GoogleAuth;
class ClaudeAPI;

class ClaudePlugin : public KDevelop::IPlugin
{
    Q_OBJECT

public:
    explicit ClaudePlugin(QObject* parent, const QVariantList& args);
    ~ClaudePlugin() override;

    void unload() override;

private:
    KDevelop::IToolViewFactory* m_viewFactory;
    ClaudeView* m_claudeView;
    GoogleAuth* m_googleAuth;
    ClaudeAPI* m_claudeAPI;
};