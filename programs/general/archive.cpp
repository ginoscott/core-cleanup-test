#include "configCosmos.h"
#include "datalib.h"
#include "timelib.h"
#include "agentlib.h"
#include "zlib/zlib.h"
#include <stdio.h>

cosmosstruc* cdata;

int main(int argc, char* argv[])
{
	string node = "hiakasat";
	string agent = "soh";
	string source = "incoming";

	switch (argc)
	{
	case 4:
		source = argv[3];
	case 3:
		agent = argv[2];
	case 2:
		node = argv[1];
		break;
	default:
		printf("Usage: archive_incoming node [agent] [\"incoming\"|\"outgoing\"]\n");
		exit (1);
	}

	if (!(cdata=agent_setup_client(SOCKET_TYPE_UDP, node.c_str(), 1000)))
	{
		printf("Couldn't establish client for node %s\n", node.c_str());
		exit (-1);
	}

	char buffer[8192];
	vector<filestruc> srcfiles;
	data_list_files(cdata[0].node.name, source.c_str(), agent.c_str(), srcfiles);

	for (filestruc srcfile: srcfiles)
	{
		if (srcfile.type != "directory")
		{
			FILE* fin;
			if ((fin = data_open(srcfile.path, (char *)"r")) != NULL)
			{
				gzFile gzin = gzdopen(fileno(fin), "r");
				uint32_t year, jday, isecond;
				sscanf(srcfile.name.c_str(), "%*[A-Z,a-z,0-9]_%4u%3u%5u", &year, &jday, &isecond);
				double utc = cal2mjd2(year, 1, isecond/86400.) + jday;

				// Check for gzip
				uint16_t namelen = srcfile.name.size();
				if (namelen >= 3 && srcfile.name.substr(namelen-3, 3) == ".gz")
				{
					srcfile.name = srcfile.name.substr(0, namelen-3);
				}

				string newpath = data_name_path(node, "data", agent, utc, srcfile.name);
				if (!newpath.empty() && !data_exists(newpath))
				{
					FILE* fout;
					if ((fout = data_open(newpath, (char *)"w")) != NULL)
					{
						size_t tbytes = 0;
						int nbytes;
						do
						{
							nbytes = gzread(gzin, buffer, 8192);
							if (nbytes > 0)
							{
								tbytes += nbytes;
								fwrite(buffer, 1, nbytes, fout);
							}
						} while (nbytes > 0 && !gzeof(gzin));
						fclose(fout);
						gzclose_r(gzin);

						if (tbytes)
						{
							printf("Success: %s: %" PRIu32 "\n", newpath.c_str(), tbytes);
						}
						else
						{
							remove(newpath.c_str());
							printf("Failure: %s\n", newpath.c_str());
						}
					}
				}
			}
		}
	}
}
