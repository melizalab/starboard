DTC= dtc
DTCFLAGS= -@ -O dtb

DTBOS= BBB-StarBoard-00A2.dtbo

all: $(DTBOS)

%.dtbo : %.dts
	$(DTC) $(DTCFLAGS) -o $@ $<

clean:
	rm -f $(DTBOS)
