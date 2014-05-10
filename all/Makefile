OBJS = mlx90614.o tmp275.o tsl2561.o bmp085.o htu21d.o i2c.o tmp102.o ad1115.o

all: ngtmon # piws

piws: piws.C ${OBJS}
	sudo rm -f piws
	g++ -I/usr/include/mysql -L/usr/local/lib piws.C ${OBJS} -lbcm2835 -lmysqlpp -o piws
	sudo chown root:root piws
	sudo chmod u+s piws

ngtmon: ngtmon.C ${OBJS}
	sudo rm -f ngtmon
	g++ -I/usr/include/mysql -L/usr/local/lib ngtmon.C ${OBJS} -lbcm2835 -lmysqlpp -o ngtmon
	sudo chown root:root ngtmon
	sudo chmod u+s ngtmon

clean:
	rm -f ${OBJS}
	sudo rm -f piws
