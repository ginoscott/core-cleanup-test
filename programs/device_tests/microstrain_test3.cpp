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

// #include <unistd.h>
#include "microstrain_lib.h"
#include "timelib.h"

int main(int argc, char *argv[])
{
int32_t iretn, handle;
rvector vaccel, vrate, mag, bias;
rvector iaccel, irate;
rvector base;
rvector position, velocity, ipos;
rmatrix matrix, imatrix;
double t1, t2, t3, t4;
double sep, lastmjd, mjd, cmjd;
uint32_t count;
avector euler;
double lsec, csec;

iretn = microstrain_connect(argv[1]);

if (iretn < 0)
	{
	printf("Error: microstrain_connect() %d\n",iretn);
	exit (1);
	}

handle = iretn;

microstrain_capture_gyro_bias(handle,10000,&bias);
iretn = microstrain_euler_angles(handle, &euler);
printf(" %f %f %f\n",DEGOF(euler.b),DEGOF(euler.e),DEGOF(euler.h));
ipos = rv_zero();
velocity = rv_zero();
iretn = microstrain_accel_rate_magfield_orientation(handle, &iaccel, &irate, &mag, &imatrix);
base = rv_mmult(matrix,rv_one());
mjd = lastmjd = currentmjd(0.);
lsec = mjd * 86400.;
count = 0;
velocity = position = rv_zero();
while (count < 10000000L)
	{
	cmjd = currentmjd(0.);
	csec = cmjd * 86400.;
	iretn = microstrain_stab_accel_rate_magfield(handle, &vaccel, &vrate, &mag);
//	iretn = microstrain_accel_rate_magfield_orientation(handle, &vaccel, &vrate, &mag, &matrix);
//	imatrix = m_mmult(imatrix,q_quaternion2dcm(q_axis2quaternion(rv_smult((csec-lsec),vrate))));
	velocity = rv_add(velocity,rv_smult((csec-lsec),vaccel));
	position = rv_add(position,rv_smult((csec-lsec),velocity));
//	target = rv_mmult(matrix,rv_one());
//	base = rv_mmult(imatrix,rv_one());
//	sep = DEGOF(sep_rv(base,target));
	sep = 0;
	iretn = microstrain_euler_angles(handle, &euler);
	printf("%10.3f %+-10.6g %+-10.6g %+-10.6g %+-10.6g %+-10.6e %+-10.6e %+-10.6e\r",86400.*(lastmjd-currentmjd(0.)),DEGOF(euler.b),DEGOF(euler.e),DEGOF(euler.h),sep,position.col[0],position.col[1],position.col[2]);
	lastmjd = cmjd;
	lsec = csec;
	fflush(stdout);
//	COSMOS_SLEEP(1);
	}
iretn = microstrain_temperature(handle,&t1,&t2,&t3,&t4);
printf("\nTemperatures: %f %f %f %f\n",t1,t2,t3,t4);
iretn = microstrain_disconnect(handle);
}
