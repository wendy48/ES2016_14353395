CXX = g++
CC = g++

PREPROC_MACROS = -D__DOL_ETHZ_GEN__  -DINCLUDE_PROFILER #-DINCLUDE_PERFORMANCE #-DINCLUDE_TRACE

SYSTEMC_INC = -I/home/zhang/systemc-2.3.1/include
SYSTEMC_LIB = /home/zhang/systemc-2.3.1/lib-linux/libsystemc.a
MY_LIB_INC = -Ilib -Isc_wrappers -Iprocesses
VPATH = lib:sc_wrappers:processes

CXXFLAGS = -g -O0 -Wall $(PREPROC_MACROS) $(SYSTEMC_INC) $(MY_LIB_INC)
CFLAGS = $(CXXFLAGS)

PROCESS_OBJS = dolSupport.o ProcessWrapper.o Fifo.o WindowedFifo.o generator_wrapper.o consumer_wrapper.o square_wrapper.o #xmlParser.o Performance_Extraction.o functional_trace.o

all:sc_application

sc_application: sc_application.o $(PROCESS_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SYSTEMC_LIB) # -lpthread -lX11 -lrt
clean:
	-rm -f *.o core core.* *.core *.tga static_characterization.xml sc_application
