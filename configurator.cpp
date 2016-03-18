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

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog | Qt::WindowStaysOnTopHint);

    // Set version
    ui->labelVersion->setText(Settings::s_Product);

    Settings::ReadSettings(ui->checkBoxSoloProfColors);
    Settings::ReadSettings(ui->professionComboBox);
    Settings::ReadSettings(ui->checkBoxSoloName);
    Settings::ReadSettings(ui->checkBoxSoloDamage);
    Settings::ReadSettings(ui->checkBoxSoloDPS);
    Settings::ReadSettings(ui->checkBoxSoloCDamage);
    Settings::ReadSettings(ui->checkBoxSoloCPerDmg);
    Settings::ReadSettings(ui->checkBoxSoloCDPS);

    Settings::ReadSettings(ui->checkBoxGroupProfColors);
    Settings::ReadSettings(ui->professionComboBox);
    Settings::ReadSettings(ui->checkBoxGroupPosition);
    Settings::ReadSettings(ui->checkBoxGroupName);
    Settings::ReadSettings(ui->checkBoxGroupDamage);
    Settings::ReadSettings(ui->checkBoxGroupPerDmg);
    Settings::ReadSettings(ui->checkBoxGroupDPS);
    Settings::ReadSettings(ui->checkBoxGroupActivity);

    Settings::ReadSettings(ui->comboBoxScreenshots);
    Settings::ReadSettings(ui->comboBoxUpdates);
    Settings::ReadSettings(ui->comboBoxSecondsInCombat);
    Settings::ReadSettings(ui->comboBoxConsideredLines);

}

Configurator::~Configurator()
{
    Settings::WriteSettings(ui->checkBoxSoloProfColors);
    Settings::WriteSettings(ui->professionComboBox);
    Settings::WriteSettings(ui->checkBoxSoloName);
    Settings::WriteSettings(ui->checkBoxSoloDamage);
    Settings::WriteSettings(ui->checkBoxSoloDPS);
    Settings::WriteSettings(ui->checkBoxSoloCDamage);
    Settings::WriteSettings(ui->checkBoxSoloCPerDmg);
    Settings::WriteSettings(ui->checkBoxSoloCDPS);

    Settings::WriteSettings(ui->checkBoxGroupProfColors);
    Settings::WriteSettings(ui->professionComboBox);
    Settings::WriteSettings(ui->checkBoxGroupPosition);
    Settings::WriteSettings(ui->checkBoxGroupName);
    Settings::WriteSettings(ui->checkBoxGroupDamage);
    Settings::WriteSettings(ui->checkBoxGroupPerDmg);
    Settings::WriteSettings(ui->checkBoxGroupDPS);
    Settings::WriteSettings(ui->checkBoxGroupActivity);

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

