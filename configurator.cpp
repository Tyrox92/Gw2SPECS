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

    // hide not used settings:
    ui->checkBoxSoloCDamage->setVisible(false);
    ui->checkBoxSoloCDPS->setVisible(false);
    ui->checkBoxSoloCPerDmg->setVisible(false);
    ui->checkBoxGroupActivity->setVisible(false);

    this->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);

    // Set version
    ui->labelVersion->setText(Settings::s_Product);

    Settings::ReadSettings(ui->checkBoxSoloProfColors);
    Settings::ReadSettings(ui->checkBoxSoloName);
    Settings::ReadSettings(ui->checkBoxSoloDamage);
    Settings::ReadSettings(ui->checkBoxSoloDPS);
    Settings::ReadSettings(ui->checkBoxSoloCDamage);
    Settings::ReadSettings(ui->checkBoxSoloCPerDmg);
    Settings::ReadSettings(ui->checkBoxSoloCDPS);

    Settings::ReadSettings(ui->professionGroupComboBox);
    Settings::ReadSettings(ui->checkBoxGroupPosition);
    Settings::ReadSettings(ui->checkBoxGroupPerDmg);
    Settings::ReadSettings(ui->checkBoxGroupActivity);

    Settings::ReadSettings(ui->checkBoxShowGraph);
    Settings::ReadSettings(ui->checkBoxRealDPS);
    Settings::ReadSettings(ui->checkBox5sDPS);
    Settings::ReadSettings(ui->checkBoxAvgSoloDPS);
    Settings::ReadSettings(ui->checkBoxAvgCDPS);
    Settings::ReadSettings(ui->checkBoxAvgGroupDPS);

    Settings::ReadSettings(ui->comboBoxScreenshots);
    Settings::ReadSettings(ui->comboBoxUpdates);
    Settings::ReadSettings(ui->comboBoxSecondsInCombat);
    Settings::ReadSettings(ui->comboBoxConsideredLines);

}

Configurator::~Configurator()
{
    Settings::WriteSettings(ui->checkBoxSoloProfColors);
    Settings::WriteSettings(ui->checkBoxSoloName);
    Settings::WriteSettings(ui->checkBoxSoloDamage);
    Settings::WriteSettings(ui->checkBoxSoloDPS);
    Settings::WriteSettings(ui->checkBoxSoloCDamage);
    Settings::WriteSettings(ui->checkBoxSoloCPerDmg);
    Settings::WriteSettings(ui->checkBoxSoloCDPS);

    Settings::WriteSettings(ui->professionGroupComboBox);
    Settings::WriteSettings(ui->checkBoxGroupPosition);
    Settings::WriteSettings(ui->checkBoxGroupPerDmg);
    Settings::WriteSettings(ui->checkBoxGroupActivity);

    Settings::WriteSettings(ui->checkBoxShowGraph);
    Settings::WriteSettings(ui->checkBoxRealDPS);
    Settings::WriteSettings(ui->checkBox5sDPS);
    Settings::WriteSettings(ui->checkBoxAvgSoloDPS);
    Settings::WriteSettings(ui->checkBoxAvgCDPS);
    Settings::WriteSettings(ui->checkBoxAvgGroupDPS);

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

