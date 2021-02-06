visualisation:
	valgrind --tool=tracergrind --output=trace.out ./aes 0123456789ABCDEF0123456789ABCDEF
	sqlitetrace trace.out trace.db
	tracegraph trace.db

enregistrement:
	./enregistrement_traces.py

clean:
	rm -f mem* stack* trace*