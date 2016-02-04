#include "configurator.h"
#include "ui_configurator.h"
#include "settings.h"
#include "mainwindow.h"

using namespace GW2;

Configurator::Configurator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Configurator)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Dialog);

    // Set version
    ui->labelVersion->setText(Settings::s_Product);


    Settings::ReadSettings(ui->comboBoxGwChatTxtSize);
    Settings::ReadSettings(ui->comboBoxGwChatWindowHeight);
    Settings::ReadSettings(ui->comboBoxGwInterfaceSize);
    Settings::ReadSettings(ui->sliderFontSize);
    Settings::ReadSettings(ui->sliderOpacity);
    Settings::ReadSettings(ui->comboBoxScreenshots);
    Settings::ReadSettings(ui->comboBoxUpdates);
    Settings::ReadSettings(ui->comboBoxSecondsInCombat);
    Settings::ReadSettings(ui->comboBoxConsideredLines);

}

Configurator::~Configurator()
{
    Settings::WriteSettings(ui->comboBoxScreenshots);
    Settings::WriteSettings(ui->comboBoxUpdates);
    Settings::WriteSettings(ui->comboBoxSecondsInCombat);
    Settings::WriteSettings(ui->comboBoxConsideredLines);
    Settings::WriteSettings(ui->comboBoxGwChatTxtSize);
    Settings::WriteSettings(ui->comboBoxGwChatWindowHeight);
    Settings::WriteSettings(ui->comboBoxGwInterfaceSize);
    Settings::WriteSettings(ui->sliderFontSize);
    Settings::WriteSettings(ui->sliderOpacity);

    delete ui;
}

void Configurator::RestoreDefaults()
{
    ui->comboBoxScreenshots->setCurrentIndex(0);
    ui->comboBoxUpdates->setCurrentIndex(0);
    ui->comboBoxSecondsInCombat->setCurrentIndex(0);
    ui->comboBoxConsideredLines->setCurrentIndex(0);
}

