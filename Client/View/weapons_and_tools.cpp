#include "weapons_and_tools.h"
#include "ui_weapons_and_tools.h"
#include <QDebug>


Weapons_and_Tools::Weapons_and_Tools(QWidget *parent,Player* player) :
    QDialog(parent),
    ui(new Ui::Weapons_and_Tools),
    player(player)
{   
    ui->setupUi(this);


    int ammo;

    ui->banana->setIcon(QIcon(ROOT_PATH"/resources/images/banana.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::BANANA)));
    if(ammo!=-1){
        ui->ammoBan->setText(QString::number(ammo));
    }
    ui->bat->setIcon(QIcon(ROOT_PATH"/resources/images/baseball.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::BASEBALL_BAT)));
    if(ammo!=-1){
        ui->ammoBat->setText(QString::number(ammo));
    }
    ui->bazooka->setIcon(QIcon(ROOT_PATH"/resources/images/bazooka.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::BAZOOKA)));
    if(ammo!=-1){
        ui->ammoBaz->setText(QString::number(ammo));
    }
    ui->dinamite->setIcon(QIcon(ROOT_PATH"/resources/images/dynamite.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::DYNAMITE)));
    if(ammo!=-1){
        ui->ammoDyn->setText(QString::number(ammo));
    }
    ui->fragGranade->setIcon(QIcon(ROOT_PATH"/resources/images/cluster.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::RED_GRENADE)));
    if(ammo!=-1){
        ui->ammoFrag->setText(QString::number(ammo));
    }
    ui->granade->setIcon(QIcon(ROOT_PATH"/resources/images/grenade.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::GREEN_GRENADE)));
    if(ammo!=-1){
        ui->ammoG->setText(QString::number(ammo));
    }
    ui->holyGranade->setIcon(QIcon(ROOT_PATH"/resources/images/hgrenade.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::HOLY_GRENADE)));
    if(ammo!=-1){
        ui->ammoHG->setText(QString::number(ammo));
    }
    ui->misil->setIcon(QIcon(ROOT_PATH"/resources/images/airstrke.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::AERIAL_ATTACK)));
    if(ammo!=-1){
        ui->ammoMisil->setText(QString::number(ammo));
    }
    ui->morter->setIcon(QIcon(ROOT_PATH"/resources/images/mortar.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::MORTAR)));
    if(ammo!=-1){
        ui->ammoMortar->setText(QString::number(ammo));
    }
    ui->teleport->setIcon(QIcon(ROOT_PATH"/resources/images/teleport.1.png"));
    ammo = (this->player->getAmmo(static_cast<int>(WeaponsIds::TELEPORTATION)));
    if(ammo!=-1){
        ui->ammoTel->setText(QString::number(ammo));
    }

    if(!this->player->canFire(static_cast<int>(WeaponsIds::AERIAL_ATTACK))){
        ui->misil->setEnabled(false);
        ui->ammoMisil->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::BAZOOKA))){
        ui->bazooka->setEnabled(false);
        ui->ammoBaz->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::BANANA))){
        ui->banana->setEnabled(false);
        ui->ammoBan->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::MORTAR))){
        ui->morter->setEnabled(false);
        ui->ammoMortar->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::BASEBALL_BAT))){
        ui->bat->setEnabled(false);
        ui->ammoBat->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::DYNAMITE))){
        ui->dinamite->setEnabled(false);
        ui->ammoDyn->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::GREEN_GRENADE))){
        ui->granade->setEnabled(false);
        ui->ammoG->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::RED_GRENADE))){
        ui->fragGranade->setEnabled(false);
        ui->ammoFrag->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::HOLY_GRENADE))){
        ui->holyGranade->setEnabled(false);
        ui->ammoHG->setText("");
    }
    if(!this->player->canFire(static_cast<int>(WeaponsIds::TELEPORTATION))){
        ui->teleport->setEnabled(false);
        ui->ammoTel->setText("");
    }


}

Player* Weapons_and_Tools::getPlayer(){
    return this->player;
}

Weapons_and_Tools::~Weapons_and_Tools()
{
    delete ui;
}

void Weapons_and_Tools::on_misil_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::AERIAL_ATTACK));
    close();
}

void Weapons_and_Tools::on_bat_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::BASEBALL_BAT));
    close();
}

void Weapons_and_Tools::on_banana_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::BANANA));
    close();
}

void Weapons_and_Tools::on_dinamite_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::DYNAMITE));
    close();
}

void Weapons_and_Tools::on_fragGranade_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::RED_GRENADE));
    close();
}

void Weapons_and_Tools::on_granade_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::GREEN_GRENADE));
    close();
}

void Weapons_and_Tools::on_holyGranade_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::HOLY_GRENADE));
    close();
}

void Weapons_and_Tools::on_morter_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::MORTAR));
    close();
}

void Weapons_and_Tools::on_teleport_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::TELEPORTATION));
    close();
}

void Weapons_and_Tools::on_bazooka_clicked()
{
    Worm_View *worm = this->player->getWormActive();
    if(worm==nullptr){
        close();
        return;
    }
    worm->loadSpriteWeapon(static_cast<int>(WeaponsIds::BAZOOKA));
    close();
}

