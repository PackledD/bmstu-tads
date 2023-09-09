#include <string.h>
#include <stdlib.h>
#include "books.h"
#include "exit_codes.h"


int get_enum_tech(char *str)
{
	if (strcmp(str, "domestic") == 0)
		return (int)DOMESTIC;
	else if (strcmp(str, "transfered") == 0)
		return (int)TRANSFERED;
	return -1;
}


int get_enum_art(char *str)
{
	if (strcmp(str, "novel") == 0)
		return (int)NOVEL;
	else if (strcmp(str, "play") == 0)
		return (int)PLAY;
	else if (strcmp(str, "poetry") == 0)
		return (int)POETRY;
	return -1;
}


int get_enum_child(char *str)
{
	if (strcmp(str, "fairy") == 0)
		return (int)FAIRY;
	else if (strcmp(str, "poetry") == 0)
		return (int)CPOETRY;
	return -1;
}


int enum_tech_to_str(enum tech src, char *dst)
{
	*dst = 0;
	int rc = ERR_BAD_ENUM;
	if (src == DOMESTIC)
	{
		strcpy(dst, "domestic");
		rc = EXIT_SUCCESS;
	}
	else if (src == TRANSFERED)
	{
		strcpy(dst, "transfered");
		rc = EXIT_SUCCESS;
	}
	return rc;
}


int enum_art_to_str(enum art src, char *dst)
{
	*dst = 0;
	int rc = ERR_BAD_ENUM;
	if (src == NOVEL)
	{
		strcpy(dst, "novel");
		rc = EXIT_SUCCESS;
	}
	else if (src == PLAY)
	{
		strcpy(dst, "play");
		rc = EXIT_SUCCESS;
	}
	else if (src == POETRY)
	{
		strcpy(dst, "poetry");
		rc = EXIT_SUCCESS;
	}
	return rc;
}


int enum_child_to_str(enum child src, char *dst)
{
	*dst = 0;
	int rc = ERR_BAD_ENUM;
	if (src == FAIRY)
	{
		strcpy(dst, "fairy");
		rc = EXIT_SUCCESS;
	}
	else if (src == CPOETRY)
	{
		strcpy(dst, "poetry");
		rc = EXIT_SUCCESS;
	}
	return rc;
}
