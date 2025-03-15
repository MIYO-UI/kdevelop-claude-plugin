#include "claudeconfigpage.h"
#include "ui_claudeconfig.h"
#include "claudesettings.h"

ClaudeConfigPage::ClaudeConfigPage(QWidget* parent, const QVariantList& args)
    : KCModule(parent, args)
    , ui(new Ui::ClaudeConfigPage)
{
    ui->setupUi(this);
    
    connect(ui->apiKeyEdit, &QLineEdit::textChanged, this, &ClaudeConfigPage::markAsChanged);
    connect(ui->modelComboBox, &QComboBox::currentTextChanged, this, &ClaudeConfigPage::markAsChanged);
    connect(ui->autoSelectCheckBox, &QCheckBox::toggled, this, &ClaudeConfigPage::markAsChanged);
    connect(ui->insertResponseCheckBox, &QCheckBox::toggled, this, &ClaudeConfigPage::markAsChanged);
}

ClaudeConfigPage::~ClaudeConfigPage()
{
    delete ui;
}

void ClaudeConfigPage::load()
{
    ClaudeSettings* settings = ClaudeSettings::self();
    
    ui->apiKeyEdit->setText(settings->apiKey());
    
    // Ustaw model w combobox
    int index = ui->modelComboBox->findText(settings->modelName());
    if (index != -1) {
        ui->modelComboBox->setCurrentIndex(index);
    }
    
    ui->autoSelectCheckBox->setChecked(settings->autoSendSelection());
    ui->insertResponseCheckBox->setChecked(settings->insertResponseAtCursor());
}

void ClaudeConfigPage::save()
{
    ClaudeSettings* settings = ClaudeSettings::self();
    
    settings->setApiKey(ui->apiKeyEdit->text());
    settings->setModelName(ui->modelComboBox->currentText());
    settings->setAutoSendSelection(ui->autoSelectCheckBox->isChecked());
    settings->setInsertResponseAtCursor(ui->insertResponseCheckBox->isChecked());
    
    settings->save();
}

void ClaudeConfigPage::defaults()
{
    ui->apiKeyEdit->clear();
    ui->modelComboBox->setCurrentIndex(0); // Pierwszy model w liście (domyślny)
    ui->autoSelectCheckBox->setChecked(false);
    ui->insertResponseCheckBox->setChecked(false);
}