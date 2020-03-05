#!/bin/bash

#初始化密码，二次输入,都对之后把该目录下的文件都隐藏（除本脚本文件）
init_password()
{
	read -p "正在第一次输入密码: " first_password
	read -p "正在第二次输入密码: " second_password
	
    if [ "$first_password" = "$second_password" ]; 
	then
        echo "$second_password" >.password.txt    #如果输入两个密码相同，则输入到.password.txt文件中
        return 1
    else
        return 0
    fi
}

#判断是否已经被解码.返回1为不需要解密，0则需要解码
if_decrypted()
{
    path=$(pwd)
    echo "$path"
    files=$(ls -la $path)
    for file_name in $files; 
	do
        if [ "$file_name" = ".password.txt" ]; 
		then
            echo "$file_name"
            return 1
        fi
    done
    return 0
}

#将隐藏文件提取并显示出来
show_hide_file()
{
    password=$(cat .password.txt)   #密码等于文件中的密码，也就是第二次输入中密码
    read -p "请输入密码: " last_password

    if [ "$last_password" = "$password" ];
	then
        path=$(pwd)
        echo "$path"
        files=$(ls -la $path)
        for file_name in $files; 
		do
            if [ "${file_name:0:1}" = "." ]; 
			then
                if [ "$file_name" = ".password.txt" ] || [ "$file_name" = "." ] || [ "$file_name" = ".." ]; 
				then
                    continue
                else
                    decrypt_name="${file_name:1:${#file_name}}"
                    echo "$file_name -> $decrypt_name"
                    mv $file_name $decrypt_name  # 移出来
                fi
            fi
        done
        psd_file=.password.txt
		#最后删除文件
        rm $psd_file
    fi
}

# 
if_decrypted
if [ "$?" -ne 1 ];
then
	# 先进行初始化密码，两次输入
	init_password
    if [ "$?" -eq 1 ];    #输入两次密码相同
	then
        path=$(pwd)
        echo "$path"
        files=$(ls $path)
        for file_name in $files;
		do
			# 为了不隐藏自己的脚本文件
            if [ "$file_name" = "jiami_file.sh" ];
			then
                continue
            fi
			# 进行mv指令
            hide_name=.$file_name
            mv $file_name $hide_name
        done
    else
        echo "输入的两次密码不同"
    fi

else
    echo "该目录已被加密，请输入密码进行解密"
    show_hide_file
    if [ "$?" -eq 1 ]; then
        echo "解密成功"
    fi
fi
