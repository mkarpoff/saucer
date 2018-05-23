CC:=clang
CFLAGS=-D_GNU_SOURCE
LFLAGS=-lpthread -lncurses

all: allm1 modules allm2 bin/saucerac bin/saucernoac bin/sauceranoc \
		bin/saucernoanoc saucer scores.sinv
	@echo "\nBuild Complete"
allm1:
	@echo "\nBuilding with flags [$(CFLAGS) $(LFLAGS)]..."
	@mkdir -p bin
	@mkdir -p bin

allm2:
	@echo "\nChecking saucer modules..."

new: rclean all

modules: modulesm bin/settings.bin bin/loadscr.bin bin/ship.bin bin/rocket.bin\
		bin/erocket.bin

modulesm:
	@echo "\nChecking support modules..."

bin/settings.bin: src/settings.c  src/settings.h
	@echo -n "Compiling [settings]..."
	@$(CC) $(CFLAGS) -o bin/settings.bin -c  src/settings.c
	@echo "Done"
bin/loadscr.bin:  src/loadscr.c  src/loadscr.h bin/settings.bin
	@echo -n "Compiling [loadscr]..."
	@$(CC) $(CFLAGS) -o bin/loadscr.bin -c  src/loadscr.c
	@echo "Done"
bin/ship.bin:  src/ship.c  src/ship.h bin/settings.bin
	@echo -n "Compiling [ship]..."
	@$(CC) $(CFLAGS) -o bin/ship.bin -c  src/ship.c
	@echo "Done"

bin/rocket.bin:  src/rocket.c  src/rocket.h bin/settings.bin
	@echo -n "Compiling [rocket]..."
	@$(CC) $(CFLAGS) -o bin/rocket.bin -c src/rocket.c
	@echo "Done"

bin/erocket.bin:  src/erocket.c  src/erocket.h bin/settings.bin bin/rocket.bin
	@echo -n "Compiling [erocket]..."
	@$(CC) $(CFLAGS) -o bin/erocket.bin -c src/erocket.c
	@echo "Done"

bin/saucerac: bin/settings.bin bin/loadscr.bin bin/ship.bin bin/rocket.bin \
		src/saucer.c  src/saucer.h bin/erocket.bin
	@echo -n "Compiling Colour:yes Arrows:yes..."
	@$(CC) $(CFLAGS) \
		-o bin/saucerac  src/saucer.c\
		bin/loadscr.bin bin/ship.bin bin/rocket.bin bin/settings.bin\
		bin/erocket.bin -DARROWS -DCOLOUR $(LFLAGS)
	@echo "Done"

bin/saucernoac: bin/settings.bin bin/loadscr.bin bin/ship.bin bin/rocket.bin\
		src/saucer.c  src/saucer.h bin/erocket.bin
	@echo -n "Compiling Colour:yes Arrows:no..."
	@$(CC) $(CFLAGS) \
		-o bin/saucernoac  src/saucer.c\
		bin/loadscr.bin bin/ship.bin bin/rocket.bin bin/settings.bin\
		bin/erocket.bin -DCOLOUR $(LFLAGS)
	@echo "Done"

bin/sauceranoc: bin/settings.bin bin/loadscr.bin bin/ship.bin bin/rocket.bin\
		src/saucer.c  src/saucer.h bin/erocket.bin
	@echo -n "Compiling Colour:no Arrows:yes..."
	@$(CC) $(CFLAGS) \
		-o bin/sauceranoc  src/saucer.c\
		bin/loadscr.bin bin/ship.bin bin/rocket.bin bin/settings.bin\
		bin/erocket.bin -DARROWS $(LFLAGS)
	@echo "Done"

bin/saucernoanoc: bin/settings.bin bin/loadscr.bin bin/ship.bin bin/rocket.bin\
		src/saucer.c  src/saucer.h bin/erocket.bin
	@echo -n "Compiling Colour:no Arrows:no..."
	@$(CC) $(CFLAGS) \
		-o bin/saucernoanoc  src/saucer.c\
		bin/loadscr.bin bin/ship.bin bin/rocket.bin bin/settings.bin\
		bin/erocket.bin $(LFLAGS)
	@echo "Done"

saucer: bin/saucerac bin/saucernoac bin/sauceranoc bin/saucernoanoc \
		 src/launcher.c  src/launcher.h
	@echo -n "Compiling launcher..."
	@$(CC) $(CFLAGS) -o saucer  src/launcher.c $(LFLAGS)
	@echo "Done"

scores.sinv:
	@echo -n "\nMaking save data..."

	@echo "1||Someone of godlike skills||30000" >> scores.sinv
	@echo "2||I got close to the sun an I burned||15000" >> scores.sinv
	@echo "3||Pretty impressive person||10000" >> scores.sinv
	@echo "4||Here is where the fun starts||7500" >> scores.sinv
	@echo "5||Afroholic||5000" >> scores.sinv
	@echo "6||Not bad||2500" >> scores.sinv
	@echo "7||I remeber my first time||1000" >> scores.sinv
	@echo "8||Baby Steps||500" >> scores.sinv
	@echo "9||I got one||100" >> scores.sinv
	@echo "10||I don't want anyone to know my name||10" >> scores.sinv
	@echo "Done"


clean:
	@echo "\nCleaning..."
	@rm -vf bin/* saucer
	@rm -vrf bin bin
	@echo "Done"

rclean: clean
	@echo -n "Removing game save..."
	@rm -f saucer scores.sinv log
	@echo "Done"

