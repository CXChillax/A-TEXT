
##作者 陈灿坚 

##本程序使用了libjepg库来显示jpeg

##目录结构
	include ======== 存放函数的声明
	lib	======== 存放函数的实现

##文件
	gomoku.tar.bz2 == 该压缩包为cotex-A8环境下的程序实现，包含素材包
	
##程序功能
	#本地对战
		白子为先手 黑子为后手
		
	#网络对战
		1.创建者为白子、先手
		  连接者为黑子、后手
		2.开局后，胜者可为先手下棋 但棋子颜色不会改变

##注1：
		可以修改main.c启动测试程序：

		./main 本地对战
		./main [port]	作为服务器
		./main [ip] [port] 连接服务器

##注2:
	若素材缺少tips.jpg 请自行讲其添加

##待做：

	1.退出界面(实现)

	2.输赢界面（实现）

	3.提示哪方落子(没实现 思路：交换的msg信息多一点)

	4.标记哪个位置落子（本地对战实现）
	1.先显示落子的位置
	2.然后自方落子前刷新棋盘