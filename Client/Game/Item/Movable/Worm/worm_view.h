#ifndef WORM_VIEW_H
#define WORM_VIEW_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <QPixmap>
#include <QPainter>
#include <QEvent>
#include <QTime>
#include <QLabel>
#include "movable.h"
#include "target.h"
#include "generalSounds.h"
#include <QColor>

class Worm_View: public QObject, public MovableItem
{
    Q_OBJECT
public:
    Worm_View();
    ~Worm_View();
    Worm_View(QObject* parent, QString color);
    bool isMoving();
    bool isAlive();
    bool isMovable();
    std::pair<int, int> &getDir();
    int getAngle();
    void moveTo(int angle, int posx, int posy);
    void removeMovable();
    int getHealth();
    void setHealth(int health);
    void setAngle(int angle);
    virtual void setPosition(int x, int y) override;
    void loadSpriteWeapon(int val);
    void movTargetAngle(int dir);
    int getWeaponId();
    bool isSelect();
    void setSelect(bool cond);
    void setAlive(bool alive);
    virtual int getX() override;
    virtual int getY() override;
    virtual QRectF areaRect() override;

    std::pair<int,int> getDirWeapon();

    bool hasClickeableTarget();
    int getTargetAngle();
    int getTimeWeapon();
    void setTimeWeapon(int time);
    void setClickDir(int x, int y);
    void setColor(std::string &color);
    void setStatus(int last_on_ground, int dir);

    bool isWeaponCtD();
private slots:
    void runSpriteWeapon();

private:
    void nextFrame();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    void checkAngle(int angle);
    QLabel* labelVida;
    QLabel* showSelected;
    bool showlabelSelect;
    int health;
    bool labelset;
    std::pair<int,int> currentDir;
    std::pair<int,int> destDir;
    std::pair<int,int> lastDir;
    int angle;
    QTimer *timer;
    QPixmap *spriteImage;   // In this QPixmap object will be placed sprite
    int currentFrame;   // Coordinates X, which starts the next frame of the sprite
    generalSounds* jump;
    generalSounds* wormDeath;
    void setSprite();
    void setDestDir(int x, int y);
    void setDir(int x, int y);
    void stepSprite();
    int weapon;
    void loadSprite(QString& path_L);
    Target* target;
    void setTarget();
    bool targetVis;
    int targetAngle;   
    bool loadingWeapon;
    bool targetClick;
    bool weaponCountDwn;
    int countDown;
    std::pair<int,int> clickTarget;
    QString color;
    int last_on_ground;
    int last_dir;
    bool jumping;
    bool isFlying();
    bool isFalling();
};

#endif // WORM_VIEW_H


