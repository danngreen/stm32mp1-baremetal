#include "plugin_api/common.hh"
#include "plugin_api/processor.hh"

struct ProcB1 : ProcessorBase {
	ProcB1()
	{
		data[0] = 101.1f;
		data[1] = 102.2f;
		data[2] = 103.3f;
		data[3] = 104.4f;
	}

	float get_val(unsigned chan) override { return data[chan]; }

	void set_val(unsigned chan, float val) override { data[chan] = val; }

	void update() override
	{
		data[2] = perform_calculationB(data[3]);
		data5--;
	}

	unsigned data5 = 99999;
};

struct ProcB2 : ProcessorBase {
	ProcB2()
	{
		data[0] = 201.1f;
		data[1] = 202.2f;
		data[2] = 203.3f;
		data[3] = 204.4f;
	}

	float get_val(unsigned chan) override { return data[chan]; }

	void set_val(unsigned chan, float val) override { data[chan] = val; }

	void update() override
	{
		data[2] = perform_calculationB(data[3]);
		data5--;
	}

	unsigned data5 = 99999;
};
