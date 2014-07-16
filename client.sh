#!/bin/bash

usage(){
COMMANDS[1]="set_led_state [on|of]; "
COMMANDS[2]="get_led_state; "
COMMANDS[3]="set_led_color [red|green|blue]; "
COMMANDS[4]="get_led_color; "
COMMANDS[5]="set_led_rate [0..5]; "
COMMANDS[6]="get_led_rate"
	echo "usage: command [args]"
	echo ${COMMANDS[@]}
	echo "for quit enter quit"
}

REQUEST_PIPE_NAME="/tmp/ivideon_request_pipe"
ANSWER_PIPE_NAME="/tmp/ivideon_answer_pipe_$$"

#Создание канала для ответов
mkfifo $ANSWER_PIPE_NAME

REQUEST=""
IS_OK_REQUEST="true"
while [ 1 ]
do
	read REQUEST
	if [ "$REQUEST" == "quit" ]
	then
		break
	elif [ "$REQUEST" == "help" ]
	then
		usage
	else
#Проверка запроса
		ARG1=`echo $REQUEST | cut -d' ' -f1`
		#проверим команду и количество аргументов. Полная проверка на стороне сервера.
		case "$ARG1" in
			#без параметров
			"get_led_state" | "get_led_color" | "get_led_rate")
				if [ "$REQUEST" != "$ARG1" ] 
				then
					IS_OK_REQUEST="false"
					echo "---BAD REQUEST"
				fi
				;;
			#с одним параметром
			"set_led_state" | "set_led_color" | "set_led_rate")
				if [ "`echo $REQUEST | cut -d' ' -f3`" !=  "" ]
				then
					IS_OK_REQUEST="false"
					echo "---BAD REQUEST"
				fi
				;;
			*)
				IS_OK_REQUEST="false"
				echo "---BAD REQUEST"
		esac
#Отправка запроса
		if [ "$IS_OK_REQUEST" == "true" ]
		then
			REQUEST1="$$_$REQUEST"
			
			if [ -p $REQUEST_PIPE_NAME ]
			then
				echo $REQUEST1 > $REQUEST_PIPE_NAME

				ANS=`cat < $ANSWER_PIPE_NAME`
				echo $ANS
			else
				echo "Request pipe is not exist"
				break
			fi
		else
			IS_OK_REQUEST="true"
		fi		
	fi
	
done

rm $ANSWER_PIPE_NAME

