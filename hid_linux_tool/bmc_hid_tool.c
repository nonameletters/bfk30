/*

  Copyright (c) 2014 Guillaume Duc <guillaume@guiduc.org>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*/

#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>

#include "../report.h"
#include "../fxp.h"

#ifdef SQL
#include <my_global.h>
#include <mysql.h>
MYSQL* con;
#endif

static uint8_t usb_hid_in_report_buf[sizeof(struct usb_hid_in_report_s)];

static struct usb_hid_in_report_s *usb_hid_in_report = (struct usb_hid_in_report_s *) usb_hid_in_report_buf;

int usb_hid_fd;


char mysql_query_buffer[4096];
char buf[32];

void close_all() {
	if(usb_hid_fd) {
		close(usb_hid_fd);
		usb_hid_fd = 0;
	}
#ifdef SQL
	if(con) {
		mysql_close(con);
		con = 0;
	}
#endif
}

static void read_in_report() {
	int res;

	res = read(usb_hid_fd, usb_hid_in_report_buf, sizeof(struct usb_hid_in_report_s));
	if(res < 0) {
		perror ("read");
		close_all();
		exit(EXIT_FAILURE);
	}
}

void usb_hid_init(const char *dev_name) {
	int res;

	//struct hidraw_devinfo info;
	//memset(&info, 0x0, sizeof (info));

	usb_hid_fd = open(dev_name, O_RDWR);

	if(usb_hid_fd < 0) {
		perror ("Unable to open device");
		exit (EXIT_FAILURE);
	}

	//printf("Expecting report size: %ld\n", sizeof(struct usb_hid_in_report_s));

#if 0
	int desc_size = 0;
	// Get Report Descriptor Size
	res = ioctl(usb_hid_fd, HIDIOCGRDESCSIZE, &desc_size);
	if (res < 0) {
		perror("HIDIOCGRDESCSIZE");
	}
	else { 
		printf("Report Descriptor Size: %d, expected %d\n", desc_size, sizeof(struct usb_hid_in_report_s));
	}

	// Get Report Descriptor
	struct hidraw_report_descriptor rpt_desc;
	memset(&rpt_desc, 0x0, sizeof (rpt_desc));
	rpt_desc.size = desc_size;
	res = ioctl(usb_hid_fd, HIDIOCGRDESC, &rpt_desc);
	if (res < 0) {
		perror("HIDIOCGRDESC");
	} else {
		//printf("Report Descriptor:\n");
		//for(i = 0; i < rpt_desc.size; i++)
		//	printf("%02hhx ", rpt_desc.value[i]);
		//puts("\n");
	}
#endif

	char buf[256];
	memset(buf, 0x0, sizeof (buf));
	res = ioctl(usb_hid_fd, HIDIOCGRAWNAME(64), &buf);
	if (res < 0) {
		perror("HIDIOCGRAWNAME");
	} else {
		printf("Name: %s\n",buf);
     
	}
}

int open_mysql_connection() {
#ifdef SQL
	con = mysql_init(NULL);

	if(con == NULL) {
		fprintf(stderr, "%s\n", mysql_error(con));
		close_all();
		exit(1);
	}

	if(mysql_real_connect(con, "localhost", "root", "root", NULL, 0, NULL, 0) == NULL) {
		fprintf(stderr, "%s\n", mysql_error(con));
		close_all();
		exit(2);
	}
#endif
	return(0);
}

int main(int argc, char **argv) {
	int i;
	char* test_id = 0;
	fixed_val_t f_val0;
	fixed_val_t f_val1;
	fixed_val_t f_val2;

	usb_hid_fd = 0;
	if(argc < 2) {
		fprintf(stderr, "Usage: %s /dev/hidrawX\n", argv[0]);
		return EXIT_FAILURE;
	}

	if(argc == 3) {
		test_id = argv[2];
		printf("Assuming second argument to be test id: '%s'\n", test_id);
#ifdef SQL
		printf("MySQL client version: %s\n", mysql_get_client_info());
#endif
	}

	usb_hid_init(argv[1]);

	if(test_id) {
		printf("Open connection\n");
		open_mysql_connection();
	}

	while(1) {
		read_in_report();
		if(test_id==0) {

#if HID_REPORT_TIME
			printf("%5i %7.2f ", usb_hid_in_report->sequence_number, usb_hid_in_report->systime/1000.0);
#else
			printf("%5i ", usb_hid_in_report->sequence_number);
#endif

			for(i = 0; i < HID_REPORT_SENSOR_CHANNELS; i++) {
				printf("\t%2.3f\t%1.4f",
					usb_hid_in_report->report.ina[i].voltage.actual / (double)RPT_V_SCALE,
					usb_hid_in_report->report.ina[i].current.actual / (double)RPT_I_SCALE); 
			}
#if HID_REPORT_VTG
			for(i = 0; i < HID_REPORT_VREG_CHANNELS; i++) {
				l11_to_str(buf,usb_hid_in_report->report.vtg_i[i]);
				printf("\t%s", buf);
			}
#endif
			printf("\n");
		} else {
#if HID_REPORT_VTG && HID_REPORT_STAT
			printf(".");
			ulval2fv(&f_val0, usb_hid_in_report->report.vtg_command[0],-13);
			ulval2fv(&f_val1, usb_hid_in_report->report.vtg_command[1],-13);
			ulval2fv(&f_val2, usb_hid_in_report->report.vtg_command[2],-13);
			snprintf(mysql_query_buffer, 4095,
				"INSERT INTO `baikal_t_test_results`.`results` (`test_id`, `sequence_number`,"
				" `v0_9_current_actual`, `v0_9_current_max`, `v0_9_current_min`, `v0_9_current_avg`,"
				" `v0_9_voltage_actual`, `v0_9_voltage_max`, `v0_9_voltage_min`, `v0_9_voltage_avg`,"
				" `v0_9_power_actual`, `v0_9_power_max`, `v0_9_power_min`, `v0_9_power_avg`,"
				" `v1_5_current_actual`, `v1_5_current_max`, `v1_5_current_min`, `v1_5_current_avg`,"
				" `v1_5_voltage_actual`, `v1_5_voltage_max`, `v1_5_voltage_min`, `v1_5_voltage_avg`,"
				" `v1_5_power_actual`, `v1_5_power_max`, `v1_5_power_min`, `v1_5_power_avg`,"
				" `v1_8_current_actual`, `v1_8_current_max`, `v1_8_current_min`, `v1_8_current_avg`,"
				" `v1_8_voltage_actual`, `v1_8_voltage_max`, `v1_8_voltage_min`, `v1_8_voltage_avg`,"
				" `v1_8_power_actual`, `v1_8_power_max`, `v1_8_power_min`, `v1_8_power_avg`,"
				" `v0_9_voltage_set`, `v1_5_voltage_set`, `v1_8_voltage_set`)"
				" VALUES ('%s', %i,"
				" %d.%06d, %d.%06d, %d.%06d, %d.%06d,"
				" %d.%03d, %d.%03d, %d.%03d, %d.%03d,"
				" %d.%06d, %d.%06d, %d.%06d, %d.%06d,"
				" %d.%06d, %d.%06d, %d.%06d, %d.%06d,"
				" %d.%03d, %d.%03d, %d.%03d, %d.%03d,"
				" %d.%06d, %d.%06d, %d.%06d, %d.%06d,"
				" %d.%06d, %d.%06d, %d.%06d, %d.%06d,"
				" %d.%03d, %d.%03d, %d.%03d, %d.%03d,"
				" %d.%06d, %d.%06d, %d.%06d, %d.%06d,"
				" %d.%d, %d.%d, %d.%d)",
				test_id,
				usb_hid_in_report->sequence_number,
				usb_hid_in_report->report.ina[0].current.actual / 1000000, usb_hid_in_report->report.ina[0].current.actual % 1000000,
				usb_hid_in_report->report.ina[0].current.max / 1000000, usb_hid_in_report->report.ina[0].current.max % 1000000,
				usb_hid_in_report->report.ina[0].current.min / 1000000, usb_hid_in_report->report.ina[0].current.min % 1000000,
				usb_hid_in_report->report.ina[0].current.avg / 1000000, usb_hid_in_report->report.ina[0].current.avg % 1000000,
				usb_hid_in_report->report.ina[0].voltage.actual / 1000, usb_hid_in_report->report.ina[0].voltage.actual % 1000,
				usb_hid_in_report->report.ina[0].voltage.max / 1000, usb_hid_in_report->report.ina[0].voltage.max % 1000,
				usb_hid_in_report->report.ina[0].voltage.min / 1000, usb_hid_in_report->report.ina[0].voltage.min % 1000,
				usb_hid_in_report->report.ina[0].voltage.avg / 1000, usb_hid_in_report->report.ina[0].voltage.avg % 1000,
				usb_hid_in_report->report.ina[0].power.actual / 1000000, usb_hid_in_report->report.ina[0].power.actual % 1000000,
				usb_hid_in_report->report.ina[0].power.max / 1000000, usb_hid_in_report->report.ina[0].power.max % 1000000,
				usb_hid_in_report->report.ina[0].power.min / 1000000, usb_hid_in_report->report.ina[0].power.min % 1000000,
				usb_hid_in_report->report.ina[0].power.avg / 1000000, usb_hid_in_report->report.ina[0].power.avg % 1000000,
				usb_hid_in_report->report.ina[1].current.actual / 1000000, usb_hid_in_report->report.ina[1].current.actual % 1000000,
				usb_hid_in_report->report.ina[1].current.max / 1000000, usb_hid_in_report->report.ina[1].current.max % 1000000,
				usb_hid_in_report->report.ina[1].current.min / 1000000, usb_hid_in_report->report.ina[1].current.min % 1000000,
				usb_hid_in_report->report.ina[1].current.avg / 1000000, usb_hid_in_report->report.ina[1].current.avg % 1000000,
				usb_hid_in_report->report.ina[1].voltage.actual / 1000, usb_hid_in_report->report.ina[1].voltage.actual % 1000,
				usb_hid_in_report->report.ina[1].voltage.max / 1000, usb_hid_in_report->report.ina[1].voltage.max % 1000,
				usb_hid_in_report->report.ina[1].voltage.min / 1000, usb_hid_in_report->report.ina[1].voltage.min % 1000,
				usb_hid_in_report->report.ina[1].voltage.avg / 1000, usb_hid_in_report->report.ina[1].voltage.avg % 1000,
				usb_hid_in_report->report.ina[1].power.actual / 1000000, usb_hid_in_report->report.ina[1].power.actual % 1000000,
				usb_hid_in_report->report.ina[1].power.max / 1000000, usb_hid_in_report->report.ina[1].power.max % 1000000,
				usb_hid_in_report->report.ina[1].power.min / 1000000, usb_hid_in_report->report.ina[1].power.min % 1000000,
				usb_hid_in_report->report.ina[1].power.avg / 1000000, usb_hid_in_report->report.ina[1].power.avg % 1000000,
				usb_hid_in_report->report.ina[2].current.actual / 1000000, usb_hid_in_report->report.ina[2].current.actual % 1000000,
				usb_hid_in_report->report.ina[2].current.max / 1000000, usb_hid_in_report->report.ina[2].current.max % 1000000,
				usb_hid_in_report->report.ina[2].current.min / 1000000, usb_hid_in_report->report.ina[2].current.min % 1000000,
				usb_hid_in_report->report.ina[2].current.avg / 1000000, usb_hid_in_report->report.ina[2].current.avg % 1000000,
				usb_hid_in_report->report.ina[2].voltage.actual / 1000, usb_hid_in_report->report.ina[2].voltage.actual % 1000,
				usb_hid_in_report->report.ina[2].voltage.max / 1000, usb_hid_in_report->report.ina[2].voltage.max % 1000,
				usb_hid_in_report->report.ina[2].voltage.min / 1000, usb_hid_in_report->report.ina[2].voltage.min % 1000,
				usb_hid_in_report->report.ina[2].voltage.avg / 1000, usb_hid_in_report->report.ina[2].voltage.avg % 1000,
				usb_hid_in_report->report.ina[2].power.actual / 1000000, usb_hid_in_report->report.ina[2].power.actual % 1000000,
				usb_hid_in_report->report.ina[2].power.max / 1000000, usb_hid_in_report->report.ina[2].power.max % 1000000,
				usb_hid_in_report->report.ina[2].power.min / 1000000, usb_hid_in_report->report.ina[2].power.min % 1000000,
				usb_hid_in_report->report.ina[2].power.avg / 1000000, usb_hid_in_report->report.ina[2].power.avg % 1000000,
				f_val0.val, f_val0.mval,
				f_val1.val, f_val1.mval,
				f_val2.val, f_val2.mval
			);
#endif
#ifdef SQL
			if (mysql_query(con, mysql_query_buffer)) {
				fprintf(stderr, "%s\n", mysql_error(con));
				break;
			}
#else
			//printf("%s\n", mysql_query_buffer);

#endif
		}

#if 0
		printf("#%i\n", usb_hid_in_report->sequence_number);
		printf("                  actual          max          min      average\r\n");
		for(i = 0; i < 3; i++) {
			printf("Current #%i: %4d.%06dA ", i, usb_hid_in_report->report.ina[i].current.actual / 1000000, usb_hid_in_report->report.ina[i].current.actual % 1000000);
			printf("%4d.%06dA ", usb_hid_in_report->report.ina[i].current.max / 1000000, usb_hid_in_report->report.ina[i].current.max % 1000000);
			printf("%4d.%06dA ", usb_hid_in_report->report.ina[i].current.min / 1000000, usb_hid_in_report->report.ina[i].current.min % 1000000);
			printf("%4d.%06dA\r\n", usb_hid_in_report->report.ina[i].current.avg / 1000000, usb_hid_in_report->report.ina[i].current.avg % 1000000);
			printf("Voltage #%i: %7d.%03dV ", i, usb_hid_in_report->report.ina[i].voltage.actual / 1000, usb_hid_in_report->report.ina[i].voltage.actual % 1000);
			printf("%7d.%03dV ", usb_hid_in_report->report.ina[i].voltage.max / 1000, usb_hid_in_report->report.ina[i].voltage.max % 1000);
			printf("%7d.%03dV ", usb_hid_in_report->report.ina[i].voltage.min / 1000, usb_hid_in_report->report.ina[i].voltage.min % 1000);
			printf("%7d.%03dV\r\n", usb_hid_in_report->report.ina[i].voltage.avg / 1000, usb_hid_in_report->report.ina[i].voltage.avg % 1000);
			printf("Power   #%i: %4d.%06dW ", i, usb_hid_in_report->report.ina[i].power.actual / 1000000, usb_hid_in_report->report.ina[i].power.actual % 1000000);
			printf("%4d.%06dW ", usb_hid_in_report->report.ina[i].power.max / 1000000, usb_hid_in_report->report.ina[i].power.max % 1000000);
			printf("%4d.%06dW ", usb_hid_in_report->report.ina[i].power.min / 1000000, usb_hid_in_report->report.ina[i].power.min % 1000000);
			printf("%4d.%06dW\r\n", usb_hid_in_report->report.ina[i].power.avg / 1000000, usb_hid_in_report->report.ina[i].power.avg % 1000000);
			printf("\r\n");
		}
#endif
	}

	close_all();

	return EXIT_SUCCESS;
}
