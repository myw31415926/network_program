# network program makefile

DIRS = \
	include  \
	tcp_echo \
	tcp_data \
	select_echo \
	poll_echo \

all:
	for i in $(DIRS); do \
		(cd $$i && echo "making $$i" && $(MAKE) ) || exit 1; \
	done

clean:
	for i in $(DIRS); do \
		(cd $$i && echo "cleaning $$i" && $(MAKE) clean) || exit 1; \
	done