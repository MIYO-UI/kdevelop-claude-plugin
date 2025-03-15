#pragma once

#include <KCModule>

namespace Ui {
class ClaudeConfigPage;
}

class ClaudeConfigPage : public KCModule
{
    Q_OBJECT

public:
    explicit ClaudeConfigPage(QWidget* parent = nullptr, const QVariantList& args = QVariantList());
    ~ClaudeConfigPage() override;

    void load() override;
    void save() override;
    void defaults() override;

private:
    Ui::ClaudeConfigPage* ui;
};