#ifndef READCONFIG
	#define READCONFIG 1
	#define MAXLEN 80
	#define CONFIG_FILE "/etc/portListener.conf"

	struct sample_parameters
	{
		char port[MAXLEN];
		char speed[MAXLEN];
		char stop[MAXLEN];
	}	
		sample_parameters;

	void init_parameters (struct sample_parameters * parms);
	char * trim (char * s);
	void parse_config (struct sample_parameters * parms);

#endif
