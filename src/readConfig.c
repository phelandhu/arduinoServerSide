#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "readConfig.h"


/*
 * initialize data to default values
 */
void init_parameters (struct sample_parameters * parms)
{
	strncpy (parms->port, "comm1", MAXLEN);
	strncpy (parms->speed, "9600", MAXLEN);
	strncpy (parms->stop, "8 N 1", MAXLEN);
}

/*
 * trim: get rid of trailing and leading whitespace...
 *       ...including the annoying "\n" from fgets()
 */
char * trim (char * s)
{
	/* Initialize start, end pointers */
	char *s1 = s, *s2 = &s[strlen (s) - 1];

	/* Trim and delimit right side */
	while ( (isspace (*s2)) && (s2 >= s1) )
		s2--;
	*(s2+1) = '\0';

	/* Trim left side */
	while ( (isspace (*s1)) && (s1 < s2) )
		s1++;

	/* Copy finished string */
	strcpy (s, s1);
	return s;
}

/*
 * parse external parameters file
 *
 * NOTES:
 * - There are millions of ways to do this, depending on your
 *   specific needs.
 *
 * - In general:
 *   a) The client will know which parameters it's expecting
 *      (hence the "struct", with a specific set of parameters).
 *   b) The client should NOT know any specifics about the
 *      configuration file itself (for example, the client
 *      shouldn't know or care about it's name, its location,
 *      its format ... or whether or not the "configuration
 *      file" is even a file ... or a database ... or something
 *      else entirely).
 *   c) The client should initialize the parameters to reasonable
 *      defaults
 *   d) The client is responsible for validating whether the
 *      pararmeters are complete, or correct.
 */
void parse_config (struct sample_parameters * parms)
{
	char *s, buff[256];
	FILE *fp = fopen (CONFIG_FILE, "r");
	if (fp == NULL)
	{
	return;
	}

	/* Read next line */
	while ((s = fgets (buff, sizeof buff, fp)) != NULL)
	{
		/* Skip blank lines and comments */
		if (buff[0] == '\n' || buff[0] == '#')
			continue;

		/* Parse name/value pair from line */
		char name[MAXLEN], value[MAXLEN];
		s = strtok (buff, "=");
		if (s==NULL)
			continue;
		else
			strncpy (name, s, MAXLEN);
		s = strtok (NULL, "=");
		if (s==NULL)
			continue;
		else
			strncpy (value, s, MAXLEN);
		trim (value);

		/* Copy into correct entry in parameters struct */
		if (strcmp(name, "port")==0)
			strncpy (parms->port, value, MAXLEN);
		else if (strcmp(name, "speed")==0)
			strncpy (parms->speed, value, MAXLEN);
		else if (strcmp(name, "stop")==0)
			strncpy (parms->stop, value, MAXLEN);
		else
			printf ("WARNING: %s/%s: Unknown name/value pair!\n",name, value);
	}

	/* Close file */
	fclose (fp);
}
