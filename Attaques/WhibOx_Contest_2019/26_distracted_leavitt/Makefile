visualisation:
	valgrind --tool=tracergrind --output=trace.out ./aes 0123456789ABCDEF0123456789ABCDEF
	sqlitetrace trace.out trace.db
	tracegraph trace.db

enregistrement:
	./enregistrement_traces.py

recherche:
	../../../Utiles/recherche_exhaustive/recherche_exhaustive 9D797E44B9CF850B21??8406??E3??4E 0123456789ABCDEF0123456789ABCDEF 6ee65987f3be09d3f451b8e8c850b563

clean:
	rm -f mem* stack* trace*.bin trace*.grind trace.db trace.out