# 第一周作业:
## 打包一个安卓项目并在手机上运行
通过教程完成了ue5的编译以及Android studio的安装以及打包，并且附上手机截图。
# 第二周第四周作业：
## 制作打方块、游戏限时运行、多人游戏的同步、最后赛后总结。并且使用UMG显示加载页面、
1. 完成：UMG显示：
    - 加载页面
    - 登陆界面   
    - 玩家个人分数、赛后总结分数
    - 子弹数以及弹夹数
    - 准星以及准星反馈、玩家移动时准星缩放
2. 完成GamePlay：
- 下面都实现了多玩家网络同步
    - 方块缩放
    - 个人子弹信息
    - 枪支拾取
    - 人物骨骼同步显示
    - 准联机大厅，在所有玩家按下enter之后进入下一关
    - 重要方块颜色同步，重要方块是rgb颜色循环播放
- 下列数值可以通过蓝图编辑：
    - 缩放速度（通过tick插值把缩放变得流畅）
    - 方块缩放大小、可放大也可以缩小
    - 方块被击中几次销毁、其中我把得分设计为玩家将方块击毁之后才会得分
    - 重要方块生成的数量
    - 击中方块后的得分、重要方块是双倍

