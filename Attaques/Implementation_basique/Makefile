visualisation:
	valgrind --tool=tracergrind --output=trace.out AES/aes 0123456789ABCDEF0123456789ABCDEF
	sqlitetrace trace.out trace.db
	tracegraph trace.db

attaque: clean
	./enregistrement_traces.py
	../../Utiles/daredevil -c mem_data*.attack_after_sbox.config


clean:
	rm -f mem* stack* trace*