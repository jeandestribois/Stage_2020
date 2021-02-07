visualisation_sans_masque:
	valgrind --tool=tracergrind --filter=0x108000-0x4000000 --output=trace1.out ./htable/src/mon_main.out 1 0123456789ABCDEF0123456789ABCDEF
	sqlitetrace trace1.out trace1.db
	tracegraph trace1.db

visualisation_ordre_1:
	valgrind --tool=tracergrind --filter=0x108000-0x4000000 --output=trace2.out ./htable/src/mon_main.out 2 0123456789ABCDEF0123456789ABCDEF
	sqlitetrace trace2.out trace2.db
	tracegraph trace2.db


attaque_sans_masque:
	./enregistrement_traces.py 1
	../../Utiles/daredevil -c mem_data*.attack_after_sbox.config


clean:
	rm -f mem* stack* trace*