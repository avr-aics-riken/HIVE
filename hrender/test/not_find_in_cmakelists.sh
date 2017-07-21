
list=`find . -type f | grep scn`
for i in ${list}; do
	hoge=`cat ../CMakeLists.txt | grep $i | wc -l`
	if [ $hoge -eq 0 ]; then
		name=`echo $i | cut -c 3-`
		if [ `echo $name | grep _err | wc -l` -eq 0 ]; then
			echo "add_test(NAME $name	COMMAND hrender ../../hrender/test/$name)"
		else
			echo "add_test(NAME $name	COMMAND hrender ../../hrender/test/err/$name)"
		fi
	fi	
done
