#include "claudeplugin.h"

#include <KLocalizedString>
#include <KPluginFactory>

#include <interfaces/icore.h>
#include <interfaces/iuicontroller.h>

#include "claudeview.h"
#include "googleauth.h"
#include "claudeapi.h"

K_PLUGIN_FACTORY_WITH_JSON(ClaudePluginFactory, "kdevclaudeplugin.json", registerPlugin<ClaudePlugin>();)

class ClaudeViewFactory : public KDevelop::IToolViewFactory
{
public:
    explicit ClaudeViewFactory(ClaudePlugin* plugin, GoogleAuth* auth, ClaudeAPI* api)
        : m_plugin(plugin)
        , m_googleAuth(auth)
        , m_claudeAPI(api)
    {}

    QWidget* create(QWidget* parent = nullptr) override
    {
        return new ClaudeView(m_plugin, m_googleAuth, m_claudeAPI, parent);
    }

    Qt::DockWidgetArea defaultPosition() const override
    {
        return Qt::RightDockWidgetArea;
    }

    QString id() const override
    {
        return QStringLiteral("org.kdevelop.ClaudeView");
    }

private:
    ClaudePlugin* m_plugin;
    GoogleAuth* m_googleAuth;
    ClaudeAPI* m_claudeAPI;
};

ClaudePlugin::ClaudePlugin(QObject* parent, const QVariantList& args)
    : KDevelop::IPlugin(QStringLiteral("kdevclaudeplugin"), parent)
    , m_claudeView(nullptr)
{
    Q_UNUSED(args);

    m_googleAuth = new GoogleAuth(this);
    m_claudeAPI = new ClaudeAPI(m_googleAuth, this);
    
    m_viewFactory = new ClaudeViewFactory(this, m_googleAuth, m_claudeAPI);
    core()->uiController()->addToolView(i18n("Claude AI"), m_viewFactory);
}

ClaudePlugin::~ClaudePlugin()
{
    // Clean up
}

void ClaudePlugin::unload()
{
    core()->uiController()->removeToolView(m_viewFactory);
}

#include "claudeplugin.moc"