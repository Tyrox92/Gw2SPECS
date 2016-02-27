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
    Settings::ReadSettings(ui->professionComboBox);
    Settings::ReadSettings(ui->checkBoxPosition);
    Settings::ReadSettings(ui->checkBoxName);
    Settings::ReadSettings(ui->checkBoxDamageDone);
    Settings::ReadSettings(ui->checkBoxPerDmg);
    Settings::ReadSettings(ui->checkBoxDPS);
    Settings::ReadSettings(ui->checkBoxActivity);

    Settings::ReadSettings(ui->comboBoxScreenshots);
    Settings::ReadSettings(ui->comboBoxUpdates);
    Settings::ReadSettings(ui->comboBoxSecondsInCombat);
    Settings::ReadSettings(ui->comboBoxConsideredLines);

}

Configurator::~Configurator()
{
    Settings::WriteSettings(ui->checkBoxProfColors);
    Settings::WriteSettings(ui->professionComboBox);
    Settings::WriteSettings(ui->checkBoxPosition);
    Settings::WriteSettings(ui->checkBoxName);
    Settings::WriteSettings(ui->checkBoxDamageDone);
    Settings::WriteSettings(ui->checkBoxPerDmg);
    Settings::WriteSettings(ui->checkBoxDPS);
    Settings::WriteSettings(ui->checkBoxActivity);

    Settings::WriteSettings(ui->comboBoxScreenshots);
    Settings::WriteSettings(ui->comboBoxUpdates);
    Settings::WriteSettings(ui->comboBoxSecondsInCombat);
    Settings::WriteSettings(ui->comboBoxConsideredLines);
    delete ui;
}

void Configurator::RestoreDefaults()
{
    ui->comboBoxScreenshots->setCurrentIndex(0);
    ui->comboBoxUpdates->setCurrentIndex(0);
    ui->comboBoxSecondsInCombat->setCurrentIndex(0);
    ui->comboBoxConsideredLines->setCurrentIndex(0);
}

