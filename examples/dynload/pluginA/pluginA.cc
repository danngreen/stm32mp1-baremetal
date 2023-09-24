#include "plugin_api/proc_create.hh"
#include "plugin_api/processor.hh"

struct ProcA1 : ProcessorBase {
	ProcA1()
	{
		data[0] = 1.1f;
		data[1] = 2.2f;
		data[2] = 3.3f;
		data[3] = 4.4f;
	}

	float get_val(unsigned chan) override { return data[chan]; }

	void set_val(unsigned chan, float val) override { data[chan] = val; }

	void update() override
	{
		// data[2] = perform_calculationA(data[3]);
		data5[0]++;
	}

	float data5[4]{};
};

struct ProcA2 : ProcessorBase {
	ProcA2()
	{
		data[0] = 11.1f;
		data[1] = 12.2f;
		data[2] = 13.3f;
		data[3] = 14.4f;
	}

	float get_val(unsigned chan) override { return data[chan]; }

	void set_val(unsigned chan, float val) override { data[chan] = val; }

	void update() override
	{
		// data[3] = perform_calculationA(data[4]);
		data5++;
		data6--;
	}

	unsigned data5 = 0;
	unsigned data6 = 0;
};

ProcessorBase *procA1_instance = create<ProcA1>("A1");
ProcessorBase *procA2_instance = create<ProcA2>("A2");
