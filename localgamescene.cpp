#include "localgamescene.h"
#include "rule.h"
#include <QDebug>

LocalGameScene::LocalGameScene()
{
    // 设置Scene大小
    setSceneRect(0,0,600,442);
    // 初始化人物位置
    item1.setPositonInfo(200, 300);
    item2.setPositonInfo(500, 300);
    healthBar_1.setPos(0,0);
    //增加血条精力条
    addItem(&healthBar_1);
    addItem(&healthBar_2);
    addItem(&energyBar_1);
    addItem(&energyBar_2);
    //设置血条精力条的位置
    healthBar_1.setPos(0,0);
    healthBar_2.setPos(600-healthBar_2.getMaxWdith(),0);
    addItem(&item1);
    energyBar_1.setPos(0,healthBar_1.getHeight());
    energyBar_2.setPos(600-energyBar_2.getMaxWdith(),healthBar_1.getHeight());
    addItem(&item2);
    //添加爆炸物,并且设置为不可见
    addItem(&m_explodingitem);
    m_explodingitem.setVisible(false);
    timerId = startTimer(100);
    m_timerCount = 0;

}

LocalGameScene::~LocalGameScene()
{

}

void LocalGameScene::timerEvent(QTimerEvent *event)
{
    if(event->timerId() == timerId)
    {
        //设想（还没实现，因为经测试这样做有点小问题）因为可能要经常判断1P和2P是否碰撞，所以在成员变量中添加一个bool型的成员变量来减少对碰撞检测函数的调用
        m_timerCount++;
        if(m_timerCount == 10)
        {
            m_timerCount = 0;
            Rule::recoverEnergy(item1);
            Rule::recoverEnergy(item2);
            Rule::recoverTenacity(item1);
            Rule::recoverTenacity(item2);
            qDebug()<<"1:"<<item1.getBlood()<<"||"<<item1.getEnergy()<<"||"<<item1.getTenacity();
            qDebug()<<"2:"<<item2.getBlood()<<"||"<<item2.getEnergy()<<"||"<<item2.getTenacity();
        }
        isCollided(item1,item2);   //1P 和2P的 碰撞检测，必须放在攻击检测的前面，否则ISATTACKED会被替换成 ISCOLLIDEDLEFT，以后可以考虑整合成一个大函数
        //人物的攻击判定  测试
        item1.JudgeingAttack();
        if(item1.getAttackedFlag())
        {
            if(isAttacked(item1,item2))
            {
               if(item2.getCollidedState()==PlayerItem::ISCOLLIDEDLEFT)
               {
                   m_explodingitem.setX(item2.x());
                   m_explodingitem.setY(item2.y()-item2.getHeight()*4/5);
                   //把爆炸物状态视为可见
                   m_explodingitem.updatePos();
                   m_explodingitem.isItemVisable=true;
                   qDebug()<<m_explodingitem.getX()<<m_explodingitem.getY()<<m_explodingitem.scenePos();
               }
               else if(item2.getCollidedState()==PlayerItem::ISCOLLIDEDRIGHT)
               {
                   m_explodingitem.setX(item2.x()+item2.getWidth());
                   m_explodingitem.setY(item2.y()-item2.getHeight()*4/5);
                   //把爆炸物状态视为可见
                   m_explodingitem.updatePos();
                   m_explodingitem.isItemVisable=true;
                   qDebug()<<m_explodingitem.getX()<<m_explodingitem.getY()<<m_explodingitem.scenePos();
               }
            }
        }
        item2.JudgeingAttack();
        if(item2.getAttackedFlag())
        {
            if(isAttacked(item2,item1))
            {
               if(item1.getCollidedState()==PlayerItem::ISCOLLIDEDLEFT)
               {
                    m_explodingitem.setX(item1.x()+item1.getWidth()*2/5);
                    m_explodingitem.setY(item1.y()-item1.getHeight()*4/5);
                    //把爆炸物状态视为可见
                    m_explodingitem.updatePos();
                    m_explodingitem.isItemVisable=true;
                    qDebug()<<m_explodingitem.getX()<<m_explodingitem.getY()<<m_explodingitem.scenePos();
               }
               else if(item1.getCollidedState()==PlayerItem::ISCOLLIDEDRIGHT)
               {
                   m_explodingitem.setX(item1.x()+item1.getWidth()*3/5);
                   m_explodingitem.setY(item1.y()-item1.getHeight()*4/5);
                   //把爆炸物状态视为可见
                   m_explodingitem.updatePos();
                   m_explodingitem.isItemVisable=true;
                   qDebug()<<m_explodingitem.getX()<<m_explodingitem.getY()<<m_explodingitem.scenePos();

               }
            }
        }
        //刷新爆炸物
        if(m_explodingitem.isItemVisable)
        {

            m_explodingitem.setVisible(true);
        }
        else
        {

             m_explodingitem.setVisible(false);
        }


        //刷新血条和精力条
        healthBar_1.setBlood(item1.getBlood());
        healthBar_2.setBlood(item2.getBlood());
        energyBar_1.setEnergy(item1.getEnergy());
        energyBar_2.setEnergy(item2.getEnergy());

        // 游戏线程 刷新视图和人物跑动 碰撞 跳跃
        item1.jump();
        item2.jump();
        item1.run();
        item2.run();
        item1.attackingMove();
        item2.attackingMove();
        // 碰撞
        item1.updatePos();
        item2.updatePos();

    }
}

// 事件分发函数，同时控制两个item人物
bool LocalGameScene::event(QEvent *event)
{
    switch(event->type())
    {
        case QEvent::KeyPress:
            sendEvent(&item1, event);
            sendEvent(&item2, event);
            return true;
        case QEvent::KeyRelease:
            sendEvent(&item1, event);
            sendEvent(&item2, event);
            return true;
        default:
            return GameScene::event(event);
    }
}
