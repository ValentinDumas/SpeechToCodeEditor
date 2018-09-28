#include <iostream>
#include <sphinxbase/cmd_ln.h>
#include <pocketsphinx.h>

#define MODEL_DIR "model/"

int main(int argc, char *argv[])
{
	ps_decoder_t *ps = NULL;
	cmd_ln_t *config = NULL;

	std::string filepath = "goforward.raw";
	FILE* file = NULL;

	config = cmd_ln_init(NULL, ps_args(), TRUE,
		"-hmm", "model/en-us/en-us",
		"-lm", "model/en-us/en-us.lm.bin",
		"-dict", "model/en-us/cmudict-en-us.dict",
		NULL);

	ps = ps_init(config);

	fopen_s(&file, filepath.c_str(), "rb");

	int rv = ps_start_utt(ps);

	int16 buf[512];
	while(!feof(file))
	{
		size_t nsamp;
		nsamp = fread(buf, 2, 512, file);
		ps_process_raw(ps, buf, nsamp, FALSE, FALSE);
	}

	rv = ps_end_utt(ps);

	int32 score;
	char const *hypothesis = ps_get_hyp(ps, &score);
	printf("Recognized: %s\n", hypothesis);

	// Cleanup
	ps_free(ps); // Free decoder
	cmd_ln_free_r(config); // Free configuration

	int a; std::cin >> a;

	return 0;
}