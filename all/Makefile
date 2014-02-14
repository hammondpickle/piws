OBJS = mlx90614.o tmp102.o tsl2561.o bmp085.o htu21d.o i2c.o

all: piws

piws: piws.C ${OBJS}
	g++ piws.C ${OBJS} -lbcm2835 -o piws


clean:
	rm -f ${OBJS} piws
