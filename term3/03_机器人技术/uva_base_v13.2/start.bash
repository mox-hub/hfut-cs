#! /bin/sh

host=localhost

team="UvaBase2003"
player=src/trilearn_player
coach=src/trilearn_coach
pconf=src/player.conf
fconf=src/formations.conf

num=1
while [ $num -le 11 ]; do
	echo "start player >>> $num"
	${player} -number $num -host ${host} -team ${team} -f ${fconf} -c ${pconf} &
	sleep 0
	let num+=1
done
sleep 2
echo "start coach >>>"
${coach} -host ${host} -team ${team} -f ${fconf} &




