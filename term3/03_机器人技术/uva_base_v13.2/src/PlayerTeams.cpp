/*
Copyright (c) 2000-2003, Jelle Kok, University of Amsterdam
All rights reserved.

Modified by ming gao Hfut..for server V14
1). dash dirction for not only forward and back but also left right and other 45*(-3,-1,1,3) dirction,,
2). add Stamina.Capacity and the senceHandle analy systerm and worldmodel update

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
this list of conditions and the following disclaimer in the documentation
and/or other materials provided with the distribution.

3. Neither the name of the University of Amsterdam nor the names of its
contributors may be used to endorse or promote products derived from this
software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*! \file PlayerTeams.cpp
<pre>
<b>File:</b>          PlayerTest.cpp
<b>Project:</b>       Robocup Soccer Simulation Team: UvA Trilearn
<b>Authors:</b>       Jelle Kok
<b>Created:</b>       10/12/2000
<b>Last Revision:</b> $ID$
<b>Contents:</b>      This file contains the class definitions for the
                      Player that are used to test the teams' high level
                      strategy.
<hr size=2>
<h2><b>Changes</b></h2>
<b>Date</b>             <b>Author</b>          <b>Comment</b>
10/12/2000        Jelle Kok       Initial version created
</pre>
*/

#include "Player.h"

/*!This method is the first complete simple team and defines the actions taken
   by all the players on the field (excluding the goalie). It is based on the
   high-level actions taken by the simple team FC Portugal that it released in
   2000. The players do the following:
   - if ball is kickable
       kick ball to goal (random corner of goal)
   - else if i am fastest player to ball
       intercept the ball
   - else
       move to strategic position based on your home position and pos ball */
///misol.gao fixed--->
///大家好！很高兴你们找到了这个函数！这个函数，就是我们球队决策的起点，所有的比赛策略都在这里，从这里出发，不会错的！
/// deMeer5 是球员的策略函数！ 在下面你们会发现 deMeer5_goalie 那个是守门员的，目前基本可以忽略。
SoccerCommand Player::deMeer5(  )
{

    SoccerCommand soc(CMD_ILLEGAL);// 定义一个命令对象，后面会频繁用它来构造一个球员动作命令
    VecPosition   posAgent = WM->getAgentGlobalPosition();// 一个点对象，表示Agent(可以理解成当前在执行这个代码的球员它自己)的位置
    ///下面我们把“我“就理解成当前执行这个代码的Agent，因为有11个球员在执行这个代码，所以对于每个球员 肯定有一个“我”存在。
    VecPosition   posBall  = WM->getBallPos();// 球的位置
    int           iTmp;


    if ( WM->isBeforeKickOff( ) ) /// 如果还没有开球 注意！ 这里只是还没有开球的情况！ 要做开球后的决策，向下面继续找另外一个 WM->isBeforeKickOff( )
    {
        if ( WM->isKickOffUs( ) && WM->getPlayerNumber() == 9 ) // 9 takes kick //就找9号去开球（判断了是不是我们开球，和我"Agent"是不是9号）
        {
            if ( WM->isBallKickable() )// 当球对于我来说是不是可踢！
            {
                VecPosition posGoal( PITCH_LENGTH/2.0,
                                     (-1 + 2*(WM->getCurrentCycle()%2)) *
                                     0.4 * SS->getGoalWidth() );// 计算一个射门点
                soc = kickTo( posGoal, SS->getBallSpeedMax() ); // kick maximal   以最大速度把球踢向射门点！
                Log.log( 100, "take kick off" );
            }
            else //如果球对于我不可踢，那我就去抢球！
            {
                soc = intercept( false );
                Log.log( 100, "move to ball to take kick-off" );
            }
            ACT->putCommandInQueue( soc );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            return soc;
        }
        //下面不用管！
        if ( formations->getFormation() != FT_INITIAL || // not in kickoff formation
                posAgent.getDistanceTo( WM->getStrategicPosition() ) > 2.0 )
        {
            formations->setFormation( FT_INITIAL );       // go to kick_off formation
            ACT->putCommandInQueue( soc=teleportToPos( WM->getStrategicPosition() ));
        }
        else                                            // else turn to center
        {
            ACT->putCommandInQueue( soc=turnBodyToPoint( VecPosition( 0, 0 ), 0 ) );
            ACT->putCommandInQueue( alignNeckWithBody( ) );
        }
    }
    else  /// 这个else 大家应该可以知道，是开球以后的决策！ 也就是最重要的比赛正常模式下的决策！ 很重要，我们就从这里开始！！！！ Start！
    {
        formations->setFormation( FT_433_OFFENSIVE );//设置球队出场阵形！
        soc.commandType = CMD_ILLEGAL;//初始化soc命令对象

        ///test SoccerCommand(CMD_DASH, 80, 45) for v14 ... fixed by misol.gao
       /* if ( WM->getPlayerNumber() == 8 )
        {
			soc = SoccerCommand(CMD_DASH, 80, 45);
            ACT->putCommandInQueue( soc ); // 放入命令队列
            ACT->putCommandInQueue( alignNeckWithBody() );
            return soc;
        }*/

        if ( WM->getConfidence( OBJECT_BALL ) < PS->getBallConfThr() )//判断对球的可信度，如果小于某个阈值，则...也就是说，如果很多周期没有看到球在哪里了
        {
            ACT->putCommandInQueue( soc = searchBall() );   // if ball pos unknown //执行找球动作！并放入命令队列
            ACT->putCommandInQueue( alignNeckWithBody( ) ); // search for it //同时把脖子随身体一起转
        }
        else if ( WM->isBallKickable())                   // if kickable // 如果球已知，而且当前球在我脚下(可踢)
        {
            VecPosition posGoal( PITCH_LENGTH/2.0,
                                 (-1 + 2*(WM->getCurrentCycle()%2)) * 0.4 * SS->getGoalWidth() ); //定义一个射门点 PITCH_LENGTH是球场的长度。这个点就是按照周期看，交替的打球门的两个死角。
            soc = kickTo( posGoal, SS->getBallSpeedMax() ); // kick maximal // 把球以最大速度踢向那个射门点
            ACT->putCommandInQueue( soc ); // 放入命令队列
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) ); // 把脖子转向球，也就是一直看着球
            Log.log( 100, "kick ball" );
        }
        else if ( WM->getFastestInSetTo( OBJECT_SET_TEAMMATES, OBJECT_BALL, &iTmp )
                  == WM->getAgentObjectType()  && !WM->isDeadBallThem() ) // 如果球不在我的控制范围下，但是当前能最快抢到球的是我，那我就去执行抢球动作
        {                                                // if fastest to ball
            Log.log( 100, "I am fastest to ball; can get there in %d cycles", iTmp );
            soc = intercept( false );                      // intercept the ball

            if ( soc.commandType == CMD_DASH &&            // if stamina low  // 这里是对体力的一个保护，体力过低就把Dash的dPower减小 保护体力
                    WM->getAgentStamina().getStamina() <
                    SS->getRecoverDecThr()*SS->getStaminaMax()+200 )
            {
                soc.dPower = 30.0 * WM->getAgentStamina().getRecovery(); // dash slow
                ACT->putCommandInQueue( soc );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
            else                                           // if stamina high
            {
                ACT->putCommandInQueue( soc );               // dash as intended
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
        }
        else if ( posAgent.getDistanceTo(WM->getStrategicPosition()) >
                  1.5 + fabs(posAgent.getX()-posBall.getX())/10.0) // 到了这里就是其他距离球相对远一点的人了，如果离自己的阵形点太远，就跑回 自己的阵形点去。
            // if not near strategic pos
        {
            if ( WM->getAgentStamina().getStamina() >    // if stamina high
                    SS->getRecoverDecThr()*SS->getStaminaMax()+800 )
            {
                soc = moveToPos(WM->getStrategicPosition(),
                                PS->getPlayerWhenToTurnAngle());
                ACT->putCommandInQueue( soc );            // move to strategic pos
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
            else                                        // else watch ball
            {
                ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
        }
        else if ( fabs( WM->getRelativeAngle( OBJECT_BALL ) ) > 1.0 ) // watch ball //其他剩下的球员呢，就看球。！
        {///这里就是无球队员的跑位决策
            ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else                                         // nothing to do
            ACT->putCommandInQueue( SoccerCommand(CMD_TURNNECK,0.0) );
    }
    return soc;
}

/*!This method is a simple goalie based on the goalie of the simple Team of
   FC Portugal. It defines a rectangle in its penalty area and moves to the
   position on this rectangle where the ball intersects if you make a line
   between the ball position and the center of the goal. If the ball can
   be intercepted in the own penalty area the ball is intercepted and catched.
*/
SoccerCommand Player::deMeer5_goalie(  )
{
    int i;
    SoccerCommand soc;
    VecPosition   posAgent = WM->getAgentGlobalPosition();
    AngDeg        angBody  = WM->getAgentGlobalBodyAngle();

    // define the top and bottom position of a rectangle in which keeper moves
    static const VecPosition posLeftTop( -PITCH_LENGTH/2.0 +
                                         0.7*PENALTY_AREA_LENGTH, -PENALTY_AREA_WIDTH/4.0 );
    static const VecPosition posRightTop( -PITCH_LENGTH/2.0 +
                                          0.7*PENALTY_AREA_LENGTH, +PENALTY_AREA_WIDTH/4.0 );

    // define the borders of this rectangle using the two points.
    static Line  lineFront = Line::makeLineFromTwoPoints(posLeftTop,posRightTop);
    static Line  lineLeft  = Line::makeLineFromTwoPoints(
                                 VecPosition( -50.0, posLeftTop.getY()), posLeftTop );
    static Line  lineRight = Line::makeLineFromTwoPoints(
                                 VecPosition( -50.0, posRightTop.getY()),posRightTop );


    if ( WM->isBeforeKickOff( ) )
    {
        if ( formations->getFormation() != FT_INITIAL || // not in kickoff formation
                posAgent.getDistanceTo( WM->getStrategicPosition() ) > 2.0 )
        {
            formations->setFormation( FT_INITIAL );       // go to kick_off formation
            ACT->putCommandInQueue( soc=teleportToPos(WM->getStrategicPosition()) );
        }
        else                                            // else turn to center
        {
            ACT->putCommandInQueue( soc = turnBodyToPoint( VecPosition( 0, 0 ), 0 ));
            ACT->putCommandInQueue( alignNeckWithBody( ) );
        }
        return soc;
    }

    if ( WM->getConfidence( OBJECT_BALL ) < PS->getBallConfThr() )
    {                                                // confidence ball too  low
        ACT->putCommandInQueue( searchBall() );        // search ball
        ACT->putCommandInQueue( alignNeckWithBody( ) );
    }
    else if ( WM->getPlayMode() == PM_PLAY_ON || WM->isFreeKickThem() ||
              WM->isCornerKickThem() )
    {
        if ( WM->isBallCatchable() )
        {
            ACT->putCommandInQueue( soc = catchBall() );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else if ( WM->isBallKickable() )
        {
            soc = kickTo( VecPosition(0,posAgent.getY()*2.0), 2.0 );
            ACT->putCommandInQueue( soc );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else if ( WM->isInOwnPenaltyArea( getInterceptionPointBall( &i, true ) ) &&
                  WM->getFastestInSetTo( OBJECT_SET_PLAYERS, OBJECT_BALL, &i ) ==
                  WM->getAgentObjectType() )
        {
            ACT->putCommandInQueue( soc = intercept( true ) );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else
        {
            // make line between own goal and the ball
            VecPosition posMyGoal = ( WM->getSide() == SIDE_LEFT )
                                    ? SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_L, SIDE_LEFT )
                                    : SoccerTypes::getGlobalPositionFlag(OBJECT_GOAL_R, SIDE_RIGHT);
            Line lineBall = Line::makeLineFromTwoPoints( WM->getBallPos(),posMyGoal);

            // determine where your front line intersects with the line from ball
            VecPosition posIntersect = lineFront.getIntersection( lineBall );

            // outside rectangle, use line at side to get intersection
            if (posIntersect.isRightOf( posRightTop ) )
                posIntersect = lineRight.getIntersection( lineBall );
            else if (posIntersect.isLeftOf( posLeftTop )  )
                posIntersect = lineLeft.getIntersection( lineBall );

            if ( posIntersect.getX() < -49.0 )
                posIntersect.setX( -49.0 );

            // and move to this position
            if ( posIntersect.getDistanceTo( WM->getAgentGlobalPosition() ) > 0.5 )
            {
                soc = moveToPos( posIntersect, PS->getPlayerWhenToTurnAngle() );
                ACT->putCommandInQueue( soc );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
            else
            {
                ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
        }
    }
    else if ( WM->isFreeKickUs() == true || WM->isGoalKickUs() == true )
    {
        if ( WM->isBallKickable() )
        {
            if ( WM->getTimeSinceLastCatch() == 25 && WM->isFreeKickUs() )
            {
                // move to position with lesser opponents.
                if ( WM->getNrInSetInCircle( OBJECT_SET_OPPONENTS,
                                             Circle(posRightTop, 15.0 )) <
                        WM->getNrInSetInCircle( OBJECT_SET_OPPONENTS,
                                                Circle(posLeftTop,  15.0 )) )
                    soc.makeCommand( CMD_MOVE,posRightTop.getX(),posRightTop.getY(),0.0);
                else
                    soc.makeCommand( CMD_MOVE,posLeftTop.getX(), posLeftTop.getY(), 0.0);
                ACT->putCommandInQueue( soc );
            }
            else if ( WM->getTimeSinceLastCatch() > 28 )
            {
                soc = kickTo( VecPosition(0,posAgent.getY()*2.0), 2.0 );
                ACT->putCommandInQueue( soc );
            }
            else if ( WM->getTimeSinceLastCatch() < 25 )
            {
                VecPosition posSide( 0.0, posAgent.getY() );
                if ( fabs( (posSide - posAgent).getDirection() - angBody) > 10 )
                {
                    soc = turnBodyToPoint( posSide );
                    ACT->putCommandInQueue( soc );
                }
                ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
            }
        }
        else if ( WM->isGoalKickUs()  )
        {
            ACT->putCommandInQueue( soc = intercept( true ) );
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
        }
        else
            ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
    }
    else
    {
        ACT->putCommandInQueue( soc = turnBodyToObject( OBJECT_BALL ) );
        ACT->putCommandInQueue( turnNeckToObject( OBJECT_BALL, soc ) );
    }
    
    return soc;
}


