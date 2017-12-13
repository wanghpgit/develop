#/bin/bash

for ((;;))  ; do
	for ((i=0;i<100;i++)) ; do
		./a.out &
	done

	wait
done
