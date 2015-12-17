/********************************************************************
* Copyright (C) 2015 by Interstel Technologies, Inc.
*   and Hawaii Space Flight Laboratory.
*
* This file is part of the COSMOS/core that is the central
* module for COSMOS. For more information on COSMOS go to
* <http://cosmos-project.com>
*
* The COSMOS/core software is licenced under the
* GNU Lesser General Public License (LGPL) version 3 licence.
*
* You should have received a copy of the
* GNU Lesser General Public License
* If not, go to <http://www.gnu.org/licenses/>
*
* COSMOS/core is free software: you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* COSMOS/core is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* Refer to the "licences" folder for further information on the
* condititons and terms to use this software.
********************************************************************/

#include "physicslib.h"
#include "math/mathlib.h"
#include "agentlib.h"
#include "jsonlib.h"
#include "datalib.h"
#include <sys/stat.h>
#include <iostream>
#include <iomanip>

gj_handle gjh;
cosmosstruc* cdata;
std::vector<shorteventstruc> eventdict;
std::vector<shorteventstruc> events;
std::string mainjstring;

void endline(){
	std::cout << std::endl;
}

int main(int argc, char* argv[])
{
	std::string node;
	int32_t order = 6;
	int32_t mode = 1; // attitude mode (0 - propagate?, 1-LVLH, ...)
	double mjdnow;
	double mjdstart = -1.;
	double mjdend = -1.;
	double dt = 1.;
	double logperiod = 1.;
	double logstride = 3600./86400.;
	int32_t iretn;
	//	bool master_timer = false;

	switch (argc)
	{
	case 5:
		mjdend = atof(argv[4]);
	case 4:
		logperiod = atof(argv[3]);
	case 3:
		mjdstart = atof(argv[2]);
	case 2:
		node = argv[1];
		break;
	default:
		std::cout << "Usage: fast_propagator nodename [mjdstart|0 [logperiod [mjdend]]]" << std::endl;
		exit (-1);
		break;
	}

	if (!(cdata = agent_setup_client(SOCKET_TYPE_BROADCAST, node.c_str(), 1000)))
	{
		printf("Failed to setup client for node %s: %d\n", node.c_str(), AGENT_ERROR_JSON_CREATE);
		exit (AGENT_ERROR_JSON_CREATE);
	}

	cdata[0].physics.mode = mode;
	json_clone(cdata);

	load_dictionary(eventdict, cdata, (char *)"events.dict");

	// Set initial state
	locstruc iloc;

	pos_clear(iloc);

	struct stat fstat;
	FILE* fdes;
	std::string fname = get_nodedir((node.c_str()));
	fname += "/state.ini";
	if ((iretn=stat(fname.c_str(), &fstat)) == 0 && (fdes=fopen(fname.c_str(),"r")) != NULL)
	{
		char* ibuf = (char *)calloc(1,fstat.st_size+1);
		size_t nbytes = fread(ibuf, 1, fstat.st_size, fdes);
		if (nbytes)
		{
			json_parse(ibuf,cdata);
		}
		free(ibuf);
		loc_update(&cdata[0].node.loc);
		iloc = cdata[0].node.loc;
//		iloc.pos.eci = cdata[0].node.loc.pos.eci;
//		iloc.att.icrf = cdata[0].node.loc.att.icrf;
//		iloc.utc = cdata[0].node.loc.pos.eci.utc;

		//        print_vector("Initial State Vector Position: ", iloc.pos.eci.s.col[0], iloc.pos.eci.s.col[1], iloc.pos.eci.s.col[2], "km");
		//std::cout << "Initial State Vector Pos: [" << iloc.pos.eci.s.col[0] << ", " << iloc.pos.eci.s.col[1] <<  ", " << iloc.pos.eci.s.col[2] << "] km " << std::endl;
		std::cout << "Initial State Vector Vel: [" << iloc.pos.eci.v.col[0] << ", " << iloc.pos.eci.v.col[1] <<  ", " << iloc.pos.eci.v.col[2] << "] km" << std::endl;
		std::cout << "Initial MJD: " << std::setprecision(10) << iloc.utc << std::endl;
	}
	else
	{
		printf("Unable to open state.ini\n");
		exit (-1);
	}

#define POLLBUFSIZE 20000
	std::string pollbuf;
	pollstruc meta;

	iretn = agent_poll(cdata, meta, pollbuf, AGENT_MESSAGE_ALL,1);
	switch (iretn)
	{
	case AGENT_MESSAGE_SOH:
	case AGENT_MESSAGE_BEAT:
		{
			std::string tbuf = json_convert_string(json_extract_namedobject(pollbuf, "agent_name"));
			if (!tbuf.empty() && tbuf == "physics")
			{
				tbuf = json_convert_string(json_extract_namedobject(pollbuf, "node_utcoffset"));
				if (!tbuf.empty())
				{
					cdata[0].node.utcoffset = atof(tbuf.c_str());
					printf("slave utcoffset: %f\n", cdata[0].node.utcoffset);
				}
			}
			else
			{
				if (mjdstart == -1.)
				{
					cdata[0].node.utcoffset = cdata[0].node.loc.utc - currentmjd(0.);
				}
				else if (mjdstart == 0.)
				{
					cdata[0].node.utcoffset = 0.;
				}
				else
				{
					cdata[0].node.utcoffset = mjdstart - currentmjd(0.);
				}
				//printf("master utcoffset: %f\n", cdata[0].node.utcoffset);
				std::cout << "master utcoffset: " << std::setprecision(5) << cdata[0].node.utcoffset << std::endl;
				//				master_timer = true;
			}
			break;
		}
	default:
		if (mjdstart == -1.)
		{
			cdata[0].node.utcoffset = cdata[0].node.loc.utc - currentmjd(0.);
		}
		else if (mjdstart == 0.)
		{
			cdata[0].node.utcoffset = 0.;
		}
		else
		{
			cdata[0].node.utcoffset = mjdstart - currentmjd(0.);
		}
		//printf("master utcoffset: %f\n", cdata[0].node.utcoffset);
		std::cout << "master utcoffset: " << cdata[0].node.utcoffset << std::endl;
		//		master_timer = true;
		break;
	}

	mjdnow =  currentmjd(cdata[0].node.utcoffset);

	if (mjdnow < iloc.utc)
	{
        hardware_init_eci(cdata[0].devspec, iloc);
        gauss_jackson_init_eci(gjh, order ,mode, -dt, iloc.utc,iloc.pos.eci, iloc.att.icrf, cdata->physics, cdata->node.loc);

		//printf("Initialize backwards %f days\n", (cdata[0].node.loc.utc-mjdnow));
		std::cout << "Initialize backwards " << cdata[0].node.loc.utc-mjdnow << "days" << std::endl;

        gauss_jackson_propagate(gjh, cdata->physics, cdata->node.loc, mjdnow);
        simulate_hardware(*cdata, cdata->node.loc);
        iloc.utc = cdata[0].node.loc.utc;
		iloc.pos.eci = cdata[0].node.loc.pos.eci;
		iloc.att.icrf = cdata[0].node.loc.att.icrf;
	}
	
	double step = 8.64 * (mjdnow-iloc.utc);
	if (step > 60.)
	{
		step = 60.;
	}
	if (step < .1)
	{
		step = .1;
	}

	//printf("Initialize forwards %f days, steps of %f\n", (mjdnow-iloc.utc), step);
	std::cout << "Initialize forwards " << (mjdnow-iloc.utc) << " days, steps of " << step << std::endl;

//	gj_kernel gjk = gauss_jackson_kernel(gjh, order,step);
//	gj_instance3d gji = gauss_jackson_instance(&gjk, 3, acceleration);
//	gauss_jackson_preset(&gji);
//	gauss_jackson_extrapolate(&gji, mjdnow);

    hardware_init_eci(cdata[0].devspec, iloc);
    gauss_jackson_init_eci(gjh, order, mode, step, iloc.utc ,iloc.pos.eci, iloc.att.icrf, cdata->physics, cdata->node.loc);
    simulate_hardware(*cdata, cdata->node.loc);
    gauss_jackson_propagate(gjh, cdata->physics, cdata->node.loc, mjdnow);
    simulate_hardware(*cdata, cdata->node.loc);
    pos_clear(iloc);
	iloc.pos.eci = cdata[0].node.loc.pos.eci;
	iloc.att.icrf = cdata[0].node.loc.att.icrf;
	iloc.utc = cdata[0].node.loc.pos.eci.utc;
    hardware_init_eci(cdata[0].devspec, iloc);
    gauss_jackson_init_eci(gjh, order, mode, dt, iloc.utc ,iloc.pos.eci, iloc.att.icrf, cdata->physics, cdata->node.loc);
    simulate_hardware(*cdata, cdata->node.loc);
    mjdnow = currentmjd(cdata[0].node.utcoffset);

	std::vector <gj_handle> tgjh(cdata[0].target.size());
	std::vector <cosmosstruc *> tcdata(cdata[0].target.size());

	for (uint16_t i=0; i<cdata[0].target.size(); ++i)
	{
		tcdata[i] = json_create();
        hardware_init_eci(cdata[0].devspec, cdata[0].target[i].loc);
        gauss_jackson_init_eci(tgjh[i], order, 0, dt, cdata[0].target[i].loc.utc, cdata[0].target[i].loc.pos.eci, cdata[0].target[i].loc.att.icrf, tcdata[i]->physics, tcdata[i]->node.loc);
        simulate_hardware(*cdata, cdata[0].target[i].loc);
    }

	std::string sohstring = json_list_of_soh(cdata);
	agent_set_sohstring(cdata, sohstring.c_str());
	std::vector<jsonentry*> logtable;
	json_table_of_list(logtable, sohstring.c_str(), cdata);

	double logdate = floor(mjdnow/logstride)*logstride;

	while (mjdend < 0. || mjdend-mjdstart > 0)
	{
		mjdnow += logperiod/86400.;
        gauss_jackson_propagate(gjh, cdata->physics, cdata->node.loc, mjdnow);
        simulate_hardware(*cdata, cdata->node.loc);
        if (cdata[0].node.loc.utc > cdata[0].node.utc)
		{
			cdata[0].node.utc = cdata[0].node.loc.utc;
		}

		double dtemp;
		if ((dtemp=floor(mjdnow/logstride)*logstride) > logdate)
		{
			logdate = dtemp;
			log_move(cdata[0].node.name, "soh");
		}

		for (uint16_t i=0; i<cdata[0].target.size(); ++i)
		{
            gauss_jackson_propagate(tgjh[i], tcdata[i]->physics, tcdata[i]->node.loc, mjdnow);
            simulate_hardware(*tcdata[i], tcdata[i]->node.loc);
        }
		update_target(cdata);
		calc_events(eventdict, cdata, events);
		for (uint32_t k=0; k<events.size(); ++k)
		{
			memcpy(&cdata[0].event[0].s,&events[k],sizeof(shorteventstruc));
			strcpy(cdata[0].event[0].l.condition,cdata[0].emap[events[k].handle.hash][events[k].handle.index].text);
			log_write(cdata[0].node.name,DATA_LOG_TYPE_EVENT,logdate, json_of_event(mainjstring, cdata));
		}

		if (cdata[0].node.utc != 0. && sohstring.size())
		{
			log_write(cdata[0].node.name,DATA_LOG_TYPE_SOH, logdate, json_of_table(mainjstring,  logtable, cdata));
		}
		//		agent_post(cdata, AGENT_MESSAGE_SOH,json_of_table(mainjstring,  cdata[0].agent[0].sohtable, cdata));
	}
	agent_shutdown_server(cdata);
}

//void acceleration(double time, double *pos, double *acc, int32_t axes)
//{
//	locstruc loc;
//	physicsstruc physics;

//	loc.pos.eci.utc = time;
//	for (uint16_t i=0; i<axes; ++i)
//	{
//		loc.pos.eci.s.col[i] = pos[i];
//	}
//	++loc.pos.eci.pass;
//	pos_eci(&loc);

//	pos_accel(physics, &loc);

//	for (uint16_t i=0; i<axes; ++i)
//	{
//		acc[i] = loc.pos.eci.a.col[i];
//	}
//}
