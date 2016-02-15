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

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    // Set version
    ui->labelVersion->setText(Settings::s_Product);
    Settings::ReadSettings(ui->checkBoxProfColors);
    Settings::ReadSettings(ui->checkBoxDamageDone);
    Settings::ReadSettings(ui->checkBoxPosition);
    Settings::ReadSettings(ui->comboBoxScreenshots);
    Settings::ReadSettings(ui->comboBoxUpdates);
    Settings::ReadSettings(ui->comboBoxSecondsInCombat);
    Settings::ReadSettings(ui->comboBoxConsideredLines);
    Settings::ReadSettings(ui->professionComboBox);

}

Configurator::~Configurator()
{
    Settings::WriteSettings(ui->checkBoxProfColors);
    Settings::WriteSettings(ui->comboBoxScreenshots);
    Settings::WriteSettings(ui->comboBoxUpdates);
    Settings::WriteSettings(ui->comboBoxSecondsInCombat);
    Settings::WriteSettings(ui->comboBoxConsideredLines);
    Settings::WriteSettings(ui->checkBoxDamageDone);
    Settings::WriteSettings(ui->checkBoxPosition);
    Settings::WriteSettings(ui->professionComboBox);
    delete ui;
}

void Configurator::RestoreDefaults()
{
    ui->comboBoxScreenshots->setCurrentIndex(0);
    ui->comboBoxUpdates->setCurrentIndex(0);
    ui->comboBoxSecondsInCombat->setCurrentIndex(0);
    ui->comboBoxConsideredLines->setCurrentIndex(0);
}

