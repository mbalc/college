all:
	mkdir sols -p && \
	cd mb385130/ && \
	./packme.sh && \
	cd ../verify && \
	python verify.py

clean clear:
	rm -rf tests-* logs sols
