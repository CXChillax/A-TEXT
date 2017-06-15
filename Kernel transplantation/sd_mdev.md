1.busybox 支持mdev（一般默认支持）
	mdev是busybox中udev设备文件系统的精简版
	# make menuconfig
	确保以下几项被选中：
	Linux System Utilities  --->
	mdev                                           

	Support /etc/mdev.conf                                

	Support subdirs/symlinks                            

	Support regular expressions substitutions when renaming dev

	Support command execution at device addition/removal       

	Support loading of firmwares

2.内核配置支持
	CONFIG_HOTPLUG=y  //确保这项为y
	CONFIG_PRINTK=y
	CONFIG_NET=y     //确保这项也为y

3.设置启动脚本rcS
	/bin/mount -a    #挂载fstab里的内容
	/bin/echo /sbin/mdev > /proc/sys/kernel/hotplug
	/sbin/mdev -s		#扫描 /sys/class 和/sys/block 中所有的类设备目录若有则创建设备节点

3.在etc下创建编辑mdev.conf
	mmcblk[0-9]p[0-9] 0:0 666        @ /etc/hotplug/sd/sd_insert
	mmcblk[0-9]       0:0 666        $ /etc/hotplug/sd/sd_remove

4.根据实际所需创建sd_insert sd_remove
	# vi /etc/sd/sd_insert
	内容
		#!/bin/bash
		mkdir /mnt/sdcard
		mount -t vfat(根据实际格式) /dev/mmcblk1p1 /mnt/sdcard
	#vi /etc/sd/sd_remove
	内容
		#!/bin/bash
		sync	#同步内容
		umonut /mnt/sdcard
		rm -rf /mnt/sdcard 

5.给这两个脚本加可执行权限
	#chmod +x /etc/sd/*

出现的问题：
	1.sd没有分区 p1
	解决：
		2.使用fdisk /dev/mmcblk1 重新创建分区
		按n 添加分区
			选p 选择逻辑分区
				默认
					默认 
		按w 保存

		使用mkfs工具格式化 /dev/mmcblk1p1
			mkfs.vaft /dev/mmcblk1p1
	附录：
		fdisk 命令参数：
		Command (m for help): 在这里按m ，就会输出帮助；
		Command action
		a toggle a bootable flag
		b edit bsd disklabel
		c toggle the dos compatibility flag
		d delete a partition 注：这是删除一个分区的动作；
		l list known partition types 注：l是列出分区类型，以供我们设置相应分区的类型；
		m print this menu 注：m 是列出帮助信息；
		n add a new partition 注：添加一个分区；
		o create a new empty DOS partition table
		p print the partition table 注：p列出分区表；
		q quit without saving changes 注：不保存退出；
		s create a new empty Sun disklabel
		t change a partition's system id 注：t 改变分区类型；
		u change display/entry units
		v verify the partition table
		w write table to disk and exit 注：把分区表写入硬盘并退出；
		x extra functionality (experts only) 注：扩展应用，专家功能；

		mdev
		使用 mdev 的可选配置文件，以控制设备节点的 所有者 和 权限。
		格式：
			<device regex> <uid>:<gid> <octal permissions>
			hd[a-z][0-9]* 0:3 660

		mdev 在找到匹配行时可以执行自定义的命令
			<device regex> <uid>:<gid> <octal permissions> [<@|$|*> <command>]
			特殊字符的意义如下：
			    @ 在创建设备节点后运行命令。
			    $ 在删除设备节点前运行命令。
			    * 在创建设备节点后和删除设备节点前都运行命令。